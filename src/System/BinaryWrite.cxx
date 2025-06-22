/*!
 * \file  src/System/BinaryWrite.cxx
 * \brief
 * \author Thomas Helfer
 * \date 10/08/2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <unistd.h>
#include "TFEL/System/BinaryWrite.hxx"

#define TFEL_SYSTEM_BINARY_WRITER_SRC(X)                             \
  void BinaryWriter<X>::exe(const int f, const X& v) {               \
    if (::write(f, static_cast<const void*>(&v), sizeof(X)) == -1) { \
      systemCall::throwSystemError("binary_write<" #X ">", errno);   \
    }                                                                \
  }

namespace tfel {

  namespace system {

    TFEL_SYSTEM_BINARY_WRITER_SRC(bool)
    TFEL_SYSTEM_BINARY_WRITER_SRC(unsigned char)
    TFEL_SYSTEM_BINARY_WRITER_SRC(unsigned short)
    TFEL_SYSTEM_BINARY_WRITER_SRC(unsigned int)
    TFEL_SYSTEM_BINARY_WRITER_SRC(long unsigned int)
    TFEL_SYSTEM_BINARY_WRITER_SRC(char)
    TFEL_SYSTEM_BINARY_WRITER_SRC(short)
    TFEL_SYSTEM_BINARY_WRITER_SRC(int)
    TFEL_SYSTEM_BINARY_WRITER_SRC(long int)
    TFEL_SYSTEM_BINARY_WRITER_SRC(float)
    TFEL_SYSTEM_BINARY_WRITER_SRC(double)
    TFEL_SYSTEM_BINARY_WRITER_SRC(long double)

    void BinaryWriter<const void*>::exe(const int f, const void* const v) {
      if (::write(f, static_cast<const void*>(&v), sizeof(void*)) == -1) {
        systemCall::throwSystemError("binary_write<void*>", errno);
      }
    }

  }  // end of namespace system

}  // end of namespace tfel
