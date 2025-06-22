/*!
 * \file   include/MFront/BehaviourBrick/IsotropicHardeningRule.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICHARDENINGRULE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICHARDENINGRULE_HXX

#include <string>
#include <memory>
#include <vector>
#include "MFront/MFrontConfig.hxx"
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

    //! \brief class describing an isotropic hardening rule
    struct MFRONT_VISIBILITY_EXPORT IsotropicHardeningRule {
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
       * \return the name of a variable from a base name and an identifier
       * \param[in] n: base name
       * \param[in] fid: flow id
       * \param[in] id: identifier
       */
      static std::string getVariableId(const std::string&,
                                       const std::string&,
                                       const std::string&);
      /*!
       * \param[in,out] bd: behaviour description
       * \param[in,out] dsl: abstract behaviour dsl
       * \param[in] fid: flow id
       * \param[in] id: identifier
       * \param[in] d: options
       */
      virtual void initialize(BehaviourDescription&,
                              AbstractBehaviourDSL&,
                              const std::string&,
                              const std::string&,
                              const DataMap&) = 0;
      /*!
       * \brief return the code computing the initial radius.
       * The code defines a variable named "Rel"+fid+"_"+id
       * \param[in] fid: flow id
       * \param[in] id: identifier
       */
      virtual std::string computeElasticPrediction(
          const std::string&, const std::string&) const = 0;
      /*!
       * \brief return the code computing the radius of the elastic limit.
       * The code defines a variable named "R"+fid+"_"+id
       * \param[in] fid: flow id
       * \param[in] id: identifier
       */
      virtual std::string computeElasticLimit(const std::string&,
                                              const std::string&) const = 0;
      /*!
       * \brief return the code computing the radius of the elastic limit.
       * The code defines two variables named "R"+fid+"_"+id and
       * "dR"+fid+"_"+id+"ddp"+fid+"_"+id.
       * \param[in] fid: flow id
       * \param[in] id: identifier
       */
      virtual std::string computeElasticLimitAndDerivative(
          const std::string&, const std::string&) const = 0;
      /*!
       * \brief method called at the end of the input file processing
       * \param[in] dsl: abstract behaviour dsl
       * \param[in] bd: behaviour description
       * \param[in] fid: flow id
       * \param[in] id: identifier
       */
      virtual void endTreatment(BehaviourDescription&,
                                const AbstractBehaviourDSL&,
                                const std::string&,
                                const std::string&) const = 0;
      //! \return the flow options
      virtual std::vector<OptionDescription> getOptions() const = 0;
      //! destructor
      virtual ~IsotropicHardeningRule();
    };  // end of struct IsotropicHardeningRule

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICHARDENINGRULE_HXX */
