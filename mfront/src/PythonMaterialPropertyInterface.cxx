/*!
 * \file   mfront/src/PythonMaterialPropertyInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <sstream>
#include <cstdlib>
#include <string>
#include <set>

#include <sys/types.h>
#include <sys/stat.h>
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
#include <unistd.h>
#else
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <io.h>
#define F_OK 0 /* Test for existence.  */
#ifndef S_ISREG
#define S_ISREG(mode) (((mode)&S_IFMT) == S_IFREG)
#endif
#endif

#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/System/System.hxx"

#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/MFrontLock.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/CMaterialPropertyInterface.hxx"
#include "MFront/MaterialPropertyParametersHandler.hxx"
#include "MFront/PythonMaterialPropertyInterface.hxx"

namespace mfront {

  std::string PythonMaterialPropertyInterface::getName() { return "python"; }

  PythonMaterialPropertyInterface::PythonMaterialPropertyInterface() = default;

  std::pair<bool, PythonMaterialPropertyInterface::tokens_iterator>
  PythonMaterialPropertyInterface::treatKeyword(
      const std::string& k,
      const std::vector<std::string>& i,
      tokens_iterator current,
      const tokens_iterator) {
    tfel::raise_if((std::find(i.begin(), i.end(), "python") != i.end()) ||
                       (std::find(i.begin(), i.end(), "Python") != i.end()),
                   "PythonMaterialPropertyInterface::treatKeyword: "
                   "unsupported keyword '" +
                       k + "'");
    return {false, current};
  }  // end of treatKeyword

