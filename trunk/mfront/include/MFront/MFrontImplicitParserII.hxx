/*! 
 * \file  MFrontImplicitParserII.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 févr. 2013
 */

#ifndef _LIB_MFRONT_MFRONTIMPLICITPARSERII_H_
#define _LIB_MFRONT_MFRONTIMPLICITPARSERII_H_ 

#include"MFront/MFrontImplicitParserBase.hxx"

namespace mfront
{

  /*!
   *
   */
  struct MFrontImplicitParserII
    : public MFrontImplicitParserBase
  {

    MFrontImplicitParserII();

    static std::string 
    getName(void);

    static std::string 
    getDescription(void);

  }; // end of struct MFrontImplicitParserII

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTIMPLICITPARSERII_H */

