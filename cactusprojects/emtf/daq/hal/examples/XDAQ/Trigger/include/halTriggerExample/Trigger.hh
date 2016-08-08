#ifndef __Trigger_h__
#define __Trigger_h__

// includes for XDAQ 3
#include "xdata/Integer.h"
#include "xdata/String.h"
#include "xdaq/ApplicationStub.h"
#include "xdaq/ApplicationContext.h"
#include "xdaq/ApplicationGroup.h"
#include "xdaq/Application.h"
#include "toolbox/exception/Handler.h"
#include "toolbox/fsm/FiniteStateMachine.h"
#include "toolbox/Event.h"
#include "xdaq/NamespaceURI.h"

#include "xoap/Method.h"
#include "xdaq/NamespaceURI.h"
#include "xoap/MessageReference.h"
#include "xoap/MessageFactory.h"
#include "xoap/SOAPPart.h"
#include "xoap/SOAPEnvelope.h"
#include "xoap/SOAPBody.h"
#include "xoap/domutils.h"

// includes for the HAL
#include "hal/VMEAddressTable.hh"
#include "hal/VMEAddressTableXMLFileReader.hh"
#include "hal/VMEDummyBusAdapter.hh"

// this class wraps the HAL with some services specific to the TTCvi:
#include "halTriggerExample/TTCviCard.hh"

namespace halTriggerExample {

/**
 *
 *
 *     @short An example XDAQ-application using the HAL.
 *            
 *            Please  note  that   in  this  application  no  exception 
 *            handling is done. If the  HAL does throw an exception the
 *            application  will  crash   ungracefully.  For  a  serious 
 *            application you might want to do some more work.
 *            
 *            This  example is  a XDAQ  application using  the  HAL to 
 *            talk  to a  TTCvi module.  It contains  one  user defined 
 *            SOAP method in order to generate a single software-trigger. 
 *
 *       @see 
 *    @author Christoph Schwick
 * $Revision: 1.1 $
 *     $Date: 2007/03/26 14:51:33 $
 *
 *
 **/
class Trigger : public xdaq::Application, 
                public xdata::ActionListener {

public:

    /**
     * Constructor of the Trigger application.
     */
    Trigger(xdaq::ApplicationStub* stub);

    /**
     * Destructor of the Trigger application.
     */
    virtual ~Trigger();

    /**
     * Single Trigger issued via a soap message.
     */
    xoap::MessageReference userTrigger(xoap::MessageReference msg )
        throw( xoap::exception::Exception);

    /**
     * Implements the state change from Halted to Ready.
     * Called by the final state machine.
     */
    void ConfigureAction(toolbox::Event::Reference e) throw(xcept::Exception);

    /**
     * Implements the state change from Ready to Running.
     * Called by the final state machine.
     */
    void EnableAction(toolbox::Event::Reference e) throw(xcept::Exception);

    /**
     * Implements the state change from Ready to Paused.
     * Called by the final state machine.
     */
    void SuspendAction(toolbox::Event::Reference e) throw(xcept::Exception);

    /**
     * Implements the state change from Paused to Running.
     * Called by the final state machine.
     */
    void ResumeAction(toolbox::Event::Reference e) throw(xcept::Exception);

    /**
     * Implements the state change from any state to Halted.
     * Called by the final state machine.
     */
    void HaltAction(toolbox::Event::Reference e) throw(xcept::Exception);

    /**
     * Implemention of the interface of the ActionListener.
     */
    void actionPerformed (xdata::Event& e);

private:

    /**
     * Message which triggers a state change.
     *
     * Soap message which should change the state of the application are
     * bound to this function. In this function the body of the Message
     * is decoded and an Event containing the state change command, is 
     * constructed. This event is then sent to the Final State Machine. 
     * It is this Statemachine which according to its configuration finds
     * out if the requested state change is legal, and if so, it calls
     * corresponding action-method in this class. If an illegal state 
     * state change has been requested, an exception is thrown.
     *
     */
    xoap::MessageReference changeState (xoap::MessageReference msg) 
        throw (xoap::exception::Exception);

    /**
     * A small helper function to clean up.
     */
    void deleteHardware();

    HAL::VMEDummyBusAdapter busAdapter_;

    /**
     * A pointer to the addressTable. 
     * The AddressTable is created dynamically during the 
     * configuration of the application, since the path to
     * the file containing the table is a property (=parameter) of 
     * the XDAQ application. Therefore the VMEAddressTable cannot
     * be instantiated in the constructor of this class. 
     */
    HAL::VMEAddressTable *addressTablePtr_;

    /**
     * The pointer to the TTCviCard.
     *
     * The BaseAddress to the TTCvi is a paramter of the XDAQ application.
     * Therefore the TTCviCard can only be constructed when the parameters
     * are already loaded into the XDAQ application. Like in the case of the 
     * addressTable, here only the pointer to the card is defined and the 
     * instantiation is defered to the configure routine. 
     */
    TTCviCard *TTCviPtr_;

    // numeric ID of trigger source, as in HW. 
    xdata::Integer triggerSource_;
    // random rate in Hz.
    xdata::Integer randomRate_;
    // internal L1A counter
    xdata::Integer triggerCount_;
    // AddressTable for the TTCvi
    xdata::String xmlAddressTablePath_;
    // baseAddress of the TTCvi module
    xdata::Integer ttcviBaseAddress_;

    // the statemachine of the application:
    toolbox::fsm::FiniteStateMachine fsm_;

};
} /* namespace halTriggerExample */
#endif
