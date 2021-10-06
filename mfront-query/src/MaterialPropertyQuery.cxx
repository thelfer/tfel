/*!
 * \file   MaterialPropertyQuery.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08 juin 2016
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>
#include <iterator>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/MaterialPropertyDSL.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/MaterialPropertyQuery.hxx"

namespace mfront {

  static void display_variable(const mfront::VariableDescription& v) {
    const auto& n = v.getExternalName();
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
  }  // end of display_variable
  MaterialPropertyQuery::MaterialPropertyQuery(
      const int argc,
      const char* const* const argv,
      std::shared_ptr<MaterialPropertyDSL> d,
      const std::string& f)
      : tfel::utilities::ArgumentParserBase<MaterialPropertyQuery>(argc, argv),
        dsl(d),
        file(f) {
    this->registerCommandLineCallBacks();
    this->parseArguments();
    this->finalizeArgumentsParsing();
    // registring interfaces
    if (!this->interfaces.empty()) {
      dsl->setInterfaces(this->interfaces);
    }
  }  // end of MaterialPropertyQuery::MaterialPropertyQuery

  void MaterialPropertyQuery::registerCommandLineCallBacks() {
    using Parser = tfel::utilities::ArgumentParserBase<MaterialPropertyQuery>;
    Parser::registerNewCallBack("--verbose",
                                &MaterialPropertyQuery::treatVerbose,
                                "set verbose output", true);
    Parser::registerNewCallBack("--unicode-output",
                                &MaterialPropertyQuery::treatUnicodeOutput,
                                "allow/disallow unicode output", true);
    Parser::registerNewCallBack(
        "--include", "-I", &MaterialPropertyQuery::treatSearchPath,
        "add a new path at the beginning of the search paths", true);
    Parser::registerNewCallBack(
        "--search-path", &MaterialPropertyQuery::treatSearchPath,
        "add a new path at the beginning of the search paths", true);
    Parser::registerNewCallBack("--install-path",
                                &MaterialPropertyQuery::treatInstallPath,
                                "set the installation directory", true);
    Parser::registerNewCallBack("--install-prefix",
                                &MaterialPropertyQuery::treatInstallPath,
                                "set the installation directory "
                                "(same as --install-path)",
                                true);
    Parser::registerNewCallBack("--debug", &MaterialPropertyQuery::treatDebug,
                                "set debug mode");
    Parser::registerNewCallBack("--warning", "-W",
                                &MaterialPropertyQuery::treatWarning,
                                "print warnings");
    Parser::registerNewCallBack("--pedantic",
                                &MaterialPropertyQuery::treatPedantic,
                                "print pedantic warning message");
    Parser::registerNewCallBack("--interface",
                                &MaterialPropertyQuery::treatInterface,
                                "define an interface", true);
    Parser::registerCallBack("--no-gui",
                             CallBack("do not display errors using "
                                      "a message box (windows only)",
                                      [] {}, false));
#ifdef MFRONT_HAVE_MADNEX
    Parser::registerNewCallBack("--material",
                                &MaterialPropertyQuery::treatMaterialIdentifier,
                                "specify a material identifier", true);
    Parser::registerNewCallBack(
        "--material-property",
        &MaterialPropertyQuery::treatMaterialPropertyIdentifier,
        "specify a material property identifier (can be a regular expression)",
        true);
    Parser::registerNewCallBack(
        "--behaviour", &MaterialPropertyQuery::treatBehaviourIdentifier,
        "specify a behaviour identifier (can be a regular expression)", true);
    Parser::registerNewCallBack(
        "--model", &MaterialPropertyQuery::treatModelIdentifier,
        "specify a model identifier (can be a regular expression)", true);
#endif /* MFRONT_HAVE_MADNEX */
    // standard queries
    const std::vector<std::pair<const char*, const char*>> sq = {
        {"--author", "show the author name"},
        {"--description", "show the file description"},
        {"--date", "show the file implementation date"},
        {"--material", "show the material name"},
        {"--library", "show the library name"},
        {"--parameters", "show the list of parameters"}};
    for (const auto& q : sq) {
      Parser::registerNewCallBack(
          q.first, &MaterialPropertyQuery::treatStandardQuery, q.second);
    }
    Parser::registerNewCallBack(
        "--parameter-default-value",
        &MaterialPropertyQuery::treatParameterDefaultValue,
        "display the default value of a parameter", true);
    Parser::registerNewCallBack("--generated-sources",
                                &MaterialPropertyQuery::treatGeneratedSources,
                                "show all the generated sources");
    Parser::registerNewCallBack("--generated-headers",
                                &MaterialPropertyQuery::treatGeneratedHeaders,
                                "show all the generated headers");
    Parser::registerNewCallBack("--cppflags",
                                &MaterialPropertyQuery::treatCppFlags,
                                "show all the global headers");
    Parser::registerNewCallBack(
        "--libraries-dependencies",
        &MaterialPropertyQuery::treatLibrariesDependencies,
        "show all the libraries dependencies");
    Parser::registerNewCallBack("--specific-targets",
                                &MaterialPropertyQuery::treatSpecificTargets,
                                "show all the specific targets");
  }  // end of MaterialPropertyQuery::registerCommandLineCallBacks

  void MaterialPropertyQuery::treatParameterDefaultValue() {
    const auto& q = this->getCurrentCommandLineArgument();
    const auto& qn = q.as_string();
    const auto pn = q.getOption();
    tfel::raise_if(pn.empty(),
                   "Behaviour::treatStandardQuery2 : "
                   "no option given to the '" +
                       qn + "' query");
    this->queries.push_back(
        {"parameter-default-value",
         [pn](const FileDescription&, const MaterialPropertyDescription& mpd) {
           const auto& p = findByExternalName(mpd.parameters, pn);
           if (p == mpd.parameters.end()) {
             tfel::raise_if(!p->hasAttribute(VariableDescription::defaultValue),
                            "no parameter named "
                            "'" +
                                pn + "'");
           }
           tfel::raise_if(!p->hasAttribute(VariableDescription::defaultValue),
                          "no default value for parameter "
                          "'" +
                              p->name + "'");
           const auto pv =
               p->getAttribute<double>(VariableDescription::defaultValue);
           std::cout << pv << '\n';
         }});
  } // end of MaterialPropertyQuery::treatParameterDefaultValue
  void MaterialPropertyQuery::treatStandardQuery() {
    using namespace std;
    const auto& q = this->getCurrentCommandLineArgument();
    const auto& qn = q.as_string();
    if (qn == "--author") {
      this->queries.push_back(
          {"author",
           [](const FileDescription& fd, const MaterialPropertyDescription&) {
             const auto& a = fd.authorName;
             cout << (!a.empty() ? a : "(undefined)") << endl;
           }});
    } else if (qn == "--description") {
      this->queries.push_back(
          {"description",
           [](const FileDescription& fd, const MaterialPropertyDescription&) {
             if (!fd.description.empty()) {
               const auto d = tfel::utilities::tokenize(fd.description, '\n');
               for (const auto& l : d) {
                 if ((l.size() >= 2) && ((l)[0] == '*') && ((l)[1] == ' ')) {
                   cout << l.substr(2) << endl;
                 } else {
                   cout << l << endl;
                 }
               }
             } else {
               cout << "(undefined)" << endl;
             }
           }});
    } else if (qn == "--date") {
      this->queries.push_back({"date", [](const FileDescription& fd,
                                          const MaterialPropertyDescription&) {
                                 const auto& d = fd.date;
                                 cout << (!d.empty() ? d : "(undefined)")
                                      << endl;
                               }});
    } else if (qn == "--material") {
      this->queries.push_back(
          {"material",
           [](const FileDescription&, const MaterialPropertyDescription& d) {
             const auto& m = d.material;
             cout << (!m.empty() ? m : "(undefined)") << endl;
           }});
    } else if (qn == "--library") {
      this->queries.push_back(
          {"library",
           [](const FileDescription&, const MaterialPropertyDescription& d) {
             const auto& l = d.library;
             cout << (!l.empty() ? l : "(undefined)") << endl;
           }});
    } else if (qn == "--parameters") {
      this->queries.push_back(
          {"parameters",
           [](const FileDescription&, const MaterialPropertyDescription& mpd) {
             for (const auto& p : mpd.parameters) {
               display_variable(p);
             }
           }});
    } else {
      tfel::raise(
          "MaterialPropertyQuery::treatStandardQuery: "
          "unsupported query '" +
          qn + "'");
    }
  }  // end of MaterialPropertyQuery::treatStandardQuery

  void MaterialPropertyQuery::treatGeneratedSources() {
    auto ldsl = this->dsl;
    auto q = [ldsl](const FileDescription&,
                    const MaterialPropertyDescription&) {
      for (const auto& l : ldsl->getTargetsDescription().libraries) {
        std::cout << l.name << " : ";  //< library
        std::copy(std::begin(l.sources), std::end(l.sources),
                  std::ostream_iterator<std::string>(std::cout, " "));
        std::cout << std::endl;
      }
    };
    this->queries.push_back({"generated-sources", q});
  }  // end of MaterialPropertyQuery::treatGeneratedSources

  void MaterialPropertyQuery::treatGeneratedHeaders() {
    auto ldsl = this->dsl;
    auto q = [ldsl](const FileDescription&,
                    const MaterialPropertyDescription&) {
      const auto headers = ldsl->getTargetsDescription().headers;
      std::copy(std::begin(headers), std::end(headers),
                std::ostream_iterator<std::string>(std::cout, " "));
      std::cout << std::endl;
    };
    this->queries.push_back({"generated-headers", q});
  }  // end of MaterialPropertyQuery::treatGeneratedHeaders

  void MaterialPropertyQuery::treatCppFlags() {
    auto ldsl = this->dsl;
    auto q = [ldsl](const FileDescription&,
                    const MaterialPropertyDescription&) {
      for (const auto& l : ldsl->getTargetsDescription().libraries) {
        std::cout << l.name << " : ";
        std::copy(std::begin(l.cppflags), std::end(l.cppflags),
                  std::ostream_iterator<std::string>(std::cout, " "));
        std::cout << std::endl;
      }
    };
    this->queries.push_back({"cppflags", q});
  }  // end of MaterialPropertyQuery::treatCppFlags

  void MaterialPropertyQuery::treatLibrariesDependencies() {
    auto ldsl = this->dsl;
    auto q = [ldsl](const FileDescription&,
                    const MaterialPropertyDescription&) {
      for (const auto& l : ldsl->getTargetsDescription().libraries) {
        std::cout << l.name << ": ";
        std::copy(std::begin(l.ldflags), std::end(l.ldflags),
                  std::ostream_iterator<std::string>(std::cout, " "));
        std::cout << std::endl;
      }
    };
    this->queries.push_back({"libraries-dependencies", q});
  }  // end of MaterialPropertyQuery::treatLibrariesDependencies

  void MaterialPropertyQuery::treatSpecificTargets() {
    auto ldsl = this->dsl;
    auto l = [ldsl](const FileDescription&,
                    const MaterialPropertyDescription&) {
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
    this->queries.push_back({"specific-targets", l});
  }  // end of MaterialPropertyQuery::treatSpecificTargets

  void MaterialPropertyQuery::exe() {
    if (getVerboseMode() >= VERBOSE_LEVEL2) {
      getLogStream() << "Treating file '" << this->file << "'" << std::endl;
    }
    // analysing the file
    this->dsl->analyseFile(this->file, this->ecmds, this->substitutions);
    const auto& fd = this->dsl->getFileDescription();
    const auto& d = this->dsl->getMaterialPropertyDescription();
    // treating the queries
    for (const auto& q : queries) {
      if (getVerboseMode() >= VERBOSE_LEVEL2) {
        getLogStream() << "Treating query '" << q.first << "'\n";
      }
      q.second(fd, d);
    }
  }  // end of MaterialPropertyQuery::exe

  const tfel::utilities::Argument&
  MaterialPropertyQuery::getCurrentCommandLineArgument() const {
    return *(this->currentArgument);
  }

  void MaterialPropertyQuery::treatUnknownArgument() {
    if (!MFrontBase::treatUnknownArgumentBase()) {
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
      ArgumentParserBase<MaterialPropertyQuery>::treatUnknownArgument();
#else
      const auto& a = static_cast<const std::string&>(
          this->getCurrentCommandLineArgument());
      std::cerr << "mfront : unsupported option '" << a << "'\n";
      ::exit(EXIT_FAILURE);
#endif /* __CYGWIN__ */
    }
  }

  std::string MaterialPropertyQuery::getVersionDescription() const {
    return MFrontHeader::getHeader();
  }

  std::string MaterialPropertyQuery::getUsageDescription() const {
    auto usage = std::string("Usage: ");
    usage += this->programName;
    usage += " [options] [files]";
    return usage;
  }

  MaterialPropertyQuery::~MaterialPropertyQuery() = default;

}  // end of namespace mfront
