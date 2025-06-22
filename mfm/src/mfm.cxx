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
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Utilities/ArgumentParserBase.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"

struct MFM : public tfel::utilities::ArgumentParserBase<MFM> {
  MFM(const int argc, const char* const* const argv)
      : tfel::utilities::ArgumentParserBase<MFM>(argc, argv) {
    using tfel::system::ExternalLibraryManager;
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    this->registerCommandLineCallBacks();
    this->parseArguments();
    for (const auto& f : this->filters) {
      auto p =
          std::remove_if(this->epts.begin(), this->epts.end(),
                         [f](const EntryPoint& e) { return !f.f(e, f.o); });
      this->epts.erase(p, this->epts.end());
    }
    for (const auto& e : this->epts) {
      if (this->show_libs) {
        std::cout << "- " << e.library << ": " << e.name;
      } else {
        std::cout << "- " << e.name;
      }
      if (this->show_sources) {
        try {
          std::cout << " (" << elm.getSource(e.library, e.name) << ')';
        } catch (std::exception&) {
          std::cout << " (undetermined source file)";
        }
      }
      std::cout << '\n';
    }
  }  // end of MFM

 private:
  //! \brief an intermediate structure describing an entry point
  struct EntryPoint {
    //! type of material knowledge
    enum MaterialKnowledgeType {
      MATERIALPROPERTY,
      BEHAVIOUR,
      MODEL
    };  // end of MaterialKnowledgeType
    std::string library;
    std::string name;
    MaterialKnowledgeType type;
  };  // end of EntryPoint

  struct Filter {
    std::function<bool(const EntryPoint&, const std::string&)> f;
    std::string o;
  };

  template <typename FilterType>
  void add_filter(const std::string& o,
                  const std::string& d,
                  const FilterType& f) {
    auto add = [this, f] {
      const auto opt = this->currentArgument->getOption();
      tfel::raise_if(opt.empty(), "add_filter: no argument given");
      this->filters.push_back({f, opt});
    };
    this->registerCallBack(o, CallBack(d, add, true));
  }  // end of addFilter

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
        this->rs = std::regex_constants::ECMAScript;
      } else if (o == "basic") {
        this->rs = std::regex_constants::basic;
      } else if (o == "extended") {
        this->rs = std::regex_constants::extended;
      } else if (o == "awk") {
        this->rs = std::regex_constants::awk;
      } else if (o == "grep") {
        this->rs = std::regex_constants::grep;
      } else if (o == "egrep") {
        this->rs = std::regex_constants::egrep;
      } else {
        tfel::raise(
            "mfm: invalid regular "
            "expression syntax '" +
            o + "'");
      }
    };
    auto interface_filter = [this](const EntryPoint& e, const std::string& i) {
      using tfel::system::ExternalLibraryManager;
      auto& elm = ExternalLibraryManager::getExternalLibraryManager();
      std::regex r(i, this->rs | std::regex_constants::icase);
      auto b = false;
      try {
        b = std::regex_match(elm.getInterface(e.library, e.name), r);
      } catch (std::exception& ex) {
        if (vlevel >= VERBOSE_DEBUG) {
          std::clog << "Error while retrieving interface for '" + e.name + "'"
                    << " in library '" + e.library + "' (" << ex.what()
                    << ")\n";
        }
      }
      return b;
    };
    auto material_filter = [this](const EntryPoint& e, const std::string& m) {
      using tfel::system::ExternalLibraryManager;
      auto& elm = ExternalLibraryManager::getExternalLibraryManager();
      std::regex r(m, this->rs | std::regex_constants::icase);
      auto b = false;
      try {
        b = std::regex_match(elm.getMaterial(e.library, e.name), r);
      } catch (std::exception& ex) {
        if (vlevel >= VERBOSE_DEBUG) {
          std::clog << "Error while retrieving material for '" + e.name + "'"
                    << " in library '" + e.library + "'(" << ex.what() << ")\n";
        }
      }
      return b;
    };
    auto name_filter = [this](const EntryPoint& e, const std::string& n) {
      std::regex r(n, this->rs | std::regex_constants::icase);
      return std::regex_match(e.name, r);
    };
    auto type_filter = [](const EntryPoint& e, const std::string& t) {
      using tfel::system::ExternalLibraryManager;
      if (t == "material_property") {
        return e.type == EntryPoint::MATERIALPROPERTY;
      } else if (t == "behaviour") {
        return e.type == EntryPoint::BEHAVIOUR;
      } else if (t == "model") {
        return e.type == EntryPoint::MODEL;
      } else {
        tfel::raise("mfm: invalid type '" + t + "'");
      }
    };
    this->registerCallBack("--regex-syntax",
                           CallBack("select the syntax used by "
                                    "regular expressions. "
                                    "Valid values are: `ECMAScript`, `basic`, "
                                    "`extended`, `awk`, `grep`, `egrep`",
                                    rsyntax, true));
    this->add_filter("--filter-by-interface",
                     "filter entry points by interface", interface_filter);
    this->add_filter("--filter-by-material", "filter entry points by material",
                     material_filter);
    this->add_filter("--filter-by-name", "filter entry points by name",
                     name_filter);
    this->add_filter(
        "--filter-by-type",
        "filter entry points by material knowledge type. Valid types are "
        "material-property, behaviour or model",
        type_filter);
    this->registerCallBack(
        "--verbose", CallBack("set verbose output", set_verbose_level, true));
    this->registerCallBack("--show-libs",
                           CallBack(
                               "show library name in front "
                               "of entry points",
                               [this] { this->show_libs = true; }, false));
    this->registerCallBack("--show-sources",
                           CallBack(
                               "show the name of the MFront "
                               "file used to generate the entry points",
                               [this] { this->show_sources = true; }, false));
  }  // end of registerCommandLineCallBacks

  const tfel::utilities::Argument& getCurrentCommandLineArgument() const {
    return *(this->currentArgument);
  }

  void treatUnknownArgument() override {
    using namespace tfel::system;
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "mfm: " + m);
    };
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    const auto& a = this->getCurrentCommandLineArgument();
    //    const auto& l = static_cast<const std::string&>(a);
    const auto& l = a.as_string();
    if (l.empty()) {
      return;
    }
    throw_if(l[0] == '-', "unsupported option '" + l + "'");
    if (vlevel >= VERBOSE_LEVEL1) {
      std::clog << "Treating library '" << l << "'\n";
    }
    try {
      const auto enames = elm.getEntryPoints(l);
      for (const auto& en : enames) {
        EntryPoint ep;
        ep.library = l;
        ep.name = en;
        const auto et = elm.getMaterialKnowledgeType(l, en);
        if (et == 0u) {
          ep.type = EntryPoint::MATERIALPROPERTY;
        } else if (et == 1u) {
          ep.type = EntryPoint::BEHAVIOUR;
        } else {
          throw_if(et != 2u,
                   "internal error "
                   "(invalid material knowledge type)");
          ep.type = EntryPoint::MODEL;
        }
        if (vlevel >= VERBOSE_DEBUG) {
          std::clog << "Adding '" + en + "' from '" + l + "'\n";
        }
        this->epts.push_back(std::move(ep));
      }
    } catch (std::exception& e) {
      if (vlevel >= VERBOSE_DEBUG) {
        std::clog << "Error while treating library '" << l << ": " << e.what()
                  << "'\n";
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

  std::vector<Filter> filters;

  std::vector<EntryPoint> epts;

  std::regex_constants::syntax_option_type rs =
      std::regex_constants::ECMAScript;

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
