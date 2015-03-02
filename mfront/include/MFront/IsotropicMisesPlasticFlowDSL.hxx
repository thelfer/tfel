/*!
 * \file   mfront/include/MFront/IsotropicMisesPlasticFlowDSL.hxx
 * \brief  This file declares the IsotropicMisesPlasticFlowDSL class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTISOTROPICPLASTICFLOWPARSER_H_
#define LIB_MFRONTISOTROPICPLASTICFLOWPARSER_H_ 

#include<string>
#include"MFront/IsotropicBehaviourDSLBase.hxx"

namespace mfront{

  struct IsotropicMisesPlasticFlowDSL
    : public IsotropicBehaviourDSLBase
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

    IsotropicMisesPlasticFlowDSL();

    virtual ~IsotropicMisesPlasticFlowDSL();
  };

} // end of namespace mfront  

#endif /* LIB_MFRONTISOTROPICPLASTICFLOWPARSER_H_ */

