/*!
 * \file   mfront/include/MFront/GenericBehaviour/GenericBehaviourTraits.hxx
 * \brief
 * \author Thomas Helfer
 * \date   31/01/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICBEHAVIOUR_GENERICBEHAVIOURTRAITS_HXX
#define LIB_MFRONT_GENERICBEHAVIOUR_GENERICBEHAVIOURTRAITS_HXX

#include "TFEL/Material/ModellingHypothesis.hxx"

namespace mfront::gb {

  /*!
   * \brief The `GenericBehaviourTraits` class defines a set of static constexpr
   * data members used for adapting the `gerneric` interface.
   * \tparam Behaviour: behaviour class
   *
   * - The `hypothesis` data member shall always be defined.
   * - For plane stress hypotheses and finite strain behaviours defined through
   *   a strain measure, the `has_axial_strain_offset` shall be defined. If
   * true, the data member `axial_strain_offset` gives the offset associated
   * with the axial strain in the internal state variables
   * - For plane stress hypotheses and standard finite strain behaviours,
   *   the `has_axial_deformation_gradient_offset` shall be defined. If true,
   *   the data member `axial_deformation_gradient_offset` gives the offset
   *   associated with the axial strain in the internal state variables
   */
  template <typename Behaviour>
  struct GenericBehaviourTraits {
    //! \brief modelling hypothesis
    static constexpr auto hypothesis =
        tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
  };  // end of struct GenericBehaviourTraits

}  // end of namespace mfront::gb

#endif /* LIB_MFRONT_GENERICBEHAVIOUR_GENERICBEHAVIOURTRAITS_HXX */
