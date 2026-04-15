/*!
 * \file   MaterialPropertyParametersHandler.cxx
 * \brief
 * \author Thomas Helfer
 * \date   10 janv. 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/MaterialPropertyParametersHandler.hxx"

namespace mfront {

  void writeMaterialPropertyParametersHandler(
      std::ostream& os,
      const MaterialPropertyDescription& mpd,
      const WriteMaterialPropertyParametersHandlerArguments& args) {
    if ((areParametersTreatedAsStaticVariables(mpd)) ||
        (mpd.parameters.empty())) {
      return;
    }
    os << "namespace " << args.interface_namespace << "{\n\n"
       << "/*!\n"
       << " * \\brief an auxiliary class handling parameters of the\n "
       << " * " << args.material_property_name << " material property\n"
       << " */\n"
       << "struct " << args.material_property_name
       << "MaterialPropertyHandler\n{\n"
       << "//! return the unique instance of this class\n"
       << "static " << args.material_property_name
       << "MaterialPropertyHandler&\n"
       << "get" << args.material_property_name << "MaterialPropertyHandler(){\n"
       << "static " << args.material_property_name
       << "MaterialPropertyHandler i;\n"
       << "return i;\n"
       << "}\n";
    if (args.allow_copy_constructor) {
      os << args.material_property_name << "MaterialPropertyHandler(const "
         << args.material_property_name
         << "MaterialPropertyHandler&) = default;\n";
    }
    for (const auto& p : mpd.parameters) {
      os << args.floating_point_type << " " << p.name << ";\n";
    }
    os << "std::string msg;\n"
       << "bool ok = false;\n"
       << "private:\n"
       << args.material_property_name << "MaterialPropertyHandler()";
    if (!mpd.parameters.empty()) {
      os << "\n: ";
      for (auto p = mpd.parameters.begin(); p != mpd.parameters.end();) {
        tfel::raise_if(!p->hasAttribute(VariableDescription::defaultValue),
                       "writeAssignMaterialPropertyParameters: "
                       "no default value for parameter "
                       "'" +
                           p->name + "'");
        const auto pv =
            p->getAttribute<double>(VariableDescription::defaultValue);
        const auto prec = os.precision();
        os.precision(14);
        os << p->name << "(" << pv << ")";
        os.precision(prec);
        if (++p != mpd.parameters.end()) {
          os << ",\n";
        }
      }
    }
    os << "\n{\n";
    if (allowsParametersInitializationFromFile(mpd)) {
      os << "auto tokenize = [](const std::string& line){\n"
         << "  std::istringstream tokenizer(line);\n"
         << "  std::vector<std::string> tokens;\n"
         << "  std::copy(std::istream_iterator<std::string>(tokenizer),\n"
         << "  	  std::istream_iterator<std::string>(),\n"
         << "  	  std::back_inserter(tokens));\n"
         << "  return tokens;\n"
         << "};\n"
         << "std::ifstream pfile(\"" << getParametersFileName(mpd) << "\");\n"
         << "if(!pfile){\n"
         << "this->ok=true;\n"
         << "return;\n"
         << "}\n"
         << "size_t ln = 1u;\n"
         << "while(!pfile.eof()){\n"
         << "  auto line = std::string{};\n"
         << "  std::getline(pfile,line);\n"
         << "  auto tokens = tokenize(line);\n"
         << "  auto set_msg = [this,ln,line](const std::string& m){\n"
         << "    this->msg = \"" << args.material_property_name
         << "MaterialPropertyHandler::\"\n"
         << "    \"" << args.material_property_name
         << "MaterialPropertyHandler: \"\n"
         << "    \"error at line '\"+std::to_string(ln)+\"' \"\n"
         << "    \"while reading parameter file '"
         << args.material_property_name << "-parameters.txt'\"\n"
         << "    \" (\"+m+\")\";\n"
         << "  };\n"
         << "  if(tokens.empty()){\n"
         << "    continue;\n"
         << "  }\n"
         << "  if(tokens[0][0]=='#'){\n"
         << "    continue;\n"
         << "  }\n"
         << "  if(tokens.size()!=2u){\n"
         << "    set_msg(\"invalid number of tokens\");\n"
         << "    return;\n"
         << "  }\n"
         << "  double pvalue;\n"
         << "  std::istringstream mfront_converter(tokens[1]);\n"
         << "  mfront_converter.imbue(std::locale::classic());\n"
         << "  mfront_converter >> pvalue;\n"
         << "  if((!mfront_converter) || (!mfront_converter.eof())){\n"
         << "    set_msg(\"can't convert '\"+tokens[1]+"
         << "            \"' to floating point value\");\n"
         << "    return;\n"
         << "  }\n";
      bool first = true;
      for (const auto& p : mpd.parameters) {
        if (!first) {
          os << " else ";
        }
        os << "if(tokens[0]==\"" << p.name << "\"){\n"
           << "this->" << p.name << " = pvalue;\n"
           << "}";
        if (p.getExternalName() != p.name) {
          os << " else if(tokens[0]==\"" << p.getExternalName() << "\"){\n"
             << "this->" << p.name << " = pvalue;\n"
             << "}";
        }
        if ((!p.symbolic_form.empty()) && (p.symbolic_form != p.name)) {
          os << " else if(tokens[0]==\"" << p.symbolic_form << "\"){\n"
             << "this->" << p.name << " = pvalue;\n"
             << "}";
        }
        first = false;
      }
      os << "else {\n"
         << "set_msg(\"invalid parameter '\"+tokens[0]+\"'\");\n"
         << "return;\n"
         << "}\n"
         << "}\n";
    }
    os << "this->ok=true;\n"
       << "}\n"
       << "#if __cplusplus > 199711L\n"
       << args.material_property_name << "MaterialPropertyHandler("
       << args.material_property_name
       << "MaterialPropertyHandler&&) = delete;\n";
    if (!args.allow_copy_constructor) {
      os << args.material_property_name << "MaterialPropertyHandler(const "
         << args.material_property_name
         << "MaterialPropertyHandler&) = delete;\n";
    }
    os << args.material_property_name << "MaterialPropertyHandler&\n"
       << "operator=(" << args.material_property_name
       << "MaterialPropertyHandler&&) = delete;\n"
       << args.material_property_name << "MaterialPropertyHandler&\n"
       << "operator=(const " << args.material_property_name
       << "MaterialPropertyHandler&) = delete;\n"
       << "#endif /* __cplusplus > 199711L */\n"
       << "}; // end of struct " << args.material_property_name
       << "MaterialPropertyHandler\n"
       << "} // end of namespace " << args.interface_namespace << "\n\n";
  }  // end of writeMaterialPropertyParametersHandler

  void writeMaterialPropertyParametersHandler(
      std::ostream& os,
      const MaterialPropertyDescription& mpd,
      const std::string& n,
      const std::string& t,
      const std::string& i) {
    writeMaterialPropertyParametersHandler(os, mpd,
                                           {.material_property_name = n,
                                            .floating_point_type = t,
                                            .interface_namespace = i});
  }  // end of writeMaterialPropertyParametersHandler

  void writeAssignMaterialPropertyParameters(
      std::ostream& os,
      const MaterialPropertyDescription& mpd,
      const std::string& n,
      const std::string& t,
      const std::string& i) {
    if (areParametersTreatedAsStaticVariables(mpd)) {
      for (const auto& p : mpd.parameters) {
        const auto pv =
            p.getAttribute<double>(VariableDescription::defaultValue);
        os << "constexpr auto " << p.name << " = ";
        if (useQuantities(mpd)) {
          os << p.type << "(" << pv << ");\n";
        } else {
          os << t << "(" << pv << ");\n";
        }
      }
    } else {
      if (useQuantities(mpd)) {
        for (const auto& p : mpd.parameters) {
          os << "const auto " << p.name << " = " << p.type << "(" << i
             << "::" << n << "MaterialPropertyHandler::get" << n
             << "MaterialPropertyHandler()." << p.name << ");\n";
        }
      } else {
        for (const auto& p : mpd.parameters) {
          os << "const " << t << " " << p.name << " = " << i << "::" << n
             << "MaterialPropertyHandler::get" << n
             << "MaterialPropertyHandler()." << p.name << ";\n";
        }
      }
    }
  }  // end of writeAssignMaterialPropertyParameters

  void writeParametersSetterFunctionDeclaration(
      std::ostream& os,
      const MaterialPropertyDescription& mpd,
      const WriteMaterialPropertyParametersSetterFunctionArguments& args) {
    const auto& params = mpd.parameters;
    if ((areParametersTreatedAsStaticVariables(mpd)) || (params.empty())) {
      return;
    }
    os << "/*!\n"
       << " * \\brief set the value of the given parameter\n"
       << " *\n"
       << " * \\param[in] n: name of the parameter\n"
       << " * \\param[in] v: new value of the parameter\n"
       << " */\n"
       << "MFRONT_SHAREDOBJ int\n"
       << args.material_property_name << "_setParameter(const char *const,"
       << "const double);\n\n";
  }  // end of writeParametersSetterFunctionDeclaration

  void writeParametersSetterFunctionImplementation(
      std::ostream& os,
      const MaterialPropertyDescription& mpd,
      const WriteMaterialPropertyParametersSetterFunctionArguments& args) {
    const auto& params = mpd.parameters;
    if ((areParametersTreatedAsStaticVariables(mpd)) || (params.empty())) {
      return;
    }
    const auto hn = getMaterialPropertyParametersHandlerClassName(
        args.material_property_name);
    os << "MFRONT_SHAREDOBJ int\n"
       << args.material_property_name << "_setParameter(const char *const p,"
       << "const double v"
       << "){\n";
    for (const auto& p : params) {
      if (p.getExternalName() != p.name) {
        os << "if(strcmp(\"" << p.getExternalName() << "\",p) == 0){\n"
           << args.interface_namespace << "::" << hn << "::get" << hn << "()."
           << p.name << " = static_cast<" << args.floating_point_type
           << ">(v);\n"
           << "return 1;\n"
           << "}\n";
      }
      if ((!p.symbolic_form.empty()) && (p.symbolic_form != p.name)) {
        os << "if(strcmp(\"" << p.symbolic_form << "\",p) == 0){\n"
           << args.interface_namespace << "::" << hn << "::get" << hn << "()."
           << p.name << " = static_cast<" << args.floating_point_type
           << ">(v);\n"
           << "return 1;\n"
           << "}\n";
      }
      os << "if(strcmp(\"" << p.name << "\",p) == 0){\n"
         << args.interface_namespace << "::" << hn << "::get" << hn << "()."
         << p.name << " = static_cast<" << args.floating_point_type << ">(v);\n"
         << "return 1;\n"
         << "}\n";
    }
    os << "return 0;\n"
       << "}\n\n";
  }  // end of writeParametersSetterFunctionImplementation

  std::string getMaterialPropertyParametersHandlerClassName(
      const std::string& n) {
    return n + "MaterialPropertyHandler";
  }

}  // end of namespace mfront
