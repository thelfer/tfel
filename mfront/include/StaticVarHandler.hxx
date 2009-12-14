/*!
 * \file   StaticVarHandler.hxx
 * \brief  This file declares the StaticVarHandler class
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#ifndef _LIB_MFRONT_STATICVARHANDLER_H_
#define _LIB_MFRONT_STATICVARHANDLER_H_ 

#include<string>
#include<vector>

#include"VarHandler.hxx"

namespace mfront{

  struct StaticVarHandler
    : public VarHandler
  {
    long double value;
        
    StaticVarHandler(const std::string&,const std::string&,
		     const unsigned short,const long double);

  }; // end of struct MFrontParserBase<Child>::StaticVarHandler

  typedef std::vector<StaticVarHandler>  StaticVarContainer;

} // end of namespace mfront

#endif /* _LIB_MFRONT_STATICVARHANDLER_H */

