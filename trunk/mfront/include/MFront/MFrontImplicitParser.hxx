/*! 
 * \file  MFrontImplicitParser.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 févr. 2013
 */

#ifndef _LIB_MFRONT_MFRONTIMPLICITPARSER_H_
#define _LIB_MFRONT_MFRONTIMPLICITPARSER_H_ 

#include"MFront/MFrontImplicitParserBase.hxx"

namespace mfront
{

  /*!
   *
   */
  struct MFrontImplicitParser
    : public MFrontImplicitParserBase
  {

    MFrontImplicitParser();

    static std::string 
    getName(void);

    static std::string 
    getDescription(void);

  }; // end of struct MFrontImplicitParser

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTIMPLICITPARSER_H */

