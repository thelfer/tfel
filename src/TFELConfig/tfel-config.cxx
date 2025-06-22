/*!
 * \file   src/TFELConfig/tfel-config.cxx
 * \brief
 * \author Helfer Thomas
 * \date   27 août 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <map>
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <stdexcept>

#include "TFEL/Config/TFELConfig.hxx"
#include "tfel-config.hxx"

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

using FuncPtr = void (*)();
using CallBacksContainer =
    std::map<std::string, std::pair<FuncPtr, std::string>>;

static std::string handleSpace(const std::string& p) {
  if (find(p.begin(), p.end(), ' ') != p.end()) {
#if defined _WIN32 || defined _WIN64
    throw(
        std::runtime_error("tfel-config handleSpace: "
                           "path to TFEL shall not contain space as "
                           "MinGW can't handle it (Found '" +
                           p +
                           "'). "
                           "Please change TFEL installation directory"));
#else
    return '"' + p + '"';
#endif
  }
  return p;
}

static std::string getTFELHOME();

static std::string libDir();

static std::string includeDir();

static void registerCallBack(const std::string&,
                             const FuncPtr&,
                             const std::string&);

static void treatCompilerFlags();

static void treatOFlags0();

static void treatOFlags();

static void treatOFlags2();

#ifdef HAVE_CASTEM
static void treatCastem();
#endif /* HAVE_CASTEM */

#ifdef HAVE_ZMAT
static void treatZMAT();
#endif /* HAVE_ZMAT */

static void treatExceptions();

static void treatMath();

static void treatMathCubicSpline();

static void treatMathKriging();

static void treatMathParser();

static void treatMathInterpreter();

static void treatUtilities();

static void treatGlossary();

static void treatAll();

static void treatCppFlags();

static void treatIncludes();

static void treatLibs();

static void listOptions(std::ostream&);

TFEL_NORETURN static void treatUnknownOption(const std::string&);

TFEL_NORETURN static void treatHelp();

TFEL_NORETURN static void treatLicences();

static CallBacksContainer callBacksContainer;
static bool compilerflags = false;
static bool oflags0 = false;
static bool oflags = false;
static bool oflags2 = false;
static bool warning = false;
static bool incs = false;
static bool cppflags = false;
static bool libs = false;
static bool exceptions = false;
static bool math = false;
static bool mathCubicSpline = false;
static bool mathKriging = false;
static bool mathParser = false;
static bool mathInterpreter = false;
static bool utilities = false;
static bool glossary = false;
static bool material = false;
static bool tests = false;
static bool mfront_profiling = false;
#ifdef HAVE_CASTEM
static bool castem = false;
#endif /* HAVE_CASTEM */
#ifdef HAVE_ZMAT
static bool zmat = false;
#endif /* HAVE_ZMAT */
static bool lsystem = false;

#if defined _WIN32 || defined _WIN64
static bool getValueInRegistry(std::string& value) {
  using namespace std;
  HKEY hKey;
  char szBuffer[512];
  DWORD dwBufferSize = sizeof(szBuffer);
  LONG nError;
  LONG lRes =
      RegOpenKeyEx(HKEY_CLASSES_ROOT, "TFELHOME-" VERSION, 0, KEY_READ, &hKey);
  if (ERROR_SUCCESS != lRes) {
    return false;
  }
  nError = RegQueryValueEx(hKey, "", nullptr, nullptr,
                           reinterpret_cast<LPBYTE>(szBuffer), &dwBufferSize);
  RegCloseKey(hKey);
  if (ERROR_SUCCESS == nError) {
    value = szBuffer;
    return true;
  }
  return false;
}
#endif

static std::string getTFELHOME() {
  using namespace std;
#if defined _WIN32 || defined _WIN64
  // check in the registry (installation through NSIS)
  string rpath;
  if (getValueInRegistry(rpath)) {
    return handleSpace(rpath);
  }
#endif

  const char* const path = getenv("TFELHOME");
  if (path != nullptr) {
    return handleSpace(path);
  }

#if defined _WIN32 || defined _WIN64
  string msg(
      "tfel-config getTFELHOME: "
      "no TFELHOME registry key defined and no TFEHOME "
      "environment variable defined");
  throw(runtime_error(msg));
#else
  return "";
#endif
}

static std::string libDir() {
  using namespace std;
  const string prefix(PREFIXDIR);
  const string execPrefix(EXECPREFIXDIR);
  string lib(LIBDIR);
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
  const string ldir("/bin");
#else
#ifdef LIB_SUFFIX
  const string ldir("/lib" LIB_SUFFIX);
#else
  const string ldir("/lib");
#endif
#endif
  const auto& th = getTFELHOME();
  if (!th.empty()) {
    return th + ldir;
  }
  if (lib.substr(0, 14) == "${exec_prefix}") {
    if (execPrefix == "${prefix}") {
      lib = prefix + ldir;
    } else {
      lib = execPrefix + ldir;
    }
  }
  return handleSpace(lib);
}  // end of libDir

