/*!
 * \file   include/TFEL/Math/General/MathObjectTraits.hxx
 * \brief
 * \author Thomas Helfer
 * \date   28/12/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_GENERAL_MATHOBJECTTRAITS_HXX
#define LIB_TFEL_MATH_GENERAL_MATHOBJECTTRAITS_HXX

#include <type_traits>
#include "TFEL/Metaprogramming/InvalidType.hxx"

namespace tfel::math {

  /*!
   * \brief a traits class describing a mathematical object
   * \tparam MathObjectType: object described by the traits
   */
  template <typename MathObjectType>
  struct MathObjectTraits {
    //! \brief numerical type on which the object is based
    using NumType = tfel::meta::InvalidType;
    //! \brief index type
    using IndexType = tfel::meta::InvalidType;
    /*!
     * \brief space dimension
     *
     * This value is set to 0 if the object is not related to the space
     * dimension
     */
    static constexpr const unsigned short dime = 0u;
  };  // end of struct MathObjectTraits

  //! \brief a simple alias
  template <typename MathObjectType>
  using MathObjectNumType =
      typename MathObjectTraits<std::decay_t<MathObjectType>>::NumType;

  /*!
   * \brief an helper function to retrieve the space dimension associated with a
   * math object.
   * \tparam MathObjectType: math object
   */
  template <typename MathObjectType>
  constexpr unsigned short getSpaceDimension() {
    return MathObjectTraits<std::decay_t<MathObjectType>>::dime;
  } // end of getSpaceDimension

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_GENERAL_MATHOBJECTTRAITS_HXX */
