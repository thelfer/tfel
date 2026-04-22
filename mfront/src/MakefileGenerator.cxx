/*!
 * \file   mfront/src/MakefileGenerator.cxx
 * \brief
 * \author Thomas Helfer
 * \date   16/08/2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <set>
#include <cstring>
#include <ostream>
#include <sstream>
#include <fstream>
#include <iterator>
#include <algorithm>

#include <sys/types.h>
#include <sys/stat.h>
#if defined _WIN32 || defined _WIN64
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <io.h>
#include <conio.h>
#include <windows.h>
#include <process.h>
#ifdef small
#undef small
#endif /* small */
#else
#include <dlfcn.h>
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#endif

#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/System.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/ConfigurationManager.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/InstallPath.hxx"
#include "MFront/SearchPathsHandler.hxx"
#include "MFront/MFrontLock.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/GeneratorOptions.hxx"
#include "MFront/MakefileGenerator.hxx"

namespace mfront {

  static const char* getMakeCommand() {
    const char* emake = ::getenv("MAKE");
    if (emake != nullptr) {
      return emake;
    }
#if defined _WIN32 || defined _WIN64
#if defined __MINGW32__ || defined __MINGW64__
    return "mingw32-make.exe";
#else
    return "make.exe";
#endif
#elif defined __FreeBSD__
    return "gmake";
#else
    return "make";
#endif
  }

  static std::string getLibraryLinkFlags(const TargetsDescription& t,
                                         const GeneratorOptions& o,
                                         const std::string& name) {
    tfel::raise_if(!describes(t, name),
                   "getLibraryLinkFlags : no library "
                   "named '" +
                       name + "'.\nInternal Error.");
    const auto& l = t.getLibrary(name);
    auto res = std::string{};
    for (const auto& d : l.link_directories) {
      if ((tfel::utilities::starts_with(d, "$(shell ")) ||
          (tfel::utilities::ends_with(d, ")"))) {
        res += "-L\"$(strip " + d + ")\" ";
      } else {
        res += "-L" + d + " ";
      }
    }
    for (const auto& ll : l.link_libraries) {
      if ((tfel::utilities::starts_with(ll, "$(shell ")) ||
          (tfel::utilities::ends_with(ll, ")"))) {
        res += "$(patsubst %,-l%," + ll + ") ";
      } else {
        res += "-l" + ll + " ";
      }
    }
    for (const auto& ld : l.ldflags) {
      res += ld + " ";
    }
    if (o.melt) {
      for (const auto& ldn : l.deps) {
        tfel::raise_if(!describes(t, ldn),
                       "getLibraryLinkFlags : no library "
                       "named '" +
                           ldn +
                           "' (dependency of library "
                           "'" +
                           name + "').\nInternal Error.");
        const auto& ld = t.getLibrary(ldn);
        for (const auto& d : ld.ldflags) {
          res += d + " ";
        }
      }
    }
    return res;
  }  // end of getLibraryLinkFlags(const std::string&)

  // res.first.first : true if the target has C++ source files
  // res.first.second: true if the target has CUDA source files
  // res.second.first : list of object files
  // res.second.first : list of library dependencies
  static std::pair<std::pair<bool, bool>, std::pair<std::string, std::string>>
  getLibrarySourcesAndDependencies(const TargetsDescription& t,
                                   const GeneratorOptions& o,
                                   const std::string& name) {
    const auto& l = t.getLibrary(name);
    auto res =
        std::pair<std::pair<bool, bool>, std::pair<std::string, std::string>>{};
    res.first = {false, false};
    for (const auto& s : l.sources) {
      if ((s.ends_with(".cpp")) || (s.ends_with(".cxx"))) {
        res.first.first = true;
        res.second.first += s.substr(0, s.size() - 4) + ".o ";
      }
      if (s.ends_with(".c")) {
        res.second.first += s.substr(0, s.size() - 2) + ".o ";
      }
      if (s.ends_with(".cu")) {
        res.first.second = true;
        res.second.first += s.substr(0, s.size() - 3) + ".o ";
      }
    }
    if (o.melt) {
      for (const auto& d : l.deps) {
        const auto rd = getLibrarySourcesAndDependencies(t, o, d);
        res.first.first = res.first.first || rd.first.first;
        res.first.second = res.first.second || rd.first.second;
        if (!res.second.first.empty()) {
          res.second.first += " ";
        }
        res.second.first += rd.second.first;
      }
    } else {
      for (const auto& d : l.deps) {
        res.second.second += " " + d;
      }
    }
    return res;
  }  // end of getLibrarySourcesAndDependencies

