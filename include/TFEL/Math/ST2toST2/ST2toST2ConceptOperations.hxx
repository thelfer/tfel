/*!
 * \file   include/TFEL/Math/ST2toST2/ST2toST2ConceptOperations.hxx
 * \brief  This file implements operations that can be applied to st2tost2.
 * \author Helfer Thomas
 * \date   01 jui 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_ST2TOST2_CONCEPT_OPERATIONS_HXX_
#define _LIB_TFEL_ST2TOST2_CONCEPT_OPERATIONS_HXX_ 

#include <cmath>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/HasRandomAccessConstIterator.hxx"

#include"TFEL/TypeTraits/IsScalar.hxx"

#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/ComputeObjectTag.hxx"
#include"TFEL/Math/General/ComputeBinaryResult.hxx"
#include"TFEL/Math/General/ComputeUnaryResult.hxx"

#include"TFEL/Math/Stensor/StensorConcept.hxx"

#include"TFEL/Math/ExpressionTemplates/ScalarMathObjectExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/ScalarMathObjectExprWithoutConstIterator.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectMathObjectExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectMathObjectExprWithoutConstIterator.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectNegExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectNegExprWithoutConstIterator.hxx"

#include"TFEL/Math/ST2toST2/ST2toST2Expr.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2StensorProductExpr.hxx"
#include"TFEL/Math/ST2toST2/StensorST2toST2ProductExpr.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2ST2toST2ProductExpr.hxx"

namespace tfel
{

  namespace math
  {

    /*
     * Partial Specialisation of ComputeBinaryResult_ for st2tost2's operation
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<ST2toST2Tag,ST2toST2Tag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename ST2toST2Type<A>::type StensA;
      typedef typename ST2toST2Type<B>::type StensB;
      typedef typename std::conditional<tfel::meta::HasRandomAccessConstIterator<A>::cond&&
      tfel::meta::HasRandomAccessConstIterator<B>::cond,
				      MathObjectMathObjectExpr<ST2toST2Concept,ST2toST2Traits,
							       A,B,Op>,
				      MathObjectMathObjectExprWithoutConstIterator<ST2toST2Concept,ST2toST2Traits,
										   A,B,Op>
      >::type Expr;			    
    public:
      typedef typename ResultType<StensA,StensB,Op>::type Result;
      typedef typename std::conditional<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      ST2toST2Expr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for scalar-st2tost2 operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<ScalarTag,ST2toST2Tag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename ST2toST2Type<B>::type StensB;
      typedef typename std::conditional<tfel::meta::HasRandomAccessConstIterator<B>::cond,
				      ScalarMathObjectExpr<ST2toST2Concept,ST2toST2Traits,
							   A,B,Op>,
				      ScalarMathObjectExprWithoutConstIterator<ST2toST2Concept,ST2toST2Traits,
									       A,B,Op>
      >::type Expr;
    public:
      typedef typename ResultType<A,StensB,Op>::type Result;
      typedef typename std::conditional<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      ST2toST2Expr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for st2tost2-scalar operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<ST2toST2Tag,ScalarTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename ST2toST2Type<A>::type      StensA;
      typedef typename std::conditional<tfel::meta::HasRandomAccessConstIterator<A>::cond,
				      MathObjectScalarExpr<ST2toST2Concept,ST2toST2Traits,
							   A,B,Op>,
				      MathObjectScalarExprWithoutConstIterator<ST2toST2Concept,ST2toST2Traits,
									       A,B,Op>
      >::type Expr;
    public:
      typedef typename ResultType<StensA,B,Op>::type Result;
      typedef typename std::conditional<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      ST2toST2Expr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for st2tost2-st2tost2 operations
     */
    template<typename A, typename B>
    class ComputeBinaryResult_<ST2toST2Tag,ST2toST2Tag,A,B,OpMult>
    {
      struct DummyHandle{};
      typedef typename ST2toST2Type<A>::type ST2toST2A;
      typedef typename ST2toST2Type<B>::type ST2toST2B;
    public:
      typedef typename ResultType<ST2toST2A,ST2toST2B,OpMult>::type Result;
      typedef typename std::conditional<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      ST2toST2Expr<Result,ST2toST2ST2toST2ProductExpr<ST2toST2Traits<Result>::dime> > >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for st2tost2-stensor operations
     */
    template<typename A, typename B>
    class ComputeBinaryResult_<ST2toST2Tag,StensorTag,A,B,OpMult>
    {
      struct DummyHandle{};
      typedef typename ST2toST2Type<A>::type ST2toST2A;
      typedef typename StensorType<B>::type  StensB;
    public:
      typedef typename ResultType<ST2toST2A,StensB,OpMult>::type Result;
      typedef typename std::conditional<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      StensorExpr<Result,ST2toST2StensorProductExpr<StensorTraits<Result>::dime> > >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for stensor-st2tost2 operations
     */
    template<typename A, typename B>
    class ComputeBinaryResult_<StensorTag,ST2toST2Tag,A,B,OpMult>
    {
      struct DummyHandle{};
      typedef typename StensorType<A>::type  StensA;
      typedef typename ST2toST2Type<B>::type ST2toST2B;
    public:
      typedef typename ResultType<StensA,ST2toST2B,OpMult>::type Result;
      typedef typename std::conditional<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      StensorExpr<Result,StensorST2toST2ProductExpr<StensorTraits<Result>::dime> > >:: type Handle;
    };

    /*
     * Partial Specialisation of ComputeUnaryResult_ for st2tost2s
     */
    template<typename A>
    struct ComputeUnaryResult_<ST2toST2Tag,UnaryOperatorTag,A,OpNeg>
    {
      struct DummyHandle{};
      typedef typename ST2toST2Type<A>::type                    StensA;
      typedef typename ST2toST2Traits<A>::NumType               NumA;
      typedef typename ComputeUnaryResult<NumA,OpNeg>::Result  NumResult;
      typedef typename std::conditional<tfel::meta::HasRandomAccessConstIterator<A>::cond,
				      MathObjectNegExpr<ST2toST2Concept,ST2toST2Traits,A>,
				      MathObjectNegExprWithoutConstIterator<ST2toST2Concept,ST2toST2Traits,A>
      >::type Expr;
    public:
      typedef typename UnaryResultType<StensA,OpNeg>::type Result;
      typedef typename std::conditional<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      ST2toST2Expr<Result,Expr> >::type Handle;
    };

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename std::enable_if<
      tfel::meta::Implements<T1,ST2toST2Concept>::cond&&
    tfel::meta::Implements<T2,ST2toST2Concept>::cond&&
    !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpPlus>::Result>::cond,
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
      tfel::meta::Implements<T1,ST2toST2Concept>::cond&&
    tfel::meta::Implements<T2,ST2toST2Concept>::cond&&
    !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMinus>::Result>::cond,
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
      tfel::meta::Implements<T1,ST2toST2Concept>::cond&&
    tfel::meta::Implements<T2,ST2toST2Concept>::cond&&
    !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMult>::Handle Handle;
      return Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename std::enable_if<
      tfel::meta::Implements<T1,ST2toST2Concept>::cond&&
      tfel::meta::Implements<T2,StensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
      >::type
    operator * (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMult>::Handle Handle;
      return Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename std::enable_if<
      tfel::meta::Implements<T1,StensorConcept>::cond&&
      tfel::meta::Implements<T2,ST2toST2Concept>::cond&&
    !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator | (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMult>::Handle Handle;
      return Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename std::enable_if<
      tfel::typetraits::IsScalar<T1>::cond&&
    tfel::meta::Implements<T2,ST2toST2Concept>::cond&&
    !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1 a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMult>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename std::enable_if<
      tfel::meta::Implements<T1,ST2toST2Concept>::cond&&
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
    TFEL_MATH_INLINE 
    typename std::enable_if<
      tfel::meta::Implements<T1,ST2toST2Concept>::cond&&
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
    typename std::enable_if<
      tfel::meta::Implements<T1,ST2toST2Concept>::cond,
      typename ComputeUnaryResult<T1,OpNeg>::Handle
    >::type
    operator - (const T1& a)
    {
      typedef typename ComputeUnaryResult<T1,OpNeg>::Handle Handle;
      return Handle(a);
    }
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_ST2TOST2_CONCEPT_OPERATIONS_HXX */

