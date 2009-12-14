/*!
 * \file   VarHandler.hxx
 * \brief  This file declares the VarHandler class
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#ifndef _LIB_MFRONT_VARHANDLER_H_
#define _LIB_MFRONT_VARHANDLER_H_ 

#include<string>
#include<vector>

namespace mfront{

  struct VarHandler
  {
    std::string    type;
    std::string    name;
    unsigned short lineNumber;
    
    VarHandler(const std::string&,const std::string&,const unsigned short);

  }; // end of struct MFrontParserBase<Child>::VarHandler

  typedef std::vector<VarHandler>  VarContainer;

} // end of namespace mfront

#endif /* _LIB_MFRONT_VARHANDLER_H */

