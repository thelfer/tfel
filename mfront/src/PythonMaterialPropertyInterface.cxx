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
    const auto headerFileName = name + "-python.hxx";
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
        return d.getLibrary(lib, "", "pyd", LibraryDescription::MODULE);
      } else if (d.system == LibraryDescription::MACOSX) {
        return d.getLibrary(lib, "", "so", LibraryDescription::MODULE);
      } else {
        const auto ls = LibraryDescription::getDefaultLibrarySuffix(
            d.system, d.libraryType);
        return d.getLibrary(lib, "", ls, LibraryDescription::MODULE);
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
      out << "if(" << v.name << " < " << v.type << "(" << b.lowerBound
          << ")){\n"
          << "std::ostringstream msg;\nmsg << \"" << name << " : " << v.name
          << " is below its physical lower bound (\"\n << " << v.name
          << " << \"<" << b.lowerBound << ").\";\n"
          << "return throwPythonRuntimeException(msg.str());\n"
          << "}\n";
    } else if (b.boundsType == VariableBoundsDescription::UPPER) {
      out << "if(" << v.name << " > " << v.type << "(" << b.upperBound
          << ")){\n"
          << "std::ostringstream msg;\nmsg << \"" << name << " : " << v.name
          << " is beyond its physical upper bound (\"\n << " << v.name
          << " << \">" << b.upperBound << ").\";\n"
          << "return throwPythonRuntimeException(msg.str());\n"
          << "}\n";
    } else {
      out << "if((" << v.name << " < " << v.type << "(" << b.lowerBound
          << "))||"
          << "(" << v.name << " > " << v.type << "(" << b.upperBound << "))){\n"
          << "if(" << v.name << " < " << v.type << "(" << b.lowerBound
          << ")){\n"
          << "std::ostringstream msg;\nmsg << \"" << name << " : " << v.name
          << " is below its physical lower bound (\"\n << " << v.name
          << " << \"<" << b.lowerBound << ").\";\n"
          << "return throwPythonRuntimeException(msg.str());\n"
          << "} else {\n"
          << "std::ostringstream msg;\n"
          << "msg << \"" << name << " : " << v.name
          << " is beyond its physical upper bound (\"\n << " << v.name
          << " << \">" << b.upperBound << ").\";\n"
          << "return throwPythonRuntimeException(msg.str());\n"
          << "}\n"
          << "}\n";
    }
  }

  static void writeBounds(std::ostream& out,
                          const MaterialPropertyDescription& mpd,
                          const std::string& name,
                          const VariableDescription& v) {
    if ((!allowRuntimeModificationOfTheOutOfBoundsPolicy(mpd)) &&
        (getDefaultOutOfBoundsPolicy(mpd) == tfel::material::None)) {
      return;
    }
    if (!v.hasBounds()) {
      return;
    }
    const auto& b = v.getBounds();
    const auto get_policy = [&mpd] {
      const auto default_policy =
          getDefaultOutOfBoundsPolicyAsUpperCaseString(mpd);
      if (allowRuntimeModificationOfTheOutOfBoundsPolicy(mpd)) {
        return "const char * const mfront_policy = "
               "[]{\n"
               " const auto* const p= "
               " ::getenv(\"PYTHON_OUT_OF_BOUNDS_POLICY\");\n"
               " if(p == nullptr){\n"
               " return \"" +
               default_policy +
               "\";\n"
               " }\n"
               " return p;\n"
               "}();\n";
      }
      return "const char * const mfront_policy = \"" + default_policy + "\";\n";
    }();
    if ((b.boundsType == VariableBoundsDescription::LOWER) ||
        (b.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
      out << "if(" << v.name << " < " << v.type << "(" << b.lowerBound
          << ")){\n"
          << get_policy  //
          << "if((::strcmp(mfront_policy,\"STRICT\")==0)||"
          << "(::strcmp(mfront_policy,\"WARNING\")==0)){\n"
          << "std::ostringstream msg;\n"
          << "msg << \"" << name << " : " << v.name
          << " is below its lower bound (\"\n << " << v.name << " << \"<"
          << b.lowerBound << ").\";\n"
          << "if(::strcmp(mfront_policy,\"STRICT\")==0){\n"
          << "return throwPythonRuntimeException(msg.str());\n"
          << "} else {\n"
          << "fprintf(stderr,\"%s\\n\",msg.str().c_str());\n"
          << "}\n"
          << "}\n"
          << "}\n";
    }
    if ((b.boundsType == VariableBoundsDescription::UPPER) ||
        (b.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
      out << "if(" << v.name << " > " << v.type << "(" << b.upperBound
          << ")){\n"
          << get_policy  //
          << "if((::strcmp(mfront_policy,\"STRICT\")==0)||"
          << "(::strcmp(mfront_policy,\"WARNING\")==0)){\n"
          << "std::ostringstream msg;\n"
          << "msg << \"" << name << " : " << v.name
          << " is over its upper bound (\"\n << " << v.name << " << \">"
          << b.upperBound << ").\";\n"
          << "if(::strcmp(mfront_policy,\"STRICT\")==0){\n"
          << "return throwPythonRuntimeException(msg.str());\n"
          << "} else {\n"
          << "fprintf(stderr,\"%s\\n\",msg.str().c_str());\n"
          << "}\n"
          << "}\n"
          << "}\n";
    }
  }

  void PythonMaterialPropertyInterface::writeOutputFiles(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    using namespace tfel::system;
    const auto& author = fd.authorName;
    const auto& description = fd.description;
    const auto& date = fd.date;
    const auto& material = mpd.material;
    const auto& library = mpd.library;
    const auto& law = mpd.law;
    const auto& output = mpd.output;
    const auto& inputs = mpd.inputs;
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
    out.exceptions(std::ios::badbit | std::ios::failbit);
    const auto srcFileName = "src/" + name + "-python.cxx";
    std::ofstream srcFile;
    srcFile.open(srcFileName);
    throw_if(!srcFile,
             "unable to open '" + srcFileName + "' for writing output file.");
    srcFile.exceptions(std::ios::badbit | std::ios::failbit);
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
            << "#include<cmath>\n"
            << "#include\"TFEL/Config/TFELTypes.hxx\"\n"
            << "#include\"TFEL/PhysicalConstants.hxx\"\n"
            << "#include\"TFEL/Math/General/IEEE754.hxx\"\n\n";
    if (useQuantities(mpd)) {
      srcFile << "#include\"TFEL/Math/qt.hxx\"\n"
              << "#include\"TFEL/Math/Quantity/qtIO.hxx\"\n";
    }
    if (!mpd.includes.empty()) {
      srcFile << mpd.includes << "\n\n";
    }
    srcFile << "#include\"" << name << "-python.hxx\"\n\n";
    writeExportDirectives(srcFile);
    writeMaterialPropertyParametersHandler(srcFile, mpd, name, "double",
                                           "python");
    srcFile << "#ifdef __cplusplus\n"
            << "extern \"C\"{\n"
            << "#endif /* __cplusplus */\n\n";

    writeVariablesNamesSymbol(srcFile, name, mpd);
    writeVariablesBoundsSymbols(srcFile, name, mpd);

    // mfront metadata
    writeFileDescriptionSymbols(srcFile, name, fd);
    writeBuildIdentifierSymbol(srcFile, name, mpd);
    writeEntryPointSymbol(srcFile, name);
    writeTFELVersionSymbol(srcFile, name);
    writeUnitSystemSymbol(srcFile, name, mpd);
    writeInterfaceSymbol(srcFile, name, "Python");
    writeLawSymbol(srcFile, name, mpd.material);
    writeMaterialSymbol(srcFile, name, mpd.material);
    writeMaterialKnowledgeTypeSymbol(srcFile, name, MATERIALPROPERTY);
    writeParametersSymbols(srcFile, name, mpd);
    // parameters
    if ((!areParametersTreatedAsStaticVariables(mpd)) &&
        (!mpd.parameters.empty())) {
      const auto hn = getMaterialPropertyParametersHandlerClassName(name);
      srcFile << "MFRONT_SHAREDOBJ int\n"
              << name << "_setParameter(const char *const p,"
              << "const double v"
              << "){\n";
      for (const auto& p : mpd.parameters) {
        srcFile << "if(strcmp(\"" << p.name << "\",p)==0){\n"
                << "python::" << hn << "::get" << hn << "()." << p.name
                << " = v;\n"
                << "return 1;\n"
                << "}\n";
      }
      srcFile << "return 0;\n"
              << "}\n\n";
    }
    //
    if (!inputs.empty()) {
      srcFile << "PyObject *\n"
              << name << "_wrapper("
              << "PyObject *,PyObject * py_args_)\n{\n";
    } else {
      srcFile << "PyObject *\n"
              << name << "_wrapper("
              << "PyObject *,PyObject*)\n{\n";
    }
    writeBeginningOfMaterialPropertyBody(srcFile, mpd, fd, "double", true);
    srcFile << "auto throwPythonRuntimeException = [](const string& msg){\n"
            << "  PyErr_SetString(PyExc_RuntimeError,msg.c_str());\n"
            << "  return nullptr;\n"
            << "};\n";
    // parameters
    if ((!areParametersTreatedAsStaticVariables(mpd)) &&
        (!mpd.parameters.empty())) {
      const auto hn = getMaterialPropertyParametersHandlerClassName(name);
      srcFile << "if(!python::" << hn << "::get" << hn << "().ok){\n"
              << "return throwPythonRuntimeException(python::" << name
              << "MaterialPropertyHandler::get" << name
              << "MaterialPropertyHandler().msg);\n"
              << "}\n";
    }
    writeAssignMaterialPropertyParameters(srcFile, mpd, name, "real", "python");
    for (const auto& i : inputs) {
      if (useQuantities(mpd)) {
        srcFile << "auto " << i.name << " = " << i.type << " {};\n";
      } else {
        srcFile << "auto " << i.name << " = real{};\n";
      }
    }
    if (!inputs.empty()) {
      srcFile << "if(!PyArg_ParseTuple(py_args_,\"";
      for (unsigned short i = 0; i != inputs.size(); ++i) {
        srcFile << "d";
      }
      srcFile << "\",";
      for (auto p3 = inputs.begin(); p3 != inputs.end();) {
        if (useQuantities(mpd)) {
          srcFile << "&(" << p3->name << ".getValue())";
        } else {
          srcFile << "&" << p3->name;
        }
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
          writeBounds(srcFile, mpd, name, i);
        }
      }
      srcFile << "#endif /* PYTHON_NO_BOUNDS_CHECK */\n";
    }
    if (useQuantities(mpd)) {
      srcFile << "auto " << output.name << " = " << output.type << "{};\n";
    } else {
      srcFile << "auto " << output.name << " = real{};\n";
    }
    srcFile
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
        writeBounds(srcFile, mpd, name, mpd.output);
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
    const auto fname = [&]() -> std::string {
      if (library.empty()) {
        if (!material.empty()) {
          return "src/python" + material + "wrapper.lst";
        } else {
          return "src/pythonmaterialwrapper.lst";
        }
      }
      return "src/python" + library + "wrapper.lst";
    }();
    std::set<std::string> interfaces;
    struct stat buffer;  // for call to stat
    if (access(fname.c_str(), F_OK) == 0) {
      throw_if(stat(fname.c_str(), &buffer) == -1,
               "can't stat file '" + fname + "'");
      throw_if(!S_ISREG(buffer.st_mode),
               "'" + fname + "' is not a regular file");
      std::ifstream iwrapper(fname);
      throw_if(!iwrapper, "unable to open '" + fname + "'");
      std::copy(std::istream_iterator<std::string>(iwrapper),
                std::istream_iterator<std::string>(),
                std::insert_iterator<std::set<std::string>>(
                    interfaces, interfaces.begin()));
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
    std::ofstream wrapper(wn);
    wrapper.exceptions(std::ios::badbit | std::ios::failbit);
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
        << "#if (defined _MSC_VER)\n"
        << "  static PyModuleDef d = {\n"
        << "    PyModuleDef_HEAD_INIT,\n"
        << "    \"" << md << "\",\n"
        << "    nullptr,sizeof(ModuleState),\n"
        << "    " << mlm << ",\n"
        << "    nullptr,traverse,clear,nullptr\n"
        << "  };\n"
        << "#else /* (defined _MSC_VER) */\n"
        << "  static PyModuleDef d = {\n"
        << "    PyModuleDef_HEAD_INIT,\n"
        << "    \"" << md << "\",\n"
        << "    nullptr,sizeof(ModuleState),\n"
        << "    " << mlm << ",\n"
        << "    nullptr,+traverse,+clear,nullptr\n"
        << "  };\n"
        << "#endif/* (defined _MSC_VER) */\n"
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
    std::copy(interfaces.begin(), interfaces.end(),
              std::ostream_iterator<std::string>(wrapper, "\n"));
    wrapper.close();
  }  // end of PythonMaterialPropertyInterface::writeSrcFile()

  PythonMaterialPropertyInterface::~PythonMaterialPropertyInterface() = default;

}  // end of namespace mfront
