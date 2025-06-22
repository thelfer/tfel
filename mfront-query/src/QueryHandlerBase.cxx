/*!
 * \file    mfront-query/src/QueryHandlerBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   22/11/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iterator>
#include <iostream>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/AbstractDSL.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/QueryHandlerBase.hxx"

namespace mfront {

  void QueryHandlerBase::displayVariable(const mfront::VariableDescription& v,
                                         const std::string& spaces) {
    const auto& n = v.getExternalName();
    std::cout << spaces;
    if (n == v.name) {
      std::cout << "- " << displayName(v);
    } else {
      std::cout << "- " << n;
    }
    if (v.arraySize != 1u) {
      std::cout << '[' << v.arraySize << ']';
    }
    if (n != v.name) {
      std::cout << " (" << mfront::displayName(v) << ")";
    }
    if (!v.description.empty()) {
      std::cout << ": " << v.description;
    } else {
      const auto& glossary = tfel::glossary::Glossary::getGlossary();
      if (glossary.contains(n)) {
        std::cout << ": " << glossary.getGlossaryEntry(n).getShortDescription();
      }
    }
    std::cout << '\n';
  }  // end of displayVariable

  QueryHandlerBase::QueryHandlerBase(const int argc,
                                     const char* const* const argv)
      : tfel::utilities::ArgumentParserBase<QueryHandlerBase>(argc, argv) {
  }  // end of QueryHandlerBase

  const tfel::utilities::Argument&
  QueryHandlerBase::getCurrentCommandLineArgument() const {
    return *(this->currentArgument);
  }

  void QueryHandlerBase::treatUnknownArgument() {
    if (!MFrontBase::treatUnknownArgumentBase()) {
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
      ArgumentParserBase<QueryHandlerBase>::treatUnknownArgument();
#else
      const auto& a = static_cast<const std::string&>(
          this->getCurrentCommandLineArgument());
      std::cerr << "mfront-query : unsupported option '" << a << "'\n";
      ::exit(EXIT_FAILURE);
#endif /* __CYGWIN__ */
    }
  }

  std::string QueryHandlerBase::getVersionDescription() const {
    return MFrontHeader::getHeader();
  }

  std::string QueryHandlerBase::getUsageDescription() const {
    auto usage = std::string("Usage: ");
    usage += this->programName;
    usage += " [options] [files]";
    return usage;
  }

  void QueryHandlerBase::registerCommandLineCallBacks() {
    this->registerNewCallBack("--verbose", &QueryHandlerBase::treatVerbose,
                              "set verbose output", true);
    this->registerNewCallBack("--unicode-output",
                              &QueryHandlerBase::treatUnicodeOutput,
                              "allow/disallow unicode output", true);
    this->registerNewCallBack(
        "--dsl-target", &QueryHandlerBase::treatDSLTarget,
        "get the kind of material knowledge treated by the dsl (material "
        "property, behaviour, or model)",
        false);
    this->registerCallBack(
        "--dsl-option",
        CallBack(
            "allow to define options passed to domain specific languages",
            [] {}, true));
    this->registerCallBack(
        "--dsl-material-property-option",
        CallBack(
            "allow to define options passed to domain specific languages "
            "related to material properties",
            [] {}, true));
    this->registerCallBack(
        "--dsl-behaviour-option",
        CallBack(
            "allow to define options passed to domain specific languages "
            "related to behaviours",
            [] {}, true));
    this->registerCallBack(
        "--dsl-model-option",
        CallBack(
            "allow to define options passed to domain specific languages "
            "related to models",
            [] {}, true));
    this->registerCallBack(
        "--dsl-options-file",
        CallBack(
            "allow to define options passed to domain specific languages  "
            "thanks to an external file in a JSON-like format",
            [] {}, true));
    this->registerCallBack(
        "--dsl-material-property-options-file",
        CallBack(
            "allow to define options passed to domain specific languages "
            "related to material properties thanks to an external file in "
            "a JSON-like format",
            [] {}, true));
    this->registerCallBack(
        "--dsl-behaviour-options-file",
        CallBack(
            "allow to define options passed to domain specific languages "
            "related to behaviours thanks to an external file in a "
            "JSON-like format",
            [] {}, true));
    this->registerCallBack(
        "--dsl-model-options-file",
        CallBack(
            "allow to define options passed to domain specific languages "
            "related to models thanks to an external file in a JSON-like "
            "format",
            [] {}, true));
    this->registerNewCallBack(
        "--include", "-I", &QueryHandlerBase::treatSearchPath,
        "add a new path at the beginning of the search paths", true);
    this->registerNewCallBack(
        "--search-path", &QueryHandlerBase::treatSearchPath,
        "add a new path at the beginning of the search paths", true);
    this->registerNewCallBack("--madnex-search-path",
                              &QueryHandlerBase::treatMadnexSearchPath,
                              "add a mandex file to the search paths", true);
    this->registerNewCallBack("--install-path",
                              &QueryHandlerBase::treatInstallPath,
                              "set the installation directory", true);
    this->registerNewCallBack("--install-prefix",
                              &QueryHandlerBase::treatInstallPath,
                              "set the installation directory "
                              "(same as --install-path)",
                              true);
    this->registerCallBack("--nomelt",
                           CallBack(
                               "don't melt librairies sources",
                               [this] { this->melt_sources = false; }, false));
    this->registerNewCallBack(
        "--warning", "-W", &QueryHandlerBase::treatWarning, "print warnings");
    this->registerNewCallBack("--pedantic", &QueryHandlerBase::treatPedantic,
                              "print pedantic warning message");
    this->registerNewCallBack("--interface", &QueryHandlerBase::treatInterface,
                              "define an interface", true);
#ifdef MFRONT_HAVE_MADNEX
    this->registerNewCallBack("--material",
                              &QueryHandlerBase::treatMaterialIdentifier,
                              "specify a material identifier", true);
    this->registerNewCallBack(
        "--material-property",
        &QueryHandlerBase::treatMaterialPropertyIdentifier,
        "specify a material property identifier (can be a regular expression)",
        true);
    this->registerNewCallBack(
        "--behaviour", &QueryHandlerBase::treatBehaviourIdentifier,
        "specify a behaviour identifier (can be a regular expression)", true);
    this->registerNewCallBack(
        "--model", &QueryHandlerBase::treatModelIdentifier,
        "specify a model identifier (can be a regular expression)", true);
#endif /* MFRONT_HAVE_MADNEX */
    this->registerCallBack(
        "--generated-sources",
        CallBack(
            "show all the generated sources",
            [this] { this->treatGeneratedSources(); }, true));
    this->registerCallBack(
        "--specific-target-generated-sources",
        CallBack(
            "show all the generated sources associated with as specific target",
            [this] { this->treatSpecificTargetGeneratedSources(); }, true));
    this->registerCallBack(
        "--all-specific-targets-generated-sources",
        CallBack(
            "show all the generated sources associated specific targets",
            [this] { this->treatAllSpecificTargetsGeneratedSources(); },
            false));
    this->registerCallBack(
        "--generated-headers",
        CallBack(
            "show all the generated headers",
            [this] { this->treatGeneratedHeaders(); }, false));
    this->registerCallBack("--cppflags",
                           CallBack(
                               "show all the global headers",
                               [this] { this->treatCppFlags(); }, false));
    this->registerCallBack(
        "--libraries-dependencies",
        CallBack(
            "show all the libraries dependencies",
            [this] { this->treatLibrariesDependencies(); }, false));
    this->registerCallBack(
        "--specific-targets",
        CallBack(
            "show all the specific targets",
            [this] { this->treatSpecificTargets(); }, false));
    this->registerCallBack("--no-gui", CallBack(
                                           "do not display errors using "
                                           "a message box (windows only)",
                                           [] {}, false));
  }

  std::function<void()> QueryHandlerBase::generateGeneratedSourcesQuery(
      const std::string& o) const {
    if (!((o.empty()) || (o == "sorted-by-libraries") || (o == "unsorted"))) {
      tfel::raise("invalid option '" + o +
                  "' for command line argument '--generated-sources'. "
                  "Valid options are 'sorted-by-libraries' and 'unsorted'");
    }
    return [lmelt_sources = this->melt_sources, ldsl = this->getDSL(), o] {
      const auto& libraries = ldsl->getTargetsDescription().libraries;
      auto write_sources = [lmelt_sources,
                            &libraries](const LibraryDescription& l) {
        std::copy(l.sources.begin(), l.sources.end(),
                  std::ostream_iterator<std::string>(std::cout, " "));
        if (lmelt_sources) {
          for (const auto& d : l.deps) {
            const auto pd = std::find_if(
                libraries.begin(), libraries.end(),
                [&d](const LibraryDescription& lib) { return lib.name == d; });
            if (pd != libraries.end()) {
              std::copy(pd->sources.begin(), pd->sources.end(),
                        std::ostream_iterator<std::string>(std::cout, " "));
            }
          }
        }
      };
      if ((o.empty()) || (o == "sorted-by-libraries")) {
        for (const auto& l : libraries) {
          if (l.name == "MFrontMaterialLaw") {
            // The MFrontMaterialLaw is used internally and shall be visible to
            // the end user
            continue;
          }
          std::cout << l.name << " : ";  //< library
          write_sources(l);
          std::cout << '\n';
        }
      } else {
        for (const auto& l : ldsl->getTargetsDescription().libraries) {
          if (l.name == "MFrontMaterialLaw") {
            continue;
          }
          write_sources(l);
        }
        std::cout << '\n';
      }
    };
  }  // end of generateGeneratedSourcesQuery

  static std::vector<std::string> getSpecificTargetSources(
      const TargetsDescription& td,
      const SpecificTargetDescription& t,
      const bool melt_sources) {
    auto sources = std::vector<std::string>{};
    std::copy(t.sources.begin(), t.sources.end(), std::back_inserter(sources));
    if (melt_sources) {
      const auto& libraries = td.libraries;
      for (const auto& d : t.libraries) {
        const auto pd = std::find_if(
            libraries.begin(), libraries.end(),
            [&d](const LibraryDescription& lib) { return lib.name == d; });
        if (pd != libraries.end()) {
          std::copy(pd->sources.begin(), pd->sources.end(),
                    std::back_inserter(sources));
        }
      }
    }
    return sources;
  }

  std::function<void()>
  QueryHandlerBase::generateSpecificTargetGeneratedSourcesQuery(
      const std::string& n) const {
    if (!n.empty()) {
      tfel::raise(
          "QueryHandlerBase::generateSpecificTargetGeneratedSourcesQuery: "
          "no option given for command line argument "
          "'--specific-target-generated-sources'. "
          "Expected specific target name");
    }
    return [lmelt_sources = this->melt_sources, ldsl = this->getDSL(), n] {
      const auto& targets = ldsl->getTargetsDescription().specific_targets;
      const auto p = targets.find(n);
      if (p == targets.end()) {
        tfel::raise(
            "QueryHandlerBase::generateSpecificTargetGeneratedSourcesQuery: "
            "no specific target named '" +
            n + "'");
      }
      const auto sources = getSpecificTargetSources(
          ldsl->getTargetsDescription(), p->second, lmelt_sources);
      std::copy(sources.begin(), sources.end(),
                std::ostream_iterator<std::string>(std::cout, " "));
      std::cout << '\n';
    };
  }  // end of generateSpecificTargetGeneratedSourcesQuery

  std::function<void()>
  QueryHandlerBase::generateAllSpecificTargetsGeneratedSourcesQuery() const {
    return [lmelt_sources = this->melt_sources, ldsl = this->getDSL()] {
      const auto& targets = ldsl->getTargetsDescription().specific_targets;
      for (const auto& t : targets) {
        if ((t.first == "all") || (t.first == "clean")) {
          continue;
        }
        const auto sources = getSpecificTargetSources(
            ldsl->getTargetsDescription(), t.second, lmelt_sources);
        std::copy(sources.begin(), sources.end(),
                  std::ostream_iterator<std::string>(std::cout, " "));
      }
      std::cout << '\n';
    };
  }  // end of generateAllSpecificTargetsGeneratedSourcesQuery

  std::function<void()> QueryHandlerBase::generateSpecificTargetsQuery() const {
    return [ldsl = this->getDSL()] {
      const auto targets = ldsl->getTargetsDescription().specific_targets;
      for (const auto& t : targets) {
        std::cout << t.first << " : ";
        std::copy(std::begin(t.second.deps), std::end(t.second.deps),
                  std::ostream_iterator<std::string>(std::cout, " "));
        std::cout << std::endl << "> rule : ";
        std::copy(std::begin(t.second.cmds), std::end(t.second.cmds),
                  std::ostream_iterator<std::string>(std::cout, "\n> rule : "));
        std::cout << std::endl;
      }
    };
  }  // end of generateSpecificTargetsQuery

  std::function<void()> QueryHandlerBase::generateLibrariesDependenciesQuery()
      const {
    return [ldsl = this->getDSL()] {
      for (const auto& l : ldsl->getTargetsDescription().libraries) {
        if (l.name == "MFrontMaterialLaw") {
          continue;
        }
        std::cout << l.name << " : ";
        std::copy(l.ldflags.begin(), l.ldflags.end(),
                  std::ostream_iterator<std::string>(std::cout, " "));
        std::cout << '\n';
      }
    };
  }  // end of generateLibrariesDependenciesQuery

  std::function<void()> QueryHandlerBase::generateCppFlagsQuery() const {
    return [ldsl = this->getDSL()] {
      for (const auto& l : ldsl->getTargetsDescription().libraries) {
        if (l.name == "MFrontMaterialLaw") {
          continue;
        }
        std::cout << l.name << " : ";
        std::copy(l.cppflags.begin(), l.cppflags.end(),
                  std::ostream_iterator<std::string>(std::cout, " "));
        std::cout << '\n';
      }
    };
  }  // end of generateCppFlagsQuery

  std::function<void()> QueryHandlerBase::generateGeneratedHeadersQuery()
      const {
    return [ldsl = this->getDSL()] {
      const auto headers = ldsl->getTargetsDescription().headers;
      std::copy(headers.begin(), headers.end(),
                std::ostream_iterator<std::string>(std::cout, " "));
      std::cout << '\n';
    };
  }  // end of generateGeneratedHeadersQuery

  QueryHandlerBase::~QueryHandlerBase() = default;

}  // end of namespace mfront
