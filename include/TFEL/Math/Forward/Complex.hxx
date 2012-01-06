/*! 
 * \file  Complex.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 janv. 2012
 */

#ifndef _LIB_TFEL_MATH_FORWARD_COMPLEX_H_
#define _LIB_TFEL_MATH_FORWARD_COMPLEX_H_ 

namespace std
{

  template<typename T>
  struct complex;

} // end of namespace std

namespace tfel
{

  namespace math
  {
    
    /*
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
    struct Complex;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_COMPLEX_H */

