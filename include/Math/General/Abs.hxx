/*!
 * \file   Abs.hxx
 * \brief  This file declares the abs function
 * \author Helfer Thomas
 * \date   13 Feb 2007
 */

#ifndef _LIB_TFEL_ABS_H_
#define _LIB_TFEL_ABS_H_ 

#include<cmath>

#include"Config/TFELConfig.hxx"
#include"Metaprogramming/EnableIf.hxx"
#include"TypeTraits/IsFundamentalNumericType.hxx"
#include"Math/qt.hxx"
#include"Math/stensor.hxx"

namespace tfel{

  namespace math{

    template<typename Scal>
    TFEL_MATH_INLINE
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsFundamentalNumericType<Scal>::cond,
      const Scal
      >::type
    abs(const Scal& s)
    {
      return std::abs(s);
    }

    template<typename Unit,typename Scal>
    TFEL_MATH_INLINE
    const Scal
    abs(const qt<Unit,Scal>& s)
    {
      return std::abs(s.getValue());
    }

    namespace internals{

      template<unsigned short N>
      struct StensorAbs;

      template<>
      struct StensorAbs<1u>
      {
	template<typename StensorType>
	TFEL_MATH_INLINE
	static const typename tfel::math::StensorTraits<StensorType>::NumType
	exe(const StensorType& s)
	{
	  return tfel::math::abs(s(0))+tfel::math::abs(s(1))+tfel::math::abs(s(2));
	}
      };

      template<>
      struct StensorAbs<2u>
      {
	template<typename StensorType>
	TFEL_MATH_INLINE
	static const typename tfel::math::StensorTraits<StensorType>::NumType
	exe(const StensorType& s)
	{
	  return tfel::math::abs(s(0))+tfel::math::abs(s(1))+tfel::math::abs(s(2))
	    +tfel::math::abs(s(3));
	}
      };

      template<>
      struct StensorAbs<3u>
      {
	template<typename StensorType>
	TFEL_MATH_INLINE
	static const typename StensorTraits<StensorType>::NumType
	exe(const StensorType& s)
	{
	  return tfel::math::abs(s(0))+tfel::math::abs(s(1))+tfel::math::abs(s(2))
	    +tfel::math::abs(s(3))+tfel::math::abs(s(4))+tfel::math::abs(s(5));
	}
      };

    } // end of namespace internals

    template<typename StensorType>
    TFEL_MATH_INLINE
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<StensorType,StensorConcept>::cond,
      const typename StensorTraits<StensorType>::NumType
    >::type
    abs(const StensorType& s)
    {
      return tfel::math::internals::StensorAbs<StensorTraits<StensorType>::dime>::exe(s);
    }

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_ABS_H */

