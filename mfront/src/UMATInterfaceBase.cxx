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
#include "TFEL/System/System.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/UMATInterfaceBase.hxx"

namespace mfront {

  UMATInterfaceBase::UMATInterfaceBase() = default;

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
          << "NumericType * const " << iprefix + "stress_, "
          << "NumericType * const " << iprefix + "statev) const\n";
    } else {
      out << "void\n"
          << iprefix + "exportStateData("
          << "NumericType * const " << iprefix << "stress_, "
          << "NumericType * const) const\n";
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
    if (flag == SupportedTypes::SCALAR) {
      if (!o.isNull()) {
        out << "*(" << a << "+" << o << ") = this->" << f.name << ";\n";
      } else {
        out << "*(" << a << ") = this->" << f.name << ";\n";
      }
    } else if (flag == SupportedTypes::STENSOR) {
      if (!o.isNull()) {
        out << "this->sig.exportTab(" << a << "+" << o << ");\n";
      } else {
        out << "this->sig.exportTab(" << a
            << ""
               ");\n";
      }
    } else if ((flag == SupportedTypes::TVECTOR) ||
               (flag == SupportedTypes::TENSOR)) {
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
        if (flag == SupportedTypes::SCALAR) {
          f << n << "(" + src + "[" << currentOffset << "])";
        } else if ((flag == SupportedTypes::TVECTOR) ||
                   (flag == SupportedTypes::STENSOR) ||
                   (flag == SupportedTypes::TENSOR)) {
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
            case SupportedTypes::SCALAR:
              f << n << "[idx] = " + src + "[" << currentOffset << "+idx];\n";
              break;
            case SupportedTypes::TVECTOR:
              f << "tfel::fsalgo::copy<TVectorSize>::exe(&" + src + "["
                << currentOffset << "+idx*TVectorSize]," << n
                << "[idx].begin());\n";
              break;
            case SupportedTypes::STENSOR:
              f << n << "[idx].import(&" + src + "[" << currentOffset
                << "+idx*StensorSize]);\n";
              break;
            case SupportedTypes::TENSOR:
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
              case SupportedTypes::SCALAR:
                f << n << "[" << i << "] = " + src + "[" << currentOffset
                  << "];\n";
                break;
              case SupportedTypes::TVECTOR:
                f << "tfel::fsalgo::copy<TVectorSize>::exe(&" + src + "["
                  << currentOffset << "]," << n << "[" << i << "].begin());\n";
                break;
              case SupportedTypes::STENSOR:
                f << n << "[" << i << "].import(&" + src + "[" << currentOffset
                  << "]);\n";
                break;
              case SupportedTypes::TENSOR:
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
        if (flag == SupportedTypes::SCALAR) {
          if (mb.useQt()) {
            f << dest << "[" << currentOffset << "] = base_type_cast(this->"
              << p->name << ");\n";
          } else {
            f << dest << "[" << currentOffset << "] = this->" << p->name
              << ";\n";
          }
        } else if ((flag == SupportedTypes::TVECTOR) ||
                   (flag == SupportedTypes::STENSOR) ||
                   (flag == SupportedTypes::TENSOR)) {
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
          if (flag == SupportedTypes::SCALAR) {
            if (mb.useQt()) {
              f << dest << "[" << currentOffset << "+idx] = common_cast(this->"
                << p->name << "[idx]);\n";
            } else {
              f << dest << "[" << currentOffset << "+idx] = this->" << p->name
                << "[idx];\n";
            }
          } else if ((flag == SupportedTypes::TVECTOR) ||
                     (flag == SupportedTypes::STENSOR) ||
                     (flag == SupportedTypes::TENSOR)) {
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
            if (flag == SupportedTypes::SCALAR) {
              if (mb.useQt()) {
                f << dest << "[" << currentOffset << "] = common_cast(this->"
                  << p->name << "[" << i << "]);\n";
              } else {
                f << dest << "[" << currentOffset << "] = this->" << p->name
                  << "[" << i << "];\n";
              }
            } else if ((flag == SupportedTypes::TVECTOR) ||
                       (flag == SupportedTypes::STENSOR) ||
                       (flag == SupportedTypes::TENSOR)) {
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

  void UMATInterfaceBase::writeBehaviourConstructorHeader(
      std::ostream& out,
      const BehaviourDescription& mb,
      const Hypothesis,
      const std::string& initStateVarsIncrements) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    const auto av = this->getBehaviourConstructorsAdditionalVariables();
    const auto abdv = this->getBehaviourDataConstructorAdditionalVariables();
    const auto aidv = this->getIntegrationDataConstructorAdditionalVariables();
    out << "/*\n"
        << " * \\brief constructor for the " << this->getInterfaceName()
        << " interface\n"
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
        << mb.getClassName() << "(const NumericType* const " << iprefix << "dt_"
        << ",\nconst NumericType* const " << iprefix << "T_";
    if (this->isTemperatureIncrementSupported()) {
      out << ",\nconst NumericType* const " << iprefix << "dT_";
    }
    out << ",\nconst NumericType* const " << iprefix << "mat"
        << ",\nconst NumericType* const " << iprefix << "int_vars";
    if (this->areExternalStateVariablesSupported()) {
      out << ",\nconst NumericType* const " << iprefix << "ext_vars"
          << ",\nconst NumericType* const " << iprefix << "dext_vars";
    }
    for (const auto& v : abdv) {
      out << ",\nconst NumericType* const " << iprefix << v.first;
    }
    for (const auto& v : aidv) {
      out << ",\nconst NumericType* const " << iprefix << v.first;
    }
    for (const auto& v : av) {
      out << ",\nconst NumericType* const " << iprefix << v.first;
    }
    out << ")\n";
    if (mb.useQt()) {
      out << ": " << mb.getClassName()
          << "BehaviourData<hypothesis, NumericType,use_qt>(" << iprefix
          << "T_," << iprefix << "mat,\n"
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
          << mb.getClassName()
          << "IntegrationData<hypothesis, NumericType,use_qt>(" << iprefix
          << "dt_";
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
          << "BehaviourData<hypothesis, NumericType,false>(" << iprefix << "T_,"
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
          << mb.getClassName()
          << "IntegrationData<hypothesis, NumericType,false>(" << iprefix
          << "dt_";
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
  }  // end of UMATInterfaceBase::writeBehaviourConstructorHeader

  void UMATInterfaceBase::writeBehaviourConstructorBody(
      std::ostream&, const BehaviourDescription&, const Hypothesis) const {
  }  // end of UMATInterfaceBase::writeBehaviourConstructorBody

  void UMATInterfaceBase::
      writeMaterialPropertiesInitializersInBehaviourDataConstructorI(
          std::ostream& f,
          bool& first,
          const Hypothesis h,
          const BehaviourDescription& mb,
          const std::vector<BehaviourMaterialProperty>& i,
          const SupportedTypes::TypeSize ioffset,
          const std::string& src,
          const std::string& prefix,
          const std::string& suffix) const {
    for (const auto& mp : mb.getBehaviourData(h).getMaterialProperties()) {
      if (mp.arraySize != 1u) {
        continue;
      }
      const auto n = prefix + mp.name + suffix;
      const auto& m = findBehaviourMaterialProperty(i, mp.getExternalName());
      if (!first) {
        f << ",\n";
      }
      first = false;
      const auto flag = SupportedTypes::getTypeFlag(mp.type);
      // note m.offset can be lower than ioffset in some interfaces
      if (flag == SupportedTypes::SCALAR) {
        f << n << "(" + src + "[" << m.offset << "-" << ioffset << "])";
      } else if ((flag == SupportedTypes::TVECTOR) ||
                 (flag == SupportedTypes::STENSOR) ||
                 (flag == SupportedTypes::TENSOR)) {
        f << n << "(&" + src + "[" << m.offset << "-" << ioffset << "])";
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
          const std::vector<BehaviourMaterialProperty>& i,
          const SupportedTypes::TypeSize ioffset,
          const std::string& src,
          const std::string& prefix,
          const std::string& suffix) const {
    for (const auto& mp : mb.getBehaviourData(h).getMaterialProperties()) {
      if (mp.arraySize == 1u) {
        continue;
      }
      const auto& m =
          findBehaviourMaterialProperty(i, mb.getExternalName(h, mp.name));
      const auto flag = SupportedTypes::getTypeFlag(mp.type);
      SupportedTypes::TypeSize offset = m.offset;
      offset -= ioffset;
      const auto n = prefix + mp.name + suffix;
      if (mb.useDynamicallyAllocatedVector(mp.arraySize)) {
        f << n << ".resize(" << mp.arraySize << ");\n";
        f << "for(unsigned short idx=0;idx!=" << mp.arraySize << ";++idx){\n";
        switch (flag) {
          case SupportedTypes::SCALAR:
            f << n << "[idx] = " + src + "[" << offset << "+idx];\n";
            break;
          case SupportedTypes::TVECTOR:
            f << "tfel::fsalgo::copy<TVectorSize>::exe(&" + src + "[" << offset
              << "+idx*TVectorSize]," << n << "[idx].begin());\n";
            break;
          case SupportedTypes::STENSOR:
            f << n << "[idx].import(&" + src + "[" << offset
              << "+idx*StensorSize]);\n";
            break;
          case SupportedTypes::TENSOR:
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
        for (int index = 0; index != mp.arraySize; ++index) {
          switch (flag) {
            case SupportedTypes::SCALAR:
              f << n << "[" << index << "] = " + src + "[" << offset << "];\n";
              break;
            case SupportedTypes::TVECTOR:
              f << "tfel::fsalgo::copy<TVectorSize>::exe(&" + src + "["
                << offset << "]," << n << "[" << index << "].begin());\n";
              break;
            case SupportedTypes::STENSOR:
              f << n << "[" << index << "].import(&" + src + "[" << offset
                << "]);\n";
              break;
            case SupportedTypes::TENSOR:
              f << "tfel::fsalgo::copy<TensorSize>::exe(&" + src + "[" << offset
                << "]," << n << "[" << index << "].begin());\n";
              break;
            default:
              tfel::raise(
                  "UMATInterfaceBase::"
                  "writeVariableInitializersInBehaviourDataConstructorII: "
                  "internal error, tag unsupported");
          }
          offset += SupportedTypes::getTypeSize(mp.type, 1u);
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
        << " * \\brief constructor for the " << this->getInterfaceName()
        << " interface\n"
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
        << "(const NumericType* const " << iprefix
        << "T_,const NumericType* const";
    if (!mp.empty()) {
      out << " " << iprefix << "mat,\n";
    } else {
      out << ",\n";
    }
    out << "const NumericType* const";
    if (!persistentVarsHolder.empty()) {
      out << " " << iprefix << "int_vars\n";
    } else {
      out << "\n";
    }
    if (this->areExternalStateVariablesSupported()) {
      out << ",const NumericType* const";
      if (externalStateVarsHolder.size() != 1) {
        out << " " << iprefix << "ext_vars";
      }
    }
    for (const auto& v : abdv) {
      out << ",const NumericType* const " << iprefix << v.first;
    }
    for (const auto& v : av) {
      out << ",const NumericType* const " << iprefix << v.first;
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
        << " * \\brief constructor for the " << this->getInterfaceName()
        << " interface\n"
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
        << "(const NumericType* const " << iprefix << "dt_";
    if (this->isTemperatureIncrementSupported()) {
      out << ",\nconst NumericType* const " << iprefix << "dT_";
    }
    if (this->areExternalStateVariablesSupported()) {
      if (externalStateVarsHolder.size() != 1) {
        out << ",const NumericType* const " << iprefix << "dext_vars";
      } else {
        out << ",const NumericType* const";
      }
    }
    for (const auto& v : aidv) {
      out << ",const NumericType* const " << iprefix << v.first;
    }
    for (const auto& v : av) {
      out << ",const NumericType* const " << iprefix << v.first;
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
       << "BehaviourDataGradients(const NumericType* const " << iprefix
       << "stran)\n"
       << "{\n";
    for (const auto& v : mb.getMainVariables()) {
      this->writeBehaviourDataGradientSetter(os, v.first, ov);
      ov += SupportedTypes::getTypeSize(v.first.type, 1u);
    }
    os << "}\n\n";
    os << "void set" << iprefix
       << "BehaviourDataThermodynamicForces(const NumericType* const "
       << iprefix << "stress_)\n"
       << "{\n";
    for (const auto& v : mb.getMainVariables()) {
      this->writeBehaviourDataThermodynamicForceSetter(os, v.second, of);
      of += SupportedTypes::getTypeSize(v.second.type, 1u);
    }
    os << "}\n\n";
  }  // end of UMATInterfaceBase::writeBehaviourDataMainVariablesSetters

  void UMATInterfaceBase::writeBehaviourDataGradientSetter(
      std::ostream& os,
      const Gradient& v,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (!Gradient::isIncrementKnown(v)) {
      if (SupportedTypes::getTypeFlag(v.type) == SupportedTypes::TVECTOR) {
        if (!o.isNull()) {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "stran+" << o
             << ",this->" << v.name << "0.begin());\n";
        } else {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "stran,this->"
             << v.name << "0.begin());\n";
        }
      } else if (SupportedTypes::getTypeFlag(v.type) ==
                 SupportedTypes::STENSOR) {
        if (!o.isNull()) {
          os << "this->" << v.name << "0.importVoigt(" << iprefix << "stran+"
             << o << ");\n";
        } else {
          os << "this->" << v.name << "0.importVoigt(" << iprefix
             << "stran);\n";
        }
      } else if (SupportedTypes::getTypeFlag(v.type) ==
                 SupportedTypes::TENSOR) {
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
      if (SupportedTypes::getTypeFlag(v.type) == SupportedTypes::TVECTOR) {
        if (!o.isNull()) {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "stran+" << o
             << ",this->" << v.name << ".begin());\n";
        } else {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "stran,this->"
             << v.name << ".begin());\n";
        }
      } else if (SupportedTypes::getTypeFlag(v.type) ==
                 SupportedTypes::STENSOR) {
        if (!o.isNull()) {
          os << "this->" << v.name << ".importVoigt(" << iprefix << "stran+"
             << o << ");\n";
        } else {
          os << "this->" << v.name << ".importVoigt(" << iprefix << "stran);\n";
        }
      } else if (SupportedTypes::getTypeFlag(v.type) ==
                 SupportedTypes::TENSOR) {
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
  }  // end of UMATInterfaceBase::writeBehaviourDataGradientSetter

  void UMATInterfaceBase::writeBehaviourDataThermodynamicForceSetter(
      std::ostream& os,
      const ThermodynamicForce& f,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (SupportedTypes::getTypeFlag(f.type) == SupportedTypes::TVECTOR) {
      if (!o.isNull()) {
        os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "stress_+" << o
           << ",this->" << f.name << ".begin());\n";
      } else {
        os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "stress_,this->"
           << f.name << ".begin());\n";
      }
    } else if (SupportedTypes::getTypeFlag(f.type) == SupportedTypes::STENSOR) {
      if (!o.isNull()) {
        os << "this->" << f.name << ".importTab(" << iprefix << "stress_+" << o
           << ");\n";
      } else {
        os << "this->" << f.name << ".importTab(" << iprefix << "stress_);\n";
      }
    } else if (SupportedTypes::getTypeFlag(f.type) == SupportedTypes::TENSOR) {
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
       << "IntegrationDataGradients(const NumericType* const " << iprefix
       << "dstran)\n"
       << "{\n";
    for (const auto& v : mb.getMainVariables()) {
      this->writeIntegrationDataGradientSetter(os, v.first, ov);
      ov += SupportedTypes::getTypeSize(v.first.type, 1u);
    }
    os << "}\n\n";
  }  // end of writeIntegrationDataMainVariablesSetters

  void UMATInterfaceBase::writeIntegrationDataGradientSetter(
      std::ostream& os,
      const Gradient& v,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (!Gradient::isIncrementKnown(v)) {
      if (SupportedTypes::getTypeFlag(v.type) == SupportedTypes::TVECTOR) {
        if (!o.isNull()) {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "dstran+" << o
             << ",this->" << v.name << "1.begin());\n";
        } else {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "dstran,this->"
             << v.name << "1.begin());\n";
        }
      } else if (SupportedTypes::getTypeFlag(v.type) ==
                 SupportedTypes::STENSOR) {
        if (!o.isNull()) {
          os << "this->" << v.name << "1.importVoigt(" << iprefix << "dstran+"
             << o << ");\n";
        } else {
          os << "this->" << v.name << "1.importVoigt(" << iprefix
             << "dstran);\n";
        }
      } else if (SupportedTypes::getTypeFlag(v.type) ==
                 SupportedTypes::TENSOR) {
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
      if (SupportedTypes::getTypeFlag(v.type) == SupportedTypes::TVECTOR) {
        if (!o.isNull()) {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "dstran+" << o
             << ",this->d" << v.name << ".begin());\n";
        } else {
          os << "tfel::fsalgo::copy<N>::exe(" << iprefix << "dstran,this->d"
             << v.name << ".begin());\n";
        }
      } else if (SupportedTypes::getTypeFlag(v.type) ==
                 SupportedTypes::STENSOR) {
        if (!o.isNull()) {
          os << "this->d" << v.name << ".importVoigt(" << iprefix << "dstran+"
             << o << ");\n";
        } else {
          os << "this->d" << v.name << ".importVoigt(" << iprefix
             << "dstran);\n";
        }
      } else if (SupportedTypes::getTypeFlag(v.type) ==
                 SupportedTypes::TENSOR) {
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
  }  // end of UMATInterfaceBase::writeIntegrationDataGradientSetter

  void UMATInterfaceBase::getExtraSrcIncludes(
      std::ostream& out, const BehaviourDescription& mb) const {
    if (mb.hasParameters()) {
      out << "#include<iostream>\n"
          << "#include<stdexcept>\n";
    }
    if (this->shallGenerateMTestFileOnFailure(mb)) {
      out << "#include<vector>\n"
          << "#include<sstream>\n"
          << "#include\"TFEL/Material/ModellingHypothesis.hxx\"\n"
          << "#include\"MFront/SupportedTypes.hxx\"\n"
          << "#include\"MFront/UmatSmallStrainMTestFileGenerator.hxx\"\n"
          << "#include\"MFront/UmatFiniteStrainMTestFileGenerator.hxx\"\n";
    }
  }  // end of UMATInterfaceBase::getExtraSrcIncludes

  void UMATInterfaceBase::generateMTestFile1(
      std::ostream& out, const BehaviourDescription& bd) const {
    if (this->shallGenerateMTestFileOnFailure(bd)) {
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
      const BehaviourDescription& mb,
      const BehaviourDescription::BehaviourType type,
      const std::string& name,
      const std::string& suffix) const {
    if (this->shallGenerateMTestFileOnFailure(mb)) {
      const auto& gh = this->gatherModellingHypothesesAndTests(mb);
      for (const auto& e : gh) {
        const auto b = e.second != "true";
        if (b) {
          out << "if(" << e.second << "){\n";
        }
        this->generateMTestFileForHypothesis(out, mb, type, name, suffix,
                                             e.first);
        if (b) {
          out << "}\n";
        }
      }
    }
  }  // end of UMATInterfaceBase::generateMTestFile2

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

  std::string UMATInterfaceBase::getModellingHypothesisTest(
      const Hypothesis) const {
    tfel::raise(
        "UMATInterfaceBase::getModellingHypothesisTest: "
        "invalid call, this method is only valid for some interfaces "
        "that must override it.");
  }  // end of UMATInterfaceBase::getModellingHypothesisTest

  void UMATInterfaceBase::generateMTestFileForHypothesis(
      std::ostream& out,
      const BehaviourDescription& bd,
      const BehaviourDescription::BehaviourType type,
      const std::string& name,
      const std::string& suffix,
      const Hypothesis h) const {
    const auto& d = bd.getBehaviourData(h);
    const auto& persistentVarsHolder = d.getPersistentVariables();
    const auto& externalStateVarsHolder = d.getExternalStateVariables();
    const auto mprops = this->buildMaterialPropertiesList(bd, h);
    unsigned int offset = 0;
    const auto fname = [&name, &suffix] {
      if (!suffix.empty()) {
        return name + "_" + suffix;
      }
      return name;
    }();
    out << "try{\n";
    if (type == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      out << "mfront::UmatSmallStrainMTestFileGenerator mg(\""
          << makeLowerCase(this->getInterfaceName()) << "\",\""
          << this->getLibraryName(bd) << "\",\""
          << this->getFunctionNameBasis(fname) << "\");\n";
    } else if (type == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "mfront::UmatFiniteStrainMTestFileGenerator mg(\""
          << makeLowerCase(this->getInterfaceName()) << "\",\""
          << this->getLibraryName(bd) << "\",\""
          << this->getFunctionNameBasis(fname) << "\");\n";
    } else {
      tfel::raise(
          "UMATInterfaceBase::generateMTestFileForHypothesis: "
          "only small strain or finite strain behaviours are supported");
    }
    this->writeMTestFileGeneratorSetModellingHypothesis(out);
    this->writeMTestFileGeneratorSetRotationMatrix(out, bd);
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
          "UMATInterfaceBase::generateMTestFileForHypothesis: "
          "only small strain or finite strain behaviours are supported");
    }
    for (const auto& m : mprops.first) {
      auto flag = SupportedTypes::getTypeFlag(m.type);
      tfel::raise_if(flag != SupportedTypes::SCALAR,
                     "UMATInterfaceBase::generateMTestFileForHypothesis: "
                     "unsupported external state variable type "
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
            out << "mg.addMaterialProperty(\"" << m.name << "[" << s << "]\","
                << "*(PROPS+" << offset << "));\n";
          }
        }
      }
    }
    SupportedTypes::TypeSize ivoffset;
    for (const auto& v : persistentVarsHolder) {
      auto flag = SupportedTypes::getTypeFlag(v.type);
      const auto& ivname = d.getExternalName(v.name);
      tfel::raise_if(
          (flag != SupportedTypes::SCALAR) && (flag != SupportedTypes::STENSOR),
          "UMATInterfaceBase::generateMTestFileForHypothesis: "
          "unsupported material property type "
          "in mtest file generation");
      if (v.arraySize == 1u) {
        if (flag == SupportedTypes::SCALAR) {
          out << "mg.addInternalStateVariable(\"" << ivname
              << "\",SupportedTypes::SCALAR,&mg_STATEV[" << ivoffset << "]);\n";
          ivoffset += SupportedTypes::TypeSize(SupportedTypes::SCALAR);
        } else {
          out << "mg.addInternalStateVariable(\"" << ivname
              << "\",SupportedTypes::STENSOR,&mg_STATEV[" << ivoffset
              << "]);\n";
          ivoffset += SupportedTypes::TypeSize(SupportedTypes::STENSOR);
        }
      } else {
        if (v.arraySize >= SupportedTypes::ArraySizeLimit) {
          out << "for(unsigned short i=0;i!=" << v.arraySize << ";++i){\n";
          out << "auto name =  \"" << ivname
              << "[\" + std::to_string(i)+ \"]\";\n";
          if (flag == SupportedTypes::SCALAR) {
            out << "mg.addInternalStateVariable(name,SupportedTypes::SCALAR,&"
                   "mg_STATEV["
                << ivoffset << "]+i);\n";
          } else {
            out << "mg.addInternalStateVariable(name,SupportedTypes::STENSOR,"
                   "&"
                   "mg_STATEV["
                << ivoffset << "]+i);\n";
          }
          out << "}\n";
          if (flag == SupportedTypes::SCALAR) {
            ivoffset +=
                SupportedTypes::TypeSize(v.arraySize, SupportedTypes::SCALAR);
          } else {
            ivoffset +=
                SupportedTypes::TypeSize(v.arraySize, SupportedTypes::STENSOR);
          }
        } else {
          for (unsigned short i = 0; i != v.arraySize; ++i) {
            if (flag == SupportedTypes::SCALAR) {
              out << "mg.addInternalStateVariable(\"" << ivname << "[" << i
                  << "]\",SupportedTypes::SCALAR,&mg_STATEV[" << ivoffset
                  << "]);\n";
              ivoffset += SupportedTypes::TypeSize(SupportedTypes::SCALAR);
            } else {
              out << "mg.addInternalStateVariable(\"" << ivname << "[" << i
                  << "]\",SupportedTypes::STENSOR,&mg_STATEV[" << ivoffset
                  << "]);\n";
              ivoffset += SupportedTypes::TypeSize(SupportedTypes::STENSOR);
            }
          }
        }
      }
    }
    out << "mg.addExternalStateVariableValue(\"Temperature\",0.,*TEMP);\n";
    out << "mg.addExternalStateVariableValue(\"Temperature\",dt,*TEMP+*DTEMP)"
           ";"
           "\n";
    auto p = std::next(externalStateVarsHolder.begin());
    for (offset = 0; p != externalStateVarsHolder.end(); ++p) {
      auto flag = SupportedTypes::getTypeFlag(p->type);
      tfel::raise_if(flag != SupportedTypes::SCALAR,
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
            out << "mg.addExternalStateVariableValue(name,0,*(PREDEF+i));\n";
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
              out << "mg.addExternalStateVariableValue(\"" << evname << "[" << i
                  << "]\",0,*PREDEF);\n";
              out << "mg.addExternalStateVariableValue(\"" << evname << "[" << i
                  << "]\",dt,*PREDEF+*DPRED);\n";
            } else {
              out << "mg.addExternalStateVariableValue(\"" << evname << "[" << i
                  << "]\","
                     "0,*(PREDEF+"
                  << offset << "));\n";
              out << "mg.addExternalStateVariableValue(\"" << evname << "[" << i
                  << "]\","
                     "dt,*(PREDEF+"
                  << offset << ")+*(DPRED+" << offset << "));\n";
            }
          }
        }
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
  }  // end of generateMTestFileForHypothesis();

  void UMATInterfaceBase::writeMTestFileGeneratorSetRotationMatrix(
      std::ostream& out, const BehaviourDescription& mb) const {
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "mg.setRotationMatrix("
          << "DROT[0],DROT[3],DROT[6],"
          << "DROT[1],DROT[4],DROT[7],"
          << "DROT[2],DROT[5],DROT[8]);\n";
    }
  }  // end of UMATInterfaceBase::writeMTestFileGeneratorSetRotationMatrix

  bool UMATInterfaceBase::areExternalStateVariablesSupported() const {
    return true;
  }  // end of UMATInterfaceBase::areExternalStateVariablesSupported()

  bool UMATInterfaceBase::isTemperatureIncrementSupported() const {
    return true;
  }  // end of UMATInterfaceBase::isTemperatureIncrementSupported()

  UMATInterfaceBase::~UMATInterfaceBase() = default;

}  // end of namespace mfront
