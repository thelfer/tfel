/*!
 * \file   LogicalOperations.hxx
 * \brief  This file implements the OpOr and OpAnd classes.
 * \author Helfer Thomas
 * \date   06 mai 2006
 */

#ifndef _LIB_TFEL_LOGICALOPERATIONS_HXX_
#define _LIB_TFEL_LOGICALOPERATIONS_HXX_ 

#include "Config/TFELConfig.hxx"

namespace tfel{

  namespace math {
    
    template<bool a, bool b>
    struct OpOr{
      static const bool cond = a || b; 
    };

    template<bool a, bool b>
    struct OpAnd{
      static const bool cond = a && b; 
    };

  }

}  

#endif /* _LIB_TFEL_LOGICALOPERATIONS_HXX */
