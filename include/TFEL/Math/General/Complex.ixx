/*!
 * \file   include/TFEL/Math/General/Complex.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   10 déc 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_COMPLEX_IXX_
#define _LIB_TFEL_MATH_COMPLEX_IXX_ 

#include"TFEL/Math/General/UnaryResultType.hxx"

namespace tfel{

  namespace math{

    template<typename ValueType>
    constexpr Complex<ValueType>::Complex(const ValueType& a,const ValueType& b)
      : std::complex<ValueType>(a,b)
    {}

    template<typename ValueType>
    constexpr Complex<ValueType>::Complex(const Complex& a)
      : std::complex<ValueType>(a)
    {}

    template<typename ValueType>
    constexpr Complex<ValueType>::Complex(const std::complex<ValueType>& a)
      : std::complex<ValueType>(a)
    {}

    template<typename ValueType>
    Complex<ValueType>& 
    Complex<ValueType>::operator=(const Complex<ValueType>& a)
    {
      std::complex<ValueType>::operator=(a);
      return *this;
    }
    
    template<typename ValueType>
    Complex<ValueType>&
    Complex<ValueType>::operator=(const std::complex<ValueType>& a)
    {
      std::complex<ValueType>::operator=(a);
      return *this;
    }

    template<typename ValueType>
    TFEL_MATH_INLINE constexpr Complex<ValueType>
    conj(const Complex<ValueType>& z)
    {
      return Complex<ValueType>(z.real(),-z.imag());
    }

    template<typename ValueType>
    TFEL_MATH_INLINE constexpr ValueType
    real(const Complex<ValueType>& z)
    {
      return z.real();
    }

    template<int N,unsigned int D>
    struct Power;
    
    template<int N,unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<Complex<float>,Power<N,D> >
    {
    public:
      typedef Complex<float> type;
    };

    template<int N,unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<Complex<double>,Power<N,D> >
    {
    public:
      typedef Complex<double> type;
    };

    template<int N,unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<Complex<long double>,Power<N,D> >
    {
    public:
      typedef Complex<long double> type;
    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_COMPLEX_IXX */

