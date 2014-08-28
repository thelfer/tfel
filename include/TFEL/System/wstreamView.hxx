/*!
 * \file   include/TFEL/System/wstreamView.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   18 nov 2007
 */

#ifndef _LIB_TFEL_WSTREAMVIEW_HXX_
#define _LIB_TFEL_WSTREAMVIEW_HXX_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/System/stream_traits.hxx"
#include"TFEL/System/basic_wstream.hxx"

namespace tfel
{
  
  namespace system
  {
    
    template<bool isBlocking>
    struct wstreamView;

    template<bool is>
    struct stream_traits<wstreamView<is> >
    {
      static const bool isBlocking = is;
    }; // end of struct stream_traits<wstreamView<isBlocking> >

    template<bool isBlocking>
    struct wstreamView
      : public basic_wstream<wstreamView<isBlocking>,
			     stream_traits<wstreamView<isBlocking> > >
    {

      wstreamView(const int);
      
      int
      getFileDescriptor(void) const;

    private:

      const int fd;

    }; // end of struct wstreamView

  } // end of namespace system  

} // end of namespace tfel  

#include"TFEL/System/wstreamView.ixx"

#endif /* _LIB_TFEL_WSTREAMVIEW_HXX */

