/*!
 * \file   FracSimplify.hxx
 * \brief  This file declares the FracSimplify metafunction.
 * \author Helfer Thomas
 * \date   18 Jul 2006
 */

#ifndef _LIB_TFEL_FRACSIMPLIFY_H_
#define _LIB_TFEL_FRACSIMPLIFY_H_ 

namespace tfel{

  namespace math{

    template<int N1, unsigned int D1>
    struct FracSimplify;
    
  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Quantity/FracSimplify.ixx"

#endif /* _LIB_TFEL_FRACSIMPLIFY_H */

