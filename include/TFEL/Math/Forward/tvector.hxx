/*!
 * \file  include/TFEL/Math/Forward/tvector.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 19 sept. 2011
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FORWARD_TVECTOR_HXX
#define LIB_TFEL_MATH_FORWARD_TVECTOR_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"

namespace tfel::math {

  /*!
   * \brief a base for tvector or classes acting like tvector.
   */
  template <typename Child, unsigned short N, typename T>
  struct tvector_base;

  /*!
   * \brief a class representing tiny vectors of fixed size.
   */
  template <unsigned short N, typename T>
  struct tvector;

  /*!
   * \brief Partial specialisation for tvectors.
   * \note This is a VectorConcept requirement.
   * \see MathObjectTraits.
   */
  template <unsigned short N, typename T>
  struct TFEL_VISIBILITY_LOCAL MathObjectTraits<tvector<N, T>>
      : MathObjectTraitsBase<T, unsigned short> {
    //! brief a `tvector` is not related to the space dimension
    static constexpr const unsigned short dime = 0;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_FORWARD_TVECTOR_HXX */
