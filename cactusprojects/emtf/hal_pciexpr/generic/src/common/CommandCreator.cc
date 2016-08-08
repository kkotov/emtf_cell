#include "hal/CommandCreator.hh"

HAL::CommandCreator::CommandCreator() {
  commandFactoryMap["write"] = new HAL::WriteFactory();
  commandFactoryMap["unmaskedWrite"] = new HAL::UnmaskedWriteFactory();
  commandFactoryMap["setBit"] = new HAL::SetBitFactory();
  commandFactoryMap["resetBit"] = new HAL::ResetBitFactory();
  commandFactoryMap["define"] = new HAL::DefineFactory();
  commandFactoryMap["add"] = new HAL::AddFactory();
  commandFactoryMap["read"] = new HAL::ReadFactory();
  commandFactoryMap["unmaskedRead"] = new HAL::UnmaskedReadFactory();
  commandFactoryMap["label"] = new HAL::LabelFactory();
  commandFactoryMap["goto"] = new HAL::GotoFactory();
  commandFactoryMap["print"] = new HAL::PrintFactory();
  commandFactoryMap["check"] = new HAL::CheckFactory();
  commandFactoryMap["pollItem"] = new HAL::PollItemFactory();
}

HAL::CommandCreator::~CommandCreator() {
  std::tr1::unordered_map<std::string, HAL::AbstractCommandFactory*>::iterator it;
  for ( it = commandFactoryMap.begin(); it != commandFactoryMap.end(); it++ ) {
    delete((*it).second);
  }
}

HAL::SequencerCommand* HAL::CommandCreator::create( std::list<std::string> commandStringList,
						    const HAL::AddressTableInterface& addressTable,
						    HAL::CommandSequence& sequence) 
  throw (HAL::SequencerSyntaxError, HAL::NoSuchItemException) {

  std::string commandString = commandStringList.front();
  commandStringList.pop_front();
  if ( commandFactoryMap.find(commandString) == commandFactoryMap.end() ) {
    std::string text;
    text = "No such sequencer command : \"" + commandString + "\"";
    throw ( HAL::SequencerSyntaxError(text) );
  }
  return (commandFactoryMap[commandString]->create(commandStringList,
                                                   addressTable,
                                                   sequence));
}


