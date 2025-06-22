/*!
 * \file   mfront/include/MFront/RungeKuttaCodeGeneratorBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   03/08/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_RUNGEKUTTACODEGENERATORBASE_HXX
#define LIB_RUNGEKUTTACODEGENERATORBASE_HXX

#include "MFront/BehaviourCodeGeneratorBase.hxx"

namespace mfront {

  struct RungeKuttaCodeGeneratorBase : BehaviourCodeGeneratorBase {
    /*!
     * \brief constructor
     * \param[in] f: file description
     * \param[in] d: behaviour description
     * \param[in] bim: behaviour interfaces
     */
    RungeKuttaCodeGeneratorBase(const FileDescription&,
                                const BehaviourDescription&,
                                const BehaviourInterfaceMap&);
    //! \brief destructor
    ~RungeKuttaCodeGeneratorBase() override;

   protected:
    enum ErrorEvaluation {
      DEFAULTERROREVALUATION,
      ERRORSUMMATIONEVALUATION,
      MAXIMUMVALUEERROREVALUATION
    };  // end of ErrorEvaluation
    bool usesStateVariableTimeDerivative() const override;
    void writeBehaviourParserSpecificIncludes(std::ostream&) const override;
    void writeBehaviourParserSpecificTypedefs(std::ostream&) const override;
    void writeBehaviourParserSpecificMembers(std::ostream&,
                                             const Hypothesis) const override;
    void writeBehaviourLocalVariablesInitialisation(
        std::ostream&, const Hypothesis) const override;
    void writeBehaviourUpdateStateVariables(std::ostream&,
                                            const Hypothesis) const override;
    void writeBehaviourUpdateAuxiliaryStateVariables(
        std::ostream&, const Hypothesis) const override;
    void writeBehaviourIntegrator(std::ostream&,
                                  const Hypothesis) const override;
    //
    virtual void writeBehaviourEulerIntegrator(std::ostream&,
                                               const Hypothesis) const;
    virtual void writeBehaviourRK2Integrator(std::ostream&,
                                             const Hypothesis) const;
    virtual void writeBehaviourRK4Integrator(std::ostream&,
                                             const Hypothesis) const;
    virtual void writeBehaviourRK42Integrator(std::ostream&,
                                              const Hypothesis) const;
    virtual void writeBehaviourRK54Integrator(std::ostream&,
                                              const Hypothesis) const;
    virtual void writeBehaviourRKCastemIntegrator(std::ostream&,
                                                  const Hypothesis) const;
  };  // end of RungeKuttaCodeGeneratorBase

}  // end of namespace mfront

#endif /* LIB_RUNGEKUTTACODEGENERATORBASE_HXX */
