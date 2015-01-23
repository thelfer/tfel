/*! 
 * \file  tfel-doc/src/ParserBase.cxx
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

#include<stdexcept>
#include"TFEL/Utilities/ParserBase.hxx"

namespace tfel
{

  namespace utilities
  {

    void
    ParserBase::checkNotEndOfFile(const_iterator p)
    {
      using namespace std;
      if(p==this->end()){
	string msg("ParserBase::checkNotEndOfFile : ");
	msg += "unexpected end of file";
	throw(runtime_error(msg));
      }
    } // end of ParserBase::checkNotEndOfFile

    void
    ParserBase::readSpecifiedToken(const std::string& v,
				   const_iterator& p)
    {
      using namespace std;
      this->checkNotEndOfFile(p);
      if(p->value!=v){
	string msg("ParserBase::readSpecifiedToken : ");
	msg += "expected token '"+v+"', read '"+p->value+"'";
	throw(runtime_error(msg));
      }
      ++p;
    } // end of ParserBase::readSpecifiedToken
      
    void
    ParserBase::readString(std::string& s,
			   const_iterator& p)
    {
      using namespace std;
      using namespace tfel::utilities;
      this->checkNotEndOfFile(p);
      if(p->flag!=Token::String){
	string msg("TestParser::readString : ");
	msg += "expected to read a string";
	throw(runtime_error(msg));
      }
      s = p->value.substr(1,p->value.size()-2);
      ++p;      
    } // end of ParserBase::readString
      
    std::string
    ParserBase::readString(const_iterator& p)
    {
      using namespace std;
      std::string r;
      this->readString(r,p);
      return r;
    } // end of ParserBase::readString

    ParserBase::~ParserBase() noexcept
    {} // end of ParserBase::~ParserBase

  } // end of namespace utilities

} // end of namespace tfel

