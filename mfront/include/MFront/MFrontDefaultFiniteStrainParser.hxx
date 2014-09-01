/*!
 * \file   mfront/include/MFront/MFrontDefaultFiniteStrainParser.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   08 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_MFRONTDEFAULTFINITESTRAINPARSER_HXX_
#define _LIB_MFRONT_MFRONTDEFAULTFINITESTRAINPARSER_HXX_ 

#include<string>
#include"MFront/MFrontDefaultParserBase.hxx"

namespace mfront{

  struct MFrontDefaultFiniteStrainParser
    : public MFrontDefaultParserBase
  {

    static std::string 
    getName(void);

    static std::string 
    getDescription(void);

    MFrontDefaultFiniteStrainParser();

    ~MFrontDefaultFiniteStrainParser();

  protected:

    virtual void
    writeBehaviourParserSpecificIncludes(void);

  }; // end of struct MFrontDefaultFiniteStrainParser

} // end of namespace mfront  

#endif /* _LIB_MFRONT_MFRONTDEFAULTFINITESTRAINPARSER_HXX_ */

