/*!
 * \file   physical_constants.cxx
 * \brief
 * \author Thomas Helfer
 * \date 18/08/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

// g++ physical_constants.cxx -o physical_constants `tfel-config
// --compiler-flags --includes --libs --utilities`
// ./physical_constants
// cp PhysicalConstants-5.2.0.hxx PhysicalConstants-6.0.0.hxx
// PhysicalConstants.hxx ../../include/TFEL/
// cp PhysicalConstants-python.cxx
// ../../bindings/python/tfel/PhysicalConstants.cxx
// cp TFELPHYSICALCONSTANTS.INC ../../bindings/fortran77/
// cp tfel_physical_constants.f90 ../../bindings/fortran90/
// cp tfel-physical-constants.md ../../docs/web/
//
// A header is generated for each compatibility mode declared in the
// physical_constants.json file, together with the PhysicalConstants.hxx
// header, which includes all of those headers and defines the aliases of the
// values of the current version of TFEL and those of the next major version.
//
// The compatibility modes are declared in increasing order in the
// physical_constants.json file. The values of the last compatibility mode
// are those of the next major version of TFEL, which are not used yet: the
// values of the second last compatibility mode are those of the current
// version of TFEL, i.e. those exposed by the names without suffix. Adding a
// compatibility mode to the physical_constants.json file is the way of
// taking into account a change in the values of the physical constants.
//
// The language bindings which are not versioned, such as the `Fortran`
// ones, use the values of the next major version. The values of the next
// major version shall be defined in a compatibility mode whose major version
// is greater than that of the compatibility mode of the current version,
// since the `Python` bindings select the values to use with the
// `TFEL_VERSION_MAJOR` preprocessor variable only.

#include <iostream>

#include <stdexcept>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cstring>
#include <limits>
#include <string>
#include <vector>
#include <array>

#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"

// a compatibility mode, i.e. a version of TFEL which introduced a change in
// the values of the physical constants
struct CompatibilityMode {
  // version of TFEL, given as `<major>.<minor>.<patch>`
  std::string version;
  // description of the compatibility mode, used in the `Python` bindings
  std::string description;
};

// compatibility modes, in increasing order. The values of the last
// compatibility mode are those of the next major version of TFEL.
static std::vector<CompatibilityMode> modes;

struct Constant {
  std::string name;
  std::string short_name;
  std::string description;
  std::string unit;
  bool has_qt_units = false;
  std::array<int, 7> qt_units;
  // value of the constant in the compatibility mode of the next major version
  // of TFEL, which is the most precise value. This is the value used by the
  // language bindings which are not versioned, such as the `Fortran` ones.
  long double value;
  // value of the constant in each compatibility mode: values[i] is the value
  // of the constant in modes[i]
  std::vector<long double> values;

  // \brief sets the value of the constant in a given compatibility mode
  // \param[in] mode: version of the compatibility mode
  // \param[in] v: value of the constant in this compatibility mode
  void set_value(const std::string& mode, const long double v) {
    const auto p = std::find_if(
        modes.begin(), modes.end(),
        [&mode](const CompatibilityMode& m) { return m.version == mode; });
    if (p == modes.end()) {
      throw(std::runtime_error("extract: unknown compatibility mode '" + mode +
                               "' for constant '" + this->name + "'"));
    }
    this->values[static_cast<std::size_t>(p - modes.begin())] = v;
  }
};

// \brief version of TFEL, as read from the version of a compatibility mode
struct Version {
  int major_number = 0;
  int minor_number = 0;
  int patch_number = 0;
};

static Version read_version(const std::string& v) {
  auto numbers = std::vector<int>{};
  auto ss = std::stringstream{v};
  auto number = std::string{};
  while (std::getline(ss, number, '.')) {
    numbers.push_back(std::stoi(number));
  }
  if (numbers.size() != 3u) {
    throw(std::runtime_error("version '" + v +
                             "' expected to be given as "
                             "'<major>.<minor>.<patch>'"));
  }
  auto r = Version{};
  r.major_number = numbers[0];
  r.minor_number = numbers[1];
  r.patch_number = numbers[2];
  return r;
}  // end of read_version

static bool is_before(const Version& a, const Version& b) {
  return std::tie(a.major_number, a.minor_number, a.patch_number) <
         std::tie(b.major_number, b.minor_number, b.patch_number);
}  // end of is_before

// \brief suffix used in the names related to a compatibility mode, such as
// `v5_2_0` for the `5.2.0` compatibility mode
static std::string get_namespace_suffix(const CompatibilityMode& m) {
  return "v" + tfel::utilities::replace_all(m.version, ".", "_");
}

// \brief name of the header of a compatibility mode
static std::string get_header_name(const CompatibilityMode& m) {
  return "PhysicalConstants-" + m.version;
}

// \brief part of the include guard of the header of a compatibility mode
static std::string get_include_guard_suffix(const CompatibilityMode& m) {
  return tfel::utilities::replace_all(m.version, ".", "_");
}

// \brief index of the compatibility mode of the next major version of TFEL
// \return the index of the last compatibility mode
static std::size_t get_next_mode_index() {
  return modes.size() - 1u;
}  // end of get_next_mode_index

// \brief index of the compatibility mode of the current version of TFEL,
// i.e. the compatibility mode whose values are those exposed by the names
// without suffix
// \return the index of the second last compatibility mode, or the index of
// the only compatibility mode if the next major version is not defined
static std::size_t get_current_mode_index() {
  return (modes.size() == 1u) ? 0u : modes.size() - 2u;
}  // end of get_current_mode_index

// \brief check if the values of the next major version differ from those of
// the current version of TFEL
static bool next_mode_is_different() {
  return get_current_mode_index() != get_next_mode_index();
}

// \brief check the consistency of the compatibility modes
static void check_modes() {
  if (modes.empty()) {
    throw(std::runtime_error(
        "extract: at least one compatibility mode is expected"));
  }
  for (std::size_t i = 1u; i != modes.size(); ++i) {
    if (not is_before(read_version(modes[i - 1u].version),
                      read_version(modes[i].version))) {
      throw(std::runtime_error("compatibility mode '" + modes[i].version +
                               "' shall be greater than compatibility mode '" +
                               modes[i - 1u].version + "'"));
    }
  }
  if (next_mode_is_different()) {
    const auto& current = read_version(modes[get_current_mode_index()].version);
    const auto& next = read_version(modes[get_next_mode_index()].version);
    if (next.major_number <= current.major_number) {
      throw(std::runtime_error(
          "the values of the next major version of TFEL shall be defined in a "
          "compatibility mode whose major version is greater than that of the "
          "compatibility mode of the current version of TFEL"));
    }
  }
}  // end of check_modes

/*!
 * \brief extract a compatibility mode
 * \param[in,out] p: position of the opening brace of the record
 * \param[in] pe: end of the file
 */
