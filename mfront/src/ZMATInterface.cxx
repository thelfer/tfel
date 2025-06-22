/*!
 * \file  mfront/src/ZMATInterface.cxx
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

#include <cstdlib>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/System/System.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"

#include "MFront/DSLUtilities.hxx"
#include "MFront/InstallPath.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/ZMATInterface.hxx"

namespace mfront {

  static void writeZMATUndefs(std::ostream& out) {
    out << "#include\"MFront/ZMAT/ZMATUndefs.hxx\"\n";
  }  // end of writeZMATUndefs

  template <typename ArrayType>
  static void writeArray(std::ostream& out, const ArrayType& a) {
    std::string buffer;
    out << "{";
    for (auto pn = a.begin(); pn != a.end();) {
      if (buffer.size() + pn->size() + 3 > 15) {
        out << buffer << '\n';
        buffer.clear();
      }
      buffer += "\"" + *pn + "\"";
      if (++pn != a.end()) {
        buffer += ",";
      }
    }
    if (!buffer.empty()) {
      out << buffer;
    }
    out << "};\n";
  }

  static std::set<std::string> getAllMaterialPropertiesNames(
      const BehaviourDescription& mb) {
    using ModellingHypothesis = ZMATInterface::ModellingHypothesis;
    auto mp_names = std::set<std::string>{};
    for (const auto h :
         {ModellingHypothesis::TRIDIMENSIONAL,
          ModellingHypothesis::GENERALISEDPLANESTRAIN,
          ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN}) {
      if (mb.isModellingHypothesisSupported(h)) {
        const auto& d = mb.getBehaviourData(h);
        const auto& mps = d.getMaterialProperties();
        for (const auto& v : mps) {
          const auto& name = d.getExternalName(v.name);
          if (v.arraySize == 1u) {
            mp_names.insert(name);
          } else {
            for (unsigned short n = 0; n != v.arraySize; ++n) {
              std::ostringstream str;
              str << name << '[' << n << ']';
              mp_names.insert(str.str());
            }
          }
        }
      }
    }
    return mp_names;
  }

  static VariableDescriptionContainer getAllStateVariables(
      const BehaviourDescription& mb) {
    using ModellingHypothesis = ZMATInterface::ModellingHypothesis;
    using Hypothesis = ZMATInterface::Hypothesis;
    auto s = VariableDescriptionContainer{};
    for (const Hypothesis h :
         {ModellingHypothesis::TRIDIMENSIONAL,
          ModellingHypothesis::GENERALISEDPLANESTRAIN,
          ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN}) {
      if (mb.isModellingHypothesisSupported(h)) {
        const auto& d = mb.getBehaviourData(h);
        const auto& svs = d.getPersistentVariables();
        for (const auto& sv : svs) {
          if (s.contains(sv.name)) {
            const auto& v = s.getVariable(sv.name);
            tfel::raise_if((sv.type != v.type) || (sv.arraySize != v.arraySize),
                           "getAllStateVariables : "
                           "inconsistent type for variable '" +
                               sv.name + "'");
          } else {
            s.push_back(sv);
          }
        }
      }
    }
    return s;
  }

  static std::string getSpaceDimensionSuffix(
      const ZMATInterface::Hypothesis h) {
    using ModellingHypothesis = ZMATInterface::ModellingHypothesis;
    if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      return "3D";
    } else if (h == ModellingHypothesis::GENERALISEDPLANESTRAIN) {
      return "2D";
    } else if (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
      return "1D";
    }
    tfel::raise("getSpaceDimension: unsupported hypothesis");
  }

  static std::string getLibraryName(const BehaviourDescription& mb) {
    if (mb.getLibrary().empty()) {
      if (!mb.getMaterialName().empty()) {
        return "ZMAT" + mb.getMaterialName();
      } else {
        return "ZMATBehaviour";
      }
    }
    return "ZMAT" + mb.getLibrary();
  }  // end of getLibraryName

  std::string ZMATInterface::getName() { return "zmat"; }

  ZMATInterface::ZMATInterface() = default;

  std::string ZMATInterface::getInterfaceName() const { return "ZMAT"; }

  std::string ZMATInterface::getInterfaceVersion() const { return ""; }

  void
  ZMATInterface::checkIfTemperatureIsDefinedAsTheFirstExternalStateVariable(
      const BehaviourDescription& bd) const {
    if (!bd.isTemperatureDefinedAsTheFirstExternalStateVariable()) {
      const auto v = this->getInterfaceVersion();
      auto msg = std::string{};
      msg +=
          "ZMATInterface::"
          "checkIfTemperatureIsDefinedAsTheFirstExternalStateVariable: "
          "the temperature must be defined as the first external state "
          "variable";
      if (!v.empty()) {
        msg += " in interface version '" + v + "'";
      }
      tfel::raise(msg);
    }
  }  // end of checkIfTemperatureIsDefinedAsTheFirstExternalStateVariable

  std::pair<bool, ZMATInterface::tokens_iterator> ZMATInterface::treatKeyword(
      BehaviourDescription&,
      const std::string& key,
      const std::vector<std::string>& i,
      tokens_iterator current,
      const tokens_iterator) {
    if (std::find(i.begin(), i.end(), this->getName()) != i.end()) {
      tfel::raise(
          "ZMATInterface::treatKeyword: "
          "unsupported keyword '" +
          key + "'");
    }
    return {false, current};
  }

  bool ZMATInterface::isBehaviourConstructorRequired(
      const Hypothesis h, const BehaviourDescription& mb) const {
    const auto ih = this->getModellingHypothesesToBeTreated(mb);
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      return !mb.areAllMechanicalDataSpecialised(ih);
    }
    return ih.find(h) != ih.end();
  }

  std::set<ZMATInterface::Hypothesis>
  ZMATInterface::getModellingHypothesesToBeTreated(
      const BehaviourDescription& mb) const {
    // treatment
    std::set<ModellingHypothesis::Hypothesis> h;
    // modelling hypotheses handled by the behaviour
    const auto& bh = mb.getModellingHypotheses();
    if (bh.find(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) !=
        bh.end()) {
      h.insert(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN);
    }
    if (bh.find(ModellingHypothesis::GENERALISEDPLANESTRAIN) != bh.end()) {
      h.insert(ModellingHypothesis::GENERALISEDPLANESTRAIN);
    }
    if (bh.find(ModellingHypothesis::TRIDIMENSIONAL) != bh.end()) {
      h.insert(ModellingHypothesis::TRIDIMENSIONAL);
    }
    tfel::raise_if(
        h.empty(),
        "ZMATInterfaceModellingHypothesesToBeTreated: "
        "no hypotheses selected. This means that the given beahviour "
        "can't be used neither in 'AxisymmetricalGeneralisedPlaneStrain' "
        "nor in 'AxisymmetricalGeneralisedPlaneStress', so it does not "
        "make sense to use the ZMAT interface");
    return h;
  }  // edn of ZMATInterface::getModellingHypothesesToBeTreated

  void ZMATInterface::exportMechanicalData(
      std::ostream& out,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    using namespace std;
    const auto& d = mb.getBehaviourData(h);
    const auto& persistentVarsHolder = d.getPersistentVariables();
    if (!persistentVarsHolder.empty()) {
      out << "void\n"
          << "ZMATexportStateData("
          << "ZSET::TENSOR2_FLUX& ZMATsig,ZSET::MAT_DATA& ZMATdata) const\n";
    } else {
      out << "void\n"
          << "ZMATexportStateData("
          << "ZSET::TENSOR2_FLUX& ZMATsig,ZSET::MAT_DATA&) const\n";
    }
    out << "{\n"
        << "using namespace tfel::math;\n";
    tfel::raise_if(!((mb.getBehaviourType() ==
                      BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
                     (mb.getBehaviourType() ==
                      BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)),
                   "ZMATInterface::exportMechanicalData: "
                   "only small or finite strain behaviours are supported");
    out << "zmat::ZMATInterface::convert(&ZMATsig[0],this->sig);\n";
    if (!persistentVarsHolder.empty()) {
      out << "auto& ZMATstatev = ZMATdata.var_int()[0];\n";
      SupportedTypes::TypeSize currentOffset;
      for (const auto& v : persistentVarsHolder) {
        const auto flag = SupportedTypes::getTypeFlag(v.type);
        if (v.arraySize == 1u) {
          if (flag == SupportedTypes::SCALAR) {
            if (mb.useQt()) {
              out << "ZMATstatev"
                  << "[" << currentOffset << "] = base_type_cast(this->"
                  << v.name << ");\n";
            } else {
              out << "ZMATstatev"
                  << "[" << currentOffset << "] = this->" << v.name << ";\n";
            }
          } else if ((flag == SupportedTypes::TVECTOR) ||
                     (flag == SupportedTypes::STENSOR) ||
                     (flag == SupportedTypes::TENSOR)) {
            out << "zmat::ZMATInterface::convert(&"
                << "ZMATstatev"
                << "[" << currentOffset << "],this->" << v.name << ");\n";
          } else {
            tfel::raise(
                "ZMATInterface::exportMechanicalData: "
                "internal error, tag unsupported");
          }
          currentOffset += this->getTypeSize(v.type, v.arraySize);
        } else {
          if (mb.useDynamicallyAllocatedVector(v.arraySize)) {
            out << "for(unsigned short idx=0;idx!=" << v.arraySize
                << ";++idx){\n";
            if (flag == SupportedTypes::SCALAR) {
              if (mb.useQt()) {
                out << "ZMATstatev"
                    << "[" << currentOffset << "+idx] = common_cast(this->"
                    << v.name << "[idx]);\n";
              } else {
                out << "ZMATstatev"
                    << "[" << currentOffset << "+idx] = this->" << v.name
                    << "[idx];\n";
              }
            } else if ((flag == SupportedTypes::TVECTOR) ||
                       (flag == SupportedTypes::STENSOR) ||
                       (flag == SupportedTypes::TENSOR)) {
              out << "zmat::ZMATInterface::convert(&ZMATstatev"
                  << "[" << currentOffset << "+idx*StensorSize],this->"
                  << v.name << "[idx]);\n";
            } else {
              tfel::raise(
                  "ZMATInterface::exportMechanicalData: "
                  "internal error, tag unsupported");
            }
            out << "}\n";
            currentOffset += this->getTypeSize(v.type, v.arraySize);
          } else {
            for (unsigned short i = 0; i != v.arraySize; ++i) {
              if (flag == SupportedTypes::SCALAR) {
                if (mb.useQt()) {
                  out << "ZMATstatev"
                      << "[" << currentOffset << "] = common_cast(this->"
                      << v.name << "[" << i << "]);\n";
                } else {
                  out << "ZMATstatev"
                      << "[" << currentOffset << "] = this->" << v.name << "["
                      << i << "];\n";
                }
              } else if ((flag == SupportedTypes::TVECTOR) ||
                         (flag == SupportedTypes::STENSOR) ||
                         (flag == SupportedTypes::TENSOR)) {
                out << "zmat::ZMATInterface::convert(&"
                    << "ZMATstatev"
                    << "[" << currentOffset << "],this->" << v.name << "[" << i
                    << "]);\n";
              } else {
                tfel::raise(
                    "ZMATInterface::exportMechanicalData: "
                    "internal error, tag unsupported");
              }
              currentOffset += this->getTypeSize(v.type, 1u);
            }
          }
        }
      }
    }
    out << "} // end of ZMATexportStateData\n\n";
  }

  void ZMATInterface::writeInterfaceSpecificIncludes(
      std::ostream& out, const BehaviourDescription& mb) const {
    out << "#include\"MFront/ZMAT/ZMATInterface.hxx\"\n"
        << "#include\"Coefficient.h\"\n";
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      out << "#include\"Auto_ptr.h\"\n"
          << "#include\"Elasticity.h\"\n";
    }
    out << "#include\"Coefficient.h\"\n"
        << "#include\"Behavior.h\"\n";
    writeZMATUndefs(out);
    out << '\n';
  }  // end of ZMATInterface::writeInterfaceSpecificIncludes

  void ZMATInterface::writeBehaviourDataConstructor(
      std::ostream& out,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    using namespace std;
    this->checkIfTemperatureIsDefinedAsTheFirstExternalStateVariable(mb);
    const auto& d = mb.getBehaviourData(h);
    const auto& mps = d.getMaterialProperties();
    const auto& ivs = d.getPersistentVariables();
    auto evs = d.getExternalStateVariables();
    // removing the temperature
    evs.erase(evs.begin());
    tfel::raise_if(!((mb.getBehaviourType() ==
                      BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
                     (mb.getBehaviourType() ==
                      BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)),
                   "ZMATInterface::writeBehaviourDataConstructor: "
                   "only small or finite strain behaviours are supported");
    out << "/*!\n"
        << " * \\brief constructor for the ZMAT interface\n"
        << " *\n";
    if ((mb.getBehaviourType() ==
         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
        (mb.getBehaviourType() ==
         BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)) {
      out << " * \\param ZMATsig     : stress at the beginning of the time "
             "step\n";
    }
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      out << " \\param ZMATeto     : strain at the beginning of the time "
             "step\n";
    }
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << " \\param ZMATF0      : deformation gradient at the begining of "
             "the time step\n";
    }
    out << " * \\param ZMATmprops  : material properties\n"
        << " * \\param ZMATdata    : material data handler\n"
        << " * \\param ZMATT_pos   : position of the temperature\n"
        << " * \\param ZMATevs_pos : position of the external state varaibles\n"
        << " */\n"
        << mb.getClassName() << "BehaviourData(";
    if ((mb.getBehaviourType() ==
         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
        (mb.getBehaviourType() ==
         BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)) {
      out << "const ZSET::TENSOR2_FLUX& ZMATsig,\n";
    }
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR:
        out << "const real * const ZMATeto,\n";
        break;
      case BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR:
        out << "const real * const ZMATF0,\n";
        break;
      default:
        tfel::raise(
            "ZMATInterface::writeBehaviourDataConstructor: "
            "unsupported behaviour type");
    }
    if (!mps.empty()) {
      out << "const ZSET::ARRAY<ZSET::COEFF>& ZMATmprops,\n";
    } else {
      out << "const ZSET::ARRAY<ZSET::COEFF>&,\n";
    }
    out << "const ZSET::MAT_DATA& ZMATdata,\n"
        << "const int ZMATT_pos,\n";
    if (!evs.empty()) {
      out << "const ZSET::ARRAY<int>& ZMATevs_pos){\n";
    } else {
      out << "const ZSET::ARRAY<int>&){\n";
    }
    if (!ivs.empty()) {
      out << "const auto& ZMATstatev = ZMATdata.var_int_ini()[0];\n";
    }
    out << "const ZSET::EXTERNAL_PARAMETER_VECTOR& ZMATextvars_t = "
           "*(ZMATdata.param_set_ini());\n"
        << "zmat::ZMATInterface::convert(this->sig,&ZMATsig[0]);\n";
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR:
        out << "zmat::ZMATInterface::convert(this->eto,&ZMATeto[0]);\n";
        break;
      case BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR:
        out << "zmat::ZMATInterface::convert(this->F0,&ZMATF0[0]);\n";
        break;
      default:
        tfel::raise(
            "ZMATInterface::writeCallMFrontBehaviour: "
            "unsupported behaviour type");
    }
    if (!mps.empty()) {
      SupportedTypes::TypeSize o;
      for (const auto& mp : mps) {
        const SupportedTypes::TypeFlag flag =
            SupportedTypes::getTypeFlag(mp.type);
        if (flag == SupportedTypes::SCALAR) {
          if (mp.arraySize == 1u) {
            out << "this->" << mp.name << " = ZMATmprops[" << o << "]();\n";
            o += this->getTypeSize(mp.type, 1u);
          } else {
            if (mb.useDynamicallyAllocatedVector(mp.arraySize)) {
              out << "this->" << mp.name << ".resize(" << mp.arraySize
                  << ");\n";
              out << "for(unsigned short idx=0;idx!=" << mp.arraySize
                  << ";++idx){\n";
              out << "this->" << mp.name << "[idx] = ZMATmprops[" << o
                  << "+idx]();\n";
              out << "}\n";
              o += this->getTypeSize(mp.type, mp.arraySize);
            } else {
              for (unsigned short i = 0; i != mp.arraySize; ++i) {
                out << "this->" << mp.name << "[" << i << "] = ZMATmprops[" << o
                    << "]();\n";
                o += this->getTypeSize(mp.type, 1u);
              }
            }
          }
        } else {
          tfel::raise(
              "ZMATInterface::writeBehaviourDataConstructor : "
              "unsupported material properties type "
              "(only scalar supported yet)");
        }
      }
    }
    if (!ivs.empty()) {
      SupportedTypes::TypeSize o;
      for (const auto& iv : ivs) {
        const string n = "this->" + iv.name;
        const SupportedTypes::TypeFlag flag =
            SupportedTypes::getTypeFlag(iv.type);
        if (iv.arraySize == 1u) {
          switch (flag) {
            case SupportedTypes::SCALAR:
              out << n << " = ZMATstatev[" << o << "];\n";
              break;
            case SupportedTypes::STENSOR:
            case SupportedTypes::TENSOR:
              out << "zmat::ZMATInterface::convert(" << n << ","
                  << "&ZMATstatev[" << o << "]);\n";
              break;
            default:
              tfel::raise(
                  "ZMATInterface::writeBehaviourDataConstructor: "
                  "unsupported variable type ('" +
                  iv.type + "')");
          }
          o += this->getTypeSize(iv.type, 1u);
        } else {
          if (mb.useDynamicallyAllocatedVector(iv.arraySize)) {
            out << n << ".resize(" << iv.arraySize << ");\n";
            out << "for(unsigned short idx=0;idx!=" << iv.arraySize
                << ";++idx){\n";
            switch (flag) {
              case SupportedTypes::SCALAR:
                out << n << "[idx] = ZMATstatev[" << o << "+idx];\n";
                break;
              // case SupportedTypes::TVECTOR :
              //   out << "tfel::fsalgo::copy<TVectorSize>::exe(&"+src+"["
              // 	<< o << "+idx*TVectorSize],"
              // 	<< n << "[idx].begin());\n";
              //   break;
              case SupportedTypes::STENSOR:
              case SupportedTypes::TENSOR:
                out << "zmat::ZMATInterface::convert(" << n
                    << "[idx],&ZMATstatev[" << o << "+idx*StensorSize]);\n";
                break;
              default:
                tfel::raise(
                    "ZMATInterface::writeBehaviourDataConstructor: "
                    "unsupported variable type ('" +
                    iv.type + "')");
            }
            out << "}\n";
            o += this->getTypeSize(iv.type, iv.arraySize);
          } else {
            for (int i = 0; i != iv.arraySize; ++i) {
              switch (flag) {
                case SupportedTypes::SCALAR:
                  out << n << "[" << i << "] = ZMATstatev[" << o << "];\n";
                  break;
                case SupportedTypes::STENSOR:
                case SupportedTypes::TENSOR:
                  out << "zmat::ZMATInterface::convert(" << n << "[" << i
                      << "],&ZMATstatev[" << o << "]);\n";
                  break;
                default:
                  tfel::raise(
                      "ZMATInterface::writeBehaviourDataConstructor: "
                      "unsupported variable type ('" +
                      iv.type + "')");
              }
              o += this->getTypeSize(iv.type, 1u);
            }
          }
        }
      }
    }
    if (!evs.empty()) {
      SupportedTypes::TypeSize o;
      for (const auto& ev : evs) {
        const auto flag = SupportedTypes::getTypeFlag(ev.type);
        if (flag == SupportedTypes::SCALAR) {
          if (ev.arraySize == 1u) {
            out << "this->" << ev.name << " = ZMATextvars_t[ZMATevs_pos[" << o
                << "]];\n";
            o += this->getTypeSize(ev.type, 1u);
          } else {
            if (mb.useDynamicallyAllocatedVector(ev.arraySize)) {
              out << "this->" << ev.name << ".resize(" << ev.arraySize
                  << ");\n";
              out << "for(unsigned short idx=0;idx!=" << ev.arraySize
                  << ";++idx){\n";
              out << "this->" << ev.name << "[idx] = ZMATextvars_t[ZMATevs_pos["
                  << o << "+idx]];\n";
              out << "}\n";
              o += this->getTypeSize(ev.type, ev.arraySize);
            } else {
              for (unsigned short i = 0; i != ev.arraySize; ++i) {
                out << "this->" << ev.name << "[" << i
                    << "] = ZMATextvars_t[ZMATevs_pos[" << o << "]];\n";
                o += this->getTypeSize(ev.type, 1u);
              }
            }
          }
        } else {
          tfel::raise(
              "ZMATInterface::writeBehaviourDataConstructor: "
              "unsupported material properties type "
              "(only scalar supported yet)");
        }
      }
    }
    out << "this->T = ZMATextvars_t[ZMATT_pos];\n"
        << "}\n\n";
  }  // end of ZMATInterface::writeBehaviourDataConstructor

  void ZMATInterface::writeBehaviourConstructorHeader(
      std::ostream& out,
      const BehaviourDescription& mb,
      const Hypothesis,
      const std::string& initStateVarsIncrements) const {
    using namespace std;
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "ZMATInterface::writeBehaviourConstructorHeader: " + m);
    };
    throw_if(!((mb.getBehaviourType() ==
                BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
               (mb.getBehaviourType() ==
                BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)),
             "only small or finite strain behaviours are supported");
    out << "/*!\n"
        << " * \\brief constructor for the ZMAT interface\n"
        << " *\n";
    if ((mb.getBehaviourType() ==
         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
        (mb.getBehaviourType() ==
         BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)) {
      out << " * \\param ZMATsig     : strains\n";
    }
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR:
        out << " \\param ZMATeto     : strain at the beginning of the time "
               "step\n"
            << " \\param ZMATdeto    : strain increment\n";
        break;
      case BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR:
        out << " \\param ZMATF0 : deformation gradient at the beginning of the "
               "time step\n"
            << " \\param ZMATF1 : deformation gradient at the end of the time "
               "step\n";
        break;
      default:
        throw_if(true, "unsupported behaviour type");
    }
    out << " * \\param ZMATmprops  : material properties\n"
        << " * \\param ZMATdata    : material data handler\n"
        << " * \\param ZMATT_pos   : position of the temperature\n"
        << " * \\param ZMATevs_pos : position of the external state varaibles\n"
        << " * \\param ZMATdt      : time step\n"
        << " */\n"
        << mb.getClassName() << "(";
    if ((mb.getBehaviourType() ==
         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
        (mb.getBehaviourType() ==
         BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)) {
      out << "const ZSET::TENSOR2_FLUX& ZMATsig,\n";
    }
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR:
        out << " const real* const ZMATeto,\n"
            << " const real* const ZMATdeto,\n";
        break;
      case BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR:
        out << " const real* const ZMATF0,\n"
            << " const real* const ZMATF1,\n";
        break;
      default:
        throw_if(true, "unsupported behaviour type");
    }
    out << "const ZSET::ARRAY<ZSET::COEFF>& ZMATmprops,\n"
        << "const ZSET::MAT_DATA& ZMATdata,\n"
        << "const int ZMATT_pos,\n"
        << "const ZSET::ARRAY<int>& ZMATevs_pos,\n"
        << "const double ZMATdt)\n";
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR:
        if (mb.useQt()) {
          out << ": " << mb.getClassName()
              << "BehaviourData<hypothesis, "
                 "NumericType,use_qt>(ZMATsig,ZMATeto,"
                 "ZMATmprops,ZMATdata,ZMATT_pos,ZMATevs_pos),\n";
          out << mb.getClassName()
              << "IntegrationData<hypothesis, NumericType,use_qt>("
              << "ZMATdeto,ZMATdata,ZMATT_pos,ZMATevs_pos,"
              << "ZMATdt)";
        } else {
          out << ": " << mb.getClassName()
              << "BehaviourData<hypothesis, NumericType,false>(ZMATsig,ZMATeto,"
                 "ZMATmprops,ZMATdata,ZMATT_pos,ZMATevs_pos),\n";
          out << mb.getClassName()
              << "IntegrationData<hypothesis, NumericType,false>("
              << "ZMATdeto,ZMATdata,ZMATT_pos,ZMATevs_pos,"
              << "ZMATdt)";
        }
        break;
      case BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR:
        if (mb.useQt()) {
          out << ": " << mb.getClassName()
              << "BehaviourData<hypothesis, NumericType,use_qt>(ZMATsig,ZMATF0,"
                 "ZMATmprops,ZMATdata,ZMATT_pos,ZMATevs_pos),\n";
          out << mb.getClassName()
              << "IntegrationData<hypothesis, NumericType,use_qt>("
              << "ZMATF1,ZMATdata,ZMATT_pos,ZMATevs_pos,"
              << "ZMATdt)";
        } else {
          out << ": " << mb.getClassName()
              << "BehaviourData<hypothesis, NumericType,false>(ZMATsig,ZMATF0,"
                 "ZMATmprops,ZMATdata,ZMATT_pos,ZMATevs_pos),\n";
          out << mb.getClassName()
              << "IntegrationData<hypothesis, NumericType,false>("
              << "ZMATF1,ZMATdata,ZMATT_pos,ZMATevs_pos,"
              << "ZMATdt)";
        }
        break;
      default:
        throw_if(true, "unsupported behaviour type");
    }
    if (!initStateVarsIncrements.empty()) {
      out << ",\n" << initStateVarsIncrements;
    }
  }

  void ZMATInterface::writeBehaviourConstructorBody(std::ostream&,
                                                    const BehaviourDescription&,
                                                    const Hypothesis) const {
  }  // end of ZMATInterface::writeBehaviourConstructorBody

  void ZMATInterface::writeIntegrationDataConstructor(
      std::ostream& out,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    using namespace std;
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "ZMATInterface::writeIntegrationDataConstructor: " + m);
    };
    this->checkIfTemperatureIsDefinedAsTheFirstExternalStateVariable(mb);
    const auto& d = mb.getBehaviourData(h);
    auto evs = d.getExternalStateVariables();
    // removing the temperature
    evs.erase(evs.begin());
    throw_if(!((mb.getBehaviourType() ==
                BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
               (mb.getBehaviourType() ==
                BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)),
             "only small or finite strain behaviours are supported");
    out << "/*!\n"
        << " * \\brief constructor for the ZMAT interface\n"
        << " *\n";
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR:
        out << " \\param ZMATdeto  : strain increment \n";
        break;
      case BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR:
        out << " \\param ZMATF1    : deformation gradient at the end of the "
               "time step\n";
        break;
      default:
        throw_if(true, "unsupported behaviour type");
    }
    out << " * \\param ZMATdata    : material data handler\n"
        << " * \\param ZMATT_pos   : position of the temperature\n"
        << " * \\param ZMATevs_pos : position of the external state varaibles\n"
        << " * \\param ZMATdt      : time step\n"
        << " */\n"
        << mb.getClassName() << "IntegrationData(";
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR:
        out << "const real * const ZMATdeto,\n";
        break;
      case BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR:
        out << "const real * const ZMATF1,\n";
        break;
      default:
        throw_if(true, "unsupported behaviour type");
    }
    out << "const ZSET::MAT_DATA& ZMATdata,\n"
        << "const int ZMATT_pos,\n";
    if (!evs.empty()) {
      out << "const ZSET::ARRAY<int>& ZMATevs_pos,\n";
    } else {
      out << "const ZSET::ARRAY<int>&,\n";
    }
    out << "const double ZMATdt)\n"
        << ": dt(ZMATdt)\n";
    out << "{\n"
        << "const auto& ZMATextvars_t   = *(ZMATdata.param_set_ini());\n"
        << "const auto& ZMATextvars_tdt = *(ZMATdata.param_set());\n";
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR:
        out << "zmat::ZMATInterface::convert(this->deto,ZMATdeto);\n";
        break;
      case BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR:
        out << "zmat::ZMATInterface::convert(this->F1,ZMATF1);\n";
        break;
      default:
        throw_if(true, "unsupported behaviour type");
    }
    if (!evs.empty()) {
      SupportedTypes::TypeSize o;
      for (const auto& ev : evs) {
        const auto flag = SupportedTypes::getTypeFlag(ev.type);
        if (flag == SupportedTypes::SCALAR) {
          if (ev.arraySize == 1u) {
            out << "this->d" << ev.name << " = ZMATextvars_tdt[ZMATevs_pos["
                << o << "]]-ZMATextvars_t[ZMATevs_pos[" << o << "]];\n";
            o += this->getTypeSize(ev.type, 1u);
          } else {
            if (mb.useDynamicallyAllocatedVector(ev.arraySize)) {
              out << "this->d" << ev.name << ".resize(" << ev.arraySize
                  << ");\n";
              out << "for(unsigned short idx=0;idx!=" << ev.arraySize
                  << ";++idx){\n";
              out << "this->d" << ev.name
                  << "[idx] = ZMATextvars_td-[ZMATevs_pos[" << o
                  << "+idx]]-ZMATextvars_t[ZMATevs_pos[" << o << "+idx]];\n";
              out << "}\n";
              o += this->getTypeSize(ev.type, ev.arraySize);
            } else {
              for (unsigned short i = 0; i != ev.arraySize; ++i) {
                out << "this->d" << ev.name << "[" << i
                    << "] = ZMATextvars_tdt[ZMATevs_pos[" << o
                    << "]]-ZMATextvars_t[ZMATevs_pos[" << o << "]];\n";
                o += this->getTypeSize(ev.type, 1u);
              }
            }
          }
        } else {
          throw_if(true,
                   "unsupported material properties type (only scalar "
                   "supported yet)");
        }
      }
    }
    out << "this->dT = ZMATextvars_tdt[ZMATT_pos]-ZMATextvars_t[ZMATT_pos];\n"
        << "}\n\n";
  }

  void ZMATInterface::writeBehaviourDataMainVariablesSetters(
      std::ostream&, const BehaviourDescription&) const {
    // handled by the constructors
  }

  void ZMATInterface::writeIntegrationDataMainVariablesSetters(
      std::ostream&, const BehaviourDescription&) const {
    // handled by the constructors
  }

  void ZMATInterface::writeBehaviourInitializeFunctions(
      std::ostream&, const BehaviourDescription&, const Hypothesis) const {
  }  // end of writeBehaviourInitializeFunctions

  void ZMATInterface::writeBehaviourPostProcessings(std::ostream&,
                                                    const BehaviourDescription&,
                                                    const Hypothesis) const {
  }  // end of writeBehaviourPostProcessings

  void ZMATInterface::endTreatment(const BehaviourDescription& mb,
                                   const FileDescription& fd) const {
    using namespace std;
    using namespace tfel::material;
    constexpr std::array<Hypothesis, 3u> hypotheses = {
        ModellingHypothesis::TRIDIMENSIONAL,
        ModellingHypothesis::GENERALISEDPLANESTRAIN,
        ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN};
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "ZMATInterface::endTreatment: " + m);
    };
    this->checkIfTemperatureIsDefinedAsTheFirstExternalStateVariable(mb);
    tfel::system::systemCall::mkdir("include/MFront");
    tfel::system::systemCall::mkdir("include/MFront/ZMAT");
    tfel::system::systemCall::mkdir("zmat");
    const auto name = mb.getLibrary() + mb.getClassName();
    const auto headerFileName = "ZMAT" + name + ".hxx";
    const auto srcFileName = "ZMAT" + name + ".cxx";
    const auto& all_mp_names = getAllMaterialPropertiesNames(mb);
    const unsigned short nbh =
        (mb.isModellingHypothesisSupported(ModellingHypothesis::TRIDIMENSIONAL)
             ? 1
             : 0) +
        (mb.isModellingHypothesisSupported(
             ModellingHypothesis::GENERALISEDPLANESTRAIN)
             ? 1
             : 0) +
        (mb.isModellingHypothesisSupported(
             ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)
             ? 1
             : 0);
    throw_if(nbh == 0, "no suitable modelling hypothesis supported");
    std::ofstream out("include/MFront/ZMAT/" + headerFileName);
    throw_if(!out, "could not open header file '" + headerFileName + "'");
    out << "/*!\n"
        << "* \\file   " << headerFileName << '\n'
        << "* \\brief  This file declares the ZMAT interface for the "
        << mb.getClassName() << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n"
        << "#ifndef MFRONT_ZMAT_" << makeUpperCase(mb.getClassName())
        << "_HXX\n"
        << "#define MFRONT_ZMAT_" << makeUpperCase(mb.getClassName())
        << "_HXX\n\n"
        << "#include\"Behavior.h\"\n";
    writeZMATUndefs(out);
    out << '\n'
        << "Z_START_NAMESPACE;\n"
        << '\n'
        << "/*!\n"
        << "* ZMAT Interface to the " << mb.getClassName() << " behaviour"
        << "*/\n"
        << "struct ZMAT" << mb.getClassName() << " final\n"
        << ": public BEHAVIOR\n"
        << "{\n"
        << "//! constructor;\n"
        << "ZMAT" << mb.getClassName() << "();\n"
        << "/*!\n"
        << " * \\brief integrate the beahviour over a time step\n"
        << " * \\param[in,out] file  : input file \n"
        << " * \\param[in]     dim   : dimension (used by the BEHAVIOUR "
           "class)\n"
        << " * \\param[in]     integ : pointer to the local integration\n"
        << " */\n"
        << "void initialize(ASCII_FILE&,int,LOCAL_INTEGRATION*) override;\n"
        << "/*!\n"
        << " * \\brief integrate the beahviour over a time step\n"
        << " * \\param[in,out] mat        : material properties\n"
        << " * \\param[out]    delta_grad : increment of driving variables\n"
        << " * \\param[out]    tg_matrix  : tangent operator\n"
        << " * \\param[int]    flags      : integration option\n"
        << " * \\return NULL on success\n"
        << " */\n"
        << "INTEGRATION_RESULT*\n"
        << "integrate(MAT_DATA&,const VECTOR&,MATRIX*&,int) override;\n";
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      out << "/*!\n"
          << " * \\brief return the elasticity matrix\n"
          << " * \\param[in] mdat : material data\n"
          << " */\n"
          << "SMATRIX get_elasticity_matrix(MAT_DATA&,double) override;\n";
    }
    out << "//! destructor\n"
        << "virtual ~ZMAT" << mb.getClassName() << "();\n"
        << '\n'
        << "protected:\n"
        << '\n';
    if (!all_mp_names.empty()) {
      for (const auto h : hypotheses) {
        if (mb.isModellingHypothesisSupported(h)) {
          out << "/*!\n";
          out << " * \\brief initialize material properties\n";
          out << " * \\param[in,out] file    : input file \n";
          out << " */\n";
          out << "virtual void initializeMaterialProperties"
              << getSpaceDimensionSuffix(h) << "(ASCII_FILE&);\n";
        }
      }
    }
    for (const auto h : hypotheses) {
      if (mb.isModellingHypothesisSupported(h)) {
        out << "/*!\n";
        out << " * \\brief initialize parameters\n";
        out << " * \\param[in,out] file    : input file \n";
        out << " */\n";
        out << "virtual void initializeParameters" << getSpaceDimensionSuffix(h)
            << "(ASCII_FILE&);\n";
      }
    }
    for (const auto h : hypotheses) {
      if (mb.isModellingHypothesisSupported(h)) {
        out << "/*!\n";
        out << " * \\brief call the mfront beahviour in "
            << ModellingHypothesis::toString(h) << '\n';
        out << " * \\param[in,out] mat     : material properties\n";
        out << " * \\param[out] delta_grad : increment of driving variables\n";
        out << " * \\param[out] tg_matrix  : tangent operator\n";
        out << " * \\param[int] flags      : integration option\n";
        out << " * \\return NULL on success\n";
        out << " */\n";
        out << "INTEGRATION_RESULT*\n";
        out << "callMFrontBehaviour" << getSpaceDimensionSuffix(h)
            << "(MAT_DATA&,const VECTOR&,MATRIX*&,int);\n";
      }
    }
    out << "//! stresses\n"
        << "TENSOR2_FLUX sig;\n";
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR:
        out << "//! strains\n"
            << "TENSOR2_GRAD eto;\n";
        break;
      case BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR:
        out << "//! deformation gradient\n"
            << "TENSOR2_GRAD F;\n";
        break;
      default:
        throw_if(true, "unsupported behaviour type");
    }
    if ((mb.getBehaviourType() ==
         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
        (mb.getBehaviourType() ==
         BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)) {
      out << "//! tangent operator\n"
          << "MATRIX tg_mat; \n";
    }
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      out << "//! elasticity matrix\n"
          << "AUTO_PTR<ELASTICITY> elasticity; \n";
    }
    out << "//! material properties\n"
        << "ARRAY<COEFF> mprops; \n";
    const auto& svs = getAllStateVariables(mb);
    if (!svs.empty()) {
      for (const auto& sv : svs) {
        out << "//! '" << sv.name << "' state variable\n";
        const auto flag = SupportedTypes::getTypeFlag(sv.type);
        if (flag == SupportedTypes::SCALAR) {
          if (sv.arraySize == 1u) {
            out << "SCALAR_VINT " << sv.name << ";\n";
          } else {
            out << "VECTOR_VINT " << sv.name << ";\n";
          }
        } else if ((flag == SupportedTypes::STENSOR) ||
                   (flag == SupportedTypes::TENSOR)) {
          if (sv.arraySize == 1u) {
            out << "TENSOR2_VINT " << sv.name << ";\n";
          } else {
            out << "ARRAY<TENSOR2_VINT> " << sv.name << ";\n";
          }
        } else {
          throw_if(true, "unsupported state variable type");
        }
      }
    }
    out << "//! external state variables\n"
        << "LIST<EXTERNAL_PARAM*> local_ep_list;\n"
        << "LIST<EXTERNAL_PARAM*>* zbb_keep_ep;\n"
        << "//! external state variables positions\n"
        << "ARRAY<int> evs_positions;\n"
        << "//! position de la température\n"
        << "int temperature_position;\n"
        << "//! local clock\n"
        << "CLOCK  local_clock; \n"
        << "//! out of bounds policy\n"
        << "tfel::material::OutOfBoundsPolicy obp;\n"
        << "};\n\n"
        << "Z_END_NAMESPACE;\n\n"
        << "#endif /* MFRONT_ZMAT_" << makeUpperCase(mb.getClassName())
        << "_HXX*/\n";
    out.close();
    out.open("src/" + srcFileName);
    throw_if(!out, "could not open src file '" + srcFileName + "'");
    // writing source file
    out << "/*!\n"
        << "* \\file   " << srcFileName << '\n'
        << "* \\brief  This file declares the ZMAT interface for the "
        << mb.getClassName() << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n"
        << '\n'
        << "#include<string>\n"
        << "#include<vector>\n"
        << "#include<algorithm>\n"
        << '\n'
        << "#include\"External_parameter.h\"\n"
        << "#include\"Verbose.h\"\n";
    writeZMATUndefs(out);
    out << '\n'
        << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n"
        << "#include\"MFront/ZMAT/" << headerFileName << "\"\n"
        << '\n';
    mfront::writeExportDirectives(out, false);
    const auto zcn = "ZMAT" + mb.getClassName();
    writeFileDescriptionSymbols(out, zcn, fd);
    writeBuildIdentifierSymbol(out, zcn, mb);
    writeEntryPointSymbol(out, zcn);
    writeTFELVersionSymbol(out, zcn);
    writeUnitSystemSymbol(out, zcn, mb);
    writeMaterialSymbol(out, zcn, mb.getMaterialName());
    exportStringSymbol(out, zcn + "_mfront_interface",
                       this->getInterfaceName());
    out << '\n'
        << "Z_START_NAMESPACE;\n"
        << '\n'
        << zcn << "::" << zcn << "()\n"
        << ": obp(tfel::material::None)\n"
        << "{\n"
        << "this->zbb_keep_ep = &this->local_ep_list;\n"
        << "#ifdef _WIN64\n"
        << "ZMAT_GLOBAL_STORAGE::zmat_once();\n"
        << "ZMAT_GLOBAL_STORAGE& zmat_globals = *thread_zmat_globals;\n"
        << "#endif\n"
        << "} // end of ZMAT" << mb.getClassName() << "::ZMAT"
        << mb.getClassName() << "()\n"
        << '\n'
        << "void\n"
        << "ZMAT" << mb.getClassName()
        << "::initialize(ASCII_FILE& file,int dim,LOCAL_INTEGRATION* integ){\n"
        << "BEHAVIOR::initialize(file, dim,integ);\n"
        << "using namespace std;\n"
        << "int keep_verbose = ZSET::Verbose;\n"
        << "this->coord.resize(dim);\n"
        << "this->sig.initialize(this,\"sig\",this->tsz(),1);\n";
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR:
        out << "this->eto.initialize(this,\"eto\",this->tsz(),1);\n";
        break;
      case BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR:
        out << "this->F.initialize(this,\"F\",this->utsz(),1);\n";
        break;
      default:
        throw_if(true, "unsupported behaviour type");
    }
    if ((mb.getBehaviourType() ==
         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
        (mb.getBehaviourType() ==
         BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)) {
      out << "this->tg_mat.resize(this->tsz(), this->tsz());\n";
    }
    out << "// initialisation dispatch\n"
        << "if(this->tsz()==6){\n";
    this->writeBehaviourInitialisation(out, mb,
                                       ModellingHypothesis::TRIDIMENSIONAL);
    out << "} else if(this->tsz()==4){\n";
    this->writeBehaviourInitialisation(
        out, mb, ModellingHypothesis::GENERALISEDPLANESTRAIN);
    out << "} else if(this->tsz()==3){\n";
    this->writeBehaviourInitialisation(
        out, mb, ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN);
    out << "} else {\n"
        << "ERROR(\"Invalid tensor size\");\n"
        << "}\n"
        << "this->temperature_position = "
           "EXTERNAL_PARAM::rank_of_nodal_ip(\"temperature\");\n"
        << "if(this->temperature_position==-1){;\n"
        << "INPUT_ERROR(\"temperature is not defined\");\n"
        << "}\n";
    if (!all_mp_names.empty()) {
      out << "// check that all material properties were initialised\n"
          << "for(int pc=0;pc!=this->mprops.size();++pc){\n"
          << "if(!this->mprops[pc].ok()){\n"
          << "INPUT_ERROR(\"Some material properties were not initialised\");\n"
          << "}\n"
          << "}\n";
    }
    out << "ZSET::Verbose = keep_verbose;\n"
        << "} // end of ZMAT" << mb.getClassName() << "::initialize\n\n";
    if (!all_mp_names.empty()) {
      for (const auto h : hypotheses) {
        if (mb.isModellingHypothesisSupported(h)) {
          this->writeMaterialPropertiesInitialisation(out, mb, h);
        }
      }
    }
    for (const auto h : hypotheses) {
      if (mb.isModellingHypothesisSupported(h)) {
        if (!areParametersTreatedAsStaticVariables(mb)) {
          this->writeParametersInitialisation(out, mb, h);
        }
      }
    }
    out << "INTEGRATION_RESULT*\n"
        << "ZMAT" << mb.getClassName() << "::integrate(MAT_DATA& mdat,\n"
        << "const VECTOR& delta_grad,\n"
        << "MATRIX*& tg_matrix,\n"
        << "int flags){\n"
        << "int keep_verbose  = ZSET::Verbose; \n"
        << "auto* const keep_clock = "
           "ZSET::stored_thread_zbase_globals->ptr()->active_clock; \n"
        << "tg_matrix = &(this->tg_mat);\n"
        << "this->set_var_aux_to_var_aux_ini();\n"
        << "this->set_var_int_to_var_int_ini();\n"
        << "auto* const ep_save = &EXTERNAL_PARAM::Get_EP_list();\n"
        << "EXTERNAL_PARAM::set_EP_list(this->zbb_keep_ep);\n"
        << "if(!this->curr_ext_param){\n"
        << "this->curr_ext_param = *mdat.param_set();\n"
        << "}\n"
        << "this->calc_local_coefs();\n"
        << "INTEGRATION_RESULT* r = NULL;\n"
        << "try{\n"
        << "if(this->tsz()==6){\n";
    if (!mb.isModellingHypothesisSupported(
            ModellingHypothesis::TRIDIMENSIONAL)) {
      out << "ERROR(\"Computations using the 'Tridimensional'\" \n"
          << "\"modelling hypothesis are not supported by the \"\n"
          << "\"'" << mb.getClassName() << "' behaviour\");\n";
    } else {
      out << "r=this->callMFrontBehaviour3D"
          << "(mdat,delta_grad,tg_matrix,flags);\n";
    }
    out << "} else if(this->tsz()==4){\n";
    if (!mb.isModellingHypothesisSupported(
            ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      out << "ERROR(\"Computations using the 'GeneralisedPlaneStrain'\" \n"
          << "\"modelling hypothesis are not supported by the \"\n"
          << "\"'" << mb.getClassName() << "' behaviour\");\n";
    } else {
      out << "r=this->callMFrontBehaviour2D"
          << "(mdat,delta_grad,tg_matrix,flags);\n";
    }
    out << "} else if(this->tsz()==3){\n";
    if (!mb.isModellingHypothesisSupported(
            ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)) {
      out << "ERROR(\"Computations using the "
             "'AxisymmetricalGeneralisedPlaneStrain'\" \n"
          << "\"modelling hypothesis are not supported by the \"\n"
          << "\"'" << mb.getClassName() << "' behaviour\");\n";
    } else {
      out << "r=this->callMFrontBehaviour1D"
          << "(mdat,delta_grad,tg_matrix,flags);\n";
    }
    out << "} else {\n"
        << "ERROR(\"Invalid tensor size\");\n"
        << "}\n"
        << "}\n"
        << "catch(std::exception& e){\n"
        << "static INTEGRATION_RESULT bad_result;\n"
        << "*(*ZSET::stored_thread_zbase_globals).ptr()->out_msg << e.what() "
           "<< endl;\n"
        << "bad_result.set_error(INTEGRATION_RESULT::UNDEFINED_BEHAVIOR);\n"
        << "ZSET::Verbose = keep_verbose; \n"
        << "ZSET::stored_thread_zbase_globals->ptr()->active_clock = "
           "keep_clock; \n"
        << "return &bad_result;\n"
        << "}\n"
        << "catch(...){\n"
        << "static INTEGRATION_RESULT bad_result;\n"
        << "bad_result.set_error(INTEGRATION_RESULT::UNDEFINED_BEHAVIOR);\n"
        << "ZSET::Verbose = keep_verbose; \n"
        << "ZSET::stored_thread_zbase_globals->ptr()->active_clock = "
           "keep_clock; \n"
        << "return &bad_result;\n"
        << "}\n"
        << "if(r!=NULL){\n"
        << "ZSET::Verbose = keep_verbose; \n"
        << "ZSET::stored_thread_zbase_globals->ptr()->active_clock = "
           "keep_clock; \n"
        << "return r;\n"
        << "}\n"
        << "this->update_var_aux();\n"
        << "this->zbb_keep_ep = &EXTERNAL_PARAM::Get_EP_list();\n"
        << "EXTERNAL_PARAM::set_EP_list(ep_save);\n"
        << "ZSET::Verbose = keep_verbose;\n"
        << "ZSET::stored_thread_zbase_globals->ptr()->active_clock = "
           "keep_clock;\n"
        << "return NULL;\n"
        << "} // end of ZMAT" << mb.getClassName() << "::integrate\n\n";
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      out << "SMATRIX\n"
          << "ZMAT" << mb.getClassName()
          << "::get_elasticity_matrix(MAT_DATA& mdat,double){\n"
          << "elasticity->attach_all(mdat);\n"
          << "elasticity->calc_coef();\n"
          << "return SMATRIX(*elasticity());\n"
          << "} // end of ZMAT" << mb.getClassName()
          << "::get_elasticity_matrix\n\n";
    }
    for (const auto h : hypotheses) {
      if (mb.isModellingHypothesisSupported(h)) {
        this->writeCallMFrontBehaviour(out, mb, h);
      }
    }
    out << '\n'
        << "ZMAT" << mb.getClassName() << "::~ZMAT" << mb.getClassName()
        << "(){\n"
        << "} // end of ZMAT" << mb.getClassName() << "::~ZMAT"
        << mb.getClassName() << "\n\n"
        << "BEHAVIOR_READER(ZMAT" << mb.getClassName() << ","
        << mb.getClassName() << ")\n\n"
        << "Z_END_NAMESPACE;\n";
    out.close();
    out.open("zmat/" + name + ".inp");
    if (!out) {
      return;
    }
    for (const auto h : hypotheses) {
      if (mb.isModellingHypothesisSupported(h)) {
        const auto mps = getAllMaterialPropertiesNames(mb);
        const auto& d = mb.getBehaviourData(h);
        const auto& ivs = d.getPersistentVariables();
        auto evs = d.getExternalStateVariables();
        const auto& params = d.getParameters();
        // removing the temperature
        evs.erase(evs.begin());
        auto print_variable = [&out, &d](const VariableDescription& v) {
          out << "# - " << d.getExternalName(v.name) << " (" << v.type << "): ";
          if (v.description.empty()) {
            out << "no description available\n";
          } else {
            out << v.description << '\n';
          }
        };
        out << "# Behaviour declaration for " << getSpaceDimension(h) << "D\n";
        if (!ivs.empty()) {
          out << "## List of state variables:\n";
          for (const auto& iv : ivs) {
            print_variable(iv);
          }
        }
        out << "## List of external state variables:\n"
            << "# temperature (temperature): the temperature\n";
        for (const auto& ev : evs) {
          print_variable(ev);
        }
        out << "\n***behavior " << name << '\n';
        if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                            false)) {
          out << "**elasticity\n"
              << "<elastic parameters here>";
        }
        if (!mps.empty()) {
          out << "**material_properties\n";
          for (const auto& mp : mps) {
            out << "  " << mp << " <value>\n";
          }
        }
        if (!params.empty()) {
          out << "# list of parameters, which are *optional*\n";
          out << "**parameters\n";
          for (const auto& p : params) {
            const auto pn = d.getExternalName(p.name);
            out << "  " << pn << " ";
            if (p.type == "int") {
              throw_if(p.arraySize != 1,
                       "unsupported array size for parameter '" + p.name + "'");
              out << d.getIntegerParameterDefaultValue(p.name) << '\n';
            } else if (p.type == "ushort") {
              throw_if(p.arraySize != 1,
                       "unsupported array size for parameter '" + p.name + "'");
              out << d.getUnsignedShortParameterDefaultValue(p.name) << '\n';
            } else {
              const auto f = SupportedTypes::getTypeFlag(p.type);
              throw_if(f != SupportedTypes::SCALAR,
                       "unsupported parameter type '" + p.type + "'");
              const auto prec = out.precision();
              out.precision(14);
              if (p.arraySize == 1) {
                out << d.getFloattingPointParameterDefaultValue(p.name) << '\n';
              } else {
                for (unsigned short i = 0; i != p.arraySize;) {
                  out << d.getFloattingPointParameterDefaultValue(p.name, i);
                  if (++i != p.arraySize) {
                    out << " ";
                  }
                }
                out << '\n';
              }
              out.precision(prec);
            }
          }
        }
        out << "# Available output of bounds policies: None Strict Warning\n"
            << "**out_of_bounds_policy None\n"
            << "***return\n\n";
      }
    }
  }  // end of ZMATInterface::endTreatment

  void ZMATInterface::writeBehaviourInitialisation(
      std::ostream& out,
      const BehaviourDescription& mb,
      const ZMATInterface::Hypothesis h) const {
    using namespace std;
    if (!mb.isModellingHypothesisSupported(h)) {
      out << "ERROR(\"Computations using the "
          << ModellingHypothesis::toString(h) << "\" \n"
          << "\"modelling hypothesis are not supported by the \"\n"
          << "\"'" << mb.getClassName() << "' behaviour\");\n";
      return;
    }
    const auto& d = mb.getBehaviourData(h);
    const auto& mps = d.getMaterialProperties();
    const auto& isvs = d.getPersistentVariables();
    auto esvs = d.getExternalStateVariables();
    const auto dime = getSpaceDimension(h);
    const auto mps_size = mps.getTypeSize().getValueForDimension(dime);
    // removing the temperature
    esvs.erase(esvs.begin());
    if (!mps.empty()) {
      out << "this->mprops.resize(" << mps_size << ");\n";
    }
    for (const auto& isv : isvs) {
      const auto flag = SupportedTypes::getTypeFlag(isv.type);
      const auto& name = d.getExternalName(isv.name);
      if (flag == SupportedTypes::SCALAR) {
        out << "this->" << isv.name << ".initialize(this,\"" << name << "\","
            << isv.arraySize << ",1);\n";
      } else if (flag == SupportedTypes::STENSOR) {
        if (isv.arraySize == 1u) {
          out << "this->" << isv.name << ".initialize(this,\"" << name
              << "\",this->tsz(),1);\n";
        } else {
          for (unsigned short i = 0; i != isv.arraySize; ++i) {
            out << "this->" << isv.name << "[" << i << "].initialize(this,\""
                << name << std::to_string(i) << "\",this->tsz(),1);\n";
          }
        }
      } else if (flag == SupportedTypes::TENSOR) {
        if (isv.arraySize == 1u) {
          out << "this->" << isv.name << ".initialize(this,\"" << name
              << "\",this->utsz(),1);\n";
        } else {
          for (unsigned short i = 0; i != isv.arraySize; ++i) {
            out << "this->" << isv.name << "[" << i << "].initialize(this,\""
                << name << std::to_string(i) << "\",this->uts(),1);\n";
          }
        }
      } else {
        tfel::raise(
            "ZMATInterface::writeBehaviourInitialisation: "
            "unsupported state variable type");
      }
    }
    out << "for(;;){\n"
        // << "if(!file.ok){\n"
        // << "INPUT_ERROR(\"file parsing failed\");\n"
        // << "}\n"
        << "STRING str=file.getSTRING();\n"
        << "if(this->base_read(str,file)){\n";
    const auto& all_mp_names = getAllMaterialPropertiesNames(mb);
    if (!all_mp_names.empty()) {
      out << "} else "
             "if((str==\"**model_coef\")||(str==\"**material_properties\")){\n"
          << "this->initializeMaterialProperties" << getSpaceDimensionSuffix(h)
          << "(file);\n";
    }
    out << "} else if(str==\"**parameters\"){\n"
        << "this->initializeParameters" << getSpaceDimensionSuffix(h)
        << "(file);\n"
        << "} else if(str==\"**out_of_bounds_policy\"){\n"
        << "STRING policy=file.getSTRING();\n"
        << "if(policy==\"None\"){\n"
        << "this->obp=tfel::material::None;\n"
        << "} else if(policy==\"Strict\"){\n"
        << "this->obp=tfel::material::Strict;\n"
        << "} else if(policy==\"Warning\"){\n"
        << "this->obp=tfel::material::Warning;\n"
        << "} else {\n"
        << "INPUT_ERROR(\"unknown policy '\"+policy+\"'\");\n"
        << "}\n";
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      out << "} else if((str==\"**elasticity\")||(str==\"**ELASTICITY\")){\n"
          << "if(!this->elasticity.Null()){\n"
          << "INPUT_ERROR(\"elasticity already defined\");\n"
          << "}\n"
          << "this->elasticity=ELASTICITY::read(file,this);\n";
    }
    out << "} else if (str.start_with(\"***\")){\n"
        << "file.back();\n"
        << "break;\n"
        << "} else {\n"
        << "INPUT_ERROR(\"Invalid keyword '\"+str+\"'\");\n"
        << "}\n"
        << "}\n";
    if (!esvs.empty()) {
      const int ext_size = esvs.getTypeSize().getValueForDimension(dime);
      out << "evs_positions.resize(" << ext_size << ");\n";
      int i = 0;
      for (const auto& v : esvs) {
        const auto& name = d.getExternalName(v.name);
        if (v.arraySize == 1u) {
          out << "this->evs_positions[" << i << "] = "
              << "EXTERNAL_PARAM::rank_of_nodal_ip(\"" << name << "\");\n";
          out << "if(this->evs_positions[" << i << "]==-1){;\n";
          out << "INPUT_ERROR(\"'" << name << "' is not defined\");\n";
          out << "}\n";
          ++i;
        } else {
          for (unsigned short n = 0; n != v.arraySize; ++n, ++i) {
            out << "this->evs_positions[" << i << "] = "
                << "EXTERNAL_PARAM::rank_of_nodal_ip(\"" << name << "[" << n
                << "]\");\n";
            out << "if(this->evs_positions[" << i << "]==-1){;\n";
            out << "INPUT_ERROR(\"'" << name << "[" << n
                << "]' is not defined\");\n";
            out << "}\n";
          }
        }
      }
    }
  }

  void ZMATInterface::writeParametersInitialisation(
      std::ostream& out,
      const BehaviourDescription& mb,
      const ZMATInterface::Hypothesis h) const {
    using namespace std;
    const auto& d = mb.getBehaviourData(h);
    out << "void\n"
        << "ZMAT" << mb.getClassName() << "::initializeParameters"
        << getSpaceDimensionSuffix(h) << "(ASCII_FILE& file){\n"
        << "for(;;){\n"
        << "STRING str=file.getSTRING();\n"
        << "if(str[0]=='*'){\n"
        << "file.back();\n"
        << "break;\n";
    if (!areParametersTreatedAsStaticVariables(mb)) {
      const auto& params = d.getParameters();
      const auto pnames = d.getExternalNames(params);
      auto p = params.begin();
      auto pn = pnames.begin();
      for (; p != params.end(); ++p, ++pn) {
        if ((p->type == "int") || (p->type == "ushort")) {
          out << "} else if(str==\"" << *pn << "\"){\n";
          if (p->type == "int") {
            out << "const int value=file.getint();\n";
          } else {
            out << "const unsigned short value=static_cast<unsigned "
                   "short>(file.getint());\n";
          }
          if (mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                              p->name)) {
            out << "tfel::material::" << mb.getClassName()
                << "ParametersInitializer::get()." << p->name << " = value;\n";
          } else {
            out << "tfel::material::" << mb.getClassName()
                << ModellingHypothesis::toString(h)
                << "ParametersInitializer::get()." << p->name << " = value;\n";
          }
        } else {
          const auto f = SupportedTypes::getTypeFlag(p->type);
          tfel::raise_if(f != SupportedTypes::SCALAR,
                         "ZMATInterface::writeParametersInitialisation: "
                         "unsupported type '" +
                             p->type + "' for parameter '" + p->name + "'");
          if (p->arraySize == 1u) {
            out << "} else if(str==\"" << *pn << "\"){\n";
            out << "const double value=file.getdouble();\n";
            if (mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                p->name)) {
              out << "tfel::material::" << mb.getClassName()
                  << "ParametersInitializer::get()." << p->name
                  << " = value;\n";
            } else {
              out << "tfel::material::" << mb.getClassName()
                  << ModellingHypothesis::toString(h)
                  << "ParametersInitializer::get()." << p->name
                  << " = value;\n";
            }
          } else {
            for (unsigned short i = 0; i != p->arraySize; ++i) {
              out << "} else if(str==\"" << *pn << "[" << i << "]\"){\n";
              out << "const double value=file.getdouble();\n";
              if (mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                  p->name)) {
                out << "tfel::material::" << mb.getClassName()
                    << "ParametersInitializer::get()." << p->name << "[" << i
                    << "] = value;\n";
              } else {
                out << "tfel::material::" << mb.getClassName()
                    << ModellingHypothesis::toString(h)
                    << "ParametersInitializer::get()." << p->name << "[" << i
                    << "] = value;\n";
              }
            }
          }
        }
      }
    }
    out << "} else {\n"
        << "INPUT_ERROR(\"invalid parameter name '\"+str+\"'\");\n"
        << "}\n"
        << "}\n"
        << "}\n\n";
  }  // end of ZMATInterface::writeParametersInitialisation

  void ZMATInterface::writeMaterialPropertiesInitialisation(
      std::ostream& out,
      const BehaviourDescription& mb,
      const ZMATInterface::Hypothesis h) const {
    using namespace std;
    const auto& d = mb.getBehaviourData(h);
    const auto& mps = d.getMaterialProperties();
    const auto& all_mp_names = getAllMaterialPropertiesNames(mb);
    const auto mpnames = d.getExternalNames(mps);
    const unsigned short nbh =
        (mb.isModellingHypothesisSupported(ModellingHypothesis::TRIDIMENSIONAL)
             ? 1
             : 0) +
        (mb.isModellingHypothesisSupported(
             ModellingHypothesis::GENERALISEDPLANESTRAIN)
             ? 1
             : 0) +
        (mb.isModellingHypothesisSupported(
             ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)
             ? 1
             : 0);
    const unsigned short dime = getSpaceDimension(h);
    const int mps_size = mps.getTypeSize().getValueForDimension(dime);
    out << "void\n"
        << "ZMAT" << mb.getClassName() << "::initializeMaterialProperties"
        << getSpaceDimensionSuffix(h) << "(ASCII_FILE& file){\n"
        << "using std::find;\n";
    if (nbh != 1) {
      if (!all_mp_names.empty()) {
        out << "const STRING all_mp_names[" << all_mp_names.size() << "] = ";
        writeArray(out, all_mp_names);
      }
    }
    if (!mpnames.empty()) {
      out << "const STRING mp_names" << getSpaceDimensionSuffix(h) << "["
          << mpnames.size() << "] = ";
      writeArray(out, mpnames);
    }
    out << "for(;;){\n"
        << "STRING str=file.getSTRING();\n"
        << "if(str[0]=='*'){\n"
        << "file.back();\n"
        << "break;\n"
        << "}\n";
    if (nbh != 1u) {
      out << "if(find(all_mp_names,all_mp_names+" << all_mp_names.size()
          << ",str)==all_mp_names+" << all_mp_names.size() << "){\n"
          << "INPUT_ERROR(\"No material property named '\"+str+\"'\");\n"
          << "}\n";
    }
    out << "const STRING * const pmat = find(mp_names"
        << getSpaceDimensionSuffix(h) << ",mp_names"
        << getSpaceDimensionSuffix(h) << "+" << mps_size << ",str);\n";
    if (nbh == 1u) {
      out << "if(pmat==mp_names" << getSpaceDimensionSuffix(h) << "+"
          << mps_size << "){\n"
          << "INPUT_ERROR(\"No material property named '\"+str+\"'\");\n"
          << "}\n";
    }
    out << "if(this->mprops[pmat-mp_names" << getSpaceDimensionSuffix(h)
        << "].ok()){\n"
        << "INPUT_ERROR(\"material property '\"+str+\"' already defined\");\n"
        << "}\n"
        << "this->mprops[pmat-mp_names" << getSpaceDimensionSuffix(h)
        << "].read(str,file,this);\n"
        << "}\n"
        << "} // end of ZMAT" << mb.getClassName()
        << "::initializeMaterialProperties" << getSpaceDimensionSuffix(h)
        << "\n\n";
  }

  void ZMATInterface::writeCallMFrontBehaviour(
      std::ostream& out,
      const BehaviourDescription& mb,
      const ZMATInterface::Hypothesis h) const {
    using namespace std;
    out << "INTEGRATION_RESULT*\n"
        << "ZMAT" << mb.getClassName() << "::callMFrontBehaviour"
        << getSpaceDimensionSuffix(h) << "(MAT_DATA& mdat,\n"
        << "const VECTOR& delta_grad,\n"
        << "MATRIX*& tg_matrix,\n"
        << "int flags){\n"
        << "typedef tfel::material::ModellingHypothesis ModellingHypothesis;\n"
        << "typedef tfel::material::" << mb.getClassName()
        << "<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h)
        << ",double,false> " << mb.getClassName() << ";\n"
        << "using tfel::math::st2tost2;\n";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      out << "// strain and strain increment\n"
          << "double stran[" << getStensorSize(h) << "];\n"
          << "double dstran[" << getStensorSize(h) << "];\n"
          << "stran[0] = this->eto[0]-delta_grad[0];\n"
          << "stran[1] = this->eto[1]-delta_grad[1];\n"
          << "stran[2] = this->eto[2]-delta_grad[2];\n";
      if ((h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
          (h == ModellingHypothesis::TRIDIMENSIONAL)) {
        out << "stran[3] = this->eto[3]-delta_grad[3];\n";
        if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          out << "stran[4] = this->eto[4]-delta_grad[4];\n";
          out << "stran[5] = this->eto[5]-delta_grad[5];\n";
        }
      }
      out << "dstran[0] = delta_grad[0];\n"
          << "dstran[1] = delta_grad[1];\n"
          << "dstran[2] = delta_grad[2];\n";
      if ((h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
          (h == ModellingHypothesis::TRIDIMENSIONAL)) {
        out << "dstran[3] = delta_grad[3];\n";
        if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          out << "dstran[4] = delta_grad[4];\n";
          out << "dstran[5] = delta_grad[5];\n";
        }
      }
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "// deformation gradients\n"
          << "double F0[" << getTensorSize(h) << "];\n"
          << "F0[0] = this->F[0]-delta_grad[0];\n"
          << "F0[1] = this->F[1]-delta_grad[1];\n"
          << "F0[2] = this->F[2]-delta_grad[2];\n";
      if ((h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
          (h == ModellingHypothesis::TRIDIMENSIONAL)) {
        out << "F0[3] = this->F[3]-delta_grad[3];\n";
        out << "F0[4] = this->F[4]-delta_grad[4];\n";
        if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          out << "F0[5] = this->F[5]-delta_grad[5];\n";
          out << "F0[6] = this->F[6]-delta_grad[6];\n";
          out << "F0[7] = this->F[7]-delta_grad[7];\n";
          out << "F0[8] = this->F[8]-delta_grad[8];\n";
        }
      }
    } else {
      tfel::raise(
          "ZMATInterface::writeCallMFrontBehaviour : "
          "unsupported behaviour type");
    }
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR:
        out << mb.getClassName() << "::SMFlag smflag = " << mb.getClassName()
            << "::STANDARDTANGENTOPERATOR;\n";
        break;
      case BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR:
        out << mb.getClassName() << "::SMFlag smflag = " << mb.getClassName()
            << "::C_TRUESDELL;\n";
        break;
      default:
        tfel::raise(
            "ZMATInterface::writeCallMFrontBehaviour : "
            "unsupported behaviour type");
    }
    out << "// tangent operator type\n"
        << mb.getClassName() << "::SMType smtype = " << mb.getClassName()
        << "::NOSTIFFNESSREQUESTED;\n"
        << "if(flags&CALC_TG_MATRIX){\n"
        << "smtype = " << mb.getClassName() << "::CONSISTENTTANGENTOPERATOR;\n"
        << "}\n";
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR:
        out << mb.getClassName() << " b(this->sig,stran,dstran,this->mprops,"
            << "mdat,this->temperature_position,\n"
            << "this->evs_positions,ZSET::stored_thread_zbase_globals->ptr()->"
               "active_clock->get_dtime());\n";
        break;
      case BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR:
        out << mb.getClassName() << " b(this->sig,F0,&(this->F[0]),this->"
            << "mprops,mdat,this->temperature_position,\n"
            << "this->evs_positions,ZSET::stored_thread_zbase_globals->ptr()->"
               "active_clock->get_dtime());\n";
        break;
      default:
        tfel::raise(
            "ZMATInterface::writeCallMFrontBehaviour: "
            "unsupported behaviour type");
    }
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      out << "zmat::ZMATInterface::convert(b.getStiffnessTensor(),this->get_"
             "elasticity_matrix(mdat,1.));\n";
    }
    out << "b.initialize();\n"
        << "if(b.integrate(smflag,smtype)!=" << mb.getClassName()
        << "::SUCCESS){\n"
        << "static INTEGRATION_RESULT bad_result;\n"
        << "bad_result.set_error(INTEGRATION_RESULT::UNDEFINED_BEHAVIOR);\n"
        << "return &bad_result;\n"
        << "}\n"
        << "b.ZMATexportStateData(this->sig,mdat);\n"
        << "if(smtype!=" << mb.getClassName() << "::NOSTIFFNESSREQUESTED){\n";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      out << "zmat::ZMATInterface::convert(*tg_matrix,b.getTangentOperator());"
             "\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "zmat::ZMATInterface::convert(*tg_matrix,b.getTangentOperator()."
             "get<st2tost2<"
          << getSpaceDimension(h) << ",double> >());\n";
    } else {
      tfel::raise(
          "ZMATInterface::writeCallMFrontBehaviour : "
          "unsupported behaviour type");
    }
    out << "}\n"
        << "return nullptr;\n"
        << "} // end of ZMAT" << mb.getClassName() << "::callMFrontBehaviour"
        << getSpaceDimensionSuffix(h) << "\n\n";
  }

  void ZMATInterface::getTargetsDescription(TargetsDescription& d,
                                            const BehaviourDescription& bd) {
    const auto lib = getLibraryName(bd);
    const auto name = bd.getLibrary() + bd.getClassName();
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    auto& l = d.getLibrary(lib);
    insert_if(l.cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
    insert_if(l.cppflags, "$(shell " + tfel_config + " --zmat)");
#pragma message("Linux specific")
    insert_if(l.cppflags, "-DLinux");
    insert_if(l.cppflags, "-DLinux_64");
    insert_if(l.cppflags, "-D_REENTRANT");
    insert_if(l.include_directories,
              "$(shell " + tfel_config + " --include-path)");
    insert_if(d.headers, "MFront/ZMAT/ZMAT" + name + ".hxx");
    insert_if(l.sources, "ZMAT" + name + ".cxx");
    insert_if(l.epts, bd.getClassName());
    insert_if(l.link_directories,
              "$(shell " + tfel_config + " --library-path)");
    insert_if(l.link_libraries, "$(shell " + tfel_config +
                                    " --library-dependency "
                                    "--material --mfront-profiling)");
    // The installation path can have already been set while treating a
    // previous file or retrieved from a previous MFront call (the
    // installation path was stored in `target.lst`).
    // Here, we force to always use the install path given by the user on
    // the command line, if any.
    // If the installation path has not been explicitely given by the user,
    // the `ZEBU_PATH` environment variable is used, if defined.
    auto& ip = l.install_path;
    const auto ipath = getInstallPath();
    if (!ipath.empty()) {
      ip = ipath;
    } else {
      if (std::getenv("ZEBU_PATH") != nullptr) {
        ip = "$(env ZEBU_PATH)";
      }
    }
  }  // end of ZMATInterface::getTargetsDescription

  ZMATInterface::~ZMATInterface() = default;

}  // end of namespace mfront
