/*!
 * \file   mfront/src/CastemInterface.cxx
 * \brief
 * \author Helfer Thomas
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iterator>
#include <algorithm>

#include <sstream>
#include <fstream>
#include <cstdlib>
#include <stdexcept>

#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/System.hxx"

#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/CastemInterface.hxx"

#ifndef _MSC_VER
static const char* const constexpr_c = "constexpr const";
#else
static const char* const constexpr_c = "const";
#endif

namespace mfront {

  static void writeUMATArguments(std::ostream& out,
                                 const BehaviourDescription::BehaviourType& t) {
    out << "(castem::CastemReal *const STRESS,\n"
        << " castem::CastemReal *const STATEV,\n"
        << " castem::CastemReal *const DDSDDE,\n"
        << " castem::CastemReal *const,\n"
        << " castem::CastemReal *const,\n"
        << " castem::CastemReal *const,\n"
        << " castem::CastemReal *const,\n"
        << " castem::CastemReal *const,\n"
        << " castem::CastemReal *const,\n"
        << " castem::CastemReal *const,\n";
    if (t != BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      out << " const castem::CastemReal *const STRAN,\n"
          << " const castem::CastemReal *const DSTRAN,\n";
    } else {
      out << " const castem::CastemReal *const,\n"
          << " const castem::CastemReal *const,\n";
    }
    out << " const castem::CastemReal *const,\n"
        << " const castem::CastemReal *const DTIME,\n"
        << " const castem::CastemReal *const TEMP,\n"
        << " const castem::CastemReal *const DTEMP,\n"
        << " const castem::CastemReal *const PREDEF,\n"
        << " const castem::CastemReal *const DPRED,\n"
        << " const char           *const,\n"
        << " const castem::CastemInt  *const NDI,\n"
        << " const castem::CastemInt  *const,\n"
        << " const castem::CastemInt  *const NTENS,\n"
        << " const castem::CastemInt  *const NSTATV,\n"
        << " const castem::CastemReal *const PROPS,\n"
        << " const castem::CastemInt  *const NPROPS,\n"
        << " const castem::CastemReal *const,\n"
        << " const castem::CastemReal *const DROT,\n"
        << "       castem::CastemReal *const PNEWDT,\n"
        << " const castem::CastemReal *const,\n";
    if (t == BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      out << " const castem::CastemReal *const F0,\n"
          << " const castem::CastemReal *const F1,\n";
    } else {
      out << " const castem::CastemReal *const,\n"
          << " const castem::CastemReal *const,\n";
    }
    out << " const castem::CastemInt  *const,\n"
        << " const castem::CastemInt  *const,\n"
        << " const castem::CastemInt  *const,\n"
        << " const castem::CastemInt  *const,\n"
        << " const castem::CastemInt  *const,\n"
        << "       castem::CastemInt  *const KINC,\n"
        << "const int)";
  }  // end of writeUMATArguments

  static void writeUMATArguments(std::ostream& out) {
    out << "(castem::CastemReal *const,\n"
        << " castem::CastemReal *const,\n"
        << " castem::CastemReal *const,\n"
        << " castem::CastemReal *const,\n"
        << " castem::CastemReal *const,\n"
        << " castem::CastemReal *const,\n"
        << " castem::CastemReal *const,\n"
        << " castem::CastemReal *const,\n"
        << " castem::CastemReal *const,\n"
        << " castem::CastemReal *const,\n"
        << " const castem::CastemReal *const,\n"
        << " const castem::CastemReal *const,\n"
        << " const castem::CastemReal *const,\n"
        << " const castem::CastemReal *const,\n"
        << " const castem::CastemReal *const,\n"
        << " const castem::CastemReal *const,\n"
        << " const castem::CastemReal *const,\n"
        << " const castem::CastemReal *const,\n"
        << " const char           *const,\n"
        << " const castem::CastemInt  *const,\n"
        << " const castem::CastemInt  *const,\n"
        << " const castem::CastemInt  *const,\n"
        << " const castem::CastemInt  *const,\n"
        << " const castem::CastemReal *const,\n"
        << " const castem::CastemInt  *const,\n"
        << " const castem::CastemReal *const,\n"
        << " const castem::CastemReal *const,\n"
        << "       castem::CastemReal *const,\n"
        << " const castem::CastemReal *const,\n"
        << " const castem::CastemReal *const,\n"
        << " const castem::CastemReal *const,\n"
        << " const castem::CastemInt  *const,\n"
        << " const castem::CastemInt  *const,\n"
        << " const castem::CastemInt  *const,\n"
        << " const castem::CastemInt  *const,\n"
        << " const castem::CastemInt  *const,\n"
        << "       castem::CastemInt  *const,\n"
        << "const int)";
  }  // end of writeUMATArguments

  static int getCastemModellingHypothesisIndex(
      const tfel::material::ModellingHypothesis::Hypothesis h) {
    using namespace tfel::material;
    switch (h) {
      case ModellingHypothesis::TRIDIMENSIONAL:
        return 2;
      case ModellingHypothesis::AXISYMMETRICAL:
        return 0;
      case ModellingHypothesis::PLANESTRAIN:
        return -1;
      case ModellingHypothesis::PLANESTRESS:
        return -2;
      case ModellingHypothesis::GENERALISEDPLANESTRAIN:
        return -3;
      case ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN:
        return 14;
      default:
        break;
    }
    std::ostringstream msg;
    msg << "mfront::getCastemModellingHypothesisIndex : "
        << "unsupported hypothesis";
    if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      msg << " ('" << ModellingHypothesis::toString(h) << "')";
    }
    throw(std::runtime_error(msg.str()));
  }  // end of getCastemModellingHypothesisIndex

  std::string CastemInterface::getName() { return "umat"; }

  std::string CastemInterface::getLibraryName(
      const BehaviourDescription& mb) const {
    auto lib = std::string{};
    if (mb.getLibrary().empty()) {
      if (!mb.getMaterialName().empty()) {
        lib = "Umat" + mb.getMaterialName();
      } else {
        lib = "UmatBehaviour";
      }
    } else {
      lib = "Umat" + mb.getLibrary();
    }
    return lib;
  }  // end of CastemInterface::getLibraryName

  std::string CastemInterface::getInterfaceName() const {
    return "Castem";
  }  // end of CastemInterface::getInterfaceName

  std::string CastemInterface::getFunctionName(const std::string& name) const {
    return "umat" + makeLowerCase(name);
  }  // end of CastemInterface::getLibraryName

  std::string CastemInterface::getBehaviourName(
      const BehaviourDescription& mb) const {
    return mb.getLibrary() + mb.getClassName();
  }  // end of CastemInterface::getBehaviourName

  std::string CastemInterface::getUmatFunctionName(
      const BehaviourDescription& mb) const {
    return "umat" + makeLowerCase(this->getBehaviourName(mb));
  }  // end of CastemInterface::getUmatFunctionName

  CastemInterface::CastemInterface() {}

  std::pair<bool, CastemInterface::tokens_iterator>
  CastemInterface::treatKeyword(BehaviourDescription&,
                                const std::string& key,
                                const std::vector<std::string>& i,
                                tokens_iterator current,
                                const tokens_iterator end) {
    using tfel::utilities::CxxTokenizer;
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error("CastemInterface::treatKeyword : " + m));
      }
    };
    if (!i.empty()) {
      if ((std::find(i.begin(), i.end(), this->getName()) != i.end()) ||
          (std::find(i.begin(), i.end(), "castem") != i.end()) ||
          (std::find(i.begin(), i.end(), "Castem") != i.end()) ||
          (std::find(i.begin(), i.end(), "Cast3M") != i.end())) {
        throw_if((key != "@CastemGenerateMTestFileOnFailure") &&
                     (key != "@UMATGenerateMTestFileOnFailure") &&
                     (key != "@CastemUseTimeSubStepping") &&
                     (key != "@UMATUseTimeSubStepping") &&
                     (key != "@CastemMaximumSubStepping") &&
                     (key != "@UMATMaximumSubStepping") &&
                     (key != "@CastemDoSubSteppingOnInvalidResults") &&
                     (key != "@UMATDoSubSteppingOnInvalidResults") &&
                     (key != "@CastemFiniteStrainStrategy") &&
                     (key != "@UMATFiniteStrainStrategy") &&
                     (key != "@CastemFiniteStrainStrategies") &&
                     (key != "@UMATFiniteStrainStrategies"),
                 "unsupported keyword '" + key + "'");
      } else {
        return {false, current};
      }
    }
    if ((key == "@CastemGenerateMTestFileOnFailure") ||
        (key == "@UMATGenerateMTestFileOnFailure")) {
      this->generateMTestFile = this->readBooleanValue(key, current, end);
      return {true, current};
    } else if ((key == "@CastemUseTimeSubStepping") ||
               (key == "@UMATUseTimeSubStepping")) {
      this->useTimeSubStepping = this->readBooleanValue(key, current, end);
      return {true, current};
    } else if ((key == "@CastemMaximumSubStepping") ||
               (key == "@UMATMaximumSubStepping")) {
      throw_if(!this->useTimeSubStepping,
               "time sub stepping is not enabled at this stage.\n"
               "Use the @CastemUseTimeSubStepping directive before "
               "@CastemMaximumSubStepping");
      throw_if(current == end, "unexpected end of file");
      std::istringstream flux(current->value);
      flux >> this->maximumSubStepping;
      throw_if(flux.fail(), "failed to read maximum substepping value.");
      throw_if(++current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';', read '" + current->value + '\'');
      ++(current);
      return {true, current};
    } else if ((key == "@CastemDoSubSteppingOnInvalidResults") ||
               (key == "@UMATDoSubSteppingOnInvalidResults")) {
      throw_if(!this->useTimeSubStepping,
               "time sub stepping is not enabled at this stage.\n"
               "Use the @CastemUseTimeSubStepping directive before "
               "@CastemMaximumSubStepping");
      this->doSubSteppingOnInvalidResults =
          this->readBooleanValue(key, current, end);
      return {true, current};
    } else if ((key == "@CastemFiniteStrainStrategy") ||
               (key == "@UMATFiniteStrainStrategy")) {
      throw_if(!this->finiteStrainStrategies.empty(),
               "at least one strategy has already been defined");
      throw_if(current == end, "unexpected end of file");
      const auto& fss = current->value;
      if (fss == "FiniteRotationSmallStrain") {
        this->finiteStrainStrategies.push_back(FINITEROTATIONSMALLSTRAIN);
      } else if (fss == "MieheApelLambrechtLogarithmicStrain") {
        this->finiteStrainStrategies.push_back(
            MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN);
      } else if (fss == "LogarithmicStrain1D") {
        this->finiteStrainStrategies.push_back(LOGARITHMICSTRAIN1D);
      } else {
        throw_if(true, "unsupported strategy '" + fss +
                           "'\n"
                           "The only supported strategies are "
                           "'FiniteRotationSmallStrain', "
                           "'MieheApelLambrechtLogarithmicStrain' and "
                           "'LogarithmicStrain1D'");
      }
      throw_if(++current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';', read '" + current->value + '\'');
      ++(current);
      return {true, current};
    } else if ((key == "@CastemFiniteStrainStrategies") ||
               (key == "@UMATFiniteStrainStrategies")) {
      throw_if(!this->finiteStrainStrategies.empty(),
               "at least one strategy has already been defined");
      auto fss = std::vector<std::string>{};
      CxxTokenizer::readArray(
          "CastemInterface::treatKeyword (@CastemFiniteStrainStrategies)", fss,
          current, end);
      CxxTokenizer::readSpecifiedToken(
          "CastemInterface::treatKeyword (@CastemFiniteStrainStrategies)", ";",
          current, end);
      throw_if(fss.empty(), "no strategy defined");
      for (const auto& fs : fss) {
        if (fs == "None") {
          throw_if(find(this->finiteStrainStrategies.begin(),
                        this->finiteStrainStrategies.end(),
                        NONE) != this->finiteStrainStrategies.end(),
                   "strategy 'None' multiply defined");
          this->finiteStrainStrategies.push_back(NONE);
        } else if (fs == "FiniteRotationSmallStrain") {
          throw_if(find(this->finiteStrainStrategies.begin(),
                        this->finiteStrainStrategies.end(),
                        FINITEROTATIONSMALLSTRAIN) !=
                       this->finiteStrainStrategies.end(),
                   "strategy 'FiniteRotationSmallStrain' multiply defined");
          this->finiteStrainStrategies.push_back(FINITEROTATIONSMALLSTRAIN);
        } else if (fs == "MieheApelLambrechtLogarithmicStrain") {
          throw_if(find(this->finiteStrainStrategies.begin(),
                        this->finiteStrainStrategies.end(),
                        MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN) !=
                       this->finiteStrainStrategies.end(),
                   "strategy 'MieheApelLambrechtLogarithmicStrain' multiply "
                   "defined");
          this->finiteStrainStrategies.push_back(
              MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN);
        } else if (fs == "LogarithmicStrain1D") {
          throw_if(
              find(this->finiteStrainStrategies.begin(),
                   this->finiteStrainStrategies.end(),
                   LOGARITHMICSTRAIN1D) != this->finiteStrainStrategies.end(),
              "strategy 'LogarithmicStrain1D' multiply defined");
          this->finiteStrainStrategies.push_back(LOGARITHMICSTRAIN1D);
        } else {
          throw_if(true, "unsupported strategy '" + fs +
                             "'\n"
                             "The only supported strategies are "
                             "'None', 'FiniteRotationSmallStrain', "
                             "'MieheApelLambrechtLogarithmicStrain' and "
                             "'LogarithmicStrain1D'");
        }
      }
      return {true, current};
    }
    return {false, current};
  }  // end of treatKeyword

  bool CastemInterface::usesGenericPlaneStressAlgorithm(
      const BehaviourDescription& mb) {
    if (mb.isModellingHypothesisSupported(ModellingHypothesis::PLANESTRESS)) {
      return false;
    }
    return mb.isModellingHypothesisSupported(
        ModellingHypothesis::GENERALISEDPLANESTRAIN);
  }

  std::string CastemInterface::treatScalar(const std::string& s) {
    return "'" + makeUpperCase(s.substr(0, 4)) + "'";
  }

  std::string CastemInterface::treatScalar(const std::string& s,
                                           const unsigned short a) {
    if (a < 9) {
      return "'" + makeUpperCase(s.substr(0, 3)) + std::to_string(a) + "'";
    }
    return "'" + makeUpperCase(s.substr(0, 2)) + std::to_string(a) + "'";
  }

  std::string CastemInterface::treatStensor(const Hypothesis h,
                                            const std::string& s) {
    auto res = std::string{};
    const auto s2 = makeUpperCase(s.substr(0, 2));
    switch (h) {
      case ModellingHypothesis::TRIDIMENSIONAL:
        res = "'" + s2 + "XX' " + "'" + s2 + "YY' " + "'" + s2 + "ZZ' " + "'" +
              s2 + "XY' " + "'" + s2 + "XZ' " + "'" + s2 + "YZ'";
        break;
      case ModellingHypothesis::AXISYMMETRICAL:
        res = "'" + s2 + "RR' " + "'" + s2 + "ZZ' " + "'" + s2 + "TT' " + "'" +
              s2 + "RZ'";
        break;
      case ModellingHypothesis::PLANESTRAIN:
      case ModellingHypothesis::PLANESTRESS:
      case ModellingHypothesis::GENERALISEDPLANESTRAIN:
        res = "'" + s2 + "XX' " + "'" + s2 + "YY' " + "'" + s2 + "ZZ' " + "'" +
              s2 + "XY'";
        break;
      case ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN:
        res = "'" + s2 + "RR' " + "'" + s2 + "ZZ' " + "'" + s2 + "TT'";
        break;
      default:
        auto msg = std::string{};
        msg += "CastemInterface::treatStensor : unsupported hypothesis";
        if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
          msg += " ('" + ModellingHypothesis::toString(h) + "')";
        }
        throw(std::runtime_error(msg));
    }
    return res;
  }

  std::string CastemInterface::treatStensor(const Hypothesis h,
                                            const std::string& s,
                                            const unsigned short a) {
    auto res = std::string{};
    std::ostringstream stmp;
    stmp << a;
    const auto s2 = makeUpperCase(s.substr(0, 1)) + stmp.str();
    switch (h) {
      case ModellingHypothesis::TRIDIMENSIONAL:
        res = "'" + s2 + "XX' " + "'" + s2 + "YY' " + "'" + s2 + "ZZ' " + "'" +
              s2 + "XY' " + "'" + s2 + "XZ' " + "'" + s2 + "YZ'";
        break;
      case ModellingHypothesis::AXISYMMETRICAL:
        res = "'" + s2 + "RR' " + "'" + s2 + "ZZ' " + "'" + s2 + "TT' " + "'" +
              s2 + "RZ'";
        break;
      case ModellingHypothesis::PLANESTRAIN:
      case ModellingHypothesis::PLANESTRESS:
      case ModellingHypothesis::GENERALISEDPLANESTRAIN:
        res = "'" + s2 + "XX' " + "'" + s2 + "YY' " + "'" + s2 + "ZZ' " + "'" +
              s2 + "XY'";
        break;
      case ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN:
        res = "'" + s2 + "RR' " + "'" + s2 + "ZZ' " + "'" + s2 + "TT'";
        break;
      default:
        auto msg = std::string{};
        msg += "CastemInterface::treatStensor : unsupported hypothesis";
        if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
          msg += " ('" + ModellingHypothesis::toString(h) + "')";
        }
        throw(std::runtime_error(msg));
    }
    return res;
  }

  std::string CastemInterface::treatTensor(const Hypothesis h,
                                           const std::string& s) {
    auto res = std::string{};
    const auto s2 = makeUpperCase(s.substr(0, 2));
    switch (h) {
      case ModellingHypothesis::TRIDIMENSIONAL:
        res = "'" + s2 + "XX' '" + s2 + "YY' '" + s2 + "ZZ' '" + s2 + "XY' '" +
              s2 + "YX' '" + s2 + "XZ' '" + s2 + "ZX' '" + s2 + "YZ' '" + s2 +
              "ZY'";
        break;
      case ModellingHypothesis::AXISYMMETRICAL:
        res = "'" + s2 + "RR' '" + s2 + "ZZ' '" + s2 + "TT' '" + s2 + "RZ' '" +
              s2 + "ZR'";
        break;
      case ModellingHypothesis::PLANESTRAIN:
      case ModellingHypothesis::PLANESTRESS:
      case ModellingHypothesis::GENERALISEDPLANESTRAIN:
        res = "'" + s2 + "XX' '" + s2 + "YY' '" + s2 + "ZZ' '" + s2 + "XY' '" +
              s2 + "YX'";
        break;
      case ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN:
        res = "'" + s2 + "RR' '" + s2 + "ZZ' '" + s2 + "TT'";
        break;
      default:
        auto msg = std::string{};
        msg += "CastemInterface::treatTensor : unsupported hypothesis";
        if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
          msg += " ('" + ModellingHypothesis::toString(h) + "')";
        }
        throw(std::runtime_error(msg));
    }
    return res;
  }

  std::string CastemInterface::treatTensor(const Hypothesis h,
                                           const std::string& s,
                                           const unsigned short a) {
    auto res = std::string{};
    std::ostringstream stmp;
    stmp << a;
    const auto s2 = makeUpperCase(s.substr(0, 1)) + stmp.str();
    switch (h) {
      case ModellingHypothesis::TRIDIMENSIONAL:
        res = "'" + s2 + "XX' '" + s2 + "YY' '" + s2 + "ZZ' '" + s2 + "XY' '" +
              s2 + "YX' '" + s2 + "XZ' '" + s2 + "ZX' '" + s2 + "YZ' '" + s2 +
              "ZY'";
        break;
      case ModellingHypothesis::AXISYMMETRICAL:
        res = "'" + s2 + "RR' '" + s2 + "ZZ' '" + s2 + "TT' '" + s2 + "RZ' '" +
              s2 + "ZR'";
        break;
      case ModellingHypothesis::PLANESTRAIN:
      case ModellingHypothesis::PLANESTRESS:
      case ModellingHypothesis::GENERALISEDPLANESTRAIN:
        res = "'" + s2 + "XX' '" + s2 + "YY' '" + s2 + "ZZ' '" + s2 + "XY' '" +
              s2 + "YX'";
        break;
      case ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN:
        res = "'" + s2 + "RR' '" + s2 + "ZZ' '" + s2 + "TT'";
        break;
      default:
        auto msg = std::string{};
        msg += "CastemInterface::treatTensor : unsupported hypothesis";
        if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
          msg += " ('" + ModellingHypothesis::toString(h) + "')";
        }
        throw(std::runtime_error(msg));
    }
    return res;
  }

  std::set<CastemInterface::Hypothesis>
  CastemInterface::getModellingHypothesesToBeTreated(
      const BehaviourDescription& mb) const {
    using Hypothesis = ModellingHypothesis::Hypothesis;
    const Hypothesis sh[6u] = {
        ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
        ModellingHypothesis::AXISYMMETRICAL,
        ModellingHypothesis::PLANESTRESS,
        ModellingHypothesis::PLANESTRAIN,
        ModellingHypothesis::GENERALISEDPLANESTRAIN,
        ModellingHypothesis::TRIDIMENSIONAL};
    // treatment
    std::set<Hypothesis> mh;
    // modelling hypotheses handled by the behaviour
    const auto& bh = mb.getModellingHypotheses();
    for (const auto& h : sh) {
      if (bh.find(h) != bh.end()) {
        mh.insert(h);
      }
    }
    if (mh.empty()) {
      throw(std::runtime_error(
          "CastemInterface::getModellingHypothesesToBeTreated : "
          "no hypotheses selected. This means that the given beahviour "
          "can't be used neither in 'AxisymmetricalGeneralisedPlaneStrain' "
          "nor in 'AxisymmetricalGeneralisedPlaneStress', so it does not "
          "make sense to use the Castem interface"));
    }
    return mh;
  }  // end of CastemInterfaceModellingHypothesesToBeTreated

  void CastemInterface::writeGetOutOfBoundsPolicyFunctionImplementation(
      std::ostream& out, const std::string& name) const {
    out << "static tfel::material::OutOfBoundsPolicy&\n"
        << getFunctionName(name) << "_getOutOfBoundsPolicy(void){\n"
        << "using namespace castem;\n"
        << "using namespace tfel::material;\n"
        << "static OutOfBoundsPolicy policy = "
           "CastemOutOfBoundsPolicy::getCastemOutOfBoundsPolicy()."
           "getOutOfBoundsPolicy();\n"
        << "return policy;\n"
        << "}\n\n";
  }  // end of
     // MFrontCastemInterface::writeGetOutOfBoundsPolicyFunctionImplementation

  void CastemInterface::writeSetOutOfBoundsPolicyFunctionImplementation2(
      std::ostream& out,
      const std::string& name,
      const std::string& name2) const {
    out << "MFRONT_SHAREDOBJ void\n"
        << this->getFunctionName(name2)
        << "_setOutOfBoundsPolicy(const int p){\n"
        << this->getFunctionName(name) << "_setOutOfBoundsPolicy(p);\n"
        << "}\n\n";
  }

  void CastemInterface::endTreatment(const BehaviourDescription& mb,
                                     const FileDescription& fd) const {
    using namespace std;
    using namespace tfel::system;
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error("CastemInterface::endTreatment: " + m));
      }
    };
    // get the modelling hypotheses to be treated
    const auto& mh = this->getModellingHypothesesToBeTreated(mb);
    // some consistency checks
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      throw_if(
          mb.getSymmetryType() != mb.getElasticSymmetryType(),
          "the type of the behaviour (isotropic or orthotropic) does not "
          "match the the type of its elastic behaviour.\n"
          "This is not allowed here:\n"
          "- an isotropic behaviour must have an isotropic elastic behaviour\n"
          "- an orthotropic behaviour must have an orthotropic elastic "
          "behaviour");
    }
    if (this->useTimeSubStepping) {
      throw_if(
          this->maximumSubStepping == 0u,
          "use of time sub stepping requested but MaximumSubStepping is zero.\n"
          "Please use the @CastemMaximumSubStepping directive");
    }
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/Castem");

    vector<FiniteStrainStrategy>::const_iterator pfss;

    const auto name = this->getBehaviourName(mb);

    auto fileName = "umat" + name + ".hxx";

    std::ofstream out("include/MFront/Castem/" + fileName);
    throw_if(!out, "could not open file '" + fileName + "'");

    out << "/*!\n";
    out << "* \\file   " << fileName << '\n';
    out << "* \\brief  This file declares the umat interface for the "
        << mb.getClassName() << " behaviour law\n";
    out << "* \\author " << fd.authorName << '\n';
    out << "* \\date   " << fd.date << '\n';
    out << "*/\n\n";

    const string header = this->getHeaderDefine(mb);
    out << "#ifndef " << header << "\n";
    out << "#define " << header << "\n\n";

    out << "#include\"castem.h\"\n";
    out << "#ifdef umat\n";
    out << "#undef umat\n";
    out << "#endif /* umat */\n\n";

    out << "#include\"TFEL/Config/TFELConfig.hxx\"\n\n";
    out << "#include\"MFront/Castem/Castem.hxx\"\n\n";

    out << "#ifdef __cplusplus\n";
    out << "#include\"MFront/Castem/CastemTraits.hxx\"\n";
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "#include\"MFront/Castem/CastemOrthotropicBehaviour.hxx\"\n";
    }
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    out << "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);

    out << "#ifdef __cplusplus\n\n";

    out << "namespace castem{\n\n";

    if (!mb.areAllMechanicalDataSpecialised(mh)) {
      this->writeUMATBehaviourTraits(out, mb,
                                     ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for (const auto& h : mh) {
      if (mb.hasSpecialisedMechanicalData(h)) {
        this->writeUMATBehaviourTraits(out, mb, h);
      }
    }

    out << "} // end of namespace castem\n\n";

    out << "#endif /* __cplusplus */\n\n";

    out << "#ifdef __cplusplus\n";
    out << "extern \"C\"{\n";
    out << "#endif /* __cplusplus */\n\n";

    this->writeSetParametersFunctionsDeclarations(out, name, mb);
    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out, name);
    if ((mb.getBehaviourType() ==
         BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
        (!this->finiteStrainStrategies.empty())) {
      for (pfss = this->finiteStrainStrategies.begin();
           pfss != this->finiteStrainStrategies.end(); ++pfss) {
        if (*pfss == FINITEROTATIONSMALLSTRAIN) {
          this->writeSetParametersFunctionsDeclarations(out, name + "_frst",
                                                        mb);
          this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,
                                                             name + "_frst");
        } else if (*pfss == MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN) {
          this->writeSetParametersFunctionsDeclarations(out, name + "_malls",
                                                        mb);
          this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,
                                                             name + "_malls");
        } else if (*pfss == LOGARITHMICSTRAIN1D) {
          this->writeSetParametersFunctionsDeclarations(out, name + "_log1D",
                                                        mb);
          this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,
                                                             name + "_log1D");
        } else if (*pfss == NONE) {
          this->writeSetParametersFunctionsDeclarations(out, name + "_ss", mb);
          this->writeSetOutOfBoundsPolicyFunctionDeclaration(out, name + "_ss");
        } else {
          throw_if(true, "internal error, unsupported finite strain strategy");
        }
      }
    }

    if (mb.getBehaviourType() ==
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      if (this->finiteStrainStrategies.empty()) {
        this->writeCastemFunctionDeclaration(out, name);
      } else {
        for (pfss = this->finiteStrainStrategies.begin();
             pfss != this->finiteStrainStrategies.end(); ++pfss) {
          if (*pfss == FINITEROTATIONSMALLSTRAIN) {
            this->writeCastemFunctionDeclaration(out, name + "_frst");
            if (this->finiteStrainStrategies.size() == 1u) {
              this->writeCastemFunctionDeclaration(out, name);
            }
          } else if (*pfss == MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN) {
            this->writeCastemFunctionDeclaration(out, name + "_malls");
            if (this->finiteStrainStrategies.size() == 1u) {
              this->writeCastemFunctionDeclaration(out, name);
            }
          } else if (*pfss == LOGARITHMICSTRAIN1D) {
            this->writeCastemFunctionDeclaration(out, name + "_log1D");
            if (this->finiteStrainStrategies.size() == 1u) {
              this->writeCastemFunctionDeclaration(out, name);
            }
          } else if (*pfss == NONE) {
            this->writeCastemFunctionDeclaration(out, name + "_ss");
            if (this->finiteStrainStrategies.size() == 1u) {
              this->writeCastemFunctionDeclaration(out, name);
            }
          } else {
            throw_if(true,
                     "internal error, unsupported finite strain strategy");
          }
        }
        if ((this->finiteStrainStrategies.size() != 1u) &&
            (find(this->finiteStrainStrategies.begin(),
                  this->finiteStrainStrategies.end(),
                  NONE) != this->finiteStrainStrategies.end())) {
          this->writeCastemFunctionDeclaration(out, name);
        }
      }
    } else {
      this->writeCastemFunctionDeclaration(out, name);
    }

    out << "#ifdef __cplusplus\n";
    out << "}\n";
    out << "#endif /* __cplusplus */\n\n";

    out << "#endif /* " << header << " */\n";

    out.close();

    fileName = "umat";
    fileName += name;
    fileName += ".cxx";

    out.open("src/" + fileName);
    if (!out) {
      throw(
          runtime_error("CastemInterface::endTreatment: "
                        "could not open file '" +
                        fileName + "'"));
    }

    out << "/*!\n";
    out << "* \\file   " << fileName << '\n';
    out << "* \\brief  This file implements the umat interface for the "
        << mb.getClassName() << " behaviour law\n";
    out << "* \\author " << fd.authorName << '\n';
    out << "* \\date   " << fd.date << '\n';
    out << "*/\n\n";

    this->getExtraSrcIncludes(out, mb);
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "#include\"MFront/BehaviourProfiler.hxx\"\n\n";
    }
    if (this->generateMTestFile) {
      throw_if(
          (mb.getBehaviourType() !=
           BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
              (mb.getBehaviourType() !=
               BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR),
          "MTest file generation is not unsupported for this behaviour type");
      out << "#include\"MFront/Castem/CastemGetModellingHypothesis.hxx\"\n";
    }
    if ((find(this->finiteStrainStrategies.begin(),
              this->finiteStrainStrategies.end(), FINITEROTATIONSMALLSTRAIN) !=
         this->finiteStrainStrategies.end()) ||
        (find(this->finiteStrainStrategies.begin(),
              this->finiteStrainStrategies.end(),
              MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN) !=
         this->finiteStrainStrategies.end())) {
      out << "#include\"MFront/Castem/CastemFiniteStrain.hxx\"\n\n";
    }
    out << "#include\"MFront/Castem/CastemOutOfBoundsPolicy.hxx\"\n";
    out << "#include\"MFront/Castem/CastemInterface.hxx\"\n\n";
    out << "#include\"MFront/Castem/CastemStressFreeExpansionHandler.hxx\"\n\n";
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    out << "#include\"MFront/Castem/umat" << name << ".hxx\"\n\n";

    this->writeGetOutOfBoundsPolicyFunctionImplementation(out, name);

    out << "extern \"C\"{\n\n";

    if (mb.getBehaviourType() ==
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      if (this->finiteStrainStrategies.empty()) {
        this->generateUMATxxGeneralSymbols(out, name, mb, fd);
        UMATInterfaceBase::writeUMATxxSupportedModellingHypothesis(out, name,
                                                                   mb);
        if (!mb.areAllMechanicalDataSpecialised(mh)) {
          const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
          this->generateUMATxxSymbols(out, name, uh, mb, fd);
        }
        for (const auto& h : mh) {
          if (mb.hasSpecialisedMechanicalData(h)) {
            this->generateUMATxxSymbols(out, name, h, mb, fd);
          }
        }
        out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
            << "_BehaviourType = 1u;\n\n";
        out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
            << "_BehaviourKinematic = 1u;\n\n";
        out << "MFRONT_SHAREDOBJ unsigned short umat" << makeLowerCase(name)
            << "_Interface = 1u;\n\n";
      } else {
        for (const auto fss : this->finiteStrainStrategies) {
          if (fss == FINITEROTATIONSMALLSTRAIN) {
            this->generateUMATxxGeneralSymbols(out, name + "_frst", mb, fd);
            UMATInterfaceBase::writeUMATxxSupportedModellingHypothesis(
                out, name + "_frst", mb);
            if (!mb.areAllMechanicalDataSpecialised(mh)) {
              const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
              this->generateUMATxxSymbols(out, name + "_frst", uh, mb, fd);
            }
            for (const auto& h : mh) {
              if (mb.hasSpecialisedMechanicalData(h)) {
                this->generateUMATxxSymbols(out, name + "_frst", h, mb, fd);
              }
            }
            out << "MFRONT_SHAREDOBJ unsigned short "
                << this->getFunctionName(name + "_frst")
                << "_BehaviourType = 2u;\n\n";
            out << "MFRONT_SHAREDOBJ unsigned short "
                << this->getFunctionName(name + "_frst")
                << "_BehaviourKinematic = 3u;\n\n";
            out << "MFRONT_SHAREDOBJ unsigned short umat"
                << makeLowerCase(name + "_frst") << "_Interface = 2u;\n\n";
            if (this->finiteStrainStrategies.size() == 1u) {
              this->generateUMATxxGeneralSymbols(out, name, mb, fd);
              UMATInterfaceBase::writeUMATxxSupportedModellingHypothesis(
                  out, name, mb);
              if (!mb.areAllMechanicalDataSpecialised(mh)) {
                const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
                this->generateUMATxxSymbols(out, name, uh, mb, fd);
              }
              for (const auto& h : mh) {
                if (mb.hasSpecialisedMechanicalData(h)) {
                  this->generateUMATxxSymbols(out, name, h, mb, fd);
                }
              }
              out << "MFRONT_SHAREDOBJ unsigned short "
                  << this->getFunctionName(name) << "_BehaviourType = 2u;\n\n";
              out << "MFRONT_SHAREDOBJ unsigned short "
                  << this->getFunctionName(name)
                  << "_BehaviourKinematic = 3u;\n\n";
              out << "MFRONT_SHAREDOBJ unsigned short umat"
                  << makeLowerCase(name) << "_Interface = 2u;\n\n";
            }
          } else if (fss == MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN) {
            this->generateUMATxxGeneralSymbols(out, name + "_malls", mb, fd);
            UMATInterfaceBase::writeUMATxxSupportedModellingHypothesis(
                out, name + "_malls", mb);
            if (!mb.areAllMechanicalDataSpecialised(mh)) {
              const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
              this->generateUMATxxSymbols(out, name + "_malls", uh, mb, fd);
            }
            for (const auto& h : mh) {
              if (mb.hasSpecialisedMechanicalData(h)) {
                this->generateUMATxxSymbols(out, name + "_malls", h, mb, fd);
              }
            }
            out << "MFRONT_SHAREDOBJ unsigned short "
                << this->getFunctionName(name + "_malls")
                << "_BehaviourType = 2u;\n\n";
            out << "MFRONT_SHAREDOBJ unsigned short "
                << this->getFunctionName(name + "_malls")
                << "_BehaviourKinematic = 3u;\n\n";
            out << "MFRONT_SHAREDOBJ unsigned short umat"
                << makeLowerCase(name + "_malls") << "_Interface = 2u;\n\n";
            if (this->finiteStrainStrategies.size() == 1u) {
              this->generateUMATxxGeneralSymbols(out, name, mb, fd);
              UMATInterfaceBase::writeUMATxxSupportedModellingHypothesis(
                  out, name, mb);
              if (!mb.areAllMechanicalDataSpecialised(mh)) {
                const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
                this->generateUMATxxSymbols(out, name, uh, mb, fd);
              }
              for (const auto& h : mh) {
                if (mb.hasSpecialisedMechanicalData(h)) {
                  this->generateUMATxxSymbols(out, name, h, mb, fd);
                }
              }
              out << "MFRONT_SHAREDOBJ unsigned short "
                  << this->getFunctionName(name) << "_BehaviourType = 2u;\n\n";
              out << "MFRONT_SHAREDOBJ unsigned short "
                  << this->getFunctionName(name)
                  << "_BehaviourKinematic = 3u;\n\n";
              out << "MFRONT_SHAREDOBJ unsigned short umat"
                  << makeLowerCase(name) << "_Interface = 2u;\n\n";
            }
          } else if (fss == LOGARITHMICSTRAIN1D) {
            const auto agps =
                ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN;
            this->generateUMATxxGeneralSymbols(out, name + "_log1D", mb, fd);
            if (!mb.areAllMechanicalDataSpecialised(mh)) {
              const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
              this->generateUMATxxSymbols(out, name + "_log1D", uh, mb, fd);
            }
            for (const auto& h : mh) {
              if (h == agps) {
                if (mb.hasSpecialisedMechanicalData(h)) {
                  this->generateUMATxxSymbols(out, name + "_log1D", h, mb, fd);
                }
              }
            }
            out << "MFRONT_SHAREDOBJ unsigned short "
                << this->getFunctionName(name + "_log1D")
                << "_BehaviourType = 2u;\n\n";
            out << "MFRONT_SHAREDOBJ unsigned short "
                << this->getFunctionName(name + "_log1D")
                << "_BehaviourKinematic = 4u;\n\n";
            out << "MFRONT_SHAREDOBJ unsigned short umat"
                << makeLowerCase(name + "_log1D") << "_Interface = 1u;\n\n";
            out << "MFRONT_SHAREDOBJ unsigned short "
                << this->getFunctionName(name + "_log1D")
                << "_nModellingHypotheses = " << 1u << "u;\n\n";
            out << "MFRONT_SHAREDOBJ const char * \n"
                << this->getFunctionName(name + "_log1D")
                << "_ModellingHypotheses[1u] = {\""
                << ModellingHypothesis::toString(agps) << "\"};\n";
            if (this->finiteStrainStrategies.size() == 1u) {
              this->generateUMATxxGeneralSymbols(out, name, mb, fd);
              out << "MFRONT_SHAREDOBJ unsigned short "
                  << this->getFunctionName(name)
                  << "_nModellingHypotheses = " << 1u << "u;\n\n";
              out << "MFRONT_SHAREDOBJ const char * \n"
                  << this->getFunctionName(name)
                  << "_ModellingHypotheses[1u] = {\""
                  << ModellingHypothesis::toString(agps) << "\"};\n";
              if (!mb.areAllMechanicalDataSpecialised(mh)) {
                const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
                this->generateUMATxxSymbols(out, name, uh, mb, fd);
              }
              for (const auto& h : mh) {
                if (mb.hasSpecialisedMechanicalData(h)) {
                  this->generateUMATxxSymbols(out, name, h, mb, fd);
                }
              }
              out << "MFRONT_SHAREDOBJ unsigned short "
                  << this->getFunctionName(name) << "_BehaviourType = 2u;\n\n";
              out << "MFRONT_SHAREDOBJ unsigned short "
                  << this->getFunctionName(name)
                  << "_BehaviourKinematic = 4u;\n\n";
              out << "MFRONT_SHAREDOBJ unsigned short umat"
                  << makeLowerCase(name) << "_Interface = 1u;\n\n";
            }
          } else if (fss == NONE) {
            this->generateUMATxxGeneralSymbols(out, name + "_ss", mb, fd);
            UMATInterfaceBase::writeUMATxxSupportedModellingHypothesis(
                out, name + "_ss", mb);
            if (!mb.areAllMechanicalDataSpecialised(mh)) {
              const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
              this->generateUMATxxSymbols(out, name + "_ss", uh, mb, fd);
            }
            for (const auto& h : mh) {
              if (mb.hasSpecialisedMechanicalData(h)) {
                this->generateUMATxxSymbols(out, name + "_ss", h, mb, fd);
              }
            }
            out << "MFRONT_SHAREDOBJ unsigned short "
                << this->getFunctionName(name + "_ss")
                << "_BehaviourType = 1u;\n\n";
            out << "MFRONT_SHAREDOBJ unsigned short "
                << this->getFunctionName(name + "_ss")
                << "_BehaviourKinematic = 1u;\n\n";
            out << "MFRONT_SHAREDOBJ unsigned short umat"
                << makeLowerCase(name + "_ss") << "_Interface = 1u;\n\n";
            if (this->finiteStrainStrategies.size() == 1u) {
              this->generateUMATxxGeneralSymbols(out, name, mb, fd);
              UMATInterfaceBase::writeUMATxxSupportedModellingHypothesis(
                  out, name, mb);
              if (!mb.areAllMechanicalDataSpecialised(mh)) {
                const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
                this->generateUMATxxSymbols(out, name, uh, mb, fd);
              }
              for (const auto& h : mh) {
                if (mb.hasSpecialisedMechanicalData(h)) {
                  this->generateUMATxxSymbols(out, name, h, mb, fd);
                }
              }
              out << "MFRONT_SHAREDOBJ unsigned short "
                  << this->getFunctionName(name) << "_BehaviourType = 1u;\n\n";
              out << "MFRONT_SHAREDOBJ unsigned short "
                  << this->getFunctionName(name)
                  << "_BehaviourKinematic = 1u;\n\n";
              out << "MFRONT_SHAREDOBJ unsigned short umat"
                  << makeLowerCase(name) << "_Interface = 1u;\n\n";
            }
          } else {
            throw_if(true,
                     "internal error, unsupported finite strain strategy");
          }
        }
        if ((this->finiteStrainStrategies.size() != 1u) &&
            (find(this->finiteStrainStrategies.begin(),
                  this->finiteStrainStrategies.end(),
                  NONE) != this->finiteStrainStrategies.end())) {
          this->generateUMATxxGeneralSymbols(out, name, mb, fd);
          UMATInterfaceBase::writeUMATxxSupportedModellingHypothesis(out, name,
                                                                     mb);
          if (!mb.areAllMechanicalDataSpecialised(mh)) {
            const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
            this->generateUMATxxSymbols(out, name, uh, mb, fd);
          }
          for (const auto& h : mh) {
            if (mb.hasSpecialisedMechanicalData(h)) {
              this->generateUMATxxSymbols(out, name, h, mb, fd);
            }
          }
          out << "MFRONT_SHAREDOBJ unsigned short "
              << this->getFunctionName(name) << "_BehaviourType = 1u;\n\n";
          out << "MFRONT_SHAREDOBJ unsigned short "
              << this->getFunctionName(name) << "_BehaviourKinematic = 1u;\n\n";
          out << "MFRONT_SHAREDOBJ unsigned short umat" << makeLowerCase(name)
              << "_Interface = 1u;\n\n";
        }
      }
    } else {
      this->generateUMATxxGeneralSymbols(out, name, mb, fd);
      UMATInterfaceBase::writeUMATxxSupportedModellingHypothesis(out, name, mb);
      if (!mb.areAllMechanicalDataSpecialised(mh)) {
        const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
        this->generateUMATxxSymbols(out, name, uh, mb, fd);
      }
      for (const auto& h : mh) {
        if (mb.hasSpecialisedMechanicalData(h)) {
          this->generateUMATxxSymbols(out, name, h, mb, fd);
        }
      }
      if (mb.getBehaviourType() ==
          BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
        out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
            << "_BehaviourType = 2u;\n\n";
        out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
            << "_BehaviourKinematic = 3u;\n\n";
        out << "MFRONT_SHAREDOBJ unsigned short umat" << makeLowerCase(name)
            << "_Interface = 2u;\n\n";
      } else {
        // cohesize zone model
        out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
            << "_BehaviourType = 3u;\n\n";
        out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
            << "_BehaviourKinematic = 2u;\n\n";
        out << "MFRONT_SHAREDOBJ unsigned short umat" << makeLowerCase(name)
            << "_Interface = 1u;\n\n";
      }
    }

    this->writeSetParametersFunctionsImplementations(out, name, mb);
    this->writeSetOutOfBoundsPolicyFunctionImplementation(out, name);
    if (mb.getBehaviourType() ==
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      for (pfss = this->finiteStrainStrategies.begin();
           pfss != this->finiteStrainStrategies.end(); ++pfss) {
        if (*pfss == FINITEROTATIONSMALLSTRAIN) {
          this->writeSetParametersFunctionsImplementations(out, name + "_frst",
                                                           mb);
          if (this->finiteStrainStrategies.size() != 1u) {
            this->writeSetOutOfBoundsPolicyFunctionImplementation2(
                out, name, name + "_frst");
          }
        } else if (*pfss == MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN) {
          this->writeSetParametersFunctionsImplementations(out, name + "_malls",
                                                           mb);
          if (this->finiteStrainStrategies.size() != 1u) {
            this->writeSetOutOfBoundsPolicyFunctionImplementation2(
                out, name, name + "_malls");
          }
        } else if (*pfss == LOGARITHMICSTRAIN1D) {
          this->writeSetParametersFunctionsImplementations(out, name + "_log1D",
                                                           mb);
          if (this->finiteStrainStrategies.size() != 1u) {
            this->writeSetOutOfBoundsPolicyFunctionImplementation2(
                out, name, name + "_log1D");
          }
        } else if (*pfss == NONE) {
          this->writeSetParametersFunctionsImplementations(out, name + "_ss",
                                                           mb);
          if (this->finiteStrainStrategies.size() != 1u) {
            this->writeSetOutOfBoundsPolicyFunctionImplementation2(
                out, name, name + "_ss");
          }
        } else {
          throw_if(true, "internal error, unsupported finite strain strategy");
        }
      }
    }

    out << "static void \numat" << makeLowerCase(name) << "_base"
        << "(const castem::CastemInt *const NTENS, const castem::CastemReal "
           "*const DTIME,\n"
        << "const castem::CastemReal *const DROT,  castem::CastemReal *const "
           "DDSDDE,\n"
        << "const castem::CastemReal *const STRAN, const castem::CastemReal "
           "*const DSTRAN,\n"
        << "const castem::CastemReal *const TEMP,  const castem::CastemReal "
           "*const DTEMP,\n"
        << "const castem::CastemReal *const PROPS, const castem::CastemInt    "
           "*const NPROPS,\n"
        << "const castem::CastemReal *const PREDEF,const castem::CastemReal "
           "*const DPRED,\n"
        << "castem::CastemReal *const STATEV,const castem::CastemInt    *const "
           "NSTATV,\n"
        << "castem::CastemReal *const STRESS,castem::CastemReal *const "
           "PNEWDT,\n"
        << "const castem::CastemInt *const NDI,\n"
        << "castem::CastemInt  *const KINC,\n"
        << "const castem::StressFreeExpansionHandler& sfeh)\n"
        << "{\n"
        << "tfel::material::OutOfBoundsPolicy op = "
        << this->getFunctionName(name) << "_getOutOfBoundsPolicy();\n"
        << "castem::CastemInterface<tfel::material::" << mb.getClassName()
        << ">::exe(NTENS,DTIME,DROT,DDSDDE,STRAN,DSTRAN,\n"
        << "TEMP,DTEMP,PROPS,NPROPS,\n"
        << "PREDEF,DPRED,STATEV,NSTATV,\n"
        << "STRESS,PNEWDT,NDI,KINC,op,sfeh);\n"
        << "}\n\n";

    if (mb.getBehaviourType() ==
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      if (this->finiteStrainStrategies.empty()) {
        this->writeCastemFunction(
            out, name, "", mb, &CastemInterface::writeStandardCastemFunction);
      } else {
        for (pfss = this->finiteStrainStrategies.begin();
             pfss != this->finiteStrainStrategies.end(); ++pfss) {
          if (*pfss == FINITEROTATIONSMALLSTRAIN) {
            this->writeCastemFunction(
                out, name, "frst", mb,
                &CastemInterface::writeFiniteRotationSmallStrainCastemFunction);
            if (this->finiteStrainStrategies.size() == 1u) {
              this->writeCastemFunction(
                  out, name, "", mb,
                  &CastemInterface::
                      writeFiniteRotationSmallStrainCastemFunction);
            }
          } else if (*pfss == MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN) {
            this->writeCastemFunction(
                out, name, "malls", mb,
                &CastemInterface::
                    writeMieheApelLambrechtLogarithmicStrainCastemFunction);
            if (this->finiteStrainStrategies.size() == 1u) {
              this->writeCastemFunction(
                  out, name, "", mb,
                  &CastemInterface::
                      writeMieheApelLambrechtLogarithmicStrainCastemFunction);
            }
          } else if (*pfss == LOGARITHMICSTRAIN1D) {
            this->writeCastemFunction(
                out, name, "log1D", mb,
                &CastemInterface::writeLogarithmicStrain1DCastemFunction);
            if (this->finiteStrainStrategies.size() == 1u) {
              this->writeCastemFunction(
                  out, name, "", mb,
                  &CastemInterface::writeLogarithmicStrain1DCastemFunction);
            }
          } else if (*pfss == NONE) {
            this->writeCastemFunction(
                out, name, "ss", mb,
                &CastemInterface::writeStandardCastemFunction);
            if (this->finiteStrainStrategies.size() == 1u) {
              this->writeCastemFunction(
                  out, name, "", mb,
                  &CastemInterface::writeStandardCastemFunction);
            }
          } else {
            throw_if(true,
                     "internal error, unsupported finite strain strategy");
          }
        }
        if ((this->finiteStrainStrategies.size() != 1u) &&
            (find(this->finiteStrainStrategies.begin(),
                  this->finiteStrainStrategies.end(),
                  NONE) != this->finiteStrainStrategies.end())) {
          this->writeCastemFunction(
              out, name, "", mb, &CastemInterface::writeStandardCastemFunction);
        }
      }
    } else {
      this->writeCastemFunction(out, name, "", mb,
                                &CastemInterface::writeStandardCastemFunction);
    }
    out << "} // end of extern \"C\"\n";
    out.close();
    if ((mb.getBehaviourType() ==
         BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
        (mb.getBehaviourType() ==
         BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)) {
      this->generateGibianeDeclaration(mb, fd);
    }
  }  // end of CastemInterface::endTreatment

  void CastemInterface::writeMTestFileGeneratorSetModellingHypothesis(
      std::ostream& out) const {
    out << "mg.setModellingHypothesis(castem::getModellingHypothesis(*NDI));\n";
  }

  void CastemInterface::writeInterfaceSpecificIncludes(
      std::ostream& out, const BehaviourDescription&) const {
    out << "#include\"MFront/Castem/Castem.hxx\"\n\n";
  }  // end of CastemInterface::writeInterfaceSpecificIncludes

  void CastemInterface::getTargetsDescription(TargetsDescription& d,
                                              const BehaviourDescription& bd) {
    using namespace std;
    const auto lib = CastemInterface::getLibraryName(bd);
    const auto name = this->getBehaviourName(bd);
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    insert_if(d[lib].cppflags, "$(shell " + tfel_config + " --includes)");
#ifdef CASTEM_CPPFLAGS
    insert_if(d[lib].cppflags, CASTEM_CPPFLAGS);
#endif /* CASTEM_CPPFLAGS */
#ifndef LOCAL_CASTEM_HEADER
#ifdef CASTEM_ROOT
    char* castem_root = ::getenv("CASTEM_ROOT");
    if (castem_root != nullptr) {
      insert_if(d[lib].cppflags, "-I" + string(castem_root) + "/include");
    } else {
      insert_if(d[lib].cppflags, "-I" + string(CASTEM_ROOT) + "/include");
    }
#else  /* CASTEM_ROOT */
    if (castem_root != 0) {
      insert_if(d[lib].cppflags, "-I" + string(castem_root) + "/include");
    }
#endif /* CASTEM_ROOT */
#endif /* LOCAL_CASTEM_HEADER_FILE */
    insert_if(d[lib].sources, "umat" + name + ".cxx");
    insert_if(d.headers, "MFront/Castem/umat" + name + ".hxx");
    insert_if(d[lib].ldflags,
              "-l" + tfel::getLibraryInstallName("CastemInterface"));
    if (this->generateMTestFile) {
      insert_if(d[lib].ldflags,
                "-l" + tfel::getLibraryInstallName("MTestFileGenerator"));
    }
    insert_if(d[lib].ldflags, "$(shell " + tfel_config +
                                  " --libs --material --mfront-profiling)");
    // entry points
    auto b = vector<string>{};
    const auto base = this->getUmatFunctionName(bd);
    if (bd.getBehaviourType() ==
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      if (this->finiteStrainStrategies.empty()) {
        b.push_back(base);
        // b.push_back(name);
      } else {
        for (const auto& fss : this->finiteStrainStrategies) {
          if (fss == FINITEROTATIONSMALLSTRAIN) {
            b.push_back(base + "_frst");
          } else if (fss == MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN) {
            b.push_back(base + "_malls");
          } else if (fss == LOGARITHMICSTRAIN1D) {
            b.push_back(base + "_log1d");
          } else if (fss == NONE) {
            b.push_back(base + "_ss");
          } else {
            throw(runtime_error(
                "CastemInterface::getGeneratedEntryPoints: "
                "internal error, unsupported finite strain strategy"));
          }
          if (this->finiteStrainStrategies.size() == 1u) {
            b.push_back(base);
            // b.push_back(name);
          }
        }
        if ((this->finiteStrainStrategies.size() != 1u) &&
            (find(this->finiteStrainStrategies.begin(),
                  this->finiteStrainStrategies.end(),
                  NONE) != this->finiteStrainStrategies.end())) {
          b.push_back(base);
          //	  b.push_back(name);
        }
      }
    } else {
      b.push_back(base);
      //      b.push_back(name);
    }
    insert_if(d[lib].epts, b);
  }  // end of CastemInterface::getTargetsDescription

  std::pair<std::vector<UMATInterfaceBase::UMATMaterialProperty>,
            SupportedTypes::TypeSize>
  CastemInterface::buildMaterialPropertiesList(const BehaviourDescription& mb,
                                               const Hypothesis h) const {
    using namespace std;
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      const auto ah = this->getModellingHypothesesToBeTreated(mb);
      set<Hypothesis> uh;
      for (const auto& lh : ah) {
        if (!mb.hasSpecialisedMechanicalData(lh)) {
          uh.insert(lh);
        }
      }
      if (uh.empty()) {
        throw(runtime_error(
            "CastemInterface::endTreatment: "
            "internal error : the mechanical behaviour says that not "
            "all handled mechanical data are specialised, but we found none."));
      }
      // material properties for all the selected hypothesis
      auto mpositions = vector<
          pair<vector<UMATMaterialProperty>, SupportedTypes::TypeSize>>{};
      for (const auto& lh : uh) {
        mpositions.push_back(this->buildMaterialPropertiesList(mb, lh));
      }
      auto ph = uh.cbegin();
      auto pum = mpositions.cbegin();
      const auto& mfirst = *pum;
      ++ph;
      ++pum;
      for (; ph != uh.end(); ++ph, ++pum) {
        const auto& d =
            mb.getBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
        const auto& mps = d.getMaterialProperties();
        for (const auto& mp : mps) {
          const auto& mp1 = findUMATMaterialProperty(
              mfirst.first, mb.getExternalName(h, mp.name));
          const auto& mp2 = findUMATMaterialProperty(
              pum->first, mb.getExternalName(h, mp.name));
          SupportedTypes::TypeSize o1 = mp1.offset;
          o1 += pum->second;
          SupportedTypes::TypeSize o2 = mp2.offset;
          o2 += mfirst.second;
          if (o1 != o2) {
            throw(runtime_error(
                "CastemInterface::buildMaterialPropertiesList : "
                "incompatible offset for material property '" +
                mp.name + "' (aka '" + mp1.name +
                "'). This is one pitfall of the umat interface. "
                "To by-pass this limitation, you may want to explicitely "
                "specialise some modelling hypotheses"));
          }
        }
      }
      return mfirst;
    }
    auto res = std::pair<std::vector<UMATMaterialProperty>,
                         SupportedTypes::TypeSize>{};
    auto& mprops = res.first;
    if ((mb.getBehaviourType() ==
         BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
        (mb.getBehaviourType() ==
         BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)) {
      if (mb.getSymmetryType() == mfront::ISOTROPIC) {
        this->appendToMaterialPropertiesList(mprops, "stress", "YoungModulus",
                                             "youn", false);
        this->appendToMaterialPropertiesList(mprops, "real", "PoissonRatio",
                                             "nu", false);
        this->appendToMaterialPropertiesList(mprops, "massdensity",
                                             "MassDensity", "rho", false);
        this->appendToMaterialPropertiesList(mprops, "thermalexpansion",
                                             "ThermalExpansion", "alph", false);
        if (h == ModellingHypothesis::PLANESTRESS) {
          this->appendToMaterialPropertiesList(mprops, "length", "PlateWidth",
                                               "dim3", false);
        }
      } else if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
        if (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
          this->appendToMaterialPropertiesList(mprops, "stress",
                                               "YoungModulus1", "yg1", false);
          this->appendToMaterialPropertiesList(mprops, "stress",
                                               "YoungModulus2", "yg2", false);
          this->appendToMaterialPropertiesList(mprops, "stress",
                                               "YoungModulus3", "yg3", false);
          this->appendToMaterialPropertiesList(mprops, "real", "PoissonRatio12",
                                               "nu12", false);
          this->appendToMaterialPropertiesList(mprops, "real", "PoissonRatio23",
                                               "nu23", false);
          this->appendToMaterialPropertiesList(mprops, "real", "PoissonRatio13",
                                               "nu13", false);
          this->appendToMaterialPropertiesList(mprops, "massdensity",
                                               "MassDensity", "rho", false);
          this->appendToMaterialPropertiesList(
              mprops, "thermalexpansion", "ThermalExpansion1", "alp1", false);
          this->appendToMaterialPropertiesList(
              mprops, "thermalexpansion", "ThermalExpansion2", "alp2", false);
          this->appendToMaterialPropertiesList(
              mprops, "thermalexpansion", "ThermalExpansion3", "alp3", false);
        } else if (h == ModellingHypothesis::PLANESTRESS) {
          this->appendToMaterialPropertiesList(mprops, "stress",
                                               "YoungModulus1", "yg1", false);
          this->appendToMaterialPropertiesList(mprops, "stress",
                                               "YoungModulus2", "yg2", false);
          this->appendToMaterialPropertiesList(mprops, "real", "PoissonRatio12",
                                               "nu12", false);
          this->appendToMaterialPropertiesList(mprops, "stress",
                                               "ShearModulus12", "g12", false);
          this->appendToMaterialPropertiesList(mprops, "real", "V1X", "v1x",
                                               true);
          this->appendToMaterialPropertiesList(mprops, "real", "V1Y", "v1y",
                                               true);
          this->appendToMaterialPropertiesList(mprops, "stress",
                                               "YoungModulus3", "yg3", false);
          this->appendToMaterialPropertiesList(mprops, "real", "PoissonRatio23",
                                               "nu23", false);
          this->appendToMaterialPropertiesList(mprops, "real", "PoissonRatio13",
                                               "nu13", false);
          this->appendToMaterialPropertiesList(mprops, "massdensity",
                                               "MassDensity", "rho", false);
          this->appendToMaterialPropertiesList(
              mprops, "thermalexpansion", "ThermalExpansion1", "alp1", false);
          this->appendToMaterialPropertiesList(
              mprops, "thermalexpansion", "ThermalExpansion2", "alp2", false);
          this->appendToMaterialPropertiesList(mprops, "length", "PlateWidth",
                                               "dim3", false);
        } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
                   (h == ModellingHypothesis::PLANESTRAIN) ||
                   (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
          this->appendToMaterialPropertiesList(mprops, "stress",
                                               "YoungModulus1", "yg1", false);
          this->appendToMaterialPropertiesList(mprops, "stress",
                                               "YoungModulus2", "yg2", false);
          this->appendToMaterialPropertiesList(mprops, "stress",
                                               "YoungModulus3", "yg3", false);
          this->appendToMaterialPropertiesList(mprops, "real", "PoissonRatio12",
                                               "nu12", false);
          this->appendToMaterialPropertiesList(mprops, "real", "PoissonRatio23",
                                               "nu23", false);
          this->appendToMaterialPropertiesList(mprops, "real", "PoissonRatio13",
                                               "nu13", false);
          this->appendToMaterialPropertiesList(mprops, "stress",
                                               "ShearModulus12", "g12", false);
          this->appendToMaterialPropertiesList(mprops, "real", "V1X", "v1x",
                                               true);
          this->appendToMaterialPropertiesList(mprops, "real", "V1Y", "v1y",
                                               true);
          this->appendToMaterialPropertiesList(mprops, "massdensity",
                                               "MassDensity", "rho", false);
          this->appendToMaterialPropertiesList(
              mprops, "thermalexpansion", "ThermalExpansion1", "alp1", false);
          this->appendToMaterialPropertiesList(
              mprops, "thermalexpansion", "ThermalExpansion2", "alp2", false);
          this->appendToMaterialPropertiesList(
              mprops, "thermalexpansion", "ThermalExpansion3", "alp3", false);
        } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          this->appendToMaterialPropertiesList(mprops, "stress",
                                               "YoungModulus1", "yg1", false);
          this->appendToMaterialPropertiesList(mprops, "stress",
                                               "YoungModulus2", "yg2", false);
          this->appendToMaterialPropertiesList(mprops, "stress",
                                               "YoungModulus3", "yg3", false);
          this->appendToMaterialPropertiesList(mprops, "real", "PoissonRatio12",
                                               "nu12", false);
          this->appendToMaterialPropertiesList(mprops, "real", "PoissonRatio23",
                                               "nu23", false);
          this->appendToMaterialPropertiesList(mprops, "real", "PoissonRatio13",
                                               "nu13", false);
          this->appendToMaterialPropertiesList(mprops, "stress",
                                               "ShearModulus12", "g12", false);
          this->appendToMaterialPropertiesList(mprops, "stress",
                                               "ShearModulus23", "g23", false);
          this->appendToMaterialPropertiesList(mprops, "stress",
                                               "ShearModulus13", "g13", false);
          this->appendToMaterialPropertiesList(mprops, "real", "V1X", "v1x",
                                               true);
          this->appendToMaterialPropertiesList(mprops, "real", "V1Y", "v1y",
                                               true);
          this->appendToMaterialPropertiesList(mprops, "real", "V1Z", "v1z",
                                               true);
          this->appendToMaterialPropertiesList(mprops, "real", "V2X", "v2x",
                                               true);
          this->appendToMaterialPropertiesList(mprops, "real", "V2Y", "v2y",
                                               true);
          this->appendToMaterialPropertiesList(mprops, "real", "V2Z", "v2z",
                                               true);
          this->appendToMaterialPropertiesList(mprops, "massdensity",
                                               "MassDensity", "rho", false);
          this->appendToMaterialPropertiesList(
              mprops, "thermalexpansion", "ThermalExpansion1", "alp1", false);
          this->appendToMaterialPropertiesList(
              mprops, "thermalexpansion", "ThermalExpansion2", "alp2", false);
          this->appendToMaterialPropertiesList(
              mprops, "thermalexpansion", "ThermalExpansion3", "alp3", false);
        } else {
          throw(
              runtime_error("CastemInterface::buildMaterialPropertiesList: "
                            "unsupported modelling hypothesis"));
        }
      } else {
        throw(
            runtime_error("CastemInterface::buildMaterialPropertiesList: "
                          "unsupported behaviour symmetry type.\n"
                          "The umat interface only support isotropic or "
                          "orthotropic behaviour at this time."));
      }
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::COHESIVEZONEMODEL) {
      if (mb.getSymmetryType() == mfront::ISOTROPIC) {
        //! those are not the Cast3M conventions, switch is performed
        //! below the CastemInterface class
        this->appendToMaterialPropertiesList(mprops, "real", "NormalStiffness",
                                             "kn", false);
        this->appendToMaterialPropertiesList(
            mprops, "real", "TangentialStiffness", "kt", false);
        this->appendToMaterialPropertiesList(mprops, "massdensity",
                                             "MassDensity", "rho", false);
        this->appendToMaterialPropertiesList(mprops, "thermalexpansion",
                                             "NormalThermalExpansion", "ALPN",
                                             false);
      } else {
        throw(
            runtime_error("CastemInterface::buildMaterialPropertiesList: "
                          "unsupported symmetry type for cohesive zone modes"));
      }
    } else {
      throw(
          runtime_error("CastemInterface::buildMaterialPropertiesList: "
                        "unsupported behaviour type"));
    }
    if (!mprops.empty()) {
      const auto& m = mprops.back();
      res.second = m.offset;
      res.second += this->getTypeSize(m.type, m.arraySize);
    }
    this->completeMaterialPropertiesList(mprops, mb, h);
    return res;
  }  // end of CastemInterface::buildMaterialPropertiesList

  void CastemInterface::writeUMATxxSupportedModellingHypothesis(
      std::ostream&, const std::string&, const BehaviourDescription&) const {
  }  // end of CastemInterface::::writeUMATxxSupportedModellingHypothesis

  void CastemInterface::writeUMATxxBehaviourTypeSymbols(
      std::ostream&, const std::string&, const BehaviourDescription&) const {
  }  // end of CastemInterface::writeUMATxxBehaviourTypeSymbols

  void CastemInterface::writeUMATxxBehaviourKinematicSymbols(
      std::ostream&, const std::string&, const BehaviourDescription&) const {
  }  // end of CastemInterface::writeUMATxxBehaviourKinematicSymbols

  void CastemInterface::writeUMATxxAdditionalSymbols(
      std::ostream&,
      const std::string&,
      const Hypothesis,
      const BehaviourDescription&,
      const FileDescription&) const {
  }  // end of CastemInterface::writeUMATxxAdditionalSymbols

  void CastemInterface::writeUMATxxSpecificSymbols(
      std::ostream& out,
      const std::string& name,
      const BehaviourDescription& mb,
      const FileDescription&) const {
    if (CastemInterface::usesGenericPlaneStressAlgorithm(mb)) {
      out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
          << "_UsesGenericPlaneStressAlgorithm = 1u;\n\n";
    } else {
      out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
          << "_UsesGenericPlaneStressAlgorithm = 0u;\n\n";
    }
  }

  void CastemInterface::writeCastemFunctionDeclaration(
      std::ostream& out, const std::string& name) const {
    out << "MFRONT_SHAREDOBJ void\numat" << makeLowerCase(name);
    writeUMATArguments(out);
    out << ";\n\n";
  }  // end of CastemInterface::writeCastemFunctionDeclaration

  void CastemInterface::writeFiniteStrainStrategiesPlaneStressSpecificCall(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& c,
      const std::string& c2) const {
    if ((mb.isModellingHypothesisSupported(ModellingHypothesis::PLANESTRESS)) ||
        (this->usesGenericPlaneStressAlgorithm(mb))) {
      out << "if(*NDI=="
          << getCastemModellingHypothesisIndex(ModellingHypothesis::PLANESTRESS)
          << "){\n";
      if (mb.isModellingHypothesisSupported(ModellingHypothesis::PLANESTRESS)) {
        const auto v = this->checkIfAxialStrainIsDefinedAndGetItsOffset(mb);
        if (v.first) {
          out << "const CastemReal ezz = STATEV["
              << v.second.getValueForDimension(2) << "];\n"
              << "const CastemReal Fzz = " << c2 << ";\n";
          out << c << ",Fzz);\n";
        } else {
          // no axial strain
          out << "std::cerr << \"no state variable standing for the axial "
                 "strain (variable with the "
              << "glossary name 'AxialStrain')\" << std::endl;\n";
          out << "*KINC=-1;\n";
          out << "return;\n";
        }
      } else {
        // generic algorithm, this means that the behaviour
        // can be called in generalised plane strain
        const auto& d =
            mb.getBehaviourData(ModellingHypothesis::GENERALISEDPLANESTRAIN);
        const auto s = this->getTotalSize(d.getPersistentVariables());
        if (s.getValueForDimension(2) == 0) {
          out << "const CastemReal ezz = STATEV[0];\n";
        } else {
          out << "if(*NSTATV!=" << s.getValueForDimension(2) + 1 << "){\n"
              << "std::cerr << \"invalid number of internal state variables\" "
                 "<< std::endl;\n"
              << "}\n";
          out << "const CastemReal ezz = STATEV[" << s.getValueForDimension(2)
              << "];\n";
        }
        out << "const CastemReal Fzz = " << c2 << ";\n";
        out << c << ",Fzz);\n";
      }
      out << "} else {\n";
      out << c << ",0);\n";
      out << "}\n";
    } else {
      out << c << ",0);\n";
    }
  }  // end of
     // CastemInterface::writeFiniteStrainStrategiesPlaneStressSpecificCall

  void CastemInterface::writeCastemFunction(
      std::ostream& out,
      const std::string& name,
      const std::string& suffix,
      const BehaviourDescription& mb,
      void (CastemInterface::*m)(std::ostream&,
                                 const std::string&,
                                 const std::string&,
                                 const std::string&,
                                 const BehaviourDescription&) const) const {
    const auto base = suffix.empty() ? name : name + "_" + suffix;
    const auto fname2 = "umat" + makeLowerCase(base);
    (this->*m)(out, name, fname2, suffix, mb);
  }

  void CastemInterface::writeFiniteRotationSmallStrainCastemFunction(
      std::ostream& out,
      const std::string& name,
      const std::string& fname,
      const std::string& suffix,
      const BehaviourDescription& mb) const {
    using namespace std;
    if (mb.getBehaviourType() !=
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      throw(runtime_error(
          "CastemInterface::writeFiniteRotationSmallStrainCastemFunction: "
          "finite strain strategies shall be used with small strain "
          "behaviours"));
    }
    out << "MFRONT_SHAREDOBJ void\n" << fname;
    writeUMATArguments(out,
                       BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR);
    out << "\n{\n";
    out << "using namespace castem;\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n";
      out << "using tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    out << "const auto k = std::abs(*DDSDDE)>0.5;\n";
    this->generateMTestFile1(out);
    out << "// computing the Green Lagrange strains\n";
    out << "CastemReal eto[6];\n";
    out << "CastemReal deto[6];\n";
    out << "CastemInt  i;\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "{\n"
          << "BehaviourProfiler::Timer pre_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
    }
    out << "CastemFiniteStrain::computeGreenLagrangeStrain(eto,F0,*NTENS,*NDI);"
           "\n";
    out << "CastemFiniteStrain::computeGreenLagrangeStrain(deto,F1,*NTENS,*NDI)"
           ";\n";
    const auto c1 =
        "CastemFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress("
        "STRESS,F0,*NTENS,*NDI";
    this->writeFiniteStrainStrategiesPlaneStressSpecificCall(
        out, mb, c1, "std::sqrt(1+2*ezz)");
    out << "for(i=0;i!=*NTENS;++i){\n";
    out << "deto[i] -= eto[i];\n";
    out << "}\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "}\n";
    }
    out << "umat" << makeLowerCase(name)
        << "_base(NTENS, DTIME,DROT,DDSDDE,eto,deto,TEMP,DTEMP,\n"
        << "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
        << "STRESS,PNEWDT,NDI,KINC,\n"
        << "castem::CastemStandardSmallStrainStressFreeExpansionHandler);\n";
    out << "if(*KINC==1){\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "BehaviourProfiler::Timer post_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
    }
    const auto c2 =
        "CastemFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress("
        "STRESS,F1,*NTENS,*NDI";
    this->writeFiniteStrainStrategiesPlaneStressSpecificCall(
        out, mb, c2, "std::sqrt(1+2*ezz)");
    out << "if(k){\n";
    const auto c3 =
        "CastemFiniteStrain::convertCSEtoCauchyTruesdellRateModuli(DDSDDE,"
        "STRESS,F1,*NTENS,*NDI";
    this->writeFiniteStrainStrategiesPlaneStressSpecificCall(
        out, mb, c3, "std::sqrt(1+2*ezz)");
    out << "}\n"
        << "}\n";
    if (this->generateMTestFile) {
      out << "if(*KINC!=1){\n";
      this->generateMTestFile2(
          out, BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR, name,
          suffix, mb);
      out << "}\n";
    }
    out << "}\n\n";
  }

  void CastemInterface::writeMieheApelLambrechtLogarithmicStrainCastemFunction(
      std::ostream& out,
      const std::string& name,
      const std::string& fname,
      const std::string& suffix,
      const BehaviourDescription& mb) const {
    if (mb.getBehaviourType() !=
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      throw(std::runtime_error(
          "CastemInterface::"
          "writeMieheApelLambrechtLogarithmicStrainCastemFunction: "
          "finite strain strategies shall be used with "
          "small strain behaviours"));
    }
    out << "MFRONT_SHAREDOBJ void\n" << fname;
    writeUMATArguments(out,
                       BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR);
    out << "\n{\n";
    out << "using namespace castem;\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n";
      out << "using tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out);
    out << "// computing the logarithmic strain\n";
    out << "CastemReal eto[6];\n";
    out << "CastemReal deto[6];\n";
    out << "CastemReal P0[36];\n";
    out << "CastemReal P1[36];\n";
    out << "CastemReal s[6];\n";
    out << "CastemInt  i;\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "{\n"
          << "BehaviourProfiler::Timer pre_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
    }
    out << "CastemFiniteStrain::computeLogarithmicStrainAndDerivative(P0,eto "
           ",F0,*NTENS,*NDI);\n";
    out << "CastemFiniteStrain::computeLogarithmicStrainAndDerivative(P1,deto,"
           "F1,*NTENS,*NDI);\n";
    const auto c1 =
        "CastemFiniteStrain::"
        "computeDualStressOfLogarithmicStrainFromCauchyStress(s,STRESS,P0,F0,*"
        "NTENS,*NDI";
    this->writeFiniteStrainStrategiesPlaneStressSpecificCall(out, mb, c1,
                                                             "std::exp(ezz)");
    out << "for(i=0;i!=*NTENS;++i){\n";
    out << "deto[i] -= eto[i];\n";
    out << "}\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "}\n";
    }
    out << "umat" << makeLowerCase(name)
        << "_base(NTENS, DTIME,DROT,DDSDDE,eto,deto,TEMP,DTEMP,\n"
        << "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
        << "s,PNEWDT,NDI,KINC,\n"
        << "castem::CastemLogarithmicStrainStressFreeExpansionHandler);\n";
    out << "if(*KINC==1){\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "BehaviourProfiler::Timer post_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
    }
    const auto c2 =
        "CastemFiniteStrain::"
        "computeCauchyStressFromDualStressOfLogarithmicStrain(STRESS,s,P1,F1,*"
        "NTENS,*NDI";
    this->writeFiniteStrainStrategiesPlaneStressSpecificCall(out, mb, c2,
                                                             "std::exp(ezz)");
    out << "}\n";
    if (this->generateMTestFile) {
      out << "if(*KINC!=1){\n";
      this->generateMTestFile2(
          out, BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR, name,
          suffix, mb);
      out << "}\n";
    }
    out << "}\n\n";
  }

  void CastemInterface::writeLogarithmicStrain1DCastemFunction(
      std::ostream& out,
      const std::string& name,
      const std::string& fname,
      const std::string& suffix,
      const BehaviourDescription& mb) const {
    using namespace std;
    if (mb.getBehaviourType() !=
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      throw(runtime_error(
          "CastemInterface::writeLogarithmicStrain1DCastemFunction : "
          "finite strain strategies shall be used with small strain "
          "behaviours"));
    }
    out << "MFRONT_SHAREDOBJ void\n" << fname;
    writeUMATArguments(out, BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR);
    out << "\n{\n";
    out << "using namespace castem;\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n";
      out << "using tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out);
    out << "bool k = std::abs(*DDSDDE)>0.5;\n";
    out << "// computing the logarithmic strain\n";
    out << "CastemReal eto[3];\n";
    out << "CastemReal deto[3];\n";
    out << "CastemReal s[3];\n";
    out << "CastemReal K[9];\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "{\n"
          << "BehaviourProfiler::Timer pre_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
    }
    out << "if(*NDI!=14){\n"
        << "*KINC=-7;\n"
        << "return;\n"
        << "}\n";
    out << "eto[0]=std::log1p(*STRAN);\n";
    out << "eto[1]=std::log1p(*(STRAN+1));\n";
    out << "eto[2]=std::log1p(*(STRAN+2));\n";
    out << "deto[0]=std::log1p(*STRAN+*DSTRAN)-eto[0];\n";
    out << "deto[1]=std::log1p(*(STRAN+1)+*(DSTRAN+1))-eto[1];\n";
    out << "deto[2]=std::log1p(*(STRAN+2)+*(DSTRAN+2))-eto[2];\n";
    out << "s[0]=(*STRESS)*(1+*STRAN);\n";
    out << "s[1]=(*(STRESS+1))*(1+*(STRAN+1));\n";
    out << "s[2]=(*(STRESS+2))*(1+*(STRAN+2));\n";
    out << "K[0]=*DDSDDE;\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "}\n";
    }
    out << "umat" << makeLowerCase(name)
        << "_base(NTENS, DTIME,DROT,K,eto,deto,TEMP,DTEMP,\n"
        << "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
        << "s,PNEWDT,NDI,KINC,\n"
        << "castem::CastemLogarithmicStrainStressFreeExpansionHandler);\n";
    out << "if(*KINC==1){\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "BehaviourProfiler::Timer post_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
    }
    // First Piola-Kirchhoff stress
    out << "STRESS[0]=s[0]/(1+*STRAN+*DSTRAN);\n";
    out << "STRESS[1]=s[1]/(1+*(STRAN+1)+*(DSTRAN+1));\n";
    out << "STRESS[2]=s[2]/(1+*(STRAN+2)+*(DSTRAN+2));\n";
    // computation of the stiffness matrix
    out << "if(k){\n"
        << "*DDSDDE     = "
           "(-STRESS[0]+K[0]/(1+STRAN[0]+DSTRAN[0]))/(1+STRAN[0]+DSTRAN[0]);\n"
        << "*(DDSDDE+3) = "
           "K[3]/((1+STRAN[1]+DSTRAN[1])*(1+STRAN[0]+DSTRAN[0]));\n"
        << "*(DDSDDE+6) = "
           "K[6]/((1+STRAN[2]+DSTRAN[2])*(1+STRAN[0]+DSTRAN[0]));\n"
        << "*(DDSDDE+1) = "
           "K[1]/((1+STRAN[0]+DSTRAN[0])*(1+STRAN[1]+DSTRAN[1]));\n"
        << "*(DDSDDE+4) = "
           "(-STRESS[1]+K[4]/(1+STRAN[1]+DSTRAN[1]))/(1+STRAN[1]+DSTRAN[1]);\n"
        << "*(DDSDDE+7) = "
           "K[7]/((1+STRAN[2]+DSTRAN[2])*(1+STRAN[1]+DSTRAN[1]));\n"
        << "*(DDSDDE+2) = "
           "K[2]/((1+STRAN[0]+DSTRAN[0])*(1+STRAN[2]+DSTRAN[2]));\n"
        << "*(DDSDDE+5) = "
           "K[5]/((1+STRAN[1]+DSTRAN[1])*(1+STRAN[2]+DSTRAN[2]));\n"
        << "*(DDSDDE+8) = "
           "(-STRESS[2]+K[8]/(1+STRAN[2]+DSTRAN[2]))/(1+STRAN[2]+DSTRAN[2]);\n"
        << "}\n";
    out << "}\n";
    if (this->generateMTestFile) {
      out << "if(*KINC!=1){\n";
      this->generateMTestFile2(
          out, BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR, name, suffix,
          mb);
      out << "}\n";
    }
    out << "}\n\n";
  }

  void CastemInterface::writeStandardCastemFunction(
      std::ostream& out,
      const std::string& name,
      const std::string& fname,
      const std::string& suffix,
      const BehaviourDescription& mb) const {
    out << "MFRONT_SHAREDOBJ void\n" << fname;
    writeUMATArguments(out, mb.getBehaviourType());
    out << "\n{\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n";
      out << "using tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out);
    if (mb.getBehaviourType() ==
        BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      out << "umat" << makeLowerCase(name)
          << "_base(NTENS, DTIME,DROT,DDSDDE,F0,F1,TEMP,DTEMP,\n"
          << "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
          << "STRESS,PNEWDT,NDI,KINC,\n"
          << "castem::CastemStandardSmallStrainStressFreeExpansionHandler);\n";
    } else {
      out << "umat" << makeLowerCase(name)
          << "_base(NTENS, DTIME,DROT,DDSDDE,STRAN,DSTRAN,TEMP,DTEMP,\n"
          << "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
          << "STRESS,PNEWDT,NDI,KINC,\n"
          << "castem::CastemStandardSmallStrainStressFreeExpansionHandler);\n";
    }
    if (this->generateMTestFile) {
      out << "if(*KINC!=1){\n";
      this->generateMTestFile2(out, mb.getBehaviourType(), name, suffix, mb);
      out << "}\n";
    }
    out << "}\n\n";
  }

  void CastemInterface::writeVariableDescriptionContainerToGibiane(
      std::ostream& out,
      const Hypothesis h,
      const VariableDescriptionContainer& v) const {
    for (const auto& vd : v) {
      const auto flag = this->getTypeFlag(vd.type);
      std::string tmp;
      tmp += ' ';
      if (flag == SupportedTypes::Scalar) {
        if (vd.arraySize == 1) {
          tmp += treatScalar(vd.name);
        } else {
          for (unsigned short j = 0; j != vd.arraySize;) {
            tmp += treatScalar(vd.name, j);
            if (++j != vd.arraySize) {
              tmp += ' ';
            }
          }
        }
      } else if (flag == SupportedTypes::Stensor) {
        if (vd.arraySize == 1) {
          tmp += treatStensor(h, vd.name);
        } else {
          for (unsigned short j = 0; j != vd.arraySize;) {
            tmp += treatStensor(h, vd.name, j);
            if (++j != vd.arraySize) {
              tmp += ' ';
            }
          }
        }
      } else if (flag == SupportedTypes::Tensor) {
        if (vd.arraySize == 1) {
          tmp += treatTensor(h, vd.name);
        } else {
          for (unsigned short j = 0; j != vd.arraySize;) {
            tmp += treatTensor(h, vd.name, j);
            if (++j != vd.arraySize) {
              tmp += ' ';
            }
          }
        }
      } else {
        throw(std::runtime_error(
            "CastemInterface::writeVariableDescriptionContainerToGibiane: "
            "internal error, tag unsupported"));
      }
      out << tmp;
    }
  }

  void CastemInterface::writeGibianeInstruction(std::ostream& out,
                                                const std::string& i) const {
    using namespace std;
    istringstream in(i);
    string buffer;
    while (!in.eof()) {
      string w;
      in >> w;
      if (buffer.size() + w.size() > 70) {
        out << buffer << '\n';
        buffer.clear();
        if (w.size() > 70) {
          out << w << '\n';
        } else {
          buffer = w;
        }
      } else {
        buffer += ' ' + w;
      }
    }
    out << buffer << '\n';
  }  // end of CastemInterface::writeGibianeInstruction

  void CastemInterface::generateGibianeDeclaration(
      const BehaviourDescription& mb, const FileDescription& fd) const {
    try {
      using namespace std;
      using namespace tfel::system;
      std::map<ModellingHypothesis::Hypothesis, std::string> mo = {
          {ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
           "'UNID' 'AXIS' 'AXGZ'"},
          {ModellingHypothesis::AXISYMMETRICAL, "'AXIS'"},
          {ModellingHypothesis::PLANESTRESS, "'PLAN' 'CONT'"},
          {ModellingHypothesis::PLANESTRAIN, "'PLAN' 'DEFO'"},
          {ModellingHypothesis::GENERALISEDPLANESTRAIN, "'PLAN' 'GENE'"},
          {ModellingHypothesis::TRIDIMENSIONAL, "'TRID'"}};
      const auto name((!mb.getLibrary().empty())
                          ? mb.getLibrary() + mb.getClassName()
                          : mb.getClassName());
      const auto fileName("castem/" + name + ".dgibi");
      // opening output file
      systemCall::mkdir("castem");
      ofstream out;
      out.open(fileName);
      if (!out) {
        throw(
            runtime_error("CastemInterface::generateGibianeDeclaration: "
                          "could not open file '" +
                          fileName + "'"));
      }
      // header
      out << "*\n";
      out << "* \\file   " << fd.fileName << '\n';
      out << "* \\brief  example of how to use the " << mb.getClassName()
          << " behaviour law\n"
          << "* in the Cast3M finite element solver\n";
      out << "* \\author " << fd.authorName << '\n';
      out << "* \\date   " << fd.date << '\n';
      out << "*\n\n";
      // specific declaration
      string nonlin;
      if (mb.getBehaviourType() ==
          BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
        nonlin = "'NON_LINEAIRE' 'UTILISATEUR'";
      } else if (mb.getBehaviourType() ==
                 BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
        nonlin = "'NON_LINEAIRE' 'UTILISATEUR' 'EPSILON' 'UTILISATEUR'";
      } else {
        throw(
            runtime_error("CastemInterface::generateGibianeDeclaration: "
                          "internal error, unsupported behaviour type"));
      }
      // loop over hypothesis
      for (const auto& h : this->getModellingHypothesesToBeTreated(mb)) {
        const auto& d = mb.getBehaviourData(h);
        const auto& persistentVarsHolder = d.getPersistentVariables();
        const auto& externalStateVarsHolder = d.getExternalStateVariables();
        const auto mprops = this->buildMaterialPropertiesList(mb, h);
        string tmp;
        out << "** " << ModellingHypothesis::toString(h) << " example\n";
        if (mo.find(h) != mo.end()) {
          out << "** 'OPTION' 'DIMENSION' " << getSpaceDimension(h)
              << " 'MODELISER' " << mo[h] << " ;\n\n";
        }
        ostringstream mcoel;
        mcoel << "coel = 'MOTS' ";
        for (auto pm = mprops.first.cbegin(); pm != mprops.first.cend();) {
          auto flag = this->getTypeFlag(pm->type);
          if (flag != SupportedTypes::Scalar) {
            throw(
                runtime_error("CastemInterface::generateGibianeDeclaration: "
                              "material properties shall be scalars"));
          }
          if (pm->arraySize == 1) {
            mcoel << treatScalar(pm->var_name);
          } else {
            for (unsigned short j = 0; j != pm->arraySize;) {
              mcoel << treatScalar(pm->var_name, j);
              if (++j != pm->arraySize) {
                mcoel << " ";
              }
            }
          }
          if (++pm != mprops.first.end()) {
            mcoel << " ";
          }
        }
        mcoel << ";";
        writeGibianeInstruction(out, mcoel.str());
        out << '\n';
        if (!persistentVarsHolder.empty()) {
          ostringstream mstatev;
          mstatev << "statev = 'MOTS' ";
          this->writeVariableDescriptionContainerToGibiane(
              mstatev, h, persistentVarsHolder);
          mstatev << ";";
          writeGibianeInstruction(out, mstatev.str());
          out << '\n';
        }
        ostringstream mparam;
        mparam << "params = 'MOTS' 'T'";
        if (!externalStateVarsHolder.empty()) {
          mparam << " ";
          this->writeVariableDescriptionContainerToGibiane(
              mparam, h, externalStateVarsHolder);
        }
        mparam << ";";
        writeGibianeInstruction(out, mparam.str());
        out << '\n';
        ostringstream mmod;
        mmod << "MO = 'MODELISER' v 'MECANIQUE' 'ELASTIQUE' ";
        if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
          mmod << "'ORTHOTROPE'";
        }
        mmod << nonlin << "\n"
             << "'LIB_LOI' 'lib" + this->getLibraryName(mb) + ".so'\n"
             << "'FCT_LOI' '" + this->getUmatFunctionName(mb) + "'\n"
             << "'C_MATERIAU' coel ";
        if (!persistentVarsHolder.empty()) {
          mmod << "'C_VARINTER' statev ";
        }
        mmod << "'PARA_LOI'   params 'CONS' M;";
        writeGibianeInstruction(out, mmod.str());
        out << '\n';
        ostringstream mi;
        mi << "MA = 'MATERIAU' MO ";
        auto mpos = 0;
        for (auto pm = mprops.first.cbegin(); pm != mprops.first.cend();
             ++mpos) {
          auto flag = this->getTypeFlag(pm->type);
          if (flag != SupportedTypes::Scalar) {
            throw(
                runtime_error("CastemInterface::generateGibianeDeclaration: "
                              "material properties shall be scalars"));
          }
          // skipping variables V1* and V2* imposed by Cast3M
          if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
            if (((h == ModellingHypothesis::PLANESTRESS) &&
                 ((mpos >= 4) && (mpos <= 5))) ||
                (((h == ModellingHypothesis::AXISYMMETRICAL) ||
                  (h == ModellingHypothesis::PLANESTRAIN) ||
                  (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) &&
                 ((mpos >= 7) && (mpos <= 8))) ||
                ((h == ModellingHypothesis::TRIDIMENSIONAL) &&
                 ((mpos >= 9) && (mpos <= 14)))) {
              ++pm;
              continue;
            }
          }
          if (pm->arraySize == 1) {
            tmp = treatScalar(pm->var_name);
            mi << tmp << " x" << makeLowerCase(pm->var_name);
          } else {
            for (unsigned short j = 0; j != pm->arraySize;) {
              tmp = treatScalar(pm->var_name, j);
              mi << tmp << " x" << makeLowerCase(pm->var_name) << j;
              if (++j != pm->arraySize) {
                mi << " ";
              }
            }
          }
          if (++pm != mprops.first.end()) {
            mi << " ";
          }
        }
        if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
          if ((h == ModellingHypothesis::PLANESTRESS) ||
              (h == ModellingHypothesis::AXISYMMETRICAL) ||
              (h == ModellingHypothesis::PLANESTRAIN) ||
              (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
            mi << " 'DIRECTION' (1 0) 'PARALLELE'";
          } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
            mi << " 'DIRECTION' (1 0 0) (0 0 1) 'PARALLELE'";
          }
        }
        mi << ";";
        writeGibianeInstruction(out, mi.str());
        out << '\n';
      }
      out.close();
    } catch (std::exception& e) {
      if (getVerboseMode() > VERBOSE_QUIET) {
        getLogStream() << e.what() << std::endl;
      }
    } catch (...) {
      if (getVerboseMode() > VERBOSE_QUIET) {
        getLogStream() << "CastemInterface::generateInputFileExample: "
                       << "unknown exception thrown" << std::endl;
      }
    }
  }  // end of CastemInterface::generateGibianeDeclaration

  void CastemInterface::writeUMATBehaviourTraits(std::ostream& out,
                                                 const BehaviourDescription& mb,
                                                 const Hypothesis h) const {
    using namespace std;
    const auto mvs = mb.getMainVariablesSize();
    const auto mprops = this->buildMaterialPropertiesList(mb, h);
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      if (mb.useQt()) {
        out << "template<tfel::material::ModellingHypothesis::Hypothesis "
               "H,typename Type,bool use_qt>\n";
      } else {
        out << "template<tfel::material::ModellingHypothesis::Hypothesis "
               "H,typename Type>\n";
      }
      out << "struct CastemTraits<tfel::material::" << mb.getClassName()
          << "<H,Type,";
    } else {
      if (mb.useQt()) {
        out << "template<typename Type,bool use_qt>\n";
      } else {
        out << "template<typename Type>\n";
      }
      out << "struct CastemTraits<tfel::material::" << mb.getClassName()
          << "<tfel::material::ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ",Type,";
    }
    if (mb.useQt()) {
      out << "use_qt";
    } else {
      out << "false";
    }
    out << "> >{\n";
    if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      out << "typedef tfel::material::ModellingHypothesis "
             "ModellingHypothesis;\n";
      out << "typedef "
             "tfel::material::ModellingHypothesisToSpaceDimension<"
             "ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h)
          << "> ModellingHypothesisToSpaceDimension;\n";
    } else {
      out << "typedef tfel::material::ModellingHypothesis "
             "ModellingHypothesis;\n";
      out << "typedef tfel::material::ModellingHypothesisToSpaceDimension<H> "
             "ModellingHypothesisToSpaceDimension;\n";
    }
    if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      out << "static " << constexpr_c << " ModellingHypothesis::Hypothesis H = "
          << "ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ";\n";
    }
    if (mb.getBehaviourType() ==
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      out << "static " << constexpr_c
          << " CastemBehaviourType btype  = SMALLSTRAINSTANDARDBEHAVIOUR;\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      out << "static " << constexpr_c
          << " CastemBehaviourType btype  = FINITESTRAINSTANDARDBEHAVIOUR;\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::COHESIVEZONEMODEL) {
      out << "static " << constexpr_c
          << " CastemBehaviourType btype  = COHESIVEZONEMODEL;\n";
    } else {
      throw(
          runtime_error("CastemInterface::writeUMATBehaviourTraits: "
                        "unsupported behaviour type"));
    }
    out << "// space dimension\n";
    out << "static " << constexpr_c
        << " unsigned short N           = "
           "ModellingHypothesisToSpaceDimension::value;\n";
    out << "// tiny vector size\n";
    out << "static " << constexpr_c << " unsigned short TVectorSize = N;\n";
    out << "// symmetric tensor size\n";
    out << "static " << constexpr_c
        << " unsigned short StensorSize = "
           "tfel::math::StensorDimeToSize<N>::value;\n";
    out << "// tensor size\n";
    out << "static " << constexpr_c
        << " unsigned short TensorSize  = "
           "tfel::math::TensorDimeToSize<N>::value;\n";
    out << "// size of the driving variable array (STRAN)\n";
    out << "static " << constexpr_c
        << " unsigned short DrivingVariableSize = " << mvs.first << ";\n";
    out << "// size of the thermodynamic force variable array (STRESS)\n";
    out << "static " << constexpr_c
        << " unsigned short ThermodynamicForceVariableSize = " << mvs.second
        << ";\n";
    out << "static " << constexpr_c << " bool useTimeSubStepping = ";
    if (this->useTimeSubStepping) {
      out << "true;\n";
    } else {
      out << "false;\n";
    }
    out << "static " << constexpr_c << " bool doSubSteppingOnInvalidResults = ";
    if (this->doSubSteppingOnInvalidResults) {
      out << "true;\n";
    } else {
      out << "false;\n";
    }
    out << "static " << constexpr_c << " unsigned short maximumSubStepping = ";
    if (this->useTimeSubStepping) {
      out << this->maximumSubStepping << ";\n";
    } else {
      out << "0u;\n";
    }
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      out << "static " << constexpr_c
          << " bool requiresStiffnessTensor = true;\n";
      if (mb.getAttribute(
              BehaviourDescription::requiresUnAlteredStiffnessTensor, false)) {
        out << "static " << constexpr_c
            << " bool requiresUnAlteredStiffnessTensor = true;\n";
      } else {
        out << "static " << constexpr_c
            << " bool requiresUnAlteredStiffnessTensor = false;\n";
      }
    } else {
      out << "static " << constexpr_c
          << " bool requiresStiffnessTensor = false;\n";
    }
    if (mb.getAttribute(
            BehaviourDescription::requiresThermalExpansionCoefficientTensor,
            false)) {
      out << "static " << constexpr_c
          << " bool requiresThermalExpansionCoefficientTensor = true;\n";
    } else {
      out << "static " << constexpr_c
          << " bool requiresThermalExpansionCoefficientTensor = false;\n";
    }
    // computing material properties size
    SupportedTypes::TypeSize msize;
    if (!mprops.first.empty()) {
      const auto& m = mprops.first.back();
      msize = m.offset;
      msize += this->getTypeSize(m.type, m.arraySize);
      msize -= mprops.second;
    }
    out << "static " << constexpr_c
        << " unsigned short material_properties_nb = " << msize << ";\n";
    if (mb.getSymmetryType() == mfront::ISOTROPIC) {
      if (mb.getBehaviourType() ==
          BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
        out << "static " << constexpr_c
            << " unsigned short propertiesOffset = "
               "CastemIsotropicOffset<castem::SMALLSTRAINSTANDARDBEHAVIOUR,H>::"
               "value;\n";
      } else if (mb.getBehaviourType() ==
                 BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
        out << "static " << constexpr_c
            << " unsigned short propertiesOffset = "
               "CastemIsotropicOffset<castem::FINITESTRAINSTANDARDBEHAVIOUR,H>:"
               ":value;\n";
      } else if (mb.getBehaviourType() ==
                 BehaviourDescription::COHESIVEZONEMODEL) {
        out << "static " << constexpr_c
            << " unsigned short propertiesOffset = "
               "CastemIsotropicOffset<castem::COHESIVEZONEMODEL,H>::value;\n";
      } else {
        throw(
            runtime_error("CastemInterface::writeUMATBehaviourTraits: "
                          "unsupported behaviour type"));
      }
    } else if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      if (mb.getBehaviourType() ==
          BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
        out << "static " << constexpr_c
            << " unsigned short propertiesOffset = "
               "CastemOrthotropicOffset<castem::SMALLSTRAINSTANDARDBEHAVIOUR,H>"
               "::value;\n";
      } else if (mb.getBehaviourType() ==
                 BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
        out << "static " << constexpr_c
            << " unsigned short propertiesOffset = "
               "CastemOrthotropicOffset<castem::FINITESTRAINSTANDARDBEHAVIOUR,"
               "H>::value;\n";
      } else {
        throw(
            runtime_error("CastemInterface::writeUMATBehaviourTraits: "
                          "unsupported behaviour type"));
      }
    } else {
      throw(
          runtime_error("CastemInterface::writeUMATBehaviourTraits:"
                        "unsupported behaviour symmetry type.\n"
                        "The umat interface only support isotropic or "
                        "orthotropic behaviour at this time."));
    }
    if (mb.getSymmetryType() == mfront::ISOTROPIC) {
      out << "static " << constexpr_c
          << " CastemSymmetryType stype = castem::ISOTROPIC;\n";
    } else if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "static " << constexpr_c
          << " CastemSymmetryType stype = castem::ORTHOTROPIC;\n";
    } else {
      string msg("CastemInterface::writeUMATBehaviourTraits : ");
      msg += "unsupported behaviour symmetry type.\n";
      msg +=
          "The umat interface only support isotropic or orthotropic behaviour "
          "at this time.";
      throw(runtime_error(msg));
    }
    out << "}; // end of class CastemTraits\n\n";
  }  // end of CastemInterface::writeUMATBehaviourTraits

  std::map<UMATInterfaceBase::Hypothesis, std::string>
  CastemInterface::gatherModellingHypothesesAndTests(
      const BehaviourDescription& mb) const {
    auto res = std::map<Hypothesis, std::string>{};
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      const auto h = this->getModellingHypothesesToBeTreated(mb);
      for (const auto& mh : h) {
        res.insert({mh, this->getModellingHypothesisTest(mh)});
      }
      return res;
    }
    return UMATInterfaceBase::gatherModellingHypothesesAndTests(mb);
  }  // end of CastemInterface::gatherModellingHypothesesAndTests

  std::string CastemInterface::getModellingHypothesisTest(
      const Hypothesis h) const {
    std::ostringstream test;
    test << "*NDI==" << getCastemModellingHypothesisIndex(h);
    return test.str();
  }

  CastemInterface::~CastemInterface() = default;

}  // end of namespace mfront
