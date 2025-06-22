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

#include <iostream>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"

namespace tfel {

  namespace glossary {

    static std::string nothrow(const char* c) noexcept {
      std::string r;
      try {
        r = c;
      } catch (std::exception& e) {
        std::cerr << "tfel::glossary::nothrow2: " << e.what() << '\n';
      } catch (...) {
        std::cerr << "tfel::glossary::nothrow2: "
                  << "unknown exception\n";
      }
      return r;
    }

    static std::vector<std::string> nothrow2(const char* c) noexcept {
      std::vector<std::string> r;
      try {
        r.emplace_back(c);
      } catch (std::exception& e) {
        std::cerr << "tfel::glossary::nothrow2: " << e.what() << '\n';
      } catch (...) {
        std::cerr << "tfel::glossary::nothrow2: "
                  << "unknown exception\n";
      }
      return r;
    }

    static std::vector<std::string> nothrow2(
        const char* const* const cb, const char* const* const ce) noexcept {
      std::vector<std::string> r;
      try {
        r.insert(r.end(), cb, ce);
      } catch (std::exception& e) {
        std::cerr << "tfel::glossary::nothrow2: " << e.what() << '\n';
      } catch (...) {
        std::cerr << "tfel::glossary::nothrow2: "
                  << "unknown exception\n";
      }
      return r;
    }

    static std::vector<std::string> tokenize(const char* c) noexcept {
      std::vector<std::string> r;
      try {
        r = tfel::utilities::tokenize(c, "@^separator^@");
      } catch (std::exception& e) {
        std::cerr << "tfel::glossary::tokenize: " << e.what() << '\n';
      } catch (...) {
        std::cerr << "tfel::glossary::tokenize: "
                  << "unknown exception\n";
      }
      return r;
    }

    GlossaryEntry::GlossaryEntry(const std::string& k,
                                 const std::string& n,
                                 const std::string& u,
                                 const std::string& t,
                                 const std::string& sd,
                                 const std::vector<std::string>& d,
                                 const std::vector<std::string>& no)
        : key(k),
          names(1u, n),
          unit(u),
          type(t),
          short_description(sd),
          description(d),
          notes(no) {
      this->check();
    }

    GlossaryEntry::GlossaryEntry(const std::string& k,
                                 const std::string& n,
                                 const std::string& u,
                                 const std::string& t,
                                 const std::string& sd,
                                 const std::string& d,
                                 const std::string& no)
        : key(k),
          names(1u, n),
          unit(u),
          type(t),
          short_description(sd),
          description(tfel::utilities::tokenize(d, "@^separator^@")),
          notes(tfel::utilities::tokenize(no, "@^separator^@")) {
      this->check();
    }

    GlossaryEntry::GlossaryEntry(const char* const k,
                                 const char* const n,
                                 const char* const u,
                                 const char* const t,
                                 const char* const sd,
                                 const char* const d,
                                 const char* const no) noexcept
        : key(nothrow(k)),
          names(nothrow2(n)),
          unit(nothrow(u)),
          type(nothrow(t)),
          short_description(nothrow(sd)),
          description(tfel::glossary::tokenize(d)),
          notes(tfel::glossary::tokenize(no)) {
      try {
        this->check();
      } catch (std::exception& ex) {
        std::cerr << "GlossaryEntry::GlossaryEntry: " << ex.what() << '\n';
      } catch (...) {
        std::cerr << "GlossaryEntry::GlossaryEntry: "
                  << "unknown exception\n";
      }
    }

    GlossaryEntry::GlossaryEntry(const std::string& k,
                                 const std::vector<std::string>& n,
                                 const std::string& u,
                                 const std::string& t,
                                 const std::string& sd,
                                 const std::vector<std::string>& d,
                                 const std::vector<std::string>& no)
        : key(k),
          names(n),
          unit(u),
          type(t),
          short_description(sd),
          description(d),
          notes(no) {
      this->check();
    }

    GlossaryEntry::GlossaryEntry(const std::string& k,
                                 const std::vector<std::string>& n,
                                 const std::string& u,
                                 const std::string& t,
                                 const std::string& sd,
                                 const std::string& d,
                                 const std::string& no)
        : key(k),
          names(n),
          unit(u),
          type(t),
          short_description(sd),
          description(tfel::utilities::tokenize(d, "@^separator^@")),
          notes(tfel::utilities::tokenize(no, "@^separator^@")) {
      this->check();
    }

    GlossaryEntry::GlossaryEntry(const std::string& k,
                                 const char* const* const b,
                                 const char* const* const e,
                                 const std::string& u,
                                 const std::string& t,
                                 const std::string& sd,
                                 const std::vector<std::string>& d,
                                 const std::vector<std::string>& no)
        : key(k),
          names(b, e),
          unit(u),
          type(t),
          short_description(sd),
          description(d),
          notes(no) {
      this->check();
    }

    GlossaryEntry::GlossaryEntry(const std::string& k,
                                 const char* const* const b,
                                 const char* const* const e,
                                 const std::string& u,
                                 const std::string& t,
                                 const std::string& sd,
                                 const std::string& d,
                                 const std::string& no)
        : key(k),
          names(b, e),
          unit(u),
          type(t),
          short_description(sd),
          description(tfel::utilities::tokenize(d, "@^separator^@")),
          notes(tfel::utilities::tokenize(no, "@^separator^@")) {
      this->check();
    }

    GlossaryEntry::GlossaryEntry(const char* const k,
                                 const char* const* const b,
                                 const char* const* const e,
                                 const char* const u,
                                 const char* const t,
                                 const char* const sd,
                                 const char* const d,
                                 const char* const no) noexcept
        : key(nothrow(k)),
          names(nothrow2(b, e)),
          unit(nothrow(u)),
          type(nothrow(t)),
          short_description(nothrow(sd)),
          description(tfel::glossary::tokenize(d)),
          notes(tfel::glossary::tokenize(no)) {
      try {
        this->check();
      } catch (std::exception& ex) {
        std::cerr << "GlossaryEntry::GlossaryEntry: " << ex.what() << '\n';
      } catch (...) {
        std::cerr << "GlossaryEntry::GlossaryEntry: "
                  << "unknown exception\n";
      }
    }

    GlossaryEntry::GlossaryEntry(const GlossaryEntry&) = default;
    GlossaryEntry::GlossaryEntry(GlossaryEntry&&) = default;

    void GlossaryEntry::check() const {
      raise_if(this->names.empty(),
               "GlossaryEntry::check: "
               "no name specified for key '" +
                   this->getKey() + "'");
      raise_if(
          !((this->type == "scalar") || (this->type == "vector") ||
            (this->type == "symmetric tensor") || (this->type == "tensor")),
          "GlossaryEntry::check: "
          "unsupported type '" +
              this->type +
              "' "
              "for entry '" +
              this->getKey() + "'");
    }  // end of GlossaryEntry::check

    const std::string& GlossaryEntry::getKey() const { return this->key; }

    const std::vector<std::string>& GlossaryEntry::getNames() const {
      return this->names;
    }

    const std::string& GlossaryEntry::getUnit() const { return this->unit; }

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

  }  // end of namespace glossary

}  // end of namespace tfel
