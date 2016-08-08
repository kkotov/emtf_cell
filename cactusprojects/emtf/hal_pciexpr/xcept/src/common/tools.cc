// $Id: tools.cc,v 1.1 2005/07/18 11:45:30 cschwick Exp $

/*************************************************************************
 * XDAQ Components for Distributed Data Acquisition                      *
 * Copyright (C) 2000-2004, CERN.			                 *
 * All rights reserved.                                                  *
 * Authors: J. Gutleber and L. Orsini					 *
 *                                                                       *
 * For the licensing terms see LICENSE.		                         *
 * For the list of contributors see CREDITS.   			         *
 *************************************************************************/

#include "xcept/tools.h"
#include <string>
#include <cxxabi.h>

std::string xcept::demangle(const std::type_info& ti)
{
	int status;
	char* realname = abi::__cxa_demangle (ti.name(), 0, 0, &status);
	std::string realNameStr = realname;
	free (realname);
	return realNameStr;
}


//
// Step through the __FILE__ string beginning from the end and
// chop off the path in the beginning.
//
std::string xcept::get_filename(const std::string& str)
{
	const char* __pstr = str.c_str();
	const char* __pstrend = &__pstr[strlen(__pstr)];
	while ((*__pstrend != '/') && (__pstrend != __pstr)) __pstrend--;
	if (*__pstrend == '/') ++__pstrend;
	return __pstrend;
}


std::string xcept::stdformat_exception_history (xcept::Exception& e)
{
	std::string s = "Caught exception: ";
	
	std::vector<xcept::ExceptionInformation> & history = e.getHistory();
	std::vector<xcept::ExceptionInformation>::reverse_iterator i = history.rbegin();
	while ( i != history.rend() )
	{
		s += (*i).getProperty("identifier");
		s += " '";
		s += (*i).getProperty("message");
		s += "' raised at ";
		s += (*i).getProperty("function");
		s += "(";
		s += (*i).getProperty("module");
		s += ":";
		s += (*i).getProperty("line");		
		s += ")";
		
		i++;
		
		if (i != history.rend())
		{
			s += ";\n\toriginated by ";
		}
	}

	return s;
}
	
std::string xcept::stdformat_exception (xcept::Exception& e)
{
	std::string s = "Caught exception: ";
	s += e.name();
	s += " '";
	s += e.message();
	s += "' raised at ";
	s += e.function();
	s += "(";
	s += e.module();
	s += ":";
	s += e.line();	
	s += ")";
	return s;
}

std::string xcept::htmlformat_exception_history (xcept::Exception& e)
{
	std::string s = "<font face=\"Arial\" size=\"2\">Caught exception: ";
	
	std::vector<xcept::ExceptionInformation> & history = e.getHistory();
	std::vector<xcept::ExceptionInformation>::reverse_iterator i = history.rbegin();
	while ( i != history.rend() )
	{
		s += "<b>";
		s += (*i).getProperty("identifier");
		s += "</b><br><span style=\"background-color: #CCFFCC\">'";
		s += (*i).getProperty("message");
		s += "'</span><br>raised at <i>";
		s += (*i).getProperty("function");
		s += "(";
		s += (*i).getProperty("module");
		s += ":";
		s += (*i).getProperty("line");		
		s += ")</i><br>";
		
		i++;
		
		if (i != history.rend())
		{
			s += "originated by ";
		}
	}
	
	return s;
}
