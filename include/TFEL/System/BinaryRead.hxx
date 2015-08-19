/*! 
 * \file  include/TFEL/System/BinaryRead.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 27 avr 2009
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_SYSTEM_BINARYREAD_H_
#define LIB_TFEL_SYSTEM_BINARYREAD_H_ 

#include <unistd.h>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/System/System.hxx"

#define TFEL_SYSTEM_BINARY_READ_DECLARATION(X) \
    /*!					       \
     * read a value in a stream		       \
     * \param [in]  f : input stream	       \
     * \param [out] v : value read	       \
     */					       \
    TFELSYSTEM_VISIBILITY_EXPORT               \
    void binary_read(const int,X&);


namespace tfel
{

  namespace system
  {
    
    /*!
     * an helper class to read value in a stream
     * \param T : type to be read
     */
    template<typename T>
    struct BinaryReader
    {
      /*!
       * read a value in a stream using basic binary read
       * \param [in]  f : input stream
       * \param [out] v : value read
       */
      static void exe(const int f,
		      T& v)
      {
	if(::read(f,static_cast<T*>(&v),sizeof(T))==-1){
	  systemCall::throwSystemError("BinaryReader<T>::exe",errno);
	}
      }
    };

    /*!
     * read a value in a stream
     * \param [in]  f : input stream
     * \param [out] v : value read
     */
    template<typename T>
    void
    binary_read(const int f,
		T& v)
    {
      BinaryReader<T>::exe(f,v);
    } // end of binary_read

    /*!
     * read a value in a stream
     * \param [in]  f : input stream
     * \return value read
     */
    template<typename T>
    T binary_read(const int f)
    {
      T res;
      binary_read(f,res);
      return res;
    } // end of binary_read

    TFEL_SYSTEM_BINARY_READ_DECLARATION(char)
    TFEL_SYSTEM_BINARY_READ_DECLARATION(unsigned short)
    TFEL_SYSTEM_BINARY_READ_DECLARATION(unsigned int)
    TFEL_SYSTEM_BINARY_READ_DECLARATION(long unsigned int)
    TFEL_SYSTEM_BINARY_READ_DECLARATION(short)
    TFEL_SYSTEM_BINARY_READ_DECLARATION(int)
    TFEL_SYSTEM_BINARY_READ_DECLARATION(long int)
    TFEL_SYSTEM_BINARY_READ_DECLARATION(float)
    TFEL_SYSTEM_BINARY_READ_DECLARATION(double)
    TFEL_SYSTEM_BINARY_READ_DECLARATION(long double)

  } // end of namespace system

} // end of namespace tfel

#endif /* LIB_TFEL_SYSTEM_BINARYREAD_H_ */

