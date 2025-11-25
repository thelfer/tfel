/*!
 * \file   mfront/src/AnsysInterface.cxx
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

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/System.hxx"

#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontLock.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/Ansys/AnsysOrthotropicBehaviour.hxx"
#include "MFront/AnsysSymbolsGenerator.hxx"
#include "MFront/AnsysInterface.hxx"

#ifndef _MSC_VER
static const char* const constexpr_c = "constexpr";
#else
static const char* const constexpr_c = "const";
#endif

namespace mfront {

  static void copyAnsysFile(const std::string& f) {
    const auto root = tfel::getInstallPath();
    std::ofstream out("ansys/" + f);
#ifdef TFEL_APPEND_SUFFIX
    const auto fn = root + "/share/doc/mfront-" VERSION "/ansys/" + f;
#else  /* TFEL_APPEND_SUFFIX */
    const auto fn = root + "/share/doc/mfront/ansys/" + f;
#endif /* TFEL_APPEND_SUFFIX */
    std::ifstream in{fn};
    if (in) {
      out << in.rdbuf();
      in.close();
    } else {
      std::cerr << "AnsysInterface::endTreatment: "
                << "could not open file '" << fn << "'" << std::endl;
    }
    out.close();
  }  // namespace copyAnsysFile

  static void checkFiniteStrainStrategy(const std::string& fs) {
    tfel::raise_if((fs != "FiniteRotationSmallStrain") && (fs != "Native") &&
                       (fs != "MieheApelLambrechtLogarithmicStrain"),
                   "checkFiniteStrainStrategy: "
                   "unsupported strategy '" +
                       fs +
                       "'\n"
                       "The only supported strategies are "
                       "'Native', 'FiniteRotationSmallStrain' and "
                       "'MieheApelLambrechtLogarithmicStrain'.");
  }  // end of checkFiniteStrainStrategy

  static void checkFiniteStrainStrategyDefinitionConsistency(
      const BehaviourDescription& bd, const std::string& fs) {
    auto throw_if = [](const bool c, const std::string& msg) {
      tfel::raise_if(c,
                     "checkFiniteStrainStrategyDefinitionConsistency: " + msg);
    };
    checkFiniteStrainStrategy(fs);
    if (bd.isStrainMeasureDefined()) {
      const auto ms = bd.getStrainMeasure();
      if (ms == BehaviourDescription::LINEARISED) {
        throw_if(fs != "Native",
                 "incompatible finite strain strategy "
                 "'" +
                     fs + "' (only `Native` accepted)");
      } else if (ms == BehaviourDescription::GREENLAGRANGE) {
        throw_if(fs != "FiniteRotationSmallStrain",
                 "incompatible finite strain strategy "
                 "'" +
                     fs + "' (only `FiniteRotationSmallStrain` accepted)");
      } else if (ms == BehaviourDescription::HENCKY) {
        throw_if(fs != "MieheApelLambrechtLogarithmicStrain",
                 "incompatible finite strain strategy '" + fs +
                     "' "
                     "(only `MieheApelLambrechtLogarithmicStrain` accepted)");
      } else {
        throw_if(true, "unsupported finite strain strategy");
      }
    }
  }  // end of checkFiniteStrainStrategyDefinitionConsistency

  static void checkFiniteStrainStrategyDefinitionConsistency(
      const BehaviourDescription& bd) {
    auto throw_if = [](const bool c, const std::string& msg) {
      tfel::raise_if(c,
                     "checkFiniteStrainStrategyDefinitionConsistency: " + msg);
    };
    if (bd.getBehaviourType() !=
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      throw_if(bd.hasAttribute(AnsysInterface::finiteStrainStrategy),
               "finite strain strategy is only supported for strain based "
               "behaviours");
    } else {
      if (bd.hasAttribute(AnsysInterface::finiteStrainStrategy)) {
        const auto fs =
            bd.getAttribute<std::string>(AnsysInterface::finiteStrainStrategy);
        checkFiniteStrainStrategyDefinitionConsistency(bd, fs);
      }
    }
  }  // end of checkFiniteStrainStrategyDefinitionConsistency

  bool AnsysInterface::hasFiniteStrainStrategy(const BehaviourDescription& bd) {
    checkFiniteStrainStrategyDefinitionConsistency(bd);
    if (bd.isStrainMeasureDefined()) {
      return bd.getStrainMeasure() != BehaviourDescription::LINEARISED;
    }
    return bd.hasAttribute(AnsysInterface::finiteStrainStrategy);
  }  // end of AnsysInterface::hasFiniteStrainStrategy

  std::string AnsysInterface::getFiniteStrainStrategy(
      const BehaviourDescription& bd) {
    checkFiniteStrainStrategyDefinitionConsistency(bd);
    auto throw_if = [](const bool c, const std::string& msg) {
      tfel::raise_if(c, "getFiniteStrainStrategy: " + msg);
    };
    if (bd.isStrainMeasureDefined()) {
      const auto ms = bd.getStrainMeasure();
      if (ms == BehaviourDescription::GREENLAGRANGE) {
        return "FiniteRotationSmallStrain";
      } else if (ms == BehaviourDescription::HENCKY) {
        return "MieheApelLambrechtLogarithmicStrain";
      } else {
        throw_if(true, "unsupported strain measure");
      }
    }
    throw_if(!bd.hasAttribute(AnsysInterface::finiteStrainStrategy),
             "no finite strain strategy defined");
    return bd.getAttribute<std::string>(AnsysInterface::finiteStrainStrategy);
  }  // end of AnsysInterface::getFiniteStrainStrategy

  static void writeArguments(std::ostream& out,
                             const BehaviourDescription& bd,
                             const bool base) {
    if (!base) {
      const auto requires_stran = [&bd] {
        if (bd.getBehaviourType() ==
            BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
          if (AnsysInterface::hasFiniteStrainStrategy(bd)) {
            const auto fs = AnsysInterface::getFiniteStrainStrategy(bd);
            return fs == "Native";
          }
        }
        return true;
      }();
      out << "(const ansys::AnsysInt *const matId,\n"
          << " const ansys::AnsysInt *const elemId,\n"
          << " const ansys::AnsysInt *const kDomIntPt,\n"
          << " const ansys::AnsysInt *const kLayer,\n"
          << " const ansys::AnsysInt *const kSectPt,\n"
          << " const ansys::AnsysInt *const ldsetp,\n"
          << " const ansys::AnsysInt *const isubst,\n"
          << "       ansys::AnsysInt *const keycut,\n"
          << " const ansys::AnsysInt *const nDirect,\n"
          << " const ansys::AnsysInt *const nShear,\n"
          << " const ansys::AnsysInt *const NTENS,\n"
          << " const ansys::AnsysInt *const NSTATV,\n"
          << " const ansys::AnsysInt *const NPROPS,\n"
          << " const ansys::AnsysReal *const TIME,\n"
          << " const ansys::AnsysReal *const DTIME,\n"
          << " const ansys::AnsysReal *const TEMP,\n"
          << " const ansys::AnsysReal *const DTEMP,\n"
          << "       ansys::AnsysReal *const STRESS,\n"
          << "       ansys::AnsysReal *const STATEV,\n"
          << "       ansys::AnsysReal *const DDSDDE,\n"
          << "       ansys::AnsysReal *const SEDEL,\n"
          << "       ansys::AnsysReal *const SEDPL,\n"
          << "       ansys::AnsysReal *const EPSEQ,\n";
      if (requires_stran) {
        out << " const ansys::AnsysReal *const STRAN,\n"
            << " const ansys::AnsysReal *const DSTRAN,\n";
      } else {
        out << " const ansys::AnsysReal *const,\n"
            << " const ansys::AnsysReal *const,\n";
      }
      out << "       ansys::AnsysReal *const EPSPL,\n"
          << " const ansys::AnsysReal *const PROPS,\n"
          << " const ansys::AnsysReal *const coords,\n"
          << " const ansys::AnsysReal *const DROT,\n"
          << " const ansys::AnsysReal *const F0,\n"
          << " const ansys::AnsysReal *const F1,\n"
          << " const ansys::AnsysReal *const tsstif,\n"
          << "       ansys::AnsysReal *const EPSZZ,\n"
          << " const ansys::AnsysReal *const var1,\n"
          << " const ansys::AnsysReal *const var2,\n"
          << " const ansys::AnsysReal *const var3,\n"
          << " const ansys::AnsysReal *const var4,\n"
          << " const ansys::AnsysReal *const var5,\n"
          << " const ansys::AnsysReal *const var6,\n"
          << " const ansys::AnsysReal *const var7,\n"
          << " const ansys::AnsysReal *const var8)";
    } else {
      out << "(const ansys::AnsysInt *const,\n"
          << " const ansys::AnsysInt *const,\n"
          << " const ansys::AnsysInt *const,\n"
          << " const ansys::AnsysInt *const,\n"
          << " const ansys::AnsysInt *const,\n"
          << " const ansys::AnsysInt *const,\n"
          << " const ansys::AnsysInt *const,\n"
          << "       ansys::AnsysInt *const keycut,\n"
          << " const ansys::AnsysInt *const,\n"
          << " const ansys::AnsysInt *const,\n"
          << " const ansys::AnsysInt *const NTENS,\n"
          << " const ansys::AnsysInt *const NSTATV,\n"
          << " const ansys::AnsysInt *const NPROPS,\n"
          << " const ansys::AnsysReal *const,\n"
          << " const ansys::AnsysReal *const DTIME,\n"
          << " const ansys::AnsysReal *const TEMP,\n"
          << " const ansys::AnsysReal *const DTEMP,\n"
          << "       ansys::AnsysReal *const STRESS,\n"
          << "       ansys::AnsysReal *const STATEV,\n"
          << "       ansys::AnsysReal *const DDSDDE,\n"
          << "       ansys::AnsysReal *const SEDEL,\n"
          << "       ansys::AnsysReal *const SEDPL,\n"
          << "       ansys::AnsysReal *const EPSEQ,\n";
      if (bd.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        out << " const ansys::AnsysReal *const STRAN,\n"
            << " const ansys::AnsysReal *const DSTRAN,\n";
      } else {
        out << " const ansys::AnsysReal *const,\n"
            << " const ansys::AnsysReal *const,\n";
      }
      out << "       ansys::AnsysReal *const EPSPL,\n"
          << " const ansys::AnsysReal *const PROPS,\n"
          << " const ansys::AnsysReal *const,\n"
          << " const ansys::AnsysReal *const DROT,\n";
      if (bd.getBehaviourType() ==
          BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        out << " const ansys::AnsysReal *const F0,\n"
            << " const ansys::AnsysReal *const F1,\n";
      } else {
        out << " const ansys::AnsysReal *const,\n"
            << " const ansys::AnsysReal *const,\n";
      }
      out << " const ansys::AnsysReal *const,\n"
          << "       ansys::AnsysReal *const EPSZZ,\n"
          << " const ansys::AnsysReal *const,\n"
          << " const ansys::AnsysReal *const,\n"
          << " const ansys::AnsysReal *const,\n"
          << " const ansys::AnsysReal *const,\n"
          << " const ansys::AnsysReal *const,\n"
          << " const ansys::AnsysReal *const,\n"
          << " const ansys::AnsysReal *const,\n"
          << " const ansys::AnsysReal *const)";
    }
  }  // end of writeArguments

  static void writeArguments(std::ostream& out) {
    out << "(const ansys::AnsysInt *const,\n"
        << " const ansys::AnsysInt *const,\n"
        << " const ansys::AnsysInt *const,\n"
        << " const ansys::AnsysInt *const,\n"
        << " const ansys::AnsysInt *const,\n"
        << " const ansys::AnsysInt *const,\n"
        << " const ansys::AnsysInt *const,\n"
        << "       ansys::AnsysInt *const,\n"
        << " const ansys::AnsysInt *const,\n"
        << " const ansys::AnsysInt *const,\n"
        << " const ansys::AnsysInt *const,\n"
        << " const ansys::AnsysInt *const,\n"
        << " const ansys::AnsysInt *const,\n"
        << " const ansys::AnsysReal *const,\n"
        << " const ansys::AnsysReal *const,\n"
        << " const ansys::AnsysReal *const,\n"
        << " const ansys::AnsysReal *const,\n"
        << "       ansys::AnsysReal *const,\n"
        << "       ansys::AnsysReal *const,\n"
        << "       ansys::AnsysReal *const,\n"
        << "       ansys::AnsysReal *const,\n"
        << "       ansys::AnsysReal *const,\n"
        << "       ansys::AnsysReal *const,\n"
        << " const ansys::AnsysReal *const,\n"
        << " const ansys::AnsysReal *const,\n"
        << "       ansys::AnsysReal *const,\n"
        << " const ansys::AnsysReal *const,\n"
        << " const ansys::AnsysReal *const,\n"
        << " const ansys::AnsysReal *const,\n"
        << " const ansys::AnsysReal *const,\n"
        << " const ansys::AnsysReal *const,\n"
        << " const ansys::AnsysReal *const,\n"
        << "       ansys::AnsysReal *const,\n"
        << " const ansys::AnsysReal *const,\n"
        << " const ansys::AnsysReal *const,\n"
        << " const ansys::AnsysReal *const,\n"
        << " const ansys::AnsysReal *const,\n"
        << " const ansys::AnsysReal *const,\n"
        << " const ansys::AnsysReal *const,\n"
        << " const ansys::AnsysReal *const,\n"
        << " const ansys::AnsysReal *const)";
  }  // end of writeArguments

  std::string AnsysInterface::getLibraryName(
      const BehaviourDescription& mb) const {
    auto lib = std::string{};
    if (mb.getLibrary().empty()) {
      if (!mb.getMaterialName().empty()) {
        lib = this->getInterfaceName() + mb.getMaterialName();
      } else {
        lib = this->getInterfaceName() + "Behaviour";
      }
    } else {
      lib = this->getInterfaceName() + mb.getLibrary();
    }
    return lib;
  }  // end of AnsysInterface::getLibraryName

  unsigned short AnsysInterface::getStateVariablesOffset(
      const BehaviourDescription& mb, const Hypothesis h) const {
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
          (h == ModellingHypothesis::PLANESTRAIN) ||
          (h == ModellingHypothesis::PLANESTRESS)) {
        return 2u;
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return 6u;
      }
      tfel::raise(
          "AnsysInterface::getStateVariablesOffset: "
          "invalid hypothesis");
    }
    return 0u;
  }

  std::string AnsysInterface::getFunctionNameBasis(
      const std::string& name) const {
    return name;
  }  // end of AnsysInterface::getFunctionName

  std::string AnsysInterface::getFunctionNameForHypothesis(
      const std::string& name, const Hypothesis h) const {
    const auto s = [h]() -> std::string {
      if (h == ModellingHypothesis::AXISYMMETRICAL) {
        return "axis";
      } else if (h == ModellingHypothesis::PLANESTRAIN) {
        return "pstrain";
      } else if (h == ModellingHypothesis::PLANESTRESS) {
        return "pstress";
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return "3D";
      }
      tfel::raise(
          "AnsysInterface::getFunctionNameForHypothesis: "
          "invalid hypothesis.");
    }();
    return name + "_" + s;
  }  // end of AnsysInterface::getFunctionNameForHypothesis

  std::set<AnsysInterface::Hypothesis>
  AnsysInterface::getModellingHypothesesToBeTreated(
      const BehaviourDescription& mb) const {
    auto h = std::set<Hypothesis>{};
    const auto& bh = mb.getModellingHypotheses();
    if (bh.find(ModellingHypothesis::AXISYMMETRICAL) != bh.end()) {
      h.insert(ModellingHypothesis::AXISYMMETRICAL);
    }
    if (bh.find(ModellingHypothesis::PLANESTRAIN) != bh.end()) {
      h.insert(ModellingHypothesis::PLANESTRAIN);
    }
    if (bh.find(ModellingHypothesis::PLANESTRESS) != bh.end()) {
      h.insert(ModellingHypothesis::PLANESTRESS);
    }
    if (bh.find(ModellingHypothesis::TRIDIMENSIONAL) != bh.end()) {
      h.insert(ModellingHypothesis::TRIDIMENSIONAL);
    }
    tfel::raise_if(
        h.empty(),
        "AnsysInterface::getModellingHypothesesToBeTreated : "
        "no hypotheses selected. This means that the given beahviour "
        "can't be used neither in 'AxisymmetricalGeneralisedPlaneStrain' "
        "nor in 'AxisymmetricalGeneralisedPlaneStress', so it does not "
        "make sense to use the Ansys interface");
    return h;
  }  // end of AnsysInterface::getModellingHypothesesToBeTreated

  void AnsysInterface::writeAnsysBehaviourTraits(std::ostream& out,
                                                 const BehaviourDescription& mb,
                                                 const Hypothesis h) const {
    const auto mvs = mb.getMainVariablesSize();
    const auto mprops = this->buildMaterialPropertiesList(mb, h);
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      out << "template<tfel::material::ModellingHypothesis::Hypothesis "
             "H,typename NumericType";
      if (mb.useQt()) {
        out << ",bool use_qt";
      }
    } else {
      out << "template<typename NumericType";
      if (mb.useQt()) {
        out << ",bool use_qt";
      }
    }
    out << ">\n";
    out << "struct AnsysTraits<tfel::material::" << mb.getClassName() << "<";
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      out << "H";
    } else {
      out << "tfel::material::ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h);
    }
    out << ", NumericType, ";
    if (mb.useQt()) {
      out << "use_qt";
    } else {
      out << "false";
    }
    out << "> >\n{\n";
    out << "//! behaviour type\n";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      out << "static " << constexpr_c
          << " AnsysBehaviourType btype = "
             "ansys::STANDARDSTRAINBASEDBEHAVIOUR;"
             "\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "static " << constexpr_c
          << " AnsysBehaviourType btype = "
             "ansys::STANDARDFINITESTRAINBEHAVIOUR;"
             "\n";
    } else {
      tfel::raise(
          "AnsysInterface::writeAnsysBehaviourTraits : "
          "unsupported behaviour type");
    }
    out << "//! space dimension\n";
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      out << "static " << constexpr_c
          << " unsigned short N           = "
             "tfel::material::ModellingHypothesisToSpaceDimension<H>::value;\n";
    } else {
      out << "static " << constexpr_c
          << " unsigned short N           = "
             "tfel::material::ModellingHypothesisToSpaceDimension<"
          << "tfel::material::ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ">::value;\n";
    }
    out << "// tiny vector size\n";
    out << "static " << constexpr_c << " unsigned short TVectorSize = N;\n";
    out << "// symmetric tensor size\n";
    out << "static " << constexpr_c
        << " unsigned short StensorSize = "
           "tfel::math::StensorDimeToSize<N>::"
           "value;\n";
    out << "// tensor size\n";
    out << "static " << constexpr_c
        << " unsigned short TensorSize  = "
           "tfel::math::TensorDimeToSize<N>::value;"
           "\n";
    out << "// size of the driving variable array\n";
    out << "static " << constexpr_c
        << " unsigned short GradientSize = " << mvs.first << ";\n";
    out << "// size of the thermodynamic force variable array (STRESS)\n";
    out << "static " << constexpr_c
        << " unsigned short ThermodynamicForceVariableSize = " << mvs.second
        << ";\n";
    if (mb.getAttribute(BehaviourDescription::requiresUnAlteredStiffnessTensor,
                        false)) {
      out << "static " << constexpr_c
          << " bool requiresUnAlteredStiffnessTensor = true;\n";
    } else {
      out << "static " << constexpr_c
          << " bool requiresUnAlteredStiffnessTensor = false;\n";
    }
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      out << "static " << constexpr_c
          << " bool requiresStiffnessTensor = true;\n";
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
    if (mb.getSymmetryType() == mfront::ISOTROPIC) {
      out << "static " << constexpr_c
          << " AnsysSymmetryType type = ansys::ISOTROPIC;\n";
    } else if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "static " << constexpr_c
          << " AnsysSymmetryType type = ansys::ORTHOTROPIC;\n";
    } else {
      tfel::raise(
          "AnsysInterface::writeAnsysBehaviourTraits: "
          "unsupported behaviour type.\n"
          "The ansys interface only support isotropic or orthotropic "
          "behaviour at this time.");
    }
    // computing material properties size
    auto msize = SupportedTypes::TypeSize{};
    if (!mprops.first.empty()) {
      const auto& m = mprops.first.back();
      msize = m.offset;
      msize += SupportedTypes::getTypeSize(m.type, m.arraySize);
      msize -= mprops.second;
    }
    out << "static " << constexpr_c
        << " unsigned short material_properties_nb = " << msize << ";\n";
    if (mb.getElasticSymmetryType() == mfront::ISOTROPIC) {
      out << "static " << constexpr_c
          << " AnsysSymmetryType etype = ansys::ISOTROPIC;\n";
      if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                          false)) {
        out << "static " << constexpr_c
            << " unsigned short elasticPropertiesOffset = 2u;\n";
      } else {
        out << "static " << constexpr_c
            << " unsigned short elasticPropertiesOffset = 0u;\n";
      }
      if (mb.getAttribute(
              BehaviourDescription::requiresThermalExpansionCoefficientTensor,
              false)) {
        out << "static " << constexpr_c
            << " unsigned short thermalExpansionPropertiesOffset = 1u;\n";
      } else {
        out << "static " << constexpr_c
            << " unsigned short thermalExpansionPropertiesOffset = 0u;\n";
      }
      out << "static " << constexpr_c
          << " unsigned short orthotropicAxesOffset = 0u;\n";
    } else if (mb.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
      out << "static " << constexpr_c
          << " AnsysSymmetryType etype = ansys::ORTHOTROPIC;\n";
      if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                          false)) {
        out << "static " << constexpr_c
            << " unsigned short elasticPropertiesOffset "
            << "= AnsysOrthotropicElasticPropertiesOffset<N>::value;\n";
      } else {
        out << "static " << constexpr_c
            << " unsigned short elasticPropertiesOffset = 0u;\n";
      }
      if (mb.getAttribute(
              BehaviourDescription::requiresThermalExpansionCoefficientTensor,
              false)) {
        out << "static " << constexpr_c
            << " unsigned short thermalExpansionPropertiesOffset = 3u;\n";
      } else {
        out << "static " << constexpr_c
            << " unsigned short thermalExpansionPropertiesOffset = 0u;\n";
      }
      out << "static " << constexpr_c
          << " unsigned short orthotropicAxesOffset = "
             "AnsysOrthotropicAxesOffset<N>::value;\n";
    } else {
      tfel::raise(
          "AnsysInterface::writeAnsysBehaviourTraits: "
          "unsupported behaviour type.\n"
          "The ansys interface only support isotropic or "
          "orthotropic behaviour at this time.");
    }
    out << "}; // end of class AnsysTraits\n\n";
  }

  void AnsysInterface::writeMTestFileGeneratorSetModellingHypothesis(
      std::ostream& out) const {
    out << "mg.setModellingHypothesis(h);\n";
  }  // end of AnsysInterface::writeMTestFileGeneratorSetModellingHypothesis

  void AnsysInterface::writeInputFileExample(const BehaviourDescription& mb,
                                             const FileDescription& fd) const {
    try {
      auto throw_if = [](const bool c, const std::string& m) {
        tfel::raise_if(c, "AnsysInterface::writeInputFileExample: " + m);
      };
      const auto name = mb.getLibrary() + mb.getClassName();
      const auto mn = this->getLibraryName(mb) + "_" + mb.getClassName();
      const auto fn = "ansys/" + name + ".dat";
      std::ofstream out{fn};
      throw_if(!out, "could not open file '" + fn + "'");
      // header
      out << "/com, \n"
          << "/com, File generated by MFront from the " << fd.fileName
          << " source\n"
          << "/com, Example of how to use the " << mb.getClassName()
          << " behaviour law\n"
          << "/com, Author " << fd.authorName << '\n'
          << "/com, Date   " << fd.date << '\n'
          << "/com,\n\n";
      // loop over hypothesis
      for (const auto& h : this->getModellingHypothesesToBeTreated(mb)) {
        const auto& d = mb.getBehaviourData(h);
        const auto mps = this->buildMaterialPropertiesList(mb, h);
        // material properties
        auto msize = SupportedTypes::TypeSize{};
        if (!mps.first.empty()) {
          const auto& m = mps.first.back();
          msize = m.offset;
          msize += SupportedTypes::getTypeSize(m.type, m.arraySize);
        }
        // internal state variables
        const auto& persistentVarsHolder = d.getPersistentVariables();
        auto vs = SupportedTypes::TypeSize{};
        for (const auto& v : persistentVarsHolder) {
          vs += SupportedTypes::getTypeSize(v.type, v.arraySize);
        }
        const auto vsize = vs.getValueForModellingHypothesis(h) +
                           this->getStateVariablesOffset(mb, h);
        // external state variables
        const auto& externalStateVarsHolder = d.getExternalStateVariables();
        throw_if(
            externalStateVarsHolder.size() != 1u,
            "The ansys interface does not support external state  variables");
        // user material declaration
        out << "/com, Example for the '" << ModellingHypothesis::toString(h)
            << "' modelling hypothesis\n";
        out << "/com, List of material properties\n";
        for (const auto& mp : mps.first) {
          out << "/com, -" << mp.name << "\n";
        }
        out << "tb,user,<mat_id>,<number of temperatures>,"
            << msize.getValueForModellingHypothesis(h) << "\n"
            << "/com, you shall now declare your material properties\n"
            << "/com, using the tbtemp an tbdata commands.\n"
            << "/com, See the \"ANSYS USER Material Subroutine\" "
            << "guide for details.\n";
        // number of state variables
        out << "/com, Declaration of state variables\n"
            << "tb,state,<mat_id>,," << vsize << "\n"
            << "\n\n";
      }
    } catch (std::exception& e) {
      if (getVerboseMode() > VERBOSE_QUIET) {
        getLogStream() << e.what() << std::endl;
      }
    } catch (...) {
      if (getVerboseMode() > VERBOSE_QUIET) {
        getLogStream() << "AnsysInterface::generateInputFileExample: "
                       << "unknown exception thrown" << std::endl;
      }
    }
  }  // end of AnsysInterface::writeInputFileExample

  const char* const AnsysInterface::finiteStrainStrategy =
      "ansys::finiteStrainStrategy";

  std::string AnsysInterface::getName() { return "ansys"; }

  std::string AnsysInterface::getInterfaceName() const {
    return "Ansys";
  }  // end of AnsysInterface::getInterfaceName

  std::pair<bool, AnsysInterface::tokens_iterator> AnsysInterface::treatKeyword(
      BehaviourDescription& bd,
      const std::string& key,
      const std::vector<std::string>& i,
      tokens_iterator current,
      const tokens_iterator end) {
    using tfel::utilities::CxxTokenizer;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "AnsysInterface::treatKeyword: " + m);
    };
    if (!i.empty()) {
      if (std::find(i.begin(), i.end(), this->getName()) != i.end()) {
        auto keys = {"@AnsysFiniteStrainStrategy",
                     "@AnsysGenerateMTestFileOnFailure",
                     "@AnsysCompareToNumericalTangentOperator",
                     "@AnsysTangentOperatorComparisonCriterium",
                     "@AnsysTangentOperatorComparisonCriterion",
                     "@AnsysStrainPerturbationValue"};
        throw_if(std::find(keys.begin(), keys.end(), key) == keys.end(),
                 "unsupported key '" + key + "'");
      } else {
        return {false, current};
      }
    }
    if (key == "@AnsysFiniteStrainStrategy") {
      throw_if(bd.hasAttribute(AnsysInterface::finiteStrainStrategy),
               "a finite strain strategy has already been defined");
      throw_if(current == end, "unexpected end of file");
      const auto fs = current->value;
      throw_if(++current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';', read '" + current->value + '\'');
      ++(current);
      checkFiniteStrainStrategyDefinitionConsistency(bd, fs);
      if (fs == "Native") {
        throw_if(bd.getSymmetryType() == mfront::ORTHOTROPIC,
                 "orthotropic behaviours are not supported with the "
                 "`Native` finite strain strategy");
      }
      bd.setAttribute(AnsysInterface::finiteStrainStrategy, fs, false);
      return {true, current};
    }
    if (key == "@AnsysGenerateMTestFileOnFailure") {
      this->setGenerateMTestFileOnFailureAttribute(
          bd, this->readBooleanValue(key, current, end));
      return {true, current};
    } else if (key == "@AnsysCompareToNumericalTangentOperator") {
      this->compareToNumericalTangentOperator =
          this->readBooleanValue(key, current, end);
      return make_pair(true, current);
    } else if ((key == "@AnsysTangentOperatorComparisonCriterium") ||
               (key == "@AnsysTangentOperatorComparisonCriterion")) {
      throw_if(
          !this->compareToNumericalTangentOperator,
          "comparison to tangent operator is not enabled at this stage.\n"
          "Use the @AnsysCompareToNumericalTangentOperator directive before "
          "@AnsysTangentOperatorComparisonCriterion");
      throw_if(current == end, "unexpected end of file");
      this->tangentOperatorComparisonCriterion =
          CxxTokenizer::readDouble(current, end);
      throw_if(current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';', read '" + current->value + "'");
      ++(current);
      return {true, current};
    } else if (key == "@AnsysStrainPerturbationValue") {
      throw_if(!this->compareToNumericalTangentOperator,
               "time stepping is not enabled at this stage.\n"
               "Use the @AnsysUseTimeSubStepping directive before "
               "@AnsysStrainPerturbationValue");
      throw_if(current == end, "unexpected end of file");
      this->strainPerturbationValue = CxxTokenizer::readDouble(current, end);
      throw_if(current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';', read '" + current->value + "'");
      ++(current);
      return {true, current};
    }
    return {false, current};
  }  // end of treatKeyword

  void AnsysInterface::endTreatment(const BehaviourDescription& mb,
                                    const FileDescription& fd) const {
    using namespace tfel::system;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "AnsysInterface::endTreatment: " + m);
    };
    throw_if(!((mb.getBehaviourType() ==
                BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
               (mb.getBehaviourType() ==
                BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)),
             "the ansys interface only supports small and "
             "finite strain behaviours");
    checkFiniteStrainStrategyDefinitionConsistency(mb);
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if ((AnsysInterface::hasFiniteStrainStrategy(mb)) &&
          (AnsysInterface::getFiniteStrainStrategy(mb) == "Native")) {
        throw_if(mb.getSymmetryType() == mfront::ORTHOTROPIC,
                 "orthotropic behaviours are not supported with the "
                 "`Native` finite strain strategy");
      }
    }
    if (this->compareToNumericalTangentOperator) {
      throw_if(mb.getBehaviourType() !=
                   BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
               "unsupported feature @AnsysSaveTangentOperator "
               "and @AnsysCompareToNumericalTangentOperator : "
               "those are only valid for small strain beahviours");
    }
    // get the modelling hypotheses to be treated
    const auto& mhs = this->getModellingHypothesesToBeTreated(mb);
    for (const auto h : mhs) {
      const auto& d = mb.getBehaviourData(h);
      throw_if(d.getExternalStateVariables().size() != 1u,
               "external state variables are not supported "
               "by ANSYS's usermat interface");
    }
    const auto name = mb.getLibrary() + mb.getClassName();
    // output directories
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/Ansys");
    systemCall::mkdir("ansys");

    {
      // copy usermat.cpp locally
      MFrontLockGuard lock;
      copyAnsysFile("usermat.cpp");
      copyAnsysFile("test-usermat.cxx");
      copyAnsysFile("CMakeLists.txt");
    }

    // header
    auto fname = "ansys" + name + ".hxx";
    std::ofstream out("include/MFront/Ansys/" + fname);
    throw_if(!out, "could not open file '" + fname + "'");

    out << "/*!\n"
        << "* \\file   " << fname << '\n'
        << "* \\brief  This file declares the ansys interface for the "
        << mb.getClassName() << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n";

    const auto header = this->getHeaderGuard(mb);
    out << "#ifndef " << header << "\n"
        << "#define " << header << "\n\n"
        << "#include\"TFEL/Config/TFELConfig.hxx\"\n\n"
        << "#include\"MFront/Ansys/Ansys.hxx\"\n"
        << "#include\"MFront/Ansys/AnsysData.hxx\"\n\n";
    out << "#ifdef __cplusplus\n"
        << "#include\"MFront/Ansys/AnsysTraits.hxx\"\n";
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "#include\"MFront/Ansys/AnsysOrthotropicBehaviour.hxx\"\n";
    }
    if ((mb.getBehaviourType() ==
         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
        (AnsysInterface::hasFiniteStrainStrategy(mb))) {
      const auto fs = AnsysInterface::getFiniteStrainStrategy(mb);
      if (fs == "FiniteRotationSmallStrain") {
        out << "#include\"MFront/Ansys/AnsysFiniteStrain.hxx\"\n\n";
      }
      if (fs == "MieheApelLambrechtLogarithmicStrain") {
        out << "#include\"TFEL/Material/LogarithmicStrainHandler.hxx\"\n\n"
            << "#include\"MFront/Ansys/AnsysTangentOperator.hxx\"\n\n";
      }
    }
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n"
        << "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);

    out << "#ifdef __cplusplus\n\n"
        << "namespace ansys{\n\n";

    if (!mb.areAllMechanicalDataSpecialised(mhs)) {
      this->writeAnsysBehaviourTraits(out, mb,
                                      ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for (const auto& h : mhs) {
      if (mb.hasSpecialisedMechanicalData(h)) {
        this->writeAnsysBehaviourTraits(out, mb, h);
      }
    }

    out << "} // end of namespace ansys\n\n"
        << "#endif /* __cplusplus */\n\n"
        << "#ifdef __cplusplus\n"
        << "extern \"C\"{\n"
        << "#endif /* __cplusplus */\n\n";

    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out, name);
    this->writeSetParametersFunctionsDeclarations(out, mb, name);

    for (const auto h : mhs) {
      out << "MFRONT_SHAREDOBJ void\n"
          << this->getFunctionNameForHypothesis(name, h);
      writeArguments(out);
      out << ";\n\n";
    }

    out << "#ifdef __cplusplus\n"
        << "}\n"
        << "#endif /* __cplusplus */\n\n"
        << "#endif /* " << header << " */\n";

    out.close();

    fname = "ansys" + name + ".cxx";
    out.open("src/" + fname);
    throw_if(!out, "could not open file '" + fname + "'");

    out << "/*!\n"
        << "* \\file   " << fname << '\n'
        << "* \\brief  This file implements the ansys interface for the "
        << mb.getClassName() << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n";

    this->getExtraSrcIncludes(out, mb);

    if (this->compareToNumericalTangentOperator) {
      out << "#include<cmath>\n"
          << "#include<vector>\n"
          << "#include<algorithm>\n";
    }
    out << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n"
        << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "#include\"MFront/BehaviourProfiler.hxx\"\n\n";
    }
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "#include\"MFront/Ansys/AnsysRotation.hxx\"\n\n";
    }
    out << "#include\"MFront/Ansys/AnsysStressFreeExpansionHandler.hxx\"\n\n"
        << "#include\"MFront/Ansys/AnsysInterface.hxx\"\n\n"
        << "#include\"MFront/Ansys/ansys" << name << ".hxx\"\n\n";

    this->writeGetOutOfBoundsPolicyFunctionImplementation(out, name);

    out << "extern \"C\"{\n\n";

    AnsysSymbolsGenerator sg;
    sg.generateGeneralSymbols(out, *this, mb, fd, mhs, name);
    if (!mb.areAllMechanicalDataSpecialised(mhs)) {
      const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      sg.generateSymbols(out, *this, mb, fd, name, uh);
    }
    for (const auto& h : mhs) {
      if (mb.hasSpecialisedMechanicalData(h)) {
        sg.generateSymbols(out, *this, mb, fd, name, h);
      }
    }

    this->writeSetParametersFunctionsImplementations(out, mb, name);
    this->writeSetOutOfBoundsPolicyFunctionImplementation(out, name);

    for (const auto h : mhs) {
      if (mb.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        if (AnsysInterface::hasFiniteStrainStrategy(mb)) {
          const auto fs = AnsysInterface::getFiniteStrainStrategy(mb);
          if (fs == "Native") {
            this->writeSmallStrainFunction(out, mb, name, h);
          } else if (fs == "FiniteRotationSmallStrain") {
            this->writeFiniteRotationSmallStrainFunction(out, mb, name, h);
          } else if (fs == "MieheApelLambrechtLogarithmicStrain") {
            this->writeMieheApelLambrechtLogarithmicStrainFunction(out, mb,
                                                                   name, h);
          } else {
            throw_if(true, "unsupported finite strain strategy !");
          }
        } else {
          this->writeSmallStrainFunction(out, mb, name, h);
        }
      } else if (mb.getBehaviourType() ==
                 BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        this->writeFiniteStrainFunction(out, mb, name, h);
      } else {
        throw_if(true,
                 "the ansys interface only supports small "
                 "and finite strain behaviours");
      }
    }
    out << "} // end of extern \"C\"\n";
    out.close();
    this->writeInputFileExample(mb, fd);
  }  // end of AnsysInterface::endTreatment

  void AnsysInterface::writeFunctionBase(std::ostream& out,
                                         const BehaviourDescription& mb,
                                         const std::string& name,
                                         const std::string& sfeh,
                                         const Hypothesis h) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "AnsysInterface::writeFunctionBase: " + m);
    };
    std::string dv0, dv1, sig;
    const auto btype = mb.getBehaviourType();
    out << "static void\n"
        << name << "_base" << this->getFunctionNameForHypothesis("", h);
    writeArguments(out, mb, true);
    out << "{\n";
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      const auto mpoffset = [h, &mb] {
        auto o = size_t{};
        if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                            false)) {
          if ((h == ModellingHypothesis::PLANESTRESS) ||
              (h == ModellingHypothesis::AXISYMMETRICAL) ||
              (h == ModellingHypothesis::PLANESTRAIN)) {
            o += ansys::AnsysOrthotropicElasticPropertiesOffset<2u>::value;
          } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
            o += ansys::AnsysOrthotropicElasticPropertiesOffset<3u>::value;
          }
        }
        if (mb.getAttribute(
                BehaviourDescription::requiresThermalExpansionCoefficientTensor,
                false)) {
          if ((h == ModellingHypothesis::PLANESTRESS) ||
              (h == ModellingHypothesis::AXISYMMETRICAL) ||
              (h == ModellingHypothesis::PLANESTRAIN)) {
            o += ansys::AnsysOrthotropicElasticPropertiesOffset<2u>::value;
          } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
            o += 3u;
          }
        }
        return o;
      }();
      const auto smpoffset = std::to_string(mpoffset);
      if (btype == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        // turning the deformation and the deformation gradient
        // increment to the material frame
        if ((h == ModellingHypothesis::PLANESTRESS) ||
            (h == ModellingHypothesis::AXISYMMETRICAL) ||
            (h == ModellingHypothesis::PLANESTRAIN)) {
          out << "if(*NPROPS<static_cast<ansys::AnsysInt>(" + smpoffset +
                     "+2u)){\n"
              << "std::cerr << \"" << name
              << this->getFunctionNameForHypothesis("", h) << ": \"\n"
              << "          << \"invalid number of material properties\\n\";\n"
              << "*keycut = 1;\n"
              << "return;\n"
              << "}\n";
          if (h == ModellingHypothesis::PLANESTRESS) {
            out << "ansys::AnsysReal STRAN_tmp[4u];\n"
                << "ansys::AnsysReal DSTRAN_tmp[4u];\n"
                << "ansys::AnsysReal STRESS_tmp[4u];\n"
                << "STRAN_tmp[0] = STRAN[0];\n"
                << "STRAN_tmp[1] = STRAN[1];\n"
                << "STRAN_tmp[2] = ansys::AnsysReal{};\n"
                << "STRAN_tmp[3] = STRAN[2];\n"
                << "DSTRAN_tmp[0] = DSTRAN[0];\n"
                << "DSTRAN_tmp[1] = DSTRAN[1];\n"
                << "DSTRAN_tmp[2] = ansys::AnsysReal{};\n"
                << "DSTRAN_tmp[3] = DSTRAN[2];\n"
                << "STRESS_tmp[0] = STRESS[0];\n"
                << "STRESS_tmp[1] = STRESS[1];\n"
                << "STRESS_tmp[2] = ansys::AnsysReal{};\n"
                << "STRESS_tmp[3] = STRESS[2];\n"
                << "ansys::AnsysRotation2D<ansys::AnsysReal> R(PROPS+" +
                       smpoffset + ");\n"
                << "ansys::AnsysReal e[4u];\n"
                << "ansys::AnsysReal de[4u];\n"
                << "ansys::AnsysReal sm[4u];\n"
                << "R.rotateStrainsForward(STRAN_tmp,e);\n"
                << "R.rotateStrainsForward(DSTRAN_tmp,de);\n"
                << "R.rotateStressesForward(STRESS_tmp,sm);\n"
                << "e[2] = e[3];\n"
                << "e[3] = ansys::AnsysReal{};\n"
                << "de[2] = de[3];\n"
                << "de[3] = ansys::AnsysReal{};\n"
                << "sm[2] = sm[3];\n"
                << "sm[3] = ansys::AnsysReal{};\n";
          } else {
            out << "ansys::AnsysRotation2D<ansys::AnsysReal> R(PROPS+" +
                       smpoffset + ");\n"
                << "ansys::AnsysReal e[4u];\n"
                << "ansys::AnsysReal de[4u];\n"
                << "ansys::AnsysReal sm[4u];\n"
                << "R.rotateStrainsForward(STRAN,e);\n"
                << "R.rotateStrainsForward(DSTRAN,de);\n"
                << "R.rotateStressesForward(STRESS,sm);\n";
          }
        } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          out << "if(*NPROPS<static_cast<ansys::AnsysInt>(" + smpoffset +
                     "+6u)){\n"
              << "std::cerr << \"" << name
              << this->getFunctionNameForHypothesis("", h) << ": \"\n"
              << "          << \"invalid number of state variables\\n\";\n"
              << "*keycut = 1;\n"
              << "return;\n"
              << "}\n"
              << "ansys::AnsysRotation3D<ansys::AnsysReal> R(PROPS+" +
                     smpoffset + ");\n"
              << "ansys::AnsysReal e[6u];\n"
              << "ansys::AnsysReal de[6u];\n"
              << "ansys::AnsysReal sm[6u];\n"
              << "R.rotateStrainsForward(STRAN, e);\n"
              << "R.rotateStrainsForward(DSTRAN, de);\n"
              << "R.rotateStressesForward(STRESS, sm);\n";
        } else {
          throw_if(true, "unsupported hypothesis");
        }
        dv0 = "e";
        dv1 = "de";
        sig = "sm";
      } else if (btype == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        // turning the deformation gradients to the material frame
        if ((h == ModellingHypothesis::PLANESTRESS) ||
            (h == ModellingHypothesis::AXISYMMETRICAL) ||
            (h == ModellingHypothesis::PLANESTRAIN)) {
          out << "if(*NPROPS<static_cast<ansys::AnsysInt>(" + smpoffset +
                     "+2u)){\n"
              << "std::cerr << \"" << name
              << this->getFunctionNameForHypothesis("", h) << ": \"\n"
              << "          << \"invalid number of material properties\\n\";\n"
              << "*keycut = 1;\n"
              << "return;\n"
              << "}\n"
              << "ansys::AnsysRotation2D<ansys::AnsysReal> R(PROPS+" +
                     smpoffset + ");\n"
              << "ansys::AnsysReal Fm0[9u];\n"
              << "ansys::AnsysReal Fm1[9u];\n"
              << "ansys::AnsysReal sm[4u];\n"
              << "R.rotateDeformationGradientForward(F0,Fm0);\n"
              << "R.rotateDeformationGradientForward(F1,Fm1);\n"
              << "R.rotateStressesForward(STRESS,sm);\n";
        } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          out << "if(*NPROPS<static_cast<ansys::AnsysInt>(" + smpoffset +
                     "+6u)){\n"
              << "std::cerr << \"" << name
              << this->getFunctionNameForHypothesis("", h) << ": \"\n"
              << "          << \"invalid number of state variables\\n\";\n"
              << "*keycut = 1;\n"
              << "return;\n"
              << "}\n"
              << "ansys::AnsysRotation3D<ansys::AnsysReal> R(PROPS+" +
                     smpoffset + ");\n"
              << "ansys::AnsysReal Fm0[9u];\n"
              << "ansys::AnsysReal Fm1[9u];\n"
              << "ansys::AnsysReal sm[6u];\n"
              << "R.rotateDeformationGradientForward(F0,Fm0);\n"
              << "R.rotateDeformationGradientForward(F1,Fm1);\n"
              << "R.rotateStressesForward(STRESS,sm);\n";
        } else {
          throw_if(true, "unsupported hypothesis");
        }
        dv0 = "Fm0";
        dv1 = "Fm1";
        sig = "sm";
      } else {
        throw_if(true,
                 "the ansys interface only supports small "
                 "and finite strain behaviours");
      }
    } else {
      if (btype == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        dv0 = "STRAN";
        dv1 = "DSTRAN";
      } else if (btype == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        dv0 = "F0";
        dv1 = "F1";
      } else {
        throw_if(true,
                 "the ansys interface only supports small "
                 "and finite strain behaviours");
      }
      sig = "STRESS";
    }
    out << "ansys::AnsysData d = {" << sig << ",DDSDDE,STATEV,\n"
        << "                      SEDEL,SEDPL,EPSEQ,EPSPL,EPSZZ,\n"
        << "                      *NTENS,*NPROPS,*NSTATV,*DTIME,DROT,\n"
        << "                      " << dv0 << "," << dv1 << ",TEMP,DTEMP,PROPS,"
        << this->getFunctionNameBasis(name) << "_getOutOfBoundsPolicy(),"
        << sfeh << "};\n"
        << "if(ansys::AnsysInterface<tfel::material::ModellingHypothesis::"
        << ModellingHypothesis::toUpperCaseString(h)
        << ",tfel::material::" << mb.getClassName() << ">::exe(d)!=0){\n"
        << "*keycut = 1;\n"
        << "return;\n"
        << "}\n";
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      if (h == ModellingHypothesis::PLANESTRESS) {
        out << "ansys::AnsysReal D[16u] = {DDSDDE[0],DDSDDE[3],0,DDSDDE[6],\n"
            << "                           DDSDDE[1],DDSDDE[4],0,DDSDDE[7],\n"
            << "                           0,0,0,0,\n"
            << "                           DDSDDE[2],DDSDDE[5],0,DDSDDE[8]};\n"
            << "sm[3] = sm[2];\n"
            << "sm[2] = ansys::AnsysReal{};\n"
            << "R.rotateStressesBackward(sm, STRESS_tmp);\n"
            << "STRESS[0] = STRESS_tmp[0];\n"
            << "STRESS[1] = STRESS_tmp[1];\n"
            << "STRESS[2] = STRESS_tmp[3];\n"
            << "R.rotateTangentOperatorBackward(D);\n"
            << "DDSDDE[0]=D[0];\n"
            << "DDSDDE[1]=D[4];\n"
            << "DDSDDE[2]=D[12];\n"
            << "DDSDDE[3]=D[1];\n"
            << "DDSDDE[4]=D[5];\n"
            << "DDSDDE[5]=D[13];\n"
            << "DDSDDE[6]=D[3];\n"
            << "DDSDDE[7]=D[7];\n"
            << "DDSDDE[8]=D[15];\n";
      } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
                 (h == ModellingHypothesis::PLANESTRAIN) ||
                 (h == ModellingHypothesis::TRIDIMENSIONAL)) {
        out << "R.rotateStressesBackward(sm,STRESS);\n"
            << "R.rotateTangentOperatorBackward(DDSDDE);\n";
      } else {
        throw_if(true, "unsupported hypothesis");
      }
    }
    if (getDebugMode()) {
      out << "std::cout << \"Dt :\" << std::endl;\n"
          << "for(ansys::AnsysInt i=0;i!=*NTENS;++i){\n"
          << "for(ansys::AnsysInt j=0;j!=*NTENS;++j){\n"
          << "std::cout << *(DDSDDE+j*(*NTENS)+i) << \" \";\n"
          << "}\n"
          << "std::cout << std::endl;\n"
          << "}\n"
          << "std::cout << std::endl;\n";
    }
    out << "}\n\n";
  }  // end of AnsysInterface::writeFunctionBase

  void AnsysInterface::writeFiniteStrainFunction(std::ostream& out,
                                                 const BehaviourDescription& mb,
                                                 const std::string& name,
                                                 const Hypothesis h) const {
    const std::string sfeh = "nullptr";
    this->writeFunctionBase(out, mb, name, sfeh, h);
    out << "MFRONT_SHAREDOBJ void\n"
        << this->getFunctionNameForHypothesis(name, h);
    writeArguments(out, mb, false);
    out << "{\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n"
          << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
          << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    out << name << "_base" << this->getFunctionNameForHypothesis("", h)
        << "(matId,elemId,kDomIntPt,kLayer,kSectPt,ldsetp,isubst,keycut,\n"
        << " nDirect,nShear,NTENS,NSTATV,NPROPS,TIME,DTIME,TEMP,DTEMP,\n"
        << " STRESS,STATEV,DDSDDE,SEDEL,SEDPL,EPSEQ,STRAN,DSTRAN,\n"
        << " EPSPL,PROPS,coords,DROT,F0,F1,tsstif,EPSZZ,\n"
        << " var1,var2,var3,var4,var5,var6,var7,var8);\n"
        << "}\n\n";
  }

  void AnsysInterface::writeSmallStrainFunction(std::ostream& out,
                                                const BehaviourDescription& mb,
                                                const std::string& name,
                                                const Hypothesis h) const {
    const std::string sfeh =
        "ansys::AnsysStandardSmallStrainStressFreeExpansionHandler";
    this->writeFunctionBase(out, mb, name, sfeh, h);
    out << "MFRONT_SHAREDOBJ void\n"
        << this->getFunctionNameForHypothesis(name, h);
    writeArguments(out, mb, false);
    out << "{\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n"
          << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
          << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    if (this->shallGenerateMTestFileOnFailure(mb)) {
      out << "constexpr auto h = "
          << "tfel::material::ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ";\n";
    }
    if (this->compareToNumericalTangentOperator) {
      out << "std::vector<ansys::AnsysReal> deto0(*NTENS);\n"
          << "std::vector<ansys::AnsysReal> sig0(*NTENS);\n"
          << "std::vector<ansys::AnsysReal> sv0(*NSTATV);\n"
          << "std::copy(DSTRAN,DSTRAN+*(NTENS),deto0.begin());\n"
          << "std::copy(STRESS,STRESS+*(NTENS),sig0.begin());\n"
          << "std::copy(STATEV,STATEV+*(NSTATV),sv0.begin());\n";
    }
    if (this->shallGenerateMTestFileOnFailure(mb)) {
      this->generateMTestFile1(out, mb);
    }
    out << name << "_base" << this->getFunctionNameForHypothesis("", h)
        << "(matId,elemId,kDomIntPt,kLayer,kSectPt,ldsetp,isubst,keycut,\n"
        << " nDirect,nShear,NTENS,NSTATV,NPROPS,TIME,DTIME,TEMP,DTEMP,\n"
        << " STRESS,STATEV,DDSDDE,SEDEL,SEDPL,EPSEQ,STRAN,DSTRAN,\n"
        << " EPSPL,PROPS,coords,DROT,F0,F1,tsstif,EPSZZ,\n"
        << " var1,var2,var3,var4,var5,var6,var7,var8);\n";
    if (this->shallGenerateMTestFileOnFailure(mb)) {
      out << "if(*keycut!=0){\n";
      this->generateMTestFileForHypothesis(out, mb, mb.getBehaviourType(), name,
                                           "", h);
      out << "}\n";
    }
    if (this->compareToNumericalTangentOperator) {
      out << "// computing the tangent operator by pertubation\n"
          << "std::vector<ansys::AnsysReal> nD((*NTENS)*(*NTENS));\n"
          << "std::vector<ansys::AnsysReal> deto(*NTENS);\n"
          << "std::vector<ansys::AnsysReal> sigf(*NTENS);\n"
          << "std::vector<ansys::AnsysReal> sigb(*NTENS);\n"
          << "std::vector<ansys::AnsysReal> sv(*NSTATV);\n"
          << "std::vector<ansys::AnsysReal> D((*NTENS)*(*NTENS));\n"
          << "ansys::AnsysReal keycut0 = 0;\n"
          << "ansys::AnsysReal m;\n"
          << "ansys::AnsysReal mDt;\n"
          << "ansys::AnsysReal mnDt;\n"
          << "for(ansys::AnsysInt i=0;i!=*NTENS;++i){\n"
          << "std::copy(deto0.begin(),deto0.end(),deto.begin());\n"
          << "std::copy(sig0.begin(),sig0.end(),sigf.begin());\n"
          << "std::copy(sv0.begin(),sv0.end(),sv.begin());\n"
          << "deto[i] += " << this->strainPerturbationValue << ";\n"
          << "ansys::AnsysData d2 = {&sigf[0],&D[0],&sv[0],\n"
          << "                         SEDEL,SEDPL,EPSEQ,EPSPL,EPSZZ,\n"
          << "                         *NTENS,*NPROPS,*NSTATV,*DTIME,DROT,\n"
          << "                         STRAN,&deto[0],TEMP,DTEMP,PROPS,"
          << this->getFunctionNameBasis(name) << "_getOutOfBoundsPolicy(),"
          << sfeh << "};\n"
          << "if(ansys::AnsysInterface<tfel::material::ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ","
          << "tfel::material::" << mb.getClassName() << ">::exe(d2)!=0){\n"
          << "return;\n"
          << "}\n"
          << "ansys::AnsysReal keycut0 = 0;\n"
          << "std::copy(deto0.begin(),deto0.end(),deto.begin());\n"
          << "std::copy(sig0.begin(),sig0.end(),sigb.begin());\n"
          << "std::copy(sv0.begin(),sv0.end(),sv.begin());\n"
          << "deto[i] -= " << this->strainPerturbationValue << ";\n"
          << "ansys::AnsysData d3 = {&sigf[0],&D[0],&sv[0],\n"
          << "                         SEDEL,SEDPL,EPSEQ,EPSPL,EPSZZ,\n"
          << "                         *NTENS,*NPROPS,*NSTATV,*DTIME,DROT,\n"
          << "                         STRAN,&deto[0],TEMP,DTEMP,PROPS,"
          << "if(ansys::AnsysInterface<tfel::material::ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ","
          << "tfel::material::" << mb.getClassName() << ">::exe(d3)!=0){\n"
          << "return;\n"
          << "}\n"
          << "for(ansys::AnsysInt j=0;j!=*NTENS;++j){\n"
          << "nD[j*(*NTENS)+i] = (sigf[j]-sigb[j])/(2.*"
          << this->strainPerturbationValue << ");\n"
          << "}\n"
          << "}\n"
          << "// comparison\n"
          << "m=0.;\n"
          << "mDt=0.;\n"
          << "mnDt=0.;\n"
          << "for(i=0;i!=(*NTENS)*(*NTENS);++i){\n"
          << "mDt=std::max(mDt,*(DDSDDE+i));\n"
          << "mnDt=std::max(mnDt,nD[i]);\n"
          << "m=std::max(m,std::abs(nD[i]-*(DDSDDE+i)));\n"
          << "}\n"
          << "if(m>" << this->tangentOperatorComparisonCriterion << "){\n"
          << "std::cout << \"||nDt-Dt|| = \" << m << \" (\" << "
             "100.*m/(0.5*(mDt+mnDt)) << \"%)\"<< std::endl;\n"
          << "std::cout << \"Dt :\" << std::endl;\n"
          << "for(ansys::AnsysInt i=0;i!=*NTENS;++i){\n"
          << "for(ansys::AnsysInt j=0;j!=*NTENS;++j){\n"
          << "std::cout << *(DDSDDE+j*(*NTENS)+i) << \" \";\n"
          << "}\n"
          << "std::cout << std::endl;\n"
          << "}\n"
          << "std::cout << \"nDt :\" << std::endl;\n"
          << "for(ansys::AnsysInt i=0;i!=*NTENS;++i){\n"
          << "for(ansys::AnsysInt j=0;j!=*NTENS;++j){\n"
          << "std::cout << nD[j*(*NTENS)+i] << \" \";\n"
          << "}\n"
          << "std::cout << std::endl;\n"
          << "}\n"
          << "std::cout << std::endl;\n"
          << "}\n";
    }
    // if(AnsysInterface::hasFiniteStrainStrategy(mb)){
    //   if(AnsysInterface::getFiniteStrainStrategy(mb)=="Native"){
    // 	out <<
    // "ansys::AnsysFiniteStrain::applyNativeFiniteStrainCorrection(DDSDDE,DFGRD1,STRESS,*NTENS);\n";
    //   }
    // }
    out << "}\n\n";
  }

  void AnsysInterface::writeFiniteRotationSmallStrainFunction(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    if (h == ModellingHypothesis::PLANESTRESS) {
      tfel::raise(
          "AnsysInterface::writeFiniteRotationSmallStrainFunction: "
          "plane stress is not supported yet");
    }
    const auto ps = h == ModellingHypothesis::PLANESTRESS ? "true" : "false";
    const std::string sfeh =
        "ansys::AnsysStandardSmallStrainStressFreeExpansionHandler";
    this->writeFunctionBase(out, mb, name, sfeh, h);
    out << "MFRONT_SHAREDOBJ void\n"
        << this->getFunctionNameForHypothesis(name, h);
    writeArguments(out, mb, false);
    out << "{\n"
        << "using namespace ansys;\n"
        << "AnsysReal eto[6];\n"
        << "AnsysReal deto[6];\n"
        << "AnsysReal CSE[36];\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n"
          << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
          << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
      out << "{\n"
          << "BehaviourProfiler::Timer pre_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
    }

    out << "AnsysFiniteStrain::computeGreenLagrangeStrain(eto,F0,*NTENS," << ps
        << ");\n"
        << "AnsysFiniteStrain::computeGreenLagrangeStrain(deto,F1,*NTENS," << ps
        << ");\n"
        << "AnsysFiniteStrain::"
           "computeSecondPiolaKirchhoffStressFromCauchyStress(STRESS,F0,*NTENS,"
        << ps << ",0);\n"
        << "for(int i=0;i!=*NTENS;++i){\n"
        << "deto[i] -= eto[i];\n"
        << "}\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "}\n";
    }
    out << name << "_base" << this->getFunctionNameForHypothesis("", h)
        << "(matId,elemId,kDomIntPt,kLayer,kSectPt,ldsetp,isubst,keycut,\n"
        << " nDirect,nShear,NTENS,NSTATV,NPROPS,TIME,DTIME,TEMP,DTEMP,\n"
        << " STRESS,STATEV,DDSDDE,SEDEL,SEDPL,EPSEQ,eto,deto,\n"
        << " EPSPL,PROPS,coords,DROT,F0,F1,tsstif,EPSZZ,\n"
        << " var1,var2,var3,var4,var5,var6,var7,var8);\n"
        << "if(*keycut==0){\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "BehaviourProfiler::Timer post_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
    }
    out << "AnsysFiniteStrain::"
           "computeCauchyStressFromSecondPiolaKirchhoffStress(STRESS,F1,*NTENS,"
        << ps << ",0);\n"
        << "AnsysFiniteStrain::computeAnsysTangentOperatorFromCSE(DDSDDE,CSE,"
           "F1,STRESS,*NTENS,"
        << ps << ");\n"
        << "}\n"
        << "}\n\n";
  }

  void AnsysInterface::writeMieheApelLambrechtLogarithmicStrainFunction(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b,
          "AnsysInterface::writeMieheApelLambrechtLogarithmicStrainFunction: " +
              m);
    };
    throw_if(h == ModellingHypothesis::PLANESTRESS,
             "plane stress is not supported yet");
    const std::string sfeh =
        "ansys::AnsysLogarithmicStrainStressFreeExpansionHandler";
    this->writeFunctionBase(out, mb, name, sfeh, h);
    const auto d = [&h, &throw_if] {
      if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return 3u;
      }
      throw_if(!((h == ModellingHypothesis::AXISYMMETRICAL) ||
                 (h == ModellingHypothesis::PLANESTRAIN) ||
                 (h == ModellingHypothesis::PLANESTRESS)),
               "unsupported modelling hypothesis");
      return 2u;
    }();
    const auto n = [&h, &throw_if] {
      if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return 6u;
      }
      throw_if(!((h == ModellingHypothesis::AXISYMMETRICAL) ||
                 (h == ModellingHypothesis::PLANESTRAIN) ||
                 (h == ModellingHypothesis::PLANESTRESS)),
               "unsupported modelling hypothesis");
      return 4u;
    }();
    out << "MFRONT_SHAREDOBJ void\n"
        << this->getFunctionNameForHypothesis(name, h);
    writeArguments(out, mb, false);
    out << "{\n";
    out << "using namespace ansys;\n"
        << "using namespace tfel::math;\n"
        << "using namespace tfel::material;\n"
        << "AnsysReal eto[" << n << "];\n"
        << "AnsysReal deto[" << n << "];\n"
        << "LogarithmicStrainHandler<" << d << ",AnsysReal> "
        << "lsh0(LogarithmicStrainHandlerBase::EULERIAN,\n"
        << "     tensor<" << d << ",AnsysReal>::buildFromFortranMatrix(F0));\n"
        << "LogarithmicStrainHandler<" << d << ",AnsysReal> "
        << "lsh1(LogarithmicStrainHandlerBase::EULERIAN,\n"
        << "     tensor<" << d << ",AnsysReal>::buildFromFortranMatrix(F1));\n"
        << "lsh0.getHenckyLogarithmicStrain(eto);\n"
        << "lsh1.getHenckyLogarithmicStrain(deto);\n";
    for (unsigned short i = 0; i != n; ++i) {
      out << "deto[" << i << "]-=eto[" << i << "];\n";
    }
    if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      out << "// conversion to Ansys' convention\n"
          << "std::swap(eto[4],eto[5]);\n"
          << "std::swap(eto[4],eto[5]);\n"
          << "// conversion to Abaqus conventions\n"
          << "std::swap(STRESS[4],STRESS[5]);\n";
    }
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n"
          << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
          << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n"
          << "{\n"
          << "BehaviourProfiler::Timer pre_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
    }
    out << "lsh0.convertFromCauchyStress(STRESS);\n";
    if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      out << "std::swap(STRESS[4],STRESS[5]);\n";
    }
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "}\n";
    }
    out << name << "_base" << this->getFunctionNameForHypothesis("", h)
        << "(matId,elemId,kDomIntPt,kLayer,kSectPt,ldsetp,isubst,keycut,\n"
        << " nDirect,nShear,NTENS,NSTATV,NPROPS,TIME,DTIME,TEMP,DTEMP,\n"
        << " STRESS,STATEV,DDSDDE,SEDEL,SEDPL,EPSEQ,eto,deto,\n"
        << " EPSPL,PROPS,coords,DROT,F0,F1,tsstif,EPSZZ,\n"
        << " var1,var2,var3,var4,var5,var6,var7,var8);\n"
        << "if(*keycut==0){\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "{\n"
          << "BehaviourProfiler::Timer post_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
    }
    if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      out << "const auto C = "
             "AnsysTangentOperator<AnsysReal>::convert3D(DDSDDE);\n"
          << "stensor<3u,AnsysReal> T;\n"
          << "T.importTab(STRESS);\n"
          << "std::swap(T[4],T[5]);\n";
    } else {
      out << "const auto C = "
             "AnsysTangentOperator<AnsysReal>::convert2D(DDSDDE);\n"
          << "stensor<2u,AnsysReal> T;\n"
          << "T.importTab(STRESS);\n";
    }
    out << "const auto D = lsh1.convertToAbaqusTangentModuli(C,T);\n"
        << "// converting the stress\n"
        << "lsh1.convertToCauchyStress(STRESS);\n";
    if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      out << "std::swap(STRESS[4],STRESS[5]);\n"
          << "tfel::math::ST2toST2View<3u,AnsysReal> Dt(DDSDDE);\n"
          << "Dt=D;\n"
          << "AnsysTangentOperator<AnsysReal>::normalize(Dt);\n";
    } else {
      out << "tfel::math::ST2toST2View<2u,AnsysReal> Dt(DDSDDE);\n"
          << "Dt=D;\n"
          << "AnsysTangentOperator<AnsysReal>::normalize(Dt);\n";
    }
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "}\n";
    }
    out << "}\n"
        << "}\n\n";
  }

  void AnsysInterface::writeInterfaceSpecificIncludes(
      std::ostream& out, const BehaviourDescription&) const {
    out << "#include\"MFront/Ansys/Ansys.hxx\"\n"
        << "#include\"MFront/Ansys/AnsysConvert.hxx\"\n\n";
  }  // end of AnsysInterface::writeInterfaceSpecificIncludes

  std::vector<std::pair<std::string, std::string>>
  AnsysInterface::getBehaviourDataConstructorAdditionalVariables() const {
    return {{"DR", "increment of rigid body rotation"}};
  }  // end of AnsysInterface::getBehaviourDataConstructorAdditionalVariables

  void AnsysInterface::writeBehaviourDataMainVariablesSetters(
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
       << iprefix << "stress_,\n"
       << "const NumericType* const )\n"
       << "{\n";
    for (const auto& v : mb.getMainVariables()) {
      this->writeBehaviourDataThermodynamicForceSetter(os, v.second, of);
      of += SupportedTypes::getTypeSize(v.second.type, 1u);
    }
    os << "}\n\n";
  }  // end of AnsysInterface::writeBehaviourDataMainVariablesSetters

  void AnsysInterface::writeBehaviourDataGradientSetter(
      std::ostream& os,
      const Gradient& v,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (!o.isNull()) {
      tfel::raise(
          "AnsysInterface::writeBehaviourDataMainVariablesSetter : "
          "only one driving variable supported");
    }
    if (Gradient::isIncrementKnown(v)) {
      os << "ansys::ImportGradients<hypothesis>::exe(this->" << v.name << ","
         << iprefix << "stran);\n";
    } else {
      os << "ansys::ImportGradients<hypothesis>::exe(this->" << v.name << "0,"
         << iprefix << "stran);\n";
    }
  }  // end of AnsysInterface::writeBehaviourDataGradientSetter

  void AnsysInterface::writeIntegrationDataGradientSetter(
      std::ostream& os,
      const Gradient& v,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (!o.isNull()) {
      tfel::raise(
          "AnsysInterface::writeIntegrationDataMainVariablesSetter : "
          "only one driving variable supported");
    }
    if (Gradient::isIncrementKnown(v)) {
      os << "ansys::ImportGradients<hypothesis>::exe(this->d" << v.name << ","
         << iprefix << "dstran);\n";
    } else {
      os << "ansys::ImportGradients<hypothesis>::exe(this->" << v.name << "1,"
         << iprefix << "dstran);\n";
    }
  }  // end of AnsysInterface::writeIntegrationDataGradientSetter

  void AnsysInterface::writeBehaviourDataThermodynamicForceSetter(
      std::ostream& os,
      const ThermodynamicForce& f,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (SupportedTypes::getTypeFlag(f.type) == SupportedTypes::STENSOR) {
      os << "ansys::ImportThermodynamicForces<hypothesis>::exe(this->" << f.name
         << ",";
      if (!o.isNull()) {
        os << iprefix << "stress_+" << o << ");\n";
      } else {
        os << iprefix << "stress_);\n";
      }
    } else {
      tfel::raise(
          "AnsysInterface::writeBehaviourDataMainVariablesSetters : "
          "unsupported forces type");
    }
  }  // end of AnsysInterface::writeBehaviourDataThermodynamicForceSetter

  void AnsysInterface::completeBehaviourDataConstructor(
      std::ostream& out,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    auto do_nothing = [&out] { out << "static_cast<void>(ANSYSDR);\n"; };
    /*!
     * We apply the rotation associated to the Jauman corotationnal frame only
     * if:
     * - the behaviour symmetry is isotropic
     * - the behaviour is written in small strain
     * - the finite strain strategy is either undefined or `Native`
     * note: this is included to comply with gcc 4.7.2
     */
    const auto c = [&mb] {
      if (mb.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        if ((AnsysInterface::hasFiniteStrainStrategy(mb)) &&
            (AnsysInterface::getFiniteStrainStrategy(mb) != "Native")) {
          return false;
        }
        return true;
      }
      return false;
    }();
    if (!c) {
      do_nothing();
      return;
    }
    // checking if there are variables that need to be rotated
    const auto& d = mb.getBehaviourData(h);
    bool b = false;  // have persistent variables that have to be updated
    for (const auto& v : d.getPersistentVariables()) {
      const auto flag = SupportedTypes::getTypeFlag(v.type);
      if ((flag == SupportedTypes::STENSOR) ||
          (flag == SupportedTypes::TENSOR)) {
        b = true;
        break;
      }
    }
    if (!b) {
      do_nothing();
      return;
    }
    // rotate variables
    out << "const tfel::math::tmatrix<3u,3u,real> ansys_dr = "
           "{ANSYSDR[0],ANSYSDR[1],ANSYSDR[2],\n"
           "                                                       "
           "ANSYSDR[3],ANSYSDR[4],ANSYSDR[5],\n"
           "                                                       "
           "ANSYSDR[6],ANSYSDR[7],ANSYSDR[8]};\n";
    for (const auto& v : d.getPersistentVariables()) {
      const auto flag = SupportedTypes::getTypeFlag(v.type);
      if ((flag == SupportedTypes::STENSOR) ||
          (flag == SupportedTypes::TENSOR)) {
        if (v.arraySize == 1u) {
          out << "this->" << v.name << ".changeBasis(ansys_dr);\n";
        } else {
          for (unsigned short i = 0; i != v.arraySize; ++i) {
            out << "this->" << v.name << "[" << i
                << "].changeBasis(ansys_dr);\n";
          }
        }
      }
    }
  }  // end of AnsysInterface::completeBehaviourDataConstructor

  void AnsysInterface::exportThermodynamicForce(
      std::ostream& out,
      const std::string& a,
      const ThermodynamicForce& f,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    const auto flag = SupportedTypes::getTypeFlag(f.type);
    if (flag == SupportedTypes::STENSOR) {
      if (!o.isNull()) {
        out << "ansys::ExportThermodynamicForces<hypothesis>::exe(" << a << "+"
            << o << ",this->sig);\n";
      } else {
        out << "ansys::ExportThermodynamicForces<hypothesis>::exe(" << a
            << ",this->sig);\n";
      }
    } else {
      tfel::raise(
          "AnsysInterface::exportThermodynamicForce: "
          "unsupported forces type");
    }
  }  // end of AnsysInterface::exportThermodynamicForce

  bool AnsysInterface::areExternalStateVariablesSupported() const {
    return false;
  }  // end of AnsysInterface::areExternalStateVariablesSupported()

  void AnsysInterface::getTargetsDescription(TargetsDescription& d,
                                             const BehaviourDescription& bd) {
    const auto lib = this->getLibraryName(bd);
    const auto name = bd.getLibrary() + bd.getClassName();
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    auto& l = d.getLibrary(lib);
    insert_if(l.cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
    insert_if(l.include_directories,
              "$(shell " + tfel_config + " --include-path)");
    insert_if(l.sources, "ansys" + name + ".cxx");
    d.headers.push_back("MFront/Ansys/ansys" + name + ".hxx");
    insert_if(l.link_directories,
              "$(shell " + tfel_config + " --library-path)");
    insert_if(l.link_libraries, tfel::getLibraryInstallName("AnsysInterface"));
    if (this->shallGenerateMTestFileOnFailure(bd)) {
      insert_if(l.link_libraries,
                tfel::getLibraryInstallName("MTestFileGenerator"));
    }
    insert_if(l.link_libraries, "$(shell " + tfel_config +
                                    " --library-dependency "
                                    "--material --mfront-profiling)");
    for (const auto h : this->getModellingHypothesesToBeTreated(bd)) {
      insert_if(l.epts, this->getFunctionNameForHypothesis(name, h));
    }
  }  // end of AnsysInterface::getTargetsDescription

  AnsysInterface::~AnsysInterface() = default;

}  // end of namespace mfront
