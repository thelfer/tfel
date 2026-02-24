/*!
 * \file   physical_constants.cxx
 * \brief
 * \author Thomas Helfer
 * \date   18 août 2017
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
// cp PhysicalConstants.hxx ../../include/
// cp PhysicalConstants-python.cxx
// ../../bindings/python/tfel/PhysicalConstants.cxx

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

struct Constant {
  std::string name;
  std::string short_name;
  std::string description;
  std::string unit;
  bool has_qt_units = false;
  std::array<int, 7> qt_units;
  long double value;
};

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
  read_token("}");
  return c;
};

std::vector<Constant> extract(const std::string& f) {
  tfel::utilities::CxxTokenizer t(f);
  t.stripComments();
  auto p = t.begin();
  const auto pe = t.end();
  std::vector<Constant> r;
  while (p != pe) {
    r.push_back(extract(p, pe));
  }
  return r;
}  // end of extract

static void generate_cxx(const std::vector<Constant>& cs) {
  auto write_comments = [](std::ostream& os, const Constant& c) {
    if ((!c.description.empty()) || (!c.unit.empty())) {
      os << "//!";
      if (!c.description.empty()) {
        os << ' ' << c.description;
      }
      if (!c.unit.empty()) {
        os << " (" << c.unit << ')';
      }
      os << '\n';
    }
  };
  std::ofstream os("PhysicalConstants.hxx");
  os << "/*!\n"
     << " * \\file   include/TFEL/PhysicalConstants.hxx\n"
     << " * \\brief\n"
     << " * \\author Thomas Helfer\n"
     << " * \\date   18 août 2017\n"
     << " * \\copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights\n"
     << " * reserved.\n"
     << " * This project is publicly released under either the GNU GPL "
        "Licence\n"
     << " * or the CECILL-A licence. A copy of thoses licences are delivered\n"
     << " * with the sources of TFEL. CEA or EDF may also distribute this\n"
     << " * project under specific licensing conditions.\n"
     << " */\n"
     << "\n"
     << "#ifndef LIB_TFEL_PHYSICALCONSTANTS_HXX\n"
     << "#define LIB_TFEL_PHYSICALCONSTANTS_HXX\n"
     << "\n"
     << "#include\"TFEL/Config/TFELConfig.hxx\"\n"
     << "#include\"TFEL/Math/qt.hxx\"\n"
     << "\n"
     << "namespace tfel{\n"
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
     << "tfel::math::qt<typename tfel::math::GenerateUnit<N1, N2, N3, N4, N5, "
        "N6, N7>::type, NumericType>;\n";
  for (const auto& c : cs) {
    write_comments(os, c);
    const auto type = [&c]() -> std::string {
      if (c.has_qt_units) {
        auto t = std::string();
        t += "quantity<";
        for (unsigned short i = 0; i != 7;) {
          t += std::to_string(c.qt_units[i]);
          if (++i != 7) {
            t += ", ";
          }
        }
        t += ">";
        return t;
      }
      return "NumericType";
    }();
    os << "static constexpr auto " << c.name << " = " << type << "(" << c.value
       << ");\n";
    if (!c.short_name.empty()) {
      write_comments(os, c);
      os << "static constexpr auto " << c.short_name << "= " << type << "("
         << c.value << ");\n";
    }
  }
  os << "  }; // end of PhysicalConstants\n"
     << "/*!\n"
     << " * \\brief partial specialisation when quantities are not used"
     << " */\n"
     << "  template<typename NumericType>\n"
     << "  struct PhysicalConstants<NumericType, false> {\n";
  for (const auto& c : cs) {
    write_comments(os, c);
    os << "static constexpr auto " << c.name << " = NumericType(" << c.value
       << ");\n";
    if (!c.short_name.empty()) {
      write_comments(os, c);
      os << "static constexpr auto " << c.short_name << "= NumericType("
         << c.value << ");\n";
    }
  }
  os << "  }; // end of PhysicalConstants\n"
     << "\n"
     << "} // end of namespace tfel\n\n"
     << "namespace tfel::constants{\n\n";
  for (const auto& c : cs) {
    write_comments(os, c);
    os << "template<typename NumericType = double, bool use_quantities = "
          "false>\n"
       << "inline constexpr auto " << c.name
       << " = tfel::PhysicalConstants<NumericType, use_quantities>::" << c.name
       << ";\n";
    if (!c.short_name.empty()) {
      write_comments(os, c);
      os << "template<typename NumericType = double, bool use_quantities = "
            "false>\n"
         << "inline constexpr auto " << c.short_name
         << " = tfel::PhysicalConstants<NumericType, use_quantities>::"
         << c.short_name << ";\n";
    }
  }
  os << "\n"
     << "} // end of namespace tfel::constants\n\n"
     << "#endif /* LIB_TFEL_PHYSICALCONSTANTS_HXX */\n";
}  // end of generate_cxx

