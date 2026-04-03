/*!
 * \file  include/TFEL/Math/Forward/vector.hxx
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

#ifndef LIB_TFEL_MATH_FORWARD_VECTOR_HXX
#define LIB_TFEL_MATH_FORWARD_VECTOR_HXX

#include "TFEL/Math/General/MathObjectTraits.hxx"

namespace tfel::math {

  /*!
   * \class vector
   * \brief  A class representing dynamically allocated vector.
   * \tparam  T: the underlying numerical type.
   * \author Thomas Helfer
   * \date   04/04/2007
   */
  template <typename T>
  struct [[nodiscard]] vector;

  /*!
   * \brief Partial specialisation for the vector class.
   * \see MathObjectTraits
   */
  template <typename T>
  struct MathObjectTraits<vector<T>> : MathObjectTraitsBase<T, std::size_t> {
    //! brief a `vector` is not related to the space dimension
    static constexpr unsigned short dime = 0;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_FORWARD_VECTOR_HXX */
