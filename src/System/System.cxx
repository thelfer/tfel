/*!
 * \file   src/System/System.cxx
 * \brief
 * \author Thomas Helfer
 * \date   06/11/2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */
#include <iostream>

#include <sstream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <climits>

#if !((defined _WIN32) || (defined _WIN64))
#include <unistd.h>
#include <dirent.h>
#else
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <direct.h>
#ifndef S_ISDIR
#define S_ISDIR(mode) (((mode)&S_IFMT) == S_IFDIR)
#endif
#ifndef S_ISREG
#define S_ISREG(mode) (((mode)&S_IFMT) == S_IFREG)
#endif
#ifndef S_ISCHR
#define S_ISCHR(m) (((m)&S_IFMT) == S_IFCHR)
#endif /* S_ISCHR */
/* Not in MS Visual Studio 2008 Express */
#ifndef S_IFBLK
#ifndef S_ISBLK
#define S_ISBLK(m) (((m)&S_IFMT) == S_IFBLK)
#endif /* S_ISBLK */
#define S_IFBLK(m) (0)
#endif /* S_IFBLK */
#ifndef S_IFIFO
#ifndef S_ISFIFO
#define S_ISFIFO(m) (((m)&S_IFMT) == S_IFIFO)
#endif /* S_ISFIFO */
#define S_IFFIFO(m) (0)
#endif /* S_IFIFO */
#endif

#include "TFEL/System/SystemError.hxx"
#include "TFEL/System/System.hxx"

namespace tfel {

  namespace system {

#if (defined _WIN32) || (defined _WIN64)
    static int System_rmdir(const std::string& d) {
      bool bSubdirectory = false;  // Flag, indicating whether
      // subdirectories have been found
      HANDLE hFile;                     // Handle to directory
      std::string strFilePath;          // Filepath
      std::string strPattern;           // Pattern
      WIN32_FIND_DATA FileInformation;  // File information
      strPattern = d + "\\*.*";
      hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
      if (hFile != INVALID_HANDLE_VALUE) {
        do {
          if (FileInformation.cFileName[0] != '.') {
            strFilePath.erase();
            strFilePath = d + "\\" + FileInformation.cFileName;
            if (FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
              // Delete subdirectory
              System_rmdir(strFilePath);
            } else {
              // Set file attributes
              if (::SetFileAttributes(strFilePath.c_str(),
                                      FILE_ATTRIBUTE_NORMAL) == FALSE) {
                return ::GetLastError();
              }
              // Delete file
              if (::DeleteFile(strFilePath.c_str()) == FALSE) {
                return ::GetLastError();
              }
            }
          }
        } while (::FindNextFile(hFile, &FileInformation) == TRUE);
        // Close handle
        ::FindClose(hFile);
        DWORD dwError = ::GetLastError();
        if (dwError != ERROR_NO_MORE_FILES) {
          return dwError;
        } else {
          if (!bSubdirectory) {
            // Set directory attributes
            if (::SetFileAttributes(d.c_str(), FILE_ATTRIBUTE_NORMAL) ==
                FALSE) {
              return ::GetLastError();
            }
            // Delete directory
            if (::RemoveDirectory(d.c_str()) == FALSE) {
              return ::GetLastError();
            }
          }
        }
      }
      return 0;
    }
#endif /* (defined _WIN32) || (defined _WIN64) */

    char dirSeparator() { return '/'; }  // end of systemCall::dirSeparator

    const std::string& dirStringSeparator() {
      using namespace std;
      static string s(1u, dirSeparator());
      return s;
    }  // end of systemCall::dirStringSeparator

