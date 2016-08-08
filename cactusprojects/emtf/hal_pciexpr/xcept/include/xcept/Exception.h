// $Id: Exception.h,v 1.1 2005/07/18 11:45:29 cschwick Exp $

/*************************************************************************
 * XDAQ Components for Distributed Data Acquisition                      *
 * Copyright (C) 2000-2004, CERN.			                 *
 * All rights reserved.                                                  *
 * Authors: J. Gutleber and L. Orsini					 *
 *                                                                       *
 * For the licensing terms see LICENSE.		                         *
 * For the list of contributors see CREDITS.   			         *
 *************************************************************************/

#ifndef _xcept_Exception_h_
#define _xcept_Exception_h_

#include <iostream>
#include <string>
#include <stdio.h>
#include <exception>
#include <typeinfo>
#include <vector>
#include <map>


//
//! Macro to throw an excpetion with line number and function name
//
#define XCEPT_RAISE( EXCEPTION, MSG ) \
throw EXCEPTION( #EXCEPTION, MSG, __FILE__, __LINE__, __FUNCTION__)

#define XCEPT_RETHROW( EXCEPTION, MSG, PREVIOUS ) \
throw EXCEPTION( #EXCEPTION, MSG, __FILE__, __LINE__, __FUNCTION__, PREVIOUS)

#define XCEPT_ASSERT(COND, EXCEPTION, MSG) \
if (!(COND)) \
{\
		XCEPT_RAISE(EXCEPTION, MSG);\
}



namespace xcept
{
	/* A record that holds all information of an exception. An exception holds
	    a list of this record, one for each stacked exception. */
	class ExceptionInformation
	{
		public:
			/* Empty constructor */
			ExceptionInformation();
			
			/* Constructor with the minimal required information entries */
			ExceptionInformation(const std::string& name,
						const std::string& message,
						const std::string& module,
						const std::string& function,
						int line);
						
		//! Sets a specific property of the exception information
		void setProperty(const std::string& name, const std::string& value);
		
		//! Retrieve a specific property of the exception information
		std::string getProperty(const std::string& name);
		
		//! Retrieve all properties
		std::map<std::string, std::string, std::less<std::string> > & getProperties();
		
		protected:

		std::map<std::string, std::string, std::less<std::string> > properties_;
		
		// information for other exceptions (for a collection of exceptions)
		// NOT SUPPORTED IN THIS VERSION
		// std::vector<std::stack<xcept::ExceptionInformation> > collection_;			
	};
	


//
//! Exception class. All other exceptions shall
//! inherit from this class. Within the xdaq framework
//! and applications, only xdaq exceptions shall be raised.
//
class Exception : public std::exception
{
	public:
		
		//! Create the exception with no additional information
		Exception() throw ();
		
		//
		//! \param message is the error message 
		//! that this exception shall contain
		//
		Exception( const std::string& name, const std::string& message, const std::string& module, int line, const std::string& function );
		
		//
		//! \param message is the error message 
		//! that this exception shall contain
		//
		Exception( const std::string& name, const std::string& message, const std::string& module, int line, const std::string& function,
				xcept::Exception& previous );

		virtual ~Exception() throw();
	
		//
		//! Retrieve the exception error message
		//
		std::string message ();
		
		//! Retrieve the line in which the exception occurred
		std::string line();
		
		//! Retrieve the function in which the exception occurred
		std::string function();
		
		//! Retrieve the module in which the exception occurred
		std::string module();
		
		//! Retrieve a string representation of the exception description
		const char* what () const throw();
		
		//! Get the name of the exception in a string
		std::string name();
		
		std::vector<xcept::ExceptionInformation>& getHistory();
			
		virtual Exception* clone();
		
		//! Sets a specific property of the exception
		void setProperty(const std::string& name, const std::string& value);
		
		//! Retrieve a specific property of the exception
		std::string getProperty(const std::string& name);
	
	protected:	
		std::vector<xcept::ExceptionInformation> history_;
		std::string what_;
		
};

/*! A class to iterate over the exception history of an xcept::Exception */
class ExceptionHistory
{
	public:
	
	//! Create an iterator over the exception history from an exception
	//
	ExceptionHistory(xcept::Exception& e);
	
	//! Return true if another exception is stacked, false if all records have been read.
	//
	bool hasMore();
	
	/* Retrieve the stacked information history. Throws an exception if an attempt
	    is made to retrieve an exception information althgouth all stacked exception
	    information records have already been read.
	 */
	xcept::ExceptionInformation& getPrevious() throw (xcept::Exception);
	
	protected:
	
	std::vector<xcept::ExceptionInformation>::reverse_iterator current_;
	std::vector<xcept::ExceptionInformation>::reverse_iterator end_;
};

} // end of namespace xcept


#define DEFINE_EXCEPTION(NAMESPACE1, EXCEPTION_NNAME) \
namespace NAMESPACE1 { \
namespace exception { \
class EXCEPTION_NAME: public xcept::Exception \
{\
	public: \
	EXCEPTION_NAME( std::string name, std::string message, std::string module, int line, std::string function ): \
		xcept::Exception(name, message, module, line, function) \
	{} \
}; \
} \
}

#endif
