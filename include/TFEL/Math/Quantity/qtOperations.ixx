/*!
 * \file   include/TFEL/Math/Quantity/qtOperations.ixx
 * \brief  This file implements usual operations on quantities.
 * \author Helfer Thomas
 * \date   07 Jun 2006
 */

#ifndef _LIB_TFEL_QTOPERATIONS_I_
#define _LIB_TFEL_QTOPERATIONS_I_ 

#include"TFEL/Math/General/Complex.hxx"

namespace tfel{

  namespace math{

    template<typename unit,typename T1, typename T2>
    TFEL_MATH_INLINE qt<unit,typename tfel::typetraits::Promote<T1,T2>::type>
    operator + (const qt<unit,T1> a,qt<unit,T2> b)
    {
      typedef typename ResultType<qt<unit,T1>,qt<unit,T2>,OpPlus>::type result;
      return static_cast<result>(a.getValue()+b.getValue());
    }

    template<typename unit,typename T1, typename T2>
    TFEL_MATH_INLINE qt<unit,typename tfel::typetraits::Promote<T1,T2>::type>
    operator - (const qt<unit,T1> a, const qt<unit,T2> b)
    {
      typedef typename ResultType<qt<unit,T1>,qt<unit,T2>,OpMinus>::type result;
      return result((a.getValue())-b.getValue());
    }

    template<typename unit,typename T1, typename unit2, typename T2>
    TFEL_MATH_INLINE qt<typename AddUnit_<unit,unit2>::type,typename tfel::typetraits::Promote<T1,T2>::type>
    operator * (const qt<unit,T1> a, const qt<unit2,T2> b)
    {
      typedef typename ResultType<qt<unit,T1>,qt<unit2,T2>,OpMult>::type result;
      return result((a.getValue())*(b.getValue()));
    }

    template<typename unit,typename T1, typename unit2, typename T2>
    TFEL_MATH_INLINE qt<typename SubUnit_<unit,unit2>::type,typename tfel::typetraits::Promote<T1,T2>::type>
    operator / (const qt<unit,T1> a, const qt<unit2,T2> b)
    {
      typedef typename ResultType<qt<unit,T1>,qt<unit2,T2>,OpDiv>::type result;
      return result((a.getValue())/(b.getValue()));
    }

    // Multiplication by a scalar

#define TFEL_MATH_QT_RESULT_TYPE_IMPL(X)                                                    \
    template<typename unit,typename T1>                                                     \
    TFEL_MATH_INLINE qt<unit,typename tfel::typetraits::Promote<T1,X >::type>               \
    operator * (const qt<unit,T1> a, const X b)                                             \
    {                                                                                       \
      typedef qt<unit,typename tfel::typetraits::Promote<T1,X >::type> result;              \
      return result((a.getValue())*b);                                                      \
    }                                                                                       \
                                                                                            \
    template<typename unit,typename T1>                                                     \
    TFEL_MATH_INLINE qt<unit,typename tfel::typetraits::Promote<T1,X >::type>               \
    operator * (const X b, const qt<unit,T1> a)                                             \
    {                                                                                       \
      typedef qt<unit,typename tfel::typetraits::Promote<T1,X >::type> result;              \
      return result(b*(a.getValue()));                                                      \
    }                                                                                       \
                                                                                            \
    template<typename unit,typename T1>                                                     \
    TFEL_MATH_INLINE qt<unit,typename tfel::typetraits::Promote<T1,X >::type>               \
    operator / (const qt<unit,T1> a, const X b)                                             \
    {                                                                                       \
      typedef qt<unit,typename tfel::typetraits::Promote<T1,X >::type> result;              \
      return result((a.getValue())/b);                                                      \
    }                                                                                       \
                                                                                            \
    template<typename T1, typename unit>                                                    \
    TFEL_MATH_INLINE qt<typename SubUnit_<NoUnit,unit>::type,                               \
			typename tfel::typetraits::Promote<T1,X >::type>                    \
    operator / (const X b, const qt<unit,T1> a)                                             \
    {                                                                                       \
      typedef qt<typename SubUnit_<NoUnit,unit>::type,                                      \
	         typename tfel::typetraits::Promote<T1,X >::type> result;                   \
      return result(b/(a.getValue()));                                                      \
    }                                                                                     
    
    TFEL_MATH_QT_RESULT_TYPE_IMPL(unsigned short)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(unsigned int)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(long unsigned int)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(short)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(int)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(long int)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(float)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(double)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(long double)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<unsigned short>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<unsigned int>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<long unsigned int>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<short>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<int>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<long int>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<float>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<double>)
    TFEL_MATH_QT_RESULT_TYPE_IMPL(tfel::math::Complex<long double>)

  } // end of Namesapce math

} // end of namespace tfel

#endif /* _LIB_TFEL_QTOPERATIONS_I_ */

