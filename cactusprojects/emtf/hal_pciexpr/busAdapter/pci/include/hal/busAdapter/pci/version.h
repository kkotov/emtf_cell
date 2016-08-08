// $Id: version.h,v 1.2 2007/06/19 08:26:38 cschwick Exp $
#ifndef _hal_pcibusadapter_version_h_
#define _hal_pcibusadapter_version_h_

#include "config/PackageInfo.h"

#define PCILINUXBUSADAPTER_VERSION_MAJOR 5
#define PCILINUXBUSADAPTER_VERSION_MINOR 1
#define PCILINUXBUSADAPTER_VERSION_PATCH 0

#undef PCILINUXBUSADAPTER_PREVIOUS_VERSIONS 

#define PCILINUXBUSADAPTER_VERSION_CODE PACKAGE_VERSION_CODE(PCILINUXBUSADAPTER_VERSION_MAJOR,PCILINUXBUSADAPTER_VERSION_MINOR,PCILINUXBUSADAPTER_VERSION_PATCH)
#ifndef PCILINUXBUSADAPTER_PREVIOUS_VERSIONS
#define PCILINUXBUSADAPTER_FULL_VERSION_LIST  PACKAGE_VERSION_STRING(PCILINUXBUSADAPTER_VERSION_MAJOR,PCILINUXBUSADAPTER_VERSION_MINOR,PCILINUXBUSADAPTER_VERSION_PATCH)
#else 
#define PCILINUXBUSADAPTER_FULL_VERSION_LIST  PCILINUXBUSADAPTER_PREVIOUS_VERSIONS "," PACKAGE_VERSION_STRING(PCILINUXBUSADAPTER_VERSION_MAJOR,PCILINUXBUSADAPTER_VERSION_MINOR,PCILINUXBUSADAPTER_VERSION_PATCH)
#endif 
namespace pcilinuxbusadapter
{
	const std::string package  =  "pcilinuxbusadapter";
	const std::string versions =  PCILINUXBUSADAPTER_FULL_VERSION_LIST;
	const std::string description = "";
	const std::string authors = "Christoph Schwick";
	const std::string summary = "Adapter class for hal for Cross-PCI";
	const std::string link = "http://cmsdoc.cern.ch/~cschwick/hal";
	config::PackageInfo getPackageInfo();
	void checkPackageDependencies() throw (config::PackageInfo::VersionException);
	std::set<std::string, std::less<std::string> > getPackageDependencies();
}

#endif
