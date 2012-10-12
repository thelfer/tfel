/*!
 * \file   MFrontPythonLawInterface.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   06 mai 2008
 */

#include<algorithm>
#include<sstream>
#include<stdexcept>
#include<iterator>
#include<string>
#include<set>

#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

#include"TFEL/System/System.hxx"

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontLock.hxx"
#include"MFront/MFrontCLawInterface.hxx"
#include"MFront/MFrontPythonLawInterface.hxx"

namespace mfront
{

  std::string
  MFrontPythonLawInterface::getName(void)
  {
    return "python";
  }

  MFrontPythonLawInterface::MFrontPythonLawInterface()
    : verboseMode(false),
      debugMode(false),
      warningMode(false)
  {}

  void
  MFrontPythonLawInterface::reset(void)
  {} // end of MFrontPythonLawInterface::reset(void)
  
  void 
  MFrontPythonLawInterface::setVerboseMode(void)
  {
    this->verboseMode = true;
  }

  void 
  MFrontPythonLawInterface::setWarningMode(void)
  {
    this->warningMode = true;
  }

  void 
  MFrontPythonLawInterface::setDebugMode(void)
  {
    this->debugMode = true;
  }

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontPythonLawInterface::treatKeyword(const std::string&,
					 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
					 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
  {
    using namespace std;
    return make_pair(false,current);
  } // end of treatKeyword

  MFrontPythonLawInterface::~MFrontPythonLawInterface()
  {}

  std::map<std::string,std::vector<std::string> >
  MFrontPythonLawInterface::getGlobalDependencies(const std::string& lib,
						  const std::string& mat,
						  const std::string&)
  {
    using namespace std;
    map<string,vector<string> > libs;
    string pylib;
    pylib = makeLowerCase(getMaterialLawLibraryNameBase(lib,mat));
    libs[pylib].push_back("-lm");
    return libs;
  } // end of MFrontPythonLawInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  MFrontPythonLawInterface::getGlobalIncludes(const std::string& lib,
					      const std::string& mat,
					      const std::string&)
  {
    using namespace std;
    map<string,vector<string> > inc;
    string pylib;
    pylib = makeLowerCase(getMaterialLawLibraryNameBase(lib,mat));
    inc[pylib].push_back(TFEL_PYTHON_INCLUDES);
    return inc;
  } // end of MFrontPythonLawInterface::getGeneratedSources

  std::map<std::string,std::vector<std::string> >
  MFrontPythonLawInterface::getGeneratedSources(const std::string& lib,
						const std::string& mat,
						const std::string& law)
  {
    using namespace std;
    map<string,vector<string> > src;
    string pylib;
    string pysrc;
    pylib = makeLowerCase(getMaterialLawLibraryNameBase(lib,mat));
    if(lib.empty()){
      if(!mat.empty()){
	pysrc = mat+"lawwrapper.cxx";
      } else {
	pysrc = "materiallawwrapper.cxx";
      }
    } else {
      pysrc = lib+"wrapper.cxx";
    }
    if(!mat.empty()){
      src[pylib].push_back(mat+"_"+law+"-python.cxx");
    } else {
      src[pylib].push_back(law+"-python.cxx");
    }
    src[pylib].push_back(pysrc);
    return src;
  } // end of MFrontPythonLawInterface::getGeneratedSources

