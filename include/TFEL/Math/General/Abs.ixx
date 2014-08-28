/*!
 * \file   include/TFEL/Math/General/Abs.ixx
 * \brief  This file declares the abs function
 * \author Helfer Thomas
 * \date   13 Feb 2007
 */

#ifndef _LIB_TFEL_MATH_ABS_IXX_
#define _LIB_TFEL_MATH_ABS_IXX_ 

#include<cmath>
#include<functional>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/TypeTraits/IsFundamentalNumericType.hxx"

namespace tfel{

  namespace math{

    template<typename T>
    AbsSum<T>::AbsSum(const T& v)
      : result(v)
    {}
    
    template<typename T>
    void
    AbsSum<T>::operator()(const T& v)
    {
      result += abs(v);
    } // end of AbsSum::operator

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_ABS_IXX */