  static std::string getLibraryFullName(const LibraryDescription& l) {
    return l.prefix + l.name + '.' + l.suffix;
  }  // end of getLibraryFullName

  static std::string getTargetName(const TargetsDescription& t,
                                   const std::string& n) {
    for (const auto& l : t.libraries) {
      if (n == l.name) {
        return getLibraryFullName(l);
      }
    }
    for (const auto& tg : t.specific_targets) {
      if (tg.first == n) {
        return tg.first;
      }
    }
    tfel::raise("getTargetName: no target associated with '" + n + "'");
  }  // end of getTargetName

  void generateMakeFile(const TargetsDescription& t,
                        const GeneratorOptions& o,
                        const std::string& d,
                        const std::string& f) {
    if (getVerboseMode() >= VERBOSE_LEVEL2) {
      getLogStream() << "generating Makefile\n";
    }
    const auto& cm = ConfigurationManager::get();
    auto get_compiler = [&cm](const char* const env,
                              const ConfigurationManager::Language l)
        -> std::optional<std::string> {
      const auto* const compiler = std::getenv(env);
      if (compiler != nullptr) {
        return compiler;
      }
      const auto ocompiler = cm.getCompiler(l);
      if (ocompiler.has_value()) {
        return *ocompiler;
      }
      return {};
    };
    auto get_compilation_flags =
        [&cm](const char* const env, const ConfigurationManager::Language l,
              const ConfigurationManager::LanguageOptionCategory c)
        -> std::set<std::string> {
      const auto* const opts = std::getenv(env);
      if (opts != nullptr) {
        return std::set<std::string>{{std::string{opts}}};
      }
      const auto opts2 = cm.getCompilationOptions(l, c);
      if (opts2.has_value()) {
        return *opts2;
      }
      return {};
    };
    auto get_linker_flags =
        [&cm](const char* const env,
              const ConfigurationManager::LinkerOptionCategory c)
        -> std::set<std::string> {
      const auto* const opts = std::getenv(env);
      if (opts != nullptr) {
        return std::set<std::string>{{std::string{opts}}};
      }
      const auto opts2 = cm.getLinkerOptions(c);
      if (opts2.has_value()) {
        return *opts2;
      }
      return {};
    };
    auto transform_flag = [](const std::string& flag) {
      if ((tfel::utilities::starts_with(flag, "$(shell ")) ||
          (tfel::utilities::ends_with(flag, ")"))) {
        return "$(strip " + flag + ")";
      }
      return flag;
    };
    MFrontLockGuard lock;
    const auto c_compiler = get_compiler("CC", ConfigurationManager::C);
    const auto cxx_compiler = get_compiler("CXX", ConfigurationManager::CXX);
    const auto cuda_compiler =
        get_compiler("CUDA_COMPILER", ConfigurationManager::CUDA);
    const auto inc = ::getenv("INCLUDES");
    const auto include_paths = cm.getIncludePaths();
    const auto cxxflags =
        get_compilation_flags("CXXFLAGS", ConfigurationManager::CXX,
                              ConfigurationManager::COMPILATION_FLAGS);
    const auto cflags =
        get_compilation_flags("CFLAGS", ConfigurationManager::C,
                              ConfigurationManager::COMPILATION_FLAGS);
    const auto cuda_flags =
        get_compilation_flags("CUDAFLAGS", ConfigurationManager::CUDA,
                              ConfigurationManager::COMPILATION_FLAGS);
    const auto ldflags =
        get_linker_flags("LDFLAGS", ConfigurationManager::LINKER_FLAGS);
    const auto sb = o.silentBuild ? "@" : "";
    const auto cc = c_compiler.value_or("$(CC)");
    const auto cxx = cxx_compiler.value_or("$(CXX)");
    const auto cuda = cuda_compiler.value_or("$(CUDA_COMPILER)");
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    auto mfile = d + tfel::system::dirStringSeparator() + f;
    std::ofstream m(mfile);
    m.exceptions(std::ios::badbit | std::ios::failbit);
    tfel::raise_if(!m, "generateMakeFile : can't open file '" + mfile + "'");
    auto cppSources = std::set<std::string>{};
    auto cSources = std::set<std::string>{};
    auto cudaSources = std::set<std::string>{};
    for (const auto& l : t.libraries) {
      for (const auto& src : l.sources) {
        if ((src.ends_with(".cpp")) || (src.ends_with(".cxx"))) {
          cppSources.insert(src);
        }
        if (src.ends_with(".c")) {
          cSources.insert(src);
        }
        if (src.ends_with(".cu")) {
          cudaSources.insert(src);
        }
      }
    }
    m << "# Makefile generated by mfront.\n"
      << MFrontHeader::getHeader("# ") << "\n";
    m << "export LD_LIBRARY_PATH:=$(PWD):$(LD_LIBRARY_PATH)\n\n";
    // COMPILERS
    if (c_compiler.has_value()) {
      m << "CC := " << *(c_compiler) << "\n";
    }
    if (cxx_compiler.has_value()) {
      m << "CXX := " << *(cxx_compiler) << "\n";
    }
    if (cuda_compiler.has_value()) {
      m << "CUDA_COMPILER := " << *(cuda_compiler) << "\n";
    }
    if ((c_compiler.has_value()) || (cxx_compiler.has_value()) ||
        (cuda_compiler.has_value())) {
      m << '\n';
    }
    // INCLUDES
    m << "INCLUDES := ";
    if (inc != nullptr) {
      m << inc << " ";
    }
    if (!include_paths.empty()) {
      for (const auto& path : include_paths) {
        m << "-I" << path << ' ';
      }
    }
    if (inc != nullptr) {
      m << inc << ' ';
    }
    m << "-I../include";
    for (const auto& path : o.include_paths) {
      m << " -I" << path;
    }
    // cpp flags
    std::vector<std::string> cppflags;
    for (const auto& l : t.libraries) {
      for (const auto& flag : l.cppflags) {
        insert_if(cppflags, transform_flag(flag));
      }
      for (const auto& id : l.include_directories) {
        insert_if(cppflags, "-I\"" + transform_flag(id) + '\"');
      }
    }
    if (!cppflags.empty()) {
      m << " \\\n";
      for (auto p7 = cppflags.begin(); p7 != cppflags.end();) {
        m << "\t     " << *p7;
        if (++p7 != cppflags.end()) {
          m << " \\\n";
        }
      }
    }
    // adding the mfront search path to the include files
    if (!SearchPathsHandler::getSearchPaths().empty()) {
      auto first = cppflags.empty();
      for (const auto& dir : SearchPathsHandler::getSearchPaths()) {
        if (!first) {
          m << "\\\n";
          first = false;
        }
        m << "\t     -I" << dir;
      }
    }
    //
    m << "\n\n";
    // LDFLAGS
    if (!ldflags.empty()) {
      m << "LDFLAGS :=";
      for (const auto& flag : ldflags) {
        m << " " << transform_flag(flag);
      }
      m << " \n";
    }
    auto write_default_c_cxx_flags = [&o, &m, &tfel_config] {
      switch (o.olevel) {
        case GeneratorOptions::LEVEL2:
          m << "$(shell " << tfel_config << " --oflags --oflags2) ";
          break;
        case GeneratorOptions::LEVEL1:
          m << "$(shell " << tfel_config << " --oflags) ";
          break;
        case GeneratorOptions::LEVEL0:
          m << "$(shell " << tfel_config << " --oflags0) ";
          break;
      }
      if (o.debugFlags) {
        m << "$(shell " << tfel_config << " --debug-flags) ";
      }
    };
    // CXXFLAGS
    if (!cppSources.empty()) {
      m << "CXXFLAGS := ";
      if (!cxxflags.empty()) {
        for (const auto& flag : cxxflags) {
          m << transform_flag(flag) << " ";
        }
      } else {
        m << "-Wall -Wfatal-errors ";
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
        m << "-ansi ";
#endif /* __CYGWIN__ */
        write_default_c_cxx_flags();
      }
      if ((o.sys == "win32") || (o.sys == "cygwin")) {
        m << "-DWIN32 -DMFRONT_COMPILING $(INCLUDES) \n\n";
      } else {
        m << "-fPIC $(INCLUDES) \n\n";
      }
    }
    // CFLAGS
    if (!cSources.empty()) {
      m << "CFLAGS := ";
      if (!cflags.empty()) {
        for (const auto& flag : cflags) {
          m << transform_flag(flag) << " ";
        }
      } else {
        m << "-W -Wall -Wfatal-errors ";
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
        m << "-ansi -std=c99 ";
#endif /* __CYGWIN__ */
        write_default_c_cxx_flags();
      }
      if ((o.sys == "win32") || (o.sys == "cygwin")) {
        m << "-DWIN32 -DMFRONT_COMPILING $(INCLUDES)\n\n";
      } else {
        m << "-fPIC $(INCLUDES)\n\n";
      }
    }
    if (!cudaSources.empty()) {
      m << "CUDAFLAGS := ";
      if (!cuda_flags.empty()) {
        for (const auto& flag : cuda_flags) {
          m << transform_flag(flag) << " ";
        }
      }
      m << "$(INCLUDES)\n\n";
    }
    // sources list
    if (!cppSources.empty()) {
      m << "SRCCXX = ";
      auto p4 = cppSources.begin();
      while (p4 != cppSources.end()) {
        m << *p4;
        if (++p4 != cppSources.end()) {
          m << " ";
        }
      }
      m << "\n\n";
    }
    if (!cSources.empty()) {
      m << "SRC = ";
      auto p4 = cSources.begin();
      while (p4 != cSources.end()) {
        m << *p4;
        if (++p4 != cSources.end()) {
          m << " ";
        }
      }
      m << "\n\n";
    }
    if (!cudaSources.empty()) {
      m << "SRCCUDA = ";
      auto p4 = cudaSources.begin();
      while (p4 != cudaSources.end()) {
        m << *p4;
        if (++p4 != cudaSources.end()) {
          m << " ";
        }
      }
      m << "\n\n";
    }
    if (!o.nodeps) {
      if (!cppSources.empty()) {
        m << "makefiles1 = $(SRCCXX:.cxx=.d)\n";
        m << "makefiles2 = $(makefiles1:.cpp=.d)\n";
      }
      if (!cSources.empty()) {
        m << "makefiles3 = $(SRC:.c=.d)\n";
      }
      m << "makefiles  =";
      if (!cppSources.empty()) {
        m << " $(makefiles2)";
      }
      if (!cSources.empty()) {
        m << " $(makefiles3)";
      }
    }
    m << "\n\n";
    m << ".PHONY = ";
    m << "all install clean";
    for (const auto& l : t.libraries) {
      if ((l.name != "MFrontMaterialLaw") && (l.name != "MFrontBehaviour")) {
        m << " " << getLibraryFullName(l);
      }
    }
    for (const auto& target : t.specific_targets) {
      if ((target.first != "all") && (target.first != "clean")) {
        m << " " << target.first;
      }
    }
    m << "\n\n";
    m << "all : ";
    for (const auto& l : t.libraries) {
      if ((l.name != "MFrontMaterialLaw") && (l.name != "MFrontBehaviour")) {
        m << getLibraryFullName(l) << " ";
      }
    }
    auto p5 = t.specific_targets.find("all");
    if (p5 != t.specific_targets.end()) {
      std::copy(p5->second.deps.begin(), p5->second.deps.end(),
                std::ostream_iterator<std::string>(m, " "));
      m << "\n";
      for (const auto& cmd : p5->second.cmds) {
        m << "\t" << cmd << "\n";
      }
    }
    m << "\n\n";
    for (const auto& target : t.specific_targets) {
      if ((target.first != "all") && (target.first != "clean")) {
        m << target.first << " :";
        for (const auto& dependency : target.second.deps) {
          m << " " << getTargetName(t, dependency);
        }
        m << '\n';
        for (const auto& cmd : target.second.cmds) {
          m << "\t" << cmd << '\n';
        }
        m << "\n";
      }
    }
    for (const auto& l : t.libraries) {
      if ((l.name == "MFrontMaterialLaw") || (l.name == "MFrontBehaviour")) {
        continue;
      }
      m << getLibraryFullName(l) << " : ";
      auto dep = getLibrarySourcesAndDependencies(t, o, l.name);
      const auto hasCxxSources = dep.first.first;
      const auto hasCUDASources = dep.first.second;
      if (!dep.second.first.empty()) {
        m << dep.second.first;
      }
      if (!dep.second.second.empty()) {
        auto sl = dep.second.second;
        m << sl;
      }
      m << "\n\t";
      if (hasCUDASources) {
        m << sb << cuda << " ";
      } else if (hasCxxSources) {
        m << sb << cxx << " ";
      } else {
        m << sb << cc << " ";
      }
      if (!ldflags.empty()) {
        m << "$(LDFLAGS) ";
      }
      if (o.sys == "win32") {
        m << "-shared -Wl,--add-stdcall-alias,--out-implib,lib" << l.name
          << "_dll.a,-no-undefined ";
      } else if (o.sys == "cygwin") {
        m << "-shared -Wl,--add-stdcall-alias,--out-implib,cyg" << l.name
          << "_dll.a,-no-undefined ";
      } else if (o.sys == "apple") {
        m << "-bundle ";
      } else {
        m << "-shared ";
      }
      m << "$^  -o $@ ";
      m << getLibraryLinkFlags(t, o, l.name);
      m << "\n";
      if (o.sys == "apple") {
        m << "\tinstall_name_tool -add_rpath  $(shell " << tfel_config
          << " --library-path)  $@\n";
      }
      m << "\n";
    }
    // install target
    auto get_install_path = [](const LibraryDescription& l) {
      const auto ipath =
          l.install_path.empty() ? getInstallPath() : l.install_path;
      if ((tfel::utilities::starts_with(ipath, "$(env ")) ||
          (tfel::utilities::ends_with(ipath, ")"))) {
        return "$(" + ipath.substr(6, ipath.size() - 7) + ")";
      }
      return ipath;
    };
    m << "install : ";
    for (const auto& l : t.libraries) {
      const auto ipath = get_install_path(l);
      if (!ipath.empty()) {
        m << " " << getLibraryFullName(l);
      }
    }
    // creating installation directories
    auto install_paths = std::set<std::string>{};
    for (const auto& l : t.libraries) {
      const auto ipath = get_install_path(l);
      if ((!ipath.empty()) && (install_paths.count(ipath) == 0)) {
        m << "\n\t" << sb << "mkdir -p " << ipath;
        install_paths.insert(ipath);
      }
    }
    // copying links
    for (const auto& l : t.libraries) {
      const auto ipath = get_install_path(l);
      if (!ipath.empty()) {
        m << "\n\t" << sb << "cp " << getLibraryFullName(l) << " " << ipath;
      }
    }
    m << "\n\n";
    // clean target
    m << "clean : ";
    p5 = t.specific_targets.find("clean");
    if (p5 != t.specific_targets.end()) {
      std::copy(p5->second.deps.begin(), p5->second.deps.end(),
                std::ostream_iterator<std::string>(m, " "));
    }
    m << "\n";
    if ((o.sys == "win32") || (o.sys == "cygwin")) {
      m << "\t" << sb << "rm -f *.o *.dll *.d *.d.*\n";
    } else if (o.sys == "apple") {
      m << "\t" << sb << "rm -f *.o *.bundle *.d *.d*\n";
    } else {
      m << "\t" << sb << "rm -f *.o *.so *.d *.d.*\n";
    }
    if (p5 != t.specific_targets.end()) {
      for (const auto& cmd : p5->second.cmds) {
        m << "\t" << sb << cmd << '\n';
      }
    }
    m << "\n";
    if (!o.nodeps) {
      m << "-include $(makefiles)\n\n";
    }
    // generic rules for objects file generation
    if (!cppSources.empty()) {
      m << "%.o:%.cxx\n";
      m << "\t" << sb << cxx << " $(CXXFLAGS) $< -o $@ -c\n\n";
      m << "%.o:%.cpp\n";
      m << "\t" << sb << cxx << " $(CXXFLAGS) $< -o $@ -c\n\n";
    }
    if (!cSources.empty()) {
      m << "%.o:%.c\n";
      m << "\t" << sb << cc << " $(CFLAGS) $< -o $@ -c\n\n";
    }
    if (!cudaSources.empty()) {
      m << "%.o:%.cu\n";
      m << "\t" << sb << cuda << " $(CUDAFLAGS) $< -o $@ -c\n\n";
    }
    if (!o.nodeps) {
      if (!cppSources.empty()) {
        m << "%.d:%.cxx\n";
        m << "\t" << sb << "set -e; rm -f $@;	    \\\n";
        m << "\t$(CXX) -M $(CXXFLAGS) $< > $@.$$$$; \\\n";
        m << "\tsed 's,\\($*\\)\\.o[ :]*,\\1.o $@ : ,g' < $@.$$$$ > $@; "
             "\\\n";
        m << "\trm -f $@.$$$$\n\n";
        m << "%.d:%.cpp\n";
        m << "\t" << sb << "set -e; rm -f $@;       \\\n";
        m << "\t$(CXX) -M $(CXXFLAGS) $< > $@.$$$$; \\\n";
        m << "\tsed 's,\\($*\\)\\.o[ :]*,\\1.o $@ : ,g' < $@.$$$$ > $@; "
             "\\\n";
        m << "\trm -f $@.$$$$\n\n";
      }
      if (!cSources.empty()) {
        m << "%.d:%.c\n";
        m << "\t" << sb << "set -e; rm -f $@;    \\\n";
        m << "\t$(CC) -M $(CFLAGS) $< > $@.$$$$; \\\n";
        m << "\tsed 's,\\($*\\)\\.o[ :]*,\\1.o $@ : ,g' < $@.$$$$ > $@; "
             "\\\n";
        m << "\trm -f $@.$$$$\n";
      }
    }
    m.close();
  }

