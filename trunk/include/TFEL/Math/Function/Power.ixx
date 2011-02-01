/*!
 * \file   Power.ixx
 * \brief  This file implements methods of class Power.
 * \author Helfer Thomas
 * \date   04 Jan 2007
 */

#ifndef _LIB_TFEL_POWER_I_
#define _LIB_TFEL_POWER_I_ 

// #include <string>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/IF.hxx"
// #include"TFEL/Utilities/ToString.hxx"
#include"TFEL/Math/Function/FunctionConcept.hxx"
#include"TFEL/Math/Function/Cst.hxx"

namespace tfel{

  namespace math{

    template<int N,unsigned int D>
    struct TFEL_VISIBILITY_LOCAL PowerGenerator
    {
      static TFEL_MATH_INLINE
      float
      exe(const float x)
      {
	return std::pow(x,static_cast<float>(N)/D);
      }

      static TFEL_MATH_INLINE
      double
      exe(const double x)
      {
	return ::pow(x,static_cast<double>(N)/D);
      }

      static TFEL_MATH_INLINE
      long double
      exe(const long double x)
      {
	return std::pow(x,static_cast<long double>(N)/D);
      }

      static TFEL_MATH_INLINE
      Complex<float>
      exe(const Complex<float> x)
      {
	return std::pow(x,static_cast<Complex<float> >(N)/static_cast<Complex<float> >(D));
      }

      static TFEL_MATH_INLINE
      Complex<double>
      exe(const Complex<double> x)
      {
	return std::pow(x,static_cast<Complex<double> >(N)/static_cast<Complex<double> >(D));
      }

      static TFEL_MATH_INLINE
      Complex<long double>
      exe(const Complex<long double> x)
      {
	typedef long double LDouble;
	return std::pow(x,static_cast<Complex<LDouble> >(N)/static_cast<Complex<LDouble> >(D));
      }

    };

    template<unsigned int N,
	     unsigned int M>
    class PowerPosImpl;

    template<unsigned int N>
    class TFEL_VISIBILITY_LOCAL PowerPos
      : public PowerPosImpl<N/4,N%4>
    {};

    template<unsigned int N,
	     unsigned int M>
    struct TFEL_VISIBILITY_LOCAL PowerPosImpl
    {  
      static TFEL_MATH_INLINE
      float
      exe(const float x)
      {
	float tmp  = PowerPos<N>::exe(x);
	float tmp2 = PowerPos<M>::exe(x); 
	return tmp*tmp*tmp*tmp*tmp2;
      }

      static TFEL_MATH_INLINE
      double
      exe(const double x)
      {
	double tmp  = PowerPos<N>::exe(x);
	double tmp2 = PowerPos<M>::exe(x); 
	return tmp*tmp*tmp*tmp*tmp2;
      }

      static TFEL_MATH_INLINE
      long double
      exe(const long double x)
      {
	double tmp  = PowerPos<N>::exe(x);
	double tmp2 = PowerPos<M>::exe(x); 
	return tmp*tmp*tmp*tmp*tmp2;
      }

      static TFEL_MATH_INLINE
      Complex<float>
      exe(const Complex<float> x)
      {
	Complex<float> tmp  = PowerPos<N>::exe(x);
	Complex<float> tmp2 = PowerPos<M>::exe(x); 
	return tmp*tmp*tmp*tmp*tmp2;
      }

      static TFEL_MATH_INLINE
      Complex<double>
      exe(const Complex<double> x)
      {
	Complex<double> tmp  = PowerPos<N>::exe(x);
	Complex<double> tmp2 = PowerPos<M>::exe(x); 
	return tmp*tmp*tmp*tmp*tmp2;
      }

      static TFEL_MATH_INLINE
      Complex<long double>
      exe(const Complex<long double> x)
      {
	Complex<long double> tmp  = PowerPos<N>::exe(x);
	Complex<long double> tmp2 = PowerPos<M>::exe(x); 
	return tmp*tmp*tmp*tmp*tmp2;
      }

    };

    template<unsigned int N>
    struct TFEL_VISIBILITY_LOCAL PowerPosImpl<N,0u>
    {  
      static TFEL_MATH_INLINE
      float
      exe(const float x)
      {
	float tmp  = PowerPos<N>::exe(x);
	return tmp*tmp*tmp*tmp;
      }

