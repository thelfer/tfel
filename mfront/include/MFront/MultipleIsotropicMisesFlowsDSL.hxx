/*!
 * \file   mfront/include/MFront/MultipleIsotropicMisesFlowsDSL.hxx
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

#include"MFront/IsotropicBehaviourDSLBase.hxx"

namespace mfront{

  struct MultipleIsotropicMisesFlowsDSL
    : public IsotropicBehaviourDSLBase
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

    MultipleIsotropicMisesFlowsDSL();

    virtual ~MultipleIsotropicMisesFlowsDSL();

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
      FlowHandler() = default;
      FlowHandler(FlowHandler&&) = default;
      FlowHandler(const FlowHandler&) = default;
      FlowHandler& operator=(FlowHandler&&) = default;
      FlowHandler& operator=(const FlowHandler&) = default;
      ~FlowHandler() noexcept;
      std::string flowRule;
      FlowType flow;
      double theta;
      bool hasSpecificTheta;
    }; // end of struct LawHandler
    
    std::vector<FlowHandler> flows;

  }; // end of struct MultipleIsotropicMisesFlowsDSL

} // end of namespace mfront  

#endif /* _LIB_MFRONTMULTIPLEISOTROPICMISESFLOWSPARSER_HXX */

