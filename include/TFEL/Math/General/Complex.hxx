/*!
 * \file   Complex.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   10 déc 2006
 */

#ifndef _LIB_TFEL_MATH_COMPLEX_HXX_
#define _LIB_TFEL_MATH_COMPLEX_HXX_ 

#include<complex>
#include"TFEL/Config/TFELConfig.hxx"
 
namespace tfel{

  namespace math{

    /*!
     * \class Complex
     * \brief a class to replace std::complex.
     * All standard operations are defined in a more tfelish way for
     * Complex than for std::complex.
     * This is mostly a way to simulate template typedef (not yet in
     * standard C++).
     * \param ValueType, underlying type.
     * \see std::complex.
     */
    template<typename ValueType>
    struct Complex
      : public std::complex<ValueType>
    {
      /*
       * \Default constructor
       * \param const ValueType&, real part value (calls
       * ValueType's default if none given).
       * \param const ValueType&, imaginary part value (calls
       * ValueType's default if none given).
       */
      Complex(const ValueType& = ValueType(), const ValueType & = ValueType());

      /*
       * Copy constructor
       * \param const Complex&, objet to be copied.
       */
      Complex(const Complex&);

      /*
       * Constructor from std::complex, so that both are
       * interchangeables. Conversion from Complex to std::complex 
       * automatic du to inheritance.
       * \param const std::complex&, object to be copied.
       */
      Complex(const std::complex<ValueType>&);

      /*
       * Assignement operator.
       * \param const Complex&, source object.
       * \return Complex&.
       */
      Complex& operator=(const Complex&);

      /*
       * Assignement operator.
       * \param const std::complex&, source object.
       * \return Complex&.
       */
      Complex& operator=(const std::complex<ValueType>&);
    };
    
    /*
     * \brief return the conjugated Complex.
     * \param ValueType, base type of the Complex.
     * \param const Complex<ValueType>&, object to be conjugated.
     * \return const Complex<ValueType>.
     */
    template<typename ValueType>
    TFEL_MATH_INLINE 
    const Complex<ValueType>
    conj(const Complex<ValueType>&);

    /*
     * \brief return the real part of a Complex.
     * \param ValueType, base type of the Complex.
     * \param const Complex<ValueType>&, source object.
     * \return const ValueType.
     */
    template<typename ValueType>
    TFEL_MATH_INLINE const ValueType
    real(const Complex<ValueType>&);

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/General/Complex.ixx"

#endif /* _LIB_TFEL_MATH_COMPLEX_HXX */

