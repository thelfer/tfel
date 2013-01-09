/*! 
 * \file  BinaryWrite.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 aoû 2010
 */

#include"TFEL/System/BinaryWrite.hxx"

#define TFEL_SYSTEM_BINARY_WRITE_SRC(X)                           \
    void binary_write(const int f,const X& v)                     \
    {                                                             \
      if(::write(f,static_cast<const void*>(&v),sizeof(X))==-1){  \
	systemCall::throwSystemError("binary_write<"#X">",errno); \
      }                                                           \
    }

namespace tfel
{

  namespace system
  {

    TFEL_SYSTEM_BINARY_WRITE_SRC(char)
    TFEL_SYSTEM_BINARY_WRITE_SRC(unsigned short)
    TFEL_SYSTEM_BINARY_WRITE_SRC(unsigned int)
    TFEL_SYSTEM_BINARY_WRITE_SRC(long unsigned int)
    TFEL_SYSTEM_BINARY_WRITE_SRC(short)
    TFEL_SYSTEM_BINARY_WRITE_SRC(int)
    TFEL_SYSTEM_BINARY_WRITE_SRC(long int)
    TFEL_SYSTEM_BINARY_WRITE_SRC(float)
    TFEL_SYSTEM_BINARY_WRITE_SRC(double)
    TFEL_SYSTEM_BINARY_WRITE_SRC(long double)

  } // end of namespace system

} // end of namespace tfel

