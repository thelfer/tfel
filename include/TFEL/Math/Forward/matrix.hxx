/*!
 * \file  include/TFEL/Math/Forward/matrix.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 19 sept. 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FORWARD_MATRIX_HXX
#define LIB_TFEL_MATH_FORWARD_MATRIX_HXX

#include "TFEL/Math/General/MathObjectTraits.hxx"

namespace tfel::math {

  /*!
   * \class matrix.
   * \brief a class representing a dynamically allocated matrix
   * \tparam T: the type hold by the matrix.
   */
  template <typename T>
  struct matrix;

  /*!
   * \brief partial specialisation for `matrix`.
   * This is a MatrixConcept requirement.
   * \see MathObjectTraits.
   */
  template <typename T>
  struct MathObjectTraits<matrix<T>> : MathObjectTraitsBase<T, std::size_t> {
    //! brief a `matrix` is not related to the space dimension
    static constexpr unsigned short dime = 0;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_FORWARD_MATRIX_HXX */
