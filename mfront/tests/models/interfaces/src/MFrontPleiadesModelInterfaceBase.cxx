/*!
 * \file  mfront/tests/models/src/MFrontPleiadesModelInterfaceBase.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 17 jun 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cassert>
#include <sstream>
#include <iterator>

#include "TFEL/Raise.hxx"
#include "TFEL/System/System.hxx"

#include "MFront/MFrontHeader.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MFrontPleiadesModelInterfaceBase.hxx"

// fixing a bug on current glibc++ cygwin versions (19/08/2015)
#if defined __CYGWIN__ && (!defined _GLIBCXX_USE_C99)
#include <sstream>
namespace std {
  template <typename T>
  std::string to_string(const T& v) {
    std::ostringstream s;
    s << v;
    return s.str();
  }
}  // namespace std
#endif /* defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99) */

namespace mfront {

  static std::string getDeclarationName(const VariableDescription& v,
                                        const std::string& g) {
    const auto ev = v.getExternalName();
    if (v.hasGlossaryName()) {
      return g + "::" + ev;
    }
    return "\"" + ev + "\"";
  }

  MFrontPleiadesModelInterfaceBase::MFrontPleiadesModelInterfaceBase() {
    this->hasDefaultConstructor = false;
    this->hasSpecializedDomain = false;
  }  // end of
     // MFrontPleiadesModelInterfaceBase::MFrontPleiadesModelInterfaceBase

  void MFrontPleiadesModelInterfaceBase::declareReservedNames(
      std::set<std::string>& v) {
    v.insert({"msg", "oss", "mesh", "nodes", "mesh", "node_k", "gnode_k", "dt",
              "ptr", "ptr2", "domains", "domain", "domainName", "PleiadesError",
              "initialize", "initializeInput", "initializeParameter",
              "initializeInputIFieldDouble"});
  }  // end of MFrontPleiadesModelInterfaceBase::declareReservedNames

  void MFrontPleiadesModelInterfaceBase::openOutputFiles() {
    using namespace std;
    using namespace tfel::system;
    // creating directories
    systemCall::mkdir("include");
    systemCall::mkdir("include/Pleiades");
    systemCall::mkdir("include/Pleiades/PMetier");
    systemCall::mkdir("include/Pleiades/PMetier/PModels");
    systemCall::mkdir("src");
    this->headerFile.open(("include/" + this->headerFileName).c_str());
    tfel::raise_if(!this->headerFile,
                   "MFrontPleiadesModelInterfaceBase::openOutputFiles: "
                   "unable to open file 'include/" +
                       this->headerFileName +
                       "' "
                       "for writing output file.");
    this->srcFile.open(("src/" + this->srcFileName).c_str());
    tfel::raise_if(!this->srcFile,
                   "MFrontPleiadesModelInterfaceBase::openOutputFiles: "
                   "unable to open file 'src/" +
                       this->srcFileName +
                       "' "
                       "for writing output file.");
    this->headerFile.exceptions(std::ios::badbit | std::ios::failbit);
    this->srcFile.exceptions(std::ios::badbit | std::ios::failbit);
  }  // end of MFrontPleiadesModelInterfaceBase::openOutputFiles()

  void MFrontPleiadesModelInterfaceBase::closeOutputFiles() {
    this->headerFile.close();
    this->srcFile.close();
  }  // end of MFrontPleiadesModelInterfaceBase::closeOutputFiles()

  void MFrontPleiadesModelInterfaceBase::writeOutputFiles(
      const FileDescription& fd, const ModelDescription& md) {
    tfel::raise_if(md.className.empty(),
                   "MFrontPleiadesModelInterfaceBase::writeOutputFiles: "
                   "no behaviour name defined.");
    this->setOutputFileNames(md);
    this->generateDomainsList(md);
    this->openOutputFiles();
    this->generateOutputFiles(fd, md);
    this->closeOutputFiles();
  }  // end of MFrontPleiadesModelInterfaceBase::writeOutputFiles

  void MFrontPleiadesModelInterfaceBase::generateOutputFiles(
      const FileDescription& fd, const ModelDescription& md) {
    this->hasDefaultConstructor = true;
    if (this->domains.empty()) {
      this->hasDefaultConstructor = false;
    }
    for (const auto& p : md.parameters) {
      if (!(this->hasDefaultConstructor)) {
        break;
      }
      if (!p.hasAttribute(VariableDescription::defaultValue)) {
        this->hasDefaultConstructor = false;
      }
    }
    this->writeHeaderFile(fd, md);
    this->writeSrcFile(fd, md);
  }  // end of MFrontPleiadesModelInterfaceBase::generateOutputFiles

  void MFrontPleiadesModelInterfaceBase::writeHeaderFile(
      const FileDescription& fd, const ModelDescription& md) {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    StaticVariableDescriptionContainer::const_iterator p2;
    set<string>::iterator p4;
    this->headerFile << "/*!\n"
                     << "* \\file   " << this->headerFileName << '\n'
                     << "* \\brief  "
                     << "this file declares the " << md.className
                     << " PleiadesModel.\n"
                     << "*         File generated by "
                     << MFrontHeader::getVersionName() << " "
                     << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!fd.authorName.empty()) {
      this->headerFile << "* \\author " << fd.authorName << '\n';
    }
    if (!fd.date.empty()) {
      this->headerFile << "* \\date   " << fd.date << '\n';
    }
    if (!fd.description.empty()) {
      this->headerFile << fd.description << '\n';
    }
    this->headerFile << " */\n\n";

    this->headerFile << "#ifndef _PLEIADES_" << makeUpperCase(md.className)
                     << "_HH\n"
                     << "#define _PLEIADES_" << makeUpperCase(md.className)
                     << "_HH\n\n";

