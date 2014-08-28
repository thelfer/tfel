/*!
 * \file   include/TFEL/System/wstreamView.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   18 nov 2007
 */

#ifndef _LIB_TFEL_WSTREAMVIEW_IXX_
#define _LIB_TFEL_WSTREAMVIEW_IXX_ 

namespace tfel
{
  
  namespace system
  {
    
    template<bool isBlocking>
    TFEL_INLINE2
    wstreamView<isBlocking>::wstreamView(const int id)
      : fd(id)
    {} // end of wstreamView<isBlocking>::wstreamView
      
    template<bool isBlocking>
    TFEL_INLINE2
    int
    wstreamView<isBlocking>::getFileDescriptor(void) const
    {
      return this->fd;
    } // end of wstreamView<isBlocking>::getFileDescriptor

  } // end of namespace system  

} // end of namespace tfel  

#endif /* _LIB_TFEL_WSTREAMVIEW_IXX */

