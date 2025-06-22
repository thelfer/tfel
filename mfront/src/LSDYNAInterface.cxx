/*!
 * \file   LSDYNAInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   16 mars 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <fstream>
#include <iostream>

#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/System/System.hxx"

#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontLock.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/LSDYNAInterface.hxx"

namespace mfront {

  //! \brief copy lsdyna utility files locally
  static void copyLSDYNAFiles() {
    std::ofstream out;
    MFrontLockGuard lock;
    for (const std::string f :
         {"CMakeLists.txt", "mfront-lsdyna.cxx", "test-lsdyna.cxx"}) {
      out.open("lsdyna/" + f);
      if (out) {
        const auto root = tfel::getInstallPath();
#ifdef TFEL_APPEND_SUFFIX
        const auto fn = root + "/share/doc/mfront-" TFEL_SUFFIX "/lsdyna/" + f;
#else  /* TFEL_APPEND_SUFFIX */
        const auto fn = root + "/share/doc/mfront/lsdyna/" + f;
#endif /* TFEL_APPEND_SUFFIX */
        std::ifstream in{fn};
        if (in) {
          out << in.rdbuf();
          in.close();
        } else {
          std::cerr << "copyLSDYNAFiles: could not open file '" << fn << "'\n";
        }
      } else {
        std::cerr << "copyLSDYNAFiles: could not open file 'lsdyna/" << f
                  << "'\n";
      }
      out.close();
    }
  }  // end of copyLSDYNAFiles

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
  }  // end of
     // checkFiniteStrainStrategyDefinitionConsistency

  static void checkFiniteStrainStrategyDefinitionConsistency(
      const BehaviourDescription& bd) {
    if (bd.getBehaviourType() !=
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (bd.hasAttribute(LSDYNAInterface::finiteStrainStrategy)) {
        tfel::raise(
            "finite strain strategy is only supported for strain based "
            "behaviours");
      }
    } else {
      if (bd.hasAttribute(LSDYNAInterface::finiteStrainStrategy)) {
        const auto fs =
            bd.getAttribute<std::string>(LSDYNAInterface::finiteStrainStrategy);
        checkFiniteStrainStrategyDefinitionConsistency(bd, fs);
      }
    }
  }  // end of checkFiniteStrainStrategyDefinitionConsistency

  static bool hasFiniteStrainStrategy(const BehaviourDescription& bd) {
    checkFiniteStrainStrategyDefinitionConsistency(bd);
    if (bd.isStrainMeasureDefined()) {
      return bd.getStrainMeasure() != BehaviourDescription::LINEARISED;
    }
    return bd.hasAttribute(LSDYNAInterface::finiteStrainStrategy);
  }  // end of hasFiniteStrainStrategy

  std::string getFiniteStrainStrategy(const BehaviourDescription& bd) {
    checkFiniteStrainStrategyDefinitionConsistency(bd);
    if (bd.isStrainMeasureDefined()) {
      const auto ms = bd.getStrainMeasure();
      if (ms == BehaviourDescription::GREENLAGRANGE) {
        return "FiniteRotationSmallStrain";
      } else if (ms == BehaviourDescription::HENCKY) {
        return "MieheApelLambrechtLogarithmicStrain";
      } else {
        tfel::raise("getFiniteStrainStrategy: unsupported strain measure");
      }
    }
    if (!bd.hasAttribute(LSDYNAInterface::finiteStrainStrategy)) {
      tfel::raise("getFiniteStrainStrategy: no finite strain strategy defined");
    }
    return bd.getAttribute<std::string>(LSDYNAInterface::finiteStrainStrategy);
  }  // end of getFiniteStrainStrategy

  static bool hasOrthotropyManagementPolicy(const BehaviourDescription& mb) {
    return mb.hasAttribute(LSDYNAInterface::orthotropyManagementPolicy);
  }  // end of hasOrthotropyManagementPolicy

  static std::string getOrthotropyManagementPolicy(
      const BehaviourDescription& mb) {
    return mb.getAttribute<std::string>(
        LSDYNAInterface::orthotropyManagementPolicy);
  }  // end of getOrthotropyManagementPolicy

  static bool usesMFrontOrthotropyManagementPolicy(
      const BehaviourDescription& mb) {
    if (hasOrthotropyManagementPolicy(mb)) {
      return getOrthotropyManagementPolicy(mb) == "MFront";
    }
    return false;
  }  // end of usesMFrontOrthotropyManagementPolicy

  static void writeLSDYNAArguments(std::ostream& out, const bool f) {
    if (f) {
      out << "(const lsdyna::LSDYNAInt *const nblock,\n"
          << "const lsdyna::LSDYNAInt *const ndir,\n"
          << "const lsdyna::LSDYNAInt *const nshr,\n"
          << "const lsdyna::LSDYNAInt *const nstatev,\n"
          << "const lsdyna::LSDYNAInt *const nfieldv,\n"
          << "const lsdyna::LSDYNAInt *const nprops,\n"
          << "const lsdyna::LSDYNAInt * const lanneal,\n"
          << "const double* const stepTime,\n"
          << "const int)";
    } else {
      out << "(const lsdyna::LSDYNAInt *const nblock,\n"
          << "const int)";
    }
  }  // end of writeLSDYNAArguments

  static void writeLSDYNAArguments(std::ostream& out) {
    out << "(const lsdyna::LSDYNAInt *const,\n"
        << "const double* const,\n"
        << "const int)";
  }  // end of writeLSDYNAArguments

  const char* const LSDYNAInterface::finiteStrainStrategy =
      "lsdyna::finiteStrainStrategy";

  const char* const LSDYNAInterface::orthotropyManagementPolicy =
      "lsdyna::orthotropyManagementPolicy";

  std::set<LSDYNAInterface::Hypothesis>
  LSDYNAInterface::getModellingHypothesesToBeTreated(
      const BehaviourDescription& bd) const {
    auto mh = std::set<LSDYNAInterface::Hypothesis>{};
    const auto& bh = bd.getModellingHypotheses();
    if (bh.find(ModellingHypothesis::TRIDIMENSIONAL) != bh.end()) {
      mh.insert(ModellingHypothesis::TRIDIMENSIONAL);
    }
    if (bh.find(ModellingHypothesis::PLANESTRESS) != bh.end()) {
      mh.insert(ModellingHypothesis::PLANESTRESS);
    }
    tfel::raise_if(
        mh.empty(),
        "LSDYNAInterface::getModellingHypothesesToBeTreated : "
        "neither the 'Tridimensional' hypothesis nor "
        "the 'PlaneStress' hypothesis are not supported. "
        "At least, one of these is required by the LSDYNA interface");
    return mh;
  }  // end of LSDYNAInterface::getModellingHypothesesToBeTreated

  std::string LSDYNAInterface::getInterfaceName() const {
    return "LSDYNA";
  }  // end of LSDYNAInterface::getInterfaceName

  std::string LSDYNAInterface::getLibraryName(
      const mfront::BehaviourDescription& bd) const {
    if (bd.getLibrary().empty()) {
      if (!bd.getMaterialName().empty()) {
        return this->getInterfaceName() + bd.getMaterialName();
      } else {
        return this->getInterfaceName() + "Behaviour";
      }
    }
    return this->getInterfaceName() + bd.getLibrary();
  }  // end of LSDYNAInterface::getLibraryName

  std::string LSDYNAInterface::getFunctionNameBasis(
      const std::string& name) const {
    return name;
  }  // end of LSDYNAInterface::getFunctionName

  std::string LSDYNAInterface::getFunctionNameForHypothesis(
      const std::string& name, const Hypothesis h) const {
    const auto s = [h]() -> std::string {
      if (h == ModellingHypothesis::PLANESTRESS) {
        return "PSTRESS";
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return "3D";
      }
      tfel::raise(
          "LSDYNAInterface::getFunctionNameForHypothesis: "
          "invalid hypothesis.");
    }();
    return name + "_" + s;
  }  // end of LSDYNAInterface::getFunctionNameForHypothesis

  std::string LSDYNAInterface::getName() { return "lsdyna"; }

  std::pair<bool, LSDYNAInterface::tokens_iterator>
  LSDYNAInterface::treatKeyword(BehaviourDescription& bd,
                                const std::string& key,
                                const std::vector<std::string>& i,
                                tokens_iterator current,
                                const tokens_iterator end) {
    if (!i.empty()) {
      if (std::find(i.begin(), i.end(), this->getName()) == i.end()) {
        return {false, current};
      }
      tfel::raise(
          "LSDYNAInterface::treatKeyword: "
          "unsupported key '" +
          key + "'");
    }
    return {false, current};
  }  // end of LSDYNAInterface::treatKeyword

  void LSDYNAInterface::getTargetsDescription(TargetsDescription& d,
                                              const BehaviourDescription& bd) {
    const auto lib = this->getLibraryName(bd);
    const auto name = bd.getLibrary() + bd.getClassName();
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    insert_if(d[lib].cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
    insert_if(d[lib].include_directories,
              "$(shell " + tfel_config + " --include-path)");
    insert_if(d[lib].sources, "lsdyna" + name + ".cxx");
    d.headers.push_back("MFront/LSDYNA/lsdyna" + name + ".hxx");
    insert_if(d[lib].link_directories,
              "$(shell " + tfel_config + " --library-path)");
    insert_if(d[lib].link_libraries,
              tfel::getLibraryInstallName("LSDYNAInterface"));
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
    for (const auto h : this->getModellingHypothesesToBeTreated(bd)) {
      insert_if(d[lib].epts, this->getFunctionNameForHypothesis(name, h));
    }
  }  // end of LSDYNAInterface::getTargetsDescription

  void LSDYNAInterface::writeInterfaceSpecificIncludes(
      std::ostream& out, const BehaviourDescription&) const {
    out << "#include\"MFront/LSDYNA/LSDYNAInterface.hxx\"\n\n";
  }  // end of LSDYNAInterface::writeInterfaceSpecificIncludes

  void LSDYNAInterface::endTreatment(const BehaviourDescription& mb,
                                     const FileDescription& fd) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "LSDYNAInterface::endTreatment: " + m);
    };
    //    checkOrthotropyManagementPolicyConsistency(mb);
    throw_if(!((mb.getBehaviourType() ==
                BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
               (mb.getBehaviourType() ==
                BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)),
             "the LSDYNA interface only supports small and "
             "finite strain behaviours");
    //     throw_if((mb.getBehaviourType() ==
    //               BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
    //                  (!hasFiniteStrainStrategy(mb)),
    //              "behaviours written in the small strain framework "
    //              "must be embedded in a strain strategy. See the "
    //              "'@LSDYNAFiniteStrainStrategy' keyword");
    // get the modelling hypotheses to be treated
    const auto& mh = this->getModellingHypothesesToBeTreated(mb);
    const auto name = mb.getLibrary() + mb.getClassName();
    // output directories
    tfel::system::systemCall::mkdir("include/MFront");
    tfel::system::systemCall::mkdir("include/MFront/LSDYNA");
    tfel::system::systemCall::mkdir("lsdyna");
    copyLSDYNAFiles();
    //     // header
    //     auto fname = "lsdyna" + name + ".hxx";
    //     std::ofstream out("include/MFront/LSDYNA/" + fname);
    //     throw_if(!out, "could not open file '" + fname + "'");
    //
    //     out << "/*!\n"
    //         << "* \\file   " << fname << '\n'
    //         << "* \\brief  This file declares the lsdyna
    //         interface
    //         for "
    //            "the "
    //         << mb.getClassName() << " behaviour law\n"
    //         << "* \\author " << fd.authorName << '\n'
    //         << "* \\date   " << fd.date << '\n'
    //         << "*/\n\n";
    //
    //     const auto header = this->getHeaderGuard(mb);
    //     out << "#ifndef " << header << "\n"
    //         << "#define " << header << "\n\n"
    //         << "#include\"TFEL/Config/TFELConfig.hxx\"\n"
    //         << "#include\"MFront/LSDYNA/LSDYNA.hxx\"\n\n"
    //         << "#ifdef __cplusplus\n"
    //         << "#include\"MFront/LSDYNA/LSDYNATraits.hxx\"\n";
    //     if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
    //       out <<
    //       "#include\"MFront/LSDYNA/LSDYNAOrthotropicBehaviour.hxx\"\n";
    //     }
    //     out << "#include\"TFEL/Material/" << mb.getClassName() <<
    //     ".hxx\"\n"
    //         << "#endif /* __cplusplus */\n\n";
    //
    //     this->writeVisibilityDefines(out);
    //
    //     out << "#ifdef __cplusplus\n\n"
    //         << "namespace lsdyna{\n\n";
    //
    //     if (!mb.areAllMechanicalDataSpecialised(mh)) {
    //       this->writeLSDYNABehaviourTraits(
    //           out, mb, ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    //     }
    //     for (const auto& h : mh) {
    //       if (mb.hasSpecialisedMechanicalData(h)) {
    //         this->writeLSDYNABehaviourTraits(out, mb, h);
    //       }
    //     }
    //
    //     out << "} // end of namespace lsdyna\n\n"
    //         << "#endif /* __cplusplus */\n\n"
    //         << "#ifdef __cplusplus\n"
    //         << "extern \"C\"{\n"
    //         << "#endif /* __cplusplus */\n\n";
    //
    //     this->writeSetOutOfBoundsPolicyFunctionDeclaration(out, name);
    //     this->writeSetParametersFunctionsDeclarations(out, name, mb);
    //
    //     for (const auto& h : mh) {
    //       out << "MFRONT_SHAREDOBJ void\n"
    //           << this->getFunctionNameForHypothesis(name, h) << "_f";
    //       writeLSDYNAArguments(out, "float");
    //       out << ";\n\n";
    //
    //       out << "MFRONT_SHAREDOBJ void\n"
    //           << this->getFunctionNameForHypothesis(name, h);
    //       writeLSDYNAArguments(out, "double");
    //       out << ";\n\n";
    //     }
    //
    //     out << "#ifdef __cplusplus\n"
    //         << "}\n"
    //         << "#endif /* __cplusplus */\n\n"
    //         << "#endif /* " << header << " */\n";
    //
    //     out.close();
    //
    //     fname = "lsdyna" + name + ".cxx";
    //     out.open("src/" + fname);
    //     throw_if(!out, "could not open file '" + fname + "'");
    //
    //     out << "/*!\n"
    //         << "* \\file   " << fname << '\n'
    //         << "* \\brief  This file implements the lsdyna
    //         interface
    //         for "
    //            "the "
    //         << mb.getClassName() << " behaviour law\n"
    //         << "* \\author " << fd.authorName << '\n'
    //         << "* \\date   " << fd.date << '\n'
    //         << "*/\n\n";
    //
    //     out << "#include<cmath>\n"
    //         << "#include<limits>\n"
    //         << "#include<cstdlib>\n";
    //     this->getExtraSrcIncludes(out, mb);
    //
    //     if (ppolicy == "ThreadPool") {
    //       out << "#include\"TFEL/System/ThreadPool.hxx\"\n";
    //     }
    //     out << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n"
    //         << "#include\"TFEL/Material/" << mb.getClassName() <<
    //         ".hxx\"\n";
    //     if (mb.getAttribute(BehaviourData::profiling, false)) {
    //       out << "#include\"MFront/BehaviourProfiler.hxx\"\n\n";
    //     }
    //     if (usesMFrontOrthotropyManagementPolicy(mb)) {
    //       out << "#include\"MFront/LSDYNA/LSDYNARotation.hxx\"\n\n";
    //     }
    //     out <<
    //     "#include\"MFront/LSDYNA/LSDYNAStressFreeExpansionHandler.hxx\"\n\n"
    //         << "#include\"MFront/LSDYNA/LSDYNAInterface.hxx\"\n\n"
    //         << "#include\"MFront/LSDYNA/lsdyna" << name <<
    //         ".hxx\"\n\n";
    //
    //     this->writeGetOutOfBoundsPolicyFunctionImplementation(out,
    //     name);
    //
    //     if (ppolicy == "ThreadPool") {
    //       out << "static size_t getLSDYNANumberOfThreads(){\n"
    //           << "const auto nthreads =
    //           ::getenv(\"LSDYNAEXPLICIT_NTHREADS\");\n"
    //           << "if(nthreads==nullptr){\n"
    //           << "return 4;\n"
    //           << "}\n"
    //           << "return std::stoi(nthreads);\n"
    //           << "}\n\n";
    //     }
    //
    //     out << "extern \"C\"{\n\n";
    //
    //     this->generateGeneralSymbols(out, name, mb, fd);
    //     if (!mb.areAllMechanicalDataSpecialised(mh)) {
    //       const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    //       this->generateSymbols(out, name, uh, mb, fd);
    //     }
    //     for (const auto& h : mh) {
    //       if (mb.hasSpecialisedMechanicalData(h)) {
    //         this->generateSymbols(out, name, h, mb, fd);
    //       }
    //     }
    //
    //     this->writeSetParametersFunctionsImplementations(out, name, mb);
    //     this->writeSetOutOfBoundsPolicyFunctionImplementation(out,
    //     name);
    //
    //     for (const auto h : mh) {
    //       for (const std::string t : {"float", "double"}) {
    //         out << "MFRONT_SHAREDOBJ void\n"
    //             << this->getFunctionNameForHypothesis(name, h);
    //         if (t == "float") {
    //           out << "_f";
    //         }
    //         writeLSDYNAArguments(out, t, false);
    //         out << "{\n"
    //             << "using namespace tfel::math;\n"
    //             << "using ModellingHypothesis = "
    //                "tfel::material::ModellingHypothesis;\n"
    //             << "using lsdyna::LSDYNATraits;\n"
    //             << "using tfel::material::" << mb.getClassName() <<
    //             ";\n"
    //             << "using LSDYNAData = lsdyna::LSDYNAData<" << t <<
    //             ">;\n"
    //             << "TFEL_CONSTEXPR const auto cste = Cste<" << t <<
    //             ">::sqrt2;\n"
    //             << "auto view = [&nblock](" << t << "* v){\n"
    //             << "  return LSDYNAData::strided_iterator(v,*nblock);\n"
    //             << "};\n"
    //             << "auto cview = [&nblock](const " << t << "* v){\n"
    //             << "  return
    //             LSDYNAData::strided_const_iterator(v,*nblock);\n"
    //             << "};\n"
    //             << "auto cdiffview = [&nblock](const " << t << "* v1,\n"
    //             << "                           const " << t << "*
    //             v2){\n"
    //             << "  return "
    //                "LSDYNAData::diff_strided_const_iterator(LSDYNAData::strided_"
    //                "const_iterator(v1,*nblock),\n"
    //             << " "
    //                "LSDYNAData::strided_const_iterator(v2,*nblock));\n"
    //             << "};\n";
    //         if (mb.getAttribute(BehaviourData::profiling, false)) {
    //           out << "using mfront::BehaviourProfiler;\n"
    //               << "using tfel::material::" << mb.getClassName() <<
    //               "Profiler;\n"
    //               << "BehaviourProfiler::Timer total_timer(" <<
    //               mb.getClassName()
    //               << "Profiler::getProfiler(),\n"
    //               << "BehaviourProfiler::TOTALTIME);\n";
    //         }
    //         if (ppolicy == "ThreadPool") {
    //           out << "static tfel::system::ThreadPool "
    //                  "pool(getLSDYNANumberOfThreads());\n";
    //         }
    //         this->writeChecks(out, mb, t, h);
    //         if (mb.getBehaviourType() ==
    //             BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
    //           throw_if(!hasFiniteStrainStrategy(mb),
    //                    "no finite strain strategy defined");
    //           const auto fs =
    //           getFiniteStrainStrategy(mb);
    //           if (fs == "Native") {
    //             this->writeNativeBehaviourCall(out, mb, t, h);
    //           } else if (fs == "FiniteRotationSmallStrain") {
    //             this->writeFiniteRotationSmallStrainBehaviourCall(out,
    //             mb, t,
    //             h);
    //           } else if (fs == "MieheApelLambrechtLogarithmicStrain") {
    //             this->writeLogarithmicStrainBehaviourCall(out, mb, t,
    //             h);
    //           } else {
    //             throw_if(true,
    //                      "unsupported finite strain strategy (internal
    //                      error)");
    //           }
    //         } else if (mb.getBehaviourType() ==
    //                    BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)
    //                    {
    //           this->writeFiniteStrainBehaviourCall(out, mb, t, h);
    //         } else {
    //           throw_if(true,
    //                    "the LS-DYNA interface "
    //                    "only supports small "
    //                    "and finite strain behaviours");
    //         }
    //         out << "}\n\n";
    //       }
    //     }
    //     out << "} // end of extern \"C\"\n";
    //     out.close();
    //     this->writeInputFileExample(mb, fd, false);
  }

  //   static void writeGetRotationMatrix(std::ostream& out,
  //                                      const LSDYNAInterface::Hypothesis h) {
  //     if ((h == LSDYNAInterface::ModellingHypothesis::PLANESTRESS) ||
  //         (h == LSDYNAInterface::ModellingHypothesis::AXISYMMETRICAL) ||
  //         (h == LSDYNAInterface::ModellingHypothesis::PLANESTRAIN)) {
  //       out << "const auto R =
  //       lsdyna::getRotationMatrix2D(cview(stateOld+i));\n";
  //     } else if (h == LSDYNAInterface::ModellingHypothesis::TRIDIMENSIONAL) {
  //       out << "const auto R =
  //       lsdyna::getRotationMatrix3D(cview(stateOld+i));\n";
  //     } else {
  //       tfel::raise("writeGetRotationMatrix: unsupported hypothesis");
  //     }
  //   }

  void LSDYNAInterface::writeBehaviourConstructorHeader(
      std::ostream& out,
      const BehaviourDescription& mb,
      const Hypothesis,
      const std::string& initStateVarsIncrements) const {
    //     const auto iprefix = makeUpperCase(this->getInterfaceName());
    //     const auto av = this->getBehaviourConstructorsAdditionalVariables();
    //     const auto abdv =
    //     this->getBehaviourDataConstructorAdditionalVariables();
    //     const auto aidv =
    //     this->getIntegrationDataConstructorAdditionalVariables();
    //     out << "/*\n"
    //         << " * \\brief constructor for the LS-DYNA interface\n"
    //         << " * \\param[in] " << iprefix << "d : data\n"
    //         << " */\n"
    //         << mb.getClassName() << "(const lsdyna::LSDYNAData<Type>& " <<
    //         iprefix
    //         << "d)\n";
    //     if (mb.useQt()) {
    //       out << ": " << mb.getClassName()
    //           << "BehaviourData<hypothesis,Type,use_qt>(" << iprefix <<
    //           "d),\n"
    //           << mb.getClassName() <<
    //           "IntegrationData<hypothesis,Type,use_qt>("
    //           << iprefix << "d)\n";
    //     } else {
    //       out << ": " << mb.getClassName()
    //           << "BehaviourData<hypothesis,Type,false>(" << iprefix <<
    //           "d),\n"
    //           << mb.getClassName() <<
    //           "IntegrationData<hypothesis,Type,false>("
    //           << iprefix << "d)\n";
    //     }
    //     if (!initStateVarsIncrements.empty()) {
    //       out << ",\n" << initStateVarsIncrements;
    //     }
  }  // end of LSDYNAInterface::writeBehaviourConstructorHeader

  void LSDYNAInterface::writeBehaviourDataConstructor(
      std::ostream& out,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    //     const auto& d = mb.getBehaviourData(h);
    //     const auto iprefix = makeUpperCase(this->getInterfaceName());
    //     const auto mprops = this->buildMaterialPropertiesList(mb, h);
    //     const auto& persistentVarsHolder = d.getPersistentVariables();
    //     const auto& externalStateVarsHolder = d.getExternalStateVariables();
    //     out << "/*\n"
    //         << " * \\brief constructor for the LS-DYNA interface\n"
    //         << " * \\param[in] " << iprefix << "d : data\n"
    //         << " */\n"
    //         << mb.getClassName() << "BehaviourData"
    //         << "(const lsdyna::LSDYNAData<Type>& " << iprefix << "d)\n: ";
    //     bool first = true;
    //     this->writeMaterialPropertiesInitializersInBehaviourDataConstructorI(
    //         out, first, h, mb, mprops.first, mprops.second, iprefix +
    //         "d.props", "",
    //         "");
    //     this->writeVariableInitializersInBehaviourDataConstructorI(
    //         out, first, persistentVarsHolder, iprefix + "d.stateOld", "",
    //         "");
    //     if (!first) {
    //       out << ",\n";
    //     }
    //     first = false;
    //     out << "T(" << iprefix << "d.tempOld)";
    //     this->writeVariableInitializersInBehaviourDataConstructorI(
    //         out, first, std::next(externalStateVarsHolder.begin()),
    //         externalStateVarsHolder.end(), iprefix + "d.fieldOld", "", "");
    //     out << "\n{\n";
    //     this->writeMaterialPropertiesInitializersInBehaviourDataConstructorII(
    //         out, h, mb, mprops.first, mprops.second, iprefix + "d.props", "",
    //         "");
    //     this->writeVariableInitializersInBehaviourDataConstructorII(
    //         out, mb, persistentVarsHolder, iprefix + "d.stateOld", "", "");
    //     this->writeVariableInitializersInBehaviourDataConstructorII(
    //         out, mb, std::next(externalStateVarsHolder.begin()),
    //         externalStateVarsHolder.end(), iprefix + "d.fieldOld", "", "");
    //     this->completeBehaviourDataConstructor(out, h, mb);
    //     out << "}\n\n";
  }

  void LSDYNAInterface::writeIntegrationDataConstructor(
      std::ostream& out,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    //     const auto av = this->getBehaviourConstructorsAdditionalVariables();
    //     const auto aidv =
    //     this->getIntegrationDataConstructorAdditionalVariables();
    //     const auto& d = mb.getBehaviourData(h);
    //     const auto iprefix = makeUpperCase(this->getInterfaceName());
    //     const auto& externalStateVarsHolder = d.getExternalStateVariables();
    //     out << "/*\n"
    //         << " * \\brief constructor for the LS-DYNA interface\n"
    //         << " * \\param[in] " << iprefix << "d : data"
    //         << " */\n"
    //         << mb.getClassName() << "IntegrationData"
    //         << "(const lsdyna::LSDYNAData<Type>& " << iprefix << "d)"
    //         << ": dt(" << iprefix << "d.dt),\n"
    //         << "  dT(" << iprefix << "d.tempNew-" << iprefix << "d.tempOld)";
    //     bool first = false;
    //     this->writeVariableInitializersInBehaviourDataConstructorI(
    //         out, first, std::next(externalStateVarsHolder.begin()),
    //         externalStateVarsHolder.end(), iprefix + "d.dfield", "d", "");
    //     out << "\n{\n";
    //     this->writeVariableInitializersInBehaviourDataConstructorII(
    //         out, mb, std::next(externalStateVarsHolder.begin()),
    //         externalStateVarsHolder.end(), iprefix + "d.dfield", "d", "");
    //     out << "}\n\n";
  }

  void LSDYNAInterface::writeBehaviourDataMainVariablesSetters(
      std::ostream& os, const BehaviourDescription& mb) const {
    auto throw_unsupported_hypothesis = [] {
      tfel::raise(
          "LSDYNAInterface::writeBehaviourDataMainVariablesSetters: "
          "only small strain or finite behaviours are supported");
    };
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if ((mb.getBehaviourType() !=
         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
        (mb.getBehaviourType() !=
         BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)) {
      throw_unsupported_hypothesis();
    }
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      os << "void setBehaviourDataGradients(const Stensor& " << iprefix
         << "stran)\n"
         << "{\n"
         << "this->eto = " << iprefix << "stran;\n"
         << "}\n\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      os << "void setBehaviourDataGradients(const Tensor& " << iprefix
         << "stran)\n"
         << "{\n"
         << "this->F0 = " << iprefix << "stran;\n"
         << "}\n\n";
    } else {
      throw_unsupported_hypothesis();
    }
    os << "void setBehaviourDataThermodynamicForces(const Stensor& " << iprefix
       << "stress)\n"
       << "{\n"
       << "this->sig = " << iprefix << "stress;\n"
       << "}\n\n";
  }  // end of LSDYNAInterface::writeBehaviourDataMainVariablesSetters

  void LSDYNAInterface::writeBehaviourDataGradientSetter(
      std::ostream&, const Gradient&, const SupportedTypes::TypeSize) const {
  }  // end of LSDYNAInterface::writeBehaviourDataGradientSetter

  void LSDYNAInterface::writeBehaviourDataThermodynamicForceSetter(
      std::ostream&,
      const ThermodynamicForce&,
      const SupportedTypes::TypeSize) const {
  }  // end of LSDYNAInterface::writeBehaviourDataThermodynamicForceSetter

  void LSDYNAInterface::writeIntegrationDataMainVariablesSetters(
      std::ostream& os, const BehaviourDescription& mb) const {
    auto throw_unsupported_hypothesis = [] {
      tfel::raise(
          "LSDYNAInterface::writeIntegrationDataMainVariablesSetters: "
          "only small strain or finite behaviours are supported");
    };
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if ((mb.getBehaviourType() !=
         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
        (mb.getBehaviourType() !=
         BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)) {
      throw_unsupported_hypothesis();
    }
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      os << "void setIntegrationDataGradients(const Stensor& " << iprefix
         << "dstran)\n"
         << "{\n"
         << "this->deto = " << iprefix << "dstran;\n"
         << "}\n\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      os << "void setIntegrationDataGradients(const Tensor& " << iprefix
         << "dstran)\n"
         << "{\n"
         << "this->F1 = " << iprefix << "dstran;\n"
         << "}\n\n";
    } else {
      throw_unsupported_hypothesis();
    }
  }  // end of writeIntegrationDataMainVariablesSetters

  void LSDYNAInterface::writeIntegrationDataGradientSetter(
      std::ostream&, const Gradient&, const SupportedTypes::TypeSize) const {
  }  // end of LSDYNAInterface::writeIntegrationDataGradientSetter

  void LSDYNAInterface::exportMechanicalData(
      std::ostream& out,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    const auto& d = mb.getBehaviourData(h);
    const auto& ivs = d.getPersistentVariables();
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (!ivs.empty()) {
      out << "void exportStateData("
          << "Stensor& " << iprefix + "s, const lsdyna::LSDYNAData<Type>& "
          << iprefix + "d) const\n";
    } else {
      out << "void exportStateData("
          << "Stensor& "
          << iprefix + "s, const lsdyna::LSDYNAData<Type>&) const\n";
    }
    out << "{\n"
        << "using namespace tfel::math;\n"
        << iprefix + "s = "
        << "this->sig;\n";
    if (!ivs.empty()) {
      this->exportResults(out, mb, ivs, iprefix + "d.stateNew");
    }
    out << "} // end of " << iprefix << "exportStateData\n\n";
  }

  //   void LSDYNAInterface::writeChecks(std::ostream& out,
  //                                     const BehaviourDescription& mb,
  //                                     const std::string& t,
  //                                     const Hypothesis h) const {
  //     out << "#ifndef MFRONT_LSDYNA_NORUNTIMECHECKS\n"
  //         << "using BV = " << mb.getClassName()
  //         << "<ModellingHypothesis::" <<
  //         ModellingHypothesis::toUpperCaseString(h)
  //         << "," << t << ",false>;\n"
  //         << "using Traits =
  //         tfel::material::MechanicalBehaviourTraits<BV>;\n"
  //         << "constexpr const unsigned short offset  = "
  //            "(LSDYNATraits<BV>::elasticPropertiesOffset+\n"
  //         << "                                          "
  //            "LSDYNATraits<BV>::thermalExpansionPropertiesOffset);\n"
  //         << "constexpr const unsigned short nprops_  = "
  //            "LSDYNATraits<BV>::material_properties_nb;\n"
  //         << "constexpr const unsigned short NPROPS_  = offset+nprops_;\n"
  //         << "constexpr const unsigned short nstatev_ = "
  //            "Traits::internal_variables_nb;\n"
  //         << "constexpr const unsigned short nfieldv_ = "
  //            "Traits::external_variables_nb2;\n";
  //     if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
  //         (h == ModellingHypothesis::PLANESTRAIN) ||
  //         (h == ModellingHypothesis::PLANESTRESS)) {
  //       out << "if(*ndir+*nshr!=4){\n"
  //           << "std::cerr << \"" << mb.getClassName() << ":"
  //           << " invalid number of components for symmetric tensors "
  //           << "(\" << *ndir+*nshr << \" given, \" << 4 << \"
  //           expected)\\n\";\n"
  //           << "::exit(-1);\n"
  //           << "}\n";
  //     } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
  //       out << "if(*ndir+*nshr!=6){\n"
  //           << "std::cerr << \"" << mb.getClassName() << ":"
  //           << " invalid number of components for symmetric tensors "
  //           << "(\" << *ndir+*nshr << \" given, \" << 6 << \"
  //           expected)\\n\";\n"
  //           << "::exit(-1);\n"
  //           << "}\n";
  //     } else {
  //       tfel::raise(
  //           "LSDYNAInterface::writeChecks: "
  //           "unsupported hypothesis");
  //     }
  //     out << "if(*nprops!=NPROPS_){\n"
  //         << "std::cerr << \"" << mb.getClassName() << ":"
  //         << " unmatched number of material properties "
  //         << "(\" << *nprops << \" given, \" << NPROPS_ << \"
  //         expected)\\n\";\n"
  //         << "::exit(-1);\n"
  //         << "}\n";
  //     const auto o = this->getStateVariablesOffset(mb, h);
  //     if (o == 0u) {
  //       out << "if(*nstatev!=nstatev_){\n";
  //     } else {
  //       out << "if(*nstatev!=nstatev_+" << o << "){\n";
  //     }
  //     out << "std::cerr << \"" << mb.getClassName() << ":"
  //         << " unmatched number of internal state variables ";
  //     if (o == 0u) {
  //       out << "(\" << *nstatev << \" given, \" << nstatev_ << \" "
  //              "expected)\\n\";\n";
  //     } else {
  //       out << "(\" << *nstatev << \" given, \" << nstatev_+" << o
  //           << " << \" expected)\\n\";\n";
  //     }
  //     out << "::exit(-1);\n"
  //         << "}\n"
  //         << "if(*nfieldv!=nfieldv_){\n"
  //         << "std::cerr << \"" << mb.getClassName() << ":"
  //         << " unmatched number of external state variables "
  //         << "(\" << *nfieldv << \" given, \" << nfieldv_ << \"
  //         expected)\\n\";\n"
  //         << "::exit(-1);\n"
  //         << "}\n"
  //         << "#endif /* MFRONT_LSDYNA_NORUNTIMECHECKS */\n";
  //  }  // end of LSDYNAInterface::writeChecks

  std::string LSDYNAInterface::getModellingHypothesisTest(
      const Hypothesis) const {
    return {};
  }  // end of LSDYNAInterface::getModellingHypothesisTest

  void LSDYNAInterface::writeMTestFileGeneratorSetModellingHypothesis(
      std::ostream&) const {
  }  // end of LSDYNAInterface::writeMTestFileGeneratorSetModellingHypothesis

  LSDYNAInterface::~LSDYNAInterface() = default;

}  // end of namespace mfront
