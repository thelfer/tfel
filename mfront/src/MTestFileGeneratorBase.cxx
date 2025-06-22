/*!
 * \file  mfront/src/MTestFileGeneratorBase.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 09 juil. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <fstream>
#include <iterator>
#include <stdexcept>

#include "MFront/MTestFileGeneratorBase.hxx"

// fixing a bug on current glibc++ cygwin versions (19/08/2015)
#if defined __CYGWIN__ && (!defined _GLIBCXX_USE_C99)
#include <sstream>
namespace std {
  template <typename T>
  std::string to_string(const T& v) {
    std::ostringstream s;
    s << v;
    return s.str();
  }
}  // namespace std
#endif /* defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99) */

namespace mfront {

  MTestFileGeneratorBase::InternalStateVariable::~InternalStateVariable() =
      default;

  unsigned int MTestFileGeneratorBase::getIdentifier() {
    static unsigned int i = 0;
    return ++i;
  }

  MTestFileGeneratorBase::MTestFileGeneratorBase()
      : hypothesis(tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS),
        handleThermalExpansion(false),
        hasRotationMatrix(false) {
  }  // end of MTestFileGeneratorBase::MTestFileGeneratorBase

  void MTestFileGeneratorBase::setHandleThermalExpansion(const bool b) {
    this->handleThermalExpansion = b;
  }

