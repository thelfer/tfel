/*!
 * \file   tfel-check.cpp
 * \brief
 * \author Helfer Thomas
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

#include "TFEL/Utilities/ArgumentParserBase.hxx"
#include "TFEL/System/System.hxx"
#include "TFEL/System/RecursiveFind.hxx"
#include "TFEL/Utilities/TerminalColors.hxx"
#include "TFELCheck/TestLauncher.hxx"
#include "TFELCheck/TestLauncherV1.hxx"
#include "TFELCheck/PCLogger.hxx"
#include "TFELCheck/PCTextDriver.hxx"
#include "TFELCheck/PCJUnitDriver.hxx"

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
  virtual ~TFELCheck();

 protected:
  /*!
   * \brief method called while parsing unregistred command line
   * arguments.
   */
  virtual void treatUnknownArgument(void) override;
  /*!
   * \brief register call backs associated to command line arguments
   */
  void registerArgumentCallBacks();

  virtual std::string getVersionDescription() const override;

  virtual std::string getUsageDescription(void) const override;

  std::vector<std::string> inputs;
};

TFELCheck::TFELCheck(const int argc, const char* const* const argv)
    : tfel::utilities::ArgumentParserBase<TFELCheck>() {
  this->setArguments(argc, argv);
  this->registerArgumentCallBacks();
  this->parseArguments();
}  // end of TFELCheck::TFELCheck

int TFELCheck::execute() {
  using namespace std;
  using namespace tfel_check;
  auto log = PCLogger(std::make_shared<PCTextDriver>("tfel-check.log"));
  log.addDriver(std::make_shared<PCTextDriver>());
  auto exe = [&log](const std::string& d, const std::string& f) {
    char path[PATH_MAX];
    char cpath[PATH_MAX];
    if (realpath(".", cpath) == nullptr) {
      cerr << "main : can't get real path of current directory, aborting\n";
      log.addMessage(
          "main : can't get real path of current directory, aborting");
      exit(EXIT_FAILURE);
    }
    if (realpath(d.c_str(), path) == nullptr) {
      log.addMessage("entering directory " + d);
    } else {
      log.addMessage(string("entering directory ") + path);
    }
    if (chdir(d.c_str()) == -1) {
      log.addMessage("can't move to directory " + d);
      log.addSimpleTestResult("* result of test " + d + '/' + f, false);
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
    } catch (exception& e) {
      log.addMessage("test failed : " + f + ", reason:\n" + e.what());
      success = false;
    }
    log.addSimpleTestResult("* end of test '" + d + '/' + f, success);
    log.addMessage("======");
    if (chdir(cpath) == -1) {
      log.addMessage(string("can't move back to top directory ") + cpath);
      log.addMessage("Aborting");
      exit(EXIT_FAILURE);
    }
    return success;
  };
  int status = EXIT_SUCCESS;
  if (this->inputs.empty()) {
    regex_t re;
    if (regcomp(&re, ".*\\.check$", REG_EXTENDED | REG_NOSUB) != 0) {
      cerr << "main : can't compile regular expression\n";
      return EXIT_FAILURE; /* Report error. */
    }
    const auto& files = tfel::system::recursiveFind(re, ".", 0);
    regfree(&re);
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

void TFELCheck::treatUnknownArgument() {
  const auto& arg = this->currentArgument->as_string();
  if (arg[0] == '-') {
    ArgumentParserBase<TFELCheck>::treatUnknownArgument();
    return;
  }
  this->inputs.push_back(arg);
}  // end of TFELCheck::treatUnknownArgument

void TFELCheck::registerArgumentCallBacks() {

}  // end of TFELCheck::registerArgumentCallBacks

std::string TFELCheck::getVersionDescription() const {
#pragma message("HERE")
  return "";
}

std::string TFELCheck::getUsageDescription() const {
  return "Usage: " + this->programName + " [options] [files]";
}

TFELCheck::~TFELCheck() = default;

int main(const int argc, const char* const* const argv) {
  TFELCheck checker(argc, argv);
  return checker.execute();
}  // end of main
