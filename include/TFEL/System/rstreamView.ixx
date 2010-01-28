/*!
 * \file   rstreamView.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   18 nov 2007
 */

#ifndef _LIB_TFEL_RSTREAMVIEW_IXX_
#define _LIB_TFEL_RSTREAMVIEW_IXX_ 

namespace tfel
{
  
  namespace system
  {
    
    template<bool isBlocking>
    TFEL_INLINE2
    rstreamView<isBlocking>::rstreamView(const int id)
      : fd(id)
    {} // end of rstreamView<isBlocking>::rstreamView
      
    template<bool isBlocking>
    TFEL_INLINE2
    int
    rstreamView<isBlocking>::getFileDescriptor(void) const
    {
      return this->fd;
    } // end of rstreamView<isBlocking>::getFileDescriptor

  } // end of namespace system  

} // end of namespace tfel  

#endif /* _LIB_TFEL_RSTREAMVIEW_IXX */

