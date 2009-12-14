/*!
 * \file   basic_rstream.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 nov 2007
 */

#ifndef _LIB_TFEL_BASIC_RSTREAM_HXX_
#define _LIB_TFEL_BASIC_RSTREAM_HXX_ 

#include<cctype>
#include<string>

namespace tfel
{

  namespace system
  {

    template<typename Child,typename Traits>
    struct basic_rstream
    {
      operator int (void);

      operator int (void) const;

      basic_rstream& 
      operator>>(char&);

      basic_rstream& 
      operator>>(bool&);
      
      basic_rstream& 
      operator>>(short&);
      
      basic_rstream& 
      operator>>(unsigned short&);

      basic_rstream& 
      operator>>(int&);
      
      basic_rstream& 
      operator>>(unsigned int&);

      basic_rstream& 
      operator>>(long&);
      
      basic_rstream& 
      operator>>(unsigned long&);

      basic_rstream& 
      operator>>(float&);

      basic_rstream& 
      operator>>(double&);

      basic_rstream& 
      operator>>(long double&);

      basic_rstream& 
      operator>>(void*&);

      void
      read(char* const,const size_t);

      void
      read(bool* const,const size_t);
      
      void
      read(short* const,const size_t);
      
      void
      read(unsigned short* const,const size_t);

      void
      read(int* const,const size_t);
      
      void
      read(unsigned int* const,const size_t);

      void
      read(long* const,const size_t);
      
      void
      read(unsigned long* const,const size_t);

      void
      read(float* const,const size_t);

      void
      read(double* const,const size_t);

      void
      read(long double* const,const size_t);

      const std::string
      getLine(void);
      
    protected :

      basic_rstream();

      basic_rstream(const basic_rstream&);

      basic_rstream&
      operator =(const basic_rstream&);

      static void
      read(int,void *const,const size_t);

    }; // end of struct basic_rstream

    struct BlockingStreamReader
    {
      static void read(int,void *const,const size_t);
    }; // end of BlockingStreamReader

    struct NonBlockingStreamReader
    {
      static void read(int,void *const,const size_t);
    }; // end of NonBlockingStreamReader
    
  } // end of namespace System  
  
} // end of namespace tfel  

#include"System/basic_rstream.ixx"
 
#endif /* _LIB_TFEL_BASIC_RSTREAM_HXX */

