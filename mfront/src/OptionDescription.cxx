/*!
 * \file   OptionDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \date   23/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"

namespace mfront::bbrick {

  OptionDescription::OptionDescription(const std::string& n,
                                       const std::string& d,
                                       const Type t)
      : name(n),
        description(d),
        type(t) {}  // end of OptionDescription::OptionDescription

  OptionDescription::OptionDescription(const std::string& n,
                                       const std::string& d,
                                       const Type t,
                                       const std::vector<std::string>& ro,
                                       const std::vector<std::string>& eo)
      : name(n),
        description(d),
        type(t),
        required_options(ro),
        incompatible_options(eo) {
  }  // end of OptionDescription::OptionDescription

  OptionDescription::OptionDescription(const std::string& n,
                                       const tfel::glossary::GlossaryEntry& e,
                                       const Type t)
      : name(n),
        description(e.getShortDescription()),
        type(t) {}  // end of OptionDescription::OptionDescription

  OptionDescription::OptionDescription(const std::string& n,
                                       const tfel::glossary::GlossaryEntry& e,
                                       const Type t,
                                       const std::vector<std::string>& ro,
                                       const std::vector<std::string>& eo)
      : name(n),
        description(e.getShortDescription()),
        type(t),
        required_options(ro),
        incompatible_options(eo) {
  }  // end of OptionDescription::OptionDescription

  OptionDescription::OptionDescription() = default;
  OptionDescription::OptionDescription(const OptionDescription&) = default;
  OptionDescription::OptionDescription(OptionDescription&&) = default;
  OptionDescription& OptionDescription::operator=(const OptionDescription&) =
      default;
  OptionDescription& OptionDescription::operator=(OptionDescription&&) =
      default;
  OptionDescription::~OptionDescription() = default;

  void check(const std::map<std::string, tfel::utilities::Data>& dv,
             const std::vector<OptionDescription>& ov) {
    auto get_opt = [&ov](const std::string& n) {
      const auto p = std::find_if(
          ov.begin(), ov.end(),
          [&n](const OptionDescription& o) { return o.name == n; });
      if (p == ov.end()) {
        auto msg = std::string{};
        msg += "check: no option named '" + n + "'.\n";
        msg += "Available options are :";
        for (const auto& o : ov) {
          msg += "\n- " + o.name + ": " + o.description;
        }
        tfel::raise(msg);
      }
      return *p;
    };
    for (const auto& d : dv) {
      const auto& o = get_opt(d.first);
      check(d.second, o);
      for (const auto& ro : o.required_options) {
        if (dv.find(ro) == dv.end()) {
          tfel::raise("check: option '" + d.first + "' requires option '" + ro +
                      "' to be defined");
        }
      }
      for (const auto& io : o.incompatible_options) {
        if (dv.find(io) != dv.end()) {
          tfel::raise("check: option '" + d.first +
                      "' is not compatible "
                      "with option '" +
                      io + "'");
        }
      }
    }
  }  // end of check

  void check(const tfel::utilities::Data& d, const OptionDescription& o) {
    auto throw_if = [&o](const bool b) {
      if (b) {
        tfel::raise("check: incompatible type for option '" + o.name + "'");
      }
    };
    auto is_material_property = [](const tfel::utilities::Data& mp) {
      return mp.is<double>() || mp.is<int>() || mp.is<std::string>();
    };
    switch (o.type) {
      case OptionDescription::BOOLEAN:
        throw_if(!d.is<bool>());
        break;
      case OptionDescription::STRING:
        throw_if(!d.is<std::string>());
        break;
      case OptionDescription::REAL:
        throw_if((!d.is<double>()) && (!d.is<int>()));
        break;
      case OptionDescription::MATERIALPROPERTY:
        throw_if(!is_material_property(d));
        break;
      case OptionDescription::ARRAYOFMATERIALPROPERTIES:
        if (d.is<std::vector<tfel::utilities::Data>>()) {
          for (const auto& mp : d.get<std::vector<tfel::utilities::Data>>()) {
            throw_if(!is_material_property(mp));
          }
        } else {
          throw_if(!is_material_property(d));
        }
        break;
      case OptionDescription::DATAMAP:
        throw_if(!d.is<std::map<std::string, tfel::utilities::Data>>());
        break;
      case OptionDescription::DATASTRUCTURE:
        throw_if(!((d.is<tfel::utilities::DataStructure>()) ||
                   (d.is<std::string>())));
        break;
      case OptionDescription::DATASTRUCTURES:
        if (d.is<std::vector<tfel::utilities::Data>>()) {
          for (const auto& e : d.get<std::vector<tfel::utilities::Data>>()) {
            throw_if(!((e.is<tfel::utilities::DataStructure>()) ||
                       (e.is<std::string>())));
          }
        } else {
          throw_if(!d.is<tfel::utilities::DataStructure>());
        }
        break;
      default:
        tfel::raise("check: unknown option type for option '" + o.name + "'");
    }
  }  // end of check

}  // end of namespace mfront::bbrick
