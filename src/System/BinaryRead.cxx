/*! 
 * \file  src/System/BinaryRead.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 aoû 2010
 */

#include"TFEL/System/BinaryRead.hxx"

#define TFEL_SYSTEM_BINARY_READ_SRC(X)                                 \
    void binary_read(const int f,X& v)                                 \
    {                                                                  \
      if(::read(f,static_cast<void*>(&v),sizeof(X))==-1){              \
	systemCall::throwSystemError("BinaryReader<"#X">::exe",errno); \
      }                                                                \
    }

namespace tfel
{

  namespace system
  {

    TFEL_SYSTEM_BINARY_READ_SRC(char)
    TFEL_SYSTEM_BINARY_READ_SRC(unsigned short)
    TFEL_SYSTEM_BINARY_READ_SRC(unsigned int)
    TFEL_SYSTEM_BINARY_READ_SRC(long unsigned int)
    TFEL_SYSTEM_BINARY_READ_SRC(short)
    TFEL_SYSTEM_BINARY_READ_SRC(int)
    TFEL_SYSTEM_BINARY_READ_SRC(long int)
    TFEL_SYSTEM_BINARY_READ_SRC(float)
    TFEL_SYSTEM_BINARY_READ_SRC(double)
    TFEL_SYSTEM_BINARY_READ_SRC(long double)

  } // end of namespace system

} // end of namespace tfel
