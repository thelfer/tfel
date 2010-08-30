/*! 
 * \file  BinaryRead.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 27 avr 2009
 */

#ifndef _LIB_TFEL_SYSTEM_BINARYREAD_H_
#define _LIB_TFEL_SYSTEM_BINARYREAD_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/System/System.hxx"

#define TFEL_SYSTEM_BINARY_READ_DECLARATION(X) \
    /*!					       \
     * read a value in a stream		       \
     * \param [in]  f : input stream	       \
     * \param [out] v : value read	       \
     */					       \
    TFEL_VISIBILITY_EXPORT                     \
    void binary_read(const int,X&);


namespace tfel
{

  namespace system
  {
    
    /*!
     * an helper class to read value in a stream
     */
    template<typename T>
    struct BinaryReader
    {
      /*!
       * read a value in a stream
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

#endif /* _LIB_TFEL_SYSTEM_BINARYREAD_H */

