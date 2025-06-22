/*!
 * \file   SchemeParserBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   21 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>
#include <sstream>
#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/Utilities/TextData.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "TFEL/Math/Parser/ExternalCastemFunction.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/MTest.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/TextDataUtilities.hxx"
#include "MTest/FunctionEvolution.hxx"
#include "MTest/SchemeParserBase.hxx"

namespace mtest {

  SchemeParserBase::SchemeParserBase()
      : externalFunctions(new ExternalFunctionManager) {
  }  // end of SchemeParserBase::SchemeParserBase

  std::string SchemeParserBase::getDocumentationFilePath(
      const std::string& s, const std::string& k) const {
    const auto root = tfel::getInstallPath();
#ifdef TFEL_APPEND_SUFFIX
    auto f =
        root + "/share/doc/mtest-" VERSION "/" + s + '/' + k.substr(1) + ".md";
#else  /* TFEL_APPEND_SUFFIX */
    auto f = root + "/share/doc/mtest/" + s + '/' + k.substr(1) + ".md";
#endif /* TFEL_APPEND_SUFFIX */
    std::ifstream desc{f};
    if (desc) {
      return f;
    }
#ifdef TFEL_APPEND_SUFFIX
    f = root + "/share/doc/mtest-" VERSION "/" + k.substr(1) + ".md";
#else  /* TFEL_APPEND_SUFFIX */
    f = root + "/share/doc/mtest/" + k.substr(1) + ".md";
