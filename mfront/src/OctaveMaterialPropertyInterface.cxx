/*!
 * \file OctaveMaterialPropertyInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date 06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <cstdlib>

#include "TFEL/Raise.hxx"
#include "TFEL/System/System.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/MaterialPropertyParametersHandler.hxx"
#include "MFront/CMaterialPropertyInterfaceBase.hxx"
#include "MFront/OctaveMaterialPropertyInterface.hxx"

namespace mfront {

  static void writeBoundsChecks(
      std::ostream& out,
      const VariableDescription& v,
      const std::string& name,
      const VariableDescriptionContainer::size_type nbr) {
    if (!v.hasBounds()) {
      return;
    }
    const auto& b = v.getBounds();
    const char* const get_out_of_bounds_policy =
        "#if OCTAVE_MAJOR_VERSION >= 6\n"
        "const octave_value mfront_policy = "
        "mfront_octave_interpreter.global_varval("
        "\"OCTAVE_OUT_OF_BOUNDS_POLICY\");\n"
        "#else /* OCTAVE_MAJOR_VERSION */\n"
        "const octave_value mfront_policy = "
        "mfront_octave_interpreter.get_symbol_table().global_varval("
        "\"OCTAVE_OUT_OF_BOUNDS_POLICY\");\n"
        "#endif /* OCTAVE_MAJOR_VERSION */\n";
    if (b.boundsType == VariableBoundsDescription::LOWER) {
      out << "if(" << v.name << " < " << b.lowerBound << "){\n"
          << get_out_of_bounds_policy  //
          << "if(mfront_policy.is_defined()){\n"
          << "if(mfront_policy.is_string()){\n"
          << "string msg(\"" << name << ": " << v.name
          << " is below its lower bound.\");\n"
          << "if(mfront_policy.string_value()==\"STRICT\"){\n"
          << "error(\"%s\\n\", msg.c_str());\n"
          << "return -" << nbr << ";\n"
          << "}\n"
          << "if(mfront_policy.string_value()==\"WARNING\"){\n"
          << "octave_stdout << msg << \"\\n\";\n"
          << "}\n"
          << "}\n"
          << "}\n"
          << "return " << nbr << ";\n"
          << "}\n";
    } else if (b.boundsType == VariableBoundsDescription::UPPER) {
      out << "if(" << v.name << " < " << b.lowerBound << "){\n"
          << get_out_of_bounds_policy  //
          << "\"OCTAVE_OUT_OF_BOUNDS_POLICY\");\n"
          << "if(mfront_policy.is_defined()){\n"
          << "if(mfront_policy.is_string()){\n"
          << "string msg(\"" << name << ": \"" << v.name
          << " is over its upper bound.\");\n"
          << "if(mfront_policy.string_value()==\"STRICT\"){\n"
          << "error(\"%s\\n\", msg.c_str());\n"
          << "return -" << nbr << ";\n"
          << "}\n"
          << "}\n"
          << "}\n"
          << "return " << nbr << ";\n"
          << "}\n";
    } else {
      out << "if((" << v.name << " < " << b.lowerBound << ")||"
          << "(" << v.name << " > " << b.upperBound << ")){\n"
          << get_out_of_bounds_policy  //
          << "if(mfront_policy.is_defined()){\n"
          << "if(mfront_policy.is_string()){\n"
          << "string msg(\"" << name << ": " << v.name
          << " is out of its bounds.\");\n"
          << "if(mfront_policy.string_value()==\"STRICT\"){\n"
          << "error(\"%s\\n\", msg.c_str());\n"
          << "return -" << nbr << ";\n"
          << "} if(mfront_policy.string_value()==\"WARNING\"){\n"
          << "octave_stdout << msg << \"\\n\";\n"
          << "}\n"
          << "}\n"
          << "}\n"
          << "return " << nbr << ";\n"
          << "}\n";
    }
  }  // end of writeBoundsChecks

  static void writePhysicalBoundsChecks(
      std::ostream& out,
      const VariableDescription& v,
      const std::string& name,
      const VariableDescriptionContainer::size_type nbr) {
    if (!v.hasPhysicalBounds()) {
      return;
    }
    const auto& b = v.getPhysicalBounds();
    if (b.boundsType == VariableBoundsDescription::LOWER) {
      out << "if(" << v.name << " < " << b.lowerBound << "){\n";
      out << "error(\"%s\\n\", \"" << name << ": " << v.name
          << " is below its physical lower bound.\");\n";
      out << "return -" << nbr << ";\n";
      out << "}\n";
    } else if (b.boundsType == VariableBoundsDescription::UPPER) {
      out << "if(" << v.name << " > " << b.upperBound << "){\n";
      out << "error(\"%s\\n\", \"" << name << ": " << v.name
          << " is over its physical upper bound.\");\n";
      out << "return -" << nbr << ";\n";
      out << "}\n";
    } else {
      out << "if((" << v.name << " < " << b.lowerBound << ")||"
          << "(" << v.name << " > " << b.upperBound << ")){\n";
      out << "if(" << v.name << " < " << b.lowerBound << "){\n";
      out << "error(\"%s\\n\", \"" << name << ": " << v.name
          << " is below its physical lower bound.\");\n";
      out << "}\n";
      out << "if(" << v.name << " > " << b.upperBound << "){\n";
      out << "error(\"%s\\n\", \"" << name << ": " << v.name
          << " is over its physical upper bound.\");\n";
      out << "}\n";
      out << "return -" << nbr << ";\n";
      out << "}\n";
    }
  }  // end of writePhysicalBoundsChecks

  static std::vector<std::string> tokenize(const std::string& s, const char c) {
    using namespace std;
    vector<string> res;
    string::size_type b = 0u;
    string::size_type e = s.find_first_of(c, b);
    while (string::npos != e || string::npos != b) {
      // Found a token, add it to the vector.
      res.push_back(s.substr(b, e - b));
      b = s.find_first_not_of(c, e);
      e = s.find_first_of(c, b);
    }
    return res;
  }  // end of tokenize

  std::string OctaveMaterialPropertyInterface::toString(
      const unsigned short src) {
    std::ostringstream os;
    os << src;
    return os.str();
  }

  std::string OctaveMaterialPropertyInterface::getName() { return "octave"; }

  OctaveMaterialPropertyInterface::OctaveMaterialPropertyInterface() = default;

  std::pair<bool, OctaveMaterialPropertyInterface::tokens_iterator>
  OctaveMaterialPropertyInterface::treatKeyword(
      const std::string& k,
      const std::vector<std::string>& i,
      tokens_iterator current,
      const tokens_iterator) {
    tfel::raise_if(std::find(i.begin(), i.end(), "octave") != i.end(),
                   "OctaveMaterialPropertyInterface::treatKeyword: "
                   "unsupported key '" +
                       k + "'");
    return {false, current};
  }  // end of treatKeyword

  void OctaveMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& td, const MaterialPropertyDescription& mpd) const {
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    const char* mkoctfile = ::getenv("MKOCTFILE");
    const auto name = (mpd.material.empty())
                          ? mpd.className
                          : mpd.material + "_" + mpd.className;
    const auto target = "../octave/" + name + ".oct";
    std::string cmd = "@cd ../octave/ && CXXFLAGS=\"$(CXXFLAGS) -std=c++17\" ";
    if (mkoctfile == nullptr) {
      cmd += "mkoctfile";
    } else {
      cmd += mkoctfile;
    }
    cmd += " $(INCLUDES) -L../src/";
    cmd += " $(shell " + tfel_config + " --cppflags --compiler-flags)";
    cmd += " $(shell " + tfel_config + " --includes)";
    cmd += " " + name + ".cpp";
    auto& res = td.specific_targets;
    insert_if(res[target].sources, "../octave/" + name + ".cpp");
    insert_if(res[target].cmds, cmd);
    insert_if(res["all"].deps, "../octave/" + name + ".oct");
  }  // end of OctaveMaterialPropertyInterface::getSpecificTargets

  void OctaveMaterialPropertyInterface::writeOutputFiles(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    tfel::system::systemCall::mkdir("octave");
    const auto name = (mpd.material.empty())
                          ? mpd.className
                          : mpd.material + "_" + mpd.className;
    const auto fn = "octave/" + name + ".cpp";
    std::ofstream out{fn};
    tfel::raise_if(!out,
                   "OctaveMaterialPropertyInterface::writeOutputFiles: "
                   "unable to open file '" +
                       fn + "'");
    out.exceptions(std::ios::badbit | std::ios::failbit);
    out << "/*!\n"
        << "* \\file   " << fn << '\n'
        << "* \\brief  "
        << "this file implements the " << name << " MaterialLaw.\n"
        << "*         File generated by " << MFrontHeader::getVersionName()
        << " "
        << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!fd.authorName.empty()) {
      out << "* \\author " << fd.authorName << '\n';
    }
    if (!fd.date.empty()) {
      out << "* \\date   " << fd.date << '\n';
    }
    if (!fd.description.empty()) {
      out << fd.description << '\n';
    }
    out << " */\n\n"
        << "#include<algorithm>\n"
        << "#include<iostream>\n"
        << "#include<iterator>\n"
        << "#include<sstream>\n"
        << "#include<fstream>\n"
        << "#include<cstring>\n"
        << "#include<cstdlib>\n"
        << "#include<string>\n"
        << "#include<vector>\n"
        << "#include<cmath>\n"
        << "#include\"TFEL/Config/TFELTypes.hxx\"\n"
        << "#include\"TFEL/PhysicalConstants.hxx\"\n"
        << "#include\"TFEL/Math/General/IEEE754.hxx\"\n\n";
    if (useQuantities(mpd)) {
      out << "#include\"TFEL/Math/qt.hxx\"\n"
          << "#include\"TFEL/Math/Quantity/qtIO.hxx\"\n";
    }
    out << "#include<octave/oct.h>\n\n";
    out << "#include<octave/interpreter.h>\n\n";
    if (!mpd.includes.empty()) {
      out << mpd.includes << "\n\n";
    }
    writeMaterialPropertyParametersHandler(out, mpd, name, "double", "octave");
    writeExportDirectives(out);
    out << "#ifdef __cplusplus\n"
        << "extern \"C\"{\n"
        << "#endif /* __cplusplus */\n\n";

    // mfront metadata
    writeVariablesNamesSymbol(out, name, mpd);
    writeVariablesBoundsSymbols(out, name, mpd);
    writeEntryPointSymbol(out, name);
    writeTFELVersionSymbol(out, name);
    writeInterfaceSymbol(out, name, "Octave");
    writeMaterialSymbol(out, name, mpd.material);
    writeMaterialKnowledgeTypeSymbol(out, name, MATERIALPROPERTY);

    if (mpd.inputs.size() > 1) {
      out << "static double\n"
          << "get_scalar_value(const octave_value& value,const "
             "octave_idx_type,const octave_idx_type){\n"
          << "return value.scalar_value();\n"
          << "} // end of get_scalar_value\n\n"
          << "static double\n"
          << "get_matrix_value(const octave_value& value,const octave_idx_type "
             "i,const octave_idx_type j){\n"
          << "return (value.matrix_value())(i,j);\n"
          << "} // end of get_matrix_value\n\n"
          << "static double\n"
          << "get_range_value(const octave_value& value,const "
             "octave_idx_type,const octave_idx_type j){\n"
          << "return value.range_value().elem(j);\n"
          << "} // end of get_range_value\n\n";
    }
    out << "static double " << name << "_compute(";
    if (!mpd.inputs.empty()) {
      for (auto p3 = mpd.inputs.begin(); p3 != mpd.inputs.end();) {
        if (useQuantities(mpd)) {
          out << "const double mfront_" << p3->name;
        } else {
          out << "const double " << p3->name;
        }
        if ((++p3) != mpd.inputs.end()) {
          out << ",";
        }
      }
    } else {
      out << "void";
    }
    out << ")\n{\n";
    writeBeginningOfMaterialPropertyBody(out, mpd, fd, "double", true);
    if (useQuantities(mpd)) {
      for (const auto& i : mpd.inputs) {
        out << "const auto " << i.name << " = "  //
            << i.type << "(mfront_" << i.name << ");\n";
      }
    }
    // parameters
    if (!mpd.parameters.empty()) {
      writeAssignMaterialPropertyParameters(out, mpd, name, "double", "octave");
    }
    if (useQuantities(mpd)) {
      out << "auto " << mpd.output.name << " = " << mpd.output.type << "{};\n";
    } else {
      out << "auto " << mpd.output.name << " = real{};\n";
    }
    out << "try{\n";
    out << mpd.f.body;
    out << "} catch(std::exception& e){\n"
        << "error(\"%s\\n\", e.what());\n"
        << "return 0;\n"
        << "} catch(...){\n"
        << "error(\"%s\\n\", \"" << name << ": unsupported C++ exception\");\n"
        << "return 0;\n"
        << "}\n";
    writePhysicalBoundsChecks(out, mpd.output, name, 0);
    writeBoundsChecks(out, mpd.output, name, 0);
    //
    if (useQuantities(mpd)) {
      out << "return " << mpd.output.name << ".getValue();\n";
    } else {
      out << "return " << mpd.output.name << ";\n";
    }
    out << "} // end of " << name << "_compute\n\n";
    if ((hasBounds(mpd.inputs)) || (hasPhysicalBounds(mpd.inputs))) {
      out << "static double " << name
          << "_checkBounds(octave::interpreter& mfront_octave_interpreter, ";
      for (auto p3 = mpd.inputs.begin(); p3 != mpd.inputs.end();) {
        out << "const double " << p3->name;
        if ((++p3) != mpd.inputs.end()) {
          out << ",";
        }
      }
      out << ")\n{\n";
      out << "using namespace std;\n";
      if (hasPhysicalBounds(mpd.inputs)) {
        out << "// treating physical bounds\n";
        for (const auto& i : mpd.inputs) {
          const auto nbr =
              CMaterialPropertyInterfaceBase::getVariableNumber(mpd, i.name);
          writePhysicalBoundsChecks(out, i, name, nbr);
        }
      }
      if (hasBounds(mpd.inputs)) {
        out << "// treating standard bounds\n";
        for (const auto& i : mpd.inputs) {
          const auto nbr =
              CMaterialPropertyInterfaceBase::getVariableNumber(mpd, i.name);
          writeBoundsChecks(out, i, name, nbr);
        }
      }
      out << "return 0;\n"
          << "} // end of " << name << "_checkBounds\n\n";
    }
    out << "DEFMETHOD_DLD(" << name << ", mfront_octave_interpreter, "
        << "args, nargout,\n"
        << "\"this function implements the " << name << " material law.\\n\"\n";
    if (!fd.description.empty()) {
      const auto desc = tokenize(fd.description, '\n');
      for (auto p = desc.begin(); p != desc.end();) {
        out << "\"" << this->treatDescriptionString(*p) << "\\n\"";
        if (++p != desc.end()) {
          out << "\n";
        }
      }
    }
    out << ")\n";
    out << "{\n";
    if (mpd.inputs.size() > 1) {
      //*      out << name << "OctaveVarType varTypes["
      //*      << mpd.inputs.size() << "];\n";
      out << "typedef double (*PtrGetFunction)(const octave_value&,const "
             "octave_idx_type,const octave_idx_type);\n";
      out << "PtrGetFunction getfunction[" << mpd.inputs.size() << "];\n";
    }
    if (mpd.inputs.size() >= 1) {
      out << "// local variables used to convert ranges to matrices\n";
      out << "octave_idx_type i,j;\n";
      out << "octave_idx_type row = -1;\n";
      out << "octave_idx_type col = -1;\n";
      out << "bool areAllVariablesMatrices = true;\n";
      out << "bool areAllVariablesScalars  = true;\n";
    }
    out << "octave_value retval;\n";
    if (!mpd.parameters.empty()) {
      const auto hn = getMaterialPropertyParametersHandlerClassName(name);
      out << "if(!octave::" << hn << "::get" << hn << "().ok){\n"
          << "error(\"%s\\n\", octave::" << name
          << "MaterialPropertyHandler::get" << name
          << "MaterialPropertyHandler().msg.c_str());\n"
          << "return retval;\n"
          << "}\n";
    }
    out << "if(args.length()!=" << mpd.inputs.size() << "){\n";
    if (mpd.inputs.size() == 0) {
      out << "error(\"%s\\n\", \"" << name << " : no argument required\");\n";
    } else if (mpd.inputs.size() == 1) {
      out << "error(\"%s\\n\", \"" << name
          << " : one argument and only one required\");\n";
    } else {
      out << "error(\"%s\\n\", \"" << name << " : " << mpd.inputs.size()
          << " arguments and only " << mpd.inputs.size()
          << " arguments required\");\n";
    }
    out << "return retval;\n";
    out << "}\n";
    if (mpd.inputs.size() == 0) {
      out << "retval = " << name << "_compute();\n";
    } else if (mpd.inputs.size() == 1) {
      out << "if(args(0).is_real_scalar()){\n";
      if ((hasBounds(mpd.inputs)) || (hasPhysicalBounds(mpd.inputs))) {
        out << "if(" << name << "_checkBounds(mfront_octave_interpreter, ";
        out << "args(0).scalar_value())<0){\n";
        out << "return retval;\n";
        out << "}\n";
      }
      out << "retval = " << name << "_compute(";
      out << "args(0).scalar_value());\n";
      out << "} else if(args(0).is_real_matrix()||args(0).is_range()){\n";
      out << "Matrix xin0(args(0).is_range() ? "
             "args(0).range_value().matrix_value() : "
             "args(0).matrix_value());\n";
      out << "Matrix xout(xin0.rows(),xin0.cols());\n";
      out << "for(i=0;i!=xin0.rows();++i){\n";
      out << "for(j=0;j!=xin0.cols();++j){\n";
      if ((hasBounds(mpd.inputs)) || (hasPhysicalBounds(mpd.inputs))) {
        out << "if(" << name << "_checkBounds(mfront_octave_interpreter, ";
        out << "xin0(i,j))<0){\n";
        out << "return retval;\n";
        out << "}\n";
      }
      out << "xout(i,j) = " << name << "_compute(";
      out << "xin0(i,j));\n";
      out << "}\n";
      out << "}\n";
      out << "retval = xout;\n";
      out << "} else {";
      out << "error(\"%s\\n\", \"" << name
          << " : argument must be either a matrix or scalar\");\n";
      out << "return retval;\n";
      out << "}\n";
    } else {
      // scalars
      out << "for(i=0;i!=" << mpd.inputs.size() << ";++i){\n"
          << "if(args(i).is_real_scalar()){\n"
          << "areAllVariablesMatrices = false;\n"
          << "getfunction[i] = &get_scalar_value;\n";
      // matrices
      out << "} else if(args(i).is_real_matrix()){\n"
          << "areAllVariablesScalars  = false;\n"
          << "getfunction[i] = &get_matrix_value;\n"
          << "if(row==-1){\n"
          << "row = args(i).matrix_value().rows();\n"
          << "col = args(i).matrix_value().cols();\n"
          << "} else {\n"
          << "if((row!=args(i).matrix_value().rows())||\n"
          << "(col!=args(i).matrix_value().cols())){\n"
          << "error(\"%s\\n\", \"" << name
          << " : all arguments shall have the same size\");\n"
          << "return retval;\n"
          << "}\n"
          << "}\n";
      // ranges
      out << "} else if(args(i).is_range()){\n"
          << "areAllVariablesScalars   = false;\n"
          << "areAllVariablesMatrices = false;\n"
          << "getfunction[i] = &get_range_value;\n"
          << "if(row==-1){\n"
          << "row = 1;\n"
          << "col = args(i).range_value().numel();\n"
          << "} else {\n"
          << "if((row!=1)||(col!=args(i).range_value().numel())){\n"
          << "error(\"%s\\n\", \"" << name
          << " : all arguments shall have the same size\");\n"
          << "return retval;\n"
          << "}\n"
          << "}\n";
      // unsupported type
      out << "} else {\n"
          << "error(\"%s\\n\", \"" << name
          << " : arguments must be either a matrix or scalar\");\n"
          << "return retval;\n"
          << "}\n"
          << "}\n";
      // all scalar case
      out << "if(areAllVariablesScalars){\n";
      decltype(mpd.inputs.size()) i;
      if ((hasBounds(mpd.inputs)) || (hasPhysicalBounds(mpd.inputs))) {
        out << "if(" << name << "_checkBounds(mfront_octave_interpreter, ";
        for (i = 0; i != mpd.inputs.size() - 1; ++i) {
          out << "args(" << i << ").scalar_value(),";
        }
        out << "args(" << i << ").scalar_value())<0){\n";
        out << "return retval;\n";
        out << "}\n";
      }
      out << "retval = " << name << "_compute(";
      for (i = 0; i != mpd.inputs.size() - 1; ++i) {
        out << "args(" << i << ").scalar_value(),";
      }
      out << "args(" << i << ").scalar_value());\n";
      // all matrices case
      out << "} else if(areAllVariablesMatrices){\n";
      for (i = 0; i != mpd.inputs.size(); ++i) {
        out << "Matrix xin" << i << "(args(" << i << ").matrix_value());\n";
      }
      out << "Matrix xout(row,col);\n";
      out << "for(i=0;i!=row;++i){\n";
      out << "for(j=0;j!=col;++j){\n";
      if ((hasBounds(mpd.inputs)) || (hasPhysicalBounds(mpd.inputs))) {
        out << "if(" << name << "_checkBounds(mfront_octave_interpreter, ";
        for (i = 0; i != mpd.inputs.size() - 1; ++i) {
          out << "xin" << i << "(i,j),";
        }
        out << "xin" << i << "(i,j))<0){\n";
        out << "return retval;\n";
        out << "}\n";
      }
      out << "xout(i,j) = " << name << "_compute(";
      for (i = 0; i != mpd.inputs.size() - 1; ++i) {
        out << "xin" << i << "(i,j),";
      }
      out << "xin" << i << "(i,j));\n";
      out << "}\n";
      out << "}\n";
      out << "retval = xout;\n";
      // general case
      out << "} else {\n";
      out << "Matrix xout(row,col);\n";
      out << "for(i=0;i!=row;++i){\n";
      out << "for(j=0;j!=col;++j){\n";
      if ((hasBounds(mpd.inputs)) || (hasPhysicalBounds(mpd.inputs))) {
        out << "if(" << name << "_checkBounds(mfront_octave_interpreter, ";
        for (i = 0; i != mpd.inputs.size() - 1; ++i) {
          out << "(*(getfunction[" << i << "]))(args(" << i << "),i,j),\n";
        }
        out << "(*(getfunction[" << i << "]))(args(" << i << "),i,j))<0){\n";
        out << "return retval;\n";
        out << "}\n";
      }
      out << "xout(i,j) = " << name << "_compute(";
      for (i = 0; i != mpd.inputs.size() - 1; ++i) {
        out << "(*(getfunction[" << i << "]))(args(" << i << "),i,j),\n";
      }
      out << "(*(getfunction[" << i << "]))(args(" << i << "),i,j));\n";
      out << "}\n";
      out << "}\n";
      out << "retval = xout;\n";
      out << "}\n";
    }
    out << "return retval;\n";
    out << "} // end of " << name << "\n\n";
    out << "#ifdef __cplusplus\n";
    out << "} // end of extern \"C\"\n";
    out << "#endif /* __cplusplus */\n\n";
    out.close();
  }  // end of OctaveMaterialPropertyInterface::writeOutputFiles

  void OctaveMaterialPropertyInterface::replace(
      std::string& s,
      const std::string::value_type a,
      const std::string::value_type b) {
    std::string::size_type p = 0;
    while ((p = s.find(a)) != std::string::npos) {
      s[p] = b;
    }
  }  // end of MFrontCLawInterfaceBase::replace

  std::string OctaveMaterialPropertyInterface::treatDescriptionString(
      const std::string& s) const {
    std::string res(s);
    OctaveMaterialPropertyInterface::replace(res, '"', ' ');
    return res;
  }  // end of OctaveMaterialPropertyInterface::treatDescriptionString

  OctaveMaterialPropertyInterface::~OctaveMaterialPropertyInterface() = default;

}  // end of namespace mfront