static CompatibilityMode extract_mode(
    tfel::utilities::CxxTokenizer::const_iterator& p,
    const tfel::utilities::CxxTokenizer::const_iterator pe) {
  using tfel::utilities::CxxTokenizer;
  auto throw_if = [](const bool c, const std::string& m) {
    if (c) {
      throw(std::runtime_error("extract: " + m));
    }
  };
  auto read_token = [throw_if, &p, pe](const std::string& v) {
    CxxTokenizer::checkNotEndOfLine("extract", p, pe);
    throw_if(p->value != v, "expected '" + v + "', read '" + p->value + "'");
    ++p;
  };
  auto read_tokens = [read_token](const std::vector<std::string>& values) {
    for (const auto& v : values) {
      read_token(v);
    }
  };
  auto m = CompatibilityMode{};
  read_tokens({"{", "mode", ":"});
  m.version = CxxTokenizer::readString(p, pe);
  read_token(";");
  CxxTokenizer::checkNotEndOfLine("extract", p, pe);
  if (p->value == "description") {
    read_tokens({"description", ":"});
    m.description = CxxTokenizer::readString(p, pe);
    read_token(";");
  }
  read_token("}");
  return m;
}  // end of extract_mode

// \brief check if a record describes a compatibility mode
// \param[in] p: position of the opening brace of the record
// \param[in] pe: end of the file
static bool is_mode(const tfel::utilities::CxxTokenizer::const_iterator p,
                    const tfel::utilities::CxxTokenizer::const_iterator pe) {
  auto q = p;
  if ((++q) == pe) {
    return false;
  }
  return q->value == "mode";
}  // end of is_mode

/*!
 * \brief extract a physical constant
 * \param[in,out] p: position of the opening brace of the record
 * \param[in] pe: end of the file
 */
Constant extract(tfel::utilities::CxxTokenizer::const_iterator& p,
                 const tfel::utilities::CxxTokenizer::const_iterator pe) {
  using tfel::utilities::CxxTokenizer;
  auto throw_if = [](const bool c, const std::string& m) {
    if (c) {
      throw(std::runtime_error("extract: " + m));
    }
  };
  auto read_token = [throw_if, &p, pe](const std::string& v) {
    CxxTokenizer::checkNotEndOfLine("extract", p, pe);
    throw_if(p->value != v, "expected '" + v + "', read '" + p->value + "'");
    ++p;
  };
  auto read_tokens = [read_token](const std::vector<std::string>& values) {
    for (const auto& v : values) {
      read_token(v);
    }
  };
  auto c = Constant{};
  c.values.resize(modes.size());
  read_tokens({"{", "name", ":"});
  c.name = CxxTokenizer::readString(p, pe);
  read_token(";");
  CxxTokenizer::checkNotEndOfLine("extract", p, pe);
  if (p->value == "short_name") {
    read_tokens({"short_name", ":"});
    c.short_name = CxxTokenizer::readString(p, pe);
    read_token(";");
  }
  CxxTokenizer::checkNotEndOfLine("extract", p, pe);
  if (p->value == "description") {
    read_tokens({"description", ":"});
    c.description = CxxTokenizer::readString(p, pe);
    read_token(";");
  }
  CxxTokenizer::checkNotEndOfLine("extract", p, pe);
  if (p->value == "unit") {
    read_tokens({"unit", ":"});
    c.unit = CxxTokenizer::readString(p, pe);
    read_token(";");
  }
  if (p->value == "qt_unit") {
    read_tokens({"qt_unit", ":", "["});
    c.has_qt_units = true;
    for (unsigned short i = 0; i != 7;) {
      c.qt_units[i] = CxxTokenizer::readInt(p, pe);
      if (++i != 7) {
        read_token(",");
      }
    }
    read_tokens({"]", ";"});
  }
  read_tokens({"value", ":"});
  CxxTokenizer::checkNotEndOfLine("extract", p, pe);
  c.value = std::stold(p->value);
  ++p;
  read_token(";");
  // the values of the compatibility modes which do not define their own
  // values are those of the next major version
  std::fill(c.values.begin(), c.values.end(), c.value);
  CxxTokenizer::checkNotEndOfLine("extract", p, pe);
  if (p->value == "values") {
    read_tokens({"values", ":", "["});
    for (;;) {
      read_token("{");
      read_tokens({"mode", ":"});
      const auto mode = CxxTokenizer::readString(p, pe);
      read_token(";");
      read_tokens({"value", ":"});
      CxxTokenizer::checkNotEndOfLine("extract", p, pe);
      c.set_value(mode, std::stold(p->value));
      ++p;
      read_token(";");
      read_token("}");
      CxxTokenizer::checkNotEndOfLine("extract", p, pe);
      if (p->value == "]") {
        break;
      }
      read_token(",");
      CxxTokenizer::checkNotEndOfLine("extract", p, pe);
      if (p->value == "]") {
        break;
      }
    }
    read_tokens({"]", ";"});
  }
  read_token("}");
  return c;
}  // end of extract

std::vector<Constant> extract(const std::string& f) {
  tfel::utilities::CxxTokenizer t(f);
  t.stripComments();
  auto p = t.begin();
  const auto pe = t.end();
  std::vector<Constant> r;
  // the compatibility modes are declared before the constants
  while ((p != pe) && (p->value == "{") && is_mode(p, pe)) {
    modes.push_back(extract_mode(p, pe));
  }
  check_modes();
  while (p != pe) {
    r.push_back(extract(p, pe));
  }
  return r;
}  // end of extract

// \brief write the comment at the beginning of a generated file
// \param[in] os: output stream
// \param[in] file_name: name of the generated file
// \param[in] brief: \brief field of the comment, which may be empty and may
// span several lines
static void write_header_comment(std::ostream& os,
                                 const std::string& file_name,
                                 const std::string& brief) {
  os << "/*!\n"
     << " * \\file   " << file_name << "\n"
     << " * \\brief" << (brief.empty() ? "" : " ") << brief << "\n";
  if (not brief.empty()) {
    os << " *\n";
  }
  os << " * \\author Thomas Helfer\n"
     << " * \\date 18/08/2017\n"
     << " * \\copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights\n"
     << " * reserved.\n"
     << " * This project is publicly released under either the GNU GPL "
        "Licence with\n"
     << " * linking exception or the CECILL-A licence. A copy of thoses "
        "licences are\n"
     << " * delivered with the sources of TFEL. CEA or EDF may also distribute "
        "this\n"
     << " * project under specific licensing conditions.\n"
     << " */\n";
}  // end of write_header_comment

