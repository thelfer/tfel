/*!
 * \file   mfm.cxx
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
#include "TFEL/Utilities/ArgumentParserBase.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/MFrontLogStream.hxx"

struct MFMTestGenerator
    : public tfel::utilities::ArgumentParserBase<MFMTestGenerator> {
  // simple alias
  static constexpr const mfront::VerboseLevel VERBOSE_DEBUG =
      mfront::VERBOSE_DEBUG;

  MFMTestGenerator(const int argc, const char* const* const argv)
      : tfel::utilities::ArgumentParserBase<MFMTestGenerator>(argc, argv) {
    this->registerCommandLineCallBacks();
    this->parseArguments();
  }  // end of MFMTestGenerator

  void execute() {
    using namespace tfel::system;
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      const auto sep = ';';
#else
      const auto sep = ':';
#endif
    auto print = [](const mfront::VerboseLevel l, const std::string& msg) {
      if (mfront::getVerboseMode() >= l) {
        mfront::getLogStream() << msg;
      }
    };
    print(VERBOSE_DEBUG,"Loading test cases");
    // calling mfm plugins
    const auto libs = ::getenv("MFM_TEST_GENERATOR_ADDITIONAL_LIBRARIES");
    if (libs != nullptr) {
      auto& lm = ExternalLibraryManager::getExternalLibraryManager();
      for (const auto& l : tfel::utilities::tokenize(libs, sep)) {
        print(VERBOSE_DEBUG, "Loading library '" + l + "'");
        lm.loadLibrary(l);
      }
    }
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
  }  // end of registerCommandLineCallBacks

  const tfel::utilities::Argument& getCurrentCommandLineArgument() const {
    return *(this->currentArgument);
  }

  void treatUnknownArgument() override {}  // end of treatUnknownArgument

  std::string getVersionDescription() const override { return "1.0"; }

  std::string getUsageDescription() const override {
    return "Usage: " + this->programName + " [options] [files]";
  }
};  // end of MFMTestGenerator

int main(const int argc, const char* const* const argv) {
#if !defined(__GLIBCXX__)
  try {
#endif /* not defined(__GLIBCXX__) */
    MFMTestGenerator m(argc, argv);
    m.execute();
#if !defined(__GLIBCXX__)
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
#endif /* !defined(__GLIBCXX__) */
  return EXIT_SUCCESS;
}
