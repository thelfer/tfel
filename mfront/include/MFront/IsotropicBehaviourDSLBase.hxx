/*!
 * \file   mfront/include/MFront/IsotropicBehaviourDSLBase.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   01 jui 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX_
#define LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX_ 

#include<string>

#include"MFront/MFrontConfig.hxx"
#include"MFront/BehaviourDSLBase.hxx"

namespace mfront{

  struct MFRONT_VISIBILITY_EXPORT IsotropicBehaviourDSLBase
    : public BehaviourDSLBase<IsotropicBehaviourDSLBase>
  {
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

    virtual void
    writeBehaviourComputeTangentOperator(const Hypothesis);

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

    friend struct BehaviourDSLBase<IsotropicBehaviourDSLBase>;

  }; // end of class IsotropicBehaviourDSLBase

} // end of namespace MFront

#endif /* LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX_ */

