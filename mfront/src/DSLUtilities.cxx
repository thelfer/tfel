/*!
 * \file   mfront/src/DSLUtilities.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   23 oct 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<stdexcept>
#include<fstream>
#include<sstream>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Glossary/GlossaryEntry.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/DSLUtilities.hxx"

namespace mfront
{

  void
  writeMaterialLaws(const std::string&,
		    std::ofstream& srcFile,
		    const std::vector<std::string>& materialLaws)
  {
    using namespace std;
    vector<string>::const_iterator p;
    for(p=materialLaws.begin();p!=materialLaws.end();++p){
      srcFile << "using mfront::" << *p << ";" << endl;
      srcFile << "using mfront::" << *p << "_checkBounds;" << endl;
    }
  } // end of writeMaterialLaws

  void
  writeStaticVariables(const std::string& method,
		       std::ofstream& srcFile,
		       const StaticVariableDescriptionContainer& staticVars,
		       const std::string& file)
  {
    using namespace std;
    StaticVariableDescriptionContainer::const_iterator p;
    // static variables
    if(!staticVars.empty()){
      for(p=staticVars.begin();p!=staticVars.end();++p){
	if(getDebugMode()){
	  srcFile << "#line " << p->lineNumber << " \"" 
		  << file << "\"\n";
	}
	if(p->type=="short"){
	  srcFile << "static constexpr  short " << p->name 
		  << " = " << static_cast<short>(p->value) << ";" << endl;
	} else if(p->type=="ushort"){
	  srcFile << "static constexpr  unsigned short "  << p->name 
		  << " = " << static_cast<unsigned short>(p->value) << ";" << endl;
	} else if(p->type=="int"){
	  srcFile << "static constexpr  int " << p->name 
		  << " = " << static_cast<int>(p->value) << ";" << endl;
	} else if(p->type=="uint"){
	  srcFile << "static constexpr  unsigned int " << p->name 
		  << " = " << static_cast<unsigned int>(p->value) << ";" << endl;
	} else if(p->type=="long"){
	  srcFile << "static constexpr  long " << p->name 
		  << " = " << static_cast<long>(p->value) << ";" << endl;
	} else if(p->type=="ulong"){
	  srcFile << "static constexpr  unsigned long " << p->name 
		  << " = " << static_cast<unsigned long>(p->value) << ";" << endl;
	} else if(p->type=="float"){
	  srcFile << "static constexpr  float " << p->name 
		  << " = " << static_cast<float>(p->value) << ";" << endl;
	} else if((p->type=="double")||(p->type=="real")){
	  srcFile << "static constexpr  double " << p->name 
		  << " = " << static_cast<double>(p->value) << ";" << endl;
	} else if(p->type=="ldouble"){
	  srcFile << "static constexpr  long double " << p->name 
		  << " = " << static_cast<long double>(p->value) << ";" << endl;
	} else {
	  ostringstream msg;
	  msg << method << "type '" + p->type 
	      << "' is not a supported type for a static variable."
	      << "Supported types are short, ushort, int, uint, long, ulong,"
	      << "float, double and ldouble.\n"
	      << "Error at line " << p->lineNumber << ".";
	  throw(runtime_error(msg.str()));
	}
      }
      srcFile << endl;
    }

  } // end of writeStaticVariables(std::ofstream&,const StaticVariableDescriptionContainer&)

  std::string
  getMaterialLawLibraryNameBase(const std::string& library,
				const std::string& material)
  {
    using namespace std;
    if(library.empty()){
      if(material.empty()){
	return "MaterialLaw";
      }
      return material;
    }
    return library;
  } // end of getLibraryNameBase

  void
  writeExportDirectives(std::ofstream& file,
			const std::string& c)
  {
    file << "#ifdef WIN32\n";
    file << "#include <windows.h>\n";
    file << "#ifndef MFRONT_SHAREDOBJ\n";
    file << "#define MFRONT_SHAREDOBJ __declspec(dllexport)\n"; 
    file << "#endif /* MFRONT_SHAREDOBJ */\n"; 
    file << "#ifndef MFRONT_CALLING_CONVENTION\n";
    file << "#define MFRONT_CALLING_CONVENTION " << c << "\n"; 
    file << "#endif /* MFRONT_CALLING_CONVENTION */\n"; 
    file << "#else\n";
    file << "#ifndef MFRONT_SHAREDOBJ\n";
    file << "#ifdef __GNUC__\n";
    file << "#define MFRONT_SHAREDOBJ __attribute__((visibility(\"default\")))\n";
    file << "#else\n";
    file << "#define MFRONT_SHAREDOBJ\n";
    file << "#endif /* __GNUC__ */\n";
    file << "#endif /* MFRONT_SHAREDOBJ */\n"; 
    file << "#ifndef MFRONT_CALLING_CONVENTION\n";
    file << "#define MFRONT_CALLING_CONVENTION\n"; 
    file << "#endif /* MFRONT_CALLING_CONVENTION */\n"; 
    file << "#endif /* WIN32 */\n\n";
  } // end of writeExportDirectives

  std::string 
  makeUpperCase(const std::string& n)
  {
    using namespace std;
    string s(n);
    string::const_iterator p;
    string::iterator p2;
    for(p=n.begin(),p2=s.begin();p!=n.end();++p,++p2){
      *p2 = static_cast<char>(toupper(*p));
    }
    return s;
  } // end of makeUpperCase

  std::string 
  makeLowerCase(const std::string& n)
  {
    using namespace std;
    string s(n);
    string::const_iterator p;
    string::iterator p2;
    for(p=n.begin(),p2=s.begin();p!=n.end();++p,++p2){
      *p2 = static_cast<char>(tolower(*p));
    }
    return s;
  } // end of makeLowerCase

  std::string
  toString(const unsigned short src)
  {
    std::ostringstream os;
    os << src;
    return os.str();
  } // end of toString

  void
  displayGlossaryEntryCompleteDescription(std::ostream& os,
					  const tfel::glossary::GlossaryEntry& e)
  {
    using namespace std;
    const string& k = e.getKey();
    const vector<string>& n = e.getNames();
    const string& sd = e.getShortDescription();
    const vector<string>& d = e.getDescription();
    if((!sd.empty())||(!d.empty())){
      os << endl
	 << "For your information, the description of the glossary entry '" << k << "' is:" << endl;
      if(!n.empty()){
	os << k << ":" ;
	for(vector<string>::const_iterator p=n.begin();p!=n.end();++p){
	  os << " '" << *p << "'";
	}
	os << endl;
      }
      if(!sd.empty()){
	os << sd << endl;
      }
      if(!d.empty()){
	for(vector<string>::const_iterator p=d.begin();p!=d.end();++p){
	  os << *p << endl;
	}
      }
    }
  } // end of displayGlossaryEntryCompleteDescription

} // end of namespace mfront
