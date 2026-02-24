/*!
 * \file   mfront/include/MFront/IsotropicBehaviourDSLBase.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   01 jui 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX
#define LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX

#include <map>
#include <vector>
#include <memory>
#include <string>
#include <string_view>

#include "MFront/MFrontConfig.hxx"
#include "MFront/BehaviourDSLBase.hxx"

namespace mfront::bbrick {

  // forward declarations
  struct IsotropicHardeningRule;

}  // namespace mfront::bbrick

namespace mfront {

  struct MFRONT_VISIBILITY_EXPORT IsotropicBehaviourDSLBase
      : public BehaviourDSLBase<IsotropicBehaviourDSLBase> {
    /*!
     * \brief attribute name
     *
     * If this attribute is set to true, the elastic strain is not defined and
     * the stress is computed incrementally.
     */
    static const char* const useStressUpdateAlgorithm;
    //! \return a validator for the options passed to the DSL
    static tfel::utilities::DataMapValidator getDSLOptionsValidator();
    /*!
     * \brief constructor
     *\param[in] opts: options passed to the DSL
     */
    IsotropicBehaviourDSLBase(const DSLOptions&);
    //
    BehaviourDSLDescription getBehaviourDSLDescription() const override;
    void getSymbols(std::map<std::string, std::string>&,
                    const Hypothesis,
                    const std::string&) override;
    //! \brief destructor
    ~IsotropicBehaviourDSLBase() override;

   protected:
    //! \brief treat the `@Predictor` keyword
    virtual void treatPredictor();
    //
    void treatExternalStateVariable() override;
    void completeVariableDeclaration() override;
    void endsInputFileProcessing() override;
    //! \return if multiple flow rules are allowed.
    virtual bool allowMultipleFlowRules() const;
    /*!
     * \brief return the number of flow rules.
     * \note this information is only meaningful after processing the input file
     */
    virtual std::size_t getNumberOfFlowRules() const;
    //! \brief return the default value of the theta parameter
    virtual double getDefaultThetaValue() const;
    //! \brief treat the `@FlowRule` keyword
    virtual void treatFlowRule();
    //! \brief treat the `@Theta` keyword
    virtual void treatTheta();
    //! \brief treat the `@Epsilon` keyword
    virtual void treatEpsilon();
    //! \brief treat the `@IterMax` keyword
    virtual void treatIterMax();
    //! \brief treat the `@IsotropicHardeningRule` keyword
    virtual void treatIsotropicHardeningRule();
    //! \brief treat the `@IsotropicHardeningRules` keyword
    virtual void treatIsotropicHardeningRules();
    /*!
     * \brief method used to modify the variables when analysing @FlowRule
     * blocks
     */
    virtual std::string flowRuleVariableModifier(const Hypothesis,
                                                 const std::string&,
                                                 const bool);
    /*!
     * \brief performs some check on the given code block describing a flow rule
     * \param[in] n: code block name
     * \param[in] id: flow id

     * \param[in] is_df_dp_required: check if the df_dp shall be used
     */
    virtual void checkFlowRule(std::string_view,
                               const std::size_t,
                               const bool) const;
    /*!
     * \return if the DSLs allows a dependency of the yield surface or the
     * viscoplastic strain rate to the equivalent plastic strain
     */
    virtual bool handleStrainHardening() const;
    //! \return the analyser of the @FlowRule code block
    virtual std::function<
        void(CodeBlock&, const Hypothesis, const std::string&)>
    getFlowRuleAnalyser(const std::size_t) const;
    //! \brief isotropic hardening rules sorted by flow rules
    std::map<
        std::size_t,
        std::vector<std::shared_ptr<::mfront::bbrick::IsotropicHardeningRule>>>
        ihrs;

   private:
    friend struct BehaviourDSLBase<IsotropicBehaviourDSLBase>;

  };  // end of class IsotropicBehaviourDSLBase

}  // namespace mfront

#endif /* LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX */
