/*!
 * \file   src/TFELConfig/tfel-config.cxx
 * \brief
 * \author Thomas Helfer
 * \date   27/08/2007
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

#include "TFEL/Raise.hxx"
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Config/GetInstallPath-defines.hxx"
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

static std::vector<std::string> tokenize(const std::string& s, const char c) {
  auto res = std::vector<std::string>{};
  std::string::size_type b = 0u;
  std::string::size_type e = s.find_first_of(c, b);
  while (std::string::npos != e || std::string::npos != b) {
    // Found a token, add it to the vector.
    res.push_back(s.substr(b, e - b));
    b = s.find_first_not_of(c, e);
    e = s.find_first_of(c, b);
  }
  return res;
}  // end of tokenize

static std::string handleSpace(const std::string& p) {
#if (defined _WIN32 || defined _WIN64) && \
    (defined __MINGW32__ || defined __MINGW64__)
  if (std::find(p.begin(), p.end(), ' ') != p.end()) {
    tfel::raise(
        "tfel-config handleSpace: "
        "path to TFEL shall not contain space as "
        "MinGW can't handle it (Found '" +
        p +
        "'). "
        "Please change TFEL installation directory");
  }
#endif /* (defined _WIN32 || defined _WIN64) && \
          (defined __MINGW32__ || defined __MINGW64__) */
  return p;
}

static std::string getTFELHOME();

static std::string libDir();

static std::string includeDir();

static void registerCallBack(const std::string&,
                             const FuncPtr&,
                             const std::string&);

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
static bool incspath = false;
static bool cppflags = false;
static bool libsdeps = false;
static bool libspath = false;
static bool ldflags = false;
static bool exceptions = false;
static bool math = false;
static bool mathCubicSpline = false;
static bool mathKriging = false;
static bool mathParser = false;
static bool utilities = false;
static bool unicodeSupport = false;
static bool glossary = false;
static bool numodis = false;
static bool material = false;
static bool tests = false;
static bool physicalConstants = false;
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
#if defined _WIN32 || defined _WIN64
  // check in the registry (installation through NSIS)
  std::string rpath;
  if (getValueInRegistry(rpath)) {
    return handleSpace(rpath);
  }
#endif

  const char* const path = getenv("TFELHOME");
  if (path != nullptr) {
    return handleSpace(path);
  }

#if defined _WIN32 || defined _WIN64
  throw(
      std::runtime_error("tfel-config getTFELHOME: "
                         "no TFELHOME registry key defined "
                         "and no TFEHOME environment "
                         "variable defined"));
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
  if (!th.empty()) {
#ifdef TFEL_APPEND_SUFFIX
    return th + "/include" + "/TFEL-" + TFEL_SUFFIX;
#else  /* TFEL_APPEND_SUFFIX */
    return th + "/include";
#endif /* TFEL_APPEND_SUFFIX */
  } else {
    if (inc.substr(0, 9) == "${prefix}") {
      inc = handleSpace(prefix + "/include");
    }
  }
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

static void listOptions(std::ostream& os) {
  os << "Valid options are:\n";
  for (const auto& c : callBacksContainer) {
    auto opt = c.first;
    if (opt.size() <= 18) {
      opt.insert(opt.size(), 18 - opt.size(), ' ');
    }
    os << opt << ": " << c.second.second << '\n';
  }
}  // end of listOptions

TFEL_NORETURN static void treatVersion() {
  std::cout << "tfel-config " << VERSION << " (git hash: " << TFEL_GIT_HASH
            << ")" << std::endl;
  std::exit(EXIT_SUCCESS);
}  // end of treatHelp

TFEL_NORETURN static void treatHelp() {
  std::cout << "usage : tfel-config [options]" << std::endl;
  listOptions(std::cout);
  std::exit(EXIT_SUCCESS);
}  // end of treatHelp

