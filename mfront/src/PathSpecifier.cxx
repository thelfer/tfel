/*!
 * \file   mfront/src/PathSpecifier.cxx
 * \brief
 * \author Thomas Helfer
 * \date   06/10/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <regex>
#include <cstring>

#ifdef MFRONT_HAVE_MADNEX
#include "Madnex/MFrontDataBase.hxx"
#include "Madnex/MFrontImplementation.hxx"
#endif /* MFRONT_HAVE_MADNEX */

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/PathSpecifier.hxx"

namespace mfront {

  PathSpecifier::PathSpecifier() = default;
  PathSpecifier::PathSpecifier(PathSpecifier&&) = default;
  PathSpecifier::PathSpecifier(const PathSpecifier&) = default;
  PathSpecifier& PathSpecifier::operator=(PathSpecifier&&) = default;
  PathSpecifier& PathSpecifier::operator=(const PathSpecifier&) = default;
  PathSpecifier::~PathSpecifier() = default;

  bool parsePathSpecifierArguments(std::vector<PathSpecifier>& paths,
                                   PathSpecifier& s,
                                   const std::string& a) {
    auto get_substring_view = [&a](const char* const r) {
      return std::string_view{a}.substr(std::strlen(r));
    };
    auto set_material_property_identifier = [&s](std::string_view n) {
      tfel::raise_if(!s.material_property_identifier.empty(),
                     "parsePathSpecifierArguments: "
                     "material property multiply defined");
      tfel::raise_if(n.empty(),
                     "parsePathSpecifierArguments: "
                     "empty material property specified");
      s.material_property_identifier = n;
    };
    auto set_behaviour_identifier = [&s](std::string_view n) {
      tfel::raise_if(!s.behaviour_identifier.empty(),
                     "parsePathSpecifierArguments: "
                     "behaviour multiply defined");
      tfel::raise_if(n.empty(),
                     "parsePathSpecifierArguments: "
                     "empty behaviour specified");
      s.behaviour_identifier = n;
    };
    auto set_model_identifier = [&s](std::string_view n) {
      tfel::raise_if(!s.model_identifier.empty(),
                     "parsePathSpecifierArguments: "
                     "model multiply defined");
      tfel::raise_if(n.empty(),
                     "parsePathSpecifierArguments: "
                     "empty model specified");
      s.model_identifier = n;
    };

    if (a[0] != '-') {
      s.file = a;
      paths.push_back(s);
      s = PathSpecifier{};
    } else if (tfel::utilities::starts_with(a, "--material=")) {
      tfel::raise_if(!s.material_identifier.empty(),
                     "parsePathSpecifierArguments: "
                     "material multiply defined");
      s.material_identifier = a.substr(std::strlen("--material="));
      tfel::raise_if(s.material_identifier.empty(),
                     "parsePathSpecifierArguments: "
                     "empty material name specified");
    } else if (a == "--all-material-properties") {
      set_material_property_identifier(".+");
    } else if (tfel::utilities::starts_with(a, "--material-property=")) {
      set_material_property_identifier(
          get_substring_view("--material-property="));
    } else if (a == "--all-behaviours") {
      set_behaviour_identifier(".+");
    } else if (tfel::utilities::starts_with(a, "--behaviour=")) {
      set_behaviour_identifier(get_substring_view("--behaviour="));
    } else if (a == "--all-models") {
      set_model_identifier(".+");
    } else if (tfel::utilities::starts_with(a, "--model=")) {
      set_model_identifier(get_substring_view("--model="));
#if (defined _WIN32) || (defined _WIN64)
    } else if (tfel::utilities::starts_with(a, "/material=")) {
      s.material_identifier = a.substr(std::strlen("/material="));
      tfel::raise_if(s.material_identifier.empty(),
                     "parsePathSpecifierArguments: "
                     "empty material name specified");
    } else if (a == "/all-material-properties") {
      set_material_property_identifier(".+");
    } else if (tfel::utilities::starts_with(a, "/material-property=")) {
      set_material_property_identifier(
          get_substring_view("/material-property="));
    } else if (a == "/all-behaviours") {
      set_behaviour_identifier(".+");
    } else if (tfel::utilities::starts_with(a, "/behaviour=")) {
      set_behaviour_identifier(get_substring_view("/behaviour="));
    } else if (a == "/all-models") {
      set_model_identifier(".+");
    } else if (tfel::utilities::starts_with(a, "/model=")) {
      set_model_identifier(get_substring_view("/model="));
#endif /* (defined _WIN32) || (defined _WIN64)*/
    } else {
      return false;
    }
    return true;
  }  // end of parsePathSpecifierArguments

