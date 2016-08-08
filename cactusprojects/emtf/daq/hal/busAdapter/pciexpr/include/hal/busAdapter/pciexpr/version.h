// $Id: version.h,v 1.2 2007/06/19 08:26:38 cschwick Exp $
#ifndef _hal_pciexprbusadapter_version_h_
#define _hal_pciexprbusadapter_version_h_

#include "config/PackageInfo.h"

#define PCIEXPRLINUXBUSADAPTER_VERSION_MAJOR 1
#define PCIEXPRLINUXBUSADAPTER_VERSION_MINOR 0
#define PCIEXPRLINUXBUSADAPTER_VERSION_PATCH 0

#undef PCIEXPRLINUXBUSADAPTER_PREVIOUS_VERSIONS 

#define PCIEXPRLINUXBUSADAPTER_VERSION_CODE PACKAGE_VERSION_CODE(PCIEXPRLINUXBUSADAPTER_VERSION_MAJOR,PCIEXPRLINUXBUSADAPTER_VERSION_MINOR,PCIEXPRLINUXBUSADAPTER_VERSION_PATCH)
#ifndef PCIEXPRLINUXBUSADAPTER_PREVIOUS_VERSIONS
#define PCIEXPRLINUXBUSADAPTER_FULL_VERSION_LIST  PACKAGE_VERSION_STRING(PCIEXPRLINUXBUSADAPTER_VERSION_MAJOR,PCIEXPRLINUXBUSADAPTER_VERSION_MINOR,PCIEXPRLINUXBUSADAPTER_VERSION_PATCH)
#else 
#define PCIEXPRLINUXBUSADAPTER_FULL_VERSION_LIST  PCIEXPRLINUXBUSADAPTER_PREVIOUS_VERSIONS "," PACKAGE_VERSION_STRING(PCIEXPRLINUXBUSADAPTER_VERSION_MAJOR,PCIEXPRLINUXBUSADAPTER_VERSION_MINOR,PCIEXPRLINUXBUSADAPTER_VERSION_PATCH)
#endif 
namespace pciexprlinuxbusadapter
{
	const std::string package  =  "pciexprlinuxbusadapter";
	const std::string versions =  PCIEXPRLINUXBUSADAPTER_FULL_VERSION_LIST;
	const std::string description = "";
	const std::string authors = "Pierluigi Bortignon";
	const std::string summary = "Adapter class for hal for Cross-PCIEXPR";
	const std::string link = "http://cmsdoc.cern.ch/~cschwick/hal";
	config::PackageInfo getPackageInfo();
	void checkPackageDependencies() throw (config::PackageInfo::VersionException);
	std::set<std::string, std::less<std::string> > getPackageDependencies();
}

#endif
