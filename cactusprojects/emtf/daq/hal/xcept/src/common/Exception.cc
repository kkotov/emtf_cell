// $Id: Exception.cc,v 1.1 2005/07/18 11:45:30 cschwick Exp $

/*************************************************************************
 * XDAQ Components for Distributed Data Acquisition                      *
 * Copyright (C) 2000-2004, CERN.			                 *
 * All rights reserved.                                                  *
 * Authors: J. Gutleber and L. Orsini					 *
 *                                                                       *
 * For the licensing terms see LICENSE.		                         *
 * For the list of contributors see CREDITS.   			         *
 *************************************************************************/

#include "xcept/Exception.h"
#include "xcept/tools.h"
#include <stdarg.h>
#include <sstream>

xcept::ExceptionInformation::ExceptionInformation()
{
	/*
	this->name = "xcept::Exception";
	this->message = "";
	this->module = "";
	this->function = "";
	this->line = "";
	*/
}

xcept::ExceptionInformation::ExceptionInformation(const std::string& name,
						const std::string& message,
						const std::string& module,
						const std::string& function,
						int line)
{
	/*
	this->name = name;
	this->message = message;
	this->module = module;
	this->function = function;
	*/

	properties_["identifier"] = name;
	properties_["message"] = message;
	properties_["module"] = module;
	properties_["function"] = function;
		
	std::stringstream ss;
	ss << line;	
	properties_["line"] = ss.str();
	
	// support for error notification schema http://xdaq.web.cern.ch/xdaq/xsd/2005/ErrorNotification-11.xsd
	properties_["qualifiedErrorSchemaURI"] = "http://xdaq.web.cern.ch/xdaq/xsd/2005/QualifiedSoftwareErrorRecord-10.xsd";
	properties_["sessionID"] = "undefined";
	properties_["notifier"] = "undefined";
	properties_["dateTime"] = "undefined";
	properties_["severity"] = "undefined";
	
}

void xcept::ExceptionInformation::setProperty(const std::string& name, const std::string& value)
{
	properties_[name] = value;
}

std::string xcept::ExceptionInformation::getProperty(const std::string& name) 
{
	std::map<std::string, std::string, std::less<std::string> >::iterator i =  properties_.find(name); 
	if ( i != properties_.end() )
		return (*i).second;
	else 
		return "";
}

std::map<std::string, std::string, std::less<std::string> > & xcept::ExceptionInformation::getProperties()
{
	return properties_;
}

xcept::Exception* xcept::Exception::clone()
{
	xcept::Exception* e = new xcept::Exception();
	e->history_ = history_;
	return e;
}

xcept::Exception::Exception() throw ()
{
	ExceptionInformation i;
	history_.push_back(i);
}

xcept::Exception::~Exception() throw() 
{

}


xcept::Exception::Exception( const std::string& name, const std::string& message, const std::string& module, int line, const std::string& function )
{
	ExceptionInformation i(name, message, xcept::get_filename(module), function, line);
	history_.push_back(i);
	what_ = message;
}

xcept::Exception::Exception( const std::string& name, const std::string& message, const std::string& module, int line, const std::string& function, 
				xcept::Exception& previous)
{
	history_ = previous.getHistory();
	// Copy stack
	ExceptionInformation i(name, message, xcept::get_filename(module), function, line);	
	history_.push_back(i);
	what_ = message;
}

std::vector<xcept::ExceptionInformation>& xcept::Exception::getHistory()
{
	return history_;
}

std::string xcept::Exception::message	()
{
	return history_.back().getProperty("message");
}

std::string xcept::Exception::line()
{
	return history_.back().getProperty("line");
}

std::string xcept::Exception::function()
{
	return history_.back().getProperty("function");
}

std::string xcept::Exception::module()
{
	return history_.back().getProperty("module");
}

const char* xcept::Exception::what () const throw()
{
	return what_.c_str();
}

std::string xcept::Exception::name() 
{
	return history_.back().getProperty("identifier");
}

void xcept::Exception::setProperty(const std::string& name, const std::string& value)
{
	history_.back().setProperty(name,value);
}

std::string xcept::Exception::getProperty(const std::string& name)
{
	return history_.back().getProperty(name);
}


// ----------------------------------------
// --- Exception History Implementation ---
// ----------------------------------------

xcept::ExceptionHistory::ExceptionHistory(xcept::Exception& e)
{
	current_ = e.getHistory().rbegin();
	end_ = e.getHistory().rend();
}
	
bool xcept::ExceptionHistory::hasMore()
{
	if (current_ != end_) return true;
	else return false;
}
	
xcept::ExceptionInformation& xcept::ExceptionHistory::getPrevious() throw (xcept::Exception)
{
	if (current_ == end_)
	{
		XCEPT_RAISE (xcept::Exception, "No more information record. End reached.");
	}
	
	xcept::ExceptionInformation& r = *current_;
	current_++;
	return r;
}
