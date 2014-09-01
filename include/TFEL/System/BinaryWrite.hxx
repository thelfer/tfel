/*! 
 * \file  include/TFEL/System/BinaryWrite.hxx
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

#ifndef _LIB_TFEL_SYSTEM_BINARYWRITE_H_
#define _LIB_TFEL_SYSTEM_BINARYWRITE_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/System/System.hxx"

#define TFEL_SYSTEM_BINARY_WRITE_DECLARATION(X) \
    /*!				   	        \
     * write a value in a stream	        \
     * \param [in] f : input stream	        \
     * \param [in] v : value read	        \
     */					        \
    TFELSYSTEM_VISIBILITY_EXPORT                \
    void binary_write(const int,const X&);

namespace tfel
{

  namespace system
  {

    /*!
     * an helper class to write value in a stream
     */
    template<typename T>
    struct BinaryWriter
    {
      /*!
       * write a value in a stream
       * \param [in] f : input stream
       * \param [in] v : value to be writtent
       */
      static void exe(const int f,
		      const T& v)
      {
	if(::write(f,static_cast<const void*>(&v),sizeof(T))==-1){
	  systemCall::throwSystemError("binary_write<T>",errno);
	}
      } // end of exe      
    }; // end of struct BinaryWriter

    /*!
     * write a value in a stream
     * \param [in] f : input stream
     * \param [in] v : value to be writtent
     */
    template<typename T>
    void binary_write(const int f,
		      const T& v)
    {
      BinaryWriter<T>::exe(f,v);
    } // end of binary_write

    /*!
     * partial specialisation for c-style string
     * \param N : number of characters
     */
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

    TFEL_SYSTEM_BINARY_WRITE_DECLARATION(char)
    TFEL_SYSTEM_BINARY_WRITE_DECLARATION(unsigned short)
    TFEL_SYSTEM_BINARY_WRITE_DECLARATION(unsigned int)
    TFEL_SYSTEM_BINARY_WRITE_DECLARATION(long unsigned int)
    TFEL_SYSTEM_BINARY_WRITE_DECLARATION(short)
    TFEL_SYSTEM_BINARY_WRITE_DECLARATION(int)
    TFEL_SYSTEM_BINARY_WRITE_DECLARATION(long int)
    TFEL_SYSTEM_BINARY_WRITE_DECLARATION(float)
    TFEL_SYSTEM_BINARY_WRITE_DECLARATION(double)
    TFEL_SYSTEM_BINARY_WRITE_DECLARATION(long double)

  } // end of namespace system

} // end of namespace tfel


#endif /* _LIB_TFEL_SYSTEM_BINARYWRITE_H */

