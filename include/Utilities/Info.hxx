/*!
 * \file   Info.hxx
 * \brief  This file implements the INFO macro.
 *	
 * \author Helfer Thomas
 * \date   13 Jul 2006
 */

#ifndef _LIB_TFEL_INFO_H_
#define _LIB_TFEL_INFO_H_ 

#ifdef VERBOSE
#include<iostream>
#endif

/*! 
 * \def   INFO
 * \brief write debugging information to std::cerr
 *
 * \note  The INFO macro may be enabled only if the VERBOSE compilation flag is set.
 *
 * \author Helfer Thomas
 * \date   13 Jul 2006k
 */
#ifdef VERBOSE
#define INFO(X) std::cerr << __FILE__ << " [" << __LINE__ << "] : " << X << std::endl; 
#else 
#define INFO(X) 
#endif

#endif /* _LIB_TFEL_INFO_H */