      static TFEL_MATH_INLINE
      double
      exe(const double x)
      {
	double tmp  = PowerPos<N>::exe(x);
	return tmp*tmp*tmp*tmp;
      }

      static TFEL_MATH_INLINE
      long double
      exe(const long double x)
      {
	long double tmp  = PowerPos<N>::exe(x);
	return tmp*tmp*tmp*tmp;
      }

      static TFEL_MATH_INLINE
      Complex<float>
      exe(const Complex<float> x)
      {
	Complex<float> tmp  = PowerPos<N>::exe(x);
	return tmp*tmp*tmp*tmp;
      }

      static TFEL_MATH_INLINE
      Complex<double>
      exe(const Complex<double> x)
      {
	Complex<double> tmp  = PowerPos<N>::exe(x);
	return tmp*tmp*tmp*tmp;
      }

      static TFEL_MATH_INLINE
      Complex<long double>
      exe(const Complex<long double> x)
      {
	Complex<long double> tmp  = PowerPos<N>::exe(x);
	return tmp*tmp*tmp*tmp;
      }

    };

    template<unsigned int M>
    struct TFEL_VISIBILITY_LOCAL PowerPosImpl<0u,M>
    {  
      static TFEL_MATH_INLINE
      float
      exe(const float x)
      {
	return PowerPos<M>::exe(x);
      }

      static TFEL_MATH_INLINE
      double
      exe(const double x)
      {
	return PowerPos<M>::exe(x);
      }

      static TFEL_MATH_INLINE
      long double
      exe(const long double x)
      {
	return PowerPos<M>::exe(x);
      }

      static TFEL_MATH_INLINE
      Complex<float>
      exe(const Complex<float> x)
      {
	return PowerPos<M>::exe(x);
      }

      static TFEL_MATH_INLINE
      Complex<double>
      exe(const Complex<double> x)
      {
	return PowerPos<M>::exe(x);
      }

      static TFEL_MATH_INLINE
      Complex<long double>
      exe(const Complex<long double> x)
      {
	return PowerPos<M>::exe(x);
      }

    };

    template<>
    struct TFEL_VISIBILITY_LOCAL PowerPosImpl<0u,0u>
    {  
      static TFEL_MATH_INLINE
      float
      exe(const float)
      {
	return 1.f;
      }

      static TFEL_MATH_INLINE
      double
      exe(const double)
      {
	return 1.;
      }

      static TFEL_MATH_INLINE
      long double
      exe(const long double)
      {
	return 1.;
      }

      static TFEL_MATH_INLINE
      Complex<float>
      exe(const Complex<float>)
      {
	return Complex<float>(1.f);
      }

      static TFEL_MATH_INLINE
      Complex<double>
      exe(const Complex<double>)
      {
	return Complex<double>(1.);
      }

      static TFEL_MATH_INLINE
      Complex<long double>
      exe(const Complex<long double>)
      {
	return Complex<long double>(1.);
      }
      
    };

    template<>
    struct TFEL_VISIBILITY_LOCAL PowerPos<0u>
    {
      static TFEL_MATH_INLINE
      float
      exe(const float)
      {
	return 1.f;
      }

      static TFEL_MATH_INLINE
      double
      exe(const double)
      {
	return 1.;
      }

      static TFEL_MATH_INLINE
      long double
      exe(const long double)
      {
	return 1.;
      }

      static TFEL_MATH_INLINE
      Complex<float>
      exe(const Complex<float>)
      {
	return 1.f;
      }

      static TFEL_MATH_INLINE
      Complex<double>
      exe(const Complex<double>)
      {
	return 1.;
      }

      static TFEL_MATH_INLINE
      Complex<long double>
      exe(const Complex<long double>)
      {
	return 1.;
      }

    };

    template<>
    struct TFEL_VISIBILITY_LOCAL PowerPos<1u>
    {
      static TFEL_MATH_INLINE
      float
      exe(const float x)
      {
	return x;
      }

      static TFEL_MATH_INLINE
      double
      exe(const double x)
      {
	return x;
      }

