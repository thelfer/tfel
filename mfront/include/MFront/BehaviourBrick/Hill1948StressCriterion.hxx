/*!
 * \file   include/MFront/BehaviourBrick/Hill1948StressCriterion.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_HILL1948STRESSCRITERION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_HILL1948STRESSCRITERION_HXX

#include "MFront/BehaviourBrick/StressCriterion.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief class describing an inelastic potential.
     */
    struct Hill1948StressCriterion final : StressCriterion {
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const std::string&,
                      const DataMap&) override;
      std::vector<OptionDescription> getOptions() const override;
      std::string computeElasticPrediction(const std::string&) const override;
      std::string computeCriterion(const std::string&) const override;
      std::string computeNormal(const std::string&) const override;
      std::string computeNormalDerivative(const std::string&) const override;
      //! destructor
      ~Hill1948StressCriterion() override;
    };  // end of struct Hill1948StressCriterion

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_HILL1948STRESSCRITERION_HXX */
