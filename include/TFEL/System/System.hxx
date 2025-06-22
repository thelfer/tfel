/*!
 * \file   include/TFEL/System/System.hxx
 * \brief  This file declares the System class
 * \author Thomas Helfer
 * \date   06 Nov 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_SYSTEM_HXX
#define LIB_TFEL_SYSTEM_HXX

#include <vector>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#if !((defined _WIN32) || (defined _WIN64))
#include <unistd.h>
#else
#ifdef _MSC_VER
using mode_t = int;
#endif
#endif

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/System/SystemError.hxx"

namespace tfel::system {

  /*!
   * \return the directory separator
   */
  TFELSYSTEM_VISIBILITY_EXPORT
  char dirSeparator();

  /*!
   * \return the directory separator as a string
   */
  TFELSYSTEM_VISIBILITY_EXPORT
  const std::string& dirStringSeparator();

  /*!
   * This structure contains C++ wrapping over many
   * posix system call.
   */
  struct TFELSYSTEM_VISIBILITY_EXPORT systemCall {
    /*
     * An helper function to convert errno values to C++ exceptions.
     * \param[in] a description of the error.
     * \param const int, value of errno.
     */
    [[noreturn]] static void throwSystemError(const std::string&, const int);

/*
 * create one or more directories.
 * This command is equivalent to the shell command 'mkdir -p'.
 * \param[in] path to be created.
 * \param const mode_t, opening mode.
 */
#if defined _WIN32 || defined _WIN64
    static void mkdir(const std::string&);
#else
    static void mkdir(const std::string&, const mode_t = S_IRWXU | S_IRWXG);
#endif /* LIB_TFEL_SYSTEM_HXX */

    /*!
     * \param f : file to unlink
     */
    static void unlink(const std::string&);

    /*!
     * \param d : directory to remove
     */
    static void rmdir(const std::string&);

#if !(defined _WIN32 || defined _WIN64)
    static void write(const int, const void* const, size_t);
#endif /* !(defined _WIN32 || defined _WIN64 ) */
    /*!
     * copy a file or a directory.
     * This command is equivalent to the shell command 'cp -R'.
     * \param[in] src.
     * \param[in] destination.
     */
    static void copy(const std::string&, const std::string&);
    /*!
     * \return the absolute path of a file or a directory (this is a
     * simple wrapper around ::realpath).
     * \param[in] f: file name
     */
    static std::string getAbsolutePath(const std::string&);
    // change the current working directory
    // (little wrapper of ::chdir)
    static void changeCurrentWorkingDirectory(const std::string&);

    // get the current working directory
    // (little wrapper of ::getwd)
    static std::string getCurrentWorkingDirectory();

#if !(defined _WIN32 || defined _WIN64)
    // get the current working directory
    // (little wrapper of ::getlogin_r)
    static std::string getUserName();

    // get the current working directory
    // (little wrapper of ::gethostname)
    static std::string getHostName();
#endif /* !(defined _WIN32 || defined _WIN64 ) */

   private:
    // copy a directory
    TFEL_VISIBILITY_LOCAL
    static void copyDirectory(const std::string&,
                              const std::string&,
                              const bool);

    // copy a file to a file
    TFEL_VISIBILITY_LOCAL
    static void copyFile(const std::string&, const std::string&);

    // return the type of a file
    // the mode_t shall be given by a call to stat
    TFEL_VISIBILITY_LOCAL
    static std::string fileType(const mode_t);

    // an helper function
    TFEL_VISIBILITY_LOCAL
    static std::vector<std::string> tokenize(const std::string&, const char);

    // an helper function
    TFEL_VISIBILITY_LOCAL
    static std::vector<std::string> tokenize(const std::string&,
                                             const std::string&);

  };  // end of struct systemCall

}  // end of namespace tfel::system

#endif /* LIB_TFEL_SYSTEM_HXX */
