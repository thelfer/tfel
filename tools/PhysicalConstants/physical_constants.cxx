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
// cp PhysicalConstants.cxx ../../src/PhysicalConstants
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

#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"

struct Constant {
  std::string name;
  std::string short_name;
  std::string description;
  std::string unit;
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
     << "\n"
     << "namespace tfel{\n"
     << "\n"
     << "#if __cplusplus >= 201703L\n\n"
     << "  template<typename real = double>\n"
     << "  struct PhysicalConstants{\n";
  for (const auto& c : cs) {
    write_comments(os, c);
    os << "static constexpr const real " << c.name << " = real(" << c.value
       << ");\n";
    if (!c.short_name.empty()) {
      write_comments(os, c);
      os << "static constexpr const real " << c.short_name << "= real("
         << c.value << ");\n";
    }
  }
  os << "  }; // end of PhysicalConstants\n"
     << "\n"
     << "namespace constants{\n\n";
  for (const auto& c : cs) {
    write_comments(os, c);
    os << "template<typename real>\n"
       << "inline constexpr const real " << c.name << " = real(" << c.value
       << ");\n";
    if (!c.short_name.empty()) {
      write_comments(os, c);
      os << "template<typename real = double>\n"
         << "inline constexpr const real " << c.short_name << " = real("
         << c.value << ");\n";
    }
  }
  os << "\n"
     << "} // end of namespace constants\n\n"
     << "#else  /* __cplusplus >= 201703L */\n\n"
     << "  template<typename real = double>\n"
     << "  struct TFELPHYSICALCONSTANTS_VISIBILITY_EXPORT PhysicalConstants{\n";
  for (const auto& c : cs) {
    write_comments(os, c);
    os << "static const real " << c.name << ";\n";
    if (!c.short_name.empty()) {
      write_comments(os, c);
      os << "static const real " << c.short_name << ";\n";
    }
  }
  os << "  }; // end of PhysicalConstants\n"
     << "#endif /* __cplusplus >= 201703L */\n\n"
     << "} // end of namespace tfel\n"
     << "\n"
     << "#endif /* LIB_TFEL_PHYSICALCONSTANTS_HXX */\n";
  os.close();
  os.open("PhysicalConstants.cxx");
  os << "/*!\n"
     << " * \\file   src/PhysicalConstants/PhysicalConstants.cxx\n"
     << " * \\brief\n"
     << " * \\author Thomas Helfer\n"
     << " * \\date   28/12/2017\n"
     << " * \\copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights\n"
     << " * reserved.\n"
     << " * This project is publicly released under either the GNU GPL "
        "Licence\n"
     << " * or the CECILL-A licence. A copy of thoses licences are delivered\n"
     << " * with the sources of TFEL. CEA or EDF may also distribute this\n"
     << " * project under specific licensing conditions.\n"
     << " */\n"
     << "\n"
     << "#include\"TFEL/PhysicalConstants.hxx\"\n"
     << "\n"
     << "#if __cplusplus < 201703L\n\n"
     << "namespace tfel{\n"
     << "\n";
  for (const auto t : {"float", "double", "long double"}) {
    for (const auto& c : cs) {
      os << "template<>\n"
         << "const " << t << " "
         << "PhysicalConstants<" << t << ">::" << c.name << " = "
         << "static_cast<" << t << ">(" << c.value << ");\n\n";
      if (!c.short_name.empty()) {
        os << "template<>\n"
           << "const " << t << " "
           << "PhysicalConstants<" << t << ">::" << c.short_name << " = "
           << "static_cast<" << t << ">(" << c.value << ");\n\n";
      }
    }
  }
  os << "} // end of namespace tfel\n\n"
     << "#endif  /* __cplusplus < 201703L */\n";
  os.close();
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

int main() {
  const auto cs = extract("physical_constants.json");
  generate_cxx(cs);
  generate_python(cs);
  generate_fortran77(cs);
  generate_fortran90(cs);
  return EXIT_SUCCESS;
}
