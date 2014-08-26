/*!
 * \file   MFrontIsotropicMisesPlasticFlowParser.hxx
 * \brief  This file declares the MFrontIsotropicMisesPlasticFlowParser class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#ifndef _LIB_MFRONTISOTROPICPLASTICFLOWPARSER_H_
#define _LIB_MFRONTISOTROPICPLASTICFLOWPARSER_H_ 

#include<string>
#include"MFront/MFrontIsotropicBehaviourParserBase.hxx"

namespace mfront{

  struct MFrontIsotropicMisesPlasticFlowParser
    : public MFrontIsotropicBehaviourParserBase
  {

    static std::string 
    getName(void);

    static std::string
    getDescription(void);

    void writeBehaviourParserSpecificInitializeMethodPart(void);

    void writeBehaviourParserSpecificMembers(void);

    void writeBehaviourIntegrator(void);

    void writeBehaviourComputeTangentOperator(void);

    MFrontIsotropicMisesPlasticFlowParser();

    ~MFrontIsotropicMisesPlasticFlowParser();
  };

} // end of namespace mfront  

#endif /* _LIB_MFRONTISOTROPICPLASTICFLOWPARSER_H */

