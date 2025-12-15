/*!
 * \file   tfel-check/src/TestLauncher.cxx
 * \brief  Class to manage tfel unit tests.
 * \author Thomas Helfer
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

#include "TFEL/Raise.hxx"
#include "TFEL/System/ProcessManager.hxx"
#include "TFEL/Utilities/TerminalColors.hxx"

#include "TFEL/Check/TestLauncher.hxx"
#include "TFEL/Check/PCLogger.hxx"
#include "TFEL/Check/PCTextDriver.hxx"
#include "TFEL/Check/PCJUnitDriver.hxx"
#include "TFEL/Check/SplineInterpolation.hxx"
#include "TFEL/Check/SplineLocalInterpolation.hxx"
#include "TFEL/Check/LinearInterpolation.hxx"
#include "TFEL/Check/NoInterpolation.hxx"
#include "TFEL/Check/AbsoluteComparison.hxx"
#include "TFEL/Check/RelativeComparison.hxx"
#include "TFEL/Check/RelativeAndAbsoluteComparison.hxx"
#include "TFEL/Check/MixedComparison.hxx"
#include "TFEL/Check/AreaComparison.hxx"

namespace tfel {

  namespace check {

    TestLauncher::TestLauncher(const std::string& f, const PCLogger& l)
        : glog(l),
          file(f),
          comparison(new AbsoluteComparison()),
          interpolation(new NoInterpolation()),
          integralInterpolation(new NoInterpolation()) {
      this->testname = this->file.substr(0, this->file.rfind("."));
      this->logfile = this->testname + ".checklog";
      this->log = PCLogger(std::make_shared<PCTextDriver>(this->logfile));
      this->log.addDriver(
          std::make_shared<PCJUnitDriver>("TEST-" + this->testname + ".xml"));
      this->ClockAction(START);
      this->ClockAction(STOP);
      this->ClockAction(GET);
      this->treatCharAsString(true);
      this->mergeStrings(false);
      // open the file
      this->openFile(this->file);
      // strip any C/C++ comments
      this->stripComments();
      // callbacks
      this->registerCallBack("@Environment", &TestLauncher::treatEnvironment);
      this->registerCallBack("@Command", &TestLauncher::treatCommand);
      this->registerCallBack("@Test", &TestLauncher::treatTest);
      this->registerCallBack("@TestType", &TestLauncher::treatTestType);
      this->registerCallBack("@Precision", &TestLauncher::treatPrecision);
      this->registerCallBack("@Interpolation",
                             &TestLauncher::treatInterpolation);
      this->analyseInputFile();
    }  // end of TestLauncher::TestLauncher

    void TestLauncher::registerCallBack(const std::string& name,
                                        const CallBack& f) {
      raise_if(!this->callBacks.insert({name, f}).second,
               "TestLauncher::registerCallBack: "
               "keyword '" +
                   name + "' has already been registred");
    }  // end of TestLauncher::registerCallBack

    void TestLauncher::throwRuntimeError(const std::string& m,
                                         const std::string& msg) {
      const auto e = [this, &m, &msg] {
        auto error = this->file;
        if (this->current != this->end()) {
          error += ':' + std::to_string(this->current->line);
          error += ':' + std::to_string(this->current->offset);
        } else if (!this->tokens.empty()) {
          const auto pt = std::prev(this->current);
          error += ':' + std::to_string(pt->line);
          error += ':' + std::to_string(pt->offset);
        }
        error += ": error: " + msg + " (" + m + ")";
        return error;
      }();
      this->log.addMessage(e);
      raise(e);
    }  // end of TestLauncher::throwRuntimeError

    void TestLauncher::checkNotEndOfFile(const std::string& m) {
      if (this->current == this->end()) {
        this->throwRuntimeError(m, "unexpected end of file");
      }
    }

    void TestLauncher::readSpecifiedToken(const std::string& m,
                                          const std::string& v) {
      this->checkNotEndOfFile(m);
      if (this->current->value != v) {
        this->throwRuntimeError(m, "unexpected token '" + this->current->value +
                                       "',"
                                       " expected '" +
                                       v + "'");
      }
      ++(this->current);
    }

    std::string TestLauncher::readString(const std::string& m) {
      this->checkNotEndOfFile(m);
      if (((this->current->flag != tfel::utilities::Token::String) &&
           (this->current->flag != tfel::utilities::Token::Char))) {
        this->throwRuntimeError(m, "expected a string");
      }
      const auto v =
          this->current->value.substr(1, this->current->value.size() - 2);
      ++(this->current);
      return v;
    }

    void TestLauncher::treatEnvironment() {
      this->readSpecifiedToken("TestLauncher::treatEnvironment", "{");
      this->checkNotEndOfFile("TestLauncher::treatEnvironment");
      while (this->current->value != "}") {
        const auto key = this->readString("TestLauncher::treatEnvironment");
        this->readSpecifiedToken("TestLauncher::treatEnvironment", ":");
        const auto value = this->readString("TestLauncher::treatEnvironment");
        this->checkNotEndOfFile("TestLauncher::treatEnvironment");
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
        if (this->current->value != "}") {
          this->readSpecifiedToken("TestLauncher::treatEnvironment", ",");
          this->checkNotEndOfFile("TestLauncher::treatEnvironment");
          if (this->current->value == "}") {
            this->throwRuntimeError(
                "TestLauncher::treatEnvironment",
                "unexpected token '" + this->current->value + "'");
          }
        }
      }
      ++(this->current);
      this->readSpecifiedToken("TestLauncher::treatEnvironment", ";");
    }  // end of TestLauncher::treatEnvironment

    void TestLauncher::treatCommand() {
      const auto c = this->readString("TestLauncher::treatCommand");
      this->commands.push_back(c);
      this->readSpecifiedToken("TestLauncher::treatCommand", ";");
    }  // end of TestLauncher::treatCommand

    void TestLauncher::treatTestType() {
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
        this->throwRuntimeError("TestLauncher::treatTest",
                                "unknown test type "
                                "(read '" +
                                    this->current->value + "').");
      }
      ++(this->current);
      if (this->comparison->getName() != "area") {
        this->readSpecifiedToken("TestLauncher::treatTest", ";");
        return;
      }
      auto check = [this] {
        if (this->current == this->end()) {
          this->throwRuntimeError("TestLauncher::treatTestType",
                                  "missing argument after '" +
                                      std::prev(this->current)->value +
                                      "'\n"
                                      "Usage : \"TestType Area interpolation "
                                      "<None|Linear|Spline|LocalSpline> "
                                      "using <columnName|columnNumber>\"");
        }
      };
      check();
      this->readSpecifiedToken("TestLauncher::treatTest", "interpolation");
      check();
      this->setInterpolation("TestLauncher::treatTest");
      this->readSpecifiedToken("TestLauncher::treatTestType", "using");
      check();
      if ((this->current->flag == tfel::utilities::Token::String) ||
          (this->current->flag == tfel::utilities::Token::Char)) {
        const auto c = this->readString("TestLauncher::treatTestType");
        this->colIntegralInterpolated.reset(new Column(c));
      } else {
        const auto c =
            CxxTokenizer::readUnsignedInt(this->current, this->end());
        this->colIntegralInterpolated.reset(new Column(c));
      }
      this->readSpecifiedToken("TestLauncher::treatTestType", ";");
    }  // end of TestLauncher::treatTestType

    void TestLauncher::treatTest() {
      auto getColumn = [this] {
        // fetching the column's name to compare
        if ((this->current->flag == tfel::utilities::Token::String) ||
            (this->current->flag == tfel::utilities::Token::Char)) {
          const auto c = this->readString("TestLauncher::treatTest");
          return std::make_shared<Column>(c);
        } else {  // or number if it was given by number
          const auto c =
              CxxTokenizer::readUnsignedInt(this->current, this->end());
          return std::make_shared<Column>(c);
        }
      };
      auto make_test = [this] {
        auto test = std::make_shared<Test>();
        test->setComparison(this->comparison);
        test->setPrec(this->prec);
        test->setPrecision2(this->precision2);
        test->setInterpolation(this->interpolation->clone());
        test->setIntegralInterpolation(this->integralInterpolation);
        if (test->getInterpolation()->isConform()) {
          // fetch the times if doing an interpolation
          test->setColInterpolated(this->ci);
        }
        // fetch the times for an integration
        test->setColIntegralInterpolated(this->colIntegralInterpolated);
        test->setAllowLessResults(this->allowLessResults);
        return test;
      };
      this->checkNotEndOfFile("TestLauncher::treatTest");
      if (this->current->value == "{") {
        auto getFileAndColumn = [this, &getColumn] {
          this->readSpecifiedToken("TestLauncher::treatTest", "{");
          const auto f = this->readString("TestLauncher::treatTest");
          this->readSpecifiedToken("TestLauncher::treatTest", ":");
          const auto c = getColumn();
          this->readSpecifiedToken("TestLauncher::treatTest", "}");
          return std::make_pair(f, c);
        };
        const auto p1 = getFileAndColumn();
        const auto p2 = getFileAndColumn();
        auto test = make_test();
        test->setFileA(p1.first);
        test->setColA(p1.second);
        test->setFileB(p2.first);
        test->setColB(p2.second);
        this->comparisons.push_back(*test);
      } else {
        // standard test
        const auto f1 = this->readString("TestLauncher::treatTest");
        const auto f2 = this->readString("TestLauncher::treatTest");
        this->checkNotEndOfFile("TestLauncher::treatTest");
        if (this->current->value == ";") {
          this->throwRuntimeError("TestLauncher::treatTest",
                                  "unexpected token ';'");
        }
        while (this->current->value != ";") {
          auto first = getColumn();
          --(this->current);
          auto second = getColumn();
          auto test = make_test();
          // setting the columns
          test->setColA(first);
          test->setColB(second);
          // sets the files, and opens them to fetch the values
          test->setFileA(f1);
          test->setFileB(f2);
          this->comparisons.push_back(*test);
        }
      }
      this->readSpecifiedToken("TestLauncher::treatTest", ";");
    }  // end of TestLauncher::treatTest

    void TestLauncher::treatPrecision() {
      this->prec = CxxTokenizer::readDouble(this->current, this->end());
      this->checkNotEndOfFile("TestLauncher::treatPrecision");
      if (this->current->value != ";") {
        // Optional second precision
        this->precision2 = CxxTokenizer::readDouble(this->current, this->end());
      }
      this->readSpecifiedToken("TestLauncher::treatPrecision", ";");
    }  // end of TestLauncher::treatPrecision

    void TestLauncher::setInterpolation(const std::string& m) {
      this->checkNotEndOfFile(m);
      if (this->current->value == "Spline") {
        this->integralInterpolation.reset(new SplineInterpolation());
      } else if (this->current->value == "Linear") {
        this->integralInterpolation.reset(new LinearInterpolation());
      } else if (this->current->value == "LocalSpline") {
        this->integralInterpolation.reset(new SplineLocalInterpolation());
      } else if (this->current->value == "None") {
        this->integralInterpolation.reset(new NoInterpolation());
      } else {
        this->throwRuntimeError(m,
                                "unknown interpolation type "
                                "(read '" +
                                    this->current->value + "')");
      }
      ++(this->current);
    }

    void TestLauncher::treatInterpolation() {
      this->setInterpolation("TestLauncher::treatInterpolation");
      this->checkNotEndOfFile("TestLauncher::treatInterpolation");
      if (this->current->value == ";") {
        if (this->interpolation->isConform()) {
          this->throwRuntimeError(
              "TestLauncher::treatInterpolation",
              "missing argument after '" + std::prev(this->current)->value +
                  "'\n"
                  "Usage : \"Interpolation <None|Linear|Spline|LocalSpline> "
                  "using <columnName|columnNumber> [AllowLessResults]\"");
        }
        this->readSpecifiedToken("TestLauncher::treatInterpolation", ";");
        return;
      }
      this->readSpecifiedToken("TestLauncher::treatInterpolation", "using");
      this->checkNotEndOfFile("TestLauncher::treatInterpolation");
      if ((this->current->flag == tfel::utilities::Token::String) ||
          (this->current->flag == tfel::utilities::Token::Char)) {
        const auto c = this->readString("TestLauncher::treatInterpolation");
        this->ci.reset(new Column(c));
      } else {
        const auto c =
            CxxTokenizer::readUnsignedInt(this->current, this->end());
        this->ci.reset(new Column(c));
      }
      this->checkNotEndOfFile("TestLauncher::treatInterpolation");
      if (this->current->value != ";") {
        this->readSpecifiedToken("TestLauncher::treatInterpolation",
                                 "AllowLessResults");
        this->allowLessResults = true;
      }
      this->readSpecifiedToken("TestLauncher::treatInterpolation", ";");
    }  // end of treatInterpolation

    void TestLauncher::analyseInputFile() {
      this->current = this->begin();
      while (this->current != this->end()) {
        auto p = this->callBacks.find(this->current->value);
        if (p == this->callBacks.end()) {
          this->throwRuntimeError(
              "TestLauncher::analyseInputFile",
              "unknown keyword '" + this->current->value + "'");
        }
        ++(this->current);
        (this->*(p->second))();
      }
      // a bit of clean-up
      this->clear();
    }  // end of TestLauncher::analyseInputFile

    double TestLauncher::ClockAction(ClockEventType clockevent) {
      using namespace std;
      clock_t r;

      switch (clockevent) {
        case START:
          r = times(&this->timeStart);
          raise_if(r == static_cast<clock_t>(-1),
                   "Failed times system call in "
                   "TestLauncher::ClockAction");
          break;
        case STOP:
          r = times(&this->timeStop);
          raise_if(r == static_cast<clock_t>(-1),
                   "Failed times system call in "
                   "TestLauncher::ClockAction");
          break;
        case GET:
          return (this->timeStop.tms_cutime - this->timeStart.tms_cutime) *
                 1.0 / sysconf(_SC_CLK_TCK);
        default:
          raise(
              "Wrong clockevent in "
              "TestLauncher::ClockAction");
      }
      return 0.;
    }

    bool TestLauncher::execute() {
      using namespace tfel::utilities;
      tfel::system::ProcessManager manager;
      bool gsuccess = true; /* global success indicator : false only
                               if comparison fails or when command
                               fails if no comparison are given*/
      bool success = true;  // current test success indicator
      unsigned short i = 1;
      // Execute
      for (const auto& c : this->commands) {
        const auto step = "Exec-" + std::to_string(i);
        success = true;
        try {
          this->ClockAction(START);
          manager.execute(c, "", this->testname + "-" + step + ".out",
                          this->environments);
          this->ClockAction(STOP);
          this->log.addTestResult(this->testname, step, c,
                                  this->ClockAction(GET), true);
        } catch (std::exception& e) {
          this->log.addTestResult(this->testname, step, c, 0.0, false,
                                  e.what());
          if (this->comparisons.empty()) {
            gsuccess = false;
          }
          success = false;
        }
        this->glog.addSimpleTestResult("** " + step + " " + c, success);
        i++;
      }
      i = 1;
      // Compare
      for (auto& c : this->comparisons) {
        const auto step = "Compare-" + std::to_string(i);
        success = true;
        try {
          c.applyInterpolation();
          c.compare();
          success = c.hasSucceed();
          gsuccess = (gsuccess == false ? false : success);
          this->log.addTestResult(
              this->testname, step,
              "Comparing '" + c.getFileA() + "' and '" + c.getFileB() + "'",
              0.0, success, c.getMsgLog());
          i++;
        } catch (std::exception& e) {
          this->log.addTestResult(
              this->testname, step,
              "Comparing '" + c.getFileA() + "' and '" + c.getFileB() + "'",
              0.0, false, e.what());
          gsuccess = false;
          success = false;
          i++;
        }
        this->glog.addSimpleTestResult(
            "** " + step + " '" + c.getFileA() + "' and '" + c.getFileB() +
                "', column '" + c.getColA()->getName() + "' ",
            success);
      }
      return gsuccess;
    }  // end of TestLauncher::execute

    TestLauncher::~TestLauncher() { this->log.terminate(); }

  }  // end of namespace check

}  // end of namespace tfel
