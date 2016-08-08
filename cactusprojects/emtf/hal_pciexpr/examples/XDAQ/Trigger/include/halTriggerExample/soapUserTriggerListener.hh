#ifndef soapUserTriggerListener_hh
#define soapUserTriggerListener_hh

#include "soapListener.h"
//
//  Codes
//
 
#define   SOAP_USER_TRIGGER      "UserTrigger"

//
//  Listener
//
class soapUserTriggerListener:  public virtual soapListener {
	public:
	
	soapUserTriggerListener();
	
	virtual SOAPMessage UserTrigger (SOAPMessage& soapMsg)    = 0;

};

#endif