    this->headerFile << "#include<string>\n\n"
                     << "#include\"Pleiades/PMetier/IncPMetier.hh\"\n";
    //    this->headerFile <<
    //    "#include\"Pleiades/PMetier/PModels/MEDModel.hh\"\n";
    if (!md.includes.empty()) {
      this->headerFile << md.includes << "\n\n";
    }
    //    writeExportDirectives(this->headerFile);
    this->headerFile << "namespace Pleiades\n{\n"
                     << "namespace PMetier\n{\n"
                     << "namespace PModels\n{\n\n";
    //    this->headerFile << "class MFRONT_SHAREDOBJ " << md.className << '\n';
    this->headerFile << "class " << md.className << '\n' << ": public IModel\n";
    //    this->headerFile << "protected MEDModel\n";
    this->headerFile << "{\n\n"
                     << "public:\n";
    if ((!md.parameters.empty()) || (this->hasSpecializedDomain)) {
      this->headerFile << "//! Constructor to initialize local parameters\n";
      this->headerFile << md.className
                       << "(const Pleiades::PMetier::IArgumentMetier&);\n\n";
    }
    if (this->hasDefaultConstructor) {
      this->headerFile << "//! Default constructor\n";
      this->headerFile << md.className << "();\n\n";
    }
    this->headerFile
        << "bool\n"
        << "initializeOutput(const Pleiades::PMetier::IArgumentMetier&);\n\n"
        << "bool\n"
        << "initializeInput(const Pleiades::PMetier::IArgumentMetier&);\n\n"
        << "bool\n"
        << "initializeParameters(const "
           "Pleiades::PMetier::IArgumentMetier&);\n\n"
        << "bool\n"
        << "initialize(const Pleiades::PMetier::IArgumentMetier&);\n\n"
        << "bool\n"
        << "execute();\n\n"
        << "void\n"
        << "executeInitialPostProcessingTasks(const bool);\n\n"
        << "void\n"
        << "executePostProcessingTasks(const bool);\n\n"
        << "private:\n\n"
        << "typedef double real;\n";
    // Disable copy constructor and assignement operator
    this->headerFile
        << "//! Copy constructor (disabled)\n"
        << md.className << "(const " << md.className << "&);\n\n"
        << "//! Assignement operator (disabled)\n"
        << md.className << "&\n"
        << "operator=(const " << md.className << "&);\n\n"
        << "static const std::vector<int>&\n"
        << "getNodesOfCELLZone(Pleiades::PMetier::PMesh::PtrIMesh Mesh,\n"
        << "const std::string&);\n\n";
    this->writeSpecificPrivateMethodDeclaration(fd, md);
    // Treat functions
    for (const auto& f : md.functions) {
      tfel::raise_if(
          f.modifiedVariables.size() == 0,
          "MFrontPleiadesModelInterfaceBase::writeHeaderFile: "
          "function " +
              f.name +
              " does not modify any variable "
              "(internal error, this shall have been verified long ago).");
      if (getDebugMode()) {
        this->headerFile << "#line " << f.line << " \"" << fd.fileName
                         << "\"\n";
      }
      if (f.modifiedVariables.size() == 1) {
        this->headerFile << "inline double\n";
        this->headerFile << f.name << "(";
      } else {
        this->headerFile << "inline\n";
        this->headerFile << "void\n";
        this->headerFile << f.name << "(";
        for (p4 = f.modifiedVariables.begin();
             p4 != f.modifiedVariables.end();) {
          this->headerFile << "double&";
          if ((++p4) != f.modifiedVariables.end()) {
            this->headerFile << ",";
          }
        }
      }
      if ((f.usedVariables.size() == 0) && (!f.useTimeIncrement)) {
        if (f.modifiedVariables.size() == 1) {
          this->headerFile << "void";
        }
      } else {
        if (f.modifiedVariables.size() != 1) {
          this->headerFile << ",";
        }
        for (p4 = f.usedVariables.begin(); p4 != f.usedVariables.end();) {
          this->headerFile << "const double";
          if ((++p4) != f.usedVariables.end()) {
            this->headerFile << ",";
          }
        }
        if (f.useTimeIncrement) {
          if (f.usedVariables.size() != 0) {
            this->headerFile << ",";
          }
          this->headerFile << "const double";
        }
      }
      this->headerFile << ");\n\n";
    }

