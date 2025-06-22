/*!
 * \file  TargetsDescription.cxx
 * \brief
 * \author Helfer Thomas
 * \date   09 mai 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include <algorithm>
#include <stdexcept>
#include "MFront/MFrontUtilities.hxx"
#include "MFront/TargetsDescription.hxx"

namespace mfront {

  TargetsDescription::TargetsDescription() = default;
  TargetsDescription::TargetsDescription(const TargetsDescription&) = default;
  TargetsDescription::TargetsDescription(TargetsDescription&&) = default;
  TargetsDescription::~TargetsDescription() = default;

  LibraryDescription& TargetsDescription::operator()(
      const std::string& n,
      const std::string& pr,
      const std::string& s,
      const LibraryDescription::LibraryType t) {
    auto c = [&n](const LibraryDescription& l) { return l.name == n; };
    const auto b = this->libraries.begin();
    const auto e = this->libraries.end();
    const auto p = std::find_if(b, e, c);
    if (p == e) {
      this->libraries.emplace_back(n, pr, s, t);
      return this->libraries.back();
    }
    auto& l = *p;
    if (l.prefix != pr) {
      throw(
          std::runtime_error("TargetsDescription::operator() : "
                             "unmatched library prefix for library '" +
                             n +
                             "'"
                             " ('" +
                             l.prefix + "' vs '" + s + "')"));
    }
    if (l.suffix != s) {
      throw(
          std::runtime_error("TargetsDescription::operator() : "
                             "unmatched library suffix for library '" +
                             n +
                             "'"
                             " ('" +
                             l.suffix + "' vs '" + s + "')"));
    }
    if (l.type != t) {
      throw(
          std::runtime_error("TargetsDescription::operator() : "
                             "unmatched library type for library '" +
                             n +
                             "'"
                             " ('" +
                             convert(l.type) + "' vs '" + convert(t) + "')"));
    }
    return l;
  }

  LibraryDescription& TargetsDescription::operator()(const std::string& n,
                                                     const std::string& p) {
    const auto s = LibraryDescription::getDefaultLibrarySuffix(
        this->system, this->libraryType);
    return this->operator()(n, p, s, this->libraryType);
  }  // end of TargetsDescription::operator()

  LibraryDescription& TargetsDescription::operator()(const std::string& n,
                                                     const std::string& pr,
                                                     const std::string& s) {
    return this->operator()(n, pr, s, this->libraryType);
  }  // end of TargetsDescription::operator()

  LibraryDescription& TargetsDescription::operator[](const std::string& n) {
    auto c = [&n](const LibraryDescription& l) { return l.name == n; };
    const auto b = this->libraries.begin();
    const auto e = this->libraries.end();
    const auto p = std::find_if(b, e, c);
    if (p == e) {
      const auto pr = LibraryDescription::getDefaultLibraryPrefix(
          this->system, this->libraryType);
      const auto s = LibraryDescription::getDefaultLibrarySuffix(
          this->system, this->libraryType);
      this->libraries.emplace_back(n, pr, s, this->libraryType);
      return this->libraries.back();
    }
    return *p;
  }  // end of TargetsDescription::operator[]

  const LibraryDescription& TargetsDescription::operator[](
      const std::string& n) const {
    auto c = [&n](const LibraryDescription& l) { return l.name == n; };
    const auto b = this->libraries.begin();
    const auto e = this->libraries.end();
    const auto p = std::find_if(b, e, c);
    if (p == e) {
      throw(
          std::runtime_error("TargetsDescription::operator[] : "
                             "no library named '" +
                             n + "'"));
    }
    return *p;
  }  // end of TargetsDescription::operator[]

  TargetsDescription::iterator TargetsDescription::begin() {
    return this->libraries.begin();
  }

  TargetsDescription::iterator TargetsDescription::end() {
    return this->libraries.end();
  }

  TargetsDescription::const_iterator TargetsDescription::begin() const {
    return this->libraries.cbegin();
  }

  TargetsDescription::const_iterator TargetsDescription::cbegin() const {
    return this->libraries.cbegin();
  }

  TargetsDescription::const_iterator TargetsDescription::end() const {
    return this->libraries.cend();
  }

  TargetsDescription::const_iterator TargetsDescription::cend() const {
    return this->libraries.cend();
  }

  void mergeTargetsDescription(TargetsDescription& d,
                               const TargetsDescription& s,
                               const bool b) {
    for (const auto& ls : s) {
      auto& ld = d(ls.name, ls.prefix, ls.suffix, ls.type);
      mergeLibraryDescription(ld, ls);
    }
    for (const auto& h : s.headers) {
      if (std::find(d.headers.begin(), d.headers.end(), h) == d.headers.end()) {
        d.headers.push_back(h);
      }
    }
    if (b) {
      for (const auto& t : s.specific_targets) {
        d.specific_targets[t.first] = t.second;
      }
    } else {
      for (const auto& t : s.specific_targets) {
        if (d.specific_targets.find(t.first) == d.specific_targets.end()) {
          d.specific_targets[t.first] = t.second;
        }
      }
    }
  }  // end of mergeTargetsDescription

  bool describes(const TargetsDescription& t, const std::string& n) {
    auto comp = [&n](const LibraryDescription& ld) { return ld.name == n; };
    return std::find_if(t.begin(), t.end(), comp) != t.end();
  }  // end of describes

  std::ostream& operator<<(std::ostream& os, const TargetsDescription& t) {
    os << "{\n";
    for (const auto& l : t) {
      os << "library : " << l;
    }
    if (!t.headers.empty()) {
      write(os, t.headers, "headers");
    }
    for (const auto& target : t.specific_targets) {
      os << "target : {\n";
      os << "name : \"" << target.first << "\";\n";
      const auto& deps = target.second.first;
      const auto& cmds = target.second.second;
      write(os, deps, "dependencies");
      write(os, cmds, "commands");
      os << "};\n";
    }
    os << "};\n";
    return os;
  }

  template <>
  TargetsDescription read(
      tfel::utilities::CxxTokenizer::const_iterator& p,
      const tfel::utilities::CxxTokenizer::const_iterator pe) {
    using tfel::utilities::CxxTokenizer;
    const auto f = "read<TargetsDescription>";
    auto error = [&f](const std::string& m) {
      throw(std::runtime_error(std::string{f} + ": " + m));
    };
    auto get_vector = [&f](
                          std::vector<std::string>& v,
                          tfel::utilities::CxxTokenizer::const_iterator& pc,
                          const tfel::utilities::CxxTokenizer::const_iterator e,
                          const std::string& n) {
      if (!v.empty()) {
        throw(std::runtime_error(std::string{f} + ": library member '" + n +
                                 "' multiply defined"));
      }
      auto c = pc;
      CxxTokenizer::readSpecifiedToken(f, ":", c, e);
      v = read<std::vector<std::string>>(c, e);
      CxxTokenizer::readSpecifiedToken(f, ";", c, e);
      pc = c;
    };
    TargetsDescription t;
    // parsing
    auto c = p;
    CxxTokenizer::readSpecifiedToken(f, "{", c, pe);
    CxxTokenizer::checkNotEndOfLine(f, c, pe);
    while (c->value != "}") {
      if (c->value == "library") {
        ++c;
        CxxTokenizer::readSpecifiedToken(f, ":", c, pe);
        const auto l = read<LibraryDescription>(c, pe);
        CxxTokenizer::readSpecifiedToken(f, ";", c, pe);
        if (describes(t, l.name)) {
          error("library '" + l.name + "' multiply defined");
        }
        mergeLibraryDescription(t(l.name, l.prefix, l.suffix, l.type), l);
      } else if (c->value == "headers") {
        ++c;
        get_vector(t.headers, c, pe, "headers");
      } else if (c->value == "target") {
        auto name = std::string{};
        auto deps = std::vector<std::string>{};
        auto cmds = std::vector<std::string>{};
        ++c;
        CxxTokenizer::readSpecifiedToken(f, ":", c, pe);
        CxxTokenizer::readSpecifiedToken(f, "{", c, pe);
        while (c->value != "}") {
          if (c->value == "name") {
            if (!name.empty()) {
              error("target name '" + name + "' multiply defined");
            }
            ++c;
            CxxTokenizer::readSpecifiedToken(f, ":", c, pe);
            name = CxxTokenizer::readString(c, pe);
            CxxTokenizer::readSpecifiedToken(f, ";", c, pe);
          } else if (c->value == "dependencies") {
            ++c;
            get_vector(deps, c, pe, "dependencies");
          } else if (c->value == "commands") {
            ++c;
            get_vector(cmds, c, pe, "commands");
          } else {
            error("unsupported tag '" + c->value +
                  "' for specific target description");
          }
        }
        CxxTokenizer::readSpecifiedToken(f, "}", c, pe);
        CxxTokenizer::readSpecifiedToken(f, ";", c, pe);
        if (name.empty()) {
          error("unspecified specific target name");
        }
        if (t.specific_targets.find(name) != t.specific_targets.end()) {
          error("specific target '" + name + "' multiply defined");
        }
        auto& st = t.specific_targets[name];
        std::swap(st.first, deps);
        std::swap(st.second, cmds);
      } else {
        error("unsupported tag '" + c->value + "'");
      }
    }
    CxxTokenizer::readSpecifiedToken(f, "}", c, pe);
    CxxTokenizer::readSpecifiedToken(f, ";", c, pe);
    p = c;
    return t;
  }  // end of read

}  // end of namespace mfront
