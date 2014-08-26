/*!
 * \file   MFrontIsotropicBehaviourParserBase.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   01 jui 2007
 */

#ifndef _LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX_
#define _LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX_ 

#include<string>

#include"TFEL/Config/TFELConfig.hxx"
#include"MFront/MFrontBehaviourParserBase.hxx"

namespace mfront{

  struct TFEL_VISIBILITY_EXPORT MFrontIsotropicBehaviourParserBase
    : public MFrontBehaviourParserBase<MFrontIsotropicBehaviourParserBase>
  {
    MFrontIsotropicBehaviourParserBase();

    virtual ~MFrontIsotropicBehaviourParserBase();

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
    virtual bool
    isModellingHypothesisSupported(const Hypothesis) const;

    virtual void
    writeBehaviourParserSpecificIncludes(void);

    virtual void
    writeBehaviourParserSpecificTypedefs(void);

    virtual void
    writeBehaviourParserSpecificInitializeMethodPart(const Hypothesis) = 0;

    virtual void
    writeBehaviourParserSpecificMembers(const Hypothesis) = 0;

    virtual void
    writeBehaviourIntegrator(const Hypothesis) = 0;

    virtual void
    writeBehaviourComputePredictionOperator(const Hypothesis);

    virtual std::string
    flowRuleVariableModifier(const Hypothesis,
			     const std::string&,const bool);

    virtual void
    treatExternalStateVariable(void);

    virtual void
    treatFlowRule(void);

    virtual void
    treatTheta(void);

    virtual void
    treatEpsilon(void);

    virtual void
    treatIterMax(void);

    virtual void
    endsInputFileProcessing(void);

    //! default value
    double theta;

  private:

    friend struct MFrontBehaviourParserBase<MFrontIsotropicBehaviourParserBase>;

  }; // end of class MFrontIsotropicBehaviourParserBase

} // end of namespace MFront

#endif /* _LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX */

