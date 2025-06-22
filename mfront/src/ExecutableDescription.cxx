/*!
 * \file   mfront/src/ExecutableDescription.cxx
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
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/ExecutableDescription.hxx"

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

  void mergeExecutableDescription(ExecutableDescription& d,
                                  const ExecutableDescription& s) {
    tfel::raise_if(
        (d.name != s.name) || (d.prefix != s.prefix) || (d.suffix != s.suffix),
        "mergeExecutableDescription : "
        "can't merge description of executable' " +
            d.name +
            "' ("
            "prefix: '" +
            d.prefix + "', " + "suffix: '" + d.suffix +
            "')"
            "and description of executable '" +
            s.name +
            "' ("
            "prefix: '" +
            s.prefix + "', " + "suffix: '" + s.suffix + "')");
    if (!d.install_path.empty()) {
      if (d.install_path != s.install_path) {
        getLogStream() << "mergeExecutableDescription : "
                       << "two descriptions of executable' " << d.name
                       << "', installation paths don't match ('"
                       << d.install_path << "' vs '" << s.install_path
                       << "'), keeping '" << s.install_path << "'\n";
        d.install_path = s.install_path;
      }
    } else {
      d.install_path = s.install_path;
    }
    insert_if(d.sources, s.sources);
    insert_if(d.cppflags, s.cppflags);
    insert_if(d.include_directories, s.include_directories);
    insert_if(d.ldflags, s.ldflags);
    insert_if(d.link_directories, s.link_directories);
    insert_if(d.link_libraries, s.link_libraries);
    insert_if(d.deps, s.deps);
  }  // end of mergeExecutableDescription

  const char* ExecutableDescription::getDefaultExecutablePrefix(
      const TargetSystem) noexcept {
    return "";
  }  // end of getDefaultExecutablePrefix

  const char* ExecutableDescription::getDefaultExecutableSuffix(
      const TargetSystem s) noexcept {
    return (s == WINDOWS) ? "exe" : "";
  }  // end of ExecutableDescription::getDefaultExecutableSuffix

  ExecutableDescription::ExecutableDescription(const std::string& n,
                                               const std::string& p,
                                               const std::string& s)
      : CompiledTargetDescriptionBase(n, p, s) {
  }  // end of ExecutableDescription::ExecutableDescription

  ExecutableDescription::ExecutableDescription(const ExecutableDescription&) =
      default;
  ExecutableDescription::ExecutableDescription(ExecutableDescription&&) =
      default;
  ExecutableDescription::~ExecutableDescription() = default;

  std::ostream& operator<<(std::ostream& os, const ExecutableDescription& l) {
    os << "{\n";
    os << "name   : \"" << l.name << "\";\n";
    os << "prefix : \"" << l.prefix << "\";\n";
    os << "suffix : \"" << l.suffix << "\";\n";
    os << "install_path : \"" << l.install_path << "\";\n";
    write(os, l.sources, "sources");
    write(os, l.cppflags, "cppflags");
    write(os, l.include_directories, "include_directories");
    write(os, l.ldflags, "ldflags");
    write(os, l.link_directories, "link_directories");
    write(os, l.link_libraries, "link_libraries");
    write(os, l.deps, "deps");
    os << "};\n";
    return os;
  }

  template <>
  MFRONT_VISIBILITY_EXPORT ExecutableDescription
  read(tfel::utilities::CxxTokenizer::const_iterator& p,
       const tfel::utilities::CxxTokenizer::const_iterator pe) {
    using tfel::utilities::CxxTokenizer;
    const auto f = "read<ExecutableDescription>";
    auto error = [&f](const std::string& m) {
      tfel::raise(std::string{f} + ": " + m);
    };
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
    auto install_path = std::string{};
    // parsing
    auto c = p;
    CxxTokenizer::readSpecifiedToken(f, "{", c, pe);
    CxxTokenizer::checkNotEndOfLine(f, c, pe);
    auto get_string = [&c, pe, &error, &f](std::string& s,
                                           const char* const e) {
      if (!s.empty()) {
        error(std::string(e) + " multiply defined");
      }
      ++c;
      CxxTokenizer::readSpecifiedToken(f, ":", c, pe);
      s = CxxTokenizer::readString(c, pe);
      CxxTokenizer::readSpecifiedToken(f, ";", c, pe);
    };
    auto get_vector = [&c, pe, &error, &f](std::vector<std::string>& v,
                                           const char* const e) {
      if (!v.empty()) {
        error("executable member '" + std::string(e) + "' multiply defined");
      }
      ++c;
      CxxTokenizer::readSpecifiedToken(f, ":", c, pe);
      v = read<std::vector<std::string>>(c, pe);
      CxxTokenizer::readSpecifiedToken(f, ";", c, pe);
    };
    while (c->value != "}") {
      if (c->value == "name") {
        get_string(name, "executable name");
      } else if (c->value == "prefix") {
        get_string(prefix, "executable prefix");
      } else if (c->value == "suffix") {
        get_string(suffix, "executable suffix");
      } else if (c->value == "sources") {
        get_vector(sources, "sources");
      } else if (c->value == "cppflags") {
        get_vector(cppflags, "cppflags");
      } else if (c->value == "include_directories") {
        get_vector(include_directories, "include_directories");
      } else if (c->value == "ldflags") {
        get_vector(ldflags, "ldflags");
      } else if (c->value == "link_directories") {
        get_vector(link_directories, "link_directories");
      } else if (c->value == "link_libraries") {
        get_vector(link_libraries, "link_libraries");
      } else if (c->value == "deps") {
        get_vector(deps, "deps");
      } else if (c->value == "install_path") {
        get_string(install_path, "installation path");
      } else {
        error("unsupported entry type '" + c->value + "'");
      }
    }
    CxxTokenizer::readSpecifiedToken(f, "}", c, pe);
    ExecutableDescription e{name, prefix, suffix};
    std::swap(e.sources, sources);
    std::swap(e.cppflags, cppflags);
    std::swap(e.include_directories, include_directories);
    std::swap(e.ldflags, ldflags);
    std::swap(e.link_directories, link_directories);
    std::swap(e.link_libraries, link_libraries);
    std::swap(e.deps, deps);
    std::swap(e.install_path, install_path);
    p = c;
    return e;
  }

}  // end of namespace mfront
