/*!
 * \file   mfm.cxx
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
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <filesystem>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Utilities/ArgumentParserBase.hxx"
#include "MFront/MFrontDatabase.hxx"

struct MFM : tfel::utilities::ArgumentParserBase<MFM>, mfront::MFrontDatabase {
  MFM(const int argc, const char* const* const argv)
      : tfel::utilities::ArgumentParserBase<MFM>(argc, argv) {
    this->registerCommandLineCallBacks();
    this->parseArguments();
    try {
      for (const auto& e : this->getEntryPoints(this->query)) {
        if (this->show_libs) {
          std::cout << "- " << e.library << ": " << e.name;
        } else {
          std::cout << "- " << e.name;
        }
        if (this->show_sources) {
          try {
            std::cout << " (" << e.source << ')';
          } catch (std::exception&) {
            std::cout << " (undetermined source file)";
          }
        }
        std::cout << '\n';
      }
    } catch (std::exception& e) {
      if (this->vlevel >= VERBOSE_DEBUG) {
        std::clog << e.what() << "'\n";
      }
    }
  }  // end of MFM

 private:
  void registerCommandLineCallBacks() {
    auto set_verbose_level = [this] {
      if (getCurrentCommandLineArgument().getOption().empty()) {
        this->vlevel = VERBOSE_LEVEL1;
      } else {
        const auto& o = this->getCurrentCommandLineArgument().getOption();
        if (o == "quiet") {
          this->vlevel = VERBOSE_QUIET;
        } else if (o == "level0") {
          this->vlevel = VERBOSE_LEVEL0;
        } else if (o == "level1") {
          this->vlevel = VERBOSE_LEVEL1;
        } else if (o == "level2") {
          this->vlevel = VERBOSE_LEVEL2;
        } else if (o == "level3") {
          this->vlevel = VERBOSE_LEVEL3;
        } else if (o == "debug") {
          this->vlevel = VERBOSE_DEBUG;
        } else if (o == "full") {
          this->vlevel = VERBOSE_FULL;
        } else {
          tfel::raise("mfm: unknown verbose level '" + o + "'");
        }
      }
    };
    auto rsyntax = [this] {
      const auto o = this->currentArgument->getOption();
      if (o == "ECMAScript") {
        this->query.regular_expression_syntax =
            std::regex_constants::ECMAScript;
      } else if (o == "basic") {
        this->query.regular_expression_syntax = std::regex_constants::basic;
      } else if (o == "extended") {
        this->query.regular_expression_syntax = std::regex_constants::extended;
      } else if (o == "awk") {
        this->query.regular_expression_syntax = std::regex_constants::awk;
      } else if (o == "grep") {
        this->query.regular_expression_syntax = std::regex_constants::grep;
      } else if (o == "egrep") {
        this->query.regular_expression_syntax = std::regex_constants::egrep;
      } else {
        tfel::raise(
            "mfm: invalid regular "
            "expression syntax '" +
            o + "'");
      }
    };
    auto type_filter = [this] {
      const auto t = this->currentArgument->getOption();
      if (t == "material_property") {
        this->query.type = EntryPoint::MATERIALPROPERTY;
      } else if (t == "behaviour") {
        this->query.type = EntryPoint::BEHAVIOUR;
      } else if (t == "model") {
        this->query.type = EntryPoint::MODEL;
      } else {
        tfel::raise("mfm: invalid type '" + t + "'");
      }
    };
    auto name_filter = [this] {
      this->query.name_filter = this->currentArgument->getOption();
    };
    auto material_filter = [this] {
      this->query.material_filter = this->currentArgument->getOption();
    };
    auto interface_filter = [this] {
      this->query.interface_filter = this->currentArgument->getOption();
    };
    this->registerCallBack("--regex-syntax",
                           CallBack("select the syntax used by "
                                    "regular expressions. "
                                    "Valid values are: `ECMAScript`, `basic`, "
                                    "`extended`, `awk`, `grep`, `egrep`",
                                    rsyntax, true));
    this->registerCallBack(
        "--filter-by-interface",
        CallBack("filter entry points by interface", interface_filter, true));
    this->registerCallBack(
        "--filter-by-material",
        CallBack("filter entry points by material", material_filter, true));
    this->registerCallBack(
        "--filter-by-name",
        CallBack("filter entry points by name", name_filter, true));
    this->registerCallBack(
        "--filter-by-type",
        CallBack(
            "filter entry points by material knowledge type. Valid types are "
            "material-property, behaviour or model",
            type_filter, true));
    this->registerCallBack(
        "--verbose", CallBack("set verbose output", set_verbose_level, true));
    this->registerCallBack(
        "--show-libs",
        CallBack(
            "show library name in front "
            "of entry points",
            [this]() noexcept { this->show_libs = true; }, false));
    this->registerCallBack(
        "--show-sources",
        CallBack(
            "show the name of the MFront "
            "file used to generate the entry points",
            [this]() noexcept { this->show_sources = true; }, false));
  }  // end of registerCommandLineCallBacks

  const tfel::utilities::Argument& getCurrentCommandLineArgument() const {
    return *(this->currentArgument);
  }

  void treatUnknownArgument() override {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "mfm: " + m);
    };
    const auto& a = this->getCurrentCommandLineArgument();
    const auto& e = a.as_string();
    if (e.empty()) {
      return;
    }
    throw_if(e[0] == '-', "unsupported option '" + e + "'");
    if (this->vlevel >= VERBOSE_LEVEL1) {
      std::clog << "Treating library or directory '" << e << "'\n";
    }
    try {
      std::filesystem::path p(e);
      if (std::filesystem::is_directory(p)) {
        if (this->vlevel >= VERBOSE_LEVEL2) {
          std::clog << "Treating directory '" << e << "'\n";
        }
        const auto r = this->analyseDirectory(e, {.ignore_errors = true});
        if (this->vlevel >= VERBOSE_DEBUG) {
          if (!r.library_analysis_failures.empty()) {
            std::clog << "Error while treating directory'" << e << ":\n";
          }
          for (const auto& error : r.library_analysis_failures) {
            std::clog << "- " << error.library << ": "  //
                      << error.error_message << "'\n";
          }
        }
      } else if (std::filesystem::is_regular_file(p)) {
        if (this->vlevel >= VERBOSE_LEVEL2) {
          std::clog << "Treating library '" << e << "'\n";
        }
        this->analyseLibrary(e);
      } else {
        tfel::raise("'" + e + "' is neither a regular file nor a directory");
      }
    } catch (std::exception& error) {
      if (this->vlevel >= VERBOSE_DEBUG) {
        std::clog << "Error while treating library or directory'" << e << ": "
                  << error.what() << "'\n";
      }
    }
  }  // end of treatUnknownArgument

  std::string getVersionDescription() const override { return "1.0"; }

  std::string getUsageDescription() const override {
    return "Usage: " + this->programName + " [options] [files]";
  }

  /*!
   * \brief list the possible values for the logging facilities
   *        provided by the getVerboseMode/getLogStream functions
   */
  enum VerboseLevel {
    VERBOSE_QUIET = -1,  //<! no output
    VERBOSE_LEVEL0 = 0,  //<! minimal output
    VERBOSE_LEVEL1 = 1,  //<! a simpler output
    VERBOSE_LEVEL2 = 2,  //<! a much detailled output
    VERBOSE_LEVEL3 = 3,  //<! the finer level for standard user
    VERBOSE_DEBUG = 4,   //<! an output adapted for debugging
    VERBOSE_FULL = 5     //<! a very detailled output
  };                     // end of enum VerboseLevel

  mfront::MFrontDatabase::Query query;

  VerboseLevel vlevel = VERBOSE_LEVEL0;

  bool show_libs = false;

  bool show_sources = false;
};

int main(const int argc, const char* const* const argv) {
#if !defined(__GLIBCXX__)
  try {
#endif /* not defined(__GLIBCXX__) */
    MFM m(argc, argv);
#if !defined(__GLIBCXX__)
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
#endif /* !defined(__GLIBCXX__) */
  return EXIT_SUCCESS;
}
