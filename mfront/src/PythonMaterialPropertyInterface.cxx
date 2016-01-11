/*!
 * \file   mfront/src/PythonMaterialPropertyInterface.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<algorithm>
#include<sstream>
#include<stdexcept>
#include<iterator>
#include<string>
#include<set>

#include<sys/types.h>
#include<sys/stat.h>
#if ! (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
#include<unistd.h>
#else
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include<io.h>
#define F_OK    0       /* Test for existence.  */
#ifndef S_ISREG
#define S_ISREG(mode)  (((mode) & S_IFMT) == S_IFREG)
#endif
#endif

#include"TFEL/System/System.hxx"

#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontLock.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/MaterialPropertyDescription.hxx"
#include"MFront/CMaterialPropertyInterface.hxx"
#include"MFront/MaterialPropertyParametersHandler.hxx"
#include"MFront/PythonMaterialPropertyInterface.hxx"

#ifndef _MSC_VER
static const char * const constexpr_c = "constexpr";
#else
static const char * const constexpr_c = "const";
#endif

namespace mfront
{

  std::string
  PythonMaterialPropertyInterface::getName(void)
  {
    return "python";
  }

  PythonMaterialPropertyInterface::PythonMaterialPropertyInterface()
  {}

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  PythonMaterialPropertyInterface::treatKeyword(const std::string&,
					 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
					 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator)
  {
    return {false,current};
  } // end of treatKeyword

  PythonMaterialPropertyInterface::~PythonMaterialPropertyInterface()
  {}

  void
  PythonMaterialPropertyInterface::getTargetsDescription(TargetsDescription& d,
							 const MaterialPropertyDescription& mpd)
  {
    const auto lib = makeLowerCase(getMaterialLawLibraryNameBase(mpd.library,mpd.material));
    const auto name = (mpd.material.empty()) ? mpd.className : mpd.material+"_"+mpd.className;
    auto src = std::string{};
    if(mpd.library.empty()){
      if(!mpd.material.empty()){
	src = mpd.material+"lawwrapper.cxx";
      } else {
	src = "materiallawwrapper.cxx";
      }
    } else {
      src = mpd.library+"wrapper.cxx";
    }
    auto& l = d(lib,"");
    insert_if(l.ldflags,"-lm");    
    insert_if(l.ldflags,TFEL_PYTHON_LIBS);
    insert_if(l.cppflags,TFEL_PYTHON_INCLUDES);
    insert_if(l.sources,name+"-python.cxx");
    insert_if(l.sources,src);
    insert_if(l.epts,name);
    insert_if(d.headers,this->headerFileName);    
  } // end of PythonMaterialPropertyInterface::getTargetsDescription

