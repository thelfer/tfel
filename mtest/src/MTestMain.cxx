/*!
 * \file   mtest/src/MTestMain.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  05 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>
#include <cstdlib>
#include <utility>
#include <vector>
#include <string>
#include <cfenv>

#if defined _WIN32 || defined _WIN64
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include "TFEL/Raise.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Tests/XMLTestOutput.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Utilities/ArgumentParserBase.hxx"

#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
#include "TFEL/System/SignalManager.hxx"
#endif

#include "MFront/MFrontLogStream.hxx"
#include "MTest/RoundingMode.hxx"
#include "MTest/Constraint.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/MTest.hxx"
#include "MTest/PipeTest.hxx"
#include "MTest/MTestParser.hxx"
#include "MTest/PipeTestParser.hxx"

namespace mtest {

  struct MTestMain : tfel::utilities::ArgumentParserBase<MTestMain> {
    MTestMain(const int, const char* const* const);
    /*!
     * \brief main entry point
     * \return EXIT_SUCESS on success
     */
    int execute();
    //! destructor
    ~MTestMain() override;

   protected:
    enum { MTEST, PTEST, DEFAULT } scheme = DEFAULT;
    friend struct tfel::utilities::ArgumentParserBase<MTestMain>;
    void treatUnknownArgument() override;
    void treatVerbose();
    void treatScheme();
    void treatXMLOutput();
    void treatResultFileOutput();
    void treatResidualFileOutput();
    TFEL_NORETURN void treatHelpCommandsList();
    TFEL_NORETURN void treatHelpCommands();
    TFEL_NORETURN void treatHelpCommand();
    //! treat the `--floating-point-exceptions` option
    void treatEnableFloatingPointExceptions();
    //! treat the `--rounding-direction-mode` option
    void treatRoundingDirectionMode();
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
    //! treat the `--backtrace` option
    void treatBacktrace();
