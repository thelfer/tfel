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
    this->headerFileName  = "Pleiades/PMetier/PModels/"+pdata.className;
    this->headerFileName += "-germinal.hh";
    this->srcFileName  = pdata.className;
    this->srcFileName += "-germinal.cpp";
  } // end of MFrontGerminalModelInterface::setOutputFileNames(void)
  
  std::string
  MFrontGerminalModelInterface::getApplicationName(void) const
  {
    return "Germinal";
  } // end of MFrontGerminalModelInterface::getLibraryBaseName

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

  void
  MFrontGerminalModelInterface::writeInitializeMethod(const MFrontGenericData& pdata,
						      const MFrontModelData& data)
  {
    using namespace std;
    VarContainer::const_iterator p;
    map<string,string>::const_iterator p2;
    map<string,double>::const_iterator p3;
    this->srcFile << "bool\n"
		  << pdata.className 
		  << "::initialize(const Pleiades::PMetier::IArgumentMetier& arg)";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
    this->srcFile << "using namespace Pleiades::PExceptions;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PGlossaire;\n";
    for(p=data.outputs.begin();p!=data.outputs.end();++p){
      string name;
      string iname;
      if((p2=data.glossaryNames.find(p->name))!=data.glossaryNames.end()){
	name  = "GlossaireField::"+p2->second;
	iname = "GlossaireField::"+p2->second+"+\".InitialValue\"";
      } else if((p2=data.entryNames.find(p->name))!=data.entryNames.end()){
	name  = "\""+p2->second+"\"";
	iname = "\""+p2->second+".InitialValue\"";
      } else {
	name  = "\""+p->name+"\"";
	iname = "\""+p->name+".InitialValue\"";
      }
      this->srcFile << "if(arg.contains(this->getMeshZoneName()+" << iname << ")){\n";
      this->srcFile << "this->initializeField(this->ple" << p->name 
		    << ",arg[this->getMeshZoneName()+" << iname << "]." 
		    << this->getGenTypeMethod("real") << "());" << endl;
      this->srcFile << "} else if(arg.contains("<<iname << ")){\n";
      this->srcFile << "this->initializeField(this->ple" << p->name 
		    << ",arg[" <<iname << "]." 
		    << this->getGenTypeMethod("real") << "());" << endl;
      this->srcFile << "} else {\n";
      if((p3=data.initialValues.find(p->name))!=data.initialValues.end()){
	this->srcFile << "this->initializeField(this->ple" << p->name
		      << "," << p3->second << ");\n";
      } else {
	this->srcFile << "string msg(\"" << pdata.className << "::initializeOutput : \");\n";
	this->srcFile << "msg += \"no initial value given for output field '\";\n";
	this->srcFile << "msg += " << name << ";\n";
	this->srcFile << "\"'\";\n";
	this->srcFile << "throw(PleiadesError(msg));\n";
      }
      this->srcFile << "}\n";

    }
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << pdata.className << "::initialize\n\n";

    this->srcFile << "void\n"
		  << pdata.className 
		  << "::initializeField(Pleiades::PMetier::PField::PtrIFieldDouble f,\n"
		  << "const double v)\n";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
    this->srcFile << "using namespace Pleiades::PExceptions;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PGlossaire;\n";
    this->srcFile << "typedef MEDMEM::FIELD<double>* MEDFieldPtr;\n";
    this->srcFile << "typedef MEDMEM::FIELD<double>& MEDField;\n\n";
    this->srcFile << "MEDField mf = *(static_cast<MEDFieldPtr>(f->getMed()));\n";
    this->srcFile << "map<string,vector<int> >::iterator ptr;\n";
    this->srcFile << "vector<int>::const_iterator ptr2;\n";
    this->srcFile << "ptr = this->nodesOfZones.find(this->getMeshZoneName());\n";
    this->srcFile << "if(ptr==this->nodesOfZones.end()){\n";
    this->srcFile << "string msg(\"" << pdata.className << "::initializeField : \");\n";
    this->srcFile << "msg += \"internal error (no group named '\";\n";
    this->srcFile << "msg += this->getMeshZoneName();\n";
    this->srcFile << "msg += \" defined).\";\n";
    this->srcFile << "throw(PleiadesError(msg));\n";
    this->srcFile << "}\n";
    this->srcFile << "const vector<int>& nodes = ptr->second;\n";
    this->srcFile << "for(ptr2=nodes.begin();";
    this->srcFile << "ptr2!=nodes.end();++ptr2){\n";
    this->srcFile << "mf.setValueIJ(*ptr2,1,v);\n";
    this->srcFile << "}\n";
    this->srcFile << "} // end of " << pdata.className << "::initializeField\n\n";
  } // end of MFrontGerminalModelInterface::writeInitializeMethod

  void
  MFrontGerminalModelInterface::writeInitializeParametersMethod(const MFrontGenericData& pdata,
								const MFrontModelData& data)
  {
    using namespace std;
    VarContainer::const_iterator p;
    map<string,string>::const_iterator p2;
    map<string,double>::const_iterator p3;
    this->srcFile << "bool\n"
		  << pdata.className 
		  << "::initializeParameters(const Pleiades::PMetier::IArgumentMetier& arg)";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
    this->srcFile << "using namespace Pleiades::PExceptions;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PGlossaire;\n";
    this->srcFile << "this->domains.insert(this->getMeshZoneName());\n";
    for(p=data.globalParameters.begin();p!=data.globalParameters.end();++p){
      this->writeGetGlobalParameter(*p,pdata,data);
    }
    for(p=data.constantMaterialProperties.begin();p!=data.constantMaterialProperties.end();++p){
      this->writeGetConstantMaterialProperty(*p,pdata,data);
    }
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << pdata.className << "::initializeParameters\n\n";
  } // end of MFrontGerminalModelInterface::writeInitializeParametersMethod

  void
  MFrontGerminalModelInterface::generateDomainsList(const MFrontModelData&)
  {
    this->domains.insert("this->getMeshZoneName()");
  } // end of MFrontGerminalModelInterface::generateDomainsList

  void
  MFrontGerminalModelInterface::buildDomainName(void){
    this->srcFile << "// building current zone name\n";
    this->srcFile << "string domainName = *ptr;\n";
  }

  void
  MFrontGerminalModelInterface::writeSpecificPrivateMethodDeclaration(const MFrontGenericData&,
								      const MFrontModelData&)
  {
    this->headerFile << "void\n"
		     << "initializeField(Pleiades::PMetier::PField::PtrIFieldDouble,\n"
		     << "const double);\n\n";
  } // end of MFrontGerminalModelInterface::writeSpecificPrivateMethodDeclaration

  bool
  MFrontGerminalModelInterface::initializeDefaultDomainListInConstrutor(const MFrontModelData&) const
  {
    return false;
  } // end of MFrontGerminalModelInterface::initializeDefaultDomainListInConstrutor

} // end of namespace mfront  
