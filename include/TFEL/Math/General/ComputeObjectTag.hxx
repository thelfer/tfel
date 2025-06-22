/*!
 * \file   include/TFEL/Math/General/ComputeObjectTag.hxx
 * \brief  This file declares the ComputeObjectTag class
 * \author Thomas Helfer
 * \date   16 Oct 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_COMPUTEOBJECTTAG_HXX
#define LIB_TFEL_MATH_COMPUTEOBJECTTAG_HXX

namespace tfel::math {

  /*!
   * \class ScalarTag
   * \brief Helper class to characterise scalars.
   */
  struct ScalarTag;

  /*!
   * \class UnaryOperatorTag
   * \brief Helper class to characterise operators.
   */
  struct UnaryOperatorTag;

  /*!
   * \class ComputeObjectTag
   * \brief a metafunction which compute the tag.
   * associated with a given type.
   * \param typename T, type of which we want to know the associated.
   * \return type, the associated tag.
   * \note   returns InvalidType if not tag matches.
   */
  template <typename T>
  struct ComputeObjectTag;

  /*!
   * \brief metafunction returning of the given type defines a type called
   * `ConceptTag`.
   * \param[in] T: type checked
   */
  template <typename T>
  constexpr bool hasConceptTag();

}  // end of namespace tfel::math

#include "TFEL/Math/General/ComputeObjectTag.ixx"

#endif /* LIB_TFEL_MATH_COMPUTEOBJECTTAG_HXX */
