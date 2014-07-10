/*!
 * \file   VarHandler.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#include<stdexcept>

#include"MFront/VarHandler.hxx"

namespace mfront{

  VarHandler::VarHandler(const std::string& type_,
			 const std::string& name_,
			 const unsigned short arraySize_,
			 const unsigned short lineNumber_)
    : type(type_),
      name(name_),
      arraySize(arraySize_),
      lineNumber(lineNumber_)
  {
    using namespace std;
    if(this->arraySize==0){
      string msg("MFrontParserBase<Child>::VarHandler::VarHandler : ");
      msg += "invalid array size";
      throw(runtime_error(msg));
    }
  } // end of MFrontParserBase<Child>::VarHandler::VarHandler

} // end of namespace mfront
