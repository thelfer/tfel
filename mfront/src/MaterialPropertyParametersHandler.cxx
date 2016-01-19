/*!
 * \file   MaterialPropertyParametersHandler.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   10 janv. 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<ostream>
#include<stdexcept>
#include"MFront/MaterialPropertyDescription.hxx"
#include"MFront/MaterialPropertyParametersHandler.hxx"

namespace mfront{

  void
  writeMaterialPropertyParametersHandler(std::ostream& os,
					 const MaterialPropertyDescription& mpd,
					 const std::string& n,
					 const std::string& t,
					 const std::string& i){
    if(mpd.parameters.empty()){
      return;
    }
    os << "namespace " << i << "{\n\n"
       << "/*!\n"
       << " * \\brief an auxiliary class handling parameters of the\n "
       << " * " << n << " material propety in the " << i
       << " interface\n"
       << " */\n"
       << "struct " << n << "MaterialPropertyHandler\n{\n"
       << "//! return the unique instance of this class\n"
       << "static " << n << "MaterialPropertyHandler&\n"
       << "get" << n << "MaterialPropertyHandler(){\n"
       << "static " << n << "MaterialPropertyHandler i;\n"
       << "return i;\n"
       << "}\n";
    for(const auto& p: mpd.parameters){
      os << t << " " << p << ";\n";
    }
    os << "std::string msg;\n"
       << "bool ok = false;\n" 
       << "private:\n"
       << n << "MaterialPropertyHandler()";
    if(!mpd.parameters.empty()){
      os << "\n: ";
      for(auto p = mpd.parameters.begin();p!=mpd.parameters.end();){
	const auto pv = mpd.parametersValues.find(*p);
	if(pv==mpd.parametersValues.end()){
	  throw(std::runtime_error("writeAssignMaterialPropertyParameters: "
				   "no default value for parameter '"+*p+"'"));
	}
	os << *p << "(" << pv->second << ")";
	if(++p!=mpd.parameters.end()){
	  os << ",\n";
	}
      }
    }
    os << "\n{\n"
       << "std::ifstream pfile(\"" << n << "-parameters.txt\");\n"
       << "if(!pfile){\n"
       << "this->ok=true;\n"
       << "return;\n"
       << "}\n"
       << "while(!pfile.eof()){\n"
       << "std::string pname;\n"
       << t << " pvalue;\n"
       << "pfile >> pname;\n"
       << "if(pfile.bad()){\n"
       << "this->msg=\"" << n << "MaterialPropertyHandler(" << n << "MaterialPropertyHandler: \"\n"
       << "          \"error file reading parameter file '" << n << "-parameters.txt'\";\n"
       << "return;\n"
       << "}\n"
       << "if(pname.empty()){\n"
       << "continue;\n"
       << "}\n"
       << "pfile >> pvalue;\n"
       << "if(pfile.bad()){\n"
       << "this->msg=\"" << n << "MaterialPropertyHandler(" << n << "MaterialPropertyHandler: \"\n"
       << "          \"error file reading parameter file '" << n << "-parameters.txt'\";\n"
       << "return;\n"
       << "}\n";
    bool first = true;
    for(const auto& p: mpd.parameters){
      if(!first){
	os << " else ";
      }
      auto pn = p;
      if(mpd.glossaryNames.find(pn)!=mpd.glossaryNames.end()){
	pn = mpd.glossaryNames.find(pn)->second;
      } else {
	if(mpd.entryNames.find(pn)!=mpd.entryNames.end()){
	  pn = mpd.entryNames.find(pn)->second;
	}
      }
      os << "if(pname==\"" << pn << "\"){\n"
	 << "this->" << p << " = pvalue;\n"
	 << "}";
      first = false;
    }
    os << " else {\n"
       << "this->msg=\"" << n << "MaterialPropertyHandler(" << n << "MaterialPropertyHandler: \"\n"
       << "          \"invalid parameter '\"+pname+\"'\";\n"           
       << "return;\n"
       << "}\n"
       << "}\n"
       << "this->ok=true;\n"
       << "}\n"
       << "#if __cplusplus > 199711L\n"
       << n << "MaterialPropertyHandler(" << n << "MaterialPropertyHandler&&) = delete;\n"
       << n << "MaterialPropertyHandler(const " << n << "MaterialPropertyHandler&) = delete;\n"
       << n << "MaterialPropertyHandler&\n"
       << "operator=(" << n << "MaterialPropertyHandler&&) = delete;\n"
       << n << "MaterialPropertyHandler&\n"
       << "operator=(const " << n << "MaterialPropertyHandler&) = delete;\n"
       << "#endif /* __cplusplus > 199711L */\n"
       << "}; // end of struct " << n << "MaterialPropertyHandler\n"
       << "} // end of namespace " << i << "\n\n";
  }

  void
  writeAssignMaterialPropertyParameters(std::ostream& os,
					const MaterialPropertyDescription& mpd,
					const std::string& n,
					const std::string& t,
					const std::string& i){
    for(const auto& p:mpd.parameters){
      os << "const " << t << " " << p << " = "
	 << i << "::" << n << "MaterialPropertyHandler::get"
	 << n << "MaterialPropertyHandler()." << p << ";\n";
    }
  }
  
  std::string
  getMaterialPropertyParametersHandlerClassName(const std::string& n)
  {
    return n+"MaterialPropertyHandler";
  }

}
