/*! 
 * \file  LibraryDescription.cxx
 * \brief
 * \author Helfer Thomas
 * \date   09 mai 2015
 */

#include<stdexcept>
#include<algorithm>
#include"MFront/LibraryDescription.hxx"

namespace mfront{

  static std::string
  to_string(const LibraryDescription::LibraryType t) noexcept{
    if(t==LibraryDescription::SHARED_LIBRARY){
      return "shared library";
    }
    return "module";
  } // end of to string
  
  void mergeLibraryDescription(LibraryDescription& d,
			       const LibraryDescription& s)
  {
    if((d.name!=s.name)||(d.suffix!=s.suffix)||(d.type!=s.type)){
      throw(std::runtime_error("mergeLibraryDescription : "
			       "can't merge description of library'"+
			       d.name+"' ("+d.suffix+","+","+to_string(d.type)+")"
			       "and description of library '"+
			       s.name+"' ("+s.suffix+","+","+to_string(s.type)+")"));
    }
    auto insert_if = [](std::vector<std::string>& dest,
			const std::vector<std::string>& src){
      for(const auto& v : src){
	if(std::find(dest.begin(),dest.end(),v)==dest.end()){
	  dest.push_back(v);
	}
      }
    };
    insert_if(d.sources,s.sources);
    insert_if(d.cppflags,s.cppflags);
    insert_if(d.dependencies,s.dependencies);
    insert_if(d.epts,s.epts);
  } // end of mergeLibraryDescription

  const char*
  LibraryDescription::getDefaultLibrarySuffix(const TargetSystem s,
					      const LibraryType l) noexcept{
    auto error = [](){
      throw(std::runtime_error("LibraryDescription::getDefaultLibrarySuffix : "
			       "internal errror : unsupported system or library type"));
    };
    const char* p = nullptr;
    switch(s){
    case UNIX:
      p = "so";
      break;
    case WINDOWS:
      p = "dll";
      break;
    case MACOSX:
      switch(l){
      case SHARED_LIBRARY:
	p = "dylib";
      case MODULE:
	p = "bundle";
	break;
      default:
	error();
      }
    default:
      error();
    }
    if(p==nullptr){
      error();
    }
    return p;
  } // end of LibraryDescription::getDefaultLibrarySuffix
  
  LibraryDescription::LibraryDescription(const std::string& n,
					 const std::string& s,
					 const LibraryType  t)
    : name(n),
      suffix(s),
      type(t)
  {} // end of LibraryDescription::LibraryDescription

  LibraryDescription::LibraryDescription(const LibraryDescription&) = default;
  LibraryDescription::LibraryDescription(LibraryDescription&&) = default;
  LibraryDescription::~LibraryDescription() = default;
  
} // end of namespace mfront


