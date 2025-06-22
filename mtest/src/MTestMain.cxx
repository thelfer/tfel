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

#include <cfenv>
#include <regex>
#include <vector>
#include <string>
#include <cstdlib>
#include <utility>
#include <iostream>

#if defined _WIN32 || defined _WIN64
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#ifdef small
#undef small
#endif /* small */
#endif

#ifdef MTEST_HAVE_MADNEX
#include "Madnex/Config.hxx"
#endif /* MTEST_HAVE_MADNEX */

#ifdef MADNEX_MTEST_TEST_SUPPORT
#include "Madnex/DataBase.hxx"
#include "Madnex/MTestTest.hxx"
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
    /*!
     * \brief add a single test and defines the output files
     * \param[in] t: pointer to the test
     * \param[in] n: name of the test
     */
    void addTest(std::shared_ptr<SchemeBase>, const std::string&);
    std::shared_ptr<SchemeBase> createMTestTest(const std::string&);
    std::shared_ptr<SchemeBase> createPTestTest(const std::string&);
    void treatMadnexInputFile(const std::string&);
    void treatStandardInputFile(const std::string&);

#ifdef MADNEX_MTEST_TEST_SUPPORT
    /*!
     * \param[in] file_name: file name
     * \param[in] material_name: name of the material
     * \param[in] behaviour_name: name of the behaviour
     * \param[in] test_name: name of the test
     */
    void appendTestFromMadnexFile(const std::string&,
                                  const std::string&,
                                  const std::string&,
                                  const std::string&);
#endif /* MADNEX_MTEST_TEST_SUPPORT */

    //! \brief input files
    std::vector<std::string> inputs;
    //! \brief external commands
    std::vector<std::string> ecmds;
    //! \brief substitutions
    std::map<std::string, std::string> substitutions;
    //! \brief material name (only useful for madnex files)
    std::string material;
    //! \brief material name (required for madnex files)
    std::string behaviour;
    //! \brief test name (required for madnex files)
    std::string test;
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
    auto treatBehaviour = [this] {
      if (!this->behaviour.empty()) {
        tfel::raise(
            "MTestMain::treatBehaviour: "
            "no option given");
      }
      this->behaviour = this->currentArgument->getOption();
    };
    auto treatMaterial = [this] {
      if (!this->material.empty()) {
        tfel::raise(
            "MTestMain::treatMaterial: "
            "no option given");
      }
      this->material = this->currentArgument->getOption();
    };
    auto treatTest = [this] {
      if (!this->test.empty()) {
        tfel::raise(
            "MTestMain::treatTest: "
            "no option given");
      }
      this->test = this->currentArgument->getOption();
    };
    auto treatAllTests = [this] {
      if (!this->test.empty()) {
        tfel::raise(
            "MTestMain::treatTest: "
            "no option given");
      }
      this->test = ".+";
    };
    this->registerCallBack("--behaviour", "-b",
                           CallBack("set the behaviour", treatBehaviour, true));
    this->registerCallBack("--material", "-m",
                           CallBack("set the material", treatMaterial, true));
    this->registerCallBack("--test", "-t",
                           CallBack("set the test name", treatTest, true));
    this->registerCallBack("--all-tests",
                           CallBack("select all tests", treatAllTests, false));
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
      const auto& l = this->currentArgument->getOption();
      mfront::setVerboseMode(l);
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
    for (const auto& i : this->inputs) {
      const auto ext = [&i]() -> std::string {
        const auto pos = i.rfind('.');
        if (pos != std::string::npos) {
          return i.substr(pos);
        }
        return "";
      }();
      if (tfel::utilities::starts_with(i, "madnex:") ||
          tfel::utilities::starts_with(i, "mdnx:")) {
        this->treatMadnexInputFile(i);
      } else {
        if ((ext == ".madnex") || (ext == ".mdnx") || (ext == ".edf")) {
          this->treatMadnexInputFile(i);
        } else {
          this->treatStandardInputFile(i);
        }
      }
    }
    auto& tm = tfel::tests::TestManager::getTestManager();
    const auto r = tm.execute();
    return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
  }  // end of execute

  std::shared_ptr<SchemeBase> MTestMain::createMTestTest(
      const std::string& path) {
    auto t = std::make_shared<MTest>();
    t->readInputFile(path, this->ecmds, this->substitutions);
    return t;
  }  // end of createMTestTest

  std::shared_ptr<SchemeBase> MTestMain::createPTestTest(
      const std::string& path) {
    auto t = std::make_shared<PipeTest>();
    PipeTestParser().execute(*t, path, this->ecmds, this->substitutions);
    return t;
  }  // end of createPTestTest

