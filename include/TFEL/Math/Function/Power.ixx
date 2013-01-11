/*!
 * \file   Power.ixx
 * \brief  This file implements methods of class Power.
 * \author Helfer Thomas
 * \date   04 Jan 2007
 */

#ifndef _LIB_TFEL_MATH_FUNCTION_POWER_I_
#define _LIB_TFEL_MATH_FUNCTION_POWER_I_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Math/Function/FunctionConcept.hxx"
#include"TFEL/Math/Function/Cst.hxx"

namespace tfel{

  namespace math{

    template<int N,unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<float,Power<N,D> >
    {
    public:
      typedef float type;
    };

    template<int N,unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<double,Power<N,D> >
    {
    public:
      typedef double type;
    };

    template<int N,unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<long double,Power<N,D> >
    {
    public:
      typedef long double type;
    };

    /*
     * Partial specialisaton for Power
     */
    template<int N,unsigned int D>
    struct TFEL_VISIBILITY_LOCAL FunctionTraits<Power<N,D> >
    {
    private:
      static const int N_ = FracSimplify<N-1,D>::N;
      static const int D_ = FracSimplify<N-1,D>::D;
      typedef Cst<N,D> Cst_;
      typedef Power<N_,D_> Pow_;
    public:
      typedef FunctionExpr<FunctionFunctionExpr<Cst_,Pow_,OpMult> > DerivateFunc;
    }; // end of FunctionTraits<Power<N,D> >
    
    /*
     * Partial specialisaton for Power
     */
    template<unsigned int D>
    struct TFEL_VISIBILITY_LOCAL FunctionTraits<Power<0,D> >
    {
      typedef Cst<0> DerivateFunc;
    }; // end of FunctionTraits<Power<N,D> >

    /*
     * Partial specialisaton for Power
     */
    template<>
    struct TFEL_VISIBILITY_LOCAL FunctionTraits<Power<1u,1u> >
    {
      typedef Cst<1u> DerivateFunc;
    }; // end of FunctionTraits<Power<N,D> >

    template<int N,unsigned int D>
    struct TFEL_VISIBILITY_LOCAL Power
      : public FunctionConcept<Power<N,D> >
    {

    private:

      TFEL_STATIC_ASSERT(D!=0);
      typedef typename PowerImplSelector<N,D>::type Implementation;

    public:

      template<typename T>
      static TFEL_MATH_INLINE
      typename UnaryResultType<T,Power<N,D> >::type
      exe(const T& x)
      {
	return Implementation::exe(x);
      }

      template<typename T>      			                                  
      static TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<                                                      
        !tfel::typetraits::IsInvalid<typename ComputeUnaryResult<T,Power<N,D> >::Result>::cond&&
      !tfel::typetraits::IsScalar<T>::cond,
	typename ComputeUnaryResult<T,Power<N,D> >::Handle                               
      >::type                                                                             
      exe(const T& v){                                                       
	typedef typename ComputeUnaryResult<T,Power<N,D> >::Handle Handle;                     
	return Handle(v,Power<N,D>());                                                           
      }                                                                                   

      template<typename T>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<                                                      
        !tfel::typetraits::IsInvalid<typename ComputeUnaryResult<T,Power<N,D> >::Result>::cond&&
	tfel::typetraits::IsScalar<T>::cond,
	typename ComputeUnaryResult<T,Power<N,D> >::Handle                               
      >::type                                                                             
      operator()(const T& x) const
      {
	using namespace tfel::math::stdfunctions;
	return power<N,D>(x);
      }

      template<typename T>      			                                  
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<                                                      
        !tfel::typetraits::IsInvalid<typename ComputeUnaryResult<T,Power<N,D> >::Result>::cond&&
      !tfel::typetraits::IsScalar<T>::cond,
	typename ComputeUnaryResult<T,Power<N,D> >::Handle                               
      >::type                                                                             
      operator()(const T& v) const{                                                       
	typedef typename ComputeUnaryResult<T,Power<N,D> >::Handle Handle;                     
	return Handle(v,*this);                                                           
      }                                                                                   

    }; // end of Power<N,D>

    namespace internals{

