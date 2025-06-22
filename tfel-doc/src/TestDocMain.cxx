/*!
 * \file  tfel-doc/src/TestDocMain.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 03 mai 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
#define NOMINAX
#include <windows.h>
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */

#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <memory>

#include "TFEL/Raise.hxx"
#include "TFEL/System/System.hxx"
#include "TFEL/System/RecursiveFind.hxx"
#include "TFEL/Utilities/TerminalColors.hxx"
#include "TFEL/Utilities/Global.hxx"
#include "TFEL/Utilities/LaTeXGenerator.hxx"
#include "TFEL/Utilities/MarkdownGenerator.hxx"
#include "TFEL/Utilities/TestDocumentation.hxx"
#include "TFEL/Utilities/TestDocParser.hxx"
#include "TFEL/Utilities/MTestDocParser.hxx"
#include "TFEL/Utilities/ConfigParser.hxx"
#include "TFEL/Utilities/TestDocMain.hxx"

namespace tfel::utilities {

  template <typename Parser>
  static void parse_files(
      std::ostream& log,
      std::map<std::string, std::vector<TestDocumentation>>& tests,
      const std::string& ext) {
    using namespace tfel::system;
    auto files = tfel::system::recursiveFind(".*\\." + ext, ".", false);
    for (const auto& d : files) {
      const auto path = systemCall::getAbsolutePath(d.first);
      log << "entering directory '" + path + "'\n";
      for (const auto& f : d.second) {
        const auto name = d.first + '/' + f;
        try {
          Parser(name).addDocumentation(tests);
        } catch (std::exception& e) {
          log << TerminalColors::Reset;
          log << "treatment of file '" + f + "' failed: " << e.what() << '\n';
          log << TerminalColors::Reset;
        }
      }
    }
  }

  static std::string getOutputDirectory(const std::string& f) {
    const auto pos = f.rfind('/');
    if (pos == std::string::npos) {
      return "";
    }
    return f.substr(0, pos);
  }  // end of getOutputDirectory

  static void declareKeys(const std::string& f) {
    ConfigParser parser;
    parser.execute(getKeysMap(), f);
  }

  static void declareCategories(const std::string& f) {
    ConfigParser parser;
    parser.execute(getCategoriesMap(), f);
  }

  static void declareTranslations(const std::string& f) {
    ConfigParser parser;
    parser.execute(getTranslationsMap(), f);
  }

  TestDocMain::TestDocMain(const int argc, const char* const* argv)
      : ArgumentParserBase<TestDocMain>(argc, argv) {
    using namespace tfel::utilities;
    this->registerArgumentCallBacks();
    this->parseArguments();
    if (this->outputFile.empty()) {
      std::cerr << "TestDocMain: no output file specified\n";
      std::cerr << this->getUsageDescription() << '\n';
      std::exit(EXIT_FAILURE);
    }
    this->opts.outputDirectory = getOutputDirectory(this->outputFile);
    this->output.open(this->outputFile.c_str());
    raise_if(!this->output,
             "TestDocMain: can't open output file "
             "'" +
                 this->outputFile + '\'');
    if (!this->logFile.empty()) {
      this->logptr = std::make_shared<std::ofstream>(argv[2]);
      raise_if(!(*(this->logptr)),
               "TestDocMain: can't open log file "
               "'" +
                   this->logFile + '\'');
      this->log = this->logptr.get();
    } else {
      this->log = &std::cerr;
    }
    if (this->opts.lang.empty()) {
      this->opts.lang = "english";
    }
    raise_if((this->latex) && (this->markdown),
             "TestDocMain::TestDocMain: "
             "can't choose both latex and markdown "
             "generator at the same time");
    if ((!this->latex) && (!this->markdown)) {
      this->latex = true;
    }
  }  // end of TestDocMain::TestDocMain

  void TestDocMain::treatUnknownArgument() {
    const auto& s = this->currentArgument->as_string();
    if (s[0] == '-') {
      ArgumentParserBase<TestDocMain>::treatUnknownArgument();
      std::cerr << "TestDocMain: unsupported option '" << s << "'\n";
      std::cerr << this->getUsageDescription() << '\n';
      std::exit(EXIT_FAILURE);
    }
    if (this->outputFile.empty()) {
      this->outputFile = s;
    } else {
      std::cerr << "TestDocMain: output file already specified\n";
      std::cerr << this->getUsageDescription() << '\n';
      std::exit(EXIT_FAILURE);
    }
  }  // end of TestDocMain::treatUnknownArgument

  void TestDocMain::registerArgumentCallBacks() {
    this->registerNewCallBack("--lang", &TestDocMain::treatLang,
                              "specify output language (french,english)", true);
    this->registerNewCallBack("--fragment", "-f", &TestDocMain::treatFragment,
                              "don't print TeX header", false);
    this->registerNewCallBack("--split", "-s", &TestDocMain::treatSplit,
                              "split outputs by categories", false);
    this->registerNewCallBack("--src", &TestDocMain::treatSrc,
                              "specify root of sources", true);
    this->registerNewCallBack("--log", &TestDocMain::treatLogFile,
                              "specify a log file", true);
    this->registerNewCallBack("--keys", &TestDocMain::treatKeyFile,
                              "specify a key file", true);
    this->registerNewCallBack("--categories", &TestDocMain::treatCategoryFile,
                              "specify a category file", true);
    this->registerNewCallBack("--prefix", &TestDocMain::treatPrefix,
                              "specify the application installation directory",
                              true);
    this->registerNewCallBack("--translations",
                              &TestDocMain::treatTranslationFile,
                              "specify a translation file", true);
    this->registerNewCallBack("--mtest", &TestDocMain::treatMTest,
                              "add mtest files");
    this->registerNewCallBack("--md", &TestDocMain::treatMarkdown,
                              "generate markdown file");
    this->registerNewCallBack("--markdown", &TestDocMain::treatMarkdown,
                              "generate markdown file");
    this->registerNewCallBack("--latex", &TestDocMain::treatLaTeX,
                              "generate latex file");
  }  // end of TestDocMain::registerArgumentCallBacks

