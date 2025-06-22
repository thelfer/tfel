/*!
 * \file  mfront/src/UMATInterfaceBase.cxx
 * \brief
 * \author Thomas Helfer
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

#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetTFELVersion.h"
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
    tfel::raise(
        "UMATInterfaceBase::findUMATMaterialProperty: "
        "no material property associated with the "
        "glossary name '" +
        n + "'");
  }  // end of UMATInterfaceBase::findUMATMaterialProperty

  UMATInterfaceBase::UMATInterfaceBase() = default;

  bool UMATInterfaceBase::isBehaviourConstructorRequired(
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

  void UMATInterfaceBase::appendToMaterialPropertiesList(
      std::vector<UMATMaterialProperty>& l,
      const std::string& t,
      const std::string& n,
      const std::string& v,
      const bool b) const {
    const auto flag = SupportedTypes::getTypeFlag(t);
    tfel::raise_if(flag != SupportedTypes::Scalar,
                   "UMATMaterialProperty::UMATMaterialProperty: "
                   "material properties shall be scalars");
    auto o = SupportedTypes::TypeSize{};
    if (!l.empty()) {
      const auto& m = l.back();
      o = m.offset;
      o += SupportedTypes::getTypeSize(t, 1u);
    }
    l.push_back(UMATMaterialProperty(t, n, v, 1u, o, b));
  }  // end of UMATInterfaceBase::appendToMaterialPropertiesList

  void UMATInterfaceBase::completeMaterialPropertiesList(
      std::vector<UMATMaterialProperty>& mprops,
      const BehaviourDescription& mb,
      const Hypothesis h) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "UMATInterfaceBase::completeMaterialPropertiesList: " + m);
    };
    const auto& d = mb.getBehaviourData(h);
    const auto& mp = d.getMaterialProperties();
    for (auto p = mp.begin(); p != mp.end(); ++p) {
      const auto& n = mb.getExternalName(h, p->name);
      bool found = false;
      const auto flag = SupportedTypes::getTypeFlag(p->type);
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
                       "UMATInterfaceBase::completeMaterialPropertiesList: "
                       "incompatible type for variable '" +
                           n + "' ('" + p->type + "' vs '" + pum->type + "')");
            } else {
              // don't use quantity
              throw_if(SupportedTypes::getTypeFlag(p->type) !=
                           SupportedTypes::getTypeFlag(pum->type),
                       "incompatible type for variable '" + n + "' ('" +
                           p->type + "' vs '" + pum->type + "')");
              if (p->type != pum->type) {
                auto& log = getLogStream();
                log << "UMATInterfaceBase::completeMaterialPropertiesList: "
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
          o += SupportedTypes::getTypeSize(m.type, m.arraySize);
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
      of += SupportedTypes::getTypeSize(v.second.type, 1u);
    }
    if (!persistentVarsHolder.empty()) {
      this->exportResults(out, mb, persistentVarsHolder, iprefix + "statev");
    }
    out << "} // end of " << iprefix << "exportStateData\n\n";
  }

  void UMATInterfaceBase::exportThermodynamicForce(
      std::ostream& out,
      const std::string& a,
      const ThermodynamicForce& f,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    const auto flag = SupportedTypes::getTypeFlag(f.type);
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
      tfel::raise(
          "UMATInterfaceBase::exportThermodynamicForce: "
          "unsupported forces type");
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

  void UMATInterfaceBase::writeVariableInitializersInBehaviourDataConstructorI(
      std::ostream& f,
      bool& first,
      const VariableDescriptionContainer& v,
      const std::string& src,
      const std::string& prefix,
      const std::string& suffix) const {
    this->writeVariableInitializersInBehaviourDataConstructorI(
        f, first, v.begin(), v.end(), src, prefix, suffix);
  }

  void UMATInterfaceBase::writeVariableInitializersInBehaviourDataConstructorI(
      std::ostream& f,
      bool& first,
      const VariableDescriptionContainer::const_iterator& b,
      const VariableDescriptionContainer::const_iterator& e,
      const std::string& src,
      const std::string& prefix,
      const std::string& suffix) const {
    SupportedTypes::TypeSize currentOffset;
    for (auto p = b; p != e; ++p) {
      if (p->arraySize == 1u) {
        const auto n = prefix + p->name + suffix;
        if (!first) {
          f << ",\n";
        }
        first = false;
        auto flag = SupportedTypes::getTypeFlag(p->type);
        if (flag == SupportedTypes::Scalar) {
          f << n << "(" + src + "[" << currentOffset << "])";
        } else if ((flag == SupportedTypes::TVector) ||
                   (flag == SupportedTypes::Stensor) ||
                   (flag == SupportedTypes::Tensor)) {
          f << n << "(&" + src + "[" << currentOffset << "])";
        } else {
          tfel::raise(
              "SupportedTypes::"
              "writeVariableInitializersInBehaviourDataConstructorI: "
              "internal error, tag unsupported");
        }
      }
      currentOffset += this->getTypeSize(p->type, p->arraySize);
    }
  }  // end of
     // SupportedTypes::writeVariableInitializersInBehaviourDataConstructorI

  void UMATInterfaceBase::writeVariableInitializersInBehaviourDataConstructorII(
      std::ostream& f,
      const BehaviourDescription& mb,
      const VariableDescriptionContainer& v,
      const std::string& src,
      const std::string& prefix,
      const std::string& suffix) const {
    this->writeVariableInitializersInBehaviourDataConstructorII(
        f, mb, v.begin(), v.end(), src, prefix, suffix);
  }

  void UMATInterfaceBase::writeVariableInitializersInBehaviourDataConstructorII(
      std::ostream& f,
      const BehaviourDescription& mb,
      const VariableDescriptionContainer::const_iterator& b,
      const VariableDescriptionContainer::const_iterator& e,
      const std::string& src,
      const std::string& prefix,
      const std::string& suffix) const {
    SupportedTypes::TypeSize currentOffset;
    for (auto p = b; p != e; ++p) {
      if (p->arraySize == 1u) {
        currentOffset += this->getTypeSize(p->type, p->arraySize);
      } else {
        const auto flag = SupportedTypes::getTypeFlag(p->type);
        const auto n = prefix + p->name + suffix;
        if (mb.useDynamicallyAllocatedVector(p->arraySize)) {
          f << n << ".resize(" << p->arraySize << ");\n";
          f << "for(unsigned short idx=0;idx!=" << p->arraySize << ";++idx){\n";
          switch (flag) {
            case SupportedTypes::Scalar:
              f << n << "[idx] = " + src + "[" << currentOffset << "+idx];\n";
              break;
            case SupportedTypes::TVector:
              f << "tfel::fsalgo::copy<TVectorSize>::exe(&" + src + "["
                << currentOffset << "+idx*TVectorSize]," << n
                << "[idx].begin());\n";
              break;
            case SupportedTypes::Stensor:
              f << n << "[idx].import(&" + src + "[" << currentOffset
                << "+idx*StensorSize]);\n";
              break;
            case SupportedTypes::Tensor:
              f << "tfel::fsalgo::copy<TensorSize>::exe(&" + src + "["
                << currentOffset << "+idx*TensorSize]," << n
                << "[idx].begin());\n";
              break;
            default:
              tfel::raise(
                  "SupportedTypes::"
                  "writeVariableInitializersInBehaviourDataConstructorII: "
                  "internal error, tag unsupported");
          }
          f << "}\n";
          currentOffset += this->getTypeSize(p->type, p->arraySize);
        } else {
          for (int i = 0; i != p->arraySize; ++i) {
            switch (flag) {
              case SupportedTypes::Scalar:
                f << n << "[" << i << "] = " + src + "[" << currentOffset
                  << "];\n";
                break;
              case SupportedTypes::TVector:
                f << "tfel::fsalgo::copy<TVectorSize>::exe(&" + src + "["
                  << currentOffset << "]," << n << "[" << i << "].begin());\n";
                break;
              case SupportedTypes::Stensor:
                f << n << "[" << i << "].import(&" + src + "[" << currentOffset
                  << "]);\n";
                break;
              case SupportedTypes::Tensor:
                f << "tfel::fsalgo::copy<TensorSize>::exe(&" + src + "["
                  << currentOffset << "]," << n << "[" << i << "].begin());\n";
                break;
              default:
                tfel::raise(
                    "SupportedTypes::"
                    "writeVariableInitializersInBehaviourDataConstructorII: "
                    "internal error, tag unsupported");
            }
            currentOffset += this->getTypeSize(p->type, 1u);
          }
        }
      }
    }
  }  // end of
     // SupportedTypes::writeVariableInitializersInBehaviourDataConstructorII

  void UMATInterfaceBase::writeResultsArrayResize(
      std::ostream& f,
      const std::string& dest,
      const VariableDescriptionContainer& v) const {
    this->writeResultsArrayResize(f, dest, v.getTypeSize());
  }  // end of UMATInterfaceBase::writeResultsArrayResize

  void UMATInterfaceBase::writeResultsArrayResize(
      std::ostream& f,
      const std::string& dest,
      const SupportedTypes::TypeSize& s) const {
    f << "if(" << dest << "!=" << s << "){\n";
    f << dest + ".resize(" << s << ");\n";
    f << "}\n";
  }

  void UMATInterfaceBase::exportResults(std::ostream& f,
                                        const BehaviourDescription& mb,
                                        const VariableDescriptionContainer& v,
                                        const std::string& dest) const {
    SupportedTypes::TypeSize currentOffset;
    for (auto p = v.begin(); p != v.end(); ++p) {
      auto flag = SupportedTypes::getTypeFlag(p->type);
      if (p->arraySize == 1u) {
        if (flag == SupportedTypes::Scalar) {
          if (mb.useQt()) {
            f << dest << "[" << currentOffset << "] = base_cast(this->"
              << p->name << ");\n";
          } else {
            f << dest << "[" << currentOffset << "] = this->" << p->name
              << ";\n";
          }
        } else if ((flag == SupportedTypes::TVector) ||
                   (flag == SupportedTypes::Stensor) ||
                   (flag == SupportedTypes::Tensor)) {
          f << "exportToBaseTypeArray(this->" << p->name << ",&" << dest << "["
            << currentOffset << "]);\n";
        } else {
          tfel::raise(
              "UMATInterfaceBase::exportResults: "
              "internal error, tag unsupported");
        }
        currentOffset += this->getTypeSize(p->type, p->arraySize);
      } else {
        if (mb.useDynamicallyAllocatedVector(p->arraySize)) {
          f << "for(unsigned short idx=0;idx!=" << p->arraySize << ";++idx){\n";
          if (flag == SupportedTypes::Scalar) {
            if (mb.useQt()) {
              f << dest << "[" << currentOffset << "+idx] = common_cast(this->"
                << p->name << "[idx]);\n";
            } else {
              f << dest << "[" << currentOffset << "+idx] = this->" << p->name
                << "[idx];\n";
            }
          } else if ((flag == SupportedTypes::TVector) ||
                     (flag == SupportedTypes::Stensor) ||
                     (flag == SupportedTypes::Tensor)) {
            f << "exportToBaseTypeArray(this->" << p->name << "[idx],&" << dest
              << "[" << currentOffset << "+idx*StensorSize]);\n";
          } else {
            tfel::raise(
                "UMATInterfaceBase::exportResults: "
                "internal error, tag unsupported");
          }
          f << "}\n";
          currentOffset += this->getTypeSize(p->type, p->arraySize);
        } else {
          for (unsigned short i = 0; i != p->arraySize; ++i) {
            if (flag == SupportedTypes::Scalar) {
              if (mb.useQt()) {
                f << dest << "[" << currentOffset << "] = common_cast(this->"
                  << p->name << "[" << i << "]);\n";
              } else {
                f << dest << "[" << currentOffset << "] = this->" << p->name
                  << "[" << i << "];\n";
              }
            } else if ((flag == SupportedTypes::TVector) ||
                       (flag == SupportedTypes::Stensor) ||
                       (flag == SupportedTypes::Tensor)) {
              f << "exportToBaseTypeArray(this->" << p->name << "[" << i
                << "],&" << dest << "[" << currentOffset << "]);\n";
            } else {
              tfel::raise(
                  "UMATInterfaceBase::exportResults: "
                  "internal error, tag unsupported");
            }
            currentOffset += this->getTypeSize(p->type, 1u);
          }
        }
      }
    }
  }

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
        << " * \\param[in] " << iprefix << "T_: temperature\n";
    if (this->isTemperatureIncrementSupported()) {
      out << " * \\param[in] " << iprefix << "dT_: temperature increment\n";
    }
    out << " * \\param[in] " << iprefix << "mat: material properties\n"
        << " * \\param[in] " << iprefix << "int_vars: state variables\n";
    if (this->areExternalStateVariablesSupported()) {
      out << " * \\param[in] " << iprefix
          << "ext_vars: external state variables\n"
          << " * \\param[in] " << iprefix
          << "dext_vars: external state variables increments\n";
    }
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
        << mb.getClassName() << "(const Type* const " << iprefix << "dt_"
        << ",\nconst Type* const " << iprefix << "T_";
    if (this->isTemperatureIncrementSupported()) {
      out << ",\nconst Type* const " << iprefix << "dT_";
    }
    out << ",\nconst Type* const " << iprefix << "mat"
        << ",\nconst Type* const " << iprefix << "int_vars";
    if (this->areExternalStateVariablesSupported()) {
      out << ",\nconst Type* const " << iprefix << "ext_vars"
          << ",\nconst Type* const " << iprefix << "dext_vars";
    }
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
          << iprefix + "int_vars";
      if (this->areExternalStateVariablesSupported()) {
        out << "," << iprefix << "ext_vars";
      }
      for (const auto& v : abdv) {
        out << "," << iprefix << v.first;
      }
      for (const auto& v : av) {
        out << "," << iprefix << v.first;
      }
      out << "),\n"
          << mb.getClassName() << "IntegrationData<hypothesis,Type,use_qt>("
          << iprefix << "dt_";
      if (this->isTemperatureIncrementSupported()) {
        out << "," << iprefix << "dT_";
      }
      if (this->areExternalStateVariablesSupported()) {
        out << "," << iprefix << "dext_vars";
      }
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
          << iprefix + "int_vars";
      if (this->areExternalStateVariablesSupported()) {
        out << "," << iprefix << "ext_vars";
      }
      for (const auto& v : abdv) {
        out << "," << iprefix << v.first;
      }
      for (const auto& v : av) {
        out << "," << iprefix << v.first;
      }
      out << "),\n"
          << mb.getClassName() << "IntegrationData<hypothesis,Type,false>("
          << iprefix << "dt_";
      if (this->isTemperatureIncrementSupported()) {
        out << "," << iprefix << "dT_";
      }
      if (this->areExternalStateVariablesSupported()) {
        out << "," << iprefix << "dext_vars";
      }
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
          bool& first,
          const Hypothesis h,
          const BehaviourDescription& mb,
          const std::vector<UMATMaterialProperty>& i,
          const SupportedTypes::TypeSize ioffset,
          const std::string& src,
          const std::string& prefix,
          const std::string& suffix) const {
    const auto& d = mb.getBehaviourData(h);
    for (const auto& mp : d.getMaterialProperties()) {
      if (mp.arraySize != 1u) {
        continue;
      }
      const auto n = prefix + mp.name + suffix;
      const auto& m =
          UMATInterfaceBase::findUMATMaterialProperty(i, mp.getExternalName());
      auto offset = m.offset;
      offset -= ioffset;
      if (!first) {
        f << ",\n";
      }
      first = false;
      const auto flag = SupportedTypes::getTypeFlag(mp.type);
      if (flag == SupportedTypes::Scalar) {
        f << n << "(" + src + "[" << offset << "])";
      } else if ((flag == SupportedTypes::TVector) ||
                 (flag == SupportedTypes::Stensor) ||
                 (flag == SupportedTypes::Tensor)) {
        f << n << "(&" + src + "[" << offset << "])";
      } else {
        tfel::raise(
            "SupportedTypes::"
            "writeMaterialPropertiesInitializersInBehaviourDataConstructorI: "
            "internal error, tag unsupported");
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
    for (auto p = v.begin(); p != v.end(); ++p) {
      if (p->arraySize == 1u) {
        continue;
      }
      const auto& m = UMATInterfaceBase::findUMATMaterialProperty(
          i, mb.getExternalName(h, p->name));
      const auto flag = SupportedTypes::getTypeFlag(p->type);
      SupportedTypes::TypeSize offset = m.offset;
      offset -= ioffset;
      const auto n = prefix + p->name + suffix;
      if (mb.useDynamicallyAllocatedVector(p->arraySize)) {
        f << n << ".resize(" << p->arraySize << ");\n";
        f << "for(unsigned short idx=0;idx!=" << p->arraySize << ";++idx){\n";
        switch (flag) {
          case SupportedTypes::Scalar:
            f << n << "[idx] = " + src + "[" << offset << "+idx];\n";
            break;
          case SupportedTypes::TVector:
            f << "tfel::fsalgo::copy<TVectorSize>::exe(&" + src + "[" << offset
              << "+idx*TVectorSize]," << n << "[idx].begin());\n";
            break;
          case SupportedTypes::Stensor:
            f << n << "[idx].import(&" + src + "[" << offset
              << "+idx*StensorSize]);\n";
            break;
          case SupportedTypes::Tensor:
            f << "tfel::fsalgo::copy<TensorSize>::exe(&" + src + "[" << offset
              << "+idx*TensorSize]," << n << "[idx].begin());\n";
            break;
          default:
            tfel::raise(
                "UMATInterfaceBase::"
                "writeVariableInitializersInBehaviourDataConstructorII: "
                "internal error, tag unsupported");
        }
        f << "}\n";
      } else {
        for (int index = 0; index != p->arraySize; ++index) {
          switch (flag) {
            case SupportedTypes::Scalar:
              f << n << "[" << index << "] = " + src + "[" << offset << "];\n";
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
              f << "tfel::fsalgo::copy<TensorSize>::exe(&" + src + "[" << offset
                << "]," << n << "[" << index << "].begin());\n";
              break;
            default:
              tfel::raise(
                  "UMATInterfaceBase::"
                  "writeVariableInitializersInBehaviourDataConstructorII: "
                  "internal error, tag unsupported");
          }
          offset += SupportedTypes::getTypeSize(p->type, 1u);
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
        << " * \\param[in] " << iprefix << "int_vars: state variables\n";
    if (this->areExternalStateVariablesSupported()) {
      out << " * \\param[in] " << iprefix
          << "ext_vars: external std::ate variables\n";
    }
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
      out << " " << iprefix << "int_vars\n";
    } else {
      out << "\n";
    }
    if (this->areExternalStateVariablesSupported()) {
      out << ",const Type* const";
      if (externalStateVarsHolder.size() != 1) {
        out << " " << iprefix << "ext_vars";
      }
    }
    for (const auto& v : abdv) {
      out << ",const Type* const " << iprefix << v.first;
    }
    for (const auto& v : av) {
      out << ",const Type* const " << iprefix << v.first;
    }
    out << ")\n: ";
    bool first = true;
    this->writeMaterialPropertiesInitializersInBehaviourDataConstructorI(
        out, first, h, mb, mprops.first, mprops.second, iprefix + "mat", "",
        "");
    this->writeVariableInitializersInBehaviourDataConstructorI(
        out, first, persistentVarsHolder, iprefix + "int_vars", "", "");
    if (!first) {
      out << ",\n";
    }
    out << "T(*" << iprefix << "T_)";
    first = false;
    this->writeVariableInitializersInBehaviourDataConstructorI(
        out, first, std::next(externalStateVarsHolder.begin()),
        externalStateVarsHolder.end(), iprefix + "ext_vars", "", "");
    out << "\n{\n";
    this->writeMaterialPropertiesInitializersInBehaviourDataConstructorII(
        out, h, mb, mprops.first, mprops.second, iprefix + "mat", "", "");
    this->writeVariableInitializersInBehaviourDataConstructorII(
        out, mb, persistentVarsHolder, iprefix + "int_vars", "", "");
    this->writeVariableInitializersInBehaviourDataConstructorII(
        out, mb, std::next(externalStateVarsHolder.begin()),
        externalStateVarsHolder.end(), iprefix + "ext_vars", "", "");
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
        << " * \\param[in] " + iprefix + "dt_: time increment\n";
    if (this->isTemperatureIncrementSupported()) {
      out << " * \\param[in] " + iprefix + "dT_: temperature increment\n";
    }
    if (this->areExternalStateVariablesSupported()) {
      out << " * \\param[in] " + iprefix +
                 "dext_vars: external state variables increments\n";
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
        << mb.getClassName() << "IntegrationData"
        << "(const Type* const " << iprefix << "dt_";
    if (this->isTemperatureIncrementSupported()) {
      out << ",\nconst Type* const " << iprefix << "dT_";
    }
    if (this->areExternalStateVariablesSupported()) {
      if (externalStateVarsHolder.size() != 1) {
        out << ",const Type* const " << iprefix << "dext_vars";
      } else {
        out << ",const Type* const";
      }
    }
    for (const auto& v : aidv) {
      out << ",const Type* const " << iprefix << v.first;
    }
    for (const auto& v : av) {
      out << ",const Type* const " << iprefix << v.first;
    }
    out << ")\n"
        << ": dt(*" << iprefix << "dt_)";
    if (this->isTemperatureIncrementSupported()) {
      out << ",dT(*" << iprefix << "dT_)";
    } else {
      out << ",dT(temperature(0))";
    }
    bool first = false;
    this->writeVariableInitializersInBehaviourDataConstructorI(
        out, first, std::next(externalStateVarsHolder.begin()),
        externalStateVarsHolder.end(), iprefix + "dext_vars", "d", "");
    out << "\n{\n";
    this->writeVariableInitializersInBehaviourDataConstructorII(
        out, mb, std::next(externalStateVarsHolder.begin()),
        externalStateVarsHolder.end(), iprefix + "dext_vars", "d", "");
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
      ov += SupportedTypes::getTypeSize(v.first.type, 1u);
    }
    os << "}\n\n";
    os << "void set" << iprefix
       << "BehaviourDataThermodynamicForces(const Type* const " << iprefix
       << "stress_)\n"
       << "{\n";
    for (const auto& v : mb.getMainVariables()) {
      this->writeBehaviourDataThermodynamicForceSetter(os, v.second, of);
      of += SupportedTypes::getTypeSize(v.second.type, 1u);
    }
    os << "}\n\n";
  }  // end of UMATInterfaceBase::writeBehaviourDataMainVariablesSetters

  void UMATInterfaceBase::writeBehaviourDataDrivingVariableSetter(
      std::ostream& os,
      const DrivingVariable& v,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (!v.increment_known) {
      if (SupportedTypes::getTypeFlag(v.type) == SupportedTypes::TVector) {
        if (!o.isNull()) {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "stran+" << o
             << ",this->" << v.name << "0.begin());\n";
        } else {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "stran,this->"
             << v.name << "0.begin());\n";
        }
      } else if (SupportedTypes::getTypeFlag(v.type) ==
                 SupportedTypes::Stensor) {
        if (!o.isNull()) {
          os << "this->" << v.name << "0.importVoigt(" << iprefix << "stran+"
             << o << ");\n";
        } else {
          os << "this->" << v.name << "0.importVoigt(" << iprefix
             << "stran);\n";
        }
      } else if (SupportedTypes::getTypeFlag(v.type) ==
                 SupportedTypes::Tensor) {
        if (!o.isNull()) {
          os << v.type << "::buildFromFortranMatrix(this->" << v.name << "0,"
             << iprefix << "stran+" << o << ");\n";
        } else {
          os << v.type << "::buildFromFortranMatrix(this->" << v.name << "0,"
             << iprefix << "stran);\n";
        }
      } else {
        tfel::raise(
            "UMATInterfaceBase::writeBehaviourDataMainVariablesSetters: "
            "unsupported driving variable type");
      }
    } else {
      if (SupportedTypes::getTypeFlag(v.type) == SupportedTypes::TVector) {
        if (!o.isNull()) {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "stran+" << o
             << ",this->" << v.name << ".begin());\n";
        } else {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "stran,this->"
             << v.name << ".begin());\n";
        }
      } else if (SupportedTypes::getTypeFlag(v.type) ==
                 SupportedTypes::Stensor) {
        if (!o.isNull()) {
          os << "this->" << v.name << ".importVoigt(" << iprefix << "stran+"
             << o << ");\n";
        } else {
          os << "this->" << v.name << ".importVoigt(" << iprefix << "stran);\n";
        }
      } else if (SupportedTypes::getTypeFlag(v.type) ==
                 SupportedTypes::Tensor) {
        if (!o.isNull()) {
          os << v.type << "::buildFromFortranMatrix(this->" << v.name << ","
             << iprefix << "stran+" << o << ");\n";
        } else {
          os << v.type << "::buildFromFortranMatrix(this->" << v.name << ","
             << iprefix + "stran);\n";
        }
      } else {
        tfel::raise(
            "UMATInterfaceBase::writeBehaviourDataMainVariablesSetters: "
            "unsupported driving variable type");
      }
    }
  }  // end of UMATInterfaceBase::writeBehaviourDataDrivingVariableSetter

  void UMATInterfaceBase::writeBehaviourDataThermodynamicForceSetter(
      std::ostream& os,
      const ThermodynamicForce& f,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (SupportedTypes::getTypeFlag(f.type) == SupportedTypes::TVector) {
      if (!o.isNull()) {
        os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "stress_+" << o
           << ",this->" << f.name << ".begin());\n";
      } else {
        os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "stress_,this->"
           << f.name << ".begin());\n";
      }
    } else if (SupportedTypes::getTypeFlag(f.type) == SupportedTypes::Stensor) {
      if (!o.isNull()) {
        os << "this->" << f.name << ".importTab(" << iprefix << "stress_+" << o
           << ");\n";
      } else {
        os << "this->" << f.name << ".importTab(" << iprefix << "stress_);\n";
      }
    } else if (SupportedTypes::getTypeFlag(f.type) == SupportedTypes::Tensor) {
      if (!o.isNull()) {
        os << f.type << "::buildFromFortranMatrix(this->" << f.name << ","
           << iprefix << "stress_+" << o << ");\n";
      } else {
        os << f.type << "::buildFromFortranMatrix(this->" << f.name << ","
           << iprefix << "stress_);\n";
      }
    } else {
      tfel::raise(
          "UMATInterfaceBase::writeBehaviourDataMainVariablesSetters: "
          "unsupported forces type");
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
      ov += SupportedTypes::getTypeSize(v.first.type, 1u);
    }
    os << "}\n\n";
  }  // end of writeIntegrationDataMainVariablesSetters

  void UMATInterfaceBase::writeIntegrationDataDrivingVariableSetter(
      std::ostream& os,
      const DrivingVariable& v,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (!v.increment_known) {
      if (SupportedTypes::getTypeFlag(v.type) == SupportedTypes::TVector) {
        if (!o.isNull()) {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "dstran+" << o
             << ",this->" << v.name << "1.begin());\n";
        } else {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "dstran,this->"
             << v.name << "1.begin());\n";
        }
      } else if (SupportedTypes::getTypeFlag(v.type) ==
                 SupportedTypes::Stensor) {
        if (!o.isNull()) {
          os << "this->" << v.name << "1.importVoigt(" << iprefix << "dstran+"
             << o << ");\n";
        } else {
          os << "this->" << v.name << "1.importVoigt(" << iprefix
             << "dstran);\n";
        }
      } else if (SupportedTypes::getTypeFlag(v.type) ==
                 SupportedTypes::Tensor) {
        if (!o.isNull()) {
          os << v.type << "::buildFromFortranMatrix(this->" << v.name << "1,"
             << iprefix << "dstran+" << o << ");\n";
        } else {
          os << v.type << "::buildFromFortranMatrix(this->" << v.name << "1,"
             << iprefix << "dstran);\n";
        }
      } else {
        tfel::raise(
            "UMATInterfaceBase::writeIntegrationDataMainVariablesSetters: "
            "unsupported driving variable type");
      }
    } else {
      if (SupportedTypes::getTypeFlag(v.type) == SupportedTypes::TVector) {
        if (!o.isNull()) {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "dstran+" << o
             << ",this->d" << v.name << ".begin());\n";
        } else {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "dstran,this->d"
             << v.name << ".begin());\n";
        }
      } else if (SupportedTypes::getTypeFlag(v.type) ==
                 SupportedTypes::Stensor) {
        if (!o.isNull()) {
          os << "this->d" << v.name << ".importVoigt(" << iprefix << "dstran+"
             << o << ");\n";
        } else {
          os << "this->d" << v.name << ".importVoigt(" << iprefix
             << "dstran);\n";
        }
      } else if (SupportedTypes::getTypeFlag(v.type) ==
                 SupportedTypes::Tensor) {
        if (!o.isNull()) {
          os << v.type << "::buildFromFortranMatrix(this->d" << v.name << ","
             << iprefix << "dstran+" << o << ");\n";
        } else {
          os << v.type << "::buildFromFortranMatrix(this->d" << v.name << ","
             << iprefix + "dstran);\n";
        }
      } else {
        tfel::raise(
            "UMATInterfaceBase::writeIntegrationDataMainVariablesSetters: "
            "unsupported driving variable type");
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
      if (v.type == "int") {
        ip = true;
      } else if (v.type == "ushort") {
        up = true;
      } else {
        const auto f = SupportedTypes::getTypeFlag(v.type);
        tfel::raise_if(f != SupportedTypes::Scalar,
                       "UMATInterfaceBase::checkParametersType: "
                       "unsupport parameter type '" +
                           v.type + "'.");
        rp = true;
      }
    }
  }  // end of UMATInterfaceBase::checkParametersType

  void UMATInterfaceBase::writeSetOutOfBoundsPolicyFunctionDeclaration(
      std::ostream& out, const std::string& name) const {
    out << "MFRONT_SHAREDOBJ void\n"
        << this->getFunctionName(name)
        << "_setOutOfBoundsPolicy(const int);\n\n";
  }

  void UMATInterfaceBase::writeGetOutOfBoundsPolicyFunctionImplementation(
      std::ostream& out, const std::string& name) const {
    out << "static tfel::material::OutOfBoundsPolicy&\n"
        << this->getFunctionName(name) << "_getOutOfBoundsPolicy(){\n"
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
        << "_setOutOfBoundsPolicy: invalid argument\\n\";\n"
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
    const auto mh = mb.getDistinctModellingHypotheses();
    const auto mh2 = this->getModellingHypothesesToBeTreated(mb);
    for (const auto& h : mh) {
      if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
          (mh2.find(h) != mh2.end())) {
        const auto& d = mb.getBehaviourData(h);
        const auto& pc = d.getParameters();
        bool rp, ip, up;
        this->checkParametersType(rp, ip, up, pc);
        auto fctName = this->getFunctionName(name);
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
              << "_setIntegerParameter(const char *const key,const int "
                 "value){\n"
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
              << "_setUnsignedShortParameter(const char *const key,const "
                 "unsigned short value){\n"
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
      out << "using tfel::material::ModellingHypothesis;\n"
          << "using mfront::SupportedTypes;\n"
          << "// double is used by MTestFileGeneratorBase\n"
          << "std::vector<double> mg_STRESS(*NTENS);\n"
          << "std::vector<double> mg_STATEV(*NSTATV);\n"
          << "std::copy(STRESS,STRESS+*NTENS,mg_STRESS.begin());\n"
          << "std::copy(STATEV,STATEV+*NSTATV,mg_STATEV.begin());\n";
    }
  }  // end of UMATInterfaceBase::generateMTestFile1

  void UMATInterfaceBase::generateMTestFile2(
      std::ostream& out,
      const BehaviourDescription::BehaviourType type,
      const std::string& name,
      const std::string& suffix,
      const BehaviourDescription& mb) const {
    if (this->generateMTestFile) {
      auto fname = name;
      if (!suffix.empty()) {
        fname += "_" + suffix;
      }
      out << "try{\n";
      if (type == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        out << "mfront::UmatSmallStrainMTestFileGenerator mg(\""
            << makeLowerCase(this->getInterfaceName()) << "\",\""
            << this->getLibraryName(mb) << "\",\""
            << this->getFunctionName(fname) << "\");\n";
      } else if (type == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        out << "mfront::UmatFiniteStrainMTestFileGenerator mg(\""
            << makeLowerCase(this->getInterfaceName()) << "\",\""
            << this->getLibraryName(mb) << "\",\""
            << this->getFunctionName(fname) << "\");\n";
      } else {
        tfel::raise(
            "UMATInterfaceBase::generateMTestFile2: "
            "only small strain or finite strain behaviours are supported");
      }
      this->writeMTestFileGeneratorSetModellingHypothesis(out);
      this->writeMTestFileGeneratorSetRotationMatrix(out, mb);
      out << "const auto TVectorSize = mg.getTVectorSize();\n"
          << "const auto StensorSize = mg.getStensorSize();\n"
          << "const auto TensorSize  = mg.getTensorSize();\n"
          << "const auto dt = *DTIME>0 ? *DTIME : 1.e-50;\n"
          << "mg.setHandleThermalExpansion(false);\n"
          << "mg.addTime(0.);\n"
          << "mg.addTime(dt);\n";
      if (type == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        out << "mg.setStrainTensor(STRAN);\n"
            << "mg.setStrainTensorIncrement(DSTRAN);\n"
            << "mg.setStressTensor(&mg_STRESS[0]);\n";
      } else if (type == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        out << "mg.setDeformationGradientAtTheBeginningOfTheStimeStep(F0);\n"
            << "mg.setDeformationGradientAtTheEndOfTheStimeStep(F1);\n"
            << "mg.setStressTensor(&mg_STRESS[0]);\n";
      } else {
        tfel::raise(
            "UMATInterfaceBase::generateMTestFile2: "
            "only small strain or finite strain behaviours are supported");
      }
      const auto& gh = this->gatherModellingHypothesesAndTests(mb);
      for (const auto& elem : gh) {
        const auto& d = mb.getBehaviourData(elem.first);
        const auto& persistentVarsHolder = d.getPersistentVariables();
        const auto& externalStateVarsHolder = d.getExternalStateVariables();
        const auto mprops = this->buildMaterialPropertiesList(mb, elem.first);
        const auto b = elem.second != "true";
        if (b) {
          out << "if(" << elem.second << "){\n";
        }
        unsigned int offset = 0;
        for (const auto& m : mprops.first) {
          auto flag = SupportedTypes::getTypeFlag(m.type);
          tfel::raise_if(flag != SupportedTypes::Scalar,
                         "UMATInterfaceBase::generateFile2: "
                         "unsupported material property type "
                         "in mtest file generation");
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
        for (const auto& v : persistentVarsHolder) {
          auto flag = SupportedTypes::getTypeFlag(v.type);
          const auto& ivname = d.getExternalName(v.name);
          tfel::raise_if((flag != SupportedTypes::Stensor) &&
                             (flag != SupportedTypes::Scalar),
                         "UMATInterfaceBase::generateFile2: "
                         "unsupported internal state variable type "
                         "in mtest file generation");
          if (v.arraySize == 1u) {
            if (flag == SupportedTypes::Scalar) {
              out << "mg.addInternalStateVariable(\"" << ivname
                  << "\",SupportedTypes::Scalar,&mg_STATEV[" << ivoffset
                  << "]);\n";
              ivoffset += SupportedTypes::TypeSize(1u, 0u, 0u, 0u);
            } else {
              out << "mg.addInternalStateVariable(\"" << ivname
                  << "\",SupportedTypes::Stensor,&mg_STATEV[" << ivoffset
                  << "]);\n";
              ivoffset += SupportedTypes::TypeSize(0u, 0u, 1u, 0u);
            }
          } else {
            if (v.arraySize >= SupportedTypes::ArraySizeLimit) {
              out << "for(unsigned short i=0;i!=" << v.arraySize << ";++i){\n";
              out << "auto name =  \"" << ivname
                  << "[\" + std::to_string(i)+ \"]\";\n";
              if (flag == SupportedTypes::Scalar) {
                out << "mg.addInternalStateVariable(name,SupportedTypes::"
                       "Scalar,&mg_STATEV["
                    << ivoffset << "]+i);\n";
              } else {
                out << "mg.addInternalStateVariable(name,SupportedTypes::"
                       "Stensor,&mg_STATEV["
                    << ivoffset << "]+i);\n";
              }
              out << "}\n";
              if (flag == SupportedTypes::Scalar) {
                ivoffset += SupportedTypes::TypeSize(v.arraySize, 0u, 0u, 0u);
              } else {
                ivoffset += SupportedTypes::TypeSize(0u, 0u, v.arraySize, 0u);
              }
            } else {
              for (unsigned short i = 0; i != v.arraySize; ++i) {
                if (flag == SupportedTypes::Scalar) {
                  out << "mg.addInternalStateVariable(\"" << ivname << "[" << i
                      << "]\",SupportedTypes::Scalar,&mg_STATEV[" << ivoffset
                      << "]);\n";
                  ivoffset += SupportedTypes::TypeSize(1u, 0u, 0u, 0u);
                } else {
                  out << "mg.addInternalStateVariable(\"" << ivname << "[" << i
                      << "]\",SupportedTypes::Stensor,&mg_STATEV[" << ivoffset
                      << "]);\n";
                  ivoffset += SupportedTypes::TypeSize(0u, 0u, 1u, 0u);
                }
              }
            }
          }
        }
        out << "mg.addExternalStateVariableValue(\"Temperature\",0.,*TEMP);\n";
        out << "mg.addExternalStateVariableValue(\"Temperature\",dt,*TEMP+*"
               "DTEMP);\n";
        auto p = std::next(externalStateVarsHolder.begin());
        for (offset = 0; p != externalStateVarsHolder.end(); ++p) {
          auto flag = SupportedTypes::getTypeFlag(p->type);
          tfel::raise_if(flag != SupportedTypes::Scalar,
                         "UMATInterfaceBase::generateFile2: "
                         "unsupported external state variable type "
                         "in mtest file generation");
          const auto& evname = d.getExternalName(p->name);
          if (p->arraySize == 1u) {
            if (offset == 0) {
              out << "mg.addExternalStateVariableValue(\"" << evname
                  << "\",0,*PREDEF);\n";
              out << "mg.addExternalStateVariableValue(\"" << evname
                  << "\",dt,*PREDEF+*DPRED);\n";
            } else {
              out << "mg.addExternalStateVariableValue(\"" << evname
                  << "\",0,*(PREDEF+" << offset << "));\n";
              out << "mg.addExternalStateVariableValue(\"" << evname
                  << "\",dt,*(PREDEF+" << offset << ")+*(DPRED+" << offset
                  << "));\n";
            }
            ++offset;
          } else {
            if (p->arraySize >= SupportedTypes::ArraySizeLimit) {
              out << "for(unsigned short i=0;i!=" << p->arraySize << ";++i){\n";
              out << "auto name = \"" << evname
                  << "[\" +std::to_string(i)+\"]\";\n";
              if (offset == 0) {
                out << "mg.addExternalStateVariableValue(name,0,*(PREDEF+i));"
                       "\n";
                out << "mg.addExternalStateVariableValue(name,"
                       "dt,*(PREDEF+i)+*(DPRED+i));\n";
              } else {
                out << "mg.addExternalStateVariableValue(name,"
                       "0,*(PREDEF+"
                    << offset << "+i));\n";
                out << "mg.addExternalStateVariableValue(name,"
                       "dt,*(PREDEF+"
                    << offset << "+i)+*(DPRED+" << offset << "+i));\n";
              }
              out << "}\n";
              offset += p->arraySize;
            } else {
              for (unsigned short i = 0; i != p->arraySize; ++i, ++offset) {
                if (offset == 0) {
                  out << "mg.addExternalStateVariableValue(\"" << evname << "["
                      << i << "]\",0,*PREDEF);\n";
                  out << "mg.addExternalStateVariableValue(\"" << evname << "["
                      << i << "]\",dt,*PREDEF+*DPRED);\n";
                } else {
                  out << "mg.addExternalStateVariableValue(\"" << evname << "["
                      << i
                      << "]\","
                         "0,*(PREDEF+"
                      << offset << "));\n";
                  out << "mg.addExternalStateVariableValue(\"" << evname << "["
                      << i
                      << "]\","
                         "dt,*(PREDEF+"
                      << offset << ")+*(DPRED+" << offset << "));\n";
                }
              }
            }
          }
        }
        if (b) {
          out << "}\n";
        }
      }
      out << "mg.generate(\"" + name + "\");\n"
          << "static_cast<void>(TVectorSize); // remove gcc warning\n"
          << "static_cast<void>(StensorSize); // remove gcc warning\n"
          << "static_cast<void>(TensorSize);  // remove gcc warning\n"
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
    this->writeUMATxxEntryPointSymbol(out, name);
    this->writeUMATxxTFELVersionSymbol(out, name);
    this->writeUMATxxMaterialSymbol(out, name, mb);
    this->writeUMATxxMaterialKnowledgeTypeSymbol(out, name);
    this->writeUMATxxInterfaceNameSymbols(out, name, mb, fd);
    this->writeUMATxxSourceFileSymbols(out, name, mb, fd);
    this->writeUMATxxSupportedModellingHypothesis(out, name, mb);
    this->writeUMATxxBehaviourTypeSymbols(out, name, mb);
    this->writeUMATxxBehaviourKinematicSymbols(out, name, mb);
    this->writeUMATxxSymmetryTypeSymbols(out, name, mb);
    this->writeUMATxxElasticSymmetryTypeSymbols(out, name, mb);
    this->writeUMATxxSpecificSymbols(out, name, mb, fd);
  }

  void UMATInterfaceBase::writeUMATxxEntryPointSymbol(
      std::ostream& out, const std::string& n) const {
    writeEntryPointSymbol(out, this->getFunctionName(n));
  }  // end of UMATInterfaceBase::writeUMATxxEntryPointSymbol

  void UMATInterfaceBase::writeUMATxxTFELVersionSymbol(
      std::ostream& out, const std::string& n) const {
    writeTFELVersionSymbol(out, this->getFunctionName(n));
  }  // end of UMATInterfaceBase::writeUMATxxTFELVersionSymbol

  void UMATInterfaceBase::writeUMATxxMaterialSymbol(
      std::ostream& out,
      const std::string& n,
      const BehaviourDescription& mb) const {
    writeMaterialSymbol(out, this->getFunctionName(n), mb.getMaterialName());
  }  // end of UMATInterfaceBase::writeUMATxxEntryPointSymbol

  void UMATInterfaceBase::writeUMATxxMaterialKnowledgeTypeSymbol(
      std::ostream& out, const std::string& n) const {
    writeMaterialKnowledgeTypeSymbol(out, this->getFunctionName(n), BEHAVIOUR);
  }  // end of UMATInterfaceBase::writeUMATxxEntryPointSymbol

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
    this->writeUMATxxParametersSymbols(out, name, h, mb);
    this->writeUMATxxParameterDefaultValueSymbols(out, name, h, mb);
    this->writeUMATxxBoundsSymbols(out, name, h, mb);
    this->writeUMATxxPhysicalBoundsSymbols(out, name, h, mb);
    this->writeUMATxxRequirementsSymbols(out, name, h, mb);
    this->writeUMATxxAdditionalSymbols(out, name, h, mb, fd);
  }

  void UMATInterfaceBase::writeUMATxxSpecificSymbols(
      std::ostream&,
      const std::string&,
      const BehaviourDescription&,
      const FileDescription&) const {
  }  // end of UMATInterfaceBase::writeUMATxxSpecificSymbols

  void UMATInterfaceBase::writeUMATxxSupportedModellingHypothesis(
      std::ostream& out,
      const std::string& name,
      const BehaviourDescription& mb) const {
    auto ih = this->getModellingHypothesesToBeTreated(mb);
    if (ih.empty()) {
      out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
          << "_nModellingHypotheses = 0u;\n\n"
          << "MFRONT_SHAREDOBJ const char * const * "
          << this->getFunctionName(name) << "_ModellingHypotheses = 0;\n\n";
    } else {
      out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
          << "_nModellingHypotheses = " << ih.size() << "u;\n\n"
          << "MFRONT_SHAREDOBJ const char * \n"
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
      tfel::raise_if(
          b, "UMATInterfaceBase::writeUMATxxMaterialPropertiesSymbols: " + m);
    };
    const auto mprops = this->buildMaterialPropertiesList(mb, h);
    if (mprops.first.empty()) {
      out << "MFRONT_SHAREDOBJ unsigned short " << this->getSymbolName(name, h)
          << "_nMaterialProperties = 0u;\n\n"
          << "MFRONT_SHAREDOBJ const char * const *"
          << this->getSymbolName(name, h)
          << "_MaterialProperties = nullptr;\n\n";
    } else {
      const auto& last = mprops.first.back();
      SupportedTypes::TypeSize s;
      throw_if((mprops.second.getTensorSize() != 0) ||
                   (mprops.second.getStensorSize() != 0) ||
                   (mprops.second.getTVectorSize() != 0),
               "internal error: the material properties shall all be scalars");
      s = last.offset;
      s += SupportedTypes::getTypeSize(last.type, last.arraySize);
      s -= mprops.second;
      throw_if((s.getTensorSize() != 0) || (s.getStensorSize() != 0) ||
                   (s.getTVectorSize() != 0),
               "internal error: the material properties shall all be scalars");
      throw_if(s.getScalarSize() < 0,
               "internal error: negative number of the material properties");
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
                 "internal error: inconsistent offset declaration");
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
        persistentVarsHolder.getNumberOfVariables());
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
        const auto flag = SupportedTypes::getTypeFlag(p->type);
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
              tfel::raise(
                  "UMATInterfaceBase::writeUMATxxStateVariablesSymbols: "
                  "internal error, tag unsupported for "
                  "variable '" +
                  p->name + "'");
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
    auto evs = d.getExternalStateVariables();
    // removing the temperature
    evs.erase(evs.begin());
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getSymbolName(name, h)
        << "_nExternalStateVariables = " << evs.getNumberOfVariables() << ";\n";
    this->writeExternalNames(out, name, h, mb.getExternalNames(h, evs),
                             "ExternalStateVariables");
  }  // end of UMATInterfaceBase::writeUMATxxExternalStateVariablesSymbols

  void UMATInterfaceBase::writeUMATxxParametersSymbols(
      std::ostream& out,
      const std::string& name,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    const auto& d = mb.getBehaviourData(h);
    const auto& parameters = d.getParameters();
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getSymbolName(name, h)
        << "_nParameters = " << parameters.getNumberOfVariables() << ";\n";
    this->writeExternalNames(out, name, h, mb.getExternalNames(h, parameters),
                             "Parameters");
    if (!parameters.empty()) {
      out << "MFRONT_SHAREDOBJ int " << this->getSymbolName(name, h)
          << "_ParametersTypes [] = {";
      for (auto p = parameters.begin(); p != parameters.end();) {
        for (unsigned short is = 0; is != p->arraySize;) {
          if (p->type == "int") {
            out << "1";
          } else if (p->type == "ushort") {
            out << "2";
          } else {
            const auto f = SupportedTypes::getTypeFlag(p->type);
            tfel::raise_if(f != SupportedTypes::Scalar,
                           "UMATInterfaceBase::writeUMATxxParametersSymbols: "
                           "internal error, unsupported type "
                           "for parameter '" +
                               p->name + "'");
            out << "0";
          }
          if (++is != p->arraySize) {
            out << ",";
          }
        }
        if (++p != parameters.end()) {
          out << ",";
        }
      }
      out << "};\n\n";
    } else {
      out << "MFRONT_SHAREDOBJ const int * " << this->getSymbolName(name, h)
          << "_ParametersTypes = nullptr;\n\n";
    }
  }  // end of UMATInterfaceBase::writeUMATxxParametersSymbols

  void UMATInterfaceBase::writeUMATxxParameterDefaultValueSymbols(
      std::ostream& out,
      const std::string& name,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "UMATInterfaceBase::"
                     "writeUMATxxParameterDefaultValueSymbols: " +
                         m);
    };
    for (const auto& p : mb.getBehaviourData(h).getParameters()) {
      if (p.type == "int") {
        throw_if(p.arraySize != 1u,
                 "unsupported parameters array of type '" + p.type + "'");
        out << "MFRONT_SHAREDOBJ int " << this->getSymbolName(name, h) << "_"
            << p.getExternalName() << "_ParameterDefaultValue  = "
            << mb.getIntegerParameterDefaultValue(h, p.name) << ";\n\n";
      } else if (p.type == "ushort") {
        throw_if(p.arraySize != 1u,
                 "unsupported parameters array of type '" + p.type + "'");
        out << "MFRONT_SHAREDOBJ unsigned short "
            << this->getSymbolName(name, h) << "_" << p.getExternalName()
            << "_ParameterDefaultValue  = "
            << mb.getUnsignedShortParameterDefaultValue(h, p.name) << ";\n\n";
      } else {
        const auto f = SupportedTypes::getTypeFlag(p.type);
        throw_if(f != SupportedTypes::Scalar,
                 "unsupported paramaeter type '" + p.type + "'");
        if (p.arraySize == 1u) {
          out << "MFRONT_SHAREDOBJ double " << this->getSymbolName(name, h)
              << "_" << p.getExternalName() << "_ParameterDefaultValue"
              << " = " << mb.getFloattingPointParameterDefaultValue(h, p.name)
              << ";\n\n";
        } else {
          for (unsigned short is = 0; is != p.arraySize; ++is) {
            out << "MFRONT_SHAREDOBJ double " << this->getSymbolName(name, h)
                << "_" << p.getExternalName() << "__" << is
                << "___ParameterDefaultValue = "
                << mb.getFloattingPointParameterDefaultValue(h, p.name, is)
                << ";\n\n";
          }
        }
      }
    }
  }  // end of UMATInterfaceBase::writeUMATxxParameterDefaultValueSymbols

  static void writewriteUMATxxBoundsSymbol(std::ostream& out,
                                           const std::string& bn,
                                           const std::string& vn,
                                           const std::string& bt,
                                           const VariableBoundsDescription& b) {
    if ((b.boundsType == VariableBoundsDescription::LOWER) ||
        (b.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
      out << "MFRONT_SHAREDOBJ long double " << bn << "_" << vn << "_"
          << "Lower" << bt << "Bound = " << b.lowerBound << ";\n\n";
    }
    if ((b.boundsType == VariableBoundsDescription::UPPER) ||
        (b.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
      out << "MFRONT_SHAREDOBJ long double " << bn << "_" << vn << "_"
          << "Upper" << bt << "Bound = " << b.upperBound << ";\n\n";
    }
  }  // end of writewriteUMATxxBoundsSymbol

  void UMATInterfaceBase::writeUMATxxBoundsSymbols(
      std::ostream& out,
      const std::string& name,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    auto write = [this, &out, &name,
                  h](const VariableDescriptionContainer& vc) {
      for (const auto& v : vc) {
        if (v.arraySize == 1u) {
          if (!v.hasBounds()) {
            continue;
          }
          mfront::writewriteUMATxxBoundsSymbol(
              out, this->getSymbolName(name, h), v.getExternalName(), "",
              v.getBounds());
        } else {
          for (auto i = 0; i != v.arraySize; ++i) {
            if (!v.hasBounds(i)) {
              continue;
            }
            mfront::writewriteUMATxxBoundsSymbol(
                out, this->getSymbolName(name, h),
                v.getExternalName() + "__" + std::to_string(i) + "__", "",
                v.getBounds(i));
          }
        }
      }
    };
    const auto& d = mb.getBehaviourData(h);
    write(d.getMaterialProperties());
    write(d.getPersistentVariables());
    write(d.getExternalStateVariables());
    write(d.getParameters());
  }  // end of UMATInterfaceBase::writeUMATxxBoundsSymbols

  void UMATInterfaceBase::writeUMATxxPhysicalBoundsSymbols(
      std::ostream& out,
      const std::string& name,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    auto write = [this, &out, &name,
                  h](const VariableDescriptionContainer& vc) {
      for (const auto& v : vc) {
        if (v.arraySize == 1u) {
          if (!v.hasPhysicalBounds()) {
            continue;
          }
          mfront::writewriteUMATxxBoundsSymbol(
              out, this->getSymbolName(name, h), v.getExternalName(),
              "Physical", v.getPhysicalBounds());
        } else {
          for (auto i = 0; i != v.arraySize; ++i) {
            if (!v.hasPhysicalBounds(i)) {
              continue;
            }
            mfront::writewriteUMATxxBoundsSymbol(
                out, this->getSymbolName(name, h),
                v.getExternalName() + "__" + std::to_string(i) + "__",
                "Physical", v.getPhysicalBounds(i));
          }
        }
      }
    };
    const auto& d = mb.getBehaviourData(h);
    write(d.getMaterialProperties());
    write(d.getPersistentVariables());
    write(d.getExternalStateVariables());
    write(d.getParameters());
  }  // end of UMATInterfaceBase::writeUMATxxPhysicalBoundsSymbols

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
      tfel::raise(
          "UMATInterfaceBase::writeUMATxxSymmetryTypeSymbols: "
          "unsupported behaviour type.\n"
          "only isotropic or orthotropic behaviours "
          "are supported at this time.");
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
      tfel::raise(
          "UMATInterfaceBase::writeUMATxxElasticSymmetryTypeSymbols: "
          "unsupported behaviour type.\n"
          "only isotropic or orthotropic behaviours are "
          "supported at this time.");
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
    writeInterfaceSymbol(out, this->getFunctionName(name),
                         this->getInterfaceName());
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
      o += SupportedTypes::getTypeSize(elem.type, elem.arraySize);
    }
    return {false, o};
  }

  bool UMATInterfaceBase::areExternalStateVariablesSupported() const {
    return true;
  }  // end of UMATInterfaceBase::areExternalStateVariablesSupported()

  bool UMATInterfaceBase::isTemperatureIncrementSupported() const {
    return true;
  }  // end of UMATInterfaceBase::isTemperatureIncrementSupported()

  UMATInterfaceBase::~UMATInterfaceBase() = default;

}  // end of namespace mfront
