/*!
 * \file   StensorExport.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   31 Jul 2006
 */

#ifndef _LIB_TFEL_STENSOREXPORT_H_
#define _LIB_TFEL_STENSOREXPORT_H_ 

namespace tfel{

  namespace math {

    namespace internals{

      template<unsigned int N>
      struct ExportToTab;

      template<unsigned int N>
      struct ExportToVoigt;

    } //end of namespace internals
    
  } //end of namespace math

} // end of namespace tfel

#include"Math/Stensor/Internals/StensorExport.ixx"

#endif /* _LIB_TFEL_STENSOREXPORT_H */

