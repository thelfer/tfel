/*!
 * \file   promote-header.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 fév 2008
 */

#ifndef _LIB_TFEL_PROMOTE_HEADER_HXX_
#define _LIB_TFEL_PROMOTE_HEADER_HXX_ 

#include<iostream>
#include<cstdlib>
#include<cassert>
#include<limits>
#include<string>

#include"Utilities/Name.hxx"
#include"TypeTraits/Promote.hxx"
#include"Math/General/Complex.hxx"

std::string Test(const float&);

std::string Test(const long double&);

std::string Test(const double&);

std::string Test(const int&);

std::string Test(const long int&);

std::string Test(const long unsigned int&);

std::string Test(const unsigned int&);

std::string Test(const short&);

std::string Test(const unsigned short&);

std::string Test(const tfel::math::Complex<unsigned short>&);

std::string Test(const tfel::math::Complex<short>&);

std::string Test(const tfel::math::Complex<unsigned int>&);

std::string Test(const tfel::math::Complex<int>&);

std::string Test(const tfel::math::Complex<float>&);

std::string Test(const tfel::math::Complex<double>&);

std::string Test(const tfel::math::Complex<long double>&);
  
#endif /* _LIB_TFEL_PROMOTE_HEADER_HXX */
