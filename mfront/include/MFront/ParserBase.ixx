/*!
 * \file   mfront/include/MFront/ParserBase.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   12 mars 2014
 */

#ifndef _LIB_MFRONT_PARSERBASE_IXX_
#define _LIB_MFRONT_PARSERBASE_IXX_ 

#include<sstream>
#include<stdexcept>

namespace mfront{

  template<typename T>
  std::pair<bool,T>
  ParserBase::readInitialisationValue(const std::string& n,
				      const bool b)
  {
    using namespace std;
    pair<bool,T> value;
    this->checkNotEndOfFile("ParserBase::readInitialisationValue",
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
      this->readSpecifiedToken("ParserBase::readInitialisationValue","=");
      this->checkNotEndOfFile("ParserBase::readInitialisationValue",
			      "Expected to read the value of variable '"+n+"'");
      istringstream tmp(this->current->value);
      tmp >> value.second;
      if(!tmp&&(!tmp.eof())){
	this->throwRuntimeError("ParserBase::readInitialisationValue",
				"Could not read value of variable '"+n+"'.");
      }
      ++(this->current);
      if(!ci.empty()){
	this->readSpecifiedToken("ParserBase::readInitialisationValue",ci);
      }
      value.first=true;
    } else {
      if(b){
	string msg("ParserBase::readInitialisationValue : ");
	msg += "expected '=', '{', '(' token, read '"+current->value+"'";
	throw(runtime_error(msg));
      }
      value.first=false;
    }
    return value;
  } // end of ParserBase::readInitialisationValue

} // end of namespace mfront  

#endif /* _LIB_MFRONT_PARSERBASE_IXX */
