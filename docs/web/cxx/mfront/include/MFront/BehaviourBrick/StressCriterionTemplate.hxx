/*!
 * \file
 * mfront/include/MFront/BehaviourBrick/__StressCriterionName__StressCriterion.hxx
 * \brief
 * \author __Author__
 * \date   __Date__
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK___STRESS_CRITERION_NAME___STRESSCRITERION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK___STRESS_CRITERION_NAME___STRESSCRITERION_HXX

#include "MFront/BehaviourBrick/StandardStressCriterionBase.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief implementation of the __StressCriterionName__ stress criterion
     */
    struct __StressCriterionName__StressCriterion
        : StandardStressCriterionBase {
      /*!
       * \brief constructor
       */
      __StressCriterionName__StressCriterion();

      std::vector<BehaviourSymmetry> getSupportedBehaviourSymmetries()
          const override;

      std::vector<OptionDescription> getOptions() const override;

      bool isNormalDeviatoric() const override;
      //! \brief destructor
      ~__StressCriterionName__StressCriterion() override;
    };  // end of struct __StressCriterionName__StressCriterion

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB__MFRONT_BEHAVIOURBRICK___STRESS_CRITERION_NAME___STRESSCRITERION_HXX \
        */
