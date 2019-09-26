/*!
 * \file   mfm-test-generator/src/mfm-test-generator.cxx
 * \brief
 * \author Thomas Helfer
 * \date   13 avril 2017
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Utilities/ArgumentParserBase.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFMTestGenerator/AbstractTestCase.hxx"
#include "MFMTestGenerator/TestCaseParameters.hxx"
#include "MFMTestGenerator/AbstractTestCaseFactory.hxx"
#include "MFMTestGenerator/InputFileGeneratorFactory.hxx"

namespace mfmtg {

  static void debug(const std::string& msg) {
    if (mfront::getVerboseMode() >= mfront::VERBOSE_DEBUG) {
      mfront::getLogStream() << msg;
    }
  }

  static void message(const std::string& msg) {
    if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL2) {
      mfront::getLogStream() << msg;
    }
  }

  struct MFMTestGenerator
      : public tfel::utilities::ArgumentParserBase<MFMTestGenerator> {
    // simple alias
    static constexpr const mfront::VerboseLevel VERBOSE_LEVEL2 =
        mfront::VERBOSE_LEVEL2;

    MFMTestGenerator(const int argc, const char* const* const argv)
        : tfel::utilities::ArgumentParserBase<MFMTestGenerator>(argc, argv) {
      this->registerCommandLineCallBacks();
      this->parseArguments();
      if (this->targets.empty()) {
        tfel::raise("MFMTestGenerator::MFMTestGenerator: no targets specified");
      }
    }  // end of MFMTestGenerator

    void execute() {
      using namespace tfel::system;
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      const auto sep = ';';
#else
      const auto sep = ':';
#endif
      debug("Loading user defined test cases\n");
      // calling mfm plugins
      const auto libs = ::getenv("MFM_TEST_GENERATOR_ADDITIONAL_LIBRARIES");
      if (libs != nullptr) {
        auto& lm = ExternalLibraryManager::getExternalLibraryManager();
        for (const auto& l : tfel::utilities::tokenize(libs, sep)) {
          debug("Loading library '" + l + "'\n");
          lm.loadLibrary(l);
        }
      }
      for (const auto& i : this->input_files) {
        try {
          this->execute(i);
        } catch (std::exception& e) {
          message("Treatment of file '" + i + "' failed (" +
                  std::string(e.what()) + ")\n");
        } catch (...) {
          message("Treatment of file '" + i + "' failed (unknown exception)\n");
        }
      }
    }  // end of execute

    /*!
     * \param[in] i: input file
     */
    void execute(const std::string& i) {
      message("Begin treatment of file '" + i + "'\n");
      const auto o = [] {
        auto opts = tfel::utilities::CxxTokenizerOptions{};
        opts.shallMergeStrings = true;
        opts.treatPreprocessorDirectives = false;
        opts.charAsString = true;
        opts.dotAsSeparator = false;
        opts.addCurlyBraces = true;
        return opts;
      }();
      const tfel::utilities::CxxTokenizer tokenizer(i, o);
      auto p = tokenizer.begin();
      const auto pe = tokenizer.end();
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
      message("End treatment of file '" + i + "'\n");
    }  // end of execute

   private:
    void registerCommandLineCallBacks() {
      this->registerCallBack(
          "--verbose",
          CallBack("set verbose output",
                   [this] {
                     if (this->currentArgument->getOption().empty()) {
                       mfront::setVerboseMode(mfront::VERBOSE_LEVEL1);
                     } else {
                       const auto& l = this->currentArgument->getOption();
                       mfront::setVerboseMode(l);
                     }
                   },
                   true));
      this->registerCallBack(
          "--target",
          CallBack("specify a target code",
                   [this] {
                     const auto& t = this->currentArgument->getOption();
                     if (std::find(this->targets.cbegin(), this->targets.cend(),
                                   t) == this->targets.cend()) {
                       this->targets.push_back(t);
                     }
                   },
                   true));
      this->registerCallBack(
          "--targets",
          CallBack("specify a set targets separated by commas",
                   [this] {
                     for (const auto& t : tfel::utilities::tokenize(
                              this->currentArgument->getOption(), ',')) {
                       if (std::find(this->targets.cbegin(),
                                     this->targets.cend(),
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
      const auto& a = this->currentArgument->as_string();
      if (a.empty()) {
        return;
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
