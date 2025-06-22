/*!
 * \file  tfel-doc/src/TestDocMain.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

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

#include <sys/param.h>

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

namespace tfel {

  namespace utilities {

    template <typename Parser>
    static void parse_files(
        std::ostream& log,
        std::map<std::string, std::vector<TestDocumentation>>& tests,
        const std::string& ext) {
      char path[MAXPATHLEN];
      auto files = tfel::system::recursiveFind(".*\\." + ext, ".", false);
      for (const auto& d : files) {
        if (realpath(d.first.c_str(), path) == nullptr) {
          log << "entering directory " << d.first << '\n';
        } else {
          log << "entering directory " << path << '\n';
        }
        for (const auto& f : d.second) {
          auto name = d.first + '/' + f;
          try {
            Parser{name}.addDocumentation(tests);
          } catch (std::exception& e) {
            log << TerminalColors::Reset;
            log << "treatment of file '" + f + "' failed : " << e.what()
                << '\n';
            log << TerminalColors::Reset;
          }
        }
      }
    }

    static std::string getOutputDirectory(const std::string& f) {
      using namespace std;
      string::size_type pos = f.rfind('/');
      if (pos == string::npos) {
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
      using namespace std;
      using namespace tfel::utilities;
      this->registerArgumentCallBacks();
      this->parseArguments();
      if (this->outputFile.empty()) {
        cerr << "TestDocMain : no output file specified\n";
        cerr << this->getUsageDescription() << endl;
        exit(EXIT_FAILURE);
      }
      this->opts.outputDirectory = getOutputDirectory(this->outputFile);
      this->output.open(this->outputFile.c_str());
      if (!this->output) {
        string msg("TestDocMain : can't open output file '");
        msg += this->outputFile;
        msg += '\'';
        throw(runtime_error(msg));
      }
      if (!this->logFile.empty()) {
        this->logptr = shared_ptr<ostream>(new ofstream(argv[2]));
        if (!(*(this->logptr))) {
          string msg("TestDocMain : can't open log file '");
          msg += this->logFile;
          msg += '\'';
          throw(runtime_error(msg));
        }
        this->log = this->logptr.get();
      } else {
        this->log = &cerr;
      }
      if (this->opts.lang.empty()) {
        this->opts.lang = "english";
      }
      if ((this->latex) && (this->markdown)) {
        throw(
            runtime_error("TestDocMain::TestDocMain : "
                          "can't choose both latex and markdown "
                          "generator at the same time"));
      }
      if ((!this->latex) && (!this->markdown)) {
        this->latex = true;
      }
    }  // end of TestDocMain::TestDocMain

    void TestDocMain::treatUnknownArgument() {
      using namespace std;
      const auto& s = this->currentArgument->as_string();
      if (s[0] == '-') {
        ArgumentParserBase<TestDocMain>::treatUnknownArgument();
        cerr << "TestDocMain : unsupported option '" << s << "'\n";
        cerr << this->getUsageDescription() << endl;
        exit(EXIT_FAILURE);
      }
      if (this->outputFile.empty()) {
        this->outputFile = s;
      } else {
        cerr << "TestDocMain : output file already specified" << endl;
        cerr << this->getUsageDescription() << endl;
        exit(EXIT_FAILURE);
      }
    }  // end of TestDocMain::treatUnknownArgument

    void TestDocMain::registerArgumentCallBacks() {
      this->registerNewCallBack("--lang", &TestDocMain::treatLang,
                                "specify output language (french,english)",
                                true);
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
      this->registerNewCallBack(
          "--prefix", &TestDocMain::treatPrefix,
          "specify the application installation directory", true);
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
      using namespace std;
      if (!this->logFile.empty()) {
        cerr << "TestDocMain : log file already specified" << endl;
        cerr << this->getUsageDescription() << endl;
        exit(EXIT_FAILURE);
      }
      this->logFile = this->currentArgument->getOption();
      if (this->logFile.empty()) {
        string msg("TestDocMain::treatlogFile : ");
        msg += "no option given to the --logFile argument";
        throw(runtime_error(msg));
      }
    }  // end of TestDocMain::treatLogFile

    void TestDocMain::treatPrefix() {
      using namespace std;
      if (!this->opts.prefix.empty()) {
        cerr << "TestDocMain : prefix already specified" << endl;
        cerr << this->getUsageDescription() << endl;
        exit(EXIT_FAILURE);
      }
      this->opts.prefix = this->currentArgument->getOption();
      if (this->opts.prefix.empty()) {
        string msg("TestDocMain::treatprefix : ");
        msg += "no option given to the --prefix argument";
        throw(runtime_error(msg));
      }
    }  // end of TestDocMain::treatPrefix

    void TestDocMain::treatSrc() {
      using namespace std;
      if (!this->srcdir.empty()) {
        cerr << "TestDocMain : srcdir file already specified" << endl;
        cerr << this->getUsageDescription() << endl;
        exit(EXIT_FAILURE);
      }
      this->srcdir = this->currentArgument->getOption();
      if (this->srcdir.empty()) {
        string msg("TestDocMain::treatSrc : ");
        msg += "no option given to the --src argument";
        throw(runtime_error(msg));
      }
    }  // end of TestDocMain::treatSrc

    void TestDocMain::treatLang() {
      using namespace std;
      if (!this->opts.lang.empty()) {
        cerr << "TestDocMain : lang file already specified" << endl;
        cerr << this->getUsageDescription() << endl;
        exit(EXIT_FAILURE);
      }
      this->opts.lang = this->currentArgument->getOption();
      if (this->opts.lang.empty()) {
        string msg("TestDocMain::treatLang : ");
        msg += "no option given to the --lang argument";
        throw(runtime_error(msg));
      }
    }  // end of TestDocMain::treatLang

    void TestDocMain::treatKeyFile() {
      using namespace std;
      const auto& f = this->currentArgument->getOption();
      declareKeys(f);
    }  // end of TestDocMain::treatKeyFile

    void TestDocMain::treatCategoryFile() {
      using namespace std;
      const auto& f = this->currentArgument->getOption();
      declareCategories(f);
    }  // end of TestDocMain::treatCategoryFile

    void TestDocMain::treatTranslationFile() {
      using namespace std;
      const auto& f = this->currentArgument->getOption();
      declareTranslations(f);
    }  // end of TestDocMain::treatTranslationFile

    std::string TestDocMain::getVersionDescription() const {
      using namespace std;
      return "1.1";
    }

    std::string TestDocMain::getUsageDescription() const {
      using namespace std;
      string usage("Usage : tfel-doc [options] output");
      return usage;
    }

    int TestDocMain::execute() {
      using namespace std;
      using namespace tfel::utilities;
      char cpath[MAXPATHLEN];
      if (realpath(".", cpath) == nullptr) {
        throw(
            runtime_error("TestDocMain::execute : can't get real path of "
                          "current directory, aborting"));
      }
      if (!this->srcdir.empty()) {
        if (chdir(this->srcdir.c_str()) == -1) {
          throw(
              runtime_error("TestDocMain::execute : "
                            "can't move to directory '" +
                            this->srcdir + '\''));
        }
      }
      // all the tests, sorted by category
      auto tests = map<string, vector<TestDocumentation>>{};
      // testdoc files
      parse_files<TestDocParser>(*(this->log), tests, "testdoc");
      if (this->mtest) {
        parse_files<MTestDocParser>(*(this->log), tests, "mtest");
      }
      if (!this->srcdir.empty()) {
        if (chdir(cpath) == -1) {
          throw(
              runtime_error("TestDocMain::execute : "
                            "can't move to directory '" +
                            string(cpath) + '\''));
        }
      }
      // output
      if (this->latex) {
        writeLaTeXFile(this->output, tests, this->opts);
      }
      if (this->markdown) {
        writeMarkdownFile(this->output, tests, this->opts);
      }
      // a short summary
      auto count = map<string, vector<TestDocumentation>>::size_type{0u};
      for (const auto& t : tests) {
        count += t.second.size();
      }
      *(this->log) << count << " tests treated in " << tests.size()
                   << " categories" << endl;
      return EXIT_SUCCESS;
    }

    TestDocMain::~TestDocMain() = default;

  }  // end of namespace utilities

}  // end of namespace tfel
