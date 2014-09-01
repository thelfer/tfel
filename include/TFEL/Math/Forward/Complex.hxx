/*! 
 * \file  include/TFEL/Math/Forward/Complex.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 janv. 2012
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_FORWARD_COMPLEX_H_
#define _LIB_TFEL_MATH_FORWARD_COMPLEX_H_ 

namespace std
{

#ifndef MICROSOFT_COMPILER_MSC_
  template<typename T>
  struct complex;
#else
  template<typename T>
  class complex;
#endif

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

