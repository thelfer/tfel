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

  VariableDescription::VariableDescription(const std::string& t,
					   const std::string& n,
					   const unsigned short s,
					   const unsigned short l)
    : type(t),
      name(n),
      arraySize(s),
      lineNumber(l)
  {
    using namespace std;
    if(this->arraySize==0){
      string msg("VariableDescription::VariableDescription : ");
      msg += "invalid array size";
      throw(runtime_error(msg));
    }
  } // end of MFrontParserBase<Child>::VariableDescription::VariableDescription

} // end of namespace mfront