TFEL_NORETURN static void treatUnknownOption(const std::string& o) {
  std::cerr << "unknown option " << o << std::endl;
  listOptions(std::cerr);
  std::exit(EXIT_FAILURE);
}  // end of treatUnknownOption

TFEL_NORETURN static void treatLicences() {
  std::cout
      << "Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights reserved.\n";
  std::cout << "This project is publicly released under either the GNU GPL "
               "Licence or the\n";
  std::cout << "CECILL-A licence. A copy of thoses licences are delivered with "
               "the sources of\n";
  std::cout << "TFEL. CEA or EDF may also distribute this project under "
               "specific licensing\n";
  std::cout << "conditions.\n\n";
  std::cout << "For further information, please read the licences files or "
               "visit the following\n";
  std::cout << "websites:\n";
  std::cout << "    http://www.gnu.org/licenses\n";
  std::cout << "    http://www.cecill.info/licences.en.html\n";
  std::cout << "    http://www.cecill.info/licences.fr.html" << std::endl;
  std::exit(EXIT_SUCCESS);
}  // end of treatLicences

static void listLibraries(const char* p) {
  auto display_if = [&p](const bool b, const char* s) {
#ifdef TFEL_APPEND_SUFFIX
    if (b) {
      std::cout << p << s << "-" << TFEL_SUFFIX << ' ';
    }
#else  /* TFEL_APPEND_SUFFIX*/
    if (b) {
      std::cout << p << s << ' ';
    }
#endif /* TFEL_APPEND_SUFFIX*/
  };
  display_if(mfront_profiling, "MFrontProfiling");
  display_if(material, "TFELMaterial");
  display_if(mathParser, "TFELMathParser");
  display_if(mathCubicSpline, "TFELMathCubicSpline");
  display_if(mathKriging, "TFELMathKriging");
  display_if(math, "TFELMath");
  display_if(lsystem, "TFELSystem");
  display_if(utilities, "TFELUtilities");
  display_if(unicodeSupport, "TFELUnicodeSupport");
  display_if(glossary, "TFELGlossary");
  display_if(exceptions, "TFELException");
  display_if(numodis, "TFELNUMODIS");
  display_if(tests, "TFELTests");
  display_if(physicalConstants, "TFELPhysicalConstants");
}

