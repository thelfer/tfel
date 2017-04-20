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
#include<stdexcept>
#include<iterator>
#include<sstream>
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

namespace mfront
{

  std::string
  PythonMaterialPropertyInterface::getName()
  {
    return "python";
  }

  PythonMaterialPropertyInterface::PythonMaterialPropertyInterface() = default;

  std::pair<bool,PythonMaterialPropertyInterface::tokens_iterator>
  PythonMaterialPropertyInterface::treatKeyword(const std::string& k,
						const std::vector<std::string>& i,
						tokens_iterator current,
						const tokens_iterator)
  {
    if((std::find(i.begin(),i.end(),"python")!=i.end())||
       (std::find(i.begin(),i.end(),"Python")!=i.end())){
      throw(std::runtime_error("PythonMaterialPropertyInterface::treatKeyword: "
			       "unsupported keyword '"+k+"'"));
    }
    return {false,current};
  } // end of treatKeyword

  void
  PythonMaterialPropertyInterface::getTargetsDescription(TargetsDescription& d,
							 const MaterialPropertyDescription& mpd)
  {
    const auto lib = makeLowerCase(getMaterialLawLibraryNameBase(mpd.library,mpd.material));
    const auto name = (mpd.material.empty()) ? mpd.className : mpd.material+"_"+mpd.className;
    const auto headerFileName  = "include/"+name+"-python.hxx";
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
    insert_if(d.headers,headerFileName);
  } // end of PythonMaterialPropertyInterface::getTargetsDescription

  static void writePhysicalBounds(std::ostream& out,
				  const std::string& name,
				  const VariableDescription& v){
    if(!v.hasPhysicalBounds()){
      return;
    }
    const auto& b = v.getPhysicalBounds();
    if(b.boundsType==VariableBoundsDescription::LOWER){
      out << "if(" << v.name<< " < "<< b.lowerBound << "){\n"
	  << "ostringstream msg;\nmsg << \"" << name << " : "
	  << v.name << " is below its physical lower bound (\"\n << "
	  << v.name << " << \"<" << b.lowerBound << ").\";\n"
	  << "return throwPythonRuntimeException(msg.str());\n"
	  << "}\n";
    } else if(b.boundsType==VariableBoundsDescription::UPPER){
      out << "if(" << v.name<< " > "<< b.upperBound << "){\n"
	  << "ostringstream msg;\nmsg << \"" << name << " : "
	  << v.name << " is beyond its physical upper bound (\"\n << "
	  << v.name << " << \">" << b.upperBound << ").\";\n"
	  << "return throwPythonRuntimeException(msg.str());\n"
	  << "}\n";
    } else {
      out << "if((" << v.name<< " < "<< b.lowerBound << ")||"
	  << "(" << v.name<< " > "<< b.upperBound << ")){\n"
	  << "if(" << v.name<< " < " << b.lowerBound << "){\n"
	  << "ostringstream msg;\nmsg << \"" << name << " : "
	  << v.name << " is below its physical lower bound (\"\n << "
	  << v.name << " << \"<" << b.lowerBound << ").\";\n"
	  << "return throwPythonRuntimeException(msg.str());\n"
	  << "} else {\n"
	  << "ostringstream msg;\nmsg << \"" << name << " : "
	  << v.name << " is beyond its physical upper bound (\"\n << "
	  << v.name << " << \">" << b.upperBound << ").\";\n"
	  << "return throwPythonRuntimeException(msg.str());\n"
	  << "}\n"
	  << "}\n";
    }
  }

