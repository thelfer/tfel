/*!
 * \file   src/System/basic_rstream.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date 14/11/2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cerrno>
#include <cstring>
#include <limits>
#include <unistd.h>
#include "TFEL/Raise.hxx"
#include "TFEL/System/basic_rstream.hxx"
#include "TFEL/System/System.hxx"

namespace tfel::system {

  void BlockingStreamReader::read(int fd, void* const buf, const size_t count) {
    auto start = static_cast<char*>(buf);
    size_t r = count;  //< remaining block to be read
    if (count > static_cast<size_t>(std::numeric_limits<ssize_t>::max())) {
      tfel::raise<SystemError>(
          "BlockingStreamReader::read: "
          "number of bytes to be read too high");
    }
    while (r > 0) {
      ssize_t rread = 0;  //< number of read blocks
      while ((rread = ::read(fd, start, r)) == -1) {
        if (errno != EINTR) {
          systemCall::throwSystemError(
              "BlockingStreamReader::read: "
              "read failed",
              errno);
        }
      }
      if (rread == 0) {
        tfel::raise<SystemError>(
            "BlockingStreamReader::read: "
            "unexpected end of file");
      }
      r = static_cast<size_t>(r - rread);
      start += rread;
    }
  }  // end of BlockingStreamReader::read

  void NonBlockingStreamReader::read(int fd,
                                     void* const buf,
                                     const size_t count) {
    auto start = static_cast<char*>(buf);
    size_t r = count;  //< remaining block to be read
    if (count > static_cast<size_t>(std::numeric_limits<ssize_t>::max())) {
      tfel::raise<SystemError>(
          "NonBlockingStreamReader::read: "
          "number of bytes to be read too high");
    }
    while (r > 0) {
      ssize_t rread = 0;  //< number of read blocks
      while ((rread = ::read(fd, start, r)) == -1) {
        if (errno != EINTR) {
          systemCall::throwSystemError(
              "NonBlockingStreamReader::read: "
              "read failed",
              errno);
        }
      }
      if (rread == 0) {
        tfel::raise<SystemError>(
            "NonBlockingStreamReader::read: "
            "unexpected end of file");
      }
      r = static_cast<size_t>(r - rread);
      start += rread;
    }
  }  // end of NonBlockingStreamReader::read

}  // end of namespace tfel::system
