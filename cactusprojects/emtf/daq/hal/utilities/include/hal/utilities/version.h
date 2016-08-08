// $Id: version.h,v 1.2 2007/06/19 08:26:38 cschwick Exp $
#ifndef _hal_utilities_version_h_
#define _hal_utilities_version_h_

#include "config/PackageInfo.h"
// !!! Edit this line to reflect the latest package version !!!

#define HALUTILITIES_VERSION_MAJOR 4
#define HALUTILITIES_VERSION_MINOR 1
#define HALUTILITIES_VERSION_PATCH 1
// If any previous versions available E.g. #define HALUTILITIES_PREVIOUS_VERSIONS "3.8.0,3.8.1"
#undef HALUTILITIES_PREVIOUS_VERSIONS 


//
// Template macros
//
#define HALUTILITIES_VERSION_CODE PACKAGE_VERSION_CODE(HALUTILITIES_VERSION_MAJOR,HALUTILITIES_VERSION_MINOR,HALUTILITIES_VERSION_PATCH)
#ifndef HALUTILITIES_PREVIOUS_VERSIONS
#define HALUTILITIES_FULL_VERSION_LIST  PACKAGE_VERSION_STRING(HALUTILITIES_VERSION_MAJOR,HALUTILITIES_VERSION_MINOR,HALUTILITIES_VERSION_PATCH)
#else 
#define HALUTILITIES_FULL_VERSION_LIST  HALUTILITIES_PREVIOUS_VERSIONS "," PACKAGE_VERSION_STRING(HALUTILITIES_VERSION_MAJOR,HALUTILITIES_VERSION_MINOR,HALUTILITIES_VERSION_PATCH)
#endif 
namespace halutilities
{
	const std::string package  =  "halutilities";
	const std::string versions =  HALUTILITIES_FULL_VERSION_LIST;
	const std::string description = "Usefull utilities for the HAL.";
	const std::string authors = "Christopoh Schwick";
	const std::string summary = "Utilities for the HAL";
	const std::string link = "http://cmsdoc.cern.ch/~cschwick/hal";
	config::PackageInfo getPackageInfo();
	void checkPackageDependencies() throw (config::PackageInfo::VersionException);
	std::set<std::string, std::less<std::string> > getPackageDependencies();
}

#endif
