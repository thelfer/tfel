/*!
 * \file   mfront/include/MFront/IsotropicBehaviourDSLBase.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   01 jui 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX_
#define LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX_

#include <string>

#include "MFront/MFrontConfig.hxx"
#include "MFront/BehaviourDSLBase.hxx"

namespace mfront {

  struct MFRONT_VISIBILITY_EXPORT IsotropicBehaviourDSLBase
      : public BehaviourDSLBase<IsotropicBehaviourDSLBase> {
    IsotropicBehaviourDSLBase();

    virtual ~IsotropicBehaviourDSLBase();

   protected:
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
     * @ModellingHypothesis or @ModellingHypotheses keywords.
     */
    virtual bool isModellingHypothesisSupported(
        const Hypothesis) const override;

    virtual void writeBehaviourParserSpecificIncludes(void) override;

    virtual void writeBehaviourParserSpecificTypedefs(void) override;

    virtual void writeBehaviourComputePredictionOperator(
        const Hypothesis) override;

    virtual void writeBehaviourComputeTangentOperator(
        const Hypothesis) override;

    virtual std::string flowRuleVariableModifier(const Hypothesis,
                                                 const std::string&,
                                                 const bool);

    virtual void treatExternalStateVariable(void) override;

    virtual void treatFlowRule(void);

    virtual void treatTheta(void);

    virtual void treatEpsilon(void);

    virtual void treatIterMax(void);

    virtual void endsInputFileProcessing(void) override;

    //! default value
    double theta;

   private:
    friend struct BehaviourDSLBase<IsotropicBehaviourDSLBase>;

  };  // end of class IsotropicBehaviourDSLBase

}  // namespace mfront

#endif /* LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX_ */
