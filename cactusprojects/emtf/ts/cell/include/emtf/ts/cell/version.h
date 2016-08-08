#ifndef __EMTF_TS_CELL_VERSION_H__
#define __EMTF_TS_CELL_VERSION_H__

#include "config/PackageInfo.h"
// !!! Edit this line to reflect the latest package version !!!

#define EMTFTSCELL_VERSION_MAJOR 1
#define EMTFTSCELL_VERSION_MINOR 0
#define EMTFTSCELL_VERSION_PATCH 0
// If any previous versions available E.g. #define XMASSENSOR_PREVIOUS_VERSIONS "3.8.0,3.8.1"
// #define EMTFTSCELL__PREVIOUS_VERSIONS "0.00"


//
// Template macros
//
#define EMTFTSCELL_VERSION_CODE PACKAGE_VERSION_CODE(EMTFTSCELL_VERSION_MAJOR,EMTFTSCELL_VERSION_MINOR,EMTFTSCELL_VERSION_PATCH)
#ifndef EMTFTSCELL_PREVIOUS_VERSIONS
#define EMTFTSCELL_FULL_VERSION_LIST  PACKAGE_VERSION_STRING(EMTFTSCELL_VERSION_MAJOR,EMTFTSCELL_VERSION_MINOR,EMTFTSCELL_VERSION_PATCH)
#else
#define EMTFTSCELL_FULL_VERSION_LIST  EMTFTSCELL_PREVIOUS_VERSIONS "," PACKAGE_VERSION_STRING(EMTFTSCELL_VERSION_MAJOR,EMTFTSCELL_VERSION_MINOR,EMTFTSCELL_VERSION_PATCH)
#endif

namespace emtf {

    const std::string package  = "emtftscell";
    const std::string versions = EMTFTSCELL_FULL_VERSION_LIST;
    const std::string description = "";
    const std::string authors = "Dan Holmes and Dayong Wang";
    const std::string summary = "TS cell for EM Track Finder";
    const std::string link = "http://cms-emtf.cern.ch/";
    config::PackageInfo getPackageInfo();
    void checkPackageDependencies() throw (config::PackageInfo::VersionException);
    std::set<std::string, std::less<std::string> > getPackageDependencies();
} // namespace

#endif /* __EMTF_TS_CELL_VERSION_H__ */

