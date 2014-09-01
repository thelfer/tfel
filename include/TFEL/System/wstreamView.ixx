/*!
 * \file   include/TFEL/System/wstreamView.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   18 nov 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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

