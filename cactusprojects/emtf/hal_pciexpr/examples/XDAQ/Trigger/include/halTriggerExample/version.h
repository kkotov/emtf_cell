// $Id: version.h,v 1.1 2007/03/26 14:51:33 cschwick Exp $
#ifndef _halextrigger_version_h_
#define _halextrigger_version_h_

#include "config/PackageInfo.h"
// !!! Edit this line to reflect the latest package version !!!

#define TRIGGER_VERSION_MAJOR 1
#define TRIGGER_VERSION_MINOR 0
#define TRIGGER_VERSION_PATCH 0
// If any previous versions available E.g. #define JOBCONTROL_PREVIOUS_VERSIONS "3.8.0,3.8.1"
#undef TRIGGER_PREVIOUS_VERSIONS 


//
// Template macros
//
#define TRIGGER_VERSION_CODE PACKAGE_VERSION_CODE(TRIGGER_VERSION_MAJOR,TRIGGER_VERSION_MINOR,TRIGGER_VERSION_PATCH)
#ifndef TRIGGER_PREVIOUS_VERSIONS
#define TRIGGER_FULL_VERSION_LIST  PACKAGE_VERSION_STRING(TRIGGER_VERSION_MAJOR,TRIGGER_VERSION_MINOR,TRIGGER_VERSION_PATCH)
#else 
#define TRIGGER_FULL_VERSION_LIST  TRIGGER_PREVIOUS_VERSIONS "," PACKAGE_VERSION_STRING(TRIGGER_VERSION_MAJOR,TRIGGER_VERSION_MINOR,TRIGGER_VERSION_PATCH)
#endif 

namespace Trigger
{
	const std::string package  =  "Trigger";
	const std::string versions =  TRIGGER_FULL_VERSION_LIST;
	const std::string description = "";
	const std::string authors = "Christoph Schwick";
	const std::string summary = "Small example to use the HAL in XDAQ";
	const std::string link =    "";
	config::PackageInfo getPackageInfo();
	void checkPackageDependencies() throw (config::PackageInfo::VersionException);
	std::set<std::string, std::less<std::string> > getPackageDependencies();
}

#endif
