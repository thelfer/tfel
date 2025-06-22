/*!
 * \file  mfront/src/StandardBehaviourInterface.cxx
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
#include "MFront/DSLUtilities.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/StandardBehaviourInterface.hxx"

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

  StandardBehaviourInterface::StandardBehaviourInterface() = default;

  const char* const StandardBehaviourInterface::generateMTestFileAttribute =
      "GenerateMTestFileOnFailure";

  bool StandardBehaviourInterface::isBehaviourConstructorRequired(
      const Hypothesis h, const BehaviourDescription& mb) const {
    const auto mhs = this->getModellingHypothesesToBeTreated(mb);
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      return !mb.areAllMechanicalDataSpecialised(mhs);
    }
    return mhs.find(h) != mhs.end();
  }  // end of StandardBehaviourInterface::isBehaviourConstructorRequired

  std::string StandardBehaviourInterface::getHeaderGuard(
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
  }  // end of StandardBehaviourInterface::getHeaderGuard

  void StandardBehaviourInterface::writeVisibilityDefines(
      std::ostream& out) const {
    out << "#ifdef _WIN32\n"
        << "#ifndef NOMINMAX\n"
        << "#define NOMINMAX\n"
        << "#endif /* NOMINMAX */\n"
        << "#include <windows.h>\n"
        << "#ifdef small\n"
        << "#undef small\n"
        << "#endif /* small */\n"
        << "#endif /* _WIN32 */\n\n"
        << "#ifndef MFRONT_SHAREDOBJ\n"
        << "#define MFRONT_SHAREDOBJ TFEL_VISIBILITY_EXPORT\n"
        << "#endif /* MFRONT_SHAREDOBJ */\n\n";
  }  // end of StandardBehaviourInterface::writeVisibilityDefines

  std::pair<std::vector<BehaviourMaterialProperty>, SupportedTypes::TypeSize>
  StandardBehaviourInterface::buildMaterialPropertiesList(
      const BehaviourDescription& bd, const Hypothesis h) const {
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      return mfront::buildMaterialPropertiesList(
          bd, this->getModellingHypothesesToBeTreated(bd));
    }
    return mfront::buildMaterialPropertiesList(bd, h);
  }  // end of StandardBehaviourInterface::buildMaterialPropertiesList

  void StandardBehaviourInterface::writeSetOutOfBoundsPolicyFunctionDeclaration(
      std::ostream& out, const std::string& name) const {
    out << "MFRONT_SHAREDOBJ void\n"
        << this->getFunctionNameBasis(name)
        << "_setOutOfBoundsPolicy(const int);\n\n";
  }

  void
  StandardBehaviourInterface::writeGetOutOfBoundsPolicyFunctionImplementation(
      std::ostream& out, const std::string& name) const {
    out << "static tfel::material::OutOfBoundsPolicy&\n"
        << this->getFunctionNameBasis(name) << "_getOutOfBoundsPolicy(){\n"
        << "using namespace tfel::material;\n"
        << "static OutOfBoundsPolicy policy = None;\n"
        << "return policy;\n"
        << "}\n\n";
  }  // end of
     // StandardBehaviourInterface::writeGetOutOfBoundsPolicyFunctionImplementation

  void
  StandardBehaviourInterface::writeSetOutOfBoundsPolicyFunctionImplementation(
      std::ostream& out, const std::string& name) const {
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

  void StandardBehaviourInterface::writeSetParametersFunctionsDeclarations(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& name) const {
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
              << fctName
              << "_setUnsignedShortParameter(const char "
                 "*const,const unsigned short);\n\n";
        }
      }
    }
  }  // end of
     // StandardBehaviourInterface::writeSetParametersFunctionsDeclarations

  void StandardBehaviourInterface::writeSetParametersFunctionsImplementations(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& name) const {
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
              << fctName
              << "_setUnsignedShortParameter(const char *const "
                 "key,const unsigned short value){\n"
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

  void StandardBehaviourInterface::setGenerateMTestFileOnFailureAttribute(
      BehaviourDescription& bd, const bool b) const {
    const auto a = this->getInterfaceName() + "::" +
                   StandardBehaviourInterface::generateMTestFileAttribute;
    bd.setAttribute(a, b, false);
  }  // end of
     // StandardBehaviourInterface::setGenerateMTestFileOnFailureAttribute

  bool StandardBehaviourInterface::shallGenerateMTestFileOnFailure(
      const BehaviourDescription& bd) const {
    const auto a = this->getInterfaceName() + "::" +
                   StandardBehaviourInterface::generateMTestFileAttribute;
    return bd.getAttribute<bool>(a, false);
  }  // end of StandardBehaviourInterface::shallGenerateMTestFileOnFailure

  std::pair<bool, SupportedTypes::TypeSize>
  StandardBehaviourInterface::checkIfAxialStrainIsDefinedAndGetItsOffset(
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

  std::pair<bool, SupportedTypes::TypeSize> StandardBehaviourInterface::
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

  bool StandardBehaviourInterface::readBooleanValue(
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
  }  // end of StandardBehaviourInterface::readBooleanValue

  StandardBehaviourInterface::~StandardBehaviourInterface() = default;

}  // end of namespace mfront
