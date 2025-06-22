/*!
 * \file   GetInstallPath.cxx
 * \brief
 * \author Thomas Helfer
 * \date   21 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <stdexcept>
#include <algorithm>
#include <cstdlib>

#if defined _WIN32 || defined _WIN64
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/Config/GetInstallPath-defines.hxx"

namespace tfel {

#if defined _WIN32 || defined _WIN64
  static bool getValueInRegistry(std::string& value) {
    using namespace std;
    HKEY hKey;
    char szBuffer[512];
    DWORD dwBufferSize = sizeof(szBuffer);
    LONG nError;
    LONG lRes = RegOpenKeyEx(HKEY_CLASSES_ROOT, "TFELHOME-" VERSION, 0,
                             KEY_READ, &hKey);
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

  static std::string handleSpace(const std::string& p) {
#if (defined _WIN32 || defined _WIN64) && \
    (defined __MINGW32__ || defined __MINGW64__)
    if (std::find(p.begin(), p.end(), ' ') != p.end()) {
      throw(std::runtime_error(
          "handleSpace: path to TFEL shall not contain space as "
          "MinGW can't handle it (Found '" +
          p +
          "'). "
          "Please change TFEL installation directory"));
    }
#endif /* (defined _WIN32 || defined _WIN64) && \
    (defined __MINGW32__ || defined __MINGW64__) */
    return p;
  }  // end of handleSpace

  std::string getInstallPath() {
#if defined _WIN32 || defined _WIN64
    // check in the registry (installation through NSIS)
    std::string rpath;
    if (getValueInRegistry(rpath)) {
      auto path = handleSpace(rpath);
      if (!path.empty()) {
        return path;
      }
    }
#endif
    const auto path = ::getenv("TFELHOME");
    if (path != nullptr) {
      return handleSpace(path);
    }
#if defined _WIN32 || defined _WIN64
    return "";
#else
    return PREFIXDIR;
#endif
  }  // end of getInstallPath

  std::string getTFELConfigExecutableName() {
#ifdef _WIN32
#ifdef TFEL_APPEND_SUFFIX
    return "tfel-config-" TFEL_SUFFIX ".exe";
#else  /* TFEL_APPEND_SUFFIX */
    return "tfel-config.exe";
#endif /* TFEL_APPEND_SUFFIX */
#else  /* WIN32 */
#ifdef TFEL_APPEND_SUFFIX
    return "tfel-config-" TFEL_SUFFIX;
#else  /* TFEL_APPEND_SUFFIX */
    return "tfel-config";
#endif /* TFEL_APPEND_SUFFIX */
#endif /* _WIN32 */
  }

  std::string getLibraryInstallName(const std::string& l) {
#ifdef TFEL_APPEND_SUFFIX
    return l + '-' + TFEL_SUFFIX;
#else  /* TFEL_APPEND_SUFFIX */
    return l;
#endif /* TFEL_APPEND_SUFFIX */
  }

}  // end of namespace tfel
