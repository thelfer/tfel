/*!
 * \file   ModelQuery.cxx
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

#include <iterator>
#include <iostream>
#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "MFront/ModelDSL.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/ModelQuery.hxx"

namespace mfront {

  ModelQuery::ModelQuery(const int argc,
                         const char* const* const argv,
                         std::shared_ptr<ModelDSL> d,
                         const std::string& f)
      : QueryHandlerBase(argc, argv), dsl(d), file(f) {
    this->registerCommandLineCallBacks();
    this->parseArguments();
    this->finalizeArgumentsParsing();
    // registring interfaces
    if (!this->interfaces.empty()) {
      this->dsl->setInterfaces(this->interfaces);
    }
  }  // end of ModelQuery

  std::shared_ptr<const AbstractDSL> ModelQuery::getDSL() const {
    return this->dsl;
  }  // end of getDSL

  void ModelQuery::registerCommandLineCallBacks() {
    QueryHandlerBase::registerCommandLineCallBacks();
    // standard queries
    const std::vector<std::pair<const char*, const char*>> sq = {
        {"--model-name", "show the model name"},
        {"--class-name", "show the class name"},
        {"--author", "show the author name"},
        {"--description", "show the file description"},
        {"--date", "show the file implementation date"},
        {"--material", "show the material name"},
        {"--library", "show the library name"},
        {"--outputs", "show model outputs"}};
    for (const auto& q : sq) {
      this->registerCallBack(
          q.first,
          CallBack(
              q.second, [this] { this->treatStandardQuery(); }, false));
    }
  }  // end of registerCommandLineCallBacks

  void ModelQuery::treatStandardQuery() {
    const auto& qn = this->getCurrentCommandLineArgument().as_string();
    if (qn == "--model-name") {
      this->queries.push_back({"model-name", [](const FileDescription&,
                                                const ModelDescription& mpd) {
                                 std::cout << mpd.modelName << std::endl;
                               }});
    } else if (qn == "--class-name") {
      this->queries.push_back({"class-name", [](const FileDescription&,
                                                const ModelDescription& mpd) {
                                 std::cout << mpd.className << std::endl;
                               }});
    } else if (qn == "--author") {
      this->queries.push_back(
          {"author", [](const FileDescription& fd, const ModelDescription&) {
             const auto& a = fd.authorName;
             std::cout << (!a.empty() ? a : "(undefined)") << std::endl;
           }});
    } else if (qn == "--description") {
      this->queries.push_back(
          {"description",
           [](const FileDescription& fd, const ModelDescription&) {
             if (!fd.description.empty()) {
               const auto d = tfel::utilities::tokenize(fd.description, '\n');
               for (const auto& l : d) {
                 if ((l.size() >= 2) && ((l)[0] == '*') && ((l)[1] == ' ')) {
                   std::cout << l.substr(2) << std::endl;
                 } else {
                   std::cout << l << std::endl;
                 }
               }
             } else {
               std::cout << "(undefined)" << std::endl;
             }
           }});
    } else if (qn == "--date") {
      this->queries.push_back(
          {"date", [](const FileDescription& fd, const ModelDescription&) {
             const auto& d = fd.date;
             std::cout << (!d.empty() ? d : "(undefined)") << std::endl;
           }});
    } else if (qn == "--material") {
      this->queries.push_back(
          {"material", [](const FileDescription&, const ModelDescription& d) {
             const auto& m = d.material;
             std::cout << (!m.empty() ? m : "(undefined)") << std::endl;
           }});
    } else if (qn == "--library") {
      this->queries.push_back(
          {"library", [](const FileDescription&, const ModelDescription& d) {
             const auto& l = d.library;
             std::cout << (!l.empty() ? l : "(undefined)") << std::endl;
           }});
    } else if (qn == "--outputs") {
      this->queries.push_back(
          {"outputs", [](const FileDescription&, const ModelDescription& d) {
             for (const auto& v : d.outputs) {
               const auto& n = v.name;  // d.getExternalName(v.name);
               std::cout << "- " << n;
               if (v.arraySize != 1u) {
                 std::cout << '[' << v.arraySize << ']';
               }
               if (n != v.name) {
                 std::cout << " (" << displayName(v) << ")";
               }
               if (!v.description.empty()) {
                 std::cout << ": " << v.description;
               } else {
                 const auto& g = tfel::glossary::Glossary::getGlossary();
                 if (g.contains(n)) {
                   std::cout << ": "
                             << g.getGlossaryEntry(n).getShortDescription();
                 }
               }
               std::cout << std::endl;
             }
           }});
    } else {
      tfel::raise(
          "Model::treatStandardQuery : "
          "unsupported query '" +
          qn + "'");
    }
  }  // end of treatStandardQuery

  void ModelQuery::treatGeneratedSources() {
    auto q = this->generateGeneratedSourcesQuery(
        this->getCurrentCommandLineArgument().getOption());
    this->queries.push_back(
        {"generated-sources",
         [q](const FileDescription&, const ModelDescription&) { q(); }});
  }  // end of treatGeneratedSources

  void ModelQuery::treatSpecificTargetGeneratedSources() {
    auto q = this->generateSpecificTargetGeneratedSourcesQuery(
        this->getCurrentCommandLineArgument().getOption());
    this->queries.push_back(
        {"specific-target-generated-sources",
         [q](const FileDescription&, const ModelDescription&) { q(); }});
  }  // end of treatSpecificTargetGeneratedSources

  void ModelQuery::treatAllSpecificTargetsGeneratedSources() {
    auto q = this->generateAllSpecificTargetsGeneratedSourcesQuery();
    this->queries.push_back(
        {"all-specific-targets-generated-sources",
         [q](const FileDescription&, const ModelDescription&) { q(); }});
  }  // end of treatAllSpecificTargetsGeneratedSources

  void ModelQuery::treatGeneratedHeaders() {
    auto q = this->generateGeneratedHeadersQuery();
    this->queries.push_back(
        {"generated-headers",
         [q](const FileDescription&, const ModelDescription&) { q(); }});
  }  // end of treatGeneratedHeaders

  void ModelQuery::treatCppFlags() {
    auto q = this->generateCppFlagsQuery();
    this->queries.push_back(
        {"cppflags",
         [q](const FileDescription&, const ModelDescription&) { q(); }});
  }  // end of treatCppFlags

  void ModelQuery::treatLibrariesDependencies() {
    auto q = this->generateLibrariesDependenciesQuery();
    this->queries.push_back(
        {"libraries-dependencies",
         [q](const FileDescription&, const ModelDescription&) { q(); }});
  }  // end of treatLibrariesDependencies

  void ModelQuery::treatSpecificTargets() {
    auto q = this->generateSpecificTargetsQuery();
    this->queries.push_back(
        {"specific-targets",
         [q](const FileDescription&, const ModelDescription&) { q(); }});
  }  // end of treatSpecificTargets

  void ModelQuery::exe() {
    if (getVerboseMode() >= VERBOSE_LEVEL2) {
      getLogStream() << "Treating file '" << this->file << "'" << std::endl;
    }
    // analysing the file
    this->dsl->analyseFile(this->file, this->ecmds, this->substitutions);
    const auto& fd = this->dsl->getFileDescription();
    const auto& d = this->dsl->getModelDescription();
    // treating the queries
    for (const auto& q : queries) {
      if (getVerboseMode() >= VERBOSE_LEVEL2) {
        getLogStream() << "Treating query '" << q.first << "'\n";
      }
      q.second(fd, d);
    }
  }  // end of exe

  void ModelQuery::treatDSLTarget() {
    std::cout << "model" << std::endl;
  }  // end of treatDSLTarget

  ModelQuery::~ModelQuery() = default;

}  // end of namespace mfront