  void finalizePathSpecifierArgumentsParsing(std::vector<PathSpecifier>& paths,
                                             const PathSpecifier& p) {
    if (!p.file.empty()) {
      paths.push_back(p);
    } else {
      tfel::raise_if(!p.material_identifier.empty(),
                     "finalizePathSpecifierArgumentsParsing: "
                     "material name specified after the last input file");
      tfel::raise_if(!p.material_property_identifier.empty(),
                     "finalizePathSpecifierArgumentsParsing: "
                     "material property specified after the last input file");
      tfel::raise_if(!p.behaviour_identifier.empty(),
                     "finalizePathSpecifierArgumentsParsing: "
                     "behaviour specified after the last input file");
      tfel::raise_if(!p.model_identifier.empty(),
                     "finalizePathSpecifierArgumentsParsing: "
                     "model specified after the last input file");
    }
  }  // end of finalizePathSpecifierArgumentsParsing

#ifdef MFRONT_HAVE_MADNEX
  static std::vector<std::string> getImplementationsPathsInMadnexFiles(
      const std::string& f,
      const std::string& material_identifier,
      const std::string& material_property_identifier,
      const std::string& behaviour_identifier,
      const std::string& model_identifier) {
    auto inputs = std::vector<std::string>{};
    auto db = madnex::MFrontDataBase{f};
    auto raise = [](const std::string& m) {
      tfel::raise("getImplementationsPathsInMadnexFiles: " + m);
    };
    if ((material_property_identifier.empty()) &&
        (behaviour_identifier.empty()) && (model_identifier.empty())) {
      raise("no material property or behaviour or model identifier specified ");
    }
    if (material_identifier.empty()) {
      auto filter = [](const std::map<std::string, std::vector<std::string>>&
                           available_implementations,
                       const std::string& p) {
        auto res = std::map<std::string, std::vector<std::string>>{};
        std::regex r(p);
        for (const auto& i : available_implementations) {
          auto impls = std::vector<std::string>{};
          std::copy_if(
              i.second.begin(), i.second.end(), std::back_inserter(impls),
              [&r](const std::string& n) { return std::regex_match(n, r); });
          if (!impls.empty()) {
            res[i.first] = impls;
          }
        }
        return res;
      };  // end of filter
      auto append = [&inputs, &f](
                        const std::map<std::string, std::vector<std::string>>&
                            selected_implementations,
                        const std::string& t) {
        for (const auto& mi : selected_implementations) {
          for (const auto& i : mi.second) {
            insert_if(inputs, "mdnx:" + f + ":" + t + ":" + mi.first + ":" + i);
          }
        }
      };
      if (!material_property_identifier.empty()) {
        const auto mps = filter(db.getAvailableMaterialProperties(),
                                material_property_identifier);
        if ((mps.empty()) && (material_property_identifier != ".*")) {
          raise("no material property matching '" +
                material_property_identifier + "' in file '" + f + "'");
        }
        append(mps, "material_property");
      }
      if (!behaviour_identifier.empty()) {
        const auto bs =
            filter(db.getAvailableBehaviours(), behaviour_identifier);
        if ((bs.empty()) && (behaviour_identifier != ".*")) {
          raise("no behaviour matching '" + behaviour_identifier +
                "' in file '" + f + "'");
        }
        append(bs, "behaviour");
      }
      if (!model_identifier.empty()) {
        const auto ms = filter(db.getAvailableModels(), model_identifier);
        if ((ms.empty()) && (model_identifier != ".*")) {
          raise("no model matching '" + model_identifier + "' in file '" + f +
                "'");
        }
        append(ms, "model");
      }
    } else {
      const auto m_id = [&material_identifier]() -> std::string {
        return material_identifier == "<none>" ? "" : material_identifier;
      }();
      auto filter = [](const std::vector<std::string>& names,
                       const std::string& p) {
        auto res = std::vector<std::string>{};
        std::regex r(p);
        std::copy_if(
            names.begin(), names.end(), std::back_inserter(res),
            [&r](const std::string& n) { return std::regex_match(n, r); });
        return res;
      };  // end of filter
      auto append =
          [&inputs, &f, &m_id](
              const std::vector<std::string>& selected_implementations,
              const std::string& t) {
            for (const auto& i : selected_implementations) {
              insert_if(inputs, "mdnx:" + f + ":" + t + ":" + m_id + ":" + i);
            }
          };
      if (!material_property_identifier.empty()) {
        const auto mps = filter(db.getAvailableMaterialProperties(m_id),
                                material_property_identifier);
        if (mps.empty()) {
          raise("no material property matching '" +
                material_property_identifier + "' in file '" + f +
                "' for material '" + material_identifier + "'");
        }
        append(mps, "material_property");
      }
      if (!behaviour_identifier.empty()) {
        const auto bs =
            filter(db.getAvailableBehaviours(m_id), behaviour_identifier);
        if (bs.empty()) {
          raise("no behaviour matching '" + behaviour_identifier +
                "' in file '" + f + "' for material '" + material_identifier +
                "'");
        }
        append(bs, "behaviour");
      }
      if (!model_identifier.empty()) {
        const auto ms = filter(db.getAvailableModels(m_id), model_identifier);
        if (ms.empty()) {
          raise("no model matching '" + model_identifier + "' in file '" + f +
                "' for material '" + material_identifier + "'");
        }
        append(ms, "model");
      }
    }
    return inputs;
  }    // end of getImplementationsPathsInMadnexFiles
#endif /* MFRONT_HAVE_MADNEX */

