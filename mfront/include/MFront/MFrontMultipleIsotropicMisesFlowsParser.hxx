/*!
 * \file   MFrontMultipleIsotropicMisesFlowsParser.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   31 jan 2008
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

    void writeBehaviourParserSpecificConstructorPart(void);

    void writeBehaviourParserSpecificMembers(void);

    void writeBehaviourParserSpecificIncludes(void);

    void writeBehaviourIntegrator(void);

    MFrontMultipleIsotropicMisesFlowsParser();

    ~MFrontMultipleIsotropicMisesFlowsParser();

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

