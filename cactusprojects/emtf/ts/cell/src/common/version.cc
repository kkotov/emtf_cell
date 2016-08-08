#include "config/version.h"
#include "emtf/ts/cell/version.h"
#include "ts/framework/version.h"
#include "ts/toolbox/version.h"
#include "ts/ajaxell/version.h"
#include "xcept/version.h"
#include "toolbox/version.h"

using namespace std;


GETPACKAGEINFO(emtf)

void checkPackageDependencies() throw (config::PackageInfo::VersionException)
{
    CHECKDEPENDENCY(config);
    CHECKDEPENDENCY(xcept);
    CHECKDEPENDENCY(tstoolbox);
    CHECKDEPENDENCY(tsframework);
    CHECKDEPENDENCY(tsajaxell);
}

set<string, less<string> > getPackageDependencies()
{
    set<string, less<string> > dependencies;
    ADDDEPENDENCY(dependencies,config);
    ADDDEPENDENCY(dependencies,xcept);
    ADDDEPENDENCY(dependencies,tstoolbox);
    ADDDEPENDENCY(dependencies,tsframework);
    ADDDEPENDENCY(dependencies,tsajaxell);

    return dependencies;
}

