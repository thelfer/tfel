/*!
 * \file   src/System/basic_wstream.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   14 nov 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cerrno>
#include <limits>
#include <unistd.h>
#include "TFEL/Raise.hxx"
#include "TFEL/System/basic_wstream.hxx"
#include "TFEL/System/System.hxx"

namespace tfel {

  namespace system {

    void BlockingStreamWriter::write(int fd,
                                     const void *const buf,
                                     const size_t count) {
      auto start = static_cast<const char *>(buf);  //< some strange
                                                    //  warning of gcc
      size_t r = count;  //< remaining block to be written
      if (count > static_cast<size_t>(std::numeric_limits<ssize_t>::max())) {
        tfel::raise<SystemError>(
            "BlockingStreamReader::write: "
            "number of bytes to write too high");
      }
      while (r > 0) {
        ssize_t w = 0;  //< number of written blocks
        while ((w = ::write(fd, start, r)) == -1) {
          if (errno == EINTR) {
            continue;
          }
          if (errno != EAGAIN) {
            systemCall::throwSystemError(
                "BlockingStreamWriter::write: write failed", errno);
          }
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#pragma message("windows port")
#else
          sleep(1);
#endif
        }
        r = static_cast<size_t>(r - w);
        start += w;
      }

    }  // end of BlockingStreamWriter::write

    void NonBlockingStreamWriter::write(int fd,
                                        const void *const buf,
                                        const size_t count) {
      ssize_t rwrite;
      if (count > static_cast<size_t>(std::numeric_limits<ssize_t>::max())) {
        tfel::raise<SystemError>(
            "NonBlockingStreamReader::write: "
            "number of bytes to write too high");
      }
      rwrite = ::write(fd, buf, count);
      if (rwrite == -1) {
        systemCall::throwSystemError(
            "NonBlockingStreamWriter::write: "
            "write failed",
            errno);
      }
    }  // end of NonBlockingStreamWriter::write

  }  // end of namespace system

}  // end of namespace tfel
