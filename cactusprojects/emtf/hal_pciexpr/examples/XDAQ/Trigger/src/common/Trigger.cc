#include "halTriggerExample/Trigger.hh"

XDAQ_INSTANTIATOR();

halTriggerExample::Trigger::Trigger(xdaq::ApplicationStub* stub) :
    xdaq::Application(stub),
    busAdapter_() {

    LOG4CPLUS_DEBUG(this->getApplicationLogger(), "constructing Trigger");
    // default for parameter values
    triggerSource_ = 0; // external line 0 
    randomRate_ = 1;    // 1 Hz
    triggerCount_ = 0;

    // export paramater to run control, direct binding
    // run control can read and write all exported variables
    // directly (no need for get and put from the user).

    // internal random trigger rate in Hz.
    getApplicationInfoSpace()->fireItemAvailable("randomRate", &randomRate_);
    // numeric trigger source ID as in HW
    getApplicationInfoSpace()->fireItemAvailable("triggerSource", &triggerSource_);
    // internal trigger counter in TTCvi card
    getApplicationInfoSpace()->fireItemAvailable("triggerCount", &triggerCount_);
    // the baseAddress of the TTCvi
    getApplicationInfoSpace()->fireItemAvailable("TTCviBaseAddress", &ttcviBaseAddress_ );
    // the path to the file containing the addressTable
    getApplicationInfoSpace()->fireItemAvailable("XMLAddressTablePath", &xmlAddressTablePath_);


    // Register this application to be notified if somebody wants to retrieve
    // one of the following properties. This corresponds to the former 
    // "ParameterGet" mechansim. If somebody wants to retrieve from this
    // application the value of one of these properties, this class is called 
    // back with the "actionPerformed" method which as a parameter has an event 
    // of type "ItemRetrieveEvent". In addition the name of the property is 
    // contained in the event.
    getApplicationInfoSpace()->addItemRetrieveListener ("triggerSource", this);
    getApplicationInfoSpace()->addItemRetrieveListener ("triggerCount", this);
    getApplicationInfoSpace()->addItemRetrieveListener ("randomRate", this);
    
    // The same as above but this time a callback for changing the properties
    // is registered. This corresponds to the former "ParameterSet" method.
    getApplicationInfoSpace()->addItemChangedListener ("triggerSource", this);
    getApplicationInfoSpace()->addItemChangedListener ("triggerCount", this);
    getApplicationInfoSpace()->addItemChangedListener ("randomRate", this);
	
    // Define FSM
    fsm_.addState ('H', "Halted");
    fsm_.addState ('R', "Ready");
    fsm_.addState ('E', "Enabled");
    fsm_.addState ('S', "Suspended");

    fsm_.addStateTransition ('H','R', "Configure", this, &Trigger::ConfigureAction);
    fsm_.addStateTransition ('R','E', "Enable", this, &Trigger::EnableAction);
    fsm_.addStateTransition ('E','H', "Halt", this, &Trigger::HaltAction);
    fsm_.addStateTransition ('R','H', "Halt", this, &Trigger::HaltAction);
    fsm_.addStateTransition ('S','H', "Halt", this, &Trigger::HaltAction);
    fsm_.addStateTransition ('E','S', "Suspend", this, &Trigger::SuspendAction);
    fsm_.addStateTransition ('S','E', "Resume", this, &Trigger::ResumeAction);
    fsm_.setInitialState('H');
    fsm_.reset();

    // Bind SOAP callbacks for control messages
    xoap::bind (this, &Trigger::changeState, "Configure", XDAQ_NS_URI);
    xoap::bind (this, &Trigger::changeState, "Enable", XDAQ_NS_URI);
    xoap::bind (this, &Trigger::changeState, "Suspend", XDAQ_NS_URI);
    xoap::bind (this, &Trigger::changeState, "Resume", XDAQ_NS_URI);
    xoap::bind (this, &Trigger::changeState, "Halt", XDAQ_NS_URI);
	
    xoap::bind (this, &Trigger::userTrigger, "userTrigger", XDAQ_NS_URI);

    TTCviPtr_ = (TTCviCard*) 0;
    addressTablePtr_ = (HAL::VMEAddressTable*) 0;
}

halTriggerExample::Trigger::~Trigger() 
{
    deleteHardware();
}	


/**
 * Callback function for registered xdata properties.
 */
