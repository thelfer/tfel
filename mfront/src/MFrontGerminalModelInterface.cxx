/*!
 * \file   MFrontGerminalModelInterface.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#include<cassert>
#include<iterator>

#include"TFEL/System/System.hxx"

#include"MFront/MFrontHeader.hxx"
#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontGerminalModelInterface.hxx"

namespace mfront{

  std::string
  MFrontGerminalModelInterface::getName(void)
  {
    return "germinal";
  } // end of MFrontPleiadesModelInterfaceBase::getName(void)

  void
  MFrontGerminalModelInterface::setOutputFileNames(const MFrontGenericData& pdata)
  {
    this->headerFileName  = "include/Pleiades/PMetier/PModels/"+pdata.className;
    this->headerFileName += "-germinal.hh";
    this->srcFileName  = "src/"+pdata.className;
    this->srcFileName += "-germinal.cpp";
  } // end of MFrontGerminalModelInterface::setOutputFileNames(void)
  
  void
  MFrontGerminalModelInterface::writeGetConstantMaterialProperty(const VarHandler& v,
								 const MFrontGenericData& pdata,
								 const MFrontModelData& data)
  {
    using namespace std;
    string name;
    string name2;
    map<string,string>::const_iterator p;
    if((p=data.glossaryNames.find(v.name))!=data.glossaryNames.end()){
      name  = "GlossaireParam::" + p->second;
      name2 = "this->getMeshZoneName()+\".\"+GlossaireParam::" + p->second;
    } else if((p=data.entryNames.find(v.name))!=data.entryNames.end()){
      name = "\""+p->second+"\"";
      name2 = "this->getMeshZoneName()+\"."+p->second+"\"";
    } else {
      name = "\""+v.name+"\"";
      name2 = "this->getMeshZoneName()+\"."+v.name+"\"";
    }
    this->srcFile << "if(arg.contains(" << name2 << ")){\n";
    this->srcFile << "this->" << v.name << " = arg[" << name2 << "]." 
		  << this->getGenTypeMethod(v.type) << "();\n";
    this->srcFile << "} else if(arg.contains(" << name << ")){\n";
    this->srcFile << "this->" << v.name << " = arg[" << name << "]." 
		  << this->getGenTypeMethod(v.type) << "();\n";
    this->srcFile << "} else {\n";
    if((p=data.defaultValues.find(v.name))!=data.defaultValues.end()){
      this->srcFile << "this->" << v.name << " = " << p->second << ";" << endl;
    } else {
      this->srcFile << "string msg(\"" << pdata.className << "::initializeParameters : \");\n";
      this->srcFile << "msg += \"can't initialize constant material property  '"
		    << v.name << "' using '\";\n";
      this->srcFile << "msg += " << name << ";\n";
      this->srcFile << "msg += '\\'';\n";
      this->srcFile << "throw(PleiadesError(msg));\n";
    }
    this->srcFile << "}\n";
  } // end of MFrontGerminalModelInterface::writeGetConstantMaterialProperty
  
} // end of namespace mfront  
