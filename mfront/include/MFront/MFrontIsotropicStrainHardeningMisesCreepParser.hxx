/*!
 * \File   MFrontIsotropicStrainHardeningMisesCreepParser.hxx
 * \brief  This file declares the MFrontIsotropicStrainHardeningMisesCreepParser class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#ifndef _LIB_MFRONTISOTROPICSTRAINHARDENINGMISESCREEPPARSER_H_
#define _LIB_MFRONTISOTROPICSTRAINHARDENINGMISESCREEPPARSER_H_ 

#include<string>
#include"MFront/MFrontIsotropicBehaviourParserBase.hxx"

namespace mfront{

  struct MFrontIsotropicStrainHardeningMisesCreepParser
    : public MFrontIsotropicBehaviourParserBase
  {

    static std::string 
    getName(void);

    static std::string
    getDescription(void);

    void writeBehaviourParserSpecificConstructorPart(void);

    void writeBehaviourParserSpecificMembers(void);

    void writeBehaviourIntegrator(void);

    void writeBehaviourComputeTangentOperator(void);

    MFrontIsotropicStrainHardeningMisesCreepParser();

    ~MFrontIsotropicStrainHardeningMisesCreepParser();
  };

} // end of namespace mfront  

#endif /* _LIB_MFRONTISOTROPICSTRAINHARDENINGMISESCREEPPARSER_H */

