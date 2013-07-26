/*!
 * \file   MFrontDefaultCZMParser.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   08 nov 2006
 */

#ifndef _LIB_MFRONT_MFRONTDEFAULTCZMPARSER_HXX_
#define _LIB_MFRONT_MFRONTDEFAULTCZMPARSER_HXX_ 

#include<string>
#include"MFront/MFrontDefaultParserBase.hxx"

namespace mfront{

  struct MFrontDefaultCZMParser
    : public MFrontDefaultParserBase
  {

    static std::string 
    getName(void);

    static std::string 
    getDescription(void);

    MFrontDefaultCZMParser();

    ~MFrontDefaultCZMParser();

  protected:

    virtual void
    writeBehaviourParserSpecificIncludes(void);

  }; // end of struct MFrontDefaultCZMParser

} // end of namespace mfront  

#endif /* _LIB_MFRONT_MFRONTDEFAULTCZMPARSER_HXX_ */

