/*!
 * \file  src/System/BinaryRead.cxx
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
#include "TFEL/System/BinaryRead.hxx"

#define TFEL_SYSTEM_BINARY_READER_SRC(X)                    \
  void BinaryReader<X>::exe(const int f, X& v) {            \
    systemCall::read(f, static_cast<void*>(&v), sizeof(X)); \
  }

namespace tfel::system {

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
    systemCall::read(f, static_cast<void*>(&v), sizeof(void*));
  }

}  // end of namespace tfel::system
