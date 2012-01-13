/*!
 * \file   Abs.hxx
 * \brief  This file declares the abs function
 * \author Helfer Thomas
 * \date   13 Feb 2007
 */

#ifndef _LIB_TFEL_ABS_H_
#define _LIB_TFEL_ABS_H_ 

#include<cmath>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/TypeTraits/IsFundamentalNumericType.hxx"

namespace tfel{

  namespace math{

    template<typename Scal>
    TFEL_MATH_INLINE
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsFundamentalNumericType<Scal>::cond,
      Scal
      >::type
    abs(const Scal& s)
    {
      return std::abs(s);
    }

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_ABS_H */

