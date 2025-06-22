/*!
 * \file   mfront/mtest/PipeTestParser.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  12 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
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

#include "TFEL/Utilities/TerminalColors.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "MFront/MFrontLogStream.hxx"

#include "MTest/AnalyticalTest.hxx"
#include "MTest/ReferenceFileComparisonTest.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/Constraint.hxx"
#include "MTest/ImposedThermodynamicForce.hxx"
#include "MTest/ImposedDrivingVariable.hxx"
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
      if (token.flag == tfel::utilities::Token::String) {
        auto delim = token.value.at(0);
        auto contents = token.value.substr(1, token.value.size() - 2);
        for (const auto& kv : s) {
          contents =
              tfel::utilities::replace_all(contents, kv.first, kv.second);
        }
        token.value = delim + contents + delim;
      } else {
        auto p = s.find(token.value);
        if (p != pe) {
          token.value = p->second;
          if (((p->second.front() == '\'') && (p->second.back() == '\'')) ||
              ((p->second.front() == '"') && (p->second.back() == '"'))) {
            token.flag = tfel::utilities::Token::String;
          }
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
        throw(
            std::runtime_error("PipeTestParser::execute: "
                               "error while parsing external command "
                               "'" +
                               c + "'\n" + std::string(e.what())));
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
      throw(std::runtime_error(msg.str()));
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
      throw(std::runtime_error(msg.str()));
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
    using namespace std;
    using namespace tfel::utilities;
    auto keys = this->getKeyWordsList();
    string::size_type msize = 0;
    for (const auto& k : keys) {
      msize = max(msize, k.size());
    }
    for (const auto& k : keys) {
      const auto f = SchemeParserBase::getDocumentationFilePath("ptest", k);
      auto key = k;
      key.resize(msize, ' ');
      cout << key << "  ";
      if (!f.empty()) {
        cout.write(TerminalColors::Green, sizeof(TerminalColors::Green));
        cout << "(documented)";
      } else {
        cout.write(TerminalColors::Red, sizeof(TerminalColors::Red));
        cout << "(undocumented)";
      }
      cout.write(TerminalColors::Reset, sizeof(TerminalColors::Reset));
      cout << endl;
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
    if (std::find(keys.begin(), keys.end(), k) == keys.end()) {
      throw(
          std::runtime_error("PipeTestParser::displayKeyWordDescription: "
                             "unknown keyword '" +
                             k + "'"));
    }
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
    this->registerCallBack("@InnerPressureEvolution",
                           &PipeTestParser::handleInnerPressureEvolution);
    this->registerCallBack("@OuterPressureEvolution",
                           &PipeTestParser::handleOuterPressureEvolution);
    this->registerCallBack("@AxialForceEvolution",
                           &PipeTestParser::handleAxialForceEvolution);
    this->registerCallBack("@AxialGrowthEvolution",
                           &PipeTestParser::handleAxialGrowthEvolution);
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
    } else if (h == "ImposedOuterRadius") {
      t.setRadialLoading(mtest::PipeTest::IMPOSEDOUTERRADIUS);
    } else {
      throw(
          std::runtime_error("PipeTestParser::handleRadialLoading: "
                             "invalid loading type ('" +
                             h +
                             "').\n"
                             "Valid modelling hypothesis are "
                             "'ImposedPressure' and 'ImposedOuterRadius'"));
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
      throw(
          std::runtime_error("PipeTestParser::handleAxialLoading: "
                             "invalid pipe modelling hypothesis ('" +
                             h +
                             "').\n"
                             "Valid modelling hypothesis are "
                             "'None', 'ImposedAxialGrowth', "
                             "'ImposedAxialForce' and 'EndCapEffect'"));
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
      throw(
          std::runtime_error("PipeTest::setElementType: "
                             "invalid element type ('" +
                             e +
                             "').\n"
                             "Valid element type are "
                             "'Linear' and 'Quadratic'"));
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
      throw(std::runtime_error(
          "PipeTestParser::handlePerformSmallStrainAnalysis: "
          "unexpected token '" +
          p->value + "'"));
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
          if (p->value == "}") {
            throw(
                std::runtime_error("PipeTestParser::handleProfile: "
                                   "unexpected token '}'"));
          }
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
    bool profile = false;
    if (type != "file") {
      throw(
          std::runtime_error("PipeTestParser::handleTest: "
                             "invalid test type '" +
                             type + "'"));
    }
    ++p;
    if (p->value == ",") {
      ++p;
      this->checkNotEndOfLine("PipeTestParser::handleTest", p,
                              this->tokens.end());
      if (p->value == "integral") {
        profile = false;
      } else if (p->value == "profile") {
        profile = true;
      } else {
        throw(
            std::runtime_error("PipeTestParser::handleTest: "
                               "invalid test option '" +
                               p->value + "'"));
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
        const unsigned int c = this->readUnsignedInt(p, this->tokens.end());
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
            if (p->value == "}") {
              throw(
                  std::runtime_error("PipeTestParser::handleTest: "
                                     "unexpected token '}'"));
            }
          }
        }
        this->readSpecifiedToken("PipeTestParser::handleTest", "}", p,
                                 this->tokens.end());
      }
      const real eps = this->readDouble(t, p);
      if (eps < 0) {
        throw(
            std::runtime_error("PipeTestParser::handleTest: "
                               "invalid criterion value"));
      }
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
    } else {
      throw(
          std::runtime_error("PipeTestParser::handleTest: "
                             "invalid test type '" +
                             type + "'"));
    }
    this->readSpecifiedToken("PipeTestParser::handleTest", ";", p,
                             this->tokens.end());
  }  // end of PipeTestParser::handleTest

  void PipeTestParser::handleAdditionalOutputs(PipeTest& t,
                                               tokens_iterator& p) {
    using tfel::utilities::Data;
    this->checkNotEndOfLine("PipeTestParser::handleAdditionalOutputs", p,
                            this->tokens.end());
    auto handle_min = [&t](const Data& v) {
      if (v.is<std::string>()) {
        t.addOutput("minimum_value", v.get<std::string>());
      } else {
        for (const auto& f :
             tfel::utilities::extract<std::vector<std::string>>(v)) {
          t.addOutput("minimum_value", f);
        }
      }
    };
    auto handle_max = [&t](const Data& v) {
      if (v.is<std::string>()) {
        t.addOutput("maximum_value", v.get<std::string>());
      } else {
        for (const auto& f :
             tfel::utilities::extract<std::vector<std::string>>(v)) {
          t.addOutput("maximum_value", f);
        }
      }
    };
    tfel::utilities::Data::parse(p, this->tokens.end(),
                                 {{"minimum_value", handle_min},
                                  {"minimum_values", handle_min},
                                  {"maximum_value", handle_max},
                                  {"maximum_values", handle_max}});
    this->readSpecifiedToken("PipeTestParser::handleAdditionalOutputs", ";", p,
                             this->tokens.end());
  }  // end of PipeTestParser::handleAdditionalOutput

  PipeTestParser::~PipeTestParser() = default;

}  // end namespace mtest
