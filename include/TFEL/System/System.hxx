/*!
 * \file   System.hxx
 * \brief  This file declares the System class
 * \author Helfer Thomas
 * \date   06 Nov 2007
 */

#ifndef _LIB_TFEL_SYSTEM_HXX_
#define _LIB_TFEL_SYSTEM_HXX_ 

#include<vector>
#include<string>

#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

#include"TFEL/System/SystemError.hxx"

namespace tfel
{

  namespace system
  {

    /*
     * This structure contains C++ wrapping over many
     * posix system call.
     */
    struct systemCall
    {

      /*
       * An helper function to convert errno values to C++ exceptions.
       * \param const std::string&, a description of the error.
       * \param const int, value of errno.
       */
      static void
      throwSystemError(const std::string&,
		       const int);

      /*
       * create one or more directories.
       * This command is equivalent to the shell command 'mkdir -p'.
       * \param const std::string&, path to be created.
       * \param const mode_t, opening mode.
       */
      static void
      mkdir(const std::string&,const mode_t = S_IRWXU|S_IRWXG);

      /*!
       * \param f : file to unlink
       */
      static void
      unlink(const std::string&);

      static void
      write(const int,const void* const,size_t);

      /*
       * copy a file or a directory.
       * This command is equivalent to the shell command 'cp -R'.
       * \param const std::string&, src.
       * \param const std::string&, destination.
       */
      static void
      copy(const std::string&,const std::string&);

      // change the current working directory
      // (little wrapper of ::chdir)
      static void
      changeCurrentWorkingDirectory(const std::string&);

      // get the current working directory
      // (little wrapper of ::getwd)
      static std::string
      getCurrentWorkingDirectory(void);

      // get the current working directory
      // (little wrapper of ::getlogin_r)
      static std::string
      getUserName(void);

      // get the current working directory
      // (little wrapper of ::gethostname)
      static std::string
      getHostName(void);

    private:

      // copy a directory
      static void
      copyDirectory(const std::string&,
		    const std::string&,
		    const bool);

      // copy a file to a file
      static void
      copyFile(const std::string&,const std::string&);

      // return the type of a file
      // the mode_t shall be given by a call to stat
      static std::string
      fileType(const mode_t);

      // an helper function
      static std::vector<std::string>
      tokenize(const std::string&,const char);

      // an helper function
      static std::vector<std::string>
      tokenize(const std::string&,const std::string&);

    }; // end of struct systemCall
  
  } // end of namespace system

} // end of namespace tfel

#endif /* _LIB_TFEL_SYSTEM_HXX_ */

