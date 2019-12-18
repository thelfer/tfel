/*!
 * \file   mfront/include/castem.h
 * 
 * \brief local version of the castem.h header file. Compared to the
 * standard castem header file, this file allows us to choose the
 * numerical type at compile time by defining cast_double
 *
 * \author Helfer Thomas
 * \date   31 Jul 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_CASTEM_H
#define LIB_CASTEM_H

#if (defined UNIX32) || ((defined _WIN32) && (!defined _WIN64))
typedef int cast_int;
typedef float cast_float;
typedef long cast_long;
#ifndef cast_double
typedef double cast_double;
#endif
#endif

#if (defined UNIX64) || (defined _WIN64)
#ifdef _WIN64
 typedef long long cast_int;
#else
 typedef long cast_int;
#endif
typedef double cast_float;
typedef long cast_long;
#ifndef cast_double
typedef double cast_double;
#endif
#endif

#endif /* LIB_CASTEM_H */
