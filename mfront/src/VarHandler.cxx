/*!
 * \file   VarHandler.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#include"MFront/VarHandler.hxx"

namespace mfront{

  VarHandler::VarHandler(const std::string& type_,
			 const std::string& name_,
			 const unsigned short lineNumber_)
    : type(type_),
      name(name_),
      lineNumber(lineNumber_)
  {} // end of MFrontParserBase<Child>::VarHandler::VarHandler

} // end of namespace mfront