void halTriggerExample::Trigger::actionPerformed( xdata::Event& e )
{

    if ((e.type() == "ItemRetrieveEvent") && TTCviPtr_ ) {

        uint32_t data;
        std::string item = dynamic_cast<xdata::ItemRetrieveEvent&>(e).itemName();
        
        if ( item == "randomRate") {
            TTCviPtr_->read("RandomTriggerRate", &data);
            randomRate_ = data; 
            LOG4CPLUS_INFO(getApplicationLogger(), toolbox::toString("randomRate: %d", data));
            
        } else if ( item == "triggerCount") {
            data =  TTCviPtr_->getTriggerCount();
            triggerCount_ =  data;
            LOG4CPLUS_INFO(getApplicationLogger(), toolbox::toString("triggerCount: %d", data));
            
        } else if ( item == "triggerSource") {
            TTCviPtr_->read("L1ATriggerSelect", &data);
            LOG4CPLUS_INFO(getApplicationLogger(), toolbox::toString("triggerSource: %d", data));
        }
        
    } 

    else if ((e.type() == "ItemChangedEvent") && TTCviPtr_ ) {  
        
        uint32_t data=0;
        std::string item = dynamic_cast<xdata::ItemChangedEvent&>(e).itemName();

        if ( item == "randomRate") {
            data = randomRate_;
            data = TTCviPtr_->setTriggerRate(data);
            randomRate_ = data;
            LOG4CPLUS_INFO(getApplicationLogger(), toolbox::toString("randomRate set to: %d", data));
            
        } else if ( item == "triggerCount") {
            data = triggerCount_;
            TTCviPtr_->setTriggerCount(data);
            LOG4CPLUS_INFO(getApplicationLogger(), toolbox::toString("triggerCount set to: %d", data));
            
        } else if ( item == "triggerSource") {
            data = triggerSource_;
            TTCviPtr_->selectTriggerSource(data); // Attention : > 7 disables the trigger.
            LOG4CPLUS_INFO(getApplicationLogger(), toolbox::toString("triggerSource set to: %d", data));
        }
        
    }
      
}

//
// SOAP Callback trigger state change 
//
xoap::MessageReference halTriggerExample::Trigger::changeState (xoap::MessageReference msg) 
    throw (xoap::exception::Exception)
{
    xoap::SOAPPart part = msg->getSOAPPart();
    xoap::SOAPEnvelope env = part.getEnvelope();
    xoap::SOAPBody body = env.getBody();
    DOMNode* node = body.getDOMNode();
    DOMNodeList* bodyList = node->getChildNodes();
    for (uint32_t i = 0; i < bodyList->getLength(); i++) 
        {
            DOMNode* command = bodyList->item(i);

            if (command->getNodeType() == DOMNode::ELEMENT_NODE)
                {
                    std::string commandName = xoap::XMLCh2String (command->getLocalName());

                    try 
                        {
                            toolbox::Event::Reference e(new toolbox::Event(commandName, this));
                            fsm_.fireEvent(e);
                            // Synchronize Web state machine
                            // wsm_.setInitialState(fsm_.getCurrentState());
                        }
                    catch (toolbox::fsm::exception::Exception & e)
                        {
                            LOG4CPLUS_ERROR(this->getApplicationLogger(),  toolbox::toString("Command not allowed : %s", e.what()));
                            XCEPT_RETHROW(xoap::exception::Exception, "Command not allowed", e);		
                            //XCEPT_RETHROW(xcept::Exception, "Command not allowed", e);
                        }

                    xoap::MessageReference reply = xoap::createMessage();
                    xoap::SOAPEnvelope envelope = reply->getSOAPPart().getEnvelope();
                    xoap::SOAPName responseName = envelope.createName( commandName +"Response", "xdaq", XDAQ_NS_URI);
                    envelope.getBody().addBodyElement ( responseName );
                    return reply;
                }
        }

    LOG4CPLUS_ERROR(this->getApplicationLogger(), "No command found");
    XCEPT_RAISE(xoap::exception::Exception, "No command found");		
}

// manual trigger
xoap::MessageReference halTriggerExample::Trigger::userTrigger(xoap::MessageReference msg)
    throw( xoap::exception::Exception) {

    try{
        if ( fsm_.getCurrentState() == 'S' ) { 
            LOG4CPLUS_DEBUG(this->getApplicationLogger(), "Sending manual trigger through TTCvi.");
            // no need to check SW state, the HW ignores this if not selected
            // (could give a warning, though)
            TTCviPtr_->sendManual();
        } else {
            LOG4CPLUS_ERROR(this->getApplicationLogger(), "NOT IN STATE ENABLED: CANNOT SEND USERTRIGGER");
        }
        // send back a reply:
        xoap::MessageReference reply = xoap::createMessage();
        xoap::SOAPEnvelope envelope = reply->getSOAPPart().getEnvelope();
        xoap::SOAPName responseName = envelope.createName( "userTriggerResponse", "xdaq", XDAQ_NS_URI);
        envelope.getBody().addBodyElement ( responseName );
        return reply;                        
    } catch ( HAL::HardwareAccessException& e) {
        LOG4CPLUS_ERROR(this->getApplicationLogger(),  e.what() );
        // delete everything which was created so far 
        deleteHardware(); 
        XCEPT_RAISE(xcept::Exception, "Configure failed, going to state Halted");
    }
}