#endif
    std::string getVersionDescription() const override;
    std::string getUsageDescription() const override;
    void registerArgumentCallBacks();
    //! input files
    std::vector<std::string> inputs;
    //! external commands
    std::vector<std::string> ecmds;
    //! substitutions
    std::map<std::string, std::string> substitutions;
    // xml output
    bool xml_output = false;
    // generate result file
    bool result_file_output = true;
    // generate residual file
    bool residual_file_output = false;
  };

  MTestMain::MTestMain(const int argc, const char* const* const argv)
      : tfel::utilities::ArgumentParserBase<MTestMain>(argc, argv) {
    this->registerArgumentCallBacks();
    this->parseArguments();
    tfel::raise_if(this->inputs.empty(),
                   "MTestMain::MTestMain: "
                   "no input file defined");
  }

  void MTestMain::registerArgumentCallBacks() {
    this->registerNewCallBack("--verbose", &MTestMain::treatVerbose,
                              "set verbose output", true);
    this->registerNewCallBack("--scheme", &MTestMain::treatScheme, "set scheme",
                              true);
    this->registerNewCallBack("--xml-output", &MTestMain::treatXMLOutput,
                              "control xml output (default no)", true);
    this->registerNewCallBack("--result-file-output",
                              &MTestMain::treatResultFileOutput,
                              "control result output (default yes)", true);
    this->registerNewCallBack("--residual-file-output",
                              &MTestMain::treatResidualFileOutput,
                              "control residual output (default no)", true);
    this->registerNewCallBack(
        "--help-keywords", &MTestMain::treatHelpCommands,
        "display the help of all available commands and exit.");
    this->registerNewCallBack(
        "--help-commands", &MTestMain::treatHelpCommands,
        "display the help of all available commands and exit."
        "(this is equivalent to --help-keywords option).");
    this->registerNewCallBack("--help-keywords-list",
                              &MTestMain::treatHelpCommandsList,
                              "list available commands and exit.");
    this->registerNewCallBack(
        "--help-commands-list", &MTestMain::treatHelpCommandsList,
        "list available commands and exit "
        "(this is equivalent to --help-keywords-list option).");
    this->registerNewCallBack("--help-keyword", &MTestMain::treatHelpCommand,
                              "display the help associated with a "
                              "keyword and exit.",
                              true);
    this->registerNewCallBack("--help-command", &MTestMain::treatHelpCommand,
                              "display the help associated with a "
                              "keyword and exit."
                              "(this is equivalent to "
                              "--help-keyword option).",
                              true);
    this->registerNewCallBack(
        "--floating-point-exceptions", "-fpe",
        &MTestMain::treatEnableFloatingPointExceptions,
        "handle floating point exceptions through SIGFPE signals");
    this->registerNewCallBack(
        "--rounding-direction-mode", "-rdm",
        &MTestMain::treatRoundingDirectionMode,
        "set the rounding mode direction. Valid options are:\n"
        "DownWard:   Round downward.\n"
        "ToNearest:  Round to nearest.\n"
        "TowardZero: Round toward zero.\n"
        "UpWard:     Round upward.\n"
        "Random:     Rounding mode is randomly changed at various "
        "stage of the compution.",
        true);
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
    this->registerNewCallBack(
        "--backtrace", "-bt", &MTestMain::treatBacktrace,
        "print process stack when getting SIGSEGV or SIGFPE signals");
#endif
  }

  void MTestMain::treatScheme() {
    tfel::raise_if(this->currentArgument->getOption().empty(),
                   "MTestMain::treatScheme: "
                   "no option given");
    tfel::raise_if(this->scheme != DEFAULT,
                   "MTestMain::treatScheme: "
                   "scheme already given");
    const auto& s = this->currentArgument->getOption();
    if ((s == "MTest") || (s == "mtest")) {
      this->scheme = MTEST;
    } else if ((s == "PTest") || (s == "ptest")) {
      this->scheme = PTEST;
    } else {
      tfel::raise(
          "MTestMain::treatScheme: "
          "invalid scheme '" +
          s + "'");
    }
  }  // end of MTestMain::treatScheme

  void MTestMain::treatEnableFloatingPointExceptions() {
    // mathematical
#ifdef HAVE_FENV
    ::feclearexcept(FE_ALL_EXCEPT);
#ifdef __GLIBC__
    ::feenableexcept(FE_DIVBYZERO);  // division by zero
    ::feenableexcept(FE_INVALID);    // invalid operation
#endif                               /* __GLIBC__ */
#endif                               /* HAVE_FENV */
  }  // end of MTestMain::treatEnableFloatingPointExceptions

  void MTestMain::treatRoundingDirectionMode() {
    const auto& o = this->currentArgument->getOption();
    tfel::raise_if(o.empty(),
                   "MTestMain::setRoundingDirectionMode: "
                   "no option given");
    mtest::setRoundingMode(o);
  }  // end of MTestMain::setRoundingDirectionMode

#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
  void MTestMain::treatBacktrace() {
    using namespace tfel::system;
    // posix signals
    auto& sm = SignalManager::getSignalManager();
    // blocking all signals during treatment of signals SIGSEGV, SIGFPE
    struct sigaction action;
    sigfillset(&(action.sa_mask));
    action.sa_flags = 0;
    sm.registerHandler(SIGSEGV, sigPtrFun(SignalManager::printBackTrace),
                       action);
    sm.registerHandler(SIGFPE, sigPtrFun(SignalManager::printBackTrace),
                       action);
  }  // end of MTestMain::treatBacktrace()