      static TFEL_MATH_INLINE
      long double
      exe(const long double x)
      {
	return x;
      }

      static TFEL_MATH_INLINE
      Complex<float>
      exe(const Complex<float> x)
      {
	return x;
      }

      static TFEL_MATH_INLINE
      Complex<double>
      exe(const Complex<double> x)
      {
	return x;
      }

      static TFEL_MATH_INLINE
      Complex<long double>
      exe(const Complex<long double> x)
      {
	return x;
      }

    };

    template<>
    struct TFEL_VISIBILITY_LOCAL PowerPos<2u>
    {
      static TFEL_MATH_INLINE
      float
      exe(const float x)
      {
	return x*x;
      }

      static TFEL_MATH_INLINE
      double
      exe(const double x)
      {
	return x*x;
      }

      static TFEL_MATH_INLINE
      long double
      exe(const long double x)
      {
	return x*x;
      }

      static TFEL_MATH_INLINE
      Complex<float>
      exe(const Complex<float> x)
      {
	return x*x;
      }

      static TFEL_MATH_INLINE
      Complex<double>
      exe(const Complex<double> x)
      {
	return x*x;
      }

      static TFEL_MATH_INLINE
      Complex<long double>
      exe(const Complex<long double> x)
      {
	return x*x;
      }

    };
  
    template<>
    struct TFEL_VISIBILITY_LOCAL PowerPos<3u>
    {
      static TFEL_MATH_INLINE
      float
      exe(const float x)
      {
	return x*x*x;
      }

      static TFEL_MATH_INLINE
      double
      exe(const double x)
      {
	return x*x*x;
      }

      static TFEL_MATH_INLINE
      long double
      exe(const long double x)
      {
	return x*x*x;
      }

      static TFEL_MATH_INLINE
      Complex<float>
      exe(const Complex<float> x)
      {
	return x*x*x;
      }

      static TFEL_MATH_INLINE
      Complex<double>
      exe(const Complex<double> x)
      {
	return x*x*x;
      }

      static TFEL_MATH_INLINE
      Complex<long double>
      exe(const Complex<long double> x)
      {
	return x*x*x;
      }

    };

    template<int N>
    struct TFEL_VISIBILITY_LOCAL PowerNeg
    {
      static TFEL_MATH_INLINE
      float
      exe(const float x)
      {
	return PowerPos<-N>::exe(1.f/x);
      }

      static TFEL_MATH_INLINE
      double
      exe(const double x)
      {
	return PowerPos<-N>::exe(1./x);
      }

      static TFEL_MATH_INLINE
      long double
      exe(const long double x)
      {
	return PowerPos<-N>::exe(1./x);
      }

      static TFEL_MATH_INLINE
      Complex<float>
      exe(const Complex<float> x)
      {
	return PowerPos<-N>::exe(1.f/x);
      }

      static TFEL_MATH_INLINE
      Complex<double>
      exe(const Complex<double> x)
      {
	return PowerPos<-N>::exe(1./x);
      }

      static TFEL_MATH_INLINE
      Complex<long double>
      exe(const Complex<long double> x)
      {
	return PowerPos<-N>::exe(1.l/x);
      }

    };

    template<int N>
    struct TFEL_VISIBILITY_LOCAL PowerSqrtNeg
    {
      static TFEL_MATH_INLINE
      float
      exe(const float x)
      {
	return std::sqrt(PowerPos<-N>::exe(1.f/x));
      }

      static TFEL_MATH_INLINE
      double
      exe(const double x)
      {
	return std::sqrt(PowerPos<-N>::exe(1./x));
      }

      static TFEL_MATH_INLINE
      long double
      exe(const long double x)
      {
	return std::sqrt(PowerPos<-N>::exe(1./x));
      }

      static TFEL_MATH_INLINE
      Complex<float>
      exe(const Complex<float> x)
      {
	return std::sqrt(PowerPos<-N>::exe(1.f/x));
      }

      static TFEL_MATH_INLINE
      Complex<double>
      exe(const Complex<double> x)
      {
	return std::sqrt(PowerPos<-N>::exe(1./x));
      }

