/*! 
 * \file  MTestDocParser.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 28 avril 2013
 */

#include<iostream>

#include"TFEL/Utilities/MTestDocParser.hxx"

namespace tfel{

  namespace utilities {
  
    MTestDocParser::MTestDocParser(const std::string& f)
      : MTest(f)
    {} // end of MTestDocParser::MTestDocParser
    
    void
    MTestDocParser::addDocumentation(std::map<std::string,
					      std::vector<TestDocumentation> >& docs)
    {
      using namespace std;
      typedef map<string,vector<TestDocumentation> >::value_type MVType;
      const string c("Mechanical behaviour unary testing");
      if(docs.find(c)==docs.end()){
	docs.insert(MVType(c,vector<TestDocumentation>()));
      }
      docs.at(c).push_back(TestDocumentation());
      TestDocumentation& t = docs.at(c).back();
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
