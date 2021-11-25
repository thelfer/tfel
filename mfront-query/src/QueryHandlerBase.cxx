/*!
 * \file    mfront-query/src/QueryHandlerBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   22/11/2021
 */

#include <iterator>
#include <iostream>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/AbstractDSL.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/QueryHandlerBase.hxx"

namespace mfront {

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
        "--include", "-I", &QueryHandlerBase::treatSearchPath,
        "add a new path at the beginning of the search paths", true);
    this->registerNewCallBack(
        "--search-path", &QueryHandlerBase::treatSearchPath,
        "add a new path at the beginning of the search paths", true);
    this->registerNewCallBack("--install-path", &QueryHandlerBase::treatInstallPath,
                                "set the installation directory", true);
    this->registerNewCallBack("--install-prefix",
                                &QueryHandlerBase::treatInstallPath,
                                "set the installation directory "
                                "(same as --install-path)",
                                true);
    this->registerCallBack(
        "--nomelt", CallBack("don't melt librairies sources",
                             [this] { this->melt_sources = false; }, false));
    this->registerNewCallBack("--warning", "-W", &QueryHandlerBase::treatWarning,
                                "print warnings");
    this->registerNewCallBack("--pedantic", &QueryHandlerBase::treatPedantic,
                                "print pedantic warning message");
    this->registerNewCallBack("--interface", &QueryHandlerBase::treatInterface,
                                "define an interface", true);
#ifdef MFRONT_HAVE_MADNEX
    this->registerNewCallBack("--material",
                                &QueryHandlerBase::treatMaterialIdentifier,
                                "specify a material identifier", true);
    this->registerNewCallBack(
        "--material-property", &QueryHandlerBase::treatMaterialPropertyIdentifier,
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
        CallBack("show all the generated sources",
                 [this] {
      this->treatGeneratedSources(); }, true));
    this->registerCallBack(
        "--generated-headers",
        CallBack("show all the generated headers",
                 [this] {
      this->treatGeneratedHeaders(); }, false));
    this->registerCallBack("--cppflags",
                           CallBack("show all the global headers",
                                    [this] {
      this->treatCppFlags(); }, false));
    this->registerCallBack(
        "--libraries-dependencies",
        CallBack("show all the libraries dependencies",
                 [this] {
      this->treatLibrariesDependencies(); }, false));
    this->registerCallBack(
        "--specific-targets",
        CallBack("show all the specific targets",
                 [this] {
      this->treatSpecificTargets(); }, false));
    this->registerCallBack("--no-gui", CallBack("do not display errors using "
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
    auto ldsl = this->getDSL();
    return [ldsl, o] {
      const auto& libraries = ldsl->getTargetsDescription().libraries;
      auto write_sources = [&libraries](const LibraryDescription& l) {
        std::copy(l.sources.begin(), l.sources.end(),
                  std::ostream_iterator<std::string>(std::cout, " "));
        for (const auto d : l.deps) {
          const auto pd = std::find_if(
              libraries.begin(), libraries.end(),
              [&d](const LibraryDescription& lib) { return lib.name == d; });
          if (pd != libraries.end()) {
            std::copy(pd->sources.begin(), pd->sources.end(),
                      std::ostream_iterator<std::string>(std::cout, " "));
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

  std::function<void()> QueryHandlerBase::generateSpecificTargetsQuery() const {
    auto ldsl = this->getDSL();
    return [ldsl] {
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
    auto ldsl = this->getDSL();
    return [ldsl] {
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
    auto ldsl = this->getDSL();
    return [ldsl] {
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
    auto ldsl = this->getDSL();
    return [ldsl] {
      const auto headers = ldsl->getTargetsDescription().headers;
      std::copy(headers.begin(), headers.end(),
                std::ostream_iterator<std::string>(std::cout, " "));
      std::cout << '\n';
    };
  }  // end of generateGeneratedHeadersQuery

  QueryHandlerBase::~QueryHandlerBase() = default;

}  // end of namespace mfront
