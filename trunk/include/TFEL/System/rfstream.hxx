/*!
 * \file   rfstream.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   14 nov 2007
 */

#ifndef _LIB_TFEL_RFSTREAM_HXX_
#define _LIB_TFEL_RFSTREAM_HXX_ 

#include<string>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/System/stream_traits.hxx"
#include"TFEL/System/basic_rstream.hxx"

namespace tfel
{

  namespace system
  {
    struct rfstream;

    template<>
    struct stream_traits<rfstream>
    {
      static const bool isBlocking = false;
    }; // end of stream_traits<rfstream>
    
    struct TFEL_VISIBILITY_EXPORT rfstream
      : public basic_rstream<rfstream,stream_traits<rfstream> >,
	protected tfel::utilities::shared_ptr<int>
    {

      // default constructor
      rfstream();

      // copy constructor
      rfstream(const rfstream&);

      // assignement operator
      rfstream &
      operator=(const rfstream&);

      rfstream(const std::string&,
	       const int=O_RDONLY);

      void
      open(const std::string&,
	   const int=O_RDONLY);

      void
      close(void);

      int
      getFileDescriptor(void) const;

      // destructor
      ~rfstream();

    }; // end of struct basic_wstream
    
  } // end of namespace system  
  
} // end of namespace tfel  
  
#endif /* _LIB_TFEL_RFSTREAM_HXX */