  void PythonMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) const {
    const auto lib = makeLowerCase(getMaterialLawLibraryNameBase(mpd));
    const auto name = (mpd.material.empty())
                          ? mpd.className
                          : mpd.material + "_" + mpd.className;
    const auto headerFileName = "include/" + name + "-python.hxx";
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    const auto* const python_include_path = std::getenv("PYTHON_INCLUDE_PATH");
    const auto* const python_library_path = std::getenv("PYTHON_LIBRARY_PATH");
    const auto* const python_library = std::getenv("PYTHON_LIBRARY");
    const auto src = [&mpd]() -> std::string {
      if (mpd.library.empty()) {
        if (!mpd.material.empty()) {
          return mpd.material + "lawwrapper.cxx";
        } else {
          return "materiallawwrapper.cxx";
        }
      }
      return mpd.library + "wrapper.cxx";
    }();
    auto& l = [&d, &lib]() -> LibraryDescription& {
      if (d.system == LibraryDescription::WINDOWS) {
        return d(lib, "", "pyd", LibraryDescription::MODULE);
      } else if (d.system == LibraryDescription::MACOSX) {
        return d(lib, "", "so", LibraryDescription::MODULE);
      } else {
        const auto ls = LibraryDescription::getDefaultLibrarySuffix(
            d.system, d.libraryType);
        return d(lib, "", ls, LibraryDescription::MODULE);
      }
    }();
    if (python_include_path != nullptr) {
      insert_if(l.include_directories, python_include_path);
    } else {
      insert_if(l.cppflags, TFEL_PYTHON_INCLUDES);
    }
    insert_if(l.cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
    insert_if(l.include_directories,
              "$(shell " + tfel_config + " --include-path)");
#if !((defined _WIN32) && (defined _MSC_VER))
    insert_if(l.link_libraries, "m");
#endif /* !((defined _WIN32) && (defined _MSC_VER)) */
#ifdef TFEL_PYTHON_LIBRARY_PATH
    if (python_library_path != nullptr) {
      insert_if(l.link_directories, python_library_path);
    } else {
      insert_if(l.link_directories, TFEL_PYTHON_LIBRARY_PATH);
    }
    if (python_library != nullptr) {
      insert_if(l.link_libraries, python_library);
    } else {
      insert_if(l.link_libraries, TFEL_PYTHON_LIBRARY);
    }
#else
    insert_if(l.ldflags, TFEL_PYTHON_LIBS);
#endif
    insert_if(l.sources, name + "-python.cxx");
    insert_if(l.sources, src);
    insert_if(l.epts, name);
    insert_if(d.headers, headerFileName);
  }  // end of PythonMaterialPropertyInterface::getTargetsDescription

  static void writePhysicalBounds(std::ostream& out,
                                  const std::string& name,
                                  const VariableDescription& v) {
    if (!v.hasPhysicalBounds()) {
      return;
    }
    const auto& b = v.getPhysicalBounds();
    if (b.boundsType == VariableBoundsDescription::LOWER) {
      out << "if(" << v.name << " < " << b.lowerBound << "){\n"
          << "ostringstream msg;\nmsg << \"" << name << " : " << v.name
          << " is below its physical lower bound (\"\n << " << v.name
          << " << \"<" << b.lowerBound << ").\";\n"
          << "return throwPythonRuntimeException(msg.str());\n"
          << "}\n";
    } else if (b.boundsType == VariableBoundsDescription::UPPER) {
      out << "if(" << v.name << " > " << b.upperBound << "){\n"
          << "ostringstream msg;\nmsg << \"" << name << " : " << v.name
          << " is beyond its physical upper bound (\"\n << " << v.name
          << " << \">" << b.upperBound << ").\";\n"
          << "return throwPythonRuntimeException(msg.str());\n"
          << "}\n";
    } else {
      out << "if((" << v.name << " < " << b.lowerBound << ")||"
          << "(" << v.name << " > " << b.upperBound << ")){\n"
          << "if(" << v.name << " < " << b.lowerBound << "){\n"
          << "ostringstream msg;\nmsg << \"" << name << " : " << v.name
          << " is below its physical lower bound (\"\n << " << v.name
          << " << \"<" << b.lowerBound << ").\";\n"
          << "return throwPythonRuntimeException(msg.str());\n"
          << "} else {\n"
          << "ostringstream msg;\nmsg << \"" << name << " : " << v.name
          << " is beyond its physical upper bound (\"\n << " << v.name
          << " << \">" << b.upperBound << ").\";\n"
          << "return throwPythonRuntimeException(msg.str());\n"
          << "}\n"
          << "}\n";
    }
  }

  static void writeBounds(std::ostream& out,
                          const std::string& name,
                          const VariableDescription& v) {
    if (!v.hasBounds()) {
      return;
    }
    const auto& b = v.getBounds();
    if ((b.boundsType == VariableBoundsDescription::LOWER) ||
        (b.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
      out << "if(" << v.name << " < " << b.lowerBound << "){\n"
          << "policy = "
          << "::getenv(\"PYTHON_OUT_OF_BOUNDS_POLICY\");\n"
          << "if(policy!=nullptr){\n"
          << "if((strcmp(policy,\"STRICT\")==0)||"
          << "(strcmp(policy,\"WARNING\")==0)){\n"
          << "ostringstream msg;\n"
          << "msg << \"" << name << " : " << v.name
          << " is below its lower bound (\"\n << " << v.name << " << \"<"
          << b.lowerBound << ").\";\n"
          << "if(strcmp(policy,\"STRICT\")==0){\n"
          << "return throwPythonRuntimeException(msg.str());\n"
          << "} else {\n"
          << "fprintf(stderr,\"%s\\n\",msg.str().c_str());\n"
          << "}\n"
          << "}\n"
          << "}\n"
          << "}\n";
    }
    if ((b.boundsType == VariableBoundsDescription::UPPER) ||
        (b.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
      out << "if(" << v.name << " > " << b.upperBound << "){\n"
          << "policy = "
          << "::getenv(\"PYTHON_OUT_OF_BOUNDS_POLICY\");\n"
          << "if(policy!=nullptr){\n"
          << "if((strcmp(policy,\"STRICT\")==0)||"
          << "(strcmp(policy,\"WARNING\")==0)){\n"
          << "ostringstream msg;\n"
          << "msg << \"" << name << " : " << v.name
          << " is over its upper bound (\"\n << " << v.name << " << \">"
          << b.upperBound << ").\";\n"
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

  void PythonMaterialPropertyInterface::writeOutputFiles(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    using namespace std;
    using namespace tfel::system;
    const auto& file = fd.fileName;
    const auto& author = fd.authorName;
    const auto& description = fd.description;
    const auto& date = fd.date;
    const auto& material = mpd.material;
    const auto& library = mpd.library;
    const auto& law = mpd.law;
    const auto& includes = mpd.includes;
    const auto& output = mpd.output;
    const auto& inputs = mpd.inputs;
    const auto& materialLaws = mpd.materialLaws;
    const auto& staticVars = mpd.staticVars;
    const auto& function = mpd.f;
    const auto name = (!material.empty()) ? material + "_" + law : law;
    const auto outName = "include/" + name + "-python.hxx";
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "PythonMaterialPropertyInterface::writeOutputFiles: " + m);
    };
    std::ofstream out;
    out.open(outName);
    throw_if(!out, "unable to open '" + outName + "' for writing output file.");
    out.exceptions(ios::badbit | ios::failbit);
    const auto srcFileName = "src/" + name + "-python.cxx";
    std::ofstream srcFile;
    srcFile.open(srcFileName);
    throw_if(!srcFile,
             "unable to open '" + srcFileName + "' for writing output file.");
    srcFile.exceptions(ios::badbit | ios::failbit);
    // writing header
    out << "/*!\n"
        << "* \\file   " << outName << '\n'
        << "* \\brief  "
        << "this file declares the " << law << " MaterialLaw.\n"
        << "*         File generated by " << MFrontHeader::getVersionName()
        << " "
        << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!author.empty()) {
      out << "* \\author " << author << '\n';
    }
    if (!date.empty()) {
      out << "* \\date   " << date << '\n';
    }
    if (!description.empty()) {
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
            << "* \\file   " << srcFileName << '\n'
            << "* \\brief  "
            << "this file implements the python interface "
            << "for the " << name << " materialLaw.\n"
            << "*         File generated by " << MFrontHeader::getVersionName()
            << " "
            << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!author.empty()) {
      srcFile << "* \\author " << author << '\n';
    }
    if (!date.empty()) {
      srcFile << "* \\date   " << date << '\n';
    }
    srcFile << " */\n\n"
            << "#include <Python.h>\n\n"
            << "#include<algorithm>\n"
            << "#include<iostream>\n"
            << "#include<iterator>\n"
            << "#include<fstream>\n"
            << "#include<sstream>\n"
            << "#include<cstring>\n"
            << "#include<cstdlib>\n"
            << "#include<cstdio>\n"
            << "#include<string>\n"
            << "#include<vector>\n"
            << "#include<cmath>\n";
    if (!includes.empty()) {
      srcFile << includes << "\n\n";
    }
    srcFile << "#include\"" << name << "-python.hxx\"\n\n";
    writeExportDirectives(srcFile);
    writeMaterialPropertyParametersHandler(srcFile, mpd, name, "double",
                                           "python");
    srcFile << "#ifdef __cplusplus\n"
            << "extern \"C\"{\n"
            << "#endif /* __cplusplus */\n\n";
    // mfront metadata
    writeEntryPointSymbol(srcFile, name);
    writeTFELVersionSymbol(srcFile, name);
    writeInterfaceSymbol(srcFile, name, "Python");
    writeMaterialSymbol(srcFile, name, mpd.material);
    writeMaterialKnowledgeTypeSymbol(srcFile, name, MATERIALPROPERTY);
    if (!inputs.empty()) {
      srcFile << "PyObject *\n"
              << name << "_wrapper("
              << "PyObject *,PyObject * py_args_)\n{\n";
    } else {
      srcFile << "PyObject *\n"
              << name << "_wrapper("
              << "PyObject *,PyObject*)\n{\n";
    }
    srcFile << "using namespace std;\n"
            << "using real = double;\n";
    // material laws
    writeMaterialLaws(srcFile, materialLaws);
    // static variables
    writeStaticVariables(srcFile, staticVars, file);
    srcFile << "auto throwPythonRuntimeException = [](const string& msg){\n"
            << "  PyErr_SetString(PyExc_RuntimeError,msg.c_str());\n"
            << "  return nullptr;\n"
            << "};\n";
    // parameters
    if (!mpd.parameters.empty()) {
      const auto hn = getMaterialPropertyParametersHandlerClassName(name);
      srcFile << "if(!python::" << hn << "::get" << hn << "().ok){\n"
              << "return throwPythonRuntimeException(python::" << name
              << "MaterialPropertyHandler::get" << name
              << "MaterialPropertyHandler().msg);\n"
              << "}\n";
      writeAssignMaterialPropertyParameters(srcFile, mpd, name, "double",
                                            "python");
    }
    for (const auto& i : inputs) {
      srcFile << "real " << i.name << ";\n";
    }
    if (hasBounds(mpd.inputs) || hasBounds(mpd.output)) {
      srcFile << "#ifndef PYTHON_NO_BOUNDS_CHECK\n";
      srcFile << "const char * policy;\n";
      srcFile << "#endif /* PYTHON_NO_BOUNDS_CHECK */\n";
    }
    if (!inputs.empty()) {
      srcFile << "if(!PyArg_ParseTuple(py_args_,\"";
      unsigned short i;
      for (i = 0; i != inputs.size(); ++i) {
        srcFile << "d";
      }
      srcFile << "\",";
      for (auto p3 = inputs.begin(); p3 != inputs.end();) {
        srcFile << "&" << p3->name;
        if (++p3 != inputs.end()) {
          srcFile << ",";
        }
      }
      srcFile << ")){\nreturn NULL;\n}\n";
    }
    if ((hasPhysicalBounds(mpd.inputs)) || (hasBounds(mpd.inputs))) {
      srcFile << "#ifndef PYTHON_NO_BOUNDS_CHECK\n";
      if (hasPhysicalBounds(mpd.inputs)) {
        srcFile << "// treating physical bounds\n";
        for (const auto& i : mpd.inputs) {
          writePhysicalBounds(srcFile, name, i);
        }
      }
      if (hasBounds(mpd.inputs)) {
        srcFile << "// treating standard bounds\n";
        for (const auto& i : mpd.inputs) {
          writeBounds(srcFile, name, i);
        }
      }
      srcFile << "#endif /* PYTHON_NO_BOUNDS_CHECK */\n";
    }
    srcFile
        << "real " << output.name << ";\n"
        << "try{\n"
        << function.body << "} catch(exception& cpp_except){\n"
        << "  return throwPythonRuntimeException(cpp_except.what());\n"
        << "} catch(...){\n"
        << "  return throwPythonRuntimeException(\"unknown C++ exception\");\n"
        << "}\n";
    if ((hasPhysicalBounds(mpd.output)) || (hasBounds(mpd.output))) {
      srcFile << "#ifndef PYTHON_NO_BOUNDS_CHECK\n";
      if (hasPhysicalBounds(mpd.output)) {
        srcFile << "// treating physical bounds\n";
        writePhysicalBounds(srcFile, name, mpd.output);
      }
      if (hasBounds(mpd.output)) {
        srcFile << "// treating standard bounds\n";
        writeBounds(srcFile, name, mpd.output);
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
    if (library.empty()) {
      if (!material.empty()) {
        fname = "src/python" + material + "wrapper.lst";
      } else {
        fname = "src/pythonmaterialwrapper.lst";
      }
    } else {
      fname = "src/python" + library + "wrapper.lst";
    }
    set<string> interfaces;
    struct stat buffer;  // for call to stat
    if (access(fname.c_str(), F_OK) == 0) {
      throw_if(stat(fname.c_str(), &buffer) == -1,
               "can't stat file '" + fname + "'");
      throw_if(!S_ISREG(buffer.st_mode),
               "'" + fname + "' is not a regular file");
      ifstream iwrapper(fname);
      throw_if(!iwrapper, "unable to open '" + fname + "'");
      copy(istream_iterator<string>(iwrapper), istream_iterator<string>(),
           insert_iterator<set<string>>(interfaces, interfaces.begin()));
      iwrapper.close();
    }
    interfaces.insert(name);
    const auto wn = [&library, &material]() -> std::string {
      if (library.empty()) {
        if (!material.empty()) {
          return "src/" + material + "lawwrapper.cxx";
        } else {
          return "src/materiallawwrapper.cxx";
        }
      }
      return "src/" + library + "wrapper.cxx";
    }();
    ofstream wrapper(wn);
    wrapper.exceptions(ios::badbit | ios::failbit);
    throw_if(!wrapper, "unable to open file '" + wn + "'");
    wrapper << "/*!\n";
    if (library.empty()) {
      if (!material.empty()) {
        wrapper << "* \\file   src/" + material + "lawwrapper.cxx\n";
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
    if (!author.empty()) {
      wrapper << "* \\author " << author << '\n';
    }
    if (!date.empty()) {
      wrapper << "* \\date   " << date << '\n';
    }
    if (!description.empty()) {
      wrapper << description << '\n';
    }
    wrapper << " */\n\n";
    for (const auto& i : interfaces) {
      wrapper << "#include\"" << i << "-python.hxx\"\n";
    }
    wrapper << '\n';
#ifndef _WIN32
    writeExportDirectives(wrapper);
    wrapper << '\n';
#endif /* _WIN32 */
    const auto mlm = (material.empty() ? "Material" : material) + "LawMethods";
    wrapper << "static PyMethodDef " << mlm << "[] = {\n";
    for (const auto& i : interfaces) {
      wrapper << "{\"" << i << "\"," << i << "_wrapper,METH_VARARGS,\n"
              << "\"compute the " << i << " law.\"},\n";
    }
    wrapper << "{NULL, NULL, 0, NULL} /* Sentinel */\n};\n\n";
    const auto md = makeLowerCase(getMaterialLawLibraryNameBase(mpd));
    wrapper << "#if PY_MAJOR_VERSION >= 3\n";
#ifndef _WIN32
    wrapper << "extern \"C\" MFRONT_SHAREDOBJ  PyObject* ";
#else
    wrapper << "PyMODINIT_FUNC ";
#endif
    wrapper
        << "PyInit_" << md << "()\n"
        << "{\n"
        << "  struct ModuleState {\n"
        << "    PyObject *error;\n"
        << "  };\n"
        << "  auto traverse = [](PyObject *m, visitproc visit, void * arg) {\n"
        << "    auto s = static_cast<ModuleState*>(PyModule_GetState(m));\n"
        << "    Py_VISIT(s->error);\n"
        << "    return 0;\n"
        << "  };\n"
        << "  auto clear = [](PyObject *m) {\n"
        << "    auto s = static_cast<ModuleState*>(PyModule_GetState(m));\n"
        << "    Py_CLEAR(s->error);\n"
        << "    return 0;\n"
        << "  };\n"
        << "  static PyModuleDef d = {\n"
        << "    PyModuleDef_HEAD_INIT,\n"
        << "    \"" << md << "\",\n"
        << "    nullptr,sizeof(ModuleState),\n"
        << "    " << mlm << ",\n"
        << "    nullptr,+traverse,+clear,nullptr\n"
        << "  };\n"
        << "  auto *m = PyModule_Create(&d);\n"
        << "  if (m == nullptr){\n"
        << "    return nullptr;\n"
        << "  }\n"
        << "  auto st = static_cast<ModuleState*>(PyModule_GetState(m));\n"
        << "  st->error = PyErr_NewException(\"" << md
        << ".error\", nullptr, nullptr);\n"
        << "  if (st->error == nullptr) {\n"
        << "    Py_DECREF(m);\n"
        << "    return nullptr;\n"
        << "  }\n"
        << "  return m;\n"
        << "} /* end of init " << md << " */\n"
        << "#else\n";
#ifndef _WIN32
    wrapper << "extern \"C\" MFRONT_SHAREDOBJ  void ";
#else
    wrapper << "PyMODINIT_FUNC ";
#endif
    wrapper
        << "init" << md << "()\n"
        << "{\n"
        << "  static struct {\n"
        << "    PyObject *error;\n"
        << "  } state;\n"
        << "  auto *m = Py_InitModule(\"" << md << "\"," << mlm << ");\n"
        << "  if (m == nullptr){\n"
        << "    return;\n"
        << "  }\n"
        << "  char exception[] = \"" << md << ".error\";\n"
        << "  state.error = PyErr_NewException(exception,nullptr,nullptr);\n"
        << "  if (state.error == nullptr) {\n"
        << "    Py_DECREF(m);\n"
        << "    return;\n"
        << "  }\n"
        << "} /* end of init " << md << "  */\n"
        << "#endif\n";
    wrapper.close();
    wrapper.open(fname);
    throw_if(!wrapper, "unable to open file '" + fname + "'");
    copy(interfaces.begin(), interfaces.end(),
         ostream_iterator<string>(wrapper, "\n"));
    wrapper.close();
  }  // end of PythonMaterialPropertyInterface::writeSrcFile()

  PythonMaterialPropertyInterface::~PythonMaterialPropertyInterface() = default;

}  // end of namespace mfront
