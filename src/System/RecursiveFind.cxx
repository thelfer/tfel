/*!
 * \file  src/System/RecursiveFind.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 03 mai 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <stdexcept>
#include <cstring>
#include <memory>
#include <cerrno>

#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
#define NOMINAX
#include <windows.h>
#include <conio.h>
#else /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/param.h>
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */

#include "TFEL/Raise.hxx"
#include "TFEL/System/System.hxx"
#include "TFEL/System/RecursiveFind.hxx"

namespace tfel::system {

  std::map<std::string, std::vector<std::string>> recursiveFind(
      const std::string& re,
      const std::string& name,
      const bool b,
      const unsigned short depth,
      const unsigned short mdepth) {
    std::map<std::string, std::vector<std::string>> r;
    recursiveFind(r, re, name, b, depth, mdepth);
    return r;
  }  // end of recursiveFind

  void recursiveFind(std::map<std::string, std::vector<std::string>>& r,
                     const std::string& re,
                     const std::string& name,
                     const bool b,
                     const unsigned short depth,
                     const unsigned short mdepth) {
    recursiveFind(r, std::regex(re), name, b, depth, mdepth);
  }  // end of recursiveFind

  std::map<std::string, std::vector<std::string>> recursiveFind(
      const std::regex& re,
      const std::string& name,
      const bool b,
      const unsigned short depth,
      const unsigned short mdepth) {
    std::map<std::string, std::vector<std::string>> r;
    recursiveFind(r, re, name, b, depth, mdepth);
    return r;
  }  // end of recursiveFind

  void recursiveFind(std::map<std::string, std::vector<std::string>>& r,
                     const std::regex& re,
                     const std::string& name,
                     const bool b,
                     const unsigned short depth,
                     const unsigned short mdepth) {
    auto throw_if = [](const bool c, const std::string& m) {
      raise_if(c, "recursiveFind: " + m);
    };
#if (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__)
    if (depth > mdepth) {
      throw_if(b, "maximal directory depth reached");
      return;
    }
    WIN32_FIND_DATA fh;  // File information
    auto f = ::FindFirstFile((name + "\\*.*").c_str(), &fh);
    if (f == INVALID_HANDLE_VALUE) {
      raise_if<SystemError>(b,
                            "tfel::system::recursiveFind: "
                            "can't open directory '" +
                                name + "'");
      return;
    }
    try {
      do {
        if (fh.cFileName[0] == '.') {
          continue;
        }
        if (fh.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
          recursiveFind(r, re, name + "\\" + fh.cFileName, b, depth + 1,
                        mdepth);
        } else {
          if (std::regex_match(fh.cFileName, re)) {
            r[name].emplace_back(fh.cFileName);
          }
        }
      } while (::FindNextFile(f, &fh) == TRUE);
      // Close handle
    } catch (...) {
      ::FindClose(f);
      throw;
    }
    ::FindClose(f);
#else  /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
    using DIRHandler = std::unique_ptr<DIR, int (*)(DIR*)>;
    DIRHandler dir(::opendir(name.c_str()), &::closedir);
    if (depth > mdepth) {
      throw_if(b, "maximal directory depth reached");
      return;
    }
    if (!dir) {
      if (b) {
        systemCall::throwSystemError("can't open directory '" + name + "'",
                                     errno);
      }
      return;
    }
    struct dirent* p;
    while ((p = readdir(dir.get())) != nullptr) {
      const auto file = name + tfel::system::dirSeparator() + p->d_name;
      struct stat buf;
      if (stat(file.c_str(), &buf) == 0) {
        if (S_ISREG(buf.st_mode)) {
          if (std::regex_match(p->d_name, re)) {
            r[name].emplace_back(p->d_name);
          }
        } else if (S_ISDIR(buf.st_mode)) {
          if ((strcmp(p->d_name, ".") != 0) && (strcmp(p->d_name, "..") != 0)) {
            std::map<std::string, std::vector<std::string>> r2;
            recursiveFind(r2, re, name + '/' + p->d_name, b, depth + 1, mdepth);
            r.insert(r2.begin(), r2.end());
          }
        }
      } else {
        throw_if(b, "can't stat file '" + file + "'");
      }
    }
#endif /* (defined _WIN32 || defined _WIN64) && (!defined __CYGWIN__) */
  }    // end of recursiveFind

}  // end of namespace tfel::system
