/*!
 * \file  src/System/RecursiveFind.cxx
 * \brief
 * \author Helfer Thomas
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
#include <cerrno>

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/param.h>
#include <regex.h>

#include "TFEL/System/System.hxx"
#include "TFEL/System/RecursiveFind.hxx"

namespace tfel {

  namespace system {

    std::map<std::string, std::vector<std::string>> recursiveFind(
        const std::string& re,
        const std::string& name,
        const bool b,
        const unsigned short depth,
        const unsigned short mdepth) {
      using namespace std;
      map<string, vector<string>> r;
      recursiveFind(r, re, name, b, depth, mdepth);
      return r;
    }  // end of recursiveFind

    void recursiveFind(std::map<std::string, std::vector<std::string>>& r,
                       const std::string& re,
                       const std::string& name,
                       const bool b,
                       const unsigned short depth,
                       const unsigned short mdepth) {
      using namespace std;
      regex_t regex;
      try {
        if (regcomp(&regex, re.c_str(), REG_EXTENDED | REG_NOSUB) != 0) {
          string msg("recursiveFind : ");
          msg += "can't compile regular expression '" + re + "'";
          throw(runtime_error(msg));
        }
        recursiveFind(r, regex, name, b, depth, mdepth);
        regfree(&regex);
      } catch (...) {
        regfree(&regex);
        throw;
      }
    }  // end of recursiveFind

    std::map<std::string, std::vector<std::string>> recursiveFind(
        const regex_t& re,
        const std::string& name,
        const bool b,
        const unsigned short depth,
        const unsigned short mdepth) {
      using namespace std;
      map<string, vector<string>> r;
      recursiveFind(r, re, name, b, depth, mdepth);
      return r;
    }  // end of recursiveFind

    void recursiveFind(std::map<std::string, std::vector<std::string>>& r,
                       const regex_t& re,
                       const std::string& name,
                       const bool b,
                       const unsigned short depth,
                       const unsigned short mdepth) {
      using namespace std;
      using namespace tfel::system;
      DIR* dir;
      struct dirent* p;
      struct stat buf;
      if (depth > mdepth) {
        if (b) {
          string msg("recursiveFind : ");
          msg += "maximal directory depth reached";
          throw(runtime_error(msg));
        }
        return;
      }
      dir = opendir(name.c_str());
      if (dir == nullptr) {
        if (b) {
          systemCall::throwSystemError("can't open directory '" + name + "'",
                                       errno);
        }
        return;
      }
      try {
        while ((p = readdir(dir)) != nullptr) {
          string file = name + "/";
          file += p->d_name;
          if (stat(file.c_str(), &buf) == 0) {
            if (S_ISREG(buf.st_mode)) {
              if (regexec(&re, p->d_name, 0, nullptr, 0) == 0) {
                r[name].emplace_back(p->d_name);
              }
            } else if (S_ISDIR(buf.st_mode)) {
              if ((strcmp(p->d_name, ".") != 0) &&
                  (strcmp(p->d_name, "..") != 0)) {
                map<string, vector<string>> r2;
                recursiveFind(r2, re, name + '/' + p->d_name, b, depth + 1,
                              mdepth);
                r.insert(r2.begin(), r2.end());
              }
            }
          } else {
            if (b) {
              string msg("tfel::system::recursiveFind : ");
              msg += "can't stat file '" + file + "'";
              systemCall::throwSystemError(msg, errno);
            }
          }
        }
      } catch (...) {
        closedir(dir);
        throw;
      }
      closedir(dir);
    }  // end of recursiveFind

  }  // end of namespace system

}  // end of namespace tfel