static std::string includeDir() {
  const std::string prefix(PREFIXDIR);
  std::string inc(INCLUDEDIR);
  const auto& th = getTFELHOME();
#ifdef TFEL_APPEND_VERSION
  if (!th.empty()) {
    return th + "/include/TFEL-" VERSION;
  } else {
    if (inc.substr(0, 9) == "${prefix}") {
      inc = handleSpace(prefix + "/include/TFEL-" VERSION);
    }
  }
#else  /* TFEL_APPEND_VERSION  */
  if (!th.empty()) {
    return th + "/include";
  } else {
    if (inc.substr(0, 9) == "${prefix}") {
      inc = handleSpace(prefix + "/include");
    }
  }
#endif /* TFEL_APPEND_VERSION  */
  return inc;
}  // end of includeDir

static void replace_all(std::string& r,
                        const std::string& s,
                        const std::string& s1,
                        const std::string& s2) {
  using namespace std;
  string::size_type rs;
  string::size_type pos = 0;
  string::size_type p;
  r.clear();
  if (!s1.empty()) {
    p = s.find(s1, pos);
    while (p != string::npos) {
      rs = r.size();
      r.resize(rs + p - pos + s2.size());
      //	  copy(s.begin()+pos,s.begin()+p,r.begin()+rs);
      copy(&s[0] + pos, &s[0] + p, &r[0] + rs);
      copy(s2.begin(), s2.end(), &r[0] + rs + p - pos);
      pos = p + s1.size();
      p = s.find(s1, pos);
    }
  }
  rs = r.size();
  r.resize(rs + s.size() - pos);
  copy(&s[0] + pos, &s[0] + s.size(), &r[0] + rs);
}

static void registerCallBack(const std::string& key,
                             const FuncPtr& f,
                             const std::string& description) {
  callBacksContainer.insert({key, {f, description}});
}  // end of registerNewCallBack

static void treatCompilerFlags() {
  compilerflags = true;
}  // end of treatCompilerFlags

static void treatOFlags0() { oflags0 = true; }  // end of treatOFlags

static void treatOFlags() {
  oflags0 = true;
  oflags = true;
}  // end of treatOFlags

static void treatOFlags2() { oflags2 = true; }  // end of treatOFlags2

static void treatWarning() { warning = true; }  // end of treatWarning

#ifdef HAVE_CASTEM
static void treatCastem() { castem = true; }  // end of treatCastem
#endif                                        /* HAVE_CASTEM */

#ifdef HAVE_ZMAT
static void treatZMAT() { zmat = true; }  // end of treatZMAT
#endif                                    /* HAVE_ZMAT */

static void treatExceptions() { exceptions = true; }  // end of treatExceptions

static void treatMath() {
  exceptions = true;
  math = true;
}  // end of treatMath

static void treatMathKriging() {
  exceptions = true;
  math = true;
  mathKriging = true;
}  // end of treatMathKriging

static void treatMathCubicSpline() {
  exceptions = true;
  math = true;
  mathCubicSpline = true;
}  // end of treatMathCubicSpline

static void treatMathParser() {
  exceptions = true;
  math = true;
  mathKriging = true;
  mathParser = true;
}  // end of treatMathParser

static void treatMathInterpreter() {
  exceptions = true;
  math = true;
  mathKriging = true;
  mathParser = true;
  mathInterpreter = true;
}  // end of treatMathInterpreter

static void treatUtilities() { utilities = true; }  // end of treatUtilities

static void treatGlossary() { glossary = true; }  // end of treatGlossary

static void treatSystem() {
  exceptions = true;
  lsystem = true;
}  // end of treatSystem

static void treatMaterial() {
  exceptions = true;
  math = true;
  utilities = true;
  material = true;
}  // end of treatMaterial

static void treatMFrontProfiling() {
  mfront_profiling = true;
}  // end of treatMFrontProfiling

static void treatTests() { tests = true; }  // end of treatTests

static void treatAll() {
  exceptions = true;
  math = true;
  material = true;
  utilities = true;
  glossary = true;
  lsystem = true;
  tests = true;
  mfront_profiling = true;
}  // end of treatAll

static void treatCppFlags() { cppflags = true; }  // end of treatCppFlags

static void treatIncludes() { incs = true; }  // end of treatIncludes

static void treatLibs() { libs = true; }  // end of treatLibs

