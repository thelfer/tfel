/*!
 * \file  mfront/src/BehaviourInterfaceBase.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 21 juil. 2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/CodeGeneratorUtilities.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/BehaviourInterfaceBase.hxx"

namespace mfront {

  /*!
   * \param[out] rp: if true, a least one floatting-point parameter
   * has been found.
   * \param[out] ip: if true, a least one integer parameter has been
   * found.
   * \param[out] up: if true, a least one parameter of type
   * `unsigned short` has been found.
   * \param[in] pc: list of parameters.
   */
  static void checkParametersType(bool& rp,
                                  bool& ip,
                                  bool& up,
                                  const VariableDescriptionContainer& pc) {
    rp = false;
    ip = false;
    up = false;
    for (const auto& v : pc) {
      if (v.type == "int") {
        ip = true;
      } else if (v.type == "ushort") {
        up = true;
      } else {
        const auto f = SupportedTypes::getTypeFlag(v.type);
        tfel::raise_if(f != SupportedTypes::SCALAR,
                       "checkParametersType: "
                       "unsupport parameter type '" +
                           v.type + "'.");
        rp = true;
      }
    }
  }  // end of checkParametersType

  const char* const BehaviourInterfaceBase::generateMTestFileAttribute =
      "GenerateMTestFileOnFailure";

  BehaviourInterfaceBase::BehaviourInterfaceBase() = default;

  std::string BehaviourInterfaceBase::getInterfaceVersion() const {
    return "";
  }  // end of getInterfaceVersion

  bool BehaviourInterfaceBase::isBehaviourConstructorRequired(
      const Hypothesis h, const BehaviourDescription& mb) const {
    const auto mhs = this->getModellingHypothesesToBeTreated(mb);
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      return !mb.areAllMechanicalDataSpecialised(mhs);
    }
    return mhs.find(h) != mhs.end();
  }  // end of isBehaviourConstructorRequired

  void BehaviourInterfaceBase::writeBehaviourInitializeFunctions(
      std::ostream&, const BehaviourDescription&, const Hypothesis) const {
  }  // end of writeBehaviourInitializeFunctions

  void BehaviourInterfaceBase::writeBehaviourPostProcessings(
      std::ostream&, const BehaviourDescription&, const Hypothesis) const {
  }  // end of writeBehaviourPostProcessings

  std::string BehaviourInterfaceBase::getHeaderGuard(
      const BehaviourDescription& mb) const {
    const auto& m = mb.getMaterialName();
    auto header = "LIB_" + makeUpperCase(this->getInterfaceName());
    if (!mb.getLibrary().empty()) {
      header += "_";
      header += makeUpperCase(mb.getLibrary());
    }
    if (!m.empty()) {
      header += "_";
      header += makeUpperCase(m);
    }
    header += "_";
    header += makeUpperCase(mb.getClassName());
    header += "_HXX";
    return header;
  }  // end of getHeaderGuard

  void BehaviourInterfaceBase::writeVisibilityDefines(std::ostream& out) const {
    mfront::writeExportDirectives(out, false);
  }  // end of writeVisibilityDefines

  std::pair<std::vector<BehaviourMaterialProperty>, SupportedTypes::TypeSize>
  BehaviourInterfaceBase::buildMaterialPropertiesList(
      const BehaviourDescription& bd, const Hypothesis h) const {
    const auto opts = BuildMaterialPropertiesListOptions{
        .useMaterialPropertiesToBuildStiffnessTensor = true,
        .useMaterialPropertiesToBuildThermalExpansionCoefficientTensor = true};
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      return mfront::buildMaterialPropertiesList(
          bd, this->getModellingHypothesesToBeTreated(bd), opts);
    }
    return mfront::buildMaterialPropertiesList(bd, h, opts);
  }  // end of buildMaterialPropertiesList

  void BehaviourInterfaceBase::writeSetOutOfBoundsPolicyFunctionDeclaration(
      std::ostream& out, const std::string& name) const {
    out << "MFRONT_SHAREDOBJ void\n"
        << this->getFunctionNameBasis(name)
        << "_setOutOfBoundsPolicy(const int);\n\n";
  }

  void BehaviourInterfaceBase::writeGetOutOfBoundsPolicyFunctionImplementation(
      std::ostream& out,
      const BehaviourDescription& bd,
      const std::string& name) const {
    if (allowRuntimeModificationOfTheOutOfBoundsPolicy(bd)) {
      out << "static tfel::material::OutOfBoundsPolicy&\n"
          << this->getFunctionNameBasis(name) << "_getOutOfBoundsPolicy(){\n"
          << "static auto policy = tfel::material::"  //
          << getDefaultOutOfBoundsPolicyAsString(bd) << ";\n"
          << "return policy;\n"
          << "}\n\n";
    } else {
      out << "static constexpr tfel::material::OutOfBoundsPolicy\n"
          << this->getFunctionNameBasis(name) << "_getOutOfBoundsPolicy(){\n"
          << "return tfel::material::"  //
          << getDefaultOutOfBoundsPolicyAsString(bd) << ";\n"
          << "}\n\n";
    }
  }  // end of writeGetOutOfBoundsPolicyFunctionImplementation

  void BehaviourInterfaceBase::writeSetOutOfBoundsPolicyFunctionImplementation(
      std::ostream& out,
      const BehaviourDescription& bd,
      const std::string& name) const {
    if (!allowRuntimeModificationOfTheOutOfBoundsPolicy(bd)) {
      out << "MFRONT_SHAREDOBJ void\n"
          << this->getFunctionNameBasis(name)
          << "_setOutOfBoundsPolicy(const int){\n"
          << "}\n\n";
      return;
    }
    out << "MFRONT_SHAREDOBJ void\n"
        << this->getFunctionNameBasis(name)
        << "_setOutOfBoundsPolicy(const int p){\n"
        << "if(p==0){\n"
        << this->getFunctionNameBasis(name)
        << "_getOutOfBoundsPolicy() = tfel::material::None;\n"
        << "} else if(p==1){\n"
        << this->getFunctionNameBasis(name)
        << "_getOutOfBoundsPolicy() = tfel::material::Warning;\n"
        << "} else if(p==2){\n"
        << this->getFunctionNameBasis(name)
        << "_getOutOfBoundsPolicy() = tfel::material::Strict;\n"
        << "} else {\n"
        << "std::cerr << \"" << this->getFunctionNameBasis(name)
        << "_setOutOfBoundsPolicy: invalid argument\\n\";\n"
        << "}\n"
        << "}\n\n";
  }

  void BehaviourInterfaceBase::writeSetParametersFunctionsDeclarations(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& name) const {
    if (areParametersTreatedAsStaticVariables(mb)) {
      return;
    }
    const auto mh = mb.getDistinctModellingHypotheses();
    const auto mh2 = this->getModellingHypothesesToBeTreated(mb);
    for (const auto h : mh) {
      if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
          (mh2.find(h) != mh2.end())) {
        bool rp, ip, up;
        const auto& d = mb.getBehaviourData(h);
        const auto& pc = d.getParameters();
        checkParametersType(rp, ip, up, pc);
        auto fctName = this->getFunctionNameBasis(name);
        auto suffix = std::string{};
        if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
          suffix += ModellingHypothesis::toString(h);
          fctName += "_" + suffix;
        }
        if (rp) {
          out << "MFRONT_SHAREDOBJ int\n"
              << fctName
              << "_setParameter(const char *const,const double);\n\n";
        }
        if (ip) {
          out << "MFRONT_SHAREDOBJ int\n"
              << fctName
              << "_setIntegerParameter(const char *const,const int);\n\n";
        }
        if (up) {
          out << "MFRONT_SHAREDOBJ int\n"
              << fctName << "_setUnsignedShortParameter(const char "
              << "*const,const unsigned short);\n\n";
        }
      }
    }
  }  // end of writeSetParametersFunctionsDeclarations

  void BehaviourInterfaceBase::writeSetParametersFunctionsImplementations(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& name) const {
    if (areParametersTreatedAsStaticVariables(mb)) {
      return;
    }
    const auto mh = mb.getDistinctModellingHypotheses();
    const auto mh2 = this->getModellingHypothesesToBeTreated(mb);
    for (const auto& h : mh) {
      if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
          (mh2.find(h) != mh2.end())) {
        const auto& d = mb.getBehaviourData(h);
        const auto& pc = d.getParameters();
        bool rp, ip, up;
        checkParametersType(rp, ip, up, pc);
        auto fctName = this->getFunctionNameBasis(name);
        std::string suffix;
        if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
          suffix += ModellingHypothesis::toString(h);
          fctName += "_" + suffix;
        }
        const auto cname = mb.getClassName() + suffix + "ParametersInitializer";
        if (rp) {
          out << "MFRONT_SHAREDOBJ int\n"
              << fctName
              << "_setParameter(const char *const key,const double value){\n"
              << "using tfel::material::" << cname << ";\n"
              << "auto& i = " << cname << "::get();\n"
              << "try{\n"
              << "i.set(key,value);\n"
              << "} catch(std::runtime_error& e){\n"
              << "std::cerr << e.what() << std::endl;\n"
              << "return 0;\n"
              << "}\n"
              << "return 1;\n"
              << "}\n\n";
        }
        if (ip) {
          out << "MFRONT_SHAREDOBJ int\n"
              << fctName
              << "_setIntegerParameter(const char *const key,const "
                 "int value){\n"
              << "using tfel::material::" << cname << ";\n"
              << "auto& i = " << cname << "::get();\n"
              << "try{\n"
              << "i.set(key,value);\n"
              << "} catch(std::runtime_error& e){\n"
              << "std::cerr << e.what() << std::endl;\n"
              << "return 0;\n"
              << "}\n"
              << "return 1;\n"
              << "}\n\n";
        }
        if (up) {
          out << "MFRONT_SHAREDOBJ int\n"
              << fctName << "_setUnsignedShortParameter(const char *const "
              << "key,const unsigned short value){\n"
              << "using tfel::material::" << cname << ";\n"
              << "auto& i = " << cname << "::get();\n"
              << "try{\n"
              << "i.set(key,value);\n"
              << "} catch(std::runtime_error& e){\n"
              << "std::cerr << e.what() << std::endl;\n"
              << "return 0;\n"
              << "}\n"
              << "return 1;\n"
              << "}\n\n";
        }
      }
    }
  }

  void BehaviourInterfaceBase::setGenerateMTestFileOnFailureAttribute(
      BehaviourDescription& bd, const bool b) const {
    const auto a = this->getInterfaceName() +
                   "::" + BehaviourInterfaceBase::generateMTestFileAttribute;
    bd.setAttribute(a, b, false);
  }  // end of setGenerateMTestFileOnFailureAttribute

  bool BehaviourInterfaceBase::shallGenerateMTestFileOnFailure(
      const BehaviourDescription& bd) const {
    const auto a = this->getInterfaceName() +
                   "::" + BehaviourInterfaceBase::generateMTestFileAttribute;
    return bd.getAttribute<bool>(a, false);
  }  // end of shallGenerateMTestFileOnFailure

  std::pair<bool, SupportedTypes::TypeSize>
  BehaviourInterfaceBase::checkIfAxialStrainIsDefinedAndGetItsOffset(
      const BehaviourDescription& mb, const Hypothesis h) const {
    using tfel::glossary::Glossary;
    const auto& d = mb.getBehaviourData(h);
    const auto& sv = d.getPersistentVariables();
    SupportedTypes::TypeSize o;
    for (const auto& elem : sv) {
      if (d.getExternalName(elem.name) == Glossary::AxialStrain) {
        return {true, o};
      }
      o += SupportedTypes::getTypeSize(elem.type, elem.arraySize);
    }
    return {false, o};
  }

  std::pair<bool, SupportedTypes::TypeSize> BehaviourInterfaceBase::
      checkIfAxialDeformationGradientIsDefinedAndGetItsOffset(
          const BehaviourDescription& mb, const Hypothesis h) const {
    using tfel::glossary::Glossary;
    const auto& d = mb.getBehaviourData(h);
    const auto& sv = d.getPersistentVariables();
    SupportedTypes::TypeSize o;
    for (const auto& elem : sv) {
      if (d.getExternalName(elem.name) == Glossary::AxialDeformationGradient) {
        return {true, o};
      }
      o += SupportedTypes::getTypeSize(elem.type, elem.arraySize);
    }
    return {false, o};
  }

  bool BehaviourInterfaceBase::readBooleanValue(
      const std::string& key,
      tokens_iterator& current,
      const tokens_iterator end) const {
    auto throw_if = [&key](const bool b, const std::string& m) {
      tfel::raise_if(b, "UMATInterfaceBase::readBooleanValue: " + m +
                            ".\nError while treating key (" + key + ")\n");
    };
    throw_if(current == end, "unexpected end of file");
    const auto b = [&current, &throw_if]() -> bool {
      if (current->value == "true") {
        return true;
      } else if (current->value == "false") {
        return false;
      } else {
        throw_if(true, "expected 'true' or 'false'");
      }
    }();
    ++(current);
    throw_if(current == end, "unexpected end of file");
    throw_if(current->value != ";",
             "expected ';', read '" + current->value + "'");
    ++(current);
    return b;
  }  // end of readBooleanValue

  BehaviourInterfaceBase::~BehaviourInterfaceBase() = default;

}  // end of namespace mfront
