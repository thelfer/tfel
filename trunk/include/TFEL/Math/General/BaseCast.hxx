/*!
 * \file   BaseCast.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   17 jan 2007
 */

#ifndef _LIB_TFEL_BASECAST_HXX_
#define _LIB_TFEL_BASECAST_HXX_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/TypeTraits/BaseType.hxx"
#include"TFEL/Math/qt.hxx"

namespace tfel{

  namespace math{

    template<typename T>
    TFEL_MATH_INLINE
    const typename tfel::typetraits::BaseType<T>::type 
    base_cast(const T x)
    {
      return x;
    }

    template<typename T,typename Unit>
    TFEL_MATH_INLINE
    const typename tfel::typetraits::BaseType<qt<T,Unit> >::type 
    base_cast(const qt<T,Unit> x)
    {
      return x.getValue();
    }

  } // end of namespace math

} // end of namespace tfel  

#endif /* _LIB_TFEL_BASECAST_HXX */

