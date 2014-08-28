/*! 
 * \file  tfel-doc/src/ConfigParser.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
 */

#include<sstream>
#include<stdexcept>

#include "TFEL/Utilities/ConfigParser.hxx"

namespace tfel{

  namespace utilities {

static void
insert(std::map<std::string,std::map<std::string,std::string> >& m,
       const std::string& n1)
{
  using namespace std;
  typedef map<string,string>::value_type MVType;
  if(!m["english"].insert(MVType(n1,n1)).second){
    string msg("insert : ");
    msg += "category '"+n1+"' already declared";
    throw(runtime_error(msg));
  }
} // end of insert

static void
insert(std::map<std::string,std::map<std::string,std::string> >& m,
       const std::string& n1,
       const std::string& n2,
       const std::string& l)
{
  using namespace std;
  typedef map<string,string>::value_type MVType;
  if(m["english"].find(n1)==m["english"].end()){
    string msg("insert : ");
    msg += "category '"+n1+"' undeclared";
    throw(runtime_error(msg));
  }
  if(!m[l].insert(MVType(n1,n2)).second){
    string msg("insert : ");
    msg += "category '"+n1+"' already declared";
    throw(runtime_error(msg));
  }
} // end of insert

    void
    ConfigParser::execute(std::map<std::string,std::map<std::string,std::string> >& m,
			  const std::string& f)
    {
      using namespace std;
      using namespace tfel::utilities;
      const_iterator p;
      const_iterator pe;
      this->openFile(f);
      try{
	this->stripComments();
	p  = this->begin();
	pe = this->end();
	this->checkNotEndOfFile(p);
	while(p!=pe){
	  string key;
	  this->readString(key,p);
	  insert(m,key);
	  this->checkNotEndOfFile(p);
	  this->readSpecifiedToken("{",p);
	  this->checkNotEndOfFile(p);
	  while(p->value!="}"){
	    string l;
	    string v;
	    this->readString(l,p);
	    this->checkNotEndOfFile(p);
	    this->readSpecifiedToken(":",p);
	    this->readString(v,p);
	    this->checkNotEndOfFile(p);
	    this->readSpecifiedToken(";",p);
	    this->checkNotEndOfFile(p);
	    if(p->value!="}"){
	      this->readSpecifiedToken(",",p);
	      if(p->value!="}"){
		string msg("ConfigParser::execute");
		throw(runtime_error(msg));
	      }
	    }
	    insert(m,key,v,l);
	  }
	  this->readSpecifiedToken("}",p);
	  this->checkNotEndOfFile(p);
	  this->readSpecifiedToken(";",p);
	}
      } catch(exception& e){
	ostringstream msg;
	msg << "ConfigParser::execute : "
	    << e.what();
	if(p!=this->end()){
	  msg << "\nError at line " << p->line
	      << " of file '" << f << "'";
	}
	throw(runtime_error(msg.str()));
      }
    } // end of ConfigParser::execute

  } // end of namespace utilities

} // end of namespace tfel