#endif

  void MTestMain::treatVerbose() {
    if (this->currentArgument->getOption().empty()) {
      mfront::setVerboseMode(mfront::VERBOSE_LEVEL1);
    } else {
      const auto& option = this->currentArgument->getOption();
      if (option == "quiet") {
        mfront::setVerboseMode(mfront::VERBOSE_QUIET);
      } else if (option == "level0") {
        mfront::setVerboseMode(mfront::VERBOSE_LEVEL0);
      } else if (option == "level1") {
        mfront::setVerboseMode(mfront::VERBOSE_LEVEL1);
      } else if (option == "level2") {
        mfront::setVerboseMode(mfront::VERBOSE_LEVEL2);
      } else if (option == "level3") {
        mfront::setVerboseMode(mfront::VERBOSE_LEVEL3);
      } else if (option == "debug") {
        mfront::setVerboseMode(mfront::VERBOSE_DEBUG);
      } else if (option == "full") {
        mfront::setVerboseMode(mfront::VERBOSE_FULL);
      } else {
        tfel::raise(
            "MTestMain::treatVerbose : "
            "unknown option '" +
            option + "'");
      }
    }
  }  // end of MTestMain::treatVerbose

  void MTestMain::treatXMLOutput() {
    if (this->currentArgument->getOption().empty()) {
      this->xml_output = true;
    } else {
      const auto& option = this->currentArgument->getOption();
      if (option == "true") {
        this->xml_output = true;
      } else if (option == "false") {
        this->xml_output = false;
      } else {
        tfel::raise(
            "MTestMain::treatXMLOutput: "
            "unknown option '" +
            option + "'");
      }
    }
  }  // end of MTestMain::treatXMLOutput

  void MTestMain::treatResultFileOutput() {
    if (this->currentArgument->getOption().empty()) {
      this->result_file_output = true;
    } else {
      const auto& option = this->currentArgument->getOption();
      if (option == "true") {
        this->result_file_output = true;
      } else if (option == "false") {
        this->result_file_output = false;
      } else {
        tfel::raise(
            "MTestMain::treatResultFileOutput: "
            "unknown option '" +
            option + "'");
      }
    }
  }  // end of MTestMain::treatResultFileOutput

  void MTestMain::treatResidualFileOutput() {
    if (this->currentArgument->getOption().empty()) {
      this->residual_file_output = true;
    } else {
      const auto& option = this->currentArgument->getOption();
      if (option == "true") {
        this->residual_file_output = true;
      } else if (option == "false") {
        this->residual_file_output = false;
      } else {
        tfel::raise(
            "MTestMain::treatResidualFileOutput : "
            "unknown option '" +
            option + "'");
      }
    }
  }  // end of MTestMain::treatResidualFileOutput

  void MTestMain::treatHelpCommandsList() {
    if ((this->scheme == MTEST) || (this->scheme == DEFAULT)) {
      MTestParser().displayKeyWordsList();
    } else if (this->scheme == PTEST) {
      PipeTestParser().displayKeyWordsList();
    }
    ::exit(EXIT_SUCCESS);
  }  // end of MTestMain::treatHelpCommandsList

  void MTestMain::treatHelpCommands() {
    if ((this->scheme == MTEST) || (this->scheme == DEFAULT)) {
      MTestParser().displayKeyWordsHelp();
    } else if (this->scheme == PTEST) {
      PipeTestParser().displayKeyWordsList();
    }
    ::exit(EXIT_SUCCESS);
  }  // end of MTestMain::treatHelpCommands

  void MTestMain::treatHelpCommand() {
    const auto& k = this->currentArgument->getOption();
    tfel::raise_if(k.empty(),
                   "MTestMain::treatHelpCommand : "
                   "no command specified");
    if ((this->scheme == MTEST) || (this->scheme == DEFAULT)) {
      MTestParser().displayKeyWordDescription(k);
    } else if (this->scheme == PTEST) {
      PipeTestParser().displayKeyWordDescription(k);
    }
    ::exit(EXIT_SUCCESS);
  }

  void MTestMain::treatUnknownArgument() {
    using tfel::utilities::starts_with;
    const auto& a = this->currentArgument->as_string();
#ifdef _WIN32
    if (starts_with(a, "--@") || starts_with(a, "/@")) {
#else  /* _WIN32 */
    if (starts_with(a, "--@")) {
#endif /* _WIN32 */
      if (a.back() == '@') {
#ifdef _WIN32
        const auto s1 = starts_with(a, "/@") ? a.substr(1) : a.substr(2);
#else  /* _WIN32 */
        const auto s1 = a.substr(2);
#endif /* _WIN32 */
        tfel::raise_if(std::count(s1.begin(), s1.end(), '@') != 2,
                       "MTestMain::treatUnknownArgument: "
                       "bas substitution pattern '" +
                           s1 + "'");
        const auto s2 = this->currentArgument->getOption();
        tfel::raise_if(s2.empty(),
                       "MTestMain::treatUnknownArgument: "
                       "no substitution given for pattern '" +
                           s1 + "'");
        if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL2) {
          mfront::getLogStream()
              << "substituting '" << s1 << "' by '" << s2 << "'\n";
        }
        tfel::raise_if(!this->substitutions.insert({s1, s2}).second,
                       "MTestMain::treatUnknownArgument: "
                       "a substitution for '" +
                           s1 +
                           "' has "
                           "already been defined");
        return;
      } else {
        const auto o = this->currentArgument->getOption();
#ifdef _WIN32
        auto cmd = starts_with(a, "/@") ? a.substr(1) : a.substr(2);
#else  /* _WIN32 */
        auto cmd = a.substr(2);
#endif /* _WIN32 */
        if (!o.empty()) {
          cmd += ' ' + o;
        }
        cmd += ';';
        this->ecmds.push_back(cmd);
        return;
      }
    }
    if (a[0] == '-') {
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
      ArgumentParserBase<MTestMain>::treatUnknownArgument();
#else
      std::cerr << "mtest : unsupported option '" << a << "'\n";
      exit(EXIT_FAILURE);
#endif /* ! (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */
    }
    this->inputs.push_back(this->currentArgument->as_string());
  }  // end of MTestMain::treatUnknownArgument()

  std::string MTestMain::getVersionDescription() const {
    return "mtest is an behaviour testing utility";
  }

  std::string MTestMain::getUsageDescription() const {
    return "Usage : mtest [options] [filesusage]";
  }

  int MTestMain::execute() {
    auto mtest = [](const std::string& f, const std::vector<std::string>& e,
                    const std::map<std::string, std::string>& s)
        -> std::shared_ptr<SchemeBase> {
      auto t = std::make_shared<MTest>();
      t->readInputFile(f, e, s);
      return std::move(t);
    };
    auto ptest = [](const std::string& f, const std::vector<std::string>& e,
                    const std::map<std::string, std::string>& s)
        -> std::shared_ptr<SchemeBase> {
      auto t = std::make_shared<PipeTest>();
      PipeTestParser().execute(*t, f, e, s);
      return std::move(t);
    };
    using namespace std;
    using namespace tfel::tests;
    auto& tm = TestManager::getTestManager();
    for (const auto& i : this->inputs) {
      string tname;
      string ext;
      const auto pos = i.rfind('.');
      if (pos != string::npos) {
        tname = i.substr(0, pos);
        ext = i.substr(pos);
      } else {
        tname = i;
      }
      tfel::raise_if(tname.back() == '/',
                     "MTestMain::execute: "
                     "invalid input file name '" +
                         i + "'");
      const auto pos2 = tname.rfind('/');
      if (pos2 != string::npos) {
        tname = tname.substr(pos2 + 1);
      }
      tfel::raise_if(tname.empty(),
                     "MTestMain::execute: "
                     "invalid input file name '" +
                         i + "'");
      auto t = std::shared_ptr<SchemeBase>{};
      if (this->scheme == MTEST) {
        t = mtest(i, this->ecmds, this->substitutions);
      } else if (this->scheme == PTEST) {
        t = ptest(i, this->ecmds, this->substitutions);
      } else if (this->scheme == DEFAULT) {
        if (ext == ".ptest") {
          t = ptest(i, this->ecmds, this->substitutions);
        } else {
          t = mtest(i, this->ecmds, this->substitutions);
        }
      }
      if (this->result_file_output) {
        if (!t->isOutputFileNameDefined()) {
          t->setOutputFileName(tname + ".res");
        }
      }
      if (this->residual_file_output) {
        if (!t->isResidualFileNameDefined()) {
          t->setResidualFileName(tname + "-residual.res");
        }
      }
      tm.addTest("MTest/" + tname, t);
      if (this->xml_output) {
        std::shared_ptr<TestOutput> o;
        if (!t->isXMLOutputFileNameDefined()) {
          o = std::make_shared<XMLTestOutput>(tname + ".xml");
        } else {
          o = std::make_shared<XMLTestOutput>(t->getXMLOutputFileName());
        }
        tm.addTestOutput("MTest/" + tname, o);
      }
      tm.addTestOutput("MTest/" + tname, cout);
    }
    const auto r = tm.execute();
    return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
  }
  MTestMain::~MTestMain() = default;

}  // end of namespace mtest

/* coverity [UNCAUGHT_EXCEPT]*/
int main(const int argc, const char* const* const argv) {
  int r = EXIT_FAILURE;
#if not defined(__GLIBCXX__)
  try {
#endif /* not defined(__GLIBCXX__) */
    mtest::MTestMain m(argc, argv);
    r = m.execute();
#if not defined(__GLIBCXX__)
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return r;
  }
#endif /* not defined(__GLIBCXX__) */
  return r;
}  // end of main
