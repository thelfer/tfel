/*! 
 * \file  BinaryRead.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 27 avr 2009
 */

#ifndef _LIB_BINARYREAD_H_
#define _LIB_BINARYREAD_H_ 

#include"TFEL/System/System.hxx"

namespace tfel
{

  namespace system
  {
    
    template<typename T>
    struct BinaryReader
    {
      static void exe(const int f,T& res)
      {
	if(::read(f,static_cast<void*>(&res),sizeof(T))==1){
	  systemCall::throwSystemError("BinaryReader<T>::exe",errno);
	}
      }
    };

    template<typename T>
    void
    binary_read(const int f,
		T& res)
    {
      BinaryReader<T>::exe(f,res);
    } // end of binary_read

    template<typename T>
    T binary_read(const int f)
    {
      T res;
      BinaryReader<T>::exe(f,res);
      return res;
    } // end of binary_read

  } // end of namespace system

} // end of namespace tfel

#endif /* _LIB_BINARYREAD_H */