/*!
 * \brief writes the comment which describes a constant
 * \param[in] os: output stream
 * \param[in] c: constant
 * \param[in] prefix: prefix of the comment, such as `//!` or `C>`
 */
static void write_comment(std::ostream& os,
                          const Constant& c,
                          const std::string& prefix) {
  if ((c.description.empty()) && (c.unit.empty())) {
    return;
  }
  using tfel::utilities::replace_all;
  os << prefix;
  if (!c.description.empty()) {
    os << ' ' << c.description;
  }
  if (!c.unit.empty()) {
    // the units of the physical_constants.json file use a tex notation,
    // such as `mol^{-1}`, which is simplified here
    os << " (" << replace_all(replace_all(c.unit, "^{", ""), "}", "") << ')';
  }
  os << '\n';
}  // end of write_comment

/*!
 * \brief generates the header of a compatibility mode
 * \param[in] cs: constants
 * \param[in] i: index of the compatibility mode
 */
static void generate_cxx(const std::vector<Constant>& cs, const std::size_t i) {
  const auto& m = modes[i];
  const auto ns = "tfel::" + get_namespace_suffix(m);
  const auto guard =
      "LIB_TFEL_PHYSICALCONSTANTS_" + get_include_guard_suffix(m) + "_HXX";
  // the values of the compatibility mode of the next major version of TFEL
  // are not those used by the current version of TFEL: only the values of
  // the already published compatibility modes are described as those which
  // are used up to the corresponding version
  const auto brief = (i == get_next_mode_index())
                         ? std::string{}
                         : "This file corresponds to the value of the "
                           "constants used up to\n * version " +
                               m.version;
  auto os = std::ofstream((get_header_name(m) + ".hxx").c_str());
  os.precision(std::numeric_limits<double>::max_digits10);
  write_header_comment(os, "include/TFEL/" + get_header_name(m) + ".hxx",
                       brief);
  os << "\n"
     << "#ifndef " << guard << "\n"
     << "#define " << guard << "\n"
     << "\n"
     << "#include\"TFEL/Config/TFELConfig.hxx\"\n"
     << "#include\"TFEL/Math/qt.hxx\"\n"
     << "\n"
     << "namespace " << ns << "{\n"
     << "\n"
     << "  template<typename NumericType = double, "
     << "           bool use_quantities = false>\n"
     << "  struct PhysicalConstants;\n\n"
     << "/*!\n"
     << " * \\brief partial specialisation when quantities are used"
     << " */\n"
     << "  template<typename NumericType>\n"
     << "  struct PhysicalConstants<NumericType, true> {\n"
     << "//! a simple alias\n"
     << "template<int N1, int N2, int N3, int N4, int N5, int N6, int N7>\n"
     << "using quantity = "
     << "tfel::math::qt<typename tfel::math::unit::UnitRebind<"
        "tfel::math::unit::makeUnitExponents<N1, N2, N3, N4, N5, N6, N7>()>::"
        "type, NumericType>;\n";
  for (const auto& c : cs) {
    write_comment(os, c, "//!");
    const auto type = [&c]() -> std::string {
      if (c.has_qt_units) {
        auto t = std::string();
        t += "quantity<";
        for (unsigned short j = 0; j != 7;) {
          t += std::to_string(c.qt_units[j]);
          if (++j != 7) {
            t += ", ";
          }
        }
        t += ">";
        return t;
      }
      return "NumericType";
    }();
    os << "static constexpr auto " << c.name << " = " << type << "("
       << c.values[i] << ");\n";
    if (!c.short_name.empty()) {
      write_comment(os, c, "//!");
      os << "static constexpr auto " << c.short_name << "= " << type << "("
         << c.values[i] << ");\n";
    }
  }
  os << "  }; // end of PhysicalConstants\n"
     << "/*!\n"
     << " * \\brief partial specialisation when quantities are not used"
     << " */\n"
     << "  template<typename NumericType>\n"
     << "  struct PhysicalConstants<NumericType, false> {\n";
  for (const auto& c : cs) {
    write_comment(os, c, "//!");
    os << "static constexpr auto " << c.name << " = NumericType(" << c.values[i]
       << ");\n";
    if (!c.short_name.empty()) {
      write_comment(os, c, "//!");
      os << "static constexpr auto " << c.short_name << "= NumericType("
         << c.values[i] << ");\n";
    }
  }
  os << "  }; // end of PhysicalConstants\n"
     << "\n"
     << "} // end of namespace " << ns << "\n\n"
     << "namespace tfel::constants::" << get_namespace_suffix(m) << "{\n\n";
  for (const auto& c : cs) {
    write_comment(os, c, "//!");
    os << "template<typename NumericType = double, bool use_quantities = "
          "false>\n"
       << "inline constexpr auto " << c.name << " = " << ns
       << "::PhysicalConstants<NumericType, use_quantities>::" << c.name
       << ";\n";
    if (!c.short_name.empty()) {
      write_comment(os, c, "//!");
      os << "template<typename NumericType = double, bool use_quantities = "
            "false>\n"
         << "inline constexpr auto " << c.short_name << " = " << ns
         << "::PhysicalConstants<NumericType, use_quantities>::" << c.short_name
         << ";\n";
    }
  }
  os << "\n"
     << "} // end of namespace tfel::constants::" << get_namespace_suffix(m)
     << "\n\n"
     << "#endif /* " << guard << " */\n";
}  // end of generate_cxx

