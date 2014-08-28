/*!
 * \file   mfront/include/MFront/MFrontDefaultParser.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   08 nov 2006
 */

#ifndef _LIB_MFRONTPARSER_HXX_
#define _LIB_MFRONTPARSER_HXX_ 

#include<string>
#include"MFront/MFrontDefaultParserBase.hxx"

namespace mfront{

  struct MFrontDefaultParser
    : public MFrontDefaultParserBase
  {

    static std::string 
    getName(void);

    static std::string 
    getDescription(void);

    MFrontDefaultParser();

    ~MFrontDefaultParser();

  }; // end of struct MFrontDefaultParser

} // end of namespace mfront  

#endif /* _LIB_MFRONTPARSER_HXX */

