/*!
 * \file   mfront/include/MFront/MFrontIsotropicStrainHardeningMisesCreepParser.hxx
 * \brief  This file declares the MFrontIsotropicStrainHardeningMisesCreepParser class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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

    MFrontIsotropicStrainHardeningMisesCreepParser();

    virtual ~MFrontIsotropicStrainHardeningMisesCreepParser();
  };

} // end of namespace mfront  

#endif /* _LIB_MFRONTISOTROPICSTRAINHARDENINGMISESCREEPPARSER_H */

