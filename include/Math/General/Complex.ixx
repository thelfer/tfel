/*!
 * \file   Complex.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   10 déc 2006
 */

#ifndef _LIB_TFEL_COMPLEX_IXX_
#define _LIB_TFEL_COMPLEX_IXX_ 

namespace tfel{

  namespace math{

    template<typename ValueType>
    Complex<ValueType>::Complex(const ValueType& a,const ValueType& b)
      : std::complex<ValueType>(a,b)
    {}

    template<typename ValueType>
    Complex<ValueType>::Complex(const Complex& a)
      : std::complex<ValueType>(a)
    {}

    template<typename ValueType>
    Complex<ValueType>::Complex(const std::complex<ValueType>& a)
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
    TFEL_MATH_INLINE const Complex<ValueType>
    conj(const Complex<ValueType>& z)
    {
      return Complex<ValueType>(z.real(),-z.imag());
    }

    template<typename ValueType>
    TFEL_MATH_INLINE const ValueType
    real(const Complex<ValueType>& z)
    {
      return z.real();
    }

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_COMPLEX_IXX */

