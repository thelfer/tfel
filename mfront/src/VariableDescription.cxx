/*!
 * \file   VariableDescription.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#include<stdexcept>

#include"MFront/VariableDescription.hxx"

namespace mfront{

  VariableDescription::VariableDescription(const std::string& type_,
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
      string msg("VariableDescription::VariableDescription : ");
      msg += "invalid array size";
      throw(runtime_error(msg));
    }
  } // end of MFrontParserBase<Child>::VariableDescription::VariableDescription

} // end of namespace mfront
