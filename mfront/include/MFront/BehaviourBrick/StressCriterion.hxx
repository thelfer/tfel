/*!
 * \file   include/MFront/BehaviourBrick/StressCriterion.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_STRESSCRITERION_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_STRESSCRITERION_HXX

#include <string>
#include <vector>
#include "MFront/MFrontConfig.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/BehaviourSymmetryType.hxx"
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
    // forward declaration
    struct StressPotential;

    //! \brief class describing a stress criterion
    struct MFRONT_VISIBILITY_EXPORT StressCriterion {
      /*!
       * \brief describe the purpose of the criterion
       */
      enum Role {
        STRESSCRITERION,  //! the criterion is only used to predict the flow
                          //! intensity
        FLOWCRITERION,    //! the criterion is only used to predict the flow
                          //! direction
        STRESSANDFLOWCRITERION  //! the criterion is used to predict the flow
                                //! intensity and the flow direction
      };
      //! a simple alias
      using Data = tfel::utilities::Data;
      //! a simple alias
      using DataMap = std::map<std::string, Data>;
      //! a simple alias
      using ModellingHypothesis = tfel::material::ModellingHypothesis;
      //! a simple alias
      using Hypothesis = ModellingHypothesis::Hypothesis;
      //! \brief a simple alias
      using BehaviourSymmetry = mfront::BehaviourSymmetryType;
      //! a simple alias
      using MaterialProperty = BehaviourDescription::MaterialProperty;
      /*!
       * \return the name of a variable from a base name and the flow id.
       * \param[in] n: base name
       * \param[in] fid: flow id
       * \param[in] r: criterion' role
       */
      static std::string getVariableId(const std::string&,
                                       const std::string&,
                                       const Role);
      //! \return the flow options
      virtual std::vector<OptionDescription> getOptions() const = 0;
      //! \return the list of supported behaviour symmetries
      virtual std::vector<BehaviourSymmetry> getSupportedBehaviourSymmetries()
          const = 0;
      /*!
       * \param[in,out] bd: behaviour description
       * \param[in,out] dsl: abstract behaviour dsl
       * \param[in] id: flow id
       * \param[in] d: options
       * \param[in] r: criterion' role
       */
      virtual void initialize(BehaviourDescription&,
                              AbstractBehaviourDSL&,
                              const std::string&,
                              const DataMap&,
                              const Role) = 0;
      /*!
       * \param[in,out] bd: behaviour description
       * \param[in,out] dsl: abstract behaviour dsl
       * \param[in] id: flow id
       * \param[in] r: criterion' role
       */
      virtual void endTreatment(BehaviourDescription&,
                                const AbstractBehaviourDSL&,
                                const std::string&,
                                const Role) = 0;
      /*!
       * \return the code that defines `seqel`+id, the elastic prediction of
       * the criterion.
       * \note this is only meaningful if the criterion' role is
       * `STRESSCRITERION` or `STRESSANDFLOWCRITERION`.
       * \param[in] id: flow id
       * \param[in] bd: behaviour description
       * \param[in] sp: stress potential
       */
      virtual std::string computeElasticPrediction(
          const std::string&,
          const BehaviourDescription&,
          const StressPotential&) const = 0;
      /*!
       * \return the code computing the criterion.
       * The code defines a variable named "seq"+id.
       * \note this is only meaningful if the criterion' role is
       * `STRESSCRITERION` or `STRESSANDFLOWCRITERION`.
       * \param[in] id: flow id
       * \param[in] bd: behaviour description
       * \param[in] sp: stress potential
       */
      virtual std::string computeCriterion(const std::string&,
                                           const BehaviourDescription&,
                                           const StressPotential&) const = 0;
      /*!
       * \return the code computing the criterion and its normal.
       *
       * If the criterion' role is `STRESSCRITERION`,
       * the code defines two variables named "seq"+id and "dseq"+id+"_ds"+id.
       * The code may also define a variable named "iseq"+id meant for internal
       * use.
       *
       * If the criterion' role is `FLOWCRITERION`,
       * the code defines a variable "n"+id. The code may also define two
       * variables respectively named "seqf"+id and "iseqf"+id meant for
       * internal use.
       *
       * If the criterion' role is `STRESSANDFLOWCRITERION`,
       * the code defines two variables named "seq"+id and "dseq"+id+"ds"+id. A
       * reference named "n"+id to "dseq"+id+"_ds"+id is also defined. For
       * internal use, the code may also define a
       * variable named "iseq"+id.
       *
       * \param[in] id: flow id
       * \param[in] bd: behaviour description
       * \param[in] sp: stress potential
       * \param[in] r: criterion' role
       */
      virtual std::string computeNormal(const std::string&,
                                        const BehaviourDescription&,
                                        const StressPotential&,
                                        const Role) const = 0;
      /*!
       * \brief return the code computing the criterion, its derivative, and its
       * second derivative.
       *
       * If the criterion' role is `STRESSCRITERION`,
       * the code defines three variables named "seq"+id, "dseq"+id+"ds"+id and
       * "d2seq"+id+"_ds"+id+"_ds"+id.
       * The code may also define a variable named "iseq"+id meant for internal
       * use.
       *
       * If the criterion' role is `FLOWCRITERION`,
       * the code defines two variables named "n"+id and "dn"+id+"_ds"+id. The
       * code may also define two variables respectively named "seqf"+id and
       * "iseqf"+id meant for internal use.
       *
       * If the criterion' role is `STRESSANDFLOWCRITERION` the code defines:
       * - three variables named "seq"+id, "dseq"+id+"ds"+id and
       *   "d2seq"+id+"_ds"+id+"_ds"+id.
       * - a reference named "n"+id to "dseq"+id+"ds"+id.
       * - a reference named "dn"+id+"ds"+id to "d2seq"+id+"_ds"+id+"_ds"+id.
       * The code may also define a variable named "iseq"+id meant for internal
       * use.
       * \param[in] id: flow id
       * \param[in] bd: behaviour description
       * \param[in] sp: stress potential
       * \param[in] r: criterion' role
       */
      virtual std::string computeNormalDerivative(const std::string&,
                                                  const BehaviourDescription&,
                                                  const StressPotential&,
                                                  const Role) const = 0;
      //! \return if the normal is deviatoric
      virtual bool isNormalDeviatoric() const = 0;
      //! destructor
      virtual ~StressCriterion();
    };  // end of struct StressCriterion

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_STRESSCRITERION_HXX */