/*!
 * \brief generates the PhysicalConstants.hxx header, which includes the
 * header of each compatibility mode and defines the aliases of the values of
 * the current version of TFEL and those of the next major version
 */
static void generate_cxx_default() {
  const auto current_ns = get_namespace_suffix(modes[get_current_mode_index()]);
  const auto next_ns = get_namespace_suffix(modes[get_next_mode_index()]);
  std::ofstream os("PhysicalConstants.hxx");
  write_header_comment(os, "include/TFEL/PhysicalConstants.hxx", "");
  os << "\n"
     << "#ifndef LIB_TFEL_PHYSICALCONSTANTS_HXX\n"
     << "#define LIB_TFEL_PHYSICALCONSTANTS_HXX\n"
     << "\n";
  for (const auto& m : modes) {
    os << "#include\"TFEL/" << get_header_name(m) << ".hxx\"\n";
  }
  os << "\n"
     << "namespace tfel{\n"
     << "\n"
     << "  using tfel::" << current_ns << "::PhysicalConstants;\n"
     << "\n"
     << "} // end of namespace tfel\n"
     << "\n"
     << "namespace tfel::constants{\n"
     << "\n"
     << "  using namespace tfel::constants::" << current_ns << ";\n"
     << "\n"
     << "} // end of namespace tfel::constants\n";
  if (next_mode_is_different()) {
    os << "\n"
       << "namespace tfel::future{\n"
       << "\n"
       << "  using tfel::" << next_ns << "::PhysicalConstants;\n"
       << "\n"
       << "} // end of namespace tfel::future\n"
       << "\n"
       << "namespace tfel::constants::future{\n"
       << "\n"
       << "  using namespace tfel::constants::" << next_ns << ";\n"
       << "\n"
       << "} // end of namespace tfel::constants::future\n";
  }
  os << "\n"
     << "#endif /* LIB_TFEL_PHYSICALCONSTANTS_HXX */\n";
}  // end of generate_cxx_default

/*!
 * \brief generates the class which holds the values of a compatibility mode
 * in the `Python` bindings
 * \param[in] os: output stream
 * \param[in] cs: constants
 * \param[in] i: index of the compatibility mode
 */
static void generate_python_class(std::ostream& os,
                                  const std::vector<Constant>& cs,
                                  const std::size_t i) {
  const auto name = "PhysicalConstants_" + get_namespace_suffix(modes[i]);
  os << "struct TFEL_VISIBILITY_LOCAL " << name << "{\n";
  for (const auto& c : cs) {
    os << "static const double " << c.name << ";\n";
  }
  os << "};\n\n";
  for (const auto& c : cs) {
    os << "const double " << name << "::" << c.name << " = double("
       << c.values[i] << ");\n";
  }
  os << "\n";
}  // end of generate_python_class