static void listOptions(std::ostream& os) {
  using namespace std;
  CallBacksContainer::const_iterator p;
  os << "Valid options are : " << endl;
  for (p = callBacksContainer.begin(); p != callBacksContainer.end(); ++p) {
    string options(p->first);
    if (options.size() <= 18) {
      options.insert(options.size(), 18 - options.size(), ' ');
    }
    os << options << " : " << p->second.second << endl;
  }
}  // end of listOptions

[[noreturn]] static void treatVersion() {
  std::cout << "tfel-config " << VERSION << " (git hash: " << TFEL_GIT_HASH
            << ")" << std::endl;
  std::exit(EXIT_SUCCESS);
}  // end of treatHelp

TFEL_NORETURN static void treatHelp() {
  using namespace std;
  cout << "usage : tfel-config [options]" << endl;
  listOptions(cout);
  exit(EXIT_SUCCESS);
}  // end of treatHelp

TFEL_NORETURN static void treatUnknownOption(const std::string& o) {
  using namespace std;
  cerr << "unknown option " << o << endl;
  listOptions(cerr);
  exit(EXIT_FAILURE);
}  // end of treatUnknownOption

TFEL_NORETURN static void treatLicences() {
  using namespace std;
  cout << "Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights reserved."
       << endl;
  cout << "This project is publicly released under either the GNU GPL Licence "
          "with linking exception "
          "or the"
       << endl;
  cout << "CECILL-A licence. A copy of thoses licences are delivered with the "
          "sources of"
       << endl;
  cout << "TFEL. CEA or EDF may also distribute this project under specific "
          "licensing"
       << endl;
  cout << "conditions." << endl;
  cout << endl;
  cout << "For further information, please read the licences files or visit "
          "the following"
       << endl;
  cout << "websites:" << endl;
  cout << "    http://www.gnu.org/licenses" << endl;
  cout << "    http://www.cecill.info/licences.en.html" << endl;
  cout << "    http://www.cecill.info/licences.fr.html" << endl;
  exit(EXIT_SUCCESS);
}  // end of treatLicences

static void listLibraries() {
  auto display_if = [](const bool b, const char* const s) {
#ifdef TFEL_APPEND_VERSION
    if (b) {
      std::cout << s << "-" << VERSION << ' ';
    }
#else  /* TFEL_APPEND_VERSION*/
    if (b) {
      std::cout << s << ' ';
    }
#endif /* TFEL_APPEND_VERSION*/
  };
  display_if(mfront_profiling, "-lMFrontProfiling");
  display_if(material, "-lTFELMaterial");
  display_if(mathInterpreter, "-lTFELMathInterpreter");
  display_if(mathParser, "-lTFELMathParser");
  display_if(mathCubicSpline, "-lTFELMathCubicSpline");
  display_if(mathKriging, "-lTFELMathKriging");
  display_if(math, "-lTFELMath");
  display_if(lsystem, "-lTFELSystem");
  display_if(utilities, "-lTFELUtilities");
  display_if(glossary, "-lTFELGlossary");
  display_if(exceptions, "-lTFELException");
  display_if(tests, "-lTFELTests");
}

