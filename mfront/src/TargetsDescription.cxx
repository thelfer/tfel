/*!
 * \file  TargetsDescription.cxx
 * \brief
 * \author Thomas Helfer
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
#include "TFEL/Raise.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/TargetsDescription.hxx"

namespace mfront {

  TargetsDescription::TargetsDescription() = default;
  TargetsDescription::TargetsDescription(const TargetsDescription&) = default;
  TargetsDescription::TargetsDescription(TargetsDescription&&) = default;
  TargetsDescription::~TargetsDescription() = default;

  LibraryDescription& TargetsDescription::getLibrary(
      const std::string& n,
      const std::string& pr,
      const std::string& s,
      const LibraryDescription::LibraryType t) {
    auto throw_if = [](const bool b, const std::string& msg) {
      tfel::raise_if(b, "TargetsDescription::getLibrary: " + msg);
    };
    auto c = [&n](const LibraryDescription& l) { return l.name == n; };
    const auto b = this->libraries.begin();
    const auto e = this->libraries.end();
    const auto p = std::find_if(b, e, c);
    if (p == e) {
      this->libraries.emplace_back(n, pr, s, t);
      return this->libraries.back();
    }
    auto& l = *p;
    throw_if(l.prefix != pr, "unmatched library prefix for library '" + n +
                                 "'"
                                 " ('" +
                                 l.prefix + "' vs '" + s + "')");
    throw_if(l.suffix != s, "unmatched library suffix for library '" + n +
                                "'"
                                " ('" +
                                l.suffix + "' vs '" + s + "')");
    throw_if(l.type != t, "unmatched library type for library '" + n +
                              "'"
                              " ('" +
                              convert(l.type) + "' vs '" + convert(t) + "')");
    return l;
  }

  LibraryDescription& TargetsDescription::getLibrary(const std::string& n,
                                                     const std::string& p) {
    const auto s = LibraryDescription::getDefaultLibrarySuffix(
        this->system, this->libraryType);
    return this->getLibrary(n, p, s, this->libraryType);
  }  // end of TargetsDescription::getLibrary

  LibraryDescription& TargetsDescription::getLibrary(const std::string& n,
                                                     const std::string& pr,
                                                     const std::string& s) {
    return this->getLibrary(n, pr, s, this->libraryType);
  }  // end of TargetsDescription::getLibrary

  LibraryDescription& TargetsDescription::getLibrary(const std::string& n) {
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

  const LibraryDescription& TargetsDescription::getLibrary(
      const std::string& n) const {
    auto c = [&n](const LibraryDescription& l) { return l.name == n; };
    const auto b = this->libraries.begin();
    const auto e = this->libraries.end();
    const auto p = std::find_if(b, e, c);
    tfel::raise_if(p == e,
                   "TargetsDescription::operator[]: "
                   "no library named '" +
                       n + "'");
    return *p;
  }  // end of TargetsDescription::operator[]

  void mergeTargetsDescription(TargetsDescription& d,
                               const TargetsDescription& s,
                               const bool b) {
    for (const auto& ls : s.libraries) {
      auto& ld = d.getLibrary(ls.name, ls.prefix, ls.suffix, ls.type);
      mergeLibraryDescription(ld, ls);
    }
    for (const auto& h : s.headers) {
      if (std::find(d.headers.begin(), d.headers.end(), h) == d.headers.end()) {
        d.headers.push_back(h);
      }
    }
    for (const auto& t : s.specific_targets) {
      if (t.first == "all") {
        continue;
      }
      if (b) {
        d.specific_targets[t.first] = t.second;
      } else {
        if (d.specific_targets.find(t.first) == d.specific_targets.end()) {
          d.specific_targets[t.first] = t.second;
        }
      }
    }
    if (s.specific_targets.count("all") != 0) {
      auto& a = d.specific_targets["all"];
      const auto& as = s.specific_targets.at("all");
      for (const auto& dep : as.deps) {
        insert_if(a.deps, dep);
      }
    }
  }  // end of mergeTargetsDescription

  bool describes(const TargetsDescription& t, const std::string& n) {
    auto comp = [&n](const LibraryDescription& ld) { return ld.name == n; };
    return std::find_if(t.libraries.begin(), t.libraries.end(), comp) !=
           t.libraries.end();
  }  // end of describes

  std::ostream& operator<<(std::ostream& os, const TargetsDescription& t) {
    os << "{\n";
    for (const auto& l : t.libraries) {
      os << "library : " << l;
    }
    if (!t.headers.empty()) {
      write(os, t.headers, "headers");
    }
    for (const auto& target : t.specific_targets) {
      os << "target : {\n";
      os << "name : \"" << target.first << "\";\n";
      write(os, target.second.deps, "dependencies");
      write(os, target.second.cmds, "commands");
      write(os, target.second.sources, "sources");
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
      tfel::raise(std::string{f} + ": " + m);
    };
    auto get_vector = [&f](
                          std::vector<std::string>& v,
                          tfel::utilities::CxxTokenizer::const_iterator& pc,
                          const tfel::utilities::CxxTokenizer::const_iterator e,
                          const std::string& n) {
      tfel::raise_if(!v.empty(), std::string{f} + ": library member '" + n +
                                     "' multiply defined");
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
        mergeLibraryDescription(
            t.getLibrary(l.name, l.prefix, l.suffix, l.type), l);
      } else if (c->value == "headers") {
        ++c;
        get_vector(t.headers, c, pe, "headers");
      } else if (c->value == "target") {
        auto name = std::string{};
        auto deps = std::vector<std::string>{};
        auto cmds = std::vector<std::string>{};
        auto sources = std::vector<std::string>{};
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
          } else if (c->value == "sources") {
            ++c;
            get_vector(sources, c, pe, "sources");
          } else {
            error("unsupported tag '" + c->value +
                  "' "
                  "for specific target description");
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
        std::swap(st.deps, deps);
        std::swap(st.cmds, cmds);
        std::swap(st.sources, sources);
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