/*!
 * \brief generates the function which declares the constants of a
 * compatibility mode in the `Python` bindings
 * \param[in] os: output stream
 * \param[in] cs: constants
 * \param[in] i: index of the compatibility mode
 */
static void generate_python_declaration(std::ostream& os,
                                        const std::vector<Constant>& cs,
                                        const std::size_t i) {
  const auto name = "PhysicalConstants_" + get_namespace_suffix(modes[i]);
  os << "static void declare" << name << "(pybind11::module_& m)\n"
     << "{\n"
     << "  pybind11::class_<" << name << "> w(m, \"PhysicalConstants\", "
     << "\"Physical constants\");\n"
     << "  w";
  for (const auto& c : cs) {
    os << ".def_readonly_static(\"" << c.name << "\", &" << name
       << "::" << c.name << ")\n";
    if (!c.short_name.empty()) {
      os << ".def_readonly_static(\"" << c.short_name << "\", &" << name
         << "::" << c.name << ")\n";
    }
  }
  os << ";\n\n"
     << "} // end of declare" << name << "\n\n";
}  // end of generate_python_declaration

/*!
 * \brief generates the `Python` bindings
 * \param[in] cs: constants
 */
static void generate_python(const std::vector<Constant>& cs) {
  const auto current_name =
      "PhysicalConstants_" +
      get_namespace_suffix(modes[get_current_mode_index()]);
  const auto next_name =
      "PhysicalConstants_" + get_namespace_suffix(modes[get_next_mode_index()]);
  const auto next_major_number =
      read_version(modes[get_next_mode_index()].version).major_number;
  std::ofstream os("PhysicalConstants-python.cxx");
  os.precision(std::numeric_limits<double>::max_digits10);
  write_header_comment(os, "bindings/python/tfel/PhysicalConstants.cxx", "");
  os << "\n"
     << "#include <pybind11/pybind11.h>\n"
     << "#include <pybind11/stl.h>\n"
     << "#include\"TFEL/Config/TFELConfig.hxx\"\n"
     << "\n"
     << "#ifndef TFEL_VERSION_MAJOR\n"
     << "#error \"TFEL_VERSION_MAJOR must be defined\"\n"
     << "#endif\n"
     << "\n"
     << "void declarePhysicalConstants(pybind11::module_& m);\n"
     << "\n";
  for (std::size_t i = 0; i != modes.size(); ++i) {
    generate_python_class(os, cs, i);
  }
  for (std::size_t i = 0; i != modes.size(); ++i) {
    generate_python_declaration(os, cs, i);
  }
  if (next_mode_is_different()) {
    os << "struct PhysicalConstants_future : " << next_name << " {};\n\n"
       << "#if TFEL_VERSION_MAJOR < " << next_major_number << "\n"
       << "struct PhysicalConstants : " << current_name << " {};\n"
       << "#else\n"
       << "struct PhysicalConstants : " << next_name << " {};\n"
       << "#endif\n\n";
  } else {
    os << "struct PhysicalConstants : " << current_name << " {};\n\n";
  }
  os << "void declarePhysicalConstants(pybind11::module_& m)\n"
     << "{\n";
  for (const auto& m : modes) {
    const auto n = get_namespace_suffix(m);
    os << "  auto m_" << n << " =\n"
       << "      m.def_submodule(\"" << n << "\", \"" << m.description
       << "\");\n";
  }
  for (std::size_t i = 0; i != modes.size(); ++i) {
    const auto n = get_namespace_suffix(modes[i]);
    os << "  declarePhysicalConstants_" << n << "(m_" << n << ");\n";
  }
  if (next_mode_is_different()) {
    os << "  //\n"
       << "  auto m_future = m.def_submodule(\n"
       << "      \"future\", \"Compatibility module with unpublished versions "
          "of TFEL.\");\n"
       << "  pybind11::class_<PhysicalConstants_future, " << next_name
       << "> w_future(\n"
       << "      m_future, \"PhysicalConstants\", \"Physical constants\");\n"
       << "#if TFEL_VERSION_MAJOR < " << next_major_number << "\n"
       << "  pybind11::class_<PhysicalConstants, " << current_name
       << "> w(m, \"PhysicalConstants\", \"Physical constants\");\n"
       << "#else\n"
       << "  pybind11::class_<PhysicalConstants, " << next_name
       << "> w(m, \"PhysicalConstants\", \"Physical constants\");\n"
       << "#endif\n";
  } else {
    os << "  pybind11::class_<PhysicalConstants, " << current_name
       << "> w(m, \"PhysicalConstants\", \"Physical constants\");\n";
  }
  os << "} // end of declarePhysicalConstants\n\n";
}  // end of generate_python

