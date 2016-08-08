// $Id: version.h,v 1.3 2008/09/13 09:38:37 cschwick Exp $
#ifndef _hal_caenbusadapter_version_h_
#define _hal_caenbusadapter_version_h_

#include "config/PackageInfo.h"
// !!! Edit this line to reflect the latest package version !!!

#define CAENLINUXBUSADAPTER_VERSION_MAJOR 4
#define CAENLINUXBUSADAPTER_VERSION_MINOR 1
#define CAENLINUXBUSADAPTER_VERSION_PATCH 1
// If any previous versions available E.g. #define CAENLINUXBUSADAPTER_PREVIOUS_VERSIONS "3.8.0,3.8.1"
#undef CAENLINUXBUSADAPTER_PREVIOUS_VERSIONS 


//
// Template macros
//
#define CAENLINUXBUSADAPTER_VERSION_CODE PACKAGE_VERSION_CODE(CAENLINUXBUSADAPTER_VERSION_MAJOR,CAENLINUXBUSADAPTER_VERSION_MINOR,CAENLINUXBUSADAPTER_VERSION_PATCH)
#ifndef CAENLINUXBUSADAPTER_PREVIOUS_VERSIONS
#define CAENLINUXBUSADAPTER_FULL_VERSION_LIST  PACKAGE_VERSION_STRING(CAENLINUXBUSADAPTER_VERSION_MAJOR,CAENLINUXBUSADAPTER_VERSION_MINOR,CAENLINUXBUSADAPTER_VERSION_PATCH)
#else 
#define CAENLINUXBUSADAPTER_FULL_VERSION_LIST  CAENLINUXBUSADAPTER_PREVIOUS_VERSIONS "," PACKAGE_VERSION_STRING(CAENLINUXBUSADAPTER_VERSION_MAJOR,CAENLINUXBUSADAPTER_VERSION_MINOR,CAENLINUXBUSADAPTER_VERSION_PATCH)
#endif 
namespace caenlinuxbusadapter
{
	const std::string package  =  "caenlinuxbusadapter";
	const std::string versions =  CAENLINUXBUSADAPTER_FULL_VERSION_LIST;
	const std::string description = "";
	const std::string authors = "CAEN";
	const std::string summary = "Adapter class for hal for CAEN BusAdapter";
	const std::string link = "http://cmsdoc.cern.ch/~cschwick/hal";
	config::PackageInfo getPackageInfo();
	void checkPackageDependencies() throw (config::PackageInfo::VersionException);
	std::set<std::string, std::less<std::string> > getPackageDependencies();
}

#endif