  std::vector<std::string>
  MFrontPythonLawInterface::getGeneratedIncludes(const std::string&,
						 const std::string&,
						 const std::string&)
  {
    using namespace std;
    vector<string> incs;
    incs.push_back(this->headerFileName);
    return incs;
  } // end of MFrontPythonLawInterface::getGeneratedIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontPythonLawInterface::getLibrariesDependencies(const std::string& lib,
						     const std::string& mat,
						     const std::string&)
  {
    using namespace std;
    map<string,vector<string> > libs;
    string pylib;
    pylib = makeLowerCase(getMaterialLawLibraryNameBase(lib,mat));
    libs[pylib].push_back(TFEL_PYTHON_LIBS);
    return libs;
  } // end of MFrontPythonLawInterface::getLibrariesDependencies()

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  MFrontPythonLawInterface::getSpecificTargets(const std::string&,
					       const std::string&,
					       const std::string&,
					       const std::vector<std::string>&)
  {
    using namespace std;
    return map<string,pair<vector<string>,vector<string> > >();
  } // end of MFrontPythonLawInterface::getSpecificTargets

  void
  MFrontPythonLawInterface::writeOutputFiles(const std::string& file,
					     const std::string& lib,
					     const std::string& material,
					     const std::string& law,
					     const std::string& author,
					     const std::string& date,
					     const std::string& description,
					     const std::string& includes,
					     const std::string& output,
					     const VarContainer& inputs,
					     const std::vector<std::string>& materialLaws,
					     const std::map<std::string,std::string>&,
					     const std::map<std::string,std::string>&,
					     const StaticVarContainer& staticVars,
					     const std::vector<std::string>& params,
					     const std::map<std::string,double>& paramValues,
					     const LawFunction& function,
					     const std::vector<VariableBoundsDescription>& bounds,
					     const std::vector<VariableBoundsDescription>& physicalBounds,
					     const bool,
					     const std::vector<std::string>&)
  {
    using namespace std;
    using namespace tfel::system;
    vector<string>::const_iterator p;
    StaticVarContainer::const_iterator p2;
    VarContainer::const_iterator p3;
    map<string,string>::const_iterator p4;
    vector<VariableBoundsDescription>::const_iterator p5;
    map<string,double>::const_iterator p6;
    set<string>::const_iterator p7;
    string dir;
    string name;
    if(!material.empty()){
      name = material+"_"+law;
    } else {
      name = law;
    }
    this->headerFileName  = "include/" + name;
    this->headerFileName += "-python.hxx";
    this->headerFile.open(this->headerFileName.c_str());
    if(!this->headerFile){
      string msg("MFrontPythonLawInterface::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->headerFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->headerFile.exceptions(ios::badbit|ios::failbit);
    this->srcFileName     = "src/" + name;
    this->srcFileName    += "-python.cxx";
    this->srcFile.open(this->srcFileName.c_str());
    if(!this->srcFile){
      string msg("MFrontPythonLawInterface::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFile.exceptions(ios::badbit|ios::failbit);
    // writing header
    this->headerFile << "/*!" << endl;
    this->headerFile << "* \\file   " << this->headerFileName  << endl;
    this->headerFile << "* \\brief  " << "this file declares the " 
		     << law << " MaterialLaw.\n";
    this->headerFile << "*         File generated by ";
    this->headerFile << MFrontHeader::getVersionName() << " ";
    this->headerFile << "version " << MFrontHeader::getVersionNumber();
    this->headerFile << endl;
    if(!author.empty()){
      this->headerFile << "* \\author " << author << endl;
    }
    if(!date.empty()){
      this->headerFile << "* \\date   " << date       << endl;
    }
    if(!description.empty()){
      this->headerFile << description << endl;
    }
    this->headerFile << " */\n\n";

    this->headerFile << "#ifndef _" 
		     << makeUpperCase(name)
		     << "_PYTHON_HH\n";
    this->headerFile << "#define _"
		     << makeUpperCase(name)
		     << "_PYTHON_HH\n\n";
    this->headerFile << "#include <Python.h>\n\n";
    this->headerFile << "#ifdef __cplusplus\n";
    this->headerFile << "extern \"C\"{\n";
    this->headerFile << "#endif /* __cplusplus */\n\n";
    this->headerFile << "PyObject *\n"
		     << name << "_wrapper(";
    this->headerFile << "PyObject *,PyObject *);\n\n";
    this->headerFile << "#ifdef __cplusplus\n";
    this->headerFile << "} // end of extern \"C\"\n";
    this->headerFile << "#endif /* __cplusplus */\n\n";
    this->headerFile << "#endif /* _"
		     << makeUpperCase(name)
		     << "_PYTHON_HH */\n";

    this->headerFile.close();
    // writing source
    this->srcFile << "/*!" << endl;
    this->srcFile << "* \\file   " << this->srcFileName  << endl;
    this->srcFile << "* \\brief  " << "this file implements the python interface "
		  << "for the " << name << " materialLaw.\n";
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
    this->srcFile << " */\n\n";
    this->srcFile << "#include <Python.h>\n\n";
    this->srcFile << "#include<iostream>\n";
    this->srcFile << "#include<sstream>\n";
    this->srcFile << "#include<cmath>\n";
    this->srcFile << "#include<cstring>\n";
    this->srcFile << "#include<cstdlib>\n";
    this->srcFile << "#include<cstdio>\n";
    if(!includes.empty()){
      this->srcFile << includes << endl << endl;
    }
    this->srcFile << "#include\"" << name << "-python.hxx\"\n\n";
    this->srcFile << "#ifdef __cplusplus\n";
    this->srcFile << "extern \"C\"{\n";
    this->srcFile << "#endif /* __cplusplus */\n\n";
    this->srcFile << "PyObject *\n"
		  << name << "_wrapper(";
    this->srcFile << "PyObject *,PyObject * args)\n{\n";
    this->srcFile << "using namespace std;\n";
    this->srcFile << "typedef double real;\n";
    // material laws
    writeMaterialLaws("MFrontPythonLawInterface::writeOutputFile",
		      this->srcFile,materialLaws);
    // static variables
    writeStaticVariables("MFrontPythonLawInterface::writeOutputFile",
			 this->srcFile,staticVars,file,debugMode);
    // parameters
    if(!params.empty()){
      for(p=params.begin();p!=params.end();++p){
	p6 = paramValues.find(*p);
	if(p6==paramValues.end()){
	  string msg("MFrontPythonLawInterface::writeOutputFile : ");
	  msg += "internal error (can't find value of parameter " + *p + ")";
	  throw(runtime_error(msg));
	}
	this->srcFile << "static const double " << *p << " = " << p6->second << ";\n";
      }
    }
    if(!inputs.empty()){
      unsigned short i;
      for(p3=inputs.begin();p3!=inputs.end();++p3){
	this->srcFile << "double " << p3->name << ";\n";
      }
      if(!bounds.empty()){
	this->srcFile << "#ifndef PYTHON_NO_BOUNDS_CHECK\n";
	this->srcFile << "const char * policy;\n";
	this->srcFile << "#endif /* PYTHON_NO_BOUNDS_CHECK */\n";
      }

      this->srcFile << "if(!PyArg_ParseTuple(args,\"";
      for(i=0;i!=inputs.size();++i){
	this->srcFile << "d";
      }
      this->srcFile << "\",";
      for(p3=inputs.begin();p3!=inputs.end();){
	this->srcFile << "&" << p3->name;
	if(++p3!=inputs.end()){
	  this->srcFile << ",";
	}
      }
      this->srcFile << ")){\nreturn NULL;\n}\n";
      if((!physicalBounds.empty())||
	 (!bounds.empty())){
	this->srcFile << "#ifndef PYTHON_NO_BOUNDS_CHECK\n";
	if(!physicalBounds.empty()){
	  this->srcFile << "// treating physical bounds\n";
	  for(p5=physicalBounds.begin();
	      p5!=physicalBounds.end();++p5){
	    if(p5->boundsType==VariableBoundsDescription::Lower){
	      this->srcFile << "if(" << p5->varName<< " < "<< p5->lowerBound << "){\n";
	      this->srcFile << "ostringstream msg;\nmsg << \"" << name << " : "
			    << p5->varName << " is below its physical lower bound (\"\n << "
			    << p5->varName << " << \"<" << p5->lowerBound << ").\";\n";
	      this->srcFile << "PyErr_SetString(PyExc_RuntimeError,msg.str().c_str());\n";
	      this->srcFile << "return NULL;\n";
	      this->srcFile << "}\n";
	    } else if(p5->boundsType==VariableBoundsDescription::Upper){
	      this->srcFile << "if(" << p5->varName<< " > "<< p5->upperBound << "){\n";
	      this->srcFile << "ostringstream msg;\nmsg << \"" << name << " : "
			    << p5->varName << " is beyond its physical upper bound (\"\n << "
			    << p5->varName << " << \">" << p5->upperBound << ").\";\n";
	      this->srcFile << "PyErr_SetString(PyExc_RuntimeError,msg.str().c_str());\n";
	      this->srcFile << "return NULL;\n";
	      this->srcFile << "}\n";
	    } else {
	      this->srcFile << "if((" << p5->varName<< " < "<< p5->lowerBound << ")||"
			    << "(" << p5->varName<< " > "<< p5->upperBound << ")){\n";
	      this->srcFile << "if(" << p5->varName<< " < " << p5->lowerBound << "){\n";
	      this->srcFile << "ostringstream msg;\nmsg << \"" << name << " : "
			    << p5->varName << " is below its physical lower bound (\"\n << "
			    << p5->varName << " << \"<" << p5->lowerBound << ").\";\n";
	      this->srcFile << "PyErr_SetString(PyExc_RuntimeError,msg.str().c_str());\n";
	      this->srcFile << "return NULL;\n";
	      this->srcFile << "} else {\n";
	      this->srcFile << "ostringstream msg;\nmsg << \"" << name << " : "
			    << p5->varName << " is beyond its physical upper bound (\"\n << "
			    << p5->varName << " << \">" << p5->upperBound << ").\";\n";
	      this->srcFile << "PyErr_SetString(PyExc_RuntimeError,msg.str().c_str());\n";
	      this->srcFile << "return NULL;\n";
	      this->srcFile << "}\n";
	      this->srcFile << "}\n";
	    }
	  }
	}
	if(!bounds.empty()){
	  this->srcFile << "// treating standard bounds\n";
	  for(p5=bounds.begin();
	      p5!=bounds.end();++p5){
	    if((p5->boundsType==VariableBoundsDescription::Lower)||
	       (p5->boundsType==VariableBoundsDescription::LowerAndUpper)){
	      this->srcFile << "if(" << p5->varName<< " < "<< p5->lowerBound << "){\n";
	      this->srcFile << "policy = "
			    << "::getenv(\"PYTHON_OUT_OF_BOUND_POLICY\");\n";
	      this->srcFile << "if(policy!=0){\n";
	      this->srcFile << "if((strcmp(policy,\"STRICT\")==0)||"
			    << "(strcmp(policy,\"WARNING\")==0)){\n";
	      this->srcFile << "ostringstream msg;\n";
	      this->srcFile << "msg << \"" << name << " : "
			    << p5->varName << " is below its lower bound (\"\n << "
			    << p5->varName << " << \"<" << p5->lowerBound << ").\";\n";
	      this->srcFile << "if(strcmp(policy,\"STRICT\")==0){\n";
	      this->srcFile << "PyErr_SetString(PyExc_RuntimeError,msg.str().c_str());\n";
	      this->srcFile << "return NULL;\n";
	      this->srcFile << "} else {\n";
	      this->srcFile << "fprintf(stderr,\"%s\\n\",msg.str().c_str());\n";
	      this->srcFile << "}\n";
	      this->srcFile << "}\n";
	      this->srcFile << "}\n";
	      this->srcFile << "}\n";
	    }
	    if((p5->boundsType==VariableBoundsDescription::Upper)||
	       (p5->boundsType==VariableBoundsDescription::LowerAndUpper)){
	      this->srcFile << "if(" << p5->varName<< " > "<< p5->upperBound << "){\n";
	      this->srcFile << "policy = "
			    << "::getenv(\"PYTHON_OUT_OF_BOUND_POLICY\");\n";
	      this->srcFile << "if(policy!=0){\n";
	      this->srcFile << "if((strcmp(policy,\"STRICT\")==0)||"
			    << "(strcmp(policy,\"WARNING\")==0)){\n";
	      this->srcFile << "ostringstream msg;\n";
	      this->srcFile << "msg << \"" << name << " : "
			    << p5->varName << " is over its upper bound (\"\n << "
			    << p5->varName << " << \">" << p5->upperBound << ").\";\n";
	      this->srcFile << "if(strcmp(policy,\"STRICT\")==0){\n";
	      this->srcFile << "PyErr_SetString(PyExc_RuntimeError,msg.str().c_str());\n";
	      this->srcFile << "return NULL;\n";
	      this->srcFile << "} else {\n";
	      this->srcFile << "fprintf(stderr,\"%s\\n\",msg.str().c_str());\n";
	      this->srcFile << "}\n";
	      this->srcFile << "}\n";
	      this->srcFile << "}\n";
	      this->srcFile << "}\n";
	    }
	  }
	}
	this->srcFile << "#endif /* PYTHON_NO_BOUNDS_CHECK */\n";
      }
    }
    this->srcFile << "double " << output << ";\n";
    this->srcFile << function.body;
    this->srcFile << "return Py_BuildValue(\"d\"," << output << ");\n";
    this->srcFile << "} // end of " << name << "\n\n";
    this->srcFile << "#ifdef __cplusplus\n";
    this->srcFile << "} // end of extern \"C\"\n";
    this->srcFile << "#endif /* __cplusplus */\n\n";
    this->srcFile.close();
    // writing python interface
    MFrontLock& lock = MFrontLock::getMFrontLock();
    lock.lock();
    string fname;
    if(lib.empty()){
      if(!material.empty()){
	fname = "src/python"+material+"wrapper.lst";
      } else {
	fname = "src/pythonmaterialwrapper.lst";
      }
    } else {
      fname = "src/python"+lib+"wrapper.lst";
    }
    set<string> interfaces;
    struct stat buffer; // for call to stat
    if(access(fname.c_str(),F_OK)==0){
      stat(fname.c_str(),&buffer);
      if(!S_ISREG(buffer.st_mode)){
	string msg("MFrontPythonLawInterface::writeOutputFiles : '"+fname+"' is not a regular file");
	throw(runtime_error(msg));
      }
      ifstream iwrapper(fname.c_str());
      if(!iwrapper){
	string msg("MFrontPythonLawInterface::writeOutputFiles : ");
	msg += "unable to open '"+fname+"'";
	throw(runtime_error(msg));
      }
      copy(istream_iterator<string>(iwrapper),
	   istream_iterator<string>(),
	   insert_iterator<set<string> >(interfaces,interfaces.begin()));      
      iwrapper.close();
    }
    interfaces.insert(name);
    ofstream wrapper;
    if(lib.empty()){
      if(!material.empty()){
	wrapper.open(("src/"+material+"lawwrapper.cxx").c_str());
      } else {
	wrapper.open("src/materiallawwrapper.cxx");
      }
    } else {
      wrapper.open(("src/"+lib+"wrapper.cxx").c_str());
    }
    wrapper.exceptions(ios::badbit|ios::failbit);
    if(!wrapper){
      string msg("MFrontPythonLawInterface::writeOutputFiles : ");
      msg += "unable to open file";
      throw(runtime_error(msg));
    }
    wrapper << "/*!" << endl;
    if(lib.empty()){
      if(!material.empty()){
	wrapper << "* \\file   src/"+material+"lawwrapper.cxx" << endl;
      } else {
	wrapper << "* \\file   src/materiallawwrapper.cxx" << endl;
      }
    } else {
      wrapper << "* \\file   src/" << lib << "wrapper.cxx" << endl;
    }
    wrapper << "*         File generated by ";
    wrapper << MFrontHeader::getVersionName() << " ";
    wrapper << "version " << MFrontHeader::getVersionNumber();
    wrapper << endl;
    if(!author.empty()){
      wrapper << "* \\author " << author << endl;
    }
    if(!date.empty()){
      wrapper << "* \\date   " << date       << endl;
    }
    if(!description.empty()){
      wrapper << description << endl;
    }
    wrapper << " */\n\n";
    for(p7=interfaces.begin();p7!=interfaces.end();++p7){
      wrapper << "#include\"" << *p7 << "-python.hxx\"\n";
    }
    writeExportDirectives(wrapper);
    wrapper << endl;
    if(!material.empty()){
      wrapper << "static PyMethodDef " << material << "LawMethods[] = {\n";
    } else {
      wrapper << "static PyMethodDef MaterialLawMethods[] = {\n";
    }
    for(p7=interfaces.begin();p7!=interfaces.end();++p7){
      wrapper << "{\"" << *p7 << "\"," << *p7 << "_wrapper,METH_VARARGS,\n"
	      << "\"compute the " << *p7 <<  " law.\"},\n";
    }
    wrapper << "{NULL, NULL, 0, NULL} /* Sentinel */\n};\n\n";
    wrapper << "PyMODINIT_FUNC MFRONT_SHAREDOBJ\ninit"
	    << makeLowerCase(getMaterialLawLibraryNameBase(lib,material))
	    << "(void)\n";
    wrapper << "{\n";
    if(!material.empty()){
      wrapper << "(void) Py_InitModule(\""
	      << makeLowerCase(getMaterialLawLibraryNameBase(lib,material))
	      << "\"," << material << "LawMethods);\n";
    } else {
      wrapper << "(void) Py_InitModule(\"materiallaw\",MaterialLawMethods);\n";
    }
    wrapper << "} /* end of initmateriallaw */\n";
    wrapper.close();
    wrapper.open(fname.c_str());
    if(!wrapper){
      string msg("MFrontPythonLawInterface::writeOutputFiles : ");
      msg += "unable to open file '"+fname+"'";
      throw(runtime_error(msg));
    }
    copy(interfaces.begin(),interfaces.end(),
	 ostream_iterator<string>(wrapper,"\n"));
    wrapper.close();
    lock.unlock();
  } // end of MFrontPythonLawInterface::writeSrcFile(void)

} // end of namespace mfront
