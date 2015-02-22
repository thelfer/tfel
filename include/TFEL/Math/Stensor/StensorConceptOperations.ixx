/*!
 * \file   include/TFEL/Math/Stensor/StensorConceptOperations.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 fév 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_STENSORCONCEPTOPERATIONS_IXX_
#define _LIB_TFEL_STENSORCONCEPTOPERATIONS_IXX_ 

namespace tfel{

  namespace math{

    template<typename T1,typename T2>
    typename std::enable_if<
      tfel::meta::Implements<T1,StensorConcept>::cond&&
      tfel::meta::Implements<T2,StensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result
    >::type
    operator | (const T1& a, const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result Result;
      typedef typename ComputeBinaryResult<T1,T2,OpDotProduct>::Handle Handle;
      return Handle::template exe<Result,T1,T2>(a,b);
    }

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_STENSORCONCEPTOPERATIONS_IXX */