      template<int N,unsigned int D>
      class TFEL_VISIBILITY_LOCAL DerivatePower
      {
	static const int N_ = FracSimplify<N-1,D>::N;
	static const int D_ = FracSimplify<N-1,D>::D;
	typedef Cst<N,D> Cst_;
	typedef Power<N_,D_> Pow_;
	
      public:

	TFEL_MATH_INLINE2 static 
	typename FunctionTraits<Power<N,D> >::DerivateFunc
	exe(void)
	{
	  Cst<N,D> c;
	  Power<N_,D_> p;	  
	  return c*p;
	}

      }; // end of DerivatePower

      template<unsigned int D>
      struct TFEL_VISIBILITY_LOCAL DerivatePower<0,D>
      {
	TFEL_MATH_INLINE2 static Cst<0>
	exe(void)
	{
	  return Cst<0>();
	}
      }; // end of DerivatePower
      
      template<>
      struct TFEL_VISIBILITY_LOCAL DerivatePower<1u,1u>
      {
	TFEL_MATH_INLINE2 static Cst<1>
	exe(void)
	{
	  return Cst<1>();
	}
      }; // end of DerivatePower

    } // end of namespace internalss

    template<int N,unsigned int D>
    const typename FunctionTraits<Power<N,D> >::DerivateFunc
    TFEL_MATH_INLINE2 derivate(const Power<N,D>)
    {
      using namespace tfel::math::internals;
      return DerivatePower<N,D>::exe();
    }

    namespace stdfunctions{


      template<int N>
      TFEL_MATH_INLINE
      Power<N>
      power(void)
      {
	return Power<N>(); 
      }

      template<int N,typename T>      			                                  
      typename tfel::meta::EnableIf<                                                      
	!tfel::typetraits::IsInvalid<typename ComputeUnaryResult<T,Power<N> >::Result>::cond&&
	!tfel::typetraits::IsScalar<T>::cond,
	typename ComputeUnaryResult<T,Power<N> >::Handle                               
      >::type                                                                             
      power(const T& v)
      {                                                       
	typedef typename ComputeUnaryResult<T,Power<N> >::Handle Handle;                     
	return Handle(v,Power<N>());                                                           
      }

      template<int N,typename F>
      typename tfel::meta::EnableIf<                                                      
	tfel::meta::Implements<F,tfel::math::FunctionConcept>::cond,
	FunctionExpr<FunctionsCompositionExpr<tfel::math::Power<N,1u>,F> >
      >::type                                                                             
      power(const F f)
      {
	using namespace tfel::math;
	typedef FunctionsCompositionExpr<Power<N,1u>,F> Expr;
	return FunctionExpr<Expr>(Power<N,1u>(),f);
      }

      template<int N,unsigned int D>
      TFEL_MATH_INLINE
      Power<N,D>
      power(void)
      {
	return Power<N,D>(); 
      }

      template<int N,unsigned int D,typename T>
      typename tfel::meta::EnableIf<                                                      
	!tfel::typetraits::IsInvalid<
	typename tfel::math::ComputeUnaryResult<T,Power<N,D> >::Result>::cond&&
      !tfel::typetraits::IsScalar<T>::cond,
	typename tfel::math::ComputeUnaryResult<T,tfel::math::Power<N,D> >::Handle
      >::type                                                                             
      power(const T& v)
      {                                                       
	using namespace tfel::math;
	typedef typename ComputeUnaryResult<T,Power<N,D> >::Handle Handle;                     
	return Handle(v,Power<N,D>());                                                           
      }

      template<int N,unsigned int D,typename F>
      typename tfel::meta::EnableIf<                                                      
	tfel::meta::Implements<F,tfel::math::FunctionConcept>::cond,
	FunctionExpr<FunctionsCompositionExpr<tfel::math::Power<N,D>,F> >
      >::type                                                                             
      power(const F f)
      {
	using namespace tfel::math;
	typedef FunctionsCompositionExpr<Power<N,D>,F> Expr;
	return FunctionExpr<Expr>(Power<N,D>(),f);
      }

      extern TFELMATH_VISIBILITY_EXPORT tfel::math::FctSqrt sqrt;

    } // end of namespace stdfunctions
    
  } // end of namespace math

} // end of namespace tfel
     
#endif /* _LIB_TFEL_MATH_FUNCTION_POWER_I_ */
