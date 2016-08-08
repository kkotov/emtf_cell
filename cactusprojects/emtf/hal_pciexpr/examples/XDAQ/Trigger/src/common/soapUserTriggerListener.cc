#include "soapUserTriggerListener.h"


soapUserTriggerListener::soapUserTriggerListener() {

	soapBindMethod(this, &soapUserTriggerListener::UserTrigger, SOAP_USER_TRIGGER);
 
}