#ifdef MADNEX_MTEST_TEST_SUPPORT
  void MTestMain::appendTestFromMadnexFile(const std::string& file_name,
                                           const std::string& material_name,
                                           const std::string& behaviour_name,
                                           const std::string& test_name) {
    const auto path = "madnex:" +                              //
                      file_name + ":" + material_name + ":" +  //
                      behaviour_name + ":" + test_name;
    auto t = std::shared_ptr<SchemeBase>{};
    const auto test_scheme = madnex::getMTestTestScheme(
        file_name, material_name, behaviour_name, test_name);
    if (!test_scheme.empty()) {
      if ((test_scheme != "mtest") && (test_scheme != "ptest")) {
        tfel::raise("MTestMain::appendTestFromMadnexFile: invalid scheme");
      }
    }
    if (this->scheme == MTEST) {
      if ((!test_scheme.empty()) && (test_scheme != "mtest")) {
        tfel::raise(
            "MTestMain::appendTestFromMadnexFile: the scheme specified on the "
            "command line does not match the scheme declared by the test");
      }
      t = this->createMTestTest(path);
    } else if (this->scheme == PTEST) {
      if ((!test_scheme.empty()) && (test_scheme != "ptest")) {
        tfel::raise(
            "MTestMain::appendTestFromMadnexFile: the scheme specified on the "
            "command line does not match the scheme declared by the test");
      }
      t = this->createPTestTest(path);
    } else {
      if (test_scheme.empty()) {
        tfel::raise(
            "MTestMain::appendTestFromMadnexFile: "
            "the scheme must be specified using the --scheme command "
            "line argument when using a madnex file");
      }
      if (test_scheme == "mtest") {
        t = this->createMTestTest(path);
      } else {
        t = this->createPTestTest(path);
      }
    }
    this->addTest(t, test_name);
  }    // end of appendTestFromMadnexFile
