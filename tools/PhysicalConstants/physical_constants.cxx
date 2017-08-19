/*!
 * \file   physical_constants.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   18 août 2017
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

// g++ physical_constants.cxx -o physical_constants `tfel-config --compiler-flags --includes --libs --utilities` && ./physical_constants && cp PhysicalConstants.cxx ../../bindings/python/tfel/PhysicalConstants.cxx

#include<iostream>

#include<stdexcept>
#include<cstdlib>
#include<fstream>
#include<string>
#include<vector>

#include"TFEL/Utilities/CxxTokenizer.hxx"

struct Constant{
  std::string name;
  std::string description;
  std::string unit;
  long double value;
};

Constant extract(tfel::utilities::CxxTokenizer::const_iterator& p,
		 const tfel::utilities::CxxTokenizer::const_iterator pe)
{
  using tfel::utilities::CxxTokenizer;
  auto throw_if = [](const bool c,const std::string& m){
    if(c){throw(std::runtime_error("extract: "+m));}
  };
  auto read_token = [throw_if,&p,pe](const std::string& v){
    CxxTokenizer::checkNotEndOfLine("extract",p,pe);
    throw_if(p->value!=v,"expected '"+v+"', read '"+p->value+"'");
    ++p;
  };
  auto read_tokens = [read_token](const std::vector<std::string>& values){
    for(const auto& v:values){
      read_token(v);
    }
  };
  auto c = Constant{};
  read_tokens({"{","name",":"});
  c.name = CxxTokenizer::readString(p,pe);
  read_token(";");
  CxxTokenizer::checkNotEndOfLine("extract",p,pe);
  if(p->value=="description"){
    read_tokens({"description",":"});
    c.description = CxxTokenizer::readString(p,pe);
    read_token(";");
  }
  CxxTokenizer::checkNotEndOfLine("extract",p,pe);
  if(p->value=="unit"){
    read_tokens({"unit",":"});
    c.unit = CxxTokenizer::readString(p,pe);
    read_token(";");
  }
  read_tokens({"value",":"});
  CxxTokenizer::checkNotEndOfLine("extract",p,pe);
  c.value = std::stold(p->value);
  ++p;
  read_token(";");
  read_token("}");
  return c;
};

std::vector<Constant> extract(const std::string& f)
{
  tfel::utilities::CxxTokenizer t(f);
  t.stripComments();
  auto p = t.begin();
  const auto pe = t.end();
  std::vector<Constant> r;
  while(p!=pe){
    r.push_back(extract(p,pe));
  }
  return r;
} // end of extract

void generate_cxx(const std::vector<Constant>& cs)
{
  std::ofstream os("PhysicalConstants.hxx");
  os << "/*!\n"
     << " * \\file   include/TFEL/PhysicalConstants.hxx\n"
     << " * \\brief\n"
     << " * \\author HELFER Thomas 202608\n"
     << " * \\date   18 août 2017\n"
     << " * \\copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights\n"
     << " * reserved.\n"
     << " * This project is publicly released under either the GNU GPL Licence\n" 
     << " * or the CECILL-A licence. A copy of thoses licences are delivered\n" 
     << " * with the sources of TFEL. CEA or EDF may also distribute this\n"
     << " * project under specific licensing conditions.\n"
     << " */\n"
     << "\n"
     << "#ifndef LIB_TFEL_PHYSICALCONSTANTS_HXX\n"
     << "#define LIB_TFEL_PHYSICALCONSTANTS_HXX\n"
     << "\n"
     << "namespace tfel{\n"
     << "\n"
     << "  template<typename real = double,bool use_qt = false>\n"
     << "  struct PhysicalConstants{\n";
  for(const auto& c : cs){
    if((!c.description.empty())||(!c.unit.empty())){
      os << "//!";
      if(!c.description.empty()){
	os << ' ' << c.description;
      }
      if(!c.unit.empty()){
	os << " (" << c.unit << ')';
      }
      os << '\n';
    }
    os << "static constexpr const real " << c.name << " = real(" << c.value << ");\n";
  }
  os << "  }; // end of PhysicalConstants\n"
     << "\n"
     << "#if __cplusplus >= 201402L\n"
     << "namespace constants{\n\n";
  for(const auto& c : cs){
    if((!c.description.empty())||(!c.unit.empty())){
      os << "//!";
      if(!c.description.empty()){
	os << ' ' << c.description;
      }
      if(!c.unit.empty()){
	os << " (" << c.unit << ')';
      }
      os << '\n';
    }
    os << "template<typename real>\n"
       << "constexpr const real " << c.name << " = real(" << c.value << ");\n";
  }
  os << "\n"
     << "} // end of namespace constants\n"
     << "#endif /* __cplusplus >= 201402L */\n\n"
     << "} // end of namespace tfel\n"
     << "\n"
     << "#endif /* LIB_TFEL_PHYSICALCONSTANTS_HXX */\n";
} // end of generate_cxx

void generate_python(const std::vector<Constant>& cs)
{
  std::ofstream os("PhysicalConstants.cxx");
  os << "/*!\n"
     << " * \\file   bindings/python/tfel/PhysicalConstants.cxx\n"
     << " * \\brief\n"
     << " * \\author Thomas Helfer\n"
     << " * \\date   18 août 2017\n"
     << " * \\copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights \n"
     << " * reserved. \n"
     << " * This project is publicly released under either the GNU GPL Licence \n"
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
  for(const auto& c : cs){
    if((!c.description.empty())||(!c.unit.empty())){
      os << "//!";
      if(!c.description.empty()){
	os << ' ' << c.description;
      }
      if(!c.unit.empty()){
	os << " (" << c.unit << ')';
      }
      os << '\n';
    }
    os << "static const double " << c.name << ";\n";
  }
  os << "};\n\n";
  for(const auto& c : cs){
    os << "const double PhysicalConstants::" << c.name
       << " = double(" << c.value << ");\n";
  }
  os << "\n"
     << "void declarePhysicalConstants()\n"
     << "{\n"
     << "  boost::python::class_<PhysicalConstants>(\"PhysicalConstants\")\n";
  for(const auto& c : cs){
    os << ".def_readonly(\"" << c.name << "\",PhysicalConstants::" << c.name;
    if((!c.description.empty())||(!c.unit.empty())){
      os << ",\n\"";
      if(!c.description.empty()){
	os << c.description;
      }
      if(!c.unit.empty()){
	if(!c.description.empty()){
	  os << ' ';
	}
	os << "(" << c.unit << ')';
      }
      os << '"';
    }
    os << ")\n";
  }
  os << ";\n\n"
     << "} // end of declarePhysicalConstants\n\n";
} // end of declarePhysicalConstants

int main(){
  const auto cs = extract("physical_constants.json");
  generate_cxx(cs);
  generate_python(cs);
  return EXIT_SUCCESS;
}
