/*! 
 * \file  BinaryWrite.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 27 avr 2009
 */

#ifndef _LIB_BINARYWRITE_H_
#define _LIB_BINARYWRITE_H_ 

#include"TFEL/System/System.hxx"

namespace tfel
{

  namespace system
  {

    template<typename T>
    struct BinaryWriter
    {
      static void exe(const int f,
		      const T& v)
      {
	systemCall::write(f,static_cast<const void*>(&v),sizeof(T));
      } // end of BinaryWriter::exe
    }; // end of struct BinaryWriter

    template<typename T>
    void binary_write(const int f,
		      const T& v)
    {
      BinaryWriter<T>::exe(f,v);
    } // end of binary_write

    template<unsigned short N>
    struct BinaryWriter<char [N]>
    {
      static void exe(const int f,
		      const char v[N])
      {
	using namespace std;
	binary_write(f,N-1);
	systemCall::write(f,&v[0],(N-1)*sizeof(string::value_type));
      }
    }; // end of BinaryWriter

  } // end of namespace system

} // end of namespace tfel


#endif /* _LIB_BINARYWRITE_H */

