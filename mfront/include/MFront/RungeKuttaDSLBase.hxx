/*!
 * \file   mfront/include/MFront/RungeKuttaDSLBase.hxx
 * \brief  This file declares the RungeKuttaDSLBase class
 * \author Thomas Helfer
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_RUNGEKUTTADSLBASE_HXX
#define LIB_MFRONT_RUNGEKUTTADSLBASE_HXX

#include <string>
#include "MFront/BehaviourDSLBase.hxx"

namespace mfront {

  struct RungeKuttaDSLBase : public BehaviourDSLBase<RungeKuttaDSLBase> {
    //! \brief code block name
    static const char* const RungeKuttaUpdateAuxiliaryStateVariables;
    //! \return a generic description of DSL based on this class
    static std::string getDescription();
    /*!
     * \brief constructor
     *\param[in] opts: options passed to the DSL
     */
    RungeKuttaDSLBase(const DSLOptions&);
    //
    std::string getCodeBlockTemplate(
        const std::string&,
        const MFrontTemplateGenerationOptions&) const override;
    //! \brief destructor
    ~RungeKuttaDSLBase() override;

   protected:
    std::string computeThermodynamicForcesVariableModifier1(const Hypothesis,
                                                            const std::string&,
                                                            const bool);

    std::string computeThermodynamicForcesVariableModifier2(const Hypothesis,
                                                            const std::string&,
                                                            const bool);
    //
    void getSymbols(std::map<std::string, std::string>&,
                    const Hypothesis,
                    const std::string&) override;
    void treatUnknownVariableMethod(const Hypothesis,
                                    const std::string&) override;
    std::unique_ptr<AbstractBehaviourCodeGenerator> getCodeGenerator()
        const override;
    //! treat the `@Epsilon` keywork
    virtual void treatEpsilon();
    //! treat the `@MinimalTimeStep` keywork
    virtual void treatMinimalTimeStep();
    //! treat the `@Algorithm` keywork
    virtual void treatAlgorithm();
    //! treat the `@StressErrorNormalizationFactor` keywork
    virtual void treatStressErrorNormalizationFactor();
    //! treat the `@ComputeThermodynamicForces` keywork
    virtual void treatComputeThermodynamicForces();
    //! treat the `@ComputeFinalThermodynamicForces` keywork
    virtual void treatComputeFinalThermodynamicForces();
    //! treat the `@UpdateAuxiliaryStateVariables` keywork
    void treatUpdateAuxiliaryStateVariables() override;
    //! treat the `@Derivative` keywork
    virtual void treatDerivative();

    virtual void setDefaultAlgorithm();

    void completeVariableDeclaration() override;

    void endsInputFileProcessing() override;

    friend struct BehaviourDSLBase<RungeKuttaDSLBase>;
  };

}  // end of namespace mfront

#endif /* LIB_MFRONT_RUNGEKUTTADSLBASE_HXX */
