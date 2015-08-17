/*!
 * \file   MFrontUtilities.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   15 août 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<ostream>
#include<cstring>
#include<algorithm>
#include"MFront/MFrontUtilities.hxx"

namespace mfront{

  void
  insert_if(std::vector<std::string>& d,
	    const std::string& v){
    if(v.empty()){
      return;
    }
    if(std::find(d.begin(),d.end(),v)==d.end()){
      d.push_back(v);
    }
  }

  void
  insert_if(std::vector<std::string>& d,
	    const char* const v){
    if(v==nullptr){
      return;
    }
    if(strlen(v)==0){
      return;
    }
    if(std::find(d.begin(),d.end(),v)==d.end()){
      d.push_back(v);
    }
  }
  
  void
  write(std::ostream& os,
	const std::vector<std::string>& v,
	const std::string& id){
    if(v.empty()){
      return;
    }
    os << id << " : {\n";
    for(auto p = v.begin();p!=v.end();){
      os << "\"" << *p << "\"";
      if(++p!=v.end()){
	os << ",\n";
      } else {
	os << "\n";
      }
    }
    os << "};\n";
  } // end of write

  template<>
  double
  read(tfel::utilities::CxxTokenizer::const_iterator& p,
       const tfel::utilities::CxxTokenizer::const_iterator pe)
  {
    using tfel::utilities::CxxTokenizer;
    auto c = p;
    auto r = CxxTokenizer::readDouble(c,pe);
    p=c;
    return r;
  } // end of read

  template<>
  std::string
  read(tfel::utilities::CxxTokenizer::const_iterator& p,
       const tfel::utilities::CxxTokenizer::const_iterator pe)
  {
    using tfel::utilities::CxxTokenizer;
    auto c = p;
    auto r = CxxTokenizer::readString(c,pe);
    p=c;
    return r;
  } // end of read

  template<>
  std::vector<std::string>
  read(tfel::utilities::CxxTokenizer::const_iterator& p,
       const tfel::utilities::CxxTokenizer::const_iterator pe)
  {
    using tfel::utilities::CxxTokenizer;
    auto c = p;
    auto r = CxxTokenizer::readStringArray(c,pe);
    p=c;
    return r;
  } // end of read
  
} // end of namespace mfront
