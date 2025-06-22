/*!
 * \file   AbaqusInterfaceBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   17 mars 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <fstream>
#include "TFEL/Raise.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/AbaqusInterfaceBase.hxx"

#ifndef _MSC_VER
static const char* const constexpr_c = "constexpr";
#else
static const char* const constexpr_c = "const";
#endif

namespace mfront {

  const char* const AbaqusInterfaceBase::finiteStrainStrategy =
      "abaqus::finiteStrainStrategy";

  const char* const AbaqusInterfaceBase::orthotropyManagementPolicy =
      "abaqus::orthotropyManagementPolicy";

  static void checkFiniteStrainStrategy(const std::string& fs) {
    tfel::raise_if((fs != "Native") && (fs != "FiniteRotationSmallStrain") &&
                       (fs != "MieheApelLambrechtLogarithmicStrain"),
                   "checkFiniteStrainStrategy: "
                   "unsupported strategy '" +
                       fs +
                       "'\n"
                       "The only supported strategies are "
                       "'Native', 'FiniteRotationSmallStrain', "
                       "'MieheApelLambrechtLogarithmicStrain'");
  }  // end of checkFiniteStrainStrategy

  void AbaqusInterfaceBase::checkFiniteStrainStrategyDefinitionConsistency(
      const BehaviourDescription& bd, const std::string& fs) {
    auto throw_if = [](const bool c, const std::string& msg) {
      tfel::raise_if(c,
                     "AbaqusInterfaceBase::"
                     "checkFiniteStrainStrategyDefinitionConsistency: " +
                         msg);
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
  }  // end of
     // AbaqusInterfaceBase::checkFiniteStrainStrategyDefinitionConsistency

  void AbaqusInterfaceBase::checkFiniteStrainStrategyDefinitionConsistency(
      const BehaviourDescription& bd) {
    auto throw_if = [](const bool c, const std::string& msg) {
      tfel::raise_if(c,
                     "AbaqusInterfaceBase::"
                     "checkFiniteStrainStrategyDefinitionConsistency: " +
                         msg);
    };
    if (bd.getBehaviourType() !=
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      throw_if(bd.hasAttribute(AbaqusInterfaceBase::finiteStrainStrategy),
               "finite strain strategy is only supported for strain based "
               "behaviours");
    } else {
      if (bd.hasAttribute(AbaqusInterfaceBase::finiteStrainStrategy)) {
        const auto fs = bd.getAttribute<std::string>(
            AbaqusInterfaceBase::finiteStrainStrategy);
        AbaqusInterfaceBase::checkFiniteStrainStrategyDefinitionConsistency(bd,
                                                                            fs);
      }
    }
  }  // end of
     // AbaqusInterfaceBase::checkFiniteStrainStrategyDefinitionConsistency

  bool AbaqusInterfaceBase::hasFiniteStrainStrategy(
      const BehaviourDescription& bd) {
    checkFiniteStrainStrategyDefinitionConsistency(bd);
    if (bd.isStrainMeasureDefined()) {
      return bd.getStrainMeasure() != BehaviourDescription::LINEARISED;
    }
    return bd.hasAttribute(AbaqusInterfaceBase::finiteStrainStrategy);
  }  // end of AbaqusInterfaceBase::hasFiniteStrainStrategy

  std::string AbaqusInterfaceBase::getFiniteStrainStrategy(
      const BehaviourDescription& bd) {
    checkFiniteStrainStrategyDefinitionConsistency(bd);
    auto throw_if = [](const bool c, const std::string& msg) {
      tfel::raise_if(c, "AbaqusInterfaceBase::getFiniteStrainStrategy: " + msg);
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
    throw_if(!bd.hasAttribute(AbaqusInterfaceBase::finiteStrainStrategy),
             "no finite strain strategy defined");
    return bd.getAttribute<std::string>(
        AbaqusInterfaceBase::finiteStrainStrategy);
  }  // end of AbaqusInterfaceBase::getFiniteStrainStrategy

  bool AbaqusInterfaceBase::hasOrthotropyManagementPolicy(
      const BehaviourDescription& mb) {
    return mb.hasAttribute(AbaqusInterfaceBase::orthotropyManagementPolicy);
  }  // end of AbaqusInterfaceBase::hasOrthotropyManagementPolicy

  std::string AbaqusInterfaceBase::getOrthotropyManagementPolicy(
      const BehaviourDescription& mb) {
    return mb.getAttribute<std::string>(
        AbaqusInterfaceBase::orthotropyManagementPolicy);
  }  // end of AbaqusInterfaceBase::getOrthotropyManagementPolicy

  void AbaqusInterfaceBase::checkOrthotropyManagementPolicyConsistency(
      const BehaviourDescription& mb) {
    if (AbaqusInterfaceBase::hasOrthotropyManagementPolicy(mb)) {
      tfel::raise_if(
          mb.getSymmetryType() != mfront::ORTHOTROPIC,
          "AbaqusInterfaceBase::checkOrthotropyManagementPolicyConsistency: "
          "orthotropy management policy is only valid "
          "for orthotropic behaviour");
    }
  }  // end of AbaqusInterfaceBase::checkOrthotropyManagementPolicyConsistency

  std::string AbaqusInterfaceBase::getLibraryName(
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
    return makeUpperCase(lib);
  }  // end of AbaqusInterfaceBase::getLibraryName

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  AbaqusInterfaceBase::treatCommonKeywords(BehaviourDescription& bd,
                                           const std::string& k,
                                           tokens_iterator current,
                                           const tokens_iterator end) {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "AbaqusInterfaceBase::treatCommonKeywords: " + m);
    };
    if (k == "@AbaqusFiniteStrainStrategy") {
      throw_if(bd.hasAttribute(AbaqusInterfaceBase::finiteStrainStrategy),
               "a finite strain strategy has already been defined");
      throw_if(current == end, "unexpected end of file");
      const auto fs = current->value;
      throw_if(++current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';', read '" + current->value + '\'');
      ++(current);
      AbaqusInterfaceBase::checkFiniteStrainStrategyDefinitionConsistency(bd,
                                                                          fs);
      bd.setAttribute(AbaqusInterfaceBase::finiteStrainStrategy, fs, false);
      return {true, current};
    }
    if (k == "@AbaqusOrthotropyManagementPolicy") {
      auto read = [&throw_if](const std::string& s) {
        throw_if((s != "Native") && (s != "MFront"),
                 "unsupported orthotropy management "
                 "policy '" +
                     s +
                     "'\n"
                     "The only supported policies are "
                     "'MFront' and 'Native'");
        return s;
      };
      throw_if(bd.getSymmetryType() != mfront::ORTHOTROPIC,
               "orthotropy management policy is only valid "
               "for orthotropic behaviour");
      throw_if(bd.hasAttribute(AbaqusInterfaceBase::orthotropyManagementPolicy),
               "an orthotropy management policy has already been defined");
      throw_if(current == end, "unexpected end of file");
      bd.setAttribute(AbaqusInterfaceBase::orthotropyManagementPolicy,
                      read(current->value), false);
      throw_if(++current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';', read '" + current->value + '\'');
      ++(current);
      return {true, current};
    }
    return {false, current};
  }  // end of AbaqusInterfaceBase::treatCommonKeyword

  unsigned short AbaqusInterfaceBase::getStateVariablesOffset(
      const BehaviourDescription& bd, const Hypothesis h) const {
    if ((bd.hasAttribute(AbaqusInterfaceBase::orthotropyManagementPolicy)) &&
        (bd.getAttribute<std::string>(
             AbaqusInterfaceBase::orthotropyManagementPolicy) == "MFront")) {
      if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
          (h == ModellingHypothesis::PLANESTRAIN) ||
          (h == ModellingHypothesis::PLANESTRESS)) {
        return 2u;
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return 6u;
      }
      tfel::raise(
          "AbaqusInterfaceBase::getStateVariablesOffset: "
          "invalid hypothesis");
    }
    return 0u;
  }

  std::vector<std::string> AbaqusInterfaceBase::getCommonKeywords() const {
    return {"@AbaqusFiniteStrainStrategy", "@AbaqusOrthotropyManagementPolicy"};
  }  // end of AbaqusInterfaceBase::getCommonKeywords

  std::string AbaqusInterfaceBase::getFunctionNameBasis(
      const std::string& name) const {
    return makeUpperCase(name);
  }  // end of AbaqusInterfaceBase::getFunctionName

  std::string AbaqusInterfaceBase::getFunctionNameForHypothesis(
      const std::string& name, const Hypothesis h) const {
    const auto s = [h]() -> std::string {
      if (h == ModellingHypothesis::AXISYMMETRICAL) {
        return "AXIS";
      } else if (h == ModellingHypothesis::PLANESTRAIN) {
        return "PSTRAIN";
      } else if (h == ModellingHypothesis::PLANESTRESS) {
        return "PSTRESS";
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return "3D";
      }
      tfel::raise(
          "AbaqusInterfaceBase::getFunctionNameForHypothesis: "
          "invalid hypothesis.");
    }();
    return makeUpperCase(name) + "_" + s;
  }  // end of AbaqusInterfaceBase::getFunctionNameForHypothesis

  std::set<AbaqusInterfaceBase::Hypothesis>
  AbaqusInterfaceBase::getModellingHypothesesToBeTreated(
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
    if (h.empty()) {
      tfel::raise(
          "AbaqusInterfaceBase::getModellingHypothesesToBeTreated : "
          "no hypotheses selected. This means that the given beahviour "
          "can't be used neither in 'AxisymmetricalGeneralisedPlaneStrain' "
          "nor in 'AxisymmetricalGeneralisedPlaneStress', so it does not "
          "make sense to use the Abaqus interface");
    }
    return h;
  }  // end of AbaqusInterfaceBase::getModellingHypothesesToBeTreated

  void AbaqusInterfaceBase::writeAbaqusBehaviourTraits(
      std::ostream& out,
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
    out << "struct AbaqusTraits<tfel::material::" << mb.getClassName() << "<";
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      out << "H";
    } else {
      out << "tfel::material::ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h);
    }
    out << ", NumericType,";
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
          << " AbaqusBehaviourType btype = "
             "abaqus::STANDARDSTRAINBASEDBEHAVIOUR;\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "static " << constexpr_c
          << " AbaqusBehaviourType btype = "
             "abaqus::STANDARDFINITESTRAINBEHAVIOUR;\n";
    } else {
      tfel::raise(
          "AbaqusInterfaceBase::writeAbaqusBehaviourTraits : "
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
           "tfel::math::StensorDimeToSize<N>::value;\n";
    out << "// tensor size\n";
    out << "static " << constexpr_c
        << " unsigned short TensorSize  = "
           "tfel::math::TensorDimeToSize<N>::value;\n";
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
          << " AbaqusSymmetryType type = abaqus::ISOTROPIC;\n";
    } else if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "static " << constexpr_c
          << " AbaqusSymmetryType type = abaqus::ORTHOTROPIC;\n";
    } else {
      tfel::raise(
          "AbaqusInterfaceBase::writeAbaqusBehaviourTraits: "
          "unsupported behaviour type.\n"
          "The abaqus interface only support isotropic or orthotropic "
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
          << " AbaqusSymmetryType etype = abaqus::ISOTROPIC;\n";
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
    } else if (mb.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
      out << "static " << constexpr_c
          << " AbaqusSymmetryType etype = abaqus::ORTHOTROPIC;\n";
      if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                          false)) {
        out << "static " << constexpr_c
            << " unsigned short elasticPropertiesOffset "
            << "= AbaqusOrthotropicElasticPropertiesOffset<N>::value;\n";
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
    } else {
      tfel::raise(
          "AbaqusInterfaceBase::writeAbaqusBehaviourTraits: "
          "unsupported behaviour type.\n"
          "The abaqus interface only support isotropic or "
          "orthotropic behaviour at this time.");
    }
    out << "}; // end of class AbaqusTraits\n\n";
  }

  void AbaqusInterfaceBase::writeMTestFileGeneratorSetModellingHypothesis(
      std::ostream& out) const {
    out << "mg.setModellingHypothesis(h);\n";
  }  // end of
     // AbaqusInterfaceBase::writeMTestFileGeneratorSetModellingHypothesis

  void AbaqusInterfaceBase::writeInputFileExample(
      const BehaviourDescription& mb,
      const FileDescription& fd,
      const bool b) const {
    try {
      auto throw_if = [](const bool c, const std::string& m) {
        tfel::raise_if(c, "AbaqusInterfaceBase::writeInputFileExample: " + m);
      };
      const auto name = mb.getLibrary() + mb.getClassName();
      const auto mn = this->getLibraryName(mb) + "_" + mb.getClassName();
      const auto fn = (b ? "abaqus/" : "abaqus-explicit/") + name + ".inp";
      std::ofstream out{fn};
      throw_if(!out, "could not open file '" + fn + "'");
      // header
      out << "** \n"
          << "** File generated by MFront from the " << fd.fileName
          << " source\n"
          << "** Example of how to use the " << mb.getClassName()
          << " behaviour law\n"
          << "** Author " << fd.authorName << '\n'
          << "** Date   " << fd.date << '\n'
          << "**\n\n";
      // loop over hypothesis
      for (const auto& h : this->getModellingHypothesesToBeTreated(mb)) {
        const auto& d = mb.getBehaviourData(h);
        const auto mps = this->buildMaterialPropertiesList(mb, h);
        auto msize = SupportedTypes::TypeSize{};
        if (!mps.first.empty()) {
          const auto& m = mps.first.back();
          msize = m.offset;
          msize += SupportedTypes::getTypeSize(m.type, m.arraySize);
        }
        const auto& persistentVarsHolder = d.getPersistentVariables();
        auto vs = SupportedTypes::TypeSize{};
        for (const auto& v : persistentVarsHolder) {
          vs += SupportedTypes::getTypeSize(v.type, v.arraySize);
        }
        const auto vsize = vs.getValueForModellingHypothesis(h) +
                           this->getStateVariablesOffset(mb, h);
        const auto& externalStateVarsHolder = d.getExternalStateVariables();
        out << "** Example for the '" << ModellingHypothesis::toString(h)
            << "' modelling hypothesis\n";
        if (!externalStateVarsHolder.empty()) {
          out << "** This behaviour requires " << externalStateVarsHolder.size()
              << " field variables to be defined:\n";
          int i = 1;
          for (auto pv = std::next(externalStateVarsHolder
                                       .begin());  // skipping the temperature
               pv != externalStateVarsHolder.end(); ++pv, ++i) {
            out << "** " << i << ": " << mb.getExternalName(h, pv->name);
          }
        }
        out << "*Material, name=" << this->getFunctionNameForHypothesis(mn, h)
            << '\n';
        if (!b) {
          out << "*DENSITY\n<density>\n";
        }
        if (vsize != 0) {
          out << "*Depvar\n" << vsize << ",\n";
          int i = 1;
          if (mb.hasAttribute(
                  AbaqusInterfaceBase::orthotropyManagementPolicy)) {
            const auto omp = mb.getAttribute<std::string>(
                AbaqusInterfaceBase::orthotropyManagementPolicy);
            if (omp == "MFront") {
              if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
                  (h == ModellingHypothesis::PLANESTRAIN) ||
                  (h == ModellingHypothesis::PLANESTRESS)) {
                out << i++ << ", FirstOrthotropicAxis_1\n";
                out << i++ << ", FirstOrthotropicAxis_2\n";
              } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
                out << i++ << ", FirstOrthotropicAxis_1\n";
                out << i++ << ", FirstOrthotropicAxis_2\n";
                out << i++ << ", FirstOrthotropicAxis_3\n";
                out << i++ << ", SecondOrthotropicAxis_1\n";
                out << i++ << ", SecondOrthotropicAxis_2\n";
                out << i++ << ", SecondOrthotropicAxis_3\n";
              }
            } else {
              throw_if(omp != "Native",
                       "unsupported orthotropy "
                       "management policy");
            }
          }
          for (const auto& v : persistentVarsHolder) {
            const auto vn = mb.getExternalName(h, v.name);
            if (v.arraySize == 1) {
              this->writeDepvar(out, i, h, v, vn);
            } else {
              for (unsigned short a = 0; a != v.arraySize; ++a) {
                this->writeDepvar(out, i, h, v,
                                  vn + '[' + std::to_string(a) + ']');
              }
            }
          }
        }
        if (!mps.first.empty()) {
          out << "** The material properties are given as if we used "
                 "parameters "
                 "to explicitly\n"
              << "** display their names. Users shall replace those "
                 "declaration "
                 "by\n"
              << "** theirs values and/or declare those parameters in the "
                 "appropriate *parameters\n"
              << "** section of the input file\n";
        }
        out << "*User Material, constants="
            << msize.getValueForModellingHypothesis(h);
        if (!mb.getAttribute(
                h, BehaviourData::isConsistentTangentOperatorSymmetric,
                false)) {
          out << ", unsymm";
        }
        out << '\n';
        if (!mps.first.empty()) {
          int i = 1;
          auto write = [&i, &out](const std::string& n) {
            if (i % 9 == 0) {
              out << "\n";
              i = 1;
            }
            out << '<' << n << '>';
            ++i;
          };
          for (auto pm = mps.first.begin(); pm != mps.first.end();) {
            if (pm->arraySize == 1u) {
              write(pm->name);
            } else {
              for (unsigned short a = 0; a != pm->arraySize;) {
                write(pm->name + "_" + std::to_string(a));
                if (++a != pm->arraySize) {
                  out << ", ";
                }
              }
            }
            if (++pm != mps.first.end()) {
              out << ", ";
            }
          }
        }
        out << "\n\n";
      }
    } catch (std::exception& e) {
      if (getVerboseMode() > VERBOSE_QUIET) {
        getLogStream() << e.what() << std::endl;
      }
    } catch (...) {
      if (getVerboseMode() > VERBOSE_QUIET) {
        getLogStream() << "AbaqusInterfaceBase::generateInputFileExample: "
                       << "unknown exception thrown" << std::endl;
      }
    }
  }  // end of AbaqusInterfaceBase::writeInputFileExample

  void AbaqusInterfaceBase::writeDepvar(std::ostream& out,
                                        int& i,
                                        const Hypothesis& h,
                                        const VariableDescription& v,
                                        const std::string& n) const {
    if (SupportedTypes::getTypeFlag(v.type) == SupportedTypes::SCALAR) {
      out << i++ << ", " << n << '\n';
    } else if (SupportedTypes::getTypeFlag(v.type) == SupportedTypes::TVECTOR) {
      out << i++ << ", " << n << "_1\n";
      out << i++ << ", " << n << "_2\n";
      if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        out << i++ << ", " << n << "_3\n";
      }
    } else if (SupportedTypes::getTypeFlag(v.type) == SupportedTypes::STENSOR) {
      out << i++ << ", " << n << "_11\n";
      out << i++ << ", " << n << "_22\n";
      out << i++ << ", " << n << "_33\n";
      out << i++ << ", " << n << "_12\n";
      if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        out << i++ << ", " << n << "_13\n";
        out << i++ << ", " << n << "_23\n";
      }
    } else if (SupportedTypes::getTypeFlag(v.type) == SupportedTypes::TENSOR) {
      out << i++ << ", " << n << "_11\n";
      out << i++ << ", " << n << "_22\n";
      out << i++ << ", " << n << "_33\n";
      out << i++ << ", " << n << "_12\n";
      out << i++ << ", " << n << "_21\n";
      if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        out << i++ << ", " << n << "_13\n";
        out << i++ << ", " << n << "_31\n";
        out << i++ << ", " << n << "_23\n";
        out << i++ << ", " << n << "_32\n";
      }
    } else {
      tfel::raise(
          "AbaqusInterfaceBase::writeDepvar: "
          "unsupported variable type");
    }
  }

  AbaqusInterfaceBase::~AbaqusInterfaceBase() = default;

}  // end of namespace mfront
