/*!
 * \file   ComputedVarHandler.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   19 Jan 2007
 */

#include"MFront/ComputedVarHandler.hxx"

namespace mfront{

  ComputedVarHandler::ComputedVarHandler(const std::string& type_,
					 const std::string& name_,
					 const unsigned short line_,
					 const std::string& description_)
    : VarHandler(type_,name_,line_),
      description(description_)
  {}

} // end of namespace mfront
