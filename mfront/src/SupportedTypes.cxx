/*!
 * \file   mfront/src/SupportedTypes.cxx
 *
 * \brief
 * \author Helfer Thomas
 * \date   12 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * re served.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <utility>
#include <sstream>
#include <stdexcept>

#include "MFront/MFrontDebugMode.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/SupportedTypes.hxx"

namespace mfront {

  /*!
   * \return a map between type names and Supported::TypeFlags
   */
  static std::map<std::string, SupportedTypes::TypeFlag>&
  SupportedTypes_getFlags() {
    using namespace std;
    using TypeFlag = SupportedTypes::TypeFlag;
    static map<string, TypeFlag> flags = {
        {"real", SupportedTypes::Scalar},
        {"frequency", SupportedTypes::Scalar},
        {"stress", SupportedTypes::Scalar},
        {"length", SupportedTypes::Scalar},
        {"time", SupportedTypes::Scalar},
        //    {"stressrate",SupportedTypes::Scalar},
        {"strain", SupportedTypes::Scalar},
        {"strainrate", SupportedTypes::Scalar},
        {"temperature", SupportedTypes::Scalar},
        {"energy_density", SupportedTypes::Scalar},
        {"thermalexpansion", SupportedTypes::Scalar},
        {"massdensity", SupportedTypes::Scalar},
        {"TVector", SupportedTypes::TVector},
        {"Stensor", SupportedTypes::Stensor},
        {"Tensor", SupportedTypes::Tensor},
        {"StressStensor", SupportedTypes::Stensor},
        {"StressRateStensor", SupportedTypes::Stensor},
        {"StrainStensor", SupportedTypes::Stensor},
        {"StrainRateStensor", SupportedTypes::Stensor},
        // CZM
        {"DisplacementTVector", SupportedTypes::TVector},
        {"ForceTVector", SupportedTypes::TVector},
        // Finite Strain
        {"DeformationGradientTensor", SupportedTypes::Tensor}};
    return flags;
  }  // end of SupportedTypes_getFlags

  const std::map<std::string, SupportedTypes::TypeFlag>&
  SupportedTypes::getTypeFlags() {
    return SupportedTypes_getFlags();
  }  // end of SupportedTypes::getTypeFlags

  SupportedTypes::TypeSize::TypeSize() = default;

  SupportedTypes::TypeSize::TypeSize(TypeSize&&) = default;

  SupportedTypes::TypeSize::TypeSize(const TypeSize&) = default;

  SupportedTypes::TypeSize::TypeSize(const int a,
                                     const int b,
                                     const int c,
                                     const int d)
      : scalarSize(a), tvectorSize(b), stensorSize(c), tensorSize(d) {}

  SupportedTypes::TypeSize& SupportedTypes::TypeSize::operator=(TypeSize&&) =
      default;

  SupportedTypes::TypeSize& SupportedTypes::TypeSize::operator=(
      const TypeSize&) = default;

  SupportedTypes::TypeSize& SupportedTypes::TypeSize::operator+=(
      const TypeSize& rhs) {
    this->scalarSize += rhs.scalarSize;
    this->tvectorSize += rhs.tvectorSize;
    this->stensorSize += rhs.stensorSize;
    this->tensorSize += rhs.tensorSize;
    return *this;
  }

  SupportedTypes::TypeSize& SupportedTypes::TypeSize::operator-=(
      const TypeSize& rhs) {
    this->scalarSize -= rhs.scalarSize;
    this->tvectorSize -= rhs.tvectorSize;
    this->stensorSize -= rhs.stensorSize;
    this->tensorSize -= rhs.tensorSize;
    return *this;
  }

  int SupportedTypes::TypeSize::getValueForDimension(
      const unsigned short d) const {
    switch (d) {
      case 1:
        return scalarSize + tvectorSize + 3 * (stensorSize + tensorSize);
      case 2:
        return scalarSize + 2 * tvectorSize + 4 * stensorSize + 5 * tensorSize;
      case 3:
        return scalarSize + 3 * tvectorSize + 6 * stensorSize + 9 * tensorSize;
    }
    throw(
        std::runtime_error("SupportedTypes::TypeSize::getValueForDimension : "
                           "invalid type size"));
  }  // end of SupportedTypes::TypeSize::getValueForDimension

  int SupportedTypes::TypeSize::getValueForModellingHypothesis(
      const Hypothesis h) const {
    return this->getValueForDimension(tfel::material::getSpaceDimension(h));
  }

  std::ostream& operator<<(std::ostream& os,
                           const SupportedTypes::TypeSize& size) {
    bool first = true;
    if (size.scalarSize != 0) {
      os << size.scalarSize;
      first = false;
    }
    if (size.tvectorSize != 0) {
      if ((!first) && (size.tvectorSize >= 0)) {
        os << "+";
      }
      if (size.tvectorSize == 1) {
        os << "TVectorSize";
      } else {
        os << size.tvectorSize << "*TVectorSize";
      }
      first = false;
    }
    if (size.stensorSize != 0) {
      if ((!first) && (size.stensorSize >= 0)) {
        os << "+";
      }
      if (size.stensorSize == 1) {
        os << "StensorSize";
      } else {
        os << size.stensorSize << "*StensorSize";
      }
      first = false;
    }
    if (size.tensorSize != 0) {
      if ((!first) && (size.tensorSize >= 0)) {
        os << "+";
      }
      if (size.tensorSize == 1) {
        os << "TensorSize";
      } else {
        os << size.tensorSize << "*TensorSize";
      }
      first = false;
    }
    if (first) {
      os << "0";
    }
    return os;
  }

  SupportedTypes::SupportedTypes()
      : areDynamicallyAllocatedVectorsAllowed_(true) {}

  void SupportedTypes::reset() {
    this->areDynamicallyAllocatedVectorsAllowed_ = true;
  }  // end of SupportedTypes::reset

  bool SupportedTypes::isSupportedType(const std::string& t) const {
    const auto& flags = SupportedTypes_getFlags();
    return flags.find(t) != flags.end();
  }  // end of SupportedTypes::isSupportedType

  SupportedTypes::TypeFlag SupportedTypes::getTypeFlag(
      const std::string& name) const {
    const auto& flags = SupportedTypes_getFlags();
    const auto p = flags.find(name);
    if (p == flags.end()) {
      throw(
          std::runtime_error("SupportedTypes::getTypeTag : "
                             "'" +
                             name + "' is not a supported type."));
    }
    return p->second;
  }

  SupportedTypes::TypeSize SupportedTypes::getTypeSize(
      const std::string& type, const unsigned short a) const {
    TypeSize res;
    switch (this->getTypeFlag(type)) {
      case Scalar:
        res = TypeSize(a, 0u, 0u, 0u);
        break;
      case TVector:
        res = TypeSize(0u, a, 0u, 0u);
        break;
      case Stensor:
        res = TypeSize(0u, 0u, a, 0u);
        break;
      case Tensor:
        res = TypeSize(0u, 0u, 0u, a);
        break;
      default:
        throw(
            std::runtime_error("SupportedTypes::getTypeSize: internal error."));
    }
    return res;
  }

  std::string SupportedTypes::getTimeDerivativeType(
      const std::string& type) const {
    if (type == "real") {
      return "frequency";
    } else if (type == "strain") {
      return "strainrate";
    } else if (type == "stress") {
      return "stressrate";
    } else if (type == "Stensor") {
      return "FrequencyStensor";
    } else if (type == "StressStensor") {
      return "StressRateStensor";
    } else if (type == "StrainStensor") {
      return "StrainRateStensor";
    } else if (type == "DeformationGradientTensor") {
      return "DeformationGradientRateTensor";
    } else {
      throw(
          std::runtime_error("SupportedTypes::getTimeDerivativeType: "
                             "internal error, unsupported type '" +
                             type + "'"));
    }
  }

  int SupportedTypes::TypeSize::getScalarSize() const {
    return this->scalarSize;
  }

  int SupportedTypes::TypeSize::getTVectorSize() const {
    return this->tvectorSize;
  }

  int SupportedTypes::TypeSize::getStensorSize() const {
    return this->stensorSize;
  }

  int SupportedTypes::TypeSize::getTensorSize() const {
    return this->tensorSize;
  }

  bool SupportedTypes::TypeSize::operator==(
      const SupportedTypes::TypeSize& rhs) const {
    return ((this->getScalarSize() == rhs.getScalarSize()) &&
            (this->getStensorSize() == rhs.getStensorSize()) &&
            (this->getTVectorSize() == rhs.getTVectorSize()) &&
            (this->getTensorSize() == rhs.getTensorSize()));
  }  // end of SupportedTypes::TypeSize::operator==

  bool SupportedTypes::TypeSize::operator!=(
      const SupportedTypes::TypeSize& rhs) const {
    return !this->operator==(rhs);
  }  // end of SupportedTypes::TypeSize::operator!=

  bool SupportedTypes::TypeSize::isNull() const {
    return ((this->getScalarSize() == 0) && (this->getStensorSize() == 0) &&
            (this->getTVectorSize() == 0) && (this->getTensorSize() == 0));
  }  // end of SupportedTypes::TypeSize::isNull

  void SupportedTypes::writeVariableInitializersInBehaviourDataConstructorI(
      std::ostream& f,
      const VariableDescriptionContainer& v,
      const std::string& src,
      const std::string& prefix,
      const std::string& suffix) const {
    SupportedTypes::TypeSize currentOffset;
    for (auto p = v.begin(); p != v.end(); ++p) {
      if (p->arraySize == 1u) {
        const auto n = prefix + p->name + suffix;
        f << ",\n";
        auto flag = this->getTypeFlag(p->type);
        if (flag == SupportedTypes::Scalar) {
          f << n << "(" + src + "[" << currentOffset << "])";
        } else if ((flag == SupportedTypes::TVector) ||
                   (flag == SupportedTypes::Stensor) ||
                   (flag == SupportedTypes::Tensor)) {
          f << n << "(&" + src + "[" << currentOffset << "])";
        } else {
          throw(std::runtime_error(
              "SupportedTypes::"
              "writeVariableInitializersInBehaviourDataConstructorI : "
              "internal error, tag unsupported"));
        }
      }
      currentOffset += this->getTypeSize(p->type, p->arraySize);
    }
  }  // end of
     // SupportedTypes::writeVariableInitializersInBehaviourDataConstructorI

  bool SupportedTypes::useDynamicallyAllocatedVector(
      const unsigned short s) const {
    return (s >= SupportedTypes::ArraySizeLimit) &&
           (this->areDynamicallyAllocatedVectorsAllowed());
  }  // end of SupportedTypes::useDynamicallyAllocatedVector

  bool SupportedTypes::areDynamicallyAllocatedVectorsAllowed() const {
    return this->areDynamicallyAllocatedVectorsAllowed_;
  }  // end of SupportedTypes::areDynamicallyAllocatedVectorsAllowed

  void SupportedTypes::writeVariableInitializersInBehaviourDataConstructorII(
      std::ostream& f,
      const VariableDescriptionContainer& v,
      const std::string& src,
      const std::string& prefix,
      const std::string& suffix) const {
    SupportedTypes::TypeSize currentOffset;
    for (auto p = v.begin(); p != v.end(); ++p) {
      if (p->arraySize == 1u) {
        currentOffset += this->getTypeSize(p->type, p->arraySize);
      } else {
        const auto flag = this->getTypeFlag(p->type);
        const auto n = prefix + p->name + suffix;
        if (this->useDynamicallyAllocatedVector(p->arraySize)) {
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
              throw(std::runtime_error(
                  "SupportedTypes::"
                  "writeVariableInitializersInBehaviourDataConstructorII: "
                  "internal error, tag unsupported"));
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
                throw(std::runtime_error(
                    "SupportedTypes::"
                    "writeVariableInitializersInBehaviourDataConstructorII: "
                    "internal error, tag unsupported"));
            }
            currentOffset += this->getTypeSize(p->type, 1u);
          }
        }
      }
    }
  }  // end of
     // SupportedTypes::writeVariableInitializersInBehaviourDataConstructorII

  std::string SupportedTypes::getIntegrationVariablesIncrementsInitializers(
      const VariableDescriptionContainer& v,
      const bool useStateVarTimeDerivative) const {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    ostringstream f;
    if (!v.empty()) {
      for (p = v.begin(); p != v.end(); ++p) {
        SupportedTypes::TypeFlag flag = getTypeFlag(p->type);
        const string n = p->name;
        const string t = (!useStateVarTimeDerivative)
                             ? p->type
                             : this->getTimeDerivativeType(p->type);
        if (p != v.begin()) {
          f << ",\n";
        }
        if (flag == SupportedTypes::Scalar) {
          if (this->useDynamicallyAllocatedVector(p->arraySize)) {
            f << "d" << n << "(" << p->arraySize << "," << t << "(0))";
          } else {
            f << "d" << n << "(" << t << "(0))";
          }
        } else if ((flag == SupportedTypes::TVector) ||
                   (flag == SupportedTypes::Stensor) ||
                   (flag == SupportedTypes::Tensor)) {
          string traits;
          if (flag == SupportedTypes::TVector) {
            traits = "VectorTraits";
          } else if (flag == SupportedTypes::Stensor) {
            traits = "StensorTraits";
          } else if (flag == SupportedTypes::Tensor) {
            traits = "TensorTraits";
          } else {
            string msg(
                "SupportedTypes::writeIntegrationVariablesInitializers : ");
            msg += "internal error, tag unsupported";
            throw(runtime_error(msg));
          }
          if (p->arraySize == 1u) {
            f << "d" << n << "(typename tfel::math::" + traits + "<" << t
              << ">::NumType(0))";
          } else {
            if (this->useDynamicallyAllocatedVector(p->arraySize)) {
              f << "d" << n << "(" << p->arraySize << "," << t
                << "(typename tfel::math::" + traits + "<" << t
                << ">::NumType(0)))";
            } else {
              f << "d" << n << "(" << t
                << "(typename tfel::math::" + traits + "<" << t
                << ">::NumType(0)))";
            }
          }
        } else {
          string msg(
              "SupportedTypes::writeIntegrationVariablesInitializers : ");
          msg += "internal error, tag unsupported";
          throw(runtime_error(msg));
        }
      }
    }
    return f.str();
  }  // end of SupportedTypes::getIntegrationVariablesInitializers

  SupportedTypes::TypeSize SupportedTypes::getTotalSize(
      const VariableDescriptionContainer& vc) const {
    SupportedTypes::TypeSize s;
    for (const auto& v : vc) {
      s += this->getTypeSize(v.type, v.arraySize);
    }
    return s;
  }  // end of SupportedTypes::getTotalSize

  unsigned short SupportedTypes::getNumberOfVariables(
      const VariableDescriptionContainer& vc) const {
    unsigned short n = 0u;
    for (const auto& v : vc) {
      n = n + v.arraySize;
    }
    return n;
  }  // end of SupportedTypes::getNumberOfVariables

  void SupportedTypes::writeResultsArrayResize(
      std::ostream& f,
      const std::string& dest,
      const VariableDescriptionContainer& v) const {
    this->writeResultsArrayResize(f, dest, this->getTotalSize(v));
  }  // end of SupportedTypes::writeResultsArrayResize

  void SupportedTypes::writeResultsArrayResize(
      std::ostream& f,
      const std::string& dest,
      const SupportedTypes::TypeSize& s) const {
    f << "if(" << dest << "!=" << s << "){\n";
    f << dest + ".resize(" << s << ");\n";
    f << "}\n";
  }

  void SupportedTypes::exportResults(std::ostream& f,
                                     const VariableDescriptionContainer& v,
                                     const std::string& dest,
                                     const bool useQt) const {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    SupportedTypes::TypeSize currentOffset;
    if (!v.empty()) {
      for (p = v.begin(); p != v.end(); ++p) {
        SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
        if (p->arraySize == 1u) {
          if (flag == SupportedTypes::Scalar) {
            if (useQt) {
              f << dest << "[" << currentOffset << "] = base_cast(this->"
                << p->name << ");\n";
            } else {
              f << dest << "[" << currentOffset << "] = this->" << p->name
                << ";\n";
            }
          } else if ((flag == SupportedTypes::TVector) ||
                     (flag == SupportedTypes::Stensor) ||
                     (flag == SupportedTypes::Tensor)) {
            f << "exportToBaseTypeArray(this->" << p->name << ",&" << dest
              << "[" << currentOffset << "]);\n";
          } else {
            string msg("SupportedTypes::exportResults : ");
            msg += "internal error, tag unsupported";
            throw(runtime_error(msg));
          }
          currentOffset += this->getTypeSize(p->type, p->arraySize);
        } else {
          if (this->useDynamicallyAllocatedVector(p->arraySize)) {
            f << "for(unsigned short idx=0;idx!=" << p->arraySize << ";++idx){"
              << endl;
            if (flag == SupportedTypes::Scalar) {
              if (useQt) {
                f << dest << "[" << currentOffset
                  << "+idx] = common_cast(this->" << p->name << "[idx]);\n";
              } else {
                f << dest << "[" << currentOffset << "+idx] = this->" << p->name
                  << "[idx];\n";
              }
            } else if ((flag == SupportedTypes::TVector) ||
                       (flag == SupportedTypes::Stensor) ||
                       (flag == SupportedTypes::Tensor)) {
              f << "exportToBaseTypeArray(this->" << p->name << "[idx],&"
                << dest << "[" << currentOffset << "+idx*StensorSize]);\n";
            } else {
              string msg("SupportedTypes::exportResults : ");
              msg += "internal error, tag unsupported";
              throw(runtime_error(msg));
            }
            f << "}" << endl;
            currentOffset += this->getTypeSize(p->type, p->arraySize);
          } else {
            for (unsigned short i = 0; i != p->arraySize; ++i) {
              if (flag == SupportedTypes::Scalar) {
                if (useQt) {
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
                string msg("SupportedTypes::exportResults : ");
                msg += "internal error, tag unsupported";
                throw(runtime_error(msg));
              }
              currentOffset += this->getTypeSize(p->type, 1u);
            }
          }
        }
      }
    }
  }

  void SupportedTypes::writeVariablesDeclarations(
      std::ostream& f,
      const VariableDescriptionContainer& v,
      const std::string& prefix,
      const std::string& suffix,
      const std::string& fileName,
      const bool useTimeDerivative) const {
    for (const auto& e : v) {
      this->writeVariableDeclaration(f, e, prefix, suffix, fileName,
                                     useTimeDerivative);
    }
  }  // end of SupportedTypes::writeVariablesDeclarations

  void SupportedTypes::writeVariableDeclaration(
      std::ostream& f,
      const VariableDescription& v,
      const std::string& prefix,
      const std::string& suffix,
      const std::string& fileName,
      const bool useTimeDerivative) const {
    const auto n = prefix + v.name + suffix;
    const auto t =
        (!useTimeDerivative) ? v.type : this->getTimeDerivativeType(v.type);
    if ((!getDebugMode()) && (v.lineNumber != 0u)) {
      f << "#line " << v.lineNumber << " \"" << fileName << "\"\n";
    }
    if (v.arraySize == 1u) {
      f << t << " " << n << ";\n";
    } else {
      if (this->useDynamicallyAllocatedVector(v.arraySize)) {
        f << "tfel::math::vector<" << t << " > " << n << ";\n";
      } else {
        f << "tfel::math::tvector<" << v.arraySize << ", " << t << " > " << n
          << ";\n";
      }
    }
  }  // end of SupportedTypes::writeVariableDeclaration

  SupportedTypes::~SupportedTypes() = default;

}  // end of namespace mfront
