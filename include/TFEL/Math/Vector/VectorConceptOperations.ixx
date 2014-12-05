/*!
 * \file   include/TFEL/Math/Vector/VectorConceptOperations.ixx
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

#ifndef _LIB_TFEL_VECTORCONCEPTOPERATIONS_IXX_
#define _LIB_TFEL_VECTORCONCEPTOPERATIONS_IXX_ 

namespace tfel{

  namespace math{

    template<typename T1,typename T2>
    TFEL_MATH_INLINE
    typename std::enable_if<
      IsVectorVectorOperationValid<T1,T2,OpPlus>::cond,
      typename ComputeBinaryResult<T1,T2,OpPlus>::Handle
    >::type
    operator + (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpPlus>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename std::enable_if<
      IsVectorVectorOperationValid<T1,T2,OpMinus>::cond,
      typename ComputeBinaryResult<T1,T2,OpMinus>::Handle
    >::type
    operator - (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMinus>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename std::enable_if<
      IsVectorVectorOperationValid<T1,T2,OpMult>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMult>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename std::enable_if<
      IsVectorVectorOperationValid<T1,T2,OpDiv>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiv>::Handle
    >::type
    operator / (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpDiv>::Handle Handle;
      return  Handle(a,b);
    }
    
    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename std::enable_if<
      IsScalarVectorOperationValid<T1,T2,OpMult>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1 a, const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMult>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename std::enable_if<
      IsVectorVectorOperationValid<T1,T2,OpDiadicProduct>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiadicProduct>::Handle
    >::type
    operator ^ (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpDiadicProduct>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename std::enable_if<
      IsVectorScalarOperationValid<T1,T2,OpMult>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1& a,const T2 b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMult>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename std::enable_if<
      IsVectorScalarOperationValid<T1,T2,OpDiv>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiv>::Handle
    >::type
    operator / (const T1& a,const T2 b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpDiv>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1>
    TFEL_MATH_INLINE 
    typename std::enable_if<
      tfel::meta::Implements<T1,VectorConcept>::cond,
      typename ComputeUnaryResult<T1,OpNeg>::Handle
    >::type
    operator - (const T1& a)
    {
      typedef typename ComputeUnaryResult<T1,OpNeg>::Handle Handle;
      return Handle(a);
    }

    template<typename T1,typename T2>
    typename std::enable_if<
      IsVectorVectorOperationValid<T1,T2,OpDotProduct>::cond,
      typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result
    >::type
    operator | (const T1& a, const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result Result;
      typedef typename ComputeBinaryResult<T1,T2,OpDotProduct>::Handle Handle;
      return Handle::template exe<Result,T1,T2>(a,b);
    }

    template<typename T1>
    typename std::enable_if<
      IsEuclidianNormValid<T1>::cond,
      typename tfel::typetraits::RealPartType<typename ComputeBinaryResult<T1,T1,OpDotProduct>::Result>::type
    >::type
    norm(const T1& v)
    {
      using namespace std;
      return sqrt(real(v|v));
    }

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_VECTORCONCEPTOPERATIONS_IXX */

