/*!
 * \file   mfront/tests/models/src/MFrontGerminalModelInterface.cxx
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

#include <cassert>
#include <iterator>
#include <algorithm>

#include "TFEL/Raise.hxx"
#include "TFEL/System/System.hxx"

#include "MFront/MFrontHeader.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/ModelInterfaceProxy.hxx"
#include "MFront/MFrontGerminalModelInterface.hxx"

namespace mfront {

  std::string MFrontGerminalModelInterface::getName() {
    return "germinal";
  }  // end of MFrontGerminalModelInterface::getName()

  std::pair<bool, MFrontGerminalModelInterface::tokens_iterator>
  MFrontGerminalModelInterface::treatKeyword(const std::string& k,
                                             const std::vector<std::string>& i,
                                             tokens_iterator c,
                                             const tokens_iterator) {
    tfel::raise_if(std::find(i.begin(), i.end(), "germinal") != i.end(),
                   "MFrontGerminalModelInterface::treatKeyword: "
                   "unsupported key '" +
                       k + "'");
    return {false, c};
  }  // end of MFrontGerminalModelInterface::treatKeyword

  void MFrontGerminalModelInterface::setOutputFileNames(
      const ModelDescription& md) {
    this->headerFileName = "Pleiades/PMetier/PModels/" + md.className;
    this->headerFileName += "-germinal.hxx";
    this->srcFileName = md.className;
    this->srcFileName += "-germinal.cxx";
  }  // end of MFrontGerminalModelInterface::setOutputFileNames()

  std::string MFrontGerminalModelInterface::getApplicationName() const {
    return "Germinal";
  }  // end of MFrontGerminalModelInterface::getLibraryBaseName

  void MFrontGerminalModelInterface::writeGetConstantMaterialProperty(
      const VariableDescription& v, const ModelDescription& md) {
    std::string name;
    std::string name2;
    const auto en = v.getExternalName();
    if (v.hasGlossaryName()) {
      name = "GlossaireParam::" + en;
      name2 = "this->getMeshZoneName()+\".\"+GlossaireParam::" + en;
    } else {
      name = "\"" + en + "\"";
      name2 = "this->getMeshZoneName()+\"." + en + "\"";
    }
    this->srcFile << "if(arg.contains(" << name2 << ")){\n"
                  << "this->" << v.name << " = arg[" << name2 << "]."
                  << this->getGenTypeMethod(v.type) << "();\n"
                  << "} else if(arg.contains(" << name << ")){\n"
                  << "this->" << v.name << " = arg[" << name << "]."
                  << this->getGenTypeMethod(v.type) << "();\n"
                  << "} else {\n";
    if (v.hasAttribute(VariableDescription::defaultValue)) {
      this->srcFile << "this->" << v.name << " = "
                    << v.getAttribute<double>(VariableDescription::defaultValue)
                    << ";\n";
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
  }  // end of MFrontGerminalModelInterface::writeGetConstantMaterialProperty

  void MFrontGerminalModelInterface::writeInitializeMethod(
      const ModelDescription& md) {
    this->srcFile
        << "bool\n"
        << md.className
        << "::initialize(const Pleiades::PMetier::IArgumentMetier& arg)"
        << "{\n"
        << "using namespace std;\n"
        << "using namespace Pleiades::PUtilitaires;\n"
        << "using namespace Pleiades::PExceptions;\n"
        << "using namespace Pleiades::PMetier::PGlossaire;\n";
    for (const auto& v : md.outputs) {
      std::string name;
      std::string iname;
      const auto& en = v.getExternalName();
      if (v.hasGlossaryName()) {
        name = "GlossaireField::" + en;
        iname = "GlossaireField::" + en + "+\".InitialValue\"";
      } else {
        name = "\"" + en + "\"";
        iname = "\"" + en + ".InitialValue\"";
      }
      this->srcFile << "if(arg.contains(this->getMeshZoneName()+\".\"+" << iname
                    << ")){\n";
      this->srcFile << "this->initializeField(this->_ple" << v.name
                    << ",arg[this->getMeshZoneName()+\".\"+" << iname << "]."
                    << this->getGenTypeMethod("real") << "());\n";
      this->srcFile << "} else if(arg.contains(" << iname << ")){\n";
      this->srcFile << "this->initializeField(this->_ple" << v.name << ",arg["
                    << iname << "]." << this->getGenTypeMethod("real")
                    << "());\n";
      this->srcFile << "} else {\n";
      if (v.hasAttribute(VariableDescription::initialValue)) {
        this->srcFile << "this->initializeField(this->_ple" << v.name << ","
                      << v.getAttribute<double>(
                             VariableDescription::initialValue)
                      << ");\n";
      } else {
        this->srcFile << "string msg(\"" << md.className
                      << "::initializeOutput : \");\n";
        this->srcFile
            << "msg += \"no initial value given for output field '\";\n";
        this->srcFile << "msg += " << name << ";\n";
        this->srcFile << "msg += "
                      << "\"'\";\n";
        this->srcFile << "throw(PleiadesError(msg));\n";
      }
      this->srcFile << "}\n";
    }
    this->srcFile
        << "return true;\n"
        << "} // end of " << md.className << "::initialize\n\n"
        << "void\n"
        << md.className
        << "::initializeField(Pleiades::PMetier::PField::PtrIFieldDouble f,\n"
        << "const double v)\n"
        << "{\n"
        << "using namespace std;\n"
        << "using namespace Pleiades::PUtilitaires;\n"
        << "using namespace Pleiades::PExceptions;\n"
        << "using namespace Pleiades::PMetier::PGlossaire;\n"
        << "const vector<int>& nodes = " << md.className
        << "::getNodesOfCELLZone(f->getMesh(),this->getMeshZoneName());\n"
        << "for(int "
           "node_k=0;node_k!=static_cast<int>(nodes.size());++node_k){\n"
        << "f->setValueType(nodes[node_k],v,0);\n"
        << "}\n"
        << "} // end of " << md.className << "::initializeField\n\n";
  }  // end of MFrontGerminalModelInterface::writeInitializeMethod

  void MFrontGerminalModelInterface::writeInitializeParametersMethod(
      const ModelDescription& md) {
    this->srcFile << "bool\n"
                  << md.className
                  << "::initializeParameters(const "
                     "Pleiades::PMetier::IArgumentMetier& arg)"
                  << "{\n"
                  << "this->domains.insert(this->getMeshZoneName());\n"
                  << "return true;\n"
                  << "} // end of " << md.className
                  << "::initializeParameters\n\n";
  }  // end of MFrontGerminalModelInterface::writeInitializeParametersMethod

  void MFrontGerminalModelInterface::generateDomainsList(
      const ModelDescription&) {
    this->domains.insert("this->getMeshZoneName()");
  }  // end of MFrontGerminalModelInterface::generateDomainsList

  void MFrontGerminalModelInterface::buildDomainName() {
    this->srcFile << "// building current zone name\n";
    this->srcFile << "string domainName = *ptr;\n";
  }

  void MFrontGerminalModelInterface::writeSpecificPrivateMethodDeclaration(
      const FileDescription&, const ModelDescription&) {
    using namespace std;
    this->headerFile
        << "void\n"
        << "initializeField(Pleiades::PMetier::PField::PtrIFieldDouble,\n"
        << "const double);\n\n";
  }  // end of
     // MFrontGerminalModelInterface::writeSpecificPrivateMethodDeclaration

  bool MFrontGerminalModelInterface::initializeDefaultDomainListInConstrutor(
      const ModelDescription&) const {
    return false;
  }  // end of
     // MFrontGerminalModelInterface::initializeDefaultDomainListInConstrutor

  ModelInterfaceProxy<MFrontGerminalModelInterface> ModelGerminalProxy;

}  // end of namespace mfront
