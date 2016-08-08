#include "hal/TestMaster.hh"

HAL::TestMaster::TestMaster( std::ostream& os ) 
  : os_(os) {
  testDone_ = false;
}

void HAL::TestMaster::registerTester(  HAL::TesterInterface& newTester ) {
  testerList_.push_back( &newTester );
}

bool HAL::TestMaster::doTest()
  throw( HardwareAccessException ) {
  testDone_ = true;
  bool allOk = true;
  bool testResult;
  std::list<HAL::TesterInterface*>::const_iterator it;
  for ( it=testerList_.begin(); it != testerList_.end(); it++ ) {
    os_ << "==============================================================================" << std::endl;
    os_ << "             doing test : " << (*it)->getName() << std::endl;
    os_ << "==============================================================================" << std::endl;
    testResult = (*it)->execute();
    os_ << "\nThe test " ;
    testResult ? os_ << "was successfull." : os_ << "failed.";
    os_ << std::endl;
    os_ << "==============================================================================" << std::endl;
    resultMap_[*it] = testResult;
    allOk &= testResult;
  }
  return allOk;
}

void HAL::TestMaster::printShortReport()  {
  if ( ! testDone_ ) {
    os_ << "Tests have not yet been done!" << std::endl;
    return;
  }
  std::list<HAL::TesterInterface*>::iterator it;
  for ( it=testerList_.begin(); it != testerList_.end(); it++ ) {
    os_ << (*it)->getName() << "  :  " << resultMap_[*it] << std::endl;
  }
}

void HAL::TestMaster::printLongReport()  {
  if ( ! testDone_ ) {
    os_ << "Tests have not yet been done!" << std::endl;
    return;
  }
  std::list<HAL::TesterInterface*>::iterator it;
  for ( it=testerList_.begin(); it != testerList_.end(); it++ ) {
    os_ << "==============================================================================" << std::endl;
    os_ << "              Test for : " << (*it)->getName() << std::endl;
    os_ << "Description:" << std::endl;
    os_ << (*it)->getDescription() << std::endl;
    if ( resultMap_[*it] ) {
      os_ << "Result: The test was successfull" << std::endl;
    } else {
      os_ << "Result: The test failed" << std::endl;
    }
  }
  os_ << "==============================================================================\n\n" << std::endl;
}
 
