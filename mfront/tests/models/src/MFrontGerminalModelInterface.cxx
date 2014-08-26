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
#include"MFront/MFrontModelInterfaceProxy.hxx"
#include"MFront/MFrontGerminalModelInterface.hxx"

namespace mfront{

  std::string
  MFrontGerminalModelInterface::getName(void)
  {
    return "germinal";
  } // end of MFrontPleiadesModelInterfaceBase::getName(void)

  void
  MFrontGerminalModelInterface::setOutputFileNames(const MFrontModelData& mdata)
  {
    this->headerFileName  = "Pleiades/PMetier/PModels/"+mdata.className;
    this->headerFileName += "-germinal.hxx";
    this->srcFileName  = mdata.className;
    this->srcFileName += "-germinal.cxx";
  } // end of MFrontGerminalModelInterface::setOutputFileNames(void)

  std::string
  MFrontGerminalModelInterface::getApplicationName(void) const
  {
    return "Germinal";
  } // end of MFrontGerminalModelInterface::getLibraryBaseName

  void
  MFrontGerminalModelInterface::writeGetConstantMaterialProperty(const VarHandler& v,
								 const MFrontModelData& mdata)
  {
    using namespace std;
    string name;
    string name2;
    map<string,string>::const_iterator p;
    if((p=mdata.glossaryNames.find(v.name))!=mdata.glossaryNames.end()){
      name  = "GlossaireParam::" + p->second;
      name2 = "this->getMeshZoneName()+\".\"+GlossaireParam::" + p->second;
    } else if((p=mdata.entryNames.find(v.name))!=mdata.entryNames.end()){
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
    if((p=mdata.defaultValues.find(v.name))!=mdata.defaultValues.end()){
      this->srcFile << "this->" << v.name << " = " << p->second << ";" << endl;
    } else {
      this->srcFile << "string msg(\"" << mdata.className << "::initializeParameters : \");\n";
      this->srcFile << "msg += \"can't initialize constant material property  '"
		    << v.name << "' using '\";\n";
      this->srcFile << "msg += " << name << ";\n";
      this->srcFile << "msg += '\\'';\n";
      this->srcFile << "throw(PleiadesError(msg));\n";
    }
    this->srcFile << "}\n";
  } // end of MFrontGerminalModelInterface::writeGetConstantMaterialProperty

  void
  MFrontGerminalModelInterface::writeInitializeMethod(const MFrontModelData& mdata)
  {
    using namespace std;
    VarContainer::const_iterator p;
    map<string,string>::const_iterator p2;
    map<string,double>::const_iterator p3;

    this->srcFile << "bool\n"
		  << mdata.className
		  << "::initialize(const Pleiades::PMetier::IArgumentMetier& arg)";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
    this->srcFile << "using namespace Pleiades::PExceptions;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PGlossaire;\n";
    for(p=mdata.outputs.begin();p!=mdata.outputs.end();++p){
      string name;
      string iname;
      if((p2=mdata.glossaryNames.find(p->name))!=mdata.glossaryNames.end()){
	name  = "GlossaireField::"+p2->second;
	iname = "GlossaireField::"+p2->second+"+\".InitialValue\"";
      } else if((p2=mdata.entryNames.find(p->name))!=mdata.entryNames.end()){
	name  = "\""+p2->second+"\"";
	iname = "\""+p2->second+".InitialValue\"";
      } else {
	name  = "\""+p->name+"\"";
	iname = "\""+p->name+".InitialValue\"";
      }
      this->srcFile << "if(arg.contains(this->getMeshZoneName()+\".\"+" << iname << ")){\n";
      this->srcFile << "this->initializeField(this->_ple" << p->name
		    << ",arg[this->getMeshZoneName()+\".\"+" << iname << "]."
		    << this->getGenTypeMethod("real") << "());" << endl;
      this->srcFile << "} else if(arg.contains("<<iname << ")){\n";
      this->srcFile << "this->initializeField(this->_ple" << p->name
		    << ",arg[" <<iname << "]."
		    << this->getGenTypeMethod("real") << "());" << endl;
      this->srcFile << "} else {\n";
      if((p3=mdata.initialValues.find(p->name))!=mdata.initialValues.end()){
	this->srcFile << "this->initializeField(this->_ple" << p->name
		      << "," << p3->second << ");\n";
      } else {
	this->srcFile << "string msg(\"" << mdata.className << "::initializeOutput : \");\n";
	this->srcFile << "msg += \"no initial value given for output field '\";\n";
	this->srcFile << "msg += " << name << ";\n";
	this->srcFile << "msg += " << "\"'\";\n";
	this->srcFile << "throw(PleiadesError(msg));\n";
      }
      this->srcFile << "}\n";

    }
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << mdata.className << "::initialize\n\n";

    this->srcFile << "void\n"
    		  << mdata.className
    		  << "::initializeField(Pleiades::PMetier::PField::PtrIFieldDouble f,\n"
    		  << "const double v)\n";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
    this->srcFile << "using namespace Pleiades::PExceptions;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PGlossaire;\n";
    this->srcFile << "const vector<int>& nodes = "<< mdata.className
    		  << "::getNodesOfCELLZone(f->getMesh(),this->getMeshZoneName());\n";
    this->srcFile << "for(int node_k=0;node_k!=static_cast<int>(nodes.size());++node_k){\n";
    this->srcFile << "f->setValueType(nodes[node_k],v,0);\n";
    this->srcFile << "}\n";
    this->srcFile << "} // end of " << mdata.className << "::initializeField\n\n";

  } // end of MFrontGerminalModelInterface::writeInitializeMethod

  void
  MFrontGerminalModelInterface::writeInitializeParametersMethod(const MFrontModelData& mdata)
  {
    using namespace std;
    VarContainer::const_iterator p;
    map<string,string>::const_iterator p2;
    map<string,double>::const_iterator p3;
    this->srcFile << "bool\n"
		  << mdata.className
		  << ":initializeParameters(const Pleiades::PMetier::IArgumentMetier& arg)";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
    this->srcFile << "using namespace Pleiades::PExceptions;\n";
    this->srcFile << "using namespace Pleiades::PMetier::PGlossaire;\n";
    this->srcFile << "this->domains.insert(this->getMeshZoneName());\n";
    for(p=mdata.globalParameters.begin();p!=mdata.globalParameters.end();++p){
      this->writeGetGlobalParameter(*p,mdata);
    }
    for(p=mdata.constantMaterialProperties.begin();p!=mdata.constantMaterialProperties.end();++p){
      this->writeGetConstantMaterialProperty(*p,mdata);
    }
    this->srcFile << "return true;\n";
    this->srcFile << "} // end of " << mdata.className << "::initializeParameters\n\n";
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
    using namespace std;
    this->headerFile << "void\n"
		     << "initializeField(Pleiades::PMetier::PField::PtrIFieldDouble,\n"
		     << "const double);\n\n";
  } // end of MFrontGerminalModelInterface::writeSpecificPrivateMethodDeclaration

  bool
  MFrontGerminalModelInterface::initializeDefaultDomainListInConstrutor(const MFrontModelData&) const
  {
    return false;
  } // end of MFrontGerminalModelInterface::initializeDefaultDomainListInConstrutor

  MFrontModelInterfaceProxy<MFrontGerminalModelInterface> ModelGerminalProxy;

} // end of namespace mfront
