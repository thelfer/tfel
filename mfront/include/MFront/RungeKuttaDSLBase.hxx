/*!
 * \file   mfront/include/MFront/RungeKuttaDSLBase.hxx
 * \brief  This file declares the RungeKuttaDSLBase class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_RUNGEKUTTADSLBASE_H_
#define LIB_MFRONT_RUNGEKUTTADSLBASE_H_

#include <string>
#include "MFront/BehaviourDSLBase.hxx"

namespace mfront {

  struct RungeKuttaDSLBase : public BehaviourDSLBase<RungeKuttaDSLBase> {
    //! \brief code block name
    static const char* const RungeKuttaUpdateAuxiliaryStateVariables;
    //! \return a generic description of DSL based on this class
    static std::string getDescription();
    //! constructor
    RungeKuttaDSLBase();
    //! destructor
    ~RungeKuttaDSLBase();

   private:
    enum ErrorEvaluation {
      DEFAULTERROREVALUATION,
      ERRORSUMMATIONEVALUATION,
      MAXIMUMVALUEERROREVALUATION
    };  // end of ErrorEvaluation

    std::string computeStressVariableModifier1(const Hypothesis,
                                               const std::string&,
                                               const bool);

    std::string computeStressVariableModifier2(const Hypothesis,
                                               const std::string&,
                                               const bool);

    virtual void treatUnknownVariableMethod(const Hypothesis,
                                            const std::string&) override;

    virtual void treatEpsilon(void);

    virtual void treatMinimalTimeStep(void);

    virtual void treatAlgorithm(void);

    virtual void treatComputeStress(void);

    virtual void treatComputeFinalStress(void);

    virtual void treatUpdateAuxiliaryStateVariables(void) override;

    virtual void treatDerivative(void);

    virtual void writeBehaviourParserSpecificIncludes(void) override;

    virtual void writeBehaviourParserSpecificTypedefs(void) override;

    virtual void writeBehaviourParserSpecificMembers(const Hypothesis) override;

    virtual void writeBehaviourLocalVariablesInitialisation(
        const Hypothesis) override;

    virtual void writeBehaviourEulerIntegrator(const Hypothesis);

    virtual void writeBehaviourRK2Integrator(const Hypothesis);

    virtual void writeBehaviourRK4Integrator(const Hypothesis);

    virtual void writeBehaviourRK42Integrator(const Hypothesis);

    virtual void writeBehaviourRK54Integrator(const Hypothesis);

    virtual void writeBehaviourRKCastemIntegrator(const Hypothesis);

    virtual void writeBehaviourUpdateStateVariables(const Hypothesis) override;

    virtual void writeBehaviourUpdateAuxiliaryStateVariables(
        const Hypothesis) override;

    virtual void writeBehaviourIntegrator(const Hypothesis) override;

    virtual void endsInputFileProcessing(void) override;

    virtual void setDefaultAlgorithm(void);

    friend struct BehaviourDSLBase<RungeKuttaDSLBase>;

    // error normalisation factors
    std::map<std::string, std::string> enf;
  };

}  // end of namespace mfront

#endif /* LIB_MFRONT_RUNGEKUTTADSLBASE_H_ */
