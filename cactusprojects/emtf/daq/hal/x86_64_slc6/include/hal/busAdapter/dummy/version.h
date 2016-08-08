// $Id: version.h,v 1.2 2007/06/19 08:26:38 cschwick Exp $
#ifndef _hal_dummybusadapters_version_h_
#define _hal_dummybusadapters_version_h_

#include "config/PackageInfo.h"
// !!! Edit this line to reflect the latest package version !!!

#define DUMMYLINUXBUSADAPTERS_VERSION_MAJOR 4
#define DUMMYLINUXBUSADAPTERS_VERSION_MINOR 1
#define DUMMYLINUXBUSADAPTERS_VERSION_PATCH 1
// If any previous versions available E.g. #define DUMMYLINUXBUSADAPTERS_PREVIOUS_VERSIONS "3.8.0,3.8.1"
#undef DUMMYLINUXBUSADAPTERS_PREVIOUS_VERSIONS 


//
// Template macros
//
#define DUMMYLINUXBUSADAPTERS_VERSION_CODE PACKAGE_VERSION_CODE(DUMMYLINUXBUSADAPTERS_VERSION_MAJOR,DUMMYLINUXBUSADAPTERS_VERSION_MINOR,DUMMYLINUXBUSADAPTERS_VERSION_PATCH)
#ifndef DUMMYLINUXBUSADAPTERS_PREVIOUS_VERSIONS
#define DUMMYLINUXBUSADAPTERS_FULL_VERSION_LIST  PACKAGE_VERSION_STRING(DUMMYLINUXBUSADAPTERS_VERSION_MAJOR,DUMMYLINUXBUSADAPTERS_VERSION_MINOR,DUMMYLINUXBUSADAPTERS_VERSION_PATCH)
#else 
#define DUMMYLINUXBUSADAPTERS_FULL_VERSION_LIST  DUMMYLINUXBUSADAPTERS_PREVIOUS_VERSIONS "," PACKAGE_VERSION_STRING(DUMMYLINUXBUSADAPTERS_VERSION_MAJOR,DUMMYLINUXBUSADAPTERS_VERSION_MINOR,DUMMYLINUXBUSADAPTERS_VERSION_PATCH)
#endif 
namespace dummylinuxbusadapters
{
	const std::string package  =  "dummylinuxbusadapters";
	const std::string versions =  DUMMYLINUXBUSADAPTERS_FULL_VERSION_LIST;
	const std::string description = "Contains pci, vme and vme64x dummy busAdapters.";
	const std::string authors = "Christoph Schwick";
	const std::string summary = "Adapter classes for hal for DUMMY BusAdapters";
	const std::string link = "http://cmsdoc.cern.ch/~cschwick/hal";
	config::PackageInfo getPackageInfo();
	void checkPackageDependencies() throw (config::PackageInfo::VersionException);
	std::set<std::string, std::less<std::string> > getPackageDependencies();
}

#endif