// we use selectManual() instead of disableTrigger()
// so that we can send triggers by hand (VME access)
void halTriggerExample::Trigger::ConfigureAction(toolbox::Event::Reference e) throw(xcept::Exception) {
    
    // create the TTCviCard
    try {
        LOG4CPLUS_DEBUG(this->getApplicationLogger(), "Configure: creating TTCviCard object");
        HAL::VMEAddressTableXMLFileReader reader( xmlAddressTablePath_.toString() );
        LOG4CPLUS_DEBUG(this->getApplicationLogger(), "Configure: reader built");
        HAL::VMEAddressTable *addressTablePtr_ = 
            new HAL::VMEAddressTable( "TTCvi address table", reader );
        LOG4CPLUS_DEBUG(this->getApplicationLogger(), "Configure: addressTable built");
                
        TTCviPtr_ = new TTCviCard( *addressTablePtr_, busAdapter_, ttcviBaseAddress_ );
        TTCviPtr_->printAddressTable();
                
        // set up the ttcvi with some default settings:
        LOG4CPLUS_DEBUG(this->getApplicationLogger(), "Configure: setting defaults in TTCviCard");
        TTCviPtr_->reset();
        randomRate_ = (xdata::Integer)TTCviPtr_->setTriggerRate((uint32_t)randomRate_);
        TTCviPtr_->selectManual();
        TTCviPtr_->setTriggerCount(0);
        triggerCount_ = (xdata::Integer)TTCviPtr_->getTriggerCount();
    } catch ( HAL::HardwareAccessException& e) {
        LOG4CPLUS_ERROR(this->getApplicationLogger(),  e.what() );
        // delete everything which was created so far 
        deleteHardware(); 
        XCEPT_RAISE(xcept::Exception, "Configure failed, going to state Halted");
    }
    LOG4CPLUS_DEBUG(this->getApplicationLogger(), "Configure: end");
}

// could add a parameter to select the trigger source
// no manual triggers should be possible while running.
void halTriggerExample::Trigger::EnableAction(toolbox::Event::Reference e) throw(xcept::Exception) {
    try{
        if(triggerSource_.value_ == 5) {
            // random trigger selected, set rate
            randomRate_ = TTCviPtr_->setTriggerRate(randomRate_);
        }
        triggerCount_ = TTCviPtr_->getTriggerCount();
        TTCviPtr_->selectTriggerSource(triggerSource_);
    } catch ( HAL::HardwareAccessException& e) {
        LOG4CPLUS_ERROR(this->getApplicationLogger(),  e.what() );
        // delete everything which was created so far 
        deleteHardware(); 
        XCEPT_RAISE(xcept::Exception, "Configure failed, going to state Halted");
    }
}

// can send manual triggers while stopped
void halTriggerExample::Trigger::SuspendAction(toolbox::Event::Reference e) throw(xcept::Exception) {
    try{ 
	  
        TTCviPtr_->selectManual();
        triggerCount_ = TTCviPtr_->getTriggerCount();
    } catch ( HAL::HardwareAccessException& e) {
        LOG4CPLUS_ERROR(this->getApplicationLogger(),  e.what() );
        // delete everything which was created so far 
        deleteHardware(); 
        XCEPT_RAISE(xcept::Exception, "Configure failed, going to state Halted");
    }
}

void halTriggerExample::Trigger::ResumeAction(toolbox::Event::Reference e)  throw(xcept::Exception){
    try{
        TTCviPtr_->selectTriggerSource(triggerSource_);
    } catch ( HAL::HardwareAccessException& e) {
        LOG4CPLUS_ERROR(this->getApplicationLogger(),  e.what() );
        // delete everything which was created so far 
        deleteHardware(); 
        XCEPT_RAISE(xcept::Exception, "Configure failed, going to state Halted");
    }
}

// No more manual triggers are possible
void halTriggerExample::Trigger::HaltAction(toolbox::Event::Reference e) throw(xcept::Exception) {
    try{
        TTCviPtr_->disableTrigger();
        triggerCount_ = TTCviPtr_->getTriggerCount();
        deleteHardware();
    } catch ( HAL::HardwareAccessException& e) {
        LOG4CPLUS_ERROR(this->getApplicationLogger(),  e.what() );
        // delete everything which was created so far 
        deleteHardware(); 
        XCEPT_RAISE(xcept::Exception, "Configure failed, going to state Halted");
    }
}

// Helper function: clean up the hardware components
void halTriggerExample::Trigger::deleteHardware() {
    delete TTCviPtr_;
    delete addressTablePtr_;
    TTCviPtr_ = (TTCviCard*) 0;
    addressTablePtr_ = (HAL::VMEAddressTable*) 0;
}