  static void writeBounds(std::ostream& out,
			  const std::string& name,
			  const VariableDescription& v){
    if(!v.hasBounds()){
      return;
    }
    const auto& b = v.getBounds();
    if((b.boundsType==VariableBoundsDescription::LOWER)||
       (b.boundsType==VariableBoundsDescription::LOWERANDUPPER)){
      out << "if(" << v.name<< " < "<< b.lowerBound << "){\n"
	  << "policy = "
	  << "::getenv(\"PYTHON_OUT_OF_BOUNDS_POLICY\");\n"
	  << "if(policy!=nullptr){\n"
	  << "if((strcmp(policy,\"STRICT\")==0)||"
	  << "(strcmp(policy,\"WARNING\")==0)){\n"
	  << "ostringstream msg;\n"
	  << "msg << \"" << name << " : "
	  << v.name << " is below its lower bound (\"\n << "
	  << v.name << " << \"<" << b.lowerBound << ").\";\n"
	  << "if(strcmp(policy,\"STRICT\")==0){\n"
	  << "return throwPythonRuntimeException(msg.str());\n"
	  << "} else {\n"
	  << "fprintf(stderr,\"%s\\n\",msg.str().c_str());\n"
	  << "}\n"
	  << "}\n"
	  << "}\n"
	  << "}\n";
    }
    if((b.boundsType==VariableBoundsDescription::UPPER)||
       (b.boundsType==VariableBoundsDescription::LOWERANDUPPER)){
      out << "if(" << v.name<< " > "<< b.upperBound << "){\n"
	  << "policy = "
	  << "::getenv(\"PYTHON_OUT_OF_BOUNDS_POLICY\");\n"
	  << "if(policy!=nullptr){\n"
	  << "if((strcmp(policy,\"STRICT\")==0)||"
	  << "(strcmp(policy,\"WARNING\")==0)){\n"
	  << "ostringstream msg;\n"
	  << "msg << \"" << name << " : "
	  << v.name << " is over its upper bound (\"\n << "
	  << v.name << " << \">" << b.upperBound << ").\";\n"
	  << "if(strcmp(policy,\"STRICT\")==0){\n"
	  << "return throwPythonRuntimeException(msg.str());\n"
	  << "} else {\n"
	  << "fprintf(stderr,\"%s\\n\",msg.str().c_str());\n"
	  << "}\n"
	  << "}\n"
	  << "}\n"
	  << "}\n";
    }
  }

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
    const auto& function=mpd.f;
    const auto name = (!material.empty()) ? material+"_"+law : law;
    const auto outName  = "include/"+name+"-python.hxx";
    std::ofstream out;
    out.open(outName);
    if(!out){
      string msg("PythonMaterialPropertyInterface::writeOutputFiles : ");
      msg += "unable to open ";
      msg += outName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    out.exceptions(ios::badbit|ios::failbit);
    const auto srcFileName = "src/"+name+"-python.cxx";
    std::ofstream srcFile;
    srcFile.open(srcFileName);
    if(!srcFile){
      string msg("PythonMaterialPropertyInterface::writeOutputFiles : ");
      msg += "unable to open ";
      msg += srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    srcFile.exceptions(ios::badbit|ios::failbit);
    // writing header
    out << "/*!\n"
	<< "* \\file   " << outName  << '\n'
	<< "* \\brief  " << "this file declares the " 
	<< law << " MaterialLaw.\n"
	<< "*         File generated by "
	<< MFrontHeader::getVersionName() << " "
	<< "version " << MFrontHeader::getVersionNumber()
	<< '\n';
    if(!author.empty()){
      out << "* \\author " << author << '\n';
    }
    if(!date.empty()){
      out << "* \\date   " << date << '\n';
    }
    if(!description.empty()){
      out << description << '\n';
    }
    out << " */\n\n";
    out << "#ifndef " << makeUpperCase(name) << "_PYTHON_HH\n"
	<< "#define " << makeUpperCase(name) << "_PYTHON_HH\n\n"
	<< "#include <Python.h>\n\n"
	<< "#ifdef __cplusplus\n"
	<< "extern \"C\"{\n"
	<< "#endif /* __cplusplus */\n\n"
	<< "PyObject *\n"
	<< name << "_wrapper("
	<< "PyObject *,PyObject *);\n\n"
	<< "#ifdef __cplusplus\n"
	<< "} // end of extern \"C\"\n"
	<< "#endif /* __cplusplus */\n\n"
	<< "#endif /* " << makeUpperCase(name) << "_PYTHON_HH */\n";
    out.close();
    // writing source
    srcFile << "/*!\n"
	    << "* \\file   " << srcFileName  << '\n'
	    << "* \\brief  " << "this file implements the python interface "
	    << "for the " << name << " materialLaw.\n"
	    << "*         File generated by "
	    << MFrontHeader::getVersionName() << " "
	    << "version " << MFrontHeader::getVersionNumber()
	    << '\n';
    if(!author.empty()){
      srcFile << "* \\author " << author << '\n';
    }
    if(!date.empty()){
      srcFile << "* \\date   " << date << '\n';
    }
    srcFile << " */\n\n"
	    << "#include <Python.h>\n\n"
	    << "#include<algorithm>\n"
	    << "#include<iostream>\n"
      	    << "#include<iterator>\n"
	    << "#include<fstream>\n"
	    << "#include<sstream>\n"
	    << "#include<string>\n"
	    << "#include<cmath>\n"
	    << "#include<cstring>\n"
	    << "#include<cstdlib>\n"
	    << "#include<cstdio>\n";
    if(!includes.empty()){
      srcFile << includes << "\n\n";
    }
    srcFile << "#include\"" << name << "-python.hxx\"\n\n";
    writeMaterialPropertyParametersHandler(srcFile,mpd,
					   name,"double","python");
    srcFile << "#ifdef __cplusplus\n"
	    << "extern \"C\"{\n"
	    << "#endif /* __cplusplus */\n\n";
    if(!inputs.empty()){
      srcFile << "PyObject *\n" << name << "_wrapper("
	      << "PyObject *,PyObject * py_args_)\n{\n";
    } else {
      srcFile << "PyObject *\n" << name << "_wrapper("
	      << "PyObject *,PyObject*)\n{\n";
    }
    srcFile << "using namespace std;\n"
	    << "using real = double;\n";
    // material laws
    writeMaterialLaws("PythonMaterialPropertyInterface::writeOutputFile",
		      srcFile,materialLaws);
    // static variables
    writeStaticVariables("PythonMaterialPropertyInterface::writeOutputFile",
			 srcFile,staticVars,file);
    srcFile << "auto throwPythonRuntimeException = [](const string& msg){\n"
	    << "  PyErr_SetString(PyExc_RuntimeError,msg.c_str());\n"
	    << "  return nullptr;\n"
	    << "};\n";
    // parameters
    if(!mpd.parameters.empty()){
      const auto hn = getMaterialPropertyParametersHandlerClassName(name);
      srcFile << "if(!python::" <<  hn << "::get" << hn << "().ok){\n"
	      << "return throwPythonRuntimeException(python::"<< name << "MaterialPropertyHandler::get"
	      << name << "MaterialPropertyHandler().msg);\n"
	      << "}\n";
      writeAssignMaterialPropertyParameters(srcFile,mpd,name,
					    "double","python");
    }
    for(const auto& i : inputs){
      srcFile << "real " << i.name << ";\n";
    }
    if(hasBounds(mpd.inputs)||hasBounds(mpd.output)){
      srcFile << "#ifndef PYTHON_NO_BOUNDS_CHECK\n";
      srcFile << "const char * policy;\n";
      srcFile << "#endif /* PYTHON_NO_BOUNDS_CHECK */\n";
    }
    if(!inputs.empty()){
      srcFile << "if(!PyArg_ParseTuple(py_args_,\"";
      unsigned short i;
      for(i=0;i!=inputs.size();++i){
	srcFile << "d";
      }
      srcFile << "\",";
      for(auto p3=inputs.begin();p3!=inputs.end();){
	srcFile << "&" << p3->name;
	if(++p3!=inputs.end()){
	  srcFile << ",";
	}
      }
      srcFile << ")){\nreturn NULL;\n}\n";
    }
    if((hasPhysicalBounds(mpd.inputs))||(hasBounds(mpd.inputs))){
      srcFile << "#ifndef PYTHON_NO_BOUNDS_CHECK\n";
      if(hasPhysicalBounds(mpd.inputs)){
	srcFile << "// treating physical bounds\n";
	for(const auto& i:mpd.inputs){
	  writePhysicalBounds(srcFile,name,i);
	}
      }
      if(hasBounds(mpd.inputs)){
	srcFile << "// treating standard bounds\n";
	for(const auto& i:mpd.inputs){
	  writeBounds(srcFile,name,i);
	}
      }
      srcFile << "#endif /* PYTHON_NO_BOUNDS_CHECK */\n";
    }
    srcFile << "real " << output.name << ";\n"
	    << "try{\n"
	    << function.body
	    << "} catch(exception& cpp_except){\n"
	    << "  return throwPythonRuntimeException(cpp_except.what());\n"
	    << "} catch(...){\n"
	    << "  return throwPythonRuntimeException(\"unknown C++ exception\");\n"
	    << "}\n";
    if((hasPhysicalBounds(mpd.output))||(hasBounds(mpd.output))){
      srcFile << "#ifndef PYTHON_NO_BOUNDS_CHECK\n";
      if(hasPhysicalBounds(mpd.output)){
	srcFile << "// treating physical bounds\n";
	writePhysicalBounds(srcFile,name,mpd.output);
      }
      if(hasBounds(mpd.output)){
	srcFile << "// treating standard bounds\n";
	writeBounds(srcFile,name,mpd.output);
      }
      srcFile << "#endif /* PYTHON_NO_BOUNDS_CHECK */\n";
    }
    srcFile << "return Py_BuildValue(\"d\"," << output.name << ");\n"
	    << "} // end of " << name << "\n\n"
	    << "#ifdef __cplusplus\n"
	    << "} // end of extern \"C\"\n"
	    << "#endif /* __cplusplus */\n\n";
    srcFile.close();
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
    wrapper << "/*!\n";
    if(library.empty()){
      if(!material.empty()){
	wrapper << "* \\file   src/"+material+"lawwrapper.cxx\n";
      } else {
	wrapper << "* \\file   src/materiallawwrapper.cxx\n";
      }
    } else {
      wrapper << "* \\file   src/" << library << "wrapper.cxx\n";
    }
    wrapper << "*         File generated by ";
    wrapper << MFrontHeader::getVersionName() << " ";
    wrapper << "version " << MFrontHeader::getVersionNumber();
    wrapper << '\n';
    if(!author.empty()){
      wrapper << "* \\author " << author << '\n';
    }
    if(!date.empty()){
      wrapper << "* \\date   " << date       << '\n';
    }
    if(!description.empty()){
      wrapper << description << '\n';
    }
    wrapper << " */\n\n";
    for(const auto& i : interfaces){
      wrapper << "#include\"" << i << "-python.hxx\"\n";
    }
    wrapper << '\n';
#ifndef _WIN32
    writeExportDirectives(wrapper);
    wrapper << '\n';
#endif /* _WIN32 */
    if(!material.empty()){
      wrapper << "static PyMethodDef " << material << "LawMethods[] = {\n";
    } else {
      wrapper << "static PyMethodDef MaterialLawMethods[] = {\n";
    }
    for(const auto& i : interfaces){
      wrapper << "{\"" << i << "\"," << i << "_wrapper,METH_VARARGS,\n"
	      << "\"compute the " << i <<  " law.\"},\n";
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

  PythonMaterialPropertyInterface::~PythonMaterialPropertyInterface() = default;
  
} // end of namespace mfront
