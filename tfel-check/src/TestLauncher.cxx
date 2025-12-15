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
#include <regex>
#include <unistd.h>  // sysconf

#include "TFEL/Raise.hxx"
#include "TFEL/System/System.hxx"
#include "TFEL/System/ProcessManager.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Utilities/TerminalColors.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"

#include "TFEL/Check/Configuration.hxx"
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
#include "TFEL/Check/TestLauncher.hxx"

namespace tfel::check {

  TestLauncher::Command::Command() = default;
  TestLauncher::Command::Command(Command&&) = default;
  TestLauncher::Command::Command(const Command&) = default;
  TestLauncher::Command&  //
  TestLauncher::Command::operator=(Command&&) = default;
  TestLauncher::Command&  //
  TestLauncher::Command::operator=(const Command&) = default;
  TestLauncher::Command::~Command() = default;

  TestLauncher::TestLauncher(const Configuration& c, const std::string& f)
      : glog(c.log),
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
    // callbacks
    this->registerCallBack("@Requires", &TestLauncher::treatRequires);
    this->registerCallBack("@Environment", &TestLauncher::treatEnvironment);
    this->registerCallBack("@Command", &TestLauncher::treatCommand);
    this->registerCallBack("@Test", &TestLauncher::treatTest);
    this->registerCallBack("@TestType", &TestLauncher::treatTestType);
    this->registerCallBack("@Precision", &TestLauncher::treatPrecision);
    this->registerCallBack("@Interpolation", &TestLauncher::treatInterpolation);
    this->registerCallBack("@CleanFiles", &TestLauncher::treatCleanFiles);
    this->registerCallBack("@CleanDirectories",
                           &TestLauncher::treatCleanDirectories);
    this->analyseInputFile(c);
  }  // end of TestLauncher

  void TestLauncher::registerCallBack(const std::string& name,
                                      const CallBack& f) {
    raise_if(!this->callBacks.insert({name, f}).second,
             "TestLauncher::registerCallBack: "
             "keyword '" +
                 name + "' has already been registred");
  }  // end of registerCallBack

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
  }  // end of throwRuntimeError

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
      this->throwRuntimeError(
          m, "expected a string, read '" + this->current->value + "'");
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
  }  // end of treatEnvironment

  void TestLauncher::treatRequires() {
    const auto r = CxxTokenizer::readStringArray(this->current, this->end());
    this->requirements.insert(this->requirements.end(), r.begin(), r.end());
    this->readSpecifiedToken("TestLauncher::treatRequires", ";");
  }  // end of treatRequires

  void TestLauncher::treatCleanFiles() {
    const auto files =
        CxxTokenizer::readStringArray(this->current, this->end());
    for (const auto& f : files) {
      if (std::find(this->cleanfiles.begin(), this->cleanfiles.end(), f) !=
          this->cleanfiles.end()) {
        this->throwRuntimeError("TestLauncher::treatCleanFiles",
                                "file '" + f + "' multiply declared");
      }
      this->cleanfiles.push_back(f);
    }
    this->readSpecifiedToken("TestLauncher::treatCleanFiles", ";");
  }  // end of treatCleanFiles

  void TestLauncher::treatCleanDirectories() {
    const auto directories =
        CxxTokenizer::readStringArray(this->current, this->end());
    for (const auto& d : directories) {
      if (std::find(this->cleandirectories.begin(),
                    this->cleandirectories.end(),
                    d) != this->cleandirectories.end()) {
        this->throwRuntimeError("TestLauncher::treatCleanDirectories",
                                "directory '" + d + "' multiply declared");
      }
      this->cleandirectories.push_back(d);
    }
    this->readSpecifiedToken("TestLauncher::treatCleanDirectories", ";");
  }  // end of treatCleanDirectories

  static std::function<
      std::tuple<bool, std::string>(const std::vector<std::string>&)>
  generateEmptyOutputCheck(const tfel::utilities::Data& option) {
    if (!option.is<bool>()) {
      tfel::raise(
          "TestLaucher::treatCommand: invalid type "
          "for option 'empty_output'");
    }
    const auto value = option.get<bool>();
    return [value](const std::vector<std::string>& output)
               -> std::tuple<bool, std::string> {
      if ((value) && (output.empty())) {
        return {true, ""};
      }
      auto msg = std::string{"Unexpected command output "};
      if (value) {
        msg += "(empty output expected)";
        msg += "# Command output:\n\n";
        for (const auto& l : output) {
          msg += "'" + l + "'\n";
        }
      } else {
        msg += "(non empty output expected)";
      }
      return {false, msg};
    };
  }  // end of generateEmptyOutputCheck

  static std::function<
      std::tuple<bool, std::string>(const std::vector<std::string>&)>
  generateExpectedOutputCheck(const tfel::utilities::Data& option) {
    if (option.is<std::string>()) {
      return [value = option.get<std::string>()](
                 const std::vector<std::string>& output)
                 -> std::tuple<bool, std::string> {
        if ((output.size() == 1u) && (output[0] == value)) {
          return {true, ""};
        }
        auto msg = std::string{"Unexpected command output.\n"};
        msg += "# Expected output:\n\n'" + value + "'\n\n";
        msg += "# Command output:\n\n";
        for (const auto& l : output) {
          msg += "'" + l + "'\n";
        }
        return {false, msg};
      };
    } else if (option.is<std::vector<tfel::utilities::Data>>()) {
      const auto values =
          tfel::utilities::convert<std::vector<std::string>>(option);
      return [values](const std::vector<std::string>& output)
                 -> std::tuple<bool, std::string> {
        if ((output.size() == values.size()) &&
            (std::equal(output.begin(), output.end(), values.begin()))) {
          return {true, ""};
        }
        auto msg = std::string{"Unexpected command output.\n"};
        msg += "# Expected output:\n\n";
        for (const auto& l : values) {
          msg += "'" + l + "'\n";
        }
        msg += "\n# Command output:\n\n";
        for (const auto& l : output) {
          msg += "'" + l + "'\n";
        }
        msg += "\n# Detailled comparison:\n\n";
        const auto n = std::min(output.size(), values.size());
        for (std::size_t i = 0; i != n; ++i) {
          if (output[i] != values[i]) {
            msg += "- '" + output[i] + "' does not match '" + values[i] + "'\n";
          }
        }
        return {false, msg};
      };
    }
    tfel::raise(
        "TestLaucher::treatCommand: invalid type "
        "for option 'expected_output'");
  }  // end of generateExpectedOutputCheck

  static std::function<
      std::tuple<bool, std::string>(const std::vector<std::string>&)>
  generateRegularExpressionOutputCheck(const tfel::utilities::Data& option) {
    if (!option.is<std::string>()) {
      tfel::raise(
          "TestLaucher::treatCommand: invalid type "
          "for option 'output_validation_regex'");
    }
    try {
      const auto value = option.get<std::string>();
      std::regex e(value);
      return [value, e](const std::vector<std::string>& output)
                 -> std::tuple<bool, std::string> {
        auto content = std::string{};
        if (!output.empty()) {
          for (const auto& l : output) {
            content += l + '\n';
          }
          content.pop_back();
        }
        if (std::regex_match(content, e)) {
          return {true, ""};
        }
        return {false,
                "output does not match regular expression '" + value + "'"};
      };
    } catch (std::exception& e) {
      tfel::raise(
          "TestLaucher::treatCommand: "
          "error while building regular expression (" +
          std::string{e.what()} + ")");
    } catch (...) {
      tfel::raise(
          "TestLaucher::treatCommand: "
          "error while building regular expression (unknown exception)");
    }
  }  // end of generateRegularExpressionOutputCheck

  static std::function<
      std::tuple<bool, std::string>(const std::vector<std::string>&)>
  generateExpectedNumericalOutputCheck(const tfel::utilities::Data& option) {
    if (!option.is<tfel::utilities::DataMap>()) {
      tfel::raise(
          "TestLaucher::treatCommand: invalid type "
          "for option 'expected_numerical_output'");
    }
    const auto& m = option.get<tfel::utilities::DataMap>();
    if ((m.size() != 2u) || (m.count("value") == 0) ||
        (m.count("criterion_value") == 0) || (!m.at("value").is<double>()) ||
        (!m.at("criterion_value").is<double>())) {
      tfel::raise(
          "TestLaucher::treatCommand: invalid options passed "
          "to 'expected_numerical_output'. Expected two real "
          "numbers named 'value' and 'criterion_value'");
    }
    const auto value = m.at("value").get<double>();
    const auto eps = m.at("criterion_value").get<double>();
    if (eps < 0) {
      tfel::raise(
          "TestLaucher::treatCommand: "
          "invalid criterion value");
    }
    return [value, eps](const std::vector<std::string>& output)
               -> std::tuple<bool, std::string> {
      if (output.size() != 1) {
        return {false, "unexected multiline output"};
      }
      const auto output_value = tfel::utilities::convert<double>(output[0]);
      return {std::abs(output_value - value) < eps, ""};
    };
  }  // end of generateExpectedNumericalOutputCheck

  void TestLauncher::treatCommand() {
    auto c = Command();
    /*!
     * \brief parse the options associated with the execution of the command
     * \param[in] c: command
     * \param[in] opts: options passed to the command
     *
     * Valid options are:
     *
     * - `empty_output` (boolean): if specified, a test is added which checks if
     *   the command output is empty or not.
     * - `expected_output` (string or array of strings): if specified, a test is
     added to which compares
     *    the command output to the expected value.
     * - `expected_numerical_output`: if specified, a test is added to which
     *   compares the command output to the expected value.
     * - `shall_fail`: if specified and set to true, the command shall return an
     *   invalid exit status.
     */
    auto parseCommandOptions = [&c](const tfel::utilities::DataMap& options) {
      auto is_output_check_defined = [&c] {
        if (c.output_check) {
          tfel::raise(
              "TestLaucher::treatCommand: an output check has already been "
              "defined");
        }
      };
      for (const auto& o : options) {
        if (o.first == "expected_output") {
          is_output_check_defined();
          c.output_check = generateExpectedOutputCheck(o.second);
        } else if (o.first == "expected_numerical_output") {
          is_output_check_defined();
          c.output_check = generateExpectedNumericalOutputCheck(o.second);
        } else if (o.first == "empty_output") {
          is_output_check_defined();
          c.output_check = generateEmptyOutputCheck(o.second);
        } else if (o.first == "output_validation_regex") {
          is_output_check_defined();
          c.output_check = generateRegularExpressionOutputCheck(o.second);
        } else if (o.first == "shall_fail") {
          if (!o.second.is<bool>()) {
            tfel::raise(
                "TestLaucher::treatCommand: invalid type "
                "for option 'shall_fail'");
          }
          c.shall_fail = o.second.get<bool>();
        } else {
          tfel::raise(
              "TestLaucher::treatCommand: "
              "invalid option '" +
              o.first + "'");
        }
      }
    };  // end of parseCommandOptions
    c.command = this->readString("TestLauncher::treatCommand");
    this->checkNotEndOfFile("TestLauncher::treatCommand");
    while ((this->current->value != "{") &&  //
           (this->current->value != ";")) {
      c.command += " " + this->readString("TestLauncher::treatCommand");
      this->checkNotEndOfFile("TestLauncher::treatCommand");
    }
    if (this->current->value == "{") {
      parseCommandOptions(
          tfel::utilities::Data::read_map(this->current, this->end()));
    }
    this->commands.push_back(c);
    this->readSpecifiedToken("TestLauncher::treatCommand", ";");
  }  // end of treatCommand

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
      const auto c = CxxTokenizer::readUnsignedInt(this->current, this->end());
      this->colIntegralInterpolated.reset(new Column(c));
    }
    this->readSpecifiedToken("TestLauncher::treatTestType", ";");
  }  // end of treatTestType

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
  }  // end of treatTest

  void TestLauncher::treatPrecision() {
    this->prec = CxxTokenizer::readDouble(this->current, this->end());
    this->checkNotEndOfFile("TestLauncher::treatPrecision");
    if (this->current->value != ";") {
      // Optional second precision
      this->precision2 = CxxTokenizer::readDouble(this->current, this->end());
    }
    this->readSpecifiedToken("TestLauncher::treatPrecision", ";");
  }  // end of treatPrecision

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
      const auto c = CxxTokenizer::readUnsignedInt(this->current, this->end());
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

  void TestLauncher::analyseInputFile(const Configuration& c) {
    this->treatCharAsString(true);
    this->mergeStrings(false);
    // open the file
    this->openFile(this->file);
    // strip any C/C++ comments
    this->stripComments();
    // applying substitutions
    for (auto& t : this->tokens) {
      for (const auto& s : c.substitutions) {
        t.value = tfel::utilities::replace_all(t.value, s.first, s.second);
      }
    }
    // starts parsing the file
    this->current = this->begin();
    while (this->current != this->end()) {
      const auto p = this->callBacks.find(this->current->value);
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
  }  // end of analyseInputFile

  static std::vector<std::string> getFileContent(const std::string& f) {
    auto rtrim = [](const std::string& s) {
      auto end = s.find_last_not_of(" \n\r\t\f\v");
      return (end == std::string::npos) ? "" : s.substr(0, end + 1);
    };
    std::ifstream file(f);
    if (!file) {
      tfel::raise("getFileContent: can't open file '" + f + "'");
    }
    auto content = std::vector<std::string>{};
    auto line = std::string{};
    while (std::getline(file, line)) {
      content.push_back(rtrim(line));
    }
    return content;
  }  // end of getSourceFileContent

  double TestLauncher::ClockAction(ClockEventType clockevent) {
    using namespace std;
    if (clockevent == START) {
      const auto r = times(&this->timeStart);
      raise_if(r == static_cast<clock_t>(-1),
               "TestLauncher::ClockAction: 'times' system call failed");
      return r;
    }
    if (clockevent == STOP) {
      const auto r = times(&this->timeStop);
      raise_if(r == static_cast<clock_t>(-1),
               "TestLauncher::ClockAction: 'times' system call failed");
      return r;
    }
    if (clockevent == GET) {
      return (this->timeStop.tms_cutime - this->timeStart.tms_cutime) * 1.0 /
             sysconf(_SC_CLK_TCK);
    }
    tfel::raise("TestLauncher::ClockAction: invalid clockevent");
  }

  bool TestLauncher::execute(const Command& c,
                             const std::string& output_file,
                             const std::string& step) {
    tfel::system::ProcessManager manager;
    try {
      this->ClockAction(START);
      manager.execute(c.command, "", output_file, this->environments);
      this->ClockAction(STOP);
      if (c.output_check) {
        const auto output = getFileContent(output_file);
        const auto [success, msg] = c.output_check(output);
        if (success) {
          this->log.addTestResult(this->testname, step, c.command,
                                  this->ClockAction(GET), true);
        } else {
          this->log.addTestResult(this->testname, step, c.command,
                                  this->ClockAction(GET), false, msg);
        }
        return success;
      } else {
        this->log.addTestResult(this->testname, step, c.command,
                                this->ClockAction(GET), true);
      }
    } catch (std::exception& e) {
      this->log.addTestResult(this->testname, step, c.command, 0.0,
                              c.shall_fail, e.what());
      return c.shall_fail;
    } catch (...) {
      this->log.addTestResult(this->testname, step, c.command, 0.0,
                              c.shall_fail, "unhandled exception thrown");
      return c.shall_fail;
    }
    return true;
  }  // end of execute

  bool TestLauncher::execute(const Configuration& configuration) {
    using namespace tfel::utilities;
    bool gsuccess = true; /* global success indicator : false only
                             if comparison fails or when command
                             fails if no comparison are given*/
    // checking that the requirements are met
    const auto& components = configuration.available_components;
    for (const auto& r : this->requirements) {
      if (std::find(components.begin(), components.end(), r) ==
          components.end()) {
        unsigned short i = 1;
        for (const auto& c : this->commands) {
          const auto step = "Exec-" + std::to_string(i);
          this->glog.reportSkippedTest("** " + step + " " + c.command);
          ++i;
        }
        return gsuccess;
      }
    }
    // Execute
    unsigned short i = 1;
    for (const auto& c : this->commands) {
      const auto step = "Exec-" + std::to_string(i);
      const auto output_file = this->testname + "-" + step + ".out";
      const auto success = this->execute(c, output_file, step);
      this->glog.addSimpleTestResult("** " + step + " " + c.command, success);
      if ((!success) && (this->comparisons.empty())) {
        gsuccess = false;
      }
      ++i;
    }
    i = 1;
    // Compare
    for (auto& c : this->comparisons) {
      const auto step = "Compare-" + std::to_string(i);
      bool success = true;
      try {
        c.applyInterpolation();
        c.compare();
        success = c.hasSucceed();
        gsuccess = (gsuccess == false ? false : success);
        this->log.addTestResult(
            this->testname, step,
            "Comparing '" + c.getFileA() + "' and '" + c.getFileB() + "'", 0.0,
            success, c.getMsgLog());
        i++;
      } catch (std::exception& e) {
        this->log.addTestResult(
            this->testname, step,
            "Comparing '" + c.getFileA() + "' and '" + c.getFileB() + "'", 0.0,
            false, e.what());
        gsuccess = false;
        success = false;
        i++;
      }
      this->glog.addSimpleTestResult(
          "** " + step + " '" + c.getFileA() + "' and '" + c.getFileB() +
              "', column '" + c.getColA()->getName() + "' ",
          success);
    }
    // files and directories clean-up
    for (const auto& f : this->cleanfiles) {
      tfel::system::systemCall::unlink(f);
    }
    for (const auto& d : this->cleandirectories) {
      tfel::system::systemCall::rmdir(d);
    }
    return gsuccess;
  }  // end of execute

  TestLauncher::~TestLauncher() { this->log.terminate(); }

}  // end of namespace tfel::check
