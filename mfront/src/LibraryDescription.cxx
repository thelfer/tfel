/*!
 * \file  LibraryDescription.cxx
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

#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/LibraryDescription.hxx"

// fixing a bug on current glibc++ cygwin versions (19/08/2015)
#if defined __CYGWIN__ && (!defined _GLIBCXX_USE_C99)
#include <sstream>
namespace std {
  template <typename T>
  std::string to_string(const T& v) {
    std::ostringstream s;
    s << v;
    return s.str();
  }
}  // namespace std
#endif /* defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99) */

namespace mfront {

  std::string convert(const LibraryDescription::LibraryType t) {
    if (t == LibraryDescription::SHARED_LIBRARY) {
      return "shared library";
    }
    tfel::raise_if(t != LibraryDescription::MODULE,
                   "convert: unsupported library type");
    return "module";
  }  // end of to string

  void mergeLibraryDescription(LibraryDescription& d,
                               const LibraryDescription& s) {
    tfel::raise_if((d.name != s.name) || (d.prefix != s.prefix) ||
                       (d.suffix != s.suffix) || (d.type != s.type),
                   "mergeLibraryDescription : "
                   "can't merge description of library' " +
                       d.name +
                       "' ("
                       "prefix: '" +
                       d.prefix + "', " + "suffix: '" + d.suffix + "', " +
                       "type: '" + convert(d.type) +
                       "')"
                       "and description of library '" +
                       s.name +
                       "' ("
                       "prefix: '" +
                       s.prefix + "', " + "suffix: '" + s.suffix + "', " +
                       "type: '" + convert(s.type) + "')");
    insert_if(d.sources, s.sources);
    insert_if(d.cppflags, s.cppflags);
    insert_if(d.include_directories, s.include_directories);
    insert_if(d.ldflags, s.ldflags);
    insert_if(d.link_directories, s.link_directories);
    insert_if(d.link_libraries, s.link_libraries);
    insert_if(d.epts, s.epts);
    insert_if(d.deps, s.deps);
  }  // end of mergeLibraryDescription

  const char* LibraryDescription::getDefaultLibraryPrefix(
      const TargetSystem s, const LibraryType) noexcept {
    if (s == CYGWIN) {
      return "cyg";
    }
    return "lib";
  }

  const char* LibraryDescription::getDefaultLibrarySuffix(
      const TargetSystem s, const LibraryType l) noexcept {
    auto error = [] {
      tfel::raise(
          "LibraryDescription::getDefaultLibrarySuffix : "
          "internal errror : unsupported system or library type");
    };
    const char* p = nullptr;
    switch (s) {
      case UNIX:
        p = "so";
        break;
      case CYGWIN:
        p = "dll";
        break;
      case WINDOWS:
        p = "dll";
        break;
      case MACOSX:
        switch (l) {
          case SHARED_LIBRARY:
            p = "dylib";
            break;
          case MODULE:
            p = "bundle";
            break;
          default:
            error();
        }
        break;
      default:
        error();
    }
    if (p == nullptr) {
      error();
    }
    return p;
  }  // end of LibraryDescription::getDefaultLibrarySuffix

  LibraryDescription::LibraryDescription(const std::string& n,
                                         const std::string& p,
                                         const std::string& s,
                                         const LibraryType t)
      : name(n),
        prefix(p),
        suffix(s),
        type(t) {}  // end of LibraryDescription::LibraryDescription

  LibraryDescription::LibraryDescription(const LibraryDescription&) = default;
  LibraryDescription::LibraryDescription(LibraryDescription&&) = default;
  LibraryDescription::~LibraryDescription() = default;

  std::ostream& operator<<(std::ostream& os, const LibraryDescription& l) {
    auto error = [&l] {
      tfel::raise(
          "operator << : "
          "unsuported library type (neither module or "
          "shared library)  for library '" +
          l.name + "'");
    };
    os << "{\n";
    os << "name   : \"" << l.name << "\";\n";
    os << "type   : ";
    switch (l.type) {
      case LibraryDescription::MODULE:
        os << "MODULE";
        break;
      case LibraryDescription::SHARED_LIBRARY:
        os << "SHARED_LIBRARY";
        break;
      default:
        error();
    }
    os << ";\n";
    os << "prefix : \"" << l.prefix << "\";\n";
    os << "suffix : \"" << l.suffix << "\";\n";
    write(os, l.sources, "sources");
    write(os, l.cppflags, "cppflags");
    write(os, l.include_directories, "include_directories");
    write(os, l.ldflags, "ldflags");
    write(os, l.link_directories, "link_directories");
    write(os, l.link_libraries, "link_libraries");
    write(os, l.epts, "epts");
    write(os, l.deps, "deps");
    os << "};\n";
    return os;
  }