#endif /* MADNEX_MTEST_TEST_SUPPORT */

  void MTestMain::treatMadnexInputFile(const std::string& i) {
#ifdef MADNEX_MTEST_TEST_SUPPORT
    if (tfel::utilities::starts_with(i, "madnex:") ||
        tfel::utilities::starts_with(i, "mdnx:")) {
      const auto [f, m, b, t] = [&i] {
        const auto details = tfel::utilities::tokenize(i, ':', true);
        if (details.size() != 5u) {
          tfel::raise(
              "MTestMain::treatMadnexInputFile: "
              "invalid path '" +
              i + "'");
        }
        const auto material_name = details[2] == "<none>" ? "" : details[2];
        return std::make_tuple(std::move(details[1]), std::move(material_name),
                               std::move(details[3]), std::move(details[4]));
      }();
      this->appendTestFromMadnexFile(f, m, b, t);
      return;
    }
    if (this->behaviour.empty()) {
      tfel::raise(
          "MTestMain::treatMadnexInputFile: no behaviour specified. "
          "This is required when using a madnex file");
    }
    if (this->test.empty()) {
      tfel::raise(
          "MTestMain::treatMadnexInputFile: no test specified. "
          "This is required when using a madnex file");
    }
    madnex::DataBase d(i);
    std::regex r(this->test);
    auto found = false;
    const auto m = this->material == "<none>" ? "" : this->material;
    for (const auto& tname :
         d.getAvailableMTestTests(this->material, this->behaviour)) {
      if (!std::regex_match(tname, r)) {
        continue;
      }
      found = true;
      this->appendTestFromMadnexFile(i, this->material, this->behaviour, tname);
    }
    if (!found) {
      tfel::raise(
          "MTestMain::treatMadnexInputFile: "
          "no test matches regular expression "
          "'" +
          this->test + "'");
    }
#else  /* MADNEX_MTEST_TEST_SUPPORT */
    tfel::raise(
        "MTestMain::treatMadnexInputFile: "
        "madnex support is not available");
#endif /* MADNEX_MTEST_TEST_SUPPORT */
  }    // end of treatMadnexInputFile

  void MTestMain::treatStandardInputFile(const std::string& i) {
#ifdef MTEST_HAVE_MADNEX
    if (!this->material.empty()) {
      tfel::raise(
          "MTestMain::treatStandardInputFile: specifying a material is only "
          "meaningful when using a madnex file");
    }
    if (!this->behaviour.empty()) {
      tfel::raise(
          "MTestMain::treatStandardInputFile: specifying a behaviour is only "
          "meaningful when using a madnex file");
    }
    if (!this->test.empty()) {
      tfel::raise(
          "MTestMain::treatStandardInputFile: specifying a test is only "
          "meaningful when using a madnex file");
    }
#endif /* MTEST_HAVE_MADNEX */
    const auto pos = i.rfind('.');
    auto tname = i.substr(0, pos);
    tfel::raise_if(tname.back() == '/',
                   "MTestMain::treatStandardInputFile: "
                   "invalid input file name '" +
                       i + "'");
    const auto pos2 = tname.rfind('/');
    if (pos2 != std::string::npos) {
      tname = tname.substr(pos2 + 1);
    }
#ifdef _WIN32
    const auto pos3 = tname.rfind('\\');
    if (pos3 != std::string::npos) {
      tname = tname.substr(pos3 + 1);
    }
#endif /* _WIN32 */
    tfel::raise_if(tname.empty(),
                   "MTestMain::treatStandardInputFile: "
                   "invalid input file name '" +
                       i + "'");
    if (this->scheme == MTEST) {
      this->addTest(this->createMTestTest(i), tname);
    } else if (this->scheme == PTEST) {
      this->addTest(this->createPTestTest(i), tname);
    } else if (this->scheme == DEFAULT) {
      const auto ext = [&i, &pos]() -> std::string {
        if (pos != std::string::npos) {
          return i.substr(pos);
        }
        return "";
      }();
      if (ext == ".ptest") {
        this->addTest(this->createPTestTest(i), tname);
      } else {
        this->addTest(this->createMTestTest(i), tname);
      }
    }
  }  // end of treatStandardInputFile

  void MTestMain::addTest(std::shared_ptr<SchemeBase> t, const std::string& n) {
    auto& tm = tfel::tests::TestManager::getTestManager();
    if (this->result_file_output) {
      if (!t->isOutputFileNameDefined()) {
        t->setOutputFileName(n + ".res");
      }
    }
    if (this->residual_file_output) {
      if (!t->isResidualFileNameDefined()) {
        t->setResidualFileName(n + "-residual.res");
      }
    }
    tm.addTest("MTest/" + n, t);
    if (this->xml_output) {
      std::shared_ptr<tfel::tests::TestOutput> o;
      if (!t->isXMLOutputFileNameDefined()) {
        o = std::make_shared<tfel::tests::XMLTestOutput>(n + ".xml");
      } else {
        o = std::make_shared<tfel::tests::XMLTestOutput>(
            t->getXMLOutputFileName());
      }
      tm.addTestOutput("MTest/" + n, o);
    }
    tm.addTestOutput("MTest/" + n, std::cout);
  }  // end of addTest
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