      static TFEL_MATH_INLINE
      Complex<long double>
      exe(const Complex<long double> x)
      {
	return std::sqrt(PowerPos<-N>::exe(1.l/x));
      }

    };

    template<int N>
    class TFEL_VISIBILITY_LOCAL PowerSqrtPos
    {

      TFEL_STATIC_ASSERT(N>=0);

    public:

      static TFEL_MATH_INLINE
      float
      exe(const float x)
      {
	return std::sqrt(PowerPos<N>::exe(x));
      }

      static TFEL_MATH_INLINE
      double
      exe(const double x)
      {
	return std::sqrt(PowerPos<N>::exe(x));
      }

      static TFEL_MATH_INLINE
      long double
      exe(const long double x)
      {
	return std::sqrt(PowerPos<N>::exe(x));
      }

      static TFEL_MATH_INLINE
      Complex<float>
      exe(const Complex<float> x)
      {
	return std::sqrt(PowerPos<N>::exe(x));
      }

      static TFEL_MATH_INLINE
      Complex<double>
      exe(const Complex<double> x)
      {
	return std::sqrt(PowerPos<N>::exe(x));
      }

      static TFEL_MATH_INLINE
      Complex<long double>
      exe(const Complex<long double> x)
      {
	return std::sqrt(PowerPos<N>::exe(x));
      }

    };

    template<int N,unsigned int D>
    class PowerImplSelector;

    template<int N>
    class TFEL_VISIBILITY_LOCAL PowerImplSelector<N,1u>
    {
      static const unsigned short AbsN = (N<0) ? -N : N;

      typedef typename tfel::meta::IF<(N<0),PowerNeg<N>,
	PowerPos<AbsN> >::type PowerImplSpe;

    public:

      typedef typename tfel::meta::IF<(AbsN>100),PowerGenerator<N,1u>,
				      PowerImplSpe>::type type;

    };

    template<int N>
    class TFEL_VISIBILITY_LOCAL PowerImplSelector<N,2u>
    {
      static const unsigned short AbsN = (N<0) ? -N : N;

      typedef typename tfel::meta::IF<(N<0),PowerSqrtNeg<N>,
	PowerSqrtPos<N> >::type PowerImplSpe;

    public:

      typedef typename tfel::meta::IF<(AbsN>100),PowerGenerator<N,2u>,
				      PowerImplSpe>::type type;
    };

    template<int N,unsigned int D>
    class TFEL_VISIBILITY_LOCAL PowerImplSelector
    {
      TFEL_STATIC_ASSERT(D!=0);

      static const int N_ = FracSimplify<N,D>::N;
      static const int D_ = FracSimplify<N,D>::D;

    public:
      
      // On fait un choix sur les valeurs de D
      typedef typename 
      tfel::meta::IF<D_==1u,
		     typename PowerImplSelector<N_,1u>::type,
		     typename tfel::meta::IF<D_==2u,
					     typename PowerImplSelector<N_,2u>::type,
					     PowerGenerator<N_,D_>
      >::type
      >::type type;

    };

    /*
     * Partial specialisaton for Power
     */
    template<int N,unsigned int D>
    class TFEL_VISIBILITY_LOCAL FunctionTraits<Power<N,D> >
    {
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
    class TFEL_VISIBILITY_LOCAL FunctionTraits<Power<0,D> >
    {
    public:
      typedef Cst<0> DerivateFunc;
    }; // end of FunctionTraits<Power<N,D> >

    /*
     * Partial specialisaton for Power
     */
    template<>
    class TFEL_VISIBILITY_LOCAL FunctionTraits<Power<1u,1u> >
    {
    public:
      typedef Cst<1u> DerivateFunc;
    }; // end of FunctionTraits<Power<N,D> >

