/*!
 * \file   mfront/src/ComsolInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   30/07/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <fstream>
#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/System/System.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/ComsolInterface.hxx"

namespace mfront {

  std::string ComsolInterface::getName() { return "comsol"; }  // end of getName

  std::string ComsolInterface::getInterfaceName() const {
    return "Comsol";
  }  // end of ComsolInterface::getInterfaceName

  std::pair<bool, ComsolInterface::tokens_iterator>
  ComsolInterface::treatKeyword(BehaviourDescription&,
                                const std::string&,
                                const std::vector<std::string>&,
                                tokens_iterator current,
                                const tokens_iterator) {
    return {false, current};
  }  // end of ComsolInterface::treatKeyword

  std::set<ComsolInterface::Hypothesis>
  ComsolInterface::getModellingHypothesesToBeTreated(
      const BehaviourDescription&) const {
    return {ModellingHypothesis::TRIDIMENSIONAL};
  }  // end of ComsolInterface::getModellingHypothesesToBeTreated

  void ComsolInterface::writeInterfaceSpecificIncludes(
      std::ostream&, const BehaviourDescription&) const {
  }  // end of ComsolInterface::writeInterfaceSpecificIncludes

  void ComsolInterface::getTargetsDescription(TargetsDescription& d,
                                              const BehaviourDescription& bd) {
    const auto lib = this->getLibraryName(bd);
    const auto name = bd.getLibrary() + bd.getClassName();
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    auto& l = d.getLibrary(lib);
    insert_if(l.cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
    insert_if(l.include_directories,
              "$(shell " + tfel_config + " --include-path)");
    insert_if(l.sources, "comsol" + name + ".cxx");
    d.headers.push_back("MFront/Comsol/comsol" + name + ".hxx");
    insert_if(l.link_directories,
              "$(shell " + tfel_config + " --library-path)");
    if (this->shallGenerateMTestFileOnFailure(bd)) {
      insert_if(l.link_libraries,
                tfel::getLibraryInstallName("MTestFileGenerator"));
    }
    insert_if(l.link_libraries, "$(shell " + tfel_config +
                                    " --library-dependency "
                                    "--material --mfront-profiling)");
    for (const auto h : this->getModellingHypothesesToBeTreated(bd)) {
      if (h != ModellingHypothesis::TRIDIMENSIONAL) {
        tfel::raise(
            "ComsolInterface::getTargetsDescription: "
            "unsupported modelling hypothesis");
      }
      insert_if(l.epts, this->getFunctionNameBasis(name));
    }
  }  // end of ComsolInterface::getTargetsDescription

  void ComsolInterface::endTreatment(const BehaviourDescription& bd,
                                     const FileDescription& fd) const {
    using namespace tfel::system;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "ComsolInterface::endTreatment: " + m);
    };
    throw_if(bd.getBehaviourType() !=
                 BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
             "the comsol interface only supports strain based behaviours");
    if (bd.isStrainMeasureDefined()) {
      const auto ms = bd.getStrainMeasure();
      throw_if(!((ms == BehaviourDescription::LINEARISED) ||
                 (ms == BehaviourDescription::GREENLAGRANGE)),
               "the comsol interface only supports the linearised "
               "and Green-Lagrange strain measures");
    }
    throw_if(bd.getSymmetryType() != mfront::ISOTROPIC,
             "the comsol interface only supports isotropic behaviours");
    // the only supported modelling hypothesis
    constexpr auto h = ModellingHypothesis::TRIDIMENSIONAL;
    const auto& d = bd.getBehaviourData(h);
    throw_if(d.getExternalStateVariables().size() != 1u,
             "external state variables are not supported "
             "by comsol interface");
    // get the modelling hypotheses to be treated
    const auto name = bd.getLibrary() + bd.getClassName();
    // output directories
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/Comsol");
    systemCall::mkdir("comsol");

    std::ofstream out;

    // header
    auto fname = "comsol" + name + ".hxx";
    out.open("include/MFront/Comsol/" + fname);
    throw_if(!out, "could not open file '" + fname + "'");

    out << "/*!\n"
        << "* \\file   " << fname << '\n'
        << "* \\brief  This file declares the comsol interface for the "
        << bd.getClassName() << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n";

    const auto header = this->getHeaderGuard(bd);
    out << "#ifndef " << header << "\n"
        << "#define " << header << "\n\n"
        << "#ifdef __cplusplus\n"
        << "#include\"TFEL/Config/TFELConfig.hxx\"\n"
        << "#include\"TFEL/Material/" << bd.getClassName() << ".hxx\"\n"
        << "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);

    out << "#ifdef __cplusplus\n"
        << "extern \"C\"{\n"
        << "#endif /* __cplusplus */\n\n";

    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out, name);
    this->writeSetParametersFunctionsDeclarations(out, bd, name);

    if (!d.getPersistentVariables().empty()) {
      out << "MFRONT_SHAREDOBJ int\n"
          << this->getFunctionNameBasis(name)
          << "(const double * const, // strain\n"
          << "double * const, // stress\n"
          << "double * const, // tangent operator\n"
          << "const int * const, // number of material properties\n"
          << "const double *const, // material properties\n"
          << "const int *const, // number of strain components\n"
          << "double *const, // strain at the beginning of the time step\n"
          << "const int *const, // number of state variables\n"
          << "double *const);// state variables\n\n";
    } else {
      out << "MFRONT_SHAREDOBJ int\n"
          << this->getFunctionNameBasis(name)
          << "(const double * const, // strain\n"
          << "double * const, // stress\n"
          << "double * const, // tangent operator\n"
          << "const int * const, // number of material properties\n"
          << "const double *const, // material properties\n"
          << "const int *const, // number of strain components\n"
          << "double *const); // strain at the beginning of the time step\n\n";
    }

    out << "#ifdef __cplusplus\n"
        << "}\n"
        << "#endif /* __cplusplus */\n\n"
        << "#endif /* " << header << " */\n";

    out.close();

    fname = "comsol" + name + ".cxx";
    out.open("src/" + fname);
    throw_if(!out, "could not open file '" + fname + "'");

    out << "/*!\n"
        << "* \\file   " << fname << '\n'
        << "* \\brief  This file implements the comsol interface for the "
        << bd.getClassName() << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n";

    //     this->getExtraSrcIncludes(out, mb);
    //
    out << "#include\"TFEL/Math/General/MathConstants.hxx\"\n"
        << "#include\"TFEL/Math/stensor.hxx\"\n"
        << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n"
        << "#include\"TFEL/Material/" << bd.getClassName() << ".hxx\"\n";
    if (bd.getAttribute(BehaviourData::profiling, false)) {
      out << "#include\"MFront/BehaviourProfiler.hxx\"\n\n";
    }
    //     out << "#include\"MFront/Comsol/"
    //            "ComsolStressFreeExpansionHandler.hxx\"\n\n"
    //         << "#include\"MFront/Comsol/ComsolInterface.hxx\"\n\n"
    out << "#include\"MFront/Comsol/comsol" << name << ".hxx\"\n\n";
    //
    this->writeGetOutOfBoundsPolicyFunctionImplementation(out, name);

    out << "extern \"C\"{\n\n";
    //
    //     ComsolSymbolsGenerator sg;
    //     sg.generateGeneralSymbols(out, *this, mb, fd, {h}, name);
    //     sg.generateSymbols(out, *this, mb, fd, name, h);
    //
    this->writeSetParametersFunctionsImplementations(out, bd, name);
    this->writeSetOutOfBoundsPolicyFunctionImplementation(out, name);

    if (!d.getPersistentVariables().empty()) {
      out << "MFRONT_SHAREDOBJ int\n"
          << this->getFunctionNameBasis(name)  //
          << "(const double * const e,\n"
          << "double * const s,\n"
          << "double * const D,\n"
          << "const int * const nprops,\n"
          << "const double *const mprops,\n"
          << "const int *const nstran,\n"
          << "double *const stran,\n"
          << "const int *const nstatv,\n"
          << "double *const statev){\n";
    } else {
      out << "MFRONT_SHAREDOBJ int\n"
          << this->getFunctionNameBasis(name)  //
          << "(const double * const e,\n"
          << "double * const s,\n"
          << "double * const D,\n"
          << "const int * const nprops,\n"
          << "const double *const mprops,\n"
          << "const int *const nstran,\n"
          << "double *const stran){\n";
    }
    out << "using tfel::material::ModellingHypothesis;\n"
        << "using Behaviour = tfel::material::" << bd.getClassName()
        << "<ModellingHypothesis::TRIDIMENSIONAL, double, "
           "false>;\n";
    if (bd.requiresStressFreeExpansionTreatment(h)) {
      out << "using StressFreeExpansionType = "
             "Behaviour::StressFreeExpansionType;\n";
    }
    out << "constexpr auto cste = tfel::math::Cste<double>::sqrt2;\n";
    // inputs checks
    const auto nprops =
        d.getMaterialProperties().getTypeSize().getValueForDimension(3) + 1;
    out << "if (*nprops != " << nprops << " ){\n"
        << "return 1;\n"
        << "}\n"
        << "if (*nstran!= 6){\n"
        << "return 2;\n"
        << "}\n";
    if (!d.getPersistentVariables().empty()) {
      const auto nstatv =
          d.getPersistentVariables().getTypeSize().getValueForDimension(3);
      out << "if (*nstatv!= " << nstatv << " ){\n"
          << "return 2;\n"
          << "}\n";
    }
    out << "auto rdt = double{1};\n"
        << "try{\n"
        << "const auto p = " << this->getFunctionNameBasis(name)
        << "_getOutOfBoundsPolicy();\n"
        << "const auto dt = double{0};\n"
        << "const auto T  = mprops[" << nprops << "];\n"
        << "const tfel::math::stensor<3u,double> e0(stran);\n"
        << "tfel::math::stensor<3u,double> e1(e);\n"
        << "tfel::math::stensor<3u,double> eto;\n"
        << "tfel::math::stensor<3u,double> deto;\n";
    if (!d.getPersistentVariables().empty()) {
      out << "Behaviour b(&dt, &T, mprops, statev);\n";
    } else {
      out << "Behaviour b(&dt, &T, mprops, nullptr);\n";
    }
    out << "// convert e1 in TFEL conventions\n"
        << "e1[3] *= cste;\n"
        << "e1[4] *= cste;\n"
        << "e1[5] *= cste;\n"
        << "std::swap(e1[3],e1[5]);\n";
    if (bd.requiresStressFreeExpansionTreatment(h)) {
      out << "// stress free expansion\n"
          << "auto s = StressFreeExpansionType{};\n"
          << "b.computeStressFreeExpansion(s);\n"
          << "const auto& s0 = s.first;\n"
          << "const auto& s1 = s.second;\n"
          << "eto  = e0-s0;\n"
          << "deto = e1-e0-(s1-s0);\n";
    } else {
      out << "eto  = e0;\n"
          << "deto = e1-e0;\n";
    }
    out << "// to Voigt conventions\n"
        << "eto[3] *= cste;\n"
        << "eto[4] *= cste;\n"
        << "eto[5] *= cste;\n"
        << "deto[3] *= cste;\n"
        << "deto[4] *= cste;\n"
        << "deto[5] *= cste;\n"
        << "b.setCOMSOLBehaviourDataGradients(eto.begin());\n"
        << "b.setCOMSOLIntegrationDataGradients(deto.begin());\n"
        << "b.setOutOfBoundsPolicy(p);\n"
        << "b.initialize();\n"
        << "b.checkBounds();\n"
        << "const auto smflag = Behaviour::STANDARDTANGENTOPERATOR;\n"
        << "const auto smt = Behaviour::CONSISTENTTANGENTOPERATOR;\n"
        << "auto tsf = b.computeAPrioriTimeStepScalingFactor(rdt);\n"
        << "rdt = tsf.second;\n"
        << "if (!tsf.first) {\n"
        << "  return -1;\n"
        << "}\n"
        << "const auto r = b.integrate(smflag, smt);\n"
        << "if (r == Behaviour::FAILURE) {\n"
        << "  return -1;\n"
        << "}\n"
        << "const auto atsf = "
           "b.computeAPosterioriTimeStepScalingFactor(rdt);\n"
        << "if (!atsf.first) {\n"
        << "  return -1;\n"
        << "}\n"
        << "rdt = std::min(atsf.second, rdt);\n";
    if (!d.getPersistentVariables().empty()) {
      out << "b.COMSOLexportStateData(s,statev);\n";
    } else {
      out << "b.COMSOLexportStateData(s,nullptr);\n";
    }
    out << "std::swap(s[3],s[5]);\n"
        << "const auto& K = b.getTangentOperator();\n"
        << "std::copy(K.begin(),K.end(), D);\n"
        << "tfel::fsalgo::copy<6>::exe(e1.begin(),stran);\n"
        << "} catch(...){\n"
        << "return -1;\n"
        << "}\n"
        << "return rdt < 0.99 ? -1 : 0;\n"
        << "} // end of " << this->getFunctionNameBasis(name) << "\n\n";

    out << "} // end of extern \"C\"\n";
    out.close();
  }  // end of ComsolInterface::endTreatment

  bool ComsolInterface::areExternalStateVariablesSupported() const {
    return false;
  }  // end of ComsolInterface::areExternalStateVariablesSupported()

  bool ComsolInterface::isTemperatureIncrementSupported() const {
    return false;
  }  // end of ComsolInterface::isTemperatureIncrementSupported()

  std::string ComsolInterface::getLibraryName(
      const BehaviourDescription& bd) const {
    if (bd.getLibrary().empty()) {
      if (!bd.getMaterialName().empty()) {
        return this->getInterfaceName() + bd.getMaterialName();
      } else {
        return this->getInterfaceName() + "Behaviour";
      }
    }
    return this->getInterfaceName() + bd.getLibrary();
  }  // end of ComsolInterface::getLibraryName

  std::string ComsolInterface::getFunctionNameBasis(
      const std::string& n) const {
    return n;
  }  // end of ComsolInterface::getFunctionName

  void ComsolInterface::writeMTestFileGeneratorSetModellingHypothesis(
      std::ostream& out) const {
    out << "mg.setModellingHypothesis(ModellingHypothesis::TRIDIMENSIONAL);\n";
  }  // end of ComsolInterface::writeMTestFileGeneratorSetModellingHypothesis

  ComsolInterface::~ComsolInterface() = default;

}  // end of namespace mfront
