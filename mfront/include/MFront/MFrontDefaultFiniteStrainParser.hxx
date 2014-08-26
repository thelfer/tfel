/*!
 * \file   MFrontDefaultFiniteStrainParser.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   08 nov 2006
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

