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

#include <iostream>
#include <algorithm>
#include <stdexcept>
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
        try {
          this->execute(i);
        } catch (std::exception& e) {
          message("Treatment of file '" + i + "' failed (" +
                  std::string(e.what()) + ")");
        } catch (...) {
          message("Treatment of file '" + i + "' failed (unknown exception)");
        }
      }
    }  // end of execute

    /*!
     * \param[in] i: input file
     */
    void execute(const std::string& i) {
      message("Begin treatment of file '" + i + "'");
      const auto opts = [] {
        auto lopts = tfel::utilities::CxxTokenizerOptions{};
        lopts.shallMergeStrings = true;
        lopts.treatPreprocessorDirectives = false;
        lopts.charAsString = true;
        lopts.dotAsSeparator = false;
        lopts.addCurlyBraces = true;
        return lopts;
      }();
      const auto tokens = [this, &i, &opts] {
        tfel::utilities::CxxTokenizer tokenizer(i, opts);
        tokenizer.stripComments();
        auto ltokens = std::vector<tfel::utilities::Token>{tokenizer.begin(),
                                                           tokenizer.end()};
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
        return ltokens;
      }();
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
      const auto test = atcf.generate(n, d);
      for (const auto& t : this->targets) {
        test->addInputFileGenerator(ifgf.get(n, t));
      }
      test->generate();
      message("End treatment of file '" + i + "'");
    }  // end of execute

   private:
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
                         "MTestMain::treatUnknownArgument: "
                         "bas substitution pattern '" +
                             s1 + "'");
          const auto s2 = this->currentArgument->getOption();
          tfel::raise_if(s2.empty(),
                         "MTestMain::treatUnknownArgument: "
                         "no substitution given for pattern '" +
                             s1 + "'");
          //           if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL2) {
          //             mfront::getLogStream() << "substituting '" << s1 << "'
          //             by '" << s2
          //                                    << "'\n";
          //           }
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
      if ((a[0] == '-') || (a[0] == '/')) {
        tfel::utilities::ArgumentParserBase<
            MFMTestGenerator>::treatUnknownArgument();
      }
      if (std::find(std::begin(this->input_files), std::end(this->input_files),
                    a) == std::end(this->input_files)) {
        this->input_files.push_back(a);
      }
    }  // end of treatUnknownArgument

    std::string getVersionDescription() const override { return "1.0"; }

    std::string getUsageDescription() const override {
      return "Usage: " + this->programName + " [options] [files]";
    }

   private:
    //! list of input files
    std::vector<std::string> input_files;
    //! list of targeted code
    std::vector<std::string> targets;
    //! external commands
    std::vector<std::string> ecmds;
    //! substitutions
    std::map<std::string, std::string> substitutions;
  };  // end of MFMTestGenerator

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
