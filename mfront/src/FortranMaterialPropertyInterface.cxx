/*!
 * \file   mfront/src/FortranMaterialPropertyInterface.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   01 déc 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>
#include<stdexcept>

#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/FortranMaterialPropertyInterface.hxx"

namespace mfront
{

  std::string
  FortranMaterialPropertyInterface::getName(void)
  {
    return "fortran";
  }

  FortranMaterialPropertyInterface::FortranMaterialPropertyInterface()
    : CMaterialPropertyInterfaceBase()
  {}

  std::map<std::string,std::vector<std::string> >
  FortranMaterialPropertyInterface::getGlobalDependencies(const std::string& library,
						   const std::string& material,
						   const std::string&)
  {
    using namespace std;
    map<string,vector<string> > libs;
    libs["libFortran"+getMaterialLawLibraryNameBase(library,material)].push_back("-lm");
    return libs;
  } // end of FortranMaterialPropertyInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  FortranMaterialPropertyInterface::getGlobalIncludes(const std::string&,
					       const std::string&,
					       const std::string&)
  {
    using namespace std;
    return map<string,vector<string> >();
  } // end of FortranMaterialPropertyInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  FortranMaterialPropertyInterface::getGeneratedSources(const std::string& library,
						 const std::string& material,
						 const std::string& className)
  {
    using namespace std;
    map<string,vector<string> > src;
    string name = this->getSrcFileName(material,className);
    src["libFortran"+getMaterialLawLibraryNameBase(library,material)].push_back(name+".cxx");
    return src;
  } // end of FortranMaterialPropertyInterface::getGeneratedSources

  std::vector<std::string>
  FortranMaterialPropertyInterface::getGeneratedIncludes(const std::string&,
						  const std::string&,
						  const std::string&)
  {
    using namespace std;
    return vector<string>();
  } // end of FortranMaterialPropertyInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  FortranMaterialPropertyInterface::getLibrariesDependencies(const std::string& library,
						      const std::string& material,
						      const std::string&)
  {
    using namespace std;
    map<string,vector<string> > libs;
    libs["libFortran"+getMaterialLawLibraryNameBase(library,material)].push_back("-lm");
    return libs;
  } // end of FortranMaterialPropertyInterface::getLibrariesDependencies()

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
    FortranMaterialPropertyInterface::getSpecificTargets(const std::string&,
						  const std::string&,
						  const std::string&,
						  const std::vector<std::string>&)
  {
    using namespace std;
    return map<string,pair<vector<string>,vector<string> > >();
  } // end of FortranMaterialPropertyInterface::getSpecificTargets

  std::string
  FortranMaterialPropertyInterface::getHeaderFileName(const std::string&,
					       const std::string&)
  {
    return "";
  } // end of FortranMaterialPropertyInterface::getHeaderFileName

  std::string
  FortranMaterialPropertyInterface::getSrcFileName(const std::string& material,
					    const std::string& className)
  {
    if(material.empty()){
      return className+"-fortran";
    }
    return material+"_"+className+"-fortran";
  } // end of FortranMaterialPropertyInterface::getSrcFileName

  void
  FortranMaterialPropertyInterface::writeInterfaceSpecificVariables(const VariableDescriptionContainer& inputs)
  {
    VariableDescriptionContainer::const_iterator p;
    for(p=inputs.begin();p!=inputs.end();++p){
      this->srcFile << "const mfront_fortran_real8 " << p->name << " =  *(_mfront_var_" << p->name << ");\n";
    }
  } // end of FortranMaterialPropertyInterface::writeInterfaceSpecificVariables

  void
  FortranMaterialPropertyInterface::writeParameterList(std::ostream& file,
					      const VariableDescriptionContainer& inputs){
    VariableDescriptionContainer::const_iterator p;
    if(!inputs.empty()){
      for(p=inputs.begin();p!=inputs.end();){
	file << "const mfront_fortran_real8 * const _mfront_var_" << p->name;
	if((++p)!=inputs.end()){
	  file << ",\n";
	}
      }
    } else {
      file << "void";
    }
  } // end of FortranMaterialPropertyInterface::writeParameterList
  
  void
  FortranMaterialPropertyInterface::writeHeaderPreprocessorDirectives(const std::string&,
							       const std::string&)
  {} // end of FortranMaterialPropertyInterface::writePreprocessorDirectives

  void
  FortranMaterialPropertyInterface::writeSrcPreprocessorDirectives(const std::string& material,
							    const std::string& className)
  {
    using namespace std;
    string name;
    string f77_func;
    if(!material.empty()){
      name = material+"_"+className;
    } else {
      name = className;
    }
    if(name.find('_')!=string::npos){
      f77_func = "F77_FUNC_";
    } else {
      f77_func = "F77_FUNC";
    }
    this->srcFile << "#define " 
		  << makeUpperCase(name)
		  << "_F77 \\\n"
		  << "        "<< f77_func << "("
		  << makeLowerCase(name) << ","
		  << makeUpperCase(name) << ")\n\n";
    this->srcFile << "#define " 
		  << makeUpperCase(name)
		  << "_CHECKBOUNDS_F77 \\\n"
		  << "        "<< f77_func << "("
		  << makeLowerCase(name) 
		  << "_checkbounds,\\\n        "
		  << makeUpperCase(name) 
		  << "_CHECKBOUNDS)\n\n";
    writeExportDirectives(this->srcFile);
    this->srcFile << "typedef double mfront_fortran_real8;" << endl;
    this->srcFile << "typedef int    mfront_fortran_integer4;" << endl << endl;
  } // end of FortranMaterialPropertyInterface::writeSrcPreprocessorDirectives

  void
  FortranMaterialPropertyInterface::writeBeginHeaderNamespace(void)
  {} // end of FortranMaterialPropertyInterface::writeBeginHeaderNamespace
  
  void
  FortranMaterialPropertyInterface::writeEndHeaderNamespace(void)
  {} // end of FortranMaterialPropertyInterface::writeEndHeaderNamespace(void)

  void
  FortranMaterialPropertyInterface::writeBeginSrcNamespace(void)
  {
    this->srcFile << "#ifdef __cplusplus\n";
    this->srcFile << "extern \"C\"{\n";
    this->srcFile << "#endif /* __cplusplus */\n\n";
  } // end of FortranMaterialPropertyInterface::writeBeginSrcNamespace
  
  void
  FortranMaterialPropertyInterface::writeEndSrcNamespace(void)
  {
    this->srcFile << "#ifdef __cplusplus\n";
    this->srcFile << "} // end of extern \"C\"\n";
    this->srcFile << "#endif /* __cplusplus */\n\n";
  } // end of FortranMaterialPropertyInterface::writeEndSrcNamespace(void)

  std::string
  FortranMaterialPropertyInterface::getFunctionDeclaration(const std::string& material,
						    const std::string& className)
  {
    if(material.empty()){
      return "MFRONT_SHAREDOBJ mfront_fortran_real8\n"+ makeUpperCase(className) + "_F77";
    }
    return "MFRONT_SHAREDOBJ mfront_fortran_real8\n"+ makeUpperCase(material+"_"+className) + "_F77";
  } // end of FortranMaterialPropertyInterface::getFunctionDeclaration
  
  bool
  FortranMaterialPropertyInterface::requiresCheckBoundsFunction(void) const
  {
    return false;
  }

  std::string
  FortranMaterialPropertyInterface::getCheckBoundsFunctionDeclaration(const std::string& material,
							       const std::string& className)
  {
    if(material.empty()){
      return "MFRONT_SHAREDOBJ mfront_fortran_integer4\n" + makeUpperCase(className) + "_CHECKBOUNDS_F77";
    }
    return "MFRONT_SHAREDOBJ mfront_fortran_integer4\n" + makeUpperCase(material+"_"+className) + "_CHECKBOUNDS_F77";
  } // end of FortranMaterialPropertyInterface::getCheckBoundsFunctionDeclaration
  
  FortranMaterialPropertyInterface::~FortranMaterialPropertyInterface()
  {} // end of FortranMaterialPropertyInterface::~FortranMaterialPropertyInterface

} // end of namespace mfront
