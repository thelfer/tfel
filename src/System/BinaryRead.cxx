/*!
 * \file  src/System/BinaryRead.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 aoû 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <unistd.h>
#include "TFEL/System/BinaryRead.hxx"

#define TFEL_SYSTEM_BINARY_READER_SRC(X)                                \
  void BinaryReader<X>::exe(const int f, X& v) {                        \
    if (::read(f, static_cast<void*>(&v), sizeof(X)) == -1) {           \
      systemCall::throwSystemError("BinaryReader<" #X ">::exe", errno); \
    }                                                                   \
  }

namespace tfel {

  namespace system {

    TFEL_SYSTEM_BINARY_READER_SRC(bool)
    TFEL_SYSTEM_BINARY_READER_SRC(unsigned char)
    TFEL_SYSTEM_BINARY_READER_SRC(unsigned short)
    TFEL_SYSTEM_BINARY_READER_SRC(unsigned int)
    TFEL_SYSTEM_BINARY_READER_SRC(long unsigned int)
    TFEL_SYSTEM_BINARY_READER_SRC(char)
    TFEL_SYSTEM_BINARY_READER_SRC(short)
    TFEL_SYSTEM_BINARY_READER_SRC(int)
    TFEL_SYSTEM_BINARY_READER_SRC(long int)
    TFEL_SYSTEM_BINARY_READER_SRC(float)
    TFEL_SYSTEM_BINARY_READER_SRC(double)
    TFEL_SYSTEM_BINARY_READER_SRC(long double)

    void BinaryReader<void*>::exe(const int f, void*& v) {
      if (::read(f, static_cast<void*>(&v), sizeof(void*)) == -1) {
        systemCall::throwSystemError("BinaryReader<void*>::exe", errno);
      }
    }

  }  // end of namespace system

}  // end of namespace tfel
