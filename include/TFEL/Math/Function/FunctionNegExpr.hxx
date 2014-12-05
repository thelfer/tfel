/*!
 * \file   include/TFEL/Math/Function/FunctionNegExpr.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   24 Aug 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/TypeTraits/IsInvalid.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/Function/FunctionConcept.hxx"

#ifndef _LIB_TFEL_FUNCTION_NEG_EXPR_H_
#define _LIB_TFEL_FUNCTION_NEG_EXPR_H_ 

namespace tfel{

  namespace math{

    template<typename F>
    class FunctionNegExpr;

    template<typename F>
    struct FunctionTraits<FunctionExpr<FunctionNegExpr<F> > >
    {
    private:
      typedef typename FunctionTraits<F>::DerivateFunc DF;
      typedef typename ComputeUnaryResult<DF,OpNeg>::Handle Res;
    public:
      typedef Res DerivateFunc;
    };

    template<typename F>
    class FunctionNegExpr
    {

      TFEL_STATIC_ASSERT((tfel::meta::Implements<F,FunctionConcept>::cond));

      const F a;

      struct invalid_argument;

      // Assignement operator (disabled)
      FunctionNegExpr&
      operator = (const FunctionNegExpr&);

      // Default Constructor (disabled)
      FunctionNegExpr();

    protected:

      typedef const F first_arg;
      typedef invalid_argument second_arg;

    public:

      FunctionNegExpr(const F a_)
	: a(a_)
      {}

      // Copy Construtor
      TFEL_MATH_INLINE
      FunctionNegExpr(const FunctionNegExpr& src)
	: a(src.a)
      {}

      template<typename T>
      TFEL_MATH_INLINE
      typename std::enable_if<
	!tfel::typetraits::IsInvalid<
	  typename ComputeUnaryResult<typename ComputeUnaryResult<T,F>::Result,OpNeg>::Result
        >::cond,
	typename ComputeUnaryResult<typename ComputeUnaryResult<T,F>::Handle,OpNeg>::Handle
      >::type
      operator()(const T& x) const
      {
	return -a(x);
      } // end of operator ()

      const F
      getFunc(void) const
      {
	return a;
      }

    };
    
    template<typename F>
    const typename FunctionTraits<FunctionExpr<FunctionNegExpr<F> > >::DerivateFunc
    derivate(const FunctionExpr<FunctionNegExpr<F> >);

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Function/FunctionNegExpr.ixx"

#endif /* _LIB_TFEL_FUNCTION_NEG_EXPR_H */

