/*!
 * \file   include/MFront/BehaviourBrick/KinematicHardeningRule.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_KINEMATICHARDENINGRULE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_KINEMATICHARDENINGRULE_HXX

#include <string>
#include <memory>
#include <vector>
#include "MFront/MFrontConfig.hxx"
#include "MFront/BehaviourDescription.hxx"

namespace tfel::utilities {
  // forward declaration
  struct Data;
  //! \brief a simple alias
  using DataMap = std::map<std::string, Data, std::less<>>;
}  // end of namespace tfel::utilities

namespace mfront {

  // forward declaration
  struct AbstractBehaviourDSL;

}  // end of namespace mfront

namespace mfront::bbrick {

  // forward declaration
  struct OptionDescription;
  // forward declaration
  struct StressPotential;
  // forward declaration
  struct StressCriterion;

  //! \brief class describing an kinematic hardening rule
  struct MFRONT_VISIBILITY_EXPORT KinematicHardeningRule {
    //! \brief a simple alias
    using Data = tfel::utilities::Data;
    //! \brief a simple alias
    using DataMap = tfel::utilities::DataMap;
    //! \brief a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    //! \brief a simple alias
    using Hypothesis = ModellingHypothesis::Hypothesis;
    //! \brief a simple alias
    using MaterialProperty = BehaviourDescription::MaterialProperty;
    //! \brief a simple alias
    using MaterialPropertyInput = BehaviourDescription::MaterialPropertyInput;
    /*!
     * \return the name of a variable from a base name, the flow id and the
     * kinematic hardening rule id.
     * \param[in] n: base name
     * \param[in] fid: flow id
     * \param[in] kid: kinematic hardening rule id
     */
    static std::string getVariableId(const std::string&,
                                     const std::string&,
                                     const std::string&);
    /*!
     * \param[in,out] bd: behaviour description
     * \param[in,out] dsl: abstract behaviour dsl
     * \param[in] fid: flow id
     * \param[in] kid: kinematic hardening rule id
     * \param[in] d: options
     */
    virtual void initialize(BehaviourDescription&,
                            AbstractBehaviourDSL&,
                            const std::string&,
                            const std::string&,
                            const DataMap&) = 0;
    /*!
     * \brief method called at the end of the input file processing
     * \param[in] dsl: abstract behaviour dsl
     * \param[in] bd: behaviour description
     * \param[in] fid: flow id
     * \param[in] kid: kinematic hardening rule id
     */
    virtual void endTreatment(BehaviourDescription&,
                              const AbstractBehaviourDSL&,
                              const std::string&,
                              const std::string&) const = 0;
    //! \return the flow options
    virtual std::vector<OptionDescription> getOptions() const = 0;
    /*!
     * \return the list of kinematic hardening variables.
     * \param[in] fid: flow id
     * \param[in] kid: kinematic hardening rule id
     */
    virtual std::vector<std::string> getKinematicHardeningsVariables(
        const std::string&, const std::string&) const = 0;
    /*!
     * \brief compute the initial values of the kinematic hardening variables.
     * Thoses initial values are used to compute an elastic prediction of the
     * criterion.
     * \param[in] fid: flow id
     * \param[in] kid: kinematic hardening rule id
     */
    virtual std::string computeKinematicHardeningsInitialValues(
        const std::string&, const std::string&) const = 0;
    /*!
     * \brief compute the values of the kinematic hardening variables
     * at \f$t+\theta\,dt\f$.
     * \param[in] fid: flow id
     * \param[in] kid: kinematic hardening rule id
     */
    virtual std::string computeKinematicHardenings(
        const std::string&, const std::string&) const = 0;
    /*!
     * \return the back-strain' name
     * \param[in] fid: flow id
     * \param[in] kid: kinematic hardening rule id
     */
    virtual std::string getBackStrainVariable(const std::string&,
                                              const std::string&) const = 0;
    /*!
     * \return the derivative of the back-stress with respect to the
     * back-strain
     * \param[in] fid: flow id
     * \param[in] kid: kinematic hardening rule id
     */
    virtual std::string getBackStressDerivative(const std::string&,
                                                const std::string&) const = 0;
    /*!
     * \brief compute the derivatives of a variable \f$v\f$ knowing the
     * derivative of \f$\frac{d\,v}{d\underline{s}}\f$.
     * \param[in] v: variable name
     * \param[in] mdfv_ds: opposite of the derivative of v with respect to the
     * effective stress.
     * \param[in] fid: flow id
     * \param[in] kid: kinematic hardening rule id
     */
    virtual std::string generateImplicitEquationDerivatives(
        const std::string&,
        const std::string&,
        const std::string&,
        const std::string&) const = 0;
    /*!
     * \return the code computing the implicit equations associated with the
     * back-strains and the derivatives of those equations (if requested).
     * \param[in] bd: behaviour description
     * \param[in] sp: stress potential
     * \param[in] fc: flow criterion
     * \param[in] khrs: list of kinematic hardening rules
     * \param[in] fid: flow id
     * \param[in] kid: kinematic hardening rule id
     * \param[in] b: compute derivatives
     */
    virtual std::string buildBackStrainImplicitEquations(
        const BehaviourDescription&,
        const StressPotential&,
        const StressCriterion&,
        const std::vector<std::shared_ptr<KinematicHardeningRule>>&,
        const std::string&,
        const std::string&,
        const bool) const = 0;
    //! destructor
    virtual ~KinematicHardeningRule();
  };  // end of struct KinematicHardeningRule

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_KINEMATICHARDENINGRULE_HXX */