static void generate_fortran77(const std::vector<Constant>& cs) {
  auto upper = [](const std::string& n) {
    std::string r(n);
    std::transform(r.begin(), r.end(), r.begin(),
                   [](const char c) { return std::toupper(c); });
    return r;
  };
  std::ofstream os("TFELPHYSICALCONSTANTS.INC");
  os << "C> \\file   bindings/fortran77/TFELPHYSICALCONSTANTS.cxx\n"
     << "C> \\brief\n"
     << "C> \\author Thomas Helfer\n"
     << "C> \\date 18/08/2017\n"
     << "C> \\copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights \n"
     << "C> reserved. \n"
     << "C> This project is publicly released under either the GNU GPL Licence "
        "with linking exception "
        "\n"
     << "C> or the CECILL-A licence. A copy of thoses licences are delivered \n"
     << "C> with the sources of TFEL. CEA or EDF may also distribute this \n"
     << "C> project under specific licensing conditions.\n\n";
  for (const auto& c : cs) {
    write_comment(os, c, "C>");
    os << "      DOUBLE PRECISION CSTE_" << upper(c.name) << "\n";
    if (!c.short_name.empty()) {
      write_comment(os, c, "C>");
      os << "      DOUBLE PRECISION CSTE_" << upper(c.short_name) << "\n";
    }
  }
  os << "*\n";
  for (const auto& c : cs) {
    auto convert = [](const double v) {
      auto append_if = [](const std::string& f) {
        if (f.find('D') == std::string::npos) {
          return f + "D0";
        }
        return f;
      };
      using tfel::utilities::replace_all;
      std::ostringstream converter;
      converter.precision(std::numeric_limits<double>::max_digits10);
      converter << v;
      return append_if(
          replace_all(replace_all(converter.str(), 'e', 'D'), 'E', 'D'));
    };
    os << "      PARAMETER (CSTE_" << upper(c.name) << " = "
       << convert(double(c.value)) << ");\n";
    if (!c.short_name.empty()) {
      os << "      PARAMETER (CSTE_" << upper(c.short_name) << " = "
         << convert(double(c.value)) << ");\n";
    }
  }
}  // end of generate_fortran77

static void generate_fortran90(const std::vector<Constant>& cs) {
  auto upper = [](const std::string& n) {
    std::string r(n);
    std::transform(r.begin(), r.end(), r.begin(),
                   [](const char c) { return std::toupper(c); });
    return r;
  };
  const auto t = std::string{"  "};
  std::ofstream os("tfel_physical_constants.f90");
  os << "!> \\file   bindings/fortran90/TFELPHYSICALCONSTANTS.mod\n"
     << "!> \\brief  physical constants from CODATA Internationally "
        "recommended values,\n"
     << "!           http://physics.nist.gov/cuu/Constants/index.html\n"
     << "!> \\author Thomas Helfer\n"
     << "!> \\date 18/08/2017\n"
     << "!> \\copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights \n"
     << "!  reserved. \n"
     << "!  This project is publicly released under either the GNU GPL Licence "
        "with linking exception "
        "\n"
     << "!  or the CECILL-A licence. A copy of thoses licences are delivered \n"
     << "!  with the sources of TFEL. CEA or EDF may also distribute this \n"
     << "!  project under specific licensing conditions.\n\n"
     << "module TFEL_PHYSICAL_CONSTANTS\n"
     << t << "use,intrinsic :: ISO_FORTRAN_ENV\n"
     << t << "implicit none\n";
  for (const auto& c : cs) {
    auto convert = [](const double v) {
      using tfel::utilities::replace_all;
      auto append_if = [](const std::string& f) {
        if (f.find('D') == std::string::npos) {
          return f + "D0";
        }
        return f;
      };
      std::ostringstream converter;
      converter.precision(std::numeric_limits<double>::max_digits10);
      converter << v;
      return append_if(
          replace_all(replace_all(converter.str(), 'e', 'D'), 'E', 'D'));
    };
    write_comment(os, c, t + "!>");
    os << t << "real (real64), parameter :: " << upper(c.name) << " = "
       << convert(double(c.value)) << "\n";
    if (!c.short_name.empty()) {
      write_comment(os, c, t + "!>");
      os << t << "real (real64), parameter :: " << upper(c.short_name) << " = "
         << convert(double(c.value)) << "\n";
    }
  }
  os << "endmodule TFEL_PHYSICAL_CONSTANTS\n";
}  // end of generate_fortran90

