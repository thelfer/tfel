/*!
 * \file OctaveMaterialPropertyInterface.cxx
 * \brief
 * \author Helfer Thomas
 * \date 06 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>
#include<stdexcept>
#include<vector>
#include<string>
#include<iterator>
#include<algorithm>
#include<cstdlib>

#include"TFEL/System/System.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/DSLUtilities.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/MaterialPropertyDescription.hxx"
#include"MFront/OctaveMaterialPropertyInterface.hxx"

namespace mfront
{

  static std::vector<std::string>
  tokenize(const std::string& s,
	   const char c)
  {
    using namespace std;
    vector<string> res;
    string::size_type b = 0u;
    string::size_type e = s.find_first_of(c, b);
    while (string::npos != e || string::npos != b){
      // Found a token, add it to the vector.
      res.push_back(s.substr(b, e - b));
      b = s.find_first_not_of(c, e);
      e = s.find_first_of(c, b);
    }
    return res;
  } // end of tokenize
  
  std::string 
  OctaveMaterialPropertyInterface::toString(const unsigned short src)
  {
    std::ostringstream os;
    os << src;
    return os.str();
  }

  std::string
  OctaveMaterialPropertyInterface::getName(void)
  {
    return "octave";
  }

  OctaveMaterialPropertyInterface::OctaveMaterialPropertyInterface()
  {}

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  OctaveMaterialPropertyInterface::treatKeyword(const std::string&,
					 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
					 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
  {
    return {false,current};
  } // end of treatKeyword

  OctaveMaterialPropertyInterface::~OctaveMaterialPropertyInterface()
  {}

  void
  OctaveMaterialPropertyInterface::getTargetsDescription(TargetsDescription& td,
							 const MaterialPropertyDescription& mpd)
  {
    using namespace std;
    const char * mkoctfile = ::getenv("MKOCTFILE");
    const auto name = (mpd.material.empty()) ? mpd.className : mpd.material+"_"+mpd.className;
    const auto target="../octave/"+name+".oct";
    string cmd = "@cd ../octave/ && ";
    if(mkoctfile==nullptr){
      cmd += "mkoctfile";
    } else {
      cmd += mkoctfile;
    }
    cmd += " $(INCLUDES) -L../src/";
    cmd += " "+name+".cpp";
    auto& res = td.specific_targets;
    res[target].first.push_back("../octave/"+name+".cpp");
    res[target].second.push_back(cmd);
    res["all"].first.push_back("../octave/"+name+".oct");
  } // end of OctaveMaterialPropertyInterface::getSpecificTargets

  void
  OctaveMaterialPropertyInterface::writeOutputFiles(const MaterialPropertyDescription& mpd,
						    const FileDescription& fd)
  {
    using namespace std;
    tfel::system::systemCall::mkdir("octave");
    const auto name = (mpd.material.empty()) ? mpd.className : mpd.material+"_"+mpd.className;
    this->srcFileName     = "octave/" + name;
    this->srcFileName    += ".cpp";
    this->srcFile.open(this->srcFileName.c_str());
    if(!this->srcFile){
      string msg("OctaveMaterialPropertyInterface::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFile.exceptions(ios::badbit|ios::failbit);
    this->writeSrcFile(fd.fileName,name,fd.authorName,fd.date,
		       fd.description,mpd.includes,mpd.output,
		       mpd.inputs,mpd.materialLaws,mpd.staticVars,
		       mpd.parameters,mpd.parametersValues,
		       mpd.f,mpd.boundsDescriptions,
		       mpd.physicalBoundsDescriptions);
  } // end of OctaveMaterialPropertyInterface::writeOutputFiles

  void
  OctaveMaterialPropertyInterface::replace(std::string& s,
					   const std::string::value_type a,
					   const std::string::value_type b)
  {
    using namespace std;
    string::size_type p = 0;
    while((p=s.find(a))!=string::npos){
      s[p]=b;
    }
  } // end of MFrontCLawInterfaceBase::replace

  std::string
  OctaveMaterialPropertyInterface::treatDescriptionString(const std::string& s) const
  {
    using namespace std;
    string res(s);
    OctaveMaterialPropertyInterface::replace(res,'"',' ');
    return res;
  } // end of OctaveMaterialPropertyInterface::treatDescriptionString

  void
  OctaveMaterialPropertyInterface::writeSrcFile(const std::string& file,
						const std::string& name,
						const std::string& author,
						const std::string& date,
						const std::string& description,
						const std::string& includes,
						const std::string& output,
						const VarContainer& inputs,
						const std::vector<std::string>& materialLaws,
						const StaticVarContainer& staticVars,
						const std::vector<std::string>& params,
						const std::map<std::string,double>& paramValues,
						const LawFunction& function,
						const std::vector<VariableBoundsDescription>& bounds,
						const std::vector<VariableBoundsDescription>& physicalBounds)
  {
    using namespace std;
    vector<string>::const_iterator p;
    StaticVarContainer::const_iterator p2;
    VarContainer::const_iterator p3;
    vector<VariableBoundsDescription>::const_iterator p5;
    map<string,double>::const_iterator p6;
    VarContainer::size_type i;

    this->srcFile << "/*!" << endl;
    this->srcFile << "* \\file   " << this->srcFileName  << endl;
    this->srcFile << "* \\brief  " << "this file implements the " 
			<< name << " MaterialLaw.\n";
    this->srcFile << "*         File generated by ";
    this->srcFile << MFrontHeader::getVersionName() << " ";
    this->srcFile << "version " << MFrontHeader::getVersionNumber();
    this->srcFile << endl;
    if(!author.empty()){
      this->srcFile << "* \\author " << author << endl;
    }
    if(!date.empty()){
      this->srcFile << "* \\date   " << date       << endl;
    }
    if(!description.empty()){
      this->srcFile << description << endl;
    }
    this->srcFile << " */\n\n";
    this->srcFile << "#include<iostream>\n";
    this->srcFile << "#include<cmath>\n";
    this->srcFile << "#include<cstring>\n";
    this->srcFile << "#include<cstdlib>\n";
    this->srcFile << "#include<octave/oct.h>\n\n";

    if(!includes.empty()){
      this->srcFile << includes << endl << endl;
    }

    this->srcFile << "#ifdef __cplusplus\n";
    this->srcFile << "extern \"C\"{\n";
    this->srcFile << "#endif /* __cplusplus */\n\n";

    if(inputs.size()>1){
      this->srcFile << "static double\n";
      this->srcFile << "get_scalar_value(const octave_value& value,const int,const int){\n";
      this->srcFile << "return value.scalar_value();\n";
      this->srcFile << "} // end of get_scalar_value\n\n";
      this->srcFile << "static double\n";
      this->srcFile << "get_matrix_value(const octave_value& value,const int i,const int j){\n";
      this->srcFile << "return (value.matrix_value())(i,j);\n";
      this->srcFile << "} // end of get_matrix_value\n\n";
    }
    this->srcFile << "static double " << name <<"_compute(";
    if(!inputs.empty()){
      for(p3=inputs.begin();p3!=inputs.end();){
	this->srcFile << "const double " << p3->name;
	if((++p3)!=inputs.end()){
	  this->srcFile << ",";
	}
      }
    } else {
      this->srcFile << "void";
    }
    this->srcFile << ")\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "typedef double real;\n";
    // material laws
    writeMaterialLaws("OctaveMaterialPropertyInterface::writeOutputFile",
		      this->srcFile,materialLaws);
    // static variables
    writeStaticVariables("OctaveMaterialPropertyInterface::writeOutputFile",
			 srcFile,staticVars,file);
    // parameters
    if(!params.empty()){
      for(p=params.begin();p!=params.end();++p){
	p6 = paramValues.find(*p);
	if(p6==paramValues.end()){
	  string msg("OctaveMaterialPropertyInterface::writeOutputFile : ");
	  msg += "internal error (can't find value of parameter " + *p + ")";
	  throw(runtime_error(msg));
	}
	this->srcFile << "static const double " << *p << " = " << p6->second << ";\n";
      }
    }
    this->srcFile << "double " << output << ";\n";
    this->srcFile << function.body;
    this->srcFile << "return " << output << ";\n";
    this->srcFile << "} // end of " << name << "_compute\n\n";

    if((!bounds.empty())||
       (!physicalBounds.empty())){
      this->srcFile << "static double " << name <<"_checkBounds(";
      for(p3=inputs.begin();p3!=inputs.end();){
	this->srcFile << "const double " << p3->name;
	if((++p3)!=inputs.end()){
	  this->srcFile << ",";
	}
      }
      this->srcFile << ")\n{\n";
      this->srcFile << "using namespace std;\n";
      if(!physicalBounds.empty()){
	this->srcFile << "// treating physical bounds\n";
	for(p5=physicalBounds.begin();
	    p5!=physicalBounds.end();++p5){
	  if(p5->boundsType==VariableBoundsDescription::Lower){
	    this->srcFile << "if(" << p5->varName<< " < "<< p5->lowerBound << "){\n";
	    this->srcFile << "error(\"" << name << " :"  << p5->varName 
			  << " is below its physical lower bound.\");\n";
	    this->srcFile << "return -" << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	  } else if(p5->boundsType==VariableBoundsDescription::Upper){
	    this->srcFile << "if(" << p5->varName<< " > "<< p5->upperBound << "){\n";
	    this->srcFile << "error(\"" << name << " : " << p5->varName 
			  << " is over its physical upper bound.\");\n";
	    this->srcFile << "return -" << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	  } else {
	    this->srcFile << "if((" << p5->varName<< " < "<< p5->lowerBound << ")||"
			   << "(" << p5->varName<< " > "<< p5->upperBound << ")){\n";
	    this->srcFile << "if(" << p5->varName<< " < "<< p5->lowerBound << "){\n";
	    this->srcFile << "error(\"" << name << " : " << p5->varName 
			  << " is below its physical lower bound.\");\n";
	    this->srcFile << "}\n";
	    this->srcFile << "if(" << p5->varName<< " > "<< p5->upperBound << "){\n";
	    this->srcFile << "error(\"" << name << " : " << p5->varName 
			  << " is over its physical upper bound.\");\n";
	    this->srcFile << "}\n";
	    this->srcFile << "return -" << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	  }
	}
      }
      if(!bounds.empty()){
	this->srcFile << "// treating standard bounds\n";
	for(p5=bounds.begin();
	    p5!=bounds.end();++p5){
	  if(p5->boundsType==VariableBoundsDescription::Lower){
	    this->srcFile << "if(" << p5->varName<< " < "<< p5->lowerBound << "){\n";
	    this->srcFile << "const octave_value policy = get_global_value("
			  << "\"OCTAVE_OUT_OF_BOUNDS_POLICY\", true);\n";
	    this->srcFile << "if(policy.is_defined()){\n";
	    this->srcFile << "if(policy.is_string()){\n";
	    this->srcFile << "string msg(\"" << name << " : " << p5->varName 
			  << " is below its physical lower bound.\");\n";
	    this->srcFile << "if(policy.string_value()==\"STRICT\"){\n";
	    this->srcFile << "error(msg.c_str());\n";
	    this->srcFile << "return -" << p5->varNbr << ";\n";
	    this->srcFile << "} if(policy.string_value()==\"WARNING\"){\n";
	    this->srcFile << "octave_stdout << msg << \"\\n\";\n";
	    this->srcFile << "} else {\n";
	    this->srcFile << "return " << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	    this->srcFile << "} else {\n";
	    this->srcFile << "return " << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	    this->srcFile << "} else {\n";
	    this->srcFile << "return " << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	    this->srcFile << "return " << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	  } else if(p5->boundsType==VariableBoundsDescription::Upper){
	    this->srcFile << "if(" << p5->varName<< " < "<< p5->lowerBound << "){\n";
	    this->srcFile << "const octave_value policy = get_global_value("
			  << "\"OCTAVE_OUT_OF_BOUNDS_POLICY\", true);\n";
	    this->srcFile << "if(policy.is_defined()){\n";
	    this->srcFile << "if(policy.is_string()){\n";
	    this->srcFile << "string msg(\"" << p5->varName 
			  << " is over its physical upper bound.\");\n";
	    this->srcFile << "if(policy.string_value()==\"STRICT\"){\n";
	    this->srcFile << "error(msg.c_str());\n";
	    this->srcFile << "return -" << p5->varNbr << ";\n";
	    this->srcFile << "} if(policy.string_value()==\"WARNING\"){\n";
	    this->srcFile << "octave_stdout << msg << \"\\n\";\n";
	    this->srcFile << "} else {\n";
	    this->srcFile << "return " << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	    this->srcFile << "} else {\n";
	    this->srcFile << "return " << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	    this->srcFile << "} else {\n";
	    this->srcFile << "return " << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	    this->srcFile << "return " << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	  } else {
	    this->srcFile << "if((" << p5->varName<< " < "<< p5->lowerBound << ")||"
			  << "(" << p5->varName<< " > "<< p5->upperBound << ")){\n";
	    this->srcFile << "const octave_value policy = get_global_value("
			  << "\"OCTAVE_OUT_OF_BOUNDS_POLICY\", true);\n";
	    this->srcFile << "if(policy.is_defined()){\n";
	    this->srcFile << "if(policy.is_string()){\n";
	    this->srcFile << "string msg(\"" << p5->varName 
			  << " is out of its bounds.\");\n";
	    this->srcFile << "if(policy.string_value()==\"STRICT\"){\n";
	    this->srcFile << "error(msg.c_str());\n";
	    this->srcFile << "return -" << p5->varNbr << ";\n";
	    this->srcFile << "} if(policy.string_value()==\"WARNING\"){\n";
	    this->srcFile << "octave_stdout << msg << \"\\n\";\n";
	    this->srcFile << "} else {\n";
	    this->srcFile << "return " << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	    this->srcFile << "} else {\n";
	    this->srcFile << "return " << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	    this->srcFile << "} else {\n";
	    this->srcFile << "return " << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	    this->srcFile << "return " << p5->varNbr << ";\n";
	    this->srcFile << "}\n";
	  }
	}
      }
      this->srcFile << "return 0;\n} // end of " << name << "_checkBounds\n\n";
    }

    this->srcFile << "DEFUN_DLD(" << name << ",args,nargout,\n"
		  << "\"this function implements the " << name << " material law.\\n\"\n";
    if(!description.empty()){
      vector<string> desc = tokenize(description,'\n');
      for(p=desc.begin();p!=desc.end();){
	this->srcFile << "\"" << this->treatDescriptionString(*p) << "\\n\"";	
	if(++p!=desc.end()){
	  this->srcFile << "\n";
	}
      }
    }
    this->srcFile << ")\n";
    this->srcFile << "{\n";
    if(inputs.size()>1){
      //*      this->srcFile << name << "OctaveVarType varTypes["
      //*      << inputs.size() << "];\n";
      this->srcFile << "typedef double (*PtrGetFunction)(const octave_value&,const int,const int);\n";
      this->srcFile << "PtrGetFunction getfunction[" << inputs.size() << "];\n";
    }
    if(inputs.size()>=1){
      this->srcFile << "int i,j;\n";
      this->srcFile << "int  row,col;\n";
    }
    if(inputs.size()>1){
      this->srcFile << "bool areAllVarMatrices;\n";
      this->srcFile << "bool areAllVarScalars;\n";
    }
    this->srcFile << "octave_value retval;\n";
    this->srcFile << "if(args.length()!=" << inputs.size() << "){\n";
    if(inputs.size()==0){
      this->srcFile << "error(\"" << name << " : no argument required\");\n";
    } else if(inputs.size()==1){
      this->srcFile << "error(\"" << name << " : one argument and only one required\");\n";
    } else {
      this->srcFile << "error(\"" << name << " : " << inputs.size() 
		    << " arguments and only " << inputs.size() << " arguments required\");\n";
    }
    this->srcFile << "return retval;\n";
    this->srcFile << "}\n";
    if(inputs.size()==0){
      this->srcFile << "retval = " << name << "_compute();\n";
    } else if(inputs.size()==1){
      this->srcFile << "if(args(0).is_real_scalar()){\n";
      if((!bounds.empty())||
	 (!physicalBounds.empty())){
	this->srcFile << "if("<< name << "_checkBounds(";
	this->srcFile << "args(0).scalar_value())<0){\n";
	this->srcFile << "return retval;\n";
	this->srcFile << "}\n";
      }
      this->srcFile << "retval = " << name << "_compute(";
      this->srcFile << "args(0).scalar_value());\n";
      this->srcFile << "} else if(args(0).is_real_matrix()){\n";
      this->srcFile << "Matrix xin0(args(0).matrix_value());\n";
      this->srcFile << "Matrix xout(xin0.rows(),xin0.cols());\n";
      this->srcFile << "for(i=0;i!=xin0.rows();++i){\n";
      this->srcFile << "for(j=0;j!=xin0.cols();++j){\n";
      if((!bounds.empty())||
	 (!physicalBounds.empty())){
	this->srcFile << "if("<< name << "_checkBounds(";
	this->srcFile << "xin0(i,j))<0){\n";
	this->srcFile << "return retval;\n";
	this->srcFile << "}\n";
      }
      this->srcFile << "xout(i,j) = " << name << "_compute(";
      this->srcFile << "xin0(i,j));\n";
      this->srcFile << "}\n";
      this->srcFile << "}\n";
      this->srcFile << "retval = xout;\n";
      this->srcFile << "} else {";
      this->srcFile << "error(\"" << name << " : argument must be either a matrix or scalar\");\n";
      this->srcFile << "return retval;\n";
      this->srcFile << "}\n";
    } else {
      this->srcFile << "areAllVarMatrices = true;\n";
      this->srcFile << "areAllVarScalars = true;\n";
      this->srcFile << "row=-1;\n";
      this->srcFile << "col=-1;\n";
      this->srcFile << "for(i=0;i!=" << inputs.size() << ";++i){\n";
      this->srcFile << "if(args(i).is_real_scalar()){\n";
      this->srcFile << "areAllVarMatrices = false;\n";
      this->srcFile << "getfunction[i] = &get_scalar_value;\n";
      this->srcFile << "} else if(args(i).is_real_matrix()){\n";
      this->srcFile << "areAllVarScalars  = false;\n";
      this->srcFile << "getfunction[i] = &get_matrix_value;\n";
      this->srcFile << "if(row==-1){\n";
      this->srcFile << "row = args(i).matrix_value().rows();\n";
      this->srcFile << "col = args(i).matrix_value().cols();\n";
      this->srcFile << "} else {\n";
      this->srcFile << "if((row!=args(i).matrix_value().rows())||\n";
      this->srcFile << "(col!=args(i).matrix_value().cols())){\n";
      this->srcFile << "error(\"" << name << " : all arguments shall have the same size\");\n";
      this->srcFile << "return retval;\n";
      this->srcFile << "}\n";
      this->srcFile << "}\n";
      this->srcFile << "} else {\n";
      this->srcFile << "error(\"" << name << " : arguments must be either a matrix or scalar\");\n";
      this->srcFile << "return retval;\n";
      this->srcFile << "}\n";
      this->srcFile << "}\n";
      this->srcFile << "if(areAllVarScalars){\n";
      if((!bounds.empty())||
	 (!physicalBounds.empty())){
	this->srcFile << "if("<< name << "_checkBounds(";
	for(i=0;i!=inputs.size()-1;++i){
	  this->srcFile << "args(" << i << ").scalar_value(),";
	}
	this->srcFile << "args(" << i << ").scalar_value())<0){\n";
	this->srcFile << "return retval;\n";
	this->srcFile << "}\n";
      }
      this->srcFile << "retval = " << name << "_compute(";
      for(i=0;i!=inputs.size()-1;++i){
	this->srcFile << "args(" << i << ").scalar_value(),";
      }
      this->srcFile << "args(" << i << ").scalar_value());\n";
      this->srcFile << "} else if(areAllVarMatrices){\n";
      for(i=0;i!=inputs.size();++i){
	this->srcFile << "Matrix xin" << i<< "(args(" << i << ").matrix_value());\n"; 
      }
      this->srcFile << "Matrix xout(row,col);\n";
      this->srcFile << "for(i=0;i!=row;++i){\n";
      this->srcFile << "for(j=0;j!=col;++j){\n";
      if((!bounds.empty())||
	 (!physicalBounds.empty())){
	this->srcFile << "if("<< name << "_checkBounds(";
	for(i=0;i!=inputs.size()-1;++i){
	  this->srcFile << "xin" << i << "(i,j),";
	}
	this->srcFile << "xin" << i << "(i,j))<0){\n";
	this->srcFile << "return retval;\n";
	this->srcFile << "}\n";
      }
      this->srcFile << "xout(i,j) = " << name << "_compute(";
      for(i=0;i!=inputs.size()-1;++i){
	this->srcFile << "xin" << i << "(i,j),";
      }
      this->srcFile << "xin" << i << "(i,j));\n";
      this->srcFile << "}\n";
      this->srcFile << "}\n";
      this->srcFile << "retval = xout;\n";
      this->srcFile << "} else {\n";
      this->srcFile << "Matrix xout(row,col);\n";
      this->srcFile << "for(i=0;i!=row;++i){\n";
      this->srcFile << "for(j=0;j!=col;++j){\n";
      if((!bounds.empty())||
	 (!physicalBounds.empty())){
	this->srcFile << "if("<< name << "_checkBounds(";
	for(i=0;i!=inputs.size()-1;++i){
	  this->srcFile << "(*(getfunction[" << i << "]))(args(" << i << "),i,j),\n";
	}
	this->srcFile << "(*(getfunction["  << i << "]))(args(" << i << "),i,j))<0){\n";
	this->srcFile << "return retval;\n";
	this->srcFile << "}\n";
      }
      this->srcFile << "xout(i,j) = " << name << "_compute(";
      for(i=0;i!=inputs.size()-1;++i){
	this->srcFile << "(*(getfunction[" << i << "]))(args(" << i << "),i,j),\n";
      }
      this->srcFile << "(*(getfunction[" << i << "]))(args(" << i << "),i,j));\n";
      this->srcFile << "}\n";
      this->srcFile << "}\n";
      this->srcFile << "retval = xout;\n";
      this->srcFile << "}\n";
    }
    this->srcFile << "return retval;\n";
    this->srcFile << "} // end of " << name << "\n\n";
    this->srcFile << "#ifdef __cplusplus\n";
    this->srcFile << "} // end of extern \"C\"\n";
    this->srcFile << "#endif /* __cplusplus */\n\n";
    this->srcFile.close();
  } // end of OctaveMaterialPropertyInterface::writeSrcFile(void)

} // end of namespace mfront
