/*!
 * \file   PleiadesModelInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 *
 * g++ -fPIC -DPIC --shared -I ../include/ `tfel-config --includes --oflags
 * --warning` PleiadesModelInterface.cxx -o libPleiadesModelInterface.so export
 * MFRONT_ADDITIONAL_LIBRARIES=/home/th202608/codes/tfel/tests/MFrontArchi/src/libPleiadesModelInterface.so
 */

#include <stdexcept>
#include <iterator>
#include <sstream>
#include <fstream>
#include <cassert>
#include <cctype>
#include <algorithm>

#include "MFront/DSLUtilities.hxx"
#include "TFEL/System/System.hxx"

#include "MFront/MFrontHeader.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/StaticVariableDescription.hxx"
#include "MFront/ModelDSLCommon.hxx"
#include "MFront/ModelInterfaceProxy.hxx"

#include "MFront/PleiadesModelInterface-2.0.hxx"

namespace mfront {

  static std::string getGlossaryName(const std::string& v,
                                     const ModelDescription& mdata) {
    if (mdata.getVariableDescription(v).hasGlossaryName()) {
      return "Glossary::" + mdata.getVariableDescription(v).getExternalName();
    }

    return "\"" + mdata.getVariableDescription(v).getExternalName() + "\"";
  }  // end of getGlossaryName

  PleiadesModelInterface::PleiadesModelInterface() = default;

  void PleiadesModelInterface::declareReservedNames(std::set<std::string>& v) {
    v.insert({"declareMaterialProperty",
              "declarePrivateMaterialProperty",
              "getMaterialProperty",
              "declareLocalFields",
              "declareLocalFields",
              "initializeLocalFields",
              "getLocalFieldsLocalities",
              "getLocalField",
              "getOutputLocalField",
              "getMaterialPropertyField",
              "registerVar",
              "declareField",
              "declareFieldArray",
              "declareParameter",
              "getParameter",
              "declareField",
              "getOutputField",
              "getOutputFieldA",
              "setUniformValueA",
              "getOutputIterField",
              "getOutputIterFieldA",
              "getField",
              "getFieldA",
              "getIterField",
              "getIterFieldA",
              "getPreviousField",
              "getPreviousFieldA",
              "getPreviousIterField",
              "getPreviousIterFieldA"});
  }  // end of PleiadesModelInterface::declareReservedNames

  static void declareField(std::ostream& out,
                           const ModelDescription& mdata,
                           const VariableDescriptionContainer& v,
                           const std::string& type) {
    using namespace std;
    string Material = "Material";
    if (mdata.domains.size() != 0) {
      Material = *(mdata.domains.begin());
      std::size_t found = Material.find_first_of(".*");
      if (found != string::npos) {
        Material.erase(found);
      }
    }

    // VariableDescriptionContainer::const_iterator p;
    for (const auto& p : v) {
      // declareField
      out << "  declareField<double>(\"" << p.name << "\","
          << getGlossaryName(p.name, mdata) << ",\"" << Material << "\","
          << type << ");" << endl;
      // depth
      if (p.hasAttribute(VariableDescription::depth)) {
        const auto d =
            p.getAttribute<unsigned short>(VariableDescription::depth);
        out << "  setTimeDepth(\"" + p.name + "\"," << d + 1 << ");\n";
      }
    }
  }

  static void getField(std::ostream& out,
                       const ModelDescription&,
                       const VariableDescriptionContainer& v,
                       const std::string& type) {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    for (p = v.begin(); p != v.end(); ++p) {
      // getting the field
      if (type == "Output") {
        out << "  Field<double>& " << p->name << " = get" << type
            << "Field<double>(\"" << p->name << "\");" << endl;
      } else {
        out << "  const Field<double>& " << p->name << " = getField<double>(\""
            << p->name << "\");" << endl;
      }
      // depth
      if (p->hasAttribute(VariableDescription::depth)) {
        const auto depth =
            p->getAttribute<unsigned short>(VariableDescription::depth);
        if (depth > 0) {
          for (unsigned short d = 1; d != depth + 1; ++d) {
            out << "  const Field<double>& " << p->name << "_" << d
                << " = getPreviousField<double>(\"" << p->name << "\"," << d
                << ");" << endl;
          }
        }
      }
    }
  }

