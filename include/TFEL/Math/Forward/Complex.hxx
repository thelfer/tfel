/*! 
 * \file  include/TFEL/Math/Forward/Complex.hxx
 * \brief This file introduces some forward declaration relative to
 * the Complex class.
 * \author Helfer Thomas
 * \brief 03 janv. 2012
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_MATH_FORWARD_COMPLEX_H_
#define LIB_TFEL_MATH_FORWARD_COMPLEX_H_ 

namespace std
{

#ifndef MICROSOFT_COMPILER_MSC_
  template<typename T>
  struct complex;
#else
  template<typename T>
  class complex;
#endif /* LIB_TFEL_MATH_FORWARD_COMPLEX_H_ */

} // end of namespace std

namespace tfel
{

  namespace math
  {
    
    /*!
     * \brief an alias std::complex.
     * \param ValueType, underlying type.
     * \see std::complex.
     */
    template<typename ValueType>
    using Complex = std::complex<ValueType>;

  } // end of namespace math

} // end of namespace tfel

#endif /* LIB_TFEL_MATH_FORWARD_COMPLEX_H_ */

