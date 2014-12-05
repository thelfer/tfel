/*! 
 * \file  include/TFEL/Math/power.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 13 janv. 2012
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_POWER_H_
#define _LIB_TFEL_MATH_POWER_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include<type_traits>
#include"TFEL/TypeTraits/IsScalar.hxx"

namespace tfel{

  namespace math{

    namespace stdfunctions{
      
      template<int N,typename T>
      TFEL_MATH_INLINE
      typename std::enable_if<
	tfel::typetraits::IsScalar<T>::cond,
	T>::type
      power(const T);

      template<int N,unsigned int D,
	       typename T>
      TFEL_MATH_INLINE
      typename std::enable_if<
	tfel::typetraits::IsScalar<T>::cond,
	T>::type
      power(const T);

      float
      pow(const float,const float);

      double
      pow(const double,const double);
      
      long double
      pow(const long double,const long double);
      
    } // end of namespace stdfunctions

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/power.ixx"

#endif /* _LIB_TFEL_MATH_POWER_H */
