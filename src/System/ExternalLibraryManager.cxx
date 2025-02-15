/*!
 * \file   src/System/ExternalLibraryManager.cxx
 * \brief
 * \author Thomas Helfer
 * \date   31 Oct 2007
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cctype>
#include <cstring>
#include <fstream>
#include <stdexcept>
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <dlfcn.h>
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */

#include <iterator>

#include "TFEL/Raise.hxx"
#include "TFEL/System/getFunction.h"
#include "TFEL/System/LibraryInformation.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"

namespace tfel::system {

#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
  // code retrieved from
  // http://www.codeproject.com/Tips/479880/GetLastError-as-std-string
  static std::string getLastWin32Error() {
    const DWORD error = GetLastError();
    if (error) {
      LPVOID lpMsgBuf;
      DWORD bufLen = FormatMessage(
          FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
              FORMAT_MESSAGE_IGNORE_INSERTS,
          nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
          (LPTSTR)&lpMsgBuf, 0, nullptr);
      if (bufLen) {
        LPCSTR lpMsgStr = (LPTSTR)lpMsgBuf;
        std::string result(lpMsgStr, lpMsgStr + bufLen);
        LocalFree(lpMsgBuf);
        return result;
      }
    }
    return std::string();
  }
#endif /*  (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */

  static std::string getErrorMessage() {
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
    return getLastWin32Error();
#else
    const auto e = ::dlerror();
    if (e != nullptr) {
      return std::string(e);
    }
    return "";
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
  }    // end of  getErrorMessage

  static void ExternalLibraryManagerCheckModellingHypothesisName(
      const std::string& h) {
    tfel::raise_if(!((h == "AxisymmetricalGeneralisedPlaneStrain") ||
                     (h == "AxisymmetricalGeneralisedPlaneStress") ||
                     (h == "Axisymmetrical") || (h == "PlaneStress") ||
                     (h == "PlaneStrain") || (h == "GeneralisedPlaneStrain") ||
                     (h == "Tridimensional")),
                   "ExternalLibraryManagerCheckModellingHypothesisName : "
                   "invalid or unsupported hypothesis '" +
                       h +
                       "'. The following "
                       "hypotheses are supported:\n"
                       "- AxisymmetricalGeneralisedPlaneStrain\n"
                       "- Axisymmetrical\n"
                       "- PlaneStress\n"
                       "- PlaneStrain\n"
                       "- GeneralisedPlaneStrain\n"
                       "- Tridimensional");
  }  // end of ExternalLibraryManagerCheckModellingHypothesisName

  static std::string decomposeVariableName(const std::string& n) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "tfel::system::decomposeVariableName: " + m);
    };
    auto p = n.cbegin();
    auto pe = n.cend();
    while ((p != pe) && (*p != '[')) {
      ++p;
    }
    if (p == pe) {
      return n;
    }
    auto r = std::string{n.cbegin(), p};
    ++p;
    throw_if(p == pe, "unexpected end of string 'n'");
    throw_if(!std::isdigit(*p), "unexpected a digit 'n'");
    r += "_mfront_index_";
    while ((p != pe) && (std::isdigit(*p))) {
      r.push_back(*p);
      ++p;
    }
    throw_if(p == pe, "unexpected end of string '" + n + "'");
    throw_if(*p != ']', "invalid variable name '" + n + "'");
    ++p;
    throw_if(p != pe, "invalid variable name '" + n + "'");
    return r;
  }  // end of decomposeVariableName

  ExternalLibraryManager& ExternalLibraryManager::getExternalLibraryManager() {
    static ExternalLibraryManager elm;
    return elm;
  }  // end of getExternalLibraryManager

  ExternalLibraryManager::ExternalLibraryManager() = default;

#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
  static HINSTANCE__*
#else
  static void*
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
  load_library(const std::string& l) {
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
    return ::LoadLibrary(TEXT(l.c_str()));
#else
    return ::dlopen(l.c_str(), RTLD_NOW);
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
  }    // end of load_library

#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
  static std::pair<HINSTANCE__*, std::string>
#else
  static std::pair<void*, std::string>
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
  try_open(const std::string& l) {
    auto starts_with = [](const std::string& s1, const char* const s2) {
      const auto ls2 = std::strlen(s2);
      return ((s1.size() >= ls2) && (std::equal(s2, s2 + ls2, s1.begin())));
    };  // end of starts_with
    auto ends_with = [](const std::string& s1, const char* const s2) {
      const auto ls2 = std::strlen(s2);
      if (!(s1.size() >= ls2)) {
        return false;
      }
      return std::equal(s2, s2 + ls2, s1.begin() + (s1.size() - ls2));
    };  // end of ends_with
#if (defined(macintosh) || defined(Macintosh) || \
     (defined(__APPLE__) && defined(__MACH__)))
    const char* const ext = ".dylib";
#elif (defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__))
    const char* const ext = ".dll";
#else
    const char* const ext = ".so";
#endif
    auto ln = l;
    auto lib = load_library(l);
#if defined(__CYGWIN__)
    if ((lib == nullptr) && (!starts_with(l, "cyg"))) {
      ln = "cyg" + l;
      lib = load_library(ln);
      if (lib == nullptr) {
        if (!ends_with(l, ext)) {
          ln = "cyg" + l + ext;
          lib = load_library(ln);
        }
      }
    }
#endif
#if !(defined(_WIN32) || defined(_WIN64))
    if ((lib == nullptr) && (!starts_with(l, "lib"))) {
      ln = "lib" + l;
      lib = load_library(ln);
      if (lib == nullptr) {
        if (!ends_with(l, ext)) {
          ln = "lib" + l + ext;
          lib = load_library(ln);
        }
      }
    }
#endif
    if ((lib == nullptr) && (!ends_with(l, ext))) {
      ln = l + ext;
      lib = load_library(ln);
    }
    // retrieving the initial error message
    if (lib == nullptr) {
      ln = l;
      lib = load_library(ln);
    }
    return {lib, ln};
  }  // end of try_open

  std::string ExternalLibraryManager::getLibraryPath(const std::string& l) {
    auto raise = [](const std::string& m) {
      tfel::raise("ExternalLibraryManager::getLibraryPath: " + m);
    };
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
    auto lib = this->loadLibrary(l);
    char path[MAX_PATH];
    GetModuleFileNameA(lib, path, sizeof(path));
    return std::string{path};
#else
    auto tokenize = [](const std::string& s, const char c) {
      std::vector<std::string> r;
      std::string::size_type b = 0u;
      std::string::size_type e = s.find_first_of(c, b);
      while (std::string::npos != e || std::string::npos != b) {
        // Found a token, add it to the vector.
        r.push_back(s.substr(b, e - b));
        b = s.find_first_not_of(c, e);
        e = s.find_first_of(c, b);
      }
      return r;
    };
    auto exists = [](const std::string& f) {
      std::ifstream file(f);
      return static_cast<bool>(file);
    };
    auto lib = try_open(l);
    if (lib.first == nullptr) {
      raise("can't load library '" + l + "'");
    }
    // check if file exists
    if (exists(lib.second)) {
      return lib.second;
    }
    // look in LD_LIBRARY_PATH
    const auto ld = std::getenv("LD_LIBRARY_PATH");
    if (ld == nullptr) {
      raise("can't find library '" + l + "'");
    }
    for (const auto& p : tokenize(ld, ':')) {
      const auto lp = p + '/' + lib.second;
      if (exists(lp)) {
        return lp;
      }
    }
    tfel::raise(
        "ExternalLibraryManager::getLibraryPath: "
        "can't find library '" +
        l + "'");
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
  }    // end of getLibraryPath

#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
  HINSTANCE__*