  void MTestFileGeneratorBase::setModellingHypothesis(
      const tfel::material::ModellingHypothesis::Hypothesis h) {
    using namespace std;
    using namespace tfel::material;
    if (this->hypothesis != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      string msg(
          "MTestFileGeneratorBase::setModellingHypothesis : "
          "modelling hypothesis already set");
      throw(runtime_error(msg));
    }
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      string msg(
          "MTestFileGeneratorBase::setModellingHypothesis : "
          "invalid modelling hypothesis");
      throw(runtime_error(msg));
    }
    this->hypothesis = h;
  }  // end of MTestFileGeneratorBase::setModellingHypothesis

  void MTestFileGeneratorBase::addTime(const MTestFileGeneratorBase::real t) {
    using namespace std;
    if (!this->times.insert(t).second) {
      string msg(
          "MTestFileGeneratorBase::addTime : "
          "time '" +
          to_string(t) + "' already defined");
      throw(runtime_error(msg));
    }
  }  // end of MTestFileGeneratorBase::addTime

  void MTestFileGeneratorBase::setRotationMatrix(const real m00,
                                                 const real m01,
                                                 const real m02,
                                                 const real m10,
                                                 const real m11,
                                                 const real m12,
                                                 const real m20,
                                                 const real m21,
                                                 const real m22) {
    this->hasRotationMatrix = true;
    this->m[0] = m00;
    this->m[1] = m01;
    this->m[2] = m02;
    this->m[3] = m10;
    this->m[4] = m11;
    this->m[5] = m12;
    this->m[6] = m20;
    this->m[7] = m21;
    this->m[8] = m22;
  }  // end of MTestFileGeneratorBase::addTime

  void MTestFileGeneratorBase::addMaterialProperty(
      const std::string& n, const MTestFileGeneratorBase::real v) {
    using namespace std;
    if (!(this->mps.insert({n, v}).second)) {
      string msg(
          "MTestFileGeneratorBase::addMaterialProperty : "
          "material property '" +
          n + "' already defined");
      throw(runtime_error(msg));
    }
  }  // end of MTestFileGeneratorBase::addMaterialProperty

  void MTestFileGeneratorBase::addInternalStateVariable(
      const std::string& n,
      const SupportedTypes::TypeFlag f,
      const MTestFileGeneratorBase::real* const v) {
    using namespace std;
    InternalStateVariable iv;
    vector<InternalStateVariable>::const_iterator p;
    for (p = this->ivs.begin(); p != this->ivs.end(); ++p) {
      if (p->name == n) {
        string msg(
            "MTestFileGeneratorBase::addInternalStateVariable : "
            "variable already declared '" +
            n + "'");
        throw(runtime_error(msg));
      }
    }
    iv.name = n;
    iv.type = f;
    if (iv.type == SupportedTypes::Scalar) {
      iv.values[0] = v[0];
    } else if (iv.type == SupportedTypes::Stensor) {
      copy(v, v + this->getStensorSize(), iv.values);
    } else {
      string msg(
          "MTestFileGeneratorBase::addInternalStateVariable : "
          "unsupported type for variable '" +
          n + "'");
      throw(runtime_error(msg));
    }
    this->ivs.push_back(iv);
  }  // end of MTestFileGeneratorBase::addInternalStateVariable

  void MTestFileGeneratorBase::addExternalStateVariableValue(
      const std::string& n,
      const MTestFileGeneratorBase::real t,
      const MTestFileGeneratorBase::real v) {
    using namespace std;
    if (!(this->evs[n].insert({t, v}).second)) {
      string msg(
          "MTestFileGeneratorBase::addExternalStateVariableValue : "
          "time '" +
          to_string(t) +
          "' already defined "
          "for variable '" +
          n + "'");
      throw(runtime_error(msg));
    }
  }  // end of MTestFileGeneratorBase::addValue

  void MTestFileGeneratorBase::generate(const std::string& n) const {
    using namespace std;
    ofstream file(n + "-" + to_string(getIdentifier()) + ".mtest");
    if (!file) {
      string msg(
          "MTestFileGeneratorBase::generate : "
          "can't open file '" +
          n + ".mtest'");
      throw(runtime_error(msg));
    }
    file << "@MaximumNumberOfSubSteps 1;\n";
    if (this->handleThermalExpansion) {
      file << "@HandleThermalExpansion true;" << endl;
    } else {
      file << "@HandleThermalExpansion false;" << endl;
    }
    this->writeModellingHypothesis(file);
    this->writeBehaviourDeclaration(file);
    this->writeRotationMatrix(file);
    this->writeMaterialProperties(file);
    this->writeInternalStateVariables(file);
    this->writeExternalStateVariables(file);
    this->writeDrivingVariables(file);
    this->writeTimes(file);
  }  // end of MTestFileGeneratorBase::generate

  void MTestFileGeneratorBase::writeRotationMatrix(std::ostream& os) const {
    using namespace std;
    if (this->hasRotationMatrix) {
      os.precision(14);
      os << "@RotationMatrix {{" << m[0] << "," << m[1] << "," << m[2] << "},"
         << endl
         << "                 {" << m[3] << "," << m[4] << "," << m[5] << "},"
         << endl
         << "                 {" << m[6] << "," << m[7] << "," << m[8] << "}};"
         << endl
         << endl;
    }
  }

  void MTestFileGeneratorBase::writeModellingHypothesis(
      std::ostream& os) const {
    using namespace std;
    using namespace tfel::material;
    if (this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      string msg("MTestFileGeneratorBase::writeModellingHypothesis : ");
      msg += "undefined modelling hypothesis";
      throw(runtime_error(msg));
    }
    os << "@ModellingHypothesis '"
       << ModellingHypothesis::toString(this->hypothesis) << "';" << endl;
  }

  void MTestFileGeneratorBase::writeMaterialProperties(std::ostream& os) const {
    using namespace std;
    map<string, real>::const_iterator p;
    if (!this->mps.empty()) {
      os << "// Material properties" << endl;
      for (p = this->mps.begin(); p != this->mps.end(); ++p) {
        os.precision(14);
        os << "@MaterialProperty<constant> '" << p->first << "' " << p->second
           << ";" << endl;
      }
      os << endl;
    }
  }  // end of MTestFileGeneratorBase::writeMaterialProperties

  void MTestFileGeneratorBase::writeTimes(std::ostream& os) const {
    using namespace std;
    set<real>::const_iterator p;
    if (this->times.empty()) {
      string msg(
          "MTestFileGeneratorBase::writeTimes : "
          "no times defined");
      throw(runtime_error(msg));
    }
    if (this->times.size() < 2) {
      string msg(
          "MTestFileGeneratorBase::writeTimes : "
          "only one time given");
      throw(runtime_error(msg));
    }
    os << "// Times" << endl;
    os << "@Times {";
    os.precision(14);
    for (p = this->times.begin(); p != times.end();) {
      os << *p;
      if (++p != times.end()) {
        os << ", ";
      }
    }
    os << "};" << endl << endl;
  }  // end of MTestFileGeneratorBase::writeTimes

  void MTestFileGeneratorBase::writeInternalStateVariables(
      std::ostream& os) const {
    using namespace std;
    vector<InternalStateVariable>::const_iterator p;
    if (!this->ivs.empty()) {
      os << "// Internal state variables" << endl;
      for (p = this->ivs.begin(); p != this->ivs.end(); ++p) {
        const InternalStateVariable& iv = *p;
        os << "@InternalStateVariable '" << iv.name << "' ";
        os.precision(14);
        if (iv.type == SupportedTypes::Scalar) {
          os << iv.values[0] << ";" << endl;
        } else if (iv.type == SupportedTypes::Stensor) {
          os << "{";
          for (unsigned short i = 0; i != this->getStensorSize();) {
            os << iv.values[i];
            if (++i != this->getStensorSize()) {
              os << ",";
            }
          }
          os << "};" << endl;
        } else {
          string msg(
              "MTestFileGeneratorBase::writeInternalStateVariables : "
              "unsupported internal state variable type");
          throw(runtime_error(msg));
        }
      }
      os << endl;
    }
  }  // end of MTestFileGeneratorBase::writeMaterialProperties

  void MTestFileGeneratorBase::writeExternalStateVariables(
      std::ostream& os) const {
    using namespace std;
    map<string, map<real, real>>::const_iterator p;
    if (!this->evs.empty()) {
      os << "// External state variables" << endl;
      for (p = this->evs.begin(); p != this->evs.end(); ++p) {
        const auto& n = p->first;
        const auto& values = p->second;
        if (values.size() == 1) {
          os.precision(14);
          os << "@ExternalStateVariable '" << n << "' "
             << values.begin()->second << endl;
        } else {
          map<real, real>::const_iterator pv;
          os.precision(14);
          os << "@ExternalStateVariable<evolution> '" << n << "' {";
          for (pv = values.begin(); pv != values.end();) {
            os << pv->first << " : " << pv->second;
            if (++pv != values.end()) {
              os << "," << endl;
            }
          }
          os << "};" << endl;
        }
      }
      os << endl;
    }
  }  // end of MTestFileGeneratorBase::writeExternalStateVariables

  std::vector<std::string>
  MTestFileGeneratorBase::getDeformationGradientComponentsNames() const {
    using namespace std;
    using namespace tfel::material;
    const string exts[9u] = {"FXX", "FYY", "FZZ", "FXY", "FYX",
                             "FXZ", "FZX", "FYZ", "FZY"};
    const string aexts[5u] = {"FRR", "FZZ", "FTT", "FRZ", "FZR"};
    vector<string> n;
    if (this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      string msg(
          "MTestFileGeneratorBase::getDeformationGradientsComponentsNames : ");
      msg += "undefined modelling hypothesis";
      throw(runtime_error(msg));
    }
    if ((this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
        (this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      copy(aexts, aexts + 3u, back_inserter(n));
    } else if (this->hypothesis == ModellingHypothesis::AXISYMMETRICAL) {
      copy(aexts, aexts + 5u, back_inserter(n));
    } else if ((this->hypothesis == ModellingHypothesis::PLANESTRESS) ||
               (this->hypothesis == ModellingHypothesis::PLANESTRAIN) ||
               (this->hypothesis ==
                ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      copy(exts, exts + 5u, back_inserter(n));
    } else if (this->hypothesis == ModellingHypothesis::TRIDIMENSIONAL) {
      copy(exts, exts + 9u, back_inserter(n));
    } else {
      string msg(
          "MTestFileGeneratorBase::getDeformationGradientsComponentsNames : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    return n;
  }

  std::vector<std::string> MTestFileGeneratorBase::getStrainComponentsNames()
      const {
    using namespace std;
    using namespace tfel::material;
    const string exts[6u] = {"EXX", "EYY", "EZZ", "EXY", "EXZ", "EYZ"};
    const string aexts[4u] = {"ERR", "EZZ", "ETT", "ERZ"};
    vector<string> n;
    if (this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      string msg("MTestFileGeneratorBase::getStrainComponentsNames : ");
      msg += "undefined modelling hypothesis";
      throw(runtime_error(msg));
    }
    if ((this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
        (this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      copy(aexts, aexts + 3u, back_inserter(n));
    } else if (this->hypothesis == ModellingHypothesis::AXISYMMETRICAL) {
      copy(aexts, aexts + 4u, back_inserter(n));
    } else if ((this->hypothesis == ModellingHypothesis::PLANESTRESS) ||
               (this->hypothesis == ModellingHypothesis::PLANESTRAIN) ||
               (this->hypothesis ==
                ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      copy(exts, exts + 4u, back_inserter(n));
    } else if (this->hypothesis == ModellingHypothesis::TRIDIMENSIONAL) {
      copy(exts, exts + 6u, back_inserter(n));
    } else {
      string msg("MTestFileGeneratorBase::getStrainComponentsNames : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    return n;
  }

  std::vector<std::string> MTestFileGeneratorBase::getStressComponentsNames()
      const {
    using namespace std;
    using namespace tfel::material;
    const string exts[6u] = {"SXX", "SYY", "SZZ", "SXY", "SXZ", "SYZ"};
    const string aexts[4u] = {"SRR", "SZZ", "STT", "SRZ"};
    vector<string> n;
    if (this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      string msg("MTestFileGeneratorBase::getStressComponentsNames : ");
      msg += "undefined modelling hypothesis";
      throw(runtime_error(msg));
    }
    if ((this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
        (this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      copy(aexts, aexts + 3u, back_inserter(n));
    } else if (this->hypothesis == ModellingHypothesis::AXISYMMETRICAL) {
      copy(aexts, aexts + 4u, back_inserter(n));
    } else if ((this->hypothesis == ModellingHypothesis::PLANESTRESS) ||
               (this->hypothesis == ModellingHypothesis::PLANESTRAIN) ||
               (this->hypothesis ==
                ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      copy(exts, exts + 4u, back_inserter(n));
    } else if (this->hypothesis == ModellingHypothesis::TRIDIMENSIONAL) {
      copy(exts, exts + 6u, back_inserter(n));
    } else {
      string msg("MTestFileGeneratorBase::getStressComponentsNames : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    return n;
  }

  unsigned short MTestFileGeneratorBase::getTVectorSize() const {
    using namespace std;
    using namespace tfel::material;
    if (this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      string msg("MTestFileGeneratorBase::getTVectorSize : ");
      msg += "undefined modelling hypothesis";
      throw(runtime_error(msg));
    }
    if ((this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
        (this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      return 1u;
    } else if ((this->hypothesis == ModellingHypothesis::AXISYMMETRICAL) ||
               (this->hypothesis == ModellingHypothesis::PLANESTRESS) ||
               (this->hypothesis == ModellingHypothesis::PLANESTRAIN) ||
               (this->hypothesis ==
                ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      return 2u;
    } else if (this->hypothesis == ModellingHypothesis::TRIDIMENSIONAL) {
      return 3u;
    }
    throw(
        std::runtime_error("MTestFileGeneratorBase::getTVectorSize : "
                           "unsupported modelling hypothesis"));
  }  // end of MTestFileGeneratorBase::getTVectorSize

  unsigned short MTestFileGeneratorBase::getStensorSize() const {
    using namespace std;
    using namespace tfel::material;
    if (this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      string msg("MTestFileGeneratorBase::getStensorSize : ");
      msg += "undefined modelling hypothesis";
      throw(runtime_error(msg));
    }
    if ((this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
        (this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      return 3u;
    } else if ((this->hypothesis == ModellingHypothesis::AXISYMMETRICAL) ||
               (this->hypothesis == ModellingHypothesis::PLANESTRESS) ||
               (this->hypothesis == ModellingHypothesis::PLANESTRAIN) ||
               (this->hypothesis ==
                ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      return 4u;
    } else if (this->hypothesis == ModellingHypothesis::TRIDIMENSIONAL) {
      return 6u;
    }
    throw(
        std::runtime_error("MTestFileGeneratorBase::getStensorSize : "
                           "unsupported modelling hypothesis"));
  }  // end of MTestFileGeneratorBase::getStensorSize

  unsigned short MTestFileGeneratorBase::getTensorSize() const {
    using namespace std;
    using namespace tfel::material;
    if (this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      string msg("MTestFileGeneratorBase::getTensorSize : ");
      msg += "undefined modelling hypothesis";
      throw(runtime_error(msg));
    }
    if ((this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
        (this->hypothesis ==
         ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      return 3u;
    } else if ((this->hypothesis == ModellingHypothesis::AXISYMMETRICAL) ||
               (this->hypothesis == ModellingHypothesis::PLANESTRESS) ||
               (this->hypothesis == ModellingHypothesis::PLANESTRAIN) ||
               (this->hypothesis ==
                ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
      return 5u;
    } else if (this->hypothesis == ModellingHypothesis::TRIDIMENSIONAL) {
      return 9u;
    }
    throw(
        std::runtime_error("MTestFileGeneratorBase::getTensorSize : "
                           "unsupported modelling hypothesis"));
  }  // end of MTestFileGeneratorBase::getTensorSize

  MTestFileGeneratorBase::~MTestFileGeneratorBase() = default;

}  // end of namespace mfront
