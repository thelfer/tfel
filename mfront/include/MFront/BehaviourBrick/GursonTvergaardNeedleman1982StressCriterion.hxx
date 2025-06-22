/*!
 * \file
 * mfront/include/MFront/BehaviourBrick/GursonTvergaardNeedleman1982StressCriterion.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_GURSON_TVERGAARD_NEEDLEMAN_1982_STRESSCRITERION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_GURSON_TVERGAARD_NEEDLEMAN_1982_STRESSCRITERION_HXX

#include "MFront/BehaviourBrick/StandardPorousStressCriterionBase.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief implementation of the MichelAndSuquet1992HollowSphere stress
     * criterion
     */
    struct GursonTvergaardNeedleman1982StressCriterion
        : StandardPorousStressCriterionBase {
      /*!
       * \brief constructor
       */
      GursonTvergaardNeedleman1982StressCriterion();

      std::vector<BehaviourSymmetry> getSupportedBehaviourSymmetries()
          const override;

      std::vector<OptionDescription> getOptions() const override;

      PorosityEffectOnFlowRule getPorosityEffectOnEquivalentPlasticStrain()
          const override;

      std::string updatePorosityUpperBound(const BehaviourDescription&,
                                           const std::string&,
                                           const Role) const override;

      //! \brief destructor
      ~GursonTvergaardNeedleman1982StressCriterion() override;
    };  // end of struct GursonTvergaardNeedleman1982StressCriterion

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB__MFRONT_BEHAVIOURBRICK_GURSON_TVERGAARD_NEEDLEMAN_1982_STRESSCRITERION_HXX \
        */
