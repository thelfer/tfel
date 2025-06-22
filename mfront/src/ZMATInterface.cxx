/*!
 * \file  mfront/src/ZMATInterface.cxx
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

#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/System/System.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"

#include "MFront/DSLUtilities.hxx"
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
            if ((sv.type != v.type) || (sv.arraySize != v.arraySize)) {
              throw(
                  std::runtime_error("getAllStateVariables : "
                                     "inconsistent type for variable '" +
                                     sv.name + "'"));
            }
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
    throw(std::runtime_error("getSpaceDimension: unsupported hypothesis"));
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
  }  // end of UMATInterface::getLibraryName

  std::string ZMATInterface::getName() { return "zmat"; }

  ZMATInterface::ZMATInterface() {}  // end of ZMATInterface::ZMATInterface

  void ZMATInterface::allowDynamicallyAllocatedArrays(const bool b) {
    this->areDynamicallyAllocatedVectorsAllowed_ = b;
  }  // end of ZMATInterface::allowDynamicallyAllocatedArrays

  std::pair<bool, ZMATInterface::tokens_iterator> ZMATInterface::treatKeyword(
      BehaviourDescription&,
      const std::string& key,
      const std::vector<std::string>& i,
      tokens_iterator current,
      const tokens_iterator) {
    if (std::find(i.begin(), i.end(), this->getName()) != i.end()) {
      throw(
          std::runtime_error("ZMATInterface::treatKeyword: "
                             "unsupported keyword '" +
                             key + "'"));
    }
    return {false, current};
  }

  bool ZMATInterface::isModellingHypothesisHandled(
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
    if (h.empty()) {
      throw(std::runtime_error(
          "ZMATInterfaceModellingHypothesesToBeTreated: "
          "no hypotheses selected. This means that the given beahviour "
          "can't be used neither in 'AxisymmetricalGeneralisedPlaneStrain' "
          "nor in 'AxisymmetricalGeneralisedPlaneStress', so it does not "
          "make sense to use the ZMAT interface"));
    }
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
    out << "{\n";
    out << "using namespace tfel::math;\n";
    if (!((mb.getBehaviourType() ==
           BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
          (mb.getBehaviourType() ==
           BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR))) {
      throw(runtime_error(
          "ZMATInterface::exportMechanicalData: "
          "only small or finite strain behaviours are supported"));
    }
    out << "zmat::ZMATInterface::convert(&ZMATsig[0],this->sig);\n";
    if (!persistentVarsHolder.empty()) {
      out << "auto& ZMATstatev = ZMATdata.var_int()[0];\n";
      VariableDescriptionContainer::const_iterator p;
      SupportedTypes::TypeSize currentOffset;
      for (p = persistentVarsHolder.begin(); p != persistentVarsHolder.end();
           ++p) {
        const auto flag = this->getTypeFlag(p->type);
        if (p->arraySize == 1u) {
          if (flag == SupportedTypes::Scalar) {
            if (mb.useQt()) {
              out << "ZMATstatev"
                  << "[" << currentOffset << "] = base_cast(this->" << p->name
                  << ");\n";
            } else {
              out << "ZMATstatev"
                  << "[" << currentOffset << "] = this->" << p->name << ";\n";
            }
          } else if ((flag == SupportedTypes::TVector) ||
                     (flag == SupportedTypes::Stensor) ||
                     (flag == SupportedTypes::Tensor)) {
            out << "zmat::ZMATInterface::convert(&"
                << "ZMATstatev"
                << "[" << currentOffset << "],this->" << p->name << ");\n";
          } else {
            throw(
                runtime_error("SupportedTypes::exportResults: "
                              "internal error, tag unsupported"));
          }
          currentOffset += this->getTypeSize(p->type, p->arraySize);
        } else {
          if (this->useDynamicallyAllocatedVector(p->arraySize)) {
            out << "for(unsigned short idx=0;idx!=" << p->arraySize
                << ";++idx){\n";
            if (flag == SupportedTypes::Scalar) {
              if (mb.useQt()) {
                out << "ZMATstatev"
                    << "[" << currentOffset << "+idx] = common_cast(this->"
                    << p->name << "[idx]);\n";
              } else {
                out << "ZMATstatev"
                    << "[" << currentOffset << "+idx] = this->" << p->name
                    << "[idx];\n";
              }
            } else if ((flag == SupportedTypes::TVector) ||
                       (flag == SupportedTypes::Stensor) ||
                       (flag == SupportedTypes::Tensor)) {
              out << "zmat::ZMATInterface::convert(&ZMATstatev"
                  << "[" << currentOffset << "+idx*StensorSize],this->"
                  << p->name << "[idx]);\n";
            } else {
              throw(
                  runtime_error("SupportedTypes::exportResults: "
                                "internal error, tag unsupported"));
            }
            out << "}\n";
            currentOffset += this->getTypeSize(p->type, p->arraySize);
          } else {
            for (unsigned short i = 0; i != p->arraySize; ++i) {
              if (flag == SupportedTypes::Scalar) {
                if (mb.useQt()) {
                  out << "ZMATstatev"
                      << "[" << currentOffset << "] = common_cast(this->"
                      << p->name << "[" << i << "]);\n";
                } else {
                  out << "ZMATstatev"
                      << "[" << currentOffset << "] = this->" << p->name << "["
                      << i << "];\n";
                }
              } else if ((flag == SupportedTypes::TVector) ||
                         (flag == SupportedTypes::Stensor) ||
                         (flag == SupportedTypes::Tensor)) {
                out << "zmat::ZMATInterface::convert(&"
                    << "ZMATstatev"
                    << "[" << currentOffset << "],this->" << p->name << "[" << i
                    << "]);\n";
              } else {
                throw(
                    runtime_error("SupportedTypes::exportResults: "
                                  "internal error, tag unsupported"));
              }
              currentOffset += this->getTypeSize(p->type, 1u);
            }
          }
        }
      }
    }
    out << "} // end of ZMATexportStateData\n";
    out << endl;
  }

  void ZMATInterface::writeInterfaceSpecificIncludes(
      std::ostream& out, const BehaviourDescription& mb) const {
    out << "#include\"MFront/ZMAT/ZMATInterface.hxx\"\n";
    out << "#include\"Coefficient.h\"\n";
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      out << "#include\"Auto_ptr.h\"\n";
      out << "#include\"Elasticity.h\"\n";
    }
    out << "#include\"Coefficient.h\"\n";
    out << "#include\"Behavior.h\"\n";
    writeZMATUndefs(out);
    out << std::endl;
  }  // end of ZMATInterface::writeInterfaceSpecificIncludes

  void ZMATInterface::writeBehaviourDataConstructor(
      std::ostream& out,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    using namespace std;
    const auto& d = mb.getBehaviourData(h);
    const auto& mps = d.getMaterialProperties();
    const auto& ivs = d.getPersistentVariables();
    const auto& evs = d.getExternalStateVariables();
    if (!((mb.getBehaviourType() ==
           BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
          (mb.getBehaviourType() ==
           BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR))) {
      throw(runtime_error(
          "ZMATInterface::writeBehaviourDataConstructor: "
          "only small or finite strain behaviours are supported"));
    }
    out << "/*!\n";
    out << " * \\brief constructor for the ZMAT interface\n";
    out << " *\n";
    if ((mb.getBehaviourType() ==
         BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
        (mb.getBehaviourType() ==
         BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)) {
      out << " * \\param ZMATsig     : stress at the beginning of the time "
             "step\n";
    }
    if (mb.getBehaviourType() ==
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      out << " \\param ZMATeto     : strain at the beginning of the time "
             "step\n";
    }
    if (mb.getBehaviourType() ==
        BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      out << " \\param ZMATF0      : deformation gradient at the begining of "
             "the time step\n";
    }
    out << " * \\param ZMATmprops  : material properties\n";
    out << " * \\param ZMATdata    : material data handler\n";
    out << " * \\param ZMATT_pos   : position of the temperature\n";
    out << " * \\param ZMATevs_pos : position of the external state "
           "varaibles\n";
    out << " */\n";
    out << mb.getClassName() << "BehaviourData(";
    if ((mb.getBehaviourType() ==
         BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
        (mb.getBehaviourType() ==
         BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)) {
      out << "const ZSET::TENSOR2_FLUX& ZMATsig,\n";
    }
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
        out << "const real * const ZMATeto,\n";
        break;
      case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
        out << "const real * const ZMATF0,\n";
        break;
      default:
        throw(
            runtime_error("ZMATInterface::writeCallMFrontBehaviour: "
                          "unsupported behaviour type"));
    }
    if (!mps.empty()) {
      out << "const ZSET::ARRAY<ZSET::COEFF>& ZMATmprops,\n";
    } else {
      out << "const ZSET::ARRAY<ZSET::COEFF>&,\n";
    }
    out << "const ZSET::MAT_DATA& ZMATdata,\n";
    out << "const int ZMATT_pos,\n";
    if (!evs.empty()) {
      out << "const ZSET::ARRAY<int>& ZMATevs_pos){\n";
    } else {
      out << "const ZSET::ARRAY<int>&){\n";
    }
    if (!ivs.empty()) {
      out << "const auto& ZMATstatev = ZMATdata.var_int_ini()[0];" << endl;
    }
    out << "const ZSET::EXTERNAL_PARAMETER_VECTOR& ZMATextvars_t = "
           "*(ZMATdata.param_set_ini());"
        << endl;
    out << "zmat::ZMATInterface::convert(this->sig,&ZMATsig[0]);\n";
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
        out << "zmat::ZMATInterface::convert(this->eto,&ZMATeto[0]);\n";
        break;
      case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
        out << "zmat::ZMATInterface::convert(this->F0,&ZMATF0[0]);\n";
        break;
      default:
        throw(
            runtime_error("ZMATInterface::writeCallMFrontBehaviour: "
                          "unsupported behaviour type"));
    }
    if (!mps.empty()) {
      SupportedTypes::TypeSize o;
      for (const auto& mp : mps) {
        const SupportedTypes::TypeFlag flag = this->getTypeFlag(mp.type);
        if (flag == SupportedTypes::Scalar) {
          if (mp.arraySize == 1u) {
            out << "this->" << mp.name << " = ZMATmprops[" << o << "]();\n";
            o += this->getTypeSize(mp.type, 1u);
          } else {
            if (this->useDynamicallyAllocatedVector(mp.arraySize)) {
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
          throw(
              runtime_error("ZMATInterface::writeBehaviourDataConstructor : "
                            "unsupported material properties type (only scalar "
                            "supported yet)"));
        }
      }
    }
    if (!ivs.empty()) {
      SupportedTypes::TypeSize o;
      for (const auto& iv : ivs) {
        const string n = "this->" + iv.name;
        const SupportedTypes::TypeFlag flag = this->getTypeFlag(iv.type);
        if (iv.arraySize == 1u) {
          switch (flag) {
            case SupportedTypes::Scalar:
              out << n << " = ZMATstatev[" << o << "];\n";
              break;
            case SupportedTypes::Stensor:
            case SupportedTypes::Tensor:
              out << "zmat::ZMATInterface::convert(" << n << ","
                  << "&ZMATstatev[" << o << "]);\n";
              break;
            default:
              string msg("ZMATInterface::writeBehaviourDataConstructor : ");
              msg += "unsupported variable type ('" + iv.type + "')";
              throw(runtime_error(msg));
          }
          o += this->getTypeSize(iv.type, 1u);
        } else {
          if (this->useDynamicallyAllocatedVector(iv.arraySize)) {
            out << n << ".resize(" << iv.arraySize << ");\n";
            out << "for(unsigned short idx=0;idx!=" << iv.arraySize
                << ";++idx){\n";
            switch (flag) {
              case SupportedTypes::Scalar:
                out << n << "[idx] = ZMATstatev[" << o << "+idx];\n";
                break;
              // case SupportedTypes::TVector :
              //   out << "tfel::fsalgo::copy<TVectorSize>::exe(&"+src+"["
              // 	<< o << "+idx*TVectorSize],"
              // 	<< n << "[idx].begin());\n";
              //   break;
              case SupportedTypes::Stensor:
              case SupportedTypes::Tensor:
                out << "zmat::ZMATInterface::convert(" << n
                    << "[idx],&ZMATstatev[" << o << "+idx*StensorSize]);\n";
                break;
              default:
                throw(runtime_error(
                    "ZMATInterface::writeBehaviourDataConstructor : "
                    "unsupported variable type ('" +
                    iv.type + "')"));
            }
            out << "}\n";
            o += this->getTypeSize(iv.type, iv.arraySize);
          } else {
            for (int i = 0; i != iv.arraySize; ++i) {
              switch (flag) {
                case SupportedTypes::Scalar:
                  out << n << "[" << i << "] = ZMATstatev[" << o << "];\n";
                  break;
                case SupportedTypes::Stensor:
                case SupportedTypes::Tensor:
                  out << "zmat::ZMATInterface::convert(" << n << "[" << i
                      << "],&ZMATstatev[" << o << "]);\n";
                  break;
                default:
                  string msg(
                      "ZMATInterface::writeBehaviourDataConstructor : "
                      "unsupported variable type ('" +
                      iv.type + "')");
                  throw(runtime_error(msg));
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
        const SupportedTypes::TypeFlag flag = this->getTypeFlag(ev.type);
        if (flag == SupportedTypes::Scalar) {
          if (ev.arraySize == 1u) {
            out << "this->" << ev.name << " = ZMATextvars_t[ZMATevs_pos[" << o
                << "]];\n";
            o += this->getTypeSize(ev.type, 1u);
          } else {
            if (this->useDynamicallyAllocatedVector(ev.arraySize)) {
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
          string msg(
              "ZMATInterface::writeBehaviourDataConstructor : "
              "unsupported material properties type (only scalar supported "
              "yet)");
          throw(runtime_error(msg));
        }
      }
    }
    out << "this->T = ZMATextvars_t[ZMATT_pos];\n";
    out << "}\n" << endl;
  }  // end of ZMATInterface::writeBehaviourDataConstructor

  void ZMATInterface::writeBehaviourConstructor(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& initStateVarsIncrements) const {
    using namespace std;
    if (!((mb.getBehaviourType() ==
           BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
          (mb.getBehaviourType() ==
           BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR))) {
      string msg(
          "ZMATInterface::writeBehaviourDataConstructor : "
          "only small or finite strain behaviours are supported");
      throw(runtime_error(msg));
    }
    out << "/*!\n";
    out << " * \\brief constructor for the ZMAT interface\n";
    out << " *\n";
    if ((mb.getBehaviourType() ==
         BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
        (mb.getBehaviourType() ==
         BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)) {
      out << " * \\param ZMATsig     : strains\n";
    }
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
        out << " \\param ZMATeto     : strain at the beginning of the time "
               "step\n";
        out << " \\param ZMATdeto    : strain increment\n";
        break;
      case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
        out << " \\param ZMATF0 : deformation gradient at the beginning of the "
               "time step\n";
        out << " \\param ZMATF1 : deformation gradient at the end of the time "
               "step\n";
        break;
      default:
        string msg(
            "ZMATInterface::writeCallMFrontBehaviour : "
            "unsupported behaviour type");
        throw(runtime_error(msg));
    }
    out << " * \\param ZMATmprops  : material properties\n";
    out << " * \\param ZMATdata    : material data handler\n";
    out << " * \\param ZMATT_pos   : position of the temperature\n";
    out << " * \\param ZMATevs_pos : position of the external state "
           "varaibles\n";
    out << " * \\param ZMATdt      : time step\n";
    out << " */\n";
    out << mb.getClassName() << "(";
    if ((mb.getBehaviourType() ==
         BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
        (mb.getBehaviourType() ==
         BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)) {
      out << "const ZSET::TENSOR2_FLUX& ZMATsig,\n";
    }
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
        out << " const real* const ZMATeto,\n";
        out << " const real* const ZMATdeto,\n";
        break;
      case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
        out << " const real* const ZMATF0,\n";
        out << " const real* const ZMATF1,\n";
        break;
      default:
        string msg(
            "ZMATInterface::writeCallMFrontBehaviour : "
            "unsupported behaviour type");
        throw(runtime_error(msg));
    }
    out << "const ZSET::ARRAY<ZSET::COEFF>& ZMATmprops,\n"
        << "const ZSET::MAT_DATA& ZMATdata,\n"
        << "const int ZMATT_pos,\n"
        << "const ZSET::ARRAY<int>& ZMATevs_pos,\n"
        << "const double ZMATdt)\n";
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
        if (mb.useQt()) {
          out << ": " << mb.getClassName()
              << "BehaviourData<hypothesis,Type,use_qt>(ZMATsig,ZMATeto,"
                 "ZMATmprops,ZMATdata,ZMATT_pos,ZMATevs_pos),\n";
          out << mb.getClassName()
              << "IntegrationData<hypothesis,Type,use_qt>(ZMATdeto,ZMATdata,"
                 "ZMATT_pos,ZMATevs_pos,ZMATdt)";
        } else {
          out << ": " << mb.getClassName()
              << "BehaviourData<hypothesis,Type,false>(ZMATsig,ZMATeto,"
                 "ZMATmprops,ZMATdata,ZMATT_pos,ZMATevs_pos),\n";
          out << mb.getClassName()
              << "IntegrationData<hypothesis,Type,false>(ZMATdeto,ZMATdata,"
                 "ZMATT_pos,ZMATevs_pos,ZMATdt)";
        }
        break;
      case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
        if (mb.useQt()) {
          out << ": " << mb.getClassName()
              << "BehaviourData<hypothesis,Type,use_qt>(ZMATsig,ZMATF0,"
                 "ZMATmprops,ZMATdata,ZMATT_pos,ZMATevs_pos),\n";
          out << mb.getClassName()
              << "IntegrationData<hypothesis,Type,use_qt>(ZMATF1,ZMATdata,"
                 "ZMATT_pos,ZMATevs_pos,ZMATdt)";
        } else {
          out << ": " << mb.getClassName()
              << "BehaviourData<hypothesis,Type,false>(ZMATsig,ZMATF0,"
                 "ZMATmprops,ZMATdata,ZMATT_pos,ZMATevs_pos),\n";
          out << mb.getClassName()
              << "IntegrationData<hypothesis,Type,false>(ZMATF1,ZMATdata,ZMATT_"
                 "pos,ZMATevs_pos,ZMATdt)";
        }
        break;
      default:
        string msg(
            "ZMATInterface::writeCallMFrontBehaviour : "
            "unsupported behaviour type");
        throw(runtime_error(msg));
    }
    if (!initStateVarsIncrements.empty()) {
      out << ",\n" << initStateVarsIncrements;
    }
  }

  void ZMATInterface::writeIntegrationDataConstructor(
      std::ostream& out,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    using namespace std;
    const auto& d = mb.getBehaviourData(h);
    const auto& evs = d.getExternalStateVariables();
    if (!((mb.getBehaviourType() ==
           BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
          (mb.getBehaviourType() ==
           BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR))) {
      string msg(
          "ZMATInterface::writeBehaviourDataConstructor : "
          "only small or finite strain behaviours are supported");
      throw(runtime_error(msg));
    }
    out << "/*!\n";
    out << " * \\brief constructor for the ZMAT interface\n";
    out << " *\n";
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
        out << " \\param ZMATdeto  : strain increment \n";
        break;
      case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
        out << " \\param ZMATF1    : deformation gradient at the end of the "
               "time step\n";
        break;
      default:
        string msg(
            "ZMATInterface::writeCallMFrontBehaviour : "
            "unsupported behaviour type");
        throw(runtime_error(msg));
    }
    out << " * \\param ZMATdata    : material data handler\n";
    out << " * \\param ZMATT_pos   : position of the temperature\n";
    out << " * \\param ZMATevs_pos : position of the external state "
           "varaibles\n";
    out << " * \\param ZMATdt      : time step\n";
    out << " */\n";
    out << mb.getClassName() << "IntegrationData(";
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
        out << "const real * const ZMATdeto,\n";
        break;
      case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
        out << "const real * const ZMATF1,\n";
        break;
      default:
        string msg(
            "ZMATInterface::writeCallMFrontBehaviour : "
            "unsupported behaviour type");
        throw(runtime_error(msg));
    }
    out << "const ZSET::MAT_DATA& ZMATdata,\n"
        << "const int ZMATT_pos,\n";
    if (!evs.empty()) {
      out << "const ZSET::ARRAY<int>& ZMATevs_pos,\n";
    } else {
      out << "const ZSET::ARRAY<int>&,\n";
    }
    out << "const double ZMATdt)\n";
    out << ": dt(ZMATdt)\n";
    out << "{\n";
    out << "const ZSET::EXTERNAL_PARAMETER_VECTOR& ZMATextvars_t   = "
           "*(ZMATdata.param_set_ini());\n";
    out << "const ZSET::EXTERNAL_PARAMETER_VECTOR& ZMATextvars_tdt = "
           "*(ZMATdata.param_set());"
        << endl;
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
        out << "zmat::ZMATInterface::convert(this->deto,ZMATdeto);\n";
        break;
      case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
        out << "zmat::ZMATInterface::convert(this->F1,ZMATF1);\n";
        break;
      default:
        string msg(
            "ZMATInterface::writeCallMFrontBehaviour : "
            "unsupported behaviour type");
        throw(runtime_error(msg));
    }
    if (!evs.empty()) {
      SupportedTypes::TypeSize o;
      for (const auto& ev : evs) {
        const SupportedTypes::TypeFlag flag = this->getTypeFlag(ev.type);
        if (flag == SupportedTypes::Scalar) {
          if (ev.arraySize == 1u) {
            out << "this->d" << ev.name << " = ZMATextvars_tdt[ZMATevs_pos["
                << o << "]]-ZMATextvars_t[ZMATevs_pos[" << o << "]];\n";
            o += this->getTypeSize(ev.type, 1u);
          } else {
            if (this->useDynamicallyAllocatedVector(ev.arraySize)) {
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
          string msg(
              "ZMATInterface::writeBehaviourDataConstructor : "
              "unsupported material properties type (only scalar supported "
              "yet)");
          throw(runtime_error(msg));
        }
      }
    }
    out << "this->dT = ZMATextvars_tdt[ZMATT_pos]-ZMATextvars_t[ZMATT_pos];\n";
    out << "}\n" << endl;
  }

  void ZMATInterface::writeBehaviourDataMainVariablesSetters(
      std::ostream&, const BehaviourDescription&) const {
    // handled by the constructors
  }

  void ZMATInterface::writeIntegrationDataMainVariablesSetters(
      std::ostream&, const BehaviourDescription&) const {
    // handled by the constructors
  }

  void ZMATInterface::endTreatment(const BehaviourDescription& mb,
                                   const FileDescription& fd) const {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::material;
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/ZMAT");
    const Hypothesis hypotheses[3u] = {
        ModellingHypothesis::TRIDIMENSIONAL,
        ModellingHypothesis::GENERALISEDPLANESTRAIN,
        ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN};
    const string name = mb.getLibrary() + mb.getClassName();
    const string headerFileName("ZMAT" + name + ".hxx");
    const string srcFileName("ZMAT" + name + ".cxx");
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
    if (nbh == 0) {
      string msg(
          "ZMATInterface::endTreatment : "
          "no suitable modelling hypothesis supported");
      throw(runtime_error(msg));
    }
    ofstream out("include/MFront/ZMAT/" + headerFileName);
    if (!out) {
      string msg(
          "UMATInterface::endTreatment : "
          "could not open header file '" +
          headerFileName + "'");
      throw(runtime_error(msg));
    }
    out << "/*!\n";
    out << "* \\file   " << headerFileName << endl;
    out << "* \\brief  This file declares the ZMAT interface for the "
        << mb.getClassName() << " behaviour law\n";
    out << "* \\author " << fd.authorName << endl;
    out << "* \\date   " << fd.date << endl;
    out << "*/\n" << endl;
    out << "#ifndef MFRONT_ZMAT_" << makeUpperCase(mb.getClassName())
        << "_HXX\n";
    out << "#define MFRONT_ZMAT_" << makeUpperCase(mb.getClassName())
        << "_HXX\n"
        << endl;
    out << "#include\"Behavior.h\"\n";
    writeZMATUndefs(out);
    out << endl;
    out << "Z_START_NAMESPACE;\n";
    out << endl;
    out << "/*!\n"
        << "* ZMAT Interface to the " << mb.getClassName() << " behaviour"
        << "*/\n";
    out << "struct ZMAT" << mb.getClassName() << endl;
    out << ": public BEHAVIOR\n";
    out << "{\n";
    out << "//! constructor;\n";
    out << "ZMAT" << mb.getClassName() << "();\n";
    out << "/*!\n";
    out << " * \\brief integrate the beahviour over a time step\n";
    out << " * \\param[in,out] file  : input file \n";
    out << " * \\param[in]     dim   : dimension (used by the BEHAVIOUR "
           "class)\n";
    out << " * \\param[in]     integ : pointer to the local integration\n";
    out << " */\n";
    out << "virtual void initialize(ASCII_FILE&,int,LOCAL_INTEGRATION*);\n";
    out << "/*!\n";
    out << " * \\brief integrate the beahviour over a time step\n";
    out << " * \\param[in,out] mat        : material properties\n";
    out << " * \\param[out]    delta_grad : increment of driving variables\n";
    out << " * \\param[out]    tg_matrix  : tangent operator\n";
    out << " * \\param[int]    flags      : integration option\n";
    out << " * \\return NULL on success\n";
    out << " */\n";
    out << "INTEGRATION_RESULT*\n";
    out << "integrate(MAT_DATA&,const VECTOR&,MATRIX*&,int);\n";
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      out << "/*!\n";
      out << " * \\brief return the elasticity matrix\n";
      out << " * \\param[in] mdat : material data\n";
      out << " */\n";
      out << "SMATRIX\n"
          << "get_elasticity_matrix(MAT_DATA&,double);\n";
    }
    out << "//! destructor\n";
    out << "virtual ~ZMAT" << mb.getClassName() << "();\n";
    out << endl;
    out << "protected:\n";
    out << endl;
    if (!all_mp_names.empty()) {
      for (const Hypothesis* ph = hypotheses; ph != hypotheses + 3u; ++ph) {
        const Hypothesis h = *ph;
        if (mb.isModellingHypothesisSupported(h)) {
          out << "/*!\n";
          out << " * \\brief initialize material properties\n";
          out << " * \\param[in,out] file    : input file \n";
          out << " */\n";
          out << "virtual void initializeMaterialProperties"
              << getSpaceDimensionSuffix(*ph) << "(ASCII_FILE&);\n";
        }
      }
    }
    for (const Hypothesis* ph = hypotheses; ph != hypotheses + 3u; ++ph) {
      const Hypothesis h = *ph;
      if (mb.isModellingHypothesisSupported(h)) {
        out << "/*!\n";
        out << " * \\brief initialize parameters\n";
        out << " * \\param[in,out] file    : input file \n";
        out << " */\n";
        out << "virtual void initializeParameters"
            << getSpaceDimensionSuffix(*ph) << "(ASCII_FILE&);\n";
      }
    }
    for (const Hypothesis* ph = hypotheses; ph != hypotheses + 3u; ++ph) {
      const Hypothesis h = *ph;
      if (mb.isModellingHypothesisSupported(h)) {
        out << "/*!\n";
        out << " * \\brief call the mfront beahviour in "
            << ModellingHypothesis::toString(*ph) << endl;
        out << " * \\param[in,out] mat     : material properties\n";
        out << " * \\param[out] delta_grad : increment of driving variables\n";
        out << " * \\param[out] tg_matrix  : tangent operator\n";
        out << " * \\param[int] flags      : integration option\n";
        out << " * \\return NULL on success\n";
        out << " */\n";
        out << "INTEGRATION_RESULT*\n";
        out << "callMFrontBehaviour" << getSpaceDimensionSuffix(*ph)
            << "(MAT_DATA&,const VECTOR&,MATRIX*&,int);\n";
      }
    }
    out << "//! stresses\n";
    out << "TENSOR2_FLUX sig;\n";
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
        out << "//! strains\n";
        out << "TENSOR2_GRAD eto;\n";
        break;
      case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
        out << "//! deformation gradient\n";
        out << "TENSOR2_GRAD F;\n";
        break;
      default:
        string msg(
            "ZMATInterface::writeCallMFrontBehaviour : "
            "unsupported behaviour type");
        throw(runtime_error(msg));
    }
    if ((mb.getBehaviourType() ==
         BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
        (mb.getBehaviourType() ==
         BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)) {
      out << "//! tangent operator\n";
      out << "MATRIX tg_mat; \n";
    }
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      out << "//! elasticity matrix\n";
      out << "AUTO_PTR<ELASTICITY> elasticity; \n";
    }
    out << "//! material properties\n";
    out << "ARRAY<COEFF> mprops; \n";
    const auto& svs = getAllStateVariables(mb);
    if (!svs.empty()) {
      for (const auto& sv : svs) {
        out << "//! '" << sv.name << "' state variable\n";
        const SupportedTypes::TypeFlag flag = this->getTypeFlag(sv.type);
        if (flag == SupportedTypes::Scalar) {
          if (sv.arraySize == 1u) {
            out << "SCALAR_VINT " << sv.name << ";\n";
          } else {
            out << "VECTOR_VINT " << sv.name << ";\n";
          }
        } else if ((flag == SupportedTypes::Stensor) ||
                   (flag == SupportedTypes::Tensor)) {
          if (sv.arraySize == 1u) {
            out << "TENSOR2_VINT " << sv.name << ";\n";
          } else {
            out << "ARRAY<TENSOR2_VINT> " << sv.name << ";\n";
          }
        } else {
          string msg(
              "ZMATInterface::endTreatment : "
              "unsupported state variable type");
          throw(runtime_error(msg));
        }
      }
    }
    out << "//! external state variables\n";
    out << "LIST<EXTERNAL_PARAM*> local_ep_list;\n";
    out << "LIST<EXTERNAL_PARAM*>* zbb_keep_ep;\n";
    out << "//! external state variables positions\n";
    out << "ARRAY<int> evs_positions;\n";
    out << "//! position de la température\n";
    out << "int temperature_position;\n";
    out << "//! local clock\n";
    out << "CLOCK  local_clock; \n";
    out << "//! out of bounds policy\n";
    out << "tfel::material::OutOfBoundsPolicy obp;\n";
    out << "};\n" << endl;
    out << "Z_END_NAMESPACE;\n" << endl;
    out << "#endif /* MFRONT_ZMAT_" << makeUpperCase(mb.getClassName())
        << "_HXX*/\n";
    out.close();
    out.open("src/" + srcFileName);
    if (!out) {
      string msg(
          "UMATInterface::endTreatment : "
          "could not open src file '" +
          srcFileName + "'");
      throw(runtime_error(msg));
    }
    // writing source file
    out << "/*!\n";
    out << "* \\file   " << srcFileName << endl;
    out << "* \\brief  This file declares the ZMAT interface for the "
        << mb.getClassName() << " behaviour law\n";
    out << "* \\author " << fd.authorName << endl;
    out << "* \\date   " << fd.date << endl;
    out << "*/\n" << endl;
    out << endl;
    out << "#include<string>\n";
    out << "#include<vector>\n";
    out << "#include<algorithm>\n";
    out << endl;
    out << "#include\"External_parameter.h\"\n";
    out << "#include\"Verbose.h\"\n";
    writeZMATUndefs(out);
    out << endl;
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    out << "#include\"MFront/ZMAT/" << headerFileName << "\"\n";
    out << endl;
    out << "Z_START_NAMESPACE;\n";
    out << endl;
    out << "ZMAT" << mb.getClassName() << "::ZMAT" << mb.getClassName()
        << "()\n"
        << ": obp(tfel::material::None)\n"
        << "{\n"
        << "this->zbb_keep_ep = &this->local_ep_list;\n"
        << "#ifdef _WIN64\n"
        << "ZMAT_GLOBAL_STORAGE::zmat_once();\n"
        << "ZMAT_GLOBAL_STORAGE& zmat_globals = *thread_zmat_globals;\n"
        << "#endif\n"
        << "} // end of ZMAT" << mb.getClassName() << "::ZMAT"
        << mb.getClassName() << "()\n";
    out << endl;
    out << "void\n";
    out << "ZMAT" << mb.getClassName()
        << "::initialize(ASCII_FILE& file,int dim,LOCAL_INTEGRATION* integ){\n";
    out << "BEHAVIOR::initialize(file, dim,integ);\n";
    out << "using namespace std;\n";
    out << "int keep_verbose = ZSET::Verbose;\n";
    out << "this->coord.resize(dim);\n";
    out << "this->sig.initialize(this,\"sig\",this->tsz(),1);\n";
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
        out << "this->eto.initialize(this,\"eto\",this->tsz(),1);\n";
        break;
      case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
        out << "this->F.initialize(this,\"F\",this->utsz(),1);\n";
        break;
      default:
        string msg(
            "ZMATInterface::writeCallMFrontBehaviour : "
            "unsupported behaviour type");
        throw(runtime_error(msg));
    }
    if ((mb.getBehaviourType() ==
         BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
        (mb.getBehaviourType() ==
         BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)) {
      out << "this->tg_mat.resize(this->tsz(), this->tsz());\n";
    }
    out << "// initialisation dispatch\n";
    out << "if(this->tsz()==6){\n";
    this->writeBehaviourInitialisation(out, mb,
                                       ModellingHypothesis::TRIDIMENSIONAL);
    out << "} else if(this->tsz()==4){\n";
    this->writeBehaviourInitialisation(
        out, mb, ModellingHypothesis::GENERALISEDPLANESTRAIN);
    out << "} else if(this->tsz()==3){\n";
    this->writeBehaviourInitialisation(
        out, mb, ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN);
    out << "} else {\n";
    out << "ERROR(\"Invalid tensor size\");\n";
    out << "}\n";
    out << "this->temperature_position = "
           "EXTERNAL_PARAM::rank_of_nodal_ip(\"temperature\");\n";
    out << "if(this->temperature_position==-1){;\n";
    out << "INPUT_ERROR(\"temperature is not defined\");\n";
    out << "}\n";
    if (!all_mp_names.empty()) {
      out << "// check that all material properties were initialised\n";
      out << "for(int pc=0;pc!=this->mprops.size();++pc){\n"
          << "if(!this->mprops[pc].ok()){\n"
          << "INPUT_ERROR(\"Some material properties were not initialised\");\n"
          << "}\n"
          << "}\n";
    }
    out << "ZSET::Verbose = keep_verbose;\n";
    out << "} // end of ZMAT" << mb.getClassName() << "::initialize\n" << endl;
    if (!all_mp_names.empty()) {
      for (const Hypothesis* ph = hypotheses; ph != hypotheses + 3u; ++ph) {
        const Hypothesis h = *ph;
        if (mb.isModellingHypothesisSupported(h)) {
          this->writeMaterialPropertiesInitialisation(out, mb, *ph);
        }
      }
    }
    for (const Hypothesis* ph = hypotheses; ph != hypotheses + 3u; ++ph) {
      const Hypothesis h = *ph;
      if (mb.isModellingHypothesisSupported(h)) {
        this->writeParametersInitialisation(out, mb, *ph);
      }
    }
    out << "INTEGRATION_RESULT*\n";
    out << "ZMAT" << mb.getClassName() << "::integrate(MAT_DATA& mdat,\n"
        << "const VECTOR& delta_grad,\n"
        << "MATRIX*& tg_matrix,\n"
        << "int flags){\n"
        << "int keep_verbose  = ZSET::Verbose; \n"
        << "CLOCK* keep_clock = "
           "ZSET::stored_thread_zbase_globals->ptr()->active_clock; \n"
        << "tg_matrix = &(this->tg_mat);\n"
        << "this->set_var_aux_to_var_aux_ini();\n"
        << "this->set_var_int_to_var_int_ini();\n"
        << "LIST<EXTERNAL_PARAM*>* ep_save = &EXTERNAL_PARAM::Get_EP_list();\n"
        << "EXTERNAL_PARAM::set_EP_list(this->zbb_keep_ep);\n"
        << "if(!this->curr_ext_param){\n"
        << "this->curr_ext_param = *mdat.param_set();\n"
        << "}\n"
        << "this->calc_local_coefs();\n"
        << "INTEGRATION_RESULT* r = NULL;\n"
        << "try{\n";
    out << "if(this->tsz()==6){\n";
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
    out << "} else {\n";
    out << "ERROR(\"Invalid tensor size\");\n";
    out << "}\n";
    out << "}\n"
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
        << "return NULL;\n";
    out << "} // end of ZMAT" << mb.getClassName() << "::integrate\n" << endl;
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      out << "SMATRIX\n"
          << "ZMAT" << mb.getClassName()
          << "::get_elasticity_matrix(MAT_DATA& mdat,double){\n"
          << "elasticity->attach_all(mdat);\n"
          << "elasticity->calc_coef();\n"
          << "return SMATRIX(*elasticity());\n"
          << "} // end of ZMAT" << mb.getClassName()
          << "::get_elasticity_matrix\n"
          << endl;
    }
    for (const Hypothesis* ph = hypotheses; ph != hypotheses + 3u; ++ph) {
      const Hypothesis h = *ph;
      if (mb.isModellingHypothesisSupported(h)) {
        this->writeCallMFrontBehaviour(out, mb, *ph);
      }
    }
    out << endl;
    out << "ZMAT" << mb.getClassName() << "::~ZMAT" << mb.getClassName()
        << "(){\n";
    out << "} // end of ZMAT" << mb.getClassName() << "::~ZMAT"
        << mb.getClassName() << endl
        << endl;
    out << "BEHAVIOR_READER(ZMAT" << mb.getClassName() << ","
        << mb.getClassName() << ")\n"
        << endl;
    out << "Z_END_NAMESPACE;\n";
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
    const auto& esvs = d.getExternalStateVariables();
    const unsigned short dime = getSpaceDimension(h);
    const int mps_size = this->getTotalSize(mps).getValueForDimension(dime);
    if (!mps.empty()) {
      out << "this->mprops.resize(" << mps_size << ");\n";
    }
    if (!isvs.empty()) {
      for (const auto& isv : isvs) {
        const SupportedTypes::TypeFlag flag = this->getTypeFlag(isv.type);
        if (flag == SupportedTypes::Scalar) {
          out << "this->" << isv.name << ".initialize(this,\"" << isv.name
              << "\"," << isv.arraySize << ",1);\n";
        } else if (flag == SupportedTypes::Stensor) {
          if (isv.arraySize == 1u) {
            out << "this->" << isv.name << ".initialize(this,\"" << isv.name
                << "\",this->tsz(),1);\n";
          } else {
            for (unsigned short i = 0; i != isv.arraySize; ++i) {
              out << "this->" << isv.name << "[" << i << "].initialize(this,\""
                  << isv.name << std::to_string(i) << "\",this->tsz(),1);\n";
            }
          }
        } else if (flag == SupportedTypes::Tensor) {
          if (isv.arraySize == 1u) {
            out << "this->" << isv.name << ".initialize(this,\"" << isv.name
                << "\",this->utsz(),1);\n";
          } else {
            for (unsigned short i = 0; i != isv.arraySize; ++i) {
              out << "this->" << isv.name << "[" << i << "].initialize(this,\""
                  << isv.name << std::to_string(i) << "\",this->uts(),1);\n";
            }
          }
        } else {
          throw(
              runtime_error("ZMATInterface::endTreatment: "
                            "unsupported state variable type"));
        }
      }
    }
    out << "for(;;){\n";
    out << "STRING str=file.getSTRING();\n";
    out << "if(this->base_read(str,file)){\n";
    const auto& all_mp_names = getAllMaterialPropertiesNames(mb);
    if (!all_mp_names.empty()) {
      out << "} else "
             "if((str==\"**model_coef\")||(str==\"**material_properties\")){\n";
      out << "this->initializeMaterialProperties" << getSpaceDimensionSuffix(h)
          << "(file);\n";
    }
    out << "} else if(str==\"**parameters\"){\n";
    out << "this->initializeParameters" << getSpaceDimensionSuffix(h)
        << "(file);\n";
    out << "} else if(str==\"**out_of_bounds_policy\"){\n";
    out << "STRING policy=file.getSTRING();\n";
    out << "if(policy==\"None\"){\n";
    out << "this->obp=tfel::material::None;\n";
    out << "} else if(policy==\"Strict\"){\n";
    out << "this->obp=tfel::material::Strict;\n";
    out << "} else if(policy==\"Warning\"){\n";
    out << "this->obp=tfel::material::Warning;\n";
    out << "} else {\n";
    out << "INPUT_ERROR(\"unknown policy '\"+policy+\"'\");\n"
        << "}\n";
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      out << "} else if((str==\"**elasticity\")||(str==\"**ELASTICITY\")){\n";
      out << "if(!this->elasticity.Null()){\n"
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
        << "}" << endl;
    if (!esvs.empty()) {
      const int ext_size = this->getTotalSize(esvs).getValueForDimension(dime);
      out << "evs_positions.resize(" << ext_size << ");" << endl;
      VariableDescriptionContainer::const_iterator pev;
      int i = 0;
      for (pev = esvs.begin(); pev != esvs.end(); ++pev) {
        const VariableDescription& v = *pev;
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
    const auto& params = d.getParameters();
    const vector<string> pnames = d.getExternalNames(params);
    VariableDescriptionContainer::const_iterator p;
    vector<string>::const_iterator pn;
    out << "void\n";
    out << "ZMAT" << mb.getClassName() << "::initializeParameters"
        << getSpaceDimensionSuffix(h) << "(ASCII_FILE& file){\n";
    out << "for(;;){\n"
        << "STRING str=file.getSTRING();\n"
        << "if(str[0]=='*'){\n"
        << "file.back();\n"
        << "break;\n";
    for (p = params.begin(), pn = pnames.begin(); p != params.end();
         ++p, ++pn) {
      out << "} else if(str==\"" << *pn << "\"){\n";
      if (p->type == "real") {
        out << "const double value=file.getdouble();\n";
      } else if (p->type == "int") {
        out << "const int value=file.getint();\n";
      } else if (p->type == "ushort") {
        out << "const unsigned short value=static_cast<unsigned "
               "short>(file.getint());\n";
      }
      if (mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS, p->name)) {
        out << "tfel::material::" << mb.getClassName()
            << "ParametersInitializer::get()." << p->name << " = value;\n";
      } else {
        out << "tfel::material::" << mb.getClassName()
            << ModellingHypothesis::toString(h)
            << "ParametersInitializer::get()." << p->name << " = value;\n";
      }
    }
    out << "} else {\n"
        << "INPUT_ERROR(\"invalid parameter name '\"+str+\"'\");\n"
        << "}\n"
        << "}\n"
        << "}\n"
        << endl;
  }  // end of ZMATInterface::writeParametersInitialisation

  void ZMATInterface::writeMaterialPropertiesInitialisation(
      std::ostream& out,
      const BehaviourDescription& mb,
      const ZMATInterface::Hypothesis h) const {
    using namespace std;
    const auto& d = mb.getBehaviourData(h);
    const auto& mps = d.getMaterialProperties();
    const auto& all_mp_names = getAllMaterialPropertiesNames(mb);
    const vector<string> mpnames = d.getExternalNames(mps);
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
    const int mps_size = this->getTotalSize(mps).getValueForDimension(dime);
    out << "void\n";
    out << "ZMAT" << mb.getClassName() << "::initializeMaterialProperties"
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
          << ",str)==all_mp_names+" << all_mp_names.size() << "){\n";
      out << "INPUT_ERROR(\"No material property named '\"+str+\"'\");\n";
      out << "}\n";
    }
    out << "const STRING * const pmat = find(mp_names"
        << getSpaceDimensionSuffix(h) << ",mp_names"
        << getSpaceDimensionSuffix(h) << "+" << mps_size << ",str);\n";
    if (nbh == 1u) {
      out << "if(pmat==mp_names" << getSpaceDimensionSuffix(h) << "+"
          << mps_size << "){\n";
      out << "INPUT_ERROR(\"No material property named '\"+str+\"'\");\n";
      out << "}\n";
    }
    out << "if(this->mprops[pmat-mp_names" << getSpaceDimensionSuffix(h)
        << "].ok()){\n";
    out << "INPUT_ERROR(\"material property '\"+str+\"' already defined\");\n";
    out << "}\n";
    out << "this->mprops[pmat-mp_names" << getSpaceDimensionSuffix(h)
        << "].read(str,file,this);\n";
    out << "}\n";
    out << "} // end of ZMAT" << mb.getClassName()
        << "::initializeMaterialProperties" << getSpaceDimensionSuffix(h)
        << endl
        << endl;
  }

  void ZMATInterface::writeCallMFrontBehaviour(
      std::ostream& out,
      const BehaviourDescription& mb,
      const ZMATInterface::Hypothesis h) const {
    using namespace std;
    out << "INTEGRATION_RESULT*\n";
    out << "ZMAT" << mb.getClassName() << "::callMFrontBehaviour"
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
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      out << "// strain and strain increment\n";
      out << "double stran[" << getStensorSize(h) << "];\n";
      out << "double dstran[" << getStensorSize(h) << "];\n";
      out << "stran[0] = this->eto[0]-delta_grad[0];\n";
      out << "stran[1] = this->eto[1]-delta_grad[1];\n";
      out << "stran[2] = this->eto[2]-delta_grad[2];\n";
      if ((h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
          (h == ModellingHypothesis::TRIDIMENSIONAL)) {
        out << "stran[3] = this->eto[3]-delta_grad[3];\n";
        if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          out << "stran[4] = this->eto[4]-delta_grad[4];\n";
          out << "stran[5] = this->eto[5]-delta_grad[5];\n";
        }
      }
      out << "dstran[0] = delta_grad[0];\n";
      out << "dstran[1] = delta_grad[1];\n";
      out << "dstran[2] = delta_grad[2];\n";
      if ((h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
          (h == ModellingHypothesis::TRIDIMENSIONAL)) {
        out << "dstran[3] = delta_grad[3];\n";
        if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          out << "dstran[4] = delta_grad[4];\n";
          out << "dstran[5] = delta_grad[5];\n";
        }
      }
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      out << "// deformation gradients\n";
      out << "double F0[" << getTensorSize(h) << "];\n";
      out << "F0[0] = this->F[0]-delta_grad[0];\n";
      out << "F0[1] = this->F[1]-delta_grad[1];\n";
      out << "F0[2] = this->F[2]-delta_grad[2];\n";
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
      string msg(
          "ZMATInterface::writeCallMFrontBehaviour : "
          "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
        out << mb.getClassName() << "::SMFlag smflag = " << mb.getClassName()
            << "::STANDARDTANGENTOPERATOR;\n";
        break;
      case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
        out << mb.getClassName() << "::SMFlag smflag = " << mb.getClassName()
            << "::C_TRUESDELL;\n";
        break;
      default:
        string msg(
            "ZMATInterface::writeCallMFrontBehaviour : "
            "unsupported behaviour type");
        throw(runtime_error(msg));
    }
    out << "// tangent operator type\n"
        << mb.getClassName() << "::SMType smtype = " << mb.getClassName()
        << "::NOSTIFFNESSREQUESTED;\n"
        << "if(flags&CALC_TG_MATRIX){\n"
        << "smtype = " << mb.getClassName() << "::CONSISTENTTANGENTOPERATOR;\n"
        << "}\n";
    switch (mb.getBehaviourType()) {
      case BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR:
        out << mb.getClassName()
            << " b(this->sig,stran,dstran,this->mprops,mdat,this->temperature_"
               "position,\n"
            << "this->evs_positions,ZSET::stored_thread_zbase_globals->ptr()->"
               "active_clock->get_dtime());\n";
        break;
      case BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR:
        out << mb.getClassName()
            << " b(this->sig,F0,&(this->F[0]),this->mprops,mdat,this->"
               "temperature_position,\n"
            << "this->evs_positions,ZSET::stored_thread_zbase_globals->ptr()->"
               "active_clock->get_dtime());\n";
        break;
      default:
        string msg(
            "ZMATInterface::writeCallMFrontBehaviour : "
            "unsupported behaviour type");
        throw(runtime_error(msg));
    }
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      out << "zmat::ZMATInterface::convert(b.getStiffnessTensor(),this->get_"
             "elasticity_matrix(mdat,1.));"
          << endl;
    }
    out << "b.initialize();\n";
    out << "if(b.integrate(smflag,smtype)!=" << mb.getClassName()
        << "::SUCCESS){\n"
        << "static INTEGRATION_RESULT bad_result;\n"
        << "bad_result.set_error(INTEGRATION_RESULT::UNDEFINED_BEHAVIOR);\n"
        << "return &bad_result;\n"
        << "}\n"
        << "b.ZMATexportStateData(this->sig,mdat);\n"
        << "if(smtype!=" << mb.getClassName() << "::NOSTIFFNESSREQUESTED){\n";
    if (mb.getBehaviourType() ==
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      out << "zmat::ZMATInterface::convert(*tg_matrix,b.getTangentOperator());"
          << endl;
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      out << "zmat::ZMATInterface::convert(*tg_matrix,b.getTangentOperator()."
             "get<st2tost2<"
          << getSpaceDimension(h) << ",double> >());" << endl;
    } else {
      string msg(
          "ZMATInterface::writeCallMFrontBehaviour : "
          "unsupported behaviour type");
      throw(runtime_error(msg));
    }
    out << "}\n"
        << "return nullptr;\n"
        << "} // end of ZMAT" << mb.getClassName() << "::callMFrontBehaviour"
        << getSpaceDimensionSuffix(h) << endl
        << endl;
  }

  void ZMATInterface::getTargetsDescription(TargetsDescription& d,
                                            const BehaviourDescription& bd) {
    const auto lib = getLibraryName(bd);
    const auto name = bd.getLibrary() + bd.getClassName();
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    insert_if(d[lib].cppflags,
              "$(shell " + tfel_config + " --includes --zmat)");
#pragma message("Linux specific")
    insert_if(d[lib].cppflags, "-DLinux");
    insert_if(d.headers, "MFront/ZMAT/ZMAT" + name + ".hxx");
    insert_if(d[lib].sources, "ZMAT" + name + ".cxx");
    insert_if(d[lib].epts, bd.getClassName());
    insert_if(d[lib].ldflags, "$(shell " + tfel_config +
                                  " --libs --material --mfront-profiling)");
  }  // end of ZMATInterface::getTargetsDescription

  ZMATInterface::~ZMATInterface() {}

}  // end of namespace mfront
