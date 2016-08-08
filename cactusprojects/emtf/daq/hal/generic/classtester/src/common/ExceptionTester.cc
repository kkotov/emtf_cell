#include "ExceptionTester.hh"

string HAL::ExceptionTester::getDescription() const {
  stringstream text;
  text << "Tests the Exception mechanism in the HAL\n"
       << "The HAL::IllegalOperationException is used for this\n"
       << ends;
  return text.str();
}

bool HAL::ExceptionTester::execute() {
  bool result = false;
  try {
    throw ( HAL::IllegalOperationException( "this is the exception message",
				       __FILE__, __LINE__,
				       __FUNCTION__ ) );
  } catch (HAL::HardwareAccessException& e ) {
    cout << e.what() << endl;
    cout << "name     : " << e.name() << endl;
    cout << "message  : " << e.message() << endl;
    cout << "module   : " << e.module() << endl;
    cout << "function : " << e.function() << endl;
    cout << "line     : " << e.line() << endl;
    result = true;
  }
  // all ok if arrived here
  return result;
}