  template <>
  MFRONT_VISIBILITY_EXPORT LibraryDescription
  read(tfel::utilities::CxxTokenizer::const_iterator& p,
       const tfel::utilities::CxxTokenizer::const_iterator pe) {
    using tfel::utilities::CxxTokenizer;
    const auto f = "read<LibraryDescription>";
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
    LibraryDescription::LibraryType type = LibraryDescription::SHARED_LIBRARY;
    auto btype = false;
    auto name = std::string{};
    auto prefix = std::string{};
    auto suffix = std::string{};
    auto sources = std::vector<std::string>{};
    auto cppflags = std::vector<std::string>{};
    auto include_directories = std::vector<std::string>{};
    auto ldflags = std::vector<std::string>{};
    auto link_directories = std::vector<std::string>{};
    auto link_libraries = std::vector<std::string>{};
    auto epts = std::vector<std::string>{};
    auto deps = std::vector<std::string>{};
    // parsing
    auto c = p;
    CxxTokenizer::readSpecifiedToken(f, "{", c, pe);
    CxxTokenizer::checkNotEndOfLine(f, c, pe);
    while (c->value != "}") {
      if (c->value == "name") {
        if (!name.empty()) {
          error("library name '" + name + "' multiply defined");
        }
        ++c;
        CxxTokenizer::readSpecifiedToken(f, ":", c, pe);
        name = CxxTokenizer::readString(c, pe);
        CxxTokenizer::readSpecifiedToken(f, ";", c, pe);
      } else if (c->value == "prefix") {
        if (!prefix.empty()) {
          error("library prefix multiply defined");
        }
        ++c;
        CxxTokenizer::readSpecifiedToken(f, ":", c, pe);
        prefix = CxxTokenizer::readString(c, pe);
        CxxTokenizer::readSpecifiedToken(f, ";", c, pe);
      } else if (c->value == "suffix") {
        if (!suffix.empty()) {
          error("library suffix multiply defined");
        }
        ++c;
        CxxTokenizer::readSpecifiedToken(f, ":", c, pe);
        suffix = CxxTokenizer::readString(c, pe);
        CxxTokenizer::readSpecifiedToken(f, ";", c, pe);
      } else if (c->value == "type") {
        ++c;
        if (btype) {
          error("library type already defined");
        }
        CxxTokenizer::readSpecifiedToken(f, ":", c, pe);
        CxxTokenizer::checkNotEndOfLine(f, c, pe);
        if (c->value == "SHARED_LIBRARY") {
          type = LibraryDescription::SHARED_LIBRARY;
          btype = true;
        } else if (c->value == "MODULE") {
          type = LibraryDescription::MODULE;
          btype = true;
        } else {
          error("unsupported library type '" + c->value + "'");
        }
        ++c;
        CxxTokenizer::readSpecifiedToken(f, ";", c, pe);
      } else if (c->value == "sources") {
        ++c;
        get_vector(sources, c, pe, "sources");
      } else if (c->value == "cppflags") {
        ++c;
        get_vector(cppflags, c, pe, "cppflags");
      } else if (c->value == "include_directories") {
        ++c;
        get_vector(include_directories, c, pe, "include_directories");
      } else if (c->value == "ldflags") {
        ++c;
        get_vector(ldflags, c, pe, "ldflags");
      } else if (c->value == "link_directories") {
        ++c;
        get_vector(link_directories, c, pe, "link_directories");
      } else if (c->value == "link_libraries") {
        ++c;
        get_vector(link_libraries, c, pe, "link_libraries");
      } else if (c->value == "epts") {
        ++c;
        get_vector(epts, c, pe, "epts");
      } else if (c->value == "deps") {
        ++c;
        get_vector(deps, c, pe, "deps");
      } else {
        error("unsupported entry type '" + c->value + "'");
      }
    }
    CxxTokenizer::readSpecifiedToken(f, "}", c, pe);
    if (!btype) {
      error("library type undefined");
    }
    LibraryDescription l{name, prefix, suffix, type};
    std::swap(l.sources, sources);
    std::swap(l.cppflags, cppflags);
    std::swap(l.include_directories, include_directories);
    std::swap(l.ldflags, ldflags);
    std::swap(l.link_directories, link_directories);
    std::swap(l.link_libraries, link_libraries);
    std::swap(l.epts, epts);
    std::swap(l.deps, deps);
    p = c;
    return l;
  }

}  // end of namespace mfront
