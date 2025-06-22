/*!
 * \file   mfm-test-generator/src/mfm-test-generator.cxx
 * \brief
 * \author Thomas Helfer
 * \date   13 avril 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <regex>
#include <iostream>
#include <algorithm>
#include <stdexcept>

#ifdef MFM_TEST_GENERATOR_HAVE_MADNEX
#include "Madnex/Config.hxx"
#endif /* MFM_TEST_GENERATOR_HAVE_MADNEX */

#ifdef MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT
#include "Madnex/DataBase.hxx"
#include "Madnex/MFMTestGeneratorTest.hxx"
#endif

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Utilities/ArgumentParserBase.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFMTestGenerator/Log.hxx"
#include "MFMTestGenerator/AbstractTestCase.hxx"
#include "MFMTestGenerator/TestCaseParameters.hxx"
#include "MFMTestGenerator/AbstractTestCaseFactory.hxx"
#include "MFMTestGenerator/InputFileGeneratorFactory.hxx"

namespace mfmtg {

  static void load_plugins(const std::string& libs) {
    using namespace tfel::system;
    debug("Loading user defined test cases");
    auto& lm = ExternalLibraryManager::getExternalLibraryManager();
    for (const auto& l : tfel::utilities::tokenize(libs, ',')) {
      debug("Loading library '" + l + "'");
      lm.loadLibrary(l);
    }
  }  // load plugins

