/*!
 * \file   TestLauncherV1.cxx
 * \brief  Class to manage tfel unit tests.
 *
 * \author Helfer Thomas
 * \date   26 jan 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iterator>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>
#include <cmath>
#include <limits>
#include <ctime>
#include <unistd.h>  // sysconf

#include "TFEL/System/ProcessManager.hxx"
#include "TFEL/Utilities/TerminalColors.hxx"

#include "TFELCheck/TestLauncherV1.hxx"
#include "TFELCheck/PCLogger.hxx"
#include "TFELCheck/PCTextDriver.hxx"
#include "TFELCheck/PCJUnitDriver.hxx"
#include "TFELCheck/SplineInterpolation.hxx"
#include "TFELCheck/SplineLocalInterpolation.hxx"
#include "TFELCheck/LinearInterpolation.hxx"
#include "TFELCheck/NoInterpolation.hxx"
#include "TFELCheck/AbsoluteComparison.hxx"
#include "TFELCheck/RelativeComparison.hxx"
#include "TFELCheck/RelativeAndAbsoluteComparison.hxx"
#include "TFELCheck/MixedComparison.hxx"
#include "TFELCheck/AreaComparison.hxx"

namespace tfel_check {

  TestLauncherV1::TestLauncherV1(const std::string& f, const PCLogger& l)
      : glog(l),
        file(f),
        prec(1.e-8),
        precision2(0.),
        comparison(new AbsoluteComparison()),
        interpolation(new NoInterpolation()),
        integralInterpolation(new NoInterpolation()),
        allowLessResults(false) {
    using namespace std;
    using namespace tfel::utilities;

    vector<Token> tmpLine;
    TokensContainer::const_iterator p;
    unsigned short currentLine;

    this->testname = this->file.substr(0, this->file.rfind("."));

    this->logfile = this->testname + ".checklog";
    this->log = PCLogger(std::make_shared<PCTextDriver>(this->logfile));

    string xmllogfile = "TEST-" + this->testname + ".xml";
    this->log.addDriver(std::make_shared<PCJUnitDriver>(xmllogfile));

    this->ClockAction(START);
    this->ClockAction(STOP);
    this->ClockAction(GET);

    this->treatCharAsString(true);
    this->mergeStrings(false);
    // open the file
    this->openFile(this->file);
    // strip any C/C++ comments
    this->stripComments();
    // store tokens line by line
    p = this->begin();
    // check that we don't treat an empty file
    if (p == this->end()) {
      string msg("TestLauncherV1::TestLauncherV1 : ");
      msg += "file " + file + " is empty.";
      log.addMessage(msg);
      throw(runtime_error(msg));
    }
    currentLine = 0u;
    while (p != this->end()) {
      if (p->line != currentLine) {
        if (!tmpLine.empty()) {
          this->tokens.push_back(tmpLine);
        }
        currentLine = p->line;
        tmpLine.clear();
      }
      tmpLine.push_back(*p);
      ++p;
    }
    this->tokens.push_back(tmpLine);
    // a bit of clean-up
    this->clear();
    // callbacks
    this->registerCallBack("Environment", &TestLauncherV1::treatEnvironment);
    this->registerCallBack("MeshCommand", &TestLauncherV1::treatMeshCommand);
    this->registerCallBack("Command", &TestLauncherV1::treatCommand);
    this->registerCallBack("Test", &TestLauncherV1::treatTest);
    this->registerCallBack("TestType", &TestLauncherV1::treatTestType);
    this->registerCallBack("Precision", &TestLauncherV1::treatPrecision);
    this->registerCallBack("Interpolation",
                           &TestLauncherV1::treatInterpolation);
    this->analyseInputFile();

  }  // end of TestLauncherV1::TestLauncherV1

  void TestLauncherV1::registerCallBack(const std::string& n,
                                        const CallBack& f) {
    if (!this->callBacks.insert({n, f}).second) {
      throw(
          std::runtime_error("TestLauncherV1::registerCallBack: "
                             "keyword '" +
                             n + "' has already been registred."));
    }
  }  // end of TestLauncherV1::registerCallBack

  void TestLauncherV1::treatEnvironment(void) {
    using namespace std;
    using namespace tfel::utilities;
    if (this->current == this->line->end()) {
      ostringstream msg;
      msg << "TestLauncherV1::treatEnvironment : ";
      msg << "unexpected end of line.\n";
      msg << "Error at line " << this->line->begin()->line;
      log.addMessage(msg.str());
      throw(runtime_error(msg.str()));
    }
    if (this->current->value != "{") {
      ostringstream msg;
      msg << "TestLauncherV1::treatEnvironment : ";
      msg << "expected token '{', read '" << this->current->value << "'.\n";
      msg << "Error at line " << this->line->begin()->line;
      log.addMessage(msg.str());
      throw(runtime_error(msg.str()));
    }
    ++(this->current);
    if (this->current == this->line->end()) {
      ostringstream msg;
      msg << "TestLauncherV1::treatEnvironment : ";
      msg << "unexpected end of line.\n";
      msg << "Error at line " << this->line->begin()->line;
      log.addMessage(msg.str());
      throw(runtime_error(msg.str()));
    }
    while (this->current->value != "}") {
      if (!((this->current->flag == Token::String) ||
            (this->current->flag == Token::Char))) {
        ostringstream msg;
        msg << "TestLauncherV1::treatEnvironment : ";
        msg << "expected a string";
        log.addMessage(msg.str());
        throw(runtime_error(msg.str()));
      }
      const auto& key =
          this->current->value.substr(1, this->current->value.size() - 2);
      ++(this->current);
      if (this->current == this->line->end()) {
        ostringstream msg;
        msg << "TestLauncherV1::treatEnvironment : ";
        msg << "unexpected end of line.\n";
        msg << "Error at line " << this->line->begin()->line;
        log.addMessage(msg.str());
        throw(runtime_error(msg.str()));
      }
      if (this->current->value != ":") {
        ostringstream msg;
        msg << "TestLauncherV1::treatEnvironment : ";
        msg << "expected token ':', read '" << this->current->value << "'.\n";
        msg << "Error at line " << this->line->begin()->line;
        log.addMessage(msg.str());
        throw(runtime_error(msg.str()));
      }
      ++(this->current);
      if (this->current == this->line->end()) {
        ostringstream msg;
        msg << "TestLauncherV1::treatEnvironment : ";
        msg << "unexpected end of line.\n";
        msg << "Error at line " << this->line->begin()->line;
        log.addMessage(msg.str());
        throw(runtime_error(msg.str()));
      }
      if (!((this->current->flag == Token::String) ||
            (this->current->flag == Token::Char))) {
        ostringstream msg;
        msg << "TestLauncherV1::treatEnvironment : ";
        msg << "expected a string";
        log.addMessage(msg.str());
        throw(runtime_error(msg.str()));
      }
      const auto& value =
          this->current->value.substr(1, this->current->value.size() - 2);
      ++(this->current);
      if (this->current == this->line->end()) {
        ostringstream msg;
        msg << "TestLauncherV1::treatEnvironment : ";
        msg << "unexpected end of line.\n";
        msg << "Error at line " << this->line->begin()->line;
        log.addMessage(msg.str());
        throw(runtime_error(msg.str()));
      }
      if (this->current->value != "}") {
        if (this->current->value != ",") {
          ostringstream msg;
          msg << "TestLauncherV1::treatEnvironment : ";
          msg << "expected token ',', read '" << this->current->value << "'.\n";
          msg << "Error at line " << this->line->begin()->line;
          log.addMessage(msg.str());
          throw(runtime_error(msg.str()));
        }
        ++(this->current);
        if (this->current == this->line->end()) {
          ++(this->line);
          if (this->line == this->tokens.end()) {
            ostringstream msg;
            msg << "TestLauncherV1::treatEnvironment : ";
            msg << "unexpected end of file";
            log.addMessage(msg.str());
            throw(runtime_error(msg.str()));
          }
          this->current = this->line->begin();
        }
      }
      if (this->environments.find(key) != this->environments.end()) {
        this->environments[key] = value + ":" + this->environments[key];
      } else {
        if (::getenv(key.c_str()) != nullptr) {
          this->environments[key] = value + ':';
          this->environments[key] += ::getenv(key.c_str());
        } else {
          this->environments[key] = value;
        }
      }
    }
    ++(this->current);
  }  // end of TestLauncherV1::treatEnvironment

  void TestLauncherV1::treatCommand(void) {
    using namespace std;
    using namespace tfel::utilities;
    if (this->current == this->line->end()) {
      ostringstream msg;
      msg << "TestLauncherV1::treatCommand : ";
      msg << " unexpected end of line.\n";
      msg << "Error at line " << this->line->begin()->line;
      log.addMessage(msg.str());
      throw(runtime_error(msg.str()));
    }
    if ((this->current->flag != Token::String) &&
        (this->current->flag != Token::Char)) {
      ostringstream msg;
      msg << "TestLauncherV1::treatCommand : ";
      msg << "invalid line " << this->line->begin()->line;
      msg << " (first filename is invalid)";
      log.addMessage(msg.str());
      throw(runtime_error(msg.str()));
    }
    this->commands.push_back(
        this->current->value.substr(1, this->current->value.size() - 2));
    ++(this->current);
  }  // end of TestLauncherV1::treatTestCommand

  void TestLauncherV1::treatMeshCommand(void) {
    using namespace std;
    using namespace tfel::utilities;
    if (this->current == this->line->end()) {
      ostringstream msg;
      msg << "TestLauncherV1::treatMeshCommand : ";
      msg << " unexpected end of line.\n";
      msg << "Error at line " << this->line->begin()->line;
      log.addMessage(msg.str());
      throw(runtime_error(msg.str()));
    }
    if ((this->current->flag != Token::String) &&
        (this->current->flag != Token::Char)) {
      ostringstream msg;
      msg << "TestLauncherV1::treatMeshCommand : ";
      msg << "invalid line " << this->line->begin()->line;
      msg << " (first filename is invalid)";
      log.addMessage(msg.str());
      throw(runtime_error(msg.str()));
    }
    this->meshCommands.push_back(
        this->current->value.substr(1, this->current->value.size() - 2));
    ++(this->current);
  }  // end of TestLauncherV1::treatTestMeshCommand

  void TestLauncherV1::treatTestType(void) {
    using namespace std;
    using namespace tfel::utilities;
    if (this->current == this->line->end()) {
      ostringstream msg;
      msg << "TestLauncherV1::treatTest : ";
      msg << " unexpected end of line.\n";
      msg << "Error at line " << this->line->begin()->line;
      log.addMessage(msg.str());
      throw(runtime_error(msg.str()));
    }  // creates a Comparison based on the word read
    if (this->current->value == "Absolute") {
      this->comparison.reset(new AbsoluteComparison());
    } else if (this->current->value == "Relative") {
      this->comparison.reset(new RelativeComparison());
    } else if (this->current->value == "RelativeAndAbsolute") {
      this->comparison.reset(new RelativeAndAbsoluteComparison());
    } else if (this->current->value == "Mixed") {
      this->comparison.reset(new MixedComparison());
    } else if (this->current->value == "Area") {
      this->comparison.reset(new AreaComparison());
    } else {
      ostringstream msg;
      msg << "TestLauncherV1::treatTest : ";
      msg << " unknown test type (read '" + this->current->value + "').\n";
      msg << "Error at line " << this->line->begin()->line;
      log.addMessage(msg.str());
      throw(runtime_error(msg.str()));
    }
    ++(this->current);
    // testing if when Area is used, we have 'using ColumnName|ColumnNumber'
    // following
    if (this->current == this->line->end()) {
      if (this->comparison->getName() == "area") {
        ostringstream msg;
        msg << "TestLauncherV1::treatTestType : missing argument after '"
            << (this->current - 1)->value << "',";
        msg << " usage : \"TestType Area interpolation "
               "<None|Linear|Spline|LocalSpline> using "
               "<columnName|columnNumber>\").\n";
        msg << "Error at line " << this->line->begin()->line;
        log.addMessage(msg.str());
        throw(runtime_error(msg.str()));
      }
    } else if (this->current->value == "interpolation") {
      ++(this->current);
      if (this->current == this->line->end()) {
        ostringstream msg;
        msg << "TestLauncherV1::treatTestType : missing argument after '"
            << (this->current - 1)->value << "',";
        msg << " usage : \"TestType Area interpolation "
               "<None|Linear|Spline|LocalSpline> using "
               "<columnName|columnNumber>\").\n";
        msg << "Error at line " << this->line->begin()->line;
        log.addMessage(msg.str());
        throw(runtime_error(msg.str()));
      } else {
        if (this->current->value == "Spline") {
          this->integralInterpolation.reset(new SplineInterpolation());
        } else if (this->current->value == "Linear") {
          this->integralInterpolation.reset(new LinearInterpolation());
        } else if (this->current->value == "LocalSpline") {
          this->integralInterpolation.reset(new SplineLocalInterpolation());
        } else if (this->current->value == "None") {
          this->integralInterpolation.reset(new NoInterpolation());
        } else {
          ostringstream msg;
          msg << "TestLauncherV1::treatTestType : ";
          msg << " unknown interpolation type (read '" + this->current->value +
                     "').\n";
          msg << "Error at line " << this->line->begin()->line;
          log.addMessage(msg.str());
          throw(runtime_error(msg.str()));
        }
        ++(this->current);
        if (this->current == this->line->end()) {
          ostringstream msg;
          msg << "TestLauncherV1::treatTestType : missing argument after '"
              << (this->current - 1)->value << "',";
          msg << " usage : \"TestType Area interpolation "
                 "<None|Linear|Spline|LocalSpline> using "
                 "<columnName|columnNumber>\").\n";
          msg << "Error at line " << this->line->begin()->line;
          log.addMessage(msg.str());
          throw(runtime_error(msg.str()));
        }

        else if (this->current->value == "using") {
          ++(this->current);
          if (this->current == this->line->end()) {
            ostringstream msg;
            msg << "TestLauncherV1::treatTestType : missing argument after '"
                << (this->current - 1)->value << "',";
            msg << " usage : \"TestType Area interpolation "
                   "<None|Linear|Spline|LocalSpline> using "
                   "<columnName|columnNumber>\").\n";
            msg << "Error at line " << this->line->begin()->line;
            log.addMessage(msg.str());
            throw(runtime_error(msg.str()));
          }
          if ((this->current->flag == Token::String) ||
              (this->current->flag == Token::Char)) {
            std::string colName =
                this->current->value.substr(1, this->current->value.size() - 2);
            this->colIntegralInterpolated.reset(new Column(colName));
          } else {
            std::istringstream converter(this->current->value);
            unsigned short c;
            converter >> c;
            if (!converter && (!converter.eof())) {
              std::ostringstream msg;
              msg << "TestLauncherV1::treatTestType : ";
              msg << "could not read value from token (" << this->current->value
                  << ").\n";
              msg << "Error ";
              log.addMessage(msg.str());
              throw(std::runtime_error(msg.str()));
            }
            this->colIntegralInterpolated.reset(new Column(c));
          }
        }
      }
      ++(this->current);
    }
  }  // end of TestLauncherV1::treatTestType

  void TestLauncherV1::treatTest(void) {
    using namespace std;
    using namespace tfel::utilities;
    if (this->current == this->line->end()) {
      ostringstream msg;
      msg << "TestLauncherV1::treatTest : ";
      msg << " unexpected end of line.\n";
      msg << "Error at line " << this->line->begin()->line;
      log.addMessage(msg.str());
      throw(runtime_error(msg.str()));
    }
    if (this->current->flag != Token::String) {
      ostringstream msg;
      msg << "TestLauncherV1::treatTest : ";
      msg << "invalid line " << this->line->begin()->line;
      msg << " (first filename is invalid, read '" + this->current->value +
                 "')";
      log.addMessage(msg.str());
      throw(runtime_error(msg.str()));
    }
    const auto f1 =
        this->current->value.substr(1, this->current->value.size() - 2);
    ++(this->current);
    if (this->current == this->line->end()) {
      ostringstream msg;
      msg << "TestLauncherV1::treatTest : ";
      msg << " unexpected end of line.\n";
      msg << "Error at line " << this->line->begin()->line;
      log.addMessage(msg.str());
      throw(runtime_error(msg.str()));
    }
    if (this->current->flag != Token::String) {
      ostringstream msg;
      msg << "TestLauncherV1::treatTest : ";
      msg << "invalid line " << this->line->begin()->line;
      msg << " (second filename is invalid, read " + this->current->value + ")";
      log.addMessage(msg.str());
      throw(runtime_error(msg.str()));
    }
    const auto f2 =
        this->current->value.substr(1, this->current->value.size() - 2);
    ++(this->current);
    if (this->current == this->line->end()) {
      ostringstream msg;
      msg << "TestLauncherV1::treatTest : ";
      msg << " unexpected end of line.\n";
      msg << "Error at line " << this->line->begin()->line;
      log.addMessage(msg.str());
      throw(runtime_error(msg.str()));
    }
    while (this->current != this->line->end()) {
      auto test = std::make_shared<Test>();
      test->setComparison(comparison);
      test->setPrec(this->prec);
      test->setPrecision2(this->precision2);

      test->setInterpolation(this->interpolation->clone());
      test->setIntegralInterpolation(this->integralInterpolation);

      if (test->getInterpolation()
              ->isConform())  // fetch the times if doing an interpolation
        test->setColInterpolated(this->ci);

      test->setColIntegralInterpolated(
          this->colIntegralInterpolated);  // fetch the times for an integration
      test->setAllowLessResults(this->allowLessResults);

      std::shared_ptr<Column> c1Tmp;
      std::shared_ptr<Column> c2Tmp;

      // fetching the column's name to compare
      if ((this->current->flag == Token::String) ||
          (this->current->flag == Token::Char)) {
        std::string colName =
            this->current->value.substr(1, this->current->value.size() - 2);
        c1Tmp.reset(new Column(colName));
        c2Tmp.reset(new Column(colName));
      } else {  // or number if it was given by number
        std::istringstream converter(this->current->value);
        unsigned short c;
        converter >> c;
        if (!converter && (!converter.eof())) {
          std::ostringstream msg;
          msg << "TestLauncherV1::treatTest : ";
          msg << "could not read value from token (" << this->current->value
              << ").\n";
          msg << "Error ";
          log.addMessage(msg.str());
          throw(std::runtime_error(msg.str()));
        }
        c1Tmp.reset(new Column(c));
        c2Tmp.reset(new Column(c));
      }

      // setting the columns
      test->setColA(c1Tmp);
      test->setColB(c2Tmp);

      // sets the files, and opens them to fetch the values
      test->setFileA(f1);
      test->setFileB(f2);

      this->comparisons.push_back(*test);
      ++(this->current);
    }
  }  // end of TestLauncherV1::treatTest

  void TestLauncherV1::treatPrecision(void) {
    using namespace std;
    this->prec = 1.e-8;
    this->precision2 = 0.;
    auto converter = [](PCLogger& os, const std::string& s,
                        const unsigned int ln) {
      istringstream c(s);
      double v;
      c >> v;
      if (!c && (!c.eof())) {
        ostringstream msg;
        msg << "TestLauncherV1::treatPrecision : ";
        msg << "could not read value from token (" << s << ").\n";
        msg << "Error at line : " << ln;
        os.addMessage(msg.str());
        throw(runtime_error(msg.str()));
      }
      return v;
    };
    if (this->current == this->line->end()) {
      ostringstream msg;
      msg << "TestLauncherV1::treatPrecision : ";
      msg << " unexpected end of line.\n";
      msg << "Error at line " << this->line->begin()->line;
      log.addMessage(msg.str());
      throw(runtime_error(msg.str()));
    }
    this->prec =
        converter(this->log, this->current->value, this->line->begin()->line);
    ++(this->current);
    // Optional second precision
    if (this->current != this->line->end()) {
      this->precision2 =
          converter(this->log, this->current->value, this->line->begin()->line);
      ++(this->current);
    }
  }  // end of TestLauncherV1::treatPrecision

  void TestLauncherV1::treatInterpolation(void) {
    using namespace std;
    using namespace tfel::utilities;
    if (this->current == this->line->end()) {
      ostringstream msg;
      msg << "TestLauncherV1::treatInterpolation : ";
      msg << " unexpected end of line.\n";
      msg << "Error at line " << this->line->begin()->line;
      log.addMessage(msg.str());
      throw(runtime_error(msg.str()));
    }
    if (this->current->value == "Spline") {
      this->interpolation.reset(new SplineInterpolation());
    } else if (this->current->value == "Linear") {
      this->interpolation.reset(new LinearInterpolation());
    } else if (this->current->value == "LocalSpline") {
      this->interpolation.reset(new SplineLocalInterpolation());
    } else if (this->current->value == "None") {
      this->interpolation.reset(new NoInterpolation());
    } else {
      ostringstream msg;
      msg << "TestLauncherV1::treatInterpolation : ";
      msg << " unknown interpolation type (read '" + this->current->value +
                 "').\n";
      msg << "Error at line " << this->line->begin()->line;
      log.addMessage(msg.str());
      throw(runtime_error(msg.str()));
    }
    ++(this->current);
    if (this->current == this->line->end()) {
      if (this->interpolation->isConform()) {
        ostringstream msg;
        msg << "TestLauncherV1::treatInterpolation : missing argument after '"
            << (this->current - 1)->value << "',";
        msg << " usage : \"Interpolation <None|Linear|Spline|LocalSpline> "
               "using <columnName|columnNumber> [AllowLessResults]\").\n";
        msg << "Error at line " << this->line->begin()->line;
        log.addMessage(msg.str());
        throw(runtime_error(msg.str()));
      }
    } else if (this->current->value == "using") {
      ++(this->current);
      if (this->current == this->line->end()) {
        ostringstream msg;
        msg << "TestLauncherV1::treatInterpolation : missing argument after '"
            << (this->current - 1)->value << "',";
        msg << " usage : \"Interpolation <None|Linear|Spline|LocalSpline> "
               "using <columnName|columnNumber> [AllowLessResults]\").\n";
        msg << "Error at line " << this->line->begin()->line;
        log.addMessage(msg.str());
        throw(runtime_error(msg.str()));
      }
      if ((this->current->flag == Token::String) ||
          (this->current->flag == Token::Char)) {
        std::string colName =
            this->current->value.substr(1, this->current->value.size() - 2);
        this->ci.reset(new Column(colName));
      } else {
        std::istringstream converter(this->current->value);
        unsigned short c;
        converter >> c;
        if (!converter && (!converter.eof())) {
          std::ostringstream msg;
          msg << "TestLauncherV1::treatTest : ";
          msg << "could not read value from token (" << this->current->value
              << ").\n";
          msg << "Error ";
          log.addMessage(msg.str());
          throw(std::runtime_error(msg.str()));
        }
        this->ci.reset(new Column(c));
      }

      ++(this->current);

      if (this->current != this->line->end()) {
        if (this->current->value == "AllowLessResults") {
          this->allowLessResults = true;
          ++(this->current);
        } else {
          unsigned short c;
          istringstream converter(this->current->value);
          converter >> c;
          if (!converter && (!converter.eof())) {
            ostringstream msg;
            msg << "TestLauncherV1::treatInterpolation : ";
            msg << "could not read value from token (" << this->current->value
                << ", expected \"AllowLessResults\").\n";
            msg << "Error at line : " << this->line->begin()->line;
            log.addMessage(msg.str());
            throw(runtime_error(msg.str()));
          }
        }
      }
    } else {
      ostringstream msg;
      msg << "TestLauncherV1::treatInterpolation : ";
      msg << " could not read argument '" << this->current->value << "'.\n";
      msg << "Error at line " << this->line->begin()->line;
      log.addMessage(msg.str());
      throw(runtime_error(msg.str()));
    }

  }  // end of treatInterpolation

  void TestLauncherV1::analyseInputFile(void) {
    using namespace std;
    map<string, CallBack>::const_iterator p;
    for (this->line = this->tokens.begin(); this->line != this->tokens.end();
         ++(this->line)) {
      this->current = this->line->begin();
      assert(this->current != this->line->end());
      p = this->callBacks.find(this->current->value);
      if (p == this->callBacks.end()) {
        ostringstream msg;
        msg << "TestLauncherV1::analyseInputFile : ";
        msg << "unknown keyword " << this->current->value << ".\n";
        msg << "Error at line " << this->line->begin()->line;
        log.addMessage(msg.str());
        throw(runtime_error(msg.str()));
      }
      ++(this->current);
      (this->*(p->second))();
      if (this->current != this->line->end()) {
        ostringstream msg;
        msg << "TestLauncherV1::analyseInputFile : ";
        msg << "some tokens remains on line " << this->line->begin()->line
            << " after treatment.";
        log.addMessage(msg.str());
        throw(runtime_error(msg.str()));
      }
    }
    // a bit of clean-up
    this->tokens.clear();
  }  // end of TestLauncherV1::analyseInputFile

  double TestLauncherV1::ClockAction(ClockEventType clockevent) {
    using namespace std;
    clock_t r;

    switch (clockevent) {
      case START:
        r = times(&this->timeStart);
        if (r == static_cast<clock_t>(-1))
          throw(runtime_error(
              "Failed times system call in TestLauncherV1::ClockAction"));
        break;
      case STOP:
        r = times(&this->timeStop);
        if (r == static_cast<clock_t>(-1))
          throw(runtime_error(
              "Failed times system call in TestLauncherV1::ClockAction"));
        break;
      case GET:
        return (this->timeStop.tms_cutime - this->timeStart.tms_cutime) * 1.0 /
               sysconf(_SC_CLK_TCK);
      default:
        throw(runtime_error("Wrong clockevent in TestLauncherV1::ClockAction"));
    }
    return 0.;
  }

  bool TestLauncherV1::execute(void) {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::utilities;
    ProcessManager manager;
    vector<Test>::iterator p;
    map<string, vector<Test>>::iterator p2;
    vector<Test>::iterator p3;
    vector<string>::const_iterator p4;

    bool gsuccess = true; /* global success indicator : false only
                             if comparison fails or when command
                             fails if no comparison are given*/
    bool success = true;  // current test success indicator
    string step;

    unsigned short i = 1;

    // Mesh
    if (!this->meshCommands.empty()) {
      for (p4 = this->meshCommands.begin(); p4 != this->meshCommands.end();
           ++p4) {
        ostringstream oss;
        oss << i;
        step = "Mesh-" + oss.str();

        success = true;
        try {
          this->ClockAction(START);
          manager.execute(*p4, "", this->testname + "-" + step + ".out",
                          this->environments);
          this->ClockAction(STOP);
          log.addTestResult(this->testname, step, *p4, this->ClockAction(GET),
                            true);
        } catch (exception& e) {
          log.addTestResult(this->testname, step, *p4, 0.0, false, e.what());
          if (this->comparisons.empty()) {
            gsuccess = false;
          }
          success = false;
        }
        glog.addSimpleTestResult("** " + step + " " + *p4, success);
        i++;
      }
    }

    i = 1;
    // Execute
    if (!this->commands.empty()) {
      for (p4 = this->commands.begin(); p4 != this->commands.end(); ++p4) {
        ostringstream oss;
        oss << i;
        step = "Exec-" + oss.str();
        success = true;
        try {
          this->ClockAction(START);
          manager.execute(*p4, "", this->testname + "-" + step + ".out",
                          this->environments);
          this->ClockAction(STOP);
          log.addTestResult(this->testname, step, *p4, this->ClockAction(GET),
                            true);
        } catch (exception& e) {
          log.addTestResult(this->testname, step, *p4, 0.0, false, e.what());
          if (this->comparisons.empty()) {
            gsuccess = false;
          }
          success = false;
        }
        glog.addSimpleTestResult("** " + step + " " + *p4, success);
        i++;
      }
    }

    i = 1;
    // Compare
    for (auto& c : this->comparisons) {
      ostringstream msg;
      msg.str("");  // wipe stream
      success = true;
      ostringstream oss;
      oss << i;
      step = "Compare-" + oss.str();
      try {
        c.applyInterpolation();
        c.compare();
        success = c.hasSucceed();
        gsuccess = (gsuccess == false ? false : success);
        msg << c.getMsgLog();
        log.addTestResult(this->testname, step,
                          string("Comparing '") + c.getFileA() + "' and '" +
                              c.getFileB() + "'",
                          0.0, success, msg.str());
        // success = true;
        i++;

      } catch (exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        log.addTestResult(this->testname, step,
                          string("Comparing '") + c.getFileA() + "' and '" +
                              c.getFileB() + "'",
                          0.0, false, msg.str() + "\n" + e.what());
        gsuccess = false;
        success = false;
        i++;
      }
      glog.addSimpleTestResult("** " + step + " '" + c.getFileA() + "' and '" +
                                   c.getFileB() + "', column '" +
                                   c.getColA()->getName() + "' ",
                               success);
    }
    return gsuccess;
  }  // end of TestLauncherV1::execute

  TestLauncherV1::~TestLauncherV1() { log.terminate(); }
}  // end of namespace tfel_check