#endif /* TFEL_APPEND_SUFFIX */
    desc.open(f);
    if (desc) {
      return f;
    }
    return "";
  }

  void SchemeParserBase::handleAuthor(SchemeBase& t, tokens_iterator& p) {
    t.setAuthor(this->readUntilEndOfInstruction(p));
  }  // end of SchemeParserBase::handleAuthor

  void SchemeParserBase::handleLonelySeparator(SchemeBase&,
                                               tokens_iterator& p) {
    mfront::getLogStream() << this->file << ":" << p->line << ":" << p->offset
                           << ": warning: extra ‘;’\n";
  }  // end of SchemeParserBase::handleAuthor

  void SchemeParserBase::handleDate(SchemeBase& t, tokens_iterator& p) {
    t.setDate(this->readUntilEndOfInstruction(p));
  }  // end of SchemeParserBase::handleDate

  void SchemeParserBase::handleDescription(SchemeBase& t, tokens_iterator& p) {
    using namespace std;
    using namespace tfel::utilities;
    this->readSpecifiedToken("SchemeParserBase::handleDescription", "{", p,
                             this->tokens.end());
    this->checkNotEndOfLine("SchemeParserBase::handleDescription", p,
                            this->tokens.end());
    auto currentLine = p->line;
    auto openedBrackets = 1u;
    auto description = std::string{};
    while ((!((p->value == "}") && (openedBrackets == 1u))) &&
           (p != this->tokens.end())) {
      if (p->value == "{") {
        const auto previous = std::prev(p);
        if ((previous->value.size() > 0) &&
            (previous->value[previous->value.size() - 1] != '\\')) {
          ++openedBrackets;
        }
      }
      if (p->value == "}") {
        const auto previous = std::prev(p);
        if ((previous->value.size() > 0) &&
            (previous->value[previous->value.size() - 1] != '\\')) {
          --openedBrackets;
        }
      }
      if (currentLine != p->line) {
        description += "\n";
        currentLine = p->line;
      }
      if (p->flag == Token::String) {
        description += p->value.substr(1, p->value.size() - 2);
      } else {
        description += p->value;
      }
      description += " ";
      ++p;
    }
    if (p == this->tokens.end()) {
      --p;
      tfel::raise(
          "SchemeParserBase::handleDescription: "
          "file ended before the end of description.");
    }
    ++p;
    this->readSpecifiedToken("SchemeParserBase::handleDescription", ";", p,
                             this->tokens.end());
    t.setDescription(description);
  }  // end of SchemeParserBase::Description

  void SchemeParserBase::handleXMLOutputFile(SchemeBase& t,
                                             tokens_iterator& p) {
    t.setXMLOutputFileName(this->readString(p, this->tokens.end()));
    this->readSpecifiedToken("SchemeParserBase::handleXMLOutputFiles", ";", p,
                             this->tokens.end());
  }  // end of SchemeParserBase::handleXMLOutputFile

  void SchemeParserBase::handleOutputFile(SchemeBase& t, tokens_iterator& p) {
    t.setOutputFileName(this->readString(p, this->tokens.end()));
    this->readSpecifiedToken("SchemeParserBase::handleOutputFiles", ";", p,
                             this->tokens.end());
  }  // end of SchemeParserBase::handleOutputFile

  void SchemeParserBase::handleOutputFilePrecision(SchemeBase& t,
                                                   tokens_iterator& p) {
    t.setOutputFilePrecision(this->readUnsignedInt(p, this->tokens.end()));
    this->readSpecifiedToken("SchemeParserBase::handleOutputFilePrecisions",
                             ";", p, this->tokens.end());
  }  // end of SchemeParserBase::handleOutputFilePrecision

  void SchemeParserBase::handleResidualFile(SchemeBase& t, tokens_iterator& p) {
    t.setResidualFileName(this->readString(p, this->tokens.end()));
    this->readSpecifiedToken("SchemeParserBase::handleResidualFiles", ";", p,
                             this->tokens.end());
  }  // end of SchemeParserBase::handleResidualFile

  void SchemeParserBase::handleResidualFilePrecision(SchemeBase& t,
                                                     tokens_iterator& p) {
    t.setResidualFilePrecision(this->readUnsignedInt(p, this->tokens.end()));
    this->readSpecifiedToken("SchemeParserBase::handleResidualFilePrecisions",
                             ";", p, this->tokens.end());
  }  // end of SchemeParserBase::handleResidualFilePrecision

  void SchemeParserBase::handleReal(SchemeBase& t, tokens_iterator& p) {
    const auto& v = this->readString(p, this->tokens.end());
    tfel::raise_if(
        !this->isValidIdentifier(v),
        "SchemeParserBase::handleReal : '" + v + "' is not a valid identifier");
    const real value = this->readDouble(t, p);
    auto mpev = std::shared_ptr<Evolution>(new ConstantEvolution(value));
    this->readSpecifiedToken("SchemeParserBase::handleReal", ";", p,
                             this->tokens.end());
    t.addEvolution(v, mpev, true, true);
  }

  void SchemeParserBase::handlePredictionPolicy(SchemeBase& t,
                                                tokens_iterator& p) {
    PredictionPolicy ppolicy;
    this->checkNotEndOfLine("handlePredictionPolicy", p, this->tokens.end());
    const auto& s = this->readString(p, this->tokens.end());
    this->readSpecifiedToken("SchemeParserBase::handlePredictionPolicy", ";", p,
                             this->tokens.end());
    if (s == "NoPrediction") {
      ppolicy = PredictionPolicy::NOPREDICTION;
    } else if (s == "LinearPrediction") {
      ppolicy = PredictionPolicy::LINEARPREDICTION;
    } else if (s == "ElasticPrediction") {
      ppolicy = PredictionPolicy::ELASTICPREDICTION;
    } else if (s == "ElasticPredictionFromMaterialProperties") {
      ppolicy = PredictionPolicy::ELASTICPREDICTIONFROMMATERIALPROPERTIES;
    } else if (s == "SecantOperatorPrediction") {
      ppolicy = PredictionPolicy::SECANTOPERATORPREDICTION;
    } else if (s == "TangentOperatorPrediction") {
      ppolicy = PredictionPolicy::TANGENTOPERATORPREDICTION;
    } else {
      tfel::raise(
          "SchemeParserBase::handlePredictionPolicy: "
          "unsupported prediction policy '" +
          s + "'");
    }
    t.setPredictionPolicy(ppolicy);
  }

  void SchemeParserBase::handleStiffnessMatrixType(SchemeBase& t,
                                                   tokens_iterator& p) {
    StiffnessMatrixType ktype;
    const auto& type = this->readString(p, this->tokens.end());
    if (type == "Elastic") {
      ktype = StiffnessMatrixType::ELASTIC;
    } else if (type == "SecantOperator") {
      ktype = StiffnessMatrixType::SECANTOPERATOR;
    } else if (type == "TangentOperator") {
      ktype = StiffnessMatrixType::TANGENTOPERATOR;
    } else if (type == "ConsistentTangentOperator") {
      ktype = StiffnessMatrixType::CONSISTENTTANGENTOPERATOR;
    } else {
      tfel::raise(
          "SchemeParserBase::handleStiffnessMatrixType: "
          "unsupported stiffness matrix type '" +
          type + "'");
    }
    this->readSpecifiedToken("SchemeParserBase::handleStiffnessMatrixType", ";",
                             p, this->tokens.end());
    t.setStiffnessMatrixType(ktype);
  }

  void SchemeParserBase::handleUseCastemAccelerationAlgorithm(
      SchemeBase& t, tokens_iterator& p) {
    bool useCastemAcceleration;
    this->checkNotEndOfLine(
        "SchemeParserBase::handleUseCastemAccelerationAlgorithm", p,
        this->tokens.end());
    if (p->value == "true") {
      useCastemAcceleration = true;
    } else if (p->value == "false") {
      useCastemAcceleration = false;
    } else {
      tfel::raise(
          "SchemeParserBase::handleUseCastemAccelerationAlgorithm: "
          "unexpected token '" +
          p->value + "'");
    }
    ++p;
    this->readSpecifiedToken(
        "SchemeParserBase::handleUseCastemAccelerationAlgorithm", ";", p,
        this->tokens.end());
    t.setUseCastemAccelerationAlgorithm(useCastemAcceleration);
  }

  void SchemeParserBase::handleCastemAccelerationTrigger(SchemeBase& t,
                                                         tokens_iterator& p) {
    const auto cat =
        static_cast<int>(this->readUnsignedInt(p, this->tokens.end()));
    this->readSpecifiedToken(
        "SchemeParserBase::handleCastemAccelerationTrigger", ";", p,
        this->tokens.end());
    t.setCastemAccelerationTrigger(cat);
  }  // end of SchemeParserBase::handleCastemAccelerationTrigger

  void SchemeParserBase::handleCastemAccelerationPeriod(SchemeBase& t,
                                                        tokens_iterator& p) {
    const auto cap =
        static_cast<int>(this->readUnsignedInt(p, this->tokens.end()));
    this->readSpecifiedToken("SchemeParserBase::handleCastemAccelerationPeriod",
                             ";", p, this->tokens.end());
    t.setCastemAccelerationPeriod(cap);
  }  // end of SchemeParserBase::handleCastemAccelerationPeriod

  void SchemeParserBase::handleAccelerationAlgorithm(SchemeBase& t,
                                                     tokens_iterator& p) {
    this->checkNotEndOfLine("SchemeParserBase::handleAccelerationAlgorithm", p,
                            this->tokens.end());
    const auto& a = this->readString(p, this->tokens.end());
    this->readSpecifiedToken("SchemeParserBase::handleAccelerationAlgorithm",
                             ";", p, this->tokens.end());
    t.setAccelerationAlgorithm(a);
  }

  void SchemeParserBase::handleAccelerationAlgorithmParameter(
      SchemeBase& t, tokens_iterator& p) {
    this->checkNotEndOfLine(
        "SchemeParserBase::handleAccelerationAlgorithmParameter", p,
        this->tokens.end());
    const auto& pn = this->readString(p, this->tokens.end());
    this->checkNotEndOfLine(
        "SchemeParserBase::handleAccelerationAlgorithmParameter", p,
        this->tokens.end());
    const auto& v = p->value;
    ++p;
    this->readSpecifiedToken(
        "SchemeParserBase::handleAccelerationAlgorithmParameter", ";", p,
        this->tokens.end());
    t.setAccelerationAlgorithmParameter(pn, v);
  }  // end of SchemeParserBase::handleIronsTuckAccelerationTrigger

  void SchemeParserBase::handleStiffnessUpdatePolicy(SchemeBase& t,
                                                     tokens_iterator& p) {
    StiffnessUpdatingPolicy ks;
    const auto& type = this->readString(p, this->tokens.end());
    if (type == "ConstantStiffness") {
      ks = StiffnessUpdatingPolicy::CONSTANTSTIFFNESS;
    } else if (type == "SecantOperator") {
      ks = StiffnessUpdatingPolicy::CONSTANTSTIFFNESSBYPERIOD;
    } else if (type == "TangentOperator") {
      ks = StiffnessUpdatingPolicy::UPDATEDSTIFFNESSMATRIX;
    } else {
      tfel::raise(
          "SchemeParserBase::handleStiffnessUpdatePolicy: "
          "unsupported stiffness matrix policy '" +
          type + "'");
    }
    this->readSpecifiedToken("SchemeParserBase::handleStiffnessUpdatePolicy",
                             ";", p, this->tokens.end());
    t.setStiffnessUpdatingPolicy(ks);
  }

  void SchemeParserBase::handleMaximumNumberOfIterations(SchemeBase& t,
                                                         tokens_iterator& p) {
    t.setMaximumNumberOfIterations(
        this->readUnsignedInt(p, this->tokens.end()));
    this->readSpecifiedToken(
        "SchemeParserBase::handleMaximumNumberOfIterations", ";", p,
        this->tokens.end());
  }  // end of SchemeParserBase::handleMaximumNumberOfIterations

  void SchemeParserBase::handleMaximumNumberOfSubSteps(SchemeBase& t,
                                                       tokens_iterator& p) {
    t.setMaximumNumberOfSubSteps(this->readUnsignedInt(p, this->tokens.end()));
    this->readSpecifiedToken("SchemeParserBase::handleMaximumNumberOfSubSteps",
                             ";", p, this->tokens.end());
  }  // end of SchemeParserBase::handleMaximumNumberOfSubSteps

  void SchemeParserBase::handleOutputFrequency(SchemeBase& t,
                                               tokens_iterator& p) {
    const auto v = this->readString(p, this->tokens.end());
    if (v == "UserDefinedTimes") {
      t.setOutputFrequency(SchemeBase::USERDEFINEDTIMES);
    } else if (v == "EveryPeriod") {
      t.setOutputFrequency(SchemeBase::EVERYPERIOD);
    } else {
      tfel::raise(
          "SchemeParserBase::handleOutputFrequency: "
          "invalid frequency '" +
          v + "'");
    }
    this->readSpecifiedToken("SchemeParserBase::handleOutputFrequency", ";", p,
                             this->tokens.end());
  }  // end of SchemeParserBase::handleOutputFrequency

  void SchemeParserBase::handleDynamicTimeStepScaling(SchemeBase& t,
                                                      tokens_iterator& p) {
    this->checkNotEndOfLine("SchemeParserBase::handleDynamicTimeStepScaling", p,
                            this->tokens.end());
    if (p->value == "true") {
      t.setDynamicTimeStepScaling(true);
    } else if (p->value == "false") {
      t.setDynamicTimeStepScaling(false);
    } else {
      tfel::raise(
          "SchemeParserBase::handleDynamicTimeStepScaling: "
          "unexpected value (expected 'true' or 'false', "
          "read '" +
          p->value + "')");
    }
    ++p;
    this->readSpecifiedToken("SchemeParserBase::handleDynamicTimeStepScaling",
                             ";", p, this->tokens.end());
  }

  void SchemeParserBase::handleMaximalTimeStep(SchemeBase& t,
                                               tokens_iterator& p) {
    t.setMaximalTimeStep(this->readDouble(t, p));
    this->readSpecifiedToken("SchemeParserBase::handleMaximalTimeStep", ";", p,
                             this->tokens.end());
  }

  void SchemeParserBase::handleMinimalTimeStep(SchemeBase& t,
                                               tokens_iterator& p) {
    t.setMinimalTimeStep(this->readDouble(t, p));
    this->readSpecifiedToken("SchemeParserBase::handleMinimalTimeStep", ";", p,
                             this->tokens.end());
  }

  void SchemeParserBase::handleMinimalTimeStepScalingFactor(
      SchemeBase& t, tokens_iterator& p) {
    t.setMinimalTimeStepScalingFactor(this->readDouble(t, p));
    this->readSpecifiedToken(
        "SchemeParserBase::handleMinimalTimeStepScalingFactor", ";", p,
        this->tokens.end());
  }

  void SchemeParserBase::handleMaximalTimeStepScalingFactor(
      SchemeBase& t, tokens_iterator& p) {
    t.setMaximalTimeStepScalingFactor(this->readDouble(t, p));
    this->readSpecifiedToken(
        "SchemeParserBase::handleMaximalTimeStepScalingFactor", ";", p,
        this->tokens.end());
  }

  std::string SchemeParserBase::readUntilEndOfInstruction(tokens_iterator& p) {
    auto res = std::string{};
    this->checkNotEndOfLine("SchemeParserBase::readUntilEndOfInstruction", p,
                            this->tokens.end());
    while ((p != this->tokens.end()) && (p->value != ";")) {
      if (!p->value.empty()) {
        tfel::raise_if(p->value[0] == '@',
                       "SchemeParserBase::readUntilEndOfInstruction: "
                       "no word beginning with '@' are allowed here");
        res += p->value;
        res += " ";
      }
      ++p;
    }
    this->readSpecifiedToken("SchemeParserBase::readUntilEndOfInstruction", ";",
                             p, this->tokens.end());
    return res;
  }

  std::vector<real> SchemeParserBase::readTimesArray(const std::string& m,
                                                     SchemeBase& t,
                                                     tokens_iterator& p) {
    auto times = std::vector<real>{};
    this->readSpecifiedToken(m, "{", p, this->tokens.end());
    this->checkNotEndOfLine(m, p, this->tokens.end());
    while (p->value != "}") {
      const real t_dt = this->readTime(t, p);
      this->checkNotEndOfLine(m, p, this->tokens.end());
      if (!times.empty()) {
        if (p->value == "in") {
          ++p;
          const auto n = this->readUnsignedInt(p, this->tokens.end());
          tfel::raise_if(n == 0,
                         "SchemeParserBase::handleTimes: "
                         "invalid number of intervals");
          const auto tt = times.back();
          const auto dt = (t_dt - tt) / (static_cast<real>(n));
          for (unsigned int i = 1; i != n; ++i) {
            times.push_back(tt + i * dt);
          }
        }
        this->checkNotEndOfLine(m, p, this->tokens.end());
      }
      times.push_back(t_dt);
      if (p->value == ",") {
        ++p;
        this->checkNotEndOfLine(m, p, this->tokens.end());
        tfel::raise_if(p->value == "}",
                       "SchemeParserBase::handleTimes: "
                       "unexpected token '}'");
      } else {
        tfel::raise_if(p->value != "}",
                       "SchemeParserBase::handleTimes: "
                       "unexpected token '" +
                           p->value +
                           "', "
                           "expected ',' or '}'");
      }
    }
    this->readSpecifiedToken(m, "}", p, this->tokens.end());
    return times;
  }  // end of SchemeParserBase::readTimesArray

  void SchemeParserBase::handleTimes(SchemeBase& t, tokens_iterator& p) {
    enum { ARRAY, FILE } entry_type = ARRAY;
    std::vector<real> times;
    this->checkNotEndOfLine("SchemeParserBase::handleTimes", p,
                            this->tokens.end());
    if (p->value == "<") {
      ++p;
      this->checkNotEndOfLine("SchemeParserBase::handleTimes", p,
                              this->tokens.end());
      if (p->value == "array") {
        entry_type = ARRAY;
      } else if ((p->value == "file") || (p->value == "data")) {
        entry_type = FILE;
      } else {
        tfel::raise(
            "SchemeParserBase::handleTimes: "
            "invalid entry type for times. Expected 'array' "
            "or 'file' or 'data', read '" +
            p->value + "'");
      }
      ++p;
      this->readSpecifiedToken("SchemeParserBase::handleTimes", ">", p,
                               this->tokens.end());
    }
    if (entry_type == ARRAY) {
      times = this->readTimesArray("SchemeParserBase::handleTimes", t, p);
    } else {
      tfel::utilities::TextData d{this->readString(p, this->tokens.end())};
      this->readSpecifiedToken("SchemeParserBase::handleTimes", "using", p,
                               this->tokens.end());
      this->checkNotEndOfLine("SchemeParserBase::handleTimes", p,
                              this->tokens.end());
      if (p->flag == tfel::utilities::Token::String) {
        times =
            eval(d, t.getEvolutions(), this->readString(p, this->tokens.end()));
      } else {
        times = d.getColumn(this->readUnsignedInt(p, this->tokens.end()));
      }
    }
    this->readSpecifiedToken("SchemeParserBase::handleTimes", ";", p,
                             this->tokens.end());
    tfel::raise_if(times.empty(),
                   "SchemeParserBase::handleTimes: "
                   "no time defined");
    tfel::raise_if(times.size() == 1,
                   "SchemeParserBase::handleTimes: "
                   "at least two different times must be defined");
    auto pt = times.begin();
    real mt(0);
    while (pt != times.end()) {
      mt = std::max(mt, std::abs(*pt));
      ++pt;
    }
    tfel::raise_if(mt < 100 * std::numeric_limits<real>::min(),
                   "SchemeParserBase::handleTimes: maximal "
                   "absolute value of times is too low");
    const real eps = 100 * mt * std::numeric_limits<real>::epsilon();
    pt = times.begin();
    auto pt2 = pt + 1u;
    while (pt2 != times.end()) {
      if ((*pt2 <= *pt) || std::abs(*pt2 - *pt) < eps) {
        std::ostringstream msg;
        msg << "SchemeParserBase::handleTimes : times '" << *pt2
            << "' is lesser than or too close to  time '" << *pt << "'";
        tfel::raise(msg.str());
      }
      ++pt2;
      ++pt;
    }
    t.setTimes(times);
  }

  void SchemeParserBase::handleEvolution(SchemeBase& t, tokens_iterator& p) {
    const auto& evt = this->readEvolutionType(p);
    const auto& n = this->readString(p, this->tokens.end());
    t.addEvolution(n, this->parseEvolution(t, evt, p), true, true);
    this->readSpecifiedToken("SchemeParserBase::handleEvolution", ";", p,
                             this->tokens.end());
  }

  real SchemeParserBase::readDouble(SchemeBase& t, tokens_iterator& p) {
    this->checkNotEndOfLine("SchemeParserBase::readDouble", p,
                            this->tokens.end());
    real r(0);
    if (p->flag == tfel::utilities::Token::String) {
      const auto& f = this->readString(p, this->tokens.end());
      tfel::math::Evaluator ev(f, this->externalFunctions);
      for (const auto& v : ev.getVariablesNames()) {
        const auto evs = t.getEvolutions();
        const auto pev = evs.find(v);
        tfel::raise_if(pev == evs.end(),
                       "SchemeParserBase::readDouble: "
                       "no evolution named '" +
                           v + "' defined");
        const auto& e = *(pev->second);
        tfel::raise_if(!e.isConstant(),
                       "SchemeParserBase::readDouble : "
                       "formula '" +
                           f +
                           "' shall "
                           "only depend on constant evolutions "
                           "(evolution '" +
                           v + "' is not constant)");
        ev.setVariableValue(v, e(0));
      }
      r = ev.getValue();
    } else {
      r = CxxTokenizer::readDouble(p, this->tokens.end());
    }
    return r;
  }  // end of SchemeParserBase::readDouble

  real SchemeParserBase::readTime(SchemeBase& t, tokens_iterator& p) {
    return this->readDouble(t, p);
  }  // end of SchemeParserBase::readTime

  void SchemeParserBase::readArrayOfSpecifiedSize(std::vector<real>& v,
                                                  SchemeBase& t,
                                                  tokens_iterator& p) {
    this->readSpecifiedToken("SchemeParserBase::readArrayOfSpecifiedSize", "{",
                             p, this->tokens.end());
    std::vector<real>::size_type i = 0;
    while (i != v.size()) {
      v[i] = this->readDouble(t, p);
      if (++i != v.size()) {
        this->readSpecifiedToken("SchemeParserBase::readArrayOfSpecifiedSize",
                                 ",", p, this->tokens.end());
      }
    }
    this->readSpecifiedToken("SchemeParserBase::readArrayOfSpecifiedSize", "}",
                             p, this->tokens.end());
  }  // end of SchemeParserBase::readArrayOfSpecifiedSize

  std::shared_ptr<Evolution> SchemeParserBase::parseEvolution(
      SchemeBase& t, const std::string& type, tokens_iterator& p) {
    auto ev = std::shared_ptr<Evolution>{};
    this->checkNotEndOfLine("SchemeParserBase::parseEvolution", p,
                            this->tokens.end());
    if (type.empty() || type == "evolution") {
      if (p->value == "{") {
        auto tvalues = std::vector<real>{};
        auto values = std::vector<real>{};
        this->readSpecifiedToken("SchemeParserBase::parseEvolution", "{", p,
                                 this->tokens.end());
        this->checkNotEndOfLine("SchemeParserBase::parseEvolution", p,
                                this->tokens.end());
        while (p->value != "}") {
          tvalues.push_back(this->readTime(t, p));
          this->readSpecifiedToken("SchemeParserBase::parseEvolution", ":", p,
                                   this->tokens.end());
          values.push_back(this->readDouble(t, p));
          this->checkNotEndOfLine("SchemeParserBase::parseEvolution", p,
                                  this->tokens.end());
          if (p->value == ",") {
            ++p;
            this->checkNotEndOfLine("SchemeParserBase::parseEvolution", p,
                                    this->tokens.end());
            tfel::raise_if(p->value == "}",
                           "SchemeParserBase::parseEvolution: "
                           "unexpected token '}'");
          } else {
            tfel::raise_if(p->value != "}",
                           "SchemeParserBase::parseEvolution: "
                           "unexpected token '" +
                               p->value + "', expected ',' or '}'");
          }
        }
        this->readSpecifiedToken("SchemeParserBase::parseEvolution", "}", p,
                                 this->tokens.end());
        ev = std::shared_ptr<Evolution>(new LPIEvolution(tvalues, values));
      } else {
        const real s = this->readDouble(t, p);
        ev = std::shared_ptr<Evolution>(new ConstantEvolution(s));
      }
    } else if (type == "function") {
      const auto& f = this->readString(p, this->tokens.end());
      ev = std::shared_ptr<Evolution>(
          new FunctionEvolution(f, t.getEvolutions()));
    } else if ((type == "data") || (type == "file")) {
      tfel::utilities::TextData data{this->readString(p, this->tokens.end())};
      this->readSpecifiedToken("SchemeParserBase::parseEvolution", "using", p,
                               this->tokens.end());
      auto tv = std::vector<double>{};
      auto vv = std::vector<double>{};
      if (p->flag == tfel::utilities::Token::String) {
        tv = eval(data, t.getEvolutions(),
                  this->readString(p, this->tokens.end()));
      } else {
        tv = data.getColumn(readUnsignedInt(p, this->tokens.end()));
      }
      this->readSpecifiedToken("SchemeParserBase::parseEvolution", ":", p,
                               this->tokens.end());
      if (p->flag == tfel::utilities::Token::String) {
        vv = eval(data, t.getEvolutions(),
                  this->readString(p, this->tokens.end()));
      } else {
        vv = data.getColumn(readUnsignedInt(p, this->tokens.end()));
      }
      ev = std::shared_ptr<Evolution>(new LPIEvolution(tv, vv));
    } else {
      tfel::raise(
          "SchemeParserBase::parseEvolution: "
          "invalid evolution type '" +
          type + "'");
    }
    return ev;
  }  // end of SchemeParserBase::parseEvolution

  std::string SchemeParserBase::readEvolutionType(tokens_iterator& p) {
    this->checkNotEndOfLine("SchemeParserBase::readEvolutionType", p,
                            this->tokens.end());
    if (p->value != "<") {
      return "";
    }
    this->readSpecifiedToken("SchemeParserBase::parseEvolution", "<", p,
                             this->tokens.end());
    this->checkNotEndOfLine("SchemeParserBase::readEvolutionType", p,
                            this->tokens.end());
    const auto evt = p->value;
    ++p;
    this->readSpecifiedToken("SchemeParserBase::parseEvolution", ">", p,
                             this->tokens.end());
    return evt;
  }  // end of SchemeParserBase::readEvolutionType

  void SchemeParserBase::registerCallBack(const std::string& k,
                                          const SchemeParserBase::CallBack& p) {
    this->callbacks.insert({k, p});
  }

  void SchemeParserBase::registerCallBacks() {
    auto add = [this](const char* n, const CallBack& c) {
      this->registerCallBack(n, c);
    };
    add(";", &SchemeParserBase::handleLonelySeparator);
    add("@Author", &SchemeParserBase::handleAuthor);
    add("@Date", &SchemeParserBase::handleDate);
    add("@Description", &SchemeParserBase::handleDescription);
    add("@OutputFile", &SchemeParserBase::handleOutputFile);
    add("@XMLOutputFile", &SchemeParserBase::handleXMLOutputFile);
    add("@OutputFrequency", &SchemeParserBase::handleOutputFrequency);
    add("@OutputFilePrecision", &SchemeParserBase::handleOutputFilePrecision);
    add("@ResidualFile", &SchemeParserBase::handleResidualFile);
    add("@ResidualFilePrecision",
        &SchemeParserBase::handleResidualFilePrecision);
    add("@Times", &SchemeParserBase::handleTimes);
    add("@AccelerationAlgorithm",
        &SchemeParserBase::handleAccelerationAlgorithm);
    add("@AccelerationAlgorithmParameter",
        &SchemeParserBase::handleAccelerationAlgorithmParameter);
    add("@UseCastemAccelerationAlgorithm",
        &SchemeParserBase::handleUseCastemAccelerationAlgorithm);
    add("@CastemAccelerationPeriod",
        &SchemeParserBase::handleCastemAccelerationPeriod);
    add("@CastemAccelerationTrigger",
        &SchemeParserBase::handleCastemAccelerationTrigger);
    add("@StiffnessMatrixType", &SchemeParserBase::handleStiffnessMatrixType);
    add("@StiffnessUpdatePolicy",
        &SchemeParserBase::handleStiffnessUpdatePolicy);
    add("@PredictionPolicy", &SchemeParserBase::handlePredictionPolicy);
    add("@Real", &SchemeParserBase::handleReal);
    add("@Evolution", &SchemeParserBase::handleEvolution);
    add("@MaximumNumberOfIterations",
        &SchemeParserBase::handleMaximumNumberOfIterations);
    add("@MaximumNumberOfSubSteps",
        &SchemeParserBase::handleMaximumNumberOfSubSteps);
    add("@DynamicTimeStepScaling",
        &SchemeParserBase::handleDynamicTimeStepScaling);
    add("@MaximalTimeStepScalingFactor",
        &SchemeParserBase::handleMaximalTimeStepScalingFactor);
    add("@MinimalTimeStepScalingFactor",
        &SchemeParserBase::handleMinimalTimeStepScalingFactor);
    add("@MaximalTimeStep", &SchemeParserBase::handleMaximalTimeStep);
    add("@MinimalTimeStep", &SchemeParserBase::handleMinimalTimeStep);
    add("@Import", &SchemeParserBase::handleImport);
    add("@Message", &SchemeParserBase::handleMessage);
    add("@Print", &SchemeParserBase::handleMessage);
  }  // end of SchemeParserBase::registerCallBacks

  void SchemeParserBase::handleImport(SchemeBase&, tokens_iterator& p) {
    this->readSpecifiedToken("SchemeParserBase::handleImport", "<", p,
                             this->tokens.end());
    this->checkNotEndOfLine("SchemeParserBase::handleImport", p,
                            this->tokens.end());
    if ((p->value == "castem") || (p->value == "Castem") ||
        (p->value == "Cast3M")) {
      ++p;
      this->readSpecifiedToken("SchemeParserBase::handleImport", ">", p,
                               this->tokens.end());
      using tfel::math::parser::ExternalCastemFunction;
      auto& elm =
          tfel::system::ExternalLibraryManager::getExternalLibraryManager();
      const auto l = this->readString(p, this->tokens.end());
      const auto f = this->readString(p, this->tokens.end());
      this->readSpecifiedToken("SchemeParserBase::handleImport", ";", p,
                               this->tokens.end());
      const auto c = elm.getCastemFunction(l, f);
      const auto n = elm.getCastemFunctionNumberOfVariables(l, f);
      const auto pc = std::make_shared<ExternalCastemFunction>(c, n);
      tfel::raise_if(!this->externalFunctions->insert({f, pc}).second,
                     "SchemeParserBase::handleImport: "
                     "function '" +
                         f + "' already declared");
    } else {
      tfel::raise(
          "SchemeParserBase::handleImport: "
          "unexpected token '" +
          p->value + "'");
    }
  }  // end of SchemeParserBase::handleImport

  void SchemeParserBase::handleMessage(SchemeBase& t, tokens_iterator& p) {
    this->checkNotEndOfLine("SchemeParserBase::handleMessage", p,
                            this->tokens.end());
    while (p->value != ";") {
      if (p->flag == tfel::utilities::Token::String) {
        auto c = p;
        try {
          std::cout << this->readDouble(t, p);
        } catch (std::exception&) {
          p = c;
          std::cout << this->readString(p, this->tokens.end());
        }
      } else {
        std::cout << this->readDouble(t, p);
        ++p;
      }
      this->checkNotEndOfLine("SchemeParserBase::handleMessage", p,
                              this->tokens.end());
    }
    this->readSpecifiedToken("SchemeParserBase::handleMessage", ";", p,
                             this->tokens.end());
    std::cout << std::endl;
  }  // end of SchemeParserBase::handleMessage

  bool SchemeParserBase::treatKeyword(SchemeBase& t, tokens_iterator& p) {
    auto pc = this->callbacks.find(p->value);
    if (pc == this->callbacks.end()) {
      return false;
    }
    if (mfront::getVerboseMode() >= mfront::VERBOSE_DEBUG) {
      auto& log = mfront::getLogStream();
      log << "SchemeParserBase::execute : treating keyword '" << p->value
          << "' at line '" << p->line << "'\n";
    }
    ++p;
    auto line = p->line;
    try {
      (this->*(pc->second))(t, p);
    } catch (std::exception& e) {
      std::ostringstream msg;
      msg << "SchemeParserBase::MTest : error while "
          << "parsing file '" << this->file << "' at line '" << line << "'.\n"
          << e.what();
      tfel::raise(msg.str());
    }
    return true;
  }  // end of SchemeParserBase::treatKeyword

  std::vector<std::string> SchemeParserBase::getKeyWordsList() const {
    auto keys = std::vector<std::string>{};
    for (const auto& k : this->callbacks) {
      keys.push_back(k.first);
    }
    return keys;
  }  // end of SchemeParserBase::getKeyWordsList

  SchemeParserBase::~SchemeParserBase() = default;

}  // end of namespace mtest