    if (!md.staticVars.empty()) {
      for (p2 = md.staticVars.begin(); p2 != md.staticVars.end(); ++p2) {
        if (getDebugMode()) {
          this->headerFile << "#line " << p2->lineNumber << " \"" << fd.fileName
                           << "\"\n";
        }
        this->headerFile << "static constexpr " << p2->type << " " << p2->name
                         << ";\n";
      }
      this->headerFile << '\n';
    }
    for (p = md.outputs.begin(); p != md.outputs.end(); ++p) {
      if (getDebugMode()) {
        this->headerFile << "#line " << p->lineNumber << " \"" << fd.fileName
                         << "\"\n";
      }
      this->headerFile
          << "VARIABLE(Pleiades::PMetier::PField::PtrIFieldDouble,ple"
          << p->name << ")\n\n";
    }
    this->headerFile << '\n';
    for (p = md.inputs.begin(); p != md.inputs.end(); ++p) {
      if (getDebugMode()) {
        this->headerFile << "#line " << p->lineNumber << " \"" << fd.fileName
                         << "\"\n";
      }
      this->headerFile
          << "VARIABLE(Pleiades::PMetier::PField::PtrIFieldDouble,ple"
          << p->name << ")\n\n";
    }
    this->headerFile << '\n' << "private:" << '\n' << '\n';
    for (p = md.constantMaterialProperties.begin();
         p != md.constantMaterialProperties.end(); ++p) {
      if (getDebugMode()) {
        this->headerFile << "#line " << p->lineNumber << " \"" << fd.fileName
                         << "\"\n";
      }
      this->headerFile << p->type << " " << p->name << ";\n\n";
    }
    for (p = md.parameters.begin(); p != md.parameters.end(); ++p) {
      if (getDebugMode()) {
        this->headerFile << "#line " << p->lineNumber << " \"" << fd.fileName
                         << "\"\n";
      }
      this->headerFile << p->type << " " << p->name << ";\n\n";
    }
    this->headerFile << '\n'
                     << "std::set<std::string> domains;\n"
                     << '\n'
                     << "}; // end of class " << md.className << "\n\n"
                     << "} // end of namespace PModels\n\n"
                     << "} // end of namespace PMetier\n\n"
                     << "} // end of namespace Pleiades\n\n"
                     << "#endif /* _PLEIADES_" << makeUpperCase(md.className)
                     << "_HH */\n";
  }  // end of MFrontPleiadesModelInterfaceBase::writeHeaderFile()

  std::string MFrontPleiadesModelInterfaceBase::getGenTypeMethod(
      const std::string& type) const {
    if ((type == "double") || (type == "real")) {
      return "getDouble";
    } else if (type == "string") {
      return "getString";
    }
    tfel::raise(
        "MFrontPleiadesModelInterfaceBase::getGenTypeMethod : "
        "no method associated with type " +
        type);
  }  // end of MFrontPleiadesModelInterfaceBase::getGenTypeMethod

  static std::pair<std::string, unsigned short> getFieldAndDepthFromFieldName(
      const ModelDescription& md, const std::string& n) {
    // this is a silly hack
    auto get = [&n](const VariableDescriptionContainer& vc)
        -> std::pair<std::string, unsigned short> {
      using size_type = unsigned short;
      for (const auto& v : vc) {
        if (v.name == n) {
          return {v.name, 0u};
        }
        const auto d = v.getAttribute<size_type>(VariableDescription::depth, 0);
        for (size_type j = 1; j <= d; ++j) {
          auto fn = v.name + "_" + std::to_string(j);
          if (fn == n) {
            return {v.name, j};
          }
        }
      }
      return {};
    };
    auto r = get(md.outputs);
    if (!r.first.empty()) {
      return r;
    }
    r = get(md.inputs);
    tfel::raise_if(r.first.empty(),
                   "getFieldAndDepthFromFieldName: "
                   "field name '" +
                       n + "' has not been found");
    return r;
  }  // end of getFieldAndDepthFromFieldName

  void MFrontPleiadesModelInterfaceBase::writeSrcFile(
      const FileDescription& fd, const ModelDescription& md) {
    using namespace std;
    map<string, string>::const_iterator p4;
    StaticVariableDescriptionContainer::const_iterator p10;
    set<string>::const_iterator p12;
    this->srcFile << "/*!\n"
                  << "* \\file   " << this->srcFileName << '\n'
                  << "* \\brief  "
                  << "this file implements the " << md.className
                  << " PleiadesModel.\n"
                  << "*         File generated by "
                  << MFrontHeader::getVersionName() << " "
                  << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!fd.authorName.empty()) {
      this->srcFile << "* \\author " << fd.authorName << '\n';
    }
    if (!fd.date.empty()) {
      this->srcFile << "* \\date   " << fd.date << '\n';
    }
    this->srcFile << " */\n\n"
                  << "#include<iostream>\n"
                  << "#include<cmath>\n\n"
                  << "#include\"Pleiades/PMetier/pglossaire.hh\"\n"
                  << "#include\"Pleiades/PExamplars/ClassProxy.hh\"\n";

    this->srcFile << "#include\"" + this->headerFileName + "\"\n\n";

    this->srcFile << "namespace Pleiades\n{\n"
                  << "namespace PMetier\n{\n"
                  << "namespace PModels\n{\n\n";
    // static variables
    if (!md.staticVars.empty()) {
      for (p10 = md.staticVars.begin(); p10 != md.staticVars.end(); ++p10) {
        if (getDebugMode()) {
          this->srcFile << "#line " << p10->lineNumber << " \"" << fd.fileName
                        << "\"\n";
        }
        if (p10->type == "short") {
          this->srcFile << "const short " << md.className << "::" << p10->name
                        << " = " << static_cast<short>(p10->value) << ";\n";
        } else if (p10->type == "ushort") {
          this->srcFile << "const unsigned short " << md.className
                        << "::" << p10->name << " = "
                        << static_cast<unsigned short>(p10->value) << ";\n";
        } else if (p10->type == "int") {
          this->srcFile << "const int " << md.className << "::" << p10->name
                        << " = " << static_cast<int>(p10->value) << ";\n";
        } else if (p10->type == "uint") {
          this->srcFile << "const unsigned int " << md.className
                        << "::" << p10->name << " = "
                        << static_cast<unsigned int>(p10->value) << ";\n";
        } else if (p10->type == "long") {
          this->srcFile << "const long " << md.className << "::" << p10->name
                        << " = " << static_cast<long>(p10->value) << ";\n";
        } else if (p10->type == "ulong") {
          this->srcFile << "const unsigned long " << md.className
                        << "::" << p10->name << " = "
                        << static_cast<unsigned long>(p10->value) << ";\n";
        } else if (p10->type == "float") {
          this->srcFile << "const float " << md.className << "::" << p10->name
                        << " = " << static_cast<float>(p10->value) << ";\n";
        } else if ((p10->type == "double") || (p10->type == "real")) {
          this->srcFile << "const double " << md.className << "::" << p10->name
                        << " = " << static_cast<double>(p10->value) << ";\n";
        } else if (p10->type == "ldouble") {
          this->srcFile << "const long double " << md.className
                        << "::" << p10->name << " = "
                        << static_cast<long double>(p10->value) << ";\n";
        } else {
          tfel::raise(
              "MFrontPleiadesModelInterfaceBase::writeSrcFile: "
              "type " +
              p10->type +
              " is not a supported type "
              "for a static variable. "
              "Supported types are `short`, `ushort`, `int`, "
              "`uint`, `long`, `ulong`,"
              "`float`, `double`, `real` and `ldouble`.\n"
              "Error at line " +
              std::to_string(p10->lineNumber));
        }
      }
      this->srcFile << '\n';
    }

    // getNodesOfCELLZone
    this->srcFile
        << "const std::vector<int>&\n"
        << md.className
        << "::getNodesOfCELLZone(Pleiades::PMetier::PMesh::PtrIMesh Mesh,\n"
        << "                   const std::string& ZoneName)\n"
        << "{\n"
        << "  using namespace std ;\n"
        << "  using namespace Pleiades::PMetier::PMesh ;\n"
        << "  static std::map<std::string,std::vector<int> > nodes;\n"
        << "  if(nodes.find(ZoneName)==nodes.end()){\n"
        << "    int nbel = Mesh->getNumberOfElements( CELL, ZoneName ) ;\n"
        << "    int nbnoel = 0, elem_j = 0, node_i = 0 ;\n"
        << "    std::vector<int> NodesStack ;\n"
        << "    for( int j=0 ; j<nbel ; j++ ){\n"
        << "	  elem_j = Mesh->getElement( CELL, ZoneName, j ) ;\n"
        << "	  nbnoel = Mesh->getNumberOfNodesOfElement( CELL, elem_j ) ;\n"
        << "	  for( int i=0 ; i<nbnoel ; i++){\n"
        << "	    node_i = Mesh->getNodeOfElement( CELL, elem_j, i ) ;\n"
        << "	    NodesStack.push_back(node_i) ;\n"
        << "	  }\n"
        << "    }\n"
        << "    int NbNodes = static_cast<int>(NodesStack.size()) ;\n"
        << "    bool newnode ;\n"
        << "    std::vector<int> NodesOfCELLZone ;\n"
        << "    NodesOfCELLZone.push_back(NodesStack.at(0)) ;\n"
        << "    for( int i=1 ; i<NbNodes ; i++){\n"
        << "	  node_i = NodesStack.at(i) ;\n"
        << "	  newnode = true ;\n"
        << "	  for(size_t j=0 ; j<NodesOfCELLZone.size() ; j++ ){\n"
        << "	    if (NodesOfCELLZone.at(j) == node_i){\n"
        << "	      newnode = false ;\n"
        << "	      break ;\n"
        << "	    }\n"
        << "	  }\n"
        << "	  if (newnode) { NodesOfCELLZone.push_back(node_i) ; }\n"
        << "    }\n"
        << "    nodes[ZoneName] = NodesOfCELLZone ;\n"
        << "  }\n"
        << "  return nodes.at(ZoneName);\n"
        << "}\n\n";

    // Constructors
    if (this->hasDefaultConstructor) {
      this->srcFile << md.className << "::" << md.className << "()\n{\n";
      for (const auto& p : md.parameters) {
        this->writeAssignDefaultValue(p);
      }
      if (this->initializeDefaultDomainListInConstrutor(md)) {
        for (p12 = this->domains.begin(); p12 != this->domains.end(); ++p12) {
          this->srcFile << "this->domains.insert(" << *p12 << ");\n";
        }
      }
      this->srcFile << "} // end of " << md.className << "::" << md.className
                    << "\n\n";
    }
    if ((!md.parameters.empty()) || (this->hasSpecializedDomain)) {
      this->srcFile << "//! Constructor to initialize local parameters\n";
      this->srcFile << md.className << "::" << md.className
                    << "(const Pleiades::PMetier::IArgumentMetier& arg)\n{\n";
      this->srcFile << "using namespace std;\n";
      this->srcFile << "using namespace Pleiades::PUtilitaires;\n";
      this->srcFile << "using namespace Pleiades::PExceptions;\n";
      for (const auto& v : md.parameters) {
        const auto name = getDeclarationName(v, "GlossaireParam");
        this->srcFile << "if(!arg.contains(" << name << ")){\n";
        if (v.hasAttribute(VariableDescription::defaultValue)) {
          this->writeAssignDefaultValue(v);
          this->srcFile << "} else {\n";
        } else {
          this->srcFile << "string msg(\"" << md.className
                        << "::" << md.className << " : \");\n"
                        << "msg += \"can't initialize parameter " << v.name
                        << "\";\n"
                        << "throw(PleiadesError(msg));\n"
                        << "} else {\n";
        }
        this->srcFile << "this->" << v.name << " = arg[" << name << "]."
                      << this->getGenTypeMethod(v.type) << "();\n";
        this->srcFile << "}\n";
      }
      if (this->hasSpecializedDomain) {
        this->srcFile
            << "if((arg.contains(\"domain\"))||(arg.contains(\"domains\"))){\n";
        this->srcFile << "if(arg.contains(\"domain\")){\n";
        this->srcFile << "this->domains.insert(arg[\"domain\"].getString());\n";
        this->srcFile << "}\n";
        this->srcFile << "if(arg.contains(\"domains\")){\n";
        this->srcFile
            << "TabString sdomains = arg[\"domains\"].getTabString();\n";
        this->srcFile << "for(int i=0;i!=sdomains.getTaille();++i){\n";
        this->srcFile << "this->domains.insert(sdomains[i]);\n";
        this->srcFile << "}\n";
        this->srcFile << "}\n";
        this->srcFile << "} else {\n";
        if (!md.domains.empty()) {
          for (p12 = this->domains.begin(); p12 != this->domains.end(); ++p12) {
            this->srcFile << "this->domains.insert(" << *p12 << ");\n";
          }
        } else {
          this->srcFile << "string msg(\"" << md.className
                        << "::" << md.className << " : \");\n";
          this->srcFile << "msg += \"no domain specified\";\n";
          this->srcFile << "throw(PleiadesError(msg));\n";
        }
        this->srcFile << "}\n";
      }
      this->srcFile << "} // end of " << md.className << "::" << md.className
                    << "\n\n";
    }
    // Functions
    for (const auto& f : md.functions) {
      tfel::raise_if(f.modifiedVariables.size() == 0,
                     "MFrontPleiadesModelInterfaceBase::writeSrcFile: "
                     "function " +
                         f.name +
                         " does not modify any variable "
                         "(internal error, this shall have "
                         "been verified long ago).");
      if (getDebugMode()) {
        this->srcFile << "#line " << f.line << " \"" << fd.fileName << "\"\n";
      }
      if (f.modifiedVariables.size() == 1) {
        this->srcFile << "double\n";
        this->srcFile << md.className << "::" << f.name << "(";
      } else {
        this->srcFile << "void\n";
        this->srcFile << md.className << "::" << f.name << "(";
        for (p12 = f.modifiedVariables.begin();
             p12 != f.modifiedVariables.end();) {
          this->srcFile << "double& " << *p12;
          if ((++p12) != f.modifiedVariables.end()) {
            this->srcFile << ",\n";
          }
        }
      }
      if ((f.usedVariables.size() == 0) && (f.useTimeIncrement)) {
        if (f.modifiedVariables.size() == 1) {
          this->srcFile << "void";
        }
      } else {
        if (f.modifiedVariables.size() != 1) {
          this->srcFile << ",";
        }
        for (p12 = f.usedVariables.begin(); p12 != f.usedVariables.end();) {
          this->srcFile << "const double " << *p12;
          if ((++p12) != f.usedVariables.end()) {
            this->srcFile << ",\n";
          }
        }
        if (f.useTimeIncrement) {
          if (f.usedVariables.size() != 0) {
            this->srcFile << ",\n";
          }
          this->srcFile << "const double dt";
        }
      }
      this->srcFile << ")\n";
      this->srcFile << "{\n";
      this->srcFile << "using namespace std;\n";
      writeMaterialLaws(this->srcFile, md.materialLaws);
      if (f.modifiedVariables.size() == 1) {
        this->srcFile << "double " << *(f.modifiedVariables.begin()) << ";\n";
      }
      this->srcFile << f.body;
      if (f.modifiedVariables.size() == 1) {
        this->srcFile << "return " << *(f.modifiedVariables.begin()) << ";\n";
      }
      this->srcFile << "} // end of " << md.className << "::" << f.name
                    << "\n\n";
    }
    // initializeOutput
    this->writeInitializeOutputMethod(md);
    // initializeInput
    this->writeInitializeInputMethod(md);
    // initializeParamaters
    this->writeInitializeParametersMethod(md);
    // initialize
    this->writeInitializeMethod(md);
    // execute
    this->srcFile << "bool\n"
                  << md.className << "::execute(){\n"
                  << "using namespace std;\n"
                  << "using namespace Pleiades::PUtilitaires;\n"
                  << "using namespace Pleiades::PExceptions;\n"
                  << "using namespace Pleiades::PMetier::PTime;\n"
                  << "using namespace Pleiades::PMetier::PMesh;\n\n"
                  << "using namespace Pleiades::PMetier::PGlossaire;\n\n";
    //     for(p=md.outputs.begin();p!=md.outputs.end();++p){
    //       this->srcFile << "MEDField " << p->name
    // 		    << " = *(static_cast<MEDFieldPtr>(this->_ple" << p->name <<
    // "->getMed()));\n";
    //       if((p5=md.depths.find(p->name))!=md.depths.end()){
    // 	for(j=1;j<=p5->second;++j){
    // 	  ostringstream converter;
    // 	  converter << j;
    // 	  string varName = p->name+"_"+converter.str();
    // 	  this->srcFile << "MEDField " << varName
    // 			<< " = *(static_cast<MEDFieldPtr>(this->_ple"
    // 			<< p->name << "->getFieldPreceding("<< j
    // <<")->getMed()));\n";
    // 	}
    //       }
    //     }
    //     for(p=md.inputs.begin();p!=md.inputs.end();++p){
    //       this->srcFile << "MEDField " << p->name
    // 		    << " = *(static_cast<MEDFieldPtr>(this->_ple" << p->name <<
    // "->getMed()));\n";
    //       if((p5=md.depths.find(p->name))!=md.depths.end()){
    // 	for(j=1;j<=p5->second;++j){
    // 	  this->srcFile << "MEDField " << p->name << "_" << j
    // 			<< " = *(static_cast<MEDFieldPtr>(this->_ple"
    // 			<< p->name << "->getFieldPreceding("<< j
    // <<")->getMed()));\n";
    // 	}
    //       }
    //     }
    this->srcFile
        << "const double t  = ITime::GlobalTime->get(Glossaire::Time);\n"
        << "const double dt = ITime::GlobalTime->get(Glossaire::TimeStep);\n"
        << "static_cast<void>(t);  // suppress unused variable warning\n"
        << "static_cast<void>(dt); // suppress unused variable warning\n\n";

    // calling functions
    tfel::raise_if(md.functions.empty(),
                   "MFrontPleiadesModelInterfaceBase::writeSrcFile: "
                   "no function defined");
    tfel::raise_if(md.outputs.size() == 0,
                   "MFrontPleiadesModelInterfaceBase::writeSrcFile: "
                   "no output defined");
    this->srcFile << "PtrIMesh mesh = this->_ple" << md.outputs.front().name
                  << "->getMesh();\n"
                  << "std::set<string>::const_iterator ptr;\n"
                  << "if(this->domains.empty()){\n"
                  << "string msg(\"" << md.className << "::execute : \");\n"
                  << "msg += \"no domain specified\";\n"
                  << "throw(PleiadesError(msg));\n"
                  << "}\n";
    //     this->srcFile << "map<string,vector<int> >::const_iterator ptr2;\n";
    //     this->srcFile << "vector<int>::const_iterator ptr3;\n";
    this->srcFile
        << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n";
    this->buildDomainName();
    this->srcFile << "const vector<int>& nodes = " << md.className
                  << "::getNodesOfCELLZone(mesh,*ptr);\n";
    //     this->srcFile << "ptr2 = this->nodesOfZones.find(*ptr);\n";
    //     this->srcFile << "if(ptr2==this->nodesOfZones.end()){\n";
    //     this->srcFile << "string msg(\"" << md.className << "::execute :
    //     \");\n"; this->srcFile << "msg += \"internal error (no group named
    //     '\"+*ptr+\"' defined\";\n"; this->srcFile <<
    //     "throw(PleiadesError(msg));\n"; this->srcFile << "}\n"; this->srcFile
    //     << "const auto& nodes = ptr2->second;\n";
    this->srcFile
        << "for(int "
           "node_k=0;node_k!=static_cast<int>(nodes.size());++node_k){\n"
        << "const int gnode_k=nodes[node_k];\n";
    this->writeFunctionCall(md, md.functions);
    this->srcFile << "}\n"
                  << "}\n"
                  << "return true;\n"
                  << "} // end of " << md.className << "::execute\n\n";

    this->srcFile
        << "void\n"
        << md.className
        << "::executeInitialPostProcessingTasks(const bool)\n{} // end of "
        << md.className << "::executeInitialPostProcessingTasks\n\n";

    this->srcFile << "void\n"
                  << md.className
                  << "::executePostProcessingTasks(const bool)\n{} // end of "
                  << md.className << "::executePostProcessingTasks\n\n";

    if (!md.parameters.empty()) {
      this->srcFile << "using Pleiades::PExamplars::TemplateModelClassProxy;\n";
      this->srcFile << "TemplateModelClassProxy<" << md.className << "> proxy"
                    << md.className << "(\"" << md.className << "\");\n";
    }
    if (this->hasDefaultConstructor) {
      this->srcFile << "GENERATE_PROXY(IModel," << md.className << ");\n\n";
    }
    this->srcFile << "} // end of namespace PModels\n\n"
                  << "} // end of namespace PMetier\n\n"
                  << "} // end of namespace Pleiades\n";
  }  // end of MFrontPleiadesModelInterfaceBase::writeSrcFile()

  void MFrontPleiadesModelInterfaceBase::getTargetsDescription(
      TargetsDescription& td, const ModelDescription& md) {
    std::string lib;
    if (md.library.empty()) {
      if (!md.material.empty()) {
        lib = "lib" + this->getApplicationName() + md.material + "Models";
      } else {
        lib = "lib" + this->getApplicationName() + "MaterialModels";
      }
    } else {
      lib = md.library;
    }
    auto& l = td.getLibrary(lib);
    l.sources.push_back(this->srcFileName);
    l.epts.push_back(md.className);
    td.headers.push_back(this->headerFileName);
    l.cppflags.push_back("`pleiades-config --includes`\n");
    l.ldflags.push_back("`pleiades-config --libs` -lm");
  }  // end of MFrontPleiadesModelInterfaceBase::getTargetsDescription

  void MFrontPleiadesModelInterfaceBase::writeAssignDefaultValue(
      const VariableDescription& v) {
    if (v.type == "string") {
      this->srcFile << "this->" << v.name << " = "
                    << v.getAttribute<std::string>(
                           VariableDescription::defaultValue)
                    << ";\n";
    } else if ((v.type == "double") || (v.type == "real")) {
      this->srcFile << "this->" << v.name << " = "
                    << v.getAttribute<double>(VariableDescription::defaultValue)
                    << ";\n";
    } else {
      tfel::raise(
          "MFrontPleiadesModelInterfaceBase::writeAssignDefaultValue: "
          "type " +
          v.type + " is not supported.\n");
    }
  }  // end of MFrontPleiadesModelInterfaceBase::writeAssignDefaultValue

  void MFrontPleiadesModelInterfaceBase::writeGetGlobalParameter(
      const VariableDescription& v, const ModelDescription& md) {
    const auto name = getDeclarationName(v, "GlossaireParam");
    this->srcFile << "if(!arg.contains(" << name << ")){\n";
    if (v.hasAttribute(VariableDescription::defaultValue)) {
      this->writeAssignDefaultValue(v);
    } else {
      this->srcFile << "string msg(\"" << md.className
                    << "::initializeParameters : \");\n"
                    << "msg += \"can't initialize parameter '" << v.name
                    << "' using '\";\n"
                    << "msg += " << name << ";\n"
                    << "msg += '\\'';\n"
                    << "throw(PleiadesError(msg));\n";
    }
    this->srcFile << "} else {\n"
                  << "this->" << v.name << " = arg[" << name << "]."
                  << this->getGenTypeMethod(v.type) << "();\n"
                  << "}\n";
  }  // end of MFrontPleiadesModelInterfaceBase::writeGetGlobalParameter

  void MFrontPleiadesModelInterfaceBase::writeGetConstantMaterialProperty(
      const VariableDescription& v, const ModelDescription& md) {
    const auto name = getDeclarationName(v, "GlossaireParam");
    this->srcFile << "if(arg.contains(" << name << ")){\n"
                  << "this->" << v.name << " = arg[" << name << "]."
                  << this->getGenTypeMethod(v.type) << "();\n"
                  << "} else {\n";
    if (v.hasAttribute(VariableDescription::defaultValue)) {
      this->writeAssignDefaultValue(v);
    } else {
      this->srcFile << "string msg(\"" << md.className
                    << "::initializeParameters : \");\n"
                    << "msg += \"can't initialize constant material property  '"
                    << v.name << "' using '\";\n"
                    << "msg += " << name << ";\n"
                    << "msg += '\\'';\n"
                    << "throw(PleiadesError(msg));\n";
    }
    this->srcFile << "}\n";
  }  // end of
     // MFrontPleiadesModelInterfaceBase::writeGetConstantMaterialProperty

  void MFrontPleiadesModelInterfaceBase::writeFunctionCall(
      const ModelDescription& md,
      const std::vector<ModelDescription::Function>& functions) {
    auto add = [](std::set<std::string>& fn,
                  const VariableDescriptionContainer& vc) {
      for (const auto& v : vc) {
        fn.insert(v.name);
        const auto d =
            v.getAttribute<unsigned short>(VariableDescription::depth, 0);
        for (unsigned short i = 1; i <= d; ++i) {
          const auto vn = v.name + "_" + std::to_string(i);
          fn.insert(vn);
        }
      }
    };
    using namespace std;
    set<string> usedVariables;
    set<string> modifiedVariables;
    set<string> fieldNames;
    vector<ModelDescription::Function>::const_iterator p;
    set<string>::const_iterator p2;
    set<string>::const_iterator p3;
    add(fieldNames, md.outputs);
    add(fieldNames, md.inputs);
    // generating temporaries for variables of thoses fields
    for (p = functions.begin(); p != functions.end(); ++p) {
      for (p2 = p->usedVariables.begin(); p2 != p->usedVariables.end(); ++p2) {
        usedVariables.insert(*p2);
      }
      for (p2 = p->modifiedVariables.begin(); p2 != p->modifiedVariables.end();
           ++p2) {
        modifiedVariables.insert(*p2);
      }
    }
    for (p2 = modifiedVariables.begin(); p2 != modifiedVariables.end(); ++p2) {
      if ((p3 = usedVariables.find(*p2)) != usedVariables.end()) {
        usedVariables.erase(*p3);
      }
    }
    // declaring temporaries
    for (p2 = usedVariables.begin(); p2 != usedVariables.end(); ++p2) {
      if (fieldNames.find(*p2) != fieldNames.end()) {
        const auto& fn = getFieldAndDepthFromFieldName(md, *p2);
        if (fn.second == 0) {
          this->srcFile << "const double tmp_" << *p2 << " = this->_ple"
                        << fn.first << "->getValueType(gnode_k,0);\n";
        } else {
          this->srcFile << "const double tmp_" << *p2 << " = this->_ple"
                        << fn.first << "->getValuePrecedingType(" << fn.second
                        << ",gnode_k);\n";
        }
      }
    }
    for (p2 = modifiedVariables.begin(); p2 != modifiedVariables.end(); ++p2) {
      if (fieldNames.find(*p2) != fieldNames.end()) {
        const auto& fn = getFieldAndDepthFromFieldName(md, *p2);
        tfel::raise_if(fn.second != 0,
                       "MFrontPleiadesModelInterfaceBase::writeFunctionCall: "
                       "variable '" +
                           *p2 + "' shall not be modified");
        this->srcFile << "double tmp_" << *p2 << " = this->_ple" << fn.first
                      << "->getValueType(gnode_k,0);\n";
      }
    }
    for (p = functions.begin(); p != functions.end(); ++p) {
      if (p->modifiedVariables.size() == 1) {
        this->srcFile << "tmp_" << *(p->modifiedVariables.begin()) << " = ";
        this->srcFile << p->name << "(";
        for (p2 = p->usedVariables.begin(); p2 != p->usedVariables.end();) {
          if (fieldNames.find(*p2) != fieldNames.end()) {
            this->srcFile << "tmp_" << *p2;
          } else {
            this->srcFile << *p2;
          }
          if ((++p2) != p->usedVariables.end()) {
            this->srcFile << ",";
          }
        }
      } else {
        this->srcFile << "this->" << p->name << "(";
        for (p2 = p->modifiedVariables.begin();
             p2 != p->modifiedVariables.end();) {
          this->srcFile << "tmp_" << *p2;
          if ((++p2) != p->modifiedVariables.end()) {
            this->srcFile << ",";
          }
        }
        if (!p->usedVariables.empty()) {
          this->srcFile << ",";
        }
        for (p2 = p->usedVariables.begin(); p2 != p->usedVariables.end();) {
          if (fieldNames.find(*p2) != fieldNames.end()) {
            this->srcFile << "tmp_" << *p2;
          } else {
            this->srcFile << *p2;
          }
          if ((++p2) != p->usedVariables.end()) {
            this->srcFile << ",";
          }
        }
      }
      if (p->useTimeIncrement) {
        this->srcFile << ",dt";
      }
      this->srcFile << ");\n";
    }
    for (p2 = modifiedVariables.begin(); p2 != modifiedVariables.end(); ++p2) {
      const auto& fn = getFieldAndDepthFromFieldName(md, *p2);
      tfel::raise_if(fn.second != 0,
                     "MFrontPleiadesModelInterfaceBase::writeFunctionCall: "
                     "variable '" +
                         *p2 + "' shall not be modified");
      this->srcFile << "this->_ple" << fn.first << "->setValueType(gnode_k,tmp_"
                    << *p2 << ",0);\n";
    }
  }

  void MFrontPleiadesModelInterfaceBase::generateDomainsList(
      const ModelDescription& md) {
    using namespace std;
    set<string>::const_iterator p;
    this->hasSpecializedDomain = true;
    if (md.domains.empty()) {
      this->hasDefaultConstructor = false;
    } else {
      for (p = md.domains.begin(); p != md.domains.end(); ++p) {
        this->domains.insert('\"' + *p + '\"');
      }
    }
  }  // end of MFrontPleiadesModelInterfaceBase::generateDomainsList

  void MFrontPleiadesModelInterfaceBase::writeInitializeMethod(
      const ModelDescription& md) {
    this->srcFile
        << "bool\n"
        << md.className
        << "::initialize(const Pleiades::PMetier::IArgumentMetier& arg)"
        << "{\n"
        << "this->initializeOutput(arg);\n"
        << "this->initializeInput(arg);\n"
        << "this->initializeParameters(arg);\n"
        << "return true;\n"
        << "} // end of " << md.className << "::initialize\n\n";
  }  // end of MFrontPleiadesModelInterfaceBase::writeInitializeMethod

  void MFrontPleiadesModelInterfaceBase::writeInitializeParametersMethod(
      const ModelDescription& md) {
    this->srcFile << "bool\n"
                  << md.className
                  << "::initializeParameters(const "
                     "Pleiades::PMetier::IArgumentMetier& arg)"
                  << "{\n"
                  << "using namespace std;\n"
                  << "using namespace Pleiades::PUtilitaires;\n"
                  << "using namespace Pleiades::PExceptions;\n"
                  << "using namespace Pleiades::PMetier::PGlossaire;\n";
    for (auto p = md.constantMaterialProperties.cbegin();
         p != md.constantMaterialProperties.cend(); ++p) {
      this->writeGetConstantMaterialProperty(*p, md);
    }
    this->srcFile << "return true;\n"
                  << "} // end of " << md.className
                  << "::initializeParameters\n\n";
  }  // end of MFrontPleiadesModelInterfaceBase::writeInitializeParametersMethod

  void MFrontPleiadesModelInterfaceBase::writeInitializeOutputMethod(
      const ModelDescription& md) {
    this->srcFile
        << "bool\n"
        << md.className
        << "::initializeOutput(const Pleiades::PMetier::IArgumentMetier& arg)"
        << "{\n"
        << "using namespace std;\n"
        << "using namespace Pleiades::PUtilitaires;\n"
        << "using namespace Pleiades::PExceptions;\n"
        << "using namespace Pleiades::PMetier::PGlossaire;\n";
    for (const auto& v : md.outputs) {
      const auto name = getDeclarationName(v, "GlossaireField");
      this->srcFile << "if(!this->initializeOutputIFieldDouble(arg," << name;
      this->srcFile << ",\nthis->_ple" << v.name << ",\"\",";
      if (v.hasAttribute(VariableDescription::initialValue)) {
        this->srcFile << v.getAttribute<double>(
            VariableDescription::initialValue);
      } else {
        this->srcFile << "0";
      }
      this->srcFile << ")){\n"
                    << "string msg(\"" << md.className
                    << "::initializeOutput : \");\n"
                    << "msg += \"can't initialize output field '\";\n"
                    << "msg += " << name << ";\n"
                    << "msg += \"\'\";\n"
                    << "throw(PleiadesError(msg));\n"
                    << "}\n";
    }
    this->srcFile << "return true;\n"
                  << "} // end of " << md.className
                  << "::initializeOutput \n\n";
  }  // end of MFrontPleiadesModelInterfaceBase::writeInitializeOutputMethod

  void MFrontPleiadesModelInterfaceBase::writeInitializeInputMethod(
      const ModelDescription& md) {
    this->srcFile
        << "bool\n"
        << md.className
        << "::initializeInput(const Pleiades::PMetier::IArgumentMetier& arg)"
        << "{\n"
        << "using namespace std;\n"
        << "using namespace Pleiades::PUtilitaires;\n"
        << "using namespace Pleiades::PExceptions;\n"
        << "using namespace Pleiades::PMetier::PGlossaire;\n";
    for (const auto& v : md.inputs) {
      const auto name = getDeclarationName(v, "GlossaireField");
      this->srcFile << "if(!this->initializeInputIFieldDouble(this->_ple"
                    << v.name << "," << name << ",\"\",arg)){\n"
                    << "string msg(\"" << md.className
                    << "::initializeInput : \");\n"
                    << "msg += \"can't initialize input field " << v.name
                    << "\";\n"
                    << "throw(PleiadesError(msg));\n"
                    << "}\n";
    }
    this->srcFile << '\n'
                  << "return true;\n"
                  << "} // end of " << md.className << "::initializeInput\n\n";
  }  // end of MFrontPleiadesModelInterfaceBase::writeInitializeInputMethod

  void MFrontPleiadesModelInterfaceBase::writeSpecificPrivateMethodDeclaration(
      const FileDescription&, const ModelDescription&) {
  }  // end of
     // MFrontPleiadesModelInterfaceBase::writeSpecificPrivateMethodDeclaration

  bool
  MFrontPleiadesModelInterfaceBase::initializeDefaultDomainListInConstrutor(
      const ModelDescription&) const {
    return true;
  }  // end of
     // MFrontPleiadesModelInterfaceBase::initializeDefaultDomainListInConstrutor

}  // end of namespace mfront
