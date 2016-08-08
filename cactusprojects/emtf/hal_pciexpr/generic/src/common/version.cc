// $Id: version.cc,v 1.4 2007/06/11 17:30:50 cschwick Exp $

/*************************************************************************
 * XDAQ Components for Distributed Data Acquisition                      *
 * Copyright (C) 2000-2004, CERN.			                 *
 * All rights reserved.                                                  *
 * Authors: J. Gutleber and L. Orsini					 *
 *                                                                       *
 * For the licensing terms see LICENSE.		                         *
 * For the list of contributors see CREDITS.   			         *
 *************************************************************************/

#include "hal/generic/version.h"
#include "config/version.h"
#include "xcept/version.h"

GETPACKAGEINFO(generichal)

void generichal::checkPackageDependencies() throw (config::PackageInfo::VersionException)
{
	CHECKDEPENDENCY(config);  
	CHECKDEPENDENCY(xcept);  
}

std::set<std::string, std::less<std::string> > generichal::getPackageDependencies()
{
	std::set<std::string, std::less<std::string> > dependencies;

	ADDDEPENDENCY(dependencies,config); 
	ADDDEPENDENCY(dependencies,xcept);
	 
	return dependencies;
}	
	
