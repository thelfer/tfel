/*!
 * \file   rstreamView.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   18 nov 2007
 */

#ifndef _LIB_TFEL_RSTREAMVIEW_HXX_
#define _LIB_TFEL_RSTREAMVIEW_HXX_ 

#include"Config/TFELConfig.hxx"
#include"System/stream_traits.hxx"
#include"System/basic_rstream.hxx"

namespace tfel
{
  
  namespace system
  {
    
    template<bool isBlocking>
    struct rstreamView;

    template<bool is>
    struct stream_traits<rstreamView<is> >
    {
      static const bool isBlocking = is;
    }; // end of struct stream_traits<rstreamView<isBlocking> >

    template<bool isBlocking>
    struct rstreamView
      : public basic_rstream<rstreamView<isBlocking>,
			     stream_traits<rstreamView<isBlocking> > >
    {

      rstreamView(const int);
      
      int
      getFileDescriptor(void) const;

    private:

      const int fd;

    }; // end of struct rstreamView

  } // end of namespace system  

} // end of namespace tfel  

#include"System/rstreamView.ixx"

#endif /* _LIB_TFEL_RSTREAMVIEW_HXX */

