/*!
 * \file    mfront-query/src/QueryHandlerBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   22/11/2021
 */

#include <iterator>
#include <iostream>
#include "TFEL/Raise.hxx"
#include "MFront/AbstractDSL.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/QueryHandlerBase.hxx"

namespace mfront {

  QueryHandlerBase::QueryHandlerBase() = default;

  std::function<void()> QueryHandlerBase::generateGeneratedSourcesQuery(
      const std::string& o) const {
    if (!((o.empty()) || (o == "sorted-by-libraries") || (o == "unsorted"))) {
      tfel::raise("invalid option '" + o +
                  "' for command line argument '--generated-sources'. "
                  "Valid options are 'sorted-by-libraries' and 'unsorted'");
    }
    return [ldsl = this->getDSL(), o]() {
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
    return [ldsl = this->getDSL()]() {
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