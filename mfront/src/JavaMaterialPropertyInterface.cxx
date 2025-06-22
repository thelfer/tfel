/*!
 * \file   mfront/src/JavaMaterialPropertyInterface.cxx
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
#include <sstream>
#include <stdexcept>
#include <iterator>
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
#include "TFEL/Utilities/StringAlgorithms.hxx"

#include "MFront/MFrontHeader.hxx"
#include "MFront/MFrontLock.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/CMaterialPropertyInterface.hxx"
#include "MFront/JavaMaterialPropertyInterface.hxx"

namespace mfront {

  static std::string getJavaClassName(const MaterialPropertyDescription& mpd) {
    if (mpd.library.empty()) {
      if (!mpd.material.empty()) {
        return mpd.material;
      } else {
        return "UnknownMaterial";
      }
    }
    return mpd.library;
  }  // end of getJavaClassName

  static std::string getJavaClassFileName(
      const MaterialPropertyDescription& mpd, const std::string& package) {
    if (package.empty()) {
      return getJavaClassName(mpd);
    }
    return package + "/" + getJavaClassName(mpd);
  }  // end of getJavaClassFileName

  std::string JavaMaterialPropertyInterface::getName() { return "java"; }

  JavaMaterialPropertyInterface::JavaMaterialPropertyInterface() {}

  std::pair<bool, JavaMaterialPropertyInterface::tokens_iterator>
  JavaMaterialPropertyInterface::treatKeyword(const std::string& key,
                                              const std::vector<std::string>& i,
                                              tokens_iterator current,
                                              const tokens_iterator end) {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "JavaMaterialPropertyInterface::treatKeyword : " + m);
    };
    if ((std::find(i.begin(), i.end(), "java") != i.end()) ||
        (std::find(i.begin(), i.end(), "Java") != i.end())) {
      throw_if(key != "@Package", "unsupported key '" + key + "'");
    }
    if (key == "@Package") {
      throw_if(!this->package.empty(), "package name already defined");
      throw_if(current == end, "unexpected end of file");
      const auto p = current->value;
      throw_if(!tfel::utilities::CxxTokenizer::isValidIdentifier(p, true),
               "invalid package name '" + p + "'");
      for (const auto c : p) {
        if (isalpha(c)) {
          throw_if(!islower(c), "invalid package name '" + p + "'");
        }
      }
      throw_if(++current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';', read '" + current->value + "'");
      ++(current);
      this->package = p;
      return make_pair(true, current);
    }
    return {false, current};
  }  // end of treatKeyword

  JavaMaterialPropertyInterface::~JavaMaterialPropertyInterface() = default;

  void JavaMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) const {
    const auto lib = getMaterialLawLibraryNameBase(mpd) + "-java";
    const auto name = (mpd.material.empty())
                          ? mpd.className
                          : mpd.material + "_" + mpd.className;
    const auto tfel_config = tfel::getTFELConfigExecutableName();
#if !((defined _WIN32) && (defined _MSC_VER))
    auto& l = d.getLibrary(lib);
    insert_if(l.link_libraries, "m");
#endif /* !((defined _WIN32) && (defined _MSC_VER)) */
    // the jni part
    insert_if(l.cppflags, TFEL_JAVA_INCLUDES);
    insert_if(l.cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
    insert_if(l.include_directories,
              "$(shell " + tfel_config + " --include-path)");
    insert_if(l.sources, name + "-java.cxx");
    if (this->package.empty()) {
      insert_if(l.epts, getJavaClassName(mpd) + "." + mpd.law);
    } else {
      insert_if(l.epts,
                this->package + "." + getJavaClassName(mpd) + "." + mpd.law);
    }
    // the java class
    const auto jfname = getJavaClassFileName(mpd, this->package);
    const auto src = "../java/" + jfname + ".java";
    const auto target = "../java/" + jfname + ".class";
    auto cmd = std::string{};
    const char* java = ::getenv("JAVAC");
    if (java == nullptr) {
      cmd += "javac";
    } else {
      cmd += java;
    }
    cmd += " " + src;
    insert_if(d.specific_targets[target].sources, src);
    insert_if(d.specific_targets[target].cmds, cmd);
    insert_if(d.specific_targets["all"].deps, target);
  }  // end of JavaMaterialPropertyInterface::getTargetsDescription

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
          << "ostringstream msg;\nmsg << \"" << name << " : " << v.name
          << " is below its physical lower bound (\"\n << " << v.name
          << " << \"<" << b.lowerBound << ").\";\n"
          << "return throwJavaRuntimeException(msg.str());\n"
          << "}\n";
    } else if (b.boundsType == VariableBoundsDescription::UPPER) {
      out << "if(" << v.name << " > " << v.type << "(" << b.upperBound
          << ")){\n"
          << "ostringstream msg;\nmsg << \"" << name << " : " << v.name
          << " is beyond its physical upper bound (\"\n << " << v.name
          << " << \">" << b.upperBound << ").\";\n"
          << "return throwJavaRuntimeException(msg.str());\n"
          << "}\n";
    } else {
      out << "if((" << v.name << " < " << v.type << "(" << b.lowerBound
          << "))||"
          << "(" << v.name << " > " << v.type << "(" << b.upperBound << "))){\n"
          << "if(" << v.name << " < " << v.type << "(" << b.lowerBound
          << ")){\n"
          << "ostringstream msg;\nmsg << \"" << name << " : " << v.name
          << " is below its physical lower bound (\"\n << " << v.name
          << " << \"<" << b.lowerBound << ").\";\n"
          << "return throwJavaRuntimeException(msg.str());\n"
          << "} else {\n"
          << "ostringstream msg;\nmsg << \"" << name << " : " << v.name
          << " is beyond its physical upper bound (\"\n << " << v.name
          << " << \">" << b.upperBound << ").\";\n"
          << "return throwJavaRuntimeException(msg.str());\n"
          << "}\n"
          << "}\n";
    }
  }  // end of writePhysicalBounds

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
               " ::getenv(\"JAVA_OUT_OF_BOUNDS_POLICY\");\n"
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
          << "ostringstream msg;\n"
          << "msg << \"" << name << " : " << v.name
          << " is below its lower bound (\"\n << " << v.name << " << \"<"
          << b.lowerBound << ").\";\n"
          << "if(::strcmp(mfront_policy,\"STRICT\")==0){\n"
          << "return throwJavaRuntimeException(msg.str());\n"
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
          << "ostringstream msg;\n"
          << "msg << \"" << name << " : " << v.name
          << " is over its upper bound (\"\n << " << v.name << " << \">"
          << b.upperBound << ").\";\n"
          << "if(::strcmp(mfront_policy,\"STRICT\")==0){\n"
          << "return throwJavaRuntimeException(msg.str());\n"
          << "} else {\n"
          << "fprintf(stderr,\"%s\\n\",msg.str().c_str());\n"
          << "}\n"
          << "}\n"
          << "}\n";
    }
  }

  void JavaMaterialPropertyInterface::writeOutputFiles(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "JavaMaterialPropertyInterface::writeOutputFiles : " + m);
    };
    using tfel::utilities::replace_all;
    tfel::system::systemCall::mkdir("java");
    if (!this->package.empty()) {
      tfel::system::systemCall::mkdir("java/" + this->package);
    }
    const auto lib = getMaterialLawLibraryNameBase(mpd) + "-java";
    const auto cname =
        (!mpd.material.empty()) ? mpd.material : "UnknowMaterial";
    const auto name =
        (!mpd.material.empty()) ? mpd.material + "_" + mpd.law : mpd.law;
    const auto srcFileName = "src/" + name + "-java.cxx";
    std::ofstream srcFile{srcFileName};
    throw_if(!srcFile,
             "unable to open '" + srcFileName + "' for writing output file.");
    srcFile.exceptions(std::ios::badbit | std::ios::failbit);
    // writing source
    srcFile << "/*!\n"
            << "* \\file   " << srcFileName << '\n'
            << "* \\brief  "
            << "this file implements the java interface "
            << "for the " << name << " material law.\n"
            << "*         File generated by " << MFrontHeader::getVersionName()
            << " "
            << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!fd.authorName.empty()) {
      srcFile << "* \\author " << fd.authorName << '\n';
    }
    if (!fd.date.empty()) {
      srcFile << "* \\date   " << fd.date << '\n';
    }
    srcFile << " */\n\n";
    // headers
    srcFile << "#include<algorithm>\n"
            << "#include<iostream>\n"
            << "#include<sstream>\n"
            << "#include<cstring>\n"
            << "#include<cstdlib>\n"
            << "#include<string>\n"
            << "#include<cmath>\n\n"
            << "#include\"TFEL/Config/TFELTypes.hxx\"\n"
            << "#include\"TFEL/PhysicalConstants.hxx\"\n"
            << "#include\"TFEL/Math/General/IEEE754.hxx\"\n\n";
    if (useQuantities(mpd)) {
      srcFile << "#include\"TFEL/Math/qt.hxx\"\n"
              << "#include\"TFEL/Math/Quantity/qtIO.hxx\"\n";
    }
    srcFile << "#include <jni.h>\n\n";
    if (!mpd.includes.empty()) {
      srcFile << mpd.includes << "\n\n";
    }
    writeExportDirectives(srcFile);
    srcFile << "#ifdef __cplusplus\n";
    srcFile << "extern \"C\"{\n";
    srcFile << "#endif /* __cplusplus */\n\n";

    writeVariablesNamesSymbol(srcFile, name, mpd);
    writeVariablesBoundsSymbols(srcFile, name, mpd);
    // mfront metadata
    writeFileDescriptionSymbols(srcFile, name, fd);
    writeBuildIdentifierSymbol(srcFile, name, mpd);
    writeEntryPointSymbol(srcFile, name);
    writeTFELVersionSymbol(srcFile, name);
    writeUnitSystemSymbol(srcFile, name, mpd);
    writeInterfaceSymbol(srcFile, name, "Java");
    writeLawSymbol(srcFile, name, mpd.material);
    writeMaterialSymbol(srcFile, name, mpd.material);
    writeMaterialKnowledgeTypeSymbol(srcFile, name, MATERIALPROPERTY);
    // java
    srcFile << "JNIEXPORT jdouble JNICALL\n";
    if (this->package.empty()) {
      srcFile << "Java_" << replace_all(getJavaClassName(mpd), "_", "_1");
    } else {
      srcFile << "Java_" << replace_all(this->package, "_", "_1") << "_"
              << replace_all(getJavaClassName(mpd), "_", "_1");
    }
    srcFile << "_" << replace_all(mpd.law, "_", "_1") << "(";
    srcFile << "JNIEnv *java_env,jclass";
    for (const auto& i : mpd.inputs) {
      if (useQuantities(mpd)) {
        srcFile << ", const jdouble mfront_" << i.name;
      } else {
        srcFile << ", const jdouble " << i.name;
      }
    }
    srcFile << ")\n{\n";
    writeBeginningOfMaterialPropertyBody(srcFile, mpd, fd, "jdouble", true);
    if (useQuantities(mpd)) {
      for (const auto& i : mpd.inputs) {
        srcFile << "const auto " << i.name << " = "  //
                << i.type << "(mfront_" << i.name << ");\n";
      }
    }
    // handle java exceptions
    srcFile
        << "auto throwJavaRuntimeException = [java_env](const string& msg){\n"
        << "  auto jexcept = "
           "java_env->FindClass(\"java/lang/RuntimeException\");\n"
        << "  if (jexcept==nullptr){\n"
        << "    cerr << \"Internal error : can't find the java \"\n"
        << "         << \"RuntimeException class\" << endl;\n"
        << "    ::exit(EXIT_FAILURE);\n"
        << "  }\n"
        << "  java_env->ThrowNew(jexcept,msg.c_str());\n"
        << "  java_env->DeleteLocalRef(jexcept);\n"
        << "  return real{};\n"
        << "};\n";
    // parameters
    for (const auto& p : mpd.parameters) {
      throw_if(
          !p.hasAttribute(VariableDescription::defaultValue),
          "internal error (can't find value of parameter '" + p.name + "')");
      const auto pv = p.getAttribute<double>(VariableDescription::defaultValue);
      if (useQuantities(mpd)) {
        srcFile << "static constexpr auto " << p.name  //
                << " = " << p.type << "(" << pv << ");\n";
      } else {
        srcFile << "static constexpr double " << p.name << " = " << pv << ";\n";
      }
    }
    if ((hasPhysicalBounds(mpd.inputs)) || (hasBounds(mpd.inputs))) {
      srcFile << "#ifndef JAVA_NO_BOUNDS_CHECK\n";
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
      srcFile << "#endif /* JAVA_NO_BOUNDS_CHECK */\n";
    }  //  if((hasPhysicalBounds(mpd))||(hasBounds(mpd))){
    if (useQuantities(mpd)) {
      srcFile << "auto " << mpd.output.name << " = "  //
              << mpd.output.type << "{};\n";
    } else {
      srcFile << "auto " << mpd.output.name << " = real{};\n";
    }
    srcFile
        << "try{\n"
        << mpd.f.body << "}\n"
        << "catch(std::exception& cpp_except){\n"
        << "  return throwJavaRuntimeException(cpp_except.what());\n"
        << "} catch(...){\n"
        << "  return throwJavaRuntimeException(\"unknown C++ exception\");\n"
        << "}\n";
    if ((hasPhysicalBounds(mpd.output)) || (hasBounds(mpd.output))) {
      srcFile << "#ifndef JAVA_NO_BOUNDS_CHECK\n";
      if (hasPhysicalBounds(mpd.output)) {
        srcFile << "// treating physical bounds\n";
        writePhysicalBounds(srcFile, name, mpd.output);
      }
      if (hasBounds(mpd.output)) {
        srcFile << "// treating standard bounds\n";
        writeBounds(srcFile, mpd, name, mpd.output);
      }
      srcFile << "#endif /* JAVA_NO_BOUNDS_CHECK */\n";
    }  // (hasPhysicalBounds(mpd.output))||(hasBounds(mpd.output))
    if (useQuantities(mpd)) {
      srcFile << "return " << mpd.output.name << ".getValue();\n";
    } else {
      srcFile << "return " << mpd.output.name << ";\n";
    }
    srcFile << "} // end of " << name << "\n\n"
            << "#ifdef __cplusplus\n"
            << "} // end of extern \"C\"\n"
            << "#endif /* __cplusplus */\n\n";
    srcFile.close();
    // writing java class
    MFrontLockGuard lock;
    auto methods = std::map<std::string, decltype(mpd.inputs.size())>{};
    auto jcname = getJavaClassName(mpd);
    auto jcmlst = "src/" + jcname + "-java.lst";
    struct stat buffer;  // for call to stat
    if (access(jcmlst.c_str(), F_OK) == 0) {
      stat(jcmlst.c_str(), &buffer);
      throw_if(!S_ISREG(buffer.st_mode),
               "'" + jcmlst + "' is not a regular file");
      std::ifstream f(jcmlst);
      throw_if(!f, "unable to open '" + jcmlst + "'");
      auto line = std::string();
      std::getline(f, line);
      throw_if(line != this->package, "inconsistent package name for class '" +
                                          jcname +
                                          "' "
                                          "(read '" +
                                          line + "')");
      while (std::getline(f, line)) {
        std::istringstream iss{line};
        iss.exceptions(std::ios::badbit | std::ios::failbit);
        auto m = std::string{};  // method name
        auto n = int{};          // number of arguments
        iss >> m >> n;
        methods.insert({m, n});
      }
      f.close();
    }
    methods[mpd.law] = mpd.inputs.size();
    std::ofstream mf{jcmlst};
    throw_if(!mf, "unable to open '" + jcmlst + "'");
    mf.exceptions(std::ios::badbit | std::ios::failbit);
    mf << this->package << '\n';
    for (const auto& m : methods) {
      mf << m.first << " " << m.second << "\n";
    }
    mf.close();
    std::ofstream jc{"java/" + getJavaClassFileName(mpd, this->package) +
                     ".java"};
    throw_if(!jc, "unable to open '" + jcname + ".java'");
    jc.exceptions(std::ios::badbit | std::ios::failbit);
    if (!this->package.empty()) {
      jc << "package " << this->package << ";\n\n";
    }
    jc << "public class " << jcname << "{\n"
       << "static {System.loadLibrary(\"" << lib << "\");}\n";
    for (const auto& m : methods) {
      jc << "public static native double " << m.first << "(";
      for (decltype(m.second) i = 0; i != m.second; ++i) {
        if (i != 0) {
          jc << ",\n";
        }
        jc << "double x" << i;
      }
      jc << ");\n";
    }
    jc << "}\n";
    jc.close();
  }  // end of JavaMaterialPropertyInterface::writeSrcFile()

}  // end of namespace mfront
