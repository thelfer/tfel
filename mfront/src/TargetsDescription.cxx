/*! 
 * \file  TargetsDescription.cxx
 * \brief
 * \author Helfer Thomas
 * \date   09 mai 2015
 */

#include<algorithm>
#include<stdexcept>
#include"MFront/TargetsDescription.hxx"

namespace mfront{

  TargetsDescription::TargetsDescription() = default;
  TargetsDescription::TargetsDescription(const TargetsDescription&) = default;
  TargetsDescription::TargetsDescription(TargetsDescription&&) = default;
  TargetsDescription::~TargetsDescription() = default;

  LibraryDescription&
  TargetsDescription::operator[](const std::string& n){
    auto c = [&n](const LibraryDescription& l){
      return l.name == n;
    };
    const auto b = this->libraries.begin();
    const auto e = this->libraries.end();
    const auto p = std::find_if(b,e,c);
    if(p==e){
      const auto s = LibraryDescription::getDefaultLibrarySuffix(this->system,
								 this->libraryType);
      this->libraries.emplace_back(n,s,this->libraryType);
      return this->libraries.back();
    }
    return *p;
  } // end of TargetsDescription::operator[]

  const LibraryDescription&
  TargetsDescription::operator[](const std::string& n) const{
    auto c = [&n](const LibraryDescription& l){
      return l.name == n;
    };
    const auto b = this->libraries.begin();
    const auto e = this->libraries.end();
    const auto p = std::find_if(b,e,c);
    if(p==e){
      throw(std::runtime_error("TargetsDescription::operator[] : "
			       "no library named '"+n+"'"));
    }
    return *p;
  } // end of TargetsDescription::operator[]

  
  TargetsDescription::const_iterator
  TargetsDescription::begin(void) const {
    return this->libraries.cbegin();
  }

  TargetsDescription::const_iterator
  TargetsDescription::cbegin(void) const {
    return this->libraries.cbegin();
  }

  TargetsDescription::const_iterator
  TargetsDescription::end(void) const {
    return this->libraries.cend();
  }

  TargetsDescription::const_iterator
  TargetsDescription::cend(void) const {
    return this->libraries.cend();
  }

  void mergeTargetsDescription(TargetsDescription& d,
			       const TargetsDescription& s)
  {
    for(const auto& ls : s){
      auto& ld = d[ls.name];
      mergeLibraryDescription(ld,ls);
    }
    for(const auto& h:s.headers){
      if(find(d.headers.begin(),d.headers.end(),h)==d.headers.end()){
	d.headers.push_back(h);
      }
    }
    for(const auto& t: s.specific_targets){
      d.specific_targets[t.first] = t.second;
    }
  } // end of mergeTargetsDescription

  bool describes(const TargetsDescription& t,
		 const std::string& n){
    auto comp = [&n](const LibraryDescription& ld){
      return ld.name==n;
    };
    return std::find_if(t.begin(),t.end(),comp)!=t.end();
  } // end of describes
    
} // end of namespace mfront


