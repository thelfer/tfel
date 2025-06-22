/*!
 * \file   mfront/tests/models/src/LicosModelInterface.cxx
 * \brief
 * \author Helfer Thomas
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

#include "MFront/DSLUtilities.hxx"
#include "TFEL/System/System.hxx"

#include "MFront/MFrontHeader.hxx"
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

  static std::string getVariableType(const std::string& v) {
    if (v == "string") {
      return "std::string";
    }
    return v;
  }

  static std::string getLibraryName(const ModelDescription& md) {
    using namespace std;
    string a = "@Application@";
    if (md.library.empty()) {
      if (!md.material.empty()) {
        return "lib" + a + md.material + "Models";
      } else {
        return "lib" + a + "MaterialModels";
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
    if (r.first.empty()) {
      throw(
          std::runtime_error("decomposeVariableName: "
                             "field name '" +
                             n + "' has not been found"));
    }
    return r;
  }  // end of MFrontModelInterface::getVariableName(const std::string& v)

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
    using namespace std;
    using namespace tfel::system;
    // creating directories
    systemCall::mkdir("include");
    systemCall::mkdir("include/Pleiades");
    systemCall::mkdir("include/Pleiades/Model");
    systemCall::mkdir("src");
    this->headerFile.open(this->headerFileName.c_str());
    if (!this->headerFile) {
      string msg("MFrontModelInterface::openOutputFiles : ");
      msg += "unable to open ";
      msg += this->headerFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFile.open(this->srcFileName.c_str());
    if (!this->srcFile) {
      string msg("MFrontModelInterface::openOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->headerFile.exceptions(ios::badbit | ios::failbit);
    this->srcFile.exceptions(ios::badbit | ios::failbit);
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
      if (f.modifiedVariables.size() == 0) {
        throw(
            std::runtime_error("MFrontModelInterface::generateOutputFiles: "
                               "function '" +
                               f.name + "' does not modify any output"));
      }
    }
    for (const auto& v : md.outputs) {
      auto found = false;
      for (auto p2 = md.functions.begin();
           (p2 != md.functions.end()) && (!found); ++p2) {
        if (p2->modifiedVariables.find(v.name) != p2->modifiedVariables.end()) {
          found = true;
        }
      }
      if (!found) {
        throw(
            std::runtime_error("MFrontModelInterface::generateOutputFiles: "
                               "output '" +
                               v.name + "' is not modified by any function"));
      }
    }
    this->writeHeaderFile(fd, md);
    this->headerFile.close();
    this->writeSrcFile(fd, md);
    this->srcFile.close();
  }  // end of MFrontModelInterface::generateOutputFiles(void)

  void MFrontModelInterface::writeHeaderFile(const FileDescription& fd,
                                             const ModelDescription& md) {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    StaticVariableDescriptionContainer::const_iterator p2;
    set<string>::iterator p4;
    set<unsigned short> applyHeaders;
    set<unsigned short>::const_iterator p18;
    unsigned short i;
    this->headerFile << "/*!\n";
    this->headerFile << "* \\file   " << this->headerFileName << '\n';
    this->headerFile << "* \\brief  "
                     << "this file declares the " << md.className
                     << " Model.\n";
    this->headerFile << "*         File generated by ";
    this->headerFile << MFrontHeader::getVersionName() << " ";
    this->headerFile << "version " << MFrontHeader::getVersionNumber();
    this->headerFile << '\n';
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
                     << "_HH\n";
    this->headerFile << "#define _PLEIADES_" << makeUpperCase(md.className)
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
    this->headerFile << "#include\"Pleiades/Model/ModelBase.hxx\"\n";
    this->headerFile
        << "#include\"Pleiades/Model/StateVariableDescription.hxx\"\n\n";
    if (!md.includes.empty()) {
      this->headerFile << md.includes << "\n\n";
    }
    writeExportDirectives(this->headerFile);
    this->headerFile << "namespace pleiades\n{\n\n";
    this->headerFile << "namespace model\n{\n\n";
    this->headerFile << "struct MFRONT_SHAREDOBJ " << md.className << '\n';
    this->headerFile << ": public ModelBase\n";
    this->headerFile << "{\n\n";
    this->headerFile << "//! Default constructor\n";
    this->headerFile << md.className << "(pleiades::mesh::SMeshManager&,\n"
                     << "pleiades::field::SFieldManager&,\n"
                     << "pleiades::time::SClock&,\n"
                     << "pleiades::loading::SLoadingManager&,\n"
                     << "pleiades::signal::SSignalManager&);\n\n";
    this->headerFile << "//! Constructor to initialize local parameters\n";
    this->headerFile
        << md.className
        << "(const std::map<std::string,pleiades::parser::Data>&,\n"
        << "pleiades::mesh::SMeshManager&,\n"
        << "pleiades::field::SFieldManager&,\n"
        << "pleiades::time::SClock&,\n"
        << "pleiades::loading::SLoadingManager&,\n"
        << "pleiades::signal::SSignalManager&);\n\n";
    this->headerFile
        << "std::string\ngetName(void) const;\n\n"
        << "void\ninitialize(const pleiades::parser::DataManager&);\n\n"
        << "void\ndeclareRequirements(pleiades::coupling::SRequirementManager&)"
           ";\n\n"
        << "void\nresolveRequirements(pleiades::coupling::SRequirementManager&)"
           ";\n\n"
        << "void\ninitializeOutput(pleiades::coupling::Requirement&);\n\n"
        << "void\nexecute(void);\n\n"
        << "void\nsave(const std::string&,\n"
        << "pleiades::parser::DataMap&);\n\n"
        << "void\nrestore(const pleiades::time::ptime,\n"
        << "const std::string&,\n"
        << "const pleiades::parser::DataMap&);\n\n";
    this->headerFile
        << "void\nexecuteInitialPostProcessingTasks(const bool);\n\n";
    this->headerFile << "void\nexecutePostProcessingTasks(const bool);\n\n";
    this->headerFile << "~" << md.className << "();\n\n";
    this->headerFile << "private:\n\n";
    // Disable copy constructor and assignement operator
    this->headerFile << "//! Copy constructor (disabled)\n";
    this->headerFile << md.className << "(const " << md.className << "&);\n\n";
    this->headerFile << "//! Assignement operator (disabled)\n";
    this->headerFile << md.className << "&\n"
                     << "operator=(const " << md.className << "&);\n\n";
    for (const auto& f : md.functions) {
      if (f.modifiedVariables.size() == 0) {
        throw(
            runtime_error("MFrontModelInterface::writeHeaderFile: "
                          "function " +
                          f.name +
                          " does not modify "
                          "any variable (internal error, this shall "
                          "have been verified long ago)."));
      }
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
          this->headerFile << "void\n";
          this->headerFile << "operator()(std::vector<pleiades::field::real>&";
        } else {
          this->headerFile << "void\n";
          this->headerFile
              << "operator()(std::vector<pleiades::field::real>&,\n"
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
                         << " " << p2->name << ";" << '\n';
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
    this->headerFile << "void\ninitializeOutputsVariablesDepths(void);\n";
    this->headerFile << "void\ninitializeOutputsVariablesInitialValues(const "
                        "pleiades::parser::DataManager&);\n";
    this->headerFile << "void\ninitializeInputsVariablesDepths(void);";
    if (!md.constantMaterialProperties.empty()) {
      this->headerFile << "\nvoid\ninitializeConstantMaterialProperties(const "
                          "pleiades::parser::DataManager&);\n\n";
    } else {
      this->headerFile << "\n\n";
    }
    i = 0;
    for (const auto& f : md.functions) {
      if (f.modifiedVariables.size() == 0) {
        string msg("MFrontModelInterface::writeHeaderFile : ");
        msg += "function " + f.name + " does not modify any variable ";
        msg += "(internal error, this shall have been verified long ago).";
        throw(runtime_error(msg));
      }
      if (getDebugMode()) {
        this->headerFile << "#line " << f.line << " \"" << fd.fileName
                         << "\"\n";
      }
      this->headerFile << f.name << " functor" << i << ";\n";
      ++i;
    }
    this->headerFile << "std::map<std::string,std::map<std::string,unsigned "
                        "short> > outputsDepths;\n";
    this->headerFile
        << "std::map<std::string,std::map<std::string,boost::shared_ptr<"
           "StateVariableDescription> > > outputsInitialValues;\n";
    this->headerFile << "std::map<std::string,std::map<std::string,unsigned "
                        "short> > inputsDepths;\n";
    if (!md.constantMaterialProperties.empty()) {
      this->headerFile << "std::map<std::string,std::map<std::string,pleiades::"
                          "field::real> > constantMaterialProperties;\n";
    }
    this->headerFile << "}; // end of struct " << md.className << "\n\n";
    this->headerFile << "} // end of namespace model\n\n";
    this->headerFile << "} // end of namespace pleiades\n\n";
    this->headerFile << "#endif /* _PLEIADES_" << makeUpperCase(md.className)
                     << "_HH */\n";
  }  // end of MFrontModelInterface::writeHeaderFile(void)

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
    throw(
        std::runtime_error("MFrontModelInterface::getGenTypeGetMethod : "
                           "no method associated with type '" +
                           type + '\''));
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
    throw(
        std::runtime_error("MFrontModelInterface::getGenTypeIsMethod : "
                           "no method associated with type '" +
                           type + '\''));
  }  // end of MFrontModelInterface::isGenTypeMethod

  VariableDescriptionContainer::const_iterator
  MFrontModelInterface::findVariableDescription(
      const VariableDescriptionContainer& v, const std::string& n) {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    for (p = v.begin(); p != v.end(); ++p) {
      if (p->name == n) {
        return p;
      }
    }
    throw(
        std::runtime_error("MFrontModelInterface::findVariableDescription : "
                           "no variable named '" +
                           n + "' found"));
  }  // MFrontModelInterface::findVariableDescription

  void MFrontModelInterface::writeSrcFile(const FileDescription& fd,
                                          const ModelDescription& md) {
    std::set<unsigned short> applyHeaders;
    unsigned short specializedParametersNumber;
    unsigned int i;
    this->srcFile << "/*!" << '\n';
    this->srcFile << "* \\file   " << this->srcFileName << '\n';
    this->srcFile << "* \\brief  "
                  << "this file implements the " << md.className
                  << "  model.\n";
    this->srcFile << "*         File generated by ";
    this->srcFile << MFrontHeader::getVersionName() << " ";
    this->srcFile << "version " << MFrontHeader::getVersionNumber();
    this->srcFile << '\n';
    if (!fd.authorName.empty()) {
      this->srcFile << "* \\author " << fd.authorName << '\n';
    }
    if (!fd.date.empty()) {
      this->srcFile << "* \\date   " << fd.date << '\n';
    }
    this->srcFile << " */\n\n";
    this->srcFile << "#include<iostream>\n";
    this->srcFile << "#include<stdexcept>\n";
    this->srcFile << "#include<sstream>\n";
    this->srcFile << "#include<cmath>\n\n";
    this->srcFile << "#include\"Pleiades/Global.hxx\"\n";
    this->srcFile << "#include\"Pleiades/OutOfBoundsPolicy.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Parser/Md.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Parser/DataManager.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Glossary/Glossary.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Time/SClock.hxx\"\n";
    this->srcFile
        << "#include\"Pleiades/Model/MaterialPropertyDescription.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Coupling/Requirement.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Coupling/SRequirementManager.hxx\"\n";
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
      this->srcFile << "#include\"Pleiades/Field/VNField.hxx\"\n";
      this->srcFile << "#include\"Pleiades/Field/NFieldHolder.hxx\"\n";
    }
    this->srcFile << "#include\"Pleiades/Model/MaterialDescription.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Model/"
                     "UniformScalarStateVariableDescription.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Model/IModelFactory.hxx\"\n";
    this->srcFile << "#include\"Pleiades/Model/" << md.className
                  << "-@application@.hxx\"\n\n";
    this->srcFile << "namespace pleiades\n{\n\n";
    this->srcFile << "namespace model\n{\n\n";
    // Functors
    for (const auto& f : md.functions) {
      // operator()
      if (getDebugMode()) {
        this->srcFile << "#line " << f.line << " \"" << fd.fileName << "\"\n";
      }
      if ((f.modifiedVariables.size() == 1) &&
          (f.usedVariables.size() <
           TFEL_MFRONTPLEAIDESPARSER_MAXUSEDVARIABLESFORUSINGAPPLY)) {
        this->srcFile << "pleiades::field::real\n";
        this->srcFile << md.className << "::" << f.name << "::operator()(";
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
        this->srcFile << "void\n";
        this->srcFile << md.className << "::" << f.name << "::operator()(";
        this->srcFile << "std::vector<pleiades::field::real>& results";
        if (f.usedVariables.size() != 0) {
          this->srcFile
              << ",\nconst std::vector<pleiades::field::real>& values";
        }
      }
      this->srcFile << ") const\n";
      this->srcFile << "{\n";
      this->srcFile << "using namespace std;\n";
      this->srcFile << "using pleiades::field::real;\n";
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
      writeMaterialLaws("MFrontModelInterface::writeSrcFile", this->srcFile,
                        md.materialLaws);
      i = 0;
      for (auto puv = f.usedVariables.begin(); puv != f.usedVariables.end();
           ++puv, ++i) {
        const auto& v = [&md, puv]() {
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
          throw(
              std::runtime_error("MFrontModelInterface::writeSrcFile: "
                                 "no input or output variable "
                                 "named '" +
                                 *puv + "'"));
        }();
        if ((v.hasAttribute(VariableDescription::bound)) ||
            (v.hasAttribute(VariableDescription::physicalBound))) {
          this->srcFile << "#ifndef NO_PLEIADES_BOUNDS_CHECK\n";
        }
        if (v.hasAttribute(VariableDescription::physicalBound)) {
          const auto& bd = v.getAttribute<VariableBoundsDescription>(
              VariableDescription::physicalBound);
          this->srcFile << "// checking " << v.name << " physical bounds\n";
          if ((bd.boundsType == VariableBoundsDescription::Lower) ||
              (bd.boundsType == VariableBoundsDescription::LowerAndUpper)) {
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
          if ((bd.boundsType == VariableBoundsDescription::Upper) ||
              (bd.boundsType == VariableBoundsDescription::LowerAndUpper)) {
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
        if (v.hasAttribute(VariableDescription::bound)) {
          const auto& bd = v.getAttribute<VariableBoundsDescription>(
              VariableDescription::bound);
          this->srcFile << "// checking " << v.name << " bounds\n";
          if ((bd.boundsType == VariableBoundsDescription::Lower) ||
              (bd.boundsType == VariableBoundsDescription::LowerAndUpper)) {
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
          if ((bd.boundsType == VariableBoundsDescription::Upper) ||
              (bd.boundsType == VariableBoundsDescription::LowerAndUpper)) {
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
        if ((v.hasAttribute(VariableDescription::bound)) ||
            (v.hasAttribute(VariableDescription::physicalBound))) {
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
        }
      }
      this->srcFile << "} // end of " << md.className << "::" << f.name
                    << "::operator()\n\n";
    }
    // static variables
    if (!md.staticVars.empty()) {
      for (const auto& sv : md.staticVars) {
        this->writeStaticVariableInitialization(fd, md, sv);
      }
      this->srcFile << '\n';
    }
    // Constructors
    if (this->hasDefaultConstructor) {
      this->srcFile << md.className << "::" << md.className
                    << "(pleiades::mesh::SMeshManager& m,\n"
                    << "pleiades::field::SFieldManager& mf,\n"
                    << "pleiades::time::SClock& c,\n"
                    << "pleiades::loading::SLoadingManager& l,\n"
                    << "pleiades::signal::SSignalManager& s)\n"
                    << ": ModelBase(m,mf,c,l,s)\n{\n";
      this->srcFile << "using namespace std;\n";
      this->srcFile << "using namespace pleiades::glossary;\n";
      this->srcFile << "vector<string> tmp;\n";
      this->srcFile << "vector<string>::const_iterator ptr;\n";
      this->srcFile << "vector<string>::const_iterator ptr2;\n";
      for (const auto& p : md.parameters) {
        this->writeAssignDefaultValue(p);
      }
      if (md.domains.empty()) {
        throw(
            std::runtime_error("MFrontModelInterface::writeSrcFile: "
                               "no domain defined (internal error, "
                               "we shall not reach this point)"));
      }
      for (const auto& d : md.domains) {
        this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,\"" << d
                      << "\");\n";
        this->srcFile << "for(ptr=tmp.begin();ptr!=tmp.end();++ptr){\n";
        this->srcFile << "if(find(this->domains.begin(),this->domains.end(),*"
                         "ptr)!=this->domains.end()){\n";
        this->srcFile << "string msg(\"" << md.className << "::" << md.className
                      << " : \");\n";
        this->srcFile << "msg += \"domain '\"+*ptr+\"' multiply defined\";\n";
        this->srcFile << "throw(runtime_error(msg));\n";
        this->srcFile << "}\n";
        this->srcFile << "this->domains.push_back(*ptr);\n";
        this->srcFile << "}\n";
      }
      this->srcFile << "this->initializeOutputsVariablesDepths();\n";
      this->srcFile << "this->initializeInputsVariablesDepths();\n";
      this->srcFile << "} // end of " << md.className << "::" << md.className
                    << "\n\n";
    } else {
      this->srcFile << md.className << "::" << md.className
                    << "(pleiades::mesh::SMeshManager& m,\n"
                    << "pleiades::field::SFieldManager& mf,\n"
                    << "pleiades::time::SClock& c,\n"
                    << "pleiades::loading::SLoadingManager& l,\n"
                    << "pleiades::signal::SSignalManager& s)\n"
                    << ": ModelBase(m,mf,c,l,s)\n{\n";
      this->srcFile << "using namespace std;\n";
      this->srcFile << "string msg(\"" << md.className << "::" << md.className
                    << " : \");\n";
      this->srcFile << "msg += \"no domain defined\";\n";
      this->srcFile << "throw(runtime_error(msg));\n";
      this->srcFile << "} // end of " << md.className << "::" << md.className
                    << "\n\n";
    }
    this->srcFile << "//! Constructor to initialize local parameters\n";
    this->srcFile
        << md.className << "::" << md.className
        << "(const std::map<std::string,pleiades::parser::Data>& data,\n"
        << "pleiades::mesh::SMeshManager& m,\n"
        << "pleiades::field::SFieldManager& mf,\n"
        << "pleiades::time::SClock& c,\n"
        << "pleiades::loading::SLoadingManager& l,\n"
        << "pleiades::signal::SSignalManager& s)\n"
        << ": ModelBase(m,mf,c,l,s)\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "using namespace pleiades::parser;\n";
    this->srcFile << "using namespace pleiades::coupling;\n";
    this->srcFile << "static constexpr string ValidParametersNames[] = {";
    this->srcFile << "\"domain\",\"domains\",\"Active\",\n"
                  << "\"ActivatingEvents\",\"DesactivatingEvents\"";
    specializedParametersNumber = 5u;
    for (const auto& p : md.parameters) {
      const auto name = this->getVariableName(p.name, md);
      this->srcFile << ",\n" << name;
      ++(specializedParametersNumber);
    }
    // domain specialisation
    this->srcFile << "};\n";
    this->srcFile << "vector<string> tmp;\n";
    this->srcFile << "map<string,Data>::const_iterator ptr;\n";
    this->srcFile << "vector<string>::const_iterator ptr2;\n";
    this->srcFile << "vector<string>::const_iterator ptr3;\n";
    this->srcFile << "for(ptr=md.begin();ptr!=md.end();++ptr){\n";
    this->srcFile << "if(find(ValidParametersNames,ValidParametersNames+"
                  << specializedParametersNumber
                  << ",ptr->first)==ValidParametersNames+"
                  << specializedParametersNumber << "){\n";
    this->srcFile << "string msg(\"" << md.className << "::" << md.className
                  << " : \");\n";
    this->srcFile << "msg += \"unknown parameter '\"+ptr->first+\"'\";\n";
    this->srcFile << "throw(runtime_error(msg));\n";
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "ActivableObjectBase::handleSpecialisationData(data);\n";
    for (const auto& p : md.parameters) {
      const auto name = this->getVariableName(p.name, md);
      this->srcFile << "ptr = md.find(" << name << ");\n";
      this->srcFile << "if(ptr==md.end()){\n";
      if (p.hasAttribute(VariableDescription::defaultValue)) {
        this->writeAssignDefaultValue(p);
      } else {
        this->srcFile << "string msg(\"" << md.className << "::" << md.className
                      << " : \");\n";
        this->srcFile << "msg += \"can't initialize parameter " << p.name
                      << "\";\n";
        this->srcFile << "throw(runtime_error(msg));\n";
      }
      this->srcFile << "} else {\n";
      this->srcFile << "if(!ptr->second." << this->getGenTypeIsMethod(p.type)
                    << "()){\n";
      this->srcFile << "string msg(\"" << md.className << "::" << md.className
                    << " : \");\n";
      this->srcFile << "msg += \"wrong type for parameter '" << p.name
                    << "' (expected a '" + p.type + "')\";\n";
      this->srcFile << "throw(runtime_error(msg));\n";
      this->srcFile << "}\n";
      this->srcFile << "this->" << p.name << " = ptr->second."
                    << this->getGenTypeGetMethod(p.type) << "();\n";
      this->srcFile << "}\n";
    }
    this->srcFile << "if((md.find(\"domain\")!=md.end())||(md.find(\"domains\")"
                     "!=md.end())){\n";
    this->srcFile << "if(md.find(\"domain\")!=md.end()){\n";
    this->srcFile << "ptr = md.find(\"domain\");\n";
    this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,ptr->second."
                     "get<string>());\n";
    this->srcFile << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n";
    this->srcFile << "if(find(this->domains.begin(),this->domains.end(),*ptr2)!"
                     "=this->domains.end()){\n";
    this->srcFile << "string msg(\"" << md.className << "::" << md.className
                  << ": \");\n";
    this->srcFile << "msg += \"domain '\"+*ptr2+\"' multiply defined\";\n";
    this->srcFile << "throw(runtime_error(msg));\n";
    this->srcFile << "}\n";
    this->srcFile << "this->domains.push_back(*ptr2);\n";
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "if(md.find(\"domains\")!=md.end()){\n";
    this->srcFile << "ptr = md.find(\"domains\");\n";
    this->srcFile << "if(ptr!=md.end()){\n";
    this->srcFile << "if(!ptr->second.is<vector<string> >()){\n";
    this->srcFile << "string msg(\"" << md.className << "::" << md.className
                  << " : \");\n";
    this->srcFile << "msg += \"invalid type for parameter 'domains'\";\n";
    this->srcFile << "throw(runtime_error(msg));\n";
    this->srcFile << "}\n";
    this->srcFile << "for(ptr3=ptr->second.get<vector<string> >().begin();\n";
    this->srcFile
        << "ptr3!=ptr->second.get<vector<string> >().end();++ptr3){\n";
    this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,*ptr3);\n";
    this->srcFile << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n";
    this->srcFile << "if(find(this->domains.begin(),this->domains.end(),*ptr2)!"
                     "=this->domains.end()){\n";
    this->srcFile << "string msg(\"" << md.className << "::" << md.className
                  << ": \");\n";
    this->srcFile << "msg += \"domain '\"+*ptr2+\"' multiply defined\";\n";
    this->srcFile << "throw(runtime_error(msg));\n";
    this->srcFile << "}\n";
    this->srcFile << "this->domains.push_back(*ptr2);\n";
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "} else {\n";
    if (md.domains.empty()) {
      this->srcFile << "string msg(\"" << md.className << "::" << md.className
                    << ": \");\n";
      this->srcFile << "msg += \"no domain defined\";\n";
      this->srcFile << "throw(runtime_error(msg));\n";
    } else {
      for (const auto& d : md.domains) {
        this->srcFile << "this->smanager.getMatchingMaterialsNames(tmp,\"" << d
                      << "\");\n";
        this->srcFile << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n";
        this->srcFile << "if(find(this->domains.begin(),this->domains.end(),*"
                         "ptr2)!=this->domains.end()){\n";
        this->srcFile << "string msg(\"" << md.className << "::" << md.className
                      << " : \");\n";
        this->srcFile << "msg += \"domain '\"+*ptr2+\"' multiply defined\";\n";
        this->srcFile << "throw(runtime_error(msg));\n";
        this->srcFile << "}\n";
        this->srcFile << "this->domains.push_back(*ptr2);\n";
        this->srcFile << "}\n";
      }
    }
    this->srcFile << "}\n";
    this->srcFile << "this->initializeOutputsVariablesDepths();\n";
    this->srcFile << "this->initializeInputsVariablesDepths();\n";
    this->srcFile << "} // end of " << md.className << "::" << md.className
                  << "\n\n";
    this->writeInitializeOutputsVariablesDepths(md);
    this->writeInitializeInputsVariablesDepths(md);
    // getName
    this->srcFile << "std::string\n"
                  << md.className << "::getName(void) const\n"
                  << "{\n"
                  << "return \"" << md.className << "\";\n"
                  << "}\n\n";
    // initialize
    this->srcFile << "void\n"
                  << md.className
                  << "::initialize(const pleiades::parser::DataManager& data)";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "this->initializeOutputsVariablesInitialValues(data);\n";
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
    this->srcFile << "void\n";
    this->srcFile << md.className
                  << "::declareRequirements(pleiades::coupling::"
                     "SRequirementManager& requirementManager)\n";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::coupling;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile
        << "map<string,map<string,unsigned short> >::const_iterator ptr;\n";
    this->srcFile << "map<string,unsigned short>::const_iterator ptr2;\n";
    this->srcFile << "for(ptr=this->inputsDepths.begin();ptr!=this->"
                     "inputsDepths.end();++ptr){\n";
    this->srcFile << "const map<string,unsigned auto& tmp = ptr->second;\n";
    this->srcFile << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n";
    this->srcFile
        << "requirementManager.addRequirement(static_cast<IModel&>(*this),\n";
    this->srcFile << "ptr->first,ptr2->first,\n";
    this->srcFile << "Requirement::SCALAR,\n";
    this->srcFile << "Requirement::MANDATORY,";
    this->srcFile << "ptr2->second);\n";
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "} // end of " << md.className
                  << "::declareRequirements\n\n";
    // resolveDependencies
    this->srcFile << "void\n";
    this->srcFile << md.className
                  << "::resolveRequirements(pleiades::coupling::"
                     "SRequirementManager& requirementManager)\n";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "using namespace pleiades::coupling;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile
        << "map<string,map<string,unsigned short> >::const_iterator ptr;\n";
    this->srcFile << "map<string,unsigned short>::const_iterator ptr2;\n";
    this->srcFile << "for(ptr=this->outputsDepths.begin();ptr!=this->"
                     "outputsDepths.end();++ptr){\n";
    this->srcFile << "const map<string,unsigned auto& tmp = ptr->second;\n";
    this->srcFile << "for(ptr2=tmp.begin();ptr2!=tmp.end();++ptr2){\n";
    this->srcFile << "if(this->outputsInitialValues.find(ptr->first)==this->"
                     "outputsInitialValues.end()){\n";
    this->srcFile << "requirementManager.setRequirementProvider(static_cast<"
                     "IModel&>(*this),\n"
                  << "ptr->first,ptr2->first,Requirement::SCALAR,false);\n";
    this->srcFile << "} else {\n";
    this->srcFile << "if(this->outputsInitialValues[ptr->first].find(ptr2->"
                     "first)!=this->outputsInitialValues[ptr->first].end()){\n";
    this->srcFile << "requirementManager.setRequirementProvider(static_cast<"
                     "IModel&>(*this),\n"
                  << "ptr->first,ptr2->first,Requirement::SCALAR,true);\n";
    this->srcFile << "} else {\n";
    this->srcFile << "requirementManager.setRequirementProvider(static_cast<"
                     "IModel&>(*this),\n"
                  << "ptr->first,ptr2->first,Requirement::SCALAR,false);\n";
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "}\n";
    this->srcFile << "} // end of " << md.className
                  << "::resolveRequirements\n\n";
    // initializeOutput
    this->srcFile << "void\n";
    this->srcFile
        << md.className
        << "::initializeOutput(pleiades::coupling::Requirement& requirement)\n";
    this->srcFile << "{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace boost;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile
        << "map<string,map<string,unsigned short> >::const_iterator ptr;\n";
    this->srcFile << "map<string,unsigned short>::const_iterator ptr2;\n";
    this->srcFile << "map<string,map<string,shared_ptr<"
                     "StateVariableDescription> > >::const_iterator ptr3;\n";
    this->srcFile << "map<string,shared_ptr<StateVariableDescription> "
                     ">::const_iterator ptr4;\n";
    this->srcFile << "ptr = this->outputsDepths.find(requirement.getName());\n";
    this->srcFile << "if(ptr==this->outputsDepths.end()){\n";
    this->srcFile << "return ModelBase::initializeOutput(requirement);\n";
    this->srcFile << "}\n";
    this->srcFile << "ptr2 = ptr->second.find(requirement.getLocation());\n";
    this->srcFile << "if(ptr2==ptr->second.end()){\n";
    this->srcFile << "return ModelBase::initializeOutput(requirement);\n";
    this->srcFile << "}\n";
    this->srcFile
        << "ptr3 = this->outputsInitialValues.find(requirement.getName());\n";
    this->srcFile << "if(ptr3==this->outputsInitialValues.end()){\n";
    this->srcFile << "string msg(\"" << md.className
                  << "::initializeOutput: \");\n";
    this->srcFile
        << "msg += \"no initial value for '\"+requirement.getName()+\"'\";\n";
    this->srcFile << "throw(runtime_error(msg));\n";
    this->srcFile << "}\n";
    this->srcFile << "ptr4 = ptr3->second.find(requirement.getLocation());\n";
    this->srcFile << "if(ptr4==ptr3->second.end()){\n";
    this->srcFile << "string msg(\"" << md.className
                  << "::initializeOutput: \");\n";
    this->srcFile
        << "msg += \"no initial value for '\"+requirement.getName()+\"'\";\n";
    this->srcFile << "throw(runtime_error(msg));\n";
    this->srcFile << "}\n";
    this->srcFile << "ptr4->second->initialize(this->smanager."
                     "getMTFieldManager(requirement.getLocation()),\n"
                  << "requirement,ptr2->second);\n";
    this->srcFile << "} // end of " << md.className << "::initializeOutput\n\n";
    // save
    this->srcFile
        << "void\n"
        << md.className
        << "::save(const std::string& directory,\npleiades::parser::DataMap& "
           "saveddata)\n"
        << "{\n"
        << "using namespace std;\n"
        << "using namespace pleiades::parser;\n"
        << "typedef map<string,string>::value_type MVType;\n"
        << "vector<string>::const_iterator ptr;\n"
        << "ModelBase::save(directory,saveddata);\n"
        << "auto& msaveddata = savedmd.get<DataMap>(this->getName(),true);\n"
        << "auto& activestate = msavedmd.get<map<string,string> "
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
        << "}\n\n";
    this->srcFile
        << "void\n"
        << md.className << "::restore(const pleiades::time::ptime t,\n"
        << "const std::string& directory,\n"
        << "const pleiades::parser::DataMap&saveddata)\n"
        << "{\n"
        << "using namespace std;\n"
        << "using namespace pleiades::parser;\n"
        << "vector<string>::const_iterator ptr;\n"
        << "map<string,string>::const_iterator ptr2;\n"
        << "ModelBase::restore(t,directory,saveddata);\n"
        << "if(!savedmd.contains(this->getName())){\n"
        << "return;"
        << "}\n"
        << "const auto& msaveddata = savedmd.get<DataMap>(this->getName());\n"
        << "if(!msavedmd.contains(\"ActiveState\")){\n"
        << "return;\n"
        << "}\n"
        << "bool first     = true;\n"
        << "bool dactivate = true;\n"
        << "const auto& activestate = msavedmd.get<map<string,string> "
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
    this->srcFile << "void\n" << md.className << "::execute(void){\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace boost;\n";
    this->srcFile << "using namespace pleiades;\n";
    this->srcFile << "using namespace pleiades::time;\n";
    this->srcFile << "using namespace pleiades::mesh;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "using std::shared_ptr;\n";
    this->srcFile << "vector<string>::const_iterator ptr;\n";
    this->srcFile << "this->computeMaterialProperties();\n";
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
          << "for(ptr=this->domains.begin();ptr!=this->domains.end();++ptr){\n";
      this->srcFile << "auto& mm = this->smanager.getMTFieldManager(*ptr);\n";
      for (const auto& uv : f.usedVariables) {
        const auto dv = decomposeVariableName(md, uv);
        this->srcFile << "const NFieldHolder& "
                      << "f_" << uv << " = mm.getNFieldHolder("
                      << this->getVariableName(dv.first, md) << "," << dv.second
                      << ");\n";
      }
      for (const auto& mv : f.modifiedVariables) {
        this->srcFile << "NFieldHolder& "
                      << "f_" << mv << " = mm.getNFieldHolder("
                      << this->getVariableName(mv, md) << ");\n";
      }
      for (const auto& c : f.constantMaterialProperties) {
        this->srcFile << "this->" << functor << "." << c
                      << " = this->constantMaterialProperties[*ptr][\"" << c
                      << "\"];\n";
      }
      this->srcFile << "if(getVerboseMode()==VERBOSE_LEVEL1){\n";
      this->srcFile << "auto& log = getLogStream();\n";
      this->srcFile << "log << \"**" << md.className << "::execute : \""
                    << "\n";
      this->srcFile << "<< \"executing function '" << f.name
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
          this->srcFile << "vector<real> results;\n";
          this->srcFile << "vector<real> values;\n";
          this->srcFile << "results.resize(" << f.modifiedVariables.size()
                        << ");\n";
          this->srcFile << "values.resize(" << f.usedVariables.size() << ");\n";
          this->srcFile
              << "shared_ptr<Support> support = mm.getSupportPtr();\n";
          this->srcFile << "if(";
          for (auto puv = f.usedVariables.begin();
               puv != f.usedVariables.end();) {
            this->srcFile << "(f_" << *puv << ".is<VNField<real> >())";
            if (++puv != f.usedVariables.end()) {
              this->srcFile << "||\n";
            }
          }
          this->srcFile << "){\n";
          // a least one input is a field
          this->srcFile
              << "map<unsigned short,const VNField<real> *> fields;\n";
          this->srcFile << "VNField<real>::size_type ptr2;\n";
          this->srcFile << "map<unsigned short,const VNField<real> "
                           "*>::const_iterator ptr3;\n";
          // converting the output
          for (const auto& mv : f.modifiedVariables) {
            this->srcFile << "if(!f_" << mv << ".is<VNField<real> >()){\n"
                          << "f_" << mv
                          << " = NFieldHolder(shared_ptr<VNField<real> >(new "
                             "VNField<real>(support,"
                          << this->getVariableName(mv, md) << ",f_" << mv
                          << ".get<real>())));\n"
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
          this->srcFile << "for(ptr2=0;ptr2!=ff_"
                        << *(f.modifiedVariables.begin())
                        << ".size();++ptr2){\n";
          this->srcFile
              << "for(ptr3=fields.begin();ptr3!=fields.end();++ptr3){\n";
          this->srcFile
              << "values[ptr3->first]=ptr3->second->operator()(ptr2);\n";
          this->srcFile << "}\n";
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
          }
          this->srcFile << "}\n";
          this->srcFile << "} else {\n";
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
            this->srcFile << "f_" << mv
                          << " = NFieldHolder(UNField<real>(support,"
                          << this->getVariableName(mv, md) << ",results[" << i
                          << "]));\n";
            ++i;
          }
          this->srcFile << "}\n";
        } else {
          // no input variables
          throw(std::runtime_error("MFrontModelInterface : untreated case"));
        }
      }
      this->srcFile << "}\n";
      ++i;
    }
    this->srcFile << "} // end of " << md.className << "::execute\n\n";
    this->srcFile
        << "void\n"
        << md.className
        << "::executeInitialPostProcessingTasks(const bool)\n{} // end of "
        << md.className << "::executeInitialPostProcessingTasks\n\n";

    this->srcFile << "void\n"
                  << md.className
                  << "::executePostProcessingTasks(const bool)\n{} // end of "
                  << md.className << "::executePostProcessingTasks\n\n";

    this->srcFile << md.className << "::~" << md.className << "()\n{}\n\n";

    this->srcFile << "PLEIADES_GENERATE_PROXY4(IModel," << md.className << ","
                  << "\"1.2-svn:6534\","
                  << "\"" << getLibraryName(md) << "\""
                  << ");\n\n";
    this->srcFile << "} // end of namespace model\n\n";
    this->srcFile << "} // end of namespace pleiades\n";
  }  // end of MFrontModelInterface::writeSrcFile(void)

  void MFrontModelInterface::writeInitializeOutputsVariablesDepths(
      const ModelDescription& md) {
    this->srcFile << "void\n"
                  << md.className
                  << "::initializeOutputsVariablesDepths(void)\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "vector<string>::const_iterator ptr;\n";
    unsigned int i = 0;
    for (const auto& f : md.functions) {
      ++i;
      for (const auto& mf : f.modifiedVariables) {
        const auto functor = "functor" + std::to_string(i);
        auto p3 = f.depths.find(mf);
        if (p3 == f.depths.end()) {
          throw(std::runtime_error(
              "MFrontModelInterface::writeInitializeOutputsVariablesDepths: "
              "internal error, no depth found for variable '" +
              mf +
              "' "
              "in function '" +
              f.name + "'"));
        }
        this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end("
                         ");++ptr){\n";
        this->srcFile << "if(!this->outputsDepths["
                      << this->getVariableName(mf, md)
                      << "].insert(make_pair(*ptr,";
        this->srcFile << p3->second;
        this->srcFile << ")).second){\n";
        this->srcFile << "string msg(\"" << md.className << "::" << md.className
                      << " : \");\n";
        this->srcFile << "msg += \"output '\";\n";
        this->srcFile << "msg += " << this->getVariableName(mf, md) << ";\n";
        this->srcFile
            << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n";
        this->srcFile << "throw(runtime_error(msg));\n";
        this->srcFile << "}\n";
        this->srcFile << "}\n";
      }
    }
    this->srcFile << "} // end of " << md.className
                  << "::initializeOutputsVariablesDepths\n\n";
  }  // end of MFrontModelInterface::writeInitializeOutputsVariablesDepths()

  void MFrontModelInterface::writeInitializeConstantMaterialProperties(
      const ModelDescription& md) {
    this->srcFile << "void\n"
                  << md.className << "::initializeConstantMaterialProperties("
                  << "const pleiades::parser::DataManager& data)\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile << "typedef ConstantMaterialPropertyDescription CMPD;\n";
    this->srcFile << "vector<string>::const_iterator ptr;\n";
    for (const auto& f : md.functions) {
      for (const auto& c : f.constantMaterialProperties) {
        const auto& v = md.constantMaterialProperties.getVariable(c);
        this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end("
                         ");++ptr){\n";
        // getting material description
        this->srcFile << "if(!md.hasMaterialDescription(*ptr)){\n";
        this->srcFile << "string msg(\"" << md.className
                      << "::initializeConstantMaterialProperties : \");\n";
        this->srcFile << "msg += \"no material description  on material "
                         "'\"+*ptr+\"'\";\n";
        this->srcFile << "throw(runtime_error(msg));\n";
        this->srcFile << "}\n";
        this->srcFile << "const MaterialDescription& md = "
                         "*(md.getMaterialDescription(*ptr));\n";
        this->srcFile << "if(md.containsMaterialProperty("
                      << this->getVariableName(c, md) << ")){\n";
        this->srcFile << "if(!md.isMaterialPropertyAccesible("
                      << this->getVariableName(c, md) << ")){\n";
        this->srcFile << "string msg(\"" << md.className
                      << "::initializeConstantMaterialProperties : \");\n";
        this->srcFile << "msg += \"material property '\";\n";
        this->srcFile << "msg += " << this->getVariableName(c, md) << ";\n";
        this->srcFile
            << "msg += \"' is not accessible on material '\"+*ptr+\"'\";\n";
        this->srcFile << "msg += \"' (this means that this property is define "
                         "within a behaviour)\";\n";
        this->srcFile << "throw(runtime_error(msg));\n";
        this->srcFile << "}\n";
        this->srcFile << "const MaterialPropertyDescription& mpd = "
                         "*(md.getMaterialProperty("
                      << this->getVariableName(c, md) << "));\n";
        this->srcFile << "if(!mpd.is<CMPD>()){\n";
        this->srcFile << "string msg(\"" << md.className
                      << "::initializeConstantMaterialProperties : \");\n";
        this->srcFile << "msg += \"material property '\";\n";
        this->srcFile << "msg += " << this->getVariableName(c, md) << ";\n";
        this->srcFile
            << "msg += \"' is not constant on material '\"+*ptr+\"'\";\n";
        this->srcFile << "throw(runtime_error(msg));\n";
        this->srcFile << "}\n";
        this->srcFile << "this->constantMaterialProperties[*ptr][\"" << c
                      << "\"]"
                      << " = mpd.get<CMPD>().getValue();\n";
        this->srcFile << "} else {\n";
        if (v.hasAttribute(VariableDescription::defaultValue)) {
          this->srcFile << "this->constantMaterialProperties[*ptr][\"" << c
                        << "\"] = "
                        << v.getAttribute<double>(
                               VariableDescription::defaultValue)
                        << ";\n";
        } else {
          this->srcFile << "string msg(\"" << md.className
                        << "::initializeConstantMaterialProperties : \");\n";
          this->srcFile << "msg += \"material property '\";\n";
          this->srcFile << "msg += " << this->getVariableName(c, md) << ";\n";
          this->srcFile
              << "msg += \"' is undefined on material '\"+*ptr+\"'\";\n";
          this->srcFile << "throw(runtime_error(msg));\n";
        }
        this->srcFile << "}\n";
        this->srcFile << "}\n";
      }
    }
    this->srcFile << "} // end of " << md.className
                  << "::initializeConstantMaterialProperties\n\n";
  }  // end of
     // MFrontModelInterface::writeInitializeConstantMaterialProperties(void)

  void MFrontModelInterface::writeInitializeOutputsVariablesInitialValues(
      const ModelDescription& md) {
    this->srcFile << "void\n"
                  << md.className
                  << "::initializeOutputsVariablesInitialValues("
                  << "const pleiades::parser::DataManager& data)\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "using namespace boost;\n";
    this->srcFile << "using namespace pleiades::glossary;\n";
    this->srcFile << "using namespace pleiades::field;\n";
    this->srcFile << "using std::shared_ptr;\n";
    this->srcFile << "typedef UniformScalarStateVariableDescription USSVD;\n";
    this->srcFile << "typedef std::shared_ptr<USSVD> USSVDPtr;\n";
    this->srcFile << "vector<string>::const_iterator ptr;\n";
    for (const auto& f : md.functions) {
      for (const auto& mv : f.modifiedVariables) {
        // external name
        const auto& en = this->getVariableName(mv, md);
        const auto& v = md.outputs.getVariable(mv);
        this->srcFile << "for(ptr=this->domains.begin();ptr!=this->domains.end("
                         ");++ptr){\n";
        // getting material description
        if (v.hasAttribute(VariableDescription::initialValue)) {
          const auto iv =
              v.getAttribute<double>(VariableDescription::initialValue);
          this->srcFile << "if(md.hasMaterialDescription(*ptr)){\n";
          this->srcFile << "const MaterialDescription& md = "
                           "*(md.getMaterialDescription(*ptr));\n";
          this->srcFile << "if(md.containsStateVariable(" << en << ")){\n";
          this->srcFile << "shared_ptr<StateVariableDescription> tmp = "
                           "md.getStateVariable("
                        << en << ");\n";
          this->srcFile << "if(!this->outputsInitialValues[" << en
                        << "].insert({*ptr,tmp}).second){\n";
          this->srcFile << "string msg(\"" << md.className
                        << "::initializeOutputsVariablesInitialValues : \");\n";
          this->srcFile << "msg += \"output '\";\n";
          this->srcFile << "msg += " << en << ";\n";
          this->srcFile
              << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n";
          this->srcFile << "throw(runtime_error(msg));\n";
          this->srcFile << "}\n";
          this->srcFile << "} else {\n";
          this->srcFile << "if(!this->outputsInitialValues[" << en
                        << "].insert({*ptr,std::make_shared<USSVD>(" << en
                        << "," << iv;
          this->srcFile << ")}).second){\n";
          this->srcFile << "string msg(\"" << md.className
                        << "::initializeOutputsVariablesInitialValues : \");\n";
          this->srcFile << "msg += \"output '\";\n";
          this->srcFile << "msg += " << en << ";\n";
          this->srcFile
              << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n";
          this->srcFile << "throw(runtime_error(msg));\n";
          this->srcFile << "}\n";
          this->srcFile << "}\n";
          this->srcFile << "} else {\n";
          this->srcFile << "if(!this->outputsInitialValues[" << en
                        << "].insert({*ptr,std::make_shared<USSVD>(" << en
                        << "," << iv;
          this->srcFile << ")}).second){\n";
          this->srcFile << "string msg(\"" << md.className
                        << "::initializeOutputsVariablesInitialValues : \");\n";
          this->srcFile << "msg += \"output '\";\n";
          this->srcFile << "msg += " << en << ";\n";
          this->srcFile
              << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n";
          this->srcFile << "throw(runtime_error(msg));\n";
          this->srcFile << "}\n";
          this->srcFile << "}\n";
        } else {
          this->srcFile << "if(!md.hasMaterialDescription(*ptr)){\n";
          this->srcFile << "string msg(\"" << md.className
                        << "::initializeOutputsVariablesInitialValues : \");\n";
          this->srcFile << "msg += \"no material description  on material "
                           "'\"+*ptr+\"', \";\n";
          this->srcFile
              << "msg += \"required to initialize output value '\";\n";
          this->srcFile << "msg += " << en << ";\n";
          this->srcFile << "msg += '\\\'';\n";
          this->srcFile << "throw(runtime_error(msg));\n";
          this->srcFile << "}\n";
          this->srcFile << "const MaterialDescription& md = "
                           "*(md.getMaterialDescription(*ptr));\n";
          this->srcFile << "if(md.containsStateVariable(" << en << ")){\n";
          this->srcFile << "shared_ptr<StateVariableDescription> tmp = "
                           "md.getStateVariable("
                        << en << ");\n";
          this->srcFile << "if(!this->outputsInitialValues[" << en
                        << "].insert({*ptr,tmp}).second){\n";
          this->srcFile << "string msg(\"" << md.className
                        << "::initializeOutputsVariablesInitialValues : \");\n";
          this->srcFile << "msg += \"output '\";\n";
          this->srcFile << "msg += " << en << ";\n";
          this->srcFile
              << "msg += \"' multiply defined on material '\"+*ptr+\"'\";\n";
          this->srcFile << "throw(runtime_error(msg));\n";
          this->srcFile << "}\n";
          this->srcFile << "}\n";
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
    this->srcFile << "void\n"
                  << md.className
                  << "::initializeInputsVariablesDepths(void)\n{\n";
    unsigned int i = 0;
    for (const auto& f : md.functions) {
      for (const auto& v : f.usedVariables) {
        if (isInputVariable(md, v)) {
          ++i;
        }
      }
    }
    if (i != 0) {
      this->srcFile << "using namespace std;\n";
      this->srcFile << "using namespace pleiades::glossary;\n";
      this->srcFile << "vector<string>::const_iterator ptr;\n";
      i = 0;
      for (const auto& f : md.functions) {
        for (const auto& uv : f.usedVariables) {
          if (isInputVariable(md, uv)) {
            const auto& v = decomposeVariableName(md, uv).first;
            if (treatedVars.find(v) == treatedVars.end()) {
              const auto p3 = f.depths.find(v);
              if (p3 == f.depths.end()) {
                throw(std::runtime_error(
                    "MFrontModelInterface::"
                    "writeInitializeInputsVariablesDepths: "
                    "internal error, no depth found for variable '" +
                    v +
                    "' "
                    "in function '" +
                    f.name + "'"));
              }
              this->srcFile << "for(ptr=this->domains.begin();ptr!=this->"
                               "domains.end();++ptr){\n";
              this->srcFile
                  << "map<string,unsigned auto& tmp = this->inputsDepths["
                  << this->getVariableName(v, md) << "];\n";
              if (p3->second == 0) {
                this->srcFile << "if(tmp.find(*ptr)==tmp.end()){\n";
                this->srcFile << "tmp[*ptr]=0;\n";
                this->srcFile << "}\n";
              } else {
                this->srcFile << "if(tmp.find(*ptr)!=tmp.end()){\n";
                this->srcFile << "if(tmp[*ptr]<" << p3->second << "){\n";
                this->srcFile << "tmp[*ptr]=" << p3->second << ";\n";
                this->srcFile << "}\n";
                this->srcFile << "} else {\n";
                this->srcFile << "tmp[*ptr]=" << p3->second << ";\n";
                this->srcFile << "}\n";
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

  std::string MFrontModelInterface::getVariableName(
      const std::string& v, const ModelDescription& md) const {
    if (md.hasGlossaryName(v)) {
      return "Glossary::" + md.getGlossaryName(v);
    }
    return "\"" + md.getExternalName(v) + "\"";
  }  // end of MFrontModelInterface::getVariableName(const std::string& v)

  void MFrontModelInterface::writeStaticVariableInitialization(
      const FileDescription& fd,
      const ModelDescription& md,
      const StaticVariableDescription& v) {
    using namespace std;
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
      string msg("MFrontModelInterface::writeStaticVariableInitialisation : ");
      msg +=
          "type " + v.type + " is not a supported type for a static variable.";
      msg += "Supported types are short, ushort, int, uint, long, ulong,";
      msg += "float, double, ldouble and real.\n";
      msg += "Error at line " + to_string(v.lineNumber);
      throw(runtime_error(msg));
    }
  }  // end of MFrontModelInterface::writeStaticVariableInitialisation

  void MFrontModelInterface::writeOutputFiles(const FileDescription& fd,
                                              const ModelDescription& md) {
    if (md.className.empty()) {
      throw(
          std::runtime_error("MFrontModelInterface::writeOutputFiles: "
                             "no class name defined."));
    }
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
      throw(
          std::runtime_error("MFrontModelInterface::writeAssignDefaultValue: "
                             "type '" +
                             v.type + "' is not supported.\n"));
    }
  }  // end of MFrontModelInterface::writeAssignDefaultValue

  std::pair<bool, MFrontModelInterface::tokens_iterator>
  MFrontModelInterface::treatKeyword(const std::string& k,
                                     const std::vector<std::string>& i,
                                     tokens_iterator c,
                                     const tokens_iterator) {
    if (std::find(i.begin(), i.end(), "licos") != i.end()) {
      throw(
          std::runtime_error("LicosModelInterface::treatKeyword: "
                             "unsupported key '" +
                             k + "'"));
    }
    return {false, c};
  }  // end of MFrontModelInterface::treatKeyword

  std::string MFrontModelInterface::getName() {
    return "@application@";
  }  // end of MFrontModelInterface::getName(void)

  void MFrontModelInterface::getTargetsDescription(TargetsDescription& td,
                                                   const ModelDescription& md) {
    const auto lib = getLibraryName(md);
    td[lib].sources.push_back(md.className + "-@application@.cxx");
    td.headers.push_back("Pleiades/Model/" + md.className +
                         "-@application@.hxx");
    td[lib].cppflags.push_back("`@application@-config --includes`\n");
    td[lib].ldflags.push_back("`@application@-config --libs` -lm");
    td[lib].epts.push_back(md.className);
  }  // end of MFrontModelInterface::getTargetsDescription

  MFrontModelInterface::~MFrontModelInterface() {
  }  // end of MFrontModelInterface::~MFrontModelInterface

  ModelInterfaceProxy<MFrontModelInterface> ModelProxy;

}  // end of namespace mfront