  void TestDocMain::treatLaTeX() { this->latex = true; }

  void TestDocMain::treatMarkdown() { this->markdown = true; }

  void TestDocMain::treatMTest() { this->mtest = true; }

  void TestDocMain::treatFragment() { this->opts.standAlone = false; }

  void TestDocMain::treatSplit() { this->opts.split = true; }

  void TestDocMain::treatLogFile() {
    if (!this->logFile.empty()) {
      std::cerr << "TestDocMain: log file already specified\n";
      std::cerr << this->getUsageDescription() << '\n';
      std::exit(EXIT_FAILURE);
    }
    this->logFile = this->currentArgument->getOption();
    raise_if(this->logFile.empty(),
             "TestDocMain::treatlogFile: "
             "no option given to the --logFile argument");
  }  // end of TestDocMain::treatLogFile

  void TestDocMain::treatPrefix() {
    if (!this->opts.prefix.empty()) {
      std::cerr << "TestDocMain: prefix already specified\n";
      std::cerr << this->getUsageDescription() << '\n';
      exit(EXIT_FAILURE);
    }
    this->opts.prefix = this->currentArgument->getOption();
    raise_if(this->opts.prefix.empty(),
             "TestDocMain::treatprefix: "
             "no option given to the --prefix argument");
  }  // end of TestDocMain::treatPrefix

  void TestDocMain::treatSrc() {
    if (!this->srcdir.empty()) {
      std::cerr << "TestDocMain: srcdir file already specified\n";
      std::cerr << this->getUsageDescription() << '\n';
      exit(EXIT_FAILURE);
    }
    this->srcdir = this->currentArgument->getOption();
    raise_if(this->srcdir.empty(),
             "TestDocMain::treatSrc: "
             "no option given to the --src argument");
  }  // end of TestDocMain::treatSrc

  void TestDocMain::treatLang() {
    if (!this->opts.lang.empty()) {
      std::cerr << "TestDocMain: lang file already specified\n";
      std::cerr << this->getUsageDescription() << '\n';
      exit(EXIT_FAILURE);
    }
    this->opts.lang = this->currentArgument->getOption();
    raise_if(this->opts.lang.empty(),
             "TestDocMain::treatLang: "
             "no option given to the --lang argument");
  }  // end of TestDocMain::treatLang

  void TestDocMain::treatKeyFile() {
    declareKeys(this->currentArgument->getOption());
  }  // end of TestDocMain::treatKeyFile

  void TestDocMain::treatCategoryFile() {
    declareCategories(this->currentArgument->getOption());
  }  // end of TestDocMain::treatCategoryFile

  void TestDocMain::treatTranslationFile() {
    declareTranslations(this->currentArgument->getOption());
  }  // end of TestDocMain::treatTranslationFile

  std::string TestDocMain::getVersionDescription() const { return "1.1"; }

  std::string TestDocMain::getUsageDescription() const {
    return "Usage: tfel-doc [options] output";
  }

  int TestDocMain::execute() {
    using namespace tfel::system;
    auto chdir = [](const std::string& d) {
      try {
        systemCall::changeCurrentWorkingDirectory(d);
      } catch (std::exception& e) {
        raise(
            "TestDocMain::execute: "
            "can't move to directory '" +
            d + "\' (" + std::string(e.what()) + ")'");
      }
    };
    const auto cpath = [] {
      try {
        return systemCall::getCurrentWorkingDirectory();
      } catch (std::exception& e) {
        raise(
            "TestDocMain::execute: can't get real path of "
            "current directory (" +
            std::string(e.what()) + "), aborting");
      }
    }();
    if (!this->srcdir.empty()) {
      chdir(this->srcdir);
    }
    // all the tests, sorted by category
    auto tests = std::map<std::string, std::vector<TestDocumentation>>{};
    // testdoc files
    parse_files<TestDocParser>(*(this->log), tests, "testdoc");
    if (this->mtest) {
      parse_files<MTestDocParser>(*(this->log), tests, "mtest");
    }
    if (!this->srcdir.empty()) {
      chdir(cpath);
    }
    // output
    if (this->latex) {
      writeLaTeXFile(this->output, tests, this->opts);
    }
    if (this->markdown) {
      writeMarkdownFile(this->output, tests, this->opts);
    }
    // a short summary
    auto count = std::vector<TestDocumentation>::size_type{0u};
    for (const auto& t : tests) {
      count += t.second.size();
    }
    *(this->log) << count << " tests treated in " << tests.size()
                 << " categories\n";
    return EXIT_SUCCESS;
  }

  TestDocMain::~TestDocMain() = default;

}  // end of namespace tfel::utilities
