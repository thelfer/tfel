/*! 
 * \file  TargetsDescription.cxx
 * \brief
 * \author Helfer Thomas
 * \date   09 mai 2015
 */

#include<stdexcept>
#include<algorithm>
#include"MFront/TargetsDescription.hxx"

namespace mfront{

  void mergeTargetsDescription(TargetsDescription& d,
			       const TargetsDescription& s)
  {
    using namespace std;
    for(const auto& lib: s.sources){
      auto& ss = d.sources[lib.first];
      for(const auto& f: lib.second){
	if(find(ss.begin(),ss.end(),f)!=ss.end()){
	  throw(runtime_error("mergeTargetsDescription : "
			      "source '"+f+"' multiply defined "
			      "for library '"+lib.first+"'"));
	}
	ss.push_back(f);
      }
    }
    for(const auto& h:s.headers){
      if(find(d.headers.begin(),d.headers.end(),h)!=d.headers.end()){
	throw(runtime_error("mergeTargetsDescription : "
			    "header '"+h+"' multiply defined "));
      }
      d.headers.push_back(h);
    }
    for(const auto& lib: s.cppflags){
      auto& cppflags = d.cppflags[lib.first];
      for(const auto& flag: lib.second){
	if(find(cppflags.begin(),cppflags.end(),flag)==cppflags.end()){
	  cppflags.push_back(flag);
	}
      }
    }
    for(const auto& lib: s.dependencies){
      auto& deps = d.dependencies[lib.first];
      for(const auto& dep: lib.second){
	if(find(deps.begin(),deps.end(),dep)==deps.end()){
	  deps.push_back(dep);
	}
      }
    }
    for(const auto& lib: s.epts){
      auto& epts = d.epts[lib.first];
      for(const auto& e: lib.second){
	if(find(epts.begin(),epts.end(),e)!=epts.end()){
	  throw(runtime_error("mergeTargetsDescription : "
			      "entry point '"+e+"' multiply defined"
			      "for library '"+lib.first+"'"));
	}
	epts.push_back(e);
      }
    }
    for(const auto& t: s.specific_targets){
      if(d.specific_targets.find(t.first)!=d.specific_targets.end()){
	throw(runtime_error("mergeTargetsDescription : "
			    "specif target '"+t.first+"' multiply defined"));
      }
    }
  } // end of mergeTargetsDescription

  TargetsDescription::TargetsDescription() = default;
  TargetsDescription::TargetsDescription(const TargetsDescription&) = default;
  TargetsDescription::TargetsDescription(TargetsDescription&&) = default;
  TargetsDescription& TargetsDescription::operator=(const TargetsDescription&) = default;
  TargetsDescription& TargetsDescription::operator=(TargetsDescription&&) = default;
  TargetsDescription::~TargetsDescription() = default;

} // end of namespace mfront


