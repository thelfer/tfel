/*!
 * \file   include/MFront/BehaviourBrick/MisesStressCriterion.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_MISESSTRESSCRITERION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_MISESSTRESSCRITERION_HXX

#include "MFront/BehaviourBrick/StressCriterion.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief class describing an inelastic potential.
     */
    struct MisesStressCriterion final : StressCriterion {
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const DataMap&) override;
      std::vector<OptionDescription> getOptions() const override;
      std::string computeNormal(const std::string&) const override;
      std::string computeNormalDerivative(const std::string&) const override;
      //! destructor
      ~MisesStressCriterion() override;
    };  // end of struct MisesStressCriterion

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_MISESSTRESSCRITERION_HXX */
