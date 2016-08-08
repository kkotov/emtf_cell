// $Id: version.h,v 1.4 2009/05/04 09:54:13 cschwick Exp $
#ifndef _hal_generic_version_h_
#define _hal_generic_version_h_

#include "config/PackageInfo.h"
// !!! Edit this line to reflect the latest package version !!!

#define GENERICHAL_VERSION_MAJOR 4
#define GENERICHAL_VERSION_MINOR 2
#define GENERICHAL_VERSION_PATCH 2

// If any previous versions available E.g. #define GENERICHAL_PREVIOUS_VERSIONS "3.8.0,3.8.1"
#undef GENERICHAL_PREVIOUS_VERSIONS 


//
// Template macros
//
#define GENERICHAL_VERSION_CODE PACKAGE_VERSION_CODE(GENERICHAL_VERSION_MAJOR,GENERICHAL_VERSION_MINOR,GENERICHAL_VERSION_PATCH)
#ifndef GENERICHAL_PREVIOUS_VERSIONS
#define GENERICHAL_FULL_VERSION_LIST  PACKAGE_VERSION_STRING(GENERICHAL_VERSION_MAJOR,GENERICHAL_VERSION_MINOR,GENERICHAL_VERSION_PATCH)
#else 
#define GENERICHAL_FULL_VERSION_LIST  GENERICHAL_PREVIOUS_VERSIONS "," PACKAGE_VERSION_STRING(GENERICHAL_VERSION_MAJOR,GENERICHAL_VERSION_MINOR,GENERICHAL_VERSION_PATCH)
#endif 
namespace generichal
{
	const std::string package  =  "generichal";
	const std::string versions =  GENERICHAL_FULL_VERSION_LIST;
	const std::string description = "Hardware Access Library";
	const std::string authors = "Christoph Schwick";
	const std::string summary = "Main library for the HAL";
	const std::string link = "http://cmsdoc.cern.ch/~cschwick/hal";
	config::PackageInfo getPackageInfo();
	void checkPackageDependencies() throw (config::PackageInfo::VersionException);
	std::set<std::string, std::less<std::string> > getPackageDependencies();
}

#endif
