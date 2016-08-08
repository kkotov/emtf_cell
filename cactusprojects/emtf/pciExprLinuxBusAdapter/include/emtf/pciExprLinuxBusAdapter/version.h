#ifndef _emtf_pciExprLinuxBusAdapter_version_h_
#define _emtf_pciExprLinuxBusAdapter_version_h_

#include "config/PackageInfo.h"
// !!! Edit this line to reflect the latest package version !!!

#define emtfPciExprLinuxBusAdapter_VERSION_MAJOR 1
#define emtfPciExprLinuxBusAdapter_VERSION_MINOR 0
#define emtfPciExprLinuxBusAdapter_VERSION_PATCH 0
// If any previous versions available E.g. #define XMASSENSOR_PREVIOUS_VERSIONS "3.8.0,3.8.1"
//#define emtfPciExprLinuxBusAdapter__PREVIOUS_VERSIONS "0.00"


// //
// // Template macros
// //
#define emtfPciExprLinuxBusAdapter_VERSION_CODE PACKAGE_VERSION_CODE(emtfPciExprLinuxBusAdapter_VERSION_MAJOR,emtfPciExprLinuxBusAdapter_VERSION_MINOR,emtfPciExprLinuxBusAdapter_VERSION_PATCH)
#ifndef emtfPciExprLinuxBusAdapter_PREVIOUS_VERSIONS
#define emtfPciExprLinuxBusAdapter_FULL_VERSION_LIST  PACKAGE_VERSION_STRING(emtfPciExprLinuxBusAdapter_VERSION_MAJOR,emtfPciExprLinuxBusAdapter_VERSION_MINOR,emtfPciExprLinuxBusAdapter_VERSION_PATCH)
#else 
#define emtfPciExprLinuxBusAdapter_FULL_VERSION_LIST  emtfPciExprLinuxBusAdapter_PREVIOUS_VERSIONS "," PACKAGE_VERSION_STRING(emtfPciExprLinuxBusAdapter_VERSION_MAJOR,emtfPciExprLinuxBusAdapter_VERSION_MINOR,emtfPciExprLinuxBusAdapter_VERSION_PATCH)
#endif 

namespace emtfPciExprLinuxBusAdapter
{
   const std::string package  = "emtfPciExprLinuxBusAdapter";
   const std::string versions = emtfPciExprLinuxBusAdapter_FULL_VERSION_LIST;
   const std::string description = "";
   const std::string authors = "Pierluigi Bortignon";
   const std::string summary = "Pci express bus adapter for endcap muon track finder";
   const std::string link = "https://twiki.cern.ch/twiki/bin/viewauth/CMS/L1CSCTrackFinder";
   config::PackageInfo getPackageInfo();
   void checkPackageDependencies() throw (config::PackageInfo::VersionException);
   std::set<std::string, std::less<std::string> > getPackageDependencies();
}
#endif

