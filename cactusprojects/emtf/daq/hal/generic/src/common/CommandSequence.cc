#include "hal/CommandSequence.hh"
#include <list>

HAL::CommandSequence::CommandSequence( std::string name,
				       HAL::CommandSequenceReader& reader,
				       const HAL::AddressTableInterface& addressTable)
  throw( HAL::SequencerSyntaxError, HAL::NoSuchItemException )
  : name( name ),
    reader(reader),
    addressTable( addressTable) {
  rescan();
}

void HAL::CommandSequence::rescan() 
  throw ( HAL::NoSuchItemException,
          HAL::SequencerSyntaxError) {
  reader.rescan();
  commandList.clear();
  commandIndex = 0;
  labelMap.clear();
  variableMap.clear();
  std::list<std::string> commandStringList;
  HAL::CommandCreator commandCreator;
  while ( reader.next( commandStringList ) ) {
    commandList.push_back( commandCreator.create(commandStringList,
						 addressTable,
						 *this));
    commandIndex++;
  }
}

HAL::CommandSequence::~CommandSequence() {
  for ( commandIterator = commandList.begin(); 
        commandIterator != commandList.end(); 
        commandIterator++ ) {
    delete(*commandIterator);
  }
}

void HAL::CommandSequence::run( const HAL::HardwareDeviceInterface& device )
  throw( HAL::SequencerSyntaxError,
         HAL::IllegalOperationException,
         HAL::BusAdapterException,
         HAL::MaskBoundaryException,
         HAL::TimeoutException ) {
  for (commandIterator = commandList.begin(); 
       commandIterator != commandList.end();
       commandIterator++) {
    (*commandIterator)->excecute(device);
  } 
}

uint32_t* HAL::CommandSequence::addVariable( std::string name, 
						  uint32_t initValue ) {
  variableMap[ name ] = initValue;
  return &(variableMap[name]);
}

void HAL::CommandSequence::addLabel( std::string labelName ) {
  // this is called during building up the sequence. The end of the sequence
  // contains the pointer to the command which is one before the label. 
  // Therefore the rescan method must increment the iterators of the label
  // Map at the end by one. 
  labelMap[ labelName ] = commandIndex;
}

uint32_t HAL::CommandSequence::getVariable( std::string name ) const
  throw( HAL::SequencerSyntaxError ) {
  existVariable( name );
  return ((*variableMap.find( name )).second);
}

uint32_t* HAL::CommandSequence::getVariablePointer( std::string name )
  throw( HAL::SequencerSyntaxError ) {
  existVariable( name );
  return &((*variableMap.find( name )).second);
}

void HAL::CommandSequence::setVariable( std::string name, 
					uint32_t value ) 
  throw( HAL::SequencerSyntaxError ) {
  existVariable( name );
  variableMap[ name ] = value;
}

void HAL::CommandSequence::gotoLabel( std::string labelName )
  throw ( HAL::SequencerSyntaxError ) {
  existLabel( labelName );
  commandIterator = (commandList.begin() + labelMap[ labelName ]);
}

void HAL::CommandSequence::existVariable( std::string variableName ) const
  throw( HAL::SequencerSyntaxError ) {
  if (variableName[0] != '$') {
	std::string text = "Unvalid Variable name : " + variableName +
	  "\n    Variables must start with a \"$\"." +
	  "\n    (HAL::CommandSequence::existVariable)";
	throw(HAL::SequencerSyntaxError( text ));
  }
  if ( variableMap.find( variableName ) == variableMap.end() ) {
	std::string text = "No such Variable : " + variableName +
	  "\n    (HAL::CommandSequence::existVariable)";
	throw (HAL::SequencerSyntaxError( text ) );
  }
}

void HAL::CommandSequence::existLabel( std::string labelName ) const
  throw ( HAL::SequencerSyntaxError ) {
  if ( labelMap.find( labelName ) == labelMap.end() ) {
	std::string text = "No such Label : " + labelName +
	  "\n    (HAL::CommandSequence::existLabel)";
	throw (HAL::SequencerSyntaxError( text ) );
  }
}

