/*!
 * \file   MFrontModelInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   03 juin 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <fstream>
#include <iterator>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/System/System.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MFrontModelInterface.hxx"

namespace mfront {

  static bool useQuantities(const ModelDescription& md) {
    return md.use_qt.has_value() ? *(md.use_qt) : false;
  }  // end of useQuantities

  static void writeScalarStandardTypedefs(std::ostream& os,
                                          const ModelDescription& md) {
    const auto use_qt = useQuantities(md) ? "true" : "false";
    os << "[[maybe_unused]] static constexpr auto use_qt = " << use_qt << ";\n";
    for (const auto& a : getScalarTypeAliases()) {
      os << "using " << a << " [[maybe_unused]] = "
         << "typename tfel::config::ScalarTypes<NumericType, " << use_qt
         << ">::" << a << ";\n";
    }
  }  // end of writeScalarStandardTypedefs

  static std::string getHeaderGuard(const ModelDescription& mb) {
    const auto& m = mb.material;
    std::string header = "LIB_MFRONT_MODEL";
    if (!mb.library.empty()) {
      header += "_";
      header += makeUpperCase(mb.library);
    }
    if (!m.empty()) {
      header += "_";
      header += makeUpperCase(m);
    }
    header += "_";
    header += makeUpperCase(mb.className);
    header += "_HXX";
    return header;
  }

  static std::string getSrcFileName(const ModelDescription& md) {
    if (md.className.empty()) {
      throw("getSrcFileName: no class name defined");
    }
    return md.className;
  }  // end of getSrcFileName

  static std::string getHeaderFileName(const ModelDescription& md) {
    return "MFront/" + getSrcFileName(md);
  }  // end of getSrcFileName

  std::string MFrontModelInterface::getName() {
    return "mfront";
  }  // end of MFrontModelInterface::getName

  MFrontModelInterface::MFrontModelInterface() = default;

  std::pair<bool, MFrontModelInterface::tokens_iterator>
  MFrontModelInterface::treatKeyword(const std::string& k,
                                     const std::vector<std::string>& i,
                                     tokens_iterator p,
                                     const tokens_iterator) {
    tfel::raise_if(std::find(i.begin(), i.end(), "mfront") != i.end(),
                   "MfrontMaterialPropertyInterface::"
                   "treatKeyword: unsupported key '" +
                       k + "'");
    return {false, p};
  }  // end of MFrontModelInterface::treatKeyword

  void MFrontModelInterface::declareReservedNames(
      std::set<std::string>& names) {
    names.insert({"BoundsCheckBase", "OutOfBoundsPolicy",
                  "setOutOfBoundsPolicy", "policy", "policy_"});
  }  // end of MFrontModelInterface::declareReservedNames

  void MFrontModelInterface::writeOutputFiles(const FileDescription& fd,
                                              const ModelDescription& md) {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "MFrontModelInterface::writeOutputFiles: " + m);
    };
    auto write_bounds = [&throw_if](std::ostream& out,
                                    const VariableDescription& v,
                                    const std::string& vn) {
      auto write = [&out, &v, &vn, &throw_if](const VariableBoundsDescription& (
                                                  VariableDescription::*m)()
                                                  const,
                                              const std::string& p) {
        const auto& bd = (v.*m)();
        const auto numeric_type = [&v] {
          if (v.isScalar()) {
            return v.type;
          }
          return "tfel::math::numeric_type<" + v.type + ">";
        }();
        if (bd.boundsType == VariableBoundsDescription::LOWER) {
          out << "BoundsCheckBase::lowerBoundCheck"
              << "(\"" << vn << "\"," << vn << ",static_cast<" << numeric_type
              << ">(" << bd.lowerBound << ")," << p << ");\n";
        } else if (bd.boundsType == VariableBoundsDescription::UPPER) {
          out << "BoundsCheckBase::upperBoundCheck"
              << "(\"" << vn << "\"," << vn << ",static_cast<" << numeric_type
              << ">(" << bd.upperBound << ")," << p << ");\n";
        } else if (bd.boundsType == VariableBoundsDescription::LOWERANDUPPER) {
          out << "BoundsCheckBase::lowerAndUpperBoundsChecks"
              << "(\"" << vn << "\"," << vn << ",static_cast<" << numeric_type
              << ">(" << bd.lowerBound << "),"
              << "static_cast<" << numeric_type << ">(" << bd.upperBound << "),"
              << p << ");\n";
        } else {
          throw_if(true, "unsupported bound type for variable '" + vn + "'");
        }
      };
      if (v.hasBounds()) {
        write(&VariableDescription::getBounds, "this->policy");
      }
      if (v.hasPhysicalBounds()) {
        write(&VariableDescription::getPhysicalBounds,
              "OutOfBoundsPolicy::Strict");
      }
    };
    tfel::system::systemCall::mkdir("include/MFront");
    const auto hname = getHeaderFileName(md) + ".hxx";
    std::ofstream header("include/" + hname);
    throw_if(!header, "can't open file 'include/" + hname + "'");
    header.exceptions(std::ios::badbit | std::ios::failbit);
    header << "/*!\n"
           << " * \\file   " << hname << '\n'
           << " * \\brief  This file declares the mfront interface for the "
           << md.className << " model\n"
           << " * \\author " << fd.authorName << '\n'
           << " * \\date   " << fd.date << '\n'
           << " */\n\n"
           << "#ifndef " << getHeaderGuard(md) << "\n"
           << "#define " << getHeaderGuard(md) << "\n\n"
           << "#include\"TFEL/Config/TFELTypes.hxx\"\n"
           << "#include\"TFEL/PhysicalConstants.hxx\"\n"
           << "#include\"TFEL/Material/BoundsCheck.hxx\"\n"
           << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n";
    const auto hasBounds = [&md] {
      const auto hasBounds2 = [](const VariableDescriptionContainer& c) {
        for (const auto& v : c) {
          if ((v.hasBounds()) || (v.hasPhysicalBounds())) {
            return true;
          }
        }
        return false;
      };
      return hasBounds2(md.outputs) || hasBounds2(md.inputs) ||
             hasBounds2(md.parameters);
    }();
    if (hasBounds) {
      header << "#include\"TFEL/Material/BoundsCheck.hxx\"\n";
    }
    if (!md.includes.empty()) {
      header << md.includes;
    }
    header << "\n"
           << "namespace mfront{\n\n"
           << "/*!\n"
           << " * \\brief  structure implementing the " << md.className
           << " model\n"
           << " */\n"
           << "template<typename NumericType>\n"
           << "struct " << md.className << '\n'
           << "{\n"
           << "//! a simple alias\n"
           << "typedef tfel::material::OutOfBoundsPolicy OutOfBoundsPolicy;\n";
    writeScalarStandardTypedefs(header, md);
    if (md.constantMaterialProperties.empty()) {
      header << "//! default constructor\n"
             << md.className << "() = default;\n";
    } else {
      header << "/*!\n"
             << " * \\brief constructor\n";
      for (const auto& v : md.constantMaterialProperties) {
        header << " * \\param[in] " << v.name << "_: \n";
      }
      header << " */\n" << md.className << "(";
      for (auto pmp = std::begin(md.constantMaterialProperties);
           pmp != std::end(md.constantMaterialProperties);) {
        header << "const real " << pmp->name << "_";
        if (++pmp != std::end(md.constantMaterialProperties)) {
          header << ",\n";
        }
      }
      header << ")\n"
             << " : ";
      for (auto pmp = std::begin(md.constantMaterialProperties);
           pmp != std::end(md.constantMaterialProperties);) {
        header << pmp->name << "(" << pmp->name << "_)";
        if (++pmp != std::end(md.constantMaterialProperties)) {
          header << ",\n";
        }
      }
      header << "\n{} // end of " << md.className << "\n";
    }
    for (const auto& f : md.functions) {
      throw_if(f.name.empty(), "unnamed function");
      throw_if(f.modifiedVariables.empty(),
               "no modified variable for function '" + f.name + "'");
      throw_if((f.usedVariables.empty()) && (!f.useTimeIncrement),
               "no used variable for function '" + f.name + "'");
      const auto args = [&f] {
        auto a = std::vector<std::string>{};
        for (const auto& uv : f.usedVariables) {
          a.push_back(uv);
        }
        if (f.useTimeIncrement) {
          a.emplace_back("dt");
        }
        return a;
      }();
      header << "/*!\n"
             << " * \\brief implementation of " << f.name << "\n";
      if (f.modifiedVariables.size() > 1) {
        for (const auto& mv : f.modifiedVariables) {
          header << " * \\param[out] " << mv << ": \n";
        }
      }
      for (const auto& a : args) {
        header << " * \\param[out] " << a << ": \n";
      }
      if (f.modifiedVariables.size() == 1) {
        header << "\\return the value of " << *(f.modifiedVariables.begin())
               << " at the end of the time step";
      }
      header << " */\n";
      if (f.modifiedVariables.size() == 1) {
        header << "real ";
      } else {
        header << "void ";
      }
      header << f.name << "(";
      if (f.modifiedVariables.size() > 1) {
        for (const auto& mv : f.modifiedVariables) {
          header << "real& " << mv << ",";
        }
      }
      for (auto pa = std::begin(args); pa != std::end(args);) {
        header << "const real " << *pa;
        if (++pa != std::end(args)) {
          header << ",";
        }
      }
      header << ") const {\n"
             << "using namespace std;\n"
             << "using namespace tfel::math;\n"
             << "using namespace tfel::material;\n"
             << "using tfel::material::BoundsCheckBase;\n";
      writeMaterialLaws(header, md.materialLaws);
      if (useQuantities(md)) {
        header << "using PhysicalConstants [[maybe_unused]] = "
               << "tfel::PhysicalConstants<NumericType, true>;\n";
      } else {
        header << "using PhysicalConstants [[maybe_unused]] = "
               << "tfel::PhysicalConstants<NumericType, false>;\n";
      }
      if (f.modifiedVariables.size() == 1) {
        header << "real " << *(f.modifiedVariables.begin()) << ";\n";
      }
      for (const auto& vn : f.usedVariables) {
        const auto dv = md.decomposeVariableName(vn);
        const auto& v = [&md, &dv]() -> const VariableDescription& {
          if (md.outputs.contains(dv.first)) {
            return md.outputs.getVariable(dv.first);
          }
          return md.inputs.getVariable(dv.first);
        }();
        write_bounds(header, v, vn);
      }
      header << f.body;
      for (const auto& vn : f.modifiedVariables) {
        const auto dv = md.decomposeVariableName(vn);
        const auto& v = [&md, &dv]() -> const VariableDescription& {
          return md.outputs.getVariable(dv.first);
        }();
        write_bounds(header, v, vn);
      }
      if (f.modifiedVariables.size() == 1) {
        header << "return " << *(f.modifiedVariables.begin()) << ";\n";
      }
      header << "} // end of " << f.name << "\n";
    }
    for (const auto& p : md.parameters) {
      throw_if(!p.hasAttribute(VariableDescription::defaultValue),
               "no default value for variable '" + p.name + "'");
      if ((p.type == "double") || (p.isScalar())) {
        const auto v =
            p.getAttribute<double>(VariableDescription::defaultValue);
        header << "real " << p.name << " = " << p.type << "{" << v << "};\n";
      } else if (p.type == "bool") {
        if (p.getAttribute<bool>(VariableDescription::defaultValue)) {
          header << "bool " << p.name << " = true;\n";
        } else {
          header << "bool " << p.name << " = false;\n";
        }
      } else {
        throw_if(true, "unsupported type (" + p.type + ") for  parameter '" +
                           p.name + "' defined at line " +
                           std::to_string(p.lineNumber));
      }
    }
    header << "/*!\n"
           << " * \\brief set the out of bounds policy\n"
           << " * \\param[in] policy_: new policy\n"
           << " */\n"
           << "void setOutOfBoundsPolicy(const OutOfBoundsPolicy& policy_){\n"
           << "this->policy = policy_;\n"
           << "}\n // end of setOutOfBoundsPolicy\n"
           << "private:\n"
           << "//! move constructor\n"
           << md.className << "(" << md.className << "&&) = delete;\n"
           << "//! copy constructor\n"
           << md.className << "(const " << md.className << "&) = delete;\n"
           << "//! move assignement\n"
           << md.className << "& operator=(" << md.className
           << "&&) = delete;\n"
           << "//! standard assignement\n"
           << md.className << "& operator=(const " << md.className
           << "&) = delete;\n";
    for (const auto& mp : md.constantMaterialProperties) {
      header << "const real " << mp.name << ";\n";
    }
    header << "//! out of bounds policy\n"
           << "OutOfBoundsPolicy policy = OutOfBoundsPolicy::None;\n"
           << "}; // end of struct " << md.className << "\n\n"
           << "} // end of namespace mfront\n\n"
           << "#endif /* " << getHeaderGuard(md) << " */\n";
  }  // end of MFrontModelInterface::writeOutputFiles

  void MFrontModelInterface::getTargetsDescription(TargetsDescription& td,
                                                   const ModelDescription& md) {
    insert_if(td.headers, getHeaderFileName(md) + ".hxx");
  }  // end of MFrontModelInterface::getTargetsDescription

  MFrontModelInterface::~MFrontModelInterface() = default;

}  // end of namespace mfront
