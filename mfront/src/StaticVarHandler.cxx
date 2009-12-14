/*!
 * \file   StaticVarHandler.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   19 Jan 2007
 */

#include"StaticVarHandler.hxx"

namespace mfront{

  StaticVarHandler::StaticVarHandler(const std::string& type_,
				     const std::string& name_,
				     const unsigned short line_,
				     const long double value_)
    : VarHandler(type_,name_,line_),
      value(value_)
  {}

} // end of namespace mfront
