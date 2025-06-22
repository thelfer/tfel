/*!
 * \file   mfm-test-generator/src/InputFileGeneratorFactory.cxx
 * \brief
 * \author Thomas Helfer
 * \date   17/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFMTestGenerator/MTestInputFileGenerators.hxx"
#include "MFMTestGenerator/LicosInputFileGenerators.hxx"
#include "MFMTestGenerator/InputFileGeneratorFactory.hxx"

namespace mfmtg {

  InputFileGeneratorFactory& InputFileGeneratorFactory::get() {
    static InputFileGeneratorFactory f;
    return f;
  }  // end of InputFileGeneratorFactory::get

  InputFileGeneratorFactory::InputFileGeneratorFactory() {
    auto add_mtest = [this](const char* n, const generator& g) {
      this->add(n, "mtest", g);
    };
    auto add_licos = [this](const char* n, const generator& g) {
      this->add(n, "licos", g);
    };
    add_mtest("UniaxialTensileTest", mfmtg::mtest::generateUniaxialTensileTest);
    add_mtest("ClosedPipeTest", mfmtg::mtest::generateClosedPipeTest);
    this->add("ClosedPipeTest", "ptest", mfmtg::mtest::generateClosedPipeTest);
    add_licos("UniaxialTensileTest", mfmtg::licos::generateUniaxialTensileTest);
  }  // end of InputFileGeneratorFactory::InputFileGeneratorFactory

  void InputFileGeneratorFactory::add(const std::string& n,
                                      const std::string& t,
                                      const generator g) {
    auto& tg = this->generators[n];
    if (!tg.insert({t, g}).second) {
      tfel::raise(
          "InputFileGeneratorFactory::register: "
          "an input file generator has already been "
          "registred for target code '" +
          t + "' for the '" + n + "' test case");
    }
  }  // end of InputFileGeneratorFactory::register

  InputFileGeneratorFactory::generator InputFileGeneratorFactory::get(
      const std::string& n, const std::string& t) const {
    const auto p = this->generators.find(n);
    if (p == this->generators.end()) {
      tfel::raise(
          "InputFileGeneratorFactory::get: "
          "no input file generator registred "
          "for test case '" +
          n + "'");
    }
    const auto& tg = p->second;
    const auto pg = tg.find(t);
    if (pg == tg.end()) {
      tfel::raise(
          "InputFileGeneratorFactory::get: "
          "no input file generator for target code '" +
          t + "' for the '" + n + "' test case ");
    }
    return pg->second;
  }  // end of InputFileGeneratorFactory::get

  InputFileGeneratorFactory::~InputFileGeneratorFactory() = default;

}  // end of namespace mfmtg