  static bool useTimeIncrement(const ModelDescription& mdata) {
    for (auto p = mdata.functions.begin(); p != mdata.functions.end(); ++p) {
      if (p->useTimeIncrement) {
        return true;
      }
    }
    return false;
  }

  void PleiadesModelInterface::writeOutputFiles(const FileDescription& fdata,
                                                const ModelDescription& mdata) {
    using namespace std;
    using namespace tfel::system;
    const string file = "src/" + mdata.className + "-" + getName() + ".cxx";
    std::ofstream out(file.c_str());
    if (!out) {
      string msg("PleiadesModelInterface::openOutputFiles : ");
      msg += "unable to open file '" + file + "'";
      throw(runtime_error(msg));
    }
    out.exceptions(ios::badbit | ios::failbit);
    out << "/*!\n";
    out << "* \\file   " << file << endl;
    out << "* \\brief  "
        << "this file declares the " << mdata.className << " Model.\n";
    out << "*         File generated by ";
    out << MFrontHeader::getVersionName() << " ";
    out << "version " << MFrontHeader::getVersionNumber();
    out << endl;
    if (!fdata.authorName.empty()) {
      out << "* \\author " << fdata.authorName << endl;
    }
    if (!fdata.date.empty()) {
      out << "* \\date   " << fdata.date << endl;
    }
    if (!fdata.description.empty()) {
      out << fdata.description << endl;
    }
    out << " */\n\n";
    out << "#include\"Pleiades/pleiades.hh\"\n\n";
    out << "#include\"Pleiades/Examplars/ClassProxy.hh\"\n\n";

    out << "namespace Pleiades\n"
        << "{\n\n";
    out << "/*!" << endl
        << "  * Implementation of the  " << mdata.className << " model" << endl
        << " */" << endl
        << "class " << mdata.className << endl
        << " : public ModelBase {" << endl
        << endl;

    //
    // configure (if needed)
    //
    out << "private:\n";
    for (auto& l : mdata.parameters) {
      out << "  " << l.type << " " << l.name << "; // local parameter\n";
    }

    for (auto& l : mdata.constantMaterialProperties) {
      out << "  " << l.type << " " << l.name
          << "; // constant material Property\n";
    }

    out << "\npublic:\n";

    //
    // declare
    //
    out << "void declare() {\n";
    out << "  // outputs\n";
    declareField(out, mdata, mdata.outputs, "OUT");
    out << "\n  // inputs\n";
    declareField(out, mdata, mdata.inputs, "IN");

    for (auto& l : mdata.parameters) {
      const auto hasDefaultValue =
          l.hasAttribute(VariableDescription::defaultValue);
      if (hasDefaultValue) {
        out << "  declareParameter<" << l.type << ">(\"" << l.name << "\", "
            << getGlossaryName(l.name, mdata) << ", "
            << l.getAttribute<double>(VariableDescription::defaultValue) << ");"
            << endl;
      } else {
        out << "  declareParameter<" << l.type << ">(\"" << l.name << "\", "
            << getGlossaryName(l.name, mdata) << ");" << endl;
      }
    }

    for (auto& l : mdata.constantMaterialProperties) {
      string FQName = getGlossaryName(l.name, mdata);
      out << "  declareParameter<" << l.type << ">(\"" << l.name << "\", "
          << FQName << ");" << endl;
    }

    out << "} // end of " << mdata.className << "::declare\n\n";

    //
    // initialize
    //
    out << "void initialize(const IArgumentMetier& arg) {\n";

    if (!mdata.parameters.empty()) {
      for (auto& l : mdata.parameters) {
        out << "  this->" << l.name << " = getParameter<" << l.type << ">(\""
            << l.name << "\");" << endl;
      }
    }

    if (!mdata.constantMaterialProperties.empty()) {
      for (auto& l : mdata.constantMaterialProperties) {
        out << "  this->" << l.name << " = getParameter<" << l.type << ">(\""
            << l.name << "\");" << endl;
      }
    }

    for (const auto& l : mdata.outputs) {
      if (l.hasAttribute(VariableDescription::initialValue)) {
        out << "  getOutputField<double>(\"" << l.name << "\").setUniformValue("
            << l.getAttribute<double>(VariableDescription::initialValue) << ");"
            << endl;
      }
    }
    out << "}\n\n";  // end of initialize

    //
    // execute
    //
    out << "void execute() {\n";
    out << "  // outputs\n";
    getField(out, mdata, mdata.outputs, "Output");
    out << "\n  // inputs\n";
    getField(out, mdata, mdata.inputs, "Input");
    if (useTimeIncrement(mdata)) {
      out << "  double dt = ITime::getInstance()->timeIncrement();\n";
    }
    for (auto pf = mdata.functions.begin(); pf != mdata.functions.end(); ++pf) {
      set<string>::const_iterator po;
      set<string>::const_iterator pi;
      out << "\n  // calling " << pf->name << endl;
      out << "  for (unsigned int n=0; n != "
          << *(pf->modifiedVariables.begin()) << ".getNumberOfValues(); ++n) {"
          << endl;
      out << "    this->" << pf->name << "(";
      for (po = pf->modifiedVariables.begin();
           po != pf->modifiedVariables.end();) {
        out << *po << "[n]";
        if (++po != pf->modifiedVariables.end()) {
          out << ",";
        }
      }
      for (pi = pf->usedVariables.begin(); pi != pf->usedVariables.end();
           ++pi) {
        out << "," << *pi << "[n]";
      }
      if (pf->useTimeIncrement) {
        out << ",dt";
      }
      out << ");\n";
      out << "  }\n";
    }
    out << "} // end of execute\n\n";
    // functions bodies
    out << "private:\n\n";
    for (auto pf = mdata.functions.begin(); pf != mdata.functions.end(); ++pf) {
      set<string>::const_iterator po;
      set<string>::const_iterator pi;
      out << "void " << pf->name << "(";
      for (po = pf->modifiedVariables.begin();
           po != pf->modifiedVariables.end();) {
        out << "double& " << *po;
        if (++po != pf->modifiedVariables.end()) {
          out << ",";
        }
      }
      for (pi = pf->usedVariables.begin(); pi != pf->usedVariables.end();
           ++pi) {
        out << ", const double& " << *pi;
      }
      if (pf->useTimeIncrement) {
        out << ", const double& dt";
      }
      out << ") const {\n";
      out << "  " << pf->body;
      out << "}\n\n";
    }
    // finalize
    out << "}; // end of class " << mdata.className << endl << endl;
    out << "GENERATE_SPECIALIZABLE_MODEL_PROXY(" << mdata.className << ");\n\n";
    out << "} // end of namespace Pleiades\n\n";
    if (out.is_open()) {
      out.close();
    }
  }  // end of PleiadesModelInterface::writeOutputFiles

