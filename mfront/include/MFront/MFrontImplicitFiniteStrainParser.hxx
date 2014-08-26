/*! 
 * \file  MFrontImplicitFiniteStrainParser.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 févr. 2013
 */

#ifndef _LIB_MFRONT_IMPLICITFINITESTRAINPARSER_H_
#define _LIB_MFRONT_IMPLICITFINITESTRAINPARSER_H_ 

#include"MFront/MFrontImplicitParserBase.hxx"

namespace mfront
{

  /*!
   *
   */
  struct MFrontImplicitFiniteStrainParser
    : public MFrontImplicitParserBase
  {

    MFrontImplicitFiniteStrainParser();

    static std::string 
    getName(void);

    static std::string 
    getDescription(void);

  }; // end of struct MFrontImplicitFiniteStrainParser

} // end of namespace mfront

#endif /* _LIB_MFRONT_IMPLICITFINITESTRAINPARSER_H */

