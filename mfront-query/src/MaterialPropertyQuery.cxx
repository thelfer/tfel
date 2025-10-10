/*!
 * \file   mfront-query/src/MaterialPropertyQuery.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08/06/2016
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
#include "MFront/QueryUtilities.hxx"
#include "MFront/MaterialPropertyQuery.hxx"

namespace mfront {

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
        {"--unit-system", "show the unit system used"},
        {"--description", "show the file description"},
        {"--date", "show the file implementation date"},
        {"--material", "show the material name"},
        {"--library", "show the library name"},
        {"--output", "show the output of the material law"},
        {"--inputs", "show the list of inputs"},
        {"--state-variables",
         "show the list of state variables. Equivalent to the `--inputs` "
         "query"},
        {"--parameters", "show the list of parameters"},
        {"--parameters-file",
         "display the name of a text file which can be used to modify the "
         "default value of the parameters"},
        {"--list-dependencies", "list the MFront dependencies"}};
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
    // bounds
    auto has_bounds = [this] {
      const auto& q = this->getCurrentCommandLineArgument();
      const auto& o = q.getOption();
      auto l = [o](const FileDescription&,
                   const MaterialPropertyDescription& mpd) {
        const auto& v = mpd.getVariableDescriptionByExternalName(o);
        std::cout << (v.hasPhysicalBounds() ? "true" : "false") << '\n';
      };
      this->queries.push_back({"has-bounds", l});
    };
    this->registerCallBack("--has-bounds",
                           CallBack("return `true` if a variable has bounds, "
                                    "`false` otherwise",
                                    has_bounds, true));
    auto bounds_type = [this] {
      const auto& q = this->getCurrentCommandLineArgument();
      const auto& o = q.getOption();
      auto l = [o](const FileDescription&,
                   const MaterialPropertyDescription& mpd) {
        const auto& v = mpd.getVariableDescriptionByExternalName(o);
        displayBounds(std::cout, v.getPhysicalBounds());
      };
      this->queries.push_back({"bounds-type", l});
    };
    this->registerCallBack(
        "--bounds-type",
        CallBack("return the bounds type associated to a variable.\n"
                 "The returned value has the follwing meaning:\n"
                 "- `None`\n"
                 "- `Lower`\n"
                 "- `Upper`\n"
                 "- `LowerAndUpper`",
                 bounds_type, true));
    const auto bounds_value = [this] {
      const auto& q = this->getCurrentCommandLineArgument();
      const auto& o = q.getOption();
      auto l = [o](const FileDescription&,
                   const MaterialPropertyDescription& mpd) {
        const auto& v = mpd.getVariableDescriptionByExternalName(o);
        displayBoundsValue(std::cout, v.getPhysicalBounds());
      };
      this->queries.push_back({"bounds-value", l});
    };
    this->registerCallBack(
        "--bounds-value",
        CallBack("show the bounds value associated as a range", bounds_value,
                 true));
    // physical bounds
    auto has_physical_bounds = [this] {
      const auto& q = this->getCurrentCommandLineArgument();
      const auto& o = q.getOption();
      auto l = [o](const FileDescription&,
                   const MaterialPropertyDescription& mpd) {
        const auto& v = mpd.getVariableDescriptionByExternalName(o);
        std::cout << (v.hasPhysicalBounds() ? "true" : "false") << '\n';
      };
      this->queries.push_back({"has-physical-bounds", l});
    };
    this->registerCallBack("--has-physical-bounds",
                           CallBack("return `true` if a variable has physical "
                                    "bounds, `false` otherwise",
                                    has_physical_bounds, true));
    auto physical_bounds_type = [this] {
      const auto& q = this->getCurrentCommandLineArgument();
      const auto& o = q.getOption();
      auto l = [o](const FileDescription&,
                   const MaterialPropertyDescription& mpd) {
        const auto& v = mpd.getVariableDescriptionByExternalName(o);
        displayBounds(std::cout, v.getPhysicalBounds());
      };
      this->queries.push_back({"physical-bounds-type", l});
    };
    this->registerCallBack(
        "--physical-bounds-type",
        CallBack("return the physical bounds type associated to a variable.\n"
                 "The returned value has the follwing meaning:\n"
                 "- `None`\n"
                 "- `Lower`\n"
                 "- `Upper`\n"
                 "- `LowerAndUpper`",
                 physical_bounds_type, true));
    const auto physical_bounds_value = [this] {
      const auto& q = this->getCurrentCommandLineArgument();
      const auto& o = q.getOption();
      auto l = [o](const FileDescription&,
                   const MaterialPropertyDescription& mpd) {
        const auto& v = mpd.getVariableDescriptionByExternalName(o);
        displayBoundsValue(std::cout, v.getPhysicalBounds());
      };
      this->queries.push_back({"physical-bounds-value", l});
    };
    this->registerCallBack(
        "--physical-bounds-value",
        CallBack("show the physical bounds value associated as a range",
                 physical_bounds_value, true));
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
           if (areParametersTreatedAsStaticVariables(mpd)) {
             return;
           }
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
    const auto pn = q.getOption();
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
    } else if (qn == "--unit-system") {
      this->queries.push_back(
          {"unit-system",
           [](const FileDescription&, const MaterialPropertyDescription& mpd) {
             if (mpd.hasUnitSystem()) {
               cout << mpd.getUnitSystem() << endl;
             } else {
               cout << "(undefined)" << endl;
             }
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
    } else if (qn == "--list-dependencies") {
      this->queries.push_back(
          {"list-dependencies",
           [](const FileDescription&, const MaterialPropertyDescription& d) {
             for (const auto& dep : d.getExternalMFrontFiles()) {
               std::cout << dep.first << " ";
             }
             std::cout << std::endl;
           }});
    } else if (qn == "--library") {
      this->queries.push_back(
          {"library",
           [](const FileDescription&, const MaterialPropertyDescription& d) {
             const auto& l = d.library;
             cout << (!l.empty() ? l : "(undefined)") << endl;
           }});
    } else if (qn == "--output") {
      this->queries.push_back(
          {"output",
           [](const FileDescription&, const MaterialPropertyDescription& mpd) {
             QueryHandlerBase::displayVariable(mpd.output);
           }});
    } else if ((qn == "--inputs") || (qn == "--state-variables")) {
      this->queries.push_back(
          {"inputs",
           [](const FileDescription&, const MaterialPropertyDescription& mpd) {
             for (const auto& p : mpd.inputs) {
               QueryHandlerBase::displayVariable(p);
             }
           }});
    } else if (qn == "--parameters") {
      this->queries.push_back(
          {"parameters",
           [](const FileDescription&, const MaterialPropertyDescription& mpd) {
             if (!areParametersTreatedAsStaticVariables(mpd)) {
               for (const auto& p : mpd.parameters) {
                 QueryHandlerBase::displayVariable(p);
               }
             }
           }});
    } else if (qn == "--parameters-file") {
      this->queries.push_back(
          {"parameters-file",
           [](const FileDescription&, const MaterialPropertyDescription& mpd) {
             if (!areParametersTreatedAsStaticVariables(mpd)) {
               std::cout << mfront::getParametersFileName(mpd) << '\n';
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

  std::size_t MaterialPropertyQuery::getNumberOfQueriesToBeTreated() {
    return this->queries.size();
  }  // end of getNumberOfQueriesToBeTreated

  void MaterialPropertyQuery::exe() {
    if (getVerboseMode() >= VERBOSE_LEVEL2) {
      getLogStream() << "Treating file '" << this->file << "'" << std::endl;
    }
    // analysing the file
    this->dsl->analyseFile(this->file, this->ecmds, this->substitutions);
    const auto& fd = this->dsl->getFileDescription();
    const auto& d = this->dsl->getMaterialPropertyDescription();
    // treating the queries
    for (const auto& q : this->queries) {
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
