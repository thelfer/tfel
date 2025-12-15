/*!
 * \file   mfront/src/BehaviourCodeGeneratorBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   03/08/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include <fstream>
#include <sstream>
#include "TFEL/Raise.hxx"
#include "TFEL/UnicodeSupport/UnicodeSupport.hxx"
#include "TFEL/System/System.hxx"
#include "TFEL/Math/Evaluator.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/PerformanceProfiling.hxx"
#include "MFront/CodeGeneratorUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/PedanticMode.hxx"
#include "MFront/SupportedTypes.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/ModelDescription.hxx"
#include "MFront/BehaviourVariableDescription.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/MFrontMaterialPropertyInterface.hxx"
#include "MFront/AbstractBehaviourInterface.hxx"
#include "MFront/FiniteStrainBehaviourTangentOperatorConversionPath.hxx"
#include "MFront/BehaviourCodeGeneratorBase.hxx"

namespace mfront {

  static std::string makeCamelCase(std::string_view s) {
    const auto words = tfel::utilities::tokenize(s, '_', false);
    auto upperCaseFirstLetter = [](const std::string& w) -> std::string {
      if (w.empty()) {
        return {};
      }
      if (w == "mfront") {
        return "MFront";
      }
      return makeUpperCase(w.substr(0, 1)) + makeLowerCase(w.substr(1));
    };
    auto r = std::string{};
    for (const auto& w : words) {
      r += upperCaseFirstLetter(w);
    }
    return r;
  }  // end of makeCamelCase

  /*!
   * \return a temporary name which has not been reserved. This
   * method shall be used to get a temporary name at a
   * code block scope.
   * \param[in,out] tmpnames: list of already used temporary names
   * in the treated code block.
   * \param[in]     p: prefix
   */
  static std::string getTemporaryVariableName(
      std::vector<std::string>& tmpnames,
      const BehaviourDescription& bd,
      const std::string& p) {
    if (!tfel::utilities::CxxTokenizer::isValidIdentifier(p)) {
      tfel::raise(
          "getTemporaryVariableName: "
          "invalid variable prefix '" +
          p + "'");
    }
    for (std::size_t i = 0; i != std::numeric_limits<std::size_t>::max(); ++i) {
      const auto c = p + std::to_string(i);
      if (!bd.isNameReserved(c)) {
        if (std::find(tmpnames.begin(), tmpnames.end(), c) == tmpnames.end()) {
          tmpnames.push_back(c);
          return c;
        }
      }
    }
    tfel::raise(
        "getTemporaryVariableName: "
        "unable to find a temporary variable");
  }  // end of getTemporaryVariableName

  static bool hasUserDefinedTangentOperatorCode(
      const BehaviourDescription& bd,
      const AbstractBehaviourCodeGenerator::Hypothesis h) {
    using tfel::material::getFiniteStrainBehaviourTangentOperatorFlags;
    if (bd.getBehaviourType() ==
        BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      // all available tangent operators for finite strain behaviours
      const auto tos = getFiniteStrainBehaviourTangentOperatorFlags();
      // search tangent operators defined by the user
      for (const auto& t : tos) {
        const auto ktype =
            convertFiniteStrainBehaviourTangentOperatorFlagToString(t);
        if (bd.hasCode(h, std::string(BehaviourData::ComputeTangentOperator) +
                              '-' + ktype)) {
          return true;
        }
      }
    } else {
      if (bd.hasCode(h, BehaviourData::ComputeTangentOperator)) {
        return true;
      }
    }
    return false;
  }  // end of hasUserDefinedTangentOperatorCode

  BehaviourCodeGeneratorBase::BehaviourCodeGeneratorBase(
      const FileDescription& f,
      const BehaviourDescription& d,
      const BehaviourInterfaceMap& bim)
      : fd(f), bd(d), interfaces(bim) {}  // end of BehaviourCodeGeneratorBase

  void BehaviourCodeGeneratorBase::throwRuntimeError(
      const std::string_view m, const std::string_view e) const {
    auto msg = std::string{m};
    msg.append(": ");
    msg.append(e);
    tfel::raise(msg);
  }  // end of throwRuntimeError

  void BehaviourCodeGeneratorBase::writeHeaderGuardBegin(std::ostream& os,
                                                         const std::string& cn,
                                                         const bool b) const {
    const auto ns = this->bd.getInternalNamespace();

    const auto suffix = b ? "HXX" : "IXX";
    if (!ns.empty()) {
      const auto tmp = makeUpperCase(ns + '_' + cn);
      os << "#ifndef LIB_TFEL_MATERIAL_" << tmp << '_' << suffix << "\n"
         << "#define LIB_TFEL_MATERIAL_" << tmp << '_' << suffix << "\n\n";
    } else {
      os << "#ifndef LIB_TFEL_MATERIAL_" << makeUpperCase(cn) << '_' << suffix
         << "\n"
         << "#define LIB_TFEL_MATERIAL_" << makeUpperCase(cn) << '_' << suffix
         << "\n\n";
    }
  }

  void BehaviourCodeGeneratorBase::writeHeaderGuardEnd(std::ostream& os,
                                                       const std::string& cn,
                                                       const bool b) const {
    const auto ns = this->bd.getInternalNamespace();
    const auto suffix = b ? "HXX" : "IXX";
    if (!ns.empty()) {
      const auto tmp = makeUpperCase(ns + '_' + cn);
      os << "#endif /* LIB_TFEL_MATERIAL_" << tmp << '_' << suffix << "*/\n";
    } else {
      os << "#endif /* LIB_TFEL_MATERIAL_" << makeUpperCase(cn) << '_' << suffix
         << " */\n";
    }
  }

  bool BehaviourCodeGeneratorBase::isSrcFileRequired() const {
    const auto profiling =
        this->bd.getAttribute(BehaviourData::profiling, false);
    const auto parameters =
        ((this->bd.hasParameters()) &&
         (!areParametersTreatedAsStaticVariables(this->bd)));
    const auto user_defined_sources = !this->bd.getSources().empty();
    return profiling || parameters || user_defined_sources;
  }  // end of isSrcFileRequired

  void BehaviourCodeGeneratorBase::generateOutputFiles(
      const std::set<Hypothesis>& mhs) {
    tfel::system::systemCall::mkdir("src");
    tfel::system::systemCall::mkdir("include");
    tfel::system::systemCall::mkdir("include/TFEL/");
    tfel::system::systemCall::mkdir("include/TFEL/Material");
    const auto ns = this->bd.getInternalNamespace();
    if (!ns.empty()) {
      tfel::system::systemCall::mkdir("include/TFEL/Material/" +
                                      makeCamelCase(ns));
      tfel::system::systemCall::mkdir("src/" + makeCamelCase(ns));
    }
    //! generating sources du to external material properties and models
    std::ofstream behaviourFile("include/" + this->bd.getBehaviourFileName());
    std::ofstream behaviourDataFile("include/" +
                                    this->bd.getBehaviourDataFileName());
    std::ofstream integrationDataFile("include/" +
                                      this->bd.getIntegrationDataFileName());
    behaviourFile.precision(14);
    behaviourDataFile.precision(14);
    integrationDataFile.precision(14);
    if (!behaviourFile) {
      this->throwRuntimeError("BehaviourCodeGeneratorBase::generateOutputFiles",
                              "unable to open '" +
                                  this->bd.getBehaviourFileName() +
                                  "' for writing output file");
    }
    if (!behaviourDataFile) {
      this->throwRuntimeError("BehaviourCodeGeneratorBase::generateOutputFiles",
                              "unable to open '" +
                                  this->bd.getBehaviourDataFileName() +
                                  "' for writing output file");
    }
    if (!integrationDataFile) {
      this->throwRuntimeError("BehaviourCodeGeneratorBase::generateOutputFiles",
                              "unable to open '" +
                                  this->bd.getIntegrationDataFileName() +
                                  "' for writing output file");
    }
    auto write_classes = [this, &behaviourFile, &behaviourDataFile,
                          &integrationDataFile](const Hypothesis h) {
      const auto n = h == ModellingHypothesis::UNDEFINEDHYPOTHESIS
                         ? "default hypothesis"
                         : "'" + ModellingHypothesis::toString(h) + "'";
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        auto& log = getLogStream();
        log << "BehaviourCodeGeneratorBase::generateOutputFiles: "
            << "treating " + n + "\n";
      }
      // Generating BehaviourData's outputClass
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        auto& log = getLogStream();
        log << "BehaviourCodeGeneratorBase::generateOutputFiles: "
            << "writing behaviour data for " + n + "\n";
      }
      this->writeBehaviourDataClass(behaviourDataFile, h);
      // Generating IntegrationData's outputClass
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        auto& log = getLogStream();
        log << "BehaviourCodeGeneratorBase::generateOutputFiles: "
            << "writing integration data for " + n + "\n";
      }
      this->writeIntegrationDataClass(integrationDataFile, h);
      // Generating Behaviour's outputFile
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        auto& log = getLogStream();
        log << "BehaviourCodeGeneratorBase::generateOutputFiles: "
            << "writing behaviour class for " + n + "\n";
      }
      this->writeBehaviourClass(behaviourFile, h);
    };
    // generate outpout files
    this->writeBehaviourDataFileBegin(behaviourDataFile);
    this->writeIntegrationDataFileBegin(integrationDataFile);
    this->writeBehaviourFileBegin(behaviourFile);
    if (this->bd.areSlipSystemsDefined()) {
      this->generateSlipSystemsFiles();
    }
    if (!this->bd.areAllMechanicalDataSpecialised(mhs)) {
      write_classes(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for (const auto& h : mhs) {
      if (this->bd.hasSpecialisedMechanicalData(h)) {
        write_classes(h);
      }
    }
    this->writeBehaviourDataFileEnd(behaviourDataFile);
    this->writeIntegrationDataFileEnd(integrationDataFile);
    this->writeBehaviourFileEnd(behaviourFile);
    // Generating behaviour's source file
    if (this->isSrcFileRequired()) {
      std::ofstream srcFile("src/" + this->bd.getSrcFileName());
      if (!srcFile) {
        this->throwRuntimeError(
            "BehaviourCodeGeneratorBase::generateOutputFiles",
            "unable to open '" + this->bd.getSrcFileName() +
                "' for writing output file");
      }
      srcFile.precision(14);
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        auto& log = getLogStream();
        log << "BehaviourCodeGeneratorBase::generateOutputFiles: "
            << "writing source file\n";
      }
      this->writeSrcFile(srcFile);
      srcFile.close();
    }
    // calling the interfaces
    for (const auto& i : this->interfaces) {
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        auto& log = getLogStream();
        log << "BehaviourCodeGeneratorBase::generateOutputFiles : "
            << "calling interface '" << i.first << "'\n";
      }
      i.second->endTreatment(this->bd, this->fd);
    }
    behaviourFile.close();
    behaviourDataFile.close();
    integrationDataFile.close();
  }  // end of generateOutputFiles

  void BehaviourCodeGeneratorBase::generateSlipSystemsFiles() {
    using SlipSystemsDescription = BehaviourDescription::SlipSystemsDescription;
    using vector = SlipSystemsDescription::vector;
    using tensor = SlipSystemsDescription::tensor;
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError(
            "FiniteStrainSingleCrystalBrick::"
            "generateSlipSystems",
            m);
      }
    };
    auto write_vector = [](std::ostream& out, const std::string& v,
                           const std::vector<vector>& ts) {
      for (decltype(ts.size()) i = 0; i != ts.size(); ++i) {
        const auto& t = ts[i];
        out << v << "[" << i << "] = vector{"
            << "real(" << t[0] << "),"
            << "real(" << t[1] << "),"
            << "real(" << t[2] << ")};\n";
      }
    };
    auto write_tensor = [](std::ostream& out, const std::string& mu,
                           const std::vector<tensor>& ts) {
      for (decltype(ts.size()) i = 0; i != ts.size(); ++i) {
        const auto& t = ts[i];
        out << mu << "[" << i << "] = tensor{"
            << "real(" << t[0] << "),"
            << "real(" << t[1] << "),"
            << "real(" << t[2] << "),"
            << "real(" << t[3] << "),"
            << "real(" << t[4] << "),"
            << "real(" << t[5] << "),"
            << "real(" << t[6] << "),"
            << "real(" << t[7] << "),"
            << "real(" << t[8] << ")};\n";
      }
    };
    auto write_stensor = [](std::ostream& out, const std::string& mus,
                            const std::vector<tensor>& ts) {
      constexpr const auto cste = tfel::math::Cste<long double>::sqrt2 / 2;
      for (decltype(ts.size()) i = 0; i != ts.size(); ++i) {
        const auto& t = ts[i];
        out << mus << "[" << i << "] = stensor{"
            << "real(" << t[0] << "),"
            << "real(" << t[1] << "),"
            << "real(" << t[2] << "),"
            << "real(" << (t[3] + t[4]) * cste << "),"
            << "real(" << (t[5] + t[6]) * cste << "),"
            << "real(" << (t[7] + t[8]) * cste << ")};\n";
      }
    };
    const auto& sss = this->bd.getSlipSystems();
    const auto nb = sss.getNumberOfSlipSystemsFamilies();
    const auto ims = sss.getInteractionMatrixStructure();
    const auto cn = this->bd.getClassName() + "SlipSystems";
    auto file = "include/" + this->bd.getSlipSystemHeaderFileName();
    std::ofstream out(file);
    throw_if(!out, "can't open file '" + file + "'");
    out.exceptions(std::ios::badbit | std::ios::failbit);
    out << "/*!\n"
        << "* \\file   " << file << '\n'
        << "* \\brief  "
        << "this file decares the " << cn << " class.\n"
        << "*         File generated by " << MFrontHeader::getVersionName()
        << " "
        << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!this->fd.authorName.empty()) {
      out << "* \\author " << this->fd.authorName << '\n';
    }
    if (!this->fd.date.empty()) {
      out << "* \\date   " << this->fd.date << '\n';
    }
    out << " */\n\n";
    this->writeHeaderGuardBegin(out, cn, true);
    out << "#if (defined _WIN32 || defined _WIN64)\n"
        << "#ifdef min\n"
        << "#undef min\n"
        << "#endif /* min */\n"
        << "#ifdef max\n"
        << "#undef max\n"
        << "#endif /* max */\n"
        << "#ifdef small\n"
        << "#undef small\n"
        << "#endif /* small */\n"
        << "#endif /* (defined _WIN32 || defined _WIN64) */\n\n"
        << "#include\"TFEL/Raise.hxx\"\n"
        << "#include\"TFEL/Math/tvector.hxx\"\n"
        << "#include\"TFEL/Math/stensor.hxx\"\n"
        << "#include\"TFEL/Math/tensor.hxx\"\n\n";
    this->writeNamespaceBegin(out);
    out << "template<typename real>\n"
        << "struct " << cn << '\n'
        << "{\n"
        << "//! a simple alias\n"
        << "using tensor = tfel::math::tensor<3u,real>;\n"
        << "//! a simple alias\n"
        << "using vector = tfel::math::tvector<3u,real>;\n"
        << "//! a simple alias\n"
        << "using stensor = tfel::math::stensor<3u,real>;\n";
    auto nss = std::size_t{};
    for (std::size_t idx = 0; idx != nb; ++idx) {
      nss += sss.getNumberOfSlipSystems(idx);
    }
    if (nb == 1u) {
      const auto nss0 = sss.getNumberOfSlipSystems(0);
      out << "//! number of sliding systems\n"
          << "static constexpr unsigned short Nss"
          << " = " << nss << ";\n"
          << "//! number of sliding systems (first and uniq family)\n"
          << "static constexpr unsigned short Nss0"
          << " = " << nss0 << ";\n";
    } else {
      for (std::size_t idx = 0; idx != nb; ++idx) {
        out << "//! number of sliding systems\n"
            << "static constexpr unsigned short Nss" << idx << " = "
            << sss.getNumberOfSlipSystems(idx) << ";\n";
      }
      out << "static constexpr unsigned short Nss = ";
      for (std::size_t idx = 0; idx != nb;) {
        out << "Nss" << idx;
        if (++idx != nb) {
          out << "+";
        }
      }
      out << ";\n";
    }
    out << "//! tensor of directional sense\n"
        << "tfel::math::tvector<Nss,tensor> mu;\n"
        << "//! symmetric tensor of directional sense\n"
        << "tfel::math::tvector<Nss,stensor> mus;\n"
        << "//! normal to slip plane\n"
        << "tfel::math::tvector<Nss,vector> np;\n"
        << "//! unit vector in the slip direction\n"
        << "tfel::math::tvector<Nss,vector> ns;\n"
        << "//! glimb tensors\n"
        << "tfel::math::tvector<Nss, stensor> climb_tensors;\n";
    for (std::size_t idx = 0; idx != nb; ++idx) {
      out << "//! tensor of directional sense\n"
          << "tfel::math::tvector<Nss" << idx << ",tensor> mu" << idx << ";\n"
          << "//! symmetric tensor of directional sense\n"
          << "tfel::math::tvector<Nss" << idx << ",stensor> mus" << idx << ";\n"
          << "//! normal to slip plane\n"
          << "tfel::math::tvector<Nss" << idx << ",vector> np" << idx << ";\n"
          << "//! glimb tensors\n"
          << "tfel::math::tvector<Nss" << idx << ", stensor> "
          << "climb_tensors" << idx << ";\n"
          << "//! unit vector in the slip direction\n"
          << "tfel::math::tvector<Nss" << idx << ",vector> ns" << idx << ";\n";
    }
    if (this->bd.hasInteractionMatrix()) {
      out << "//! interaction matrix\n"
          << "tfel::math::tmatrix<Nss,Nss,real> mh;\n";
      out << "//! interaction matrix\n"
          << "tfel::math::tmatrix<Nss,Nss,real> him;\n";
    }
    if (this->bd.hasDislocationsMeanFreePathInteractionMatrix()) {
      out << "tfel::math::tmatrix<Nss,Nss,real> dim;\n";
    }
    if (nb != 1u) {
      out << "/*!\n"
          << " * \\return the gobal index of the jth system of ith family\n"
          << " * \\param[in] i: slip system family\n"
          << " * \\param[in] j: local slip system index\n"
          << " */\n"
          << "constexpr unsigned short offset(const unsigned short,\nconst "
             "unsigned short) const;\n";
      for (std::size_t i = 0; i != nb; ++i) {
        out << "/*!\n"
            << " * \\return the gobal index of the ith system of " << i
            << "th family\n"
            << " * \\param[in] i: local slip system index\n"
            << " */\n"
            << "constexpr unsigned short offset" << i
            << "(const unsigned short) const;\n";
      }
    }
    out << "/*!\n"
        << " * \\return true if two systems are coplanar\n"
        << " * \\param[in] i: first slip system index\n"
        << " * \\param[in] j: second slip system index\n"
        << " */\n"
        << "bool areCoplanar(const unsigned short,\n"
        << "                 const unsigned short) const;\n"
        << "/*!\n"
        << " * \\return an interaction matrix\n"
        << " * \\param[in] m: coefficients of the interaction matrix\n"
        << " */\n"
        << "constexpr auto\n"
        << "buildInteractionMatrix("
        << "const tfel::math::fsarray<" << ims.rank() << ", real>&) const\n"
        << " -> tfel::math::tmatrix<Nss, Nss, real>;\n"
        << "//! return the unique instance of the class\n"
        << "static const " << cn << "&\n"
        << "getSlidingSystems();\n"
        << "//! return the unique instance of the class\n"
        << "static const " << cn << "&\n"
        << "getSlipSystems();\n"
        << "//! return the unique instance of the class\n"
        << "static const " << cn << "&\n"
        << "getGlidingSystems();\n"
        << "private:\n"
        << "//! Constructor\n"
        << cn << "();\n"
        << "//! move constructor (disabled)\n"
        << cn << "(" << cn << "&&) = delete;\n"
        << "//! copy constructor (disabled)\n"
        << cn << "(const " << cn << "&) = delete;\n"
        << "//! move operator (disabled)\n"
        << cn << "&\n"
        << "operator=(" << cn << "&&) = delete;\n"
        << "//! copy assignement (disabled)\n"
        << cn << "&\n"
        << "operator=(const " << cn << "&) = delete;\n"
        << "}; // end of struct " << cn << "\n\n"
        << "//! a simple alias\n"
        << "template<typename real>\n"
        << "using " << this->bd.getClassName() << "SlidingSystems "
        << "= " << cn << "<real>;\n\n"
        << "//! a simple alias\n"
        << "template<typename real>\n"
        << "using " << this->bd.getClassName() << "GlidingSystems "
        << "= " << cn << "<real>;\n\n";
    this->writeNamespaceEnd(out);
    out << "#include\"" << this->bd.getSlipSystemImplementationFileName()
        << "\"\n\n";
    this->writeHeaderGuardEnd(out, cn, true);
    out.close();
    file = "include/" + this->bd.getSlipSystemImplementationFileName();
    out.open(file);
    throw_if(!out, "can't open file '" + file + "'");
    out.exceptions(std::ios::badbit | std::ios::failbit);
    out.precision(std::numeric_limits<long double>::digits10);
    out << "/*!\n"
        << "* \\file   " << file << '\n'
        << "* \\brief  "
        << "this file implements the " << cn << " class.\n"
        << "*         File generated by " << MFrontHeader::getVersionName()
        << " "
        << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!this->fd.authorName.empty()) {
      out << "* \\author " << this->fd.authorName << '\n';
    }
    if (!this->fd.date.empty()) {
      out << "* \\date   " << this->fd.date << '\n';
    }
    out << " */\n\n";
    this->writeHeaderGuardBegin(out, cn, false);
    out << "#include\"TFEL/Math/General/MathConstants.hxx\"\n\n";
    this->writeNamespaceBegin(out);
    out << "template<typename real>\n"
        << "const " << cn << "<real>&\n"
        << cn << "<real>::getSlidingSystems(){\n"
        << "static const " << cn << " i;\n"
        << "return i;\n"
        << "} // end of " << cn << "::getSlidingSystems\n\n"
        << "template<typename real>\n"
        << "const " << cn << "<real>&\n"
        << cn << "<real>::getSlipSystems(){\n"
        << "return " << cn << "<real>::getSlidingSystems();\n"
        << "} // end of " << cn << "::getSlipSystems\n\n"
        << "template<typename real>\n"
        << "const " << cn << "<real>&\n"
        << cn << "<real>::getGlidingSystems(){\n"
        << "return " << cn << "<real>::getSlidingSystems();\n"
        << "} // end of " << cn << "::getGlidingSystems\n\n"
        << "template<typename real>\n"
        << cn << "<real>::" << cn << "(){\n";
    std::vector<tensor> gots;
    std::vector<vector> gnss;
    std::vector<vector> gnps;
    // orientation tensors
    for (std::size_t idx = 0; idx != nb; ++idx) {
      const auto& ots = sss.getOrientationTensors(idx);
      write_tensor(out, "this->mu" + std::to_string(idx), ots);
      gots.insert(gots.end(), ots.begin(), ots.end());
    }
    write_tensor(out, "this->mu", gots);
    // symmetric orientation tensors
    for (std::size_t idx = 0; idx != nb; ++idx) {
      write_stensor(out, "this->mus" + std::to_string(idx),
                    sss.getOrientationTensors(idx));
    }
    write_stensor(out, "this->mus", gots);
    // normal to slip planes
    for (std::size_t idx = 0; idx != nb; ++idx) {
      const auto& nps = sss.getSlipPlaneNormals(idx);
      write_vector(out, "this->np" + std::to_string(idx), nps);
      gnps.insert(gnps.end(), nps.begin(), nps.end());
    }
    write_vector(out, "this->np", gnps);
    // slip direction
    for (std::size_t idx = 0; idx != nb; ++idx) {
      const auto& nss2 = sss.getSlipDirections(idx);
      write_vector(out, "this->ns" + std::to_string(idx), nss2);
      for (decltype(nss2.size()) i = 0; i != nss2.size(); ++i) {
        out << "this->climb_tensors" << idx << "[" << i << "] = "
            << "stensor::buildFromVectorDiadicProduct("
            << "this->ns" << idx << "[" << i << "]);\n";
      }
      gnss.insert(gnss.end(), nss2.begin(), nss2.end());
    }
    write_vector(out, "this->ns", gnss);
    for (decltype(gnss.size()) i = 0; i != gnss.size(); ++i) {
      out << "this->climb_tensors[" << i << "] = "
          << "stensor::buildFromVectorDiadicProduct("
          << "this->ns[" << i << "]);\n";
    }
    auto write_imatrix = [&out, &sss, &nb, &nss, &ims](
                             const std::vector<long double>& m,
                             const std::string& n) {
      auto count =
          std::size_t{};  // number of terms of the matrix treated so far
      out << "this->" << n << " = {";
      for (std::size_t idx = 0; idx != nb; ++idx) {
        const auto gsi = sss.getSlipSystems(idx);
        for (std::size_t idx2 = 0; idx2 != gsi.size(); ++idx2) {
          for (std::size_t jdx = 0; jdx != nb; ++jdx) {
            const auto gsj = sss.getSlipSystems(jdx);
            for (std::size_t jdx2 = 0; jdx2 != gsj.size(); ++jdx2) {
              const auto r = ims.getRank(gsi[idx2], gsj[jdx2]);
              out << "real(" << m[r] << ")";
              if (++count != nss * nss) {
                out << ",";
              }
            }
          }
          out << '\n';
        }
      }
      out << "};\n";
    };
    if (this->bd.hasInteractionMatrix()) {
      write_imatrix(sss.getInteractionMatrix(), "mh");
      write_imatrix(sss.getInteractionMatrix(), "him");
    }
    if (this->bd.hasDislocationsMeanFreePathInteractionMatrix()) {
      write_imatrix(sss.getDislocationsMeanFreePathInteractionMatrix(), "dim");
    }
    out << "} // end of " << cn << "::" << cn << "\n\n";
    if (nb != 1u) {
      out << "template<typename real>\n"
          << "constexpr unsigned short\n"
          << cn << "<real>::offset(const unsigned short i,\n"
          << "const unsigned short j\n) const{\n"
          << "const auto oi = [&i]() -> unsigned short{\n"
          << "switch(i){\n";
      for (std::size_t i = 0; i != nb; ++i) {
        out << "case " << i << ":\n";
        if (i == 0) {
          out << "return 0;\n"
              << "break;\n";
        } else {
          out << "return ";
          for (std::size_t j = 0; j != i;) {
            out << "Nss" << j;
            if (++j != i) {
              out << "+";
            }
          }
          out << ";\n"
              << "break;\n";
        }
      }
      out << "default:\n"
          << "tfel::raise<std::out_of_range>(\"" << cn
          << "::offset: :\"\n\"invalid index"
          << "\");\n"
          << "}\n"
          << "}();\n"
          << "return oi+j;\n"
          << "} // end of offset\n\n";
      for (std::size_t i = 0; i != nb; ++i) {
        out << "template<typename real>\n"
            << "constexpr unsigned short\n"
            << cn << "<real>::offset" << i
            << "(const unsigned short i) const{\n";
        if (i != 0) {
          out << "constexpr unsigned short o = ";
          for (std::size_t j = 0; j != i;) {
            out << "Nss" << j;
            if (++j != i) {
              out << "+";
            }
          }
          out << ";\n"
              << "return o+i;\n";
        } else {
          out << "return i;\n";
        }
        out << "} // end of offset" << i << "\n\n";
      }
    }
    out << "template<typename real>\n"
        << "bool " << cn << "<real>::areCoplanar(const unsigned short i,\n"
        << "                                     const unsigned short j) "
           "const{\n";
    std::vector<std::vector<bool>> are_coplanar(nss, std::vector<bool>(nss));
    auto i = std::size_t{};
    for (std::size_t idx = 0; idx != nb; ++idx) {
      const auto gsi = sss.getSlipSystems(idx);
      for (std::size_t idx2 = 0; idx2 != gsi.size(); ++idx2, ++i) {
        auto j = std::size_t{};
        for (std::size_t jdx = 0; jdx != nb; ++jdx) {
          const auto gsj = sss.getSlipSystems(jdx);
          for (std::size_t jdx2 = 0; jdx2 != gsj.size(); ++jdx2, ++j) {
            if (gsi[idx2].is<SlipSystemsDescription::system3d>()) {
              const auto& si =
                  gsi[idx2].get<SlipSystemsDescription::system3d>();
              const auto& sj =
                  gsj[jdx2].get<SlipSystemsDescription::system3d>();
              const auto& ni = si.plane;
              const auto& nj = sj.plane;
              are_coplanar[i][j] =
                  (std::equal(ni.begin(), ni.end(), nj.begin()) ||
                   std::equal(
                       ni.begin(), ni.end(), nj.begin(),
                       [](const int a, const int b) { return a == -b; }));
            } else {
              const auto& si =
                  gsi[idx2].get<SlipSystemsDescription::system4d>();
              const auto& sj =
                  gsj[jdx2].get<SlipSystemsDescription::system4d>();
              const auto& ni = si.plane;
              const auto& nj = sj.plane;
              are_coplanar[i][j] =
                  (std::equal(ni.begin(), ni.end(), nj.begin()) ||
                   std::equal(
                       ni.begin(), ni.end(), nj.begin(),
                       [](const int a, const int b) { return a == -b; }));
            }
          }
        }
      }
    }
    out << "const auto mi = std::min(i,j);\n"
        << "const auto mj = std::max(i,j);\n"
        << "switch(mi){\n";
    for (i = 0; i != nss; ++i) {
      out << "case " << i << ":\n";
      if (i + 1 == nss) {
        out << "return (mi==" << nss - 1 << ")&&(mj==" << nss - 1 << ");\n";
      } else {
        out << "switch (mj){\n";
        for (std::size_t j = i; j != nss; ++j) {
          out << "case " << j << ":\n"
              << "return " << (are_coplanar[i][j] ? "true" : "false") << ";\n"
              << "break;\n";
        }
        out << "default:\n"
            << "return false;\n"
            << "}\n";
      }
      out << "break;\n";
    }
    out << "default:\n"
        << "break;\n"
        << "}\n"
        << "return false;\n"
        << "}\n\n";
    // buildInteractionMatrix
    auto count = std::size_t{};  // number of terms of the matrix treated so far
    out << "template<typename real>\n"
        << "constexpr auto "
        << cn << "<real>::buildInteractionMatrix("
        << "const tfel::math::fsarray<" << ims.rank() << ", real>& m) const\n"
        << "-> tfel::math::tmatrix<Nss,Nss,real>{\n"
        << "return {";
    for (std::size_t idx = 0; idx != nb; ++idx) {
      const auto gsi = sss.getSlipSystems(idx);
      for (std::size_t idx2 = 0; idx2 != gsi.size(); ++idx2) {
        for (std::size_t jdx = 0; jdx != nb; ++jdx) {
          const auto gsj = sss.getSlipSystems(jdx);
          for (std::size_t jdx2 = 0; jdx2 != gsj.size(); ++jdx2) {
            const auto r = ims.getRank(gsi[idx2], gsj[jdx2]);
            out << "m[" << r << "]";
            if (++count != nss * nss) {
              out << ",";
            }
          }
        }
        out << '\n';
      }
    }

    out << "};\n"
        << "} // end of buildInteractionMatrix\n\n";
    this->writeNamespaceEnd(out);
    this->writeHeaderGuardEnd(out, cn, false);
  }

  void BehaviourCodeGeneratorBase::writeModelCall(
      std::ostream& out,
      std::vector<std::string>& tmpnames,
      const Hypothesis h,
      const ModelDescription& md,
      const std::vector<std::string>& outputVariables,
      const std::vector<std::string>& inputVariables,
      const std::string& bn) const {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("BehaviourDSLCommon::writeModelCall", m);
      }
    };
    auto write_variable = [throw_if, &md, &out](const std::string& v,
                                                const unsigned short d) {
      if (d == 0) {
        out << "this->" << v << "+this->d" << v;
      } else if (d == 1) {
        out << "this->" << v;
      } else {
        throw_if(true, "invalid depth for the variable '" + v +
                           "' "
                           "in model '" +
                           md.className + "'");
      }
    };
    const auto& bdata = this->bd.getBehaviourData(h);
    throw_if(md.outputs.size() != outputVariables.size(),
             "invalid number of outputs for model '" + md.className + "'");
    throw_if(md.outputs.size() != inputVariables.size(),
             "invalid number of outputs for model '" + md.className + "'");
    throw_if(!md.constantMaterialProperties.empty(),
             "constant material properties are not supported yet");
    throw_if(md.functions.size() != 1u,
             "invalid number of functions in model '" + md.className + "'");
    const auto& f = md.functions[0];
    throw_if(f.modifiedVariables.empty(),
             "no modified variable for function '" + f.name + "'");
    throw_if(f.modifiedVariables.size() != outputVariables.size(),
             "invalid number of functions in model '" + md.className + "'");
    throw_if(f.name.empty(), "unnamed function");
    throw_if((f.usedVariables.empty()) && (!f.useTimeIncrement),
             "no used variable for function '" + f.name + "'");
    const auto sm = getTemporaryVariableName(tmpnames, this->bd, bn);
    if (inputVariables.size() == 1) {
      out << "// updating " << inputVariables[0] << "\n";
    }
    out << "mfront::" << md.className << "<NumericType> " << sm << ";\n"
        << "" << sm << ".setOutOfBoundsPolicy(this->policy);\n";
    if (md.outputs.size() == 1) {
      out << "this->" << outputVariables[0] << " = " << sm << "." << f.name
          << "(";
    } else {
      // modified variables can be in a different order than the outputs
      out << sm << "." << f.name << "(";
      for (const auto& mv : f.modifiedVariables) {
        const auto pb = md.outputs.begin();
        const auto p =
            std::find_if(pb, md.outputs.end(),
                         [&mv](const auto& vout) { return vout.name == mv; });
        throw_if(p == md.outputs.end(),
                 "internal error: modified variable '" + mv +
                     "' was not found in the outputs of the model");
        out << "this->" << outputVariables[p - pb] << ", ";
      }
    }
    const auto args = [&f] {
      auto a = std::vector<std::string>{};
      for (const auto& uv : f.usedVariables) {
        a.push_back(uv);
      }
      if (f.useTimeIncrement) {
        a.emplace_back("dt");
      }
      return a;
    }();
    const auto asvn =
        bdata.getExternalNames(bdata.getAuxiliaryStateVariables());
    for (auto pa = std::begin(args); pa != std::end(args);) {
      if (*pa == "dt") {
        out << "this->dt";
        if (++pa != std::end(args)) {
          out << ",";
        }
        continue;
      }
      const auto a = md.decomposeVariableName(*pa);
      const auto& ea = md.getVariableDescription(a.first).getExternalName();
      auto treated = false;
      for (const auto& vs : inputVariables) {
        if (ea == bdata.getExternalName(vs)) {
          throw_if(a.second != 1, "invalid depth for variable '" + a.first +
                                      "' in model '" + md.className + "'");
          out << "this->" << vs;
          treated = true;
          break;
        }
      }
      if (!treated) {
        if (std::find(std::begin(asvn), std::end(asvn), ea) != std::end(asvn)) {
          const auto& av =
              bdata.getAuxiliaryStateVariableDescriptionByExternalName(ea);
          throw_if(!av.getAttribute<bool>("ComputedByExternalModel", false),
                   "only auxiliary state variable computed by a model are "
                   "allowed here");
          write_variable(av.name, a.second);
        } else {
          const auto& en =
              bdata.getExternalStateVariableDescriptionByExternalName(ea);
          write_variable(en.name, a.second);
        }
      }
      if (++pa != std::end(args)) {
        out << ",";
      }
    }
    out << ");\n";
  }

  void BehaviourCodeGeneratorBase::writeVariablesDeclarations(
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
  }  // end of writeVariablesDeclarations

  void BehaviourCodeGeneratorBase::writeVariableDeclaration(
      std::ostream& f,
      const VariableDescription& v,
      const std::string& prefix,
      const std::string& suffix,
      const std::string& fileName,
      const bool useTimeDerivative) const {
    const auto n = prefix + v.name + suffix;
    const auto t = (!useTimeDerivative)
                       ? v.type
                       : SupportedTypes::getTimeDerivativeType(v.type);
    if ((!getDebugMode()) && (v.lineNumber != 0u)) {
      f << "#line " << v.lineNumber << " \"" << fileName << "\"\n";
    }
    if (v.arraySize == 1u) {
      f << t << " " << n << ";\n";
    } else {
      if (this->bd.useDynamicallyAllocatedVector(v.arraySize)) {
        f << "tfel::math::runtime_array<" << t << " > " << n << ";\n";
      } else {
        f << "tfel::math::fsarray<" << v.arraySize << ", " << t << " > " << n
          << ";\n";
      }
    }
  }  // end of writeVariableDeclaration

  void BehaviourCodeGeneratorBase::writeIncludes(std::ostream& file) const {
    if ((!file) || (!file.good())) {
      this->throwRuntimeError("BehaviourCodeGeneratorBase::writeIncludes",
                              "output file is not valid");
    }
    const auto& h = this->bd.getIncludes();
    if (!h.empty()) {
      file << h << '\n';
    }
  }

  void BehaviourCodeGeneratorBase::writeNamespaceBegin(
      std::ostream& file) const {
    if ((!file) || (!file.good())) {
      this->throwRuntimeError("BehaviourCodeGeneratorBase::writeNamespaceBegin",
                              "output file is not valid");
    }
    const auto ns = this->bd.getInternalNamespace();
    if (!ns.empty()) {
      file << "namespace tfel::material::" << ns << "{\n\n";
    } else {
      file << "namespace tfel::material{\n\n";
    }
  }

  void BehaviourCodeGeneratorBase::writeNamespaceEnd(std::ostream& file) const {
    if ((!file) || (!file.good())) {
      this->throwRuntimeError("BehaviourCodeGeneratorBase::writeNamespaceEnd",
                              "output file is not valid");
    }
    const auto ns = this->bd.getInternalNamespace();
    if (!ns.empty()) {
      file << "} // end of namespace tfel::material::" << ns << "\n\n";
    } else {
      file << "} // end of namespace tfel::material\n\n";
    }
  }

  void BehaviourCodeGeneratorBase::writeTypeAliases(std::ostream& file) const {
    if ((!file) || (!file.good())) {
      this->throwRuntimeError("BehaviourCodeGeneratorBase::writeTypeAliases",
                              "output file is not valid");
    }
    file << "using ushort =  unsigned short;\n";
    if (this->bd.useQt()) {
      file << "using Types = tfel::config::Types<N, NumericType, use_qt>;\n";
    } else {
      file << "using Types = tfel::config::Types<N, NumericType, false>;\n";
    }
    file << "using Type = NumericType;\n";
    for (const auto& a : getTypeAliases()) {
      file << "using " << a << " = typename Types::" << a << ";\n";
    }
    // tangent operator
    if (this->bd.hasTangentOperator()) {
      file << "using TangentOperator = " << this->bd.getTangentOperatorType()
           << ";\n";
    }
    // physical constants
    if (this->bd.useQt()) {
      file << "using PhysicalConstants = "
           << "tfel::PhysicalConstants<NumericType, use_qt>;\n";
    } else {
      file << "using PhysicalConstants = "
           << "tfel::PhysicalConstants<NumericType, false>;\n";
    }
  }  // end of writeTypeAliases

  bool BehaviourCodeGeneratorBase::usesStateVariableTimeDerivative() const {
    return false;
  }  // end of usesStateVariableTimeDerivative

  std::string
  BehaviourCodeGeneratorBase::getIntegrationVariablesIncrementsInitializers(
      const Hypothesis h) const {
    std::ostringstream f;
    const auto& vc = this->bd.getBehaviourData(h).getIntegrationVariables();
    for (auto p = vc.begin(); p != vc.end(); ++p) {
      const auto& v = *p;
      const auto flag = SupportedTypes::getTypeFlag(v.type);
      const auto n = v.name;
      const auto t = this->usesStateVariableTimeDerivative()
                         ? SupportedTypes::getTimeDerivativeType(v.type)
                         : v.type;
      if (p != vc.begin()) {
        f << ",\n";
      }
      if (flag == SupportedTypes::SCALAR) {
        if (this->bd.useDynamicallyAllocatedVector(v.arraySize)) {
          f << "d" << n << "(" << v.arraySize << "," << t << "(0))";
        } else {
          f << "d" << n << "(" << t << "(0))";
        }
      } else {
        const auto traits = "MathObjectTraits<" + t + ">";
        if (v.arraySize == 1u) {
          f << "d" << n << "(typename tfel::math::" + traits + "::NumType(0))";
        } else {
          if (this->bd.useDynamicallyAllocatedVector(v.arraySize)) {
            f << "d" << n << "(" << v.arraySize << "," << t
              << "(typename tfel::math::" + traits + "::NumType(0)))";
          } else {
            f << "d" << n << "(" << t
              << "(typename tfel::math::" + traits + "::NumType(0)))";
          }
        }
      }
    }
    return f.str();
  }  // end of SupportedTypes::getIntegrationVariablesInitializers

  void BehaviourCodeGeneratorBase::checkBehaviourDataFile(
      std::ostream& os) const {
    if ((!os) || (!os.good())) {
      this->throwRuntimeError(
          "BehaviourCodeGeneratorBase::checkBehaviourDataOutputFile",
          "output file is not valid");
    }
  }

  void BehaviourCodeGeneratorBase::writeBehaviourDataFileHeader(
      std::ostream& os) const {
    this->checkBehaviourDataFile(os);
    os << "/*!\n"
       << "* \\file   " << this->bd.getBehaviourDataFileName() << '\n'
       << "* \\brief  this file implements the " << this->bd.getClassName()
       << "BehaviourData class.\n"
       << "*         File generated by " << MFrontHeader::getVersionName()
       << " "
       << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!this->fd.authorName.empty()) {
      os << "* \\author " << this->fd.authorName << '\n';
    }
    if (!this->fd.date.empty()) {
      os << "* \\date   " << this->fd.date << '\n';
    }
    os << " */\n\n";
  }

  void BehaviourCodeGeneratorBase::writeBehaviourDataFileHeaderBegin(
      std::ostream& os) const {
    this->checkBehaviourDataFile(os);
    const auto cn = this->bd.getClassName() + "_behaviour_data";
    this->writeHeaderGuardBegin(os, cn, true);
  }

  void BehaviourCodeGeneratorBase::writeBehaviourDataFileHeaderEnd(
      std::ostream& os) const {
    this->checkBehaviourDataFile(os);
    const auto cn = this->bd.getClassName() + "_behaviour_data";
    this->writeHeaderGuardEnd(os, cn, true);
  }

  static bool hasVariableOfType(const BehaviourData& bd,
                                const SupportedTypes::TypeFlag f) {
    auto update = [f](const auto& variables) {
      const auto& flags = SupportedTypes::getTypeFlags();
      for (const auto& v : variables) {
        const auto pf = flags.find(v.type);
        if (pf == flags.end()) {
          continue;
        }
        if (pf->second == f) {
          return true;
        }
      }
      return false;
    };
    if ((update(bd.getMaterialProperties())) ||
        (update(bd.getIntegrationVariables())) ||
        (update(bd.getStateVariables())) ||
        (update(bd.getAuxiliaryStateVariables())) ||
        //        (update(bd.getLocalVariables())) ||
        (update(bd.getExternalStateVariables())) ||
        (update(bd.getInitializeFunctionVariables())) ||
        (update(bd.getPostProcessingVariables()))) {
      return true;
    }
    return false;
  }  // end of hasVariableOfType

  static bool hasVariableOfType(const BehaviourDescription& bd,
                                const SupportedTypes::TypeFlag f) {
    using ModellingHypothesis = BehaviourDescription::ModellingHypothesis;
    for (const auto& mv : bd.getMainVariables()) {
      if (mv.first.getTypeFlag() == f) {
        return true;
      }
      if (mv.second.getTypeFlag() == f) {
        return true;
      }
    }
    if (!bd.areAllMechanicalDataSpecialised()) {
      return hasVariableOfType(
          bd.getBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS), f);
    }
    for (const auto& h : bd.getDistinctModellingHypotheses()) {
      if (hasVariableOfType(bd.getBehaviourData(h), f)) {
        return true;
      }
    }
    return false;
  }  // end of requiresTVectorOrVectorIncludes

  void BehaviourCodeGeneratorBase::writeBehaviourDataStandardTFELIncludes(
      std::ostream& os) const {
    auto b1 = false;
    auto b2 = false;
    this->checkBehaviourDataFile(os);
    os << "#include<limits>\n"
       << "#include<string>\n"
       << "#include<sstream>\n"
       << "#include<iostream>\n"
       << "#include<stdexcept>\n"
       << "#include<algorithm>\n\n"
       << "#include\"TFEL/Raise.hxx\"\n"
       << "#include\"TFEL/PhysicalConstants.hxx\"\n"
       << "#include\"TFEL/Config/TFELConfig.hxx\"\n"
       << "#include\"TFEL/Config/TFELTypes.hxx\"\n"
       << "#include\"TFEL/TypeTraits/IsFundamentalNumericType.hxx\"\n"
       << "#include\"TFEL/TypeTraits/IsReal.hxx\"\n"
       << "#include\"TFEL/Math/General/Abs.hxx\"\n"
       << "#include\"TFEL/Math/General/IEEE754.hxx\"\n"
       << "#include\"TFEL/Math/General/DerivativeType.hxx\"\n"
       << "#include\"TFEL/Math/Array/ViewsArrayIO.hxx\"\n"
       << "#include\"TFEL/Math/Array/fsarrayIO.hxx\"\n"
       << "#include\"TFEL/Math/Array/runtime_arrayIO.hxx\"\n"
       << "#include\"TFEL/Math/fsarray.hxx\"\n"
       << "#include\"TFEL/Math/runtime_array.hxx\"\n";
    if (this->bd.useQt()) {
      os << "#include\"TFEL/Math/qt.hxx\"\n";
      os << "#include\"TFEL/Math/Quantity/qtIO.hxx\"\n";
    }
    this->bd.requiresTVectorOrVectorIncludes(b1, b2);
    if (b1) {
      os << "#include\"TFEL/Math/tvector.hxx\"\n"
         << "#include\"TFEL/Math/Vector/tvectorIO.hxx\"\n";
    }
    if (b2) {
      os << "#include\"TFEL/Math/vector.hxx\"\n";
    }
    os << "#include\"TFEL/Math/tmatrix.hxx\"\n"
       << "#include\"TFEL/Math/Matrix/tmatrixIO.hxx\"\n";
    if (hasVariableOfType(this->bd, SupportedTypes::STENSOR)) {
      os << "#include\"TFEL/Math/stensor.hxx\"\n"
         << "#include\"TFEL/Math/Stensor/StensorConceptIO.hxx\"\n"
         << "#include\"TFEL/Math/st2tost2.hxx\"\n"
         << "#include\"TFEL/Math/ST2toST2/ST2toST2ConceptIO.hxx\"\n";
    }
    if (hasVariableOfType(this->bd, SupportedTypes::TENSOR)) {
      os << "#include\"TFEL/Math/tensor.hxx\"\n"
         << "#include\"TFEL/Math/Tensor/TensorConceptIO.hxx\"\n"
         << "#include\"TFEL/Math/t2tot2.hxx\"\n"
         << "#include\"TFEL/Math/T2toT2/T2toT2ConceptIO.hxx\"\n";
    }
    if ((hasVariableOfType(this->bd, SupportedTypes::STENSOR)) &&
        (hasVariableOfType(this->bd, SupportedTypes::TENSOR))) {
      os << "#include\"TFEL/Math/t2tost2.hxx\"\n"
         << "#include\"TFEL/Math/T2toST2/T2toST2ConceptIO.hxx\"\n"
         << "#include\"TFEL/Math/st2tot2.hxx\"\n"
         << "#include\"TFEL/Math/ST2toT2/ST2toT2ConceptIO.hxx\"\n";
    }
    if ((this->bd.getBehaviourType() ==
         BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) ||
        (this->bd.getBehaviourType() ==
         BehaviourDescription::GENERALBEHAVIOUR)) {
      os << "#include\"TFEL/Math/ST2toST2/ConvertToTangentModuli.hxx\"\n"
         << "#include\"TFEL/Math/ST2toST2/"
            "ConvertSpatialModuliToKirchhoffJaumanRateModuli.hxx\"\n"
         << "#include\"TFEL/Material/"
            "FiniteStrainBehaviourTangentOperator.hxx\"\n";
    }
    os << "#include\"TFEL/Material/ModellingHypothesis.hxx\"\n\n";
  }  // end of writeBehaviourDataStandardTFELIncludes

  void BehaviourCodeGeneratorBase::writeBehaviourDataDefaultMembers(
      std::ostream& os) const {
    this->checkBehaviourDataFile(os);
    if (this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                              false)) {
      os << "//! stiffness tensor computed by the calling solver\n"
         << "StiffnessTensor D;\n";
    }
    if (this->bd.getAttribute(
            BehaviourDescription::requiresThermalExpansionCoefficientTensor,
            false)) {
      os << "ThermalExpansionCoefficientTensor A;\n";
    }
    for (const auto& mv : this->bd.getMainVariables()) {
      if (mv.first.arraySize != mv.second.arraySize) {
        tfel::raise(
            "BehaviourCodeGeneratorBase::writeBehaviourDataDefaultMembers: "
            "the array size of the gradient '" +
            mv.first.name +
            "' is "
            "different from the array size of the thermodynamic force '" +
            mv.second.name + "'");
      }
      if (mv.first.arraySize == 1) {
        if (Gradient::isIncrementKnown(mv.first)) {
          os << mv.first.type << " " << mv.first.name << ";\n\n";
        } else {
          os << mv.first.type << " " << mv.first.name << "0;\n\n";
        }
        os << mv.second.type << " " << mv.second.name << ";\n\n";
      } else {
        if (Gradient::isIncrementKnown(mv.first)) {
          os << "tfel::math::fsarray<" << mv.first.arraySize << ", "
             << mv.first.type << "> " << mv.first.name << ";\n\n";
        } else {
          os << "tfel::math::fsarray<" << mv.first.arraySize << ", "
             << mv.first.type << "> " << mv.first.name << "0;\n\n";
        }
        os << "tfel::math::fsarray<" << mv.second.arraySize << ", "
           << mv.second.type << "> " << mv.second.name << ";\n\n";
      }
    }
  }

  void BehaviourCodeGeneratorBase::writeBehaviourDataTypeAliases(
      std::ostream& os) const {
    this->checkBehaviourDataFile(os);
    os << "static constexpr unsigned short TVectorSize = N;\n"
       << "using StensorDimeToSize = tfel::math::StensorDimeToSize<N>;\n"
       << "static constexpr unsigned short StensorSize = "
       << "StensorDimeToSize::value;\n"
       << "using TensorDimeToSize = tfel::math::TensorDimeToSize<N>;\n"
       << "static constexpr unsigned short TensorSize = "
       << "TensorDimeToSize::value;\n\n";
    this->writeTypeAliases(os);
    os << '\n';
  }

  void BehaviourCodeGeneratorBase::writeBehaviourDataDisabledConstructors(
      std::ostream& os) const {
    this->checkBehaviourDataFile(os);
  }

  void BehaviourCodeGeneratorBase::writeBehaviourDataConstructors(
      std::ostream& os, const Hypothesis h) const {
    const auto& md = this->bd.getBehaviourData(h);
    this->checkBehaviourDataFile(os);
    os << "/*!\n"
       << "* \\brief Default constructor\n"
       << "*/\n"
       << this->bd.getClassName() << "BehaviourData()\n"
       << "{}\n\n"
       << "/*!\n"
       << "* \\brief copy constructor\n"
       << "*/\n"
       << this->bd.getClassName() << "BehaviourData(const "
       << this->bd.getClassName() << "BehaviourData& src)\n"
       << ": ";
    auto first = true;
    if (this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                              false)) {
      os << "D(src.D)";
      first = false;
    }
    if (this->bd.getAttribute(
            BehaviourDescription::requiresThermalExpansionCoefficientTensor,
            false)) {
      if (!first) {
        os << ",\n";
      }
      os << "A(src.A)";
      first = false;
    }
    for (const auto& mv : this->bd.getMainVariables()) {
      if (!first) {
        os << ",\n";
      }
      if (Gradient::isIncrementKnown(mv.first)) {
        os << mv.first.name << "(src." << mv.first.name << "),\n";
      } else {
        os << mv.first.name << "0(src." << mv.first.name << "0),\n";
      }
      os << mv.second.name << "(src." << mv.second.name << ")";
      first = false;
    }
    for (const auto& v : md.getMaterialProperties()) {
      if (!first) {
        os << ",\n";
      }
      os << v.name << "(src." << v.name << ")";
      first = false;
    }
    for (const auto& v : md.getStateVariables()) {
      if (!first) {
        os << ",\n";
      }
      os << v.name << "(src." << v.name << ")";
      first = false;
    }
    for (const auto& v : md.getAuxiliaryStateVariables()) {
      if (!first) {
        os << ",\n";
      }
      os << v.name << "(src." << v.name << ")";
      first = false;
    }
    for (const auto& v : md.getExternalStateVariables()) {
      if (!first) {
        os << ",\n";
      }
      os << v.name << "(src." << v.name << ")";
      first = false;
    }
    os << "\n{}\n\n";
    // Creating constructor for external interfaces
    for (const auto& i : this->interfaces) {
      if (i.second->isBehaviourConstructorRequired(h, this->bd)) {
        i.second->writeBehaviourDataConstructor(os, h, this->bd);
      }
    }
  }  // end of writeBehaviourDataConstructors

  void BehaviourCodeGeneratorBase::writeBehaviourDataAssignementOperator(
      std::ostream& os, const Hypothesis h) const {
    const auto& md = this->bd.getBehaviourData(h);
    this->checkBehaviourDataFile(os);
    os << "/*\n"
       << "* \\brief Assignement operator\n"
       << "*/\n"
       << this->bd.getClassName() << "BehaviourData&\n"
       << "operator=(const " << this->bd.getClassName()
       << "BehaviourData& src){\n";
    for (const auto& dv : this->bd.getMainVariables()) {
      if (Gradient::isIncrementKnown(dv.first)) {
        os << "this->" << dv.first.name << " = src." << dv.first.name << ";\n";
      } else {
        os << "this->" << dv.first.name << "0 = src." << dv.first.name
           << "0;\n";
      }
      os << "this->" << dv.second.name << " = src." << dv.second.name << ";\n";
    }
    for (const auto& mp : md.getMaterialProperties()) {
      os << "this->" << mp.name << " = src." << mp.name << ";\n";
    }
    for (const auto& iv : md.getStateVariables()) {
      os << "this->" << iv.name << " = src." << iv.name << ";\n";
    }
    for (const auto& iv : md.getAuxiliaryStateVariables()) {
      os << "this->" << iv.name << " = src." << iv.name << ";\n";
    }
    for (const auto& ev : md.getExternalStateVariables()) {
      os << "this->" << ev.name << " = src." << ev.name << ";\n";
    }
    os << "return *this;\n"
       << "}\n\n";
  }  // end of writeBehaviourAssignementOperator

  void BehaviourCodeGeneratorBase::writeBehaviourDataExport(
      std::ostream& os, const Hypothesis h) const {
    this->checkBehaviourDataFile(os);
    for (const auto& i : this->interfaces) {
      i.second->exportMechanicalData(os, h, this->bd);
    }
  }

  void BehaviourCodeGeneratorBase::writeBehaviourDataInitializeMethods(
      std::ostream& os, const Hypothesis h) const {
    this->checkBehaviourDataFile(os);
    const auto& d = this->bd.getBehaviourData(h);
    for (const auto& n : d.getUserDefinedInitializeCodeBlocksNames()) {
      const auto& c = d.getUserDefinedInitializeCodeBlock(n);
      os << "void initialize" << n << "(){\n"
         << "using namespace std;\n"
         << "using namespace tfel::math;\n"
         << "using std::vector;\n";
      writeMaterialLaws(os, this->bd.getMaterialLaws());
      os << c.code;
      os << "} // end of initialize" << n << "\n\n";
    }
  }  // end of writeBehaviourDataInitializeMethods

  void BehaviourCodeGeneratorBase::writeBehaviourDataPublicMembers(
      std::ostream& os) const {
    this->checkBehaviourDataFile(os);
    if (this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                              false)) {
      os << "StiffnessTensor& getStiffnessTensor()\n"
         << "{\nreturn this->D;\n}\n\n"
         << "const StiffnessTensor& getStiffnessTensor() const\n"
         << "{\nreturn this->D;\n}\n\n";
    }
    if (this->bd.getAttribute(
            BehaviourDescription::requiresThermalExpansionCoefficientTensor,
            false)) {
      os << "ThermalExpansionCoefficientTensor& "
         << "getThermalExpansionCoefficientTensor()\n"
         << "{\nreturn this->A;\n}\n\n"
         << "const ThermalExpansionCoefficientTensor& "
         << "getThermalExpansionCoefficientTensor() const\n"
         << "{\nreturn this->A;\n}\n\n";
    }
  }  // end of writeBehaviourDataPublicMembers

  void BehaviourCodeGeneratorBase::writeBehaviourDataClassHeader(
      std::ostream& os) const {
    this->checkBehaviourDataFile(os);
    os << "/*!\n"
       << "* \\class " << this->bd.getClassName() << "BehaviourData\n"
       << "* \\brief This class implements the " << this->bd.getClassName()
       << "BehaviourData"
       << " .\n"
       << "* \\tparam H: modelling hypothesis.\n"
       << "* \\tparam NumericType: numerical type.\n"
       << "* \\tparam use_qt: conditional saying if quantities are use.\n";
    if (!this->fd.authorName.empty()) {
      os << "* \\author " << this->fd.authorName << '\n';
    }
    if (!this->fd.date.empty()) {
      os << "* \\date   " << this->fd.date << '\n';
    }
    os << "*/\n";
  }

  void BehaviourCodeGeneratorBase::writeBehaviourDataForwardDeclarations(
      std::ostream& os) const {
    this->checkBehaviourDataFile(os);
    os << "//! \\brief forward declaration\n"
       << "template<ModellingHypothesis::Hypothesis "
          "hypothesis,typename,bool>\n"
       << "class " << this->bd.getClassName() << "BehaviourData;\n\n"
       << "//! \\brief forward declaration\n"
       << "template<ModellingHypothesis::Hypothesis hypothesis, "
       << "typename NumericType,bool use_qt>\n"
       << "class " << this->bd.getClassName() << "IntegrationData;\n\n";
    if (this->bd.useQt()) {
      os << "//! \\brief forward declaration\n";
      os << "template<ModellingHypothesis::Hypothesis hypothesis, "
         << "typename NumericType, bool use_qt>\n";
      os << "std::ostream&\n operator <<(std::ostream&,";
      os << "const " << this->bd.getClassName()
         << "BehaviourData<hypothesis, NumericType, use_qt>&);\n\n";
    } else {
      os << "//! \\brief forward declaration\n";
      os << "template<ModellingHypothesis::Hypothesis hypothesis,"
         << "typename NumericType>\n";
      os << "std::ostream&\n operator <<(std::ostream&,";
      os << "const " << this->bd.getClassName()
         << "BehaviourData<hypothesis, NumericType,false>&);\n\n";
    }
    // maintenant, il faut déclarer toutes les spécialisations partielles...
    for (const auto& h : this->bd.getModellingHypotheses()) {
      if (this->bd.hasSpecialisedMechanicalData(h)) {
        if (this->bd.useQt()) {
          os << "//! \\brief forward declaration\n"
             << "template<typename NumericType,bool use_qt>\n"
             << "std::ostream&\n operator <<(std::ostream&,"
             << "const " << this->bd.getClassName()
             << "BehaviourData<ModellingHypothesis::"
             << ModellingHypothesis::toUpperCaseString(h)
             << ", NumericType, use_qt>&);\n\n";
        } else {
          os << "//! \\brief forward declaration\n"
             << "template<typename NumericType>\n"
             << "std::ostream&\n operator <<(std::ostream&,"
             << "const " << this->bd.getClassName()
             << "BehaviourData<ModellingHypothesis::"
             << ModellingHypothesis::toUpperCaseString(h)
             << ", NumericType, false>&);\n\n";
        }
      }
    }
  }

  void BehaviourCodeGeneratorBase::writeBehaviourDataClassBegin(
      std::ostream& os, const Hypothesis h) const {
    this->checkBehaviourDataFile(os);
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      if (this->bd.useQt()) {
        os << "template<ModellingHypothesis::Hypothesis hypothesis,"
           << "typename NumericType,bool use_qt>\n";
        os << "class " << this->bd.getClassName() << "BehaviourData\n";
      } else {
        os << "template<ModellingHypothesis::Hypothesis hypothesis, "
           << "typename NumericType>\n";
        os << "class " << this->bd.getClassName()
           << "BehaviourData<hypothesis, NumericType,false>\n";
      }
    } else {
      if (this->bd.useQt()) {
        os << "template<typename NumericType,bool use_qt>\n";
        os << "class " << this->bd.getClassName()
           << "BehaviourData<ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h)
           << ", NumericType, use_qt>\n";
      } else {
        os << "template<typename NumericType>\n";
        os << "class " << this->bd.getClassName()
           << "BehaviourData<ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h)
           << ", NumericType, false>\n";
      }
    }
    os << "{\n\n";
    if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      os << "static constexpr ModellingHypothesis::Hypothesis hypothesis = "
         << "ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h)
         << ";\n";
    }
    os << "static constexpr unsigned short N = "
       << "ModellingHypothesisToSpaceDimension<hypothesis>::value;\n"
       << "static_assert(N==1||N==2||N==3);\n"
       << "static_assert(tfel::typetraits::"
       << "IsFundamentalNumericType<NumericType>::cond);\n"
       << "static_assert(tfel::typetraits::IsReal<NumericType>::cond);\n\n"
       << "friend std::ostream& operator<< <>(std::ostream&,const "
       << this->bd.getClassName() << "BehaviourData&);\n\n"
       << "/* integration data is declared friend to access"
       << "   driving variables at the beginning of the time step */\n";
    if (this->bd.useQt()) {
      os << "friend class " << this->bd.getClassName()
         << "IntegrationData<hypothesis, NumericType, use_qt>;\n\n";
    } else {
      os << "friend class " << this->bd.getClassName()
         << "IntegrationData<hypothesis, NumericType, false>;\n\n";
    }
  }

  void BehaviourCodeGeneratorBase::writeBehaviourDataClassEnd(
      std::ostream& os) const {
    this->checkBehaviourDataFile(os);
    os << "}; // end of " << this->bd.getClassName() << "BehaviourData"
       << " class\n\n";
  }

  void BehaviourCodeGeneratorBase::writeBehaviourDataMaterialProperties(
      std::ostream& os, const Hypothesis h) const {
    this->checkBehaviourDataFile(os);
    this->writeVariablesDeclarations(
        os, this->bd.getBehaviourData(h).getMaterialProperties(), "", "",
        this->fd.fileName, false);
    os << '\n';
  }

  void BehaviourCodeGeneratorBase::writeBehaviourDataStateVariables(
      std::ostream& os, const Hypothesis h) const {
    this->checkBehaviourDataFile(os);
    const auto& d = this->bd.getBehaviourData(h);
    this->writeVariablesDeclarations(os, d.getStateVariables(), "", "",
                                     this->fd.fileName, false);
    this->writeVariablesDeclarations(os, d.getAuxiliaryStateVariables(), "", "",
                                     this->fd.fileName, false);
    this->writeVariablesDeclarations(os, d.getExternalStateVariables(), "", "",
                                     this->fd.fileName, false);
    os << '\n';
  }

  void BehaviourCodeGeneratorBase::writeBehaviourDataOutputOperator(
      std::ostream& os, const Hypothesis h) const {
    this->checkBehaviourFile(os);
    const auto& d = this->bd.getBehaviourData(h);
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      if (this->bd.useQt()) {
        os << "template<ModellingHypothesis::Hypothesis hypothesis,"
           << "typename NumericType, bool use_qt>\n";
        os << "std::ostream&\n";
        os << "operator <<(std::ostream& os,";
        os << "const " << this->bd.getClassName()
           << "BehaviourData<hypothesis, NumericType, use_qt>& b)\n";
      } else {
        os << "template<ModellingHypothesis::Hypothesis hypothesis, "
           << "typename NumericType>\n";
        os << "std::ostream&\n";
        os << "operator <<(std::ostream& os,";
        os << "const " << this->bd.getClassName()
           << "BehaviourData<hypothesis, NumericType, false>& b)\n";
      }
    } else {
      if (this->bd.useQt()) {
        os << "template<typename NumericType, bool use_qt>\n";
        os << "std::ostream&\n";
        os << "operator <<(std::ostream& os,";
        os << "const " << this->bd.getClassName()
           << "BehaviourData<ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h)
           << ", NumericType, use_qt>& b)\n";
      } else {
        os << "template<typename NumericType>\n";
        os << "std::ostream&\n";
        os << "operator <<(std::ostream& os,";
        os << "const " << this->bd.getClassName()
           << "BehaviourData<ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h)
           << ", NumericType, false>& b)\n";
      }
    }
    os << "{\n";
    for (const auto& v : this->bd.getMainVariables()) {
      if (Gradient::isIncrementKnown(v.first)) {
        os << "os << \"" << displayName(v.first) << " : \" << b."
           << v.first.name << " << '\\n';\n";
      } else {
        if (getUnicodeOutputOption()) {
          os << "os << \"" << displayName(v.first) << "\u2080 : \" << b."
             << v.first.name << "0 << '\\n';\n";
        } else {
          os << "os << \"" << displayName(v.first) << "0 : \" << b."
             << v.first.name << "0 << '\\n';\n";
        }
      }
      os << "os << \"" << displayName(v.second) << " : \" << b."
         << v.second.name << " << '\\n';\n";
    }
    for (const auto& v : d.getMaterialProperties()) {
      os << "os << \"" << displayName(v) << " : \" << b." << v.name
         << " << '\\n';\n";
    }
    for (const auto& v : d.getStateVariables()) {
      os << "os << \"" << displayName(v) << " : \" << b." << v.name
         << " << '\\n';\n";
    }
    for (const auto& v : d.getAuxiliaryStateVariables()) {
      os << "os << \"" << displayName(v) << " : \" << b." << v.name
         << " << '\\n';\n";
    }
    for (const auto& v : d.getExternalStateVariables()) {
      os << "os << \"" << displayName(v) << " : \" << b." << v.name
         << " << '\\n';\n";
    }
    os << "return os;\n"
       << "}\n\n";
  }  //  BehaviourCodeGeneratorBase::writeBehaviourDataOutputOperator

  void BehaviourCodeGeneratorBase::writeBehaviourDataFileBegin(
      std::ostream& os) const {
    this->checkBehaviourDataFile(os);
    this->writeBehaviourDataFileHeader(os);
    this->writeBehaviourDataFileHeaderBegin(os);
    this->writeBehaviourDataStandardTFELIncludes(os);
    this->writeIncludes(os);
    // includes specific to interfaces
    for (const auto& i : this->interfaces) {
      i.second->writeInterfaceSpecificIncludes(os, this->bd);
    }
    this->writeNamespaceBegin(os);
    this->writeBehaviourDataForwardDeclarations(os);
  }  // end of writeBehaviourDataFile

  void BehaviourCodeGeneratorBase::writeBehaviourDataClass(
      std::ostream& os, const Hypothesis h) const {
    this->checkBehaviourDataFile(os);
    this->writeBehaviourDataClassBegin(os, h);
    this->writeBehaviourDataTypeAliases(os);
    os << "protected:\n\n";
    this->writeBehaviourDataDefaultMembers(os);
    this->writeBehaviourDataMaterialProperties(os, h);
    this->writeBehaviourDataStateVariables(os, h);
    os << "public:\n\n";
    this->writeBehaviourDataDisabledConstructors(os);
    this->writeBehaviourDataConstructors(os, h);
    this->writeBehaviourDataMainVariablesSetters(os);
    this->writeBehaviourDataPublicMembers(os);
    this->writeBehaviourDataAssignementOperator(os, h);
    this->writeBehaviourDataInitializeMethods(os, h);
    this->writeBehaviourDataExport(os, h);
    this->writeBehaviourDataClassEnd(os);
    this->writeBehaviourDataOutputOperator(os, h);
  }

  void BehaviourCodeGeneratorBase::writeBehaviourDataFileEnd(
      std::ostream& os) const {
    this->writeNamespaceEnd(os);
    this->writeBehaviourDataFileHeaderEnd(os);
  }  // end of writeBehaviourDataFileEnd

  void BehaviourCodeGeneratorBase::checkBehaviourFile(std::ostream& os) const {
    if ((!os) || (!os.good())) {
      this->throwRuntimeError(
          "BehaviourCodeGeneratorBase::checkBehaviourDataOutputFile",
          "output file is not valid");
    }
  }

  void BehaviourCodeGeneratorBase::writeBehaviourForwardDeclarations(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    os << "//! \\brief forward declaration\n"
       << "template<ModellingHypothesis::Hypothesis, "
       << "typename NumericType, bool use_qt>\n"
       << "struct " << this->bd.getClassName() << ";\n\n";
    if (this->bd.useQt()) {
      os << "//! \\brief forward declaration\n"
         << "template<ModellingHypothesis::Hypothesis hypothesis, "
         << "typename NumericType, bool use_qt>\n"
         << "std::ostream&\n operator <<(std::ostream&,"
         << "const " << this->bd.getClassName()
         << "<hypothesis, NumericType, use_qt>&);\n\n";
    } else {
      os << "//! \\brief forward declaration\n"
         << "template<ModellingHypothesis::Hypothesis hypothesis, "
         << "typename NumericType>\n"
         << "std::ostream&\n operator <<(std::ostream&,"
         << "const " << this->bd.getClassName()
         << "<hypothesis, NumericType, false>&);\n\n";
    }
    // maintenant, il faut déclarer toutes les spécialisations partielles...
    const auto& mh = this->bd.getModellingHypotheses();
    for (const auto& h : mh) {
      if (this->bd.hasSpecialisedMechanicalData(h)) {
        if (this->bd.useQt()) {
          os << "//! \\brief forward declaration\n"
             << "template<typename NumericType,bool use_qt>\n"
             << "std::ostream&\n operator <<(std::ostream&,"
             << "const " << this->bd.getClassName() << "<ModellingHypothesis::"
             << ModellingHypothesis::toUpperCaseString(h)
             << ", NumericType, use_qt>&);\n\n";
        } else {
          os << "//! \\brief forward declaration\n"
             << "template<typename NumericType>\n"
             << "std::ostream&\n operator <<(std::ostream&,"
             << "const " << this->bd.getClassName() << "<ModellingHypothesis::"
             << ModellingHypothesis::toUpperCaseString(h)
             << ", NumericType, false>&);\n\n";
        }
      }
    }
  }  // end of writeBehaviourClassForwardDeclarations

  void BehaviourCodeGeneratorBase::writeBehaviourClassBegin(
      std::ostream& os, const Hypothesis h) const {
    this->checkBehaviourFile(os);
    os << "/*!\n";
    os << "* \\class " << this->bd.getClassName() << '\n';
    os << "* \\brief This class implements the " << this->bd.getClassName()
       << " behaviour.\n";
    os << "* \\tparam hypothesis: modelling hypothesis.\n";
    os << "* \\tparam NumericType: numerical type.\n";
    if (this->bd.useQt()) {
      os << "* \\tparam use_qt: conditional saying if quantities are use.\n";
    }
    if (!this->fd.authorName.empty()) {
      os << "* \\author " << this->fd.authorName << '\n';
    }
    if (!this->fd.date.empty()) {
      os << "* \\date   " << this->fd.date << '\n';
    }
    if (!this->fd.description.empty()) {
      os << this->fd.description << '\n';
    }
    os << "*/\n";
    const auto btype = this->bd.getBehaviourTypeFlag();
    const auto final_specifier = this->bd.isFinal() ? "final" : "";
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      if (this->bd.useQt()) {
        os << "template<ModellingHypothesis::Hypothesis hypothesis, "
           << "typename NumericType, bool use_qt>\n"
           << "struct " << this->bd.getClassName() << " " << final_specifier
           << "\n"
           << ": public MechanicalBehaviour<" << btype
           << ",hypothesis, NumericType, use_qt>,\n";
        if (this->bd.getAttribute(BehaviourData::profiling, false)) {
          os << "public " << this->bd.getClassName() << "Profiler,\n";
        }
        os << "public " << this->bd.getClassName()
           << "BehaviourData<hypothesis, NumericType, use_qt>,\n";
        os << "public " << this->bd.getClassName()
           << "IntegrationData<hypothesis, NumericType, use_qt>";
        this->writeBehaviourParserSpecificInheritanceRelationship(os, h);
      } else {
        os << "template<ModellingHypothesis::Hypothesis hypothesis,"
           << "typename NumericType>\n";
        os << "struct " << this->bd.getClassName()
           << "<hypothesis, NumericType, false> " << final_specifier << "\n";
        os << ": public MechanicalBehaviour<" << btype
           << ",hypothesis, NumericType, false>,\n";
        if (this->bd.getAttribute(BehaviourData::profiling, false)) {
          os << "public " << this->bd.getClassName() << "Profiler,\n";
        }
        os << "public " << this->bd.getClassName()
           << "BehaviourData<hypothesis, NumericType, false>,\n";
        os << "public " << this->bd.getClassName()
           << "IntegrationData<hypothesis, NumericType, false>";
        this->writeBehaviourParserSpecificInheritanceRelationship(os, h);
      }
    } else {
      if (this->bd.useQt()) {
        os << "template<typename NumericType,bool use_qt>\n";
        os << "struct " << this->bd.getClassName() << "<ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h)
           << ", NumericType, use_qt> " << final_specifier << "\n";
        os << ": public MechanicalBehaviour<" << btype
           << ",ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h)
           << ", NumericType, use_qt>,\n";
        if (this->bd.getAttribute(BehaviourData::profiling, false)) {
          os << "public " << this->bd.getClassName() << "Profiler,\n";
        }
        os << "public " << this->bd.getClassName()
           << "BehaviourData<ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h)
           << ", NumericType, use_qt>,\n";
        os << "public " << this->bd.getClassName()
           << "IntegrationData<ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h)
           << ", NumericType, use_qt>";
        this->writeBehaviourParserSpecificInheritanceRelationship(os, h);
      } else {
        os << "template<typename NumericType>\n";
        os << "struct " << this->bd.getClassName() << "<ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h)
           << ", NumericType, false> " << final_specifier << "\n";
        os << ": public MechanicalBehaviour<" << btype
           << ",ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h)
           << ", NumericType, false>,\n";
        if (this->bd.getAttribute(BehaviourData::profiling, false)) {
          os << "public " << this->bd.getClassName() << "Profiler,\n";
        }
        os << "public " << this->bd.getClassName()
           << "BehaviourData<ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h)
           << ", NumericType, false>,\n";
        os << "public " << this->bd.getClassName()
           << "IntegrationData<ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h)
           << ", NumericType, false>";
        this->writeBehaviourParserSpecificInheritanceRelationship(os, h);
      }
    }
    os << "{\n\n";
    if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      os << "static constexpr ModellingHypothesis::Hypothesis hypothesis = "
         << "ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h)
         << ";\n";
    }
    os << "static constexpr unsigned short N = "
          "ModellingHypothesisToSpaceDimension<hypothesis>::value;\n\n";
    os << "static_assert(N==1||N==2||N==3);\n";
    os << "static_assert(tfel::typetraits::"
       << "IsFundamentalNumericType<NumericType>::cond);\n";
    os << "static_assert(tfel::typetraits::IsReal<NumericType>::cond);\n\n";
  }

  void BehaviourCodeGeneratorBase::writeBehaviourFriends(
      std::ostream& os, const Hypothesis) const {
    os << "friend std::ostream& operator<< <>(std::ostream&,const "
       << this->bd.getClassName() << "&);\n\n";
  }  // end of writeBehaviourFriends

  void BehaviourCodeGeneratorBase::writeBehaviourFileHeader(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    os << "/*!\n"
       << "* \\file   " << this->bd.getBehaviourFileName() << '\n'
       << "* \\brief  "
       << "this file implements the " << this->bd.getClassName()
       << " Behaviour.\n"
       << "*         File generated by " << MFrontHeader::getVersionName()
       << " "
       << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!this->fd.authorName.empty()) {
      os << "* \\author " << this->fd.authorName << '\n';
    }
    if (!this->fd.date.empty()) {
      os << "* \\date   " << this->fd.date << '\n';
    }
    os << " */\n\n";
  }

  void BehaviourCodeGeneratorBase::writeBehaviourFileHeaderBegin(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    const auto cn = this->bd.getClassName();
    this->writeHeaderGuardBegin(os, cn, true);
  }

  void BehaviourCodeGeneratorBase::writeBehaviourFileHeaderEnd(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    const auto cn = this->bd.getClassName();
    this->writeHeaderGuardEnd(os, cn, true);
  }

  void BehaviourCodeGeneratorBase::writeBehaviourClassEnd(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    os << "}; // end of " << this->bd.getClassName() << " class\n\n";
  }

  void BehaviourCodeGeneratorBase::writeBehaviourUpdateIntegrationVariables(
      std::ostream& os, const Hypothesis h) const {
    const auto& d = this->bd.getBehaviourData(h);
    this->checkBehaviourFile(os);
    os << "/*!\n"
       << "* \\brief Update internal variables at end of integration\n"
       << "*/\n"
       << "TFEL_HOST_DEVICE void updateIntegrationVariables()";
    if (!d.getIntegrationVariables().empty()) {
      os << "{\n";
      for (const auto& v : d.getIntegrationVariables()) {
        if (!d.isStateVariableName(v.name)) {
          if (d.isMemberUsedInCodeBlocks(v.name)) {
            os << "this->" << v.name << " += "
               << "this->d" << v.name << ";\n";
          }
        }
      }
      os << "}\n\n";
    } else {
      os << "\n{}\n\n";
    }
  }  // end of writeBehaviourUpdateIntegrationVariables

  void BehaviourCodeGeneratorBase::writeBehaviourUpdateStateVariables(
      std::ostream& os, const Hypothesis h) const {
    const auto& d = this->bd.getBehaviourData(h);
    this->checkBehaviourFile(os);
    os << "/*!\n"
       << "* \\brief Update internal variables at end of integration\n"
       << "*/\n"
       << "TFEL_HOST_DEVICE void updateStateVariables()";
    if (!d.getStateVariables().empty()) {
      os << "{\n";
      for (const auto& v : d.getStateVariables()) {
        os << "this->" << v.name << " += "
           << "this->d" << v.name << ";\n";
      }
      os << "}\n\n";
    } else {
      os << "\n{}\n\n";
    }
  }  // end of writeBehaviourUpdateStateVariables

  void BehaviourCodeGeneratorBase::writeBehaviourUpdateAuxiliaryStateVariables(
      std::ostream& os, const Hypothesis h) const {
    using ExternalModel =
        BehaviourDescription::ExternalModelBasedOnBehaviourVariableFactory;
    auto tmpnames = std::vector<std::string>{};
    os << "/*!\n"
       << " * \\brief Update auxiliary state variables at end of integration\n"
       << " */\n"
       << "TFEL_HOST_DEVICE [[nodiscard]] bool updateAuxiliaryStateVariables()"
       << "{\n"
       << "using namespace std;\n"
       << "using namespace tfel::math;\n";
    writeMaterialLaws(os, this->bd.getMaterialLaws());
    for (const auto& m : this->bd.getModelsDescriptions()) {
      if (std::holds_alternative<ModelDescription>(m)) {
        const auto& md = std::get<ModelDescription>(m);
        for (const auto& output : md.outputs) {
          const auto vn = output.name;
          os << "this->" << vn << " += this->d" << vn << ";\n";
        }
      } else {
        const auto& md = std::get<ExternalModel>(m);
        const auto& f =
            this->bd.getBehaviourData(h).getBehaviourVariableFactory(
                md.factory);
        for (const auto& v :
             f.behaviour.getBehaviourData(h).getPersistentVariables()) {
          const auto vn = applyNamesChanges(f, v);
          os << "this->" << vn.name << " += this->d" << vn.name << ";\n";
        }
      }
    }
    const auto& md = this->bd.getBehaviourData(h);
    for (const auto& b : md.getBehaviourVariables()) {
      if (b.automatically_save_associated_auxiliary_state_variables) {
        os << "this->updateAuxiliaryStateVariables(" << b.name << ");\n";
      }
    }
    if (this->bd.hasCode(h, BehaviourData::UpdateAuxiliaryStateVariables)) {
      os << this->bd.getCode(h, BehaviourData::UpdateAuxiliaryStateVariables)
         << "\n";
    }
    for (const auto& m : this->bd.getAuxiliaryModelsDescriptions()) {
      if (std::holds_alternative<ModelDescription>(m)) {
        const auto& model = std::get<ModelDescription>(m);
        auto inputs = std::vector<std::string>{};
        auto outputs = std::vector<std::string>{};
        for (const auto& v : model.outputs) {
          inputs.push_back(v.name);
          outputs.push_back(v.name);
        }
        this->writeModelCall(os, tmpnames, h, model, outputs, inputs, "em");
      } else {
        const auto& model = std::get<ExternalModel>(m);
        const auto& f =
            this->bd.getBehaviourData(h).getBehaviourVariableFactory(
                model.factory);
        const auto instance = "mfront_auxiliary_model_" + f.name + "_instance";
        os << "auto " << instance << " = " << f.name << ".make();\n"
           << "if(!this->initialize(" << instance << ")){\n"
           << "return false;\n"
           << "}\n"
           << "if(!" << instance << ". integrate("
           << "::tfel::material::TangentOperatorTraits<::tfel::"
           << "material::MechanicalBehaviourBase::GENERALBEHAVIOUR>::"
           << "STANDARDTANGENTOPERATOR, NOSTIFFNESSREQUESTED)){\n"
           << "return false;\n"
           << "}\n";
        for (const auto& v :
             f.behaviour.getBehaviourData(h).getPersistentVariables()) {
          const auto vn = applyNamesChanges(f, v);
          os << "this->updateAuxiliaryStateVariables(" << instance << ");\n";
        }
      }
    }
    os << "return true;\n"
       << "}\n\n";
  }  // end of writeBehaviourUpdateAuxiliaryStateVariables

  void BehaviourCodeGeneratorBase::writeBehaviourComputeInternalEnergy(
      std::ostream& os, const Hypothesis h) const {
    os << "/*!\n"
       << "* \\brief Update the internal energy at end of the time step\n"
       << "* \\param[in] Psi_s: internal energy at end of the time step\n"
       << "*/\n"
       << "TFEL_HOST_DEVICE void computeInternalEnergy(stress& Psi_s) const";
    if (this->bd.hasCode(h, BehaviourData::ComputeInternalEnergy)) {
      os << "{\n"
         << "using namespace std;\n"
         << "using namespace tfel::math;\n";
      writeMaterialLaws(os, this->bd.getMaterialLaws());
      os << this->bd.getCode(h, BehaviourData::ComputeInternalEnergy)
         << "\n}\n\n";
    } else {
      os << "\n{\nPsi_s=stress{0};\n}\n\n";
    }
  }  // end of writeBehaviourComputeInternalEnergy

  void BehaviourCodeGeneratorBase::writeBehaviourComputeDissipatedEnergy(
      std::ostream& os, const Hypothesis h) const {
    os << "/*!\n"
       << "* \\brief Update the dissipated energy at end of the time step\n"
       << "* \\param[in] Psi_d: dissipated energy at end of the time step\n"
       << "*/\n"
       << "TFEL_HOST_DEVICE void computeDissipatedEnergy(stress& Psi_d) "
          "const";
    if (this->bd.hasCode(h, BehaviourData::ComputeDissipatedEnergy)) {
      os << "{\n"
         << "using namespace std;\n"
         << "using namespace tfel::math;\n";
      writeMaterialLaws(os, this->bd.getMaterialLaws());
      os << this->bd.getCode(h, BehaviourData::ComputeDissipatedEnergy)
         << "\n}\n\n";
    } else {
      os << "\n{\nPsi_d=stress{0};\n}\n\n";
    }
  }  // end of writeBehaviourComputeDissipatedEnergy

  void BehaviourCodeGeneratorBase::writeBehaviourComputeSpeedOfSound(
      std::ostream& os, const Hypothesis h) const {
    os << "/*!\n"
       << "* \\brief compute the sound velocity\n"
       << "* \\param[in] rho_m0: mass density in the reference "
          "configuration\n"
       << "*/\n";
    if (this->bd.hasCode(h, BehaviourData::ComputeSpeedOfSound)) {
      const auto vs = tfel::unicode::getMangledString("vₛ");
      const auto rho_m0 = tfel::unicode::getMangledString("ρₘ₀");
      os << "TFEL_HOST_DEVICE speed computeSpeedOfSound(const massdensity& "
            "rho_m0) const {\n"
         << "using namespace std;\n"
         << "using namespace tfel::math;\n";
      writeMaterialLaws(os, this->bd.getMaterialLaws());
      os << "const auto " << rho_m0 << " = rho_m0;\n"
         << "auto v_sound = speed{};\n"
         << "auto& " << vs << " = v_sound;\n"
         << this->bd.getCode(h, BehaviourData::ComputeSpeedOfSound)
         << "static_cast<void>(" << rho_m0 << ");\n"
         << "static_cast<void>(" << vs << ");\n"
         << "return v_sound;\n"
         << "}\n\n";
    } else {
      os << "TFEL_HOST_DEVICE speed computeSpeedOfSound(const massdensity&) "
            "const {\n"
         << "return speed(0);\n"
         << "\n}\n\n";
    }
  }  // end of writeBehaviourComputeSpeedOfSound

  void BehaviourCodeGeneratorBase::writeBehaviourIntegrator(
      std::ostream& os, const Hypothesis h) const {
    const auto btype = this->bd.getBehaviourTypeFlag();
    this->checkBehaviourFile(os);
    os << "/*!\n"
       << "* \\brief Integrate behaviour  over the time step\n"
       << "*/\n";
    if (!this->bd.getTangentOperatorBlocks().empty()) {
      os << "[[nodiscard]] IntegrationResult\n"
         << "integrate(const SMFlag smflag, const SMType smt) override "
            "final{\n";
    } else {
      os << "[[nodiscard]] IntegrationResult\n"
         << "integrate(const SMFlag, const SMType) override final{\n";
    }
    os << "using namespace std;\n"
       << "using namespace tfel::math;\n";
    writeMaterialLaws(os, this->bd.getMaterialLaws());
    if (!this->bd.getTangentOperatorBlocks().empty()) {
      if ((this->bd.getBehaviourType() ==
           BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
          (this->bd.getBehaviourType() ==
           BehaviourDescription::COHESIVEZONEMODEL) ||
          (this->bd.getBehaviourType() ==
           BehaviourDescription::GENERALBEHAVIOUR)) {
        if (this->bd.useQt()) {
          os << "raise_if(smflag!=MechanicalBehaviour<" << btype
             << ",hypothesis, NumericType, "
                "use_qt>::STANDARDTANGENTOPERATOR,\n"
             << "\"invalid tangent operator flag\");\n";
        } else {
          os << "raise_if(smflag!=MechanicalBehaviour<" << btype
             << ",hypothesis, NumericType, false>::STANDARDTANGENTOPERATOR,\n"
             << "\"invalid tangent operator flag\");\n";
        }
      }
      os << "bool computeTangentOperator_ = smt!=NOSTIFFNESSREQUESTED;\n";
    }
    if (this->bd.hasCode(h, BehaviourData::ComputePredictor)) {
      os << this->bd.getCode(h, BehaviourData::ComputePredictor) << '\n';
    }
    if (this->bd.hasCode(h, BehaviourData::Integrator)) {
      os << this->bd.getCode(h, BehaviourData::Integrator) << '\n';
    }
    os << "this->updateIntegrationVariables();\n"
       << "this->updateStateVariables();\n"
       << "if(!this->updateAuxiliaryStateVariables()){\n";
    if (this->bd.useQt()) {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType, use_qt>::FAILURE;\n";
    } else {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType, false>::FAILURE;\n";
    }
    os << "}\n";
    if (!areRuntimeChecksDisabled(this->bd)) {
      for (const auto& v :
           this->bd.getBehaviourData(h).getPersistentVariables()) {
        this->writePhysicalBoundsChecks(os, v, false);
      }
      for (const auto& v :
           this->bd.getBehaviourData(h).getPersistentVariables()) {
        this->writeBoundsChecks(os, v, false);
      }
    }
    if (!this->bd.getTangentOperatorBlocks().empty()) {
      if (hasUserDefinedTangentOperatorCode(this->bd, h)) {
        os << "if(computeTangentOperator_){\n";
        if (this->bd.getBehaviourType() ==
            BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
          os << "if(!this->computeConsistentTangentOperator(smflag,smt)){\n";
        } else {
          os << "if(!this->computeConsistentTangentOperator(smt)){\n";
        }
        if (this->bd.useQt()) {
          os << "return MechanicalBehaviour<" << btype
             << ",hypothesis, NumericType, use_qt>::FAILURE;\n";
        } else {
          os << "return MechanicalBehaviour<" << btype
             << ",hypothesis, NumericType, false>::FAILURE;\n";
        }
        os << "}\n"
           << "}\n";
      }
    }
    if (this->bd.useQt()) {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType, use_qt>::SUCCESS;\n";
    } else {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType, false>::SUCCESS;\n";
    }
    os << "}\n\n";
  }

  void BehaviourCodeGeneratorBase::writeBehaviourDisabledConstructors(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    if (!this->bd.shallDefineDefaultConstructor()) {
      os << "//! \\brief default constructor (disabled)\n"
         << this->bd.getClassName() << "() = delete ;\n";
    }
    os << "//! \\brief move constructor (disabled)\n"
       << this->bd.getClassName() << "(" << this->bd.getClassName()
       << "&&) = delete;\n"
       << "//! \\brief copy constructor (disabled)\n"
       << this->bd.getClassName() << "(const " << this->bd.getClassName()
       << "&) = delete;\n";
  }  // end of writeBehaviourDisabledConstructors

  void BehaviourCodeGeneratorBase::writeBehaviourDisabledAssignementOperators(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    os << "//! \\brief assignement operator (disabled)\n"
       << this->bd.getClassName() << "& operator = (const "
       << this->bd.getClassName() << "&) = default;\n\n"
       << "//! \\brief assignement operator (disabled)\n"
       << this->bd.getClassName() << "& operator = (" << this->bd.getClassName()
       << "&&) = default;\n\n";
  }  // end of writeBehaviourDisabledAssignementOperators

  void BehaviourCodeGeneratorBase::writeBehaviourSetOutOfBoundsPolicy(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    os << "/*!\n"
       << " * \\brief set the policy for \"out of bounds\" conditions\n"
       << " */\n";
    if (allowRuntimeModificationOfTheOutOfBoundsPolicy(this->bd)) {
      os << "TFEL_HOST_DEVICE void\n"
         << "setOutOfBoundsPolicy(const OutOfBoundsPolicy "
         << "mfront_policy_value){\n"
         << "  this->policy = mfront_policy_value;\n"
         << "} // end of setOutOfBoundsPolicy\n\n";
    } else {
      os << "TFEL_HOST_DEVICE void\n"
         << "setOutOfBoundsPolicy(const OutOfBoundsPolicy) const {\n"
         << "} // end of setOutOfBoundsPolicy\n\n";
    }
  }  // end of writeBehaviourSetOutOfBoundsPolicy

  void BehaviourCodeGeneratorBase::writeBoundsChecks(
      std::ostream& os, const VariableDescription& v, const bool b) const {
    if (v.arraySize == 1u) {
      mfront::writeBoundsChecks(os, v, v.name, "N", "policy", true, b);
    } else {
      for (unsigned short i = 0; i != v.arraySize; ++i) {
        mfront::writeBoundsChecks(os, v, v.name + '[' + std::to_string(i) + ']',
                                  "N", "policy", true, b);
      }
    }
  }  // end of writeBoundsChecks

  void BehaviourCodeGeneratorBase::writePhysicalBoundsChecks(
      std::ostream& os, const VariableDescription& v, const bool b) const {
    if (v.arraySize == 1u) {
      mfront::writePhysicalBoundsChecks(os, v, v.name, "N", true, b);
    } else {
      for (unsigned short i = 0; i != v.arraySize; ++i) {
        mfront::writePhysicalBoundsChecks(
            os, v, v.name + '[' + std::to_string(i) + ']', "N", true, b);
      }
    }
  }  // end of writePhysicalBoundsChecks

  void BehaviourCodeGeneratorBase::writeBehaviourCheckBounds(
      std::ostream& os, const Hypothesis h) const {
    auto write_physical_bounds =
        [this, &os](const VariableDescriptionContainer& c, const bool b) {
          for (const auto& v : c) {
            this->writePhysicalBoundsChecks(os, v, b);
          }
        };
    auto write_bounds = [this, &os](const VariableDescriptionContainer& c,
                                    const bool b) {
      for (const auto& v : c) {
        this->writeBoundsChecks(os, v, b);
      }
    };
    const auto& md = this->bd.getBehaviourData(h);
    this->checkBehaviourFile(os);
    os << "//! \\brief check physical bounds and standard bounds\n"
       << "TFEL_HOST_DEVICE void checkBounds() const{\n";
    write_physical_bounds(md.getMaterialProperties(), false);
    write_physical_bounds(md.getPersistentVariables(), false);
    write_physical_bounds(md.getExternalStateVariables(), true);
    write_physical_bounds(md.getLocalVariables(), false);
    const auto check_bounds =
        !((!allowRuntimeModificationOfTheOutOfBoundsPolicy(this->bd)) &&
          (getDefaultOutOfBoundsPolicy(this->bd) == tfel::material::None));
    if (check_bounds) {
      write_bounds(md.getMaterialProperties(), false);
      write_bounds(md.getPersistentVariables(), false);
      write_bounds(md.getExternalStateVariables(), true);
      write_bounds(md.getLocalVariables(), false);
    }
    os << "} // end of checkBounds\n\n";
  }  // end of writeBehaviourCheckBounds

  std::string BehaviourCodeGeneratorBase::getBehaviourConstructorsInitializers(
      const Hypothesis h) const {
    // variable initialisation
    auto init = std::string();
    auto append = [&init](const std::string& s) {
      if (s.empty()) {
        return;
      }
      if (!init.empty()) {
        init += ",\n";
      }
      init += s;
    };
    append(this->getIntegrationVariablesIncrementsInitializers(h));
    append(this->getLocalVariablesInitializers(h));
    // tangent operator blocks
    const auto& blocks = this->bd.getTangentOperatorBlocks();
    if (this->bd.hasTrivialTangentOperatorStructure()) {
      tfel::raise_if(
          ((blocks.size() != 1u) || (blocks.front().first.arraySize != 1u) ||
           (blocks.front().second.arraySize != 1u)),
          "BehaviourCodeGeneratorBase::getBehaviourConstructorsInitializers: "
          "internal "
          "error");
      if (this->bd.getBehaviourType() !=
          BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        append(this->bd.getTangentOperatorBlockName(blocks.front()) + "(Dt)");
      }
    } else {
      auto o = SupportedTypes::TypeSize{};
      // write blocks
      for (const auto& b : blocks) {
        const auto& v1 = b.first;
        const auto& v2 = b.second;
        if ((v1.arraySize == 1u) && (v2.arraySize == 1u)) {
          const auto bn = this->bd.getTangentOperatorBlockName(b);
          if ((v1.getTypeFlag() == SupportedTypes::SCALAR) &&
              (v2.getTypeFlag() == SupportedTypes::SCALAR)) {
            append(bn + "(Dt[" + o.asString() + "])");
          } else {
            if (o.isNull()) {
              append(bn + "(Dt.begin())");
            } else {
              append(bn + "(Dt.begin()+" + o.asString() + ")");
            }
          }
        }
        const auto block_size = SupportedTypes::TypeSize::getDerivativeSize(
            SupportedTypes::getTypeSize(v1.type),
            SupportedTypes::getTypeSize(v2.type));
        o += (v1.arraySize) * (v2.arraySize) * block_size;
      }
    }
    return init;
  }  // end of getBehaviourConstructorsInitializers

  std::string BehaviourCodeGeneratorBase::getLocalVariablesInitializers(
      const Hypothesis) const {
    return {};
  }  // end of getLocalVariablesInitializers

  void BehaviourCodeGeneratorBase::writeBehaviourConstructors(
      std::ostream& os, const Hypothesis h) const {
    auto tmpnames = std::vector<std::string>{};
    auto write_body = [this, &os, h] {
      os << "using namespace std;\n"
         << "using namespace tfel::math;\n"
         << "using std::vector;\n";
      writeMaterialLaws(os, this->bd.getMaterialLaws());
      this->writeBehaviourParameterInitialisation(os, h);
    };
    this->checkBehaviourFile(os);
    // initializers
    const auto& init = this->getBehaviourConstructorsInitializers(h);
    // writing constructors
    if (this->bd.shallDefineDefaultConstructor()) {
      os << "//! \\brief default constructor\n"
         << this->bd.getClassName() << "()";
      if (!init.empty()) {
        os << "\n: " << init;
      }
      os << "\n{";
      write_body();
      os << "}\n\n";
    }
    os << "/*!\n"
       << "* \\brief Constructor\n"
       << "*/\n";
    if (this->bd.useQt()) {
      os << this->bd.getClassName() << "("
         << "const " << this->bd.getClassName()
         << "BehaviourData<hypothesis, NumericType, use_qt>& src1,\n"
         << "const " << this->bd.getClassName()
         << "IntegrationData<hypothesis, NumericType, use_qt>& src2)\n"
         << ": " << this->bd.getClassName()
         << "BehaviourData<hypothesis, NumericType, use_qt>(src1),\n"
         << this->bd.getClassName()
         << "IntegrationData<hypothesis, NumericType, use_qt>(src2)";
    } else {
      os << this->bd.getClassName() << "("
         << "const " << this->bd.getClassName()
         << "BehaviourData<hypothesis, NumericType, false>& src1,\n"
         << "const " << this->bd.getClassName()
         << "IntegrationData<hypothesis, NumericType, false>& src2)\n"
         << ": " << this->bd.getClassName()
         << "BehaviourData<hypothesis, NumericType, false>(src1),\n"
         << this->bd.getClassName()
         << "IntegrationData<hypothesis, NumericType, false>(src2)";
    }
    if (!init.empty()) {
      os << ",\n" << init;
    }
    os << "\n{\n";
    write_body();
    os << "}\n\n";
    // constructor specific to interfaces
    for (const auto& i : this->interfaces) {
      if (i.second->isBehaviourConstructorRequired(h, this->bd)) {
        i.second->writeBehaviourConstructorHeader(os, this->bd, h, init);
        os << "\n{\n";
        write_body();
        i.second->writeBehaviourConstructorBody(os, this->bd, h);
        os << "}\n\n";
      }
    }
  }

  void BehaviourCodeGeneratorBase::writeHillTensorComputation(
      std::ostream& out,
      const std::string& H,
      const BehaviourDescription::HillTensor& h,
      std::function<std::string(const MaterialPropertyInput&)>& f) const {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError(
            "BehaviourCodeGeneratorBase::writeHillTensorComputation", m);
      }
    };
    throw_if(this->bd.getSymmetryType() == mfront::ISOTROPIC,
             "material is not orthotropic");
    for (decltype(h.c.size()) i = 0; i != h.c.size(); ++i) {
      this->writeMaterialPropertyCheckBoundsEvaluation(out, h.c[i], f);
    }
    if (this->bd.getOrthotropicAxesConvention() ==
        OrthotropicAxesConvention::PIPE) {
      out << H << " = tfel::material::computeHillTensor<hypothesis,"
          << "OrthotropicAxesConvention::PIPE, real>(";
    } else if (this->bd.getOrthotropicAxesConvention() ==
               OrthotropicAxesConvention::PLATE) {
      out << H << " = tfel::material::computeHillTensor<hypothesis,"
          << "OrthotropicAxesConvention::PLATE, real>(";
    } else {
      out << H << " = tfel::material::computeHillTensor<hypothesis,"
          << "OrthotropicAxesConvention::DEFAULT, real>(";
    }
    for (decltype(h.c.size()) i = 0; i != h.c.size();) {
      this->writeMaterialPropertyEvaluation(out, h.c[i], f);
      if (++i != h.c.size()) {
        out << ",\n";
      }
    }
    out << ");\n";
  }  // end of writeHillTensorComputation

  void BehaviourCodeGeneratorBase::writeStiffnessTensorComputation(
      std::ostream& out,
      const std::string& D,
      std::function<std::string(const MaterialPropertyInput&)>& f) const {
    const auto& emps = this->bd.getElasticMaterialProperties();
    if ((this->bd.getSymmetryType() == mfront::ISOTROPIC) &&
        (this->bd.getElasticSymmetryType() != mfront::ISOTROPIC)) {
      this->throwRuntimeError(
          "BehaviourCodeGeneratorBase::writeStiffnessTensorComputation",
          "inconsistent symmetry type for the material and "
          "the elastic behaviour.");
    }
    bool ua = true;
    if (!this->bd.hasAttribute(
            BehaviourDescription::requiresUnAlteredStiffnessTensor)) {
      const auto& mh = this->bd.getModellingHypotheses();
      if ((mh.find(ModellingHypothesis::PLANESTRESS) != mh.end()) ||
          (mh.find(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) !=
           mh.end())) {
        this->throwRuntimeError(
            "BehaviourCodeGeneratorBase::writeStiffnessTensorComputation",
            "For plane stress hypotheses, it is required to precise whether "
            "the expected stiffness tensor is 'Altered' (the plane stress "
            "hypothesis is taken into account) or 'UnAltered' (the stiffness "
            "tensor is the same as in plane strain). "
            "See the '@ComputeStiffnessTensor' documentation");
      }
    } else {
      ua = this->bd.getAttribute<bool>(
          BehaviourDescription::requiresUnAlteredStiffnessTensor);
    }
    if (this->bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
      if (emps.size() != 2u) {
        this->throwRuntimeError(
            "BehaviourCodeGeneratorBase::writeStiffnessTensorComputation",
            "invalid number of material properties");
      }
      this->writeMaterialPropertyCheckBoundsEvaluation(out, emps[0], f);
      this->writeMaterialPropertyCheckBoundsEvaluation(out, emps[1], f);
      if (ua) {
        out << "tfel::material::computeIsotropicStiffnessTensor<hypothesis,"
               "StiffnessTensorAlterationCharacteristic::"
               "UNALTERED"
            << ">(" << D << ",";
      } else {
        out << "tfel::material::computeIsotropicStiffnessTensor<hypothesis,"
               "StiffnessTensorAlterationCharacteristic::"
               "ALTERED"
            << ">(" << D << ", ";
      }
      out << "stress(";
      this->writeMaterialPropertyEvaluation(out, emps[0], f);
      out << "), \n";
      this->writeMaterialPropertyEvaluation(out, emps[1], f);
      out << ");\n";
    } else if (this->bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
      if (emps.size() != 9u) {
        this->throwRuntimeError(
            "BehaviourCodeGeneratorBase::writeStiffnessTensorComputation",
            "invalid number of material properties");
      }
      for (decltype(emps.size()) i = 0; i != emps.size(); ++i) {
        this->writeMaterialPropertyCheckBoundsEvaluation(out, emps[i], f);
      }
      if (ua) {
        if (this->bd.getOrthotropicAxesConvention() ==
            OrthotropicAxesConvention::PIPE) {
          out << "tfel::material::computeOrthotropicStiffnessTensor<"
                 "hypothesis,"
              << "StiffnessTensorAlterationCharacteristic::UNALTERED,"
              << "OrthotropicAxesConvention::PIPE>(" << D << ",";
        } else if (this->bd.getOrthotropicAxesConvention() ==
                   OrthotropicAxesConvention::PLATE) {
          out << "tfel::material::computeOrthotropicStiffnessTensor<"
                 "hypothesis,"
              << "StiffnessTensorAlterationCharacteristic::UNALTERED,"
              << "OrthotropicAxesConvention::PLATE>(" << D << ",";
        } else {
          out << "tfel::material::computeOrthotropicStiffnessTensor<"
                 "hypothesis,"
              << "StiffnessTensorAlterationCharacteristic::UNALTERED,"
              << "OrthotropicAxesConvention::DEFAULT>(" << D << ",";
        }
      } else {
        if (this->bd.getOrthotropicAxesConvention() ==
            OrthotropicAxesConvention::PIPE) {
          out << "tfel::material::computeOrthotropicStiffnessTensor<"
                 "hypothesis,"
              << "StiffnessTensorAlterationCharacteristic::ALTERED,"
              << "OrthotropicAxesConvention::PIPE>(" << D << ",";
        } else if (this->bd.getOrthotropicAxesConvention() ==
                   OrthotropicAxesConvention::PLATE) {
          out << "tfel::material::computeOrthotropicStiffnessTensor<"
                 "hypothesis,"
              << "StiffnessTensorAlterationCharacteristic::ALTERED,"
              << "OrthotropicAxesConvention::PLATE>(" << D << ",";
        } else {
          out << "tfel::material::computeOrthotropicStiffnessTensor<"
                 "hypothesis,"
              << "StiffnessTensorAlterationCharacteristic::ALTERED,"
              << "OrthotropicAxesConvention::DEFAULT>(" << D << ",";
        }
      }
      for (decltype(emps.size()) i = 0; i != emps.size();) {
        if ((i == 0) || (i == 1) || (i == 2) ||  //
            (i == 6) || (i == 7) || (i == 8)) {
          out << "stress(";
          this->writeMaterialPropertyEvaluation(out, emps[i], f);
          out << ")";
        } else {
          this->writeMaterialPropertyEvaluation(out, emps[i], f);
        }
        if (++i != emps.size()) {
          out << ",\n";
        }
      }
      out << ");\n";
    } else {
      this->throwRuntimeError(
          "BehaviourCodeGeneratorBase::writeStiffnessTensorComputation",
          "unsupported elastic symmetry type");
    }
  }  // end of writeStiffnessTensorComputation

  void BehaviourCodeGeneratorBase::writeExternalMFrontMaterialPropertyArguments(
      std::ostream& out,
      const BehaviourDescription::MaterialProperty& m,
      std::function<std::string(const MaterialPropertyInput&)>& f) const {
    const auto& cmp =
        m.get<BehaviourDescription::ExternalMFrontMaterialProperty>();
    const auto& mpd = *(cmp.mpd);
    out << '(';
    const auto use_qt = this->bd.useQt();
    if (!mpd.inputs.empty()) {
      const auto& inputs = this->bd.getMaterialPropertyInputs(mpd);
      auto pi = std::begin(inputs);
      const auto pie = std::end(inputs);
      while (pi != pie) {
        if (use_qt) {
          out << "tfel::math::base_type_cast(" << f(*pi) << ")";
        } else {
          out << f(*pi);
        }
        if (++pi != pie) {
          out << ",";
        }
      }
    }
    out << ")";
  }

  void BehaviourCodeGeneratorBase::writeMaterialPropertyCheckBoundsEvaluation(
      std::ostream& out,
      const BehaviourDescription::MaterialProperty& m,
      std::function<std::string(const MaterialPropertyInput&)>& f) const {
    if (m.is<BehaviourDescription::ExternalMFrontMaterialProperty>()) {
      const auto& cmp =
          m.get<BehaviourDescription::ExternalMFrontMaterialProperty>();
      const auto& mpd = *(cmp.mpd);
      if ((!hasBounds(mpd.inputs)) && (!(hasPhysicalBounds(mpd.inputs)))) {
        return;
      }
      const auto& n = MFrontMaterialPropertyInterface().getFunctionName(mpd);
      out << "{\n // check bounds for material property '" << n << "'\n"
          << "const auto " << n << "_bounds_check_status = " << n
          << "_checkBounds";
      this->writeExternalMFrontMaterialPropertyArguments(out, cmp, f);
      out << ";\n"
          << "if(" << n << "_bounds_check_status!=0){\n"
          << "// physical bounds\n"
          << "tfel::raise_if<OutOfBoundsException>(" << n
          << "_bounds_check_status<0,\n"
          << "\"" << this->bd.getClassName()
          << ": a variable is out of its physical bounds \"\n"
          << "\"when calling the material property '" << n << "'\");\n"
          << "} else {\n"
          << "// standard bounds\n"
          << "if(this->policy==Strict){\n"
          << "tfel::raise<OutOfBoundsException>(\"" << this->bd.getClassName()
          << ": "
          << "a variable is out of its bounds \"\n"
          << "\"when calling the material property '" << n << "'\");\n"
          << "} else if(this->policy==Warning){\n"
          << "std::cerr << \"" << this->bd.getClassName() << ": "
          << "a variable is out of its bounds \"\n"
          << "\"when calling the material property '" << n << "'\\n\";\n"
          << "}\n"
          << "}\n"
          << "}\n";
    } else if (!((m.is<BehaviourDescription::ConstantMaterialProperty>()) ||
                 (m.is<BehaviourDescription::AnalyticMaterialProperty>()))) {
      this->throwRuntimeError(
          "BehaviourCodeGeneratorBase::"
          "writeMaterialPropertyCheckBoundsEvaluation",
          "unsupported material property type");
    }
  }  // end of writeMaterialPropertyEvaluation

  void BehaviourCodeGeneratorBase::writeMaterialPropertyEvaluation(
      std::ostream& out,
      const BehaviourDescription::MaterialProperty& m,
      std::function<std::string(const MaterialPropertyInput&)>& f) const {
    if (m.is<BehaviourDescription::ConstantMaterialProperty>()) {
      const auto& cmp = m.get<BehaviourDescription::ConstantMaterialProperty>();
      if (!cmp.name.empty()) {
        out << "this->" << cmp.name;
      } else {
        out << cmp.value;
      }
    } else if (m.is<BehaviourDescription::ExternalMFrontMaterialProperty>()) {
      const auto& cmp =
          m.get<BehaviourDescription::ExternalMFrontMaterialProperty>();
      const auto& mpd = *(cmp.mpd);
      if (mpd.inputs.empty()) {
        out << MFrontMaterialPropertyInterface().getFunctionName(mpd);
        out << "<NumericType>()";
      } else {
        out << MFrontMaterialPropertyInterface().getFunctionName(mpd);
        this->writeExternalMFrontMaterialPropertyArguments(out, cmp, f);
      }
    } else if (m.is<BehaviourDescription::AnalyticMaterialProperty>()) {
      const auto& amp = m.get<BehaviourDescription::AnalyticMaterialProperty>();
      tfel::math::Evaluator e(amp.f);
      auto mi = std::map<std::string, std::string>{};
      for (const auto& i :
           this->bd.getMaterialPropertyInputs(e.getVariablesNames())) {
        mi[i.name] = f(i);
      }
      out << e.getCxxFormula(mi);
    } else if (m.empty()) {
      this->throwRuntimeError(
          "BehaviourCodeGeneratorBase::writeMaterialPropertyEvaluation",
          "empty material property");
    } else {
      this->throwRuntimeError(
          "BehaviourCodeGeneratorBase::writeMaterialPropertyEvaluation",
          "unsupported material property type");
    }
  }  // end of writeMaterialPropertyEvaluation

  void BehaviourCodeGeneratorBase::writeThermalExpansionCoefficientComputation(
      std::ostream& out,
      const BehaviourDescription::MaterialProperty& a,
      const std::string& T,
      const std::string& idx,
      const std::string& s) const {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError(
            "BehaviourCodeGeneratorBase::"
            "writeThermalExpansionCoefficientComputation",
            m);
      }
    };
    out << "const thermalexpansion alpha" << s;
    if (!idx.empty()) {
      out << "_" << idx;
    }
    out << " = ";
    if (a.is<BehaviourDescription::ConstantMaterialProperty>()) {
      const auto& cmp = a.get<BehaviourDescription::ConstantMaterialProperty>();
      if (cmp.name.empty()) {
        out << cmp.value << ";\n";
      } else {
        out << "this->" << cmp.name << ";\n";
      }
    } else if (a.is<BehaviourDescription::ExternalMFrontMaterialProperty>()) {
      const auto& mpd =
          *(a.get<BehaviourDescription::ExternalMFrontMaterialProperty>().mpd);
      const auto inputs = this->bd.getMaterialPropertyInputs(mpd);
      if (this->bd.useQt()) {
        out << "thermalexpansion(";
      }
      out << MFrontMaterialPropertyInterface().getFunctionName(mpd) << '(';
      for (auto pi = inputs.begin(); pi != inputs.end();) {
        const auto c = pi->category;
        if (c == BehaviourDescription::MaterialPropertyInput::TEMPERATURE) {
          out << "tfel::math::base_type_cast(" << T << ")";
        } else if ((c == BehaviourDescription::MaterialPropertyInput::
                             MATERIALPROPERTY) ||
                   (c ==
                    BehaviourDescription::MaterialPropertyInput::PARAMETER)) {
          out << "tfel::math::base_type_cast(this->" << pi->name << ")";
        } else if (c == BehaviourDescription::MaterialPropertyInput::
                            STATICVARIABLE) {
          out << "tfel::math::base_type_cast(" << this->bd.getClassName()
              << "::" << pi->name << ")";
        } else {
          throw_if(true,
                   "thermal expansion coefficients must depend "
                   "on the temperature only");
        }
        if (++pi != inputs.end()) {
          out << ",";
        }
      }
      if (this->bd.useQt()) {
        out << ")";
      }
      out << ");\n";
    } else if (a.is<BehaviourDescription::AnalyticMaterialProperty>()) {
      const auto& amp = a.get<BehaviourDescription::AnalyticMaterialProperty>();
      auto m = std::map<std::string, std::string>{};
      for (const auto& i :
           this->bd.getMaterialPropertyInputs(amp.getVariablesNames())) {
        const auto c = i.category;
        if (c == BehaviourDescription::MaterialPropertyInput::TEMPERATURE) {
          m.insert({"T", "tfel::math::base_type_cast(" + T + ")"});
        } else if ((c == BehaviourDescription::MaterialPropertyInput::
                             MATERIALPROPERTY) ||
                   (c ==
                    BehaviourDescription::MaterialPropertyInput::PARAMETER)) {
          m.insert(
              {i.name, "tfel::math::base_type_cast(this->" + i.name + ")"});
        } else if (c == BehaviourDescription::MaterialPropertyInput::
                            STATICVARIABLE) {
          m.insert({i.name, "tfel::math::base_type_cast(" +
                                this->bd.getClassName() + "::" + i.name + ")"});
        } else {
          throw_if(true,
                   "thermal expansion coefficients must depend "
                   "on the temperature only");
        }
      }
      tfel::math::Evaluator ev(amp.f);
      if (this->bd.useQt()) {
        out << "thermalexpansion(";
      }
      out << ev.getCxxFormula(m);
      if (this->bd.useQt()) {
        out << ")";
      }
      out << ";\n";
    } else {
      throw_if(true, "unsupported material property type");
    }
  }  // end of writeThermalExpansionCoefficientComputation

  void
  BehaviourCodeGeneratorBase::writeThermalExpansionCoefficientsComputations(
      std::ostream& out,
      const BehaviourDescription::MaterialProperty& a,
      const std::string& suffix) const {
    this->writeThermalExpansionCoefficientComputation(
        out, a, "this->initial_geometry_reference_temperature", "",
        suffix + "_Ti");
    this->writeThermalExpansionCoefficientComputation(out, a, "this->T", "t",
                                                      suffix + "_T");
    this->writeThermalExpansionCoefficientComputation(
        out, a, "this->T+this->dT", "t_dt", suffix + "_T");
  }  // end of writeThermalExpansionCoefficientComputation

  void BehaviourCodeGeneratorBase::writeThermalExpansionComputation(
      std::ostream& out,
      const std::string& t,
      const std::string& c,
      const std::string& suffix) const {
    const auto Tref = "this->thermal_expansion_reference_temperature";
    const auto T = (t == "t") ? "this->T" : "this->T+this->dT";
    const auto i = (t == "t") ? "0" : "1";
    out << "const auto mfront_thermal_expansion" << c << "_" << i << "  "
        << "= 1 / (1 + alpha" << suffix
        << "_Ti * (this->initial_geometry_reference_temperature-" << Tref
        << ")) * ("
        << "alpha" << suffix << "_T_" << t << " * (" << T << "-" << Tref
        << ") - "
        << "alpha" << suffix
        << "_Ti * (this->initial_geometry_reference_temperature-" << Tref
        << "));\n";
  }  // end of writeThermalExpansionComputation

  void BehaviourCodeGeneratorBase::writeBehaviourComputeStressFreeExpansion(
      std::ostream& os, const Hypothesis h) const {
    auto tmpnames = std::vector<std::string>{};
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError(
            "BehaviourCodeGeneratorBase::"
            "writeBehaviourComputeStressFreeExpansion",
            m);
      }
    };
    auto eval = [](std::ostream& out,
                   const BehaviourDescription::MaterialProperty& mp,
                   const std::string& c, const bool b) {
      const auto& cmp =
          mp.get<BehaviourDescription::ConstantMaterialProperty>();
      const auto Tref = "this->thermal_expansion_reference_temperature";
      const auto i = b ? "1" : "0";
      const auto T = b ? "this->T+this->dT" : "this->T";
      if (cmp.name.empty()) {
        out << "const auto mfront_thermal_expansion" << c << "_" << i << " "
            << "= " << cmp.value << "/(1+" << cmp.value
            << "*(this->initial_geometry_reference_temperature-" << Tref << "))"
            << "*(" << T << "-this->initial_geometry_reference_temperature);\n";
      } else {
        out << "const auto mfront_thermal_expansion" << c << "_" << i << " "
            << "= (this->" << cmp.name << ")/(1+(this->" << cmp.name
            << ")*(this->initial_geometry_reference_temperature-" << Tref
            << "))"
            << "*(" << T << "-this->initial_geometry_reference_temperature);\n";
      }
    };
    if (!this->bd.requiresStressFreeExpansionTreatment(h)) {
      return;
    }
    if (this->bd.areThermalExpansionCoefficientsDefined()) {
      throw_if((this->bd.getBehaviourType() !=
                BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
                   (this->bd.getBehaviourType() !=
                    BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR),
               "only finite strain or small strain behaviour are supported");
      if (this->bd.getSymmetryType() == mfront::ORTHOTROPIC) {
        if ((this->bd.getOrthotropicAxesConvention() ==
             OrthotropicAxesConvention::DEFAULT) &&
            (this->bd.getThermalExpansionCoefficients().size() == 3u)) {
          // in this case, only tridimensional case is supported
          for (const auto mh : this->bd.getDistinctModellingHypotheses()) {
            throw_if(mh != ModellingHypothesis::TRIDIMENSIONAL,
                     "an orthotropic axes convention must be choosen when "
                     "using @ComputeThermalExpansion keyword in behaviours "
                     "which shall be valid in other modelling hypothesis "
                     "than 'Tridimensional'.\n"
                     "Either restrict the validity of the behaviour to "
                     "'Tridimensional' (see @ModellingHypothesis) or "
                     "choose and orthotropic axes convention as on option "
                     "to the @OrthotropicBehaviour keyword");
          }
        }
      }
    }
    this->checkBehaviourFile(os);
    const auto type = this->bd.getBehaviourType();
    if (type == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      os << "std::pair<StrainStensor, StrainStensor> "
            "computeStressFreeStrain(){\n";
      if (bd.isStrainMeasureDefined()) {
        const auto ms = bd.getStrainMeasure();
        if ((ms == BehaviourDescription::LINEARISED) ||
            (ms == BehaviourDescription::GREENLAGRANGE)) {
          os << "return this->computeStressFreeExpansion();\n";
        } else if (ms == BehaviourDescription::HENCKY) {
          os << "const auto mfront_dl01_l0 = "
             << "this->computeStressFreeExpansion();"
             << "auto mfront_sfs_bts = StrainStensor{strain(0)};\n"
             << "auto mfront_sfs_ets = StrainStensor{strain(0)};\n"
             << "mfront_sfs_bts[0] = "
             << "strain(std::log1p(mfront_dl01_l0.first[0]));\n"
             << "mfront_sfs_bts[1] = "
             << "strain(std::log1p(mfront_dl01_l0.first[1]));\n"
             << "mfront_sfs_bts[2] = "
             << "strain(std::log1p(mfront_dl01_l0.first[2]));\n"
             << "mfront_sfs_ets[0] = "
             << "strain(std::log1p(mfront_dl01_l0.second[0]));\n"
             << "mfront_sfs_ets[1] = "
             << "strain(std::log1p(mfront_dl01_l0.second[1]));\n"
             << "mfront_sfs_ets[2] = "
             << "strain(std::log1p(mfront_dl01_l0.second[2]));\n"
             << "return {mfront_sfs_bts, mfront_sfs_ets};\n";
        } else {
          throw_if(true, "unsupported finite strain strategy");
        }
      } else {
        os << "return this->computeStressFreeExpansion();\n";
      }
      os << "}\n\n";
    }
    os << "[[nodiscard]] std::pair<StressFreeExpansionType, "
       << "StressFreeExpansionType>\n"
       << "computeStressFreeExpansion()\n{\n"
       << "auto mfront_dl01_l0 = "
       << "std::pair<StressFreeExpansionType, StressFreeExpansionType>{};\n"
       << "this->computeStressFreeExpansion(mfront_dl01_l0);\n"
       << "return mfront_dl01_l0;\n"
       << "}\n\n"
       << "void\n"
       << "computeStressFreeExpansion(std::pair<StressFreeExpansionType,"
       << "StressFreeExpansionType>& mfront_dl01_l0)\n{\n"
       << "using namespace std;\n"
       << "using namespace tfel::math;\n"
       << "using std::vector;\n";
    writeMaterialLaws(os, this->bd.getMaterialLaws());
    os << "auto& mfront_dl0_l0 = mfront_dl01_l0.first;\n"
       << "auto& mfront_dl1_l0 = mfront_dl01_l0.second;\n"
       << "mfront_dl0_l0 = StressFreeExpansionType(typename "
          "StressFreeExpansionType::value_type(0));\n"
       << "mfront_dl1_l0 = StressFreeExpansionType(typename "
          "StressFreeExpansionType::value_type(0));\n";
    if (this->bd.hasCode(h, BehaviourData::ComputeStressFreeExpansion)) {
      os << "{\n"
         << "auto& dl0_l0 = mfront_dl0_l0;\n"
         << "auto& dl1_l0 = mfront_dl1_l0;\n"
         << this->bd.getCode(h, BehaviourData::ComputeStressFreeExpansion)
         << '\n'
         << "}\n";
    }
    if (this->bd.areThermalExpansionCoefficientsDefined()) {
      const auto& acs = this->bd.getThermalExpansionCoefficients();
      if (acs.size() == 1u) {
        const auto& a = acs.front();
        if (a.is<BehaviourDescription::ConstantMaterialProperty>()) {
          eval(os, a, "0", false);
        } else {
          this->writeThermalExpansionCoefficientsComputations(os, acs.front());
          this->writeThermalExpansionComputation(os, "t", "0");
        }
        os << "mfront_dl0_l0[0] += mfront_thermal_expansion0_0;\n"
           << "mfront_dl0_l0[1] += mfront_thermal_expansion0_0;\n"
           << "mfront_dl0_l0[2] += mfront_thermal_expansion0_0;\n";
        if (a.is<BehaviourDescription::ConstantMaterialProperty>()) {
          eval(os, a, "0", true);
        } else {
          this->writeThermalExpansionComputation(os, "t_dt", "0");
        }
        os << "mfront_dl1_l0[0] += mfront_thermal_expansion0_1;\n"
           << "mfront_dl1_l0[1] += mfront_thermal_expansion0_1;\n"
           << "mfront_dl1_l0[2] += mfront_thermal_expansion0_1;\n";
        if (bd.getAttribute<bool>(BehaviourDescription::saveThermalExpansion,
                                  false)) {
          os << "this->mfront_thermal_expansion = "
                "mfront_thermal_expansion0_1;\n";
        }
      } else if (acs.size() == 3u) {
        throw_if(this->bd.getSymmetryType() != mfront::ORTHOTROPIC,
                 "invalid number of thermal expansion coefficients");
        for (size_t i = 0; i != 3; ++i) {
          if (!acs[i].is<BehaviourDescription::ConstantMaterialProperty>()) {
            this->writeThermalExpansionCoefficientsComputations(
                os, acs[i], std::to_string(i));
          }
        }
        for (size_t i = 0; i != 3; ++i) {
          const auto idx = std::to_string(i);
          if (acs[i].is<BehaviourDescription::ConstantMaterialProperty>()) {
            eval(os, acs[i], idx, false);
            eval(os, acs[i], idx, true);
          } else {
            this->writeThermalExpansionComputation(os, "t", idx, idx);
            this->writeThermalExpansionComputation(os, "t_dt", idx, idx);
          }
        }
        os << "mfront_dl0_l0[0] += mfront_thermal_expansion0_0;\n"
           << "mfront_dl0_l0[1] += mfront_thermal_expansion1_0;\n"
           << "mfront_dl0_l0[2] += mfront_thermal_expansion2_0;\n";
        os << "mfront_dl1_l0[0] += mfront_thermal_expansion0_1;\n"
           << "mfront_dl1_l0[1] += mfront_thermal_expansion1_1;\n"
           << "mfront_dl1_l0[2] += mfront_thermal_expansion2_1;\n";
        if (bd.getAttribute<bool>(BehaviourDescription::saveThermalExpansion,
                                  false)) {
          os << "this->mfront_thermal_expansion[0] = "
                "mfront_thermal_expansion0_1;\n"
                "this->mfront_thermal_expansion[1] = "
                "mfront_thermal_expansion1_1;\n"
                "this->mfront_thermal_expansion[2] = "
                "mfront_thermal_expansion2_1;\n";
        }
      } else {
        throw_if(true, "unsupported behaviour symmetry");
      }
    }
    for (const auto& d : this->bd.getStressFreeExpansionDescriptions(h)) {
      if (d.is<BehaviourData::AxialGrowth>()) {
        throw_if((this->bd.getBehaviourType() !=
                  BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
                     (this->bd.getBehaviourType() !=
                      BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR),
                 "only finite strain or small strain behaviour are supported");
        throw_if(this->bd.getSymmetryType() != mfront::ORTHOTROPIC,
                 "axial growth is only supported for orthotropic behaviours");
        const auto& s = d.get<BehaviourData::AxialGrowth>();
        throw_if(s.sfe.is<BehaviourData::NullExpansion>(),
                 "null swelling is not supported here");
        // The z-axis is supposed to be aligned with the second
        // direction of orthotropy.
        if (s.sfe.is<BehaviourData::SFED_ESV>()) {
          const auto ev = s.sfe.get<BehaviourData::SFED_ESV>().vname;
          os << "mfront_dl0_l0[1]+=this->" << ev << ";\n"
             << "mfront_dl0_l0[0]+=real(1)/std::sqrt(1+this->" << ev
             << ")-real(1);\n"
             << "mfront_dl0_l0[2]+=real(1)/std::sqrt(1+this->" << ev
             << ")-real(1);\n"
             << "mfront_dl1_l0[1]+=this->" << ev << "+this->d" << ev << ";\n"
             << "mfront_dl1_l0[0]+=real(1)/std::sqrt(1+this->" << ev
             << "+this->d" << ev << ")-real(1);\n"
             << "mfront_dl1_l0[2]+=real(1)/std::sqrt(1+this->" << ev
             << "+this->d" << ev << ")-real(1);\n";
        } else if (s.sfe.is<std::shared_ptr<ModelDescription>>()) {
          const auto& md = *(s.sfe.get<std::shared_ptr<ModelDescription>>());
          throw_if(
              md.outputs.size() != 1u,
              "invalid number of outputs for model '" + md.className + "'");
          const auto vs = md.className + "_" + md.outputs[0].name;
          os << "mfront_dl0_l0[1]+=this->" << vs << ";\n"
             << "mfront_dl0_l0[0]+=real(1)/std::sqrt(1+this->" << vs
             << ")-real(1);\n"
             << "mfront_dl0_l0[2]+=real(1)/std::sqrt(1+this->" << vs
             << ")-real(1);\n";
          this->writeModelCall(os, tmpnames, h, md,
                               std::vector<std::string>(1u, vs),
                               std::vector<std::string>(1u, vs), "sfeh");
          os << "mfront_dl1_l0[1]+=this->" << vs << ";\n"
             << "mfront_dl1_l0[0]+=real(1)/std::sqrt(1+this->" << vs
             << ")-real(1);\n"
             << "mfront_dl1_l0[2]+=real(1)/std::sqrt(1+this->" << vs
             << ")-real(1);\n";
        } else {
          throw_if(true, "internal error, unsupported stress free expansion");
        }
      } else if (d.is<BehaviourData::Relocation>()) {
        throw_if((this->bd.getBehaviourType() !=
                  BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
                     (this->bd.getBehaviourType() !=
                      BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR),
                 "only finite strain or small strain behaviour are supported");
        const auto& s = d.get<BehaviourData::Relocation>();
        throw_if(s.sfe.is<BehaviourData::NullExpansion>(),
                 "null swelling is not supported here");
        if (s.sfe.is<BehaviourData::SFED_ESV>()) {
          const auto ev = s.sfe.get<BehaviourData::SFED_ESV>().vname;
          if ((h ==
               ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) ||
              (h ==
               ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)) {
            os << "mfront_dl0_l0[0]+=this->" << ev << "/2;\n"
               << "mfront_dl0_l0[2]+=this->" << ev << "/2;\n"
               << "mfront_dl1_l0[0]+=(this->" << ev << "+this->d" << ev
               << ")/2;\n"
               << "mfront_dl1_l0[2]+=(this->" << ev << "+this->d" << ev
               << ")/2;\n";
          }
          if ((h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
              (h == ModellingHypothesis::PLANESTRAIN) ||
              (h == ModellingHypothesis::PLANESTRESS)) {
            os << "mfront_dl0_l0[0]+=this->" << ev << "/2;\n"
               << "mfront_dl0_l0[1]+=this->" << ev << "/2;\n"
               << "mfront_dl1_l0[0]+=(this->" << ev << "+this->d" << ev
               << ")/2;\n"
               << "mfront_dl1_l0[1]+=(this->" << ev << "+this->d" << ev
               << ")/2;\n";
          }
        } else if (s.sfe.is<std::shared_ptr<ModelDescription>>()) {
          const auto& md = *(s.sfe.get<std::shared_ptr<ModelDescription>>());
          throw_if(
              md.outputs.size() != 1u,
              "invalid number of outputs for model '" + md.className + "'");
          const auto vs = md.className + "_" + md.outputs[0].name;
          if ((h ==
               ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) ||
              (h ==
               ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN)) {
            os << "mfront_dl0_l0[0]+=(this->" << vs << ")/2;\n"
               << "mfront_dl0_l0[2]+=(this->" << vs << ")/2;\n";
          }
          this->writeModelCall(os, tmpnames, h, md,
                               std::vector<std::string>(1u, vs),
                               std::vector<std::string>(1u, vs), "sfeh");
          if ((h == ModellingHypothesis::GENERALISEDPLANESTRAIN) ||
              (h == ModellingHypothesis::PLANESTRAIN) ||
              (h == ModellingHypothesis::PLANESTRESS)) {
            os << "mfront_dl0_l0[0]+=(this->" << vs << ")/2;\n"
               << "mfront_dl0_l0[1]+=(this->" << vs << ")/2;\n";
          }
        } else {
          throw_if(true, "internal error, unsupported stress free expansion");
        }
      } else if (d.is<BehaviourData::OrthotropicStressFreeExpansion>()) {
        using StressFreeExpansionHandler =
            BehaviourData::StressFreeExpansionHandler;
        const auto& s = d.get<BehaviourData::OrthotropicStressFreeExpansion>();
        auto write = [this, &os, &tmpnames, throw_if, h](
                         const StressFreeExpansionHandler& sfe,
                         const char* const c) {
          if (sfe.is<BehaviourData::SFED_ESV>()) {
            const auto& ev = sfe.get<BehaviourData::SFED_ESV>().vname;
            os << "mfront_dl0_l0[" << c << "]+=this->" << ev << ";\n";
            os << "mfront_dl1_l0[" << c << "]+=this->" << ev << "+this->d" << ev
               << ";\n";
          } else if (sfe.is<std::shared_ptr<ModelDescription>>()) {
            const auto& md = *(sfe.get<std::shared_ptr<ModelDescription>>());
            throw_if(
                md.outputs.size() != 1u,
                "invalid number of outputs for model '" + md.className + "'");
            const auto vs = md.className + "_" + md.outputs[0].name;
            os << "mfront_dl0_l0[" << c << "]+=this->" << vs << ";\n";
            this->writeModelCall(os, tmpnames, h, md,
                                 std::vector<std::string>(1u, vs),
                                 std::vector<std::string>(1u, vs), "sfeh");
            os << "mfront_dl1_l0[" << c << "]+=this->" << vs << ";\n";
          } else if (!sfe.is<BehaviourData::NullExpansion>()) {
            throw_if(true, "internal error, unsupported stress free expansion");
          }
        };
        throw_if((this->bd.getBehaviourType() !=
                  BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
                     (this->bd.getBehaviourType() !=
                      BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR),
                 "only finite strain or small strain behaviour are supported");
        throw_if(this->bd.getSymmetryType() != mfront::ORTHOTROPIC,
                 "orthotropic stress free expansion is only supported "
                 "for orthotropic behaviours");
        throw_if(s.sfe0.is<BehaviourData::NullExpansion>() &&
                     s.sfe1.is<BehaviourData::NullExpansion>() &&
                     s.sfe2.is<BehaviourData::NullExpansion>(),
                 "null swelling is not supported here");
        write(s.sfe0, "0");
        write(s.sfe1, "1");
        write(s.sfe2, "2");
      } else if (d.is<BehaviourData::OrthotropicStressFreeExpansionII>()) {
        const auto& s =
            d.get<BehaviourData::OrthotropicStressFreeExpansionII>();
        const auto& ev = s.esv.vname;
        throw_if((this->bd.getBehaviourType() !=
                  BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
                     (this->bd.getBehaviourType() !=
                      BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR),
                 "only finite strain or small strain behaviour are supported");
        throw_if(this->bd.getSymmetryType() != mfront::ORTHOTROPIC,
                 "orthotropic stress free expansion is only supported "
                 "for orthotropic behaviours");
        os << "mfront_dl0_l0[0]+=this->" << ev << "[0];\n"
           << "mfront_dl0_l0[1]+=this->" << ev << "[1];\n"
           << "mfront_dl0_l0[2]+=this->" << ev << "[2];\n"
           << "mfront_dl1_l0[0]+=this->" << ev << "[0]+this->d" << ev
           << "[0];\n"
           << "mfront_dl1_l0[1]+=this->" << ev << "[1]+this->d" << ev
           << "[1];\n"
           << "mfront_dl1_l0[2]+=this->" << ev << "[2]+this->d" << ev
           << "[2];\n";
      } else if (d.is<BehaviourData::IsotropicStressFreeExpansion>()) {
        throw_if((this->bd.getBehaviourType() !=
                  BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
                     (this->bd.getBehaviourType() !=
                      BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR),
                 "only finite strain or small strain behaviour are supported");
        const auto& s = d.get<BehaviourData::IsotropicStressFreeExpansion>();
        throw_if(s.sfe.is<BehaviourData::NullExpansion>(),
                 "null swelling is not supported here");
        if (s.sfe.is<BehaviourData::SFED_ESV>()) {
          const auto ev = s.sfe.get<BehaviourData::SFED_ESV>().vname;
          os << "mfront_dl0_l0[0]+=this->" << ev << ";\n"
             << "mfront_dl0_l0[1]+=this->" << ev << ";\n"
             << "mfront_dl0_l0[2]+=this->" << ev << ";\n"
             << "mfront_dl1_l0[0]+=this->" << ev << "+this->d" << ev << ";\n"
             << "mfront_dl1_l0[1]+=this->" << ev << "+this->d" << ev << ";\n"
             << "mfront_dl1_l0[2]+=this->" << ev << "+this->d" << ev << ";\n";
        } else if (s.sfe.is<std::shared_ptr<ModelDescription>>()) {
          const auto& md = *(s.sfe.get<std::shared_ptr<ModelDescription>>());
          throw_if(
              md.outputs.size() != 1u,
              "invalid number of outputs for model '" + md.className + "'");
          const auto vs = md.className + "_" + md.outputs[0].name;
          os << "mfront_dl0_l0[0]+=this->" << vs << ";\n"
             << "mfront_dl0_l0[1]+=this->" << vs << ";\n"
             << "mfront_dl0_l0[2]+=this->" << vs << ";\n";
          this->writeModelCall(os, tmpnames, h, md,
                               std::vector<std::string>(1u, vs),
                               std::vector<std::string>(1u, vs), "sfeh");
          os << "mfront_dl1_l0[0]+=this->" << vs << ";\n"
             << "mfront_dl1_l0[1]+=this->" << vs << ";\n"
             << "mfront_dl1_l0[2]+=this->" << vs << ";\n";
        } else {
          throw_if(true, "internal error, unsupported stress free expansion");
        }
      } else if (d.is<BehaviourData::VolumeSwellingStressFreeExpansion>()) {
        throw_if((this->bd.getBehaviourType() !=
                  BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
                     (this->bd.getBehaviourType() !=
                      BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR),
                 "only finite strain or small strain behaviour are supported");
        const auto& s =
            d.get<BehaviourData::VolumeSwellingStressFreeExpansion>();
        throw_if(s.sfe.is<BehaviourData::NullExpansion>(),
                 "null swelling is not supported here");
        if (s.sfe.is<BehaviourData::SFED_ESV>()) {
          const auto ev = s.sfe.get<BehaviourData::SFED_ESV>().vname;
          os << "mfront_dl0_l0[0]+=this->" << ev << "/3;\n"
             << "mfront_dl0_l0[1]+=this->" << ev << "/3;\n"
             << "mfront_dl0_l0[2]+=this->" << ev << "/3;\n"
             << "mfront_dl1_l0[0]+=(this->" << ev << "+this->d" << ev
             << ")/3;\n"
             << "mfront_dl1_l0[1]+=(this->" << ev << "+this->d" << ev
             << ")/3;\n"
             << "mfront_dl1_l0[2]+=(this->" << ev << "+this->d" << ev
             << ")/3;\n";
        } else if (s.sfe.is<std::shared_ptr<ModelDescription>>()) {
          const auto& md = *(s.sfe.get<std::shared_ptr<ModelDescription>>());
          throw_if(
              md.outputs.size() != 1u,
              "invalid number of outputs for model '" + md.className + "'");
          const auto vs = md.className + "_" + md.outputs[0].name;
          os << "mfront_dl0_l0[0]+=this->" << vs << "/3;\n"
             << "mfront_dl0_l0[1]+=this->" << vs << "/3;\n"
             << "mfront_dl0_l0[2]+=this->" << vs << "/3;\n";
          this->writeModelCall(os, tmpnames, h, md,
                               std::vector<std::string>(1u, vs),
                               std::vector<std::string>(1u, vs), "sfeh");
          os << "mfront_dl1_l0[0]+=this->" << vs << "/3;\n"
             << "mfront_dl1_l0[1]+=this->" << vs << "/3;\n"
             << "mfront_dl1_l0[2]+=this->" << vs << "/3;\n";
        } else {
          throw_if(true, "internal error, unsupported stress free expansion");
        }
      } else {
        throw_if(true,
                 "internal error, unsupported stress "
                 "free expansion description");
      }
    }
    if (this->bd.getSymmetryType() == mfront::ORTHOTROPIC) {
      if (this->bd.getOrthotropicAxesConvention() ==
          OrthotropicAxesConvention::PIPE) {
        os << "tfel::material::convertStressFreeExpansionStrain<hypothesis,"
              "tfel::material::OrthotropicAxesConvention::"
              "PIPE>(mfront_dl0_l0);\n"
           << "tfel::material::convertStressFreeExpansionStrain<hypothesis,"
              "tfel::material::OrthotropicAxesConvention::"
              "PIPE>(mfront_dl1_l0);\n";
      } else if (this->bd.getOrthotropicAxesConvention() ==
                 OrthotropicAxesConvention::PLATE) {
        os << "tfel::material::convertStressFreeExpansionStrain<hypothesis,"
              "tfel::material::OrthotropicAxesConvention::"
              "PLATE>(mfront_dl0_l0);\n"
           << "tfel::material::convertStressFreeExpansionStrain<hypothesis,"
              "tfel::material::OrthotropicAxesConvention::"
              "PLATE>(mfront_dl1_l0);\n";
      } else {
        throw_if(this->bd.getOrthotropicAxesConvention() !=
                     OrthotropicAxesConvention::DEFAULT,
                 "internal error, unsupported orthotropic axes convention");
        for (const auto mh : this->bd.getDistinctModellingHypotheses()) {
          throw_if(mh != ModellingHypothesis::TRIDIMENSIONAL,
                   "an orthotropic axes convention must be choosen when "
                   "defining a stress free expansion in behaviours "
                   "which shall be valid in other modelling hypothesis "
                   "than 'Tridimensional'.\n"
                   "Either restrict the validity of the behaviour to "
                   "'Tridimensional' (see @ModellingHypothesis) or "
                   "choose and orthotropic axes convention as on option "
                   "to the @OrthotropicBehaviour keyword");
        }
      }
    }
    os << "}\n\n";
  }  // end of writeBehaviourComputeStressFreeExpansion

  void BehaviourCodeGeneratorBase::writeBehaviourInitializeMethods(
      std::ostream& os, const Hypothesis h) const {
    using ExternalModel =
        BehaviourDescription::ExternalModelBasedOnBehaviourVariableFactory;
    this->checkBehaviourFile(os);
    os << "/*!\n"
       << " * \\ brief initialize the behaviour with user code\n"
       << " */\n"
       << "[[nodiscard]] TFEL_HOST_DEVICE bool initialize(){\n"
       << "using namespace std;\n"
       << "using namespace tfel::math;\n"
       << "using std::vector;\n";
    writeMaterialLaws(os, this->bd.getMaterialLaws());
    // calling models
    auto tmpnames = std::vector<std::string>{};
    for (const auto& m : this->bd.getModelsDescriptions()) {
      if (std::holds_alternative<ModelDescription>(m)) {
        const auto& md = std::get<ModelDescription>(m);
        auto inputs = std::vector<std::string>{};
        auto outputs = std::vector<std::string>{};
        for (const auto& v : md.outputs) {
          inputs.push_back(v.name);
          outputs.push_back("d" + v.name);
        }
        this->writeModelCall(os, tmpnames, h, md, outputs, inputs, "em");
        for (const auto& v : md.outputs) {
          os << "this->d" << v.name << " -= this->" << v.name << ";\n";
        }
      } else {
        const auto& md = std::get<ExternalModel>(m);
        const auto mv =
            this->bd.getBehaviourData(h).getBehaviourVariableFactory(
                md.factory);
        const auto instance = mv.name + "_instance";
        os << "auto " << instance << " = " << mv.name << ".make();\n"
           << "if(!this->initialize(" << instance << ")){\n"
           << "return false;\n"
           << "}\n"
           << "if(!" << instance << ". integrate("
           << "::tfel::material::TangentOperatorTraits<::tfel::"
           << "material::MechanicalBehaviourBase::GENERALBEHAVIOUR>::"
           << "STANDARDTANGENTOPERATOR, NOSTIFFNESSREQUESTED)){\n"
           << "return false;\n"
           << "}\n";
        for (const auto& v :
             mv.behaviour.getBehaviourData(h).getPersistentVariables()) {
          const auto vn = applyNamesChanges(mv, v);
          os << "this->d" << vn.name << " = " << instance << ". " << v.name
             << " - this->" << vn.name << ";\n";
        }
      }
    }
    this->writeBehaviourLocalVariablesInitialisation(os, h);
    //
    const auto& md = this->bd.getBehaviourData(h);
    for (const auto& b : md.getBehaviourVariables()) {
      os << "if(!this->initialize(" << b.name << ")){\n"
         << "return false;\n"
         << "}\n";
    }
    //
    if (this->bd.hasCode(h, BehaviourData::BeforeInitializeLocalVariables)) {
      if (this->bd.getAttribute(BehaviourData::profiling, false)) {
        writeStandardPerformanceProfilingBegin(
            os, this->bd.getClassName(),
            BehaviourData::BeforeInitializeLocalVariables, "binit");
      }
      os << this->bd
                .getCodeBlock(h, BehaviourData::BeforeInitializeLocalVariables)
                .code
         << '\n';
      if (this->bd.getAttribute(BehaviourData::profiling, false)) {
        writeStandardPerformanceProfilingEnd(os);
      }
    }
    if (this->bd.hasCode(h, BehaviourData::InitializeLocalVariables)) {
      if (this->bd.getAttribute(BehaviourData::profiling, false)) {
        writeStandardPerformanceProfilingBegin(
            os, this->bd.getClassName(),
            BehaviourData::InitializeLocalVariables, "init");
      }
      os << this->bd.getCodeBlock(h, BehaviourData::InitializeLocalVariables)
                .code
         << '\n';
      if (this->bd.getAttribute(BehaviourData::profiling, false)) {
        writeStandardPerformanceProfilingEnd(os);
      }
    }
    if (this->bd.hasCode(h, BehaviourData::AfterInitializeLocalVariables)) {
      if (this->bd.getAttribute(BehaviourData::profiling, false)) {
        writeStandardPerformanceProfilingBegin(
            os, this->bd.getClassName(),
            BehaviourData::AfterInitializeLocalVariables, "ainit");
      }
      os << this->bd
                .getCodeBlock(h, BehaviourData::AfterInitializeLocalVariables)
                .code
         << '\n';
      if (this->bd.getAttribute(BehaviourData::profiling, false)) {
        writeStandardPerformanceProfilingEnd(os);
      }
    }
    this->writeBehaviourParserSpecificInitializeMethodPart(os, h);
    os << "return true;\n"
       << "}\n\n";
  }  // end of writeBehaviourInitializeMethod

  void BehaviourCodeGeneratorBase::writeBehaviourLocalVariablesInitialisation(
      std::ostream& os, const Hypothesis h) const {
    const auto& md = this->bd.getBehaviourData(h);
    this->checkBehaviourFile(os);
    for (const auto& v : md.getLocalVariables()) {
      if (this->bd.useDynamicallyAllocatedVector(v.arraySize)) {
        os << "this->" << v.name << ".resize(" << v.arraySize << ");\n";
      }
    }
  }

  void BehaviourCodeGeneratorBase::writeBehaviourParameterInitialisation(
      std::ostream& os, const Hypothesis h) const {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto use_static_variables =
        areParametersTreatedAsStaticVariables(this->bd);
    this->checkBehaviourFile(os);
    const auto& d = this->bd.getBehaviourData(h);
    for (const auto& p : d.getParameters()) {
      if (use_static_variables) {
        if (!p.getAttribute<bool>(
                VariableDescription::variableDeclaredInBaseClass, false)) {
          continue;
        }
        if (p.type == "int") {
          os << "this->" << p.name << " = "
             << this->bd.getIntegerParameterDefaultValue(h, p.name) << ";\n";
        } else if (p.type == "ushort") {
          os << "this->" << p.name << " = "
             << this->bd.getUnsignedShortParameterDefaultValue(h, p.name)
             << ";\n";
        } else {
          const auto f = SupportedTypes::getTypeFlag(p.type);
          if (f != SupportedTypes::SCALAR) {
            this->throwRuntimeError(
                "BehaviourCodeGeneratorBase::"
                "writeBehaviourParameterInitialisation",
                "unsupported parameter type '" + p.type +
                    "' "
                    "for parameter '" +
                    p.name + "'");
          }
          if (p.arraySize == 1u) {
            os << "this->" << p.name << " = " << p.type << "("
               << this->bd.getFloattingPointParameterDefaultValue(h, p.name)
               << ");\n";
          } else {
            for (unsigned short i = 0; i != p.arraySize; ++i) {
              os << "this->" << p.name << "[" << i << "] = " << p.type << "("
                 << this->bd.getFloattingPointParameterDefaultValue(h, p.name,
                                                                    i)
                 << ");\n";
            }
          }
        }
      } else {
        const auto getter = [this, h, &p] {
          if ((h == uh) || (this->bd.hasParameter(uh, p.name))) {
            return this->bd.getClassName() + "ParametersInitializer::get()";
          }
          return this->bd.getClassName() + ModellingHypothesis::toString(h) +
                 "ParametersInitializer::get()";
        }();
        if (p.arraySize == 1u) {
          os << "this->" << p.name << " = ";
          if (this->bd.useQt()) {
            os << p.type << "(" << getter << "." << p.name << ");\n";
          } else {
            os << getter << "." << p.name << ";\n";
          }
        } else {
          os << "tfel::fsalgo::transform<" << p.arraySize << ">::exe("  //
             << getter << "." << p.name << ".begin(), "                 //
             << "this->" << p.name << ".begin(), "
             << "[](const auto& mfront_parameter_value){\n"
             << "return " << p.type
             << "{static_cast<NumericType>(mfront_parameter_value)};\n"
             << "});\n";
        }
      }
    }
  }  // end of writeBehaviourParameterInitialisation

  void BehaviourCodeGeneratorBase::writeBehaviourDataMainVariablesSetters(
      std::ostream& os) const {
    this->checkBehaviourDataFile(os);
    for (const auto& i : this->interfaces) {
      i.second->writeBehaviourDataMainVariablesSetters(os, this->bd);
      os << '\n';
    }
  }  // end of writeBehaviourDataMainVariablesSetters

  void BehaviourCodeGeneratorBase::writeIntegrationDataMainVariablesSetters(
      std::ostream& os) const {
    this->checkIntegrationDataFile(os);
    for (const auto& i : this->interfaces) {
      i.second->writeIntegrationDataMainVariablesSetters(os, this->bd);
      os << '\n';
    }
  }  // end of writeIntegrationDataMainVariablesSetters

  void BehaviourCodeGeneratorBase::writeBehaviourGetModellingHypothesis(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    os << "/*!\n"
       << "* \\return the modelling hypothesis\n"
       << "*/\n"
       << "constexpr "
          "ModellingHypothesis::Hypothesis\ngetModellingHypothesis() "
          "const{\n"
       << "return hypothesis;\n"
       << "} // end of getModellingHypothesis\n\n";
  }  // end of writeBehaviourGetModellingHypothesis();

  void BehaviourCodeGeneratorBase::writeBehaviourLocalVariables(
      std::ostream& os, const Hypothesis h) const {
    this->checkBehaviourFile(os);
    const auto& md = this->bd.getBehaviourData(h);
    this->writeVariablesDeclarations(os, md.getLocalVariables(), "", "",
                                     this->fd.fileName, false);
    os << '\n';
    auto write_wrapper = [this, &os](const BehaviourVariableDescription& b) {
      const auto wrapper = getBehaviourWrapperClassName(b);
      const auto bn = [this, &b] {
        if (this->bd.useQt()) {
          return b.behaviour.getFullClassName() +
                 "<hypothesis, NumericType, use_qt>";
        }
        return b.behaviour.getFullClassName() +
               "<hypothesis, NumericType, false>";
      }();
      os << "struct " << wrapper << " final\n: " << bn << "\n"
         << "{\n"
         << "//! \\brief default constructor\n"
         << wrapper << "()\n"
         << ": " << bn << "(){\n"
         << "}\n"
         << "//! \\brief copy constructor\n"
         << wrapper << "(const " << wrapper << "&) = delete;\n"
         << "//! \\brief move constructor\n"
         << wrapper << "(" << wrapper << "&&) = delete;\n"
         << "//! \\brief move assignement\n"
         << wrapper << "& operator=(" << wrapper << "&&) = default;\n"
         << "//! \\brief destructor\n"
         << "~" << wrapper << "() override = default;\n"
         << "private:\n"
         << "friend struct " << this->bd.getClassName() << ";\n"
         << "};\n\n";
    };
    for (const auto& b : md.getBehaviourVariables()) {
      const auto wrapper = getBehaviourWrapperClassName(b);
      write_wrapper(b);
      os << wrapper << " " << b.name << ";\n";
    }
    for (const auto& b : md.getBehaviourVariableFactories()) {
      const auto wrapper = getBehaviourWrapperClassName(b);
      const auto factory = getBehaviourVariableFactoryClassName(b);
      write_wrapper(b);
      os << "struct " << factory << " final\n"
         << "{\n"
         << factory << "(const " << this->bd.getClassName()
         << "& mfront_behaviour_reference_argument)\n"
         << ": mfront_behaviour_reference("
         << "mfront_behaviour_reference_argument)\n"
         << "{}\n"
         << "[[nodiscard]] " << wrapper << " make(){\n"
         << "return " << wrapper << "{};\n"
         << "} // end of make\n"
         << "private:\n"
         << "const " << this->bd.getClassName()
         << "& mfront_behaviour_reference;\n"
         << "};\n\n"
         << factory << " " << b.name << " = " << factory << "{*this};\n"
         << "friend struct " << factory << ";\n";
    }
  }

  void BehaviourCodeGeneratorBase::writeBehaviourIntegrationVariables(
      std::ostream& os, const Hypothesis h) const {
    this->checkBehaviourFile(os);
    const auto& md = this->bd.getBehaviourData(h);
    for (const auto& v : md.getIntegrationVariables()) {
      if (!md.isStateVariableName(v.name)) {
        if (md.isMemberUsedInCodeBlocks(v.name)) {
          this->writeVariableDeclaration(os, v, "", "", this->fd.fileName,
                                         false);
        }
      }
    }
    os << '\n';
  }  // end od BehaviourCodeGeneratorBase::writeBehaviourIntegrationVariables

  void BehaviourCodeGeneratorBase::writeBehaviourParameters(
      std::ostream& os, const Hypothesis h) const {
    this->checkBehaviourFile(os);
    const auto use_static_variables =
        areParametersTreatedAsStaticVariables(this->bd);
    const auto& d = this->bd.getBehaviourData(h);
    for (const auto& p : d.getParameters()) {
      if (p.getAttribute<bool>(VariableDescription::variableDeclaredInBaseClass,
                               false)) {
        continue;
      }
      if (!getDebugMode()) {
        if (p.lineNumber != 0u) {
          os << "#line " << p.lineNumber << " \"" << this->fd.fileName
             << "\"\n";
        }
      }
      if (use_static_variables) {
        os << "static constexpr ";
        if (p.type == "int") {
          os << "int " << p.name << " = "
             << this->bd.getIntegerParameterDefaultValue(h, p.name) << ";\n";
        } else if (p.type == "ushort") {
          os << "unsigned short " << p.name << " = "
             << this->bd.getUnsignedShortParameterDefaultValue(h, p.name)
             << ";\n";
        } else {
          const auto f = SupportedTypes::getTypeFlag(p.type);
          if (f != SupportedTypes::SCALAR) {
            this->throwRuntimeError(
                "BehaviourCodeGeneratorBase::writeBehaviourParameters",
                "unsupported parameter type '" + p.type +
                    "' "
                    "for parameter '" +
                    p.name + "'");
          }
          if (p.arraySize == 1u) {
            os << p.type << " " << p.name << " = " << p.type << "("
               << this->bd.getFloattingPointParameterDefaultValue(h, p.name)
               << ");\n";
          } else {
            os << "tfel::math::fsarray<" << p.arraySize << ", " << p.type
               << "> " << p.name << " = {";
            for (unsigned short i = 0; i != p.arraySize;) {
              os << p.type << "("
                 << this->bd.getFloattingPointParameterDefaultValue(h, p.name,
                                                                    i)
                 << ")";
              if (++i != p.arraySize) {
                os << ", ";
              }
            }
            os << "};\n";
          }
        }
      } else {
        if (p.arraySize == 1) {
          os << p.type << " " << p.name << ";\n";
        } else {
          os << "tfel::math::fsarray<" << p.arraySize << "," << p.type << "> "
             << p.name << ";\n";
        }
      }
    }
    os << '\n';
  }  // end of writeBehaviourParameters

  void BehaviourCodeGeneratorBase::writeBehaviourOutOfBoundsPolicyVariable(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    const auto p = getDefaultOutOfBoundsPolicyAsString(this->bd);
    os << "//! policy for treating out of bounds conditions\n";
    if (allowRuntimeModificationOfTheOutOfBoundsPolicy(this->bd)) {
      os << "OutOfBoundsPolicy policy = tfel::material::" << p << ";\n\n";
    } else {
      os << "static constexpr auto policy = tfel::material::" << p << ";\n\n";
    }
  }  // end of writeBehaviourOutOfBoundsPolicyVariable

  void BehaviourCodeGeneratorBase::writeBehaviourStaticVariables(
      std::ostream& os, const Hypothesis h) const {
    const auto& md = this->bd.getBehaviourData(h);
    this->checkBehaviourFile(os);
    for (const auto& v : md.getStaticVariables()) {
      if (!getDebugMode()) {
        if (v.lineNumber != 0u) {
          os << "#line " << v.lineNumber << " \"" << this->fd.fileName
             << "\"\n";
        }
      }
      os << "static constexpr auto " << v.name << " = "  //
         << v.type << "{" << v.value << "};\n";
    }
    os << '\n';
  }

  void BehaviourCodeGeneratorBase::writeBehaviourIntegrationVariablesIncrements(
      std::ostream& os, const Hypothesis h) const {
    const auto& md = this->bd.getBehaviourData(h);
    this->checkBehaviourFile(os);
    this->writeVariablesDeclarations(os, md.getIntegrationVariables(), "d", "",
                                     this->fd.fileName,
                                     this->usesStateVariableTimeDerivative());
    os << '\n';
  }

  void BehaviourCodeGeneratorBase::writeBehaviourOutputOperator(
      std::ostream& os, const Hypothesis h) const {
    const auto& md = this->bd.getBehaviourData(h);
    this->checkBehaviourFile(os);
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      if (this->bd.useQt()) {
        os << "template<ModellingHypothesis::Hypothesis hypothesis, "
           << "typename NumericType, bool use_qt>\n"
           << "std::ostream&\n"
           << "operator <<(std::ostream& os,"
           << "const " << this->bd.getClassName()
           << "<hypothesis, NumericType, use_qt>& b)\n";
      } else {
        os << "template<ModellingHypothesis::Hypothesis hypothesis, "
           << "typename NumericType>\n"
           << "std::ostream&\n"
           << "operator <<(std::ostream& os,"
           << "const " << this->bd.getClassName()
           << "<hypothesis, NumericType, false>& b)\n";
      }
    } else {
      if (this->bd.useQt()) {
        os << "template<typename NumericType,bool use_qt>\n"
           << "std::ostream&\n"
           << "operator <<(std::ostream& os,"
           << "const " << this->bd.getClassName() << "<ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h)
           << ", NumericType, use_qt>& b)\n";
      } else {
        os << "template<typename NumericType>\n"
           << "std::ostream&\n"
           << "operator <<(std::ostream& os,"
           << "const " << this->bd.getClassName() << "<ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h)
           << ", NumericType, false>& b)\n";
      }
    }
    os << "{\n";
    for (const auto& v : this->bd.getMainVariables()) {
      if (Gradient::isIncrementKnown(v.first)) {
        os << "os << \"" << displayName(v.first) << " : \" << b."
           << v.first.name << " << '\\n';\n";
        if (getUnicodeOutputOption()) {
          os << "os << \"\u0394" << displayName(v.first) << " : \" << b.d"
             << v.first.name << " << '\\n';\n";
        } else {
          os << "os << \"d" << displayName(v.first) << " : \" << b.d"
             << v.first.name << " << '\\n';\n";
        }
      } else {
        if (getUnicodeOutputOption()) {
          os << "os << \"" << displayName(v.first) << "\u2080 : \" << b."
             << v.first.name << "0 << '\\n';\n"
             << "os << \"" << displayName(v.first) << "\u2081 : \" << b."
             << v.first.name << "1 << '\\n';\n";
        } else {
          os << "os << \"" << displayName(v.first) << "0 : \" << b."
             << v.first.name << "0 << '\\n';\n"
             << "os << \"" << displayName(v.first) << "1 : \" << b."
             << v.first.name << "1 << '\\n';\n";
        }
      }
      os << "os << \"" << displayName(v.second) << " : \" << b."
         << v.second.name << " << '\\n';\n";
    }
    if (getUnicodeOutputOption()) {
      os << "os << \"\u0394t : \" << b.dt << '\\n';\n";
    } else {
      os << "os << \"dt : \" << b.dt << '\\n';\n";
    }
    for (const auto& v : md.getMaterialProperties()) {
      os << "os << \"" << displayName(v) << " : \" << b." << v.name
         << " << '\\n';\n";
    }
    for (const auto& v : md.getStateVariables()) {
      os << "os << \"" << displayName(v) << " : \" << b." << v.name
         << " << '\\n';\n";
      if (getUnicodeOutputOption()) {
        os << "os << \"\u0394" << displayName(v) << " : \" << b.d" << v.name
           << " << '\\n';\n";
      } else {
        os << "os << \"d" << displayName(v) << " : \" << b.d" << v.name
           << " << '\\n';\n";
      }
    }
    for (const auto& v : md.getAuxiliaryStateVariables()) {
      os << "os << \"" << displayName(v) << " : \" << b." << v.name
         << " << '\\n';\n";
    }
    for (const auto& v : md.getExternalStateVariables()) {
      os << "os << \"" << displayName(v) << " : \" << b." << v.name
         << " << '\\n';\n";
      if (getUnicodeOutputOption()) {
        os << "os << \"\u0394" << displayName(v) << " : \" << b.d" << v.name
           << " << '\\n';\n";
      } else {
        os << "os << \"d" << displayName(v) << " : \" << b.d" << v.name
           << " << '\\n';\n";
      }
    }
    for (const auto& v : md.getLocalVariables()) {
#pragma message( \
    "BehaviourCodeGeneratorBase: handle LocalDataStructure properly")
      if ((v.type.size() >= 7) && (v.type.substr(0, 7) != "struct{")) {
        os << "os << \"" << displayName(v) << " : \" << b." << v.name
           << " << '\\n';\n";
      }
    }
    for (const auto& v : md.getParameters()) {
      os << "os << \"" << displayName(v) << " : \" << b." << v.name
         << " << '\\n';\n";
    }
    os << "return os;\n"
       << "}\n\n";
  }

  void BehaviourCodeGeneratorBase::writeBehaviourDestructor(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    os << "//!\n"
       << "~" << this->bd.getClassName() << "()\n"
       << " override = default;\n\n";
  }

  void BehaviourCodeGeneratorBase::writeBehaviourUpdateExternalStateVariables(
      std::ostream& os, const Hypothesis h) const {
    const auto& md = this->bd.getBehaviourData(h);
    this->checkBehaviourFile(os);
    os << "TFEL_HOST_DEVICE void updateExternalStateVariables(){\n";
    for (const auto& v : this->bd.getMainVariables()) {
      if (Gradient::isIncrementKnown(v.first)) {
        os << "this->" << v.first.name << "  += this->d" << v.first.name
           << ";\n";
      } else {
        os << "this->" << v.first.name << "0  = this->" << v.first.name
           << "1;\n";
      }
    }
    for (const auto& v : md.getExternalStateVariables()) {
      os << "this->" << v.name << " += this->d" << v.name << ";\n";
    }
    os << "}\n\n";
  }

  void BehaviourCodeGeneratorBase::writeBehaviourInitializeFunctions(
      std::ostream& os, const Hypothesis h) const {
    for (const auto& i : this->interfaces) {
      i.second->writeBehaviourInitializeFunctions(os, this->bd, h);
    }
  }  // end of writeBehaviourInitializeFunctions

  void BehaviourCodeGeneratorBase::writeBehaviourPostProcessings(
      std::ostream& os, const Hypothesis h) const {
    for (const auto& i : this->interfaces) {
      i.second->writeBehaviourPostProcessings(os, this->bd, h);
    }
  }  // end of writeBehaviourPostProcessings

  void BehaviourCodeGeneratorBase::writeBehaviourIncludes(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    os << "#include<string>\n"
       << "#include<iostream>\n"
       << "#include<limits>\n"
       << "#include<stdexcept>\n"
       << "#include<algorithm>\n\n"
       << "#include\"TFEL/Raise.hxx\"\n"
       << "#include\"TFEL/PhysicalConstants.hxx\"\n"
       << "#include\"TFEL/Config/TFELConfig.hxx\"\n"
       << "#include\"TFEL/Config/TFELTypes.hxx\"\n"
       << "#include\"TFEL/TypeTraits/IsFundamentalNumericType.hxx\"\n"
       << "#include\"TFEL/TypeTraits/IsReal.hxx\"\n"
       << "#include\"TFEL/Math/General/IEEE754.hxx\"\n"
       << "#include\"TFEL/Material/MaterialException.hxx\"\n"
       << "#include\"TFEL/Material/MechanicalBehaviour.hxx\"\n"
       << "#include\"TFEL/Material/MechanicalBehaviourTraits.hxx\"\n"
       << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n"
       << "#include\"TFEL/Material/BoundsCheck.hxx\"\n"
       << "#include\"TFEL/Material/IsotropicPlasticity.hxx\"\n"
       << "#include\"TFEL/Material/Lame.hxx\"\n"
       << "#include\"TFEL/Material/Hosford1972YieldCriterion.hxx\"\n";
    if (this->bd.getSymmetryType() == ORTHOTROPIC) {
      os << "#include\"TFEL/Material/OrthotropicPlasticity.hxx\"\n"
         << "#include\"TFEL/Material/"
            "OrthotropicStressLinearTransformation.hxx\"\n"
         << "#include\"TFEL/Material/Hill.hxx\"\n"
         << "#include\"TFEL/Material/Barlat2004YieldCriterion.hxx\"\n"
         << "#include\"TFEL/Material/OrthotropicAxesConvention.hxx\"\n";
    }
    if (this->bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                              false)) {
      os << "#include\"TFEL/Material/StiffnessTensor.hxx\"\n";
    }
    if ((this->bd.isStrainMeasureDefined()) &&
        (this->bd.getStrainMeasure() == BehaviourDescription::HENCKY)) {
      os << "#include\"TFEL/Material/"
            "LogarithmicStrainComputeAxialStrainIncrementElasticPrediction."
            "hxx\"\n";
    }
    if (this->bd.getAttribute<bool>(BehaviourData::profiling, false)) {
      os << "#include\"MFront/BehaviourProfiler.hxx\"\n";
    }
    os << "#include\"" << this->bd.getBehaviourDataFileName() << "\"\n"
       << "#include\"" << this->bd.getIntegrationDataFileName() << "\"\n";
    os << '\n';
  }

  void BehaviourCodeGeneratorBase::writeBehaviourAdditionalMembers(
      std::ostream& os, const Hypothesis h) const {
    this->checkBehaviourFile(os);
    const auto& m = this->bd.getMembers(h);
    if (!m.empty()) {
      os << m << "\n\n";
    }
  }

  void BehaviourCodeGeneratorBase::writeBehaviourPrivate(
      std::ostream& os, const Hypothesis h) const {
    this->checkBehaviourFile(os);
    const auto& c = this->bd.getPrivateCode(h);
    if (!c.empty()) {
      os << c << "\n\n";
    }
  }  // end of void BehaviourCodeGeneratorBase::writeBehaviourPrivate

  void BehaviourCodeGeneratorBase::writeBehaviourTypeAliases(
      std::ostream& os) const {
    using namespace tfel::material;
    this->checkBehaviourFile(os);
    const auto btype = this->bd.getBehaviourTypeFlag();
    os << "static constexpr unsigned short TVectorSize = N;\n"
       << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n"
       << "static constexpr unsigned short StensorSize = "
       << "StensorDimeToSize::value;\n"
       << "typedef tfel::math::TensorDimeToSize<N> TensorDimeToSize;\n"
       << "static constexpr unsigned short TensorSize = "
       << "TensorDimeToSize::value;\n\n";
    this->writeTypeAliases(os);
    os << '\n' << "public :\n\n";
    const auto qt = this->bd.useQt() ? "use_qt" : "false";
    os << "typedef " << this->bd.getClassName()
       << "BehaviourData<hypothesis, NumericType, " << qt
       << "> BehaviourData;\n"
       << "typedef " << this->bd.getClassName()
       << "IntegrationData<hypothesis, NumericType, " << qt
       << "> IntegrationData;\n"
       << "typedef typename MechanicalBehaviour<" << btype
       << ",hypothesis, NumericType, " << qt << ">::SMFlag SMFlag;\n"
       << "typedef typename MechanicalBehaviour<" << btype
       << ",hypothesis, NumericType, " << qt << ">::SMType SMType;\n"
       << "using MechanicalBehaviour<" << btype << ",hypothesis, NumericType, "
       << qt << ">::ELASTIC;\n"
       << "using MechanicalBehaviour<" << btype << ",hypothesis, NumericType, "
       << qt << ">::SECANTOPERATOR;\n"
       << "using MechanicalBehaviour<" << btype << ",hypothesis, NumericType, "
       << qt << ">::TANGENTOPERATOR;\n"
       << "using MechanicalBehaviour<" << btype << ",hypothesis, NumericType, "
       << qt << ">::CONSISTENTTANGENTOPERATOR;\n"
       << "using MechanicalBehaviour<" << btype << ",hypothesis, NumericType, "
       << qt << ">::NOSTIFFNESSREQUESTED;\n";
    if ((this->bd.getBehaviourType() ==
         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
        (this->bd.getBehaviourType() ==
         BehaviourDescription::COHESIVEZONEMODEL)) {
      os << "using MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType, " << qt
         << ">::STANDARDTANGENTOPERATOR;\n";
    } else if (this->bd.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      for (const auto& toflag :
           getFiniteStrainBehaviourTangentOperatorFlags()) {
        os << "using MechanicalBehaviour<" << btype
           << ",hypothesis, NumericType, " << qt << ">::"
           << convertFiniteStrainBehaviourTangentOperatorFlagToString(toflag)
           << ";\n";
      }
    }
    os << "using IntegrationResult = typename MechanicalBehaviour<" << btype
       << ",hypothesis, NumericType, " << qt << ">::IntegrationResult;\n\n"
       << "using MechanicalBehaviour<" << btype << ",hypothesis, NumericType, "
       << qt << ">::SUCCESS;\n"
       << "using MechanicalBehaviour<" << btype << ",hypothesis, NumericType, "
       << qt << ">::FAILURE;\n"
       << "using MechanicalBehaviour<" << btype << ",hypothesis, NumericType, "
       << qt << ">::UNRELIABLE_RESULTS;\n\n";
    if ((this->bd.getBehaviourType() ==
         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
        (this->bd.getBehaviourType() ==
         BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)) {
      os << "using StressFreeExpansionType = "
         << this->bd.getStressFreeExpansionType() << ";\n\n";
    }
  }  // end of writeBehaviourTypeAliases

  void BehaviourCodeGeneratorBase::writeBehaviourTraits(
      std::ostream& os) const {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->checkBehaviourFile(os);
    const auto& ah = ModellingHypothesis::getModellingHypotheses();
    // writing partial specialisations
    if (this->bd.getModellingHypotheses().size() >= 4u) {
      // on définit toutes les hypothèses par défaut
      this->writeBehaviourTraitsSpecialisation(os, uh, true);
      // unsupported hypothesis
      for (const auto h : ah) {
        if (this->bd.isModellingHypothesisSupported(h)) {
          if (this->bd.hasSpecialisedMechanicalData(h)) {
            this->writeBehaviourTraitsSpecialisation(os, h, true);
          }
        } else {
          this->writeBehaviourTraitsSpecialisation(os, h, false);
        }
      }
    } else {
      // on exclut toutes les hypothèses par défaut
      this->writeBehaviourTraitsSpecialisation(os, uh, false);
      // unsupported hypothesis
      for (const auto h : this->bd.getModellingHypotheses()) {
        this->writeBehaviourTraitsSpecialisation(os, h, true);
      }
    }
  }

  void BehaviourCodeGeneratorBase::writeBehaviourTraitsSpecialisation(
      std::ostream& os, const Hypothesis h, const bool b) const {
    auto boolean = [](const bool bv) { return bv ? "true" : "false"; };
    auto get_boolean_attribute = [boolean, &b, &h,
                                  this](const char* const attribute) {
      if (b) {
        if (this->bd.getAttribute<bool>(h, attribute, false)) {
          return boolean(true);
        }
      }
      return boolean(false);
    };
    auto has_code = [boolean, &b, &h, this](const char* const cb) {
      if (b) {
        if (this->bd.hasCode(h, cb)) {
          return boolean(true);
        }
      }
      return boolean(false);
    };
    SupportedTypes::TypeSize coefSize;
    SupportedTypes::TypeSize stateVarsSize;
    SupportedTypes::TypeSize externalStateVarsSize;
    if (b) {
      const auto& d = this->bd.getBehaviourData(h);
      for (const auto& m : d.getMaterialProperties()) {
        coefSize += SupportedTypes::getTypeSize(m.type, m.arraySize);
      }
      for (const auto& v : d.getPersistentVariables()) {
        stateVarsSize += SupportedTypes::getTypeSize(v.type, v.arraySize);
      }
      for (const auto& v : d.getExternalStateVariables()) {
        externalStateVarsSize +=
            SupportedTypes::getTypeSize(v.type, v.arraySize);
      }
    }
    os << "/*!\n"
       << "* Partial specialisation for " << this->bd.getClassName() << ".\n"
       << "*/\n";
    const auto cn = this->bd.getFullClassName();
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      if (this->bd.useQt()) {
        os << "template<ModellingHypothesis::Hypothesis hypothesis, "
           << "typename NumericType,bool use_qt>\n"
           << "class MechanicalBehaviourTraits<" << cn
           << "<hypothesis, NumericType, use_qt> >\n";
      } else {
        os << "template<ModellingHypothesis::Hypothesis hypothesis, "
           << "typename NumericType>\n"
           << "class MechanicalBehaviourTraits<" << cn
           << "<hypothesis, NumericType, false> >\n";
      }
    } else {
      if (this->bd.useQt()) {
        os << "template<typename NumericType,bool use_qt>\n"
           << "class MechanicalBehaviourTraits<" << cn
           << "<ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h)
           << ", NumericType, use_qt> >\n";
      } else {
        os << "template<typename NumericType>\n"
           << "class MechanicalBehaviourTraits<" << cn
           << "<ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h)
           << ", NumericType, false> >\n";
      }
    }
    os << "{\n"
       << "using size_type = unsigned short;\n";
    if (b) {
      if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        os << "static constexpr unsigned short N = "
           << "ModellingHypothesisToSpaceDimension<hypothesis>::value;\n";
      } else {
        os << "static constexpr unsigned short N = "
              "ModellingHypothesisToSpaceDimension<"
           << "ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h) << ">::value;\n";
      }
      os << "static constexpr unsigned short TVectorSize = N;\n"
         << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n"
         << "static constexpr unsigned short StensorSize = "
         << "StensorDimeToSize::value;\n"
         << "typedef tfel::math::TensorDimeToSize<N> TensorDimeToSize;\n"
         << "static constexpr unsigned short TensorSize = "
         << "TensorDimeToSize::value;\n";
    }
    os << "public:\n";
    os << "static constexpr bool is_defined = " << boolean(b) << ";\n";
    if (this->bd.useQt()) {
      os << "static constexpr bool use_quantities = use_qt;\n";
    } else {
      os << "static constexpr bool use_quantities = false;\n";
    }
    if (this->bd.getSymmetryType() == mfront::ORTHOTROPIC) {
      os << "//! orthotropic axes convention\n";
      if (this->bd.getOrthotropicAxesConvention() ==
          OrthotropicAxesConvention::DEFAULT) {
        os << "static constexpr OrthotropicAxesConvention oac = "
           << "OrthotropicAxesConvention::DEFAULT;\n";
      } else if (this->bd.getOrthotropicAxesConvention() ==
                 OrthotropicAxesConvention::PIPE) {
        os << "static constexpr OrthotropicAxesConvention oac = "
           << "OrthotropicAxesConvention::PIPE;\n";
      } else if (this->bd.getOrthotropicAxesConvention() ==
                 OrthotropicAxesConvention::PLATE) {
        os << "static constexpr OrthotropicAxesConvention oac = "
           << "OrthotropicAxesConvention::PLATE;\n";
      } else {
        this->throwRuntimeError(
            "BehaviourCodeGeneratorBase::writeBehaviourTraitsSpecialisation",
            "internal error : unsupported orthotropic axes convention");
      }
    }
    os << "static constexpr bool hasStressFreeExpansion = "
       << boolean((b) && (this->bd.requiresStressFreeExpansionTreatment(h)))
       << ";\n";
    os << "static constexpr bool handlesThermalExpansion = "
       << boolean(this->bd.areThermalExpansionCoefficientsDefined()) << ";\n";
    if (b) {
      os << "static constexpr unsigned short dimension = N;\n";
    } else {
      os << "static constexpr unsigned short dimension = 0u;\n";
    }
    os << "static constexpr size_type material_properties_nb = " << coefSize
       << ";\n"
       << "static constexpr size_type internal_variables_nb  = "
       << stateVarsSize << ";\n";
    if (this->bd.getAttribute(
            BehaviourDescription::
                automaticDeclarationOfTheTemperatureAsFirstExternalStateVariable,
            true)) {
      SupportedTypes::TypeSize externalStateVarsSize2 = externalStateVarsSize;
      if (b) {
        externalStateVarsSize2 -=
            SupportedTypes::TypeSize(SupportedTypes::SCALAR);
      }
      os << "static constexpr size_type external_variables_nb  = "
         << externalStateVarsSize << ";\n"
         << "static constexpr size_type external_variables_nb2 = "
         << externalStateVarsSize2 << ";\n";
    } else {
      os << "static constexpr size_type external_variables_nb  = "
         << externalStateVarsSize << ";\n"
         << "static constexpr size_type external_variables_nb2 = "
         << externalStateVarsSize << ";\n";
    }
    os << "static constexpr bool hasConsistentTangentOperator = "
       << get_boolean_attribute(BehaviourData::hasConsistentTangentOperator)
       << ";\n";
    os << "static constexpr bool isConsistentTangentOperatorSymmetric = "
       << get_boolean_attribute(
              BehaviourData::isConsistentTangentOperatorSymmetric)
       << ";\n";
    os << "static constexpr bool hasPredictionOperator = "
       << get_boolean_attribute(BehaviourData::hasPredictionOperator) << ";\n";
    os << "static constexpr bool hasAPrioriTimeStepScalingFactor = "
       << get_boolean_attribute(BehaviourData::hasAPrioriTimeStepScalingFactor)
       << ";\n";
    // internal enery
    os << "static constexpr bool hasComputeInternalEnergy = "
       << has_code(BehaviourData::ComputeInternalEnergy) << ";\n";
    // dissipated energy
    os << "static constexpr bool hasComputeDissipatedEnergy = "
       << has_code(BehaviourData::ComputeDissipatedEnergy) << ";\n";
    // name of the class
    os << "/*!\n"
       << "* \\return the name of the class.\n"
       << "*/\n"
       << "static const char* getName(){\n"
       << "return \"" << this->bd.getClassName() << "\";\n"
       << "}\n\n"
       << "};\n\n";
  }

  void BehaviourCodeGeneratorBase::
      writeBehaviourParserSpecificInheritanceRelationship(
          std::ostream& os, const Hypothesis) const {
    os << '\n';
  }

  void BehaviourCodeGeneratorBase::writeBehaviourParserSpecificTypedefs(
      std::ostream&) const {
    // Empty member meant to be overriden in Child if necessary
  }

  void BehaviourCodeGeneratorBase::writeBehaviourParserSpecificMembers(
      std::ostream&, const Hypothesis) const {
    // Empty member meant to be overriden in Child if necessary
  }

  void BehaviourCodeGeneratorBase::writeBehaviourParserSpecificIncludes(
      std::ostream&) const {
    // Empty member meant to be overriden in Child if necessary
  }

  void BehaviourCodeGeneratorBase::writeBehaviourParametersInitializers(
      std::ostream& os) const {
    if ((areParametersTreatedAsStaticVariables(this->bd)) ||
        (!this->bd.hasParameters())) {
      return;
    }
    auto mh = this->bd.getDistinctModellingHypotheses();
    mh.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    for (const auto h : mh) {
      if (this->bd.hasParameters(h)) {
        this->writeBehaviourParametersInitializer(os, h);
      }
    }
  }  // end of writeBehaviourParametersInitializers

  void BehaviourCodeGeneratorBase::writeBehaviourParametersInitializer(
      std::ostream& os, const Hypothesis h) const {
    // useless and paranoid test
    if ((areParametersTreatedAsStaticVariables(this->bd)) ||
        (!this->bd.hasParameters())) {
      return;
    }
    const auto& md = this->bd.getBehaviourData(h);
    const auto& params = md.getParameters();
    std::string cname(this->bd.getClassName());
    if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      cname += ModellingHypothesis::toString(h);
    }
    cname += "ParametersInitializer";
    bool rp = false;
    bool ip = false;
    bool up = false;
    bool rp2 = false;
    bool ip2 = false;
    bool up2 = false;
    this->checkBehaviourFile(os);
    os << "struct " << cname << '\n'
       << "{\n"
       << "static " << cname << "&\n"
       << "get();\n\n";
    for (const auto& p : params) {
      if (p.type == "int") {
        ip = true;
        if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
            ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
             (!this->bd.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                     p.name)))) {
          ip2 = true;
          os << "int " << p.name << ";\n";
        }
      } else if (p.type == "ushort") {
        up = true;
        if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
            ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
             (!this->bd.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                     p.name)))) {
          up2 = true;
          os << "unsigned short " << p.name << ";\n";
        }
      } else {
        const auto f = SupportedTypes::getTypeFlag(p.type);
        if (f != SupportedTypes::SCALAR) {
          this->throwRuntimeError(
              "BehaviourCodeGeneratorBase::writeBehaviourParametersInitializer",
              "invalid type for parameter '" + p.name + "' ('" + p.type + "')");
        }
        rp = true;
        if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
            ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
             (!this->bd.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                     p.name)))) {
          rp2 = true;
          if (p.arraySize == 1) {
            os << "double " << p.name << ";\n";
          } else {
            os << "tfel::math::fsarray<" << p.arraySize << ",double> " << p.name
               << ";\n";
          }
        }
      }
    }
    if (!params.empty()) {
      os << '\n';
    }
    if (rp) {
      os << "void set(const char* const,const double);\n\n";
    }
    if (ip) {
      os << "void set(const char* const,const int);\n\n";
    }
    if (up) {
      os << "void set(const char* const,const unsigned short);\n\n";
    }
    if (rp2) {
      os << "/*!\n"
         << " * \\brief convert a string to double\n"
         << " * \\param[in] p : parameter\n"
         << " * \\param[in] v : value\n"
         << " */\n"
         << "static double getDouble(const std::string&,const std::string&);\n";
    }
    if (ip2) {
      os << "/*!\n"
         << " * \\brief convert a string to int\n"
         << " * \\param[in] p : parameter\n"
         << " * \\param[in] v : value\n"
         << " */\n"
         << "static int getInt(const std::string&,const std::string&);\n";
    }
    if (up2) {
      os << "/*!\n"
         << " * \\brief convert a string to unsigned short\n"
         << " * \\param[in] p : parameter\n"
         << " * \\param[in] v : value\n"
         << " */\n"
         << "static unsigned short getUnsignedShort(const std::string&,const "
            "std::string&);\n";
    }
    os << "private :\n\n"
       << cname << "();\n\n"
       << cname << "(const " << cname << "&);\n\n"
       << cname << "&\n"
       << "operator=(const " << cname << "&);\n";
    if (allowsParametersInitializationFromFile(this->bd)) {
      os << "/*!\n"
         << " * \\brief read the parameters from the given file\n"
         << " * \\param[out] pi : parameters initializer\n"
         << " * \\param[in]  fn : file name\n"
         << " */\n"
         << "static void readParameters(" << cname << "&,const char* const);\n";
    }
    os << "};\n\n";
  }  // end of writeBehaviourParametersInitializer

  void
  BehaviourCodeGeneratorBase::writeBehaviourParserSpecificInitializeMethodPart(
      std::ostream&, const Hypothesis) const {
    // Empty member meant to be overriden in Child if necessary
  }

  void BehaviourCodeGeneratorBase::writeBehaviourFileBegin(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    this->writeBehaviourFileHeader(os);
    this->writeBehaviourFileHeaderBegin(os);
    this->writeBehaviourIncludes(os);
    this->writeBehaviourParserSpecificIncludes(os);
    this->writeIncludes(os);
    // includes specific to interfaces
    for (const auto& i : this->interfaces) {
      i.second->writeInterfaceSpecificIncludes(os, this->bd);
    }
    this->writeNamespaceBegin(os);
    this->writeBehaviourParametersInitializers(os);
    this->writeBehaviourForwardDeclarations(os);
    this->writeBehaviourProfiler(os);
  }  // end of writeBehaviourFileBegin

  void BehaviourCodeGeneratorBase::writeBehaviourProfiler(
      std::ostream& os) const {
    if (this->bd.getAttribute(BehaviourData::profiling, false)) {
      this->checkBehaviourFile(os);
      os << "/*!\n"
         << " * " << this->bd.getClassName() << " profiler\n"
         << " */\n"
         << "struct " << this->bd.getClassName() << "Profiler\n"
         << "{\n"
         << "//! return the profiler associated with the behaviour\n"
         << "static mfront::BehaviourProfiler& getProfiler();\n"
         << "}; // end of struct " << this->bd.getClassName() << "Profiler\n\n";
    }
  }  // end of writeBehaviourProfiler

  void BehaviourCodeGeneratorBase::writeBehaviourClass(
      std::ostream& os, const Hypothesis h) const {
    this->checkBehaviourFile(os);
    this->writeBehaviourClassBegin(os, h);
    this->writeBehaviourFriends(os, h);
    this->writeBehaviourTypeAliases(os);
    os << "private :\n\n";
    this->writeBehaviourParserSpecificTypedefs(os);
    this->writeBehaviourStaticVariables(os, h);
    os << "protected:\n\n";
    this->writeBehaviourIntegrationVariables(os, h);
    this->writeBehaviourIntegrationVariablesIncrements(os, h);
    os << "private :\n\n";
    this->writeBehaviourLocalVariables(os, h);
    this->writeBehaviourParameters(os, h);
    this->writeBehaviourTangentOperator(os);
    this->writeBehaviourParserSpecificMembers(os, h);
    this->writeBehaviourUpdateIntegrationVariables(os, h);
    this->writeBehaviourUpdateStateVariables(os, h);
    this->writeBehaviourUpdateAuxiliaryStateVariables(os, h);
    this->writeBehaviourAdditionalMembers(os, h);
    this->writeBehaviourPrivate(os, h);
    this->writeBehaviourDisabledConstructors(os);
    // from this point, all is public
    os << "public:\n\n";
    this->writeBehaviourConstructors(os, h);
    this->writeBehaviourInitializeFunctions(os, h);
    this->writeBehaviourComputeStressFreeExpansion(os, h);
    this->writeBehaviourInitializeMethods(os, h);
    this->writeBehaviourSetOutOfBoundsPolicy(os);
    this->writeBehaviourGetModellingHypothesis(os);
    this->writeBehaviourCheckBounds(os, h);
    this->writeBehaviourComputePredictionOperator(os, h);
    this->writeBehaviourGetTimeStepScalingFactor(os);
    this->writeBehaviourComputeAPrioriTimeStepScalingFactor(os);
    this->writeBehaviourIntegrator(os, h);
    this->writeBehaviourComputeAPosterioriTimeStepScalingFactor(os);
    this->writeBehaviourComputeInternalEnergy(os, h);
    this->writeBehaviourComputeDissipatedEnergy(os, h);
    this->writeBehaviourComputeTangentOperator(os, h);
    this->writeBehaviourComputeSpeedOfSound(os, h);
    this->writeBehaviourGetTangentOperator(os);
    this->writeBehaviourUpdateExternalStateVariables(os, h);
    this->writeBehaviourPostProcessings(os, h);
    this->writeBehaviourDestructor(os);
    this->checkBehaviourFile(os);
    os << "private:\n\n";
    this->writeBehaviourDisabledAssignementOperators(os);
    this->writeBehaviourComputeAPrioriTimeStepScalingFactorII(os, h);
    this->writeBehaviourComputeAPosterioriTimeStepScalingFactorII(os, h);
    this->writeBehaviourBehaviourVariablesMethods(os, h);
    this->writeBehaviourOutOfBoundsPolicyVariable(os);
    this->writeBehaviourClassEnd(os);
    this->writeBehaviourOutputOperator(os, h);
  }

  void BehaviourCodeGeneratorBase::writeBehaviourFileEnd(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    this->writeNamespaceEnd(os);
    os << "namespace tfel::material{\n\n";
    this->writeBehaviourTraits(os);
    os << "} // end of namespace tfel::material\n\n";
    this->writeBehaviourFileHeaderEnd(os);
  }  // end of writeBehaviourFileBegin

  static bool hasUserDefinedPredictionOperatorCode(
      const BehaviourDescription& mb,
      const tfel::material::ModellingHypothesis::Hypothesis h) {
    using tfel::material::getFiniteStrainBehaviourTangentOperatorFlags;
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      // all available tangent operators for finite strain behaviours
      const auto tos = getFiniteStrainBehaviourTangentOperatorFlags();
      // search tangent operators defined by the user
      for (const auto& t : tos) {
        const auto ktype =
            convertFiniteStrainBehaviourTangentOperatorFlagToString(t);
        if (mb.hasCode(
                h, std::string(BehaviourData::ComputePredictionOperator) + '-' +
                       ktype)) {
          return true;
        }
      }
    } else {
      if (mb.hasCode(h, BehaviourData::ComputePredictionOperator)) {
        return true;
      }
    }
    return false;
  }  // end of hasUserDefinedTangentOperatorCode

  void BehaviourCodeGeneratorBase::writeBehaviourComputePredictionOperator(
      std::ostream& os, const Hypothesis h) const {
    using namespace tfel::material;

    const auto btype = this->bd.getBehaviourTypeFlag();
    if ((!this->bd.getAttribute<bool>(h, BehaviourData::hasPredictionOperator,
                                      false)) &&
        (this->bd.hasCode(h, BehaviourData::ComputePredictionOperator))) {
      this->throwRuntimeError(
          "BehaviourCodeGeneratorBase::writeBehaviourComputePredictionOperator "
          ": ",
          "attribute 'hasPredictionOperator' is set but no associated code "
          "defined");
    }
    if (!hasUserDefinedPredictionOperatorCode(this->bd, h)) {
      os << "[[nodiscard]] IntegrationResult computePredictionOperator("
         << "const SMFlag, const SMType) override final{\n"
         << "tfel::raise(\"" << this->bd.getClassName()
         << "::computePredictionOperator: \"\n"
         << "\"unsupported prediction operator flag\");\n"
         << "}\n\n";
      return;
    }
    if (this->bd.getBehaviourType() ==
        BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      // all available tangent operators for finite strain behaviours
      const auto tos(getFiniteStrainBehaviourTangentOperatorFlags());
      // all known converters
      const auto converters = FiniteStrainBehaviourTangentOperatorConversion::
          getAvailableFiniteStrainBehaviourTangentOperatorConversions();
      // tangent operators defined by the user
      std::vector<FiniteStrainBehaviourTangentOperatorBase::Flag> ktos;
      for (const auto& t : tos) {
        const auto ktype =
            convertFiniteStrainBehaviourTangentOperatorFlagToString(t);
        if (this->bd.hasCode(
                h, std::string(BehaviourData::ComputePredictionOperator) + '-' +
                       ktype)) {
          ktos.push_back(t);
        }
      }
      if (!ktos.empty()) {
        // computing all the conversion paths starting from user defined ones
        std::vector<FiniteStrainBehaviourTangentOperatorConversionPath> paths;
        for (const auto& k : ktos) {
          const auto kpaths =
              FiniteStrainBehaviourTangentOperatorConversionPath::
                  getConversionsPath(k, ktos, converters);
          paths.insert(paths.end(), kpaths.begin(), kpaths.end());
        }
        for (const auto& t : tos) {
          const auto ktype =
              convertFiniteStrainBehaviourTangentOperatorFlagToString(t);
          if (std::find(ktos.begin(), ktos.end(), t) != ktos.end()) {
            os << "[[nodiscard]] TFEL_HOST_DEVICE "
                  "IntegrationResult\ncomputePredictionOperator_"
               << ktype << "(const SMType smt){\n"
               << "using namespace std;\n"
               << "using namespace tfel::math;\n"
               << "using std::vector;\n";
            writeMaterialLaws(os, this->bd.getMaterialLaws());
            os << this->bd.getCode(
                      h, std::string(BehaviourData::ComputePredictionOperator) +
                             "-" + ktype)
               << '\n'
               << "return SUCCESS;\n"
               << "}\n\n";
          } else {
            if ((h ==
                 ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) ||
                (h == ModellingHypothesis::PLANESTRESS)) {
              os << "[[nodiscard]] TFEL_HOST_DEVICE IntegrationResult "
                    "computePredictionOperator_"
                 << ktype << "(const SMType){\n"
                 << "tfel::raise(\"" << this->bd.getClassName()
                 << "::computePredictionOperator_" << ktype << ": \"\n"
                 << "\"computing the prediction operator '" << ktype
                 << "' is not supported\");\n"
                 << "}\n\n";
            } else {
              const auto path =
                  FiniteStrainBehaviourTangentOperatorConversionPath::
                      getShortestPath(paths, t);
              if (path.empty()) {
                os << "[[nodiscard]] TFEL_HOST_DEVICE IntegrationResult "
                      "computePredictionOperator_"
                   << ktype << "(const SMType){\n"
                   << "tfel::raise(\"" << this->bd.getClassName()
                   << "::computePredictionOperator_" << ktype << ": \"\n"
                   << "\"computing the prediction operator '" << ktype
                   << "' is not supported\");\n"
                   << "}\n\n";
              } else {
                os << "[[nodiscard]] TFEL_HOST_DEVICE IntegrationResult "
                      "computePredictionOperator_"
                   << ktype << "(const SMType smt){\n";
                auto pc = path.begin();
                const auto k =
                    convertFiniteStrainBehaviourTangentOperatorFlagToString(
                        pc->from());
                const auto from_type_flag =
                    getFiniteStrainBehaviourTangentOperatorFlagType(pc->from());
                os << "using namespace tfel::math;\n";
                os << "// computing " << k << '\n';
                os << "if(this->computePredictionOperator_" << k
                   << "(smt) != SUCCESS){\n"
                   << "return FAILURE;\n"
                   << "}\n"
                   << "const " << from_type_flag << "<N,stress> "
                   << "tangentOperator_" << k << " = this->Dt.template get<"
                   << from_type_flag << "<N,stress> >();\n";
                for (; pc != path.end();) {
                  const auto converter = *pc;
                  if (++pc == path.end()) {
                    os << converter.getFinalConversion() << '\n';
                  } else {
                    os << converter.getIntermediateConversion() << '\n';
                  }
                }
                os << "return SUCCESS;\n"
                   << "}\n\n";
              }
            }
          }
        }
        os << "[[nodiscard]] TFEL_HOST_DEVICE IntegrationResult "
           << "computePredictionOperator(const SMFlag smflag, "
           << "const SMType smt) override final{\n"
           << "using namespace std;\n"
           << "switch(smflag){\n";
        for (const auto& t : tos) {
          const auto ktype =
              convertFiniteStrainBehaviourTangentOperatorFlagToString(t);
          os << "case " << ktype << ":\n"
             << "return this->computePredictionOperator_" << ktype
             << "(smt);\n";
        }
        os << "}\n"
           << "tfel::raise(\"" << this->bd.getClassName()
           << "::computePredictionOperator: \"\n"
           << "\"unsupported prediction operator flag\");\n"
           << "}\n\n";
      }
    } else {
      os << "[[nodiscard]] TFEL_HOST_DEVICE IntegrationResult\n"
         << "computePredictionOperator(const SMFlag smflag,const SMType smt) "
         << "override final{\n"
         << "using namespace std;\n"
         << "using namespace tfel::math;\n"
         << "using std::vector;\n";
      writeMaterialLaws(os, this->bd.getMaterialLaws());
      if ((this->bd.getBehaviourType() ==
           BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
          (this->bd.getBehaviourType() ==
           BehaviourDescription::COHESIVEZONEMODEL) ||
          (this->bd.getBehaviourType() ==
           BehaviourDescription::GENERALBEHAVIOUR)) {
        if (this->bd.useQt()) {
          os << "tfel::raise_if(smflag!=MechanicalBehaviour<" << btype
             << ",hypothesis, NumericType, use_qt>::STANDARDTANGENTOPERATOR,\n"
             << "\"invalid prediction operator flag\");\n";
        } else {
          os << "tfel::raise_if(smflag!=MechanicalBehaviour<" << btype
             << ",hypothesis, NumericType, false>::STANDARDTANGENTOPERATOR,\n"
             << "\"invalid prediction operator flag\");\n";
        }
      }
      os << this->bd.getCode(h, BehaviourData::ComputePredictionOperator)
         << "return SUCCESS;\n"
         << "}\n\n";
    }
  }  // end of writeBehaviourComputePredictionOperator

  void BehaviourCodeGeneratorBase::writeBehaviourComputeTangentOperator(
      std::ostream& os, const Hypothesis h) const {
    using namespace tfel::material;
    if (this->bd.getBehaviourType() ==
        BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      // all available tangent operators for finite strain behaviours
      const auto tos(getFiniteStrainBehaviourTangentOperatorFlags());
      // all known converters
      const auto converters = FiniteStrainBehaviourTangentOperatorConversion::
          getAvailableFiniteStrainBehaviourTangentOperatorConversions();
      // tangent operators defined by the user
      std::vector<FiniteStrainBehaviourTangentOperatorBase::Flag> ktos;
      for (const auto& t : tos) {
        const auto ktype =
            convertFiniteStrainBehaviourTangentOperatorFlagToString(t);
        if (this->bd.hasCode(
                h, std::string(BehaviourData::ComputeTangentOperator) + '-' +
                       ktype)) {
          ktos.push_back(t);
        }
      }
      if (!ktos.empty()) {
        // computing all the conversion paths starting from user defined ones
        std::vector<FiniteStrainBehaviourTangentOperatorConversionPath> paths;
        for (const auto& k : ktos) {
          const auto kpaths =
              FiniteStrainBehaviourTangentOperatorConversionPath::
                  getConversionsPath(k, ktos, converters);
          paths.insert(paths.end(), kpaths.begin(), kpaths.end());
        }
        for (const auto& t : tos) {
          const auto ktype =
              convertFiniteStrainBehaviourTangentOperatorFlagToString(t);
          if (find(ktos.begin(), ktos.end(), t) != ktos.end()) {
            os << "TFEL_HOST_DEVICE bool computeConsistentTangentOperator_"
               << ktype << "(const SMType smt){\n"
               << "using namespace std;\n"
               << "using namespace tfel::math;\n"
               << "using std::vector;\n";
            os << "auto mfront_success = true;\n";
            writeMaterialLaws(os, this->bd.getMaterialLaws());
            this->writeBehaviourComputeTangentOperatorBody(
                os, h,
                std::string(BehaviourData::ComputeTangentOperator) + "-" +
                    ktype);
            os << "return mfront_success;\n"
               << "}\n\n";
          } else {
            if ((h ==
                 ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) ||
                (h == ModellingHypothesis::PLANESTRESS)) {
              os << "TFEL_HOST_DEVICE bool computeConsistentTangentOperator_"
                 << ktype << "(const SMType){\n"
                 << "tfel::raise(\"" << this->bd.getClassName()
                 << "::computeConsistentTangentOperator_" << ktype << ": \"\n"
                 << "\"computing the tangent operator '" << ktype
                 << "' is not supported\");\n"
                 << "}\n\n";
            } else {
              const auto path =
                  FiniteStrainBehaviourTangentOperatorConversionPath::
                      getShortestPath(paths, t);
              if (path.empty()) {
                os << "TFEL_HOST_DEVICE bool computeConsistentTangentOperator_"
                   << ktype << "(const SMType){\n"
                   << "tfel::raise(\"" << this->bd.getClassName()
                   << "::computeConsistentTangentOperator_" << ktype << ": \"\n"
                   << "\"computing the tangent operator '" << ktype
                   << "' is not supported\");\n"
                   << "}\n\n";
              } else {
                auto pc = path.begin();
                const auto k =
                    convertFiniteStrainBehaviourTangentOperatorFlagToString(
                        pc->from());
                const auto k_type_flag =
                    getFiniteStrainBehaviourTangentOperatorFlagType(pc->from());
                os << "TFEL_HOST_DEVICE bool computeConsistentTangentOperator_"
                   << ktype << "(const SMType smt){\n";
                os << "using namespace tfel::math;\n";
                os << "// computing " << k << '\n';
                os << "if(!this->computeConsistentTangentOperator_" << k
                   << "(smt)){\n"
                   << "return false;\n"
                   << "}\n"
                   << "const " << k_type_flag << "<N,stress> "
                   << "tangentOperator_" << k << " = this->Dt.template get<"
                   << k_type_flag << "<N,stress>>();\n";
                for (; pc != path.end();) {
                  const auto converter = *pc;
                  if (++pc == path.end()) {
                    os << converter.getFinalConversion() << '\n';
                  } else {
                    os << converter.getIntermediateConversion() << '\n';
                  }
                }
                os << "return true;\n"
                   << "}\n\n";
              }
            }
          }
        }
        os << "TFEL_HOST_DEVICE bool computeConsistentTangentOperator(const "
              "SMFlag smflag,const "
              "SMType smt){\n"
           << "switch(smflag){\n";
        for (const auto& t : tos) {
          const auto ktype =
              convertFiniteStrainBehaviourTangentOperatorFlagToString(t);
          os << "case " << ktype << ":\n"
             << "return this->computeConsistentTangentOperator_" << ktype
             << "(smt);\n";
        }
        os << "}\n"
           << "tfel::raise(\"" << this->bd.getClassName()
           << "::computeConsistentTangentOperator: \"\n"
           << "\"unsupported tangent operator flag\");\n"
           << "}\n\n";
      }
    } else {
      if (this->bd.hasCode(h, BehaviourData::ComputeTangentOperator)) {
        os << "TFEL_HOST_DEVICE bool computeConsistentTangentOperator(const "
              "SMType smt){\n"
           << "using namespace std;\n"
           << "using namespace tfel::math;\n"
           << "using std::vector;\n";
        os << "auto mfront_success = true;\n";
        writeMaterialLaws(os, this->bd.getMaterialLaws());
        this->writeBehaviourComputeTangentOperatorBody(
            os, h, BehaviourData::ComputeTangentOperator);
        os << "return mfront_success;\n"
           << "}\n\n";
      }
    }
  }  // end of writeBehaviourComputeTangentOperator

  void BehaviourCodeGeneratorBase::writeBehaviourComputeTangentOperatorBody(
      std::ostream& os, const Hypothesis h, const std::string& n) const {
    os << this->bd.getCode(h, n) << '\n';
  }  // end of writeBehaviourComputeTangentOperatorBody

  void BehaviourCodeGeneratorBase::writeBehaviourGetTangentOperator(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    if (this->bd.hasTangentOperator()) {
      os << "TFEL_HOST_DEVICE const TangentOperator& getTangentOperator() "
            "const{\n"
         << "return this->Dt;\n"
         << "}\n\n";
    }
  }  // end of writeBehaviourComputeTangentOperator()

  void BehaviourCodeGeneratorBase::writeBehaviourGetTimeStepScalingFactor(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    os << "TFEL_HOST_DEVICE real getMinimalTimeStepScalingFactor() const "
          "noexcept override final{\n"
          "  return this->minimal_time_step_scaling_factor;\n"
          "}\n\n";
  }

  void
  BehaviourCodeGeneratorBase::writeBehaviourComputeAPrioriTimeStepScalingFactor(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    os << "TFEL_HOST_DEVICE std::pair<bool, real>\n"
          "computeAPrioriTimeStepScalingFactor(const real "
          "current_time_step_scaling_factor) const override final{\n"
          "const auto time_scaling_factor = "
          "this->computeAPrioriTimeStepScalingFactorII();\n"
          "return {time_scaling_factor.first,\n"
          "        std::min(std::min(std::max(time_scaling_factor.second,\n"
          "                                   "
          "this->minimal_time_step_scaling_factor),\n"
          "                          this->maximal_time_step_scaling_factor),\n"
          "                  current_time_step_scaling_factor)};\n"
          "}\n\n";
  }  // end of writeBehaviourComputeAPrioriTimeStepScalingFactor

  void BehaviourCodeGeneratorBase::
      writeBehaviourComputeAPrioriTimeStepScalingFactorII(
          std::ostream& os, const Hypothesis h) const {
    this->checkBehaviourFile(os);
    os << "TFEL_HOST_DEVICE std::pair<bool, real> "
          "computeAPrioriTimeStepScalingFactorII() "
          "const{\n";
    if (this->bd.hasCode(h, BehaviourData::APrioriTimeStepScalingFactor)) {
      os << "using namespace std;\n"
         << "using namespace tfel::math;\n"
         << "using std::vector;\n";
      writeMaterialLaws(os, this->bd.getMaterialLaws());
      os << this->bd.getCode(h, BehaviourData::APrioriTimeStepScalingFactor)
         << '\n';
    }
    os << "return {true, this->maximal_time_step_scaling_factor};\n"
       << "}\n\n";
  }

  void BehaviourCodeGeneratorBase::
      writeBehaviourComputeAPosterioriTimeStepScalingFactor(
          std::ostream& os) const {
    this->checkBehaviourFile(os);
    os << "TFEL_HOST_DEVICE std::pair<bool, real>\n"
          "computeAPosterioriTimeStepScalingFactor(const real "
          "current_time_step_scaling_factor) const override final{\n"
          "const auto time_scaling_factor = "
          "this->computeAPosterioriTimeStepScalingFactorII();\n"
          "return {time_scaling_factor.first,\n"
          "        std::min(std::min(std::max(time_scaling_factor.second,\n"
          "                                   "
          "this->minimal_time_step_scaling_factor),\n"
          "                          this->maximal_time_step_scaling_factor),\n"
          "                 current_time_step_scaling_factor)};\n"
          "}\n\n";
  }  // end of
     // writeBehaviourComputeAPosterioriTimeStepScalingFactor

  void BehaviourCodeGeneratorBase::
      writeBehaviourComputeAPosterioriTimeStepScalingFactorII(
          std::ostream& os, const Hypothesis h) const {
    this->checkBehaviourFile(os);
    os << "TFEL_HOST_DEVICE std::pair<bool, real> "
          "computeAPosterioriTimeStepScalingFactorII() "
          "const{\n";
    if (this->bd.hasCode(h, BehaviourData::APosterioriTimeStepScalingFactor)) {
      os << "using namespace std;\n"
         << "using namespace tfel::math;\n"
         << "using std::vector;\n";
      writeMaterialLaws(os, this->bd.getMaterialLaws());
      os << this->bd.getCode(h, BehaviourData::APosterioriTimeStepScalingFactor)
         << '\n';
    }
    os << "return {true,this->maximal_time_step_scaling_factor};\n"
       << "}\n\n";
  }  // end of
     // writeBehaviourComputeAPosterioriTimeStepScalingFactor

  void BehaviourCodeGeneratorBase::writeBehaviourTangentOperator(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    if (!this->bd.hasTangentOperator()) {
      return;
    }
    const auto& blocks = this->bd.getTangentOperatorBlocks();
    os << "//! \\brief tangent operator;\n"
       << "TangentOperator Dt;\n";
    if (this->bd.hasTrivialTangentOperatorStructure()) {
      tfel::raise_if(
          ((blocks.size() != 1u) || (blocks.front().first.arraySize != 1u) ||
           (blocks.front().second.arraySize != 1u)),
          "BehaviourCodeGeneratorBase::writeBehaviourTangentOperator: internal "
          "error");
      if (this->bd.getBehaviourType() !=
          BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        os << "//! \\brief alias to the tangent operator;\n"
           << "TangentOperator& "
           << this->bd.getTangentOperatorBlockName(blocks.front()) << ";\n";
      }
      return;
    }
    // write blocks
    auto o = SupportedTypes::TypeSize{};
    for (const auto& b : blocks) {
      const auto& v1 = b.first;
      const auto& v2 = b.second;
      const auto bn = this->bd.getTangentOperatorBlockName(b);
      const auto block_size = SupportedTypes::TypeSize::getDerivativeSize(
          SupportedTypes::getTypeSize(v1.type),
          SupportedTypes::getTypeSize(v2.type));
      if ((v1.arraySize == 1u) && (v2.arraySize == 1u)) {
        if ((v1.getTypeFlag() == SupportedTypes::SCALAR) &&
            (v2.getTypeFlag() == SupportedTypes::SCALAR)) {
          if (this->bd.useQt()) {
            os << "typename tfel::math::MakeQuantityReferenceType<"
               << "tfel::math::derivative_type<" << v1.type << "," << v2.type
               << ">>::type " << bn << ";\n";
          } else {
            os << "tfel::math::derivative_type<" << v1.type << "," << v2.type
               << ">& " << bn << ";\n";
          }
        } else {
          os << "tfel::math::View<tfel::math::derivative_type<" << v1.type
             << "," << v2.type << ">> " << bn << ";\n";
        }
      } else if ((v1.arraySize == 1u) || (v2.arraySize == 1u)) {
        os << "/*!\n"
           << " * \\return the derivative of " << v1.name << " with respect "
           << v2.name << "\n"
           << " * \\param[in] mfront_idx: array index relative to " << v1.name
           << "\n"
           << " */\n"
           << "auto " << bn << "(const ushort mfront_idx) noexcept {\n"
           << "return tfel::math::map<tfel::math::derivative_type<" << v1.type
           << ", " << v2.type << ">>(this->Dt.data() + ";
        if (!o.isNull()) {
          os << o << " + ";
        }
        os << "mfront_idx * " << block_size << ");\n"
           << "}\n";
      } else {
        os << "/*!\n"
           << " * \\return the derivative of " << v1.name << " with respect "
           << v2.name << "\n"
           << " * \\param[in] mfront_idx: array index relative to " << v1.name
           << "\n"
           << " * \\param[in] mfront_idx2: array index relative to " << v2.name
           << "\n"
           << " */\n"
           << "decltype(auto) " << bn
           << "(const ushort mfront_idx, const ushort mfront_idx2) noexcept "
           << "{\n"
           << "return tfel::math::map<tfel::math::derivative_type<" << v1.type
           << ", " << v2.type << ">>(this->Dt.data() + ";
        if (!o.isNull()) {
          os << o << " + ";
        }
        os << "(" << v2.arraySize << " * mfront_idx + mfront_idx2) * "
           << block_size << ");\n"
           << "}\n";
      }
      o += (v1.arraySize) * (v2.arraySize) * block_size;
    }
  }  // end of writeBehaviourTangentOperator()

  void BehaviourCodeGeneratorBase::writeBehaviourBehaviourVariablesMethods(
      std::ostream& os, const Hypothesis h) const {
    const auto& md = this->bd.getBehaviourData(h);
    // initialize
    auto write_initialize = [this, &os, &md,
                             h](const BehaviourVariableDescription& b) {
      using ExternalModel =
          BehaviourDescription::ExternalModelBasedOnBehaviourVariableFactory;
      const auto is_auxiliary_model = [this, &b] {
        for (const auto& m : this->bd.getAuxiliaryModelsDescriptions()) {
          if (!std::holds_alternative<ExternalModel>(m)) {
            continue;
          }
          const auto& model = std::get<ExternalModel>(m);
          if (model.factory == getBehaviourVariableFactoryClassName(b)) {
            return true;
          }
        }
        return false;
      }();
      const auto warnings = checkInitializeMethods(
          b.behaviour, h,
          {.checkGradientsAtTheBeginningOfTheTimeStep = true,
           .checkGradientsAtTheEndOfTheTimeStep = true,
           .checkGradientsIncrements = true,
           .checkThermodynamicForcesAtTheBeginningOfTheTimeStep = true});
      if (!warnings.empty()) {
        auto msg =
            "BehaviourCodeGeneratorBase::writeBehaviourInitializeMethods: "
            "error while treating behavour variable '" +
            b.name + "'.";
        for (const auto& w : warnings) {
          msg += "\n" + w;
        }
        tfel::raise(msg);
      }
      const auto wrapper = getBehaviourWrapperClassName(b);
      os << "/*!\n"
         << " * \\ brief initialize the behaviour variable " << b.name << "\n"
         << " */\n"
         << "[[nodiscard]] TFEL_HOST_DEVICE bool "
         << "initialize(" << wrapper << "& mfront_behaviour_variable_" << b.name
         << ") const {\n"
         << "mfront_behaviour_variable_" << b.name
         << ".setOutOfBoundsPolicy(this->policy);\n";
      if (b.store_gradients) {
        for (const auto& [g, th] : b.behaviour.getMainVariables()) {
          static_cast<void>(th);
          const auto ng = applyNamesChanges(b, g);
          os << "mfront_behaviour_variable_" << b.name << ". " << g.name  //
             << " = this->" << ng.name << ";\n";
        }
      }
      if (b.store_thermodynamic_forces) {
        for (const auto& [g, th] : b.behaviour.getMainVariables()) {
          static_cast<void>(th);
          const auto nth = applyNamesChanges(b, th);
          os << "mfront_behaviour_variable_" << b.name << ". " << th.name  //
             << " = this->" << nth.name << ";\n";
        }
      }
      for (const auto& mp : getSharedMaterialProperties(b, h)) {
        const auto v = md.getMaterialProperties().getVariableByExternalName(
            mp.getExternalName());
        os << "mfront_behaviour_variable_" << b.name << ". " << mp.name
           << " = this->" << v.name << ";\n";
      }
      for (const auto& mp : getEvaluatedMaterialProperties(b, h)) {
        const auto& source = [this, &md, &b, &mp]() -> std::string {
          if (this->bd.isGradientExternalName(mp.getExternalName())) {
            return this->bd.getGradientByExternalName(mp.getExternalName()).name;
          }
          if (this->bd.isThermodynamicForceExternalName(mp.getExternalName())) {
            return this->bd.getThermodynamicForceByExternalName(
                mp.getExternalName()).name;
          }
          if (md.isStaticVariableName(mp.getExternalName())) {

            return md.getStaticVariableDescription(mp.getExternalName()).name;
          }
          try {
            return md.getVariableDescriptionByExternalName(mp.getExternalName())
                .name;
          } catch (std::exception& e) {
            tfel::raise("variable '" + mp.getExternalName() + "' ('" + mp.name +
                        "') of behaviour variable '" + b.name +
                        "' can't be evaluated");
          }
        }();
        if (!((md.isExternalStateVariableName(source)) ||
              (md.isMaterialPropertyName(source)) ||
              (md.isParameterName(source)) ||
              (md.isStaticVariableName(source)) ||
              (this->bd.isGradientName(source)) ||
              ((this->bd.isThermodynamicForceName(source)) &&
               is_auxiliary_model) ||
              ((md.isPersistentVariableName(source)) && is_auxiliary_model))) {
          tfel::raise("variable '" + mp.getExternalName() + "' ('" + mp.name +
                      "') of behaviour variable '" + b.name +
                      "' can't be evaluated with neither an external state "
                      "variable, a material property, a parameter nor a static "
                      "variable ");
        }
        if (this->bd.isGradientName(source)){
          const auto& g = this->bd.getGradient(source);
          if (Gradient::isIncrementKnown(g)) {
          os << "mfront_behaviour_variable_" << b.name << ". " << mp.name
             << " = this->" << source << " + this->d" << source << ";\n";
          } else {
            os << "mfront_behaviour_variable_" << b.name << ". " << mp.name
               << " = this->" << source << "1;\n";
          }
        } else if (md.isExternalStateVariableName(source)) {
          os << "mfront_behaviour_variable_" << b.name << ". " << mp.name
             << " = this->" << source << " + this->d" << source << ";\n";
        } else if (md.isStaticVariableName(source)) {
          os << "mfront_behaviour_variable_" << b.name << ". " << mp.name
             << " = " << bd.getClassName() << "::" << source << ";\n";
        } else {
          os << "mfront_behaviour_variable_" << b.name << ". " << mp.name
             << " = this->" << source << ";\n";
        }
      }
      for (const auto& mp : getUnSharedNorEvaluatedMaterialProperties(b, h)) {
        const auto nmp = applyNamesChanges(b, mp);
        os << "mfront_behaviour_variable_" << b.name << ". " << mp.name
           << " = this->" << nmp.name << ";\n";
      }
      for (const auto& esv : getSharedExternalStateVariables(b, h)) {
        const auto v = md.getExternalStateVariables().getVariableByExternalName(
            esv.getExternalName());
        os << "mfront_behaviour_variable_" << b.name << ". " << esv.name
           << " = this->" << v.name << ";\n";
        os << "mfront_behaviour_variable_" << b.name << ". d" << esv.name
           << " = this->d" << v.name << ";\n";
      }
      for (const auto& esv : getEvaluatedExternalStateVariables(b, h)) {
        const auto source = [this, &md, &b, &esv]() -> std::string {
          try {
            if (this->bd.isGradientExternalName(esv.getExternalName())) {
              return this->bd.getGradientByExternalName(esv.getExternalName())
                  .name;
            }
            if (this->bd.isThermodynamicForceExternalName(
                    esv.getExternalName())) {
              return this->bd
                  .getThermodynamicForceByExternalName(esv.getExternalName())
                  .name;
            }
            if (md.isStaticVariableName(esv.getExternalName())) {
              return md.getStaticVariableDescription(esv.getExternalName()).name;
            }
            return md
                .getVariableDescriptionByExternalName(esv.getExternalName())
                .name;
          } catch (std::exception& e) {
            tfel::raise("variable '" + esv.getExternalName() + "' ('" +
                        esv.name + "') of behaviour variable '" + b.name +
                        "' can't be evaluated");
          }
        }();
        if (!((md.isExternalStateVariableName(source)) ||
              (md.isMaterialPropertyName(source)) ||
              (md.isParameterName(source)) ||
              (md.isStaticVariableName(source)) ||
              (this->bd.isGradientName(source)) ||
              ((this->bd.isThermodynamicForceName(source)) &&
               is_auxiliary_model) ||
              ((md.isPersistentVariableName(source)) &&
               is_auxiliary_model))) {
          tfel::raise("variable '" + esv.getExternalName() + "' ('" + esv.name +
                      "') of behaviour variable '" + b.name +
                      "' can't be evaluated with neither an external state "
                      "variable, a material property, a parameter, a gradient, "
                      "a thermodynamic_force, nor a static "
                      "variable ");
        }
        if (this->bd.isGradientName(source)){
          const auto& g = this->bd.getGradient(source);
          if (Gradient::isIncrementKnown(g)) {
            os << "mfront_behaviour_variable_" << b.name << ". " << esv.name
               << " = this->" << source << ";\n";
            os << "mfront_behaviour_variable_" << b.name << ". d" << esv.name
               << " = this->d" << source << ";\n";
          } else {
            os << "mfront_behaviour_variable_" << b.name << ". " << esv.name
               << " = this->" << source << "0;\n";
            os << "mfront_behaviour_variable_" << b.name << ". d" << esv.name
               << " = this->" << source << "1 - this->" << source << "0;\n";
          }
        } else if (md.isExternalStateVariableName(source)) {
          os << "mfront_behaviour_variable_" << b.name << ". " << esv.name
             << " = this->" << source << ";\n";
          os << "mfront_behaviour_variable_" << b.name << ". d" << esv.name
             << " = this->d" << source << ";\n";
        } else if (md.isStaticVariableName(source)) {
          os << "mfront_behaviour_variable_" << b.name << ". " << esv.name
             << " = " << bd.getClassName() << "::" << source << ";\n";
          os << "mfront_behaviour_variable_" << b.name << ". d" << esv.name
             << " = " << esv.type << "{};\n";
        } else {
          if ((this->bd.isThermodynamicForceName(source)) ||
              (md.isPersistentVariableName(source))) {
            os << "mfront_behaviour_variable_" << b.name << ". " << esv.name
               << " = this->mfront_initial_values. " << source << ";\n";
            os << "mfront_behaviour_variable_" << b.name << ". d" << esv.name
               << " = this->" << source << "- this->mfront_initial_values. "
               << source << ";\n";
          } else {
            os << "mfront_behaviour_variable_" << b.name << ". " << esv.name
               << " = this->" << source << ";\n";
            os << "mfront_behaviour_variable_" << b.name << ". d" << esv.name
               << " = " << esv.type << "{};\n";
          }
        }
      }
      for (const auto& esv :
           getUnSharedNorEvaluatedExternalStateVariables(b, h)) {
        const auto nesv = applyNamesChanges(b, esv);
        os << "mfront_behaviour_variable_" << b.name << ". " << esv.name
           << " = this->" << nesv.name << ";\n";
        os << "mfront_behaviour_variable_" << b.name << ". d" << esv.name
           << " = this->d" << nesv.name << ";\n";
      }
      for (const auto& isv :
           b.behaviour.getBehaviourData(h).getPersistentVariables()) {
        const auto nisv = applyNamesChanges(b, isv);
        os << "mfront_behaviour_variable_" << b.name << ". " << isv.name
           << " = this->" << nisv.name << ";\n";
      }
      os << "mfront_behaviour_variable_" << b.name << ". dt = this->dt;\n";
      os << "return mfront_behaviour_variable_" << b.name << ".initialize();\n"
         << "}\n\n";
    };
    for (const auto& b : md.getBehaviourVariables()) {
      write_initialize(b);
    }
    for (const auto& b : md.getBehaviourVariableFactories()) {
      write_initialize(b);
    }
    // reset
    auto write_reset = [this, &os, h](const BehaviourVariableDescription& b) {
      const auto wrapper = getBehaviourWrapperClassName(b);
      os << "//! \\brief reset the behaviour variable\n"
         << "[[nodiscard]] TFEL_HOST_DEVICE bool reset(" << wrapper
         << "& mfront_behaviour_variable_" << b.name << "){\n"
         << "if(!this->initialize(mfront_behaviour_variable_" << b.name
         << ")){\n"
         << "return false;\n"
         << "}\n";
      for (const auto& v :
           b.behaviour.getBehaviourData(h).getIntegrationVariables()) {
        const auto flag = SupportedTypes::getTypeFlag(v.type);
        const auto& n = v.name;
        const auto t = this->usesStateVariableTimeDerivative()
                           ? SupportedTypes::getTimeDerivativeType(v.type)
                           : v.type;
        if (flag == SupportedTypes::SCALAR) {
          if (v.arraySize == 1u) {
            os << "mfront_behaviour_variable_" << b.name << ". d" << n  //
               << " = " << t << "(0);\n";
          } else {
            if (this->bd.useDynamicallyAllocatedVector(v.arraySize)) {
              os << "std::fill(mfront_behaviour_variable_" << b.name << ". d"
                 << n << ".begin(), mfront_behaviour_variable_" << b.name
                 << ". d" << n << ".end(), " << t << "(0));\n";
            } else {
              os << "tfel::fsalgo::fill<" << v.arraySize
                 << ">(mfront_behaviour_variable_" << b.name << ". d" << n
                 << ".begin(), " << t << "(0));\n";
            }
          }
        } else {
          const auto traits = "MathObjectTraits<" + t + ">";
          if (v.arraySize == 1u) {
            os << "mfront_behaviour_variable_" << b.name << ". d" << n << " = "
               << t << "(typename tfel::math::" + traits + "::NumType(0));\n";
          } else {
            if (this->bd.useDynamicallyAllocatedVector(v.arraySize)) {
              os << "std::fill(mfront_behaviour_variable_" << b.name << ". d"
                 << n << ".begin(), mfront_behaviour_variable_" << b.name
                 << ". d" << n << ".end(), " << t
                 << "(typename tfel::math::" << traits << "::NumType(0)));\n";
            } else {
              os << "tfel::fsalgo::fill<" << v.arraySize
                 << ">(mfront_behaviour_variable_" << b.name << ". d" << n
                 << ".begin(), " << t << "(typename tfel::math::" << traits
                 << "::NumType(0)));\n";
            }
          }
        }
      }
      os << "return true;\n"
            "}\n\n";
    };
    for (const auto& b : md.getBehaviourVariables()) {
      write_reset(b);
    }
    for (const auto& b : md.getBehaviourVariableFactories()) {
      write_reset(b);
    }
    // updateAuxiliaryStateVariables
    auto write_update_auxiliary_state_variables =
        [&os, h](const BehaviourVariableDescription& b) {
          const auto wrapper = getBehaviourWrapperClassName(b);
          os << "/*!\n"
             << "* \\brief update auxiliary state variables at end of "
                "integration\n"
             << "*/\n"
             << "TFEL_HOST_DEVICE void "
             << "updateAuxiliaryStateVariables(" << wrapper
             << "& mfront_behaviour_variable_" << b.name << "){\n";
          if (b.store_gradients) {
            for (const auto& [g, th] : b.behaviour.getMainVariables()) {
              static_cast<void>(th);
              const auto ng = applyNamesChanges(b, g);
              os << "this->" << ng.name << " = "
                 << "mfront_behaviour_variable_" << b.name << ". " << g.name
                 << " + "
                 << "mfront_behaviour_variable_" << b.name << ". d" << g.name
                 << ";\n";
            }
          }
          if (b.store_thermodynamic_forces) {
            for (const auto& [g, th] : b.behaviour.getMainVariables()) {
              static_cast<void>(th);
              const auto nth = applyNamesChanges(b, th);
              os << "this->" << nth.name << " = "
                 << "mfront_behaviour_variable_" << b.name << ". " << th.name
                 << ";\n";
            }
          }
          for (const auto& isv :
               b.behaviour.getBehaviourData(h).getPersistentVariables()) {
            const auto nisv = applyNamesChanges(b, isv);
            os << "this->" << nisv.name << " = "
               << "mfront_behaviour_variable_" << b.name << ". " << isv.name
               << ";\n";
          }
          os << "}\n";
        };
    for (const auto& b : md.getBehaviourVariables()) {
      write_update_auxiliary_state_variables(b);
    }
    for (const auto& b : md.getBehaviourVariableFactories()) {
      write_update_auxiliary_state_variables(b);
    }
  }  // end of writeBehaviourBehaviourVariablesMethods

  void BehaviourCodeGeneratorBase::checkIntegrationDataFile(
      std::ostream& os) const {
    if ((!os) || (!os.good())) {
      this->throwRuntimeError(
          "BehaviourCodeGeneratorBase::checkIntegrationDataOutputFile",
          "output file is not valid");
    }
  }

  void BehaviourCodeGeneratorBase::writeIntegrationDataFileHeader(
      std::ostream& os) const {
    this->checkIntegrationDataFile(os);
    os << "/*!\n";
    os << "* \\file   " << this->bd.getIntegrationDataFileName() << '\n';
    os << "* \\brief  "
       << "this file implements the " << this->bd.getClassName()
       << "IntegrationData"
       << " class.\n";
    os << "*         File generated by ";
    os << MFrontHeader::getVersionName() << " ";
    os << "version " << MFrontHeader::getVersionNumber();
    os << '\n';
    if (!this->fd.authorName.empty()) {
      os << "* \\author " << this->fd.authorName << '\n';
    }
    if (!this->fd.date.empty()) {
      os << "* \\date   " << this->fd.date << '\n';
    }
    os << " */\n\n";
  }

  void BehaviourCodeGeneratorBase::writeIntegrationDataFileHeaderBegin(
      std::ostream& os) const {
    this->checkIntegrationDataFile(os);
    const auto cn = this->bd.getClassName() + "_integration_data";
    this->writeHeaderGuardBegin(os, cn, true);
  }

  void BehaviourCodeGeneratorBase::writeIntegrationDataFileHeaderEnd(
      std::ostream& os) const {
    this->checkIntegrationDataFile(os);
    const auto cn = this->bd.getClassName() + "_integration_data";
    this->writeHeaderGuardEnd(os, cn, true);
  }

  void BehaviourCodeGeneratorBase::writeIntegrationDataStandardTFELIncludes(
      std::ostream& os) const {
    bool b1 = false;
    bool b2 = false;
    this->checkIntegrationDataFile(os);
    os << "#include<string>\n"
       << "#include<iostream>\n"
       << "#include<limits>\n"
       << "#include<stdexcept>\n"
       << "#include<algorithm>\n\n"
       << "#include\"TFEL/Raise.hxx\"\n"
       << "#include\"TFEL/PhysicalConstants.hxx\"\n"
       << "#include\"TFEL/Config/TFELConfig.hxx\"\n"
       << "#include\"TFEL/Config/TFELTypes.hxx\"\n"
       << "#include\"TFEL/TypeTraits/IsFundamentalNumericType.hxx\"\n"
       << "#include\"TFEL/TypeTraits/IsScalar.hxx\"\n"
       << "#include\"TFEL/TypeTraits/IsReal.hxx\"\n"
       << "#include\"TFEL/TypeTraits/Promote.hxx\"\n"
       << "#include\"TFEL/Math/General/IEEE754.hxx\"\n";
    this->bd.requiresTVectorOrVectorIncludes(b1, b2);
    if (b1) {
      os << "#include\"TFEL/Math/tvector.hxx\"\n"
         << "#include\"TFEL/Math/Vector/tvectorIO.hxx\"\n";
    }
    if (b2) {
      os << "#include\"TFEL/Math/vector.hxx\"\n";
    }
    os << "#include\"TFEL/Math/stensor.hxx\"\n"
       << "#include\"TFEL/Math/st2tost2.hxx\"\n";
    if (this->bd.getBehaviourType() ==
        BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      os << "#include\"TFEL/Math/tensor.hxx\"\n"
         << "#include\"TFEL/Math/t2tot2.hxx\"\n"
         << "#include\"TFEL/Math/t2tost2.hxx\"\n"
         << "#include\"TFEL/Math/st2tot2.hxx\"\n";
    }
  }

  void BehaviourCodeGeneratorBase::writeIntegrationDataDefaultMembers(
      std::ostream& os) const {
    this->checkIntegrationDataFile(os);
    os << "protected: \n\n";
    for (const auto& v : this->bd.getMainVariables()) {
      if (v.first.arraySize != v.second.arraySize) {
        tfel::raise(
            "BehaviourCodeGeneratorBase::writeIntegrationDataDefaultMembers: "
            "the array size of the gradient '" +
            v.first.name +
            "' is "
            "different from the array size of the thermodynamic force '" +
            v.second.name + "'");
      }
      if (Gradient::isIncrementKnown(v.first)) {
        os << "/*!\n"
           << " * \\brief " << v.first.name << " increment\n"
           << " */\n";
        if (v.first.arraySize == 1u) {
          os << v.first.type << " d" << v.first.name << ";\n\n";
        } else {
          os << v.first.type << " d" << v.first.name << "[" << v.first.arraySize
             << "];\n\n";
        }
      } else {
        os << "/*!\n"
           << " * \\brief " << v.first.name << " at the end of the time step\n"
           << " */\n";
        if (v.first.arraySize == 1u) {
          os << v.first.type << " " << v.first.name << "1;\n\n";
        } else {
          os << v.first.type << " " << v.first.name << "1[" << v.first.arraySize
             << "];\n\n";
        }
      }
    }
    os << "/*!\n"
       << " * \\brief time increment\n"
       << " */\n"
       << "time dt;\n\n";
  }

  void BehaviourCodeGeneratorBase::writeIntegrationDataTypeAliases(
      std::ostream& os) const {
    this->checkIntegrationDataFile(os);
    os << "static constexpr unsigned short TVectorSize = N;\n"
       << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;\n"
       << "static constexpr unsigned short StensorSize = "
       << "StensorDimeToSize::value;\n"
       << "typedef tfel::math::TensorDimeToSize<N> TensorDimeToSize;\n"
       << "static constexpr unsigned short TensorSize = "
       << "TensorDimeToSize::value;\n\n";
    this->writeTypeAliases(os);
    os << '\n';
  }

  void BehaviourCodeGeneratorBase::writeIntegrationDataDisabledConstructors(
      std::ostream& os) const {
    this->checkIntegrationDataFile(os);
  }

  void BehaviourCodeGeneratorBase::writeIntegrationDataConstructors(
      std::ostream& os, const Hypothesis h) const {
    const auto& md = this->bd.getBehaviourData(h);
    this->checkIntegrationDataFile(os);
    os << "/*!\n"
       << "* \\brief Default constructor\n"
       << "*/\n"
       << this->bd.getClassName() << "IntegrationData()\n"
       << "{}\n\n"
       << "/*!\n"
       << "* \\brief Copy constructor\n"
       << "*/\n"
       << this->bd.getClassName() << "IntegrationData(const "
       << this->bd.getClassName() << "IntegrationData& src)\n"
       << ": ";
    for (const auto& v : this->bd.getMainVariables()) {
      if (Gradient::isIncrementKnown(v.first)) {
        os << "d" << v.first.name << "(src.d" << v.first.name << "),\n";
      } else {
        os << v.first.name << "1(src." << v.first.name << "1),\n";
      }
    }
    os << "dt(src.dt)";
    for (const auto& v : md.getExternalStateVariables()) {
      os << ",\nd" << v.name << "(src.d" << v.name << ")";
    }
    os << "\n{}\n\n";
    // Creating constructor for external interfaces
    for (const auto& i : this->interfaces) {
      if (i.second->isBehaviourConstructorRequired(h, this->bd)) {
        i.second->writeIntegrationDataConstructor(os, h, this->bd);
      }
    }
  }

  void BehaviourCodeGeneratorBase::writeIntegrationDataScaleOperators(
      std::ostream& os, const Hypothesis h) const {
    const auto& md = this->bd.getBehaviourData(h);
    bool iknown = true;
    for (const auto& v : this->bd.getMainVariables()) {
      iknown = Gradient::isIncrementKnown(v.first);
    }
    this->checkIntegrationDataFile(os);
    os << "/*\n"
       << "* \\brief scale the integration data by a scalar.\n"
       << "*/\n"
       << "template<typename Scal>\n"
       << "TFEL_HOST_DEVICE " << this->bd.getClassName()
       << "IntegrationData&\n";
    if (!iknown) {
      if (this->bd.useQt()) {
        os << "scale(const " << this->bd.getClassName()
           << "BehaviourData<hypothesis, NumericType, use_qt>& behaviourData, "
              "const "
              "Scal time_scaling_factor)\n";
      } else {
        os << "scale(const " << this->bd.getClassName()
           << "BehaviourData<hypothesis, NumericType, false>& behaviourData, "
              "const Scal "
              "time_scaling_factor)\n";
      }
    } else {
      if (this->bd.useQt()) {
        os << "scale(const " << this->bd.getClassName()
           << "BehaviourData<hypothesis, NumericType, use_qt>&, const Scal "
              "time_scaling_factor)\n";
      } else {
        os << "scale(const " << this->bd.getClassName()
           << "BehaviourData<hypothesis, NumericType, false>&, const Scal "
              "time_scaling_factor)\n";
      }
    }
    os << "requires(tfel::typetraits::IsFundamentalNumericType<Scal>::cond&&\n"
       << "tfel::typetraits::IsScalar<Scal>::cond&&\n"
       << "tfel::typetraits::IsReal<Scal>::cond&&\n"
       << "std::is_same<NumericType,"
       << "typename tfel::typetraits::Promote"
       << "<NumericType,Scal>::type>::value){\n"
       << "this->dt   *= time_scaling_factor;\n";
    for (const auto& v : this->bd.getMainVariables()) {
      if (Gradient::isIncrementKnown(v.first)) {
        os << "this->d" << v.first.name << " *= time_scaling_factor;\n";
      } else {
        os << "this->" << v.first.name
           << "1 = (1-time_scaling_factor)*(behaviourData." << v.first.name
           << "0)+time_scaling_factor*(this->" << v.first.name << "1);\n";
      }
    }
    for (const auto& v : md.getExternalStateVariables()) {
      os << "this->d" << v.name << " *= time_scaling_factor;\n";
    }
    os << "return *this;\n"
       << "}\n\n";
  }  // end of writeIntegrationDataScaleOpeartors

  void
  BehaviourCodeGeneratorBase::writeIntegrationDataUpdateDrivingVariablesMethod(
      std::ostream& os) const {
    bool iknown = true;
    for (const auto& v : this->bd.getMainVariables()) {
      iknown = Gradient::isIncrementKnown(v.first);
    }
    this->checkIntegrationDataFile(os);
    os << "/*!\n"
       << "* \\brief update the driving variable in case of substepping.\n"
       << "*/\n"
       << "TFEL_HOST_DEVICE " << this->bd.getClassName()
       << "IntegrationData&\n";
    if (!iknown) {
      if (this->bd.useQt()) {
        os << "updateDrivingVariables(const " << this->bd.getClassName()
           << "BehaviourData<hypothesis, NumericType, use_qt>& "
              "behaviourData){\n";
      } else {
        os << "updateDrivingVariables(const " << this->bd.getClassName()
           << "BehaviourData<hypothesis, NumericType, false>& "
              "behaviourData){\n";
      }
    } else {
      if (this->bd.useQt()) {
        os << "updateDrivingVariables(const " << this->bd.getClassName()
           << "BehaviourData<hypothesis, NumericType, use_qt>&){\n";
      } else {
        os << "updateDrivingVariables(const " << this->bd.getClassName()
           << "BehaviourData<hypothesis, NumericType, false>&){\n";
      }
    }
    for (const auto& v : this->bd.getMainVariables()) {
      if (!Gradient::isIncrementKnown(v.first)) {
        os << "this->" << v.first.name << "1 += "
           << "this->" << v.first.name << "1 - (behaviourData." << v.first.name
           << "0);\n";
      }
    }
    os << "return *this;\n"
       << "}\n\n";
  }  // end of writeIntegrationUpdateDrivingVariablesMethod

  void BehaviourCodeGeneratorBase::writeIntegrationDataClassHeader(
      std::ostream& os) const {
    this->checkIntegrationDataFile(os);
    os << "/*!\n"
       << "* \\class " << this->bd.getClassName() << "IntegrationData\n"
       << "* \\brief This class implements the " << this->bd.getClassName()
       << "IntegrationData"
       << " behaviour.\n"
       << "* \\tparam N: space dimension.\n"
       << "* \\tparam NumericType: numerical type.\n"
       << "* \\tparam use_qt: conditional saying if quantities are use.\n";
    if (!this->fd.authorName.empty()) {
      os << "* \\author " << this->fd.authorName << '\n';
    }
    if (!this->fd.date.empty()) {
      os << "* \\date   " << this->fd.date << '\n';
    }
    os << "*/\n";
  }

  void BehaviourCodeGeneratorBase::writeIntegrationDataForwardDeclarations(
      std::ostream& os) const {
    this->checkIntegrationDataFile(os);
    os << "//! \\brief forward declaration\n"
       << "template<ModellingHypothesis::Hypothesis hypothesis, "
       << "typename NumericType, bool use_qt>\n"
       << "class " << this->bd.getClassName() << "IntegrationData;\n\n";
    if (this->bd.useQt()) {
      os << "//! \\brief forward declaration\n"
         << "template<ModellingHypothesis::Hypothesis hypothesis, "
         << "typename NumericType, bool use_qt>\n"
         << "std::ostream&\n operator <<(std::ostream&,"
         << "const " << this->bd.getClassName()
         << "IntegrationData<hypothesis, NumericType, use_qt>&);\n\n";
    } else {
      os << "//! \\brief forward declaration\n"
         << "template<ModellingHypothesis::Hypothesis hypothesis, "
         << "typename NumericType>\n"
         << "std::ostream&\n operator <<(std::ostream&,"
         << "const " << this->bd.getClassName()
         << "IntegrationData<hypothesis, NumericType, false>&);\n\n";
    }
    // maintenant, il faut déclarer toutes les spécialisations partielles...
    const auto& mh = this->bd.getModellingHypotheses();
    for (const auto& h : mh) {
      if (this->bd.hasSpecialisedMechanicalData(h)) {
        if (this->bd.useQt()) {
          os << "//! \\brief forward declaration\n"
             << "template<typename NumericType,bool use_qt>\n"
             << "std::ostream&\n operator <<(std::ostream&,"
             << "const " << this->bd.getClassName()
             << "IntegrationData<ModellingHypothesis::"
             << ModellingHypothesis::toUpperCaseString(h)
             << ", NumericType, use_qt>&);\n\n";
        } else {
          os << "//! \\brief forward declaration\n"
             << "template<typename NumericType>\n"
             << "std::ostream&\n operator <<(std::ostream&,"
             << "const " << this->bd.getClassName()
             << "IntegrationData<ModellingHypothesis::"
             << ModellingHypothesis::toUpperCaseString(h)
             << ", NumericType, false>&);\n\n";
        }
      }
    }
  }

  void BehaviourCodeGeneratorBase::writeIntegrationDataClassBegin(
      std::ostream& os, const Hypothesis h) const {
    this->checkIntegrationDataFile(os);
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      if (this->bd.useQt()) {
        os << "template<ModellingHypothesis::Hypothesis hypothesis, "
           << "typename NumericType, bool use_qt>\n";
        os << "class " << this->bd.getClassName() << "IntegrationData\n";
      } else {
        os << "template<ModellingHypothesis::Hypothesis hypothesis, "
           << "typename NumericType>\n";
        os << "class " << this->bd.getClassName()
           << "IntegrationData<hypothesis, NumericType, false>\n";
      }
    } else {
      if (this->bd.useQt()) {
        os << "template<typename NumericType,bool use_qt>\n";
        os << "class " << this->bd.getClassName()
           << "IntegrationData<ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h)
           << ", NumericType, use_qt>\n";
      } else {
        os << "template<typename NumericType>\n";
        os << "class " << this->bd.getClassName()
           << "IntegrationData<ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h)
           << ", NumericType, false>\n";
      }
    }
    os << "{\n\n";
    if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      os << "static constexpr ModellingHypothesis::Hypothesis hypothesis = "
         << "ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h)
         << ";\n";
    }
    os << "static constexpr unsigned short N = "
          "ModellingHypothesisToSpaceDimension<hypothesis>::value;\n";
    os << "static_assert(N==1||N==2||N==3);\n";
    os << "static_assert(tfel::typetraits::"
       << "IsFundamentalNumericType<NumericType>::cond);\n";
    os << "static_assert(tfel::typetraits::IsReal<NumericType>::cond);\n\n";
    os << "friend std::ostream& operator<< <>(std::ostream&,const ";
    os << this->bd.getClassName() << "IntegrationData&);\n\n";
  }

  void BehaviourCodeGeneratorBase::writeIntegrationDataOutputOperator(
      std::ostream& os, const Hypothesis h) const {
    const auto& md = this->bd.getBehaviourData(h);
    this->checkBehaviourFile(os);
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      if (this->bd.useQt()) {
        os << "template<ModellingHypothesis::Hypothesis hypothesis, "
           << "typename NumericType,bool use_qt>\n"
           << "std::ostream&\n"
           << "operator <<(std::ostream& os,"
           << "const " << this->bd.getClassName()
           << "IntegrationData<hypothesis, NumericType, use_qt>& b)\n";
      } else {
        os << "template<ModellingHypothesis::Hypothesis hypothesis, "
           << "typename NumericType>\n"
           << "std::ostream&\n"
           << "operator <<(std::ostream& os,"
           << "const " << this->bd.getClassName()
           << "IntegrationData<hypothesis, NumericType, false>& b)\n";
      }
    } else {
      if (this->bd.useQt()) {
        os << "template<typename NumericType,bool use_qt>\n"
           << "std::ostream&\n"
           << "operator <<(std::ostream& os,"
           << "const " << this->bd.getClassName()
           << "IntegrationData<ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h)
           << ", NumericType, use_qt>& b)\n";
      } else {
        os << "template<typename NumericType>\n"
           << "std::ostream&\n"
           << "operator <<(std::ostream& os,"
           << "const " << this->bd.getClassName()
           << "IntegrationData<ModellingHypothesis::"
           << ModellingHypothesis::toUpperCaseString(h)
           << ", NumericType, false>& b)\n";
      }
    }
    os << "{\n";
    for (const auto& v : this->bd.getMainVariables()) {
      if (Gradient::isIncrementKnown(v.first)) {
        if (getUnicodeOutputOption()) {
          os << "os << \"\u0394" << displayName(v.first) << " : \" << b.d"
             << v.first.name << " << '\\n';\n";
        } else {
          os << "os << \"d" << displayName(v.first) << " : \" << b.d"
             << v.first.name << " << '\\n';\n";
        }
      } else {
        if (getUnicodeOutputOption()) {
          os << "os << \"" << displayName(v.first) << "\u2081 : \" << b."
             << v.first.name << "1 << '\\n';\n";
        } else {
          os << "os << \"" << displayName(v.first) << "1 : \" << b."
             << v.first.name << "1 << '\\n';\n";
        }
      }
      os << "os << \"" << displayName(v.second) << " : \" << b."
         << v.second.name << " << '\\n';\n";
    }
    if (getUnicodeOutputOption()) {
      os << "os << \"\u0394t : \" << b.dt << '\\n';\n";
    } else {
      os << "os << \"dt : \" << b.dt << '\\n';\n";
    }
    for (const auto& ev : md.getExternalStateVariables()) {
      if (getUnicodeOutputOption()) {
        os << "os << \"\u0394" << displayName(ev) << " : \" << b.d" << ev.name
           << " << '\\n';\n";
      } else {
        os << "os << \"d" << displayName(ev) << " : \" << b.d" << ev.name
           << " << '\\n';\n";
      }
    }
    os << "return os;\n";
    os << "}\n\n";
  }  // end of writeIntegrationDataOutputOperator

  void BehaviourCodeGeneratorBase::writeIntegrationDataClassEnd(
      std::ostream& os) const {
    this->checkIntegrationDataFile(os);
    os << "}; // end of " << this->bd.getClassName() << "IntegrationData"
       << "class\n\n";
  }

  void BehaviourCodeGeneratorBase::writeIntegrationDataExternalStateVariables(
      std::ostream& os, const Hypothesis h) const {
    const auto& md = this->bd.getBehaviourData(h);
    this->checkIntegrationDataFile(os);
    this->writeVariablesDeclarations(os, md.getExternalStateVariables(), "d",
                                     "", this->fd.fileName, false);
  }

  void BehaviourCodeGeneratorBase::writeIntegrationDataFileBegin(
      std::ostream& os) const {
    this->checkIntegrationDataFile(os);
    this->writeIntegrationDataFileHeader(os);
    this->writeIntegrationDataFileHeaderBegin(os);
    this->writeIntegrationDataStandardTFELIncludes(os);
    this->writeIncludes(os);
    // includes specific to interfaces
    for (const auto& i : this->interfaces) {
      i.second->writeInterfaceSpecificIncludes(os, this->bd);
    }
    this->writeNamespaceBegin(os);
    this->writeIntegrationDataForwardDeclarations(os);
  }  // end of writeIntegrationDataFile

  void BehaviourCodeGeneratorBase::writeIntegrationDataClass(
      std::ostream& os, const Hypothesis h) const {
    this->checkIntegrationDataFile(os);
    this->writeIntegrationDataClassBegin(os, h);
    this->writeIntegrationDataTypeAliases(os);
    this->writeIntegrationDataDefaultMembers(os);
    this->writeIntegrationDataExternalStateVariables(os, h);
    this->writeIntegrationDataDisabledConstructors(os);
    os << "public:\n\n";
    this->writeIntegrationDataConstructors(os, h);
    this->writeIntegrationDataMainVariablesSetters(os);
    this->writeIntegrationDataScaleOperators(os, h);
    this->writeIntegrationDataUpdateDrivingVariablesMethod(os);
    this->writeIntegrationDataClassEnd(os);
    this->writeIntegrationDataOutputOperator(os, h);
  }

  void BehaviourCodeGeneratorBase::writeIntegrationDataFileEnd(
      std::ostream& os) const {
    this->checkIntegrationDataFile(os);
    this->writeNamespaceEnd(os);
    this->writeIntegrationDataFileHeaderEnd(os);
  }  // end of writeIntegrationDataFileEnd

  void BehaviourCodeGeneratorBase::checkSrcFile(std::ostream& os) const {
    if ((!os) || (!os.good())) {
      this->throwRuntimeError("BehaviourCodeGeneratorBase::checkSrcFile",
                              "output file is not valid");
    }
  }

  void BehaviourCodeGeneratorBase::writeSrcFileHeader(std::ostream& os) const {
    this->checkSrcFile(os);
    os << "/*!\n"
       << "* \\file   " << this->bd.getSrcFileName() << '\n'
       << "* \\brief  "
       << "this file implements the " << this->bd.getClassName()
       << " Behaviour.\n"
       << "*         File generated by " << MFrontHeader::getVersionName()
       << " "
       << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!this->fd.authorName.empty()) {
      os << "* \\author " << this->fd.authorName << '\n';
    }
    if (!this->fd.date.empty()) {
      os << "* \\date   " << this->fd.date << '\n';
    }
    os << " */\n\n";
    if (this->bd.hasParameters()) {
      os << "#include<string>\n"
         << "#include<locale>\n"
         << "#include<cstring>\n"
         << "#include<sstream>\n"
         << "#include<fstream>\n"
         << "#include<stdexcept>\n\n";
    }
    os << "#include\"TFEL/Raise.hxx\"\n"
       << "#include\"" << this->bd.getBehaviourDataFileName() << "\"\n"
       << "#include\"" << this->bd.getIntegrationDataFileName() << "\"\n"
       << "#include\"" << this->bd.getBehaviourFileName() << "\"\n\n";
  }  // end of writeSrcFileHeader()

  void BehaviourCodeGeneratorBase::writeSrcFileUserDefinedCode(
      std::ostream& os) const {
    this->checkSrcFile(os);
    const auto& s = this->bd.getSources();
    if (!s.empty()) {
      os << s << "\n\n";
    }
  }  // end of writeSrcFileUserDefinedCode

  void BehaviourCodeGeneratorBase::writeSrcFileParametersInitializers(
      std::ostream& os) const {
    // useless and paranoid test
    if ((areParametersTreatedAsStaticVariables(this->bd)) ||
        (!this->bd.hasParameters())) {
      return;
    }
    auto hs = this->bd.getDistinctModellingHypotheses();
    hs.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    for (const auto& h : hs) {
      if (this->bd.hasParameters(h)) {
        this->writeSrcFileParametersInitializer(os, h);
      }
    }
  }  // end of writeSrcFileParametersInitializer

  static void BehaviourCodeGeneratorBase_writeConverter(
      std::ostream& f,
      const std::string& cname,
      const std::string& type,
      const std::string& type2) {
    f << type << '\n'
      << cname << "::get" << type2 << "(const std::string& n,\n"
      << "const std::string& v)\n"
      << "{\n"
      << type << " value;\n"
      << "std::istringstream converter(v);\n"
      << "converter.imbue(std::locale::classic());\n"
      << "converter >> value;\n"
      << "tfel::raise_if(!converter||(!converter.eof()),\n"
      << "\"" << cname << "::get" << type2 << ": \"\n"
      << "\"can't convert '\"+v+\"' to " << type << " "
      << "for parameter '\"+ n+\"'\");\n"
      << "return value;\n"
      << "}\n\n";
  }

  void BehaviourCodeGeneratorBase::writeSrcFileParametersInitializer(
      std::ostream& os, const Hypothesis h) const {
    // useless and paranoid test
    if ((areParametersTreatedAsStaticVariables(this->bd)) ||
        (!this->bd.hasParameters())) {
      return;
    }
    this->checkBehaviourFile(os);
    // treating the default case
    bool rp = false;   // real    parameter found
    bool ip = false;   // integer parameter found
    bool up = false;   // unsigned short parameter found
    bool rp2 = false;  // real    parameter found
    bool ip2 = false;  // integer parameter found
    bool up2 = false;  // unsigned short parameter found
    std::string cname(this->bd.getClassName());
    if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      cname += ModellingHypothesis::toString(h);
    }
    cname += "ParametersInitializer";
    std::string dcname(this->bd.getClassName() + "ParametersInitializer");
    os << cname << "&\n"
       << cname << "::get()\n"
       << "{\n"
       << "static " << cname << " i;\n"
       << "return i;\n"
       << "}\n\n";
    os << cname << "::" << cname << "()\n"
       << "{\n";
    for (const auto& p : this->bd.getBehaviourData(h).getParameters()) {
      if (p.type == "int") {
        ip = true;
        if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
            ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
             (!this->bd.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                     p.name)))) {
          ip2 = true;
          os << "this->" << p.name << " = "
             << this->bd.getIntegerParameterDefaultValue(h, p.name) << ";\n";
        }
      } else if (p.type == "ushort") {
        up = true;
        if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
            ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
             (!this->bd.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                     p.name)))) {
          up2 = true;
          os << "this->" << p.name << " = "
             << this->bd.getUnsignedShortParameterDefaultValue(h, p.name)
             << ";\n";
        }
      } else {
        const auto f = SupportedTypes::getTypeFlag(p.type);
        if (f != SupportedTypes::SCALAR) {
          this->throwRuntimeError(
              "BehaviourCodeGeneratorBase::writeSrcFileParametersInitializer",
              "unsupported parameter type '" + p.type +
                  "' "
                  "for parameter '" +
                  p.name + "'");
        }
        rp = true;
        if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
            ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
             (!this->bd.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                     p.name)))) {
          rp2 = true;
          if (p.arraySize == 1u) {
            os << "this->" << p.name << " = "
               << this->bd.getFloattingPointParameterDefaultValue(h, p.name)
               << ";\n";
          } else {
            for (unsigned short i = 0; i != p.arraySize; ++i) {
              os << "this->" << p.name << "[" << i << "] = "
                 << this->bd.getFloattingPointParameterDefaultValue(h, p.name,
                                                                    i)
                 << ";\n";
            }
          }
        }
      }
    }
    if (allowsParametersInitializationFromFile(this->bd)) {
      os << "// Reading parameters from a file\n";
      os << cname << "::readParameters(*this,\""
         << getParametersFileName(this->bd) << "\");\n";
      if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        os << cname << "::readParameters(*this,\""
           << getParametersFileName(this->bd, h) << "\");\n";
      }
    }
    os << "}\n\n";
    auto write_if = [&os](bool& b) {
      if (b) {
        os << "if(";
        b = false;
      } else {
        os << "} else if(";
      }
    };
    if (rp) {
      os << "void\n"
         << cname << "::set(const char* const key,\nconst double v)"
         << "{\n"
         << "using namespace std;\n";
      bool first = true;
      for (const auto& p : this->bd.getBehaviourData(h).getParameters()) {
        if ((p.type == "int") || (p.type == "ushort")) {
          continue;
        }
        const auto f = SupportedTypes::getTypeFlag(p.type);
        if (f != SupportedTypes::SCALAR) {
          this->throwRuntimeError(
              "BehaviourCodeGeneratorBase::writeSrcFileParametersInitializer",
              "unsupported parameter type '" + p.type +
                  "' "
                  "for parameter '" +
                  p.name + "'");
        }
        if (p.arraySize == 1u) {
          write_if(first);
          os << "::strcmp(\"" + this->bd.getExternalName(h, p.name) +
                    "\",key)==0){\n";
          if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
              ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
               (!this->bd.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                       p.name)))) {
            os << "this->" << p.name << " = v;\n";
          } else {
            os << dcname << "::get().set(\""
               << this->bd.getExternalName(h, p.name) << "\",v);\n";
          }
        } else {
          for (unsigned short i = 0; i != p.arraySize; ++i) {
            write_if(first);
            const auto vn = p.name + '[' + std::to_string(i) + ']';
            const auto en = this->bd.getExternalName(h, p.name) + '[' +
                            std::to_string(i) + ']';
            os << "::strcmp(\"" + en + "\",key)==0){\n";
            if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
                ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
                 (!this->bd.hasParameter(
                     ModellingHypothesis::UNDEFINEDHYPOTHESIS, p.name)))) {
              os << "this->" << vn << " = v;\n";
            } else {
              os << dcname << "::get().set(\"" << en << "\",v);\n";
            }
          }
        }
      }
      os << "} else {\n";
      os << "tfel::raise(\"" << cname << "::set: \"\n"
         << "\" no parameter named "
         << "'\"+std::string(key)+\"'\");\n"
         << "}\n"
         << "}\n\n";
    }
    if (ip) {
      os << "void\n"
         << cname << "::set(const char* const key,\nconst int v)"
         << "{\n"
         << "using namespace std;\n";
      bool first = true;
      for (const auto& p : this->bd.getBehaviourData(h).getParameters()) {
        if (p.type == "int") {
          write_if(first);
          os << "::strcmp(\"" + this->bd.getExternalName(h, p.name) +
                    "\",key)==0){\n";
          if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
              ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
               (!this->bd.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                       p.name)))) {
            os << "this->" << p.name << " = v;\n";
          } else {
            os << dcname << "::get().set(\""
               << this->bd.getExternalName(h, p.name) << "\",v);\n";
          }
        }
      }
      os << "} else {\n";
      os << "tfel::raise(\"" << cname << "::set: \"\n"
         << "\"no parameter named "
         << "'\"+std::string(key)+\"'\");\n"
         << "}\n"
         << "}\n\n";
    }
    if (up) {
      os << "void\n"
         << cname << "::set(const char* const key,\nconst unsigned short v)"
         << "{\n"
         << "using namespace std;\n";
      bool first = true;
      for (const auto& p : this->bd.getBehaviourData(h).getParameters()) {
        if (p.type == "ushort") {
          write_if(first);
          os << "::strcmp(\"" + this->bd.getExternalName(h, p.name) +
                    "\",key)==0){\n";
          if ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
              ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
               (!this->bd.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                       p.name)))) {
            os << "this->" << p.name << " = v;\n";
          } else {
            os << dcname << "::get().set(\""
               << this->bd.getExternalName(h, p.name) << "\",v);\n";
          }
        }
      }
      os << "} else {\n";
      os << "tfel::raise(\"" << cname << "::set: \"\n"
         << "\"no parameter named '\"+std::string(key)+\"'\");\n"
         << "}\n"
         << "}\n\n";
    }
    if (allowsParametersInitializationFromFile(this->bd)) {
      if (rp2) {
        BehaviourCodeGeneratorBase_writeConverter(os, cname, "double",
                                                  "Double");
      }
      if (ip2) {
        BehaviourCodeGeneratorBase_writeConverter(os, cname, "int", "Int");
      }
      if (up2) {
        BehaviourCodeGeneratorBase_writeConverter(os, cname, "unsigned short",
                                                  "UnsignedShort");
      }
      os << "void\n" << cname << "::readParameters(" << cname << "&";
      if (rp2 || ip2 || up2) {
        os << " pi";
      }
      os << ",const char* const fn)"
         << "{\n"
         << "auto tokenize = [](const std::string& line){\n"
         << "std::istringstream tokenizer(line);\n"
         << "std::vector<std::string> tokens;\n"
         << "std::copy(std::istream_iterator<std::string>(tokenizer),\n"
         << "std::istream_iterator<std::string>(),\n"
         << "std::back_inserter(tokens));\n"
         << "return tokens;\n"
         << "};\n"
         << "std::ifstream f(fn);\n"
         << "if(!f){\n"
         << "return;\n"
         << "}\n"
         << "size_t ln = 1u;\n"
         << "auto line = std::string{};\n"
         << "while(std::getline(f,line)){\n"
         << "auto tokens = tokenize(line);\n"
         << "auto throw_if = [line_number = ln, fn](const bool mfront_c,\n"
         << "const std::string& mfront_error_message){\n"
         << "tfel::raise_if(mfront_c,\"" << cname << "::readParameters: \"\n"
         << "\"error at line '\"+std::to_string(line_number)+\"' \"\n"
         << "\"while reading parameter file '\"+std::string(fn)+\"'\"\n"
         << "\"(\"+mfront_error_message+\")\");\n"
         << "};\n"
         << "if(tokens.empty()){\n"
         << "continue;\n"
         << "}\n"
         << "if(tokens[0][0]=='#'){\n"
         << "continue;\n"
         << "}\n"
         << "throw_if(tokens.size()!=2u,\"invalid number of tokens\");\n";
      bool first = true;
      for (const auto& p : this->bd.getBehaviourData(h).getParameters()) {
        const auto b =
            ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) ||
             ((h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) &&
              (!this->bd.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                      p.name))));
        auto write = [this, &os, &p, &b, &dcname, &cname](
                         const std::string& vn, const std::string& en) {
          os << "\"" << en << "\"==tokens[0]){\n";
          if (b) {
            os << "pi." << vn << " = ";
            if (p.type == "int") {
              os << cname << "::getInt(tokens[0],tokens[1]);\n";
            } else if (p.type == "ushort") {
              os << cname << "::getUnsignedShort(tokens[0],tokens[1]);\n";
            } else {
              const auto f = SupportedTypes::getTypeFlag(p.type);
              if (f != SupportedTypes::SCALAR) {
                this->throwRuntimeError(
                    "BehaviourCodeGeneratorBase::"
                    "writeSrcFileParametersInitializer",
                    "invalid parameter type '" + p.type + "'");
              }

              os << cname << "::getDouble(tokens[0],tokens[1]);\n";
            }
          } else {
            os << dcname << "::get().set(\"" << en << "\",\n";
            if (p.type == "int") {
              os << dcname << "::getInt(tokens[0],tokens[1])";
            } else if (p.type == "ushort") {
              os << dcname << "::getUnsignedShort(tokens[0],tokens[1])";
            } else {
              const auto f = SupportedTypes::getTypeFlag(p.type);
              if (f != SupportedTypes::SCALAR) {
                this->throwRuntimeError(
                    "BehaviourCodeGeneratorBase::"
                    "writeSrcFileParametersInitializer",
                    "invalid parameter type '" + p.type + "'");
              }
              os << dcname << "::getDouble(tokens[0],tokens[1])";
            }
            os << ");\n";
          }
        };
        if (p.arraySize == 1u) {
          write_if(first);
          write(p.name, this->bd.getExternalName(h, p.name));
        } else {
          for (unsigned short i = 0; i != p.arraySize; ++i) {
            const auto vn = p.name + '[' + std::to_string(i) + ']';
            const auto en = this->bd.getExternalName(h, p.name) + '[' +
                            std::to_string(i) + ']';
            write_if(first);
            write(vn, en);
          }
        }
      }
      os << "} else {\n"
         << "throw_if(true,\"invalid parameter '\"+tokens[0]+\"'\");\n"
         << "}\n"
         << "}\n"
         << "}\n\n";
    }
  }  // end of writeSrcFileParametersInitializer

  void BehaviourCodeGeneratorBase::writeSrcFileBehaviourProfiler(
      std::ostream& os) const {
    if (this->bd.getAttribute(BehaviourData::profiling, false)) {
      this->checkSrcFile(os);
      os << "mfront::BehaviourProfiler&\n"
         << this->bd.getClassName() << "Profiler::getProfiler()\n"
         << "{\n"
         << "static mfront::BehaviourProfiler profiler(\""
         << this->bd.getClassName() << "\");\n"
         << "return profiler;\n"
         << "}\n\n";
    }
  }  // end of writeSrcFileBehaviourProfiler

  void BehaviourCodeGeneratorBase::writeSrcFile(std::ostream& os) const {
    this->writeSrcFileHeader(os);
    this->writeSrcFileUserDefinedCode(os);
    this->writeNamespaceBegin(os);
    this->writeSrcFileBehaviourProfiler(os);
    this->writeSrcFileParametersInitializers(os);
    this->writeNamespaceEnd(os);
  }  // end of writeSrcFile

  BehaviourCodeGeneratorBase::~BehaviourCodeGeneratorBase() = default;

}  // end of namespace mfront