static void generate_python(const std::vector<Constant>& cs) {
  std::ofstream os("PhysicalConstants-python.cxx");
  os << "/*!\n"
     << " * \\file   bindings/python/tfel/PhysicalConstants.cxx\n"
     << " * \\brief\n"
     << " * \\author Thomas Helfer\n"
     << " * \\date   18 août 2017\n"
     << " * \\copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights \n"
     << " * reserved. \n"
     << " * This project is publicly released under either the GNU GPL Licence "
        "with linking exception "
        "\n"
     << " * or the CECILL-A licence. A copy of thoses licences are delivered \n"
     << " * with the sources of TFEL. CEA or EDF may also distribute this \n"
     << " * project under specific licensing conditions. \n"
     << " */\n"
     << "\n"
     << "#include<boost/python.hpp>\n"
     << "#include\"TFEL/Config/TFELConfig.hxx\"\n"
     << "\n"
     << "void declarePhysicalConstants();\n"
     << "\n"
     << "struct TFEL_VISIBILITY_LOCAL PhysicalConstants{\n";
  for (const auto& c : cs) {
    if ((!c.description.empty()) || (!c.unit.empty())) {
      os << "//!";
      if (!c.description.empty()) {
        os << ' ' << c.description;
      }
      if (!c.unit.empty()) {
        os << " (" << c.unit << ')';
      }
      os << '\n';
    }
    os << "static const double " << c.name << ";\n";
  }
  os << "};\n\n";
  for (const auto& c : cs) {
    os << "const double PhysicalConstants::" << c.name << " = double("
       << c.value << ");\n";
  }
  os << "\n"
     << "void declarePhysicalConstants()\n"
     << "{\n"
     << "  boost::python::class_<PhysicalConstants>(\"PhysicalConstants\")\n";
  for (const auto& c : cs) {
    auto write_comments = [&os, &c] {
      if ((!c.description.empty()) || (!c.unit.empty())) {
        os << ",\n\"";
        if (!c.description.empty()) {
          os << c.description;
        }
        if (!c.unit.empty()) {
          if (!c.description.empty()) {
            os << ' ';
          }
          os << "(" << c.unit << ')';
        }
        os << '"';
      }
    };
    os << ".def_readonly(\"" << c.name << "\",PhysicalConstants::" << c.name;
    write_comments();
    os << ")\n";
    if (!c.short_name.empty()) {
      os << ".def_readonly(\"" << c.short_name
         << "\",PhysicalConstants::" << c.name;
      write_comments();
      os << ")\n";
    }
  }
  os << ";\n\n"
     << "} // end of declarePhysicalConstants\n\n";
}  // end of declarePhysicalConstants

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
     << "C> \\date   18 août 2017\n"
     << "C> \\copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights \n"
     << "C> reserved. \n"
     << "C> This project is publicly released under either the GNU GPL Licence "
        "with linking exception "
        "\n"
     << "C> or the CECILL-A licence. A copy of thoses licences are delivered \n"
     << "C> with the sources of TFEL. CEA or EDF may also distribute this \n"
     << "C> project under specific licensing conditions.\n\n";
  for (const auto& c : cs) {
    auto write_comment = [&os, &c] {
      if ((!c.description.empty()) || (!c.unit.empty())) {
        os << "C>";
        if (!c.description.empty()) {
          os << ' ' << c.description;
        }
        if (!c.unit.empty()) {
          os << " (" << c.unit << ')';
        }
        os << '\n';
      }
    };
    write_comment();
    os << "      DOUBLE PRECISION CSTE_" << upper(c.name) << "\n";
    if (!c.short_name.empty()) {
      write_comment();
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
    os << "      PARAMETER (CSTE_" << upper(c.name) << " = " << convert(c.value)
       << ");\n";
    if (!c.short_name.empty()) {
      os << "      PARAMETER (CSTE_" << upper(c.short_name) << " = "
         << convert(c.value) << ");\n";
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
  const auto t = "  ";
  std::ofstream os("tfel_physical_constants.f90");
  os << "!> \\file   bindings/fortran90/TFELPHYSICALCONSTANTS.mod\n"
     << "!> \\brief  physical constants from CODATA Internationally "
        "recommended values,\n"
     << "!           http://physics.nist.gov/cuu/Constants/index.html\n"
     << "!> \\author Thomas Helfer\n"
     << "!> \\date   18 août 2017\n"
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
    auto write_comment = [&t, &os, &c] {
      if ((!c.description.empty()) || (!c.unit.empty())) {
        os << t << "!>";
        if (!c.description.empty()) {
          os << ' ' << c.description;
        }
        if (!c.unit.empty()) {
          os << " (" << c.unit << ')';
        }
        os << '\n';
      }
    };
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
    write_comment();
    os << t << "real (real64), parameter :: " << upper(c.name) << " = "
       << convert(c.value) << "\n";
    if (!c.short_name.empty()) {
      write_comment();
      os << t << "real (real64), parameter :: " << upper(c.short_name) << " = "
         << convert(c.value) << "\n";
    }
  }
  os << "endmodule TFEL_PHYSICAL_CONSTANTS\n";
}

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
     << "### `Python` bindings\n"
     << "\n"
     << "The `tfel` module exposes the class `PhysicalConstants` which "
     << "declares the physical constants as read-only attributes.\n"
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
}

int main() {
  const auto cs = extract("physical_constants.json");
  generate_cxx(cs);
  generate_python(cs);
  generate_fortran77(cs);
  generate_fortran90(cs);
  generate_pandoc(cs);
  return EXIT_SUCCESS;
}
