/*!
 * \file   mfront/tests/properties/interfaces/src/MFrontPleiadesMaterialPropertyInterface-2.0.cxx
 * \brief
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>
#include<algorithm>
#include<stdexcept>

#include"TFEL/System/System.hxx"

#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/MaterialPropertyDescription.hxx"
#include"MFront/MaterialPropertyInterfaceProxy.hxx"
#include"MFront/MFrontPleiadesMaterialPropertyInterface-2.0.hxx"

namespace mfront {

  std::string MFrontPleiadesMaterialPropertyInterface::getName() {
    return "pleiades-2.0";
    // TODO: make a MFrontPleiadesMaterialPropertyInterface.cxx.in and use @PACKAGE_VERSION@
  }

  MFrontPleiadesMaterialPropertyInterface::MFrontPleiadesMaterialPropertyInterface() = default;

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontPleiadesMaterialPropertyInterface::treatKeyword(const std::string& k,
							const std::vector<std::string>& i,
							tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
							const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator) {
    if(std::find(i.begin(),i.end(),"pleiades-2.0")!=i.end()){
      throw(std::runtime_error("MFrontPleiadesMaterialPropertyInterface::treatKeyword: "
			       "unsupported key '"+k+"'"));
    }
    return {false,current};
  } // end of treatKeyword

  MFrontPleiadesMaterialPropertyInterface::~MFrontPleiadesMaterialPropertyInterface() = default;

  void
  MFrontPleiadesMaterialPropertyInterface::getTargetsDescription(TargetsDescription& d,
								 const MaterialPropertyDescription& mpd)
  {
    const auto lib    = "libPleiades"+getMaterialLawLibraryNameBase(mpd.library,mpd.material);
    const auto name = mpd.material.empty() ? mpd.className : mpd.material+"_"+mpd.className;
    d[lib].ldflags.push_back("-lm");
    d[lib].cppflags.push_back("`pleiades-config --includes`\n");
    d[lib].sources.push_back(name+"-pleiades.cpp");
    d[lib].epts.push_back(name);
    d.headers.push_back(this->headerFileName.substr(8));
  } // end of MFrontPleiadesMaterialPropertyInterface::getTargetsDescription

  void MFrontPleiadesMaterialPropertyInterface::writeOutputFiles(const MaterialPropertyDescription& mpd,
								 const FileDescription& fd){
    using namespace std;
    using namespace tfel::system;
    const auto& file=fd.fileName;
    const auto& author=fd.authorName;
    const auto& description=fd.description;
    const auto& date=fd.date;
    const auto& material=mpd.material;
    const auto& className=mpd.className;
    const auto& includes=mpd.includes;
    const auto& output=mpd.output;
    const auto& inputs=mpd.inputs;
    const auto& materialLaws=mpd.materialLaws;
    const auto& staticVars=mpd.staticVars;
    const auto& params=mpd.parameters;
    const auto& function=mpd.f;
    systemCall::mkdir("include/Pleiades");
    systemCall::mkdir("include/Pleiades/Metier");
    systemCall::mkdir("include/Pleiades/Metier/MaterialProperty");
    const auto name = (material.empty()) ? className : material+"_"+className;
    this->headerFileName  = "include/Pleiades/Metier/MaterialProperty/"+name;
    this->headerFileName += "-pleiades.hh";
    this->srcFileName  = "src/" + name;
    this->srcFileName += "-pleiades.cpp";
    this->headerFile.open(this->headerFileName.c_str());
    if(!this->headerFile){
      string msg("MaterialLawParser::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->headerFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->headerFile.exceptions(ios::badbit|ios::failbit);
    this->srcFile.open(this->srcFileName.c_str());
    if(!this->srcFile){
      string msg("MaterialLawParser::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFile.exceptions(ios::badbit|ios::failbit);
    this->writeHeaderFile(file,name,author,date,description,includes,inputs);
    this->writeSrcFile(file,name,author,date,output,
		       inputs,materialLaws,staticVars,
		       params,function);
  } // end of MFrontPleiadesMaterialPropertyInterface::writeOutputFiles

  void MFrontPleiadesMaterialPropertyInterface::writeHeaderFile(const std::string&,
								const std::string& name,
								const std::string& author,
								const std::string& date,
								const std::string& description,
								const std::string& includes,
								const VariableDescriptionContainer& inputs) {
    using namespace std;
    this->headerFile << "/*!" << endl;
    this->headerFile << "* \\file   " << this->headerFileName  << endl;
    this->headerFile << "* \\brief  " << "this file declares the "
		     << name << " MaterialLaw.\n";
    this->headerFile << "*         File generated by ";
    this->headerFile << MFrontHeader::getVersionName() << " ";
    this->headerFile << "version " << MFrontHeader::getVersionNumber();
    this->headerFile << endl;
    if(!author.empty()){
      this->headerFile << "* \\author " << author << endl;
    }
    if(!date.empty()){
      this->headerFile << "* \\date   " << date       << endl;
    }
    if(!description.empty()){
      this->headerFile << description << endl;
    }
    this->headerFile << " */\n\n";

    this->headerFile << "#ifndef _PLEIADES_"
		     << makeUpperCase(name)
		     << "_HH\n";
    this->headerFile << "#define _PLEIADES_"
		     << makeUpperCase(name)
		     << "_HH\n\n";

    //  writeExportDirectives(this->headerFile);

    this->headerFile << "#include<cmath>\n";
    if(!includes.empty()){
      this->headerFile << includes << endl;
    }
    this->headerFile << "#include \"Pleiades/Metier/MaterialProperty/MaterialPropertyBase.hh\"\n\n";
    this->headerFile << "namespace Pleiades\n{\n";
    //  this->headerFile << "\nstruct MFRONT_SHAREDOBJ " << name << endl;

    this->headerFile << "\nclass " << name << endl;
    this->headerFile << ": public MaterialPropertyBase\n";
    this->headerFile << "{\n\n";
    this->headerFile << "public:\n";
    this->headerFile << name << "();\n";
    this->headerFile << "void declare();\n ";
    this->headerFile << "void compute();\n ";
    this->headerFile << "double operator()(";
    if(!inputs.empty()){
      for(auto p4=inputs.begin();p4!=inputs.end();){
	this->headerFile << "const double& " << p4->name ;
	if((++p4)!=inputs.end()){
	  this->headerFile << ",";
	}
      }
    } else {
      this->headerFile << "void";
    }
    this->headerFile << ") const;\n\n";
    // Disable copy constructor and assignement operator
    this->headerFile << "private:\n\n";
    this->headerFile << "//! Copy constructor (disabled)\n";
    this->headerFile << name << "(const "
		     << name << "&);\n\n";
    this->headerFile << "//! Assignement operator (disabled)\n";
    this->headerFile << name << "&\n"
		     << "operator=(const "
		     << name << "&);\n\n";
    this->headerFile << "}; // end of class " << name << endl << endl;

    this->headerFile << "} // end of namespace Pleiades\n\n";
    this->headerFile << "#endif /* _PLEIADES_"
		     << makeUpperCase(name)
		     << "_HH */\n";
    this->headerFile.close();
  } // end of MFrontPleiadesMaterialPropertyInterface::writeHeaderFile(void)

  void
  MFrontPleiadesMaterialPropertyInterface::writeSrcFile(const std::string& file,
							const std::string& name,
							const std::string& author,
							const std::string& date,
							const VariableDescription& output,
							const VariableDescriptionContainer& inputs,
							const std::vector<std::string>& materialLaws,
							const StaticVariableDescriptionContainer& staticVars,
							const VariableDescriptionContainer& params,
							const LawFunction& function) {
    using namespace std;
    this->srcFile << "/*!" << endl;
    this->srcFile << "* \\file   " << this->srcFileName  << endl;
    this->srcFile << "* \\brief  " << "this file implements the "
		  << name << " MaterialLaw.\n";
    this->srcFile << "*         File generated by ";
    this->srcFile << MFrontHeader::getVersionName() << " ";
    this->srcFile << "version " << MFrontHeader::getVersionNumber();
    this->srcFile << endl;
    if(!author.empty()){
      this->srcFile << "* \\author " << author << endl;
    }
    if(!date.empty()){
      this->srcFile << "* \\date   " << date       << endl;
    }
    this->srcFile << " */\n\n";
    this->srcFile << "#include<string>\n";
    this->srcFile << "#include<cmath>\n\n";
    this->srcFile << "#include\"Pleiades/Examplars/ClassProxy.hh\"\n";
    this->srcFile << "#include\"Pleiades/Exceptions/pexceptions.hh\"\n";
    this->srcFile << "#include\"Pleiades/Metier/Glossaire/pglossaire.hh\"\n";
    this->srcFile << "#include\"Pleiades/Metier/MaterialProperty/" << name << "-pleiades.hh\"\n";
    this->srcFile << "namespace Pleiades {\n\n";
    this->srcFile << name << "::" << name <<"(){}\n\n";

    // declare
    this->srcFile << "void ";
    this->srcFile << name
                  << "::declare() {\n";
    this->srcFile << "using namespace Pleiades::PGlossaire;\n";
    std::string oname;
    if(output.hasGlossaryName()){
      oname = "GlossaireField::"+output.getExternalName();
    } else if (output.hasEntryName()){
      oname = "\"" + output.getExternalName() + "\"";
    } else {
      this->srcFile << "AFF(\"WARNING !!! Glossary name not defined for output field: "
		    << output.name << " in " << name << "\");" << endl;
      oname = "\""+output.name+"\"";
    }
    this->srcFile << "declareField<double>(\"OutputField\", OUT);\n";
    this->srcFile << "setFieldName(\"OutputField\", " << oname << ");\n";

    for(const auto& i : inputs){
      string iname;
      if(i.hasGlossaryName()){
	iname = "GlossaireField::"+i.getExternalName();
      } else if(i.hasEntryName()){
	iname = "\""+i.getExternalName()+"\"";
      } else {
	iname = "\""+i.name+"\"";
      }
      this->srcFile << "declareField<double>(\"" << i.name << "\", " << "IN" << ");\n";
      this->srcFile << "setFieldName(\"" << i.name << "\", " << iname << ");\n";
    }

    this->srcFile << "}\n\n";

    // Compute
    this->srcFile << "void ";
    this->srcFile << name << "::compute(void) {\n";
    this->srcFile << "  apply(*this, getOutputField<double>(\"OutputField\")";
    for(const auto& i : inputs){
      this->srcFile << ", getField<double>(\"" << i.name << "\")";
    }
    this->srcFile << ");\n";
    this->srcFile << "} // end of " << name << "::compute\n\n";
    // Law
    this->srcFile << "double ";
    this->srcFile << name << "::operator()(";
    if(!inputs.empty()){
      for(auto pi=inputs.begin();pi!=inputs.end();){
	this->srcFile << "const double& " << pi->name;
	if((++pi)!=inputs.end()){
	  this->srcFile << ",";
	}
      }
    } else {
      this->srcFile << "void";
    }
    this->srcFile << ") const {\n";
    this->srcFile << "using namespace std;" << endl;
    this->srcFile << "using namespace Pleiades::PGlossaire;\n";

    // material laws
    writeMaterialLaws("MFrontPleiadesMaterialPropertyInterface::writeSrcFile",
		      this->srcFile,materialLaws);
    // static variables
    writeStaticVariables("MFrontPleiadesMaterialPropertyInterface::writeSrcFile",
			 srcFile,staticVars,file);
    if(!params.empty()){
      for(const auto& p : params){
	if(!p.hasAttribute(VariableDescription::defaultValue)){
	  throw(runtime_error("MFrontPleiadesMaterialPropertyInterface::writeCSrcFile: "
			      "internal error (can't find value of "
			      "parameter " + p.name + ")"));
	}
	this->srcFile << "static constexpr double " << p.name << " = "
		      << p.getAttribute<double>(VariableDescription::defaultValue) << ";\n";
      }
    }
    this->srcFile << "double " << output.name << ";\n";
    if((hasPhysicalBounds(inputs))||(hasBounds(inputs))){
      this->srcFile << "#ifndef NO_PLEIADES_BOUNDS_CHECK\n";
    }
    auto get_ename = [](const VariableDescription& v){
      if(v.hasGlossaryName()){
	return "GlossaireField::"+v.getExternalName();
      } else if(v.hasEntryName()){
	return "\""+v.getExternalName()+"\"";
      }
      return "\""+v.name+"\"";
    };    
    if(hasPhysicalBounds(inputs)){
      this->srcFile << "// treating physical bounds\n";
      for(const auto&i : inputs){
	if(!i.hasPhysicalBounds()){
	  continue;
	}
	const auto& b = i.getPhysicalBounds();
	const auto fname = get_ename(i);
	if(b.boundsType==VariableBoundsDescription::LOWER){
	  this->srcFile << "if(" << i.name << " < "<< b.lowerBound << "){\n";
	  this->srcFile << "string msg (\"" << name << "::compute : \");\n"
			<< "msg += " << fname << ";\n"
			<< "msg += \" is below its physical lower bound.\";\n";
	  this->srcFile << "PLEIADES_THROW(msg);\n";
	  this->srcFile << "}\n";
	} else if(b.boundsType==VariableBoundsDescription::UPPER){
	  this->srcFile << "if(" << i.name << " > "<< b.upperBound << "){\n";
	  this->srcFile << "string msg (\"" << name << "::compute : \");\n"
			<< "msg += " << fname << ";\n"
			<< "msg += \" is over its physical upper bound.\";\n";
	  this->srcFile << "PLEIADES_THROW(msg);\n";
	  this->srcFile << "}\n";
	} else {
	  this->srcFile << "if((" << i.name << " < "<< b.lowerBound << ")||"
			<< "(" << i.name << " > "<< b.upperBound << ")){\n";
	  this->srcFile << "if(" << i.name << " < " << b.lowerBound << "){\n";
	  this->srcFile << "string msg (\"" << name << "::compute : \");\n"
			<< "msg += " << fname << ";\n"
			<< "msg += \" is below its physical lower bound.\";\n";
	  this->srcFile << "PLEIADES_THROW(msg);\n";
	  this->srcFile << "} else {\n";
	  this->srcFile << "string msg (\"" << name << "::compute : \");\n"
			<< "msg += " << fname << ";\n"
			<< "msg += \" is over its physical upper bound.\";\n";
	  this->srcFile << "PLEIADES_THROW(msg);\n";
	  this->srcFile << "}\n";
	  this->srcFile << "}\n";
	}
      }
    }
    if(hasBounds(inputs)){
      this->srcFile << "// treating standard bounds\n";
      for(const auto&i : inputs){
	if(!i.hasBounds()){
	  continue;
	}
	const auto fname = get_ename(i);
	const auto& b = i.getBounds();
	if(b.boundsType==VariableBoundsDescription::LOWER){
	  this->srcFile << "if(" << i.name << " < "<< b.lowerBound << "){\n";
	  this->srcFile << "string msg(\"" << name << "::compute : value of\");\n";
	  this->srcFile << "msg += " << fname << ";\n";
	  this->srcFile << "msg += \" is below its lower bound\";\n";
	  this->srcFile << "treatOutOfBounds(msg);\n";
	  this->srcFile << "}\n";
	} else if(b.boundsType==VariableBoundsDescription::UPPER){
	  this->srcFile << "if(" << i.name << " > "<< b.upperBound << "){\n";
	  this->srcFile << "string msg(\"" << name << "::compute : value of\");\n";
	  this->srcFile << "msg += " << fname << ";\n";
	  this->srcFile << "msg += \" is over its upper bound\";\n";
	  this->srcFile << "treatOutOfBounds(msg);\n";
	  this->srcFile << "}\n";
	} else {
	  this->srcFile << "if((" << i.name << " < "<< b.lowerBound << ")||"
			<< "(" << i.name << " > "<< b.upperBound << ")){\n";
	  this->srcFile << "string msg(\"" << name << "::compute : value of\");\n";
	  this->srcFile << "msg += " << fname << ";\n";
	  this->srcFile << "msg += \" is out of bounds\";\n";
	  this->srcFile << "treatOutOfBounds(msg);\n";
	  this->srcFile << "}\n";
	}
      }
    }
    if((hasPhysicalBounds(inputs))||(hasBounds(inputs))){
      this->srcFile << "#endif /* NO_PLEIADES_BOUNDS_CHECK */\n";
    }
    this->srcFile << function.body;
    this->srcFile << "return " << output.name << ";\n";
    this->srcFile << "} // end of " << name << "::law\n\n";

    this->srcFile << "GENERATE_PROXY(IMaterialProperty,";
    this->srcFile << name << ");\n";

    //  this->srcFile << "GENERATE_PROXY2(IFunction,";
    //  for(p  = names.begin();
    //          p != names.end();){
    //    this->srcFile << *p;
    //    if((++p)!=names.end()){
    //      this->srcFile << "::";
    //    }
    //  }
    //  this->srcFile << ",";
    //  for(p  = names.begin();
    //          p != names.end();){
    //    this->srcFile << *p;
    //    if((++p)!=names.end()){
    //      this->srcFile << "_";
    //    }
    //  }
    //  this->srcFile << ");\n\n";
    this->srcFile << "} // end of namespace Pleiades\n";
    this->srcFile.close();
  } // end of MFrontPleiadesMaterialPropertyInterface::writeSrcFile(void)

  MaterialPropertyInterfaceProxy<MFrontPleiadesMaterialPropertyInterface>         pleiadesLawProxy;

} // end of namespace mfront
