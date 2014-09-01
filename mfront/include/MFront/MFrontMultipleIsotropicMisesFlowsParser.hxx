/*!
 * \file   mfront/include/MFront/MFrontMultipleIsotropicMisesFlowsParser.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   31 jan 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONTMULTIPLEISOTROPICMISESFLOWSPARSER_HXX_
#define _LIB_MFRONTMULTIPLEISOTROPICMISESFLOWSPARSER_HXX_ 

#include<vector>
#include<string>

#include"MFront/MFrontIsotropicBehaviourParserBase.hxx"

namespace mfront{

  struct MFrontMultipleIsotropicMisesFlowsParser
    : public MFrontIsotropicBehaviourParserBase
  {

    static std::string 
    getName(void);

    static std::string
    getDescription(void);

    virtual void
    writeBehaviourParserSpecificInitializeMethodPart(const Hypothesis);

    virtual void
    writeBehaviourParserSpecificMembers(const Hypothesis);

    virtual void
    writeBehaviourParserSpecificIncludes(void);

    virtual void
    writeBehaviourIntegrator(const Hypothesis);

    virtual void
    writeBehaviourComputeTangentOperator(const Hypothesis);

    MFrontMultipleIsotropicMisesFlowsParser();

    virtual ~MFrontMultipleIsotropicMisesFlowsParser();

  private:

    void
    treatFlowRule(void);

    struct FlowHandler
    {
      enum FlowType{
	PlasticFlow,
	CreepFlow,
	StrainHardeningCreepFlow
      }; // end of enum FlowType
      FlowType flow;
      std::string flowRule;
      double theta;
      bool hasSpecificTheta;
    }; // end of struct LawHandler
    
    std::vector<FlowHandler> flows;

  }; // end of struct MFrontMultipleIsotropicMisesFlowsParser

} // end of namespace mfront  

#endif /* _LIB_MFRONTMULTIPLEISOTROPICMISESFLOWSPARSER_HXX */

