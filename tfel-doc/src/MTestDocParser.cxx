/*! 
 * \file  tfel-doc/src/MTestDocParser.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 28 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<iostream>

#include"MTest/MTestParser.hxx"
#include"TFEL/Utilities/MTestDocParser.hxx"

namespace tfel{

  namespace utilities {
  
    MTestDocParser::MTestDocParser(const std::string& f)
      : MTest(),
	file(f)
    {
      using namespace mfront;
      MTestParser parser;
      parser.execute(*this,f);
    } // end of MTestDocParser::MTestDocParser
    
    void
    MTestDocParser::addDocumentation(std::map<std::string,
					      std::vector<TestDocumentation> >& docs)
    {
      using namespace std;
      typedef map<string,vector<TestDocumentation> >::value_type MVType;
      map<string,vector<TestDocumentation> >::iterator pdoc;
      const string c("Mechanical behaviour unary testing");
      pdoc = docs.find(c);
      if(docs.find(c)==docs.end()){
	pdoc = docs.insert(MVType(c,vector<TestDocumentation>())).first;
      }
      pdoc->second.push_back(TestDocumentation());
      TestDocumentation& t = pdoc->second.back();
      t.name    = this->file;
      t.date    = this->date;
      t.author  = this->author;
      // t.src     = ;
      // t.install = ;
      //      t.models.insert(make_pair("MTest",vector<string>(1u,"MTest")));
      // std::map<std::string,
      // 	       std::vector<std::string> > models;
      //      std::map<std::string,std::string>
      t.descriptions.insert(make_pair("english",this->description));
      //      t.keys.insert(make_pair("Mechanics",vector<string>(1u,"Mechanical behaviour testing")));
    } // end of MTestDocParser::execute

    MTestDocParser::~MTestDocParser()
    {} // end of MTestDocParser::~MTestDocParser

  } // end of namespace utilities

} // end of namespace tfel