  struct MFMTestGenerator
      : public tfel::utilities::ArgumentParserBase<MFMTestGenerator> {
    MFMTestGenerator(const int argc, const char* const* const argv)
        : tfel::utilities::ArgumentParserBase<MFMTestGenerator>(argc, argv) {
      this->registerCommandLineCallBacks();
      this->parseArguments();
      if (this->targets.empty()) {
        tfel::raise("MFMTestGenerator::MFMTestGenerator: no targets specified");
      }
      // calling mfm plugins
      const auto libs = ::getenv("MFM_TEST_GENERATOR_ADDITIONAL_LIBRARIES");
      if (libs != nullptr) {
        load_plugins(libs);
      }
    }  // end of MFMTestGenerator

    void execute() {
      for (const auto& i : this->input_files) {
        this->execute(i);
      }
    }  // end of execute

    /*!
     * \param[in] i: input file
     */
    void execute(const std::string& i) {
      if (tfel::utilities::starts_with(i, "madnex:")) {
#ifdef MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT
        this->treatMadnexPath(i);
#else  /* MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT */
        tfel::raise(
            "MFMTestGenerator::execute: "
            "madnex support is not enabled");
#endif /* MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT */
      }
      const auto ext = [&i]() -> std::string {
        const auto pos = i.rfind('.');
        if (pos != std::string::npos) {
          return i.substr(pos);
        }
        return "";
      }();
      if ((ext == ".madnex") || (ext == ".mdnx") || (ext == ".edf")) {
#ifdef MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT
        this->treatMadnexInputFile(i);
#else  /* MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT */
        tfel::raise(
            "MFMTestGenerator::execute: "
            "madnex support is not enabled");
#endif /* MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT */
      }
      this->treatStandardInputFile(i);
    }  // end of execute

   private:
    //! \return the options used to parse a mfm-test-generator file
    static tfel::utilities::CxxTokenizerOptions getInputFileParsingOptions() {
      auto lopts = tfel::utilities::CxxTokenizerOptions{};
      lopts.shallMergeStrings = true;
      lopts.treatPreprocessorDirectives = false;
      lopts.charAsString = true;
      lopts.dotAsSeparator = false;
      lopts.addCurlyBraces = true;
      return lopts;
    }  // end of getInputFileParsingOptions

    //! \brief register the command line call backs
    void registerCommandLineCallBacks() {
      this->registerCallBack(
          "--verbose", CallBack(
                           "set verbose output",
                           [this] {
                             if (this->currentArgument->getOption().empty()) {
                               mfront::setVerboseMode(mfront::VERBOSE_LEVEL1);
                             } else {
                               const auto& l =
                                   this->currentArgument->getOption();
                               mfront::setVerboseMode(l);
                             }
                           },
                           true));
      this->registerCallBack(
          "--target",
          CallBack(
              "specify a target code",
              [this] {
                const auto& t = this->currentArgument->getOption();
                if (std::find(this->targets.cbegin(), this->targets.cend(),
                              t) == this->targets.cend()) {
                  this->targets.push_back(t);
                }
              },
              true));
      this->registerCallBack(
          "--plugins",
          CallBack(
              "specify a list of shared libraries containing user defined "
              "test cases or generators, separated by a comma ','",
              [this] { load_plugins(this->currentArgument->getOption()); },
              true));
      this->registerCallBack(
          "--targets",
          CallBack(
              "specify a set targets separated by commas",
              [this] {
                for (const auto& t : tfel::utilities::tokenize(
                         this->currentArgument->getOption(), ',')) {
                  if (std::find(this->targets.cbegin(), this->targets.cend(),
                                t) == this->targets.cend()) {
                    this->targets.push_back(t);
                  }
                }
              },
              true));
#ifdef MFM_TEST_GENERATOR_HAVE_MADNEX
      auto treatBehaviour = [this] {
        if (!this->behaviour.empty()) {
          tfel::raise("mfm-test-generator: behaviour already defined");
        }
        const auto& o = this->currentArgument->getOption();
        if (o.empty()) {
          tfel::raise(
              "mfm-test-generator: no option given to "
              "the --behaviour command line argument");
        }
        this->behaviour = o;
      };
      auto treatMaterial = [this] {
        if (!this->material.empty()) {
          tfel::raise("mfm-test-generator: material already defined");
        }
        const auto& o = this->currentArgument->getOption();
        if (o.empty()) {
          tfel::raise(
              "mfm-test-generator: no option given to "
              "the --material command line argument");
        }
        this->material = o;
      };
      auto treatTest = [this] {
        if (!this->test.empty()) {
          tfel::raise("mfm-test-generator: test already defined");
        }
        const auto& o = this->currentArgument->getOption();
        if (o.empty()) {
          tfel::raise(
              "mfm-test-generator: no option given to "
              "the --test command line argument");
        }
        this->test = o;
      };
      auto treatAllTests = [this] {
        if (!this->test.empty()) {
          tfel::raise("mfm-test-generator: test already defined");
        }
        this->test = ".+";
      };
      this->registerCallBack(
          "--behaviour", "-b",
          CallBack("set the behaviour", treatBehaviour, true));
      this->registerCallBack("--material", "-m",
                             CallBack("set the material", treatMaterial, true));
      this->registerCallBack("--test", "-t",
                             CallBack("set the test name", treatTest, true));
      this->registerCallBack(
          "--all-tests", CallBack("select all tests", treatAllTests, false));
#ifdef _WIN32
      this->registerCallBack(
          "/behaviour", "/b",
          CallBack("set the behaviour", treatBehaviour, true));
      this->registerCallBack("/material", "/m",
                             CallBack("set the material", treatMaterial, true));
      this->registerCallBack("/test", "/t",
                             CallBack("set the test name", treatTest, true));
      this->registerCallBack(
          "/all-tests", CallBack("select all tests", treatAllTests, false));
#endif /* _WIN32 */
#endif /* MFM_TEST_GENERATOR_HAVE_MADNEX */
    }  // end of registerCommandLineCallBacks

    const tfel::utilities::Argument& getCurrentCommandLineArgument() const {
      return *(this->currentArgument);
    }

    void treatUnknownArgument() override {
      using tfel::utilities::starts_with;
      const auto& a = this->currentArgument->as_string();
      if (a.empty()) {
        return;
      }
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
                         "MFMTestGenerator::treatUnknownArgument: "
                         "bas substitution pattern '" +
                             s1 + "'");
          const auto s2 = this->currentArgument->getOption();
          tfel::raise_if(s2.empty(),
                         "MFMTestGenerator::treatUnknownArgument: "
                         "no substitution given for pattern '" +
                             s1 + "'");
          tfel::raise_if(!this->substitutions.insert({s1, s2}).second,
                         "MFMTestGenerator::treatUnknownArgument: "
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
      if ((a[0] == '-') || (a[0] == '/')) {
        tfel::utilities::ArgumentParserBase<
            MFMTestGenerator>::treatUnknownArgument();
      }
      if (std::find(std::begin(this->input_files), std::end(this->input_files),
                    a) == std::end(this->input_files)) {
        this->input_files.push_back(a);
      }
    }  // end of treatUnknownArgument

    std::string getVersionDescription() const override {
      return "1.0";
    }  // end of getVersionDescription

    std::string getUsageDescription() const override {
      return "Usage: " + this->programName + " [options] [files]";
    }  // end of getUsageDescription

    void treatTest(const std::vector<tfel::utilities::Token>& tokens) const {
      auto p = tokens.begin();
      const auto pe = tokens.end();
      const auto d = TestCaseParameter::read(p, pe);
      if (!d.is<TestCaseParameters>()) {
        tfel::raise(
            "MFMTestGenerator::execute: "
            "invalid input file");
      }
      // name of the test case
      const auto n = get<std::string>(d, "test_case");
      const auto& atcf = AbstractTestCaseFactory::get();
      const auto& ifgf = InputFileGeneratorFactory::get();
      const auto t = atcf.generate(n, d);
      for (const auto& target : this->targets) {
        t->addInputFileGenerator(ifgf.get(n, target));
      }
      t->generate();
    }  // end of treatTest

    void treatStandardInputFile(const std::string& i) const {
#ifdef MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT
      if (!this->test.empty()) {
        tfel::raise(
            "MFMTestGenerator::treatStandardInputFile: "
            "specifying a test and a standard input file is not allowed");
      }
      if (!this->behaviour.empty()) {
        tfel::raise(
            "MFMTestGenerator::treatStandardInputFile: "
            "specifying a behaviour and a standard input file is not allowed");
      }
      if (!this->test.empty()) {
        tfel::raise(
            "MFMTestGenerator::treatStandardInputFile: "
            "specifying a test and a standard input file is not allowed");
      }
#endif /* MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT */
      message("Begin treatment of file '" + i + "'");
      const auto opts = getInputFileParsingOptions();
      tfel::utilities::CxxTokenizer tokenizer(i, opts);
      tokenizer.stripComments();
      auto ltokens = std::vector<tfel::utilities::Token>{tokenizer.begin(),
                                                         tokenizer.end()};
      this->applySubstitutionsAndExternalCommands(ltokens, opts);
      this->treatTest(ltokens);
      message("End treatment of file '" + i + "'");
    }  // end of treatStandardInputFile

#ifdef MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT

    void treatMadnexInputFile(const std::string& i) const {
      if (this->behaviour.empty()) {
        tfel::raise(
            "MFMTestGenerator::treatMadnexInputFile: "
            "no behaviour specified");
      }
      if (this->test.empty()) {
        tfel::raise(
            "MFMTestGenerator::treatMadnexInputFile: "
            "no test specified");
      }
      madnex::DataBase d(i);
      std::regex r(this->test);
      const auto m = this->material == "<none>" ? "" : this->material;
      auto tests = std::vector<std::string>{};
      for (const auto& tname : d.getAvailableMFMTestGeneratorTests(
               this->material, this->behaviour)) {
        if (std::regex_match(tname, r)) {
          tests.push_back(tname);
        }
      }
      if (tests.empty()) {
        tfel::raise(
            "MFMTestGenerator::treatMadnexInputFile: "
            "no test matches regular expression "
            "'" +
            this->test + "'");
      }
      for (const auto& t : tests) {
        this->treatMadnexTest(this->material, this->behaviour, t, i);
      }
    }  // end of treatMadnexInputFile

    void treatMadnexPath(const std::string& i) const {
      if (!this->material.empty()) {
        tfel::raise(
            "MFMTestGenerator::treatMadnexPath: "
            "specifying a material and a madnex path is not allowed");
      }
      if (!this->behaviour.empty()) {
        tfel::raise(
            "MFMTestGenerator::treatMadnexPath: "
            "specifying a behaviour and a madnex path is not allowed");
      }
      if (!this->test.empty()) {
        tfel::raise(
            "MFMTestGenerator::treatMadnexPath: "
            "specifying a test and a madnex path is not allowed");
      }
      const auto [f, m, b, t] = [&i] {
        const auto details = tfel::utilities::tokenize(i, ':', true);
        if (details.size() != 5u) {
          tfel::raise(
              "MFMTestGenerator::treatMadnexPath: "
              "invalid path '" +
              i + "'");
        }
        const auto material_name = details[2] == "<none>" ? "" : details[2];
        return std::make_tuple(std::move(details[1]), std::move(material_name),
                               std::move(details[3]), std::move(details[4]));
      }();
      this->treatMadnexTest(m, b, t, f);
    }  // end of treatMadnexPath

    void treatMadnexTest(const std::string& m,
                         const std::string& b,
                         const std::string& t,
                         const std::string& f) const {
      const auto test_description = [&] {
        if ((m.empty()) || (m == "<none>")) {
          return "test '" + t + "' associated with behaviour '" + b +
                 "' in file '" + f + "'";
        }
        return "test '" + t + "' associated with behaviour '" + b +
               "' of material '" + m + "' in file '" + f + "'";
      }();
      message("Begin treatment of " + test_description);
      const auto mfm_test = madnex::getMFMTestGeneratorTest(f, m, b, t);
      const auto opts = getInputFileParsingOptions();
      tfel::utilities::CxxTokenizer tokenizer(opts);
      tokenizer.parseString(mfm_test.test);
      tokenizer.stripComments();
      auto ltokens = std::vector<tfel::utilities::Token>{tokenizer.begin(),
                                                         tokenizer.end()};
      this->applySubstitutionsAndExternalCommands(ltokens, opts);
      this->treatTest(ltokens);
      message("End treatment of " + test_description);
    }  // end of treatMadnexTest

#endif /* MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT */

    void applySubstitutionsAndExternalCommands(
        std::vector<tfel::utilities::Token>& ltokens,
        const tfel::utilities::CxxTokenizerOptions& opts) const {
      // substitutions
      const auto pe = this->substitutions.end();
      for (auto& token : ltokens) {
        auto p = this->substitutions.find(token.value);
        if (p != pe) {
          token.value = p->second;
          if (((p->second.front() == '\'') && (p->second.back() == '\'')) ||
              ((p->second.front() == '"') && (p->second.back() == '"'))) {
            token.flag = tfel::utilities::Token::String;
          }
        }
      }
      // treating external commands
      for (const auto& c : this->ecmds) {
        tfel::utilities::CxxTokenizer etokenizer(opts);
        try {
          etokenizer.parseString(c);
        } catch (std::exception& e) {
          tfel::raise(
              "MFMTestGenerator::execute : "
              "error while parsing external command "
              "'" +
              c + "'\n" + std::string(e.what()));
        }
        ltokens.insert(ltokens.begin(), etokenizer.begin(), etokenizer.end());
      }
    }  // end of applySubstitutionsAndExternalCommands

   private:
    //! \brief list of input files
    std::vector<std::string> input_files;
    //! \brief list of targeted code
    std::vector<std::string> targets;
    //! \brief external commands
    std::vector<std::string> ecmds;
    //! \brief substitutions
    std::map<std::string, std::string> substitutions;
#ifdef MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT
    //! \brief material name (only useful for madnex files)
    std::string material;
    //! \brief material name (required for madnex files)
    std::string behaviour;
    //! \brief test name (required for madnex files)
    std::string test;
#endif /* MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT */
  };   // end of MFMTestGenerator

}  // end of namespace mfmtg

int main(const int argc, const char* const* const argv) {
#if !defined(__GLIBCXX__)
  try {
#endif /* not defined(__GLIBCXX__) */
    mfmtg::MFMTestGenerator m(argc, argv);
    m.execute();
#if !defined(__GLIBCXX__)
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
#endif /* !defined(__GLIBCXX__) */
  return EXIT_SUCCESS;
}