  void
  PythonMaterialPropertyInterface::writeOutputFiles(const MaterialPropertyDescription& mpd,
						    const FileDescription& fd)
  {
    using namespace std;
    using namespace tfel::system;
    const auto& file=fd.fileName;
    const auto& author=fd.authorName;
    const auto& description=fd.description;
    const auto& date=fd.date;
    const auto& material=mpd.material;
    const auto& library=mpd.library;
    const auto& law=mpd.law;
    const auto& includes=mpd.includes;
    const auto& output=mpd.output;
    const auto& inputs=mpd.inputs;
    const auto& materialLaws=mpd.materialLaws;
    const auto& staticVars=mpd.staticVars;
    const auto& params=mpd.parameters;
    const auto& paramValues=mpd.parametersValues;
    const auto& function=mpd.f;
    const auto& bounds=mpd.boundsDescriptions;
    const auto& physicalBounds=mpd.physicalBoundsDescriptions;
    const auto name = (!material.empty()) ? material+"_"+law : law;
    this->headerFileName  = "include/" + name;
    this->headerFileName += "-python.hxx";
    this->headerFile.open(this->headerFileName);
    if(!this->headerFile){
      string msg("PythonMaterialPropertyInterface::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->headerFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->headerFile.exceptions(ios::badbit|ios::failbit);
    this->srcFileName     = "src/" + name;
    this->srcFileName    += "-python.cxx";
    this->srcFile.open(this->srcFileName);
    if(!this->srcFile){
      string msg("PythonMaterialPropertyInterface::writeOutputFiles : ");
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

    this->headerFile << "#ifndef " << makeUpperCase(name) << "_PYTHON_HH\n";
    this->headerFile << "#define " << makeUpperCase(name) << "_PYTHON_HH\n\n";
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
    this->headerFile << "#endif /* " << makeUpperCase(name) << "_PYTHON_HH */\n";

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
    this->srcFile << "#include<fstream>\n";
    this->srcFile << "#include<sstream>\n";
    this->srcFile << "#include<string>\n";
    this->srcFile << "#include<cmath>\n";
    this->srcFile << "#include<algorithm>\n";
    this->srcFile << "#include<cstring>\n";
    this->srcFile << "#include<cstdlib>\n";
    this->srcFile << "#include<cstdio>\n";
    if(!includes.empty()){
      this->srcFile << includes << endl << endl;
    }
    this->srcFile << "#include\"" << name << "-python.hxx\"\n\n";
    writeMaterialPropertyParametersHandler(this->srcFile,mpd,
					   name,"double","python");
    this->srcFile << "#ifdef __cplusplus\n";
    this->srcFile << "extern \"C\"{\n";
    this->srcFile << "#endif /* __cplusplus */\n\n";
    if(!inputs.empty()){
      this->srcFile << "PyObject *\n" << name << "_wrapper(";
      this->srcFile << "PyObject *,PyObject * py_args_)\n{\n";
    } else {
      this->srcFile << "PyObject *\n" << name << "_wrapper(";
      this->srcFile << "PyObject *,PyObject*)\n{\n";
    }
    this->srcFile << "using namespace std;\n";
    this->srcFile << "typedef double real;\n";
    // material laws
    writeMaterialLaws("PythonMaterialPropertyInterface::writeOutputFile",
		      this->srcFile,materialLaws);
    // static variables
    writeStaticVariables("PythonMaterialPropertyInterface::writeOutputFile",
			 this->srcFile,staticVars,file);
    this->srcFile << "auto throwPythonRuntimeException = [](const string& msg){\n"
		  << "  PyErr_SetString(PyExc_RuntimeError,msg.c_str());\n"
		  << "  return nullptr;\n"
		  << "};\n";
    // parameters
    if(!mpd.parameters.empty()){
      const auto hn = getMaterialPropertyParametersHandlerClassName(name);
      this->srcFile << "if(!python::" <<  hn << "::get" << hn << "().ok){\n"
		    << "return throwPythonRuntimeException(python::"<< name << "MaterialPropertyHandler::get"
		    << name << "MaterialPropertyHandler().msg);\n"
		    << "}\n";
      writeAssignMaterialPropertyParameters(this->srcFile,mpd,name,
					    "double","python");
    }
    if(!inputs.empty()){
      unsigned short i;
      for(auto p3=inputs.begin();p3!=inputs.end();++p3){
	this->srcFile << "double " << p3->name << ";\n";
      }
      if(!bounds.empty()){
	this->srcFile << "#ifndef PYTHON_NO_BOUNDS_CHECK\n";
	this->srcFile << "const char * policy;\n";
	this->srcFile << "#endif /* PYTHON_NO_BOUNDS_CHECK */\n";
      }
      this->srcFile << "if(!PyArg_ParseTuple(py_args_,\"";
      for(i=0;i!=inputs.size();++i){
	this->srcFile << "d";
      }
      this->srcFile << "\",";
      for(auto p3=inputs.begin();p3!=inputs.end();){
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
	  for(auto p5=physicalBounds.begin();p5!=physicalBounds.end();++p5){
	    if(p5->boundsType==VariableBoundsDescription::Lower){
	      this->srcFile << "if(" << p5->varName<< " < "<< p5->lowerBound << "){\n";
	      this->srcFile << "ostringstream msg;\nmsg << \"" << name << " : "
			    << p5->varName << " is below its physical lower bound (\"\n << "
			    << p5->varName << " << \"<" << p5->lowerBound << ").\";\n";
	      this->srcFile << "return throwPythonRuntimeException(msg.str());\n";
	      this->srcFile << "}\n";
	    } else if(p5->boundsType==VariableBoundsDescription::Upper){
	      this->srcFile << "if(" << p5->varName<< " > "<< p5->upperBound << "){\n";
	      this->srcFile << "ostringstream msg;\nmsg << \"" << name << " : "
			    << p5->varName << " is beyond its physical upper bound (\"\n << "
			    << p5->varName << " << \">" << p5->upperBound << ").\";\n";
	      this->srcFile << "return throwPythonRuntimeException(msg.str());\n";
	      this->srcFile << "}\n";
	    } else {
	      this->srcFile << "if((" << p5->varName<< " < "<< p5->lowerBound << ")||"
			    << "(" << p5->varName<< " > "<< p5->upperBound << ")){\n";
	      this->srcFile << "if(" << p5->varName<< " < " << p5->lowerBound << "){\n";
	      this->srcFile << "ostringstream msg;\nmsg << \"" << name << " : "
			    << p5->varName << " is below its physical lower bound (\"\n << "
			    << p5->varName << " << \"<" << p5->lowerBound << ").\";\n";
	      this->srcFile << "return throwPythonRuntimeException(msg.str());\n";
	      this->srcFile << "} else {\n";
	      this->srcFile << "ostringstream msg;\nmsg << \"" << name << " : "
			    << p5->varName << " is beyond its physical upper bound (\"\n << "
			    << p5->varName << " << \">" << p5->upperBound << ").\";\n";
	      this->srcFile << "return throwPythonRuntimeException(msg.str());\n";
	      this->srcFile << "}\n";
	      this->srcFile << "}\n";
	    }
	  }
	}
	if(!bounds.empty()){
	  this->srcFile << "// treating standard bounds\n";
	  for(auto p5=bounds.begin();p5!=bounds.end();++p5){
	    if((p5->boundsType==VariableBoundsDescription::Lower)||
	       (p5->boundsType==VariableBoundsDescription::LowerAndUpper)){
	      this->srcFile << "if(" << p5->varName<< " < "<< p5->lowerBound << "){\n";
	      this->srcFile << "policy = "
			    << "::getenv(\"PYTHON_OUT_OF_BOUNDS_POLICY\");\n";
	      this->srcFile << "if(policy!=nullptr){\n";
	      this->srcFile << "if((strcmp(policy,\"STRICT\")==0)||"
			    << "(strcmp(policy,\"WARNING\")==0)){\n";
	      this->srcFile << "ostringstream msg;\n";
	      this->srcFile << "msg << \"" << name << " : "
			    << p5->varName << " is below its lower bound (\"\n << "
			    << p5->varName << " << \"<" << p5->lowerBound << ").\";\n";
	      this->srcFile << "if(strcmp(policy,\"STRICT\")==0){\n";
	      this->srcFile << "return throwPythonRuntimeException(msg.str());\n";
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
			    << "::getenv(\"PYTHON_OUT_OF_BOUNDS_POLICY\");\n";
	      this->srcFile << "if(policy!=nullptr){\n";
	      this->srcFile << "if((strcmp(policy,\"STRICT\")==0)||"
			    << "(strcmp(policy,\"WARNING\")==0)){\n";
	      this->srcFile << "ostringstream msg;\n";
	      this->srcFile << "msg << \"" << name << " : "
			    << p5->varName << " is over its upper bound (\"\n << "
			    << p5->varName << " << \">" << p5->upperBound << ").\";\n";
	      this->srcFile << "if(strcmp(policy,\"STRICT\")==0){\n";
	      this->srcFile << "return throwPythonRuntimeException(msg.str());\n";
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
    this->srcFile << "double " << output << ";\n"
		  << "try{\n"
		  << function.body
		  << "} catch(exception& cpp_except){\n"
		  << "  return throwPythonRuntimeException(cpp_except.what());\n"
		  << "} catch(...){\n"
		  << "  return throwPythonRuntimeException(\"unknown C++ exception\");\n"
		  << "}\n"
		  << "return Py_BuildValue(\"d\"," << output << ");\n";
    this->srcFile << "} // end of " << name << "\n\n";
    this->srcFile << "#ifdef __cplusplus\n";
    this->srcFile << "} // end of extern \"C\"\n";
    this->srcFile << "#endif /* __cplusplus */\n\n";
    this->srcFile.close();
    // writing python interface
    MFrontLockGuard lock;
    string fname;
    if(library.empty()){
      if(!material.empty()){
	fname = "src/python"+material+"wrapper.lst";
      } else {
	fname = "src/pythonmaterialwrapper.lst";
      }
    } else {
      fname = "src/python"+library+"wrapper.lst";
    }
    set<string> interfaces;
    struct stat buffer; // for call to stat
    if(access(fname.c_str(),F_OK)==0){
      if(stat(fname.c_str(),&buffer)==-1){
	throw(runtime_error("PythonMaterialPropertyInterface::writeOutputFiles : "
			    "can't stat file '"+fname+"'"));
      }
      if(!S_ISREG(buffer.st_mode)){
	throw(runtime_error("PythonMaterialPropertyInterface::writeOutputFiles : "
			    "'"+fname+"' is not a regular file"));
      }
      ifstream iwrapper(fname);
      if(!iwrapper){
	string msg("PythonMaterialPropertyInterface::writeOutputFiles : ");
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
    if(library.empty()){
      if(!material.empty()){
	wrapper.open(("src/"+material+"lawwrapper.cxx").c_str());
      } else {
	wrapper.open("src/materiallawwrapper.cxx");
      }
    } else {
      wrapper.open(("src/"+library+"wrapper.cxx").c_str());
    }
    wrapper.exceptions(ios::badbit|ios::failbit);
    if(!wrapper){
      string msg("PythonMaterialPropertyInterface::writeOutputFiles : ");
      msg += "unable to open file";
      throw(runtime_error(msg));
    }
    wrapper << "/*!" << endl;
    if(library.empty()){
      if(!material.empty()){
	wrapper << "* \\file   src/"+material+"lawwrapper.cxx" << endl;
      } else {
	wrapper << "* \\file   src/materiallawwrapper.cxx" << endl;
      }
    } else {
      wrapper << "* \\file   src/" << library << "wrapper.cxx" << endl;
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
    for(auto p7=interfaces.begin();p7!=interfaces.end();++p7){
      wrapper << "#include\"" << *p7 << "-python.hxx\"\n";
    }
    wrapper << endl;
#ifndef _WIN32
    writeExportDirectives(wrapper);
    wrapper << endl;
#endif /* _WIN32 */
    if(!material.empty()){
      wrapper << "static PyMethodDef " << material << "LawMethods[] = {\n";
    } else {
      wrapper << "static PyMethodDef MaterialLawMethods[] = {\n";
    }
    for(auto p7=interfaces.begin();p7!=interfaces.end();++p7){
      wrapper << "{\"" << *p7 << "\"," << *p7 << "_wrapper,METH_VARARGS,\n"
	      << "\"compute the " << *p7 <<  " law.\"},\n";
    }
    wrapper << "{NULL, NULL, 0, NULL} /* Sentinel */\n};\n\n";
#ifndef _WIN32
    wrapper << "PyMODINIT_FUNC MFRONT_SHAREDOBJ\n"
#else /* _WIN32 */
    wrapper << "PyMODINIT_FUNC\n"
#endif /* _WIN32 */
	    << "init" << makeLowerCase(getMaterialLawLibraryNameBase(library,material))
	    << "(void)\n";
    wrapper << "{\n";
    if(!material.empty()){
      wrapper << "(void) Py_InitModule(\""
	      << makeLowerCase(getMaterialLawLibraryNameBase(library,material))
	      << "\"," << material << "LawMethods);\n";
    } else {
      wrapper << "(void) Py_InitModule(\"materiallaw\",MaterialLawMethods);\n";
    }
    wrapper << "} /* end of initmateriallaw */\n";
    wrapper.close();
    wrapper.open(fname);
    if(!wrapper){
      string msg("PythonMaterialPropertyInterface::writeOutputFiles : ");
      msg += "unable to open file '"+fname+"'";
      throw(runtime_error(msg));
    }
    copy(interfaces.begin(),interfaces.end(),
	 ostream_iterator<string>(wrapper,"\n"));
    wrapper.close();
  } // end of PythonMaterialPropertyInterface::writeSrcFile(void)

} // end of namespace mfront
