/*!
 * \file
 * mfront/tests/properties/interfaces/src/MFrontPleiadesMaterialPropertyInterface-2.0.cxx
 * \brief
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <algorithm>
#include <stdexcept>

#include "TFEL/System/System.hxx"

#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/MaterialPropertyInterfaceProxy.hxx"
#include "MFront/MFrontPleiadesMaterialPropertyInterface-2.0.hxx"

namespace mfront {

  std::string MFrontPleiadesMaterialPropertyInterface::getName() {
    return "pleiades-2.0";
    // TODO: make a MFrontPleiadesMaterialPropertyInterface.cxx.in and use
    // @PACKAGE_VERSION@
  }

  MFrontPleiadesMaterialPropertyInterface::
      MFrontPleiadesMaterialPropertyInterface() = default;

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontPleiadesMaterialPropertyInterface::treatKeyword(
      const std::string& k,
      const std::vector<std::string>& i,
      tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
      const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator) {
    if (std::find(i.begin(), i.end(), "pleiades-2.0") != i.end()) {
      throw(std::runtime_error(
          "MFrontPleiadesMaterialPropertyInterface::treatKeyword: "
          "unsupported key '" +
          k + "'"));
    }
    return {false, current};
  }  // end of treatKeyword

  MFrontPleiadesMaterialPropertyInterface::
      ~MFrontPleiadesMaterialPropertyInterface() = default;

  void MFrontPleiadesMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) {
    const auto lib = "libPleiades" +
                     getMaterialLawLibraryNameBase(mpd.library, mpd.material);
    const auto name = mpd.material.empty() ? mpd.className
                                           : mpd.material + "_" + mpd.className;
    d[lib].ldflags.push_back("-lm");
    d[lib].cppflags.push_back("`pleiades-config --includes`\n");
    d[lib].sources.push_back(name + "-pleiades.cpp");
    d[lib].epts.push_back(name);
    d.headers.push_back(this->headerFileName.substr(8));
  }  // end of MFrontPleiadesMaterialPropertyInterface::getTargetsDescription

  void MFrontPleiadesMaterialPropertyInterface::writeOutputFiles(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) {
    using namespace std;
    using namespace tfel::system;
    const auto& file = fd.fileName;
    const auto& author = fd.authorName;
    const auto& description = fd.description;
    const auto& date = fd.date;
    const auto& material = mpd.material;
    const auto& className = mpd.className;
    const auto& includes = mpd.includes;
    const auto& output = mpd.output;
    const auto& inputs = mpd.inputs;
    const auto& materialLaws = mpd.materialLaws;
    const auto& glossaryNames = mpd.glossaryNames;
    const auto& entryNames = mpd.entryNames;
    const auto& staticVars = mpd.staticVars;
    const auto& params = mpd.parameters;
    const auto& paramValues = mpd.parametersValues;
    const auto& function = mpd.f;
    const auto& bounds = mpd.bounds;
    const auto& physicalBounds = mpd.physicalBounds;
    systemCall::mkdir("include/Pleiades");
    systemCall::mkdir("include/Pleiades/Metier");
    systemCall::mkdir("include/Pleiades/Metier/MaterialProperty");
    const auto name =
        (material.empty()) ? className : material + "_" + className;
    this->headerFileName = "include/Pleiades/Metier/MaterialProperty/" + name;
    this->headerFileName += "-pleiades.hh";
    this->srcFileName = "src/" + name;
    this->srcFileName += "-pleiades.cpp";
    this->headerFile.open(this->headerFileName.c_str());
    if (!this->headerFile) {
      string msg("MaterialLawParser::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->headerFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->headerFile.exceptions(ios::badbit | ios::failbit);
    this->srcFile.open(this->srcFileName.c_str());
    if (!this->srcFile) {
      string msg("MaterialLawParser::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFile.exceptions(ios::badbit | ios::failbit);
    this->writeHeaderFile(file, name, author, date, description, includes,
                          inputs);
    this->writeSrcFile(file, name, author, date, output, inputs, materialLaws,
                       glossaryNames, entryNames, staticVars, params,
                       paramValues, function, bounds, physicalBounds);
  }  // end of MFrontPleiadesMaterialPropertyInterface::writeOutputFiles

  void MFrontPleiadesMaterialPropertyInterface::writeHeaderFile(
      const std::string&,
      const std::string& name,
      const std::string& author,
      const std::string& date,
      const std::string& description,
      const std::string& includes,
      const VarContainer& inputs) {
    using namespace std;
    VarContainer::const_iterator p4;
    this->headerFile << "/*!" << endl;
    this->headerFile << "* \\file   " << this->headerFileName << endl;
    this->headerFile << "* \\brief  "
                     << "this file declares the " << name << " MaterialLaw.\n";
    this->headerFile << "*         File generated by ";
    this->headerFile << MFrontHeader::getVersionName() << " ";
    this->headerFile << "version " << MFrontHeader::getVersionNumber();
    this->headerFile << endl;
    if (!author.empty()) {
      this->headerFile << "* \\author " << author << endl;
    }
    if (!date.empty()) {
      this->headerFile << "* \\date   " << date << endl;
    }
    if (!description.empty()) {
      this->headerFile << description << endl;
    }
    this->headerFile << " */\n\n";

    this->headerFile << "#ifndef _PLEIADES_" << makeUpperCase(name) << "_HH\n";
    this->headerFile << "#define _PLEIADES_" << makeUpperCase(name)
                     << "_HH\n\n";

    //  writeExportDirectives(this->headerFile);

    this->headerFile << "#include<cmath>\n";
    if (!includes.empty()) {
      this->headerFile << includes << endl;
    }
    this->headerFile
        << "#include "
           "\"Pleiades/Metier/MaterialProperty/MaterialPropertyBase.hh\"\n\n";
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
    if (!inputs.empty()) {
      for (p4 = inputs.begin(); p4 != inputs.end();) {
        this->headerFile << "const double& " << p4->name;
        if ((++p4) != inputs.end()) {
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
    this->headerFile << name << "(const " << name << "&);\n\n";
    this->headerFile << "//! Assignement operator (disabled)\n";
    this->headerFile << name << "&\n"
                     << "operator=(const " << name << "&);\n\n";
    this->headerFile << "}; // end of class " << name << endl << endl;

    this->headerFile << "} // end of namespace Pleiades\n\n";
    this->headerFile << "#endif /* _PLEIADES_" << makeUpperCase(name)
                     << "_HH */\n";
    this->headerFile.close();
  }  // end of MFrontPleiadesMaterialPropertyInterface::writeHeaderFile(void)

  void MFrontPleiadesMaterialPropertyInterface::writeSrcFile(
      const std::string& file,
      const std::string& name,
      const std::string& author,
      const std::string& date,
      const std::string& output,
      const VarContainer& inputs,
      const std::vector<std::string>& materialLaws,
      const std::map<std::string, std::string>& glossaryNames,
      const std::map<std::string, std::string>& entryNames,
      const StaticVarContainer& staticVars,
      const std::vector<std::string>& params,
      const std::map<std::string, double>& paramValues,
      const LawFunction& function,
      const std::vector<VariableBoundsDescription>& bounds,
      const std::vector<VariableBoundsDescription>& physicalBounds) {
    using namespace std;
    vector<string>::const_iterator p;
    VarContainer::const_iterator p3;
    map<string, string>::const_iterator p4;
    map<string, double>::const_iterator p7;
    this->srcFile << "/*!" << endl;
    this->srcFile << "* \\file   " << this->srcFileName << endl;
    this->srcFile << "* \\brief  "
                  << "this file implements the " << name << " MaterialLaw.\n";
    this->srcFile << "*         File generated by ";
    this->srcFile << MFrontHeader::getVersionName() << " ";
    this->srcFile << "version " << MFrontHeader::getVersionNumber();
    this->srcFile << endl;
    if (!author.empty()) {
      this->srcFile << "* \\author " << author << endl;
    }
    if (!date.empty()) {
      this->srcFile << "* \\date   " << date << endl;
    }
    this->srcFile << " */\n\n";
    this->srcFile << "#include<string>\n";
    this->srcFile << "#include<cmath>\n\n";
    this->srcFile << "#include\"Pleiades/Examplars/ClassProxy.hh\"\n";
    this->srcFile << "#include\"Pleiades/Exceptions/pexceptions.hh\"\n";
    this->srcFile << "#include\"Pleiades/Metier/Glossaire/pglossaire.hh\"\n";
    this->srcFile << "#include\"Pleiades/Metier/MaterialProperty/" << name
                  << "-pleiades.hh\"\n";
    this->srcFile << "namespace Pleiades {\n\n";
    this->srcFile << name << "::" << name << "(){}\n\n";

    // declare
    this->srcFile << "void ";
    this->srcFile << name << "::declare() {\n";
    this->srcFile << "using namespace Pleiades::PGlossaire;\n";

    string oname;
    if ((p4 = glossaryNames.find(output)) != glossaryNames.end()) {
      oname = "GlossaireField::" + p4->second;
    } else if ((p4 = entryNames.find(output)) != entryNames.end()) {
      oname = "\"" + p4->second + "\"";
    } else {
      this->srcFile
          << "AFF(\"WARNING !!! Glossary name not defined for output field : "
          << output << " in " << name << "\");" << endl;
      oname = "\"" + output + "\"";
    }
    this->srcFile << "declareField<double>(\"OutputField\", OUT);\n";
    this->srcFile << "setFieldName(\"OutputField\", " << oname << ");\n";

    string iname;
    for (p3 = inputs.begin(); p3 != inputs.end(); ++p3) {
      if ((p4 = glossaryNames.find(p3->name)) != glossaryNames.end()) {
        iname = "GlossaireField::" + p4->second;
      } else if ((p4 = entryNames.find(p3->name)) != entryNames.end()) {
        iname = "\"" + p4->second + "\"";
      } else {
        iname = "\"" + p3->name + "\"";
      }

      this->srcFile << "declareField<double>(\"" << p3->name << "\", "
                    << "IN"
                    << ");\n";
      this->srcFile << "setFieldName(\"" << p3->name << "\", " << iname
                    << ");\n";
    }

    this->srcFile << "}\n\n";

    // Compute
    this->srcFile << "void ";
    this->srcFile << name << "::compute(void) {\n";
    this->srcFile << "  apply(*this, getOutputField<double>(\"OutputField\")";
    for (p3 = inputs.begin(); p3 != inputs.end();) {
      this->srcFile << ", getField<double>(\"" << p3->name << "\")";
      ++p3;
    }
    this->srcFile << ");\n";
    this->srcFile << "} // end of " << name << "::compute\n\n";
    // Law
    this->srcFile << "double ";
    this->srcFile << name << "::operator()(";
    if (!inputs.empty()) {
      for (p3 = inputs.begin(); p3 != inputs.end();) {
        this->srcFile << "const double& " << p3->name;
        if ((++p3) != inputs.end()) {
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
                      this->srcFile, materialLaws);
    // static variables
    writeStaticVariables(
        "MFrontPleiadesMaterialPropertyInterface::writeSrcFile", srcFile,
        staticVars, file);
    if (!params.empty()) {
      for (p = params.begin(); p != params.end(); ++p) {
        p7 = paramValues.find(*p);
        if (p7 == paramValues.end()) {
          string msg(
              "MFrontPleiadesMaterialPropertyInterface::writeCSrcFile : ");
          msg += "internal error (can't find value of parameter " + *p + ")";
          throw(runtime_error(msg));
        }
        this->srcFile << "static constexpr double " << *p << " = " << p7->second
                      << ";\n";
      }
    }
    this->srcFile << "double " << output << ";\n";
    if ((!physicalBounds.empty()) || (!bounds.empty())) {
      this->srcFile << "#ifndef NO_PLEIADES_BOUNDS_CHECK\n";
    }
    if (!physicalBounds.empty()) {
      this->srcFile << "// treating physical bounds\n";
      for (auto p6 = physicalBounds.cbegin(); p6 != physicalBounds.cend();
           ++p6) {
        string fname;
        if ((p4 = glossaryNames.find(p6->varName)) != glossaryNames.end()) {
          fname = "GlossaireField::" + p4->second;
        } else if ((p4 = entryNames.find(p6->varName)) != entryNames.end()) {
          fname = "\"" + p4->second + "\"";
        } else {
          fname = "\"" + p6->varName + "\"";
        }
        if (p6->boundsType == VariableBoundsDescription::Lower) {
          this->srcFile << "if(" << p6->varName << " < " << p6->lowerBound
                        << "){\n";
          this->srcFile << "string msg (\"" << name << "::compute : \");\n"
                        << "msg += " << fname << ";\n"
                        << "msg += \" is below its physical lower bound.\";\n";
          this->srcFile << "PLEIADES_THROW(msg);\n";
          this->srcFile << "}\n";
        } else if (p6->boundsType == VariableBoundsDescription::Upper) {
          this->srcFile << "if(" << p6->varName << " > " << p6->upperBound
                        << "){\n";
          this->srcFile << "string msg (\"" << name << "::compute : \");\n"
                        << "msg += " << fname << ";\n"
                        << "msg += \" is over its physical upper bound.\";\n";
          this->srcFile << "PLEIADES_THROW(msg);\n";
          this->srcFile << "}\n";
        } else {
          this->srcFile << "if((" << p6->varName << " < " << p6->lowerBound
                        << ")||"
                        << "(" << p6->varName << " > " << p6->upperBound
                        << ")){\n";
          this->srcFile << "if(" << p6->varName << " < " << p6->lowerBound
                        << "){\n";
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
    if (!bounds.empty()) {
      this->srcFile << "// treating standard bounds\n";
      for (auto p6 = bounds.cbegin(); p6 != bounds.cend(); ++p6) {
        string fname;
        if ((p4 = glossaryNames.find(p6->varName)) != glossaryNames.end()) {
          fname = "GlossaireField::" + p4->second;
        } else if ((p4 = entryNames.find(p6->varName)) != entryNames.end()) {
          fname = "\"" + p4->second + "\"";
        } else {
          fname = "\"" + p6->varName + "\"";
        }
        if (p6->boundsType == VariableBoundsDescription::Lower) {
          this->srcFile << "if(" << p6->varName << " < " << p6->lowerBound
                        << "){\n";
          this->srcFile << "string msg(\"" << name
                        << "::compute : value of\");\n";
          this->srcFile << "msg += " << fname << ";\n";
          this->srcFile << "msg += \" is below its lower bound\";\n";
          this->srcFile << "treatOutOfBounds(msg);\n";
          this->srcFile << "}\n";
        } else if (p6->boundsType == VariableBoundsDescription::Upper) {
          this->srcFile << "if(" << p6->varName << " > " << p6->upperBound
                        << "){\n";
          this->srcFile << "string msg(\"" << name
                        << "::compute : value of\");\n";
          this->srcFile << "msg += " << fname << ";\n";
          this->srcFile << "msg += \" is over its upper bound\";\n";
          this->srcFile << "treatOutOfBounds(msg);\n";
          this->srcFile << "}\n";
        } else {
          this->srcFile << "if((" << p6->varName << " < " << p6->lowerBound
                        << ")||"
                        << "(" << p6->varName << " > " << p6->upperBound
                        << ")){\n";
          this->srcFile << "string msg(\"" << name
                        << "::compute : value of\");\n";
          this->srcFile << "msg += " << fname << ";\n";
          this->srcFile << "msg += \" is out of bounds\";\n";
          this->srcFile << "treatOutOfBounds(msg);\n";
          this->srcFile << "}\n";
        }
      }
    }
    if ((!physicalBounds.empty()) || (!bounds.empty())) {
      this->srcFile << "#endif /* NO_PLEIADES_BOUNDS_CHECK */\n";
    }
    this->srcFile << function.body;
    this->srcFile << "return " << output << ";\n";
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
  }  // end of MFrontPleiadesMaterialPropertyInterface::writeSrcFile(void)

  MaterialPropertyInterfaceProxy<MFrontPleiadesMaterialPropertyInterface>
      pleiadesLawProxy;

}  // end of namespace mfront
