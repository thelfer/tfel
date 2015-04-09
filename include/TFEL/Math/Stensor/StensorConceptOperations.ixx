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
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,StensorConcept>::cond&&
      tfel::meta::Implements<T2,StensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpPlus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpPlus>::Handle
    >::type
    operator + (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpPlus>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,StensorConcept>::cond&&
      tfel::meta::Implements<T2,StensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMinus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMinus>::Handle
    >::type
    operator - (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMinus>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,StensorConcept>::cond&&
      tfel::meta::Implements<T2,StensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMult>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,StensorConcept>::cond&&
      tfel::meta::Implements<T2,StensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDiadicProduct>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiadicProduct>::Handle
    >::type
    operator ^ (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpDiadicProduct>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
      tfel::meta::Implements<T2,StensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1 a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMult>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,StensorConcept>::cond&&
      tfel::typetraits::IsScalar<T2>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1& a,const T2 b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMult>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,StensorConcept>::cond&&
      tfel::typetraits::IsScalar<T2>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDiv>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiv>::Handle
    >::type
    operator / (const T1& a,const T2 b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpDiv>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,StensorConcept>::cond,
      typename ComputeUnaryResult<T1,OpNeg>::Handle
    >::type
    operator - (const T1& a)
    {
      typedef typename ComputeUnaryResult<T1,OpNeg>::Handle Handle;
      return Handle(a);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,StensorConcept>::cond&&
      tfel::meta::Implements<T2,StensorConcept>::cond&&
      StensorTraits<T1>::dime==1u&&
      StensorTraits<T2>::dime==1u&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result
    >::type
    operator | (const T1& a, const T2& b)
    {
      return a(0)*b(0)+a(1)*b(1)+a(2)*b(2);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,StensorConcept>::cond&&
      tfel::meta::Implements<T2,StensorConcept>::cond&&
      StensorTraits<T1>::dime==2u&&
      StensorTraits<T2>::dime==2u&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result
    >::type
    operator | (const T1& a, const T2& b)
    {
      return a(0)*b(0)+a(1)*b(1)+a(2)*b(2)+a(3)*b(3);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,StensorConcept>::cond&&
      tfel::meta::Implements<T2,StensorConcept>::cond&&
      StensorTraits<T1>::dime==3u&&
      StensorTraits<T2>::dime==3u&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result
    >::type
    operator | (const T1& a, const T2& b)
    {
      return a(0)*b(0)+a(1)*b(1)+a(2)*b(2)
	+a(3)*b(3)+a(4)*b(4)+a(5)*b(5);
    }

    template<typename StensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<StensorType,StensorConcept>::cond&&
      StensorTraits<StensorType>::dime==1u,
      StensorExpr<stensor<1u,typename ComputeBinaryResult<typename StensorTraits<StensorType>::NumType,
							  typename StensorTraits<StensorType>::NumType,OpMult>::Result>,
		  StensorSquareExpr1D<StensorType> >
      >::type
    square(const StensorType& s)
    {
      return StensorExpr<stensor<1u,typename ComputeBinaryResult<typename StensorTraits<StensorType>::NumType,
								 typename StensorTraits<StensorType>::NumType,OpMult>::Result>,
			 StensorSquareExpr1D<StensorType> >(s);
    } // end of square

    template<typename StensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<StensorType,StensorConcept>::cond&&
      StensorTraits<StensorType>::dime==2u,
      StensorExpr<stensor<2u,typename ComputeBinaryResult<typename StensorTraits<StensorType>::NumType,
							  typename StensorTraits<StensorType>::NumType,OpMult>::Result>,
		  StensorSquareExpr2D<StensorType> >
      >::type
    square(const StensorType& s)
    {
      return StensorExpr<stensor<2u,typename ComputeBinaryResult<typename StensorTraits<StensorType>::NumType,
								 typename StensorTraits<StensorType>::NumType,OpMult>::Result>,
			 StensorSquareExpr2D<StensorType> >(s);
    } // end of square

    template<typename StensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<StensorType,StensorConcept>::cond&&
      StensorTraits<StensorType>::dime==3u,
      StensorExpr<stensor<3u,typename ComputeBinaryResult<typename StensorTraits<StensorType>::NumType,
							  typename StensorTraits<StensorType>::NumType,OpMult>::Result>,
		  StensorSquareExpr3D<StensorType> >
      >::type
    square(const StensorType& s)
    {
      return StensorExpr<stensor<3u,typename ComputeBinaryResult<typename StensorTraits<StensorType>::NumType,
								 typename StensorTraits<StensorType>::NumType,OpMult>::Result>,
			 StensorSquareExpr3D<StensorType> >(s);
    } // end of square


  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_STENSORCONCEPTOPERATIONS_IXX */

