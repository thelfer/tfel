/*!
 * \file   include/TFEL/TypeTraits/IsFundamentalNumericType.hxx
 * \brief  This file declares the IsFundamentalNumericType traits
 * class and specialises it for standard numerical types.
 * \author Thomas Helfer
 * \date   05 jui 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TYPETRAITS_ISFUNDAMENTALNUMERICTYPE_HXX
#define LIB_TFEL_TYPETRAITS_ISFUNDAMENTALNUMERICTYPE_HXX

#include <type_traits>
#include "TFEL/Math/Forward/Complex.hxx"

namespace tfel::typetraits {

  /*!
   * \brief Traits class which says if its argument is one of the
   * standard numerical types
   *
   * \param  T, type to be tested.
   * \return bool cond, true if T is one of the standard numerical
   * types, false otherwise.
   *
   * \see IsReal, IsComplex
   *
   * \author Thomas Helfer
   * \date   04 Jul 2006
   */
  template <typename T>
  struct IsFundamentalNumericType {
    //! \brief result
    static constexpr bool cond = std::is_arithmetic_v<T>;
  };

  /*!
   * \brief partial specialisation for `tfel::math::Complex`
   */
  template <typename T>
  struct IsFundamentalNumericType<tfel::math::Complex<T>> {
    //! \brief result
    static constexpr bool cond = std::is_arithmetic_v<T>;
  };

  /*!
   * \brief partial specialisation for `const tfel::math::Complex`
   */
  template <typename T>
  struct IsFundamentalNumericType<const tfel::math::Complex<T>> {
    //! \brief result
    static constexpr bool cond = std::is_arithmetic_v<T>;
  };

  /*!
   * \brief a simple helper function
   * \tparam T: type tested
   */
  template <typename T>
  constexpr bool isFundamentalNumericType() {
    return IsFundamentalNumericType<std::decay_t<T>>::cond;
  }

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_TYPETRAITS_ISFUNDAMENTALNUMERICTYPE_HXX */