/* coverity [UNCAUGHT_EXCEPT]*/
int main(const int argc, const char* const* const argv) {
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
  try {
#endif /* __CYGWIN__ */

    registerCallBack(
        "--compiler-flags", [] { compilerflags = true; },
        "return tfel recommended compiler flags.");
    registerCallBack(
        "--oflags0", [] { oflags0 = true; },
        "return tfel recommended optimisation "
        "flags without architecture specific flags.");
    registerCallBack(
        "--oflags", [] { oflags0 = oflags = true; },
        "return tfel recommended optimisation "
        "flags with architecture specific flags.");
    registerCallBack(
        "--oflags2", [] { oflags2 = true; },
        "return some aggressive optimisation flags, "
        "possibly at the expense of numerical precision. "
        "This shall be added to `--oflags` results.");
    registerCallBack(
        "--warning", [] { warning = true; },
        "return tfel recommended warnings.");
    registerCallBack(
        "--includes", [] { incs = true; }, "return tfel include path.");
    registerCallBack(
        "--cppflags", [] { cppflags = true; }, "return preprocessor flags.");
    registerCallBack(
        "--ldflags", [] { ldflags = true; }, "return linking flags.");
    registerCallBack(
        "--libs", [] { ldflags = true; }, "return linking flags.");
    registerCallBack(
        "--include-path", [] { incspath = true; },
        "return the path to the `TFEL` headers.");
    registerCallBack(
        "--library-path", [] { libspath = true; },
        "return the path to the `TFEL` library.");
    registerCallBack(
        "--library-dependency", [] { libsdeps = true; },
        "return the dependencies of a `TFEL` library.");
    registerCallBack("--help", &treatHelp, "print this help message.");
#ifdef HAVE_CASTEM
    registerCallBack(
        "--castem", [] { castem = true; }, "request flags for castem.");
#endif /* HAVE_CASTEM */
#ifdef HAVE_ZMAT
    registerCallBack(
        "--zmat", [] { zmat = true; }, "request flags for zmat.");
#endif /* HAVE_ZMAT */
    registerCallBack(
        "--exceptions", [] { exceptions = true; },
        "request flags for TFELException.");
    registerCallBack(
        "--math", [] { math = exceptions = true; },
        "request flags for TFELMath.");
    registerCallBack(
        "--math-kriging", [] { mathKriging = math = exceptions = true; },
        "request flags for TFELMathKriging.");
    registerCallBack(
        "--math-cubic-spline",
        [] { mathCubicSpline = math = exceptions = true; },
        "request flags for TFELMathCubicSpline.");
    registerCallBack(
        "--math-parser",
        [] {
#if __cplusplus < 201703L
          physicalConstants = true;
#endif /* __cplusplus > 201703L */
          math = exceptions = true;
          unicodeSupport = true;
          mathParser = mathKriging = true;
        },
        "request flags for TFELMathParser.");
    registerCallBack(
        "--tests", [] { tests = true; }, "request flags for TFELTests.");
    registerCallBack(
        "--physical-constants", [] { physicalConstants = true; },
        "request flags for TFELPhysicalConstants.");
    registerCallBack(
        "--system", [] { lsystem = exceptions = true; },
        "request flags for TFELSystem.");
    registerCallBack(
        "--utilities", [] { utilities = true; },
        "request flags for TFELUtilities.");
    registerCallBack(
        "--unicode-support", [] { unicodeSupport = true; },
        "request flags for TFELUnicodeSupport.");
    registerCallBack(
        "--glossary", [] { glossary = true; },
        "request flags for TFELGlossary.");
    registerCallBack(
        "--material",
        [] {
          exceptions = numodis = true;
          material = utilities = math = true;
        },
        "request flags for TFELMaterial.");
    registerCallBack(
        "--numodis", [] { numodis = true; }, "request flags for TFELNUMODIS.");
    registerCallBack(
        "--mfront-profiling", [] { mfront_profiling = true; },
        "request flags for libMFrontProfiling.");
    registerCallBack(
        "--all",
        [] {
          exceptions = math = numodis = material = true;
          utilities = glossary = lsystem = tests = true;
          unicodeSupport = true;
          mfront_profiling = true;
        },
        "request flags for all librairies.");
    registerCallBack("--version", &treatVersion,
                     "print tfel version and svn revision.");
    registerCallBack(
        "--major-number",
        [] {
          const auto v = tokenize(VERSION, '.');
          if (v.size() >= 1) {
            std::cout << v[0] << " ";
          }
        },
        "print tfel major version.");
    registerCallBack(
        "--minor-number",
        [] {
          const auto v = tokenize(VERSION, '.');
          if (v.size() >= 2) {
            std::cout << v[1] << " ";
          }
        },
        "print tfel minor version.");
    registerCallBack(
        "--revision-number",
        [] {
          const auto v = tokenize(VERSION, '.');
          if (v.size() >= 3) {
            std::cout << v[2] << " ";
          }
        },
        "print tfel revision version.");
    registerCallBack(
        "--cxx-standard",
        [] {
#if __cplusplus >= 201703L
          std::cout << "17";
#elif __cplusplus >= 201402L
        std::cout << "14";
#else  /* __cplusplus >= 201703L */
        std::cout << "11";
#endif /* __cplusplus >= 201703L */
        },
        "print the version of the C++ standard "
        "used to compile TFEL.");
    registerCallBack("--licence", &treatLicences, "print tfel licences.");
#ifdef TFEL_PYTHON_BINDINGS
    registerCallBack(
        "--python-version", [] { std::cout << PYTHON_VERSION << " "; },
        "print the python version used to build the python bindings.");
#endif /* TFEL_PYTHON_BINDINGS */

    if (argc == 1) {
      treatHelp();
    }

    for (auto p2 = argv + 1; p2 != argv + argc; ++p2) {
      const auto p = callBacksContainer.find(*p2);
      if (p == callBacksContainer.end()) {
        treatUnknownOption(*p2);
      }
      (*(p->second.first))();
    }

    if (cppflags) {
#ifdef TFEL_ARCH32
      std::cout << "-DTFEL_ARCH32"
                << " ";
#endif
#ifdef TFEL_ARCH64
      std::cout << "-DTFEL_ARCH64"
                << " ";
#endif
    }

    if (incs) {
      std::cout << "-I" << includeDir() << " ";
    }
    if (incspath) {
      std::cout << includeDir() << " ";
    }

#ifdef HAVE_CASTEM
#ifdef LOCAL_CASTEM_HEADER
    if (castem) {
      if (!incs) {
        std::cout << "-I" << includeDir() << " ";
        std::cout << CASTEMFLAGS1 << " ";
      }
    }
#else  /* LOCAL_CASTEM_HEADER */
    if (castem) {
      if (!incs) {
        std::cout << CASTEMFLAGS1 << " ";
      }
      const auto castempath = getenv("CASTEM_ROOT");
      if (castempath != nullptr) {
        std::cout << "-I" << castempath << "/include ";
      } else {
        std::cout << CASTEMFLAGS2 << " ";
      }
    }
#endif /* LOCAL_CASTEM_HEADER */
#endif /* HAVE_CASTEM */

#ifdef HAVE_ZMAT
    if (zmat) {
      if (!incs) {
        std::cout << ZMATFLAGS1 << " ";
      }
      const auto* zmatpath = getenv("Z7PATH");
      if (zmatpath != nullptr) {
        std::cout << "-I" << zmatpath << "/include ";
      } else if ((zmatpath = getenv("ZSET_ROOT")) != nullptr) {
        std::cout << "-I" << zmatpath << "/include";
      } else {
#ifndef ZSET_ROOT
        throw(
            std::runtime_error("tfel-config : no installation path "
                               "to the ZSET softwares.\n"
                               "Please define the 'ZSET_ROOT' "
                               "variable environnement"));
#else
        std::cout << ZMATFLAGS2 << " ";
#endif
      }
    }
#endif /* HAVE_ZMAT */

    if (libspath) {
      std::cout << libDir() << " ";
    }

    if (ldflags) {
      std::cout << "-L" << libDir() << " ";
      listLibraries("-l");
    }

    if (libsdeps) {
      listLibraries("");
    }

    if (compilerflags) {
      const auto* const cxx_standard =
          getenv("TFEL_CXX_STANDARD_COMPILER_FLAG");
      if (cxx_standard != nullptr) {
        std::cout << " " << cxx_standard << " ";
        auto ncompiler_cxxflags = std::string{};
        replace_all(ncompiler_cxxflags, COMPILER_CXXFLAGS, "-std=c++11", "");
        std::cout << COMPILER_FLAGS << " " << ncompiler_cxxflags << " ";
      } else {
        std::cout << COMPILER_FLAGS << " " << COMPILER_CXXFLAGS << " ";
      }
    }

    if (oflags0) {
      std::cout << OPTIMISATION_FLAGS0 << " ";
    }

    if (oflags) {
      std::cout << OPTIMISATION_FLAGS << " ";
    }

    if (oflags2) {
      std::cout << OPTIMISATION_FLAGS2 << " ";
    }

    if (warning) {
      std::cout << COMPILER_WARNINGS << " ";
    }

    std::cout << std::endl;

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
  } catch (std::exception& e) {
    MessageBox(nullptr, e.what(), "mfront", 0);
    return EXIT_FAILURE;
  } catch (...) {
    return EXIT_FAILURE;
  }
#endif /* __CYGWIN__ */

  return EXIT_SUCCESS;
}
