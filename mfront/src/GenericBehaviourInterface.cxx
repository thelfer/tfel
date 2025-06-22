/*!
 * \file   mfront/src/GenericBehaviourInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   30/06/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <fstream>
#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/System/System.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/LibraryDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/GenericBehaviourSymbolsGenerator.hxx"
#include "MFront/GenericBehaviourInterface.hxx"

namespace mfront {

  std::string GenericBehaviourInterface::getName() {
    return "generic";
  }  // end of getName

  std::string GenericBehaviourInterface::getInterfaceName() const {
    return "Generic";
  }  // end of GenericBehaviourInterface::getInterfaceName

  std::pair<bool, GenericBehaviourInterface::tokens_iterator>
  GenericBehaviourInterface::treatKeyword(BehaviourDescription& bd,
                                          const std::string& k,
                                          const std::vector<std::string>& i,
                                          tokens_iterator current,
                                          const tokens_iterator end) {
    using tfel::utilities::CxxTokenizer;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "GenericBehaviourInterface::treatKeyword: " + m);
    };
    if (!i.empty()) {
      if (std::find(i.begin(), i.end(), this->getName()) != i.end()) {
        const auto keys = std::vector<std::string>{
            {"@GenericInterfaceGenerateMTestFileOnFailure",
             "@GenerateMTestFileOnFailure"}};
        throw_if(std::find(keys.begin(), keys.end(), k) == keys.end(),
                 "unsupported key '" + k + "'");
      } else {
        return {false, current};
      }
    }
    if ((k == "@GenericInterfaceGenerateMTestFileOnFailure") ||
        (k == "@GenerateMTestFileOnFailure")) {
      this->setGenerateMTestFileOnFailureAttribute(
          bd, this->readBooleanValue(k, current, end));
      return {true, current};
    }
    return {false, current};
  }  // end of GenericBehaviourInterface::treatKeyword

  std::set<GenericBehaviourInterface::Hypothesis>
  GenericBehaviourInterface::getModellingHypothesesToBeTreated(
      const BehaviourDescription& bd) const {
    return bd.getModellingHypotheses();
  }  // end of GenericBehaviourInterface::getModellingHypothesesToBeTreated

  void GenericBehaviourInterface::writeInterfaceSpecificIncludes(
      std::ostream& os, const BehaviourDescription& bd) const {
    os << "#include \"MFront/GenericBehaviour/State.hxx\"\n"
       << "#include \"MFront/GenericBehaviour/BehaviourData.hxx\"\n";
    if (bd.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      os << "#include \"MFront/GenericBehaviour/ComputeStiffnessTensor.hxx\"\n";
    }
  }  // end of GenericBehaviourInterface::writeInterfaceSpecificIncludes

  void GenericBehaviourInterface::getTargetsDescription(
      TargetsDescription& d, const BehaviourDescription& bd) {
    const auto lib = this->getLibraryName(bd);
    const auto name = bd.getLibrary() + bd.getClassName();
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    insert_if(d[lib].cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
    insert_if(d[lib].include_directories,
              "$(shell " + tfel_config + " --include-path)");
    insert_if(d[lib].sources, name + "-generic.cxx");
    d.headers.push_back("MFront/GenericBehaviour/" + name + "-generic.hxx");
    insert_if(d[lib].link_directories,
              "$(shell " + tfel_config + " --library-path)");
    if (this->shallGenerateMTestFileOnFailure(bd)) {
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
    for (const auto h : this->getModellingHypothesesToBeTreated(bd)) {
      insert_if(d[lib].epts, this->getFunctionNameForHypothesis(name, h));
    }
  }  // end of GenericBehaviourInterface::getTargetsDescription

  void GenericBehaviourInterface::endTreatment(
      const BehaviourDescription& bd, const FileDescription& fd) const {
    auto raise = [](const std::string& m) {
      tfel::raise("GenericBehaviourInterface::endTreatment: " + m);
    };
    tfel::system::systemCall::mkdir("include/MFront");
    tfel::system::systemCall::mkdir("include/MFront/GenericBehaviour");
    const auto mhs = this->getModellingHypothesesToBeTreated(bd);
    const auto name = bd.getLibrary() + bd.getClassName();
    const auto header = name + "-generic.hxx";
    std::ofstream out("include/MFront/GenericBehaviour/" + header);
    if (!out) {
      raise("could not open file '" + header + "'");
    }
    out << "/*!\n"
        << "* \\file   " << header << '\n'
        << "* \\brief  This file declares the umat interface for the "
        << bd.getClassName() << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n";

    const auto hg = this->getHeaderGuard(bd);
    out << "#ifndef " << hg << "\n"
        << "#define " << hg << "\n\n"
        << "#include\"TFEL/Config/TFELConfig.hxx\"\n"
        << "#include\"MFront/GenericBehaviour/BehaviourData.h\"\n\n";

    this->writeVisibilityDefines(out);
    out << "#ifdef __cplusplus\n"
        << "extern \"C\"{\n"
        << "#endif /* __cplusplus */\n\n";

    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out, name);
    this->writeSetParametersFunctionsDeclarations(out, bd, name);

    for (const auto h : mhs) {
      const auto f = this->getFunctionNameForHypothesis(name, h);
      out << "/*!\n"
          << " * \\param[in,out] d: material data\n"
          << " */\n"
          << "MFRONT_SHAREDOBJ int " << f
          << "(MFront_GB_BehaviourData* const);\n\n";
    }

    out << "#ifdef __cplusplus\n"
        << "}\n"
        << "#endif /* __cplusplus */\n\n";
    out << "#endif /* " << hg << " */\n";
    out.close();

    const auto src = name + "-generic.cxx";
    out.open("src/" + src);
    if (!out) {
      raise("could not open file '" + src + "'");
    }

    out << "/*!\n"
        << "* \\file   " << src << '\n'
        << "* \\brief  This file implements the umat interface for the "
        << bd.getClassName() << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n";

    this->writeVisibilityDefines(out);

    out << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n";
    if (((bd.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
         (bd.getBehaviourType() == BehaviourDescription::GENERALBEHAVIOUR)) &&
        (bd.isStrainMeasureDefined())) {
      out << "#include\"TFEL/Math/Stensor/StensorView.hxx\"\n"
          << "#include\"TFEL/Math/T2toST2/T2toST2View.hxx\"\n"
          << "#include\"TFEL/Material/"
             "FiniteStrainBehaviourTangentOperator.hxx\"\n";
    }
    out << "#include\"TFEL/Material/" << bd.getClassName() << ".hxx\"\n";
    if (bd.getAttribute(BehaviourData::profiling, false)) {
      out << "#include\"MFront/BehaviourProfiler.hxx\"\n";
    }
    if (this->shallGenerateMTestFileOnFailure(bd)) {
      if (bd.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        if (bd.isStrainMeasureDefined()) {
          const auto ms = bd.getStrainMeasure();
          if (ms == BehaviourDescription::LINEARISED) {
            out << "#include "
                << "\"MFront/"
                   "GenericBehaviourSmallStrainMTestFileGenerator.hxx\"\n";
          } else {
            out << "#include "
                << "\"MFront/"
                   "GenericBehaviourFiniteStrainMTestFileGenerator.hxx\"\n";
          }
        } else {
          out << "#include "
              << "\"MFront/"
                 "GenericBehaviourSmallStrainMTestFileGenerator.hxx\"\n";
        }
      } else if (bd.getBehaviourType() ==
                 BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        out << "#include "
            << "\"MFront/"
               "GenericBehaviourFiniteStrainMTestFileGenerator.hxx\"\n";
      } else {
        tfel::raise(
            "GenericBehaviourInterface::writeInterfaceSpecificIncludes: "
            "unsupported behaviour type for MTest file generation");
      }
    }
    out << "#include\"MFront/GenericBehaviour/"
           "Integrate.hxx\"\n\n";
    out << "#include\"MFront/GenericBehaviour/" << header << "\"\n\n";

    this->writeGetOutOfBoundsPolicyFunctionImplementation(out, name);

    out << "#ifdef __cplusplus\n"
        << "extern \"C\"{\n"
        << "#endif /* __cplusplus */\n\n";

    GenericBehaviourSymbolsGenerator sg;
    sg.generateGeneralSymbols(out, *this, bd, fd, mhs, name);
    if (!bd.areAllMechanicalDataSpecialised(mhs)) {
      const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      sg.generateSymbols(out, *this, bd, fd, name, uh);
    }
    for (const auto& h : mhs) {
      if (bd.hasSpecialisedMechanicalData(h)) {
        sg.generateSymbols(out, *this, bd, fd, name, h);
      }
    }

    this->writeSetOutOfBoundsPolicyFunctionImplementation(out, name);
    // parameters
    this->writeSetParametersFunctionsImplementations(out, bd, name);
    // behaviour implementations
    const auto type = bd.getBehaviourType();
    for (const auto h : mhs) {
      auto oC = SupportedTypes::TypeSize{};  // Cauchy stress offset
      const auto f = this->getFunctionNameForHypothesis(name, h);
      auto get_ptr = [h](const char* ptr,
                         const SupportedTypes::TypeSize o) -> std::string {
        if (o.isNull()) {
          return ptr;
        }
        return ptr +
               ("+" + std::to_string(o.getValueForModellingHypothesis(h)));
      };
      out << "MFRONT_SHAREDOBJ int " << f
          << "(MFront_GB_BehaviourData* const d){\n"
          << "using namespace tfel::material;\n";
      if (((bd.getBehaviourType() ==
            BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
           (bd.getBehaviourType() == BehaviourDescription::GENERALBEHAVIOUR)) &&
          (bd.isStrainMeasureDefined())) {
        out << "using TangentOperator = "
               "FiniteStrainBehaviourTangentOperatorBase;\n";
      }
      out << "using real = mfront::gb::real;\n"
          << "constexpr const auto h = ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ";\n"
          << "using Behaviour = " << bd.getClassName() << "<h,real,false>;\n";
      if (bd.getAttribute(BehaviourData::profiling, false)) {
        out << "using mfront::BehaviourProfiler;\n"
            << "using tfel::material::" << bd.getClassName() << "Profiler;\n"
            << "BehaviourProfiler::Timer total_timer(" << bd.getClassName()
            << "Profiler::getProfiler(),\n"
            << "BehaviourProfiler::TOTALTIME);\n";
      }
      if (this->shallGenerateMTestFileOnFailure(bd)) {
        out << "using mfront::SupportedTypes;\n";
      }
      // treating strain measures
      if (((bd.getBehaviourType() ==
            BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
           (bd.getBehaviourType() == BehaviourDescription::GENERALBEHAVIOUR)) &&
          (bd.isStrainMeasureDefined())) {
        auto oF = SupportedTypes::TypeSize{};  // deformation gradient offset
        auto oe = SupportedTypes::TypeSize{};  // Hencky strain offset
        auto ocig =
            SupportedTypes::TypeSize{};  // current gradients offset on input
        auto ocog =
            SupportedTypes::TypeSize{};  // current gradients offset on output
        auto octh =
            SupportedTypes::TypeSize{};  // current thermodynamic forces offset
        // determining the size of the new arrays of gradients and thermodynamic
        // forces
        for (const auto& mv : bd.getMainVariables()) {
          ocig +=
              SupportedTypes::getTypeSize(mv.first.type, mv.first.arraySize);
          octh +=
              SupportedTypes::getTypeSize(mv.second.type, mv.second.arraySize);
        }
        out << "mfront::gb::real gradients0["
            << ocig.getValueForModellingHypothesis(h) << "];\n";
        out << "mfront::gb::real gradients1["
            << ocig.getValueForModellingHypothesis(h) << "];\n";
        out << "mfront::gb::real thermodynamic_forces0["
            << octh.getValueForModellingHypothesis(h) << "];\n";
        out << "mfront::gb::real thermodynamic_forces1["
            << octh.getValueForModellingHypothesis(h) << "];\n";
        out << "mfront::gb::real K["
            << octh.getValueForModellingHypothesis(h) *
                   ocig.getValueForModellingHypothesis(h)
            << "];\n";
        // copying
        ocig = SupportedTypes::TypeSize{};
        octh = SupportedTypes::TypeSize{};
        for (const auto& mv : bd.getMainVariables()) {
          if (mv.first.name == "eto") {
            oe = ocig;
            oF = ocig;
            oC = octh;
            ocig +=
                SupportedTypes::getTypeSize(mv.first.type, mv.first.arraySize);
            ocog += SupportedTypes::getTypeSize("strain", 1);
          } else {
            const auto ng =
                SupportedTypes::getTypeSize(mv.first.type, mv.first.arraySize)
                    .getValueForModellingHypothesis(h);
            const auto nth =
                SupportedTypes::getTypeSize(mv.second.type, mv.second.arraySize)
                    .getValueForModellingHypothesis(h);
            out << "tfel::fsalgo::copy<" << ng << ">::exe("
                << "d->s0.gradients[" << ocig << "],"
                << "gradients0[" << ocog << "]);\n";
            out << "tfel::fsalgo::copy<" << ng << ">::exe("
                << "d->s1.gradients[" << ocig << "],"
                << "gradients1[" << ocog << "]);\n";
            out << "tfel::fsalgo::copy<" << nth << ">::exe("
                << "d->s0.thermodynamic_forces[" << ocig << "],"
                << "thermodynamic_forces0[" << ocog << "]);\n";
            out << "tfel::fsalgo::copy<" << nth << ">::exe("
                << "d->s1.thermodynamic_forces[" << ocig << "],"
                << "thermodynamic_forces1[" << ocog << "]);\n";
            ocig += SupportedTypes::getTypeSize("DeformationGradientTensor", 1);
            ocog += SupportedTypes::getTypeSize("strain", 1);
          }
          octh +=
              SupportedTypes::getTypeSize(mv.second.type, mv.second.arraySize);
        }
        // now converting the deformation gradient
        const auto ms = bd.getStrainMeasure();
        const auto N = tfel::material::getSpaceDimension(h);
        if ((ms == BehaviourDescription::GREENLAGRANGE) ||
            (ms == BehaviourDescription::HENCKY)) {
          const auto fs =
              SupportedTypes::getTypeSize("DeformationGradientTensor", 1)
                  .getValueForModellingHypothesis(h);
          const auto ss = SupportedTypes::getTypeSize("StressStensor", 1)
                              .getValueForModellingHypothesis(h);
          out << "tfel::math::tensor<" << N << ",real> F0;\n";
          out << "tfel::math::tensor<" << N << ",real> F1;\n";
          out << "tfel::math::stensor<" << N << ",real> s0;\n";
          if (oF.getValueForModellingHypothesis(h) == 0) {
            out << "tfel::fsalgo::copy<" << fs << ">::exe(d->s0.gradients"
                << ",F0.begin());\n";
            out << "tfel::fsalgo::copy<" << fs << ">::exe(d->s1.gradients"
                << ",F1.begin());\n";
          } else {
            out << "tfel::fsalgo::copy<" << fs << ">::exe(d->s0.gradients+"
                << oF.getValueForModellingHypothesis(h) << ",F0.begin());\n";
            out << "tfel::fsalgo::copy<" << fs << ">::exe(d->s1.gradients+"
                << oF.getValueForModellingHypothesis(h) << ",F1.begin());\n";
          }
          if (oC.getValueForModellingHypothesis(h) == 0) {
            out << "tfel::fsalgo::copy<" << ss
                << ">::exe(d->s0.thermodynamic_forces"
                << ",s0.begin());\n";
          } else {
            out << "tfel::fsalgo::copy<" << ss
                << ">::exe(d->s0.thermodynamic_forces+"
                << oC.getValueForModellingHypothesis(h) << ",s0.begin());\n";
          }
          if (ms == BehaviourDescription::GREENLAGRANGE) {
            out << "tfel::math::StensorView<" << N << ",real> g0_view("
                << get_ptr("gradients0", oe) << ");\n"
                << "g0_view = tfel::math::computeGreenLagrangeTensor(F0);\n"
                << "tfel::math::StensorView<" << N << ",real> g1_view("
                << get_ptr("gradients1", oe) << ");\n"
                << "g1_view = tfel::math::computeGreenLagrangeTensor(F1);\n";
            if ((h == ModellingHypothesis::PLANESTRESS) ||
                (h ==
                 ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
              const auto as =
                  this->checkIfAxialStrainIsDefinedAndGetItsOffset(bd);
              out << "const auto F0zz = "
                     "std::sqrt(1+2*(d->s0.internal_state_variables["
                  << as.second.getValueForModellingHypothesis(h) << "]));\n";
              if (!as.first) {
                raise("the axial strain is not defined");
              }
              if (h == ModellingHypothesis::PLANESTRESS) {
                out << "F0[2] += F0zz;\n";
              } else {
                out << "F0[1] += F0zz;\n";
              }
            }
            out << "tfel::math::StensorView<" << N << ",real> s0_view("
                << get_ptr("thermodynamic_forces0", oC) << ");\n"
                << "s0_view = "
                   "tfel::math::"
                   "convertCauchyStressToSecondPiolaKirchhoffStress("
                   "s0, F0);\n";
          } else {
            out << "LogarithmicStrainHandler<" << N << ",real> lgh0("
                << "LogarithmicStrainHandler<" << N
                << ",real>::EULERIAN,F0);\n";
            out << "LogarithmicStrainHandler<" << N << ",real> lgh1("
                << "LogarithmicStrainHandler<" << N
                << ",real>::EULERIAN,F1);\n";
            out << "tfel::math::StensorView<" << N << ",real> g0_view("
                << get_ptr("gradients0", oe) << ");\n"
                << "g0_view = lgh0.getHenckyLogarithmicStrain();\n";
            out << "tfel::math::StensorView<" << N << ",real> g1_view("
                << get_ptr("gradients1", oe) << ");\n"
                << "g1_view = lgh1.getHenckyLogarithmicStrain();\n";
            if ((h == ModellingHypothesis::PLANESTRESS) ||
                (h ==
                 ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
              const auto as =
                  this->checkIfAxialStrainIsDefinedAndGetItsOffset(bd);
              if (!as.first) {
                raise("the axial strain is not defined");
              }
              out << "lgh0.updateAxialDeformationGradient("
                  << "std::exp(d->s0.internal_state_variables["
                  << as.second.getValueForModellingHypothesis(h) << "]));\n";
            }
            out << "tfel::math::StensorView<" << N << ",real> s0_view("
                << get_ptr("thermodynamic_forces0", oC) << ");\n"
                << "s0_view = lgh0.convertFromCauchyStress(s0);\n";
          }
          out << "auto *const gradients0_old = d->s0.gradients;\n"
              << "auto *const gradients1_old = d->s1.gradients;\n"
              << "auto *const thermodynamic_forces0_old = "
                 "d->s0.thermodynamic_forces;\n"
              << "auto *const thermodynamic_forces1_old = "
                 "d->s1.thermodynamic_forces;\n"
              << "auto *const K_old = d->K;\n"
              << "K[0] = d->K[0];\n"
              << "d->s0.gradients = gradients0;\n"
              << "d->s1.gradients = gradients1;\n"
              << "d->s0.thermodynamic_forces = thermodynamic_forces0;\n"
              << "d->s1.thermodynamic_forces = thermodynamic_forces1;\n"
              << "d->K = K;\n"
              << "const auto bp = K[0]<-0.5;\n"
              << "const auto bk = K[0]>0.5;\n";
        } else {
          raise("unsupported strain measure");
        }
      }
      // behaviour integration
      out << "const auto r = mfront::gb::integrate<Behaviour>(*d,";
      if (type == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        out << "Behaviour::STANDARDTANGENTOPERATOR";
      } else if (type == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        out << "Behaviour::DSIG_DF";
      } else if (type == BehaviourDescription::COHESIVEZONEMODEL) {
        out << "Behaviour::STANDARDTANGENTOPERATOR";
      } else {
        raise("unsupported behaviour type");
      }
      out << ", " << name << "_getOutOfBoundsPolicy());\n";
      // treating strain measures
      if (((bd.getBehaviourType() ==
            BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
           (bd.getBehaviourType() == BehaviourDescription::GENERALBEHAVIOUR)) &&
          (bd.isStrainMeasureDefined())) {
        out << "d->s0.gradients = gradients0_old;\n"
            << "d->s1.gradients = gradients1_old;\n"
            << "d->s0.thermodynamic_forces = thermodynamic_forces0_old;\n"
            << "d->s1.thermodynamic_forces = thermodynamic_forces1_old;\n"
            << "d->K = K_old;\n";
        out << "if(r){\n";
        // post-processing
        const auto ms = bd.getStrainMeasure();
        if (ms == BehaviourDescription::GREENLAGRANGE) {
          const auto N = tfel::material::getSpaceDimension(h);
          const auto s = tfel::material::getStensorSize(h);
          out << "if(bp){\n"
              << "const tfel::math::stensor<" << N << ",real> S0("
              << get_ptr("thermodynamic_forces0", oC) << ");\n"
              << "tfel::math::st2tost2<" << N << ",real> K0;\n"
              << "tfel::fsalgo::copy<" << s * s << ">::exe(K,K0.begin());\n"
              << "tfel::math::T2toST2View<" << N << ",real>(d->K) = "
              << "convert<TangentOperator::DSIG_DF,"
              << "TangentOperator::DS_DEGL>(K0,F0,F0,S0);\n"
              << "} else {\n";
          if ((h == ModellingHypothesis::PLANESTRESS) ||
              (h ==
               ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
            const auto as =
                this->checkIfAxialStrainIsDefinedAndGetItsOffset(bd);
            if (!as.first) {
              raise("the axial strain is not defined");
            }
            out << "const auto F1zz = "
                   "std::sqrt(1+2*(d->s1.internal_state_variables["
                << as.second.getValueForModellingHypothesis(h) << "]));\n";
            if (h == ModellingHypothesis::PLANESTRESS) {
              out << "F1[2] += F1zz;\n";
            } else {
              out << "F1[1] += F1zz;\n";
            }
          }
          out << "const tfel::math::stensor<" << N << ",real> S1("
              << get_ptr("thermodynamic_forces1", oC) << ");\n"
              << "tfel::math::StensorView<" << N << ",real> s1_view("
              << get_ptr("d->s1.thermodynamic_forces", oC) << ");\n"
              << "s1_view = "
                 "tfel::math::convertSecondPiolaKirchhoffStressToCauchyStress("
                 "S1,F1);\n"
              << "if(bk){\n"
              << "tfel::math::st2tost2<" << N << ",real> K1;\n"
              << "tfel::fsalgo::copy<" << s * s << ">::exe(K,K1.begin());\n"
              << "tfel::math::T2toST2View<" << N << ",real>(d->K) = "
              << "convert<TangentOperator::DSIG_DF,"
              << "TangentOperator::DS_DEGL>(K1,F0,F1,S1);\n"
              << "}\n"
              << "}\n";
        } else if (ms == BehaviourDescription::HENCKY) {
          const auto N = tfel::material::getSpaceDimension(h);
          const auto s = tfel::material::getStensorSize(h);
          out << "if(bp){\n"
              << "const tfel::math::stensor<" << N << ",real> T0("
              << get_ptr("thermodynamic_forces0", oC) << ");\n"
              << "tfel::math::st2tost2<" << N << ",real> K0;\n"
              << "tfel::fsalgo::copy<" << s * s << ">::exe(K,K0.begin());\n"
              << "const auto Cs = lgh0.convertToSpatialTangentModuli(K0,T0);\n"
              << "const auto Dt = convert<TangentOperator::DTAU_DF,"
              << "TangentOperator::SPATIAL_MODULI>(Cs,F0,F0,s0);\n"
              << "tfel::math::T2toST2View<" << N << ",real>(d->K) = "
              << "convert<TangentOperator::DSIG_DF,"
              << "        TangentOperator::DTAU_DF>(Dt,F0,F0,s0);\n"
              << "} else {\n";
          if ((h == ModellingHypothesis::PLANESTRESS) ||
              (h ==
               ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
            const auto as =
                this->checkIfAxialStrainIsDefinedAndGetItsOffset(bd);
            if (!as.first) {
              raise("the axial strain is not defined");
            }
            out << "lgh1.updateAxialDeformationGradient("
                << "std::exp(d->s1.internal_state_variables["
                << as.second.getValueForModellingHypothesis(h) << "]));\n";
          }
          out << "const tfel::math::stensor<" << N << ",real> T1("
              << get_ptr("thermodynamic_forces1", oC) << ");\n"
              << "const auto s1 = lgh1.convertToCauchyStress(T1);\n"
              << "tfel::math::StensorView<" << N << ",real> s1_view("
              << get_ptr("d->s1.thermodynamic_forces", oC) << ");\n"
              << "s1_view = s1;\n"
              << "if(bk){\n"
              << "tfel::math::st2tost2<" << N << ",real> K1;\n"
              << "tfel::fsalgo::copy<" << s * s << ">::exe(K,K1.begin());\n"
              << "const auto Cs = lgh1.convertToSpatialTangentModuli(K1,T1);\n"
              << "const auto Dt = convert<TangentOperator::DTAU_DF,"
              << "                        "
                 "TangentOperator::SPATIAL_MODULI>(Cs,F0,F1,s1);\n"
              << "tfel::math::T2toST2View<" << N << ",real>(d->K) = "
              << "convert<TangentOperator::DSIG_DF,"
              << "        TangentOperator::DTAU_DF>(Dt,F0,F1,s1);\n"
              << "}\n"
              << "}\n";
        } else {
          raise("unsupported strain measure");
        }
        out << "}\n";
      }
      if (this->shallGenerateMTestFileOnFailure(bd)) {
        out << "if(r!=1){\n";
        this->generateMTestFile(out, bd, h);
        out << "}\n";
      }
      out << "return r;\n"
          << "} // end of " << f << "\n\n";
    }
    out << "#ifdef __cplusplus\n"
        << "}\n"
        << "#endif /* __cplusplus */\n\n";
    out.close();
  }  // end of GenericBehaviourInterface::endTreatment

  void GenericBehaviourInterface::generateMTestFile(
      std::ostream& out,
      const BehaviourDescription& bd,
      const Hypothesis h) const {
    const auto& d = bd.getBehaviourData(h);
    const auto& persistentVarsHolder = d.getPersistentVariables();
    const auto& externalStateVarsHolder = d.getExternalStateVariables();
    const auto mprops = this->buildMaterialPropertiesList(bd, h);
    const auto fs = [&bd] {
      if (bd.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        if (bd.isStrainMeasureDefined()) {
          const auto ms = bd.getStrainMeasure();
          return ms != BehaviourDescription::LINEARISED;
        } else {
          return false;
        }
      } else if (bd.getBehaviourType() !=
                 BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        tfel::raise(
            "GenericBehaviourInterface::writeInterfaceSpecificIncludes: "
            "unsupported behaviour type for MTest file generation");
      }
      return true;
    }();
    unsigned int offset = 0;
    const auto name = bd.getLibrary() + bd.getClassName();
    out << "try{\n";
    if (fs) {
      out << "mfront::GenericBehaviourFiniteStrainMTestFileGenerator mg(\""
          << this->getLibraryName(bd) << "\",\"" << name << "\");\n";
    } else {
      out << "mfront::GenericBehaviourSmallStrainMTestFileGenerator mg(\""
          << this->getLibraryName(bd) << "\",\"" << name << "\");\n";
    }
    out << "mg.setModellingHypothesis(tfel::material::ModellingHypothesis:"
           ":"
        << tfel::material::ModellingHypothesis::toUpperCaseString(h) << ");\n"
        << "// must be declared after setting the hypothesis\n"
        << "const auto TVectorSize = mg.getTVectorSize();\n"
        << "const auto StensorSize = mg.getStensorSize();\n"
        << "const auto TensorSize  = mg.getTensorSize();\n"
        << "const auto dt = std::max(d->dt,1.e-50);\n"
        << "mg.setHandleThermalExpansion(false);\n"
        << "mg.addTime(0.);\n"
        << "mg.addTime(dt);\n";
    if (fs) {
      out << "mg.setDeformationGradientTensorAtTheBeginningOfTheTimeStep(d->"
             "s0.gradients);\n"
          << "mg.setDeformationGradientTensorAtTheEndOfTheTimeStep(d->s1."
             "gradients);\n"
          << "mg.setStressTensor(d->s0.thermodynamic_forces);\n";
    } else {
      out << "mg.setStrainTensorAtTheBeginningOfTheTimeStep(d->"
             "s0.gradients);\n"
          << "mg.setStrainTensorAtTheEndOfTheTimeStep(d->s1."
             "gradients);\n"
          << "mg.setStressTensor(d->s0.thermodynamic_forces);\n";
    }
    for (const auto& m : mprops.first) {
      const auto flag = SupportedTypes::getTypeFlag(m.type);
      tfel::raise_if(flag != SupportedTypes::Scalar,
                     "GenericBehaviourInterface::generateMTestFile: "
                     "unsupported external state variable type "
                     "in mtest file generation");
      if (m.arraySize == 1u) {
        if (offset == 0) {
          out << "mg.addMaterialProperty(\"" << m.name
              << "\",*(d->s1.material_properties));\n";
        } else {
          out << "mg.addMaterialProperty(\"" << m.name
              << "\",*(d->s1.material_properties+" << offset << "));\n";
        }
        ++offset;
      } else {
        for (unsigned short s = 0; s != m.arraySize; ++s, ++offset) {
          if (offset == 0) {
            out << "mg.addMaterialProperty(\"" << m.name << "[" << s
                << "]\",*(d->s1.material_properties));\n";
          } else {
            out << "mg.addMaterialProperty(\"" << m.name << "[" << s << "]\","
                << "*(d->s1.material_properties+" << offset << "));\n";
          }
        }
      }
    }
    SupportedTypes::TypeSize ivoffset;
    for (const auto& v : persistentVarsHolder) {
      auto flag = SupportedTypes::getTypeFlag(v.type);
      const auto& ivname = d.getExternalName(v.name);
      if (v.arraySize == 1u) {
        if (flag == SupportedTypes::Scalar) {
          out << "mg.addInternalStateVariable(\"" << ivname
              << "\",SupportedTypes::Scalar,&(d->s0.internal_state_variables["
              << ivoffset << "]));\n";
          ivoffset += SupportedTypes::TypeSize(1u, 0u, 0u, 0u);
        } else {
          out << "mg.addInternalStateVariable(\"" << ivname
              << "\",SupportedTypes::Stensor,&(d->s0.internal_state_variables["
              << ivoffset << "]));\n";
          ivoffset += SupportedTypes::TypeSize(0u, 0u, 1u, 0u);
        }
      } else {
        if (v.arraySize >= SupportedTypes::ArraySizeLimit) {
          out << "for(unsigned short i=0;i!=" << v.arraySize << ";++i){\n";
          out << "auto name =  \"" << ivname
              << "[\" + std::to_string(i)+ \"]\";\n";
          if (flag == SupportedTypes::Scalar) {
            out << "mg.addInternalStateVariable(name,SupportedTypes::Scalar,&"
                   "d->s0.internal_state_variables["
                << ivoffset << "]+i);\n";
          } else {
            out << "mg.addInternalStateVariable(name,SupportedTypes::Stensor,"
                   "&"
                   "d->s0.internal_state_variables["
                << ivoffset << "]+i);\n";
          }
          out << "}\n";
          if (flag == SupportedTypes::Scalar) {
            ivoffset += SupportedTypes::TypeSize(v.arraySize, 0u, 0u, 0u);
          } else {
            ivoffset += SupportedTypes::TypeSize(0u, 0u, v.arraySize, 0u);
          }
        } else {
          for (unsigned short i = 0; i != v.arraySize; ++i) {
            if (flag == SupportedTypes::Scalar) {
              out << "mg.addInternalStateVariable(\"" << ivname << "[" << i
                  << "]\",SupportedTypes::Scalar,&(d->s0.internal_state_"
                     "variables["
                  << ivoffset << "]));\n";
              ivoffset += SupportedTypes::TypeSize(1u, 0u, 0u, 0u);
            } else {
              out << "mg.addInternalStateVariable(\"" << ivname << "[" << i
                  << "]\",SupportedTypes::Stensor,&(d->s0.internal_state_"
                     "variables["
                  << ivoffset << "]));\n";
              ivoffset += SupportedTypes::TypeSize(0u, 0u, 1u, 0u);
            }
          }
        }
      }
    }
    auto p = externalStateVarsHolder.begin();
    for (offset = 0; p != externalStateVarsHolder.end(); ++p) {
      auto flag = SupportedTypes::getTypeFlag(p->type);
      tfel::raise_if(flag != SupportedTypes::Scalar,
                     "GenericBehaviourInterface::generateMTestFile: "
                     "unsupported external state variable type "
                     "in mtest file generation");
      const auto& evname = d.getExternalName(p->name);
      if (p->arraySize == 1u) {
        if (offset == 0) {
          out << "mg.addExternalStateVariableValue(\"" << evname
              << "\",0,*(d->s0.external_state_variables));\n"
              << "mg.addExternalStateVariableValue(\"" << evname
              << "\",dt,*(d->s1.external_state_variables));\n";
        } else {
          out << "mg.addExternalStateVariableValue(\"" << evname
              << "\",0,*(d->s0.external_state_variables+" << offset << "));\n"
              << "mg.addExternalStateVariableValue(\"" << evname
              << "\",dt,*(d->s1.external_state_variables+" << offset << "));\n";
        }
        ++offset;
      } else {
        if (p->arraySize >= SupportedTypes::ArraySizeLimit) {
          out << "for(unsigned short i=0;i!=" << p->arraySize << ";++i){\n";
          out << "auto name = \"" << evname
              << "[\" +std::to_string(i)+\"]\";\n";
          if (offset == 0) {
            out << "mg.addExternalStateVariableValue(name,0,*(d->s0.external_"
                   "state_variables+i));\n"
                   "mg.addExternalStateVariableValue(name,dt,*(d->s1."
                   "external_state_variables+i));\n";
          } else {
            out << "mg.addExternalStateVariableValue(name,"
                   "0,*(d->s0.external_state_variables+"
                << offset << "+i));\n";
            out << "mg.addExternalStateVariableValue(name,"
                   "dt,*(d->s1.external_state_variables+"
                << offset << "+i));\n";
          }
          out << "}\n";
          offset += p->arraySize;
        } else {
          for (unsigned short i = 0; i != p->arraySize; ++i, ++offset) {
            if (offset == 0) {
              out << "mg.addExternalStateVariableValue(\"" << evname << "[" << i
                  << "]\",0,*(d->s0.external_state_variables));\n"
                  << "mg.addExternalStateVariableValue(\"" << evname << "[" << i
                  << "]\",dt,*(d->s1.external_state_variables));\n";
            } else {
              out << "mg.addExternalStateVariableValue(\"" << evname << "[" << i
                  << "]\","
                     "0,*(d->s0.external_state_variables+"
                  << offset << "));\n"
                  << "mg.addExternalStateVariableValue(\"" << evname << "[" << i
                  << "]\","
                     "dt,*(d->s1.external_state_variables+"
                  << offset << "))\n";
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
  }  // end of GenericBehaviourInterface::generateMTestFile

  std::string GenericBehaviourInterface::getLibraryName(
      const BehaviourDescription& bd) const {
    if (bd.getLibrary().empty()) {
      if (!bd.getMaterialName().empty()) {
        return bd.getMaterialName() + "-generic";
      } else {
        return "Behaviour";
      }
    }
    return bd.getLibrary() + "-generic";
  }  // end of GenericBehaviourInterface::getLibraryName

  std::string GenericBehaviourInterface::getFunctionNameBasis(
      const std::string& n) const {
    return n;
  }  // end of GenericBehaviourInterface::getFunctionName

  std::string GenericBehaviourInterface::getFunctionNameForHypothesis(
      const std::string& n, const Hypothesis h) const {
    return n + "_" + ModellingHypothesis::toString(h);
  }  // end of GenericBehaviourInterface::getFunctionNameForHypothesis

  void GenericBehaviourInterface::writeBehaviourConstructorHeader(
      std::ostream& os,
      const BehaviourDescription& bd,
      const Hypothesis,
      const std::string& initStateVarsIncrements) const {
    os << "/*\n"
       << " * \\brief constructor for the " << this->getInterfaceName()
       << " interface\n"
       << " * \\param[in] mgb_d: behaviour data\n"
       << " */\n"
       << bd.getClassName() << "(const mfront::gb::BehaviourData& mgb_d)\n";
    if (bd.useQt()) {
      os << ": " << bd.getClassName()
         << "BehaviourData<hypothesis,Type,use_qt>(mgb_d),\n"
         << bd.getClassName()
         << "IntegrationData<hypothesis,Type,use_qt>(mgb_d)";
    } else {
      os << ": " << bd.getClassName()
         << "BehaviourData<hypothesis,Type,false>(mgb_d),\n"
         << bd.getClassName()
         << "IntegrationData<hypothesis,Type,false>(mgb_d)";
    }
    if (!initStateVarsIncrements.empty()) {
      os << ",\n" << initStateVarsIncrements;
    }
  }  // end of GenericBehaviourInterface::writeBehaviourConstructorHeader

  void GenericBehaviourInterface::writeBehaviourConstructorBody(
      std::ostream& os,
      const BehaviourDescription& bd,
      const Hypothesis h) const {
    auto throw_if = [](const bool b, const char* msg) {
      if (b) {
        tfel::raise(
            "GenericBehaviourInterface::writeBehaviourConstructorBody: " +
            std::string(msg));
      }
    };
    // setting driving variables and thermodynamic forces
    auto vsize = [&throw_if](const SupportedTypes::TypeFlag f) {
      if (f == SupportedTypes::TVector) {
        return "TVectorSize";
      } else if (f == SupportedTypes::Stensor) {
        return "StensorSize";
      } else if (f != SupportedTypes::Tensor) {
        throw_if(true, "invalid variable type");
      }
      return "TensorSize";
    };
    auto init = [&os, &vsize, this](
                    SupportedTypes::TypeSize& o, const VariableDescription& v,
                    const std::string& n, const std::string& src) {
      const auto f = this->getTypeFlag(v.type);
      if (f == SupportedTypes::Scalar) {
        os << "this->" << n << " = mgb_d." << src << "[" << o << "];\n";
      } else {
        const auto s = vsize(f);
        if (!o.isNull()) {
          os << "tfel::fsalgo::copy<" << s << ">::exe(mgb_d." << src << "+" << o
             << ","
             << "this->" << n << ".begin());\n";
        } else {
          os << "tfel::fsalgo::copy<" << s << " >::exe(mgb_d." << src
             << ",this->" << n << ".begin());\n";
        }
      }
    };  // end of init
    auto odv = SupportedTypes::TypeSize{};
    auto oth = SupportedTypes::TypeSize{};
    for (const auto& mv : bd.getMainVariables()) {
      const auto& dv = mv.first;
      const auto& th = mv.second;
      throw_if(dv.arraySize != 1, "arrays of gradients are not supported");
      throw_if(th.arraySize != 1,
               "arrays of thermodynamic forces are not supported");
      // driving variable
      const auto dvname = dv.increment_known ? dv.name : dv.name + "0";
      init(odv, dv, dvname, "s0.gradients");
      if (!dv.increment_known) {
        const auto f = this->getTypeFlag(dv.type);
        if (f == SupportedTypes::Scalar) {
          os << "this->" << dv.name << "1 = mgb_d.s1.gradients[" << odv
             << "];\n";
        } else {
          const auto s = vsize(f);
          if (!odv.isNull()) {
            os << "tfel::fsalgo::copy<" << s << ">::exe("
               << "mgb_d.s1.gradients+" << odv << ","
               << "this->" << dv.name << "1.begin());\n";
          } else {
            os << "tfel::fsalgo::copy<" << s << " >::exe("
               << "mgb_d.s1.gradients,"
               << "this->" << dv.name << "1.begin());\n";
          }
        }
      } else {
        const auto f = this->getTypeFlag(dv.type);
        if (f == SupportedTypes::Scalar) {
          os << "this->d" << dv.name << " = "
             << "mgb_d.s1.gradients[" << odv << "] - "
             << "mgb_d.s0.gradients[" << odv << "];\n";
        } else {
          const auto s = vsize(f);
          if (!odv.isNull()) {
            os << "tfel::fsalgo::transform<" << s << ">::exe("
               << "mgb_d.s1.gradients+" << odv << ","
               << "mgb_d.s0.gradients+" << odv << ","
               << "this->d" << dv.name << ".begin(),std::minus<real>());\n";
          } else {
            os << "tfel::fsalgo::transform<" << s << ">::exe("
               << "mgb_d.s1.gradients,"
               << "mgb_d.s0.gradients,"
               << "this->d" << dv.name << ".begin(),std::minus<real>());\n";
          }
        }
      }
      init(oth, th, th.name, "s0.thermodynamic_forces");
      odv += SupportedTypes::getTypeSize(dv.type, dv.arraySize);
      oth += SupportedTypes::getTypeSize(th.type, th.arraySize);
    }
    if (bd.requiresStressFreeExpansionTreatment(h)) {
      os << "std::pair<StressFreeExpansionType,StressFreeExpansionType> "
            "mgb_dl01_l0;\n"
         << "this->computeStressFreeExpansion(mgb_dl01_l0);\n";
      if (bd.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        if (bd.isStrainMeasureDefined()) {
          const auto ms = bd.getStrainMeasure();
          if ((ms == BehaviourDescription::LINEARISED) ||
              (ms == BehaviourDescription::GREENLAGRANGE)) {
            os << "this->eto  -= mgb_dl01_l0.first;\n"
               << "this->deto -= mgb_dl01_l0.second-mgb_dl01_l0.first;\n";
          } else if (ms == BehaviourDescription::HENCKY) {
            os << "this->eto[0]  -= std::log(1+mgb_dl01_l0.first[0]);\n"
               << "this->eto[1]  -= std::log(1+mgb_dl01_l0.first[1]);\n"
               << "this->eto[2]  -= std::log(1+mgb_dl01_l0.first[2]);\n"
               << "this->deto[0] -= "
                  "std::log((1+mgb_dl01_l0.second[0])/"
                  "(1+mgb_dl01_l0.first[0]));\n"
               << "this->deto[1] -= "
                  "std::log((1+mgb_dl01_l0.second[1])/"
                  "(1+mgb_dl01_l0.first[1]));\n"
               << "this->deto[2] -= "
                  "std::log((1+mgb_dl01_l0.second[2])/"
                  "(1+mgb_dl01_l0.first[2]));\n";
          } else {
            throw_if(true, "unsupported finite strain strategy");
          }
        } else {
          os << "this->eto  -= mgb_dl01_l0.first;\n"
             << "this->deto -= mgb_dl01_l0.second-mgb_dl01_l0.first;\n";
        }
      }
    }
  }  // end of GenericBehaviourInterface::writeBehaviourConstructorBody

  void GenericBehaviourInterface::writeBehaviourDataConstructor(
      std::ostream& os,
      const Hypothesis h,
      const BehaviourDescription& bd) const {
    // write variable initializer
    // this is captured for gcc 4.7.2
    auto wvi = [&os, this](bool& first, const VariableDescription& v,
                           const std::string& src,
                           const SupportedTypes::TypeSize& o,
                           const std::string& eo) {
      if (v.arraySize != 1u) {
        return;
      }
      os << (first ? "\n: " : ",\n");
      const auto flag = SupportedTypes::getTypeFlag(v.type);
      if (flag == SupportedTypes::Scalar) {
        if (eo.empty()) {
          os << v.name << "(" << src << "[" << o << "])";
        } else {
          os << v.name << "(" << src << "[" << eo << "+" << o << "])";
        }
      } else if ((flag == SupportedTypes::TVector) ||
                 (flag == SupportedTypes::Stensor) ||
                 (flag == SupportedTypes::Tensor)) {
        if (eo.empty()) {
          os << v.name << "(&" << src << "[" << o << "])";
        } else {
          os << v.name << "(&" << src << "[" << eo << " " << o << "])";
        }
      } else {
        tfel::raise(
            "GenericBehaviourInterface::writeBehaviourDataConstructor: "
            "internal error, tag unsupported");
      }
      first = false;
    };  // end of wvi
    auto wvci = [&wvi, this](bool& first,
                             const VariableDescriptionContainer& vs,
                             const std::string& src, const std::string& eo) {
      auto o = SupportedTypes::TypeSize{};
      for (const auto& v : vs) {
        wvi(first, v, src, o, eo);
        o += this->getTypeSize(v.type, v.arraySize);
      }
    };  // end of wvci
    // write variable initializer
    // this is captured for gcc 4.7.2
    auto wvi2 = [&os, &bd, this](
                    const VariableDescription& v, const std::string& src,
                    const SupportedTypes::TypeSize& c, const std::string& eo) {
      auto get_offset = [&eo](const SupportedTypes::TypeSize o) {
        std::ostringstream out;
        out << o;
        if (!eo.empty()) {
          return eo + '+' + out.str();
        }
        return out.str();
      };
      auto o = c;
      if (v.arraySize == 1u) {
        return;
      }
      const auto flag = SupportedTypes::getTypeFlag(v.type);
      if (bd.useDynamicallyAllocatedVector(v.arraySize)) {
        os << "this->" << v.name << ".resize(" << v.arraySize << ");\n";
        os << "for(unsigned short idx=0;idx!=" << v.arraySize << ";++idx){\n";
        switch (flag) {
          case SupportedTypes::Scalar:
            os << "this->" << v.name << "[idx] = " << src << "["
               << get_offset(o) << "+idx];\n";
            break;
          case SupportedTypes::TVector:
            os << "tfel::fsalgo::copy<TVectorSize>::exe(&" << src << "["
               << get_offset(o) << "+idx*TVectorSize],this->" << v.name
               << "[idx].begin());\n";
            break;
          case SupportedTypes::Stensor:
            os << "this->" << v.name << "[idx].import(&" << src << "["
               << get_offset(o) << "+idx*StensorSize]);\n";
            break;
          case SupportedTypes::Tensor:
            os << "tfel::fsalgo::copy<TensorSize>::exe(&" << src << "["
               << get_offset(o) << "+idx*TensorSize],this->" << v.name
               << "[idx].begin());\n";
            break;
          default:
            tfel::raise(
                "GenericBehaviourInterface::writeBehaviourDataConstructor: "
                "internal error, tag unsupported");
        }
        os << "}\n";
      } else {
        for (int index = 0; index != v.arraySize; ++index) {
          switch (flag) {
            case SupportedTypes::Scalar:
              os << "this->" << v.name << "[" << index << "] = " << src << "["
                 << get_offset(o) << "];\n";
              break;
            case SupportedTypes::TVector:
              os << "tfel::fsalgo::copy<TVectorSize>::exe(&" << src << "["
                 << get_offset(o) << "],this->" << v.name << "[" << index
                 << "].begin());\n";
              break;
            case SupportedTypes::Stensor:
              os << "this->" << v.name << "[" << index << "].import(&" << src
                 << "[" << get_offset(o) << "]);\n";
              break;
            case SupportedTypes::Tensor:
              os << "tfel::fsalgo::copy<TensorSize>::exe(&" << src << "["
                 << get_offset(o) << "],"
                 << "this->" << v.name << "[" << index << "].begin());\n";
              break;
            default:
              tfel::raise(
                  "GenericBehaviourInterface::writeBehaviourDataConstructor: "
                  "internal error, tag unsupported");
          }
          o += SupportedTypes::getTypeSize(v.type, 1u);
        }
      }
    };  // end of wvi2
    auto wvci2 = [&wvi2, this](const VariableDescriptionContainer& vs,
                               const std::string& src, const std::string& eo) {
      auto o = SupportedTypes::TypeSize{};
      for (const auto& v : vs) {
        wvi2(v, src, o, eo);
        o += this->getTypeSize(v.type, v.arraySize);
      }
    };  // end of wvci2
    const auto& d = bd.getBehaviourData(h);
    os << "/*\n"
       << " * \\brief constructor for the " << this->getInterfaceName()
       << " interface\n"
       << " * \\param[in] mgb_d: behaviour data\n"
       << " */\n"
       << bd.getClassName()
       << "BehaviourData(const mfront::gb::BehaviourData& mgb_d)";
    auto first = true;
    const auto mprops = this->buildMaterialPropertiesList(bd, h);
    auto ioffset = mprops.second;
    auto mps_offset = std::string{};
    auto emps_offset = std::string{};
    if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                         false)) ||
        (bd.getAttribute(
            BehaviourDescription::requiresThermalExpansionCoefficientTensor,
            false))) {
      mps_offset =
          "mfront::gb::ThermoElasticMaterialPropertiesNumber<hypothesis,";
      if (bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                          false)) {
        emps_offset = "mfront::gb::ElasticMaterialPropertiesNumber<hypothesis,";
        if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
          mps_offset += "true, true";
          emps_offset += "true";
        } else if (bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
          mps_offset += "true, false";
          emps_offset += "false";
        } else {
          tfel::raise(
              "GenericBehaviourInterface::writeBehaviourDataConstructor:"
              "unsupported elastic symmetry type");
        }
        emps_offset += ">::value";
      } else {
        mps_offset += "false, false";
      }
      if (bd.getAttribute(
              BehaviourDescription::requiresThermalExpansionCoefficientTensor,
              false)) {
        if (bd.getSymmetryType() == mfront::ISOTROPIC) {
          mps_offset += ", true, true";
        } else if (bd.getSymmetryType() == mfront::ORTHOTROPIC) {
          mps_offset += ", true, false";
        } else {
          tfel::raise(
              "GenericBehaviourInterface::writeBehaviourDataConstructor:"
              "unsupported elastic symmetry type");
        }
      } else {
        mps_offset += ", false, false";
      }
      mps_offset += ">::value";
    }
    for (const auto& mp : d.getMaterialProperties()) {
      const auto& m =
          findBehaviourMaterialProperty(mprops.first, mp.getExternalName());
      auto offset = m.offset;
      offset -= ioffset;
      wvi(first, mp, "mgb_d.s1.material_properties", offset, mps_offset);
    }
    wvci(first, d.getPersistentVariables(), "mgb_d.s0.internal_state_variables",
         "");
    wvci(first, d.getExternalStateVariables(),
         "mgb_d.s0.external_state_variables", "");
    os << "\n{\n";
    for (const auto& mp : d.getMaterialProperties()) {
      const auto& m =
          findBehaviourMaterialProperty(mprops.first, mp.getExternalName());
      auto offset = m.offset;
      offset -= ioffset;
      wvi2(mp, "mgb_d.s1.material_properties", offset, mps_offset);
    }
    wvci2(d.getPersistentVariables(), "mgb_d.s0.internal_state_variables", "");
    wvci2(d.getExternalStateVariables(), "mgb_d.s0.external_state_variables",
          "");
    if (bd.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
        const auto stac = [&bd] {
          if (bd.getAttribute(
                  BehaviourDescription::requiresUnAlteredStiffnessTensor,
                  false)) {
            return "StiffnessTensorAlterationCharacteristic::UNALTERED";
          }
          return "StiffnessTensorAlterationCharacteristic::ALTERED";
        }();
        os << "computeIsotropicStiffnessTensor"
           << "<hypothesis," << stac << ",stress,real>(this->D,"
           << "mgb_d.s1.material_properties[0],\n"
           << "mgb_d.s1.material_properties[1]);\n";
      } else if (bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
        if (bd.getAttribute(
                BehaviourDescription::requiresUnAlteredStiffnessTensor,
                false)) {
          os << "mfront::gb::computeOrthotropicUnAlteredElasticStiffnessTensor<"
                "hypothesis>(this->D,"
             << "mgb_d.s1.material_properties);\n";
        } else {
          os << "mfront::gb::computeOrthotropicAlteredElasticStiffnessTensor<"
                "hypothesis>(this->D,"
             << "mgb_d.s1.material_properties);\n";
        }
      } else {
        tfel::raise(
            "GenericBehaviourInterface::writeBehaviourDataConstructor:"
            "unsupported symmetry type");
      }
    }
    if (bd.getAttribute(
            BehaviourDescription::requiresThermalExpansionCoefficientTensor,
            false)) {
      auto eoffset = std::string{};
      if (bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                          false)) {
        eoffset = "mgb_eoffset";
        os << "constexpr const auto mgb_eoffset = " << emps_offset << ";\n";
      }
      if (bd.getSymmetryType() == mfront::ISOTROPIC) {
        if (eoffset.empty()) {
          os << "this->A = mgb_d.s1.material_properties[0] * "
             << "ThermalExpansionCoefficientTensor::Id();\n";
        } else {
          os << "this->A = mgb_d.s1.material_properties[" << eoffset << "] * "
             << "ThermalExpansionCoefficientTensor::Id();\n";
        }
      } else if (bd.getSymmetryType() == mfront::ORTHOTROPIC) {
        os << "this->A = "
              "ThermalExpansionCoefficientTensor(thermalexpansion(0));\n";
        if (eoffset.empty()) {
          os << "this->A[0] = "
                "thermalexpansion(mgb_d.s1.material_properties[0]);\n"
             << "this->A[1] = "
                "thermalexpansion(mgb_d.s1.material_properties[1]);\n"
             << "this->A[2] = "
                "thermalexpansion(mgb_d.s1.material_properties[2]);\n";
        } else {
          os << "this->A[0] = thermalexpansion(mgb_d.s1.material_properties["
             << eoffset << "]);\n"
             << "this->A[1] = thermalexpansion(mgb_d.s1.material_properties["
             << eoffset << "+1]);\n"
             << "this->A[2] = thermalexpansion(mgb_d.s1.material_properties["
             << eoffset << "+2]);\n";
        }
      } else {
        tfel::raise(
            "GenericBehaviourInterface::writeBehaviourDataConstructor:"
            "unsupported symmetry type");
      }
      os << ";\n";
    }
    os << "}\n\n";
  }  // end of GenericBehaviourInterface::writeBehaviourDataConstructor

  void GenericBehaviourInterface::writeIntegrationDataConstructor(
      std::ostream& os,
      const Hypothesis h,
      const BehaviourDescription& bd) const {
    const auto& evs = bd.getBehaviourData(h).getExternalStateVariables();
    os << "/*\n"
       << " * \\brief constructor for the " << this->getInterfaceName()
       << " interface\n"
       << " * \\param[in] mgb_d: behaviour data\n"
       << " */\n"
       << bd.getClassName()
       << "IntegrationData(const mfront::gb::BehaviourData& mgb_d)\n"
       << ": dt(mgb_d.dt)";
    auto o = SupportedTypes::TypeSize{};
    for (const auto& ev : evs) {
      if (ev.arraySize != 1u) {
        o += this->getTypeSize(ev.type, ev.arraySize);
        continue;
      }
      if (SupportedTypes::getTypeFlag(ev.type) != SupportedTypes::Scalar) {
        tfel::raise(
            "GenericBehaviourInterface::writeIntegrationDataConstructor: "
            "internal error, tag unsupported");
      }
      os << ",\nd" << ev.name << "(mgb_d.s1.external_state_variables[" << o
         << "]-"
         << "mgb_d.s0.external_state_variables[" << o << "])";
      o += this->getTypeSize(ev.type, ev.arraySize);
    }
    os << "\n{\n";
    o = SupportedTypes::TypeSize{};
    for (const auto& ev : evs) {
      if (ev.arraySize == 1u) {
        o += this->getTypeSize(ev.type, ev.arraySize);
        continue;
      }
      if (SupportedTypes::getTypeFlag(ev.type) != SupportedTypes::Scalar) {
        tfel::raise(
            "GenericBehaviourInterface::writeIntegrationDataConstructor: "
            "internal error, tag unsupported");
      }
      if (bd.useDynamicallyAllocatedVector(ev.arraySize)) {
        os << "this->d" << ev.name << ".resize(" << ev.arraySize << ");\n";
        os << "for(unsigned short idx=0;idx!=" << ev.arraySize << ";++idx){\n";
        os << "this->d" << ev.name << "[idx] = "
           << "mgb_d.s1.external_state_variables[" << o << "+idx] - "
           << "mgb_d.s0.external_state_variables[" << o << "+idx];\n";
        os << "}\n";
        o += this->getTypeSize(ev.type, ev.arraySize);
      } else {
        for (int index = 0; index != ev.arraySize; ++index) {
          os << "this->d" << ev.name << "[" << index << "] = "
             << "mgb_d.s1.external_state_variables[" << o << "] - "
             << "mgb_d.s0.external_state_variables[" << o << "];\n";
          o += SupportedTypes::getTypeSize(ev.type, 1u);
        }
      }
    }
    os << "}\n\n";
  }  // end of GenericBehaviourInterface::writeIntegrationDataConstructor

  void GenericBehaviourInterface::writeBehaviourDataMainVariablesSetters(
      std::ostream&, const BehaviourDescription&) const {
  }  // end of GenericBehaviourInterface::writeBehaviourDataMainVariablesSetters

  void GenericBehaviourInterface::writeIntegrationDataMainVariablesSetters(
      std::ostream&, const BehaviourDescription&) const {
  }  // end of
     // GenericBehaviourInterface::writeIntegrationDataMainVariablesSetters

  void GenericBehaviourInterface::exportMechanicalData(
      std::ostream& os,
      const Hypothesis h,
      const BehaviourDescription& bd) const {
    auto vsize = [](const SupportedTypes::TypeFlag f) {
      if (f == SupportedTypes::TVector) {
        return "TVectorSize";
      } else if (f == SupportedTypes::Stensor) {
        return "StensorSize";
      } else if (f != SupportedTypes::Tensor) {
        tfel::raise(
            "GenericBehaviourInterface::writeBehaviourDataMainVariablesSetters:"
            " "
            "invalid variable type");
      }
      return "TensorSize";
    };
    auto export_variable = [this, &os, &vsize](
                               const VariableDescription& v,
                               const char* const dest,
                               const SupportedTypes::TypeSize o) {
      const auto f = this->getTypeFlag(v.type);
      if (v.arraySize == 1u) {
        if (f == SupportedTypes::Scalar) {
          os << "mbg_s1." << dest << "[" << o << "] "
             << "= this->" << v.name << ";\n";
        } else {
          const auto s = vsize(f);
          if (o.isNull()) {
            os << "tfel::fsalgo::copy<" << s << ">::exe("
               << "this->" << v.name << ".begin(), "
               << "mbg_s1." << dest << ");\n";
          } else {
            os << "tfel::fsalgo::copy<" << s << ">::exe("
               << "this->" << v.name << ".begin(), "
               << "mbg_s1." << dest << " + " << o << ");\n";
          }
        }
      } else {
        auto c = o;
        for (unsigned short idx = 0; idx != v.arraySize; ++idx) {
          if (f == SupportedTypes::Scalar) {
            os << "mbg_s1." << dest << "[" << c << "] "
               << "= this->" << v.name << "[" << idx << "];\n";
          } else {
            const auto s = vsize(f);
            if (c.isNull()) {
              os << "tfel::fsalgo::copy<" << s << ">::exe("
                 << "this->" << v.name << "[" << idx << "].begin(), "
                 << "mbg_s1." << dest << ");\n";
            } else {
              os << "tfel::fsalgo::copy<" << s << ">::exe("
                 << "this->" << v.name << "[" << idx << "].begin(), "
                 << "mbg_s1." << dest << " + " << c << ");\n";
            }
          }
          c += this->getTypeSize(v.type, 1u);
        }
      }
    };
    const auto& d = bd.getBehaviourData(h);
    os << "void exportStateData(mfront::gb::State& mbg_s1) const\n";
    os << "{\n";
    os << "using namespace tfel::math;\n";
    auto o = SupportedTypes::TypeSize{};
    for (const auto& v : bd.getMainVariables()) {
      const auto& th = v.second;
      if (th.arraySize != 1) {
        tfel::raise(
            "GenericBehaviourInterface::exportMechancialData: "
            "array of thermodynamical forces are not supported");
      }
      export_variable(th, "thermodynamic_forces", o);
      o += SupportedTypes::getTypeSize(th.type, th.arraySize);
    }
    o = SupportedTypes::TypeSize{};
    for (const auto& iv : d.getPersistentVariables()) {
      export_variable(iv, "internal_state_variables", o);
      o += SupportedTypes::getTypeSize(iv.type, iv.arraySize);
    }
    os << "} // end of exportStateData\n\n";
  }  // end of GenericBehaviourInterface::exportMechanicalData

  GenericBehaviourInterface::~GenericBehaviourInterface() = default;

}  // end of namespace mfront
