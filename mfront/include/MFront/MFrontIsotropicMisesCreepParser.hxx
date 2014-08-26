/*!
 * \file   MFrontIsotropicMisesCreepParser.hxx
 * \brief  This file declares the MFrontIsotropicMisesCreepParser class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#ifndef _LIB_MFRONTISOTROPICMISESCREEPPARSER_H_
#define _LIB_MFRONTISOTROPICMISESCREEPPARSER_H_ 

#include<string>
#include"MFront/MFrontIsotropicBehaviourParserBase.hxx"

namespace mfront{

  struct MFrontIsotropicMisesCreepParser
    : public MFrontIsotropicBehaviourParserBase
  {

    static std::string 
    getName(void);

    static std::string
    getDescription(void);

    virtual void
    endsInputFileProcessing(void);

    virtual void
    writeBehaviourParserSpecificInitializeMethodPart(const Hypothesis);

    virtual void
    writeBehaviourParserSpecificMembers(const Hypothesis);

    virtual void
    writeBehaviourIntegrator(const Hypothesis);

    virtual void
    writeBehaviourComputeTangentOperator(const Hypothesis);

    MFrontIsotropicMisesCreepParser();

    virtual ~MFrontIsotropicMisesCreepParser();
  };

} // end of namespace mfront  

#endif /* _LIB_MFRONTISOTROPICMISESCREEPPARSER_H */