    void systemCall::throwSystemError(const std::string& errMsg,
                                      const int errNbr) {
      std::string err = strerror(errNbr);
      std::transform(err.begin(), err.end(), err.begin(),
                     [](const char c) { return ::tolower(c); });
      auto msg = errMsg;
      msg += " (system error: ";
      msg += err;
      msg += ")";
      switch (errNbr) {
        case EPERM:
          throw(PosixError<EPERM>(msg));
        case ENOENT:
          throw(PosixError<ENOENT>(msg));
        case ESRCH:
          throw(PosixError<ESRCH>(msg));
        case EINTR:
          throw(PosixError<EINTR>(msg));
        case EIO:
          throw(PosixError<EIO>(msg));
        case ENXIO:
          throw(PosixError<ENXIO>(msg));
        case E2BIG:
          throw(PosixError<E2BIG>(msg));
        case ENOEXEC:
          throw(PosixError<ENOEXEC>(msg));
        case EBADF:
          throw(PosixError<EBADF>(msg));
        case ECHILD:
          throw(PosixError<ECHILD>(msg));
        case EAGAIN:
          throw(PosixError<EAGAIN>(msg));
        case ENOMEM:
          throw(PosixError<ENOMEM>(msg));
        case EACCES:
          throw(PosixError<EACCES>(msg));
        case EFAULT:
          throw(PosixError<EFAULT>(msg));
        case EBUSY:
          throw(PosixError<EBUSY>(msg));
        case EEXIST:
          throw(PosixError<EEXIST>(msg));
        case EXDEV:
          throw(PosixError<EXDEV>(msg));
        case ENODEV:
          throw(PosixError<ENODEV>(msg));
        case ENOTDIR:
          throw(PosixError<ENOTDIR>(msg));
        case EISDIR:
          throw(PosixError<EISDIR>(msg));
        case EINVAL:
          throw(PosixError<EINVAL>(msg));
        case ENFILE:
          throw(PosixError<ENFILE>(msg));
        case EMFILE:
          throw(PosixError<EMFILE>(msg));
        case ENOTTY:
          throw(PosixError<ENOTTY>(msg));
        case EFBIG:
          throw(PosixError<EFBIG>(msg));
        case ENOSPC:
          throw(PosixError<ENOSPC>(msg));
        case ESPIPE:
          throw(PosixError<ESPIPE>(msg));
        case EROFS:
          throw(PosixError<EROFS>(msg));
        case EMLINK:
          throw(PosixError<EMLINK>(msg));
        case EPIPE:
          throw(PosixError<EPIPE>(msg));
        case EDOM:
          throw(PosixError<EDOM>(msg));
        case ERANGE:
          throw(PosixError<ERANGE>(msg));
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
        case ENOTBLK:
          throw(PosixError<ENOTBLK>(msg));
        case ETXTBSY:
          throw(PosixError<ETXTBSY>(msg));
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */
        default:
          throw(SystemError(msg));
      }
    }  // end of throwSystemError

    std::vector<std::string> systemCall::tokenize(const std::string& s,
                                                  const char c) {
      using namespace std;
      vector<string> res;
      string::size_type b = 0u;
      string::size_type e = s.find_first_of(c, b);
      while (string::npos != e || string::npos != b) {
        // Found a token, add it to the vector.
        res.push_back(s.substr(b, e - b));
        b = s.find_first_not_of(c, e);
        e = s.find_first_of(c, b);
      }
      return res;
    }  // end of systemCall::tokenize

    std::vector<std::string> systemCall::tokenize(const std::string& s,
                                                  const std::string& delim) {
      using namespace std;
      vector<string> res;
      string::size_type b = 0u;
      string::size_type e = s.find_first_of(delim, b);
      while (string::npos != e || string::npos != b) {
        // Found a token, add it to the vector.
        res.push_back(s.substr(b, e - b));
        b = s.find_first_not_of(delim, e);
        e = s.find_first_of(delim, b);
      }
      return res;
    }  // end of systemCall::tokenize

