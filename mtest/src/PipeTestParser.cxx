/*!
 * \file   mtest/src/PipeTestParser.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  12 avril 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif

#include <map>
#include <cmath>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <stdexcept>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/TerminalColors.hxx"
#include "MFront/MFrontLogStream.hxx"

#include "MTest/AnalyticalTest.hxx"
#include "MTest/ReferenceFileComparisonTest.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/FunctionEvolution.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/Constraint.hxx"
#include "MTest/ImposedThermodynamicForce.hxx"
#include "MTest/ImposedGradient.hxx"
#include "MTest/PipeTest.hxx"
#include "MTest/PipeTestParser.hxx"

namespace mtest {

  PipeTestParser::PipeTestParser() { this->registerCallBacks(); }

  void PipeTestParser::parseString(PipeTest& t, const std::string& f) {
    this->file = "user defined string";
    this->treatCharAsString(true);
    CxxTokenizer::parseString(f);
    this->stripComments();
    this->execute(t);
  }  // end of PipeTestParser::parseString

  void PipeTestParser::execute(PipeTest& t,
                               const std::string& f,
                               const std::vector<std::string>& ecmds,
                               const std::map<std::string, std::string>& s) {
    this->file = f;
    this->treatCharAsString(true);
    this->openFile(f);
    this->stripComments();
    // substitutions
    const auto pe = s.end();
    for (auto& token : this->tokens) {
      auto p = s.find(token.value);
      if (p != pe) {
        token.value = p->second;
        if (((p->second.front() == '\'') && (p->second.back() == '\'')) ||
            ((p->second.front() == '"') && (p->second.back() == '"'))) {
          token.flag = tfel::utilities::Token::String;
        }
      }
    }
    // treating external commands
    for (const auto& c : ecmds) {
      CxxTokenizer tokenizer;
      tokenizer.treatCharAsString(true);
      try {
        tokenizer.parseString(c);

      } catch (std::exception& e) {
        tfel::raise(
            "PipeTestParser::execute: "
            "error while parsing external command "
            "'" +
            c + "'\n" + std::string(e.what()));
      }
      this->tokens.insert(this->tokens.begin(), tokenizer.begin(),
                          tokenizer.end());
    }
    this->execute(t);
  }

  void PipeTestParser::execute(PipeTest& t) {
    auto p = this->tokens.cbegin();
    while (p != tokens.end()) {
      if (PipeTestParser::treatKeyword(t, p)) {
        continue;
      }
      if (SingleStructureSchemeParser::treatKeyword(t, p)) {
        continue;
      }
      if (SchemeParserBase::treatKeyword(t, p)) {
        continue;
      }
      std::ostringstream msg;
      msg << "PipeTestParser::execute : invalid keyword '" << p->value
          << "'. Error at line " << p->line << ".";
      tfel::raise(msg.str());
    }
  }  // end of PipeTestParser::execute

  bool PipeTestParser::treatKeyword(PipeTest& t, tokens_iterator& p) {
    auto pc = this->callbacks.find(p->value);
    if (pc == this->callbacks.end()) {
      return false;
    }
    if (mfront::getVerboseMode() >= mfront::VERBOSE_DEBUG) {
      auto& log = mfront::getLogStream();
      log << "PipeTestParser::execute : treating keyword '" << p->value
          << "' at line '" << p->line << "'\n";
    }
    ++p;
    auto line = p->line;
    try {
      (this->*(pc->second))(t, p);
    } catch (std::exception& e) {
      std::ostringstream msg;
      msg << "PipeTestParser::treatKeyword: error while "
          << "parsing file '" << this->file << "' at line '" << line << "'.\n"
          << e.what();
      tfel::raise(msg.str());
    }
    return true;
  }  // end of PipeTestParser::treatKeyword

  std::vector<std::string> PipeTestParser::getKeyWordsList() const {
    auto keys = SchemeParserBase::getKeyWordsList();
    auto keys2 = SingleStructureSchemeParser::getKeyWordsList();
    keys.insert(keys.end(), keys2.begin(), keys2.end());
    for (const auto& c : this->callbacks) {
      keys.push_back(c.first);
    }
    return keys;
  }

  void PipeTestParser::displayKeyWordsList() const {
    using namespace tfel::utilities;
    auto keys = this->getKeyWordsList();
    std::string::size_type msize = 0;
    for (const auto& k : keys) {
      msize = std::max(msize, k.size());
    }
    for (const auto& k : keys) {
      const auto f = SchemeParserBase::getDocumentationFilePath("ptest", k);
      auto key = k;
      key.resize(msize, ' ');
      std::cout << key << "  ";
      if (!f.empty()) {
        std::cout.write(TerminalColors::Green, sizeof(TerminalColors::Green));
        std::cout << "(documented)";
      } else {
        std::cout.write(TerminalColors::Red, sizeof(TerminalColors::Red));
        std::cout << "(undocumented)";
      }
      std::cout.write(TerminalColors::Reset, sizeof(TerminalColors::Reset));
      std::cout << '\n';
    }
  }  // end of PipeTestParser::displayKeywordsList

  void PipeTestParser::displayKeyWordsHelp() const {
    auto keys = this->getKeyWordsList();
    std::cout << "% `PipeTest` keywords\n\n";
    for (const auto& k : keys) {
      const auto f = SchemeParserBase::getDocumentationFilePath("ptest", k);
      std::cout << "\n# The `" << k << "` keyword\n\n";
      if (!f.empty()) {
        std::ifstream desc{f};
        if (!desc) {
          std::cout << desc.rdbuf();
        } else {
          std::cout << "The keyword `" << k << "` is not documented yet\n";
        }
      } else {
        std::cout << "The keyword `" << k << "` is not documented yet\n";
      }
    }
  }  // end of PipeTestParser::displayKeywordsHelp

  void PipeTestParser::displayKeyWordDescription(const std::string& k) const {
    auto keys = this->getKeyWordsList();
    tfel::raise_if(std::find(keys.begin(), keys.end(), k) == keys.end(),
                   "PipeTestParser::displayKeyWordDescription: "
                   "unknown keyword '" +
                       k + "'");
    const auto f = SchemeParserBase::getDocumentationFilePath("ptest", k);
    if (f.empty()) {
      std::cout << "no description available for keyword '" << k << "'"
                << std::endl;
      return;
    }
    std::ifstream desc{f};
    if (!desc) {
      std::cout << "no description available for keyword '" << k << "'"
                << std::endl;
    } else {
      std::cout << desc.rdbuf();
    }
  }

  void PipeTestParser::registerCallBacks() {
    SchemeParserBase::registerCallBacks();
    SingleStructureSchemeParser::registerCallBacks();
    this->registerCallBack("@RadialLoading",
                           &PipeTestParser::handleRadialLoading);
    this->registerCallBack("@AxialLoading",
                           &PipeTestParser::handleAxialLoading);
    this->registerCallBack("@InnerRadius", &PipeTestParser::handleInnerRadius);
    this->registerCallBack("@OuterRadius", &PipeTestParser::handleOuterRadius);
    this->registerCallBack("@NumberOfElements",
                           &PipeTestParser::handleNumberOfElements);
    this->registerCallBack("@ElementType", &PipeTestParser::handleElementType);
    this->registerCallBack("@MandrelRadiusEvolution",
                           &PipeTestParser::handleMandrelRadiusEvolution);
    this->registerCallBack("@MandrelAxialGrowthEvolution",
                           &PipeTestParser::handleMandrelAxialGrowthEvolution);
    this->registerCallBack("@InnerPressureEvolution",
                           &PipeTestParser::handleInnerPressureEvolution);
    this->registerCallBack("@OuterPressureEvolution",
                           &PipeTestParser::handleOuterPressureEvolution);
    this->registerCallBack("@AxialForceEvolution",
                           &PipeTestParser::handleAxialForceEvolution);
    this->registerCallBack("@AxialGrowthEvolution",
                           &PipeTestParser::handleAxialGrowthEvolution);
    this->registerCallBack("@InnerRadiusEvolution",
                           &PipeTestParser::handleInnerRadiusEvolution);
    this->registerCallBack("@OuterRadiusEvolution",
                           &PipeTestParser::handleOuterRadiusEvolution);
    this->registerCallBack("@FillingPressure",
                           &PipeTestParser::handleFillingPressure);
    this->registerCallBack("@FillingTemperature",
                           &PipeTestParser::handleFillingTemperature);
    this->registerCallBack("@PerformSmallStrainAnalysis",
                           &PipeTestParser::handlePerformSmallStrainAnalysis);
    this->registerCallBack("@DisplacementEpsilon",
                           &PipeTestParser::handleDisplacementEpsilon);
    this->registerCallBack("@ResidualEpsilon",
                           &PipeTestParser::handleResidualEpsilon);
    this->registerCallBack("@Profile", &PipeTestParser::handleProfile);
    this->registerCallBack("@Test", &PipeTestParser::handleTest);
    this->registerCallBack("@GasEquationOfState",
                           &PipeTestParser::handleGasEquationOfState);
    this->registerCallBack("@AdditionalOutputs",
                           &PipeTestParser::handleAdditionalOutputs);
    this->registerCallBack("@FailurePolicy",
                           &PipeTestParser::handleFailurePolicy);
    this->registerCallBack("@FailureCriterion",
                           &PipeTestParser::handleFailureCriterion);
    this->registerCallBack("@OxidationModel",
                           &PipeTestParser::handleOxidationModel);
  }

  void PipeTestParser::registerCallBack(const std::string& k,
                                        const PipeTestParser::CallBack& p) {
    this->callbacks.insert({k, p});
  }

  void PipeTestParser::handleRadialLoading(PipeTest& t, tokens_iterator& p) {
    this->checkNotEndOfLine("PipeTestParser::handleRadialLoading", p,
                            this->tokens.end());
    const auto& h = this->readString(p, this->tokens.end());
    if (h == "ImposedPressure") {
      t.setRadialLoading(mtest::PipeTest::IMPOSEDPRESSURE);
    } else if (h == "TightPipe") {
      t.setRadialLoading(mtest::PipeTest::TIGHTPIPE);
    } else if (h == "ImposedInnerRadius") {
      t.setRadialLoading(mtest::PipeTest::IMPOSEDINNERRADIUS);
    } else if (h == "ImposedOuterRadius") {
      t.setRadialLoading(mtest::PipeTest::IMPOSEDOUTERRADIUS);
    } else {
      tfel::raise(
          "PipeTestParser::handleRadialLoading: "
          "invalid loading type ('" +
          h +
          "').\n"
          "Valid modelling hypothesis are "
          "'ImposedPressure' and 'ImposedOuterRadius'");
    }
    this->checkNotEndOfLine("PipeTestParser::handleRadialLoading", p,
                            this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handleRadialLoading", ";", p,
                             this->tokens.end());
  }  // end of PipeTestParser::handleRadialLoading

  void PipeTestParser::handleAxialLoading(PipeTest& t, tokens_iterator& p) {
    this->checkNotEndOfLine("PipeTestParser::handleAxialLoading", p,
                            this->tokens.end());
    const auto& h = this->readString(p, this->tokens.end());
    if (h == "None") {
      t.setAxialLoading(mtest::PipeTest::NONE);
    } else if (h == "EndCapEffect") {
      t.setAxialLoading(mtest::PipeTest::ENDCAPEFFECT);
    } else if (h == "ImposedAxialForce") {
      t.setAxialLoading(mtest::PipeTest::IMPOSEDAXIALFORCE);
    } else if (h == "ImposedAxialGrowth") {
      t.setAxialLoading(mtest::PipeTest::IMPOSEDAXIALGROWTH);
    } else {
      tfel::raise(
          "PipeTestParser::handleAxialLoading: "
          "invalid pipe modelling hypothesis ('" +
          h +
          "').\n"
          "Valid modelling hypothesis are "
          "'None', 'ImposedAxialGrowth', "
          "'ImposedAxialForce' and 'EndCapEffect'");
    }
    this->checkNotEndOfLine("PipeTestParser::handleAxialLoading", p,
                            this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handleAxialLoading", ";", p,
                             this->tokens.end());
  }  // end of PipeTestParser::handleAxialLoading

  void PipeTestParser::handleInnerRadius(PipeTest& t, tokens_iterator& p) {
    this->checkNotEndOfLine("PipeTestParser::handleInnerRadius", p,
                            this->tokens.end());
    t.setInnerRadius(this->readDouble(t, p));
    this->checkNotEndOfLine("PipeTestParser::handleInnerRadius", p,
                            this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handleInnerRadius", ";", p,
                             this->tokens.end());
  }  // end of PipeTestParser::handleInnerRadius

  void PipeTestParser::handleOuterRadius(PipeTest& t, tokens_iterator& p) {
    this->checkNotEndOfLine("PipeTestParser::handleOuterRadius", p,
                            this->tokens.end());
    t.setOuterRadius(this->readDouble(t, p));
    this->checkNotEndOfLine("PipeTestParser::handleOuterRadius", p,
                            this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handleOuterRadius", ";", p,
                             this->tokens.end());
  }  // end of PipeTestParser::handleOuterRadius

  void PipeTestParser::handleNumberOfElements(PipeTest& t, tokens_iterator& p) {
    this->checkNotEndOfLine("PipeTestParser::handleNumberOfElements", p,
                            this->tokens.end());
    t.setNumberOfElements(this->readInt(p, this->tokens.end()));
    this->checkNotEndOfLine("PipeTestParser::handleNumberOfElements", p,
                            this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handleNumberOfElements", ";", p,
                             this->tokens.end());
  }  // end of PipeTestParser::handleNumberOfElements

  void PipeTestParser::handleElementType(PipeTest& t, tokens_iterator& p) {
    this->checkNotEndOfLine("PipeTestParser::handleElementType", p,
                            this->tokens.end());
    const auto& e = this->readString(p, this->tokens.end());
    if (e == "Linear") {
      t.setElementType(PipeMesh::LINEAR);
    } else if (e == "Quadratic") {
      t.setElementType(PipeMesh::QUADRATIC);
    } else if (e == "Cubic") {
      t.setElementType(PipeMesh::CUBIC);
    } else {
      tfel::raise(
          "PipeTest::setElementType: "
          "invalid element type ('" +
          e +
          "').\n"
          "Valid element type are "
          "'Linear' and 'Quadratic'");
    }
    this->checkNotEndOfLine("PipeTestParser::handleElementType", p,
                            this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handleElementType", ";", p,
                             this->tokens.end());
  }  // end of PipeTestParser::handleElementType

  void PipeTestParser::handleGasEquationOfState(PipeTest& t,
                                                tokens_iterator& p) {
    const auto& e = this->readString(p, this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handleGasEquationOfState", ";", p,
                             this->tokens.end());
    t.setGasEquationOfState(e);
  }

  void PipeTestParser::handlePerformSmallStrainAnalysis(PipeTest& t,
                                                        tokens_iterator& p) {
    this->checkNotEndOfLine("PipeTestParser::handlePerformSmallStrainAnalysis",
                            p, this->tokens.end());
    if (p->value == "true") {
      t.performSmallStrainAnalysis();
    } else if (p->value != "false") {
      tfel::raise(
          "PipeTestParser::handlePerformSmallStrainAnalysis: "
          "unexpected token '" +
          p->value + "'");
    }
    ++p;
    this->checkNotEndOfLine("PipeTestParser::handlePerformSmallStrainAnalysis",
                            p, this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handlePerformSmallStrainAnalysis",
                             ";", p, this->tokens.end());
  }  // end of PipeTestParser::handlePerformSmallStrainAnalysis

  void PipeTestParser::handleInnerPressureEvolution(PipeTest& t,
                                                    tokens_iterator& p) {
    const auto& evt = this->readEvolutionType(p);
    this->checkNotEndOfLine("PipeTestParser::handleInnerPressureEvolution", p,
                            this->tokens.end());
    t.setInnerPressureEvolution(this->parseEvolution(t, evt, p));
    this->checkNotEndOfLine("PipeTestParser::handleInnerPressureEvolution", p,
                            this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handleInnerPressureEvolution",
                             ";", p, this->tokens.end());
  }

  void PipeTestParser::handleOuterPressureEvolution(PipeTest& t,
                                                    tokens_iterator& p) {
    const auto& evt = this->readEvolutionType(p);
    this->checkNotEndOfLine("PipeTestParser::handleOuterPressureEvolution", p,
                            this->tokens.end());
    t.setOuterPressureEvolution(this->parseEvolution(t, evt, p));
    this->checkNotEndOfLine("PipeTestParser::handleOuterPressureEvolution", p,
                            this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handleOuterPressureEvolution",
                             ";", p, this->tokens.end());
  }

  void PipeTestParser::handleInnerRadiusEvolution(PipeTest& t,
                                                  tokens_iterator& p) {
    const auto& evt = this->readEvolutionType(p);
    this->checkNotEndOfLine("PipeTestParser::handleInnerRadiusEvolution", p,
                            this->tokens.end());
    t.setInnerRadiusEvolution(this->parseEvolution(t, evt, p));
    this->checkNotEndOfLine("PipeTestParser::handleInnerRadiusEvolution", p,
                            this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handleInnerRadiusEvolution", ";",
                             p, this->tokens.end());
  }

  void PipeTestParser::handleOuterRadiusEvolution(PipeTest& t,
                                                  tokens_iterator& p) {
    const auto& evt = this->readEvolutionType(p);
    this->checkNotEndOfLine("PipeTestParser::handleOuterRadiusEvolution", p,
                            this->tokens.end());
    t.setOuterRadiusEvolution(this->parseEvolution(t, evt, p));
    this->checkNotEndOfLine("PipeTestParser::handleOuterRadiusEvolution", p,
                            this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handleOuterRadiusEvolution", ";",
                             p, this->tokens.end());
  }

  void PipeTestParser::handleMandrelRadiusEvolution(PipeTest& t,
                                                    tokens_iterator& p) {
    const auto& evt = this->readEvolutionType(p);
    this->checkNotEndOfLine("PipeTestParser::handleMandrelRadiusEvolution", p,
                            this->tokens.end());
    t.setMandrelRadiusEvolution(this->parseEvolution(t, evt, p));
    this->checkNotEndOfLine("PipeTestParser::handleMandrelRadiusEvolution", p,
                            this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handleMandrelRadiusEvolution",
                             ";", p, this->tokens.end());
  }  // end of handleMandrelRadiusEvolution

  void PipeTestParser::handleMandrelAxialGrowthEvolution(PipeTest& t,
                                                         tokens_iterator& p) {
    const auto& evt = this->readEvolutionType(p);
    this->checkNotEndOfLine("PipeTestParser::handleMandrelAxialGrowthEvolution",
                            p, this->tokens.end());
    t.setMandrelAxialGrowthEvolution(this->parseEvolution(t, evt, p));
    this->checkNotEndOfLine("PipeTestParser::handleMandrelAxialGrowthEvolution",
                            p, this->tokens.end());
    this->readSpecifiedToken(
        "PipeTestParser::handleMandrelAxialGrowthEvolution", ";", p,
        this->tokens.end());
  }  // end of handleMandrelAxialGrowthEvolution

  void PipeTestParser::handleAxialForceEvolution(PipeTest& t,
                                                 tokens_iterator& p) {
    const auto& evt = this->readEvolutionType(p);
    this->checkNotEndOfLine("PipeTestParser::handleAxialForceEvolution", p,
                            this->tokens.end());
    t.setAxialForceEvolution(this->parseEvolution(t, evt, p));
    this->checkNotEndOfLine("PipeTestParser::handleAxialForceEvolution", p,
                            this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handleAxialForceEvolution", ";",
                             p, this->tokens.end());
  }

  void PipeTestParser::handleAxialGrowthEvolution(PipeTest& t,
                                                  tokens_iterator& p) {
    const auto& evt = this->readEvolutionType(p);
    this->checkNotEndOfLine("PipeTestParser::handleAxialGrowthEvolution", p,
                            this->tokens.end());
    t.setAxialGrowthEvolution(this->parseEvolution(t, evt, p));
    this->checkNotEndOfLine("PipeTestParser::handleAxialGrowthEvolution", p,
                            this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handleAxialGrowthEvolution", ";",
                             p, this->tokens.end());
  }

  void PipeTestParser::handleFillingPressure(PipeTest& t, tokens_iterator& p) {
    t.setFillingPressure(this->readDouble(t, p));
    this->checkNotEndOfLine("PipeTestParser::handleFillingPressure", p,
                            this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handleFillingPressure", ";", p,
                             this->tokens.end());
  }  // end of PipeTestParser::handleFillingPressure

  void PipeTestParser::handleFillingTemperature(PipeTest& t,
                                                tokens_iterator& p) {
    t.setFillingTemperature(this->readDouble(t, p));
    this->checkNotEndOfLine("PipeTestParser::handleFillingTemperature", p,
                            this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handleFillingTemperature", ";", p,
                             this->tokens.end());
  }  // end of PipeTestParser::handleFillingTemperature

  void PipeTestParser::handleDisplacementEpsilon(PipeTest& t,
                                                 tokens_iterator& p) {
    this->checkNotEndOfLine("PipeTestParser::handleDisplacementEpsilon", p,
                            this->tokens.end());
    t.setDisplacementEpsilon(this->readDouble(t, p));
    this->checkNotEndOfLine("PipeTestParser::handleDisplacementEpsilon", p,
                            this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handleDisplacementEpsilon", ";",
                             p, this->tokens.end());
  }

  void PipeTestParser::handleResidualEpsilon(PipeTest& t, tokens_iterator& p) {
    this->checkNotEndOfLine("PipeTestParser::handleResidualEpsilon", p,
                            this->tokens.end());
    t.setResidualEpsilon(this->readDouble(t, p));
    this->checkNotEndOfLine("PipeTestParser::handleResidualEpsilon", p,
                            this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handleResidualEpsilon", ";", p,
                             this->tokens.end());
  }

  void PipeTestParser::handleProfile(PipeTest& t, tokens_iterator& p) {
    this->checkNotEndOfLine("PipeTestParser::handleProfile", p,
                            this->tokens.end());
    const auto f = this->readString(p, this->tokens.end());
    this->checkNotEndOfLine("PipeTestParser::handleProfile", p,
                            this->tokens.end());
    auto v = std::vector<std::string>{};
    if (p->flag == tfel::utilities::Token::String) {
      v.push_back(this->readString(p, this->tokens.end()));
    } else {
      this->readSpecifiedToken("PipeTestParser::handleProfile", "{", p,
                               this->tokens.end());
      this->checkNotEndOfLine("PipeTestParser::handleProfile", p,
                              this->tokens.end());
      while (p->value != "}") {
        v.push_back(this->readString(p, this->tokens.end()));
        this->checkNotEndOfLine("PipeTestParser::handleProfile", p,
                                this->tokens.end());
        if (p->value != "}") {
          this->readSpecifiedToken("PipeTestParser::handleProfile", ",", p,
                                   this->tokens.end());
          this->checkNotEndOfLine("PipeTestParser::handleProfile", p,
                                  this->tokens.end());
          tfel::raise_if(p->value == "}",
                         "PipeTestParser::handleProfile: "
                         "unexpected token '}'");
        }
      }
      this->readSpecifiedToken("PipeTestParser::handleProfile", "}", p,
                               this->tokens.end());
    }
    t.addProfile(f, v);
    this->checkNotEndOfLine("PipeTestParser::handleProfile", p,
                            this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handleProfile", ";", p,
                             this->tokens.end());
  }

  void PipeTestParser::handleTest(PipeTest& t, tokens_iterator& p) {
    this->readSpecifiedToken("PipeTestParser::handleTest", "<", p,
                             this->tokens.end());
    this->checkNotEndOfLine("PipeTestParser::handleTest", p,
                            this->tokens.end());
    const auto& type = p->value;
    tfel::raise_if((type != "file") && (type != "function"),
                   "PipeTestParser::handleTest: invalid test type '" + type +
                       "', expected 'file' or 'function'");
    ++p;
    bool profile = false;
    if (p->value == ",") {
      ++p;
      this->checkNotEndOfLine("PipeTestParser::handleTest", p,
                              this->tokens.end());
      if (p->value == "integral") {
        profile = false;
      } else if (p->value == "profile") {
        profile = true;
      } else {
        tfel::raise(
            "PipeTestParser::handleTest: "
            "invalid test option '" +
            p->value + "'");
      }
      ++p;
      this->checkNotEndOfLine("PipeTestParser::handleTest", p,
                              this->tokens.end());
    }
    this->readSpecifiedToken("PipeTestParser::handleTest", ">", p,
                             this->tokens.end());
    if (type == "file") {
      const auto& f = this->readString(p, this->tokens.end());
      this->checkNotEndOfLine("PipeTestParser::handleTest", p,
                              this->tokens.end());
      auto variables = std::map<std::string, unsigned int>{};
      if (p->flag == tfel::utilities::Token::String) {
        const auto& v = this->readString(p, this->tokens.end());
        const auto c = this->readUnsignedInt(p, this->tokens.end());
        variables.insert({v, c});
      } else {
        this->readSpecifiedToken("PipeTestParser::handleTest", "{", p,
                                 this->tokens.end());
        this->checkNotEndOfLine("PipeTestParser::handleTest", p,
                                this->tokens.end());
        while (p->value != "}") {
          const auto& v = this->readString(p, this->tokens.end());
          this->readSpecifiedToken("PipeTestParser::handleTest", ":", p,
                                   this->tokens.end());
          const auto c = this->readUnsignedInt(p, this->tokens.end());
          variables.insert({v, c});
          this->checkNotEndOfLine("PipeTestParser::handleTest", p,
                                  this->tokens.end());
          if (p->value != "}") {
            this->readSpecifiedToken("PipeTestParser::handleTest", ",", p,
                                     this->tokens.end());
            this->checkNotEndOfLine("PipeTestParser::handleTest", p,
                                    this->tokens.end());
            tfel::raise_if(p->value == "}",
                           "PipeTestParser::handleTest: "
                           "unexpected token '}'");
          }
        }
        this->readSpecifiedToken("PipeTestParser::handleTest", "}", p,
                                 this->tokens.end());
      }
      const real eps = this->readDouble(t, p);
      tfel::raise_if(eps < 0,
                     "PipeTestParser::handleTest: "
                     "invalid criterion value");
      const tfel::utilities::TextData data(f);
      if (profile) {
        for (const auto& v : variables) {
          t.addProfileTest(v.first, data, v.second, eps);
        }
      } else {
        for (const auto& v : variables) {
          t.addIntegralTest(v.first, data, v.second, eps);
        }
      }
    } else if (type == "function") {
      auto variables = std::map<std::string, std::string>{};
      if (p->flag == tfel::utilities::Token::String) {
        const auto& v = this->readString(p, this->tokens.end());
        const auto ev = this->readString(p, this->tokens.end());
        variables.insert({v, ev});
      } else {
        tfel::raise_if(profile,
                       "PipeTestParser::handleTest: "
                       "analytical test defined by a "
                       "function are not supported yet");
        this->readSpecifiedToken("PipeTestParser::handleTest", "{", p,
                                 this->tokens.end());
        this->checkNotEndOfLine("PipeTestParser::handleTest", p,
                                this->tokens.end());
        while (p->value != "}") {
          const auto& v = this->readString(p, this->tokens.end());
          this->readSpecifiedToken("PipeTestParser::handleTest", ":", p,
                                   this->tokens.end());
          const auto ev = this->readString(p, this->tokens.end());
          variables.insert({v, ev});
          this->checkNotEndOfLine("PipeTestParser::handleTest", p,
                                  this->tokens.end());
          if (p->value != "}") {
            this->readSpecifiedToken("PipeTestParser::handleTest", ",", p,
                                     this->tokens.end());
            this->checkNotEndOfLine("PipeTestParser::handleTest", p,
                                    this->tokens.end());
            tfel::raise_if(p->value == "}",
                           "PipeTestParser::handleTest: "
                           "unexpected token '}'");
          }
        }
        this->readSpecifiedToken("PipeTestParser::handleTest", "}", p,
                                 this->tokens.end());
      }
      const real eps = this->readDouble(t, p);
      tfel::raise_if(eps < 0,
                     "PipeTestParser::handleTest: "
                     "invalid criterion value");
      for (const auto& v : variables) {
        auto ev =
            std::make_shared<FunctionEvolution>(v.second, t.getEvolutions());
        t.addIntegralTest(v.first, ev, eps);
      }
    } else {
      tfel::raise(
          "PipeTestParser::handleTest: "
          "invalid test type '" +
          type + "'");
    }
    this->readSpecifiedToken("PipeTestParser::handleTest", ";", p,
                             this->tokens.end());
  }  // end of PipeTestParser::handleTest

  void PipeTestParser::handleAdditionalOutputs(PipeTest& t,
                                               tokens_iterator& p) {
    using tfel::utilities::Data;
    this->checkNotEndOfLine("PipeTestParser::handleAdditionalOutputs", p,
                            this->tokens.end());
    auto generate_function = [&t](const std::string& n) {
      return [&t, n = n](const Data& v) {
        if (v.is<std::string>()) {
          t.addOutput(n, v.get<std::string>());
        } else {
          for (const auto& f :
               tfel::utilities::convert<std::vector<std::string>>(v)) {
            t.addOutput(n, f);
          }
        }
      };
    };
    tfel::utilities::Data::parse(
        p, this->tokens.end(),
        {{"minimum_value", generate_function("minimum_value")},
         {"minimum_values", generate_function("minimum_value")},
         {"maximum_value", generate_function("maximum_value")},
         {"maximum_values", generate_function("maximum_value")},
         {"integral_value",
          generate_function("integral_value_initial_configuration")},
         {"integral_values",
          generate_function("integral_value_initial_configuration")},
         {"integral_value_initial_configuration",
          generate_function("integral_value_initial_configuration")},
         {"integral_values_initial_configuration",
          generate_function("integral_value_initial_configuration")},
         {"integral_value_current_configuration",
          generate_function("integral_value_current_configuration")},
         {"integral_values_current_configuration",
          generate_function("integral_value_current_configuration")},
         {"mean_value", generate_function("mean_value_initial_configuration")},
         {"mean_values", generate_function("mean_value_initial_configuration")},
         {"mean_value_initial_configuration",
          generate_function("mean_value_initial_configuration")},
         {"mean_values_initial_configuration",
          generate_function("mean_value_initial_configuration")},
         {"mean_value_current_configuration",
          generate_function("mean_value_current_configuration")},
         {"mean_values_current_configuration",
          generate_function("mean_value_current_configuration")}});
    this->readSpecifiedToken("PipeTestParser::handleAdditionalOutputs", ";", p,
                             this->tokens.end());
  }  // end of PipeTestParser::handleAdditionalOutput

  void PipeTestParser::handleFailurePolicy(PipeTest& t, tokens_iterator& p) {
    this->checkNotEndOfLine("PipeTestParser::handleFailurePolicy", p,
                            this->tokens.end());
    const auto policy = this->readString(p, this->tokens.end());
    this->readSpecifiedToken("PipeTestParser::handleFailurePolicy", ";", p,
                             this->tokens.end());
    if (policy == "ReportOnly") {
      t.setFailurePolicy(PipeTest::REPORTONLY);
    } else if (policy == "StopComputation") {
      t.setFailurePolicy(PipeTest::STOPCOMPUTATION);
    } else if ((policy == "FreezeState") ||
               (policy == "FreezeStateUntilEndOfComputation")) {
      t.setFailurePolicy(PipeTest::FREEZESTATEUNTILENDOFCOMPUTATION);
    } else {
      tfel::raise(
          "PipeTestParser::handleFailurePolicy: "
          "invalid failure policy '" +
          policy +
          "'. Valid policies are:\n"
          "- 'ReportOnly': failure detection is reported in the output file, "
          "but computations are performed as usual\n"
          "- 'StopComputation': if a failure is detected, computations are "
          "stopped\n"
          "- 'FreezeState' or 'FreezeStateUntilEndOfComputation': if a "
          "failure is detected, the state of the structure is freezed and do "
          "not evolve. No equilibrium is performed, the behaviour is no more "
          "called and PipeTest will output the same results again and again "
          "until the end of computation. This option may be useful when "
          "optimizing material parameters.\n");
    }
  }  // end of handleFailurePolicy

  void PipeTestParser::handleFailureCriterion(PipeTest& t, tokens_iterator& p) {
    this->checkNotEndOfLine("PipeTestParser::handleFailureCriterion", p,
                            this->tokens.end());
    const auto n = this->readString(p, this->tokens.end());
    this->checkNotEndOfLine("PipeTestParser::handleFailureCriterion", p,
                            this->tokens.end());
    if (p->value == ";") {
      t.addFailureCriterion(n, tfel::utilities::DataMap{});
    } else {
      t.addFailureCriterion(
          n, tfel::utilities::Data::read_map(p, this->tokens.end())
                 .get<tfel::utilities::DataMap>());
    }
    this->readSpecifiedToken("PipeTestParser::handleFailureCriterion", ";", p,
                             this->tokens.end());
  }  // end of handleFailureCriterion

  void PipeTestParser::handleOxidationModel(PipeTest& t, tokens_iterator& p) {
    this->checkNotEndOfLine("PipeTestParser::handleOxidationModel", p,
                            this->tokens.end());
    const auto options = tfel::utilities::Data::read_map(p, this->tokens.end())
                             .get<tfel::utilities::DataMap>();
    this->readSpecifiedToken("PipeTestParser::handleOxidationModel", ";", p,
                             this->tokens.end());
    //
    auto validator =
        tfel::utilities::DataMapValidator()
            .addDataTypeValidator<std::string>("model")
            .addDataTypeValidator<std::string>("library")
            .addDataValidator("boundary", [](const tfel::utilities::Data& d) {
              if (!d.is<std::string>()) {
                tfel::raise(
                    "PipeTestParser::handleOxidationModel: "
                    "invalid type for option 'boundary'");
              }
              const auto& b = d.get<std::string>();
              if (!((b == "inner_boundary") || (b == "outer_boundary"))) {
                tfel::raise(
                    "PipeTestParser::handleOxidationModel: "
                    "invalid value for option 'boundary', expected "
                    "'inner_boundary' or 'outer_boundary', read '" +
                    b + "'");
              }
            });
    validator.validate(options);
    //
    if (options.size() != 3u) {
      tfel::raise(
          "PipeTestParser::handleOxidationModel: "
          "invalid number of options, expected "
          "'model', 'library', and 'boundary'");
    }
    //
    t.addOxidationModel(options.at("library").get<std::string>(),
                        options.at("model").get<std::string>(),
                        options.at("boundary").get<std::string>());
  }  // end of handleOxidationModel

  PipeTestParser::~PipeTestParser() = default;

}  // end namespace mtest