  std::pair<bool, PleiadesModelInterface::TokensContainer::const_iterator>
  PleiadesModelInterface::treatKeyword(const std::string& k,
                                       const std::vector<std::string>& i,
                                       TokensContainer::const_iterator c,
                                       const TokensContainer::const_iterator) {
    if (std::find(i.begin(), i.end(), "pleiades-2.0") != i.end()) {
      throw(
          std::runtime_error("PleiadesModelInterface::treatKeyword: "
                             "unsupported key '" +
                             k + "'"));
    }
    return {false, c};
  }  // end of PleiadesModelInterface::treatKeyword

  std::string PleiadesModelInterface::getName(void) {
    return "pleiades-2.0";
  }  // end of PleiadesModelInterface::getName(void)

  void PleiadesModelInterface::getTargetsDescription(
      TargetsDescription& td, const ModelDescription& md) {
    const auto lib = [&md]() -> std::string {
      if (md.library.empty()) {
        if (!md.material.empty()) {
          return "libPleiades" + md.material + "Models";
        } else {
          return "libPleiadesMaterialModels";
        }
      }
      return md.library;
    }();
    auto& l = td.getLibrary(lib);
    l.sources.push_back(md.className + "-" + getName() + ".cxx");
    l.cppflags.push_back("`pleiades-config --includes`");
    l.ldflags.push_back("`pleiades-config --libs` -lm");
    l.epts.push_back(md.className);
  }  // end of MFrontModelInterface::getTargetsDescription

  PleiadesModelInterface::~PleiadesModelInterface() = default;

  ModelInterfaceProxy<PleiadesModelInterface> ModelProxy;

}  // end of namespace mfront