    void systemCall::copy(const std::string& src, const std::string& dest) {
      using namespace std;
      struct stat srcInfos;
      struct stat destInfos;
      bool destStatus;
      if (src == dest) {
        return;
      }
      if (::stat(src.c_str(), &srcInfos) == -1) {
        systemCall::throwSystemError(
            "systemCall::copy : can't stat file " + src, errno);
      }
      destStatus = ::stat(dest.c_str(), &destInfos) == 0;
      if (!destStatus) {
        if (errno != ENOENT) {
          systemCall::throwSystemError(
              "systemCall::copy : can't stat file " + dest, errno);
        }
      }
      if (S_ISDIR(srcInfos.st_mode)) {
        // copying a directory
        if (destStatus) {
          // the destination exist, it shall be a directory
          if (!S_ISDIR(destInfos.st_mode)) {
            throw(SystemError(
                "systemCall::copy : can't copy directory '" + src + "' on " +
                systemCall::fileType(destInfos.st_mode) + " '" + dest + "'."));
          }
          systemCall::copyDirectory(src, dest, true);
          return;
        } else {
          systemCall::copyDirectory(src, dest, false);
          return;
        }
      } else if (S_ISREG(srcInfos.st_mode)) {
        // copying a file, the destination
        // shall be a file or a directory
        if (destStatus) {
          if (S_ISDIR(destInfos.st_mode)) {
            // destination is a directory
            const auto& tmp = systemCall::tokenize(src, '/');
            systemCall::copyFile(src, dest + '/' + tmp.back());
            return;
          } else if (S_ISREG(destInfos.st_mode)) {
            // destination is a file
            systemCall::copyFile(src, dest);
            return;
          }
          throw(SystemError("systemCall::copy : can't copy file '" + src +
                            "' on " + systemCall::fileType(destInfos.st_mode) +
                            " '" + dest + "'."));
        }
        // destination don't exist, trying to copy to it
        systemCall::copyFile(src, dest);
        return;
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
      }
#else
      } else if (S_ISCHR(destInfos.st_mode) || S_ISBLK(destInfos.st_mode) ||
                 S_ISFIFO(destInfos.st_mode)) {
        string msg("systemCall::copy : can't copy object of type " +
                   systemCall::fileType(destInfos.st_mode));
        throw(SystemError(msg));
      }
#endif /* defined _WIN32 || _WIN64 */
    }  // end of systemCall::copy

#if defined _WIN32 || defined _WIN64
    void systemCall::mkdir(const std::string& dir)
#else
    void systemCall::mkdir(const std::string& dir, const mode_t mode)
#endif /* defined _WIN32 || _WIN64 */
    {
      using namespace std;
      string path;
      const auto& paths = systemCall::tokenize(dir, '/');
      vector<string>::const_iterator p;
      for (p = paths.begin(); p != paths.end(); ++p) {
        if (!path.empty()) {
          path += "/" + *p;
        } else {
          path = *p;
        }
        struct stat infos;
        if (::stat(path.c_str(), &infos) == -1) {
          if (errno == ENOENT) {
            // the file does not exist, create the directory
#if defined _WIN32 || defined _WIN64
            if (::CreateDirectoryA(path.c_str(), nullptr) == 0) {
#else
            if (::mkdir(path.c_str(), mode) != 0) {
#endif /* defined _WIN32 || _WIN64 */
              if (errno == EEXIST) {
                // this may happen if many processes are
                // used in the same time
                if (::stat(path.c_str(), &infos) == 0) {
                  if (S_ISDIR(infos.st_mode)) {
                    return;
                  }
                } else {
                  systemCall::throwSystemError(
                      "systemCall::mkdir : can't stat file '" + *p + "'",
                      errno);
                }
              }
              systemCall::throwSystemError(
                  "systemCall::mkdir : can't create directory '" + dir + "'",
                  errno);
            }
          } else {
            systemCall::throwSystemError(
                "systemCall::mkdir : can't stat file '" + *p + "'", errno);
          }
        } else {
          // the path is valid, check if it is a directory
          if (!S_ISDIR(infos.st_mode)) {
            string msg("systemCall::mkdir : '");
            msg += path + "' exists and is not a directory.";
            throw(SystemError(msg));
          }
        }
      }
    }  // end of systemCall::mkdir

    void systemCall::unlink(const std::string& f) {
      if (::unlink(f.c_str()) != 0) {
        systemCall::throwSystemError(
            "systemCall::unlink : can't unlink file " + f, errno);
      }
    }  // end of systemCall::unlinkx

    void systemCall::rmdir(const std::string& d) {
#if defined _WIN32 || defined _WIN64
      System_rmdir(d);
#else
      using namespace std;
      struct stat dInfos;
      vector<string> o;
      if (::stat(d.c_str(), &dInfos) == -1) {
        systemCall::throwSystemError(
            "systemCall::rmdir : can't stat directory " + d, errno);
      }
      if (!S_ISDIR(dInfos.st_mode)) {
        string msg("systemCall::rmdir : '");
        msg += d + "' is not a directory";
        throw(runtime_error(msg));
      }
      DIR* dir = opendir(d.c_str());
      if (dir == nullptr) {
        systemCall::throwSystemError(
            "systemCall::rmdir, can't open directory " + d, errno);
      }
      struct dirent* p;
      while ((p = readdir(dir)) != nullptr) {
        if ((strcmp(p->d_name, ".") != 0) && (strcmp(p->d_name, "..") != 0)) {
          o.emplace_back(p->d_name);
        }
      }
      vector<string>::const_iterator po;
      for (po = o.begin(); po != o.end(); ++po) {
        struct stat pInfos;
        string f;
        f = d + "/" + *po;
        if (::stat(f.c_str(), &pInfos) == -1) {
          systemCall::throwSystemError(
              "systemCall::rmdir : can't stat '" + f + "'", errno);
        }
        if (S_ISDIR(pInfos.st_mode)) {
          systemCall::rmdir(f);
        } else {
          systemCall::unlink(f);
        }
      }
      closedir(dir);
      ::rmdir(d.c_str());
#endif
    }  // end of systemCall::rmdir

#if !(defined _WIN32 || defined _WIN64)
    void systemCall::write(const int f, const void* const v, size_t s) {
      /*
       * code adapated from C. Blaess
       * "Programmation système en C sous linux"
       * 2ème édition, 2005, Eyrolles
       * page 5009
       */
      using namespace std;
      const unsigned char* b = static_cast<const unsigned char*>(v);
      size_t r = s;
      if (s >= SSIZE_MAX) {
        string msg("systemCall::write : invalid size (s>=SSIZE_MAX)");
        throw(SystemError(msg));
      }
      while (r > 0) {
        ssize_t w;
        while ((w = ::write(f, b, r)) == -1) {
          if (errno == EINTR) {
            continue;
          }
          if (errno != EAGAIN) {
            systemCall::throwSystemError("systemCall::write failed", errno);
          }
          sleep(1);
        }
        r = static_cast<size_t>(r - w);
        b += w;
      }
    }  // end of systemCall::write
#endif /* !(defined _WIN32 || defined _WIN64 ) */

    std::string systemCall::fileType(const mode_t mode) {
      using namespace std;
      if (S_ISDIR(mode)) {
        return "directory";
      } else if (S_ISREG(mode)) {
        return "regular file";
      } else if (S_ISCHR(mode)) {
        return "character device";
#if !(defined _WIN32 || defined _WIN64)
      } else if (S_ISBLK(mode)) {
        return "block device";
      } else if (S_ISFIFO(mode)) {
        return "fifo";
      } else if (S_ISLNK(mode)) {
        return "symbolic link";
      } else if (S_ISSOCK(mode)) {
        return "socket";
#endif /* defined _WIN32 || _WIN64 */
      }
      return "unknown";
    }  // end of systemCall::fileType

    void systemCall::copyDirectory(const std::string& src,
                                   const std::string& dest,
                                   const bool exists) {
      using namespace std;
      const auto& paths = systemCall::tokenize(src, '/');
      string rdest;
      if (exists) {
        rdest = dest + '/' + paths.back();
      } else {
        rdest = dest;
      }
      systemCall::mkdir(rdest);
#if defined _WIN32 || defined _WIN64
#pragma message("windows port")
#else
      DIR* dir = opendir(src.c_str());
      if (dir == nullptr) {
        systemCall::throwSystemError(
            "systemCall::copy, can't open directory " + src, errno);
      }
      struct dirent* p;
      while ((p = readdir(dir)) != nullptr) {
        if ((strcmp(p->d_name, ".") != 0) && (strcmp(p->d_name, "..") != 0)) {
          systemCall::copy(src + '/' + p->d_name, rdest);
        }
      }
      closedir(dir);
#endif
    }  // end of systemCall::copyDirectory

    void systemCall::copyFile(const std::string& src, const std::string& dest) {
      struct stat srcInfos;
      if (::stat(src.c_str(), &srcInfos) == -1) {
        systemCall::throwSystemError(
            "systemCall::copyFile : can't stat file " + src, errno);
      }
      if (!S_ISREG(srcInfos.st_mode)) {
        throw(std::runtime_error("systemCall::copyFile : source '" + src +
                                 "' "
                                 "is not a regular file"));
      }
      // simple check
      if (src == dest) {
        return;
      }
      // do the copy
      std::ofstream out(dest.c_str(), std::ios_base::binary);
      std::ifstream in(src.c_str(), std::ios_base::binary);
      if (!in) {
        throw(
            SystemError("systemCall::copyFile : can't open file "
                        "'" +
                        src + "' in read mode."));
      }
      if (!out) {
        throw(
            SystemError("systemCall::copyFile : can't open file "
                        "'" +
                        dest + "' in write mode."));
      }
      // local copy of the file
      if (srcInfos.st_size != 0) {
        // file is not empty
        out.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        out << in.rdbuf();
      }
      in.close();
      out.close();
    }  // end of systemCall::copyFile

    std::string systemCall::getCurrentWorkingDirectory() {
      using namespace std;
      char* name = nullptr;
      size_t size = 16u;
      while (1) {
        auto ptr = static_cast<char*>(realloc(name, size));
        if (ptr == nullptr) {
          if (name != nullptr) {
            ::free(name);
          }
          throw(SystemError(
              "systemCall::getCurrentWorkingDirectory : out of memory"));
        }
        name = ptr;
#if defined _WIN32 || defined _WIN64
        if (_getcwd(name, size) != nullptr) {
#else
        if (::getcwd(name, size) != nullptr) {
#endif
          break;
        }
        if (errno != ERANGE) {
          if (name != nullptr) {
            ::free(name);
          }
          systemCall::throwSystemError(
              "systemCall::getCurrentWorkingDirectory ", errno);
        }
        size *= 2u;
      }
      const auto res = std::string{name};
      ::free(name);
      return res;
    }  // end of systemCall::getCurrentWorkingDirectory

#if !(defined _WIN32 || defined _WIN64)
    std::string systemCall::getHostName() {
      using namespace std;
      char* name = nullptr;
      size_t size = 16u;
      while (true) {
        auto ptr = static_cast<char*>(realloc(name, size));
        if (ptr == nullptr) {
          if (name != nullptr) {
            ::free(name);
          }
          throw(SystemError("systemCall::getHostName : out of memory"));
        }
        name = ptr;
        if (::gethostname(name, size) == 0) {
          break;
        }
        if (errno != ENAMETOOLONG) {
          if (name != nullptr) {
            ::free(name);
          }
          systemCall::throwSystemError("systemCall::getHostName ", errno);
        }
        size *= 2u;
      }
      const auto res = std::string{name};
      ::free(name);
      return res;
    }  // end of systemCall::getHostName

    std::string systemCall::getUserName() {
      const char* const l = ::getlogin();
      if (l != nullptr) {
        return l;
      }
      return "";
    }  // end of systemCall::getUserName
#endif /* !(defined _WIN32 || defined _WIN64 ) */

    void systemCall::changeCurrentWorkingDirectory(const std::string& name) {
      using namespace std;
#if defined _WIN32 || defined _WIN64
      if (_chdir(name.c_str()) == -1) {
#else
      if (::chdir(name.c_str()) == -1) {
#endif
        systemCall::throwSystemError(
            "systemCall::changeCurrentWorkingDirectory : "
            "can't change to directory " +
                name + ".",
            errno);
      }
    }  // end of systemCall::changeCurrentWorkingDirectory

  }  // end of namespace system

}  // end of namespace tfel