#else
  void*
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
  ExternalLibraryManager::loadLibrary(const std::string& name, const bool b) {
    auto p = this->librairies.find(name);
    if (p == librairies.end()) {
      // this library has not been
      auto r = try_open(name);
      auto lib = r.first;
      raise_if((lib == nullptr) && (!b),
               "ExternalLibraryManager::loadLibrary:"
               " library '" +
                   name +
                   "' could not be loaded, "
                   "(" +
                   getErrorMessage() + ")");
      if ((lib == nullptr) && (b)) {
        return lib;
      }
      this->librairies.insert({name, lib});
      return lib;
    }
    return p->second;
  }  // end of loadLibrary

  std::vector<std::string> ExternalLibraryManager::getEntryPoints(
      const std::string& l) {
    auto ends_with = [](const std::string& s1, const std::string& s2) {
      return ((s1.size() >= s2.size()) &&
              (std::equal(s2.rbegin(), s2.rend(), s1.rbegin())));
    };  // end of ends_with
    auto r = std::vector<std::string>{};
    auto lib = try_open(l);
    raise_if(lib.first == nullptr,
             "ExternalLibraryManager::getEntryPoints:"
             " library '" +
                 l +
                 "' could not be loaded, "
                 "(" +
                 getErrorMessage() + ")");
    auto pl = this->getLibraryPath(lib.second);
    for (const auto& s : LibraryInformation(pl).symbols()) {
      if (ends_with(s, "_mfront_ept")) {
        r.push_back(s.substr(0, s.size() - 11));
      }
    }
    return r;
  }  // end of getEntryPoints

  unsigned short ExternalLibraryManager::getMaterialKnowledgeType(
      const std::string& l, const std::string& f) {
    auto throw_if = [l, f](const bool c, const std::string& m) {
      raise_if(c, "ExternalLibraryManager::getMaterialKnowledgeType: " + m);
    };
    const auto lib = this->loadLibrary(l);
    const int nb = ::tfel_getUnsignedShort(lib, (f + "_mfront_mkt").c_str());
    throw_if(nb == -1,
             "the material knowledge type could not be read "
             "(" +
                 getErrorMessage() + ")");
    if (nb == 0) {
      return 0u;
    } else if (nb == 1) {
      return 1u;
    }
    throw_if(nb != 2, "invalid material knowledge type");
    return 2u;
  }  // end of getMaterialKnowledgeType

  bool ExternalLibraryManager::contains(const std::string& l,
                                        const std::string& s) {
    const auto lib = this->loadLibrary(l);
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
    int (*p)() = (int (*)())::GetProcAddress(lib, s.c_str());
    return p != static_cast<int (*)()>(nullptr);
#else
    void* p = ::dlsym(lib, s.c_str());
    return p != nullptr;
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
  }    // end of contains

  std::string ExternalLibraryManager::getString(const std::string& l,
                                                const std::string& s) {
    if (!this->contains(l, s)) {
      tfel::raise(
          "ExternalLibraryManager::getString: "
          "no symbol named '" +
          s + "' in library '" + l + "'");
    }
    return this->getStringIfDefined(l, s);
  }  // end of ExternalLibraryManager::getString

  std::string ExternalLibraryManager::getStringIfDefined(const std::string& l,
                                                         const std::string& s) {
    const auto lib = this->loadLibrary(l);
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
    const auto p = (const char* const*)::GetProcAddress(lib, s.c_str());
#else
    const auto p = ::dlsym(lib, s.c_str());
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
    if (p == nullptr) {
      return "";
    }
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
    return *p;
#else
    return *(static_cast<const char* const*>(p));
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
  }    // end of ExternalLibraryManager::getStringIfDefined

  std::string ExternalLibraryManager::getAuthor(const std::string& l,
                                                const std::string& s) {
    return this->getStringIfDefined(l, s + "_author");
  }  // end of getAuthor

  std::string ExternalLibraryManager::getDate(const std::string& l,
                                              const std::string& s) {
    return this->getStringIfDefined(l, s + "_date");
  }  // end of getDate

  std::string ExternalLibraryManager::getDescription(const std::string& l,
                                                     const std::string& s) {
    return this->getStringIfDefined(l, s + "_description");
  }  // end of getDescription

  std::string ExternalLibraryManager::getValidator(const std::string& l,
                                                   const std::string& s) {
    return this->getStringIfDefined(l, s + "_validator");
  }  // end of getValidator

  std::string ExternalLibraryManager::getBuildId(const std::string& l,
                                                 const std::string& s) {
    return this->getBuildIdentifier(l, s);
  }  // end of getBuildId

  std::string ExternalLibraryManager::getBuildIdentifier(const std::string& l,
                                                         const std::string& s) {
    return this->getStringIfDefined(l, s + "_build_id");
  }  // end of getBuildIdentifier

  std::string ExternalLibraryManager::getSource(const std::string& l,
                                                const std::string& f) {
    return this->getStringIfDefined(l, f + "_src");
  }  // end of getSource

  std::string ExternalLibraryManager::getInterface(const std::string& l,
                                                   const std::string& f) {
    const auto lib = this->loadLibrary(l);
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
    const auto p = (const char* const*)::GetProcAddress(
        lib, (f + "_mfront_interface").c_str());
#else
    const auto p = ::dlsym(lib, (f + "_mfront_interface").c_str());
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
    raise_if(p == nullptr,
             "ExternalLibraryManager::getInterface: "
             "no interface found for entry point '" +
                 f +
                 "' "
                 "in library '" +
                 l + "'");
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
    return *p;
#else
    return *(static_cast<const char* const*>(p));
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
  }    // end of getInterface

  std::string ExternalLibraryManager::getLaw(const std::string& l,
                                             const std::string& f) {
    return this->getString(l, f + "_mfront_law");
  }  // end of getLaw

  std::string ExternalLibraryManager::getMaterial(const std::string& l,
                                                  const std::string& f) {
    return this->getStringIfDefined(l, f + "_mfront_material");
  }  // end of getMaterial

  std::string ExternalLibraryManager::getTFELVersion(const std::string& l,
                                                     const std::string& f) {
    return this->getStringIfDefined(l, f + "_tfel_version");
  }  // end of getTFELVersion

  std::string ExternalLibraryManager::getUnitSystem(const std::string& l,
                                                    const std::string& f) {
    return this->getStringIfDefined(l, f + "_unit_system");
  }  // end of getUnitSystem

  std::vector<std::string>
  ExternalLibraryManager::getSupportedModellingHypotheses(
      const std::string& l, const std::string& f) {
    std::vector<std::string> h;
    const auto lib = this->loadLibrary(l);
    const auto nb =
        ::tfel_getUnsignedShort(lib, (f + "_nModellingHypotheses").c_str());
    char** res;
    raise_if(nb == -1,
             "ExternalLibraryManager::"
             "getSupportedModellingHypotheses: "
             "number of modelling hypotheses could not be read (" +
                 getErrorMessage() + ")");
    res = ::tfel_getArrayOfStrings(lib, (f + "_ModellingHypotheses").c_str());
    raise_if(res == nullptr,
             "ExternalLibraryManager::"
             "getSupportedModellingHypotheses: "
             "modelling hypotheses could not be read (" +
                 getErrorMessage() + ")");
    std::copy(res, res + nb, std::back_inserter(h));
    return h;
  }  // end of getSupportedModellingHypotheses

  void ExternalLibraryManager::setOutOfBoundsPolicy(
      const std::string& l,
      const std::string& f,
      const tfel::material::OutOfBoundsPolicy p) {
    const auto lib = this->loadLibrary(l);
    int(TFEL_ADDCALL_PTR fct)(int);
    fct = ::tfel_getSetOutOfBoundsPolicyFunction(
        lib, (f + "_setOutOfBoundsPolicy").c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::setOutOfBoundsPolicy: "
             "can't get the '" +
                 f +
                 "_setOutOfBoundsPolicy' function "
                 "(" +
                 getErrorMessage() + ")");
    if (p == tfel::material::None) {
      fct(0);
    } else if (p == tfel::material::Warning) {
      fct(1);
    } else if (p == tfel::material::Strict) {
      fct(2);
    } else {
      raise(
          "ExternalLibraryManager::setOutOfBoundsPolicy: "
          "unsupported policy");
    }
  }  // end of setOutOfBoundsPolicy

  void ExternalLibraryManager::setParameter(const std::string& l,
                                            const std::string& f,
                                            const std::string& p,
                                            const double v) {
    const auto lib = this->loadLibrary(l);
    int(TFEL_ADDCALL_PTR fct)(const char* const, const double);
    fct = ::tfel_getSetParameterFunction(lib, (f + "_setParameter").c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::setParameter: "
             "can't get the '" +
                 f + "_setParameter' function (" + getErrorMessage() + ")");
    raise_if(!fct(p.c_str(), v),
             "ExternalLibraryManager::setParameter: "
             "call to the '" +
                 f + "_setParameter' function failed");
  }  // end of setParameter

  void ExternalLibraryManager::setParameter(const std::string& l,
                                            const std::string& f,
                                            const std::string& p,
                                            const int v) {
    const auto lib = this->loadLibrary(l);
    int(TFEL_ADDCALL_PTR fct)(const char* const, const int);
    fct = ::tfel_getSetIntegerParameterFunction(
        lib, (f + "_setIntegerParameter").c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::setParameter: "
             "can't get the '" +
                 f + "_setParameter' function (" + getErrorMessage() + ")");
    raise_if(!fct(p.c_str(), v),
             "ExternalLibraryManager::setParameter: "
             "call to the '" +
                 f + "_setParameter' function failed");
  }  // end of setParameter

  void ExternalLibraryManager::setParameter(const std::string& l,
                                            const std::string& f,
                                            const std::string& p,
                                            const unsigned short v) {
    const auto lib = this->loadLibrary(l);
    int(TFEL_ADDCALL_PTR fct)(const char* const, const unsigned short);
    fct = ::tfel_getSetUnsignedShortParameterFunction(
        lib, (f + "_setUnsignedShortParameter").c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::setParameter: "
             "can't get the '" +
                 f + "_setParameter' function (" + getErrorMessage() + ")");
    raise_if(!fct(p.c_str(), v),
             "ExternalLibraryManager::setParameter: "
             "call to the '" +
                 f + "_setParameter' function failed");
  }  // end of setParameter

  void ExternalLibraryManager::setParameter(const std::string& l,
                                            const std::string& f,
                                            const std::string& h,
                                            const std::string& p,
                                            const double v) {
    const auto lib = this->loadLibrary(l);
    int(TFEL_ADDCALL_PTR fct)(const char* const, const double);
    fct = ::tfel_getSetParameterFunction(
        lib, (f + "_" + h + "_setParameter").c_str());
    if (fct == nullptr) {
      fct = ::tfel_getSetParameterFunction(lib, (f + "_setParameter").c_str());
    }
    raise_if(fct == nullptr,
             "ExternalLibraryManager::setParameter: "
             "can't get the '" +
                 f + "_setParameter' function (" + getErrorMessage() + ")");
    raise_if(!fct(p.c_str(), v),
             "ExternalLibraryManager::setParameter: "
             "call to the '" +
                 f + "_setParameter' function failed");
  }  // end of setParameter

  void ExternalLibraryManager::setParameter(const std::string& l,
                                            const std::string& f,
                                            const std::string& h,
                                            const std::string& p,
                                            const int v) {
    const auto lib = this->loadLibrary(l);
    int(TFEL_ADDCALL_PTR fct)(const char* const, const int);
    fct = ::tfel_getSetIntegerParameterFunction(
        lib, (f + "_" + h + "_setIntegerParameter").c_str());
    if (fct == nullptr) {
      fct = ::tfel_getSetIntegerParameterFunction(
          lib, (f + "_setIntegerParameter").c_str());
    }
    raise_if(fct == nullptr,
             "ExternalLibraryManager::setParameter: "
             "can't get the '" +
                 f + "_setParameter' function (" + getErrorMessage() + ")");
    raise_if(!fct(p.c_str(), v),
             "ExternalLibraryManager::setParameter: "
             "call to the '" +
                 f + "_setParameter' function failed");
  }  // end of setParameter

  void ExternalLibraryManager::setParameter(const std::string& l,
                                            const std::string& f,
                                            const std::string& h,
                                            const std::string& p,
                                            const unsigned short v) {
    const auto lib = this->loadLibrary(l);
    int(TFEL_ADDCALL_PTR fct)(const char* const, const unsigned short);
    fct = ::tfel_getSetUnsignedShortParameterFunction(
        lib, (f + "_" + h + "_setUnsignedShortParameter").c_str());
    if (fct == nullptr) {
      fct = ::tfel_getSetUnsignedShortParameterFunction(
          lib, (f + "_setUnsignedShortParameter").c_str());
    }
    raise_if(fct == nullptr,
             "ExternalLibraryManager::setParameter: "
             "can't get the '" +
                 f + "_setParameter' function (" + getErrorMessage() + ")");
    raise_if(!fct(p.c_str(), v),
             "ExternalLibraryManager::setParameter: "
             "call to the '" +
                 f + "_setParameter' function failed");
  }  // end of setParameter

  double ExternalLibraryManager::getRealParameterDefaultValue(
      const std::string& l,
      const std::string& f,
      const std::string& h,
      const std::string& p) {
    const auto lib = this->loadLibrary(l);
    const auto pn = decomposeVariableName(p);
    const auto n1 = f + "_" + h + "_" + pn + "_ParameterDefaultValue";
    if (this->contains(l, n1)) {
      return tfel_getDouble(lib, n1.c_str());
    }
    const auto n2 = f + "_" + pn + "_ParameterDefaultValue";
    raise_if(!this->contains(l, n2),
             "ExternalLibraryManager::getRealParameterDefaultValue: "
             "can't get default value for parameter '" +
                 p + "'");
    return tfel_getDouble(lib, n2.c_str());
  }  // end of getRealParameterDefaultValue

  int ExternalLibraryManager::getIntegerParameterDefaultValue(
      const std::string& l,
      const std::string& f,
      const std::string& h,
      const std::string& p) {
    const auto lib = this->loadLibrary(l);
    const auto pn = decomposeVariableName(p);
    const auto n1 = f + "_" + h + "_" + pn + "_ParameterDefaultValue";
    if (this->contains(l, n1)) {
      return tfel_getInteger(lib, n1.c_str());
    }
    const auto n2 = f + "_" + pn + "_ParameterDefaultValue";
    raise_if(!this->contains(l, n2),
             "ExternalLibraryManager::getIntegerParameterDefaultValue: "
             "can't get default value for parameter '" +
                 p + "'");
    return tfel_getInteger(lib, n2.c_str());
  }  // end of getIntegerParameterDefaultValue

  unsigned short ExternalLibraryManager::getUnsignedShortParameterDefaultValue(
      const std::string& l,
      const std::string& f,
      const std::string& h,
      const std::string& p) {
    const auto lib = this->loadLibrary(l);
    const auto pn = decomposeVariableName(p);
    const auto n1 = f + "_" + h + "_" + pn + "_ParameterDefaultValue";
    auto res = ::tfel_getUnsignedShort(lib, n1.c_str());
    if (res < 0) {
      res = ::tfel_getUnsignedShort(
          lib, (f + "_" + pn + "_ParameterDefaultValue").c_str());
      raise_if(res < 0,
               "ExternalLibraryManager::"
               "getUnsignedShortParameterDefaultValue: "
               "information could not be read (" +
                   getErrorMessage() + ")");
    }
    return static_cast<unsigned short>(res);
  }  // end of getUnsignedShortParameterDefaultValue

  bool ExternalLibraryManager::hasBounds(const std::string& l,
                                         const std::string& f,
                                         const std::string& h,
                                         const std::string& n) {
    const auto vn = decomposeVariableName(n);
    const auto n1 = f + "_" + h + "_" + vn + "_LowerBound";
    const auto n2 = f + "_" + h + "_" + vn + "_UpperBound";
    const auto n3 = f + "_" + vn + "_LowerBound";
    const auto n4 = f + "_" + vn + "_UpperBound";
    return ((this->contains(l, n1)) || (this->contains(l, n2)) ||
            (this->contains(l, n3)) || (this->contains(l, n4)));
  }  // end of hasBounds

  bool ExternalLibraryManager::hasLowerBound(const std::string& l,
                                             const std::string& f,
                                             const std::string& h,
                                             const std::string& n) {
    const auto vn = decomposeVariableName(n);
    const auto n1 = f + "_" + h + "_" + vn + "_LowerBound";
    const auto n2 = f + "_" + vn + "_LowerBound";
    return ((this->contains(l, n1)) || (this->contains(l, n2)));
  }  // end of hasLowerBound

  bool ExternalLibraryManager::hasUpperBound(const std::string& l,
                                             const std::string& f,
                                             const std::string& h,
                                             const std::string& n) {
    const auto vn = decomposeVariableName(n);
    const auto n1 = f + "_" + h + "_" + vn + "_UpperBound";
    const auto n2 = f + "_" + vn + "_UpperBound";
    return ((this->contains(l, n1)) || (this->contains(l, n2)));
  }  // end of hasUpperBound

  long double ExternalLibraryManager::getLowerBound(const std::string& l,
                                                    const std::string& f,
                                                    const std::string& h,
                                                    const std::string& n) {
    const auto lib = this->loadLibrary(l);
    const auto vn = decomposeVariableName(n);
    const auto n1 = f + "_" + h + "_" + vn + "_LowerBound";
    if (this->contains(l, n1)) {
      return tfel_getLongDouble(lib, n1.c_str());
    }
    const auto n2 = f + "_" + vn + "_LowerBound";
    raise_if(!this->contains(l, n2),
             "ExternalLibraryManager::getLowerBound: "
             "no lower bound associated to variable '" +
                 vn + "'");
    return tfel_getLongDouble(lib, n2.c_str());
  }  // end of getLowerBound

  long double ExternalLibraryManager::getUpperBound(const std::string& l,
                                                    const std::string& f,
                                                    const std::string& h,
                                                    const std::string& n) {
    const auto lib = this->loadLibrary(l);
    const auto vn = decomposeVariableName(n);
    const auto n1 = f + "_" + h + "_" + vn + "_UpperBound";
    if (this->contains(l, n1)) {
      return tfel_getLongDouble(lib, n1.c_str());
    }
    const auto n2 = f + "_" + vn + "_UpperBound";
    raise_if(!this->contains(l, n2),
             "ExternalLibraryManager::getUpperBound: "
             "no upper bound associated to variable '" +
                 vn + "'");
    return tfel_getLongDouble(lib, n2.c_str());
  }  // end of getUpperBound

  bool ExternalLibraryManager::hasPhysicalBounds(const std::string& l,
                                                 const std::string& f,
                                                 const std::string& h,
                                                 const std::string& n) {
    const auto vn = decomposeVariableName(n);
    const auto n1 = f + "_" + h + "_" + vn + "_LowerPhysicalBound";
    const auto n2 = f + "_" + h + "_" + vn + "_UpperPhysicalBound";
    const auto n3 = f + "_" + vn + "_LowerPhysicalBound";
    const auto n4 = f + "_" + vn + "_UpperPhysicalBound";
    return ((this->contains(l, n1)) || (this->contains(l, n2)) ||
            (this->contains(l, n3)) || (this->contains(l, n4)));
  }  // end of hasPhysicalBounds

  bool ExternalLibraryManager::hasLowerPhysicalBound(const std::string& l,
                                                     const std::string& f,
                                                     const std::string& h,
                                                     const std::string& n) {
    const auto vn = decomposeVariableName(n);
    const auto n1 = f + "_" + h + "_" + vn + "_LowerPhysicalBound";
    const auto n2 = f + "_" + vn + "_LowerPhysicalBound";
    return ((this->contains(l, n1)) || (this->contains(l, n2)));
  }  // end of hasLowerPhysicalBound

  bool ExternalLibraryManager::hasUpperPhysicalBound(const std::string& l,
                                                     const std::string& f,
                                                     const std::string& h,
                                                     const std::string& n) {
    const auto vn = decomposeVariableName(n);
    const auto n1 = f + "_" + h + "_" + vn + "_UpperPhysicalBound";
    const auto n2 = f + "_" + vn + "_UpperPhysicalBound";
    return ((this->contains(l, n1)) || (this->contains(l, n2)));
  }  // end of hasUpperPhysicalBound

  long double ExternalLibraryManager::getLowerPhysicalBound(
      const std::string& l,
      const std::string& f,
      const std::string& h,
      const std::string& n) {
    const auto lib = this->loadLibrary(l);
    const auto vn = decomposeVariableName(n);
    const auto n1 = f + "_" + h + "_" + vn + "_LowerPhysicalBound";
    if (this->contains(l, n1)) {
      return tfel_getLongDouble(lib, n1.c_str());
    }
    const auto n2 = f + "_" + vn + "_LowerPhysicalBound";
    raise_if(!this->contains(l, n2),
             "ExternalLibraryManager::getLowerPhysicalBound: "
             "no physical lower bound associated to variable '" +
                 vn + "'");
    return tfel_getLongDouble(lib, n2.c_str());
  }  // end of getLowerPhysicalBound

  long double ExternalLibraryManager::getUpperPhysicalBound(
      const std::string& l,
      const std::string& f,
      const std::string& h,
      const std::string& n) {
    const auto lib = this->loadLibrary(l);
    const auto vn = decomposeVariableName(n);
    const auto n1 = f + "_" + h + "_" + vn + "_UpperPhysicalBound";
    if (this->contains(l, n1)) {
      return tfel_getLongDouble(lib, n1.c_str());
    }
    const auto n2 = f + "_" + vn + "_UpperPhysicalBound";
    raise_if(!this->contains(l, n2),
             "ExternalLibraryManager::getUpperPhysicalBound: "
             "no physical upper bound associated to variable '" +
                 vn + "'");
    return tfel_getLongDouble(lib, n2.c_str());
  }  // end of getUpperPhysicalBound

  bool ExternalLibraryManager::hasBounds(const std::string& l,
                                         const std::string& f,
                                         const std::string& n) {
    const auto vn = decomposeVariableName(n);
    const auto n1 = f + "_" + vn + "_LowerBound";
    const auto n2 = f + "_" + vn + "_UpperBound";
    return ((this->contains(l, n1)) || (this->contains(l, n2)));
  }  // end of hasBounds

  bool ExternalLibraryManager::hasLowerBound(const std::string& l,
                                             const std::string& f,
                                             const std::string& n) {
    const auto vn = decomposeVariableName(n);
    const auto n1 = f + "_" + vn + "_LowerBound";
    return ((this->contains(l, n1)));
  }  // end of hasLowerBound

  bool ExternalLibraryManager::hasUpperBound(const std::string& l,
                                             const std::string& f,
                                             const std::string& n) {
    const auto vn = decomposeVariableName(n);
    const auto n1 = f + "_" + vn + "_UpperBound";
    return ((this->contains(l, n1)));
  }  // end of hasUpperBound

  long double ExternalLibraryManager::getLowerBound(const std::string& l,
                                                    const std::string& f,
                                                    const std::string& n) {
    const auto lib = this->loadLibrary(l);
    const auto vn = decomposeVariableName(n);
    const auto n1 = f + "_" + vn + "_LowerBound";
    raise_if(!this->contains(l, n1),
             "ExternalLibraryManager::getLowerBound: "
             "no lower bound associated to variable '" +
                 vn + "'");
    return tfel_getLongDouble(lib, n1.c_str());
  }  // end of getLowerBound

  long double ExternalLibraryManager::getUpperBound(const std::string& l,
                                                    const std::string& f,
                                                    const std::string& n) {
    const auto lib = this->loadLibrary(l);
    const auto vn = decomposeVariableName(n);
    const auto n1 = f + "_" + vn + "_UpperBound";
    raise_if(!this->contains(l, n1),
             "ExternalLibraryManager::getUpperBound: "
             "no upper bound associated to variable '" +
                 vn + "'");
    return tfel_getLongDouble(lib, n1.c_str());
  }  // end of getUpperBound

  bool ExternalLibraryManager::hasPhysicalBounds(const std::string& l,
                                                 const std::string& f,
                                                 const std::string& n) {
    const auto vn = decomposeVariableName(n);
    const auto n1 = f + "_" + vn + "_LowerPhysicalBound";
    const auto n2 = f + "_" + vn + "_UpperPhysicalBound";
    return ((this->contains(l, n1)) || (this->contains(l, n2)));
  }  // end of hasPhysicalBounds

  bool ExternalLibraryManager::hasLowerPhysicalBound(const std::string& l,
                                                     const std::string& f,
                                                     const std::string& n) {
    const auto vn = decomposeVariableName(n);
    const auto n1 = f + "_" + vn + "_LowerPhysicalBound";
    return ((this->contains(l, n1)));
  }  // end of hasLowerPhysicalBound

  bool ExternalLibraryManager::hasUpperPhysicalBound(const std::string& l,
                                                     const std::string& f,
                                                     const std::string& n) {
    const auto vn = decomposeVariableName(n);
    const auto n1 = f + "_" + vn + "_UpperPhysicalBound";
    return ((this->contains(l, n1)));
  }  // end of hasUpperPhysicalBound

  long double ExternalLibraryManager::getLowerPhysicalBound(
      const std::string& l, const std::string& f, const std::string& n) {
    const auto lib = this->loadLibrary(l);
    const auto vn = decomposeVariableName(n);
    const auto n1 = f + "_" + vn + "_LowerPhysicalBound";
    raise_if(!this->contains(l, n1),
             "ExternalLibraryManager::getLowerPhysicalBound: "
             "no physical lower bound associated to variable '" +
                 vn + "'");
    return tfel_getLongDouble(lib, n1.c_str());
  }  // end of getLowerPhysicalBound

  long double ExternalLibraryManager::getUpperPhysicalBound(
      const std::string& l, const std::string& f, const std::string& n) {
    const auto lib = this->loadLibrary(l);
    const auto vn = decomposeVariableName(n);
    const auto n1 = f + "_" + vn + "_UpperPhysicalBound";
    raise_if(!this->contains(l, n1),
             "ExternalLibraryManager::getUpperPhysicalBound: "
             "no physical upper bound associated to variable '" +
                 vn + "'");
    return tfel_getLongDouble(lib, n1.c_str());
  }  // end of getUpperPhysicalBound

  unsigned short ExternalLibraryManager::getCastemFunctionNumberOfVariables(
      const std::string& l, const std::string& f) {
    return this->getMaterialPropertyNumberOfVariables(l, f);
  }  // end of getCastemFunctionNumberOfVariables

  bool ExternalLibraryManager::getUMATRequiresStiffnessTensor(
      const std::string& l, const std::string& f, const std::string& h) {
    ExternalLibraryManagerCheckModellingHypothesisName(h);
    int res;
    const auto lib = this->loadLibrary(l);
    res = ::tfel_getUMATRequiresStiffnessTensor(lib, (f + "_" + h).c_str());
    if (res < 0) {
      res = ::tfel_getUMATRequiresStiffnessTensor(lib, f.c_str());
    }
    raise_if(res < 0,
             "ExternalLibraryManager::getUMATRequiresStiffnessTensor: "
             "information could not be read (" +
                 getErrorMessage() + ")");
    if (res == 1) {
      return true;
    }
    raise_if(res != 0,
             "ExternalLibraryManager::getUMATRequiresStiffnessTensor: "
             "invalid returned value");
    return false;
  }  // end of getUMATRequiresStiffnessTensor

  bool ExternalLibraryManager::getUMATRequiresThermalExpansionCoefficientTensor(
      const std::string& l, const std::string& f, const std::string& h) {
    ExternalLibraryManagerCheckModellingHypothesisName(h);
    auto lib = this->loadLibrary(l);
    auto res = ::tfel_getUMATRequiresThermalExpansionCoefficientTensor(
        lib, (f + "_" + h).c_str());
    if (res < 0) {
      res = ::tfel_getUMATRequiresThermalExpansionCoefficientTensor(lib,
                                                                    f.c_str());
    }
    raise_if(res < 0,
             "ExternalLibraryManager::"
             "getUMATRequiresThermalExpansionCoefficientTensor: "
             "information could not be read (" +
                 getErrorMessage() + ")");
    if (res == 1) {
      return true;
    }
    raise_if(res != 0,
             "ExternalLibraryManager::"
             "getUMATRequiresThermalExpansionCoefficientTensor: "
             "invalid returned value");
    return false;
  }  // end of
     // ExternalLibraryManager::getUMATRequiresThermalExpansionCoefficientTensor

  bool ExternalLibraryManager::checkIfAsterBehaviourSavesTangentOperator(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto res =
        ::tfel_checkIfAsterBehaviourSavesTangentOperator(lib, f.c_str());
    raise_if(res < 0,
             "ExternalLibraryManager::"
             "checkIfAsterBehaviourSaveTangentOperator: "
             "information could not be read (" +
                 getErrorMessage() + ")");
    if (res == 1) {
      return true;
    }
    raise_if(res != 0,
             "ExternalLibraryManager::"
             "checkIfAsterBehaviourSaveTangentOperator: "
             "invalid returned value");
    return false;
  }  // end of
     // ExternalLibraryManager::checkIfAsterBehaviourSaveTangentOperator

  unsigned short ExternalLibraryManager::getAsterFiniteStrainFormulation(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto s = f + "_FiniteStrainFormulation";
    const auto res = ::tfel_getUnsignedShort(lib, s.c_str());
    raise_if(res < 0,
             "ExternalLibraryManager::"
             "getAsterFiniteStrainFormulation: "
             "information could not be read (" +
                 getErrorMessage() + ")");
    raise_if((res != 1) && (res != 2),
             "ExternalLibraryManager::"
             "getAsterFiniteStrainFormulation: "
             "invalid returned value");
    return static_cast<unsigned short>(res);
  }  // end of getAsterFiniteStrainFormulation

  AsterIntegrationErrorMessageFctPtr
  ExternalLibraryManager::getAsterIntegrationErrorMessageFunction(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto s = f + "_getIntegrationErrorMessage";
    return ::tfel_getAsterIntegrationErrorMessage(lib, s.c_str());
  }  // end of getAsterIntegrationErrorMessage

  unsigned short ExternalLibraryManager::getAbaqusOrthotropyManagementPolicy(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto s = f + "_OrthotropyManagementPolicy";
    const auto res = ::tfel_getUnsignedShort(lib, s.c_str());
    raise_if(res < 0,
             "ExternalLibraryManager::"
             "getAbaqusOrthotropyManagementPolicy: "
             "information could not be read (" +
                 getErrorMessage() + ")");
    raise_if((res != 0) && (res != 1) && (res != 2),
             "ExternalLibraryManager::"
             "getAbaqusOrthotropyManagementPolicy: "
             "invalid returned value");
    return static_cast<unsigned short>(res);
  }  // end of getAbaqusOrthotropyManagementPolicy

  std::vector<std::string> ExternalLibraryManager::getCastemFunctionVariables(
      const std::string& l, const std::string& f) {
    return this->getMaterialPropertyVariables(l, f);
  }  // end of getCastemFunctionVariables

  void ExternalLibraryManager::getCastemFunctionVariables(
      std::vector<std::string>& vars,
      const std::string& l,
      const std::string& f) {
    this->getMaterialPropertyVariables(vars, l, f);
  }  // end of getCastemFunctionVariables

  unsigned short
  ExternalLibraryManager::getGenericMaterialPropertyNumberOfVariables(
      const std::string& l, const std::string& f) {
    return this->getMaterialPropertyNumberOfVariables(l, f);
  }

  std::vector<std::string>
  ExternalLibraryManager::getGenericMaterialPropertyVariables(
      const std::string& l, const std::string& f) {
    return this->getMaterialPropertyVariables(l, f);
  }  // end of getGenericMaterialPropertyVariables

  void ExternalLibraryManager::getGenericMaterialPropertyVariables(
      std::vector<std::string>& vars,
      const std::string& l,
      const std::string& f) {
    this->getMaterialPropertyVariables(vars, l, f);
  }  // end of getGenericMaterialPropertyVariables

  GenericMaterialPropertyPtr ExternalLibraryManager::getGenericMaterialProperty(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getGenericMaterialProperty(lib, f.c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::getGenericMaterialProperty: "
             "could not load generic material property '" +
                 f + "' (" + getErrorMessage() + ")");
    return fct;
  }

  GenericBehaviourFctPtr ExternalLibraryManager::getGenericBehaviourFunction(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getGenericBehaviourFunction(lib, f.c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::getGenericBehaviourFunction: "
             "could not load generic behaviour function '" +
                 f + "' (" + getErrorMessage() + ")");
    return fct;
  }

  std::vector<std::string>
  ExternalLibraryManager::getGenericBehaviourInitializeFunctions(
      const std::string& l, const std::string& f, const std::string& h) {
    auto pfcts = std::vector<std::string>{};
    this->getUMATNames(pfcts, l, f, h, "InitializeFunctions");
    return pfcts;
  }  // end of getGenericBehaviourInitializeFunctions

  std::vector<std::string>
  ExternalLibraryManager::getGenericBehaviourInitializeFunctionInputs(
      const std::string& l,
      const std::string& f,
      const std::string& h,
      const std::string& p) {
    auto outputs = std::vector<std::string>{};
    this->getUMATNames(outputs, l, f, h, "InitializeFunction_" + p + "_Inputs");
    return outputs;
  }  // end of getGenericBehaviourInitializeFunctionInputs

  std::vector<int>
  ExternalLibraryManager::getGenericBehaviourInitializeFunctionInputsTypes(
      const std::string& l,
      const std::string& f,
      const std::string& h,
      const std::string& p) {
    auto otypes = std::vector<int>{};
    this->getUMATTypes(otypes, l, f, h, "InitializeFunction_" + p + "_Inputs");
    return otypes;
  }  // end of getGenericBehaviourInitializeFunctionInputsTypes

  GenericBehaviourInitializeFunctionPtr
  ExternalLibraryManager::getGenericBehaviourInitializeFunction(
      const std::string& l,
      const std::string& f,
      const std::string& h,
      const std::string& i) {
    const auto lib = this->loadLibrary(l);
    if (!h.empty()) {
      const auto fct = ::tfel_getGenericBehaviourInitializeFunction(
          lib, (f + "_" + h + "_InitializeFunction_" + i).c_str());
      if (fct != nullptr) {
        return fct;
      }
    }
    const auto fct = ::tfel_getGenericBehaviourInitializeFunction(
        lib, (f + "_InitializeFunction_" + i).c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::getGenericBehaviourInitializeFunction: "
             "could not load post-processing function '" +
                 i + "' for generic behaviour function '" + f + "' (" +
                 getErrorMessage() + ")");
    return fct;
  }  // end of getGenericBehaviourInitializeFunction

  std::vector<std::string>
  ExternalLibraryManager::getGenericBehaviourPostProcessingFunctions(
      const std::string& l, const std::string& f, const std::string& h) {
    auto pfcts = std::vector<std::string>{};
    this->getUMATNames(pfcts, l, f, h, "PostProcessings");
    return pfcts;
  }  // end of getGenericBehaviourPostProcessingFunctions

  std::vector<std::string>
  ExternalLibraryManager::getGenericBehaviourPostProcessingFunctionOutputs(
      const std::string& l,
      const std::string& f,
      const std::string& h,
      const std::string& p) {
    auto outputs = std::vector<std::string>{};
    this->getUMATNames(outputs, l, f, h, "PostProcessing_" + p + "_Outputs");
    return outputs;
  }  // end of getGenericBehaviourPostProcessingFunctionOutputs

  std::vector<int>
  ExternalLibraryManager::getGenericBehaviourPostProcessingFunctionOutputsTypes(
      const std::string& l,
      const std::string& f,
      const std::string& h,
      const std::string& p) {
    auto otypes = std::vector<int>{};
    this->getUMATTypes(otypes, l, f, h, "PostProcessing_" + p + "_Outputs");
    return otypes;
  }  // end of getGenericBehaviourPostProcessingFunctionOutputsTypes

  GenericBehaviourPostProcessingFctPtr
  ExternalLibraryManager::getGenericBehaviourPostProcessingFunction(
      const std::string& l,
      const std::string& f,
      const std::string& h,
      const std::string& i) {
    const auto lib = this->loadLibrary(l);
    if (!h.empty()) {
      const auto fct = ::tfel_getGenericBehaviourPostProcessingFunction(
          lib, (f + "_" + h + "_PostProcessing_" + i).c_str());
      if (fct != nullptr) {
        return fct;
      }
    }
    const auto fct = ::tfel_getGenericBehaviourPostProcessingFunction(
        lib, (f + "_PostProcessing_" + i).c_str());
    raise_if(
        fct == nullptr,
        "ExternalLibraryManager::getGenericBehaviourPostProcessingFunction: "
        "could not load post-processing function '" +
            i + "' for generic behaviour function '" + f + "' (" +
            getErrorMessage() + ")");
    return fct;
  }  // end of getGenericBehaviourPostProcessingFunction

  GenericBehaviourRotateGradientsFctPtr
  ExternalLibraryManager::getGenericBehaviourRotateGradientsFunction(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct =
        ::tfel_getGenericBehaviourRotateGradientsFunction(lib, f.c_str());
    raise_if(
        fct == nullptr,
        "ExternalLibraryManager::getGenericBehaviourRotateGradientsFunction: "
        "could not load generic gehaviour function '" +
            f + "' (" + getErrorMessage() + ")");
    return fct;
  }  // end of
     // ExternalLibraryManager::getGenericBehaviourRotateGradientsFunction

  GenericBehaviourRotateArrayOfGradientsFctPtr
  ExternalLibraryManager::getGenericBehaviourRotateArrayOfGradientsFunction(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getGenericBehaviourRotateArrayOfGradientsFunction(
        lib, f.c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::"
             "getGenericBehaviourRotateArrayOfGradientsFunction: "
             "could not load generic gehaviour function '" +
                 f + "' (" + getErrorMessage() + ")");
    return fct;
  }  // end of
  // ExternalLibraryManager::getGenericBehaviourRotateArrayOfGradientsFunction

  GenericBehaviourRotateThermodynamicForcesFctPtr
  ExternalLibraryManager::getGenericBehaviourRotateThermodynamicForcesFunction(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct =
        ::tfel_getGenericBehaviourRotateThermodynamicForcesFunction(lib,
                                                                    f.c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::"
             "getGenericBehaviourRotateThermodynamicForcesFunction: "
             "could not load generic gehaviour function '" +
                 f + "' (" + getErrorMessage() + ")");
    return fct;
  }  // end of
     // ExternalLibraryManager::getGenericBehaviourRotateThermodynamicForcesFunction

  GenericBehaviourRotateArrayOfThermodynamicForcesFctPtr
  ExternalLibraryManager::
      getGenericBehaviourRotateArrayOfThermodynamicForcesFunction(
          const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct =
        ::tfel_getGenericBehaviourRotateArrayOfThermodynamicForcesFunction(
            lib, f.c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::"
             "getGenericBehaviourRotateArrayOfThermodynamicForcesFunction: "
             "could not load generic gehaviour function '" +
                 f + "' (" + getErrorMessage() + ")");
    return fct;
  }  // end of
     // ExternalLibraryManager::getGenericBehaviourRotateArrayOfThermodynamicForcesFunction

  GenericBehaviourRotateTangentOperatorBlocksFctPtr ExternalLibraryManager::
      getGenericBehaviourRotateTangentOperatorBlocksFunction(
          const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct =
        ::tfel_getGenericBehaviourRotateTangentOperatorBlocksFunction(
            lib, f.c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::"
             "getGenericBehaviourRotateTangentOperatorBlocksFunction: "
             "could not load generic gehaviour function '" +
                 f + "' (" + getErrorMessage() + ")");
    return fct;
  }  // end of
     // ExternalLibraryManager::getGenericBehaviourRotateTangentOperatorBlocksFunction

  GenericBehaviourRotateArrayOfTangentOperatorBlocksFctPtr
  ExternalLibraryManager::
      getGenericBehaviourRotateArrayOfTangentOperatorBlocksFunction(
          const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct =
        ::tfel_getGenericBehaviourRotateArrayOfTangentOperatorBlocksFunction(
            lib, f.c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::"
             "getGenericBehaviourRotateArrayOfTangentOperatorBlocksFunction: "
             "could not load generic gehaviour function '" +
                 f + "' (" + getErrorMessage() + ")");
    return fct;
  }  // end of
     // ExternalLibraryManager::getGenericBehaviourRotateArrayOfTangentOperatorBlocksFunction

  bool ExternalLibraryManager::isUMATBehaviourAbleToComputeInternalEnergy(
      const std::string& l, const std::string& f, const std::string& h) {
    ExternalLibraryManagerCheckModellingHypothesisName(h);
    const auto lib = this->loadLibrary(l);
    auto b =
        ::tfel_getBool(lib, (f + "_" + h + "_ComputesInternalEnergy").c_str());
    if (b == -1) {
      b = ::tfel_getBool(lib, (f + "_ComputesInternalEnergy").c_str());
    }
    if (b == -1) {
      return false;
    }
    return (b == 1);
  }  // end of isUMATBehaviourAbleToComputeInternalEnergy

  bool ExternalLibraryManager::isUMATBehaviourAbleToComputeDissipatedEnergy(
      const std::string& l, const std::string& f, const std::string& h) {
    ExternalLibraryManagerCheckModellingHypothesisName(h);
    const auto lib = this->loadLibrary(l);
    auto b = ::tfel_getBool(
        lib, (f + "_" + h + "_ComputesDissipatedEnergy").c_str());
    if (b == -1) {
      b = ::tfel_getBool(lib, (f + "_ComputesDissipatedEnergy").c_str());
    }
    if (b == -1) {
      return false;
    }
    return (b == 1);
  }  // end of isUMATBehaviourAbleToComputeDissipatedEnergy

  unsigned short ExternalLibraryManager::getMaterialPropertyNumberOfVariables(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto res =
        ::tfel_getMaterialPropertyNumberOfVariables(lib, f.c_str());
    raise_if(res < 0,
             "ExternalLibraryManager::getMaterialPropertyNumberOfVariables: "
             "number of variables could not be read (" +
                 getErrorMessage() + ")");
    return static_cast<unsigned short>(res);
  }

  std::string ExternalLibraryManager::getMaterialPropertyOutput(
      const std::string& l, const std::string& f) {
    return this->getString(l, f + "_output");
  }  // end of getMaterialPropertyOutput

  std::vector<std::string> ExternalLibraryManager::getMaterialPropertyVariables(
      const std::string& l, const std::string& f) {
    std::vector<std::string> vars;
    this->getMaterialPropertyVariables(vars, l, f);
    return vars;
  }  // end of getMaterialPropertyVariables

  void ExternalLibraryManager::getMaterialPropertyVariables(
      std::vector<std::string>& arguments,
      const std::string& l,
      const std::string& f) {
    if (this->getMaterialPropertyNumberOfVariables(l, f) != 0) {
      arguments = this->getArrayOfStrings(l, f, "args");
    } else {
      arguments.clear();
    }
  }  // end of getMaterialPropertyVariables

  std::vector<std::string>
  ExternalLibraryManager::getMaterialPropertyParameters(const std::string& l,
                                                        const std::string& f) {
    std::vector<std::string> parameters;
    this->getMaterialPropertyParameters(parameters, l, f);
    return parameters;
  }  // end of getMaterialPropertyParameters

  void ExternalLibraryManager::getMaterialPropertyParameters(
      std::vector<std::string>& parameters,
      const std::string& l,
      const std::string& f) {
    parameters = this->getArrayOfStrings(l, f, "Parameters");
  }  // end of getMaterialPropertyParameters

  unsigned short
  ExternalLibraryManager::getCyranoMaterialPropertyNumberOfVariables(
      const std::string& l, const std::string& f) {
    return this->getMaterialPropertyNumberOfVariables(l, f);
  }

  std::vector<std::string>
  ExternalLibraryManager::getCyranoMaterialPropertyVariables(
      const std::string& l, const std::string& f) {
    return this->getMaterialPropertyVariables(l, f);
  }  // end of getCyranoMaterialPropertyVariables

  void ExternalLibraryManager::getCyranoMaterialPropertyVariables(
      std::vector<std::string>& vars,
      const std::string& l,
      const std::string& f) {
    this->getMaterialPropertyVariables(vars, l, f);
  }  // end of getCyranoMaterialPropertyVariables

  CyranoMaterialPropertyPtr ExternalLibraryManager::getCyranoMaterialProperty(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getCyranoMaterialProperty(lib, f.c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::getCyranoMaterialProperty: "
             "could not load Cyrano function '" +
                 f + "' (" + getErrorMessage() + ")");
    return fct;
  }

  CyranoBehaviourPtr ExternalLibraryManager::getCyranoFunction(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getCyranoFunction(lib, f.c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::getCyranoFunction: "
             "could not load Cyrano function '" +
                 f +
                 "' "
                 "(" +
                 getErrorMessage() + ")");
    return fct;
  }

  AbaqusFctPtr ExternalLibraryManager::getAbaqusExternalBehaviourFunction(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getAbaqusExternalBehaviourFunction(lib, f.c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::getAbaqusExternalBehaviourFunction: "
             "could not load Abaqus external behaviour '" +
                 f +
                 "' "
                 "(" +
                 getErrorMessage() + ")");
    return fct;
  }

  AnsysFctPtr ExternalLibraryManager::getAnsysExternalBehaviourFunction(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getAnsysExternalBehaviourFunction(lib, f.c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::getAnsysExternalBehaviourFunction: "
             "could not load Ansys external behaviour '" +
                 f +
                 "' "
                 "(" +
                 getErrorMessage() + ")");
    return fct;
  }

  AbaqusExplicitFctPtr
  ExternalLibraryManager::getAbaqusExplicitExternalBehaviourFunction(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct =
        ::tfel_getAbaqusExplicitExternalBehaviourFunction(lib, f.c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::"
             "getAbaqusExplicitExternalBehaviourFunction: "
             "could not load AbaqusExplicit external "
             "behaviour '" +
                 f + "' (" + getErrorMessage() + ")");
    return fct;
  }

  CalculiXFctPtr ExternalLibraryManager::getCalculiXExternalBehaviourFunction(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    auto fct = ::tfel_getCalculiXExternalBehaviourFunction(lib, f.c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::"
             "getCalculiXExternalBehaviourFunction: "
             "could not load CalculiX external behaviour '" +
                 f +
                 "' "
                 "(" +
                 getErrorMessage() + ")");
    return fct;
  }

  DianaFEAFctPtr ExternalLibraryManager::getDianaFEAExternalBehaviourFunction(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    auto fct = ::tfel_getDianaFEAExternalBehaviourFunction(lib, f.c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::"
             "getDianaFEAExternalBehaviourFunction: "
             "could not load Diana FEA external behaviour '" +
                 f +
                 "' "
                 "(" +
                 getErrorMessage() + ")");
    return fct;
  }  // end of getDianaFEAExternalBehaviourFunction

  CastemFctPtr ExternalLibraryManager::getCastemExternalBehaviourFunction(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getCastemExternalBehaviourFunction(lib, f.c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::"
             "getCastemExternalBehaviourFunction: "
             "could not load castem external behaviour '" +
                 f +
                 "' "
                 "(" +
                 getErrorMessage() + ")");
    return fct;
  }

  AsterFctPtr ExternalLibraryManager::getAsterFunction(const std::string& l,
                                                       const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getAsterFunction(lib, f.c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::getAsterFunction: "
             "could not load Aster function '" +
                 f +
                 "' "
                 "(" +
                 getErrorMessage() + ")");
    return fct;
  }

  EuroplexusFctPtr ExternalLibraryManager::getEuroplexusFunction(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getEuroplexusFunction(lib, f.c_str());
    raise_if(fct == nullptr,
             "ExternalLibraryManager::getEuroplexusFunction: "
             " could not load Europlexus function '" +
                 f +
                 "' "
                 "(" +
                 getErrorMessage() + ")");
    return fct;
  }

  std::vector<std::string> ExternalLibraryManager::getArrayOfStrings(
      const std::string& l, const std::string& e, const std::string& n) {
    const auto lib = this->loadLibrary(l);
    auto nb = -1;
    nb = ::tfel_getUnsignedShort(lib, (e + "_n" + n).c_str());
    raise_if(nb == -1,
             "ExternalLibraryManager::getArrayOfStrings: "
             "number of variables names could not be read "
             "(" +
                 getErrorMessage() + ")");
    const auto res = ::tfel_getArrayOfStrings(lib, (e + '_' + n).c_str());
    raise_if(res == nullptr,
             "ExternalLibraryManager::getArrayOfStrings: "
             "variables names could not be read "
             "(" +
                 getErrorMessage() + ")");
    return std::vector<std::string>(res, res + nb);
  }  // end of getArrayOfStrings

  void ExternalLibraryManager::getUMATNames(std::vector<std::string>& vars,
                                            const std::string& l,
                                            const std::string& f,
                                            const std::string& h,
                                            const std::string& n) {
    if (!h.empty()) {
      ExternalLibraryManagerCheckModellingHypothesisName(h);
    }
    const auto lib = this->loadLibrary(l);
    auto nb = -1;
    if (!h.empty()) {
      nb = ::tfel_getUnsignedShort(lib, (f + "_" + h + "_n" + n).c_str());
    }
    if (nb == -1) {
      nb = ::tfel_getUnsignedShort(lib, (f + "_n" + n).c_str());
    }
    raise_if(nb == -1,
             "ExternalLibraryManager::getUMATNames: "
             "number of variables names could not be read "
             "(" +
                 getErrorMessage() + ")");
    char** res = nullptr;
    if (!h.empty()) {
      res = ::tfel_getArrayOfStrings(lib, (f + "_" + h + '_' + n).c_str());
    }
    if (res == nullptr) {
      res = ::tfel_getArrayOfStrings(lib, (f + '_' + n).c_str());
    }
    raise_if(res == nullptr,
             "ExternalLibraryManager::getUMATNames: "
             "variables names could not be read "
             "(" +
                 getErrorMessage() + ")");
    std::copy(res, res + nb, std::back_inserter(vars));
  }  // end of getUMATNames

  void ExternalLibraryManager::getUMATTypes(std::vector<int>& types,
                                            const std::string& l,
                                            const std::string& f,
                                            const std::string& h,
                                            const std::string& n) {
    if (!h.empty()) {
      ExternalLibraryManagerCheckModellingHypothesisName(h);
    }
    const auto lib = this->loadLibrary(l);
    auto nb = -1;
    if (!h.empty()) {
      nb = ::tfel_getUnsignedShort(lib, (f + "_" + h + "_n" + n).c_str());
    }
    if (nb == -1) {
      nb = ::tfel_getUnsignedShort(lib, (f + "_n" + n).c_str());
    }
    raise_if(nb == -1,
             "ExternalLibraryManager::getUMATTypes: "
             "number of variables names could not be read "
             "(" +
                 getErrorMessage() + ")");
    int* res = nullptr;
    if (!h.empty()) {
      res =
          ::tfel_getArrayOfInts(lib, (f + "_" + h + '_' + n + "Types").c_str());
    }
    if (res == nullptr) {
      res = ::tfel_getArrayOfInts(lib, (f + '_' + n + "Types").c_str());
    }
    raise_if(res == nullptr,
             "ExternalLibraryManager::getUMATTypes: "
             "variables names could not be read "
             "(" +
                 getErrorMessage() + ")");
    std::copy(res, res + nb, std::back_inserter(types));
  }  // end of getUMATTypes

  bool ExternalLibraryManager::isUMATBehaviourUsableInPurelyImplicitResolution(
      const std::string& l, const std::string& f, const std::string& h) {
    ExternalLibraryManagerCheckModellingHypothesisName(h);
    const auto lib = this->loadLibrary(l);
    int b = ::tfel_getBool(
        lib, (f + "_" + h + "_UsableInPurelyImplicitResolution").c_str());
    if (b == -1) {
      b = ::tfel_getBool(lib,
                         (f + "_UsableInPurelyImplicitResolution").c_str());
    }
    if (b == -1) {
      return false;
    }
    return (b == 1);
  }  // end of
     // ExternalLibraryManager::isUMATBehaviourUsableInPurelyImplicitResolution

  bool
  ExternalLibraryManager::checkIfUMATBehaviourUsesGenericPlaneStressAlgorithm(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto b =
        ::tfel_getBool(lib, (f + "_UsesGenericPlaneStressAlgorithm").c_str());
    if (b == -1) {
      return false;
    }
    return b == 1;
  }  // end of
     // ExternalLibraryManager::checkIfUMATBehaviourUsesGenericPlaneStressAlgorithm

  unsigned short ExternalLibraryManager::getUMATBehaviourType(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto u = ::tfel_getUnsignedShort(lib, (f + "_BehaviourType").c_str());
    raise_if(u == -1,
             "ExternalLibraryManager::getUMATBehaviourType: "
             "behaviour type could not be read (" +
                 getErrorMessage() + ")");
    return static_cast<unsigned short>(u);
  }  // end of getUMATBehaviourType

  unsigned short ExternalLibraryManager::getUMATBehaviourKinematic(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto u =
        ::tfel_getUnsignedShort(lib, (f + "_BehaviourKinematic").c_str());
    raise_if(u == -1,
             "ExternalLibraryManager::getUMATBehaviourKinematic: "
             "behaviour type could not be read (" +
                 getErrorMessage() + ")");
    return static_cast<unsigned short>(u);
  }  // end of getUMATBehaviourKinematic

  unsigned short ExternalLibraryManager::getUMATSymmetryType(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto u = ::tfel_getUnsignedShort(lib, (f + "_SymmetryType").c_str());
    raise_if(u == -1,
             "ExternalLibraryManager::getUMATSymmetryType: "
             "symmetry type could not be read (" +
                 getErrorMessage() + ")");
    return static_cast<unsigned short>(u);
  }  // end of getUMATSymmetryType

  unsigned short ExternalLibraryManager::getUMATElasticSymmetryType(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto u =
        ::tfel_getUnsignedShort(lib, (f + "_ElasticSymmetryType").c_str());
    raise_if(u == -1,
             "ExternalLibraryManager::getUMATElasticSymmetryType: "
             "elastic symmetry type could not be read "
             "(" +
                 getErrorMessage() + ")");
    return static_cast<unsigned short>(u);
  }  // end of getUMATElasticSymmetryType

  std::vector<std::string>
  ExternalLibraryManager::getUMATElasticMaterialPropertiesEntryPoints(
      const std::string& l, const std::string& f) {
    if (!this->contains(l, f + "_ElasticMaterialPropertiesEntryPoints")) {
      return {};
    }
    std::vector<std::string> names;
    this->getUMATNames(names, l, f, "", "ElasticMaterialPropertiesEntryPoints");
    return names;
  }  // end of getUMATElasticMaterialPropertiesEntryPoints

  std::vector<std::string>
  ExternalLibraryManager::getUMATLinearThermalExpansionCoefficientsEntryPoints(
      const std::string& l, const std::string& f) {
    if (!this->contains(l,
                        f + "_LinearThermalExpansionCoefficientsEntryPoints")) {
      return {};
    }
    std::vector<std::string> names;
    this->getUMATNames(names, l, f, "",
                       "LinearThermalExpansionCoefficientsEntryPoints");
    return names;
  }  // end of getUMATLinearThermalExpansionCoefficientsEntryPoints

  std::vector<std::string>
  ExternalLibraryManager::getUMATMaterialPropertiesNames(const std::string& l,
                                                         const std::string& f,
                                                         const std::string& h) {
    std::vector<std::string> vars;
    this->getUMATNames(vars, l, f, h, "MaterialProperties");
    return vars;
  }  // end of getUMATMaterialPropertiesNames

  std::vector<std::string> ExternalLibraryManager::getUMATGradientsNames(
      const std::string& l, const std::string& f) {
    std::vector<std::string> names;
    this->getUMATNames(names, l, f, "", "Gradients");
    return names;
  }  // end of getUMATGradientsNames

  std::vector<std::string> ExternalLibraryManager::getUMATDrivingVariablesNames(
      const std::string& l, const std::string& f) {
    return this->getUMATGradientsNames(l, f);
  }  // end of getUMATDrivingVariablesNames

  std::vector<int> ExternalLibraryManager::getUMATGradientsTypes(
      const std::string& l, const std::string& f) {
    std::vector<int> types;
    this->getUMATTypes(types, l, f, "", "Gradients");
    return types;
  }  // end of getUMATGradientsTypes

  std::vector<int> ExternalLibraryManager::getUMATDrivingVariablesTypes(
      const std::string& l, const std::string& f) {
    return this->getUMATGradientsTypes(l, f);
  }  // end of getUMATDrivingVariablesTypes

  std::vector<std::string>
  ExternalLibraryManager::getUMATThermodynamicForcesNames(
      const std::string& l, const std::string& f) {
    std::vector<std::string> names;
    this->getUMATNames(names, l, f, "", "ThermodynamicForces");
    return names;
  }  // end of getUMATThermodynamicForcesNames

  std::vector<int> ExternalLibraryManager::getUMATThermodynamicForcesTypes(
      const std::string& l, const std::string& f) {
    std::vector<int> types;
    this->getUMATTypes(types, l, f, "", "ThermodynamicForces");
    return types;
  }  // end of getUMATThermodynamicForcesTypes

  std::vector<std::pair<std::string, std::string>>
  ExternalLibraryManager::getUMATTangentOperatorBlocksNames(
      const std::string& l, const std::string& f) {
    std::vector<std::pair<std::string, std::string>> blocks;
    std::vector<std::string> names;
    this->getUMATNames(names, l, f, "", "TangentOperatorBlocks");
    const auto s = names.size();
    tfel::raise_if(
        s % 2 != 0,
        "ExternalLibraryManager::getUMATTangentOperatorBlocksNames: "
        "invalid declaration of the tangent operator blocks is invalid");
    for (decltype(names.size()) i = 0; i != s / 2; ++i) {
      blocks.push_back({names[2 * i], names[2 * i + 1]});
    }
    return blocks;
  }  // end of getUMATTangentOperatorBlocksNames

  std::vector<std::string>
  ExternalLibraryManager::getUMATInternalStateVariablesNames(
      const std::string& l, const std::string& f, const std::string& h) {
    std::vector<std::string> vars;
    this->getUMATNames(vars, l, f, h, "InternalStateVariables");
    return vars;
  }  // end of getUMATInternalStateVariablesNames

  std::vector<int> ExternalLibraryManager::getUMATInternalStateVariablesTypes(
      const std::string& l, const std::string& f, const std::string& h) {
    std::vector<int> types;
    this->getUMATTypes(types, l, f, h, "InternalStateVariables");
    return types;
  }  // end of getUMATInternalVariablesTypes

  bool
  ExternalLibraryManager::hasTemperatureBeenRemovedFromExternalStateVariables(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto s = f + "_TemperatureRemovedFromExternalStateVariables";
    const auto u = ::tfel_getUnsignedShort(lib, s.c_str());
    if (u == -1) {
      tfel::raise(
          "ExternalLibraryManager::"
          "hasTemperatureBeenRemovedFromExternalStateVariables: "
          "undefined symbol '" +
          s + "'");
    }
    return u == 1;
  }  // end of hasTemperatureBeenRemovedFromExternalStateVariables

  std::vector<std::string>
  ExternalLibraryManager::getUMATExternalStateVariablesNames(
      const std::string& l, const std::string& f, const std::string& h) {
    std::vector<std::string> vars;
    this->getUMATNames(vars, l, f, h, "ExternalStateVariables");
    return vars;
  }  // end of getUMATMaterialPropertiesNames

  std::vector<int> ExternalLibraryManager::getUMATExternalStateVariablesTypes(
      const std::string& l, const std::string& f, const std::string& h) {
    std::vector<int> types;
    this->getUMATTypes(types, l, f, h, "ExternalStateVariables");
    return types;
  }  // end of getUMATExternalVariablesTypes

  std::vector<std::string> ExternalLibraryManager::getUMATParametersNames(
      const std::string& l, const std::string& f, const std::string& h) {
    std::vector<std::string> names;
    this->getUMATNames(names, l, f, h, "Parameters");
    return names;
  }  // end of getUMATMaterialPropertiesNames

  std::vector<int> ExternalLibraryManager::getUMATParametersTypes(
      const std::string& l, const std::string& f, const std::string& h) {
    std::vector<int> types;
    this->getUMATTypes(types, l, f, h, "Parameters");
    return types;
  }  // end of getUMATInternalVariablesTypes

  CastemFunctionPtr ExternalLibraryManager::getCastemFunction(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    auto fct = ::tfel_getCastemFunction(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getCastemFunction: "
                   "could not load castem function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  CFunction0Ptr ExternalLibraryManager::getCFunction0(const std::string& l,
                                                      const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getCFunction0(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getCFunction0: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  CFunction1Ptr ExternalLibraryManager::getCFunction1(const std::string& l,
                                                      const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getCFunction1(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getCFunction1: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  CFunction2Ptr ExternalLibraryManager::getCFunction2(const std::string& l,
                                                      const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getCFunction2(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getCFunction2: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  CFunction3Ptr ExternalLibraryManager::getCFunction3(const std::string& l,
                                                      const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getCFunction3(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getCFunction3: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  CFunction4Ptr ExternalLibraryManager::getCFunction4(const std::string& l,
                                                      const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getCFunction4(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getCFunction4: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  CFunction5Ptr ExternalLibraryManager::getCFunction5(const std::string& l,
                                                      const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getCFunction5(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getCFunction5: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  CFunction6Ptr ExternalLibraryManager::getCFunction6(const std::string& l,
                                                      const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getCFunction6(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getCFunction6: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  CFunction7Ptr ExternalLibraryManager::getCFunction7(const std::string& l,
                                                      const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getCFunction7(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getCFunction7: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  CFunction8Ptr ExternalLibraryManager::getCFunction8(const std::string& l,
                                                      const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getCFunction8(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getCFunction8: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  CFunction9Ptr ExternalLibraryManager::getCFunction9(const std::string& l,
                                                      const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getCFunction9(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getCFunction9: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  CFunction10Ptr ExternalLibraryManager::getCFunction10(const std::string& l,
                                                        const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getCFunction10(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getCFunction10: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  CFunction11Ptr ExternalLibraryManager::getCFunction11(const std::string& l,
                                                        const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getCFunction11(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getCFunction11: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  CFunction12Ptr ExternalLibraryManager::getCFunction12(const std::string& l,
                                                        const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getCFunction12(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getCFunction12: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  CFunction13Ptr ExternalLibraryManager::getCFunction13(const std::string& l,
                                                        const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getCFunction13(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getCFunction13: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  CFunction14Ptr ExternalLibraryManager::getCFunction14(const std::string& l,
                                                        const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getCFunction14(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getCFunction14: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  CFunction15Ptr ExternalLibraryManager::getCFunction15(const std::string& l,
                                                        const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getCFunction15(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getCFunction15: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  FortranFunction0Ptr ExternalLibraryManager::getFortranFunction0(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getFortranFunction0(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getFortranFunction0: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  FortranFunction1Ptr ExternalLibraryManager::getFortranFunction1(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getFortranFunction1(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getFortranFunction1: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  FortranFunction2Ptr ExternalLibraryManager::getFortranFunction2(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getFortranFunction2(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getFortranFunction2: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  FortranFunction3Ptr ExternalLibraryManager::getFortranFunction3(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getFortranFunction3(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getFortranFunction3: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  FortranFunction4Ptr ExternalLibraryManager::getFortranFunction4(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getFortranFunction4(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getFortranFunction4: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  FortranFunction5Ptr ExternalLibraryManager::getFortranFunction5(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getFortranFunction5(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getFortranFunction5: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  FortranFunction6Ptr ExternalLibraryManager::getFortranFunction6(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getFortranFunction6(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getFortranFunction6: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  FortranFunction7Ptr ExternalLibraryManager::getFortranFunction7(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getFortranFunction7(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getFortranFunction7: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  FortranFunction8Ptr ExternalLibraryManager::getFortranFunction8(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getFortranFunction8(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getFortranFunction8: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  FortranFunction9Ptr ExternalLibraryManager::getFortranFunction9(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getFortranFunction9(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getFortranFunction9: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  FortranFunction10Ptr ExternalLibraryManager::getFortranFunction10(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getFortranFunction10(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getFortranFunction10: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  FortranFunction11Ptr ExternalLibraryManager::getFortranFunction11(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getFortranFunction11(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getFortranFunction11: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  FortranFunction12Ptr ExternalLibraryManager::getFortranFunction12(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getFortranFunction12(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getFortranFunction12: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  FortranFunction13Ptr ExternalLibraryManager::getFortranFunction13(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getFortranFunction13(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getFortranFunction13: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  FortranFunction14Ptr ExternalLibraryManager::getFortranFunction14(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getFortranFunction14(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getFortranFunction14: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  FortranFunction15Ptr ExternalLibraryManager::getFortranFunction15(
      const std::string& l, const std::string& f) {
    const auto lib = this->loadLibrary(l);
    const auto fct = ::tfel_getFortranFunction15(lib, f.c_str());
    tfel::raise_if(fct == nullptr,
                   "ExternalLibraryManager::getFortranFunction15: "
                   "could not load function '" +
                       f +
                       "' "
                       "(" +
                       getErrorMessage() + ")");
    return fct;
  }

  ExternalLibraryManager::~ExternalLibraryManager() = default;

}  // end of namespace tfel::system
