/*!
 * \file   include/TFEL/Utilities/Info.hxx
 * \brief  This file implements the INFO macro.
 *	
 * \author Helfer Thomas
 * \date   13 Jul 2006
 */

#ifndef _LIB_TFEL_INFO_H_
#define _LIB_TFEL_INFO_H_ 

#ifdef TFEL_UTILITIES_VERBOSE
#include<iostream>
#endif

/*! 
 * \def   TFEL_UTILITIES_INFO
 * \brief write debugging information to std::cerr
 *
 * \note The TFEL_UTILITIES_INFO macro may be enabled only if the
 * TFEL_VERBOSE compilation flag is set.
 *
 * \author Helfer Thomas
 * \date   13 Jul 2006
 */
#ifdef TFEL_UTILITIES_VERBOSE
#define TFEL_UTILITIES_INFO(X) std::cerr << __FILE__ << " [" << __LINE__ << "] : " << X << std::endl; 
#else 
#define TFEL_UTILITIES_INFO(X) 
#endif

#endif /* _LIB_TFEL_INFO_H */