static void generate_pandoc(const std::vector<Constant>& cs) {
  std::ofstream os("tfel-physical-constants.md");
  os << "---\n"
     << "title: The `TFEL/PhysicalConstants` library\n"
     << "author: Thomas Helfer\n"
     << "date: 17/08/2021\n"
     << "lang: en-EN\n"
     << "link-citations: true\n"
     << "colorlinks: true\n"
     << "figPrefixTemplate: \"$$i$$\"\n"
     << "tblPrefixTemplate: \"$$i$$\"\n"
     << "secPrefixTemplate: \"$$i$$\"\n"
     << "eqnPrefixTemplate: \"($$i$$)\"\n"
     << "---\n"
     << "\n"
     << "# Description of the library\n\n"
     << "The `TFEL/PhysicalConstants` library is header-only which "
     << "provides:\n\n"
     << "- A template class called `PhysicalConstants` in the namespace "
     << "`tfel`. This class defined a constexpr static data member per "
     << "physical constant. This class has two template parameters:\n"
     << "    - `NumericType`: which is the floatting point number type\n"
     << "      to be used to define the physical constants.\n"
     << "    - `use_quantities`: which states if quantities (i.e. numbers\n"
     << "       with units) shall be used.\n"
     << "      to be used to define the physical constants.\n"
     << "- A set of template constexpr inline variables defined in the\n"
     << "  namespace `tfel::constants`. Those variables have the same\n"
     << "  template parameters than the `PhysicalConstants class`\n"
     << "\n"
     << "The values of the physical constants are extracted from: "
     << "<http://physics.nist.gov/cuu/Constants/index.html>\n"
     << "\n"
     << "The values of the physical constants depend on the compatibility\n"
     << "modes: those of the current version of `TFEL` are those exposed by\n"
     << "the names without suffix, whereas those of the next major version\n"
     << "are those of the `tfel::future` and `tfel::constants::future`\n"
     << "namespaces, as described in the page devoted to\n"
     << "[compatibility modes in the `TFEL` libraries]"
     << "(tfel-compatibility-modes.html).\n"
     << "\n"
     << "## Bindings\n\n"
     << "### `Fortran` bindings\n"
     << "\n"
     << "A include file named `TFELPHYSICALCONSTANTS.INC` is provided for "
     << "`fortran77`. This include declares the physical constants as `real64` "
     << "parameters.\n"
     << "\n"
     << "A module named `TFEL_PHYSICAL_CONSTANTS` is provided for "
     << "`fortran95`. This module exposes the physical constants as `real64` "
     << "parameters.\n"
     << "\n"
     << "The `Fortran` bindings are not versioned: they expose the values of\n"
     << "the next major version of `TFEL`.\n"
     << "\n"
     << "### `Python` bindings\n"
     << "\n"
     << "The `tfel` module exposes the class `PhysicalConstants` which "
     << "declares the physical constants as read-only attributes. The "
     << "constants of a given compatibility mode are exposed by a submodule "
     << "of the same name.\n"
     << "\n"
     << "# Physical constants\n"
     << "\n";
  for (const auto& c : cs) {
    os << "- " << c.name;
    if ((!c.short_name.empty()) || (!c.unit.empty())) {
      os << " (";
      if (!c.short_name.empty()) {
        os << c.short_name;
        if (!c.unit.empty()) {
          os << ", \\(" << c.unit << "\\)";
        }
      } else {
        os << ", \\(" << c.unit << "\\)";
      }
      os << ")";
    }
    if (!c.description.empty()) {
      os << ": " << c.description;
    }
    os << '\n';
  }
}  // end of generate_pandoc

int main() {
  try {
    const auto cs = extract("physical_constants.json");
    for (std::size_t i = 0; i != modes.size(); ++i) {
      generate_cxx(cs, i);
    }
    generate_cxx_default();
    generate_python(cs);
    generate_fortran77(cs);
    generate_fortran90(cs);
    generate_pandoc(cs);
  } catch (const std::exception& e) {
    std::cerr << "physical_constants: " << e.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
