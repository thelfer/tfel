/*!
 * \file
 * mfront/include/MFront/BehaviourBrick/MichelAndSuquet1992HollowSphereStressCriterion.hxx
 * \brief
 * \author Thomas Helfer, Jérémy Hure, Mohamed Shokeir
 * \date   20/07/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_MICHEL_SUQUET_1992_HOLLOW_SPHERE_STRESSCRITERION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_MICHEL_SUQUET_1992_HOLLOW_SPHERE_STRESSCRITERION_HXX

#include "MFront/BehaviourBrick/StandardPorousStressCriterionBase.hxx"

namespace mfront::bbrick {

  /*!
   * \brief implementation of the MichelAndSuquet1992HollowSphere stress
   * criterion
   */
  struct MichelAndSuquet1992HollowSphereStressCriterion
      : StandardPorousStressCriterionBase {
    /*!
     * \brief constructor
     */
    MichelAndSuquet1992HollowSphereStressCriterion();

    std::vector<BehaviourSymmetry> getSupportedBehaviourSymmetries()
        const override;

    std::vector<OptionDescription> getOptions() const override;

    PorosityEffectOnFlowRule getPorosityEffectOnEquivalentPlasticStrain()
        const override;

    //! \brief destructor
    ~MichelAndSuquet1992HollowSphereStressCriterion() override;
  };  // end of struct MichelAndSuquet1992HollowSphereStressCriterion

}  // end of namespace mfront::bbrick

#endif /* LIB__MFRONT_BEHAVIOURBRICK_MICHEL_SUQUET_1992_HOLLOW_SPHERE_STRESSCRITERION_HXX \
        */
