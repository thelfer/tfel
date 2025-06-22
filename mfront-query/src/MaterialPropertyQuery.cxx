/*!
 * \file   MaterialPropertyQuery.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08 juin 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
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
      : QueryHandlerBase(argc, argv), dsl(d), file(f) {
    this->registerCommandLineCallBacks();
    this->parseArguments();
    this->finalizeArgumentsParsing();
    // registring interfaces
    if (!this->interfaces.empty()) {
      this->dsl->setInterfaces(this->interfaces);
    }
  }  // end of MaterialPropertyQuery

  std::shared_ptr<const AbstractDSL> MaterialPropertyQuery::getDSL() const {
    return this->dsl;
  }  // end of getDSL

  void MaterialPropertyQuery::registerCommandLineCallBacks() {
    QueryHandlerBase::registerCommandLineCallBacks();
    // standard queries
    const std::vector<std::pair<const char*, const char*>> sq = {
        {"--law-name", "show the law name"},
        {"--class-name", "show the class name"},
        {"--author", "show the author name"},
        {"--description", "show the file description"},
        {"--date", "show the file implementation date"},
        {"--material", "show the material name"},
        {"--library", "show the library name"},
        {"--parameters", "show the list of parameters"}};
    for (const auto& q : sq) {
      this->registerCallBack(
          q.first,
          CallBack(
              q.second, [this] { this->treatStandardQuery(); }, false));
    }
    this->registerCallBack(
        "--parameter-default-value",
        CallBack(
            "display the default value of a parameter",
            [this] { this->treatParameterDefaultValue(); }, true));
  }  // end of registerCommandLineCallBacks

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
  }  // end of treatParameterDefaultValue

  void MaterialPropertyQuery::treatStandardQuery() {
    using namespace std;
    const auto& q = this->getCurrentCommandLineArgument();
    const auto& qn = q.as_string();
    if (qn == "--law-name") {
      this->queries.push_back(
          {"law-name",
           [](const FileDescription&, const MaterialPropertyDescription& mpd) {
             cout << mpd.law << endl;
           }});
    } else if (qn == "--class-name") {
      this->queries.push_back(
          {"class-name",
           [](const FileDescription&, const MaterialPropertyDescription& mpd) {
             cout << mpd.className << endl;
           }});
    } else if (qn == "--author") {
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
  }  // end of treatStandardQuery

  void MaterialPropertyQuery::treatGeneratedSources() {
    auto q = this->generateGeneratedSourcesQuery(
        this->getCurrentCommandLineArgument().getOption());
    this->queries.push_back({"generated-sources",
                             [q](const FileDescription&,
                                 const MaterialPropertyDescription&) { q(); }});
  }  // end of treatGeneratedSources

  void MaterialPropertyQuery::treatSpecificTargetGeneratedSources() {
    auto q = this->generateSpecificTargetGeneratedSourcesQuery(
        this->getCurrentCommandLineArgument().getOption());
    this->queries.push_back({"specific-target-generated-sources",
                             [q](const FileDescription&,
                                 const MaterialPropertyDescription&) { q(); }});
  }  // end of treatSpecificTargetGeneratedSources

  void MaterialPropertyQuery::treatAllSpecificTargetsGeneratedSources() {
    auto q = this->generateAllSpecificTargetsGeneratedSourcesQuery();
    this->queries.push_back({"all-specific-targets-generated-sources",
                             [q](const FileDescription&,
                                 const MaterialPropertyDescription&) { q(); }});
  }  // end of treatAllSpecificTargetsGeneratedSources

  void MaterialPropertyQuery::treatGeneratedHeaders() {
    auto q = this->generateGeneratedHeadersQuery();
    this->queries.push_back({"generated-headers",
                             [q](const FileDescription&,
                                 const MaterialPropertyDescription&) { q(); }});
  }  // end of treatGeneratedHeaders

  void MaterialPropertyQuery::treatCppFlags() {
    auto q = this->generateCppFlagsQuery();
    this->queries.push_back(
        {"cppflags", [q](const FileDescription&,
                         const MaterialPropertyDescription&) { q(); }});
  }  // end of treatCppFlags

  void MaterialPropertyQuery::treatLibrariesDependencies() {
    auto q = this->generateLibrariesDependenciesQuery();
    this->queries.push_back({"libraries-dependencies",
                             [q](const FileDescription&,
                                 const MaterialPropertyDescription&) { q(); }});
  }  // end of treatLibrariesDependencies

  void MaterialPropertyQuery::treatSpecificTargets() {
    auto q = this->generateSpecificTargetsQuery();
    this->queries.push_back(
        {"specific-targets", [q](const FileDescription&,
                                 const MaterialPropertyDescription&) { q(); }});
  }  // end of treatSpecificTargets

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
  }  // end of exe

  void MaterialPropertyQuery::treatDSLTarget() {
    std::cout << "material property" << std::endl;
  }  // end of treatDSLTarget

  MaterialPropertyQuery::~MaterialPropertyQuery() = default;

}  // end of namespace mfront
