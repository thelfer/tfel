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
    return "make.exe";
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

  // res.first : true if the target has C++ source files
  // res.second.first : list of object files
  // res.second.first : list of library dependencies
  static std::pair<bool, std::pair<std::string, std::string>>
  getLibrarySourcesAndDependencies(const TargetsDescription& t,
                                   const GeneratorOptions& o,
                                   const std::string& name) {
    const auto& l = t.getLibrary(name);
    auto res = std::pair<bool, std::pair<std::string, std::string>>{};
    res.first = false;
    for (const auto& s : l.sources) {
      if (s.size() > 4) {
        const auto ext = s.substr(s.size() - 4);
        if ((ext == ".cpp") || (ext == ".cxx")) {
          res.first = true;
          res.second.first += s.substr(0, s.size() - 4) + ".o ";
        }
      }
      if (s.size() > 2) {
        if (s.substr(s.size() - 2) == ".c") {
          res.second.first += s.substr(0, s.size() - 2) + ".o ";
        }
      }
    }
    if (o.melt) {
      for (const auto& d : l.deps) {
        const auto rd = getLibrarySourcesAndDependencies(t, o, d);
        res.first = res.first || rd.first;
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
    MFrontLockGuard lock;
    const auto env_cc = ::getenv("CC");
    const auto env_cxx = ::getenv("CXX");
    const auto inc = ::getenv("INCLUDES");
    const auto cxxflags = ::getenv("CXXFLAGS");
    const auto cflags = ::getenv("CFLAGS");
    const auto ldflags = ::getenv("LDFLAGS");
    const auto sb = o.silentBuild ? "@" : "";
    const auto cxx = (env_cxx == nullptr) ? "$(CXX)" : env_cxx;
    const auto cc = (env_cc == nullptr) ? "$(CC)" : env_cc;
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    auto mfile = d + tfel::system::dirStringSeparator() + f;
    std::ofstream m(mfile);
    m.exceptions(std::ios::badbit | std::ios::failbit);
    tfel::raise_if(!m, "generateMakeFile : can't open file '" + mfile + "'");
    auto cppSources = std::set<std::string>{};
    auto cSources = std::set<std::string>{};
    for (const auto& l : t.libraries) {
      for (const auto& src : l.sources) {
        if (src.size() > 4) {
          if ((src.substr(src.size() - 4) == ".cpp") ||
              (src.substr(src.size() - 4) == ".cxx")) {
            cppSources.insert(src);
          }
        }
        if (src.size() > 2) {
          if (src.substr(src.size() - 2) == ".c") {
            cSources.insert(src);
          }
        }
      }
    }
    m << "# Makefile generated by mfront.\n"
      << MFrontHeader::getHeader("# ") << "\n";
    m << "export LD_LIBRARY_PATH:=$(PWD):$(LD_LIBRARY_PATH)\n\n";
    // COMPILERS
    if (env_cc != nullptr) {
      m << "CC := " << env_cc << "\n";
    }
    if (env_cxx != nullptr) {
      m << "CXX := " << env_cxx << "\n";
    }
    if ((env_cc != nullptr) || (env_cxx != nullptr)) {
      m << '\n';
    }
    // INCLUDES
    m << "INCLUDES := ";
    if (inc != nullptr) {
      m << inc << " ";
    }
    m << "-I../include";
    for (const auto& path : o.include_paths) {
      m << " -I" << path;
    }
    // cpp flags
    std::vector<std::string> cppflags;
    for (const auto& l : t.libraries) {
      for (const auto& flags : l.cppflags) {
        insert_if(cppflags, flags);
      }
      for (const auto& id : l.include_directories) {
        if ((tfel::utilities::starts_with(id, "$(shell ")) ||
            (tfel::utilities::ends_with(id, ")"))) {
          insert_if(cppflags, "-I\"$(strip " + id + ")\"");
        } else {
          insert_if(cppflags, "-I" + id);
        }
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
    if (ldflags != nullptr) {
      m << "LDFLAGS := " << ldflags << '\n';
    }
    // CXXFLAGS
    if (!cppSources.empty()) {
      m << "CXXFLAGS := -Wall -Wfatal-errors ";
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
      m << "-ansi ";
#endif /* __CYGWIN__ */
      if (cxxflags != nullptr) {
        m << cxxflags << " ";
      } else {
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
      }
      if ((o.sys == "win32") || (o.sys == "cygwin")) {
        m << "-DWIN32 -DMFRONT_COMPILING $(INCLUDES) \n\n";
      } else {
        m << "-fPIC $(INCLUDES) \n\n";
      }
    }
    // CFLAGS
    if (!cSources.empty()) {
      m << "CFLAGS := -W -Wall -Wfatal-errors ";
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
      m << "-ansi -std=c99 ";
#endif /* __CYGWIN__ */
      if (cflags != nullptr) {
        m << cflags << " ";
      } else {
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
      }
      if ((o.sys == "win32") || (o.sys == "cygwin")) {
        m << "-DWIN32 -DMFRONT_COMPILING $(INCLUDES)\n\n";
      } else {
        m << "-fPIC $(INCLUDES)\n\n";
      }
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
      const auto hasCxxSources = dep.first;
      if (!dep.second.first.empty()) {
        m << dep.second.first;
      }
      if (!dep.second.second.empty()) {
        auto sl = dep.second.second;
        m << sl;
      }
      m << "\n\t";
      if (hasCxxSources) {
        m << sb << cxx << " ";
      } else {
        m << sb << cc << " ";
      }
      if (ldflags != nullptr) {
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
