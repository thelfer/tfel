/*!
 * \file   ParserUtilities.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   23 oct 2008
 */

#include<iostream>
#include<stdexcept>
#include<sstream>
#include"ParserUtilities.hxx"

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
    }
  } // end of writeMaterialLaws

  void
  writeStaticVariables(const std::string& method,
		       std::ofstream& srcFile,
		       const StaticVarContainer& staticVars,
		       const std::string& file,
		       const bool debugMode)
  {
    using namespace std;
    StaticVarContainer::const_iterator p;
    // static variables
    if(!staticVars.empty()){
      for(p=staticVars.begin();p!=staticVars.end();++p){
	if(debugMode){
	  srcFile << "#line " << p->lineNumber << " \"" 
		  << file << "\"\n";
	}
	if(p->type=="short"){
	  srcFile << "static const  short " << p->name 
		  << " = " << static_cast<short>(p->value) << ";" << endl;
	} else if(p->type=="ushort"){
	  srcFile << "static const  unsigned short "  << p->name 
		  << " = " << static_cast<unsigned short>(p->value) << ";" << endl;
	} else if(p->type=="int"){
	  srcFile << "static const  int " << p->name 
		  << " = " << static_cast<int>(p->value) << ";" << endl;
	} else if(p->type=="uint"){
	  srcFile << "static const  unsigned int " << p->name 
		  << " = " << static_cast<unsigned int>(p->value) << ";" << endl;
	} else if(p->type=="long"){
	  srcFile << "static const  long " << p->name 
		  << " = " << static_cast<long>(p->value) << ";" << endl;
	} else if(p->type=="ulong"){
	  srcFile << "static const  unsigned long " << p->name 
		  << " = " << static_cast<unsigned long>(p->value) << ";" << endl;
	} else if(p->type=="float"){
	  srcFile << "static const  float " << p->name 
		  << " = " << static_cast<float>(p->value) << ";" << endl;
	} else if((p->type=="double")||(p->type=="real")){
	  srcFile << "static const  double " << p->name 
		  << " = " << static_cast<double>(p->value) << ";" << endl;
	} else if(p->type=="ldouble"){
	  srcFile << "static const  long double " << p->name 
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

  } // end of writeStaticVariables(std::ofstream&,const StaticVarContainer&)

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

} // end of namespace mfront
