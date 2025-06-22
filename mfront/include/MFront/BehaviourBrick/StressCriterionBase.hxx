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

#include "MFront/MFrontConfig.hxx"
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
    struct MFRONT_VISIBILITY_EXPORT StressCriterionBase : StressCriterion {
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
      /*!
       * \return the code updating the upper bound of the porosity.
       * If this stress criterion is not coupled with porosity, the returned
       * value may be empty.
       * \param[in] bd: behaviour description
       */
      std::string updatePorosityUpperBound(const BehaviourDescription&,
                                           const std::string&,
                                           const Role) const override;
      //! destructor
      ~StressCriterionBase() override;
    };  // end of StressCriterionBase

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_STRESSCRITERIONBASE_HXX */
