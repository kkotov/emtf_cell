// $Id: version.cc,v 1.1 2007/03/26 14:51:33 cschwick Exp $
#include "halTriggerExample/version.h"
#include "config/version.h"
#include "xgi/version.h"
#include "xdata/version.h"
#include "xdaq/version.h"
#include "xcept/version.h"
#include "toolbox/version.h"

GETPACKAGEINFO(Trigger)

void Trigger::checkPackageDependencies() throw (config::PackageInfo::VersionException)
{
	CHECKDEPENDENCY(config);  
	CHECKDEPENDENCY(xcept);  
	CHECKDEPENDENCY(xgi);  
	CHECKDEPENDENCY(toolbox); 
	CHECKDEPENDENCY(xdata);  
	CHECKDEPENDENCY(xdaq);  
}

std::set<std::string, std::less<std::string> > Trigger::getPackageDependencies()
{
	std::set<std::string, std::less<std::string> > dependencies;

	ADDDEPENDENCY(dependencies,config); 
	ADDDEPENDENCY(dependencies,xcept);
	ADDDEPENDENCY(dependencies,xgi);
	ADDDEPENDENCY(dependencies,toolbox);
	ADDDEPENDENCY(dependencies,xdata);
	ADDDEPENDENCY(dependencies,xdaq);
	 
	return dependencies;
}	
	