    template<int N,unsigned int D>
    class TFEL_VISIBILITY_LOCAL Power
      : public FunctionConcept<Power<N,D> >
    {

      TFEL_STATIC_ASSERT(D!=0);
      typedef typename PowerImplSelector<N,D>::type Implementation;

    public:

//       /*!						                                  
//        * \brief  Return the name of the class.		                                  
//        * \param  void.					                                  
//        * \return std::string, the name of the class.	                                  
//        * \see    Name.					                                  
//        */						                                  
//       static
//       std::string getName(void)
//       {
// 	using namespace std;
// 	using namespace tfel::utilities;
// 	return string("Power<")+ToString(N)+string(">");
//       }                                             

      static TFEL_MATH_INLINE
      float
      exe(const float x)
      {
	return Implementation::exe(x);
      }

      static TFEL_MATH_INLINE
      double
      exe(const double x)
      {
	return Implementation::exe(x);
      }

      static TFEL_MATH_INLINE
      long double
      exe(const long double x) 
      {
	return Implementation::exe(x);
      }

      static TFEL_MATH_INLINE
      Complex<float>
      exe(const Complex<float> x) 
      {
	return Implementation::exe(x);
      }

      static TFEL_MATH_INLINE
      Complex<double>
      exe(const Complex<double> x)
      {
	return Implementation::exe(x);
      }

      static TFEL_MATH_INLINE
      Complex<long double>
      exe(const Complex<long double> x)
      {
	return Implementation::exe(x);
      }

      template<typename Unit>
      static TFEL_MATH_INLINE
      qt<typename PowUnit_<N,D,Unit>::type,float> 
      exe(const qt<Unit,float> x)
      {
	typedef qt<typename PowUnit_<N,D,Unit>::type,float> Res;
	return Res(Implementation::exe(x.getValue()));
      }
      
      template<typename Unit>
      static TFEL_MATH_INLINE
      qt<typename PowUnit_<N,D,Unit>::type,double>
      exe(const qt<Unit,double> x)
      {
	typedef qt<typename PowUnit_<N,D,Unit>::type,double> Res;
	return Res(Implementation::exe(x.getValue()));
      }
      
      template<typename Unit>
      static TFEL_MATH_INLINE
      qt<typename PowUnit_<N,D,Unit>::type,long double>
      exe(const qt<Unit,long double> x)
      {
	typedef qt<typename PowUnit_<N,D,Unit>::type,long double> Res;
	return Res(Implementation::exe(x.getValue()));
      }
      
      template<typename Unit>
      static TFEL_MATH_INLINE
      qt<typename PowUnit_<N,D,Unit>::type,Complex<float> > 
      exe(const qt<Unit,Complex<float> > x)
      {
	typedef qt<typename PowUnit_<N,D,Unit>::type,Complex<float> > Res;
	return Res(Implementation::exe(x.getValue()));
      }
      
      template<typename Unit>
      static TFEL_MATH_INLINE
      qt<typename PowUnit_<N,D,Unit>::type,Complex<double> >
      exe(const qt<Unit,Complex<double> > x)
      {
	typedef qt<typename PowUnit_<N,D,Unit>::type,Complex<double> > Res;
	return Res(Implementation::exe(x.getValue()));
      }
      
      template<typename Unit>
      static TFEL_MATH_INLINE
      qt<typename PowUnit_<N,D,Unit>::type,Complex<long double> > 
      exe(const qt<Unit,Complex<long double> > x){
	typedef qt<typename PowUnit_<N,D,Unit>::type,Complex<long double> > Res;
	return Res(Implementation::exe(x.getValue()));
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

      TFEL_MATH_INLINE
      float
      operator()(const float x) const
      {
	return Implementation::exe(x);
      }

      TFEL_MATH_INLINE
      double
      operator()(const double x) const
      {
	return Implementation::exe(x);
      }

      TFEL_MATH_INLINE
      long double
      operator()(const long double x) const 
      {
	return Implementation::exe(x);
      }

      TFEL_MATH_INLINE
      Complex<float>
      operator()(const Complex<float> x) const 
      {
	return Implementation::exe(x);
      }

      TFEL_MATH_INLINE
      Complex<double>
      operator()(const Complex<double> x) const
      {
	return Implementation::exe(x);
      }

      TFEL_MATH_INLINE
      Complex<long double>
      operator()(const Complex<long double> x) const
      {
	return Implementation::exe(x);
      }

      template<typename Unit>
      TFEL_MATH_INLINE
      qt<typename PowUnit_<N,D,Unit>::type,float> 
      operator()(const qt<Unit,float> x) const
      {
	typedef qt<typename PowUnit_<N,D,Unit>::type,float> Res;
	return Res(Implementation::exe(x.getValue()));
      }
      
      template<typename Unit>
      TFEL_MATH_INLINE
      qt<typename PowUnit_<N,D,Unit>::type,double>
      operator()(const qt<Unit,double> x) const
      {
	typedef qt<typename PowUnit_<N,D,Unit>::type,double> Res;
	return Res(Implementation::exe(x.getValue()));
      }
      
      template<typename Unit>
      TFEL_MATH_INLINE
      qt<typename PowUnit_<N,D,Unit>::type,long double>
      operator()(const qt<Unit,long double> x) const
      {
	typedef qt<typename PowUnit_<N,D,Unit>::type,long double> Res;
	return Res(Implementation::exe(x.getValue()));
      }
      
      template<typename Unit>
      TFEL_MATH_INLINE
      qt<typename PowUnit_<N,D,Unit>::type,Complex<float> > 
      operator()(const qt<Unit,Complex<float> > x) const
      {
	typedef qt<typename PowUnit_<N,D,Unit>::type,Complex<float> > Res;
	return Res(Implementation::exe(x.getValue()));
      }
      
      template<typename Unit>
      TFEL_MATH_INLINE
      qt<typename PowUnit_<N,D,Unit>::type,Complex<double> >
      operator()(const qt<Unit,Complex<double> > x) const
      {
	typedef qt<typename PowUnit_<N,D,Unit>::type,Complex<double> > Res;
	return Res(Implementation::exe(x.getValue()));
      }
      
      template<typename Unit>
      TFEL_MATH_INLINE
      qt<typename PowUnit_<N,D,Unit>::type,Complex<long double> > 
      operator()(const qt<Unit,Complex<long double> > x){
	typedef qt<typename PowUnit_<N,D,Unit>::type,Complex<long double> > Res;
	return Res(Implementation::exe(x.getValue()));
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
      float 
      power(const float x){
	typedef typename tfel::math::PowerImplSelector<N,1u>::type Implementation;
	return Implementation::exe(x);
      }

      template<int N>
      TFEL_MATH_INLINE
      double 
      power(const double x){
	typedef typename tfel::math::PowerImplSelector<N,1u>::type Implementation;
	return Implementation::exe(x);
      }

      template<int N>
      TFEL_MATH_INLINE
      long double 
      power(const long double x){
	typedef typename tfel::math::PowerImplSelector<N,1u>::type Implementation;
	return Implementation::exe(x);
      }

      template<int N>
      TFEL_MATH_INLINE
      Complex<float> 
      power(const Complex<float> x){
	typedef typename tfel::math::PowerImplSelector<N,1u>::type Implementation;
	return Implementation::exe(x);
      }

      template<int N>
      TFEL_MATH_INLINE
      Complex<double> 
      power(const Complex<double> x){
	typedef typename tfel::math::PowerImplSelector<N,1u>::type Implementation;
	return Implementation::exe(x);
      }

      template<int N>
      TFEL_MATH_INLINE
      Complex<long double> 
      power(const Complex<long double> x){
	typedef typename tfel::math::PowerImplSelector<N,1u>::type Implementation;
	return Implementation::exe(x);
      }

      template<int N,typename Unit>
      TFEL_MATH_INLINE
      tfel::math::qt<typename PowUnit_<N,1u,Unit>::type,float> 
      power(const tfel::math::qt<Unit,float> x){
	typedef typename tfel::math::PowerImplSelector<N,1u>::type Implementation;
	typedef tfel::math::qt<typename PowUnit_<N,1u,Unit>::type,float> Res;
	return Res(Implementation::exe(x.getValue()));
      }

      template<int N,typename Unit>
      TFEL_MATH_INLINE
      tfel::math::qt<typename PowUnit_<N,1u,Unit>::type,double>
      power(const tfel::math::qt<Unit,double> x){
	typedef typename tfel::math::PowerImplSelector<N,1u>::type Implementation;
	typedef tfel::math::qt<typename PowUnit_<N,1u,Unit>::type,double> Res;
	return Res(Implementation::exe(x.getValue()));
      }

      template<int N,typename Unit>
      TFEL_MATH_INLINE
      tfel::math::qt<typename PowUnit_<N,1u,Unit>::type,long double>
      power(const tfel::math::qt<Unit,long double> x){
	typedef typename tfel::math::PowerImplSelector<N,1u>::type Implementation;
	typedef tfel::math::qt<typename PowUnit_<N,1u,Unit>::type,long double> Res;
	return Res(Implementation::exe(x.getValue()));
      }

      template<int N,typename Unit>
      TFEL_MATH_INLINE
      tfel::math::qt<typename PowUnit_<N,1u,Unit>::type,Complex<float> > 
      power(const tfel::math::qt<Unit,Complex<float> > x){
	typedef typename tfel::math::PowerImplSelector<N,1u>::type Implementation;
	typedef tfel::math::qt<typename PowUnit_<N,1u,Unit>::type,Complex<float> > Res;
	return Res(Implementation::exe(x.getValue()));
      }

      template<int N,typename Unit>
      TFEL_MATH_INLINE
      tfel::math::qt<typename PowUnit_<N,1u,Unit>::type,Complex<double> >
      power(const tfel::math::qt<Unit,Complex<double> > x){
	typedef typename tfel::math::PowerImplSelector<N,1u>::type Implementation;
	typedef tfel::math::qt<typename PowUnit_<N,1u,Unit>::type,Complex<double> > Res;
	return Res(Implementation::exe(x.getValue()));
      }

      template<int N,typename Unit>
      TFEL_MATH_INLINE
      tfel::math::qt<typename PowUnit_<N,1u,Unit>::type,Complex<long double> > 
      power(const tfel::math::qt<Unit,Complex<long double> > x){
	typedef typename tfel::math::PowerImplSelector<N,1u>::type Implementation;
	typedef tfel::math::qt<typename PowUnit_<N,1u,Unit>::type,Complex<long double> > Res;
	return Res(Implementation::exe(x.getValue()));
      }

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
      float 
      power(const float x){
	typedef typename tfel::math::PowerImplSelector<N,D>::type Implementation;
	return Implementation::exe(x);
      }

      template<int N,unsigned int D>
      TFEL_MATH_INLINE
      double 
      power(const double x){
	typedef typename tfel::math::PowerImplSelector<N,D>::type Implementation;
	return Implementation::exe(x);
      }

      template<int N,unsigned int D>
      TFEL_MATH_INLINE
      long double 
      power(const long double x){
	typedef typename tfel::math::PowerImplSelector<N,D>::type Implementation;
	return Implementation::exe(x);
      }

      template<int N,unsigned int D>
      TFEL_MATH_INLINE
      tfel::math::Complex<float> 
      power(const tfel::math::Complex<float> x){
	typedef typename tfel::math::PowerImplSelector<N,D>::type Implementation;
	return Implementation::exe(x);
      }

      template<int N,unsigned int D>
      TFEL_MATH_INLINE
      tfel::math::Complex<double> 
      power(const tfel::math::Complex<double> x){
	typedef typename tfel::math::PowerImplSelector<N,D>::type Implementation;
	return Implementation::exe(x);
      }

      template<int N,unsigned int D>
      TFEL_MATH_INLINE
      tfel::math::Complex<long double> 
      power(const tfel::math::Complex<long double> x){
	typedef typename tfel::math::PowerImplSelector<N,D>::type Implementation;
	return Implementation::exe(x);
      }

      template<int N,unsigned int D,typename Unit>
      TFEL_MATH_INLINE
      tfel::math::qt<typename PowUnit_<N,D,Unit>::type,float> 
      power(const tfel::math::qt<Unit,float> x){
	typedef typename tfel::math::PowerImplSelector<N,D>::type Implementation;
	typedef tfel::math::qt<typename PowUnit_<N,D,Unit>::type,float> Res;
	return Res(Implementation::exe(x.getValue()));
      }

      template<int N,unsigned int D,typename Unit>
      TFEL_MATH_INLINE
      tfel::math::qt<typename PowUnit_<N,D,Unit>::type,double>
      power(const tfel::math::qt<Unit,double> x){
	typedef typename tfel::math::PowerImplSelector<N,D>::type Implementation;
	typedef tfel::math::qt<typename PowUnit_<N,D,Unit>::type,double> Res;
	return Res(Implementation::exe(x.getValue()));
      }

      template<int N,unsigned int D,typename Unit>
      TFEL_MATH_INLINE
      tfel::math::qt<typename PowUnit_<N,D,Unit>::type,long double>
      power(const tfel::math::qt<Unit,long double> x){
	typedef typename tfel::math::PowerImplSelector<N,D>::type Implementation;
	typedef tfel::math::qt<typename PowUnit_<N,D,Unit>::type,long double> Res;
	return Res(Implementation::exe(x.getValue()));
      }

      template<int N,unsigned int D,typename Unit>
      TFEL_MATH_INLINE
      tfel::math::qt<typename PowUnit_<N,D,Unit>::type,tfel::math::Complex<float> > 
      power(const tfel::math::qt<Unit,tfel::math::Complex<float> > x){
	typedef typename tfel::math::PowerImplSelector<N,D>::type Implementation;
	typedef tfel::math::qt<typename PowUnit_<N,D,Unit>::type,tfel::math::Complex<float> > Res;
	return Res(Implementation::exe(x.getValue()));
      }

      template<int N,unsigned int D,typename Unit>
      TFEL_MATH_INLINE
      tfel::math::qt<typename PowUnit_<N,D,Unit>::type,tfel::math::Complex<double> >
      power(const tfel::math::qt<Unit,tfel::math::Complex<double> > x){
	typedef typename tfel::math::PowerImplSelector<N,D>::type Implementation;
	typedef tfel::math::qt<typename PowUnit_<N,D,Unit>::type,tfel::math::Complex<double> > Res;
	return Res(Implementation::exe(x.getValue()));
      }

      template<int N,unsigned int D,typename Unit>
      TFEL_MATH_INLINE
      tfel::math::qt<typename PowUnit_<N,D,Unit>::type,tfel::math::Complex<long double> > 
      power(const tfel::math::qt<Unit,tfel::math::Complex<long double> > x){
	typedef typename tfel::math::PowerImplSelector<N,D>::type Implementation;
	typedef tfel::math::qt<typename PowUnit_<N,D,Unit>::type,tfel::math::Complex<long double> > Res;
	return Res(Implementation::exe(x.getValue()));
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

      extern tfel::math::FctSqrt sqrt;

    } // end of namespace stdfunctions
    
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

    template<int N,unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<Complex<float>,Power<N,D> >
    {
    public:
      typedef Complex<float> type;
    };

    template<int N,unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<Complex<double>,Power<N,D> >
    {
    public:
      typedef Complex<double> type;
    };

    template<int N,unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<Complex<long double>,Power<N,D> >
    {
    public:
      typedef Complex<long double> type;
    };

    template<typename Unit,int N,unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<qt<Unit,float>,Power<N,D> >
    {
    public:
      typedef qt<typename PowUnit_<N,D,Unit>::type,float> type;
    };

    template<typename Unit,int N,unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<qt<Unit,double>,Power<N,D> >
    {
    public:
      typedef qt<typename PowUnit_<N,D,Unit>::type,double> type;
    };

    template<typename Unit,int N,unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<qt<Unit,long double>,Power<N,D> >
    {
    public:
      typedef qt<typename PowUnit_<N,D,Unit>::type,long double> type;
    };

    template<typename Unit,int N,unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<qt<Unit,Complex<float> >,Power<N,D> >
    {
    public:
      typedef qt<typename PowUnit_<N,D,Unit>::type,Complex<float> > type;
    };

    template<typename Unit,int N,unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<qt<Unit,Complex<double> >,Power<N,D> >
    {
    public:
      typedef qt<typename PowUnit_<N,D,Unit>::type,Complex<double> > type;
    };

    template<typename Unit,int N,unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<qt<Unit,Complex<long double> >,Power<N,D> >
    {
    public:
      typedef qt<typename PowUnit_<N,D,Unit>::type,Complex<long double> > type;
    };

  } // end of namespace math

} // end of namespace tfel
     
#endif /* _LIB_TFEL_POWER_I_ */
