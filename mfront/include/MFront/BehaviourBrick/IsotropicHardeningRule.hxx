/*!
 * \file   include/MFront/BehaviourBrick/IsotropicHardeningRule.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICHARDENINGRULE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICHARDENINGRULE_HXX

#include <string>
#include <memory>
#include <vector>
#include "MFront/BehaviourDescription.hxx"

namespace tfel {
  namespace utilities {
    // forward declaration
    struct Data;
  }  // end of namespace utilities
}  // end of namespace tfel

namespace mfront {

  // forward declaration
  struct AbstractBehaviourDSL;

  namespace bbrick {

    // forward declaration
    struct OptionDescription;

    /*!
     * \brief class describing an isotropic hardening rule
     */
    struct IsotropicHardeningRule {
      //! a simple alias
      using Data = tfel::utilities::Data;
      //! a simple alias
      using DataMap = std::map<std::string, Data>;
      //! a simple alias
      using ModellingHypothesis = tfel::material::ModellingHypothesis;
      //! a simple alias
      using Hypothesis = ModellingHypothesis::Hypothesis;
      //! a simple alias
      using MaterialProperty = BehaviourDescription::MaterialProperty;
      //! a simple alias
      using MaterialPropertyInput = BehaviourDescription::MaterialPropertyInput;
      /*!
       * \param[in,out] bd: behaviour description
       * \param[in,out] dsl: abstract behaviour dsl
       * \param[in] id: flow id
       * \param[in] d: options
       */
      virtual void initialize(BehaviourDescription&,
                              AbstractBehaviourDSL&,
                              const std::string&,
                              const DataMap&) = 0;
      /*!
       * \brief return the code computing the initial radius.
       * The code defines a variable named "Rel"+id
       * \param[in] id: flow id
       */
      virtual std::string computeElasticPrediction(const std::string&) const = 0;
      /*!
       * \brief return the code computing the radius of the elastic limit.
       * The code defines a variable named "R"+id
       * \param[in] id: flow id
       */
      virtual std::string computeElasticLimit(const std::string&) const = 0;
      /*!
       * \brief return the code computing the radius of the elastic limit.
       * The code defines two variables named "R"+id and "dR"+id+"ddp"+id.
       * \param[in] id: flow id
       */
      virtual std::string computeElasticLimitAndDerivative(
          const std::string&) const = 0;
      /*!
       * \brief method called at the end of the input file processing
       * \param[in] dsl: abstract behaviour dsl
       * \param[in] bd: behaviour description
       * \param[in] id: flow id
       */
      virtual void endTreatment(BehaviourDescription&,
                                const AbstractBehaviourDSL&,
                                const std::string&) const = 0;
      //! \return the flow options
      virtual std::vector<OptionDescription> getOptions() const = 0;
      //! destructor
      virtual ~IsotropicHardeningRule();
    };  // end of struct IsotropicHardeningRule

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICHARDENINGRULE_HXX */
