/*!
 * \file   src/System/ExternalLibraryManager.cxx
 * \brief
 * \author Thomas Helfer
 * \date   31 Oct 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
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

namespace tfel {

  namespace system {

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
    }  // end of  getErrorMessage

    static void ExternalLibraryManagerCheckModellingHypothesisName(
        const std::string& h) {
      raise_if(!((h == "AxisymmetricalGeneralisedPlaneStrain") ||
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
        raise_if(c, "tfel::system::decomposeVariableName: " + m);
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
      r += "__";
      while ((p != pe) && (std::isdigit(*p))) {
        r.push_back(*p);
        ++p;
      }
      throw_if(p == pe, "unexpected end of string '" + n + "'");
      throw_if(*p != ']', "invalid variable name '" + n + "'");
      ++p;
      throw_if(p != pe, "invalid variable name '" + n + "'");
      r += "__";
      return r;
    }  // end of decomposeVariableName

    ExternalLibraryManager&
    ExternalLibraryManager::getExternalLibraryManager() {
      static ExternalLibraryManager elm;
      return elm;
    }  // end of ExternalLibraryManager::getExternalLibraryManager()

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
    }  // end of load_library

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
      auto throw_if = [](const bool c, const std::string& m) {
        raise_if(c, "ExternalLibraryManager::getLibraryPath: " + m);
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
      throw_if(lib.first == nullptr, "can't load library '" + l + "'");
      // check if file exists
      if (exists(lib.second)) {
        return lib.second;
      }
      // look in LD_LIBRARY_PATH
      const auto ld = std::getenv("LD_LIBRARY_PATH");
      throw_if(ld == nullptr, "can't find library '" + l + "'");
      for (const auto& p : tokenize(ld, ':')) {
        const auto lp = p + '/' + lib.second;
        if (exists(lp)) {
          return lp;
        }
      }
      throw_if(true, "can't find library '" + l + "'");
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
    }  // end of ExternalLibraryManager::getLibraryPath

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
    }  // end of ExternalLibraryManager::loadLibrary

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
    }  // end of ExternalLibraryManager::getEntryPoints

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
    }  // end of ExternalLibraryManager::getMaterialKnowledgeType

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
    }  // end of ExternalLibraryManager::contains

    std::string ExternalLibraryManager::getSource(const std::string& l,
                                                  const std::string& f) {
      auto s = std::string{};
      const auto lib = this->loadLibrary(l);
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      const auto p =
          (const char* const*)::GetProcAddress(lib, (f + "_src").c_str());
#else
      auto p = ::dlsym(lib, (f + "_src").c_str());
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
      if (p != nullptr) {
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
        s = *p;
#else
        s = *(static_cast<const char* const*>(p));
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
      }
      return s;
    }  // end of ExternalLibraryManager::getSource

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
    }  // end of ExternalLibraryManager::getInterface

    std::string ExternalLibraryManager::getMaterial(const std::string& l,
                                                    const std::string& f) {
      const auto lib = this->loadLibrary(l);
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      const auto p = (const char* const*)::GetProcAddress(
          lib, (f + "_mfront_material").c_str());
#else
      const auto p = ::dlsym(lib, (f + "_mfront_material").c_str());
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
      if (p == nullptr) {
        return "";
      }
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      return *p;
#else
      return *(static_cast<const char* const*>(p));
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
    }  // end of ExternalLibraryManager::getMaterial

    std::string ExternalLibraryManager::getTFELVersion(const std::string& l,
                                                       const std::string& f) {
      const auto lib = this->loadLibrary(l);
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      const auto p = (const char* const*)::GetProcAddress(
          lib, (f + "_tfel_version").c_str());
#else
      const auto p = ::dlsym(lib, (f + "_tfel_version").c_str());
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
      if (p == nullptr) {
        return "";
      }
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
      return *p;
#else
      return *(static_cast<const char* const*>(p));
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
    }  // end of ExternalLibraryManager::getTFELVersion

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
    }  // end of ExternalLibraryManager::getSupportedModellingHypotheses

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
    }  // end of ExternalLibraryManager::setParameter

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
    }  // end of ExternalLibraryManager::setParameter

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
    }  // end of ExternalLibraryManager::setParameter

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
    }  // end of ExternalLibraryManager::setParameter

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
        fct =
            ::tfel_getSetParameterFunction(lib, (f + "_setParameter").c_str());
      }
      raise_if(fct == nullptr,
               "ExternalLibraryManager::setParameter: "
               "can't get the '" +
                   f + "_setParameter' function (" + getErrorMessage() + ")");
      raise_if(!fct(p.c_str(), v),
               "ExternalLibraryManager::setParameter: "
               "call to the '" +
                   f + "_setParameter' function failed");
    }  // end of ExternalLibraryManager::setParameter

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
    }  // end of ExternalLibraryManager::setParameter

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
    }  // end of ExternalLibraryManager::setParameter

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
    }  // end of ExternalLibraryManager::getRealParameterDefaultValue

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
    }  // end of ExternalLibraryManager::getIntegerParameterDefaultValue

    unsigned short
    ExternalLibraryManager::getUnsignedShortParameterDefaultValue(
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
    }  // end of ExternalLibraryManager::getUnsignedShortParameterDefaultValue

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
    }  // end of ExternalLibraryManager::hasBounds

    bool ExternalLibraryManager::hasLowerBound(const std::string& l,
                                               const std::string& f,
                                               const std::string& h,
                                               const std::string& n) {
      const auto vn = decomposeVariableName(n);
      const auto n1 = f + "_" + h + "_" + vn + "_LowerBound";
      const auto n2 = f + "_" + vn + "_LowerBound";
      return ((this->contains(l, n1)) || (this->contains(l, n2)));
    }  // end of ExternalLibraryManager::hasLowerBound

    bool ExternalLibraryManager::hasUpperBound(const std::string& l,
                                               const std::string& f,
                                               const std::string& h,
                                               const std::string& n) {
      const auto vn = decomposeVariableName(n);
      const auto n1 = f + "_" + h + "_" + vn + "_UpperBound";
      const auto n2 = f + "_" + vn + "_UpperBound";
      return ((this->contains(l, n1)) || (this->contains(l, n2)));
    }  // end of ExternalLibraryManager::hasUpperBound

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
    }  // end of ExternalLibraryManager::getLowerBound

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
    }  // end of ExternalLibraryManager::getUpperBound

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
    }  // end of ExternalLibraryManager::hasPhysicalBounds

    bool ExternalLibraryManager::hasLowerPhysicalBound(const std::string& l,
                                                       const std::string& f,
                                                       const std::string& h,
                                                       const std::string& n) {
      const auto vn = decomposeVariableName(n);
      const auto n1 = f + "_" + h + "_" + vn + "_LowerPhysicalBound";
      const auto n2 = f + "_" + vn + "_LowerPhysicalBound";
      return ((this->contains(l, n1)) || (this->contains(l, n2)));
    }  // end of ExternalLibraryManager::hasLowerPhysicalBound

    bool ExternalLibraryManager::hasUpperPhysicalBound(const std::string& l,
                                                       const std::string& f,
                                                       const std::string& h,
                                                       const std::string& n) {
      const auto vn = decomposeVariableName(n);
      const auto n1 = f + "_" + h + "_" + vn + "_UpperPhysicalBound";
      const auto n2 = f + "_" + vn + "_UpperPhysicalBound";
      return ((this->contains(l, n1)) || (this->contains(l, n2)));
    }  // end of ExternalLibraryManager::hasUpperPhysicalBound

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
    }  // end of ExternalLibraryManager::getLowerPhysicalBound

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
    }  // end of ExternalLibraryManager::getUpperPhysicalBound

    unsigned short ExternalLibraryManager::getCastemFunctionNumberOfVariables(
        const std::string& l, const std::string& f) {
      const auto lib = this->loadLibrary(l);
      const auto res =
          ::tfel_getCastemFunctionNumberOfVariables(lib, f.c_str());
      raise_if(res < 0,
               "ExternalLibraryManager::getCastemFunctionNumberOfVariables: "
               "number of variables could not be read (" +
                   getErrorMessage() + ")");
      return static_cast<unsigned short>(res);
    }

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
    }  // end of ExternalLibraryManager::getUMATRequiresStiffnessTensor

    bool
    ExternalLibraryManager::getUMATRequiresThermalExpansionCoefficientTensor(
        const std::string& l, const std::string& f, const std::string& h) {
      ExternalLibraryManagerCheckModellingHypothesisName(h);
      auto lib = this->loadLibrary(l);
      auto res = ::tfel_getUMATRequiresThermalExpansionCoefficientTensor(
          lib, (f + "_" + h).c_str());
      if (res < 0) {
        res = ::tfel_getUMATRequiresThermalExpansionCoefficientTensor(
            lib, f.c_str());
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
    }  // end of ExternalLibraryManager::getAsterFiniteStrainFormulation

    AsterIntegrationErrorMessageFctPtr
    ExternalLibraryManager::getAsterIntegrationErrorMessageFunction(
        const std::string& l, const std::string& f) {
      const auto lib = this->loadLibrary(l);
      const auto s = f + "_getIntegrationErrorMessage";
      return ::tfel_getAsterIntegrationErrorMessage(lib, s.c_str());
    }  // end of ExternalLibraryManager::getAsterIntegrationErrorMessage

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
    }  // end of ExternalLibraryManager::getAbaqusOrthotropyManagementPolicy

    std::vector<std::string> ExternalLibraryManager::getCastemFunctionVariables(
        const std::string& l, const std::string& f) {
      std::vector<std::string> vars;
      this->getCastemFunctionVariables(vars, l, f);
      return vars;
    }  // end of ExternalLibraryManager::getCastemFunctionVariables

    void ExternalLibraryManager::getCastemFunctionVariables(
        std::vector<std::string>& vars,
        const std::string& l,
        const std::string& f) {
      const auto lib = this->loadLibrary(l);
      unsigned short nb = this->getCastemFunctionNumberOfVariables(l, f);
      char** res = ::tfel_getCastemFunctionVariables(lib, f.c_str());
      char** p;
      raise_if(res == nullptr,
               "ExternalLibraryManager::getCastemFunctionNumberOfVariables: "
               " variables names could not be read (" +
                   getErrorMessage() + ")");
      for (p = res; p != res + nb; ++p) {
        vars.emplace_back(*p);
      }
    }  // end of ExternalLibraryManager::getCastemFunctionVariables

    GenericBehaviourFctPtr ExternalLibraryManager::getGenericBehaviourFunction(
        const std::string& l, const std::string& f) {
      const auto lib = this->loadLibrary(l);
      const auto fct = ::tfel_getGenericBehaviourFunction(lib, f.c_str());
      raise_if(fct == nullptr,
               "ExternalLibraryManager::getGenericBehaviourFunction: "
               "could not load generic gehaviour function '" +
                   f + "' (" + getErrorMessage() + ")");
      return fct;
    }

    CyranoFctPtr ExternalLibraryManager::getCyranoFunction(
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
      const auto fct =
          ::tfel_getAbaqusExternalBehaviourFunction(lib, f.c_str());
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

    CastemFctPtr ExternalLibraryManager::getCastemExternalBehaviourFunction(
        const std::string& l, const std::string& f) {
      const auto lib = this->loadLibrary(l);
      const auto fct =
          ::tfel_getCastemExternalBehaviourFunction(lib, f.c_str());
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
    }  // end of ExternalLibraryManager::getUMATNames

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
        res = ::tfel_getArrayOfInts(lib,
                                    (f + "_" + h + '_' + n + "Types").c_str());
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
    }  // end of ExternalLibraryManager::getUMATTypes

    bool
    ExternalLibraryManager::isUMATBehaviourUsableInPurelyImplicitResolution(
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
      const auto u =
          ::tfel_getUnsignedShort(lib, (f + "_BehaviourType").c_str());
      raise_if(u == -1,
               "ExternalLibraryManager::getUMATBehaviourType: "
               "behaviour type could not be read (" +
                   getErrorMessage() + ")");
      return static_cast<unsigned short>(u);
    }  // end of ExternalLibraryManager::getUMATBehaviourType

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
    }  // end of ExternalLibraryManager::getUMATBehaviourKinematic

    unsigned short ExternalLibraryManager::getUMATSymmetryType(
        const std::string& l, const std::string& f) {
      const auto lib = this->loadLibrary(l);
      const auto u =
          ::tfel_getUnsignedShort(lib, (f + "_SymmetryType").c_str());
      raise_if(u == -1,
               "ExternalLibraryManager::getUMATSymmetryType: "
               "symmetry type could not be read (" +
                   getErrorMessage() + ")");
      return static_cast<unsigned short>(u);
    }  // end of ExternalLibraryManager::getUMATSymmetryType

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
    }  // end of ExternalLibraryManager::getUMATElasticSymmetryType

    std::vector<std::string>
    ExternalLibraryManager::getUMATMaterialPropertiesNames(
        const std::string& l, const std::string& f, const std::string& h) {
      std::vector<std::string> vars;
      this->getUMATNames(vars, l, f, h, "MaterialProperties");
      return vars;
    }  // end of ExternalLibraryManager::getUMATMaterialPropertiesNames

    std::vector<std::string> ExternalLibraryManager::getUMATGradientsNames(
        const std::string& l, const std::string& f) {
      std::vector<std::string> names;
      this->getUMATNames(names, l, f, "", "Gradients");
      return names;
    }  // end of ExternalLibraryManager::getUMATGradientsNames

    std::vector<std::string>
    ExternalLibraryManager::getUMATDrivingVariablesNames(const std::string& l,
                                                         const std::string& f) {
      return this->getUMATGradientsNames(l, f);
    }  // end of ExternalLibraryManager::getUMATDrivingVariablesNames

    std::vector<int> ExternalLibraryManager::getUMATGradientsTypes(
        const std::string& l, const std::string& f) {
      std::vector<int> types;
      this->getUMATTypes(types, l, f, "", "Gradients");
      return types;
    }  // end of ExternalLibraryManager::getUMATGradientsTypes

    std::vector<int> ExternalLibraryManager::getUMATDrivingVariablesTypes(
        const std::string& l, const std::string& f) {
      return this->getUMATGradientsTypes(l, f);
    }  // end of ExternalLibraryManager::getUMATDrivingVariablesTypes

    std::vector<std::string>
    ExternalLibraryManager::getUMATThermodynamicForcesNames(
        const std::string& l, const std::string& f) {
      std::vector<std::string> names;
      this->getUMATNames(names, l, f, "", "ThermodynamicForces");
      return names;
    }  // end of ExternalLibraryManager::getUMATThermodynamicForcesNames

    std::vector<int> ExternalLibraryManager::getUMATThermodynamicForcesTypes(
        const std::string& l, const std::string& f) {
      std::vector<int> types;
      this->getUMATTypes(types, l, f, "", "ThermodynamicForces");
      return types;
    }  // end of ExternalLibraryManager::getUMATThermodynamicForcesTypes

    std::vector<std::string>
    ExternalLibraryManager::getUMATInternalStateVariablesNames(
        const std::string& l, const std::string& f, const std::string& h) {
      std::vector<std::string> vars;
      this->getUMATNames(vars, l, f, h, "InternalStateVariables");
      return vars;
    }  // end of ExternalLibraryManager::getUMATInternalStateVariablesNames

    std::vector<int> ExternalLibraryManager::getUMATInternalStateVariablesTypes(
        const std::string& l, const std::string& f, const std::string& h) {
      std::vector<int> types;
      this->getUMATTypes(types, l, f, h, "InternalStateVariables");
      return types;
    }  // end of ExternalLibraryManager::getUMATInternalVariablesTypes

    std::vector<std::string>
    ExternalLibraryManager::getUMATExternalStateVariablesNames(
        const std::string& l, const std::string& f, const std::string& h) {
      std::vector<std::string> vars;
      this->getUMATNames(vars, l, f, h, "ExternalStateVariables");
      return vars;
    }  // end of ExternalLibraryManager::getUMATMaterialPropertiesNames

    std::vector<std::string> ExternalLibraryManager::getUMATParametersNames(
        const std::string& l, const std::string& f, const std::string& h) {
      std::vector<std::string> names;
      this->getUMATNames(names, l, f, h, "Parameters");
      return names;
    }  // end of ExternalLibraryManager::getUMATMaterialPropertiesNames

    std::vector<int> ExternalLibraryManager::getUMATParametersTypes(
        const std::string& l, const std::string& f, const std::string& h) {
      std::vector<int> types;
      this->getUMATTypes(types, l, f, h, "Parameters");
      return types;
    }  // end of ExternalLibraryManager::getUMATInternalVariablesTypes

    CastemFunctionPtr ExternalLibraryManager::getCastemFunction(
        const std::string& l, const std::string& f) {
      const auto lib = this->loadLibrary(l);
      auto fct = ::tfel_getCastemFunction(lib, f.c_str());
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
               "ExternalLibraryManager::getCFunction9: "
               "could not load function '" +
                   f +
                   "' "
                   "(" +
                   getErrorMessage() + ")");
      return fct;
    }

    CFunction10Ptr ExternalLibraryManager::getCFunction10(
        const std::string& l, const std::string& f) {
      const auto lib = this->loadLibrary(l);
      const auto fct = ::tfel_getCFunction10(lib, f.c_str());
      raise_if(fct == nullptr,
               "ExternalLibraryManager::getCFunction10: "
               "could not load function '" +
                   f +
                   "' "
                   "(" +
                   getErrorMessage() + ")");
      return fct;
    }

    CFunction11Ptr ExternalLibraryManager::getCFunction11(
        const std::string& l, const std::string& f) {
      const auto lib = this->loadLibrary(l);
      const auto fct = ::tfel_getCFunction11(lib, f.c_str());
      raise_if(fct == nullptr,
               "ExternalLibraryManager::getCFunction11: "
               "could not load function '" +
                   f +
                   "' "
                   "(" +
                   getErrorMessage() + ")");
      return fct;
    }

    CFunction12Ptr ExternalLibraryManager::getCFunction12(
        const std::string& l, const std::string& f) {
      const auto lib = this->loadLibrary(l);
      const auto fct = ::tfel_getCFunction12(lib, f.c_str());
      raise_if(fct == nullptr,
               "ExternalLibraryManager::getCFunction12: "
               "could not load function '" +
                   f +
                   "' "
                   "(" +
                   getErrorMessage() + ")");
      return fct;
    }

    CFunction13Ptr ExternalLibraryManager::getCFunction13(
        const std::string& l, const std::string& f) {
      const auto lib = this->loadLibrary(l);
      const auto fct = ::tfel_getCFunction13(lib, f.c_str());
      raise_if(fct == nullptr,
               "ExternalLibraryManager::getCFunction13: "
               "could not load function '" +
                   f +
                   "' "
                   "(" +
                   getErrorMessage() + ")");
      return fct;
    }

    CFunction14Ptr ExternalLibraryManager::getCFunction14(
        const std::string& l, const std::string& f) {
      const auto lib = this->loadLibrary(l);
      const auto fct = ::tfel_getCFunction14(lib, f.c_str());
      raise_if(fct == nullptr,
               "ExternalLibraryManager::getCFunction14: "
               "could not load function '" +
                   f +
                   "' "
                   "(" +
                   getErrorMessage() + ")");
      return fct;
    }

    CFunction15Ptr ExternalLibraryManager::getCFunction15(
        const std::string& l, const std::string& f) {
      const auto lib = this->loadLibrary(l);
      const auto fct = ::tfel_getCFunction15(lib, f.c_str());
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
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
      raise_if(fct == nullptr,
               "ExternalLibraryManager::getFortranFunction15: "
               "could not load function '" +
                   f +
                   "' "
                   "(" +
                   getErrorMessage() + ")");
      return fct;
    }

    ExternalLibraryManager::~ExternalLibraryManager() = default;

  }  // end of namespace system

}  // end of namespace tfel
