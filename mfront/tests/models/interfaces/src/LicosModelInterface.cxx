/*!
 * \file   mfront/tests/models/src/LicosModelInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>

#include <stdexcept>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <cctype>

#include "TFEL/Raise.hxx"
#include "TFEL/System/System.hxx"

#include "MFront/MFrontHeader.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/StaticVariableDescription.hxx"
#include "MFront/ModelDSLCommon.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/ModelDescription.hxx"
#include "MFront/ModelInterfaceProxy.hxx"
#include "MFront/LicosModelInterface.hxx"

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

static constexpr unsigned short
    TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY = 8;

namespace mfront {

  static std::string getDeclaration(const VariableDescription& v) {
    const auto& en = v.getExternalName();
    if (v.hasGlossaryName()) {
      return "Glossary::" + en;
    }
    return "\"" + en + "\"";
  }  // end of getDeclaration

  static std::string getVariableType(const std::string& v) {
    if (v == "string") {
      return "std::string";
    }
    return v;
  }

  static std::string getLibraryName(const ModelDescription& md) {
    std::string a = "@Application@";
    if (md.library.empty()) {
      if (!md.material.empty()) {
        return a + md.material + "Models";
      } else {
        return a + "MaterialModels";
      }
    }
    return md.library;
  }  // end of getLibraryName

  static bool isInputVariable(const ModelDescription& md,
                              const std::string& v) {
    return md.inputs.contains(v);
  }  // end of isInputVariable

  static std::pair<std::string, unsigned short> decomposeVariableName(
      const ModelDescription& md, const std::string& n) {
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
                   "decomposeVariableName: "
                   "field name '" +
                       n + "' has not been found");
    return r;
  }  // end of decomposeVariableName

  MFrontModelInterface::MFrontModelInterface()
      : hasDefaultConstructor(false) {
  }  // end of MFrontModelInterface::MFrontModelInterface

  void MFrontModelInterface::declareReservedNames(std::set<std::string>& v) {
    v.insert("first");
    v.insert("saveddata");
    v.insert("msaveddata");
    v.insert("activestate");
    v.insert("dt");
    v.insert("boost");
    v.insert("field");
    v.insert("support");
    v.insert("values");
    v.insert("results");
    v.insert("domains");
    v.insert("requirement");
    v.insert("requirementManager");
    v.insert("md");
    v.insert("ptr");
    v.insert("ptr2");
    v.insert("ptr3");
    v.insert("tmp");
    v.insert("outputsDepths");
    v.insert("outputsInitialValues");
    v.insert("inputsDepths");
    v.insert("ValidParametersNames");
    v.insert("computeMaterialProperties");
    v.insert("apply");
    v.insert("sapply");
    v.insert("UNField");
    v.insert("Field");
    v.insert("NFieldHolder");
    v.insert("MTFieldManager");
    v.insert("mm");
    v.insert("initialize");
    v.insert("initializeOutput");
    v.insert("initializeOutputsVariablesDepths");
    v.insert("initializeOutputsVariablesInitialValues");
    v.insert("initializeInputsVariablesDepths");
    v.insert("initializeConstantMaterialProperties");
    v.insert("constantMaterialProperties");
    v.insert("declareRequirements");
    v.insert("resolveDependencies");
    v.insert("save");
    v.insert("restore");
    v.insert("execute");
    v.insert("executeInitialPostProcessingTasks");
    v.insert("executePostProcessingTasks");
    v.insert("getName");
    v.insert("data");
  }

  void MFrontModelInterface::openOutputFiles() {
    // creating directories
    tfel::system::systemCall::mkdir("include");
    tfel::system::systemCall::mkdir("include/Pleiades");
    tfel::system::systemCall::mkdir("include/Pleiades/Model");
    tfel::system::systemCall::mkdir("src");
    this->headerFile.open(this->headerFileName);
    tfel::raise_if(!this->headerFile,
                   "MFrontModelInterface::openOutputFiles: "
                   "unable to open '" +
                       this->headerFileName +
                       "' "
                       "for writing output file.");
    this->srcFile.open(this->srcFileName);
    tfel::raise_if(!this->srcFile,
                   "MFrontModelInterface::openOutputFiles: "
                   "unable to open " +
                       this->srcFileName +
                       "' "
                       "for writing output file.");
    this->headerFile.exceptions(std::ios::badbit | std::ios::failbit);
    this->srcFile.exceptions(std::ios::badbit | std::ios::failbit);
  }  // end of MFrontModelInterface::openOutputFiles()

  void MFrontModelInterface::closeOutputFiles() {
    if (this->headerFile.is_open()) {
      this->headerFile.close();
    }
    if (this->srcFile.is_open()) {
      this->srcFile.close();
    }
  }  // end of MFrontModelInterface::closeOutputFiles()

  void MFrontModelInterface::generateOutputFiles(const FileDescription& fd,
                                                 const ModelDescription& md) {
    this->hasDefaultConstructor = true;
    if (md.domains.empty()) {
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
    // sanity checks
    for (const auto& f : md.functions) {
      tfel::raise_if(f.modifiedVariables.size() == 0,
                     "MFrontModelInterface::generateOutputFiles: "
                     "function '" +
                         f.name + "' does not modify any output");
    }
    for (const auto& v : md.outputs) {
      auto found = false;
      for (auto p2 = md.functions.begin();
           (p2 != md.functions.end()) && (!found); ++p2) {
        if (p2->modifiedVariables.find(v.name) != p2->modifiedVariables.end()) {
          found = true;
        }
      }
      tfel::raise_if(!found,
                     "MFrontModelInterface::generateOutputFiles: "
                     "output '" +
                         v.name + "' is not modified by any function");
    }
    this->writeHeaderFile(fd, md);
    this->headerFile.close();
    this->writeSrcFile(fd, md);
    this->srcFile.close();
  }  // end of MFrontModelInterface::generateOutputFiles()

  void MFrontModelInterface::writeHeaderFile(const FileDescription& fd,
                                             const ModelDescription& md) {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    StaticVariableDescriptionContainer::const_iterator p2;
    set<string>::iterator p4;
    set<unsigned short> applyHeaders;
    set<unsigned short>::const_iterator p18;
    unsigned short i;
    this->headerFile << "/*!\n"
                     << "* \\file   " << this->headerFileName << '\n'
                     << "* \\brief  "
                     << "this file declares the " << md.className << " Model.\n"
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

    this->headerFile << "#include<string>\n\n";
    for (const auto& f : md.functions) {
      if ((f.modifiedVariables.size() == 1) &&
          (f.usedVariables.size() <
           TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)) {
        applyHeaders.insert(
            static_cast<unsigned short>(f.usedVariables.size()));
      }
    }
    for (p18 = applyHeaders.begin(); p18 != applyHeaders.end(); ++p18) {
      if (*p18 <= 5) {
        this->headerFile << "#include\"Pleiades/Field/FieldSApply" << *p18
                         << ".hxx\"\n";
      }
    }
    this->headerFile
        << "#include\"Pleiades/Model/ModelBase.hxx\"\n"
        << "#include\"Pleiades/Model/StateVariableDescription.hxx\"\n\n";
    if (!md.includes.empty()) {
      this->headerFile << md.includes << "\n\n";
    }
    writeExportDirectives(this->headerFile);
    this->headerFile
        << "namespace pleiades\n{\n\n"
        << "namespace model\n{\n\n"
        << "struct MFRONT_SHAREDOBJ " << md.className << '\n'
        << ": public ModelBase\n"
        << "{\n\n"
        << "//! Default constructor\n"
        << md.className << "(pleiades::mesh::SMeshManager&,\n"
        << "pleiades::field::SFieldManager&,\n"
        << "pleiades::time::SClock&,\n"
        << "pleiades::loading::SLoadingManager&,\n"
        << "pleiades::signal::SSignalManager&);\n\n"
        << "//! Constructor to initialize local parameters\n"
        << md.className
        << "(const std::map<std::string,pleiades::parser::Data>&,\n"
        << "pleiades::mesh::SMeshManager&,\n"
        << "pleiades::field::SFieldManager&,\n"
        << "pleiades::time::SClock&,\n"
        << "pleiades::loading::SLoadingManager&,\n"
        << "pleiades::signal::SSignalManager&);\n\n"
        << "std::string\ngetName() const;\n\n"
        << "void initialize(const pleiades::parser::DataManager&);\n\n"
        << "void "
           "declareRequirements(pleiades::coupling::SRequirementManager&);\n\n"
        << "void "
           "resolveRequirements(pleiades::coupling::SRequirementManager&);\n\n"
        << "void initializeOutput(pleiades::coupling::Requirement&);\n\n"
        << "void execute();\n\n"
        << "void save(const std::string&,\n"
        << "pleiades::parser::DataMap&);\n\n"
        << "void restore(const pleiades::time::ptime,\n"
        << "const std::string&,\n"
        << "const pleiades::parser::DataMap&);\n\n"
        << "void executeInitialPostProcessingTasks(const bool);\n\n"
        << "void executePostProcessingTasks(const bool);\n\n"
        << "~" << md.className << "();\n\n"
        << "private:\n\n";
    // Disable copy constructor and assignement operator
    this->headerFile << "//! Copy constructor (disabled)\n"
                     << md.className << "(const " << md.className << "&);\n\n"
                     << "//! Assignement operator (disabled)\n"
                     << md.className << "&\n"
                     << "operator=(const " << md.className << "&);\n\n";
    for (const auto& f : md.functions) {
      tfel::raise_if(f.modifiedVariables.size() == 0,
                     "MFrontModelInterface::writeHeaderFile: "
                     "function " +
                         f.name +
                         " does not modify "
                         "any variable (internal error, this shall "
                         "have been verified long ago).");
      if (getDebugMode()) {
        this->headerFile << "#line " << f.line << " \"" << fd.fileName
                         << "\"\n";
      }
      this->headerFile << "struct " << f.name << "\n";
      if ((f.modifiedVariables.size() == 1) &&
          (f.usedVariables.size() <
           TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)) {
        if (f.usedVariables.size() <= 5) {
          this->headerFile << ": public pleiades::field::ScalarFieldFunctor"
                           << f.usedVariables.size() << '\n';
        }
      }
      this->headerFile << "{\n\n";
      if ((f.modifiedVariables.size() == 1) &&
          (f.usedVariables.size() <
           TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)) {
        if (f.usedVariables.size() > 5) {
          this->headerFile << "typedef pleiades::field::real Result;\n";
          for (i = 1, p4 = f.usedVariables.begin(); p4 != f.usedVariables.end();
               ++i, ++p4) {
            this->headerFile << "typedef pleiades::field::real Arg" << i
                             << ";\n";
          }
        }
        this->headerFile << "\nResult\n";
        this->headerFile << "operator()(";
        if (f.usedVariables.size() == 0) {
          this->headerFile << "void";
        } else {
          for (i = 1, p4 = f.usedVariables.begin(); p4 != f.usedVariables.end();
               ++i) {
            this->headerFile << "const Arg" << i;
            if ((++p4) != f.usedVariables.end()) {
              this->headerFile << ",\n";
            }
          }
        }
      } else {
        if (f.usedVariables.size() == 0) {
          this->headerFile
              << "void operator()(std::vector<pleiades::field::real>&";
        } else {
          this->headerFile
              << "void operator()(std::vector<pleiades::field::real>&,\n"
              << "const std::vector<pleiades::field::real>&";
        }
      }
      this->headerFile << ") const;\n\n";
      this->headerFile << "private:\n\n";
      this->headerFile << "friend class " << md.className << ";\n\n";
      for (p4 = f.parameters.begin(); p4 != f.parameters.end(); ++p4) {
        p = MFrontModelInterface::findVariableDescription(md.parameters, *p4);
        if (getDebugMode()) {
          this->headerFile << "#line " << p->lineNumber << " \"" << fd.fileName
                           << "\"\n";
        }
        this->headerFile << getVariableType(p->type) << " " << p->name << ";\n";
      }
      for (p4 = f.constantMaterialProperties.begin();
           p4 != f.constantMaterialProperties.end(); ++p4) {
        p = MFrontModelInterface::findVariableDescription(
            md.constantMaterialProperties, *p4);
        if (getDebugMode()) {
          this->headerFile << "#line " << p->lineNumber << " \"" << fd.fileName
                           << "\"\n";
        }
        this->headerFile << getVariableType(p->type) << " " << p->name << ";\n";
      }
      if (f.useTimeIncrement) {
        this->headerFile << "pleiades::field::real dt;\n";
      }
      this->headerFile << "}; // end of struct " << f.name << "\n\n";
    }
    if (!md.staticVars.empty()) {
      for (p2 = md.staticVars.begin(); p2 != md.staticVars.end(); ++p2) {
        if (getDebugMode()) {
          this->headerFile << "#line " << p2->lineNumber << " \"" << fd.fileName
                           << "\"\n";
        }
        this->headerFile << "static constexpr " << getVariableType(p2->type)
                         << " " << p2->name << " = " << p2->value << ";"
                         << '\n';
      }
      this->headerFile << '\n';
    }
    this->headerFile << "std::vector<std::string> domains;\n";
    for (p = md.parameters.begin(); p != md.parameters.end(); ++p) {
      if (getDebugMode()) {
        this->headerFile << "#line " << p->lineNumber << " \"" << fd.fileName
                         << "\"\n";
      }
      this->headerFile << getVariableType(p->type) << " " << p->name << ";\n";
    }
    this->headerFile << "void initializeOutputsVariablesDepths();\n"
                     << "void initializeOutputsVariablesInitialValues(const "
                        "pleiades::parser::DataManager&);\n"
                     << "void initializeInputsVariablesDepths();";
    if (!md.constantMaterialProperties.empty()) {
      this->headerFile << "\nvoid initializeConstantMaterialProperties(const "
                          "pleiades::parser::DataManager&);\n\n";
    } else {
      this->headerFile << "\n\n";
    }
    i = 0;
    for (const auto& f : md.functions) {
      tfel::raise_if(
          f.modifiedVariables.size() == 0,
          "MFrontModelInterface::writeHeaderFile: "
          "function '" +
              f.name +
              "' does not modify any variable "
              "(internal error, this shall have been verified long ago).");
      if (getDebugMode()) {
        this->headerFile << "#line " << f.line << " \"" << fd.fileName
                         << "\"\n";
      }
      this->headerFile << f.name << " functor" << i << ";\n";
      ++i;
    }
    this->headerFile
        << "std::map<std::string,std::map<std::string,unsigned short> > "
           "outputsDepths;\n"
        << "std::map<std::string,std::map<std::string,std::shared_ptr<"
           "StateVariableDescription>>> outputsInitialValues;\n"
        << "std::map<std::string,std::map<std::string,unsigned short> > "
           "inputsDepths;\n";
    if (!md.constantMaterialProperties.empty()) {
      this->headerFile << "std::map<std::string,std::map<std::string,pleiades::"
                          "field::real> > constantMaterialProperties;\n";
    }
    this->headerFile << "}; // end of struct " << md.className << "\n\n"
                     << "} // end of namespace model\n\n"
                     << "} // end of namespace pleiades\n\n"
                     << "#endif /* _PLEIADES_" << makeUpperCase(md.className)
                     << "_HH */\n";
  }  // end of MFrontModelInterface::writeHeaderFile()

  std::string MFrontModelInterface::getGenTypeGetMethod(
      const std::string& type) const {
    if (type == "bool") {
      return "get<bool>";
    } else if (type == "real") {
      return "get<real>";
    } else if (type == "double") {
      return "get<double>";
    } else if (type == "string") {
      return "get<string>";
    }
    tfel::raise(
        "MFrontModelInterface::getGenTypeGetMethod : "
        "no method associated with type '" +
        type + '\'');
  }  // end of MFrontModelInterface::getGenTypeGetMethod

  std::string MFrontModelInterface::getGenTypeIsMethod(
      const std::string& type) const {
    if (type == "real") {
      return "is<real>";
    } else if (type == "bool") {
      return "is<bool>";
    } else if (type == "double") {
      return "is<double>";
    } else if (type == "string") {
      return "is<string>";
    }
    tfel::raise(
        "MFrontModelInterface::getGenTypeIsMethod : "
        "no method associated with type '" +
        type + '\'');
  }  // end of MFrontModelInterface::isGenTypeMethod

  VariableDescriptionContainer::const_iterator
  MFrontModelInterface::findVariableDescription(
      const VariableDescriptionContainer& v, const std::string& n) {
    for (auto p = v.begin(); p != v.end(); ++p) {
      if (p->name == n) {
        return p;
      }
    }
    tfel::raise(
        "MFrontModelInterface::findVariableDescription : "
        "no variable named '" +
        n + "' found");
  }  // MFrontModelInterface::findVariableDescription

  void MFrontModelInterface::writeSrcFile(const FileDescription& fd,
                                          const ModelDescription& md) {
    std::set<unsigned short> applyHeaders;
    unsigned short specializedParametersNumber;
    unsigned int i;
    this->srcFile << "/*!" << '\n'
                  << "* \\file   " << this->srcFileName << '\n'
                  << "* \\brief  "
                  << "this file implements the " << md.className << "  model.\n"
                  << "*         File generated by "
                  << MFrontHeader::getVersionName() << " "
                  << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!fd.authorName.empty()) {
      this->srcFile << "* \\author " << fd.authorName << '\n';
    }
    if (!fd.date.empty()) {
      this->srcFile << "* \\date   " << fd.date << '\n';
    }
    this->srcFile
        << " */\n\n"
        << "#include<iostream>\n"
        << "#include<stdexcept>\n"
        << "#include<sstream>\n"
        << "#include<cmath>\n\n"
        << "#include\"Pleiades/Global.hxx\"\n"
        << "#include\"Pleiades/OutOfBoundsPolicy.hxx\"\n"
        << "#include\"Pleiades/Parser/Data.hxx\"\n"
        << "#include\"Pleiades/Parser/DataManager.hxx\"\n"
        << "#include\"Pleiades/Glossary/Glossary.hxx\"\n"
        << "#include\"Pleiades/Time/SClock.hxx\"\n"
        << "#include\"Pleiades/Model/MaterialPropertyDescription.hxx\"\n"
        << "#include\"Pleiades/Coupling/Requirement.hxx\"\n"
        << "#include\"Pleiades/Coupling/SRequirementManager.hxx\"\n";
    // Functions
    bool requiresNFieldHolder = false;
    for (const auto& f : md.functions) {
      if ((f.modifiedVariables.size() == 1) &&
          (f.usedVariables.size() <
           TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)) {
        applyHeaders.insert(
            static_cast<unsigned short>(f.usedVariables.size()));
      } else {
        requiresNFieldHolder = true;
      }
    }
    for (const auto& ah : applyHeaders) {
      if (ah > 5) {
        this->srcFile << "#include\"Pleiades/Field/FieldApply" << ah
                      << ".hxx\"\n";
      }
    }
    if (requiresNFieldHolder) {
      this->srcFile << "#include\"Pleiades/Field/VNField.hxx\"\n"
                    << "#include\"Pleiades/Field/NFieldHolder.hxx\"\n";
    }
    this->srcFile << "#include\"Pleiades/Model/MaterialDescription.hxx\"\n"
                  << "#include\"Pleiades/Model/"
                     "UniformScalarStateVariableDescription.hxx\"\n"
                  << "#include\"Pleiades/Model/IModelFactory.hxx\"\n"
                  << "#include\"Pleiades/Model/" << md.className
                  << "-@application@.hxx\"\n\n"
                  << "namespace pleiades\n{\n\n"
                  << "namespace model\n{\n\n";
    // Functors
    for (const auto& f : md.functions) {
      // operator()
      if (getDebugMode()) {
        this->srcFile << "#line " << f.line << " \"" << fd.fileName << "\"\n";
      }
      if ((f.modifiedVariables.size() == 1) &&
          (f.usedVariables.size() <
           TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)) {
        this->srcFile << "pleiades::field::real\n"
                      << md.className << "::" << f.name << "::operator()(";
        if (f.usedVariables.size() == 0) {
          if (f.modifiedVariables.size() == 1) {
            this->srcFile << "void";
          }
        } else {
          if (f.modifiedVariables.size() != 1) {
            this->srcFile << ",\n";
          }
          for (auto puv = f.usedVariables.begin();
               puv != f.usedVariables.end();) {
            this->srcFile << "const pleiades::field::real " << *puv;
            if ((++puv) != f.usedVariables.end()) {
              this->srcFile << ",\n";
            }
          }
        }
      } else {
        this->srcFile << "void " << md.className << "::" << f.name
                      << "::operator()("
                      << "std::vector<pleiades::field::real>& results";
        if (f.usedVariables.size() != 0) {
          this->srcFile
              << ",\nconst std::vector<pleiades::field::real>& values";
        }
      }
      this->srcFile << ") const\n"
                    << "{\n"
                    << "using namespace std;\n"
                    << "using pleiades::field::real;\n";
      if ((f.modifiedVariables.size() == 1) &&
          (f.usedVariables.size() <
           TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)) {
        this->srcFile << "real " << *(f.modifiedVariables.begin()) << ";\n";
      } else {
        i = 0;
        for (const auto& v : f.usedVariables) {
          this->srcFile << "const real " << v << " = values[" << i << "];\n";
          ++i;
        }
        i = 0;
        for (const auto& v : f.modifiedVariables) {
          this->srcFile << "real " << v << " = results[" << i << "];\n";
          ++i;
        }
      }
      writeMaterialLaws(this->srcFile, md.materialLaws);
      i = 0;
      for (auto puv = f.usedVariables.begin(); puv != f.usedVariables.end();
           ++puv, ++i) {
        const auto& v = [&md, puv] {
          for (const auto& vo : md.outputs) {
            if (*puv == vo.name) {
              return vo;
            }
          }
          for (const auto& vi : md.inputs) {
            if (*puv == vi.name) {
              return vi;
            }
          }
          tfel::raise(
              "MFrontModelInterface::writeSrcFile: "
              "no input or output variable "
              "named '" +
              *puv + "'");
        }();
        if ((v.hasBounds()) || (v.hasPhysicalBounds())) {
          this->srcFile << "#ifndef NO_PLEIADES_BOUNDS_CHECK\n";
        }
        if (v.hasPhysicalBounds()) {
          const auto& bd = v.getPhysicalBounds();
          this->srcFile << "// checking " << v.name << " physical bounds\n";
          if ((bd.boundsType == VariableBoundsDescription::LOWER) ||
              (bd.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
            this->srcFile << "if(" << v.name << " < " << bd.lowerBound
                          << "){\n";
            this->srcFile << "ostringstream msg;\n"
                          << "msg << \"" << md.className << "::" << f.name
                          << "::operator() : \"\n"
                          << "<< \"input '" << v.name
                          << "' is below its physical lower bound (\" << "
                          << v.name << " << \"<" << bd.lowerBound
                          << ").\\n\";\n";
            this->srcFile << "throw(runtime_error(msg.str()));\n";
            this->srcFile << "}\n";
          }
          if ((bd.boundsType == VariableBoundsDescription::UPPER) ||
              (bd.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
            this->srcFile << "if(" << v.name << " > " << bd.upperBound
                          << "){\n";
            this->srcFile << "ostringstream msg;\n"
                          << "msg << \"" << md.className << "::" << f.name
                          << "::operator() : \"\n"
                          << "<< \"input '" << v.name
                          << "' is over its physical upper bound (\" << "
                          << v.name << " << \" > " << bd.upperBound
                          << ").\\n\";\n";
            this->srcFile << "throw(runtime_error(msg.str()));\n";
            this->srcFile << "}\n";
          }
        }
        if (v.hasBounds()) {
          const auto& bd = v.getBounds();
          this->srcFile << "// checking " << v.name << " bounds\n";
          if ((bd.boundsType == VariableBoundsDescription::LOWER) ||
              (bd.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
            this->srcFile << "if(" << v.name << " < " << bd.lowerBound
                          << "){\n";
            this->srcFile << "if(pleiades::getOutOfBoundsPolicy()!= "
                             "pleiades::NO_OUT_OF_BOUNDS_POLICY){\n";
            this->srcFile << "ostringstream msg;\n"
                          << "msg << \"" << md.className << "::" << f.name
                          << "::operator() : \"\n"
                          << "<< \"input '" << v.name
                          << "' is below its lower bound (\" << " << v.name
                          << " << \"<" << bd.lowerBound << ").\\n\";\n";
            this->srcFile << "switch(pleiades::getOutOfBoundsPolicy()){\n";
            this->srcFile << "case pleiades::STRICT_OUT_OF_BOUNDS_POLICY :\n";
            this->srcFile << "throw(runtime_error(msg.str()));\n";
            this->srcFile << "break;\n";
            this->srcFile << "case pleiades::WARNING_OUT_OF_BOUNDS_POLICY :\n";
            this->srcFile << "pleiades::printWarning(msg.str());\n";
            this->srcFile << "break;\n";
            this->srcFile << "case pleiades::NO_OUT_OF_BOUNDS_POLICY :\n";
            this->srcFile << "break;\n";
            this->srcFile << "}\n";
            this->srcFile << "}\n";
            this->srcFile << "}\n";
          }
          if ((bd.boundsType == VariableBoundsDescription::UPPER) ||
              (bd.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
            this->srcFile << "if(" << v.name << " > " << bd.upperBound
                          << "){\n";
            this->srcFile << "if(pleiades::getOutOfBoundsPolicy()!= "
                             "pleiades::NO_OUT_OF_BOUNDS_POLICY){\n";
            this->srcFile << "ostringstream msg;\n"
                          << "msg << \"" << md.className << "::" << f.name
                          << "::operator() : \"\n"
                          << "<< \"input '" << v.name
                          << "' is over its upper bound (\" << " << v.name
                          << " << \" > " << bd.upperBound << ").\\n\";\n";
            this->srcFile << "switch(pleiades::getOutOfBoundsPolicy()){\n";
            this->srcFile << "case pleiades::STRICT_OUT_OF_BOUNDS_POLICY :\n";
            this->srcFile << "throw(runtime_error(msg.str()));\n";
            this->srcFile << "break;\n";
            this->srcFile << "case pleiades::WARNING_OUT_OF_BOUNDS_POLICY :\n";
            this->srcFile << "pleiades::printWarning(msg.str());\n";
            this->srcFile << "break;\n";
            this->srcFile << "case pleiades::NO_OUT_OF_BOUNDS_POLICY :\n";
            this->srcFile << "break;\n";
            this->srcFile << "}\n";
            this->srcFile << "}\n";
            this->srcFile << "}\n";
          }
        }
        if ((v.hasBounds()) || (v.hasPhysicalBounds())) {
          this->srcFile << "#endif /* NO_PLEIADES_BOUNDS_CHECK */\n";
        }
      }
      this->srcFile << f.body;
      if ((f.modifiedVariables.size() == 1) &&
          (f.usedVariables.size() <
           TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)) {
        this->srcFile << "return " << *(f.modifiedVariables.begin()) << ";\n";
      } else {
        i = 0;
        for (auto puv = f.modifiedVariables.begin();
             puv != f.modifiedVariables.end(); ++puv) {
          this->srcFile << "results[" << i << "] = " << *puv << ";\n";
          ++i;
        }
      }
      this->srcFile << "} // end of " << md.className << "::" << f.name
                    << "::operator()\n\n";
    }
    // Constructors
    if (this->hasDefaultConstructor) {
      this->srcFile << md.className << "::" << md.className
                    << "(pleiades::mesh::SMeshManager& m,\n"
                    << "pleiades::field::SFieldManager& mf,\n"
                    << "pleiades::time::SClock& c,\n"
                    << "pleiades::loading::SLoadingManager& l,\n"
                    << "pleiades::signal::SSignalManager& s)\n"
                    << ": ModelBase(m,mf,c,l,s)\n{\n"
                    << "using namespace std;\n"
                    << "using namespace pleiades::glossary;\n"
                    << "vector<string> tmp;\n"
                    << "vector<string>::const_iterator ptr;\n"
                    << "vector<string>::const_iterator ptr2;\n";
      for (const auto& p : md.parameters) {
        this->writeAssignDefaultValue(p);
      }
      tfel::raise_if(md.domains.empty(),
                     "MFrontModelInterface::writeSrcFile: "
                     "no domain defined (internal error, "
                     "we shall not reach this point)");
      for (const auto& d : md.domains) {
        this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,\"" << d
                      << "\");\n"
                      << "for(ptr=tmp.begin();ptr!=tmp.end();++ptr){\n"
                      << "if(find(this->domains.begin(),this->domains.end(),*"
                         "ptr)!=this->domains.end()){\n"
                      << "string msg(\"" << md.className << "::" << md.className
                      << " : \");\n"
                      << "msg += \"domain '\"+*ptr+\"' multiply defined\";\n"
                      << "throw(runtime_error(msg));\n"
                      << "}\n"
                      << "this->domains.push_back(*ptr);\n"
                      << "}\n";
      }
      this->srcFile << "this->initializeOutputsVariablesDepths();\n"
                    << "this->initializeInputsVariablesDepths();\n"
                    << "} // end of " << md.className << "::" << md.className
                    << "\n\n";
    } else {
      this->srcFile << md.className << "::" << md.className
                    << "(pleiades::mesh::SMeshManager& m,\n"
                    << "pleiades::field::SFieldManager& mf,\n"
                    << "pleiades::time::SClock& c,\n"
                    << "pleiades::loading::SLoadingManager& l,\n"
                    << "pleiades::signal::SSignalManager& s)\n"
                    << ": ModelBase(m,mf,c,l,s)\n{\n"
                    << "using namespace std;\n"
                    << "string msg(\"" << md.className << "::" << md.className
                    << " : \");\n"
                    << "msg += \"no domain defined\";\n"
                    << "throw(runtime_error(msg));\n"
                    << "} // end of " << md.className << "::" << md.className
                    << "\n\n";
    }
    this->srcFile
        << "//! Constructor to initialize local parameters\n"
        << md.className << "::" << md.className
        << "(const std::map<std::string,pleiades::parser::Data>& data,\n"
        << "pleiades::mesh::SMeshManager& m,\n"
        << "pleiades::field::SFieldManager& mf,\n"
        << "pleiades::time::SClock& c,\n"
        << "pleiades::loading::SLoadingManager& l,\n"
        << "pleiades::signal::SSignalManager& s)\n"
        << ": ModelBase(m,mf,c,l,s)\n{\n"
        << "using namespace std;\n"
        << "using namespace pleiades::field;\n"
        << "using namespace pleiades::glossary;\n"
        << "using namespace pleiades::parser;\n"
        << "using namespace pleiades::coupling;\n"
        << "static constexpr string ValidParametersNames[] = {"
        << "\"domain\",\"domains\",\"Active\",\n"
        << "\"ActivatingEvents\",\"DesactivatingEvents\"";
    specializedParametersNumber = 5u;
    for (const auto& p : md.parameters) {
      const auto name = getDeclaration(p);
      this->srcFile << ",\n" << name;
      ++(specializedParametersNumber);
    }
    // domain specialisation
    this->srcFile << "};\n"
                  << "vector<string> tmp;\n"
                  << "map<string,Data>::const_iterator ptr;\n"
                  << "vector<string>::const_iterator ptr2;\n"
                  << "vector<string>::const_iterator ptr3;\n"
                  << "for(ptr=data.begin();ptr!=data.end();++ptr){\n"
                  << "if(find(ValidParametersNames,ValidParametersNames+"
                  << specializedParametersNumber
                  << ",ptr->first)==ValidParametersNames+"
                  << specializedParametersNumber << "){\n"
                  << "string msg(\"" << md.className << "::" << md.className
                  << " : \");\n"
                  << "msg += \"unknown parameter '\"+ptr->first+\"'\";\n"
                  << "throw(runtime_error(msg));\n"
                  << "}\n"
                  << "}\n"
                  << "ActivableObjectBase::handleSpecialisationData(data);\n";
    for (const auto& p : md.parameters) {
      const auto name = getDeclaration(p);
      this->srcFile << "ptr = data.find(" << name << ");\n"
                    << "if(ptr==data.end()){\n";
      if (p.hasAttribute(VariableDescription::defaultValue)) {
        this->writeAssignDefaultValue(p);
      } else {
        this->srcFile << "string msg(\"" << md.className << "::" << md.className
                      << " : \");\n"
                      << "msg += \"can't initialize parameter " << p.name
                      << "\";\n"
                      << "throw(runtime_error(msg));\n";
      }
      this->srcFile << "} else {\n"
                    << "if(!ptr->second." << this->getGenTypeIsMethod(p.type)
                    << "()){\n"
                    << "string msg(\"" << md.className << "::" << md.className
                    << " : \");\n"
                    << "msg += \"wrong type for parameter '" << p.name
                    << "' (expected a '" + p.type + "')\";\n"
                    << "throw(runtime_error(msg));\n"
                    << "}\n"
                    << "this->" << p.name << " = ptr->second."
                    << this->getGenTypeGetMethod(p.type) << "();\n"
                    << "}\n";
    }
    this->srcFile << "if((data.find(\"domain\")!=data.end())||(data.find("
                     "\"domains\")!=data.end())){\n"
                  << "if(data.find(\"domain\")!=data.end()){\n"
                  << "ptr = data.find(\"domain\");\n"
                  << "this->smanager.getMatchingMaterialsNames(tmp,ptr->second."
                     "get<string>());\n"
                  << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n"
                  << "if(find(this->domains.begin(),this->domains.end(),*ptr2)!"
                     "=this->domains.end()){\n"
                  << "string msg(\"" << md.className << "::" << md.className
                  << ": \");\n"
                  << "msg += \"domain '\"+*ptr2+\"' multiply defined\";\n"
                  << "throw(runtime_error(msg));\n"
                  << "}\n"
                  << "this->domains.push_back(*ptr2);\n"
                  << "}\n"
                  << "}\n"
                  << "if(data.find(\"domains\")!=data.end()){\n"
                  << "ptr = data.find(\"domains\");\n"
                  << "if(ptr!=data.end()){\n"
                  << "if(!ptr->second.is<vector<string> >()){\n"
                  << "string msg(\"" << md.className << "::" << md.className
                  << " : \");\n"
                  << "msg += \"invalid type for parameter 'domains'\";\n"
                  << "throw(runtime_error(msg));\n"
                  << "}\n"
                  << "for(ptr3=ptr->second.get<vector<string> >().begin();\n"
                  << "ptr3!=ptr->second.get<vector<string> >().end();++ptr3){\n"
                  << "this->smanager.getMatchingMaterialsNames(tmp,*ptr3);\n"
                  << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n"
                  << "if(find(this->domains.begin(),this->domains.end(),*ptr2)!"
                     "=this->domains.end()){\n"
                  << "string msg(\"" << md.className << "::" << md.className
                  << ": \");\n"
                  << "msg += \"domain '\"+*ptr2+\"' multiply defined\";\n"
                  << "throw(runtime_error(msg));\n"
                  << "}\n"
                  << "this->domains.push_back(*ptr2);\n"
                  << "}\n"
                  << "}\n"
                  << "}\n"
                  << "}\n"
                  << "} else {\n";
    if (md.domains.empty()) {
      this->srcFile << "string msg(\"" << md.className << "::" << md.className
                    << ": \");\n"
                    << "msg += \"no domain defined\";\n"
                    << "throw(runtime_error(msg));\n";
    } else {
      for (const auto& d : md.domains) {
        this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,\"" << d
                      << "\");\n"
                      << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n"
                      << "if(find(this->domains.begin(),this->domains.end(),*"
                         "ptr2)!=this->domains.end()){\n"
                      << "string msg(\"" << md.className << "::" << md.className
                      << " : \");\n"
                      << "msg += \"domain '\"+*ptr2+\"' multiply defined\";\n"
                      << "throw(runtime_error(msg));\n"
                      << "}\n"
                      << "this->domains.push_back(*ptr2);\n"
                      << "}\n";
      }
    }
    this->srcFile << "}\n"
                  << "this->initializeOutputsVariablesDepths();\n"
                  << "this->initializeInputsVariablesDepths();\n"
                  << "} // end of " << md.className << "::" << md.className
                  << "\n\n";
    this->writeInitializeOutputsVariablesDepths(md);
    this->writeInitializeInputsVariablesDepths(md);
    // getName
    this->srcFile << "std::string\n"
                  << md.className << "::getName() const\n"
                  << "{\n"
                  << "return \"" << md.className << "\";\n"
                  << "}\n\n";
    // initialize
    this->srcFile << "void " << md.className
                  << "::initialize(const pleiades::parser::DataManager& data)"
                  << "{\n"
                  << "using namespace std;\n"
                  << "using namespace pleiades::field;\n"
                  << "using namespace pleiades::glossary;\n"
                  << "this->initializeOutputsVariablesInitialValues(data);\n";
    if (!md.constantMaterialProperties.empty()) {
      this->srcFile << "this->initializeConstantMaterialProperties(data);\n";
    }
    // initializing functor members
    i = 0;
    for (const auto& f : md.functions) {
      const auto functor = "functor" + std::to_string(i);
      for (const auto& p : f.parameters) {
        this->srcFile << "this->" << functor << '.' << p << " = this->" << p
                      << ";\n";
      }
      ++i;
    }
    this->srcFile << "} // end of " << md.className << "::initialize\n\n";
    this->writeInitializeOutputsVariablesInitialValues(md);
    if (!md.constantMaterialProperties.empty()) {
      this->writeInitializeConstantMaterialProperties(md);
    }
    // declareRequirements
    this->srcFile
        << "void " << md.className
        << "::declareRequirements(pleiades::coupling::SRequirementManager& "
           "requirementManager)\n"
        << "{\n"
        << "using namespace std;\n"
        << "using namespace pleiades::coupling;\n"
        << "using namespace pleiades::glossary;\n"
        << "map<string,map<string,unsigned short> >::const_iterator ptr;\n"
        << "map<string,unsigned short>::const_iterator ptr2;\n"
        << "for(ptr=this->inputsDepths.begin();ptr!=this->inputsDepths.end();++"
           "ptr){\n"
        << "auto& tmp = ptr->second;\n"
        << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n"
        << "requirementManager.addRequirement(static_cast<IModel&>(*this),\n"
        << "ptr->first,ptr2->first,\n"
        << "Requirement::SCALAR,\n"
        << "Requirement::MANDATORY,"
        << "ptr2->second);\n"
        << "}\n"
        << "}\n"
        << "} // end of " << md.className << "::declareRequirements\n\n";
    // resolveDependencies
    this->srcFile
        << "void " << md.className
        << "::resolveRequirements(pleiades::coupling::SRequirementManager& "
           "requirementManager)\n"
        << "{\n"
        << "using namespace std;\n"
        << "using namespace pleiades::glossary;\n"
        << "using namespace pleiades::coupling;\n"
        << "using namespace pleiades::field;\n"
        << "map<string,map<string,unsigned short> >::const_iterator ptr;\n"
        << "map<string,unsigned short>::const_iterator ptr2;\n"
        << "for(ptr=this->outputsDepths.begin();ptr!=this->outputsDepths.end();"
           "++ptr){\n"
        << "auto& tmp = ptr->second;\n"
        << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n"
        << "if(this->outputsInitialValues.find(ptr->first)==this->"
           "outputsInitialValues.end()){\n"
        << "requirementManager.setRequirementProvider(static_cast<IModel&>(*"
           "this),\n"
        << "ptr->first,ptr2->first,Requirement::SCALAR,false);\n"
        << "} else {\n"
        << "if(this->outputsInitialValues[ptr->first].find(ptr2->first)!=this->"
           "outputsInitialValues[ptr->first].end()){\n"
        << "requirementManager.setRequirementProvider(static_cast<IModel&>(*"
           "this),\n"
        << "ptr->first,ptr2->first,Requirement::SCALAR,true);\n"
        << "} else {\n"
        << "requirementManager.setRequirementProvider(static_cast<IModel&>(*"
           "this),\n"
        << "ptr->first,ptr2->first,Requirement::SCALAR,false);\n"
        << "}\n"
        << "}\n"
        << "}\n"
        << "}\n"
        << "} // end of " << md.className << "::resolveRequirements\n\n";
    // initializeOutput
    this->srcFile
        << "void " << md.className
        << "::initializeOutput(pleiades::coupling::Requirement& requirement)\n"
        << "{\n"
        << "using namespace std;\n"
        << "using namespace pleiades::glossary;\n"
        << "using namespace pleiades::field;\n"
        << "map<string,map<string,unsigned short> >::const_iterator ptr;\n"
        << "map<string,unsigned short>::const_iterator ptr2;\n"
        << "map<string,map<string,shared_ptr<StateVariableDescription> > "
           ">::const_iterator ptr3;\n"
        << "map<string,shared_ptr<StateVariableDescription> >::const_iterator "
           "ptr4;\n"
        << "ptr = this->outputsDepths.find(requirement.getName());\n"
        << "if(ptr==this->outputsDepths.end()){\n"
        << "return ModelBase::initializeOutput(requirement);\n"
        << "}\n"
        << "ptr2 = ptr->second.find(requirement.getLocation());\n"
        << "if(ptr2==ptr->second.end()){\n"
        << "return ModelBase::initializeOutput(requirement);\n"
        << "}\n"
        << "ptr3 = this->outputsInitialValues.find(requirement.getName());\n"
        << "if(ptr3==this->outputsInitialValues.end()){\n"
        << "string msg(\"" << md.className << "::initializeOutput: \");\n"
        << "msg += \"no initial value for '\"+requirement.getName()+\"'\";\n"
        << "throw(runtime_error(msg));\n"
        << "}\n"
        << "ptr4 = ptr3->second.find(requirement.getLocation());\n"
        << "if(ptr4==ptr3->second.end()){\n"
        << "string msg(\"" << md.className << "::initializeOutput: \");\n"
        << "msg += \"no initial value for '\"+requirement.getName()+\"'\";\n"
        << "throw(runtime_error(msg));\n"
        << "}\n"
        << "ptr4->second->initialize(this->smanager.getMTFieldManager("
           "requirement.getLocation()),\n"
        << "requirement,ptr2->second);\n"
        << "} // end of " << md.className << "::initializeOutput\n\n";
    // save
    this->srcFile
        << "void " << md.className
        << "::save(const std::string& directory,\npleiades::parser::DataMap& "
           "saveddata)\n"
        << "{\n"
        << "using namespace std;\n"
        << "using namespace pleiades::parser;\n"
        << "vector<string>::const_iterator ptr;\n"
        << "ModelBase::save(directory,saveddata);\n"
        << "auto& msaveddata = saveddata.get<DataMap>(this->getName(),true);\n"
        << "auto& activestate = msaveddata.get<map<string,string> "
           ">(\"ActiveState\",true);\n"
        << "if(this->isActive()){\n"
        << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n"
        << "activestate.insert({*ptr,\"true\"});\n"
        << "}\n"
        << "} else {\n"
        << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n"
        << "activestate.insert({*ptr,\"false\"});\n"
        << "}\n"
        << "}\n"
        << "}\n\n"
        << "void " << md.className
        << "::restore(const pleiades::time::ptime t,\n"
        << "const std::string& directory,\n"
        << "const pleiades::parser::DataMap& saveddata)\n"
        << "{\n"
        << "using namespace std;\n"
        << "using namespace pleiades::parser;\n"
        << "vector<string>::const_iterator ptr;\n"
        << "map<string,string>::const_iterator ptr2;\n"
        << "ModelBase::restore(t,directory,saveddata);\n"
        << "if(!saveddata.contains(this->getName())){\n"
        << "return;"
        << "}\n"
        << "const auto& msaveddata = saveddata.get<DataMap>(this->getName());\n"
        << "if(!msaveddata.contains(\"ActiveState\")){\n"
        << "return;\n"
        << "}\n"
        << "bool first     = true;\n"
        << "bool dactivate = true;\n"
        << "const auto& activestate = msaveddata.get<map<string,string> "
           ">(\"ActiveState\");\n"
        << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n"
        << "ptr2 = activestate.find(*ptr);\n"
        << "if(ptr2!=activestate.end()){\n"
        << "if(ptr2->second==\"true\"){\n"
        << "dactivate = true;"
        << "} else {\n"
        << "dactivate = false;"
        << "}\n"
        << "} else {\n"
        << "dactivate = true;"
        << "}\n"
        << "if(first){\n"
        << "this->active = dactivate;"
        << "} else {\n"
        << "if(this->active != dactivate){"
        << "string msg(\"" << md.className << "::restore : \");\n"
        << "msg += \"incoherent activation state\";\n"
        << "throw(runtime_error(msg));\n"
        << "}"
        << "}\n"
        << "first = false;"
        << "}\n"
        << "}\n\n";
    // execute
    this->srcFile << "void " << md.className << "::execute(){\n"
                  << "using namespace std;\n"
                  << "using namespace pleiades;\n"
                  << "using namespace pleiades::time;\n"
                  << "using namespace pleiades::mesh;\n"
                  << "using namespace pleiades::field;\n"
                  << "using namespace pleiades::glossary;\n"
                  << "using std::shared_ptr;\n"
                  << "vector<string>::const_iterator ptr;\n"
                  << "this->computeMaterialProperties();\n";
    // do we need time increment ?
    for (const auto& f : md.functions) {
      if (f.useTimeIncrement) {
        this->srcFile
            << "const pleiades::field::real dt = "
            << "convert_to_double(this->sclock.getCurrentTimeIncrement());\n";
        break;
      }
    }
    i = 0;
    for (const auto& f : md.functions) {
      const auto functor = "functor" + std::to_string(i);
      if (f.useTimeIncrement) {
        this->srcFile << "this->" << functor << ".dt=dt;\n";
      }
      this->srcFile
          << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n"
          << "auto& mm = this->smanager.getMTFieldManager(*ptr);\n";
      for (const auto& uv : f.usedVariables) {
        const auto dv = decomposeVariableName(md, uv);
        const auto en = getDeclaration(md.getVariableDescription(dv.first));
        this->srcFile << "const NFieldHolder& "
                      << "f_" << uv << " = mm.getNFieldHolder(" << en << ","
                      << dv.second << ");\n";
      }
      for (const auto& mv : f.modifiedVariables) {
        const auto en = getDeclaration(md.getVariableDescription(mv));
        this->srcFile << "NFieldHolder& "
                      << "f_" << mv << " = mm.getNFieldHolder(" << en << ");\n";
      }
      for (const auto& c : f.constantMaterialProperties) {
        this->srcFile << "this->" << functor << "." << c
                      << " = this->constantMaterialProperties[*ptr][\"" << c
                      << "\"];\n";
      }
      this->srcFile << "if(getVerboseMode()==VERBOSE_LEVEL1){\n"
                    << "auto& log = getLogStream();\n"
                    << "log << \"**" << md.className << "::execute : \""
                    << "\n"
                    << "<< \"executing function '" << f.name
                    << "' on domain '\" << *ptr << \"'\" << endl;\n"
                    << "}\n";
      if ((f.modifiedVariables.size() == 1) &&
          (f.usedVariables.size() <
           TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)) {
        const auto output = *(f.modifiedVariables.begin());
        if (f.usedVariables.size() <= 5) {
          this->srcFile << "sapply(this->" << functor << ",f_" << output;
        } else {
          this->srcFile << "apply(this->" << functor << ",f_" << output;
        }
        if (f.usedVariables.size() != 0) {
          this->srcFile << ",\n";
          for (auto pmv = f.usedVariables.begin();
               pmv != f.usedVariables.end();) {
            this->srcFile << "f_" << *pmv;
            if (++pmv != f.usedVariables.end()) {
              this->srcFile << ",";
            }
          }
        }
        this->srcFile << ");\n";
      } else {
        if (f.usedVariables.size() != 0) {
          this->srcFile << "vector<real> results;\n"
                        << "vector<real> values;\n"
                        << "results.resize(" << f.modifiedVariables.size()
                        << ");\n"
                        << "values.resize(" << f.usedVariables.size() << ");\n"
                        << "shared_ptr<Support> support = mm.getSupportPtr();\n"
                        << "if(";
          for (auto puv = f.usedVariables.begin();
               puv != f.usedVariables.end();) {
            this->srcFile << "(f_" << *puv << ".is<VNField<real> >())";
            if (++puv != f.usedVariables.end()) {
              this->srcFile << "||\n";
            }
          }
          this->srcFile << "){\n";
          // a least one input is a field
          this->srcFile << "map<unsigned short,const VNField<real> *> fields;\n"
                        << "VNField<real>::size_type ptr2;\n"
                        << "map<unsigned short,const VNField<real> "
                           "*>::const_iterator ptr3;\n";
          // converting the output
          for (const auto& mv : f.modifiedVariables) {
            const auto en = getDeclaration(md.getVariableDescription(mv));
            this->srcFile << "if(!f_" << mv << ".is<VNField<real> >()){\n"
                          << "f_" << mv
                          << " = NFieldHolder(shared_ptr<VNField<real> >(new "
                             "VNField<real>(support,"
                          << en << ",f_" << mv << ".get<real>())));\n"
                          << "}\n";
          }
          // getting output field
          for (const auto& mv : f.modifiedVariables) {
            this->srcFile << "VNField<real>& ff_" << mv << " = f_" << mv
                          << ".get<VNField<real> >();\n";
          }
          i = 0;
          for (const auto& uv : f.usedVariables) {
            this->srcFile << "if(f_" << uv << ".is<VNField<real> >()){\n"
                          << "fields[" << i << "] = &(f_" << uv
                          << ".get<VNField<real> >());\n"
                          << "} else {\n"
                          << "values[" << i << "] = f_" << uv
                          << ".get<real>();\n"
                          << "}\n";
            ++i;
          }
          this->srcFile
              << "for(ptr2=0;ptr2!=ff_" << *(f.modifiedVariables.begin())
              << ".size();++ptr2){\n"
              << "for(ptr3=fields.begin();ptr3!=fields.end();++ptr3){\n"
              << "values[ptr3->first]=ptr3->second->operator()(ptr2);\n"
              << "}\n";
          i = 0;
          for (const auto& mv : f.modifiedVariables) {
            this->srcFile << "results[" << i << "] = ff_" << mv << "[ptr2];\n";
            ++i;
          }
          this->srcFile << "this->" << functor << "(results,values);\n";
          i = 0;
          for (const auto& mv : f.modifiedVariables) {
            this->srcFile << "ff_" << mv << "[ptr2] = "
                          << "results[" << i << "];\n";
            ++i;
          }
          this->srcFile << "}\n"
                        << "} else {\n";
          // all fields are uniform
          i = 0;
          for (const auto& uv : f.usedVariables) {
            this->srcFile << "values[" << i << "] = f_" << uv
                          << ".get<real>();\n";
            ++i;
          }
          i = 0;
          for (const auto& mv : f.modifiedVariables) {
            this->srcFile << "results[" << i << "] = f_" << mv
                          << ".get<real>();\n";
            ++i;
          }
          this->srcFile << "this->" << functor << "(results,values);\n";
          i = 0;
          for (const auto& mv : f.modifiedVariables) {
            const auto en = getDeclaration(md.getVariableDescription(mv));
            this->srcFile << "f_" << mv
                          << " = NFieldHolder(UNField<real>(support," << en
                          << ",results[" << i << "]));\n";
            ++i;
          }
          this->srcFile << "}\n";
        } else {
          // no input variables
          tfel::raise("MFrontModelInterface: untreated case");
        }
      }
      this->srcFile << "}\n";
      ++i;
    }
    this->srcFile
        << "} // end of " << md.className << "::execute\n\n"
        << "void " << md.className
        << "::executeInitialPostProcessingTasks(const bool)\n{} // end of "
        << md.className << "::executeInitialPostProcessingTasks\n\n";

    this->srcFile << "void " << md.className
                  << "::executePostProcessingTasks(const bool)\n{} // end of "
                  << md.className << "::executePostProcessingTasks\n\n";

    this->srcFile << md.className << "::~" << md.className << "()\n{}\n\n";

    this->srcFile << "PLEIADES_GENERATE_PROXY4(IModel," << md.className << ","
                  << "\"1.2-svn:6534\","
                  << "\"" << getLibraryName(md) << "\""
                  << ");\n\n"
                  << "} // end of namespace model\n\n"
                  << "} // end of namespace pleiades\n";
  }  // end of MFrontModelInterface::writeSrcFile()

  void MFrontModelInterface::writeInitializeOutputsVariablesDepths(
      const ModelDescription& md) {
    this->srcFile << "void " << md.className
                  << "::initializeOutputsVariablesDepths()\n{\n"
                  << "using namespace std;\n"
                  << "using namespace pleiades::glossary;\n"
                  << "vector<string>::const_iterator ptr;\n";
    unsigned int i = 0;
    for (const auto& f : md.functions) {
      ++i;
      for (const auto& mf : f.modifiedVariables) {
        const auto en = getDeclaration(md.getVariableDescription(mf));
        const auto functor = "functor" + std::to_string(i);
        auto p3 = f.depths.find(mf);
        tfel::raise_if(
            p3 == f.depths.end(),
            "MFrontModelInterface::writeInitializeOutputsVariablesDepths: "
            "internal error, no depth found for variable '" +
                mf +
                "' "
                "in function '" +
                f.name + "'");
        this->srcFile
            << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){"
               "\n"
            << "if(!this->outputsDepths[" << en << "].insert(make_pair(*ptr,"
            << p3->second << ")).second){\n"
            << "string msg(\"" << md.className << "::" << md.className
            << " : \");\n"
            << "msg += \"output '\";\n"
            << "msg += " << en << ";\n"
            << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n"
            << "throw(runtime_error(msg));\n"
            << "}\n"
            << "}\n";
      }
    }
    this->srcFile << "} // end of " << md.className
                  << "::initializeOutputsVariablesDepths\n\n";
  }  // end of MFrontModelInterface::writeInitializeOutputsVariablesDepths()

  void MFrontModelInterface::writeInitializeConstantMaterialProperties(
      const ModelDescription& md) {
    this->srcFile << "void " << md.className
                  << "::initializeConstantMaterialProperties("
                  << "const pleiades::parser::DataManager& data)\n{\n"
                  << "using namespace std;\n"
                  << "using namespace pleiades::glossary;\n"
                  << "using namespace pleiades::field;\n"
                  << "typedef ConstantMaterialPropertyDescription CMPD;\n"
                  << "vector<string>::const_iterator ptr;\n";
    for (const auto& f : md.functions) {
      for (const auto& c : f.constantMaterialProperties) {
        const auto& v = md.constantMaterialProperties.getVariable(c);
        const auto& en = getDeclaration(v);
        this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end("
                         ");++ptr){\n";
        // getting material description
        this->srcFile
            << "if(!data.hasMaterialDescription(*ptr)){\n"
            << "string msg(\"" << md.className
            << "::initializeConstantMaterialProperties : \");\n"
            << "msg += \"no material description  on material '\"+*ptr+\"'\";\n"
            << "throw(runtime_error(msg));\n"
            << "}\n"
            << "const auto& md = *(data.getMaterialDescription(*ptr));\n"
            << "if(md.containsMaterialProperty(" << en << ")){\n"
            << "if(!md.isMaterialPropertyAccesible(" << en << ")){\n"
            << "string msg(\"" << md.className
            << "::initializeConstantMaterialProperties : \");\n"
            << "msg += \"material property '\";\n"
            << "msg += " << en << ";\n"
            << "msg += \"' is not accessible on material '\"+*ptr+\"'\";\n"
            << "msg += \"' (this means that this property is define within a "
               "behaviour)\";\n"
            << "throw(runtime_error(msg));\n"
            << "}\n"
            << "const MaterialPropertyDescription& mpd = "
               "*(md.getMaterialProperty("
            << en << "));\n"
            << "if(!mpd.is<CMPD>()){\n"
            << "string msg(\"" << md.className
            << "::initializeConstantMaterialProperties : \");\n"
            << "msg += \"material property '\";\n"
            << "msg += " << en << ";\n"
            << "msg += \"' is not constant on material '\"+*ptr+\"'\";\n"
            << "throw(runtime_error(msg));\n"
            << "}\n"
            << "this->constantMaterialProperties[*ptr][\"" << c << "\"]"
            << " = mpd.get<CMPD>().getValue();\n"
            << "} else {\n";
        if (v.hasAttribute(VariableDescription::defaultValue)) {
          this->srcFile << "this->constantMaterialProperties[*ptr][\"" << c
                        << "\"] = "
                        << v.getAttribute<double>(
                               VariableDescription::defaultValue)
                        << ";\n";
        } else {
          this->srcFile
              << "string msg(\"" << md.className
              << "::initializeConstantMaterialProperties : \");\n"
              << "msg += \"material property '\";\n"
              << "msg += " << en << ";\n"
              << "msg += \"' is undefined on material '\"+*ptr+\"'\";\n"
              << "throw(runtime_error(msg));\n";
        }
        this->srcFile << "}\n"
                      << "}\n";
      }
    }
    this->srcFile << "} // end of " << md.className
                  << "::initializeConstantMaterialProperties\n\n";
  }  // end of MFrontModelInterface::writeInitializeConstantMaterialProperties()

  void MFrontModelInterface::writeInitializeOutputsVariablesInitialValues(
      const ModelDescription& md) {
    this->srcFile << "void " << md.className
                  << "::initializeOutputsVariablesInitialValues("
                  << "const pleiades::parser::DataManager& data)\n{\n"
                  << "using namespace std;\n"
                  << "using namespace pleiades::glossary;\n"
                  << "using namespace pleiades::field;\n"
                  << "using std::shared_ptr;\n"
                  << "typedef UniformScalarStateVariableDescription USSVD;\n"
                  << "vector<string>::const_iterator ptr;\n";
    for (const auto& f : md.functions) {
      for (const auto& mv : f.modifiedVariables) {
        // external name
        const auto& v = md.outputs.getVariable(mv);
        const auto& en = getDeclaration(v);
        this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end("
                         ");++ptr){\n";
        // getting material description
        if (v.hasAttribute(VariableDescription::initialValue)) {
          const auto iv =
              v.getAttribute<double>(VariableDescription::initialValue);
          this->srcFile
              << "if(data.hasMaterialDescription(*ptr)){\n"
              << "const auto& md = *(data.getMaterialDescription(*ptr));\n"
              << "if(md.containsStateVariable(" << en << ")){\n"
              << "auto tmp = md.getStateVariable(" << en << ");\n"
              << "if(!this->outputsInitialValues[" << en
              << "].insert({*ptr,tmp}).second){\n"
              << "string msg(\"" << md.className
              << "::initializeOutputsVariablesInitialValues : \");\n"
              << "msg += \"output '\";\n"
              << "msg += " << en << ";\n"
              << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n"
              << "throw(runtime_error(msg));\n"
              << "}\n"
              << "} else {\n"
              << "if(!this->outputsInitialValues[" << en
              << "].insert({*ptr,std::make_shared<USSVD>(" << en << "," << iv
              << ")}).second){\n"
              << "string msg(\"" << md.className
              << "::initializeOutputsVariablesInitialValues : \");\n"
              << "msg += \"output '\";\n"
              << "msg += " << en << ";\n"
              << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n"
              << "throw(runtime_error(msg));\n"
              << "}\n"
              << "}\n"
              << "} else {\n"
              << "if(!this->outputsInitialValues[" << en
              << "].insert({*ptr,std::make_shared<USSVD>(" << en << "," << iv
              << ")}).second){\n"
              << "string msg(\"" << md.className
              << "::initializeOutputsVariablesInitialValues : \");\n"
              << "msg += \"output '\";\n"
              << "msg += " << en << ";\n"
              << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n"
              << "throw(runtime_error(msg));\n"
              << "}\n"
              << "}\n";
        } else {
          this->srcFile
              << "if(!data.hasMaterialDescription(*ptr)){\n"
              << "string msg(\"" << md.className
              << "::initializeOutputsVariablesInitialValues : \");\n"
              << "msg += \"no material description  on material '\"+*ptr+\"', "
                 "\";\n"
              << "msg += \"required to initialize output value '\";\n"
              << "msg += " << en << ";\n"
              << "msg += '\\\'';\n"
              << "throw(runtime_error(msg));\n"
              << "}\n"
              << "const auto& md = *(data.getMaterialDescription(*ptr));\n"
              << "if(md.containsStateVariable(" << en << ")){\n"
              << "auto tmp = md.getStateVariable(" << en << ");\n"
              << "if(!this->outputsInitialValues[" << en
              << "].insert({*ptr,tmp}).second){\n"
              << "string msg(\"" << md.className
              << "::initializeOutputsVariablesInitialValues : \");\n"
              << "msg += \"output '\";\n"
              << "msg += " << en << ";\n"
              << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n"
              << "throw(runtime_error(msg));\n"
              << "}\n"
              << "}\n";
        }
        this->srcFile << "}\n";
      }
    }
    this->srcFile << "} // end of " << md.className
                  << "::initializeOutputsVariablesInitialValues\n\n";
  }  // end of
     // MFrontModelInterface::writeInitializeOutputsVariablesInitialValues()

  void MFrontModelInterface::writeInitializeInputsVariablesDepths(
      const ModelDescription& md) {
    std::set<std::string> treatedVars;
    this->srcFile << "void " << md.className
                  << "::initializeInputsVariablesDepths()\n{\n";
    unsigned int i = 0;
    for (const auto& f : md.functions) {
      for (const auto& v : f.usedVariables) {
        if (isInputVariable(md, v)) {
          ++i;
        }
      }
    }
    if (i != 0) {
      this->srcFile << "using namespace std;\n"
                    << "using namespace pleiades::glossary;\n"
                    << "vector<string>::const_iterator ptr;\n";
      i = 0;
      for (const auto& f : md.functions) {
        for (const auto& uv : f.usedVariables) {
          if (isInputVariable(md, uv)) {
            const auto& v = decomposeVariableName(md, uv).first;
            const auto& en = getDeclaration(md.getVariableDescription(v));
            if (treatedVars.find(v) == treatedVars.end()) {
              const auto p3 = f.depths.find(v);
              tfel::raise_if(p3 == f.depths.end(),
                             "MFrontModelInterface::"
                             "writeInitializeInputsVariablesDepths: "
                             "internal error, no depth found for variable '" +
                                 v +
                                 "' "
                                 "in function '" +
                                 f.name + "'");
              this->srcFile << "for(ptr=this->domains.begin();ptr!=this->"
                               "domains.end();++ptr){\n"
                            << "auto& tmp = this->inputsDepths[" << en
                            << "];\n";
              if (p3->second == 0) {
                this->srcFile << "if(tmp.find(*ptr)==tmp.end()){\n"
                              << "tmp[*ptr]=0;\n"
                              << "}\n";
              } else {
                this->srcFile << "if(tmp.find(*ptr)!=tmp.end()){\n"
                              << "if(tmp[*ptr]<" << p3->second << "){\n"
                              << "tmp[*ptr]=" << p3->second << ";\n"
                              << "}\n"
                              << "} else {\n"
                              << "tmp[*ptr]=" << p3->second << ";\n"
                              << "}\n";
              }
              this->srcFile << "}\n";
              treatedVars.insert(v);
            }
          }
        }
      }
    }
    this->srcFile << "} // end of " << md.className
                  << "::initializeInputsVariablesDepths\n\n";
  }  // end of MFrontModelInterface::writeInitializeInputsVariablesDepths()

  void MFrontModelInterface::writeStaticVariableInitialization(
      const FileDescription& fd,
      const ModelDescription& md,
      const StaticVariableDescription& v) {
    if (getDebugMode()) {
      this->srcFile << "#line " << v.lineNumber << " \"" << fd.fileName
                    << "\"\n";
    }
    if (v.type == "short") {
      this->srcFile << "const short " << md.className << "::" << v.name << " = "
                    << static_cast<short>(v.value) << ";" << '\n';
    } else if (v.type == "ushort") {
      this->srcFile << "const unsigned short " << md.className << "::" << v.name
                    << " = " << static_cast<unsigned short>(v.value) << ";"
                    << '\n';
    } else if (v.type == "int") {
      this->srcFile << "const int " << md.className << "::" << v.name << " = "
                    << static_cast<int>(v.value) << ";" << '\n';
    } else if (v.type == "uint") {
      this->srcFile << "const unsigned int " << md.className << "::" << v.name
                    << " = " << static_cast<unsigned int>(v.value) << ";"
                    << '\n';
    } else if (v.type == "long") {
      this->srcFile << "const long " << md.className << "::" << v.name << " = "
                    << static_cast<long>(v.value) << ";" << '\n';
    } else if (v.type == "ulong") {
      this->srcFile << "const unsigned long " << md.className << "::" << v.name
                    << " = " << static_cast<unsigned long>(v.value) << ";"
                    << '\n';
    } else if (v.type == "float") {
      this->srcFile << "const float " << md.className << "::" << v.name << " = "
                    << static_cast<float>(v.value) << ";" << '\n';
    } else if (v.type == "double") {
      this->srcFile << "const double " << md.className << "::" << v.name
                    << " = " << static_cast<double>(v.value) << ";" << '\n';
    } else if (v.type == "ldouble") {
      this->srcFile << "const long double " << md.className << "::" << v.name
                    << " = " << static_cast<long double>(v.value) << ";"
                    << '\n';
    } else if (v.type == "real") {
      this->srcFile << "const pleiades::field::real " << md.className
                    << "::" << v.name << " = " << static_cast<double>(v.value)
                    << ";" << '\n';
    } else {
      tfel::raise(
          "MFrontModelInterface::writeStaticVariableInitialisation: "
          "type " +
          v.type +
          " is not a supported type for a static variable."
          "Supported types are short, ushort, int, uint, long, ulong,"
          "float, double, ldouble and real.\n"
          "Error at line " +
          std::to_string(v.lineNumber));
    }
  }  // end of MFrontModelInterface::writeStaticVariableInitialisation

  void MFrontModelInterface::writeOutputFiles(const FileDescription& fd,
                                              const ModelDescription& md) {
    tfel::raise_if(md.className.empty(),
                   "MFrontModelInterface::writeOutputFiles: "
                   "no class name defined.");
    this->headerFileName = "include/Pleiades/Model/" + md.className;
    this->headerFileName += "-@application@.hxx";
    this->srcFileName = "src/" + md.className;
    this->srcFileName += "-@application@.cxx";
    this->openOutputFiles();
    this->generateOutputFiles(fd, md);
    this->closeOutputFiles();
  }  // end of class MFrontModelInterface::writeOutputFiles()

  void MFrontModelInterface::writeAssignDefaultValue(
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
    } else if (v.type == "bool") {
      this->srcFile << "this->" << v.name << " = "
                    << v.getAttribute<bool>(VariableDescription::defaultValue)
                    << ";\n";
    } else {
      tfel::raise(
          "MFrontModelInterface::writeAssignDefaultValue: "
          "type '" +
          v.type + "' is not supported.\n");
    }
  }  // end of MFrontModelInterface::writeAssignDefaultValue

  std::pair<bool, MFrontModelInterface::tokens_iterator>
  MFrontModelInterface::treatKeyword(const std::string& k,
                                     const std::vector<std::string>& i,
                                     tokens_iterator c,
                                     const tokens_iterator) {
    tfel::raise_if(std::find(i.begin(), i.end(), "licos") != i.end(),
                   "LicosModelInterface::treatKeyword: "
                   "unsupported key '" +
                       k + "'");
    return {false, c};
  }  // end of MFrontModelInterface::treatKeyword

  std::string MFrontModelInterface::getName() {
    return "@application@";
  }  // end of MFrontModelInterface::getName()

  void MFrontModelInterface::getTargetsDescription(TargetsDescription& td,
                                                   const ModelDescription& md) {
    const auto lib = getLibraryName(md);
    auto& l = td.getLibrary(lib);
    l.sources.push_back(md.className + "-@application@.cxx");
    td.headers.push_back("Pleiades/Model/" + md.className +
                         "-@application@.hxx");
    l.cppflags.push_back("`@application@-config --includes`");
    l.ldflags.push_back("`@application@-config --libs` -lm");
    l.epts.push_back(md.className);
  }  // end of MFrontModelInterface::getTargetsDescription

  MFrontModelInterface::~MFrontModelInterface() = default;

  ModelInterfaceProxy<MFrontModelInterface> ModelProxy;

}  // end of namespace mfront