/* coverity [UNCAUGHT_EXCEPT]*/
int main(const int argc, const char* const* const argv) {
  using namespace std;

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
  try {
#endif /* __CYGWIN__ */

    registerCallBack("--compiler-flags", &treatCompilerFlags,
                     "return tfel recommended compiler flags.");
    registerCallBack("--oflags0", &treatOFlags0,
                     "return tfel recommended optimisation flags without "
                     "architecture specific flags.");
    registerCallBack("--oflags", &treatOFlags,
                     "return tfel recommended optimisation flags with "
                     "architecture specific flags.");
    registerCallBack(
        "--oflags2", &treatOFlags2,
        "return some aggressive optimisation flags, possibly at the expense of "
        "numerical precision. This shall be added to `--oflags` results.");
    registerCallBack("--warning", &treatWarning,
                     "return tfel recommended warnings.");
    registerCallBack("--includes", &treatIncludes, "return tfel include path.");
    registerCallBack("--cppflags", &treatCppFlags,
                     "return preprocessor flags.");
    registerCallBack("--libs", &treatLibs, "return linking flags.");
    registerCallBack("--help", &treatHelp, "print this help message.");
#ifdef HAVE_CASTEM
    registerCallBack("--castem", &treatCastem, "request flags for castem.");
#endif /* HAVE_CASTEM */
#ifdef HAVE_ZMAT
    registerCallBack("--zmat", &treatZMAT, "request flags for zmat.");
#endif /* HAVE_ZMAT */
    registerCallBack("--exceptions", &treatExceptions,
                     "request flags for libTFELException.");
    registerCallBack("--math-kriging", &treatMathKriging,
                     "request flags for libTFELMathKriging.");
    registerCallBack("--math-cubic-spline", &treatMathCubicSpline,
                     "request flags for libTFELMathCubicSpline.");
    registerCallBack("--math", &treatMath, "request flags for libTFELMath.");
    registerCallBack("--math-parser", &treatMathParser,
                     "request flags for libTFELMathParser.");
    registerCallBack("--math-interpreter", &treatMathInterpreter,
                     "request flags for libTFELMathInterpreter.");
    registerCallBack("--tests", &treatTests, "request flags for libTFELTests.");
    registerCallBack("--system", &treatSystem,
                     "request flags for libTFELSystem.");
    registerCallBack("--utilities", &treatUtilities,
                     "request flags for libTFELUtilities.");
    registerCallBack("--glossary", &treatGlossary,
                     "request flags for libTFELGlossary.");
    registerCallBack("--material", &treatMaterial,
                     "request flags for libTFELMaterial.");
    registerCallBack("--mfront-profiling", &treatMFrontProfiling,
                     "request flags for libMFrontProfiling.");
    registerCallBack("--all", &treatAll, "request flags for all librairies.");
    registerCallBack("--version", &treatVersion,
                     "print tfel version and svn revision.");
    registerCallBack("--licence", &treatLicences, "print tfel licences.");
    for (auto p2 = argv + 1; p2 != argv + argc; ++p2) {
      auto p = callBacksContainer.find(*p2);
      if (p == callBacksContainer.end()) {
        treatUnknownOption(*p2);
      }
      (*(p->second.first))();
    }

    if (cppflags) {
#ifdef TFEL_ARCH32
      cout << "-DTFEL_ARCH32"
           << " ";
#endif
#ifdef TFEL_ARCH64
      cout << "-DTFEL_ARCH64"
           << " ";
#endif
    }

    if (incs) {
      cout << "-I" << includeDir() << " ";
    }

#ifdef HAVE_CASTEM
#ifdef LOCAL_CASTEM_HEADER
    if (castem) {
      if (!incs) {
        cout << "-I" << includeDir() << " ";
        cout << CASTEMFLAGS1 << " ";
      }
    }
#else  /* LOCAL_CASTEM_HEADER */
    if (castem) {
      if (!incs) {
        cout << CASTEMFLAGS1 << " ";
      }
      const char* const castempath = getenv("CASTEM_ROOT");
      if (castempath != nullptr) {
        cout << "-I" << castempath << "/include ";
      } else {
        cout << CASTEMFLAGS2 << " ";
      }
    }
#endif /* LOCAL_CASTEM_HEADER */
#endif /* HAVE_CASTEM */

#ifdef HAVE_ZMAT
    if (zmat) {
      if (!incs) {
        cout << ZMATFLAGS1 << " ";
      }
      const char* const zmatpath = getenv("ZSET_ROOT");
      if (zmatpath != nullptr) {
        cout << "-I" << zmatpath << "/include ";
      } else {
#ifndef ZSET_ROOT
        string msg(
            "tfel-config : no installation path to the ZSET softwares.\n"
            "Please define the 'ZSET_ROOT' variable environnement");
        throw(runtime_error(msg));
#else
        cout << ZMATFLAGS2 << " ";
#endif
      }
    }
#endif /* HAVE_ZMAT */

    if (libs) {
      cout << "-L" << libDir() << " ";
      listLibraries();
    }

    if (compilerflags) {
      const auto* const cxx_standard =
          getenv("TFEL_CXX_STANDARD_COMPILER_FLAG");
      if (cxx_standard != nullptr) {
        cout << " " << cxx_standard << " ";
        auto ncompiler_cxxflags = std::string{};
        replace_all(ncompiler_cxxflags, COMPILER_CXXFLAGS, "-std=c++11", "");
        cout << COMPILER_FLAGS << " " << ncompiler_cxxflags << " ";
      } else {
        cout << COMPILER_FLAGS << " " << COMPILER_CXXFLAGS << " ";
      }
    }

    if (oflags0) {
      cout << OPTIMISATION_FLAGS0 << " ";
    }

    if (oflags) {
      cout << OPTIMISATION_FLAGS << " ";
    }

    if (oflags2) {
      cout << OPTIMISATION_FLAGS2 << " ";
    }

    if (warning) {
      cout << COMPILER_WARNINGS << " ";
    }

#ifdef HAVE_CASTEM
    if (cppflags || incs || libs || oflags0 || oflags || oflags2 || warning ||
        castem) {
      cout << endl;
    }
#else
  if (cppflags || incs || libs || oflags0 || oflags || oflags2 || warning) {
    cout << endl;
  }
#endif /* HAVE_CASTEM */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
  } catch (exception& e) {
    MessageBox(nullptr, e.what(), "mfront", 0);
    return EXIT_FAILURE;
  } catch (...) {
    return EXIT_FAILURE;
  }
#endif /* __CYGWIN__ */

  return EXIT_SUCCESS;
}
