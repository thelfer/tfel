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
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <climits>
#include <unistd.h>
#include <libgen.h>

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/ArgumentParserBase.hxx"
#include "TFEL/System/System.hxx"
#include "TFEL/System/RecursiveFind.hxx"
#include "TFEL/Utilities/TerminalColors.hxx"
#include "TFEL/Check/ConfigurationManager.hxx"
#include "TFEL/Check/TestLauncher.hxx"
#include "TFEL/Check/TestLauncherV1.hxx"
#include "TFEL/Check/PCLogger.hxx"
#include "TFEL/Check/PCTextDriver.hxx"
#include "TFEL/Check/PCJUnitDriver.hxx"

namespace tfel {

  namespace check {

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
      //! \brief register call backs associated to command line arguments
      virtual void registerArgumentCallBacks();
      //! \return the current version of `tfel-check`
      std::string getVersionDescription() const override;
      //! \return the description of the usage of `tfel-check`
      std::string getUsageDescription() const override;
      //! list of configuration files
      std::vector<std::string> configFiles;
      //! list of input files
      std::vector<std::string> inputs;
    };  // end of struct TFELCheck

    void TFELCheck::treatUnknownArgument() {
      auto throw_if = [](const bool c, const std::string& m) {
        raise_if(c, "TFELCheck::treatUnknownArgument: " + m);
      };
      const auto& arg = this->currentArgument->as_string();
      const auto b = [&arg] {
        if (arg.size() >= 4) {
          return ((arg[0] == '-') && (arg[1] == '-') && (arg[2] == '@') &&
                  (arg.back() == '@'));
        }
        return false;
      }();
      if (b) {
        const auto s1 = arg.substr(2);
        throw_if(std::count(s1.begin(), s1.end(), '@') != 2,
                 "bad substitution pattern '" + s1 + "'");
        // throw_if(!this->substitutions.insert({s1,o}).second,
        // 	 "a substitution for '"+s1+"' has "
        // 	 "already been defined");
      }
      if (arg[0] == '-') {
        ArgumentParserBase<TFELCheck>::treatUnknownArgument();
        return;
      }
      this->inputs.push_back(arg);
    }  // end of TFELCheck::treatUnknownArgument

    void TFELCheck::registerArgumentCallBacks() {
      auto add_config_files = [this] {
        const auto f = this->currentArgument->getOption();
        raise_if(std::find(this->configFiles.begin(), this->configFiles.end(),
                           f) == this->configFiles.end(),
                 "tfel-check: configuration file '" + f +
                     "' "
                     "already defined");
        this->configFiles.push_back(f);
      };
      // auto declare = [this](const char* const n,
      // 			    const CallBack& c){
      // 	this->registerCallBack(n,c);
      // };
      auto declare2 = [this](const char* const n, const char* const a,
                             const CallBack& c) {
        this->registerCallBack(n, a, c);
      };
      declare2("--config", "-c",
               CallBack("add a configuration file", add_config_files, true));
    }  // end of TFELCheck::registerArgumentCallBacks

    std::string TFELCheck::getVersionDescription() const { return VERSION; }

    std::string TFELCheck::getUsageDescription() const {
      return "Usage: " + this->programName + " [options] [files]";
    }

    TFELCheck::TFELCheck(const int argc, const char* const* const argv) {
      this->setArguments(argc, argv);
      this->registerArgumentCallBacks();
      this->parseArguments();
    }  // end of TFELCheck::TFELCheck

    int TFELCheck::execute() {
      using namespace std;
      auto log = PCLogger(std::make_shared<PCTextDriver>("tfel-check.log"));
      log.addDriver(std::make_shared<PCTextDriver>());
      auto exe = [&log](const std::string& d, const std::string& f) {
        using namespace tfel::system;
        const auto cpath = systemCall::getCurrentWorkingDirectory();
        const auto path = systemCall::getAbsolutePath(d);
        log.addMessage("entering directory '" + path + "'");
        try {
          systemCall::changeCurrentWorkingDirectory(d);
        } catch (std::exception& e) {
          log.addMessage("can't move to directory '" + d + "' (" +
                         std::string(e.what()) + ")");
          log.addSimpleTestResult("* result of test '" + d + '/' + f + "'",
                                  false);
          return false;
        }
        log.addMessage("* beginning of test '" + d + '/' + f + "'");
        const auto name = d + '/' + f;
        auto success = true;
        try {
          // if(this->file_version==TestLauncher::V1){
          // 	TestLauncherV1 c(f,log);
          // 	success = c.execute();
          // } else {
          TestLauncher c(f, log);
          success = c.execute();
          //      }
        } catch (std::exception& e) {
          log.addMessage("test failed : '" + f + "', reason:\n" + e.what());
          success = false;
        }
        log.addSimpleTestResult("* end of test '" + d + '/' + f + "'", success);
        log.addMessage("======");
        try {
          systemCall::changeCurrentWorkingDirectory(cpath);
        } catch (std::exception& e) {
          log.addMessage("can't move back to top directory '" + cpath + "' (" +
                         std::string(e.what()) + ")");
          log.addMessage("Aborting");
          exit(EXIT_FAILURE);
        }
        return success;
      };
      int status = EXIT_SUCCESS;
      ConfigurationManager c;
      if (this->configFiles.empty()) {
        std::ifstream cf("tfel-check.config");
        if (cf) {
          log.addMessage("Parsing configuration file 'tfel-check.config'");
          parse(c, "tfel-check.config");
          log.addMessage("======");
        }
      } else {
        for (const auto& cf : this->configFiles) {
          log.addMessage("Parsing configuration file '" + cf + "'");
          parse(c, cf);
          log.addMessage("======");
        }
      }
      if (this->inputs.empty()) {
        std::regex re(".+\\.check", std::regex_constants::extended);
        const auto& files = tfel::system::recursiveFind(re, ".", 0);
        for (const auto& d : files) {
          for (const auto& f : d.second) {
            if (!exe(d.first, f)) {
              status = EXIT_FAILURE;
            }
          }
        }
      } else {
        for (const auto& i : this->inputs) {
          const auto path = strdup(i.c_str());
          const auto path2 = strdup(path);
          const auto d = std::string(::dirname(path));
          const auto f = std::string(::basename(path2));
          ::free(path);
          ::free(path2);
          // basename
          if (!exe(d, f)) {
            status = EXIT_FAILURE;
          }
        }
      }
      log.terminate();
      return status;
    }

    TFELCheck::~TFELCheck() = default;

  }  // end of namespace check

}  // end of namespace tfel

int main(const int argc, const char* const* const argv) {
  tfel::check::TFELCheck check(argc, argv);
  return check.execute();
}  // end of main
