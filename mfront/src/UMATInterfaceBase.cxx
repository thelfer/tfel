/*!
 * \file  mfront/src/UMATInterfaceBase.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juil. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

#include "TFEL/System/System.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"

#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/UMATInterfaceBase.hxx"

namespace mfront {

  UMATInterfaceBase::UMATMaterialProperty::UMATMaterialProperty(
      const std::string& t,
      const std::string& n,
      const std::string& v,
      const unsigned short a,
      const SupportedTypes::TypeSize o,
      const bool d)
      : type(t),
        name(n),
        var_name(v),
        arraySize(a),
        offset(o),
        dummy(d) {}  // end olf UMATMaterialProperty::UMATMaterialProperty

  UMATInterfaceBase::UMATMaterialProperty::~UMATMaterialProperty() = default;

  const UMATInterfaceBase::UMATMaterialProperty&
  UMATInterfaceBase::findUMATMaterialProperty(
      const std::vector<UMATMaterialProperty>& mprops, const std::string& n) {
    for (const auto& m : mprops) {
      if ((m.name == n) && (!m.dummy)) {
        return m;
      }
    }
    throw(
        std::runtime_error("UMATInterfaceBase::findUMATMaterialProperty : "
                           "no material property associated with the "
                           "glossary name '" +
                           n + "'"));
  }  // end of UMATInterfaceBase::findUMATMaterialProperty

  UMATInterfaceBase::UMATInterfaceBase()
      : generateMTestFile(false) {
  }  // end of UMATInterfaceBase::UMATInterfaceBase

  bool UMATInterfaceBase::isModellingHypothesisHandled(
      const Hypothesis h, const BehaviourDescription& mb) const {
    const auto ih = this->getModellingHypothesesToBeTreated(mb);
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      return !mb.areAllMechanicalDataSpecialised(ih);
    }
    return ih.find(h) != ih.end();
  }

  std::string UMATInterfaceBase::getSymbolName(const std::string& n,
                                               const Hypothesis h) const {
    if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      return this->getFunctionName(n) + "_" + ModellingHypothesis::toString(h);
    }
    return this->getFunctionName(n);
  }  // end of UMATInterfaceBase::getLibraryName

  void UMATInterfaceBase::allowDynamicallyAllocatedArrays(const bool b) {
    this->areDynamicallyAllocatedVectorsAllowed_ = b;
  }  // end of UMATInterfaceBase::allowDynamicallyAllocatedArrays

  void UMATInterfaceBase::appendToMaterialPropertiesList(
      std::vector<UMATMaterialProperty>& l,
      const std::string& t,
      const std::string& n,
      const std::string& v,
      const bool b) const {
    const auto flag = this->getTypeFlag(t);
    if (flag != SupportedTypes::Scalar) {
      throw(
          std::runtime_error("UMATMaterialProperty::UMATMaterialProperty: "
                             "material properties shall be scalars"));
    }
    auto o = SupportedTypes::TypeSize{};
    if (!l.empty()) {
      const auto& m = l.back();
      o = m.offset;
      o += this->getTypeSize(t, 1u);
    }
    l.push_back(UMATMaterialProperty(t, n, v, 1u, o, b));
  }  // end of UMATInterfaceBase::appendToMaterialPropertiesList

  void UMATInterfaceBase::completeMaterialPropertiesList(
      std::vector<UMATMaterialProperty>& mprops,
      const BehaviourDescription& mb,
      const Hypothesis h) const {
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error(
            "UMATInterfaceBase::completeMaterialPropertiesList: " + m));
      }
    };
    const auto& d = mb.getBehaviourData(h);
    const auto& mp = d.getMaterialProperties();
    for (auto p = mp.begin(); p != mp.end(); ++p) {
      const auto& n = mb.getExternalName(h, p->name);
      bool found = false;
      const auto flag = this->getTypeFlag(p->type);
      throw_if(flag != SupportedTypes::Scalar,
               "Invalid type for material property '" + p->name + "' (" +
                   p->type +
                   ").\n"
                   "Material properties shall be scalars");
      for (auto pum = mprops.begin(); (pum != mprops.end()) && (!found);
           ++pum) {
        if (!pum->dummy) {
          if (pum->name == n) {
            // type check
            if (mb.useQt()) {
              throw_if(p->type != pum->type,
                       "UMATInterfaceBase::completeMaterialPropertiesList : "
                       "incompatible type for variable '" +
                           n + "' ('" + p->type + "' vs '" + pum->type + "')");
            } else {
              // don't use quantity
              throw_if(
                  this->getTypeFlag(p->type) != this->getTypeFlag(pum->type),
                  "incompatible type for variable '" + n + "' ('" + p->type +
                      "' vs '" + pum->type + "')");
              if (p->type != pum->type) {
                auto& log = getLogStream();
                log << "UMATInterfaceBase::completeMaterialPropertiesList : "
                    << "inconsistent type for variable '" << n << "' ('"
                    << p->type << "' vs '" << pum->type << "')\n";
              }
            }
            throw_if(p->arraySize != pum->arraySize,
                     "incompatible array size for variable '" + n + "' ('" +
                         p->type + "' vs '" + pum->type + "')");
            found = true;
          }
        }
      }
      if (!found) {
        SupportedTypes::TypeSize o;
        if (!mprops.empty()) {
          const auto& m = mprops.back();
          o = m.offset;
          o += this->getTypeSize(m.type, m.arraySize);
        }
        mprops.push_back(
            UMATMaterialProperty(p->type, n, p->name, p->arraySize, o, false));
      }
    }
  }  // end of UMATInterfaceBase::completeMaterialPropertiesList

  void UMATInterfaceBase::exportMechanicalData(
      std::ostream& out,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    const auto& d = mb.getBehaviourData(h);
    const auto& persistentVarsHolder = d.getPersistentVariables();
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (!persistentVarsHolder.empty()) {
      out << "void\n"
          << iprefix + "exportStateData("
          << "Type * const " << iprefix + "stress_,Type * const "
          << iprefix + "statev) const\n";
    } else {
      out << "void\n"
          << iprefix + "exportStateData("
          << "Type * const " << iprefix << "stress_,Type * const) const\n";
    }
    out << "{\n";
    out << "using namespace tfel::math;\n";
    SupportedTypes::TypeSize of;
    for (const auto& v : mb.getMainVariables()) {
      this->exportThermodynamicForce(out, iprefix + "stress_", v.second, of);
      of += this->getTypeSize(v.second.type, 1u);
    }
    if (!persistentVarsHolder.empty()) {
      this->exportResults(out, persistentVarsHolder, iprefix + "statev",
                          mb.useQt());
    }
    out << "} // end of " << iprefix << "exportStateData\n\n";
  }

  void UMATInterfaceBase::exportThermodynamicForce(
      std::ostream& out,
      const std::string& a,
      const ThermodynamicForce& f,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    const auto flag = this->getTypeFlag(f.type);
    if (flag == SupportedTypes::Scalar) {
      if (!o.isNull()) {
        out << "*(" << a << "+" << o << ") = this->" << f.name << ";\n";
      } else {
        out << "*(" << a << ") = this->" << f.name << ";\n";
      }
    } else if (flag == SupportedTypes::Stensor) {
      if (!o.isNull()) {
        out << "this->sig.exportTab(" << a << "+" << o << ");\n";
      } else {
        out << "this->sig.exportTab(" << a
            << ""
               ");\n";
      }
    } else if ((flag == SupportedTypes::TVector) ||
               (flag == SupportedTypes::Tensor)) {
      if (!o.isNull()) {
        out << "exportToBaseTypeArray(this->" << f.name << "," << a << "+" << o
            << ");\n";
      } else {
        out << "exportToBaseTypeArray(this->" << f.name << "," << a << ");\n";
      }
    } else {
      throw(
          std::runtime_error("UMATInterfaceBase::exportThermodynamicForce: "
                             "unsupported forces type"));
    }
  }  // end of UMATInterfaceBase::exportThermodynamicForce

  std::vector<std::pair<std::string, std::string>>
  UMATInterfaceBase::getBehaviourConstructorsAdditionalVariables() const {
    return {};
  }  // end of UMATInterfaceBase::getBehaviourConstructorsAdditionalVariables

  std::vector<std::pair<std::string, std::string>>
  UMATInterfaceBase::getBehaviourDataConstructorAdditionalVariables() const {
    return {};
  }  // end of UMATInterfaceBase::getBehaviourDataConstructorAdditionalVariables

  std::vector<std::pair<std::string, std::string>>
  UMATInterfaceBase::getIntegrationDataConstructorAdditionalVariables() const {
    return {};
  }  // end of
     // UMATInterfaceBase::getIntegrationDataConstructorAdditionalVariables

  void UMATInterfaceBase::writeBehaviourConstructor(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& initStateVarsIncrements) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    const auto av = this->getBehaviourConstructorsAdditionalVariables();
    const auto abdv = this->getBehaviourDataConstructorAdditionalVariables();
    const auto aidv = this->getIntegrationDataConstructorAdditionalVariables();
    out << "/*\n"
        << " * \\brief constructor for the umat interface\n"
        << " * \\param[in] " << iprefix << "dt_: time increment\n"
        << " * \\param[in] " << iprefix << "T_: temperature\n"
        << " * \\param[in] " << iprefix << "dT_: temperature increment\n"
        << " * \\param[in] " << iprefix << "mat: material properties\n"
        << " * \\param[in] " << iprefix << "int_vars: state variables\n"
        << " * \\param[in] " << iprefix
        << "ext_vars: external state variables\n"
        << " * \\param[in] " << iprefix
        << "dext_vars: external state variables increments\n";
    for (const auto& v : abdv) {
      out << " * \\param[in] " << iprefix << v.first << ":  " << v.second
          << "\n";
    }
    for (const auto& v : aidv) {
      out << " * \\param[in] " << iprefix << v.first << ":  " << v.second
          << "\n";
    }
    for (const auto& v : av) {
      out << " * \\param[in] " << iprefix << v.first << ":  " << v.second
          << "\n";
    }
    out << " */\n"
        << mb.getClassName() << "(const Type* const " << iprefix << "dt_,\n"
        << "const Type* const " << iprefix << "T_,const Type* const " << iprefix
        << "dT_,\n"
        << "const Type* const " << iprefix << "mat,const Type* const "
        << iprefix << "int_vars,\n"
        << "const Type* const " << iprefix << "ext_vars,const Type* const "
        << iprefix << "dext_vars";
    for (const auto& v : abdv) {
      out << ",\nconst Type* const " << iprefix << v.first;
    }
    for (const auto& v : aidv) {
      out << ",\nconst Type* const " << iprefix << v.first;
    }
    for (const auto& v : av) {
      out << ",\nconst Type* const " << iprefix << v.first;
    }
    out << ")\n";
    if (mb.useQt()) {
      out << ": " << mb.getClassName()
          << "BehaviourData<hypothesis,Type,use_qt>(" << iprefix << "T_,"
          << iprefix << "mat,\n"
          << iprefix + "int_vars," << iprefix << "ext_vars";
      for (const auto& v : abdv) {
        out << "," << iprefix << v.first;
      }
      for (const auto& v : av) {
        out << "," << iprefix << v.first;
      }
      out << "),\n"
          << mb.getClassName() << "IntegrationData<hypothesis,Type,use_qt>("
          << iprefix << "dt_," << iprefix << "dT_," << iprefix << "dext_vars";
      for (const auto& v : aidv) {
        out << "," << iprefix << v.first;
      }
      for (const auto& v : av) {
        out << "," << iprefix << v.first;
      }
      out << ")";
    } else {
      out << ": " << mb.getClassName()
          << "BehaviourData<hypothesis,Type,false>(" << iprefix << "T_,"
          << iprefix << "mat,\n"
          << iprefix + "int_vars," << iprefix << "ext_vars";
      for (const auto& v : abdv) {
        out << "," << iprefix << v.first;
      }
      for (const auto& v : av) {
        out << "," << iprefix << v.first;
      }
      out << "),\n"
          << mb.getClassName() << "IntegrationData<hypothesis,Type,false>("
          << iprefix << "dt_," << iprefix << "dT_," << iprefix << "dext_vars";
      for (const auto& v : aidv) {
        out << "," << iprefix << v.first;
      }
      for (const auto& v : av) {
        out << "," << iprefix << v.first;
      }
      out << ")";
    }
    if (!initStateVarsIncrements.empty()) {
      out << ",\n" << initStateVarsIncrements;
    }
  }  // end of UMATInterfaceBase::writeBehaviourConstructor

  void UMATInterfaceBase::
      writeMaterialPropertiesInitializersInBehaviourDataConstructorI(
          std::ostream& f,
          const Hypothesis h,
          const BehaviourDescription& mb,
          const std::vector<UMATMaterialProperty>& i,
          const SupportedTypes::TypeSize ioffset,
          const std::string& src,
          const std::string& prefix,
          const std::string& suffix) const {
    const auto& d = mb.getBehaviourData(h);
    const auto& v = d.getMaterialProperties();
    if (!v.empty()) {
      for (auto p = v.cbegin(); p != v.cend(); ++p) {
        if (p->arraySize == 1u) {
          const auto n = prefix + p->name + suffix;
          const auto& m = UMATInterfaceBase::findUMATMaterialProperty(
              i, mb.getExternalName(h, p->name));
          auto offset = m.offset;
          offset -= ioffset;
          f << ",\n";
          auto flag = this->getTypeFlag(p->type);
          if (flag == SupportedTypes::Scalar) {
            f << n << "(" + src + "[" << offset << "])";
          } else if ((flag == SupportedTypes::TVector) ||
                     (flag == SupportedTypes::Stensor) ||
                     (flag == SupportedTypes::Tensor)) {
            f << n << "(&" + src + "[" << offset << "])";
          } else {
            throw(
                std::runtime_error("SupportedTypes::"
                                   "writeMaterialPropertiesInitializersInBehavi"
                                   "ourDataConstructorI: "
                                   "internal error, tag unsupported"));
          }
        }
      }
    }

  }  // end of
     // UMATInterfaceBase::writeMaterialPropertiesInitializersInBehaviourDataConstructorI

  void UMATInterfaceBase::
      writeMaterialPropertiesInitializersInBehaviourDataConstructorII(
          std::ostream& f,
          const Hypothesis h,
          const BehaviourDescription& mb,
          const std::vector<UMATMaterialProperty>& i,
          const SupportedTypes::TypeSize ioffset,
          const std::string& src,
          const std::string& prefix,
          const std::string& suffix) const {
    const auto& d = mb.getBehaviourData(h);
    const auto& v = d.getMaterialProperties();
    if (!v.empty()) {
      for (auto p = v.begin(); p != v.end(); ++p) {
        if (p->arraySize != 1u) {
          const auto& m = UMATInterfaceBase::findUMATMaterialProperty(
              i, mb.getExternalName(h, p->name));
          const auto flag = this->getTypeFlag(p->type);
          SupportedTypes::TypeSize offset = m.offset;
          offset -= ioffset;
          const auto n = prefix + p->name + suffix;
          if (this->useDynamicallyAllocatedVector(p->arraySize)) {
            f << n << ".resize(" << p->arraySize << ");\n";
            f << "for(unsigned short idx=0;idx!=" << p->arraySize
              << ";++idx){\n";
            switch (flag) {
              case SupportedTypes::Scalar:
                f << n << "[idx] = " + src + "[" << offset << "+idx];\n";
                break;
              case SupportedTypes::TVector:
                f << "tfel::fsalgo::copy<TVectorSize>::exe(&" + src + "["
                  << offset << "+idx*TVectorSize]," << n << "[idx].begin());\n";
                break;
              case SupportedTypes::Stensor:
                f << n << "[idx].import(&" + src + "[" << offset
                  << "+idx*StensorSize]);\n";
                break;
              case SupportedTypes::Tensor:
                f << "tfel::fsalgo::copy<TensorSize>::exe(&" + src + "["
                  << offset << "+idx*TensorSize]," << n << "[idx].begin());\n";
                break;
              default:
                throw(std::runtime_error(
                    "UMATInterfaceBase::"
                    "writeVariableInitializersInBehaviourDataConstructorII: "
                    "internal error, tag unsupported"));
            }
            f << "}\n";
          } else {
            for (int index = 0; index != p->arraySize; ++index) {
              switch (flag) {
                case SupportedTypes::Scalar:
                  f << n << "[" << index << "] = " + src + "[" << offset
                    << "];\n";
                  break;
                case SupportedTypes::TVector:
                  f << "tfel::fsalgo::copy<TVectorSize>::exe(&" + src + "["
                    << offset << "]," << n << "[" << index << "].begin());\n";
                  break;
                case SupportedTypes::Stensor:
                  f << n << "[" << index << "].import(&" + src + "[" << offset
                    << "]);\n";
                  break;
                case SupportedTypes::Tensor:
                  f << "tfel::fsalgo::copy<TensorSize>::exe(&" + src + "["
                    << offset << "]," << n << "[" << index << "].begin());\n";
                  break;
                default:
                  throw(std::runtime_error(
                      "UMATInterfaceBase::"
                      "writeVariableInitializersInBehaviourDataConstructorII: "
                      "internal error, tag unsupported"));
              }
              offset += this->getTypeSize(p->type, 1u);
            }
          }
        }
      }
    }
  }  // end of
     // UMATInterfaceBase::writeMaterialPropertiesInitializersInBehaviourDataConstructorII

  void UMATInterfaceBase::writeBehaviourDataConstructor(
      std::ostream& out,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    const auto av = this->getBehaviourConstructorsAdditionalVariables();
    const auto abdv = this->getBehaviourDataConstructorAdditionalVariables();
    const auto& d = mb.getBehaviourData(h);
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    auto mprops = this->buildMaterialPropertiesList(mb, h);
    const auto& mp = d.getMaterialProperties();
    const auto& persistentVarsHolder = d.getPersistentVariables();
    const auto& externalStateVarsHolder = d.getExternalStateVariables();
    out << "/*\n"
        << " * \\brief constructor for the umat interface\n"
        << " * \\param[in] " << iprefix << "T_: temperature\n"
        << " * \\param[in] " << iprefix << "mat: material properties\n"
        << " * \\param[in] " << iprefix << "int_vars: state variables\n"
        << " * \\param[in] " << iprefix
        << "ext_vars: external std::ate variables\n";
    for (const auto& v : abdv) {
      out << " * \\param[in] " << iprefix << v.first << ":  " << v.second
          << "\n";
    }
    for (const auto& v : av) {
      out << " * \\param[in] " << iprefix << v.first << ":  " << v.second
          << "\n";
    }
    out << " */\n"
        << mb.getClassName() << "BehaviourData"
        << "(const Type* const " << iprefix << "T_,const Type* const";
    if (!mp.empty()) {
      out << " " << iprefix << "mat,\n";
    } else {
      out << ",\n";
    }
    out << "const Type* const";
    if (!persistentVarsHolder.empty()) {
      out << " " << iprefix << "int_vars,\n";
    } else {
      out << ",\n";
    }
    out << "const Type* const";
    if (!externalStateVarsHolder.empty()) {
      out << " " << iprefix << "ext_vars";
    }
    for (const auto& v : abdv) {
      out << ",const Type* const " << iprefix << v.first;
    }
    for (const auto& v : av) {
      out << ",const Type* const " << iprefix << v.first;
    }
    out << ")\n";
    out << ": T(*" << iprefix << "T_)";
    this->writeMaterialPropertiesInitializersInBehaviourDataConstructorI(
        out, h, mb, mprops.first, mprops.second, iprefix + "mat", "", "");
    this->writeVariableInitializersInBehaviourDataConstructorI(
        out, persistentVarsHolder, iprefix + "int_vars", "", "");
    this->writeVariableInitializersInBehaviourDataConstructorI(
        out, externalStateVarsHolder, iprefix + "ext_vars", "", "");
    out << "\n{\n";
    this->writeMaterialPropertiesInitializersInBehaviourDataConstructorII(
        out, h, mb, mprops.first, mprops.second, iprefix + "mat", "", "");
    this->writeVariableInitializersInBehaviourDataConstructorII(
        out, persistentVarsHolder, iprefix + "int_vars", "", "");
    this->writeVariableInitializersInBehaviourDataConstructorII(
        out, externalStateVarsHolder, iprefix + "ext_vars", "", "");
    this->completeBehaviourDataConstructor(out, h, mb);
    out << "}\n\n";
  }

  void UMATInterfaceBase::completeBehaviourDataConstructor(
      std::ostream&, const Hypothesis, const BehaviourDescription&) const {
  }  // end of UMATInterfaceBase::completeBehaviourDataConstructor

  void UMATInterfaceBase::writeIntegrationDataConstructor(
      std::ostream& out,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    const auto av = this->getBehaviourConstructorsAdditionalVariables();
    const auto aidv = this->getIntegrationDataConstructorAdditionalVariables();
    const auto& d = mb.getBehaviourData(h);
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    const auto& externalStateVarsHolder = d.getExternalStateVariables();
    out << "/*\n"
        << " * \\brief constructor for the umat interface\n"
        << " * \\param[in] " + iprefix + "dt_: time increment\n"
        << " * \\param[in] " + iprefix + "dT_: temperature increment\n"
        << " * \\param[in] " + iprefix +
               "dext_vars: external state variables increments\n";
    for (const auto& v : aidv) {
      out << " * \\param[in] " << iprefix << v.first << ":  " << v.second
          << "\n";
    }
    for (const auto& v : av) {
      out << " * \\param[in] " << iprefix << v.first << ":  " << v.second
          << "\n";
    }
    out << " */\n"
        << mb.getClassName() << "IntegrationData"
        << "(const Type* const " << iprefix << "dt_,\n"
        << "const Type* const " << iprefix << "dT_,const Type* const";
    if (!externalStateVarsHolder.empty()) {
      out << " " << iprefix << "dext_vars";
    }
    for (const auto& v : aidv) {
      out << ",const Type* const " << iprefix << v.first;
    }
    for (const auto& v : av) {
      out << ",const Type* const " << iprefix << v.first;
    }
    out << ")\n";
    out << ": dt(*" << iprefix << "dt_),dT(*" << iprefix << "dT_)";
    if (!externalStateVarsHolder.empty()) {
      this->writeVariableInitializersInBehaviourDataConstructorI(
          out, externalStateVarsHolder, iprefix + "dext_vars", "d", "");
    }
    out << "\n{\n";
    if (!externalStateVarsHolder.empty()) {
      this->writeVariableInitializersInBehaviourDataConstructorII(
          out, externalStateVarsHolder, iprefix + "dext_vars", "d", "");
    }
    out << "}\n\n";
  }

  void UMATInterfaceBase::writeBehaviourDataMainVariablesSetters(
      std::ostream& os, const BehaviourDescription& mb) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    SupportedTypes::TypeSize ov, of;
    os << "void set" << iprefix
       << "BehaviourDataDrivingVariables(const Type* const " << iprefix
       << "stran)\n"
       << "{\n";
    for (const auto& v : mb.getMainVariables()) {
      this->writeBehaviourDataDrivingVariableSetter(os, v.first, ov);
      ov += this->getTypeSize(v.first.type, 1u);
    }
    os << "}\n\n";
    os << "void set" << iprefix
       << "BehaviourDataThermodynamicForces(const Type* const " << iprefix
       << "stress_)\n"
       << "{\n";
    for (const auto& v : mb.getMainVariables()) {
      this->writeBehaviourDataThermodynamicForceSetter(os, v.second, of);
      of += this->getTypeSize(v.second.type, 1u);
    }
    os << "}\n\n";
  }  // end of UMATInterfaceBase::writeBehaviourDataMainVariablesSetters

  void UMATInterfaceBase::writeBehaviourDataDrivingVariableSetter(
      std::ostream& os,
      const DrivingVariable& v,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (!v.increment_known) {
      if (this->getTypeFlag(v.type) == SupportedTypes::TVector) {
        if (!o.isNull()) {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "stran+" << o
             << ",this->" << v.name << "0.begin());\n";
        } else {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "stran,this->"
             << v.name << "0.begin());\n";
        }
      } else if (this->getTypeFlag(v.type) == SupportedTypes::Stensor) {
        if (!o.isNull()) {
          os << "this->" << v.name << "0.importVoigt(" << iprefix << "stran+"
             << o << ");\n";
        } else {
          os << "this->" << v.name << "0.importVoigt(" << iprefix
             << "stran);\n";
        }
      } else if (this->getTypeFlag(v.type) == SupportedTypes::Tensor) {
        if (!o.isNull()) {
          os << v.type << "::buildFromFortranMatrix(this->" << v.name << "0,"
             << iprefix << "stran+" << o << ");\n";
        } else {
          os << v.type << "::buildFromFortranMatrix(this->" << v.name << "0,"
             << iprefix << "stran);\n";
        }
      } else {
        throw(std::runtime_error(
            "UMATInterfaceBase::writeBehaviourDataMainVariablesSetters : "
            "unsupported driving variable type"));
      }
    } else {
      if (this->getTypeFlag(v.type) == SupportedTypes::TVector) {
        if (!o.isNull()) {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "stran+" << o
             << ",this->" << v.name << ".begin());\n";
        } else {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "stran,this->"
             << v.name << ".begin());\n";
        }
      } else if (this->getTypeFlag(v.type) == SupportedTypes::Stensor) {
        if (!o.isNull()) {
          os << "this->" << v.name << ".importVoigt(" << iprefix << "stran+"
             << o << ");\n";
        } else {
          os << "this->" << v.name << ".importVoigt(" << iprefix << "stran);\n";
        }
      } else if (this->getTypeFlag(v.type) == SupportedTypes::Tensor) {
        if (!o.isNull()) {
          os << v.type << "::buildFromFortranMatrix(this->" << v.name << ","
             << iprefix << "stran+" << o << ");\n";
        } else {
          os << v.type << "::buildFromFortranMatrix(this->" << v.name << ","
             << iprefix + "stran);\n";
        }
      } else {
        throw(std::runtime_error(
            "UMATInterfaceBase::writeBehaviourDataMainVariablesSetters : "
            "unsupported driving variable type"));
      }
    }
  }  // end of UMATInterfaceBase::writeBehaviourDataDrivingVariableSetter

  void UMATInterfaceBase::writeBehaviourDataThermodynamicForceSetter(
      std::ostream& os,
      const ThermodynamicForce& f,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (this->getTypeFlag(f.type) == SupportedTypes::TVector) {
      if (!o.isNull()) {
        os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "stress_+" << o
           << ",this->" << f.name << ".begin());\n";
      } else {
        os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "stress_,this->"
           << f.name << ".begin());\n";
      }
    } else if (this->getTypeFlag(f.type) == SupportedTypes::Stensor) {
      if (!o.isNull()) {
        os << "this->" << f.name << ".importTab(" << iprefix << "stress_+" << o
           << ");\n";
      } else {
        os << "this->" << f.name << ".importTab(" << iprefix << "stress_);\n";
      }
    } else if (this->getTypeFlag(f.type) == SupportedTypes::Tensor) {
      if (!o.isNull()) {
        os << f.type << "::buildFromFortranMatrix(this->" << f.name << ","
           << iprefix << "stress_+" << o << ");\n";
      } else {
        os << f.type << "::buildFromFortranMatrix(this->" << f.name << ","
           << iprefix << "stress_);\n";
      }
    } else {
      throw(std::runtime_error(
          "UMATInterfaceBase::writeBehaviourDataMainVariablesSetters : "
          "unsupported forces type"));
    }
  }  // end of UMATInterfaceBase::writeBehaviourDataThermodynamicForceSetter

  void UMATInterfaceBase::writeIntegrationDataMainVariablesSetters(
      std::ostream& os, const BehaviourDescription& mb) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    SupportedTypes::TypeSize ov;
    os << "void set" << iprefix
       << "IntegrationDataDrivingVariables(const Type* const " << iprefix
       << "dstran)\n"
       << "{\n";
    for (const auto& v : mb.getMainVariables()) {
      this->writeIntegrationDataDrivingVariableSetter(os, v.first, ov);
      ov += this->getTypeSize(v.first.type, 1u);
    }
    os << "}\n\n";
  }  // end of writeIntegrationDataMainVariablesSetters

  void UMATInterfaceBase::writeIntegrationDataDrivingVariableSetter(
      std::ostream& os,
      const DrivingVariable& v,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (!v.increment_known) {
      if (this->getTypeFlag(v.type) == SupportedTypes::TVector) {
        if (!o.isNull()) {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "dstran+" << o
             << ",this->" << v.name << "1.begin());\n";
        } else {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "dstran,this->"
             << v.name << "1.begin());\n";
        }
      } else if (this->getTypeFlag(v.type) == SupportedTypes::Stensor) {
        if (!o.isNull()) {
          os << "this->" << v.name << "1.importVoigt(" << iprefix << "dstran+"
             << o << ");\n";
        } else {
          os << "this->" << v.name << "1.importVoigt(" << iprefix
             << "dstran);\n";
        }
      } else if (this->getTypeFlag(v.type) == SupportedTypes::Tensor) {
        if (!o.isNull()) {
          os << v.type << "::buildFromFortranMatrix(this->" << v.name << "1,"
             << iprefix << "dstran+" << o << ");\n";
        } else {
          os << v.type << "::buildFromFortranMatrix(this->" << v.name << "1,"
             << iprefix << "dstran);\n";
        }
      } else {
        throw(std::runtime_error(
            "UMATInterfaceBase::writeIntegrationDataMainVariablesSetters : "
            "unsupported driving variable type"));
      }
    } else {
      if (this->getTypeFlag(v.type) == SupportedTypes::TVector) {
        if (!o.isNull()) {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "dstran+" << o
             << ",this->d" << v.name << ".begin());\n";
        } else {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "dstran,this->d"
             << v.name << ".begin());\n";
        }
      } else if (this->getTypeFlag(v.type) == SupportedTypes::Stensor) {
        if (!o.isNull()) {
          os << "this->d" << v.name << ".importVoigt(" << iprefix << "dstran+"
             << o << ");\n";
        } else {
          os << "this->d" << v.name << ".importVoigt(" << iprefix
             << "dstran);\n";
        }
      } else if (this->getTypeFlag(v.type) == SupportedTypes::Tensor) {
        if (!o.isNull()) {
          os << v.type << "::buildFromFortranMatrix(this->d" << v.name << ","
             << iprefix << "dstran+" << o << ");\n";
        } else {
          os << v.type << "::buildFromFortranMatrix(this->d" << v.name << ","
             << iprefix + "dstran);\n";
        }
      } else {
        throw(std::runtime_error(
            "UMATInterfaceBase::writeIntegrationDataMainVariablesSetters : "
            "unsupported driving variable type"));
      }
    }
  }  // end of UMATInterfaceBase::writeIntegrationDataDrivingVariableSetter

  void UMATInterfaceBase::writeExternalNames(std::ostream& f,
                                             const std::string& name,
                                             const Hypothesis& h,
                                             const std::vector<std::string>& v,
                                             const std::string& t) const {
    if (v.empty()) {
      f << "MFRONT_SHAREDOBJ const char * const * "
        << this->getSymbolName(name, h) << "_" << t << " = nullptr;\n\n";
    } else {
      std::vector<std::string>::size_type s = 0u;
      std::vector<std::string>::const_iterator p = v.begin();
      f << "MFRONT_SHAREDOBJ const char * " << this->getSymbolName(name, h)
        << "_" << t << "[" << v.size() << "] = {";
      while (p != v.end()) {
        f << '"' << *p << '"';
        if (++p != v.end()) {
          if (s % 5 == 0) {
            f << ",\n";
          } else {
            f << ",";
          }
        }
        ++s;
      }
      f << "};\n";
    }
  }  // end of UMATInterfaceBase::writeExternalNames

  bool UMATInterfaceBase::readBooleanValue(const std::string& key,
                                           tokens_iterator& current,
                                           const tokens_iterator end) const {
    auto throw_if = [&key](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error("UMATInterfaceBase::readBooleanValue: " + m +
                                 ".\nError while treating key (" + key +
                                 ")\n"));
      }
    };
    bool b = true;
    throw_if(current == end, "unexpected end of file");
    if (current->value == "true") {
      b = true;
    } else if (current->value == "false") {
      b = false;
    } else {
      throw_if(true, "expected 'true' or 'false'");
    }
    ++(current);
    throw_if(current == end, "unexpected end of file");
    throw_if(current->value != ";",
             "expected ';', read '" + current->value + "'");
    ++(current);
    return b;
  }

  void UMATInterfaceBase::writeVisibilityDefines(std::ostream& out) const {
    out << "#ifdef _WIN32\n"
        << "#ifndef NOMINMAX\n"
        << "#define NOMINMAX\n"
        << "#endif /* NOMINMAX */\n"
        << "#include <windows.h>\n"
        << "#endif /* _WIN32 */\n\n"
        << "#ifndef MFRONT_SHAREDOBJ\n"
        << "#define MFRONT_SHAREDOBJ TFEL_VISIBILITY_EXPORT\n"
        << "#endif /* MFRONT_SHAREDOBJ */\n\n";
  }  // end of UMATInterfaceBase::writeVisibilityDefines

  void UMATInterfaceBase::checkParametersType(
      bool& rp,
      bool& ip,
      bool& up,
      const VariableDescriptionContainer& pc) const {
    rp = false;
    ip = false;
    up = false;
    for (const auto& v : pc) {
      if (v.type == "real") {
        rp = true;
      } else if (v.type == "int") {
        ip = true;
      } else if (v.type == "ushort") {
        up = true;
      } else {
        throw(
            std::runtime_error("UMATInterfaceBase::checkParametersType: "
                               "unsupport parameter type '" +
                               v.type + "'."));
      }
    }
  }

  void UMATInterfaceBase::writeSetOutOfBoundsPolicyFunctionDeclaration(
      std::ostream& out, const std::string& name) const {
    out << "MFRONT_SHAREDOBJ void\n"
        << this->getFunctionName(name)
        << "_setOutOfBoundsPolicy(const int);\n\n";
  }

  void UMATInterfaceBase::writeGetOutOfBoundsPolicyFunctionImplementation(
      std::ostream& out, const std::string& name) const {
    out << "static tfel::material::OutOfBoundsPolicy&\n"
        << this->getFunctionName(name) << "_getOutOfBoundsPolicy(void){\n"
        << "using namespace tfel::material;\n"
        << "static OutOfBoundsPolicy policy = None;\n"
        << "return policy;\n"
        << "}\n\n";
  }  // end of
     // UMATInterfaceBase::writeGetOutOfBoundsPolicyFunctionImplementation

  void UMATInterfaceBase::writeSetOutOfBoundsPolicyFunctionImplementation(
      std::ostream& out, const std::string& name) const {
    out << "MFRONT_SHAREDOBJ void\n"
        << this->getFunctionName(name)
        << "_setOutOfBoundsPolicy(const int p){\n"
        << "if(p==0){\n"
        << this->getFunctionName(name)
        << "_getOutOfBoundsPolicy() = tfel::material::None;\n"
        << "} else if(p==1){\n"
        << this->getFunctionName(name)
        << "_getOutOfBoundsPolicy() = tfel::material::Warning;\n"
        << "} else if(p==2){\n"
        << this->getFunctionName(name)
        << "_getOutOfBoundsPolicy() = tfel::material::Strict;\n"
        << "} else {\n"
        << "std::cerr << \"" << this->getFunctionName(name)
        << "_setOutOfBoundsPolicy : invalid argument\\n\";\n"
        << "}\n"
        << "}\n\n";
  }

  void UMATInterfaceBase::writeSetParametersFunctionsDeclarations(
      std::ostream& out,
      const std::string& name,
      const BehaviourDescription& mb) const {
    const auto mh = mb.getDistinctModellingHypotheses();
    const auto mh2 = this->getModellingHypothesesToBeTreated(mb);
    for (const auto h : mh) {
      if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
          (mh2.find(h) != mh2.end())) {
        bool rp, ip, up;
        const auto& d = mb.getBehaviourData(h);
        const auto& pc = d.getParameters();
        this->checkParametersType(rp, ip, up, pc);
        auto fctName = this->getFunctionName(name);
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
              << "_setUnsignedShortParameter(const char *const,const unsigned "
                 "short);\n\n";
        }
      }
    }
  }  // end of UMATInterfaceBase::writeSetParametersFunctionsDeclarations

  void UMATInterfaceBase::writeSetParametersFunctionsImplementations(
      std::ostream& out,
      const std::string& name,
      const BehaviourDescription& mb) const {
    using namespace std;
    set<Hypothesis> h = mb.getDistinctModellingHypotheses();
    set<Hypothesis> h2 = this->getModellingHypothesesToBeTreated(mb);
    set<Hypothesis>::const_iterator p;
    for (p = h.begin(); p != h.end(); ++p) {
      if ((*p == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
          (h2.find(*p) != h2.end())) {
        const auto& d = mb.getBehaviourData(*p);
        const auto& pc = d.getParameters();
        bool rp, ip, up;
        this->checkParametersType(rp, ip, up, pc);
        string fctName = this->getFunctionName(name);
        string suffix;
        if (*p != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
          suffix += ModellingHypothesis::toString(*p);
          fctName += "_" + suffix;
        }
        string cname = mb.getClassName() + suffix + "ParametersInitializer";
        if (rp) {
          out << "MFRONT_SHAREDOBJ int\n"
              << fctName
              << "_setParameter(const char *const key,const double value){\n"
              << "using namespace std;\n"
              << "using namespace tfel::material;\n"
              << cname << "& i = " << cname << "::get();\n"
              << "try{\n"
              << "i.set(key,value);\n"
              << "} catch(runtime_error& e){\n"
              << "cerr << e.what() << endl;\n"
              << "return 0;\n"
              << "}\n"
              << "return 1;\n"
              << "}\n\n";
        }
        if (ip) {
          out << "MFRONT_SHAREDOBJ int\n"
              << fctName
              << "_setIntegerParameter(const char *const key,const int "
                 "value){\n"
              << "using namespace std;\n"
              << "using namespace tfel::material;\n"
              << cname << "& i = " << cname << "::get();\n"
              << "try{\n"
              << "i.set(key,value);\n"
              << "} catch(runtime_error& e){\n"
              << "cerr << e.what() << endl;\n"
              << "return 0;\n"
              << "}\n"
              << "return 1;\n"
              << "}\n\n";
        }
        if (up) {
          out << "MFRONT_SHAREDOBJ int\n"
              << fctName
              << "_setUnsignedShortParameter(const char *const key,const "
                 "unsigned short value){\n"
              << "using namespace std;\n"
              << "using namespace tfel::material;\n"
              << cname << "& i = " << cname << "::get();\n"
              << "try{\n"
              << "i.set(key,value);\n"
              << "} catch(runtime_error& e){\n"
              << "cerr << e.what() << endl;\n"
              << "return 0;\n"
              << "}\n"
              << "return 1;\n"
              << "}\n\n";
        }
      }
    }
  }

  std::string UMATInterfaceBase::getHeaderDefine(
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
  }

  void UMATInterfaceBase::getExtraSrcIncludes(
      std::ostream& out, const BehaviourDescription& mb) const {
    if (mb.hasParameters()) {
      out << "#include<iostream>\n"
          << "#include<stdexcept>\n";
    }
    if (this->generateMTestFile) {
      out << "#include<vector>\n"
          << "#include<sstream>\n"
          << "#include\"TFEL/Material/ModellingHypothesis.hxx\"\n"
          << "#include\"MFront/SupportedTypes.hxx\"\n"
          << "#include\"MFront/UmatSmallStrainMTestFileGenerator.hxx\"\n"
          << "#include\"MFront/UmatFiniteStrainMTestFileGenerator.hxx\"\n";
    }
  }  // end of UMATInterfaceBase::getExtraSrcIncludes

  void UMATInterfaceBase::generateMTestFile1(std::ostream& out) const {
    if (this->generateMTestFile) {
      out << "using namespace std;\n"
          << "using tfel::material::ModellingHypothesis;\n"
          << "using mfront::SupportedTypes;\n"
          << "// double is used by MTestFileGeneratorBase\n"
          << "vector<double> mg_STRESS(*NTENS);\n"
          << "vector<double> mg_STATEV(*NSTATV);\n"
          << "copy(STRESS,STRESS+*NTENS,mg_STRESS.begin());\n"
          << "copy(STATEV,STATEV+*NSTATV,mg_STATEV.begin());\n";
    }
  }  // end of UMATInterfaceBase::generateMTestFile1

  void UMATInterfaceBase::generateMTestFile2(
      std::ostream& out,
      const BehaviourDescription::BehaviourType type,
      const std::string& name,
      const std::string& suffix,
      const BehaviourDescription& mb) const {
    using namespace std;
    if (this->generateMTestFile) {
      string fname = name;
      if (!suffix.empty()) {
        fname += "_" + suffix;
      }
      out << "try{\n";
      if (type == BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
        out << "mfront::UmatSmallStrainMTestFileGenerator mg(\""
            << makeLowerCase(this->getInterfaceName()) << "\",\""
            << this->getLibraryName(mb) << "\",\""
            << this->getFunctionName(fname) << "\");\n";
      } else if (type == BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
        out << "mfront::UmatFiniteStrainMTestFileGenerator mg(\""
            << makeLowerCase(this->getInterfaceName()) << "\",\""
            << this->getLibraryName(mb) << "\",\""
            << this->getFunctionName(fname) << "\");\n";
      } else {
        string msg("UMATInterfaceBase::generateMTestFile2 : ");
        msg += "only small strain or finite strain behaviours are supported";
        throw(runtime_error(msg));
      }
      this->writeMTestFileGeneratorSetModellingHypothesis(out);
      this->writeMTestFileGeneratorSetRotationMatrix(out, mb);
      out << "const unsigned short TVectorSize = mg.getTVectorSize();\n";
      out << "const unsigned short StensorSize = mg.getStensorSize();\n";
      out << "const unsigned short TensorSize  = mg.getTensorSize();\n";
      out << "mg.setHandleThermalExpansion(false);\n";
      out << "mg.addTime(0.);\n";
      out << "mg.addTime(*DTIME>0 ? *DTIME : 1.e-50);\n";
      if (type == BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
        out << "mg.setStrainTensor(STRAN);\n";
        out << "mg.setStrainTensorIncrement(DSTRAN);\n";
        out << "mg.setStressTensor(&mg_STRESS[0]);\n";
      } else if (type == BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
        out << "mg.setDeformationGradientAtTheBeginningOfTheStimeStep(F0);\n";
        out << "mg.setDeformationGradientAtTheEndOfTheStimeStep(F1);\n";
        out << "mg.setStressTensor(&mg_STRESS[0]);\n";
      } else {
        string msg("UMATInterfaceBase::generateMTestFile2 : ");
        msg += "only small strain or finite strain behaviours are supported";
        throw(runtime_error(msg));
      }
      const auto& gh = this->gatherModellingHypothesesAndTests(mb);
      for (const auto& elem : gh) {
        const auto& d = mb.getBehaviourData(elem.first);
        const auto& persistentVarsHolder = d.getPersistentVariables();
        const auto& externalStateVarsHolder = d.getExternalStateVariables();
        pair<vector<UMATMaterialProperty>, SupportedTypes::TypeSize> mprops =
            this->buildMaterialPropertiesList(mb, elem.first);
        VariableDescriptionContainer::const_iterator p;
        unsigned short i;
        unsigned int offset;
        out << "if(" << elem.second << "){\n";
        offset = 0;
        for (const auto& m : mprops.first) {
          auto flag = this->getTypeFlag(m.type);
          if (flag != SupportedTypes::Scalar) {
            throw(
                runtime_error("UMATInterfaceBase::generateFile2 : "
                              "unsupported external state variable type "
                              "in mtest file generation"));
          }
          if (m.arraySize == 1u) {
            if (offset == 0) {
              out << "mg.addMaterialProperty(\"" << m.name << "\",*(PROPS));\n";
            } else {
              out << "mg.addMaterialProperty(\"" << m.name << "\",*(PROPS+"
                  << offset << "));\n";
            }
            ++offset;
          } else {
            for (unsigned short s = 0; s != m.arraySize; ++s, ++offset) {
              if (offset == 0) {
                out << "mg.addMaterialProperty(\"" << m.name << "[" << s
                    << "]\",*(PROPS));\n";
              } else {
                out << "mg.addMaterialProperty(\"" << m.name << "[" << s
                    << "]\","
                    << "*(PROPS+" << offset << "));\n";
              }
            }
          }
        }
        SupportedTypes::TypeSize ivoffset;
        for (p = persistentVarsHolder.begin(); p != persistentVarsHolder.end();
             ++p) {
          auto flag = this->getTypeFlag(p->type);
          const auto& ivname = d.getExternalName(p->name);
          if (p->arraySize == 1u) {
            if (flag == SupportedTypes::Scalar) {
              out << "mg.addInternalStateVariable(\"" << ivname
                  << "\",SupportedTypes::Scalar,&mg_STATEV[" << ivoffset
                  << "]);\n";
              ivoffset += SupportedTypes::TypeSize(1u, 0u, 0u, 0u);
            } else if (flag == SupportedTypes::Stensor) {
              out << "mg.addInternalStateVariable(\"" << ivname
                  << "\",SupportedTypes::Stensor,&mg_STATEV[" << ivoffset
                  << "]);\n";
              ivoffset += SupportedTypes::TypeSize(0u, 0u, 1u, 0u);
            } else {
              throw(
                  runtime_error("UMATInterfaceBase::generateFile2 : "
                                "unsupported internal state variable type "
                                "in mtest file generation"));
            }
          } else {
            if (p->arraySize >= SupportedTypes::ArraySizeLimit) {
              out << "for(unsigned short i=0;i!=" << p->arraySize << ";++i){\n";
              out << "ostringstream name;\n";
              out << "name << \"" << ivname << "[\" << i << \"]\";\n";
              if (flag == SupportedTypes::Scalar) {
                out << "mg.addInternalStateVariable(name.str(),SupportedTypes::"
                       "Scalar,&mg_STATEV["
                    << ivoffset << "]+i);\n";
              } else if (flag == SupportedTypes::Stensor) {
                out << "mg.addInternalStateVariable(name.str(),SupportedTypes::"
                       "Stensor,&mg_STATEV["
                    << ivoffset << "]+i);\n";
              } else {
                throw(
                    runtime_error("UMATInterfaceBase::generateFile2 : "
                                  "unsupported internal state variable type "
                                  "in mtest file generation"));
              }
              out << "}\n";
              if (flag == SupportedTypes::Scalar) {
                ivoffset += SupportedTypes::TypeSize(p->arraySize, 0u, 0u, 0u);
              } else if (flag == SupportedTypes::Stensor) {
                ivoffset += SupportedTypes::TypeSize(0u, 0u, p->arraySize, 0u);
              } else {
                throw(
                    runtime_error("UMATInterfaceBase::generateFile2 : "
                                  "unsupported internal state variable type "
                                  "in mtest file generation"));
              }
            } else {
              for (i = 0; i != p->arraySize; ++i) {
                if (flag == SupportedTypes::Scalar) {
                  out << "mg.addInternalStateVariable(\"" << ivname << "[" << i
                      << "]\",SupportedTypes::Scalar,&mg_STATEV[" << ivoffset
                      << "]);\n";
                  ivoffset += SupportedTypes::TypeSize(1u, 0u, 0u, 0u);
                } else if (flag == SupportedTypes::Stensor) {
                  out << "mg.addInternalStateVariable(\"" << ivname << "[" << i
                      << "]\",SupportedTypes::Stensor,&mg_STATEV[" << ivoffset
                      << "]);\n";
                  ivoffset += SupportedTypes::TypeSize(0u, 0u, 1u, 0u);
                } else {
                  throw(
                      runtime_error("UMATInterfaceBase::generateFile2 : "
                                    "unsupported internal state variable type "
                                    "in mtest file generation"));
                }
              }
            }
          }
        }
        out << "mg.addExternalStateVariableValue(\"Temperature\",0.,*TEMP);\n";
        out << "mg.addExternalStateVariableValue(\"Temperature\",*DTIME,*TEMP+*"
               "DTEMP);\n";
        for (p = externalStateVarsHolder.begin(), offset = 0;
             p != externalStateVarsHolder.end(); ++p) {
          auto flag = this->getTypeFlag(p->type);
          if (flag != SupportedTypes::Scalar) {
            string msg(
                "UMATInterfaceBase::generateFile2 : "
                "unsupported external state variable type "
                "in mtest file generation");
            throw(runtime_error(msg));
          }
          const auto& evname = d.getExternalName(p->name);
          if (p->arraySize == 1u) {
            if (offset == 0) {
              out << "mg.addExternalStateVariableValue(\"" << evname
                  << "\",0,*PREDEF);\n";
              out << "mg.addExternalStateVariableValue(\"" << evname
                  << "\",*DTIME,*PREDEF+*DPRED);\n";
            } else {
              out << "mg.addExternalStateVariableValue(\"" << evname
                  << "\",0,*(PREDEF+" << offset << "));\n";
              out << "mg.addExternalStateVariableValue(\"" << evname
                  << "\",*DTIME,*(PREDEF+" << offset << ")+*(DPRED+" << offset
                  << "));\n";
            }
            ++offset;
          } else {
            if (p->arraySize >= SupportedTypes::ArraySizeLimit) {
              out << "for(unsigned short i=0;i!=" << p->arraySize << ";++i){\n";
              out << "ostringstream name;\n";
              out << "name << \"" << evname << "[\" << i << \"]\";\n";
              if (offset == 0) {
                out << "mg.addExternalStateVariableValue(name.str(),0,*(PREDEF+"
                       "i));\n";
                out << "mg.addExternalStateVariableValue(name.str(),"
                       "*DTIME,*(PREDEF+i)+*(DPRED+i));\n";
              } else {
                out << "mg.addExternalStateVariableValue(name.str(),"
                       "0,*(PREDEF+"
                    << offset << "+i));\n";
                out << "mg.addExternalStateVariableValue(name.str(),"
                       "*DTIME,*(PREDEF+"
                    << offset << "+i)+*(DPRED+" << offset << "+i));\n";
              }
              out << "}\n";
              offset += p->arraySize;
            } else {
              for (i = 0; i != p->arraySize; ++i, ++offset) {
                if (offset == 0) {
                  out << "mg.addExternalStateVariableValue(\"" << evname << "["
                      << i << "]\",0,*PREDEF);\n";
                  out << "mg.addExternalStateVariableValue(\"" << evname << "["
                      << i << "]\",*DTIME,*PREDEF+*DPRED);\n";
                } else {
                  out << "mg.addExternalStateVariableValue(\"" << evname << "["
                      << i
                      << "]\","
                         "0,*(PREDEF+"
                      << offset << "));\n";
                  out << "mg.addExternalStateVariableValue(\"" << evname << "["
                      << i
                      << "]\","
                         "*DTIME,*(PREDEF+"
                      << offset << ")+*(DPRED+" << offset << "));\n";
                }
              }
            }
          }
        }
        out << "}\n";
      }
      out << "mg.generate(\"" + name + "\");\n";
      out << "static_cast<void>(TVectorSize); // remove gcc warning\n";
      out << "static_cast<void>(StensorSize); // remove gcc warning\n";
      out << "static_cast<void>(TensorSize);  // remove gcc warning\n"
          << "} catch(std::exception& mtest_generation_exception){\n"
          << "std::cerr << \"MTest file generation failed: \" << "
          << "mtest_generation_exception.what() << \"\\n\";\n"
          << "} catch(...){\n"
          << "std::cerr << \"MTest file generation failed\\n\";"
          << "}\n";
    }
  }

  void UMATInterfaceBase::writeMTestFileGeneratorSetRotationMatrix(
      std::ostream& out, const BehaviourDescription& mb) const {
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "mg.setRotationMatrix("
          << "DROT[0],DROT[3],DROT[6],"
          << "DROT[1],DROT[4],DROT[7],"
          << "DROT[2],DROT[5],DROT[8]);\n";
    }
  }  // end of UMATInterfaceBase::writeMTestFileGeneratorSetRotationMatrix

  void UMATInterfaceBase::generateUMATxxGeneralSymbols(
      std::ostream& out,
      const std::string& name,
      const BehaviourDescription& mb,
      const FileDescription& fd) const {
    this->writeUMATxxSourceFileSymbols(out, name, mb, fd);
    this->writeUMATxxInterfaceNameSymbols(out, name, mb, fd);
    this->writeUMATxxSupportedModellingHypothesis(out, name, mb);
    this->writeUMATxxBehaviourTypeSymbols(out, name, mb);
    this->writeUMATxxBehaviourKinematicSymbols(out, name, mb);
    this->writeUMATxxSymmetryTypeSymbols(out, name, mb);
    this->writeUMATxxElasticSymmetryTypeSymbols(out, name, mb);
    this->writeUMATxxSpecificSymbols(out, name, mb, fd);
  }

  void UMATInterfaceBase::generateUMATxxSymbols(
      std::ostream& out,
      const std::string& name,
      const Hypothesis h,
      const BehaviourDescription& mb,
      const FileDescription& fd) const {
    this->writeUMATxxIsUsableInPurelyImplicitResolutionSymbols(out, name, h,
                                                               mb);
    this->writeUMATxxMaterialPropertiesSymbols(out, name, h, mb);
    this->writeUMATxxStateVariablesSymbols(out, name, h, mb);
    this->writeUMATxxExternalStateVariablesSymbols(out, name, h, mb);
    this->writeUMATxxRequirementsSymbols(out, name, h, mb);
    this->writeUMATxxAdditionalSymbols(out, name, h, mb, fd);
  }

  void UMATInterfaceBase::writeUMATxxSpecificSymbols(
      std::ostream&,
      const std::string&,
      const BehaviourDescription&,
      const FileDescription&) const {
  }  // end of UMATInterfaceBase::writeUMATxxSpecificSymbols

  void UMATInterfaceBase::writeUMATxxBehaviourTypeSymbols(
      std::ostream& out,
      const std::string& name,
      const BehaviourDescription& mb) const {
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
        << "_BehaviourType = ";
    if (mb.getBehaviourType() == BehaviourDescription::GENERALBEHAVIOUR) {
      out << "0u;\n\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      out << "1u;\n\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      out << "2u;\n\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::COHESIVEZONEMODEL) {
      out << "3u;\n\n";
    } else {
      throw(std::runtime_error(
          "UMATInterfaceBase::writeUMATxxBehaviourTypeSymbols: "
          "unsupported behaviour type."));
    }
  }  // end of UMATInterfaceBase::writeUMATxxBehaviourTypeSymbols

  void UMATInterfaceBase::writeUMATxxBehaviourKinematicSymbols(
      std::ostream& out,
      const std::string& name,
      const BehaviourDescription& mb) const {
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
        << "_BehaviourKinematic = ";
    if (mb.getBehaviourType() == BehaviourDescription::GENERALBEHAVIOUR) {
      out << "0u;\n\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      out << "1u;\n\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::COHESIVEZONEMODEL) {
      out << "2u;\n\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      out << "3u;\n\n";
    } else {
      throw(std::runtime_error(
          "UMATInterfaceBase::writeUMATxxBehaviourKinematicSymbols: "
          "unsupported behaviour type."));
    }
  }  // end of UMATInterfaceBase::writeUMATxxBehaviourKinematicSymbols

  void UMATInterfaceBase::writeUMATxxSupportedModellingHypothesis(
      std::ostream& out,
      const std::string& name,
      const BehaviourDescription& mb) const {
    auto ih = this->getModellingHypothesesToBeTreated(mb);
    if (ih.empty()) {
      out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
          << "_nModellingHypotheses = 0u;\n\n";
      out << "MFRONT_SHAREDOBJ const char * const * "
          << this->getFunctionName(name) << "_ModellingHypotheses = 0;\n\n";
    } else {
      out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
          << "_nModellingHypotheses = " << ih.size() << "u;\n\n";
      out << "MFRONT_SHAREDOBJ const char * \n"
          << this->getFunctionName(name) << "_ModellingHypotheses[" << ih.size()
          << "u] = {";
      for (auto ph = ih.begin(); ph != ih.end();) {
        out << "\"" << ModellingHypothesis::toString(*ph) << "\"";
        if (++ph != ih.end()) {
          out << ",\n";
        }
      }
      out << "};\n\n";
    }
  }  // end of UMATInterfaceBase::writeUMATxxSupportedModellingHypothesis

  void UMATInterfaceBase::writeUMATxxMaterialPropertiesSymbols(
      std::ostream& out,
      const std::string& name,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    using namespace std;
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error(
            "UMATInterfaceBase::writeUMATxxMaterialPropertiesSymbols: " + m));
      }
    };
    const auto mprops = this->buildMaterialPropertiesList(mb, h);
    if (mprops.first.empty()) {
      out << "MFRONT_SHAREDOBJ unsigned short " << this->getSymbolName(name, h)
          << "_nMaterialProperties = 0u;\n\n";
      out << "MFRONT_SHAREDOBJ const char * const *"
          << this->getSymbolName(name, h)
          << "_MaterialProperties = nullptr;\n\n";
    } else {
      const auto& last = mprops.first.back();
      SupportedTypes::TypeSize s;
      throw_if((mprops.second.getTensorSize() != 0) ||
                   (mprops.second.getStensorSize() != 0) ||
                   (mprops.second.getTVectorSize() != 0),
               "internal error : the material properties shall all be scalars");
      s = last.offset;
      s += this->getTypeSize(last.type, last.arraySize);
      s -= mprops.second;
      throw_if((s.getTensorSize() != 0) || (s.getStensorSize() != 0) ||
                   (s.getTVectorSize() != 0),
               "internal error : the material properties shall all be scalars");
      throw_if(s.getScalarSize() < 0,
               "internal error : negative number of the material properties");
      vector<UMATMaterialProperty>::size_type ib =
          0; /* index of the first element which
              * is not imposed by the material properties */
      bool found = false;
      for (decltype(mprops.first.size()) i = 0;
           (i != mprops.first.size()) && (!found); ++i) {
        if (mprops.first[i].offset == mprops.second) {
          ib = i;
          found = true;
        }
      }
      if (!found) {
        throw_if(s.getScalarSize() != 0,
                 "internal error : inconsistent offset declaration");
        out << "MFRONT_SHAREDOBJ unsigned short "
            << this->getSymbolName(name, h) << "_nMaterialProperties = 0u;\n\n";
        out << "MFRONT_SHAREDOBJ const char * const *"
            << this->getSymbolName(name, h)
            << "_MaterialProperties = nullptr;\n\n";
      } else {
        out << "MFRONT_SHAREDOBJ unsigned short "
            << this->getSymbolName(name, h)
            << "_nMaterialProperties = " << s.getScalarSize() << "u;\n\n";
        out << "MFRONT_SHAREDOBJ const char *" << this->getSymbolName(name, h)
            << "_MaterialProperties[" << s.getScalarSize() << "u] = {";
        for (auto i = ib; i != mprops.first.size();) {
          const auto& m = mprops.first[i];
          if (m.arraySize == 1u) {
            out << "\"" << m.name << "\"";
          } else {
            for (unsigned short j = 0; j != m.arraySize;) {
              out << "\"" << m.name << "[" << j << "]\"";
              if (++j != m.arraySize) {
                out << ",\n";
              }
            }
          }
          if (++i != mprops.first.size()) {
            out << ",\n";
          }
        }
        out << "};\n\n";
      }
    }
  }  // end of UMATInterface::writeUMATxxMaterialPropertiesSymbol

  void UMATInterfaceBase::writeUMATxxStateVariablesSymbols(
      std::ostream& out,
      const std::string& name,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    using namespace std;
    const auto& d = mb.getBehaviourData(h);
    const auto& persistentVarsHolder = d.getPersistentVariables();
    const unsigned short nStateVariables = static_cast<unsigned short>(
        this->getNumberOfVariables(persistentVarsHolder));
    VariableDescriptionContainer::const_iterator p;
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getSymbolName(name, h)
        << "_nInternalStateVariables = " << nStateVariables << ";\n";
    vector<string> stateVariablesNames;
    mb.getExternalNames(stateVariablesNames, h, persistentVarsHolder);
    this->writeExternalNames(out, name, h, stateVariablesNames,
                             "InternalStateVariables");
    if (!persistentVarsHolder.empty()) {
      out << "MFRONT_SHAREDOBJ int " << this->getSymbolName(name, h)
          << "_InternalStateVariablesTypes [] = {";
      for (p = persistentVarsHolder.begin(); p != persistentVarsHolder.end();) {
        const auto flag = this->getTypeFlag(p->type);
        for (unsigned short is = 0; is != p->arraySize;) {
          switch (flag) {
            case SupportedTypes::Scalar:
              out << 0;
              break;
            case SupportedTypes::Stensor:
              out << 1;
              break;
            case SupportedTypes::TVector:
              out << 2;
              break;
            case SupportedTypes::Tensor:
              out << 3;
              break;
            default:
              string msg(
                  "UMATInterfaceBase::writeUMATxxStateVariablesSymbols : ");
              msg += "internal error, tag unsupported for variable '" +
                     p->name + "'";
              throw(runtime_error(msg));
          }
          if (++is != p->arraySize) {
            out << ",";
          }
        }
        if (++p != persistentVarsHolder.end()) {
          out << ",";
        }
      }
      out << "};\n\n";
    } else {
      out << "MFRONT_SHAREDOBJ const int * " << this->getSymbolName(name, h)
          << "_InternalStateVariablesTypes = nullptr;\n\n";
    }
  }  // end of UMATInterfaceBase::writeUMATxxStateVariablesSymbols

  void UMATInterfaceBase::writeUMATxxExternalStateVariablesSymbols(
      std::ostream& out,
      const std::string& name,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    const auto& d = mb.getBehaviourData(h);
    const auto& externalStateVarsHolder = d.getExternalStateVariables();
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getSymbolName(name, h)
        << "_nExternalStateVariables = "
        << this->getNumberOfVariables(externalStateVarsHolder) << ";\n";
    this->writeExternalNames(out, name, h,
                             mb.getExternalNames(h, externalStateVarsHolder),
                             "ExternalStateVariables");
  }  // end of UMATInterfaceBase::writeUMATxxExternalStateVariablesSymbols

  void UMATInterfaceBase::writeUMATxxRequirementsSymbols(
      std::ostream& out,
      const std::string& name,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getSymbolName(name, h);
    out << "_requiresStiffnessTensor = ";
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      out << "1";
    } else {
      out << "0";
    }
    out << ";\n";
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getSymbolName(name, h);
    out << "_requiresThermalExpansionCoefficientTensor = ";
    if (mb.getAttribute(
            BehaviourDescription::requiresThermalExpansionCoefficientTensor,
            false)) {
      out << "1";
    } else {
      out << "0";
    }
    out << ";\n";

  }  // end of UMATInterfaceBase::writeUMATxxRequirementsSymbols

  void UMATInterfaceBase::writeUMATxxIsUsableInPurelyImplicitResolutionSymbols(
      std::ostream& out,
      const std::string& name,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    const auto& d = mb.getBehaviourData(h);
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getSymbolName(name, h)
        << "_UsableInPurelyImplicitResolution = ";
    if (d.isUsableInPurelyImplicitResolution()) {
      out << "1;\n\n";
    } else {
      out << "0;\n\n";
    }
  }  // end of UMATInterfaceBase::writeUMATxxIsUsableInPurelyImplicitResolution

  void UMATInterfaceBase::writeUMATxxSymmetryTypeSymbols(
      std::ostream& out,
      const std::string& name,
      const BehaviourDescription& mb) const {
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
        << "_SymmetryType = ";
    if (mb.getSymmetryType() == mfront::ISOTROPIC) {
      out << "0u;\n\n";
    } else if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "1u;\n\n";
    } else {
      throw(std::runtime_error(
          "UMATInterfaceBase::writeUMATxxSymmetryTypeSymbols: "
          "unsupported behaviour type.\n"
          "only isotropic or orthotropic behaviours "
          "are supported at this time."));
    }
  }  // end of UMATInterfaceBase::writeUMATxxSymmetryTypeSymbols

  void UMATInterfaceBase::writeUMATxxElasticSymmetryTypeSymbols(
      std::ostream& out,
      const std::string& name,
      const BehaviourDescription& mb) const {
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
        << "_ElasticSymmetryType = ";
    if (mb.getElasticSymmetryType() == mfront::ISOTROPIC) {
      out << "0u;\n\n";
    } else if (mb.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
      out << "1u;\n\n";
    } else {
      throw(std::runtime_error(
          "UMATInterfaceBase::writeUMATxxElasticSymmetryTypeSymbols: "
          "unsupported behaviour type.\n"
          "only isotropic or orthotropic behaviours are "
          "supported at this time."));
    }
  }  // end of UMATInterfaceBase::writeUMATxxElasticSymmetryTypeSymbols

  void UMATInterfaceBase::writeUMATxxSourceFileSymbols(
      std::ostream& out,
      const std::string& name,
      const BehaviourDescription&,
      const mfront::FileDescription& fd) const {
    using namespace tfel::system;
    using namespace tfel::utilities;
    out << "MFRONT_SHAREDOBJ const char *\n"
        << this->getFunctionName(name) << "_src = \""
        << tokenize(fd.fileName, dirSeparator()).back() << "\";\n\n";
  }

  void UMATInterfaceBase::writeUMATxxInterfaceNameSymbols(
      std::ostream& out,
      const std::string& name,
      const BehaviourDescription&,
      const mfront::FileDescription&) const {
    out << "MFRONT_SHAREDOBJ const char *\n"
        << this->getFunctionName(name) << "_interface = \""
        << this->getInterfaceName() << "\";\n\n";
  }

  std::map<UMATInterfaceBase::Hypothesis, std::string>
  UMATInterfaceBase::gatherModellingHypothesesAndTests(
      const BehaviourDescription& mb) const {
    const auto mh = this->getModellingHypothesesToBeTreated(mb);
    auto res = std::map<Hypothesis, std::string>{};
    auto h1 = std::set<Hypothesis>{};
    auto h2 = std::set<Hypothesis>{};
    for (const auto& h : mh) {
      if (!mb.hasSpecialisedMechanicalData(h)) {
        h1.insert(h);
      } else {
        h2.insert(h);
      }
    }
    if (!h1.empty()) {
      if (h1.size() == 1u) {
        res.insert({ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                    this->getModellingHypothesisTest(*(h1.begin()))});
      } else {
        auto p = h1.begin();
        std::string r =
            "(" + this->getModellingHypothesisTest(*(h1.begin())) + ")";
        ++p;
        for (; p != h1.end(); ++p) {
          r += "||(" + this->getModellingHypothesisTest(*p) + ")";
        }
        res.insert({ModellingHypothesis::UNDEFINEDHYPOTHESIS, r});
      }
    }
    for (const auto& h : h2) {
      res.insert({h, this->getModellingHypothesisTest(h)});
    }
    return res;
  }  // end of UMATInterface::gatherModellingHypothesesAndTests

  std::pair<bool, SupportedTypes::TypeSize>
  UMATInterfaceBase::checkIfAxialStrainIsDefinedAndGetItsOffset(
      const BehaviourDescription& mb) const {
    using tfel::glossary::Glossary;
    const auto& d = mb.getBehaviourData(ModellingHypothesis::PLANESTRESS);
    const auto& sv = d.getPersistentVariables();
    SupportedTypes::TypeSize o;
    for (const auto& elem : sv) {
      if (d.getExternalName(elem.name) == Glossary::AxialStrain) {
        return {true, o};
      }
      o += this->getTypeSize(elem.type, elem.arraySize);
    }
    return {false, o};
  }

  UMATInterfaceBase::~UMATInterfaceBase() {}

}  // end of namespace mfront