  void callMake(const std::string& t,
                const std::string& d,
                const std::string& f) {
    const char* make = getMakeCommand();
    const char* silent = getDebugMode() ? nullptr : "-s";
    const char* const argv[] = {make,      "-C",      d.c_str(), "-f",
                                f.c_str(), t.c_str(), silent,    nullptr};
    auto error = [&argv, &t](const std::string& e) {
      auto msg = "callMake: can't build target '" + t + "'\n";
      if (!e.empty()) {
        msg += e + '\n';
      }
      msg += "Command was: ";
      for (const char* const* a = argv; *a != nullptr; ++a) {
        msg += *a;
        msg += ' ';
      }
      tfel::raise(msg);
    };
    tfel::raise_if(::strlen(make) == 0u, "callMake: empty make command");
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
    if (_spawnvp(_P_WAIT, make, argv) != 0) {
      error("");
    }
#else
    const auto child_pid = fork();
    if (child_pid != 0) {
      int status;
      if (wait(&status) == -1) {
        error(
            "something went wrong while "
            "waiting end of make process");
      }
      if (status != 0) {
        error(
            "Libraries building went wrong. "
            "This may be due to an error in your implementation.\n"
            "In rarer cases, this may be due to a "
            "incompatibility with a previous run of MFront and you "
            "may want to remove to the 'include' and 'src' directories. "
            "You may also invoke MFront with the `--verbose=debug` flag for "
            "more (hopefully helpful) information.");
      }
    } else {
      execvp(make, const_cast<char* const*>(argv));
      ::exit(EXIT_FAILURE);
    }
#endif
  }

}  // end of namespace mfront
