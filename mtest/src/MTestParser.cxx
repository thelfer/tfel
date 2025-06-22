/*!
 * \file   mtest/src/MTestParser.cxx
 * \brief
 * \author Thomas Helfer
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
#if defined _WIN32 || defined _WIN64
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#ifdef small
#undef small
#endif /* small */
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <map>
#include <cmath>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <cstdlib>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Utilities/TerminalColors.hxx"
#include "TFEL/Utilities/GenTypeBase.hxx"
#include "MFront/MFrontLogStream.hxx"

#include "MTest/AnalyticalTest.hxx"
#include "MTest/ReferenceFileComparisonTest.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/Constraint.hxx"
#include "MTest/NonLinearConstraint.hxx"
#include "MTest/ImposedThermodynamicForce.hxx"
#include "MTest/ImposedGradient.hxx"
#include "MTest/MTestParser.hxx"

namespace mtest {

  MTestParser::MTestParser() { this->registerCallBacks(); }

  void MTestParser::parseString(MTest& t, const std::string& f) {
    this->file = "user defined string";
    this->treatCharAsString(true);
    CxxTokenizer::parseString(f);
    this->stripComments();
    this->execute(t);
  }  // end of MTestParser::parseString

  void MTestParser::execute(MTest& t,
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
        tfel::raise(
            "MTestParser::execute : "
            "error while parsing external command "
            "'" +
            c + "'\n" + std::string(e.what()));
      }
      this->tokens.insert(this->tokens.begin(), tokenizer.begin(),
                          tokenizer.end());
    }
    this->execute(t);
  }

  void MTestParser::execute(MTest& t) {
    auto p = this->tokens.cbegin();
    while (p != tokens.end()) {
      if (MTestParser::treatKeyword(t, p)) {
        continue;
      }
      if (SingleStructureSchemeParser::treatKeyword(t, p)) {
        continue;
      }
      if (SchemeParserBase::treatKeyword(t, p)) {
        continue;
      }
      std::ostringstream msg;
      msg << "MTestParser::execute : invalid keyword '" << p->value
          << "'. Error at line " << p->line << ".";
      tfel::raise(msg.str());
    }
  }  // end of MTestParser::execute

  bool MTestParser::treatKeyword(MTest& t, tokens_iterator& p) {
    const auto pc = this->callbacks.find(p->value);
    if (pc == this->callbacks.end()) {
      return false;
    }
    if (mfront::getVerboseMode() >= mfront::VERBOSE_DEBUG) {
      auto& log = mfront::getLogStream();
      log << "MTestParser::execute : treating keyword '" << p->value
          << "' at line '" << p->line << "'\n";
    }
    ++p;
    const auto line = p->line;
    try {
      (this->*(pc->second))(t, p);
    } catch (std::exception& e) {
      std::ostringstream msg;
      msg << "MTestParser::MTest : error while "
          << "parsing file '" << this->file << "' at line '" << line << "'.\n"
          << e.what();
      tfel::raise(msg.str());
    }
    return true;
  }  // end of MTestParser::treatKeyword

  std::vector<std::string> MTestParser::getKeyWordsList() const {
    auto keys = SchemeParserBase::getKeyWordsList();
    auto keys2 = SingleStructureSchemeParser::getKeyWordsList();
    keys.insert(keys.end(), keys2.begin(), keys2.end());
    for (const auto& c : this->callbacks) {
      keys.push_back(c.first);
    }
    return keys;
  }

  void MTestParser::displayKeyWordsList() const {
    using namespace std;
    using namespace tfel::utilities;
    auto keys = this->getKeyWordsList();
    string::size_type msize = 0;
    for (const auto& k : keys) {
      msize = max(msize, k.size());
    }
    for (const auto& k : keys) {
      const auto f = SchemeParserBase::getDocumentationFilePath("mtest", k);
      string key = k;
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
  }  // end of MTestParser::displayKeywordsList

  void MTestParser::displayKeyWordsHelp() const {
    auto keys = this->getKeyWordsList();
    std::cout << "% `MTest` keywords\n\n";
    for (const auto& k : keys) {
      const auto f = SchemeParserBase::getDocumentationFilePath("mtest", k);
      std::cout << "\n# The `" << k << "` keyword\n\n";
      if (!f.empty()) {
        std::ifstream desc{f};
        if (desc) {
          std::cout << desc.rdbuf();
        } else {
          std::cout << "The keyword `" << k << "` is not documented yet\n";
        }
      } else {
        std::cout << "The keyword `" << k << "` is not documented yet\n";
      }
    }
  }  // end of MTestParser::displayKeywordsHelp

  void MTestParser::displayKeyWordDescription(const std::string& k) const {
    auto keys = this->getKeyWordsList();
    tfel::raise_if(std::find(keys.begin(), keys.end(), k) == keys.end(),
                   "MTestParser::displayKeyWordDescription: "
                   "unknown keyword '" +
                       k + "'");
    const auto f = SchemeParserBase::getDocumentationFilePath("mtest", k);
    if (f.empty()) {
      std::cout << "no description available for keyword '" << k << "'"
                << std::endl;
      return;
    }
    std::ifstream desc{f};
    if (!desc) {
      std::cout << "no description available for keyword '" << k << "'"
                << std::endl;
      return;
    }
    std::cout << desc.rdbuf();
  }

  void MTestParser::registerCallBacks() {
    SchemeParserBase::registerCallBacks();
    SingleStructureSchemeParser::registerCallBacks();
    auto add = [this](const char* n, const CallBack& c) {
      this->registerCallBack(n, c);
    };
    add("@Event", &MTestParser::handleEvent);
    add("@Test", &MTestParser::handleTest);
    add("@RotationMatrix", &MTestParser::handleRotationMatrix);
    add("@StrainEpsilon", &MTestParser::handleStrainEpsilon);
    add("@DeformationGradientEpsilon",
        &MTestParser::handleDeformationGradientEpsilon);
    add("@OpeningDisplacementEpsilon",
        &MTestParser::handleOpeningDisplacementEpsilon);
    add("@DrivingVariableEpsilon", &MTestParser::handleGradientEpsilon);
    add("@GradientEpsilon", &MTestParser::handleGradientEpsilon);
    add("@StressEpsilon", &MTestParser::handleStressEpsilon);
    add("@CohesiveForceEpsilon", &MTestParser::handleCohesiveForceEpsilon);
    add("@ThermodynamicForceEpsilon",
        &MTestParser::handleThermodynamicForceEpsilon);
    add("@ModellingHypothesis", &MTestParser::handleModellingHypothesis);
    add("@Strain", &MTestParser::handleStrain);
    add("@OpeningDisplacement", &MTestParser::handleOpeningDisplacement);
    add("@DrivingVariable", &MTestParser::handleGradient);
    add("@Gradient", &MTestParser::handleGradient);
    add("@DeformationGradient", &MTestParser::handleDeformationGradient);
    add("@Stress", &MTestParser::handleStress);
    add("@CohesiveForce", &MTestParser::handleCohesiveForce);
    add("@ThermodynamicForce", &MTestParser::handleThermodynamicForce);
    add("@ImposedStrain", &MTestParser::handleImposedStrain);
    add("@ImposedOpeningDisplacement",
        &MTestParser::handleImposedOpeningDisplacement);
    add("@ImposedDeformationGradient",
        &MTestParser::handleImposedDeformationGradient);
    add("@ImposedDrivingVariable", &MTestParser::handleImposedGradient);
    add("@ImposedGradient", &MTestParser::handleImposedGradient);
    add("@ImposedStress", &MTestParser::handleImposedStress);
    add("@ImposedCohesiveForce", &MTestParser::handleImposedCohesiveForce);
    add("@ImposedThermodynamicForce",
        &MTestParser::handleImposedThermodynamicForce);
    add("@NonLinearConstraint", &MTestParser::handleNonLinearConstraint);
    add("@CompareToNumericalTangentOperator",
        &MTestParser::handleCompareToNumericalTangentOperator);
    add("@TangentOperatorComparisonCriterium",
        &MTestParser::handleTangentOperatorComparisonCriterium);
    add("@NumericalTangentOperatorPerturbationValue",
        &MTestParser::handleNumericalTangentOperatorPerturbationValue);
    add("@UserDefinedPostProcessing",
        &MTestParser::handleUserDefinedPostProcessing);
  }

  void MTestParser::registerCallBack(const std::string& k,
                                     const MTestParser::CallBack& p) {
    this->callbacks.insert({k, p});
  }

  void MTestParser::handleEvent(MTest& t, tokens_iterator& p) {
    const auto n = this->readString(p, this->tokens.end());
    std::vector<double> evt;
    this->checkNotEndOfLine("MTestParser::handleEvent", p, this->tokens.end());
    if (p->value == "{") {
      evt = this->readTimesArray("MTest::handleEvent", t, p);
    } else {
      evt.push_back(this->readDouble(t, p));
    }
    t.addEvent(n, evt);
    this->readSpecifiedToken("MTestParser::handleEvent", ";", p,
                             this->tokens.end());
  }  // end of MTestParser::handleEvent

  void MTestParser::handleCompareToNumericalTangentOperator(
      MTest& t, tokens_iterator& p) {
    this->checkNotEndOfLine("handleCompareToNumericalTangentOperator", p,
                            this->tokens.end());
    if (p->value == "true") {
      t.setCompareToNumericalTangentOperator(true);
    } else if (p->value == "false") {
      t.setCompareToNumericalTangentOperator(false);
    } else {
      tfel::raise(
          "MTestParser::handleCompareToNumericalTangentOperator: "
          "unexpected value (expected 'true' or 'false', "
          "read '" +
          p->value + "')");
    }
    ++p;
    this->checkNotEndOfLine("handleCompareToNumericalTangentOperator", p,
                            this->tokens.end());
    this->readSpecifiedToken(
        "MTestParser::handleCompareToNumericalTangentOperator", ";", p,
        this->tokens.end());
  }  // end of MTestParser::handleCompareToNumericalTangentOperator

  void MTestParser::handleTangentOperatorComparisonCriterium(
      MTest& t, tokens_iterator& p) {
    this->checkNotEndOfLine("handleTangentOperatorComparisonCriterium", p,
                            this->tokens.end());
    t.setTangentOperatorComparisonCriterium(this->readDouble(t, p));
    this->checkNotEndOfLine("handleTangentOperatorComparisonCriterium", p,
                            this->tokens.end());
    this->readSpecifiedToken(
        "MTestParser::handleTangentOperatorComparisonCriterium", ";", p,
        this->tokens.end());
  }  // end of MTestParser::handleTangentOperatorComparisonCriterium

  void MTestParser::handleNumericalTangentOperatorPerturbationValue(
      MTest& t, tokens_iterator& p) {
    this->checkNotEndOfLine("handleNumericalTangentOperatorPerturbationValue",
                            p, this->tokens.end());
    t.setNumericalTangentOperatorPerturbationValue(this->readDouble(t, p));
    this->checkNotEndOfLine("handleNumericalTangentOperatorPerturbationValue",
                            p, this->tokens.end());
    this->readSpecifiedToken(
        "MTestParser::handleNumericalTangentOperatorPerturbationValue", ";", p,
        this->tokens.end());
  }  // end of MTestParser::handleNumericalTangentOperatorPerturbationValue

  void MTestParser::handleTest(MTest& t, tokens_iterator& p) {
    using namespace std;
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "MTestParser::handleTest: " + m);
    };
    this->readSpecifiedToken("MTestParser::handleTest", "<", p,
                             this->tokens.end());
    this->checkNotEndOfLine("MTestParser::handleTest", p, this->tokens.end());
    const auto& type = p->value;
    throw_if((type != "function") && (type != "file"),
             "invalid test type '" + type + "'");
    ++p;
    this->readSpecifiedToken("MTestParser::handleTest", ">", p,
                             this->tokens.end());
    if (type == "function") {
      this->checkNotEndOfLine("MTestParser::handleTest", p, this->tokens.end());
      map<string, string> functions;
      if (p->flag == tfel::utilities::Token::String) {
        const auto& v = this->readString(p, this->tokens.end());
        const auto& f = this->readString(p, this->tokens.end());
        functions.insert({v, f});
      } else {
        this->readSpecifiedToken("MTestParser::handleTest", "{", p,
                                 this->tokens.end());
        this->checkNotEndOfLine("MTestParser::handleTest", p,
                                this->tokens.end());
        while (p->value != "}") {
          const auto& v = this->readString(p, this->tokens.end());
          this->readSpecifiedToken("MTestParser::handleTest", ":", p,
                                   this->tokens.end());
          const auto& f = this->readString(p, this->tokens.end());
          functions.insert({v, f});
          this->checkNotEndOfLine("MTestParser::handleTest", p,
                                  this->tokens.end());
          if (p->value != "}") {
            this->readSpecifiedToken("MTestParser::handleTest", ",", p,
                                     this->tokens.end());
            this->checkNotEndOfLine("MTestParser::handleTest", p,
                                    this->tokens.end());
            throw_if(p->value == "}", "unexpected token '}'");
          }
        }
        this->readSpecifiedToken("MTestParser::handleTest", "}", p,
                                 this->tokens.end());
      }
      const auto eps = this->readDouble(t, p);
      throw_if(eps < 0, "invalid criterion value");
      for (const auto& f : functions) {
        const auto& g = buildValueExtractor(*(t.getBehaviour()), f.first);
        auto test = std::make_shared<AnalyticalTest>(f.second, f.first, g,
                                                     t.getEvolutions(), eps);
        t.addTest(test);
      }
    } else if (type == "file") {
      const auto& f = this->readString(p, this->tokens.end());
      this->checkNotEndOfLine("MTestParser::handleTest", p, this->tokens.end());
      using gentype = tfel::utilities::GenType<std::string, unsigned short>;
      map<string, gentype> columns;
      if (p->flag == tfel::utilities::Token::String) {
        const auto& v = this->readString(p, this->tokens.end());
        const unsigned short c = this->readUnsignedInt(p, this->tokens.end());
        columns.insert({v, gentype(c)});
      } else {
        this->readSpecifiedToken("MTestParser::handleTest", "{", p,
                                 this->tokens.end());
        this->checkNotEndOfLine("MTestParser::handleTest", p,
                                this->tokens.end());
        while (p->value != "}") {
          const auto& v = this->readString(p, this->tokens.end());
          this->readSpecifiedToken("MTestParser::handleTest", ":", p,
                                   this->tokens.end());
          if (p->flag == tfel::utilities::Token::String) {
            columns.insert({v, p->value.substr(1, p->value.size() - 2)});
            ++p;
          } else {
            const unsigned short c =
                this->readUnsignedInt(p, this->tokens.end());
            columns.insert({v, gentype(c)});
          }
          this->checkNotEndOfLine("MTestParser::handleTest", p,
                                  this->tokens.end());
          if (p->value != "}") {
            this->readSpecifiedToken("MTestParser::handleTest", ",", p,
                                     this->tokens.end());
            this->checkNotEndOfLine("MTestParser::handleTest", p,
                                    this->tokens.end());
            throw_if(p->value == "}", "unexpected token '}'");
          }
        }
        this->readSpecifiedToken("MTestParser::handleTest", "}", p,
                                 this->tokens.end());
      }
      const real eps = this->readDouble(t, p);
      throw_if(eps < 0, "invalid criterion value");
      auto data = std::make_shared<tfel::utilities::TextData>(f);
      for (const auto& c : columns) {
        const auto& g = buildValueExtractor(*(t.getBehaviour()), c.first);
        if (c.second.is<unsigned short>()) {
          const auto cn = static_cast<unsigned short>(c.second);
          t.addTest(std::make_shared<ReferenceFileComparisonTest>(
              *data, cn, c.first, g, eps));
        } else {
          const auto ef = static_cast<std::string>(c.second);
          t.addTest(std::make_shared<ReferenceFileComparisonTest>(
              *data, t.getEvolutions(), ef, c.first, g, eps));
        }
      }
    } else {
      throw_if(true, "invalid test type '" + type + "'");
    }
    this->readSpecifiedToken("MTestParser::handleTest", ";", p,
                             this->tokens.end());
  }  // end of MTestParser::handleTest

  void MTestParser::handleRotationMatrix(MTest& t, tokens_iterator& p) {
    auto from_euler = [](tfel::math::tmatrix<3, 3, real>& r, const real psi,
                         const real the, const real phi) {
      const real cospsi = std::cos(psi);
      const real costhe = std::cos(the);
      const real cosphi = std::cos(phi);
      const real sinpsi = std::sin(psi);
      const real sinthe = std::sin(the);
      const real sinphi = std::sin(phi);
      r(0, 0) = cosphi * cospsi - sinphi * costhe * sinpsi;
      r(0, 1) = cosphi * sinpsi + sinphi * costhe * cospsi;
      r(0, 2) = sinphi * sinthe;
      r(1, 0) = -sinphi * cospsi - cosphi * costhe * sinpsi;
      r(1, 1) = -sinphi * sinpsi + cosphi * costhe * cospsi;
      r(1, 2) = cosphi * sinthe;
      r(2, 0) = sinthe * sinpsi;
      r(2, 1) = -sinthe * cospsi;
      r(2, 2) = costhe;
    };
    enum { STANDARD, EULER, MILLER } choice = STANDARD;
    this->checkNotEndOfLine("MTestParser::handleRotationMatrix", p,
                            this->tokens.end());
    if (p->value == "<") {
      ++p;
      this->checkNotEndOfLine("MTestParser::handleRotationMatrix", p,
                              this->tokens.end());
      if (p->value == "Standard") {
        choice = STANDARD;
      } else if (p->value == "Euler") {
        choice = EULER;
      } else if (p->value == "Miller") {
        choice = MILLER;
      } else {
        tfel::raise(
            "MTestParser::handleRotationMatrix: "
            "unsupported roation matrix type");
      }
      ++p;
      this->readSpecifiedToken("MTestParser::handleRotationMatrix", ">", p,
                               this->tokens.end());
      this->checkNotEndOfLine("MTestParser::handleRotationMatrix", p,
                              this->tokens.end());
    }
    // saving the read values
    tfel::math::tmatrix<3u, 3u, real> rm;
    if (choice == MILLER) {
      this->readSpecifiedToken("MTestParser::handleRotationMatrix", "{", p,
                               this->tokens.end());
      const auto h = this->readInt(p, this->tokens.end());
      this->readSpecifiedToken("MTestParser::handleRotationMatrix", ",", p,
                               this->tokens.end());
      const auto k = this->readInt(p, this->tokens.end());
      this->readSpecifiedToken("MTestParser::handleRotationMatrix", ",", p,
                               this->tokens.end());
      const auto l = this->readInt(p, this->tokens.end());
      this->readSpecifiedToken("MTestParser::handleRotationMatrix", "}", p,
                               this->tokens.end());
      if ((h == 0) && (k == 0) && (l == 0)) {
        throw(
            std::runtime_error("MTestParser::handleRotationMatrix: "
                               "invalide Miller indices"));
      }
      const auto s = std::sqrt(static_cast<real>(h * h + k * k + l * l));
      const auto nx = static_cast<real>(h) / s;
      const auto ny = static_cast<real>(k) / s;
      const auto nz = static_cast<real>(l) / s;
      real cx, sx, sq;
      std::tie(cx, sx, sq) = [&]() -> std::tuple<real, real, real> {
        if ((h == 0) && (k == 0)) {
          return std::tuple<real, real, real>{1, 0, 0};
        }
        const auto tsq = std::sqrt(nx * nx + ny * ny);
        return std::tuple<real, real, real>{ny / tsq, nx / tsq, tsq};
      }();
      rm = tfel::math::tmatrix<3u, 3u, real>{cx,      -sx,     real(0),  //
                                             nz * sx, nz * cx, -sq,      //
                                             nx,      ny,      nz};
    } else if (choice == EULER) {
      std::vector<real> v(3);
      this->checkNotEndOfLine("MTestParser::handleRotationMatrix", p,
                              this->tokens.end());
      auto np = std::next(p);
      this->checkNotEndOfLine("MTestParser::handleRotationMatrix", np,
                              this->tokens.end());
      const auto b = np->value == "{";
      if (b) {
        auto& log = mfront::getLogStream();
        log << "MTestParser::handleRotationMatrix: using two curly brackets is "
            << "deprecated. Refer to https://github.com/thelfer/tfel/issues/16 "
            << "for details\n";
        this->readSpecifiedToken("MTestParser::handleRotationMatrix", "{", p,
                                 this->tokens.end());
      }
      this->readArrayOfSpecifiedSize(v, t, p);
      if (b) {
        this->readSpecifiedToken("MTestParser::handleRotationMatrix", "}", p,
                                 this->tokens.end());
      }
      from_euler(rm, v[0], v[1], v[2]);
    } else {
      // standard choice
      this->readSpecifiedToken("MTestParser::handleRotationMatrix", "{", p,
                               this->tokens.end());
      std::vector<std::vector<real>> v(3);
      for (unsigned short i = 0; i != 3;) {
        v[i].resize(3);
        this->readArrayOfSpecifiedSize(v[i], t, p);
        if (++i != 3) {
          this->readSpecifiedToken("MTestParser::handleRotationMatrix", ",", p,
                                   this->tokens.end());
        }
      }
      this->readSpecifiedToken("MTestParser::handleRotationMatrix", "}", p,
                               this->tokens.end());
      for (unsigned short i = 0; i != 3; ++i) {
        for (unsigned short j = 0; j != 3; ++j) {
          rm(i, j) = v[i][j];
        }
      }
    }
    this->readSpecifiedToken("MTestParser::handleRotationMatrix", ";", p,
                             this->tokens.end());
    t.setRotationMatrix(rm);
  }  // end of MTestParser::handleRotationMatrix

  void MTestParser::handleStrainEpsilon(MTest& t, tokens_iterator& p) {
    using namespace tfel::material;
    tfel::raise_if(
        !((t.getBehaviourType() ==
           MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) ||
          ((t.getBehaviourType() ==
            MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) &&
           (t.getBehaviourKinematic() ==
            MechanicalBehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1))),
        "MTestParser::handleStrainEpsilon: "
        "the @StrainEpsilon keyword is only valid "
        "for small strain behaviours");
    this->handleGradientEpsilon(t, p);
  }

  void MTestParser::handleDeformationGradientEpsilon(MTest& t,
                                                     tokens_iterator& p) {
    using namespace tfel::material;
    tfel::raise_if(!((t.getBehaviourType() ==
                      MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) &&
                     (t.getBehaviourKinematic() ==
                      MechanicalBehaviourBase::FINITESTRAINKINEMATIC_F_CAUCHY)),
                   "MTestParser::handleDeformationGradientEpsilon: "
                   "the @DeformationGradientEpsilon keyword is only valid "
                   "for finite strain behaviours");
    this->handleGradientEpsilon(t, p);
  }  // end of MTestParser::handleDeformationGradientEpsilon

  void MTestParser::handleOpeningDisplacementEpsilon(MTest& t,
                                                     tokens_iterator& p) {
    using namespace tfel::material;
    tfel::raise_if(
        t.getBehaviourType() != MechanicalBehaviourBase::COHESIVEZONEMODEL,
        "MTestParser::handleOpeningDisplacementEpsilon: "
        "the @OpeningDisplacementEpsilon keyword is only valid "
        "for cohesive zone model behaviours");
    this->handleGradientEpsilon(t, p);
  }

  void MTestParser::handleGradientEpsilon(MTest& t, tokens_iterator& p) {
    t.setGradientEpsilon(this->readDouble(t, p));
    this->readSpecifiedToken("MTestParser::handleGradientEpsilon", ";", p,
                             this->tokens.end());
  }  // end of MTestParser::handleGradientEpsilon

  void MTestParser::handleStressEpsilon(MTest& t, tokens_iterator& p) {
    using namespace tfel::material;
    tfel::raise_if((t.getBehaviourType() !=
                    MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) &&
                       (t.getBehaviourType() !=
                        MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR),
                   "MTestParser::handleStressEpsilon: "
                   "the @StressEpsilon keyword is only valid "
                   "for small strain behaviours");
    this->handleThermodynamicForceEpsilon(t, p);
  }

  void MTestParser::handleCohesiveForceEpsilon(MTest& t, tokens_iterator& p) {
    using namespace tfel::material;
    tfel::raise_if(
        t.getBehaviourType() != MechanicalBehaviourBase::COHESIVEZONEMODEL,
        "MTestParser::handleCohesiveForceEpsilon: "
        "the @CohesiveForceEpsilon keyword is only valid "
        "for cohesive zone model behaviours");
    this->handleThermodynamicForceEpsilon(t, p);
  }

  void MTestParser::handleThermodynamicForceEpsilon(MTest& t,
                                                    tokens_iterator& p) {
    t.setThermodynamicForceEpsilon(this->readDouble(t, p));
    this->readSpecifiedToken("MTestParser::handleThermodynamicForceEpsilon",
                             ";", p, this->tokens.end());
  }

  void MTestParser::handleModellingHypothesis(MTest& t, tokens_iterator& p) {
    t.setModellingHypothesis(this->readString(p, this->tokens.end()));
    this->readSpecifiedToken("MTestParser::handleModellingHypothesis", ";", p,
                             this->tokens.end());
  }

  void MTestParser::handleImposedStress(MTest& t, tokens_iterator& p) {
    using namespace tfel::material;
    tfel::raise_if((t.getBehaviourType() !=
                    MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) &&
                       (t.getBehaviourType() !=
                        MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR),
                   "MTestParser::handleImposedStress: "
                   "the @ImposedStress keyword is only valid "
                   "for standard behaviours");
    this->handleImposedThermodynamicForce(t, p);
  }  // end of MTestParser::handleImposedStress

  void MTestParser::handleImposedCohesiveForce(MTest& t, tokens_iterator& p) {
    using namespace tfel::material;
    tfel::raise_if(
        t.getBehaviourType() != MechanicalBehaviourBase::COHESIVEZONEMODEL,
        "MTestParser::handleImposedCohesiveForce: "
        "the @ImposedCohesiveForce keyword is only valid "
        "for cohesive zone model behaviours");
    this->handleImposedThermodynamicForce(t, p);
  }  // end of MTestParser::handleImposedCohesiveForce

  void MTestParser::handleImposedThermodynamicForce(MTest& t,
                                                    tokens_iterator& p) {
    const auto& evt = this->readEvolutionType(p);
    const auto& c = this->readString(p, this->tokens.end());
    this->checkNotEndOfLine("MTestParser::handleImposedThermodynamicForce", p,
                            this->tokens.end());
    auto sev = this->parseEvolution(t, evt, p);
    const auto opts = this->readConstraintOptions(
        "MTestParser::handleNonLinearConstraint", p);
    this->readSpecifiedToken("MTestParser::handleImposedThermodynamicForce",
                             ";", p, this->tokens.end());
    auto sc = std::make_shared<ImposedThermodynamicForce>(*(t.getBehaviour()),
                                                          c, sev);
    applyConstraintOptions(*(sc.get()), opts);
    t.addEvolution(c, sev, false, true);
    t.addConstraint(sc);
  }  // end of MTestParser::handleImposedThermodynamicForce

  void MTestParser::handleNonLinearConstraint(MTest& t, tokens_iterator& p) {
    using namespace tfel::material;
    NonLinearConstraint::NormalisationPolicy np;
    this->readSpecifiedToken("MTestParser::handleNonLinearConstraint", "<", p,
                             this->tokens.end());
    this->checkNotEndOfLine("MTestParser::handleNonLinearConstraint", p,
                            this->tokens.end());
    if ((p->value == "Gradient") ||
        ((p->value == "Strain") &&
         (t.getBehaviourType() ==
          MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR)) ||
        ((p->value == "DeformationGradient") &&
         (t.getBehaviourType() ==
          MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR)) ||
        ((p->value == "OpeningDisplacement") &&
         (t.getBehaviourType() ==
          MechanicalBehaviourBase::COHESIVEZONEMODEL))) {
      np = NonLinearConstraint::DRIVINGVARIABLECONSTRAINT;
    } else if ((p->value == "ThermodynamicForce") ||
               ((p->value == "Stress") &&
                ((t.getBehaviourType() ==
                  MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) ||
                 (t.getBehaviourType() ==
                  MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR))) ||
               ((p->value == "CohesiveForce") &&
                (t.getBehaviourType() ==
                 MechanicalBehaviourBase::COHESIVEZONEMODEL))) {
      np = NonLinearConstraint::THERMODYNAMICFORCECONSTRAINT;
    } else {
      tfel::raise(
          "MTestParser::handleNonLinearConstraint: "
          "invalid normalisation policy '" +
          p->value + "'");
    }
    ++p;
    this->readSpecifiedToken("MTestParser::handleNonLinearConstraint", ">", p,
                             this->tokens.end());
    const auto& c = this->readString(p, this->tokens.end());
    const auto opts = this->readConstraintOptions(
        "MTestParser::handleNonLinearConstraint", p);
    this->readSpecifiedToken("MTestParser::handleNonLinearConstraint", ";", p,
                             this->tokens.end());
    auto sc = std::make_shared<NonLinearConstraint>(*(t.getBehaviour()), c,
                                                    t.getEvolutions(), np);
    applyConstraintOptions(*(sc.get()), opts);
    t.addConstraint(sc);
  }  // end of MTestParser::handleNonLinearConstraint

  void MTestParser::handleImposedStrain(MTest& t, tokens_iterator& p) {
    using namespace tfel::material;
    tfel::raise_if(
        !((t.getBehaviourType() ==
           MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) ||
          ((t.getBehaviourType() ==
            MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) &&
           (t.getBehaviourKinematic() ==
            MechanicalBehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1))),
        "MTestParser::handleImposedStrain: "
        "the @ImposedStrain keyword is only valid "
        "for small strain behaviours");
    this->handleImposedGradient(t, p);
  }

  void MTestParser::handleImposedDeformationGradient(MTest& t,
                                                     tokens_iterator& p) {
    using namespace tfel::material;
    tfel::raise_if(t.getBehaviourType() !=
                       MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR,
                   "MTestParser::handleImposedDeformationGradient: "
                   "the @ImposedDeformationGradient keyword is only valid "
                   "for finite strain behaviours");
    tfel::raise_if(t.getBehaviourKinematic() !=
                       MechanicalBehaviourBase::FINITESTRAINKINEMATIC_F_CAUCHY,
                   "MTestParser::handleImposedDeformationGradient: "
                   "the @ImposedDeformationGradient keyword is only valid "
                   "invalid finite strain kinematic");
    this->handleImposedGradient(t, p);
  }

  void MTestParser::handleImposedOpeningDisplacement(MTest& t,
                                                     tokens_iterator& p) {
    using namespace tfel::material;
    tfel::raise_if(
        t.getBehaviourType() != MechanicalBehaviourBase::COHESIVEZONEMODEL,
        "MTestParser::ImposedOpeningDisplacement: "
        "the @ImposedOpeningDisplacement keyword is only valid "
        "for cohesive zone model behaviours");
    this->handleImposedGradient(t, p);
  }

  void MTestParser::handleImposedGradient(MTest& t, tokens_iterator& p) {
    const auto& evt = this->readEvolutionType(p);
    const auto& c = this->readString(p, this->tokens.end());
    this->checkNotEndOfLine("MTestParser::handleImposedGradient", p,
                            this->tokens.end());
    auto sev = this->parseEvolution(t, evt, p);
    const auto opts =
        this->readConstraintOptions("MTestParser::handleImposedGradient", p);
    this->readSpecifiedToken("MTestParser::handleImposedGradient", ";", p,
                             this->tokens.end());
    auto sc = std::make_shared<ImposedGradient>(*(t.getBehaviour()), c, sev);
    applyConstraintOptions(*(sc.get()), opts);
    t.addConstraint(sc);
    t.addEvolution(c, sev, false, true);
  }  // end of MTestParser::handleImposedGradient

  void MTestParser::handleStrain(MTest& t, tokens_iterator& p) {
    using namespace tfel::material;
    tfel::raise_if(
        !((t.getBehaviourType() ==
           MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) ||
          ((t.getBehaviourType() ==
            MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) &&
           (t.getBehaviourKinematic() ==
            MechanicalBehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1))),
        "MTestParser::handleStrain: "
        "the @Strain keyword is only valid "
        "for small strain behaviours");
    this->handleGradient(t, p);
  }

  void MTestParser::handleDeformationGradient(MTest& t, tokens_iterator& p) {
    using namespace tfel::material;
    tfel::raise_if(!((t.getBehaviourType() ==
                      MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) &&
                     (t.getBehaviourKinematic() ==
                      MechanicalBehaviourBase::FINITESTRAINKINEMATIC_F_CAUCHY)),
                   "MTestParser::handleDeformationGradient: "
                   "the @DeformationGradient keyword is only valid "
                   "for finite strain behaviours");
    this->handleGradient(t, p);
  }

  void MTestParser::handleOpeningDisplacement(MTest& t, tokens_iterator& p) {
    using namespace tfel::material;
    tfel::raise_if(t.getBehaviour()->getBehaviourType() !=
                       MechanicalBehaviourBase::COHESIVEZONEMODEL,
                   "MTestParser::handleOpeningDisplacement: "
                   "the @OpeningDisplacement keyword is only valid "
                   "for cohesive zone models behaviours");
    this->handleGradient(t, p);
  }

  void MTestParser::handleGradient(MTest& t, tokens_iterator& p) {
    const auto N = t.getBehaviour()->getGradientsSize();
    std::vector<real> e_t0;
    e_t0.resize(N, 0);
    this->readArrayOfSpecifiedSize(e_t0, t, p);
    this->readSpecifiedToken("MTestParser::handleGradient", ";", p,
                             this->tokens.end());
    t.setGradientsInitialValues(e_t0);
  }  // end of MTestParser::handleGradient

  void MTestParser::handleStress(MTest& t, tokens_iterator& p) {
    using namespace tfel::material;
    tfel::raise_if((t.getBehaviour()->getBehaviourType() !=
                    MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) &&
                       (t.getBehaviour()->getBehaviourType() !=
                        MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR),
                   "MTestParser::handleStress: "
                   "the @Stress keyword is only valid "
                   "for small strain behaviours");
    this->handleThermodynamicForce(t, p);
  }

  void MTestParser::handleCohesiveForce(MTest& t, tokens_iterator& p) {
    using namespace tfel::material;
    tfel::raise_if(t.getBehaviour()->getBehaviourType() !=
                       MechanicalBehaviourBase::COHESIVEZONEMODEL,
                   "MTestParser::handleCohesiveForce: "
                   "the @CohesiveForce keyword is only valid "
                   "for cohesive zone model behaviours");
    this->handleThermodynamicForce(t, p);
  }

  void MTestParser::handleThermodynamicForce(MTest& t, tokens_iterator& p) {
    const auto N = t.getBehaviour()->getThermodynamicForcesSize();
    std::vector<real> s_t0;
    s_t0.resize(N, 0);
    this->readArrayOfSpecifiedSize(s_t0, t, p);
    this->readSpecifiedToken("MTestParser::handleThermodynamicForce", ";", p,
                             this->tokens.end());
    t.setThermodynamicForcesInitialValues(s_t0);
  }  // end of MTestParser::handleThermodynamicForce

  void MTestParser::handleUserDefinedPostProcessing(MTest& t,
                                                    tokens_iterator& p) {
    const std::string m = "MTestParser::handleUserDefinedPostProcessing";
    // output file
    const auto& f = this->readString(p, this->tokens.end());
    this->checkNotEndOfLine(m, p, this->tokens.end());
    auto v = std::vector<std::string>{};
    if (p->flag == tfel::utilities::Token::String) {
      v.push_back(this->readString(p, this->tokens.end()));
    } else {
      this->readSpecifiedToken(m, "{", p, this->tokens.end());
      this->checkNotEndOfLine(m, p, this->tokens.end());
      while (p->value != "}") {
        v.push_back(this->readString(p, this->tokens.end()));
        this->checkNotEndOfLine(m, p, this->tokens.end());
        if (p->value != "}") {
          this->readSpecifiedToken(m, ",", p, this->tokens.end());
          this->checkNotEndOfLine(m, p, this->tokens.end());
          tfel::raise_if(p->value == "}", m + ": unexpected token '}'");
        }
      }
      this->readSpecifiedToken(m, "}", p, this->tokens.end());
    }
    this->readSpecifiedToken(m, ";", p, this->tokens.end());
    t.addUserDefinedPostProcessing(f, v);
  }  // end of MTestParser::handleUserDefinedPostProcessing

  ConstraintOptions MTestParser::readConstraintOptions(const std::string& m,
                                                       tokens_iterator& p) {
    auto throw_if = [&m](const bool b, const char* msg) {
      if (b) {
        tfel::raise(m + ':' + msg);
      }
    };
    this->checkNotEndOfLine(m, p, this->tokens.end());
    if (p->value == ";") {
      return {};
    }
    const auto d = tfel::utilities::Data::read_map(p, this->tokens.end())
                       .get<std::map<std::string, tfel::utilities::Data>>();
    ConstraintOptions opts;
    for (const auto& kv : d) {
      if (kv.first == "active") {
        throw_if(!kv.second.is<bool>(),
                 "invalid type for constraint option 'active'");
        opts.active = kv.second.get<bool>();
      } else if (kv.first == "activating_event") {
        throw_if(!tfel::utilities::is_convertible<std::string>(kv.second),
                 "invalid type for constraint option 'activating_event'");
        opts.activating_events.push_back(
            tfel::utilities::convert<std::string>(kv.second));
      } else if (kv.first == "activating_events") {
        throw_if(!tfel::utilities::is_convertible<std::vector<std::string>>(
                     kv.second),
                 "invalid type for constraint option 'activating_events'");
        opts.activating_events =
            tfel::utilities::convert<std::vector<std::string>>(kv.second);
      } else if (kv.first == "desactivating_event") {
        throw_if(!tfel::utilities::is_convertible<std::string>(kv.second),
                 "invalid type for constraint option 'desactivating_event'");
        opts.desactivating_events.push_back(
            tfel::utilities::convert<std::string>(kv.second));
      } else if (kv.first == "desactivating_events") {
        throw_if(!tfel::utilities::is_convertible<std::vector<std::string>>(
                     kv.second),
                 "invalid type for constraint option 'desactiving_events'");
        opts.desactivating_events =
            tfel::utilities::convert<std::vector<std::string>>(kv.second);
      } else {
        tfel::raise(m + ": unknown constraint option '" + kv.first + "'");
      }
    }
    return opts;
  }  // end of MTestParser::readConstraintOptions

  MTestParser::~MTestParser() = default;

}  // end namespace mtest
