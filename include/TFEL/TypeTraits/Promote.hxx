/*!
 * \file   include/TFEL/TypeTraits/Promote.hxx  
 * \brief  This file declares the Promote traits class.  
 * \author Helfer Thomas
 * \date   03 May 2006
 */

#ifndef _LIB_TFEL_PROMOTE_H_
#define _LIB_TFEL_PROMOTE_H_ 

#include <limits>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/Forward/Complex.hxx"

namespace tfel{
  
  namespace typetraits{

    /*!
     * \class Promote
     * \brief Returns the greater type able to contain the type A and
     * B.
     *
     * \param  A, the first  type  
     * \param  B, the second type  
     * \return type, the result
     *
     * \pre    A and B must fundamental numeric types
     * \see    IsFundamentalNumericType
     *
     * \author Helfer Thomas
     * \date   03 May 2006
     */
    template<typename A, typename B>
    struct Promote;
    
  } // end of namespace typetraits

} // end of namespace tfel

#include"TFEL/TypeTraits/Promote/Promote.ixx"

#endif /* _LIB_TFEL_PROMOTE_H */

