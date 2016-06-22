/*!
 * \file   MFrontModelInterface.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   03 juin 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<fstream>
#include "TFEL/System/System.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MFrontModelInterface.hxx"

namespace mfront{

  static std::string
  getHeaderDefine(const ModelDescription& mb)
  {
    const auto& m = mb.material;
    std::string header = "LIB_MFRONTMODEL_";
    if(!mb.library.empty()){
      header += "_";
      header += makeUpperCase(mb.library);
    }
    if(!m.empty()){
      header += "_";
      header += makeUpperCase(m);
    }
    header += "_";
    header += makeUpperCase(mb.className);
    header += "_HXX";
    return header;
  }
  
  static std::string
  getSrcFileName(const ModelDescription& md){
    if(md.className.empty()){
      throw("getSrcFileName: no class name defined");
    }
    return md.className;
  } // end of getSrcFileName

  static std::string
  getHeaderFileName(const ModelDescription& md){
    return "MFront/"+getSrcFileName(md);
  } // end of getSrcFileName

  static std::pair<std::string,unsigned short>
  decomposeVariableName(const ModelDescription& md,
			const std::string& n)
  {
    auto get = [&n](const VariableDescriptionContainer& vc)
      -> std::pair<std::string,unsigned short>
    {
      using size_type = unsigned short;
      for(const auto& v:vc){
	if(v.name==n){
	  return {v.name,0u};
	}
	const auto d = v.getAttribute<size_type>(VariableDescription::depth,0);
	for(size_type j=1;j<=d;++j){
	  auto fn = v.name + "_" + std::to_string(j);
	  if(fn==n){
	    return {v.name,j};
	  }
	}
      }
      return {};
    };
    auto r = get(md.outputs);
    if(!r.first.empty()){
      return r;
    }
    r = get(md.inputs);
    if(r.first.empty()){
      throw(std::runtime_error("decomposeVariableName: "
			       "field name '"+n+"' has not been found"));
    }
    return r;
  } // end of MFrontModelInterface::decomposeVariableName(const std::string& v)
  
  std::string MFrontModelInterface::getName(void){
    return "mfront";
  } // end of MFrontModelInterface::getName

  MFrontModelInterface::MFrontModelInterface() = default;

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontModelInterface::treatKeyword(const std::string&,
				     TokensContainer::const_iterator p,
				     const TokensContainer::const_iterator)
  {
    return {false,p};
  } // end of MFrontModelInterface::treatKeyword
    
  void MFrontModelInterface::declareReservedNames(std::set<std::string>&)
  {
    //    names.insert({"inputs"});
  } // end of MFrontModelInterface::declareReservedNames

  void MFrontModelInterface::writeOutputFiles(const FileDescription&  fd,
					      const ModelDescription& md)
  {
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("MFrontModelInterface::writeOutputFiles: "+m));}
    };
    auto write_bounds = [&throw_if](std::ostream& out,
				    const VariableDescription& v){
      auto write = [&out,&v,&throw_if](const std::string& bn,const std::string& p){
	const auto& bd = v.getAttribute<VariableBoundsDescription>(bn);
	if(bd.boundsType==VariableBoundsDescription::Lower){
	  out << "tfel::material::BoundsCheckBase::lowerBoundCheck"
	      << "(\"" << v.name << "\"," << v.name
	      << "," << bd.lowerBound << "," << p << ");\n";
	} else if(bd.boundsType==VariableBoundsDescription::Upper){
	  out << "tfel::material::BoundsCheckBase::upperBoundCheck"
	      << "(\"" << v.name << "\"," << v.name
	      << "," << bd.upperBound << "," << p << ");\n";
	} else if(bd.boundsType==VariableBoundsDescription::LowerAndUpper){
	  out << "tfel::material::BoundsCheckBase::lowerAndUpperBoundsChecks"
	      << "(\"" << v.name << "\"," << v.name
	      << "," << bd.lowerBound << "," << bd.upperBound << "," << p << ");\n";
	} else {
	  throw_if(true,"unsupported bound type for variable '"+v.name+"'");
	}
      };
      if(v.hasAttribute(VariableDescription::bound)){
	write(VariableDescription::bound,"this->policy");
      }
      if(v.hasAttribute(VariableDescription::physicalBound)){
	write(VariableDescription::physicalBound,
	      "tfel::material::OutOfBoundsPolicy::Strict");
      }
    };
    tfel::system::systemCall::mkdir("include/MFront");
    tfel::system::systemCall::mkdir("src");
    const auto hname = getHeaderFileName(md)+".hxx";
    std::ofstream header("include/"+hname);
    throw_if(!header,"can't open file 'include/"+hname+"'");
    header.exceptions(std::ios::badbit|std::ios::failbit);
    header << "/*!\n"
	   << " * \\file   "  << hname << '\n'
	   << " * \\brief  This file declares the mfront interface for the " 
	   << md.className << " model\n"
	   << " * \\author "  << fd.authorName << '\n'
	   << " * \\date   "  << fd.date       << '\n'
	   << " */\n\n"
	   << "#ifndef "<< getHeaderDefine(md) << "\n"
	   << "#define "<< getHeaderDefine(md) << "\n\n";
    const auto hasBounds = [&md](){
      for(const auto& v: md.outputs){
	if((v.hasAttribute(VariableDescription::bound))||
	   (v.hasAttribute(VariableDescription::physicalBound))){
	  return true;
	}
      }
      for(const auto& v: md.inputs){
	if((v.hasAttribute(VariableDescription::bound))||
	   (v.hasAttribute(VariableDescription::physicalBound))){
	  return true;
	}
      }
      return false;
    }();
    if(hasBounds){
      header << "#include\"TFEL/Material/BoundsCheck.hxx\"\n\n";
    }
    header << "namespace mfront{\n\n"
	   << "/*!\n"
	   << " * \\brief  structure implementing the "  << md.className << " model\n"
	   << " */\n"
	   << "template<typename real>\n"
	   << "struct " << md.className << '\n'
	   << "{\n";
    if(md.constantMaterialProperties.empty()){
      header << "//! default constructor\n"
	     <<md.className << "() = default;\n";
    } else {
      header << "/*!\n"
	     << " * \\brief constructor\n";
      for(const auto& v : md.constantMaterialProperties){
	header << " * \\param[in] " << v.name << "_: \n";
      }
      header << " */\n"
	     << md.className << "(";
      for(auto pmp=std::begin(md.constantMaterialProperties);
	  pmp!=std::end(md.constantMaterialProperties);){
	header << "const real " << pmp->name << "_";
	if(++pmp!=std::end(md.constantMaterialProperties)){
	  header << ",\n";
	}
      }
      header << ")\n"
	     << " : ";
      for(auto pmp=std::begin(md.constantMaterialProperties);
	  pmp!=std::end(md.constantMaterialProperties);){
	header << pmp->name << "(" << pmp->name  << "_)";
	if(++pmp!=std::end(md.constantMaterialProperties)){
	  header << ",\n";
	}
      }
      header << "\n{} // end of " << md.className << "\n";
    }
    for(const auto& f: md.functions){
      throw_if(f.name.empty(),"unnamed function");
      throw_if(f.modifiedVariables.empty(),"no modified variable for function '"+f.name+"'");
      throw_if(f.usedVariables.empty(),"no used variable for function '"+f.name+"'");
      if(f.modifiedVariables.size()==1){
	header << "real ";
      } else {
	header << "void ";
      }
      header << f.name << "(";
      if(f.modifiedVariables.size()>1){
	for(const auto& mv : f.modifiedVariables){
	  header << "real& " << mv << ",";
	}
      }
      for(auto puv=std::begin(f.usedVariables);puv!=std::end(f.usedVariables);){
	header << "const real " << *puv;
	if(++puv!=std::end(f.usedVariables)){
	  header << ",";
	}
      }
      header << ") const {\n";
      if(f.modifiedVariables.size()==1){
	header << "real " << *(f.modifiedVariables.begin()) << ";\n";
      }
      for(const auto& vn : f.usedVariables){
	const auto dv = decomposeVariableName(md,vn);
	const auto& v = [&md,&dv]() -> const VariableDescription& {
	  if(md.outputs.contains(dv.first)){
	    return md.outputs.getVariable(dv.first);
	  }
	  return md.inputs.getVariable(dv.first);
	}();
	write_bounds(header,v);
      }
      header << f.body;
      for(const auto& vn : f.modifiedVariables){
	const auto dv = decomposeVariableName(md,vn);
	const auto& v = [&md,&dv]() -> const VariableDescription& {
	  return md.outputs.getVariable(dv.first);
	}();
	write_bounds(header,v);
      }
      if(f.modifiedVariables.size()==1){
	header << "return " << *(f.modifiedVariables.begin()) << ";\n";
      }      
      header << "} // end of " << f.name << "\n";
    }
    for(const auto& p: md.parameters){
      throw_if(!p.hasAttribute(VariableDescription::defaultValue),
	       "no default value for variable '"+p.name+"'");
      if((p.type=="double")||(p.type=="real")){
	const auto v = p.getAttribute<double>(VariableDescription::defaultValue);
	header << "real " << p.name << " = " << v << ";\n";
      } else if (p.type=="bool"){
	if(p.getAttribute<bool>(VariableDescription::defaultValue)){
	  header << "bool " << p.name << " = true;\n";
	} else {
	  header << "bool " << p.name << " = false;\n";
	}
      } else {
	throw_if(true,"unsupported type ("+p.type+") for  parameter '"+
		 p.name+"' defined at line "+std::to_string(p.lineNumber));
      }
    }
    header << "private:\n"
	   << "//! move constructor\n"
	   << md.className << "(" << md.className << "&&) = delete;\n"
	   << "//! copy constructor\n"
	   << md.className << "(const " << md.className << "&) = delete;\n"
      	   << "//! move assignement\n"
	   << md.className << "& operator=(" << md.className << "&&) = delete;\n"
	   << "//! standard assignement\n"
	   << md.className << "& operator=(const " << md.className << "&) = delete;\n";
    for(const auto& mp: md.constantMaterialProperties){
      header << "const real " << mp.name << ";\n";
    }
    header << "};\n\n"
      	   << "}\n\n"
	   << "#endif /* "<< getHeaderDefine(md) << " */\n";
  } // end of MFrontModelInterface::writeOutputFiles

  void MFrontModelInterface::getTargetsDescription(TargetsDescription& td,
						   const ModelDescription& md)
  {
    insert_if(td.headers,"MFront/"+getHeaderFileName(md)+".hxx");
    // const auto lib = "MFrontModel";
    // insert_if(td[lib].ldflags,"-lm");
    // insert_if(td[lib].sources,getSrcFileName(md)+".cxx");
    // insert_if(td[lib].epts,md.className);
  } // end of MFrontModelInterface::getTargetsDescription

  MFrontModelInterface::~MFrontModelInterface() = default;
  
} // end of namespace mfront
