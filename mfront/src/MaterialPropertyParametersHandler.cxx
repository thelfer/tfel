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
      const std::string& n,
      const std::string& t,
      const std::string& i) {
    if (mpd.parameters.empty()) {
      return;
    }
    os << "namespace " << i << "{\n\n"
       << "/*!\n"
       << " * \\brief an auxiliary class handling parameters of the\n "
       << " * " << n << " material propety in the " << i << " interface\n"
       << " */\n"
       << "struct " << n << "MaterialPropertyHandler\n{\n"
       << "//! return the unique instance of this class\n"
       << "static " << n << "MaterialPropertyHandler&\n"
       << "get" << n << "MaterialPropertyHandler(){\n"
       << "static " << n << "MaterialPropertyHandler i;\n"
       << "return i;\n"
       << "}\n";
    for (const auto& p : mpd.parameters) {
      os << t << " " << p.name << ";\n";
    }
    os << "std::string msg;\n"
       << "bool ok = false;\n"
       << "private:\n"
       << n << "MaterialPropertyHandler()";
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
        os << p->name << "(" << pv << ")";
        if (++p != mpd.parameters.end()) {
          os << ",\n";
        }
      }
    }
    os << "\n{\n"
       << "auto tokenize = [](const std::string& line){\n"
       << "  std::istringstream tokenizer(line);\n"
       << "  std::vector<std::string> tokens;\n"
       << "  std::copy(std::istream_iterator<std::string>(tokenizer),\n"
       << "  	  std::istream_iterator<std::string>(),\n"
       << "  	  std::back_inserter(tokens));\n"
       << "  return tokens;\n"
       << "};\n"
       << "std::ifstream pfile(\"" << n << "-parameters.txt\");\n"
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
       << "    this->msg = \"" << n << "MaterialPropertyHandler::\"\n"
       << "    \"" << n << "MaterialPropertyHandler: \"\n"
       << "    \"error at line '\"+std::to_string(ln)+\"' \"\n"
       << "    \"while reading parameter file '" << n << "-parameters.txt'\"\n"
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
       << "}\n"
       << "this->ok=true;\n"
       << "}\n"
       << "#if __cplusplus > 199711L\n"
       << n << "MaterialPropertyHandler(" << n
       << "MaterialPropertyHandler&&) = delete;\n"
       << n << "MaterialPropertyHandler(const " << n
       << "MaterialPropertyHandler&) = delete;\n"
       << n << "MaterialPropertyHandler&\n"
       << "operator=(" << n << "MaterialPropertyHandler&&) = delete;\n"
       << n << "MaterialPropertyHandler&\n"
       << "operator=(const " << n << "MaterialPropertyHandler&) = delete;\n"
       << "#endif /* __cplusplus > 199711L */\n"
       << "}; // end of struct " << n << "MaterialPropertyHandler\n"
       << "} // end of namespace " << i << "\n\n";
  }

  void writeAssignMaterialPropertyParameters(
      std::ostream& os,
      const MaterialPropertyDescription& mpd,
      const std::string& n,
      const std::string& t,
      const std::string& i) {
    for (const auto& p : mpd.parameters) {
      os << "const " << t << " " << p.name << " = " << i << "::" << n
         << "MaterialPropertyHandler::get" << n << "MaterialPropertyHandler()."
         << p.name << ";\n";
    }
  }

  std::string getMaterialPropertyParametersHandlerClassName(
      const std::string& n) {
    return n + "MaterialPropertyHandler";
  }

}  // end of namespace mfront
