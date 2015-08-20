/*! 
 * \file  tfel-doc/src/TestDocParser.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>
#include<iostream>
#include<stdexcept>
#include<algorithm>

#include"TFEL/Utilities/Global.hxx"
#include"TFEL/Utilities/TestDocParser.hxx"
#include"TFEL/Utilities/TerminalColors.hxx"

namespace tfel{

  namespace utilities {

    TestDocParser::TestDocParser(const std::string& f)
    {
      this->openFile(f);
    } // end of TestDocParser::TestDocParser

    void
    TestDocParser::addDocumentation(std::map<std::string,
					     std::vector<TestDocumentation> >& tests)
    {
      using namespace std;
      using namespace tfel::utilities;
      this->stripComments();
      auto p  = this->begin();
      auto pe = this->end();
      try{
	while(p!=pe){
	  if(p->flag!=Token::String){
	    string msg("TestDocParser::addDocumentation : ");
	    msg += "expected to read a string";
	    throw(runtime_error(msg));
	  }
	  TestDocumentation t;
	  this->readString(t.name,p);
	  for(const auto& td : tests){
	    for(auto p3=td.second.begin();p3!=td.second.end();++p3){
	      if(t.name==p3->name){
		string msg("TestDocParser::addDocumentation : ");
		msg += "test '"+t.name+"' already described";
		throw(runtime_error(msg));	  
	      }
	    }
	  }
	  this->checkNotEndOfFile(p);
	  this->readSpecifiedToken("{",p);
	  string c;
	  this->treatTest(t,c,p);
	  this->checkNotEndOfFile(p);
	  this->readSpecifiedToken("}",p);
	  if(c.empty()){
	    string msg("no category defined for test '"+t.name+"'");
	    throw(runtime_error(msg));
	  }
	  tests[c].push_back(t);
	}
      } catch(exception& e){
	ostringstream msg;
	msg << "TestDocParser::addDocumentation : "
	    << e.what();
	if(p!=this->end()){
	  msg << "\nError at line " << p->line;
	}
	throw(runtime_error(msg.str()));
      }
    }

    void
    TestDocParser::treatTest(TestDocumentation& t,
			     std::string& c,
			     const_iterator& p)
    {
      using namespace std;
      using namespace tfel::utilities;
      this->checkNotEndOfFile(p);
      while(p->value!="}"){
	string key = p->value;
	++p;
	this->readSpecifiedToken(":",p);
	if(key=="author"){
	  this->getStringValue(t.author,t.name,key,p);
	} else if(key=="date"){
	  this->getStringValue(t.date,t.name,key,p);
	} else if((key=="source")||(key=="src")){
	  this->getStringValue(t.src,t.name,"source",p);
	} else if((key=="install")||(key=="prefix")){
	  this->getStringValue(t.install,t.name,key,p);
	} else if(key=="category"){
	  auto& categories = getCategoriesMap("english");
	  map<string,string>::const_iterator pc;
	  this->getStringValue(c,t.name,key,p);
	  pc = categories.find(c);
	  if(pc==categories.end()){
	    cerr << TerminalColors::Red;
	    cerr << "undefined category '" << c 
		 << "'" << endl;
	    cerr << TerminalColors::Reset;
	  }
	} else if(key=="description"){
	  this->treatDescription(t.descriptions,t.name,p);
	} else if(key=="models"){
	  this->treatIndex(t.models,t.name,p,false);
	} else if(key=="keys"){
	  this->treatIndex(t.keys,t.name,p,true);
	} else {
	  string msg("TestDocParser::treatTest : ");
	  msg += "unsupported keyword '"+key+"'";
	  throw(runtime_error(msg));
	}
	this->readSpecifiedToken(";",p);
	this->checkNotEndOfFile(p);
      }
    }

    void
    TestDocParser::getStringValue(std::string& s,
				  const std::string& name,
				  const std::string& v,
				  const_iterator& p)
    {
      using namespace std;
      this->checkNotEndOfFile(p);
      if(!s.empty()){
	string msg("TestDocParser::getStringValue : ");
	msg += v+" already defined for test '"+name+"'";
	throw(runtime_error(msg));
      }
      this->readString(s,p);
    }

    void
    TestDocParser::treatDescription(std::map<std::string,std::string>& dm,
				    std::string& n,
				    const_iterator& p)
    {
      using namespace std;
      string l;
      this->checkNotEndOfFile(p);
      if(p->value!="{"){
	l = this->readString(p);
      } else {
	l = "english";
      }
      if(dm.find(l)!=dm.end()){
	string msg("TestDocParser::treatDescription : ");
	msg += "description already defined for language '"+l;
	msg += " 'for test '"+n+"'";
	throw(runtime_error(msg));
      }
      auto& d = dm[l];
      this->readSpecifiedToken("{",p);
      this->checkNotEndOfFile(p);
      while(p->value!="}"){
	if(!d.empty()){
	  d += '\n';
	}
	d += this->readString(p);
	this->checkNotEndOfFile(p);
      }
      this->readSpecifiedToken("}",p);
      this->checkNotEndOfFile(p);
    } // end of treatDescription

    void
    TestDocParser::treatIndex(std::map<std::string,std::vector<std::string> >& i,
			      const std::string& n,
			      const_iterator& p,
			      const bool b)
    {
      using namespace std;
      using namespace tfel::utilities;
      const auto& k = getKeysMap("english");
      this->checkNotEndOfFile(p);
      this->readSpecifiedToken("{",p);
      this->checkNotEndOfFile(p);
      while(p->value!="}"){
	auto key1 = string{};
	this->readString(key1,p);
	if(b){
	  if(!key1.empty()){
	    if(key1[0]!='!'){
	      if(k.find(key1)==k.end()){
		cerr << TerminalColors::Red;
		cerr << "undefined key '" << key1 
		     << "'" << endl;
		cerr << TerminalColors::Reset;
	      }
	    }
	  }
	}
	this->checkNotEndOfFile(p);
	if(p->value=="@"){
	  ++p;
	  this->checkNotEndOfFile(p);
	  auto key2 = string{};
	  this->readString(key2,p);
	  if(b){
	    if(!key2.empty()){
	      if(key2[0]!='!'){
		if(k.find(key2)==k.end()){
		  cerr << TerminalColors::Red;
		  cerr << "undefined key '" << key2 
		       << "'" << endl;
		  cerr << TerminalColors::Reset;
		}
	      }
	    }
	  }
	  this->checkNotEndOfFile(p);
	  auto p2 = i.find(key2);
	  if(p2==i.end()){
	    p2 = i.insert(make_pair(key2,vector<string>())).first;
	  }
	  auto& keys = p2->second;
	  if(find(keys.begin(),keys.end(),key1)!=keys.end()){
	    string msg("TestDocParser::treatIndex : ");
	    msg += "key '"+key1+"' already defined for entry '"+key2+"' for test '"+n+"'";
	    throw(runtime_error(msg));
	  }
	  keys.push_back(key1);
	} else {
	  auto p2 = i.find(key1);
	  if(p2==i.end()){	
	    i.insert(make_pair(key1,vector<string>()));
	  }
	}
	if(p->value!="}"){
	  this->readSpecifiedToken(",",p);
	  this->checkNotEndOfFile(p);
	  if(p->value==","){
	    string msg("TestDocParser::treatIndex : ");
	    msg += "unexpected comma";
	    throw(runtime_error(msg));
	  }
	}
      }
      this->checkNotEndOfFile(p);
      this->readSpecifiedToken("}",p);
    }

    TestDocParser::~TestDocParser() noexcept
    {} // end of MTestDocParser::~MTestDocParser

  } // end of namespace utilities

} // end of namespace tfel
