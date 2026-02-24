/*!
 * \file  src/Glossary/GlossaryEntry.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 31 mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"

namespace tfel::glossary {

  const char* GlossaryEntry::separator = "@^separator^@";

  static std::vector<std::string> vector_from_range(
      const char* const* const cb, const char* const* const ce) noexcept {
    std::vector<std::string> r;
    r.insert(r.end(), cb, ce);
    return r;
  }

  static std::vector<std::string> tokenize(const char* c) noexcept {
    return tfel::utilities::tokenize(c, GlossaryEntry::separator);
  }

  static std::map<std::string, std::string, std::less<>> map_from_string(
      const std::string_view& s) {
    std::map<std::string, std::string, std::less<>> r;
    const auto pairs = tfel::utilities::tokenize(s, GlossaryEntry::separator);
    for (const auto& p : pairs) {
      const auto kv = tfel::utilities::tokenize(p, ':');
      if (kv.size() != 2u) {
        tfel::raise(
            "map_from_string: invalid map encoding. "
            "Error while treating string '" +
            std::string{s} + "'");
      }
      if (!r.insert({kv[0], kv[1]}).second) {
        tfel::raise("map_from_string: key '" + kv[0] +
                    "' multiply defined. "
                    "Error while treating string '" +
                    std::string{s} + "'");
      }
    }
    return r;
  }  // end of map_from_string

  GlossaryEntry::GlossaryEntry(const char* const k,
                               const char* const* const b,
                               const char* const* const e,
                               const char* const u,
                               const char* const t,
                               const char* const sd,
                               const char* const d,
                               const char* const no,
                               const char* const lbs,
                               const char* const ubs)
      : key(k),
        names(vector_from_range(b, e)),
        units(map_from_string(u)),
        type(t),
        short_description(sd),
        description(tfel::glossary::tokenize(d)),
        notes(tfel::glossary::tokenize(no)),
        lower_physical_bounds(map_from_string(lbs)),
        upper_physical_bounds(map_from_string(ubs)) {
    this->check();
  }

  GlossaryEntry::GlossaryEntry(const GlossaryEntry&) = default;
  GlossaryEntry::GlossaryEntry(GlossaryEntry&&) = default;

  void GlossaryEntry::check() const {
    auto convert = [this](const std::string& s) {
      auto throw_if = [this, &s](const bool b) {
        if (b) {
          tfel::raise<std::invalid_argument>(
              "GlossaryEntry::check: "
              "could not convert '" +
              s +
              "' "
              "to double while treating bounds of entry '" +
              this->key + "'");
        }
      };
      throw_if(s.empty());
      auto r = double{};
      std::istringstream is(s);
      is.imbue(std::locale("C"));
      is >> r;
      if (is.fail() || is.bad() || !is.eof()) {
        throw_if(true);
      }
      return r;
    };
    if (this->names.empty()) {
      tfel::raise(
          "GlossaryEntry::check: "
          "no name specified for key '" +
          this->getKey() + "'");
    }
    if (!((this->type == "scalar") || (this->type == "vector") ||
          (this->type == "symmetric tensor") || (this->type == "tensor"))) {
      tfel::raise(
          "GlossaryEntry::check: "
          "unsupported type '" +
          this->type +
          "' "
          "for entry '" +
          this->getKey() + "'");
    }
    for (const auto& lb : lower_physical_bounds) {
      convert(lb.second);
    }
    for (const auto& ub : upper_physical_bounds) {
      convert(ub.second);
    }
    for (const auto& lb : lower_physical_bounds) {
      if (this->hasUpperPhysicalBound(lb.first)) {
        const auto lbv = convert(lb.second);
        const auto lbu = convert(this->upper_physical_bounds.at(lb.first));
        if (lbv > lbu) {
          tfel::raise(
              "GlossaryEntry::check: "
              "lower bound is greater than upper bound "
              "for key '" +
              this->key + "'");
        }
      }
    }
  }  // end of GlossaryEntry::check

  const std::string& GlossaryEntry::getKey() const { return this->key; }

  const std::vector<std::string>& GlossaryEntry::getNames() const {
    return this->names;
  }

  const std::string& GlossaryEntry::getUnit(const std::string_view s) const {
    if (this->units.empty()) {
      static const std::string empty;
      return empty;
    }
    const auto p = this->units.find(s);
    if (p == this->units.end()) {
      raise("GlossaryEntry::getUnit: no unit defined for entry " +
            std::string{*this} + " for unit system '" + std::string{s} + "'");
    }
    return p->second;
  }  // end of getUnit

  std::map<std::string, std::string> GlossaryEntry::getUnits() const {
    return std::map<std::string, std::string>{this->units.begin(),
                                              this->units.end()};
  }  // end of getUnits

  const std::string& GlossaryEntry::getType() const { return this->type; }

  const std::string& GlossaryEntry::getShortDescription() const {
    return this->short_description;
  }

  const std::vector<std::string>& GlossaryEntry::getDescription() const {
    return this->description;
  }

  const std::vector<std::string>& GlossaryEntry::getNotes() const {
    return this->notes;
  }

  GlossaryEntry::operator const std::string&() const {
    return this->key;
  }  // end of operator std::string

  bool GlossaryEntry::hasLowerPhysicalBound(const std::string_view s) const {
    return this->lower_physical_bounds.count(s) != 0;
  }  // end of hasLowerPhysicalBound

  std::string GlossaryEntry::getLowerPhysicalBound(
      const std::string_view s) const {
    if (!this->hasLowerPhysicalBound(s)) {
      tfel::raise(
          "GlossaryEntry::getLowerPhysicalBound: "
          "no physical lower bound defined for entry '" +
          this->key + "' in the '" + std::string{s} + "' unit system");
    }
    return this->lower_physical_bounds.find(s)->second;
  }  // end of getLowerPhysicalBound

  bool GlossaryEntry::hasUpperPhysicalBound(const std::string_view s) const {
    return this->upper_physical_bounds.count(s) != 0;
  }  // end of hasUpperPhysicalBound

  std::string GlossaryEntry::getUpperPhysicalBound(
      const std::string_view s) const {
    if (!this->hasUpperPhysicalBound(s)) {
      tfel::raise(
          "GlossaryEntry::getUpperPhysicalBound: "
          "no physical upper bound defined for entry '" +
          this->key + "' in the '" + std::string{s} + "' unit system");
    }
    return this->upper_physical_bounds.find(s)->second;
  }  // end of getUpperPhysicalBound

  GlossaryEntry::~GlossaryEntry() = default;

  bool operator<(const GlossaryEntry& e1, const GlossaryEntry& e2) {
    return e1.key < e2.key;
  }

  bool operator!=(const std::string& e1, const GlossaryEntry& e2) {
    return e1 != e2.key;
  }

  bool operator!=(const GlossaryEntry& e1, const std::string& e2) {
    return e1.key != e2;
  }

  TFELGLOSSARY_VISIBILITY_EXPORT bool operator==(const std::string& e1,
                                                 const GlossaryEntry& e2) {
    return e1 == e2.key;
  }

  TFELGLOSSARY_VISIBILITY_EXPORT bool operator==(const GlossaryEntry& e1,
                                                 const std::string& e2) {
    return e1.key == e2;
  }

}  // end of namespace tfel::glossary
