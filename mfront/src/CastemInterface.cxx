/*!
 * \file   mfront/src/CastemInterface.cxx
 * \brief
 * \author Thomas Helfer
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

#include "TFEL/Raise.hxx"
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
    if (t != BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
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
    if (t == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
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
    tfel::raise(msg.str());
  }  // end of getCastemModellingHypothesisIndex

  const char* const CastemInterface::finiteStrainStrategies =
      "castem::finiteStrainStrategies";

  const char* const CastemInterface::useTimeSubStepping =
      "castem::useTimeSubStepping";

  const char* const CastemInterface::doSubSteppingOnInvalidResults =
      "castem::doSubSteppingOnInvalidResults";

  const char* const CastemInterface::maximumSubStepping =
      "castem::::maximumSubStepping";

  static void checkFiniteStrainStrategy(const std::string& fs) {
    tfel::raise_if((fs != "None") && (fs != "FiniteRotationSmallStrain") &&
                       (fs != "MieheApelLambrechtLogarithmicStrain") &&
                       (fs != "LogarithmicStrain1D"),
                   "checkFiniteStrainStrategy: "
                   "unsupported strategy '" +
                       fs +
                       "'\n"
                       "The only supported strategies are "
                       "'None', 'FiniteRotationSmallStrain', "
                       "'MieheApelLambrechtLogarithmicStrain' and "
                       "'LogarithmicStrain1D'");
  }  // end of checkFiniteStrainStrategy

  static void checkFiniteStrainStrategyDefinitionConsistency(
      const BehaviourDescription& bd, const std::vector<std::string>& fss) {
    auto throw_if = [](const bool c, const std::string& msg) {
      tfel::raise_if(c,
                     "checkFiniteStrainStrategyDefinitionConsistency "
                     "(CastemInterface): " +
                         msg);
    };
    throw_if(bd.getBehaviourType() !=
                 BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
             "checkFiniteStrainStrategyDefinitionConsistency "
             "(CastemInterface): invalid behaviour type");
    for (const auto& fs : fss) {
      checkFiniteStrainStrategy(fs);
    }
    if (bd.isStrainMeasureDefined()) {
      const auto ms = bd.getStrainMeasure();
      if (ms == BehaviourDescription::LINEARISED) {
        throw_if(fss.size() != 1u, "only one finite strain strategy expected");
        throw_if(fss[0] != "None",
                 "incompatible finite strain strategy "
                 "'" +
                     fss[0] + "' (only `None` accepted)");
      } else if (ms == BehaviourDescription::GREENLAGRANGE) {
        throw_if(fss.size() != 1u, "only one finite strain strategy expected");
        throw_if(fss[0] != "FiniteRotationSmallStrain",
                 "incompatible finite strain strategy "
                 "'" +
                     fss[0] + "' (only `FiniteRotationSmallStrain` accepted)");
      } else if (ms == BehaviourDescription::HENCKY) {
        throw_if((fss.size() != 1u) && (fss.size() != 2u),
                 "only one finite strain strategy expected");
        throw_if((fss[0] != "MieheApelLambrechtLogarithmicStrain") &&
                     (fss[0] != "LogarithmicStrain1D"),
                 "incompatible finite strain strategy "
                 "'" +
                     fss[0] +
                     "' (only `MieheApelLambrechtLogarithmicStrain` "
                     "and 'LogarithmicStrain1D' accepted)");
        if (fss.size() == 2u) {
          throw_if((fss[1] != "MieheApelLambrechtLogarithmicStrain") &&
                       (fss[1] != "LogarithmicStrain1D"),
                   "incompatible finite strain strategy "
                   "'" +
                       fss[1] +
                       "' (only `MieheApelLambrechtLogarithmicStrain` "
                       "and 'LogarithmicStrain1D' accepted)");
        }
      } else {
        throw_if(true, "unsupported finite strain strategy");
      }
    }
  }  // end of checkFiniteStrainStrategyDefinitionConsistency

  static void checkFiniteStrainStrategyDefinitionConsistency(
      const BehaviourDescription& bd) {
    tfel::raise_if(bd.getBehaviourType() !=
                       BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
                   "checkFiniteStrainStrategyDefinitionConsistency "
                   "(CastemInterface): invalid behaviour type");
    if (!bd.hasAttribute(CastemInterface::finiteStrainStrategies)) {
      return;
    }
    const auto fss = bd.getAttribute<std::vector<std::string>>(
        CastemInterface::finiteStrainStrategies);
    checkFiniteStrainStrategyDefinitionConsistency(bd, fss);
  }  // end of checkFiniteStrainStrategyDefinitionConsistency

  static bool areFiniteStrainStrategiesDefined(const BehaviourDescription& bd) {
    checkFiniteStrainStrategyDefinitionConsistency(bd);
    return (bd.isStrainMeasureDefined() ||
            bd.hasAttribute(CastemInterface::finiteStrainStrategies));
  }  // end of areFiniteStrainStrategiesDefined

  static std::vector<std::string> getFiniteStrainStrategies(
      const BehaviourDescription& bd) {
    checkFiniteStrainStrategyDefinitionConsistency(bd);
    if (bd.hasAttribute(CastemInterface::finiteStrainStrategies)) {
      return bd.getAttribute<std::vector<std::string>>(
          CastemInterface::finiteStrainStrategies);
    }
    const auto sm = bd.getStrainMeasure();
    if (sm == BehaviourDescription::LINEARISED) {
      return {"None"};
    } else if (sm == BehaviourDescription::GREENLAGRANGE) {
      return {"FiniteRotationSmallStrain"};
    }
    tfel::raise_if(sm != BehaviourDescription::HENCKY,
                   "getFiniteStrainStrategies (CastemInterface): "
                   "unsupported strain measure");
    return {"MieheApelLambrechtLogarithmicStrain", "LogarithmicStrain1D"};
  }  // end of getFiniteStrainStrategies

  CastemInterface::CastemInterface() = default;

  std::string CastemInterface::getName() { return "umat"; }

  std::string CastemInterface::getInterfaceName() const {
    return "Castem";
  }  // end of CastemInterface::getInterfaceName

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

  std::pair<bool, CastemInterface::tokens_iterator>
  CastemInterface::treatKeyword(BehaviourDescription& bd,
                                const std::string& key,
                                const std::vector<std::string>& i,
                                tokens_iterator current,
                                const tokens_iterator end) {
    using tfel::utilities::CxxTokenizer;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "CastemInterface::treatKeyword : " + m);
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
      bd.setAttribute(CastemInterface::useTimeSubStepping,
                      this->readBooleanValue(key, current, end), false);
      return {true, current};
    } else if ((key == "@CastemMaximumSubStepping") ||
               (key == "@UMATMaximumSubStepping")) {
      throw_if(
          !bd.getAttribute<bool>(CastemInterface::useTimeSubStepping, false),
          "time sub stepping is not enabled at this stage.\n"
          "Use the @CastemUseTimeSubStepping directive before "
          "@CastemMaximumSubStepping");
      throw_if(current == end, "unexpected end of file");
      const auto mss = CxxTokenizer::readUnsignedInt(current, end);
      bd.setAttribute(CastemInterface::maximumSubStepping,
                      static_cast<unsigned short>(mss), false);
      throw_if(current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';', read '" + current->value + '\'');
      ++(current);
      return {true, current};
    } else if ((key == "@CastemDoSubSteppingOnInvalidResults") ||
               (key == "@UMATDoSubSteppingOnInvalidResults")) {
      throw_if(
          !bd.getAttribute<bool>(CastemInterface::useTimeSubStepping, false),
          "time sub stepping is not enabled at this stage.\n"
          "Use the @CastemUseTimeSubStepping directive before "
          "@CastemMaximumSubStepping");
      bd.setAttribute(CastemInterface::doSubSteppingOnInvalidResults,
                      this->readBooleanValue(key, current, end), false);
      return {true, current};
    } else if ((key == "@CastemFiniteStrainStrategy") ||
               (key == "@UMATFiniteStrainStrategy")) {
      throw_if(bd.hasAttribute(CastemInterface::finiteStrainStrategies),
               "at least one strategy has already been defined");
      throw_if(current == end, "unexpected end of file");
      std::vector<std::string> fss(1u, current->value);
      checkFiniteStrainStrategyDefinitionConsistency(bd, fss);
      bd.setAttribute(CastemInterface::finiteStrainStrategies, fss, false);
      throw_if(++current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';', read '" + current->value + '\'');
      ++(current);
      return {true, current};
    } else if ((key == "@CastemFiniteStrainStrategies") ||
               (key == "@UMATFiniteStrainStrategies")) {
      throw_if(bd.hasAttribute(CastemInterface::finiteStrainStrategies),
               "at least one strategy has already been defined");
      auto fss = std::vector<std::string>{};
      CxxTokenizer::readArray(
          "CastemInterface::treatKeyword "
          "(@CastemFiniteStrainStrategies)",
          fss, current, end);
      CxxTokenizer::readSpecifiedToken(
          "CastemInterface::treatKeyword "
          "(@CastemFiniteStrainStrategies)",
          ";", current, end);
      throw_if(fss.empty(), "no strategy defined");
      checkFiniteStrainStrategyDefinitionConsistency(bd, fss);
      bd.setAttribute(CastemInterface::finiteStrainStrategies, fss, false);
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
        tfel::raise(msg);
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
        tfel::raise(msg);
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
        tfel::raise(msg);
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
        tfel::raise(msg);
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
    tfel::raise_if(mh.empty(),
                   "CastemInterface::getModellingHypothesesToBeTreated: "
                   "no hypotheses selected. This means that the "
                   "given beahviour can't be used neither in "
                   "'AxisymmetricalGeneralisedPlaneStrain' "
                   "nor in 'Axisymmetrical', "
                   "nor in 'PlaneStress', "
                   "nor in 'PlaneStrain', "
                   "nor in 'GeneralisedPlaneStrain', "
                   "and nor in 'Tridimensional', "
                   "so it does not make sense to use "
                   "the Castem interface");
    return mh;
  }  // end of CastemInterfaceModellingHypothesesToBeTreated

  bool CastemInterface::isModellingHypothesisSupported(
      const Hypothesis h, const BehaviourDescription& mb) const {
    const auto mh = this->getModellingHypothesesToBeTreated(mb);
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      return !mb.areAllMechanicalDataSpecialised(mh);
    }
    if (mh.find(h) != mh.end()) {
      return true;
    }
    if (h == ModellingHypothesis::PLANESTRESS) {
      return this->usesGenericPlaneStressAlgorithm(mb);
    }
    return false;
  }  // end of CastemInterface::isModellingHypothesisSupported

  void CastemInterface::writeGetOutOfBoundsPolicyFunctionImplementation(
      std::ostream& out, const std::string& name) const {
    out << "static tfel::material::OutOfBoundsPolicy&\n"
        << getFunctionName(name) << "_getOutOfBoundsPolicy(){\n"
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
      tfel::raise_if(b, "CastemInterface::endTreatment: " + m);
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
    if (mb.getAttribute<bool>(CastemInterface::useTimeSubStepping, false)) {
      throw_if(!mb.hasAttribute(CastemInterface::maximumSubStepping),
               "use of time sub stepping requested but no "
               "maximum number of substeps defined.\n"
               "Please use the @CastemMaximumSubStepping directive");
    }
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/Castem");

    const auto name = this->getBehaviourName(mb);

    auto fileName = "umat" + name + ".hxx";
    std::ofstream out("include/MFront/Castem/" + fileName);
    throw_if(!out, "could not open file '" + fileName + "'");

    out << "/*!\n"
        << "* \\file   " << fileName << '\n'
        << "* \\brief  This file declares the umat interface for the "
        << mb.getClassName() << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n";

    const string header = this->getHeaderDefine(mb);
    out << "#ifndef " << header << "\n"
        << "#define " << header << "\n\n";

    out << "#include\"castem.h\"\n"
        << "#ifdef umat\n"
        << "#undef umat\n"
        << "#endif /* umat */\n\n";

    out << "#include\"TFEL/Config/TFELConfig.hxx\"\n\n"
        << "#include\"MFront/Castem/Castem.hxx\"\n\n";

    out << "#ifdef __cplusplus\n"
        << "#include\"MFront/Castem/CastemTraits.hxx\"\n";
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "#include\"MFront/Castem/CastemOrthotropicBehaviour.hxx\"\n";
    }
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n"
        << "#endif /* __cplusplus */\n\n";

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

    out << "#ifdef __cplusplus\n"
        << "extern \"C\"{\n"
        << "#endif /* __cplusplus */\n\n";
    this->writeSetParametersFunctionsDeclarations(out, name, mb);
    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out, name);
    if ((mb.getBehaviourType() ==
         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
        (areFiniteStrainStrategiesDefined(mb))) {
      for (const auto& fs : getFiniteStrainStrategies(mb)) {
        if (fs == "FiniteRotationSmallStrain") {
          this->writeSetParametersFunctionsDeclarations(out, name + "_frst",
                                                        mb);
          this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,
                                                             name + "_frst");
        } else if (fs == "MieheApelLambrechtLogarithmicStrain") {
          this->writeSetParametersFunctionsDeclarations(out, name + "_malls",
                                                        mb);
          this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,
                                                             name + "_malls");
        } else if (fs == "LogarithmicStrain1D") {
          this->writeSetParametersFunctionsDeclarations(out, name + "_log1D",
                                                        mb);
          this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,
                                                             name + "_log1D");
        } else if (fs == "None") {
          this->writeSetParametersFunctionsDeclarations(out, name + "_ss", mb);
          this->writeSetOutOfBoundsPolicyFunctionDeclaration(out, name + "_ss");
        } else {
          throw_if(true, "internal error, unsupported finite strain strategy");
        }
      }
    }
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (!areFiniteStrainStrategiesDefined(mb)) {
        this->writeCastemFunctionDeclaration(out, name);
      } else {
        const auto fss = getFiniteStrainStrategies(mb);
        for (const auto& fs : fss) {
          if (fs == "FiniteRotationSmallStrain") {
            this->writeCastemFunctionDeclaration(out, name + "_frst");
            if (fss.size() == 1u) {
              this->writeCastemFunctionDeclaration(out, name);
            }
          } else if (fs == "MieheApelLambrechtLogarithmicStrain") {
            this->writeCastemFunctionDeclaration(out, name + "_malls");
            if (fss.size() == 1u) {
              this->writeCastemFunctionDeclaration(out, name);
            }
          } else if (fs == "LogarithmicStrain1D") {
            this->writeCastemFunctionDeclaration(out, name + "_log1D");
            if (fss.size() == 1u) {
              this->writeCastemFunctionDeclaration(out, name);
            }
          } else if (fs == "None") {
            this->writeCastemFunctionDeclaration(out, name + "_ss");
            if (fss.size() == 1u) {
              this->writeCastemFunctionDeclaration(out, name);
            }
          } else {
            throw_if(true,
                     "internal error, unsupported finite strain strategy");
          }
        }
        if ((fss.size() != 1u) &&
            (std::find(fss.begin(), fss.end(), "None") != fss.end())) {
          this->writeCastemFunctionDeclaration(out, name);
        }
      }
    } else {
      this->writeCastemFunctionDeclaration(out, name);
    }

    out << "#ifdef __cplusplus\n"
        << "}\n"
        << "#endif /* __cplusplus */\n\n";

    out << "#endif /* " << header << " */\n";

    out.close();

    fileName = "umat";
    fileName += name;
    fileName += ".cxx";

    out.open("src/" + fileName);
    tfel::raise_if(!out,
                   "CastemInterface::endTreatment: "
                   "could not open file '" +
                       fileName + "'");

    out << "/*!\n"
        << "* \\file   " << fileName << '\n'
        << "* \\brief  This file implements the umat interface for the "
        << mb.getClassName() << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n";

    this->getExtraSrcIncludes(out, mb);
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "#include\"MFront/BehaviourProfiler.hxx\"\n\n";
    }
    if (this->generateMTestFile) {
      throw_if(
          (mb.getBehaviourType() !=
           BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
              (mb.getBehaviourType() !=
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR),
          "MTest file generation is not unsupported for this behaviour type");
      out << "#include\"MFront/Castem/CastemGetModellingHypothesis.hxx\"\n";
    }
    if ((mb.getBehaviourType() ==
         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
        (areFiniteStrainStrategiesDefined(mb))) {
      const auto fss = getFiniteStrainStrategies(mb);
      if ((std::find(fss.begin(), fss.end(), "FiniteRotationSmallStrain") !=
           fss.end()) ||
          (find(fss.begin(), fss.end(),
                "MieheApelLambrechtLogarithmicStrain") != fss.end())) {
        out << "#include\"MFront/Castem/CastemFiniteStrain.hxx\"\n\n";
      }
      if (find(fss.begin(), fss.end(), "MieheApelLambrechtLogarithmicStrain") !=
          fss.end()) {
        out << "#include\"TFEL/Material/LogarithmicStrainHandler.hxx\"\n\n";
      }
    }
    out << "#include\"MFront/Castem/CastemOutOfBoundsPolicy.hxx\"\n"
        << "#include\"MFront/Castem/CastemInterface.hxx\"\n\n"
        << "#include\"MFront/Castem/CastemStressFreeExpansionHandler.hxx\"\n\n"
        << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n"
        << "#include\"MFront/Castem/umat" << name << ".hxx\"\n\n";

    this->writeGetOutOfBoundsPolicyFunctionImplementation(out, name);

    out << "extern \"C\"{\n\n";

    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (!areFiniteStrainStrategiesDefined(mb)) {
        this->generateUMATxxGeneralSymbols(out, name, mb, fd);
        UMATInterfaceBase::writeUMATxxSupportedModellingHypothesis(out, name,
                                                                   mb);
        if (!mb.areAllMechanicalDataSpecialised(mh)) {
          const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
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
        const auto fss = getFiniteStrainStrategies(mb);
        for (const auto fs : fss) {
          if (fs == "FiniteRotationSmallStrain") {
            this->generateUMATxxGeneralSymbols(out, name + "_frst", mb, fd);
            UMATInterfaceBase::writeUMATxxSupportedModellingHypothesis(
                out, name + "_frst", mb);
            if (!mb.areAllMechanicalDataSpecialised(mh)) {
              const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
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
            if (fss.size() == 1u) {
              this->generateUMATxxGeneralSymbols(out, name, mb, fd);
              UMATInterfaceBase::writeUMATxxSupportedModellingHypothesis(
                  out, name, mb);
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
                  << "_BehaviourKinematic = 3u;\n\n";
              out << "MFRONT_SHAREDOBJ unsigned short umat"
                  << makeLowerCase(name) << "_Interface = 2u;\n\n";
            }
          } else if (fs == "MieheApelLambrechtLogarithmicStrain") {
            this->generateUMATxxGeneralSymbols(out, name + "_malls", mb, fd);
            UMATInterfaceBase::writeUMATxxSupportedModellingHypothesis(
                out, name + "_malls", mb);
            if (!mb.areAllMechanicalDataSpecialised(mh)) {
              const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
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
            if (fss.size() == 1u) {
              this->generateUMATxxGeneralSymbols(out, name, mb, fd);
              UMATInterfaceBase::writeUMATxxSupportedModellingHypothesis(
                  out, name, mb);
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
                  << "_BehaviourKinematic = 3u;\n\n";
              out << "MFRONT_SHAREDOBJ unsigned short umat"
                  << makeLowerCase(name) << "_Interface = 2u;\n\n";
            }
          } else if (fs == "LogarithmicStrain1D") {
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
            if (fss.size() == 1u) {
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
          } else if (fs == "None") {
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
            if (fss.size() == 1u) {
              this->generateUMATxxGeneralSymbols(out, name, mb, fd);
              UMATInterfaceBase::writeUMATxxSupportedModellingHypothesis(
                  out, name, mb);
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
        if ((fss.size() != 1u) &&
            (std::find(fss.begin(), fss.end(), "None") != fss.end())) {
          this->generateUMATxxGeneralSymbols(out, name, mb, fd);
          UMATInterfaceBase::writeUMATxxSupportedModellingHypothesis(out, name,
                                                                     mb);
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
        const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
        this->generateUMATxxSymbols(out, name, uh, mb, fd);
      }
      for (const auto& h : mh) {
        if (mb.hasSpecialisedMechanicalData(h)) {
          this->generateUMATxxSymbols(out, name, h, mb, fd);
        }
      }
      if (mb.getBehaviourType() ==
          BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
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
    if ((mb.getBehaviourType() ==
         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
        (areFiniteStrainStrategiesDefined(mb))) {
      const auto fss = getFiniteStrainStrategies(mb);
      for (const auto& fs : fss) {
        if (fs == "FiniteRotationSmallStrain") {
          this->writeSetParametersFunctionsImplementations(out, name + "_frst",
                                                           mb);
          if (fss.size() != 1u) {
            this->writeSetOutOfBoundsPolicyFunctionImplementation2(
                out, name, name + "_frst");
          }
        } else if (fs == "MieheApelLambrechtLogarithmicStrain") {
          this->writeSetParametersFunctionsImplementations(out, name + "_malls",
                                                           mb);
          if (fss.size() != 1u) {
            this->writeSetOutOfBoundsPolicyFunctionImplementation2(
                out, name, name + "_malls");
          }
        } else if (fs == "LogarithmicStrain1D") {
          this->writeSetParametersFunctionsImplementations(out, name + "_log1D",
                                                           mb);
          if (fss.size() != 1u) {
            this->writeSetOutOfBoundsPolicyFunctionImplementation2(
                out, name, name + "_log1D");
          }
        } else if (fs == "None") {
          this->writeSetParametersFunctionsImplementations(out, name + "_ss",
                                                           mb);
          if (fss.size() != 1u) {
            this->writeSetOutOfBoundsPolicyFunctionImplementation2(
                out, name, name + "_ss");
          }
        } else {
          throw_if(true, "internal error, unsupported finite strain strategy");
        }
      }
    }

    for (const auto& h :
         {ModellingHypothesis::TRIDIMENSIONAL,
          ModellingHypothesis::AXISYMMETRICAL, ModellingHypothesis::PLANESTRAIN,
          ModellingHypothesis::PLANESTRESS,
          ModellingHypothesis::GENERALISEDPLANESTRAIN,
          ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN}) {
      if (this->isModellingHypothesisSupported(h, mb)) {
        out << "static void \numat" << makeLowerCase(name) << "_base_"
            << ModellingHypothesis::toUpperCaseString(h)
            << "(const castem::CastemInt *const NTENS, const "
               "castem::CastemReal *const DTIME,\n"
            << "const castem::CastemReal *const DROT,  castem::CastemReal "
               "*const DDSDDE,\n"
            << "const castem::CastemReal *const STRAN, const "
               "castem::CastemReal *const DSTRAN,\n"
            << "const castem::CastemReal *const TEMP,  const "
               "castem::CastemReal *const DTEMP,\n"
            << "const castem::CastemReal *const PROPS, const castem::CastemInt "
               "   *const NPROPS,\n"
            << "const castem::CastemReal *const PREDEF,const "
               "castem::CastemReal *const DPRED,\n"
            << "castem::CastemReal *const STATEV,const castem::CastemInt    "
               "*const NSTATV,\n"
            << "castem::CastemReal *const STRESS,castem::CastemReal *const "
               "PNEWDT,\n"
            << "castem::CastemInt  *const KINC,\n"
            << "const castem::StressFreeExpansionHandler& sfeh)\n"
            << "{\n"
            << "const auto op = " << this->getFunctionName(name)
            << "_getOutOfBoundsPolicy();\n"
            << "castem::CastemInterface<tfel::material::ModellingHypothesis::"
            << ModellingHypothesis::toUpperCaseString(h) << ","
            << "tfel::material::" << mb.getClassName()
            << ">::exe(NTENS,DTIME,DROT,DDSDDE,STRAN,DSTRAN,\n"
            << "TEMP,DTEMP,PROPS,NPROPS,\n"
            << "PREDEF,DPRED,STATEV,NSTATV,\n"
            << "STRESS,PNEWDT,KINC,op,sfeh);\n"
            << "}\n\n";
      } else {
        out << "static void \numat" << makeLowerCase(name) << "_base_"
            << ModellingHypothesis::toUpperCaseString(h)
            << "(const castem::CastemInt *const, const castem::CastemReal "
               "*const,\n"
            << "const castem::CastemReal *const,  castem::CastemReal *const,\n"
            << "const castem::CastemReal *const, const castem::CastemReal "
               "*const,\n"
            << "const castem::CastemReal *const,  const castem::CastemReal "
               "*const,\n"
            << "const castem::CastemReal *const, const castem::CastemInt    "
               "*const,\n"
            << "const castem::CastemReal *const,const castem::CastemReal "
               "*const,\n"
            << "castem::CastemReal *const,const castem::CastemInt    *const,\n"
            << "castem::CastemReal *const,castem::CastemReal *const,\n"
            << "castem::CastemInt  *const KINC,\n"
            << "const castem::StressFreeExpansionHandler&)\n"
            << "{\n"
            << "std::cerr << \"" << name
            << ": unsupported modelling hypothesis\\n\";\n"
            << "*KINC = -2;\n"
            << "}\n\n";
      }
    }

    auto ndi_dispatch = [&out, &name](const int ndi, const Hypothesis h,
                                      const bool b) {
      out << (b ? "if" : " else if") << "(*NDI==" << ndi << "){\n"
          << "	umat" << makeLowerCase(name) << "_base_"
          << ModellingHypothesis::toUpperCaseString(h)
          << "(NTENS,DTIME,DROT,DDSDDE,STRAN,DSTRAN,\n"
          << " TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,\n"
          << " STATEV,NSTATV,STRESS,PNEWDT,KINC,sfeh);\n"
          << " }";
    };

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
        << "{\n";
    ndi_dispatch(2, ModellingHypothesis::TRIDIMENSIONAL, true);
    ndi_dispatch(0, ModellingHypothesis::AXISYMMETRICAL, false);
    ndi_dispatch(-1, ModellingHypothesis::PLANESTRAIN, false);
    ndi_dispatch(-2, ModellingHypothesis::PLANESTRESS, false);
    ndi_dispatch(-3, ModellingHypothesis::GENERALISEDPLANESTRAIN, false);
    ndi_dispatch(14, ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
                 false);
    out << " else {\n"
        << "castem::CastemInterfaceExceptions::"
           "displayInvalidModellingHypothesisErrorMessage();\n"
        << "*KINC = -7;\n"
        << "}\n"
        << "}\n\n";

    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (!areFiniteStrainStrategiesDefined(mb)) {
        this->writeCastemFunction(
            out, name, "", mb, &CastemInterface::writeStandardCastemFunction);
      } else {
        const auto fss = getFiniteStrainStrategies(mb);
        for (const auto& fs : fss) {
          if (fs == "FiniteRotationSmallStrain") {
            this->writeCastemFunction(
                out, name, "frst", mb,
                &CastemInterface::writeFiniteRotationSmallStrainCastemFunction);
            if (fss.size() == 1u) {
              this->writeCastemFunction(
                  out, name, "", mb,
                  &CastemInterface::
                      writeFiniteRotationSmallStrainCastemFunction);
            }
          } else if (fs == "MieheApelLambrechtLogarithmicStrain") {
            this->writeCastemFunction(
                out, name, "malls", mb,
                &CastemInterface::
                    writeMieheApelLambrechtLogarithmicStrainCastemFunction);
            if (fss.size() == 1u) {
              this->writeCastemFunction(
                  out, name, "", mb,
                  &CastemInterface::
                      writeMieheApelLambrechtLogarithmicStrainCastemFunction);
            }
          } else if (fs == "LogarithmicStrain1D") {
            this->writeCastemFunction(
                out, name, "log1D", mb,
                &CastemInterface::writeLogarithmicStrain1DCastemFunction);
            if (fss.size() == 1u) {
              this->writeCastemFunction(
                  out, name, "", mb,
                  &CastemInterface::writeLogarithmicStrain1DCastemFunction);
            }
          } else if (fs == "None") {
            this->writeCastemFunction(
                out, name, "ss", mb,
                &CastemInterface::writeStandardCastemFunction);
            if (fss.size() == 1u) {
              this->writeCastemFunction(
                  out, name, "", mb,
                  &CastemInterface::writeStandardCastemFunction);
            }
          } else {
            throw_if(true,
                     "internal error, unsupported finite strain strategy");
          }
        }
        if ((fss.size() != 1u) &&
            (std::find(fss.begin(), fss.end(), "None") != fss.end())) {
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
         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
        (mb.getBehaviourType() ==
         BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)) {
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
    const auto lib = CastemInterface::getLibraryName(bd);
    const auto name = this->getBehaviourName(bd);
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    insert_if(d[lib].cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
#ifdef CASTEM_CPPFLAGS
    insert_if(d[lib].cppflags, CASTEM_CPPFLAGS);
#endif /* CASTEM_CPPFLAGS */
#ifndef LOCAL_CASTEM_HEADER
#ifdef CASTEM_ROOT
    const auto* castem_root = ::getenv("CASTEM_ROOT");
    if (castem_root != nullptr) {
      insert_if(d[lib].cppflags, "-I" + std::string(castem_root) + "/include");
    } else {
      insert_if(d[lib].cppflags, "-I" + std::string(CASTEM_ROOT) + "/include");
    }
#else  /* CASTEM_ROOT */
    if (castem_root != 0) {
      insert_if(d[lib].cppflags, "-I" + std::string(castem_root) + "/include");
    }
#endif /* CASTEM_ROOT */
#endif /* LOCAL_CASTEM_HEADER_FILE */
    insert_if(d[lib].include_directories,
              "$(shell " + tfel_config + " --include-path)");
    insert_if(d[lib].sources, "umat" + name + ".cxx");
    insert_if(d.headers, "MFront/Castem/umat" + name + ".hxx");
    insert_if(d[lib].link_directories,
              "$(shell " + tfel_config + " --library-path)");
    insert_if(d[lib].link_libraries,
              tfel::getLibraryInstallName("CastemInterface"));
    if (this->generateMTestFile) {
      insert_if(d[lib].link_libraries,
                tfel::getLibraryInstallName("MTestFileGenerator"));
    }
#if __cplusplus >= 201703L
    insert_if(d[lib].link_libraries, "$(shell " + tfel_config +
                                         " --library-dependency "
                                         "--material --mfront-profiling)");
#else  /* __cplusplus < 201703L */
    insert_if(d[lib].link_libraries,
              "$(shell " + tfel_config +
                  " --library-dependency "
                  "--material --mfront-profiling --physical-constants)");
#endif /* __cplusplus < 201703L */
    // entry points
    auto b = std::vector<std::string>{};
    const auto base = this->getUmatFunctionName(bd);
    if (bd.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (!areFiniteStrainStrategiesDefined(bd)) {
        b.push_back(base);
        // b.push_back(name);
      } else {
        const auto fss = getFiniteStrainStrategies(bd);
        for (const auto& fs : fss) {
          if (fs == "FiniteRotationSmallStrain") {
            b.push_back(base + "_frst");
          } else if (fs == "MieheApelLambrechtLogarithmicStrain") {
            b.push_back(base + "_malls");
          } else if (fs == "LogarithmicStrain1D") {
            b.push_back(base + "_log1d");
          } else if (fs == "None") {
            b.push_back(base + "_ss");
          } else {
            tfel::raise(
                "CastemInterface::getGeneratedEntryPoints: "
                "internal error, unsupported finite strain strategy");
          }
          if (fss.size() == 1u) {
            b.push_back(base);
            // b.push_back(name);
          }
        }
        if ((fss.size() != 1u) &&
            (std::find(fss.begin(), fss.end(), "None") != fss.end())) {
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
      tfel::raise_if(uh.empty(),
                     "CastemInterface::endTreatment: "
                     "internal error : the mechanical behaviour "
                     "says that not all handled mechanical data "
                     "are specialised, but we found none.");
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
          tfel::raise_if(
              o1 != o2,
              "CastemInterface::buildMaterialPropertiesList: "
              "incompatible offset for material "
              "property '" +
                  mp.name + "' (aka '" + mp1.name +
                  "'). "
                  "This is one pitfall of the umat interface. "
                  "To by-pass this limitation, you may want to "
                  "explicitely specialise some modelling hypotheses");
        }
      }
      return mfirst;
    }
    auto res = std::pair<std::vector<UMATMaterialProperty>,
                         SupportedTypes::TypeSize>{};
    auto& mprops = res.first;
    if ((mb.getBehaviourType() ==
         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
        (mb.getBehaviourType() ==
         BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)) {
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
          tfel::raise(
              "CastemInterface::buildMaterialPropertiesList: "
              "unsupported modelling hypothesis");
        }
      } else {
        tfel::raise(
            "CastemInterface::buildMaterialPropertiesList: "
            "unsupported behaviour symmetry type.\n"
            "The umat interface only support isotropic or "
            "orthotropic behaviour at this time.");
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
        tfel::raise(
            "CastemInterface::buildMaterialPropertiesList: "
            "unsupported symmetry type for cohesive zone modes");
      }
    } else {
      tfel::raise(
          "CastemInterface::buildMaterialPropertiesList: "
          "unsupported behaviour type");
    }
    if (!mprops.empty()) {
      const auto& m = mprops.back();
      res.second = m.offset;
      res.second += SupportedTypes::getTypeSize(m.type, m.arraySize);
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

  bool CastemInterface::writeInitializeAxialStrain(
      std::ostream& out, const BehaviourDescription& mb, const char c) const {
    if (mb.isModellingHypothesisSupported(ModellingHypothesis::PLANESTRESS)) {
      const auto v = this->checkIfAxialStrainIsDefinedAndGetItsOffset(mb);
      if (v.first) {
        out << "const CastemReal ezz" << c << " = STATEV["
            << v.second.getValueForDimension(2) << "];\n";
      } else {
        // no axial strain
        out << "std::cerr << \"no state variable standing for the axial strain "
               "`\"\n"
            << "\"(variable with the glossary name 'AxialStrain')\" << "
               "std::endl;\n"
            << "*KINC=-1;\n"
            << "return;\n";
        return false;
      }
    } else {
      // generic algorithm, this means that the behaviour
      // can be called in generalised plane strain
      const auto& d =
          mb.getBehaviourData(ModellingHypothesis::GENERALISEDPLANESTRAIN);
      const auto s = d.getPersistentVariables().getTypeSize();
      if (s.getValueForDimension(2) == 0) {
        out << "const CastemReal ezz" << c << " = STATEV[0];\n";
      } else {
        out << "if(*NSTATV!=" << s.getValueForDimension(2) + 1 << "){\n"
            << "std::cerr << \"invalid number of internal state variables\" << "
               "std::endl;\n"
            << "*KINC=-1;\n"
            << "return;\n"
            << "}\n"
            << "const CastemReal ezz" << c << " = STATEV["
            << s.getValueForDimension(2) << "];\n";
      }
    }
    return true;
  }  // end of CastemInterface::writeInitializeAxialStrain

  void CastemInterface::writeFiniteStrainStrategiesPlaneStressSpecificCall(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& c,
      const std::string& c2,
      const char c3) const {
    if ((mb.isModellingHypothesisSupported(ModellingHypothesis::PLANESTRESS)) ||
        (this->usesGenericPlaneStressAlgorithm(mb))) {
      out << "if(*NDI=="
          << getCastemModellingHypothesisIndex(ModellingHypothesis::PLANESTRESS)
          << "){\n";
      if (this->writeInitializeAxialStrain(out, mb, c3)) {
        out << "const CastemReal Fzz" << c3 << " = " << c2 << ";\n"
            << c << ",Fzz" << c3 << ");\n";
      }
      out << "} else {\n"
          << c << ",0);\n"
          << "}\n";
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
    tfel::raise_if(
        mb.getBehaviourType() !=
            BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
        "CastemInterface::writeFiniteRotationSmallStrainCastemFunction: "
        "finite strain strategies shall be used with small strain behaviours");
    out << "MFRONT_SHAREDOBJ void\n" << fname;
    writeUMATArguments(out,
                       BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR);
    out << "\n{\n"
        << "using namespace castem;\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n";
      out << "using tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    out << "const auto k = std::abs(*DDSDDE)>0.5;\n";
    this->generateMTestFile1(out);
    out << "// computing the Green Lagrange strains\n"
        << "CastemReal eto[6];\n"
        << "CastemReal deto[6];\n"
        << "CastemInt  i;\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "{\n"
          << "BehaviourProfiler::Timer pre_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
    }
    out << "CastemFiniteStrain::computeGreenLagrangeStrain(eto,F0,*NTENS,*NDI);"
           "\n"
        << "CastemFiniteStrain::computeGreenLagrangeStrain(deto,F1,*NTENS,*NDI)"
           ";\n";
    const auto c1 =
        "CastemFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress("
        "STRESS,F0,*NTENS,*NDI";
    this->writeFiniteStrainStrategiesPlaneStressSpecificCall(
        out, mb, c1, "std::sqrt(1+2*ezz0)", '0');
    out << "for(i=0;i!=*NTENS;++i){\n"
        << "deto[i] -= eto[i];\n"
        << "}\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "}\n";
    }
    out << "umat" << makeLowerCase(name)
        << "_base(NTENS, DTIME,DROT,DDSDDE,eto,deto,TEMP,DTEMP,\n"
        << "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
        << "STRESS,PNEWDT,NDI,KINC,\n"
        << "castem::CastemStandardSmallStrainStressFreeExpansionHandler);\n"
        << "if(*KINC==1){\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "BehaviourProfiler::Timer post_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
    }
    const auto c2 =
        "CastemFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress("
        "STRESS,F1,*NTENS,*NDI";
    this->writeFiniteStrainStrategiesPlaneStressSpecificCall(
        out, mb, c2, "std::sqrt(1+2*ezz1)", '1');
    out << "if(k){\n";
    const auto c3 =
        "CastemFiniteStrain::convertCSEtoCauchyTruesdellRateModuli(DDSDDE,"
        "STRESS,F1,*NTENS,*NDI";
    this->writeFiniteStrainStrategiesPlaneStressSpecificCall(
        out, mb, c3, "std::sqrt(1+2*ezz1)", '1');
    out << "}\n"
        << "}\n";
    if (this->generateMTestFile) {
      out << "if(*KINC!=1){\n";
      this->generateMTestFile2(
          out, BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR, name,
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
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b,
          "CastemInterface::"
          "writeMieheApelLambrechtLogarithmicStrainCastemFunction: " +
              m);
    };
    throw_if(mb.getBehaviourType() !=
                 BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
             "finite strain strategies shall be used with "
             "small strain behaviours");
    out << "MFRONT_SHAREDOBJ void\n" << fname;
    writeUMATArguments(out,
                       BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR);
    out << "\n{\n"
        << "using namespace castem;\n"
        << "using namespace tfel::math;\n"
        << "using namespace tfel::material;\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n";
      out << "using tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    out << "const auto k = std::abs(*DDSDDE)>0.5;\n";
    this->generateMTestFile1(out);
    auto preprocessing = [&out, this, &mb](const unsigned short d,
                                           const unsigned short n,
                                           const bool ps) {
      if (ps) {
        out << "LogarithmicStrainHandler<" << d << ",CastemReal> "
            << "lsh0(LogarithmicStrainHandlerBase::EULERIAN,\n"
            << "     tensor<" << d
            << ",CastemReal>::buildFromFortranMatrix(F0),false);\n"
            << "LogarithmicStrainHandler<" << d << ",CastemReal> "
            << "lsh1(LogarithmicStrainHandlerBase::EULERIAN,\n"
            << "     tensor<" << d
            << ",CastemReal>::buildFromFortranMatrix(F1),false);\n";
        if (this->writeInitializeAxialStrain(out, mb, '0')) {
          out << "lsh0.updateAxialDeformationGradient(std::exp(ezz0));\n";
        }
      } else {
        out << "LogarithmicStrainHandler<" << d << ",CastemReal> "
            << "lsh0(LogarithmicStrainHandlerBase::EULERIAN,\n"
            << "     tensor<" << d
            << ",CastemReal>::buildFromFortranMatrix(F0));\n"
            << "LogarithmicStrainHandler<" << d << ",CastemReal> "
            << "lsh1(LogarithmicStrainHandlerBase::EULERIAN,\n"
            << "     tensor<" << d
            << ",CastemReal>::buildFromFortranMatrix(F1));\n";
      }
      out << "CastemReal eto[" << n << "];\n"
          << "CastemReal deto[" << n << "];\n";
      if (mb.getAttribute(BehaviourData::profiling, false)) {
        out << "{\n"
            << "BehaviourProfiler::Timer pre_timer(" << mb.getClassName()
            << "Profiler::getProfiler(),\n"
            << "BehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
      }
      out << "lsh0.getHenckyLogarithmicStrain(eto);\n"
          << "lsh1.getHenckyLogarithmicStrain(deto);\n";
      for (unsigned short i = 0; i != n; ++i) {
        out << "deto[" << i << "]-=eto[" << i << "];\n";
      }
      out << "lsh0.convertFromCauchyStress(STRESS);\n";
      if (mb.getAttribute(BehaviourData::profiling, false)) {
        out << "}\n";
      }
    };
    auto postprocessing = [&out, this, &mb](const bool ps) {
      if (ps) {
        if (this->writeInitializeAxialStrain(out, mb, '1')) {
          out << "lsh1.updateAxialDeformationGradient(std::exp(ezz1));\n";
        }
      }
      if (mb.getAttribute(BehaviourData::profiling, false)) {
        out << "{\n"
            << "BehaviourProfiler::Timer post_timer(" << mb.getClassName()
            << "Profiler::getProfiler(),\n"
            << "BehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
      }
      out << "// converting the consistent tangent operator\n"
          << "if(k){\n"
          << "  "
             "lsh1.convertToCauchyStressTruesdellRateTangentModuli(DDSDDE,"
             "STRESS);\n"
          << "}\n"
          << "// converting the stress\n"
          << "lsh1.convertToCauchyStress(STRESS);\n";
      if (mb.getAttribute(BehaviourData::profiling, false)) {
        out << "}\n";
      }
    };
    auto ndi_dispatch = [this, &out, &name, &mb, &suffix, &preprocessing,
                         &postprocessing, &throw_if](
                            const int ndi, const Hypothesis h, const bool b) {
      out << (b ? "if" : " else if") << "(*NDI==" << ndi << "){\n";
      if (!this->isModellingHypothesisSupported(h, mb)) {
        // don't do any pre- or post-processing
        out << "	umat" << makeLowerCase(name) << "_base_"
            << ModellingHypothesis::toUpperCaseString(h)
            << "(NTENS,DTIME,DROT,DDSDDE,nullptr,nullptr,\n"
            << " TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,\n"
            << " STATEV,NSTATV,STRESS,PNEWDT,KINC,\n"
            << " castem::CastemLogarithmicStrainStressFreeExpansionHandler);\n"
            << "}";
        return;
      }
      if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        preprocessing(3u, 6u, false);
      } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
                 (h == ModellingHypothesis::PLANESTRAIN) ||
                 (h == ModellingHypothesis::PLANESTRESS) ||
                 (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
        preprocessing(2u, 4u, h == ModellingHypothesis::PLANESTRESS);
      } else {
        throw_if(h != ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
                 "unsupported modelling hypothesis");
        preprocessing(1u, 3u, false);
      }
      out << "	umat" << makeLowerCase(name) << "_base_"
          << ModellingHypothesis::toUpperCaseString(h)
          << "(NTENS,DTIME,DROT,DDSDDE,&eto[0],&deto[0],\n"
          << " TEMP,DTEMP,PROPS,NPROPS,PREDEF,DPRED,\n"
          << " STATEV,NSTATV,STRESS,PNEWDT,KINC,\n"
          << " castem::CastemLogarithmicStrainStressFreeExpansionHandler);\n"
          << "if(*KINC==1){\n";
      if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        postprocessing(false);
      } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
                 (h == ModellingHypothesis::PLANESTRAIN) ||
                 (h == ModellingHypothesis::PLANESTRESS) ||
                 (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
        postprocessing(h == ModellingHypothesis::PLANESTRESS);
      } else {
        throw_if(h != ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
                 "unsupported modelling hypothesis");
        postprocessing(false);
      }
      out << "}\n";
      if (this->generateMTestFile) {
        out << "if(*KINC!=1){\n";
        this->generateMTestFile2(
            out, BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR, name,
            suffix, mb);
        out << "}\n";
      }
      out << "}";
    };
    ndi_dispatch(2, ModellingHypothesis::TRIDIMENSIONAL, true);
    ndi_dispatch(0, ModellingHypothesis::AXISYMMETRICAL, false);
    ndi_dispatch(-1, ModellingHypothesis::PLANESTRAIN, false);
    ndi_dispatch(-2, ModellingHypothesis::PLANESTRESS, false);
    ndi_dispatch(-3, ModellingHypothesis::GENERALISEDPLANESTRAIN, false);
    ndi_dispatch(14, ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
                 false);
    out << " else {\n"
        << "castem::CastemInterfaceExceptions::"
           "displayInvalidModellingHypothesisErrorMessage();\n"
        << "*KINC = -7;\n"
        << "}\n"
        << "}\n\n";
  }

  void CastemInterface::writeLogarithmicStrain1DCastemFunction(
      std::ostream& out,
      const std::string& name,
      const std::string& fname,
      const std::string& suffix,
      const BehaviourDescription& mb) const {
    using namespace std;
    tfel::raise_if(
        mb.getBehaviourType() !=
            BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
        "CastemInterface::writeLogarithmicStrain1DCastemFunction : "
        "finite strain strategies shall be used with small strain behaviours");
    out << "MFRONT_SHAREDOBJ void\n" << fname;
    writeUMATArguments(out, BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR);
    out << "\n{\n"
        << "using namespace castem;\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n";
      out << "using tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out);
    out << "bool k = std::abs(*DDSDDE)>0.5;\n"
        << "// computing the logarithmic strain\n"
        << "CastemReal eto[3];\n"
        << "CastemReal deto[3];\n"
        << "CastemReal s[3];\n"
        << "CastemReal K[9];\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "{\n"
          << "BehaviourProfiler::Timer pre_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
    }
    out << "if(*NDI!=14){\n"
        << "*KINC=-7;\n"
        << "return;\n"
        << "}\n"
        << "eto[0]=std::log1p(*STRAN);\n"
        << "eto[1]=std::log1p(*(STRAN+1));\n"
        << "eto[2]=std::log1p(*(STRAN+2));\n"
        << "deto[0]=std::log1p(*STRAN+*DSTRAN)-eto[0];\n"
        << "deto[1]=std::log1p(*(STRAN+1)+*(DSTRAN+1))-eto[1];\n"
        << "deto[2]=std::log1p(*(STRAN+2)+*(DSTRAN+2))-eto[2];\n"
        << "s[0]=(*STRESS)*(1+*STRAN);\n"
        << "s[1]=(*(STRESS+1))*(1+*(STRAN+1));\n"
        << "s[2]=(*(STRESS+2))*(1+*(STRAN+2));\n"
        << "K[0]=*DDSDDE;\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "}\n";
    }
    out << "umat" << makeLowerCase(name)
        << "_base(NTENS, DTIME,DROT,K,eto,deto,TEMP,DTEMP,\n"
        << "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
        << "s,PNEWDT,NDI,KINC,\n"
        << "castem::CastemLogarithmicStrainStressFreeExpansionHandler);\n"
        << "if(*KINC==1){\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "{\n"
          << "BehaviourProfiler::Timer post_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
    }
    // First Piola-Kirchhoff stress
    out << "STRESS[0]=s[0]/(1+*STRAN+*DSTRAN);\n"
        << "STRESS[1]=s[1]/(1+*(STRAN+1)+*(DSTRAN+1));\n"
        << "STRESS[2]=s[2]/(1+*(STRAN+2)+*(DSTRAN+2));\n";
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
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "}\n";
    }
    out << "}\n";
    if (this->generateMTestFile) {
      out << "if(*KINC!=1){\n";
      this->generateMTestFile2(
          out, BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR, name, suffix,
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
        BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "umat" << makeLowerCase(name)
          << "_base(NTENS, DTIME,DROT,DDSDDE,F0,F1,TEMP,DTEMP,\n"
          << "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
          << "STRESS,PNEWDT,NDI,KINC,nullptr);\n";
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
    this->writeVariableDescriptionsToGibiane(out, h, v.begin(), v.end());
  }

  void CastemInterface::writeVariableDescriptionsToGibiane(
      std::ostream& out,
      const Hypothesis h,
      const VariableDescriptionContainer::const_iterator pb,
      const VariableDescriptionContainer::const_iterator pe) const {
    for (auto p = pb; p != pe; ++p) {
      const auto flag = SupportedTypes::getTypeFlag(p->type);
      std::string tmp;
      tmp += ' ';
      if (flag == SupportedTypes::Scalar) {
        if (p->arraySize == 1) {
          tmp += treatScalar(p->name);
        } else {
          for (unsigned short j = 0; j != p->arraySize;) {
            tmp += treatScalar(p->name, j);
            if (++j != p->arraySize) {
              tmp += ' ';
            }
          }
        }
      } else if (flag == SupportedTypes::Stensor) {
        if (p->arraySize == 1) {
          tmp += treatStensor(h, p->name);
        } else {
          for (unsigned short j = 0; j != p->arraySize;) {
            tmp += treatStensor(h, p->name, j);
            if (++j != p->arraySize) {
              tmp += ' ';
            }
          }
        }
      } else if (flag == SupportedTypes::Tensor) {
        if (p->arraySize == 1) {
          tmp += treatTensor(h, p->name);
        } else {
          for (unsigned short j = 0; j != p->arraySize;) {
            tmp += treatTensor(h, p->name, j);
            if (++j != p->arraySize) {
              tmp += ' ';
            }
          }
        }
      } else {
        tfel::raise(
            "CastemInterface::writeVariableDescriptionContainerToGibiane: "
            "internal error, tag unsupported");
      }
      out << tmp;
    }
  }

  void CastemInterface::writeGibianeInstruction(std::ostream& out,
                                                const std::string& i) const {
    std::istringstream in(i);
    auto buffer = std::string{};
    while (!in.eof()) {
      auto w = std::string{};
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
      auto throw_if = [](const bool b, const std::string& m) {
        tfel::raise_if(b, "CastemInterface::generateGibianeDeclaration: " + m);
      };
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
      tfel::system::systemCall::mkdir("castem");
      std::ofstream out;
      out.open(fileName);
      throw_if(!out, "could not open file '" + fileName + "'");
      // header
      out << "*\n"
          << "* \\file   " << fd.fileName << '\n'
          << "* \\brief  example of how to use the " << mb.getClassName()
          << " behaviour law\n"
          << "* in the Cast3M finite element solver\n"
          << "* \\author " << fd.authorName << '\n'
          << "* \\date   " << fd.date << '\n'
          << "*\n\n";
      // specific declaration
      auto nonlin = std::string{};
      if (mb.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        nonlin = "'NON_LINEAIRE' 'UTILISATEUR'";
      } else if (mb.getBehaviourType() ==
                 BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        nonlin = "'NON_LINEAIRE' 'UTILISATEUR' 'EPSILON' 'UTILISATEUR'";
      } else {
        throw_if(true, "internal error, unsupported behaviour type");
      }
      // loop over hypothesis
      for (const auto& h : this->getModellingHypothesesToBeTreated(mb)) {
        const auto& d = mb.getBehaviourData(h);
        const auto& persistentVarsHolder = d.getPersistentVariables();
        const auto& externalStateVarsHolder = d.getExternalStateVariables();
        const auto mprops = this->buildMaterialPropertiesList(mb, h);
        auto tmp = std::string{};
        out << "** " << ModellingHypothesis::toString(h) << " example\n";
        if (mo.find(h) != mo.end()) {
          out << "** 'OPTION' 'DIMENSION' " << getSpaceDimension(h)
              << " 'MODELISER' " << mo[h] << " ;\n\n";
        }
        std::ostringstream mcoel;
        mcoel << "coel = 'MOTS' ";
        for (auto pm = mprops.first.cbegin(); pm != mprops.first.cend();) {
          const auto flag = SupportedTypes::getTypeFlag(pm->type);
          throw_if(flag != SupportedTypes::Scalar,
                   "material properties shall be scalars");
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
          std::ostringstream mstatev;
          mstatev << "statev = 'MOTS' ";
          this->writeVariableDescriptionContainerToGibiane(
              mstatev, h, persistentVarsHolder);
          mstatev << ";";
          writeGibianeInstruction(out, mstatev.str());
          out << '\n';
        }
        std::ostringstream mparam;
        mparam << "params = 'MOTS' 'T'";
        if (!externalStateVarsHolder.empty()) {
          mparam << " ";
          const auto pb = std::next(externalStateVarsHolder.begin());
          const auto pe = externalStateVarsHolder.end();
          this->writeVariableDescriptionsToGibiane(mparam, h, pb, pe);
        }
        mparam << ";";
        writeGibianeInstruction(out, mparam.str());
        out << '\n';
        std::ostringstream mmod;
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
        std::ostringstream mi;
        mi << "MA = 'MATERIAU' MO ";
        auto mpos = 0;
        for (auto pm = mprops.first.cbegin(); pm != mprops.first.cend();
             ++mpos) {
          const auto flag = SupportedTypes::getTypeFlag(pm->type);
          throw_if(flag != SupportedTypes::Scalar,
                   "material properties shall be scalars");
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
    auto lthrow = [](const std::string& m) {
      tfel::raise("CastemInterface::writeUMATBehaviourTraits: " + m);
    };
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
      out << "using  ModellingHypothesis = "
             "tfel::material::ModellingHypothesis;\n"
          << "using  ModellingHypothesisToSpaceDimension = "
          << "tfel::material::ModellingHypothesisToSpaceDimension<"
             "ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ">;\n";
    } else {
      out << "using ModellingHypothesis = "
             "tfel::material::ModellingHypothesis;\n"
          << "using ModellingHypothesisToSpaceDimension = "
          << "tfel::material::ModellingHypothesisToSpaceDimension<H>;\n";
    }
    if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      out << "static " << constexpr_c << " ModellingHypothesis::Hypothesis H = "
          << "ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ";\n";
    }
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      out << "static " << constexpr_c
          << " CastemBehaviourType btype  = STANDARDSTRAINBASEDBEHAVIOUR;\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "static " << constexpr_c
          << " CastemBehaviourType btype  = STANDARDFINITESTRAINBEHAVIOUR;\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::COHESIVEZONEMODEL) {
      out << "static " << constexpr_c
          << " CastemBehaviourType btype  = COHESIVEZONEMODEL;\n";
    } else {
      lthrow("unsupported behaviour type");
    }
    out << "// space dimension\n"
        << "static " << constexpr_c
        << " unsigned short N           = "
           "ModellingHypothesisToSpaceDimension::value;\n"
        << "// tiny vector size\n"
        << "static " << constexpr_c << " unsigned short TVectorSize = N;\n"
        << "// symmetric tensor size\n"
        << "static " << constexpr_c
        << " unsigned short StensorSize = "
           "tfel::math::StensorDimeToSize<N>::value;\n"
        << "// tensor size\n"
        << "static " << constexpr_c
        << " unsigned short TensorSize  = "
           "tfel::math::TensorDimeToSize<N>::value;\n"
        << "// size of the driving variable array (STRAN)\n"
        << "static " << constexpr_c
        << " unsigned short DrivingVariableSize = " << mvs.first << ";\n"
        << "// size of the thermodynamic force variable array (STRESS)\n"
        << "static " << constexpr_c
        << " unsigned short ThermodynamicForceVariableSize = " << mvs.second
        << ";\n"
        << "static " << constexpr_c << " bool useTimeSubStepping = ";
    if (mb.getAttribute<bool>(CastemInterface::useTimeSubStepping, false)) {
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
    if (mb.getAttribute<bool>(CastemInterface::useTimeSubStepping, false)) {
      out << mb.getAttribute<unsigned short>(
                 CastemInterface::maximumSubStepping)
          << ";\n";
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
      msize += SupportedTypes::getTypeSize(m.type, m.arraySize);
      msize -= mprops.second;
    }
    out << "static " << constexpr_c
        << " unsigned short material_properties_nb = " << msize << ";\n";
    if (mb.getSymmetryType() == mfront::ISOTROPIC) {
      if (mb.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        out << "static " << constexpr_c << " unsigned short propertiesOffset = "
            << "CastemIsotropicOffset<castem::STANDARDSTRAINBASEDBEHAVIOUR,H>::"
               "value;\n";
      } else if (mb.getBehaviourType() ==
                 BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        out << "static " << constexpr_c << " unsigned short propertiesOffset = "
            << "CastemIsotropicOffset<castem::STANDARDFINITESTRAINBEHAVIOUR,H>:"
               ":value;\n";
      } else if (mb.getBehaviourType() ==
                 BehaviourDescription::COHESIVEZONEMODEL) {
        out << "static " << constexpr_c << " unsigned short propertiesOffset = "
            << "CastemIsotropicOffset<castem::COHESIVEZONEMODEL,H>::value;\n";
      } else {
        lthrow("unsupported behaviour type");
      }
    } else if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      if (mb.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        out << "static " << constexpr_c << " unsigned short propertiesOffset = "
            << "CastemOrthotropicOffset<castem::STANDARDSTRAINBASEDBEHAVIOUR,H>"
               "::value;\n";
      } else if (mb.getBehaviourType() ==
                 BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        out << "static " << constexpr_c << " unsigned short propertiesOffset = "
            << "CastemOrthotropicOffset<castem::STANDARDFINITESTRAINBEHAVIOUR,"
               "H>::value;\n";
      } else {
        lthrow("unsupported behaviour type");
      }
    } else {
      lthrow(
          "unsupported behaviour symmetry type.\n"
          "The umat interface only support isotropic or "
          "orthotropic behaviour at this time.");
    }
    if (mb.getSymmetryType() == mfront::ISOTROPIC) {
      out << "static " << constexpr_c
          << " CastemSymmetryType stype = castem::ISOTROPIC;\n";
    } else if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "static " << constexpr_c
          << " CastemSymmetryType stype = castem::ORTHOTROPIC;\n";
    } else {
      lthrow(
          "unsupported behaviour symmetry type.\n"
          "The umat interface only support isotropic or "
          "orthotropic behaviour at this time.");
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
