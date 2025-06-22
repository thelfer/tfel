/*!
 * \file
 * mfront/include/MFront/BehaviourBrick/RousselierTanguyBesson2002StressCriterion.hxx
 * \brief
 * \author Thomas Helfer, Jérémy Hure, Mohamed Shokeir
 * \date   14/04/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_ROUSSELIER_TANGUY_BESSON_2002_STRESSCRITERION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_ROUSSELIER_TANGUY_BESSON_2002_STRESSCRITERION_HXX

#include "MFront/BehaviourBrick/StandardPorousStressCriterionBase.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief implementation of the RousselierTanguyBesson2002 stress criterion
     */
    struct RousselierTanguyBesson2002StressCriterion
        : StandardPorousStressCriterionBase {
      /*!
       * \brief constructor
       */
      RousselierTanguyBesson2002StressCriterion();

      std::vector<BehaviourSymmetry> getSupportedBehaviourSymmetries()
          const override;

      std::vector<OptionDescription> getOptions() const override;

      PorosityEffectOnFlowRule getPorosityEffectOnEquivalentPlasticStrain()
          const override;

      std::string updatePorosityUpperBound(const BehaviourDescription&,
                                           const std::string&,
                                           const Role) const override;

      //! \brief destructor
      ~RousselierTanguyBesson2002StressCriterion() override;
    };  // end of struct RousselierTanguyBesson2002StressCriterion

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB__MFRONT_BEHAVIOURBRICK_ROUSSELIER_TANGUY_BESSON_2002_STRESSCRITERION_HXX \
        */
