/*!
 * \file   StressCriterionBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   10/04/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_STRESSCRITERIONBASE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_STRESSCRITERIONBASE_HXX

#include "MFront/BehaviourBrick/StressCriterion.hxx"

namespace mfront {

  // forward declaration
  struct AbstractBehaviourDSL;

  namespace bbrick {

    // forward declaration
    struct OptionDescription;

    /*!
     * \brief an helper class used to build a stress criterion.
     */
    struct StressCriterionBase : StressCriterion {
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const std::string&,
                      const DataMap&,
                      const Role) override;
      void endTreatment(BehaviourDescription&,
                        const AbstractBehaviourDSL&,
                        const std::string&,
                        const Role) override;
      std::vector<OptionDescription> getOptions() const override;
      //! destructor
      ~StressCriterionBase() override;
    };  // end of StressCriterionBase

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_STRESSCRITERIONBASE_HXX */
