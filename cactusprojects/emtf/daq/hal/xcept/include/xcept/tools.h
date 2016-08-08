// $Id: tools.h,v 1.1 2005/07/18 11:45:29 cschwick Exp $

/*************************************************************************
 * XDAQ Components for Distributed Data Acquisition                      *
 * Copyright (C) 2000-2004, CERN.			                 *
 * All rights reserved.                                                  *
 * Authors: J. Gutleber and L. Orsini					 *
 *                                                                       *
 * For the licensing terms see LICENSE.		                         *
 * For the list of contributors see CREDITS.   			         *
 *************************************************************************/

#ifndef _xcept_tools_h_
#define _xcept_tools_h_

#include<string>
#include<typeinfo>
#include "xcept/Exception.h"

namespace xcept
{
	//! Undocumented
	std::string demangle (const std::type_info& ti);

	//! Undocumented
	std::string get_filename(const std::string& str);

	/*! Return a string that contains a formatted representation of the exception 
	  * ready for console output. The output also contains the whole exception trace,
	  * if the exception contains other exceptions.
	  */
	std::string stdformat_exception_history (xcept::Exception& e);
	
	/*! Return a string that contains a formatted representation of the exception 
	  * ready for console output. Only the information of the passed exception parameter
	  * is considered for the formatting process. Contained exceptions are not formatted.
	  */
	std::string stdformat_exception (xcept::Exception& e);
	
	/*! Return a string that contains a formatted representation of the exception 
	  * ready for output in a Web browser. The output also contains the whole exception trace,
	  * if the exception contains other exceptions.
	  */
	std::string htmlformat_exception_history (xcept::Exception& e);

}

#endif
