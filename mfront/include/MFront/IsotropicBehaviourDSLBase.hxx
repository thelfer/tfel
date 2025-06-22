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

#include <string>

#include "MFront/MFrontConfig.hxx"
#include "MFront/BehaviourDSLBase.hxx"

namespace mfront {

  struct MFRONT_VISIBILITY_EXPORT IsotropicBehaviourDSLBase
      : public BehaviourDSLBase<IsotropicBehaviourDSLBase> {
    IsotropicBehaviourDSLBase();

    ~IsotropicBehaviourDSLBase() override;

   protected:
    void treatExternalStateVariable() override;

    void completeVariableDeclaration() override;

    void endsInputFileProcessing() override;

    /*!
     * \return true if the given modelling hypothesis is handled by
     * the parser
     *
     * Some parsers have restrictions on the modelling hypotheses
     * supported. For example, the isotropic behaviours were not able
     * to handle plane stress hypotheses when this comment was
     * written(but it was planned, so this comment may be outdated
     * now).
     *
     * The fact that this method returns true means that the user
     * *can* provide code to support this modelling hypothesis. For
     * example, to support plane stress in RungeKutta and Implicit
     * parsers, the user must provide some hand-crafted code. He must
     * enable this modelling hypothesis by calling explicitely
     * `@ModellingHypothesis` or `@ModellingHypotheses` keywords.
     */
    bool isModellingHypothesisSupported(const Hypothesis) const override;

    void writeBehaviourParserSpecificIncludes(std::ostream&) const override;

    void writeBehaviourParserSpecificTypedefs(std::ostream&) const override;

    void writeBehaviourLocalVariablesInitialisation(
        std::ostream&, const Hypothesis) const override;

    void writeBehaviourComputePredictionOperator(
        std::ostream&, const Hypothesis) const override;

    void writeBehaviourComputeTangentOperator(std::ostream&,
                                              const Hypothesis) const override;

    virtual double getDefaultThetaValue() const;

    virtual void treatFlowRule();

    virtual void treatTheta();

    virtual void treatEpsilon();

    virtual void treatIterMax();

    virtual std::string flowRuleVariableModifier(const Hypothesis,
                                                 const std::string&,
                                                 const bool);

   private:
    friend struct BehaviourDSLBase<IsotropicBehaviourDSLBase>;

  };  // end of class IsotropicBehaviourDSLBase

}  // namespace mfront

#endif /* LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX */
