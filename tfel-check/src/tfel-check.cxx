/*!
 * \file   tfel-check/src/tfel-check.cxx
 * \brief
 * \author Thomas Helfer
 * \date   28 Jan 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <map>
#include <thread>
#include <vector>
#include <string>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>

#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/Utilities/ArgumentParserBase.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/System.hxx"
#include "TFEL/System/RecursiveFind.hxx"
#include "TFEL/System/ThreadPool.hxx"
#include "TFEL/Utilities/TerminalColors.hxx"
#include "MFront/InitDSLs.hxx"
#include "MFront/InitInterfaces.hxx"
#include "MFront/MaterialPropertyInterfaceFactory.hxx"
#include "MFront/BehaviourInterfaceFactory.hxx"
#include "MFront/ModelInterfaceFactory.hxx"
#include "TFEL/Check/ConfigurationManager.hxx"
#include "TFEL/Check/TestLauncher.hxx"
#include "TFEL/Check/PCLogger.hxx"
#include "TFEL/Check/PCTextDriver.hxx"
#include "TFEL/Check/PCJUnitDriver.hxx"

namespace tfel::check {

  /*!
   * \brief parse a user defined configuration file
   * \param[out] c: configuration manager
   * \param[in] f: configuration files given by the user
   */
  static void parseConfigFiles(ConfigurationManager& c) {
    const auto s = tfel::system::dirSeparator();
    std::regex re("tfel-check.config", std::regex_constants::extended);
    const auto& files = tfel::system::recursiveFind(re, ".", false);
    for (const auto& d : files) {
      for (const auto& f : d.second) {
        parse(c.getConfigurationManager(d.first), d.first + s + f);
      }
    }  // end of parseConfigFile
  }

  static void declareTFELComponents(ConfigurationManager& c) {
    const auto& mpf = mfront::MaterialPropertyInterfaceFactory::
        getMaterialPropertyInterfaceFactory();
    const auto& bf =
        mfront::BehaviourInterfaceFactory::getBehaviourInterfaceFactory();
    const auto& mf = mfront::ModelInterfaceFactory::getModelInterfaceFactory();
#ifdef TFEL_PYTHON_BINDINGS
    c.addComponent("tfel::python");
    c.addComponent("mfront::python");
#endif
#ifdef TFEL_HAVE_MADNEX
    c.addComponent("tfel::madnex");
#endif
    // registring mfront interface
    for (const auto& i : mpf.getRegistredInterfaces()) {
      c.addComponent("mfront::material_property::" + i + "_interface");
    }
    for (const auto& i : bf.getRegistredInterfaces()) {
      c.addComponent("mfront::behaviour::" + i + "_interface");
    }
    for (const auto& i : mf.getRegistredInterfaces()) {
      c.addComponent("mfront::model::" + i + "_interface");
    }
  }  // end of declareTFELComponents

  static void declareTFELExecutables(ConfigurationManager& c) {
    for (const std::string e : {"mfront", "mtest", "mfront-doc",
                                "mfm-test-generator", "mfront-query"}) {
      c.addSubstitution('@' + e + '@', tfel::getTFELExecutableName(e), false);
    }
#ifdef TFEL_PYTHON_EXECUTABLE
    c.addSubstitution("@python@", TFEL_PYTHON_EXECUTABLE, false);
#endif /* */
  }    // end of declareTFELExecutables

  /*!
   * \brief main entry point
   */
  struct TFELCheck : tfel::utilities::ArgumentParserBase<TFELCheck> {
    /*!
     * \param[in] argc: number of command line arguments
     * \param[in] argv: command line arguments
     */
    TFELCheck(const int, const char* const* const);
    /*!
     * \brief loop over the *.check files
     */
    int execute();
    //! desctructor
    ~TFELCheck() override;

   protected:
    /*!
     * \brief method called while parsing unregistred command line
     * arguments.
     */
    void treatUnknownArgument() override;
    //! \brief treat a substituion pattern
    virtual bool treatSubstitution();
    //! \brief register call backs associated to command line arguments
    virtual void registerArgumentCallBacks();
    //! \return the current version of `tfel-check`
    std::string getVersionDescription() const override;
    //! \return the description of the usage of `tfel-check`
    std::string getUsageDescription() const override;
    //! \brief configuration manager
    ConfigurationManager configurations;
    //! \brief list of configuration files
    std::vector<std::string> configFiles;
    //! list of input files
    std::vector<std::string> inputs;
    //! \brief number of parallel jobs
    tfel::system::ThreadPool::size_type njobs = 1;
    /*!
     * \brief boolean stating if the number of jobs allowed shall be lesser
     * than the number of cores
     */
    bool discard_jobs_limit = false;
  };  // end of struct TFELCheck

  bool TFELCheck::treatSubstitution() {
    const auto& an = this->currentArgument->as_string();
    if (an.back() != '@') {
      return false;
    }
    const auto& o = this->currentArgument->getOption();
    if (o.empty()) {
      return false;
    }
#ifdef _WIN32
    const auto s1 = starts_with(an, "/@") ? an.substr(1) : an.substr(2);
#else  /* _WIN32 */
    const auto s1 = an.substr(2);
#endif /* _WIN32 */
    if (std::count(s1.begin(), s1.end(), '@') != 2) {
      tfel::raise("bad substitution pattern '" + s1 + "'");
    }
    if (s1.empty()) {
      return false;
    }
    this->configurations.addSubstitution(s1, o);
    return true;
  }  // end of TFELCheck::treatSubstitution

  void TFELCheck::treatUnknownArgument() {
    using tfel::utilities::starts_with;
    const auto& an = this->currentArgument->as_string();
#ifdef _WIN32
    if (starts_with(an, "--@") || starts_with(an, "/@")) {
#else  /* _WIN32 */
    if (starts_with(an, "--@")) {
#endif /* _WIN32 */
      if (this->treatSubstitution()) {
        return;
      }
    }
    if (an[0] == '-') {
      ArgumentParserBase<TFELCheck>::treatUnknownArgument();
      return;
    }
    this->inputs.push_back(an);
  }  // end of TFELCheck::treatUnknownArgument

  void TFELCheck::registerArgumentCallBacks() {
    auto declare2 = [this](const char* const n, const char* const a,
                           const CallBack& c) {
      this->registerCallBack(n, a, c);
    };
    auto read_boolean_value = [this](const char* const m) {
      const auto& opt = this->currentArgument->getOption();
      if (opt == "true") {
        return true;
      } else if (opt != "false") {
        std::cerr << "invalid option '" << opt << "' passed to '" << m << "', "
                  << "expected 'true' or 'false'.\n";
        std::exit(EXIT_FAILURE);
      }
      return false;
    };
    declare2(
        "--jobs", "-j",
        CallBack(
            "specifies the number of jobs (commands) to run simultaneously",
            [this] {
              const auto& o = this->currentArgument->getOption();
              if (o.empty()) {
                const auto processor_count =
                    std::thread::hardware_concurrency();
                if (processor_count == 0) {
                  std::cerr << "unable to determine the number of cores and no "
                            << "value passed to '--jobs'\n";
                  std::exit(EXIT_FAILURE);
                }
                this->njobs = processor_count;
                return;
              }
              auto pos = std::size_t{};
              try {
                this->njobs = std::stoul(o, &pos);
              } catch (std::invalid_argument& e) {
                std::cerr << "invalid value '" << o << "' passed to '--jobs'\n";
                std::exit(EXIT_FAILURE);
              }
              if (pos != o.size()) {
                std::cerr << "invalid value '" << o << "' passed to '--jobs'\n";
                std::exit(EXIT_FAILURE);
              }
            },
            true));
    declare2("--config", "-c",
             CallBack(
                 "add a configuration file",
                 [this] {
                   const auto f = this->currentArgument->getOption();
                   parse(this->configurations, f);
                 },
                 true));
    this->registerCallBack(
        "--discard-commands-failure",
        CallBack(
            "discard command's failure if comparisons are ok (default "
            "behaviour). If no comparisons is declared, command's failure is "
            "never ignored.",
            [this, read_boolean_value] {
              const auto b = read_boolean_value("--discard-commands-failure");
              this->configurations.setDiscardCommandsFailure(b);
            },
            true));
    this->registerCallBack(
        "--discard-jobs-limit",
        CallBack(
            "disable test on the number of jobs allowed to run simultaneously.",
            [this, read_boolean_value] {
              this->discard_jobs_limit =
                  read_boolean_value("--discard-commands-failure");
            },
            true));
    this->registerCallBack(
        "--list-default-components",
        CallBack(
            "list all default components",
            [this] {
              auto global_configuration =
                  this->configurations.getConfiguration("");
              for (const auto& c : global_configuration.available_components) {
                std::cout << "- " << c << '\n';
              }
              std::exit(EXIT_SUCCESS);
            },
            false));
  }  // end of TFELCheck::registerArgumentCallBacks

  std::string TFELCheck::getVersionDescription() const { return VERSION; }

  std::string TFELCheck::getUsageDescription() const {
    return "Usage: " + this->programName + " [options] [files]";
  }

  TFELCheck::TFELCheck(const int argc, const char* const* const argv) {
    parseConfigFiles(this->configurations);
    declareTFELComponents(this->configurations);
    this->setArguments(argc, argv);
    this->registerArgumentCallBacks();
    this->parseArguments();
    //
    if (!this->discard_jobs_limit) {
      const auto processor_count = std::thread::hardware_concurrency();
      if (processor_count != 0) {
        if (this->njobs > processor_count) {
          std::cerr
              << "the number of jobs is greater than the number of "
              << "available cores. This is not supported by default. Use "
              << "the '--discard-jobs-limit' option to discard this check\n";
          std::exit(EXIT_FAILURE);
        }
      }
    }
    // this is done after argument parsing to allow the user to modify default
    // executables' names
    declareTFELExecutables(this->configurations);
  }  // end of TFELCheck::TFELCheck

  int TFELCheck::execute() {
    using namespace std;
    auto pool = tfel::system::ThreadPool{this->njobs};
    auto log = PCLogger(std::make_shared<PCTextDriver>("tfel-check.log"));
    log.addDriver(std::make_shared<PCTextDriver>());
    auto exe = [this, &log](const std::string& d, const std::string& f) {
      using namespace tfel::system;
      const auto cpath = systemCall::getCurrentWorkingDirectory();
      const auto path = systemCall::getAbsolutePath(d);
      log.addMessage("entering directory '" + path + "'");
      log.addMessage("* beginning of test '" + d + '/' + f + "'");
      const auto name = d + '/' + f;
      auto success = true;
      try {
        auto c = this->configurations.getConfiguration(d);
        c.log = log;
        c.directory = d;
        TestLauncher t(c, f);
        success = t.execute(c);
      } catch (std::exception& e) {
        log.addMessage("test failed : '" + f + "', reason:\n" + e.what());
        success = false;
      }
      log.addSimpleTestResult("* end of test '" + d + '/' + f + "'", success);
      log.addMessage("======");
      return success;
    };
    auto future_results =
        std::vector<std::future<tfel::system::ThreadedTaskResult<bool>>>{};
    if (this->inputs.empty()) {
      std::regex re(".+\\.check", std::regex_constants::extended);
      const auto& files = tfel::system::recursiveFind(re, ".", false);
      for (const auto& d : files) {
        for (const auto& f : d.second) {
          future_results.push_back(
              pool.addTask([exe, d, f] { return exe(d.first, f); }));
        }
      }
    } else {
      // some tests on the input files
      for (const auto& i : this->inputs) {
        struct stat file_info;
        if (::stat(i.c_str(), &file_info) == -1) {
          log.addMessage("can't get information on input  '" + i + "'");
          log.addMessage("Aborting");
          exit(EXIT_FAILURE);
        }
        if (!S_ISREG(file_info.st_mode)) {
          log.addMessage("input  '" + i + "' is not a regular file");
          log.addMessage("Aborting");
          exit(EXIT_FAILURE);
        }
      }
      // executing each input files
      for (const auto& i : this->inputs) {
        //
        const auto path = strdup(i.c_str());
        const auto path2 = strdup(path);
        const auto d = std::string(::dirname(path));
        const auto f = std::string(::basename(path2));
        ::free(path);
        ::free(path2);
        // shall a new thread which starts a fork
        future_results.push_back(
            pool.addTask([exe, d, f] { return exe(d, f); }));
      }
    }
    // waiting for all jobs to terminate
    pool.wait();
    // checking the output values of all jobs
    int status = EXIT_SUCCESS;
    for (auto& f : future_results) {
      auto r = f.get();
      if (!r) {
        status = EXIT_FAILURE;
      } else {
        if (!(*r)) {
          status = EXIT_FAILURE;
        }
      }
    }
    log.terminate();
    return status;
  }

  TFELCheck::~TFELCheck() = default;

}  // end of namespace tfel::check

int main(const int argc, const char* const* const argv) {
  mfront::initDSLs();
  mfront::initInterfaces();
  tfel::check::TFELCheck check(argc, argv);
  return check.execute();
}  // end of main