  std::vector<std::string> getImplementationsPaths(
      const std::string& f,
      const std::string& material_identifier,
      const std::string& material_property_identifier,
      const std::string& behaviour_identifier,
      const std::string& model_identifier) {
    auto inputs = std::vector<std::string>{};
    if ((tfel::utilities::starts_with(f, "madnex:")) ||
        (tfel::utilities::starts_with(f, "mdnx:")) ||
        (tfel::utilities::starts_with(f, "edf:"))) {
#ifdef MFRONT_HAVE_MADNEX
      // full path specifier
      auto check = [](const std::string& s, const char* const t) {
        if (s.empty()) {
          return;
        }
        tfel::raise(
            "getImplementationsPaths: specifiying a " + std::string(t) +
            "' identifier is not valid when specifying a full path specifier");
      };
      check(material_identifier, "material");
      check(material_property_identifier, "material property");
      check(behaviour_identifier, "behaviour");
      check(model_identifier, "model");
      const auto details = tfel::utilities::tokenize(f, ':', true);
      if (details.size() != 5) {
        tfel::raise(
            "decomposeImplementationPathInMadnexFile: "
            "invalid path '" +
            f + "'");
      }
#else  /* MFRONT_HAVE_MADNEX */
      tfel::raise(
          "getImplementationsPaths: "
          "madnex support has not been enabled");
#endif /* MFRONT_HAVE_MADNEX */
      return {f};
    }
    // file extensions
    const auto ext = [&f]() -> std::string {
      const auto p = f.rfind(".");
      if (p != std::string::npos) {
        return f.substr(p + 1);
      }
      return "";
    }();
    if ((ext == "madnex") || (ext == "mdnx") || (ext == "edf")) {
#ifdef MFRONT_HAVE_MADNEX
      inputs = getImplementationsPathsInMadnexFiles(
          f, material_identifier, material_property_identifier,
          behaviour_identifier, model_identifier);
#else  /* MFRONT_HAVE_MADNEX */
      tfel::raise(
          "getImplementationsPaths: "
          "madnex support has not been enabled");
#endif /* MFRONT_HAVE_MADNEX */
    } else {
      auto check = [&ext](const std::string& s, const char* const t) {
        if (s.empty()) {
          return;
        }
        tfel::raise("getImplementationsPaths: specifiying a " + std::string(t) +
                    "' identifier is not valid for files with the extension '" +
                    ext + "'");
      };
      check(material_identifier, "material");
      check(material_property_identifier, "material property");
      check(behaviour_identifier, "behaviour");
      check(model_identifier, "model");
      inputs.push_back(f);
    }
    return inputs;
  }  // end of getImplementationsPaths

  std::vector<std::string> getImplementationsPaths(const PathSpecifier& p) {
    return getImplementationsPaths(p.file, p.material_identifier,
                                   p.material_property_identifier,
                                   p.behaviour_identifier, p.model_identifier);
  }  // end of getImplementationsPaths

  std::vector<std::string> getImplementationsPaths(
      const std::vector<PathSpecifier>& paths) {
    auto implementations = std::vector<std::string>{};
    for (const auto& p : paths) {
      const auto impls = mfront::getImplementationsPaths(p);
      implementations.insert(implementations.end(), impls.begin(), impls.end());
    }
    return implementations;
  }  // end of getImplementationsPaths

}  // end of namespace mfront
