/*!
 * \file   InterfaceBase.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   07 nov 2008
 */

#include<sstream>
#include<stdexcept>
#include"InterfaceBase.hxx"

namespace mfront{

  std::string
  InterfaceBase::toString(const unsigned short src)
  {
    std::ostringstream os;
    os << src;
    return os.str();
  } // end of InterfaceBase::toString

  void
  InterfaceBase::checkNotEndOfFile(InterfaceBase::TokensContainer::const_iterator& current,
				   const InterfaceBase::TokensContainer::const_iterator end,
				   const std::string& method,
				   const std::string& error)
  {
    using namespace std;
    if(current==end){
      --(current);
      string msg(method+" : ");
      msg += "unexpected end of file.";
      if(!error.empty()){
	msg += "\n"+error;
      }
      msg += "\nError at line " + InterfaceBase::toString(current->line);
      throw(runtime_error(msg));
    }
  } // end of InterfaceBase::checkNotEndOfFile
  
  void
  InterfaceBase::readSpecifiedToken(InterfaceBase::TokensContainer::const_iterator& current,
				    const InterfaceBase::TokensContainer::const_iterator end,
				    const std::string& method,
				    const std::string& token)
  {
    using namespace std;
    InterfaceBase::checkNotEndOfFile(current,end,
				     method,"Expected '"+token+"'.");
    if(current->value!=token){
      string msg(method+" : ");
      msg += "expected '"+token+"', read ";
      msg += current->value;
      msg += ".\n";
      msg += "Error at line : ";
      msg += InterfaceBase::toString(current->line);
      throw(runtime_error(msg));
    }
    ++(current);
  } // end of InterfaceBase::readSpecifiedToken
  
  void
  InterfaceBase::throwRuntimeError(InterfaceBase::TokensContainer::const_iterator& current,
				   const InterfaceBase::TokensContainer::const_iterator end,
				   const std::string& method,
				   const std::string& m)
  {
    using namespace std;
    if(current==end){
      --(current);
    }
    string msg(method);
    if(!m.empty()){
      msg +=" : " + m;
    }
    msg += "\nError at line " + InterfaceBase::toString(current->line);
    throw(runtime_error(msg));
  } // end of InterfaceBase::throwRuntimeError
  
  std::vector<std::string>
  InterfaceBase::readArrayOfString(InterfaceBase::TokensContainer::const_iterator& current,
				   const InterfaceBase::TokensContainer::const_iterator end,
				   const std::string& method)
  {
    using namespace std;
    using namespace tfel::utilities;
    vector<string> res;
    InterfaceBase::readSpecifiedToken(current,end,method,"{");
    InterfaceBase::checkNotEndOfFile(current,end,method,"Expected '}'");
    while(current->value!="}"){
      if(current->flag!=Token::String){
	InterfaceBase::throwRuntimeError(current,end,method,"Expected a string");
      }
      res.push_back(current->value.substr(1,current->value.size()-2));
      ++(current);
      InterfaceBase::checkNotEndOfFile(current,end,method,"Expected '}'");
      if(!((current->value=="}")||
	   (current->value==","))){
	InterfaceBase::throwRuntimeError(current,end,method,
					 "Expected ',' or '}', read '"+current->value+"'");
      }
      if(current->value==","){
	++(current);
	InterfaceBase::checkNotEndOfFile(current,end,method,"Expected '}'");
	if(current->value=="}"){
	  InterfaceBase::throwRuntimeError(current,end,method,"Expected a string");
	}
      }
    }
    ++(current);
    return res;
  } // end of InterfaceBase::readArrayOfString
  
  std::vector<std::string>
  InterfaceBase::readStringOrArrayOfString(InterfaceBase::TokensContainer::const_iterator& current,
					   const InterfaceBase::TokensContainer::const_iterator end,
					   const std::string& method)
  {
    using namespace std;
    using namespace tfel::utilities;
    vector<string> res;
    InterfaceBase::checkNotEndOfFile(current,end,
				     method,"Expected a string or '{'");
    if(current->value=="{"){
      return InterfaceBase::readArrayOfString(current,end,method);
    }
    if(current->flag!=Token::String){
      InterfaceBase::throwRuntimeError(current,end,
				       method,"Expected a string");
    }
    res.push_back(current->value.substr(1,current->value.size()-2));
    ++(current);
    return res;
  } // end of InterfaceBase::readStringOrArrayOfString

} // end of namespace mfront
