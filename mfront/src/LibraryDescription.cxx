/*! 
 * \file  LibraryDescription.cxx
 * \brief
 * \author Helfer Thomas
 * \date   09 mai 2015
 */

#include<iostream>
#include<stdexcept>
#include<algorithm>
#include"MFront/MFrontUtilities.hxx"
#include"MFront/LibraryDescription.hxx"

namespace mfront{

  std::string
  convert(const LibraryDescription::LibraryType t){
    if(t==LibraryDescription::SHARED_LIBRARY){
      return "shared library";
    }
    if(t!=LibraryDescription::MODULE){
      throw(std::runtime_error("to_string : unsupported library type"));
    }
    return "module";
  } // end of to string
  
  void mergeLibraryDescription(LibraryDescription& d,
			       const LibraryDescription& s)
  {
    if((d.name!=s.name)||(d.suffix!=s.suffix)||(d.type!=s.type)){
      throw(std::runtime_error("mergeLibraryDescription : "
			       "can't merge description of library'"+
			       d.name+"' ("+d.suffix+","+","+convert(d.type)+")"
			       "and description of library '"+
			       s.name+"' ("+s.suffix+","+","+convert(s.type)+")"));
    }
    insert_if(d.sources,s.sources);
    insert_if(d.cppflags,s.cppflags);
    insert_if(d.ldflags,s.ldflags);
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

  std::ostream&
  operator << (std::ostream& os,
	       const LibraryDescription& l){
    auto error = [&l](){
      throw(std::runtime_error("operator << : "
			       "unsuported library type (neither module or "
			       "shared library)  for library '"+l.name+"'"));
    };
    os << "{\n";
    os << "name   : \"" << l.name   << "\";\n";
    os << "type   : ";
    switch(l.type){
    case LibraryDescription::MODULE:
      os << "MODULE";
      break;
    case LibraryDescription::SHARED_LIBRARY:
      os << "SHARED_LIBRARY";
      break;
    default:
      error();
    }
    os << ";\n";    
    os << "suffix : \"" << l.suffix << "\";\n";
    write(os,l.sources,"sources");
    write(os,l.cppflags,"cppflags");
    write(os,l.ldflags,"ldflags");
    write(os,l.epts,"epts");
    os << "};\n";
    return os;
  }

  template<>
  MFRONT_VISIBILITY_EXPORT LibraryDescription
  read(tfel::utilities::CxxTokenizer::const_iterator& p,
       const tfel::utilities::CxxTokenizer::const_iterator pe){
    using tfel::utilities::CxxTokenizer;
    const auto f = "read<LibraryDescription>";
    auto error = [&f](const std::string& m){
      throw(std::runtime_error(std::string{f}+" : "+m));
    };
    auto get_vector = [&f](std::vector<std::string>& v,
			   tfel::utilities::CxxTokenizer::const_iterator& pc,
			   const tfel::utilities::CxxTokenizer::const_iterator e,
			   const std::string& n){
      if(!v.empty()){
	throw(std::runtime_error(std::string{f}+" : library member '"+
				 n+"' multiply defined"));
      }
      auto c = pc;
      CxxTokenizer::readSpecifiedToken(f,":",c,e);
      v  = read<std::vector<std::string>>(c,e);
      CxxTokenizer::readSpecifiedToken(f,";",c,e);
      pc = c;
    };
    LibraryDescription::LibraryType type = LibraryDescription::SHARED_LIBRARY;
    auto btype    = false;
    auto name     = std::string{};
    auto suffix   = std::string{};
    auto sources  = std::vector<std::string>{};
    auto cppflags = std::vector<std::string>{};
    auto ldflags  = std::vector<std::string>{};
    auto epts     = std::vector<std::string>{};
    // parsing 
    auto c = p;
    CxxTokenizer::readSpecifiedToken(f,"{",c,pe);
    CxxTokenizer::checkNotEndOfLine(f,c,pe);
    while(c->value!="}"){
      if(c->value=="name"){
	if(!name.empty()){
	  error("library name '"+name+"' multiply defined");
	}
	++c;
	CxxTokenizer::readSpecifiedToken(f,":",c,pe);
	name = CxxTokenizer::readString(c,pe);
	CxxTokenizer::readSpecifiedToken(f,";",c,pe);
      } else if(c->value=="suffix"){
	if(!suffix.empty()){
	  error("library suffix multiply defined");
	}
	++c;
	CxxTokenizer::readSpecifiedToken(f,":",c,pe);
	suffix = CxxTokenizer::readString(c,pe);
	CxxTokenizer::readSpecifiedToken(f,";",c,pe);
      } else if(c->value=="type"){
	++c;
	if(btype){
	  error("library type already defined");
	}
	CxxTokenizer::readSpecifiedToken(f,":",c,pe);
	CxxTokenizer::checkNotEndOfLine(f,c,pe);
	if(c->value=="SHARED_LIBRARY"){
	  type = LibraryDescription::SHARED_LIBRARY;
	  btype = true;
	} else if(c->value=="MODULE"){
	  type = LibraryDescription::MODULE;
	  btype = true;
	} else {
	  error("unsupported library type '"+c->value+"'");
	}
	++c;
	CxxTokenizer::readSpecifiedToken(f,";",c,pe);
      } else if(c->value=="sources"){
	++c;
	get_vector(sources,c,pe,"sources");
      } else if(c->value=="cppflags"){
	++c;
	get_vector(cppflags,c,pe,"cppflags");
      } else if(c->value=="ldflags"){
	++c;
	get_vector(ldflags,c,pe,"ldflags");
      } else if(c->value=="epts"){
	++c;
	get_vector(epts,c,pe,"epts");
      }
      CxxTokenizer::checkNotEndOfLine(f,c,pe);
    }
    CxxTokenizer::readSpecifiedToken(f,"}",c,pe);
    if(!btype){
      error("library type undefined");
    }
    LibraryDescription l{name,suffix,type};
    std::swap(l.sources,sources);
    std::swap(l.cppflags,cppflags);
    std::swap(l.ldflags,ldflags);
    std::swap(l.epts,epts);
    p = c;
    return l;
  }

  
} // end of namespace mfront


