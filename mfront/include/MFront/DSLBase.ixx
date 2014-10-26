/*!
 * \file   mfront/include/MFront/DSLBase.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   12 mars 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_PARSERBASE_IXX_
#define _LIB_MFRONT_PARSERBASE_IXX_ 

#include<sstream>
#include<stdexcept>

namespace mfront{

  template<typename T>
  std::pair<bool,T>
  DSLBase::readInitialisationValue(const std::string& n,
				      const bool b)
  {
    using namespace std;
    pair<bool,T> value;
    this->checkNotEndOfFile("DSLBase::readInitialisationValue",
			    "unexpected end of file");
    if((this->current->value=="=")||
       (this->current->value=="{")||
       (this->current->value=="(")){
      string ci; // closing initializer
      if(this->current->value=="{"){
	ci="}";
      }
      if(this->current->value=="("){
	ci=")";
      }
      this->readSpecifiedToken("DSLBase::readInitialisationValue","=");
      this->checkNotEndOfFile("DSLBase::readInitialisationValue",
			      "Expected to read the value of variable '"+n+"'");
      istringstream tmp(this->current->value);
      tmp >> value.second;
      if(!tmp&&(!tmp.eof())){
	this->throwRuntimeError("DSLBase::readInitialisationValue",
				"Could not read value of variable '"+n+"'.");
      }
      ++(this->current);
      if(!ci.empty()){
	this->readSpecifiedToken("DSLBase::readInitialisationValue",ci);
      }
      value.first=true;
    } else {
      if(b){
	string msg("DSLBase::readInitialisationValue : ");
	msg += "expected '=', '{', '(' token, read '"+current->value+"'";
	throw(runtime_error(msg));
      }
      value.first=false;
    }
    return value;
  } // end of DSLBase::readInitialisationValue

} // end of namespace mfront  

#endif /* _LIB_MFRONT_PARSERBASE_IXX */
