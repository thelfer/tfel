/*!
 * \file   include/TFEL/Math/Function/StandardFunctions.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   05 nov 2006
 */

#ifndef _LIB_TFEL_STANDARDFUNCTION_HXX_
#define _LIB_TFEL_STANDARDFUNCTION_HXX_ 

#include<cmath>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/TypeTraits/IsInvalid.hxx"
#include"TFEL/Math/General/ComputeUnaryResult.hxx"
#include"TFEL/Math/Forward/qt.hxx"
#include"TFEL/Math/Forward/Complex.hxx"
#include"TFEL/Math/Function/FunctionConcept.hxx"
#include"TFEL/Math/Function/Id.hxx"

#define TFEL_MATH_UNARY_FUNCTION_I(X,Y,Z)                                                 \
    template<>                                                                            \
    struct FunctionTraits<Fct##X>                                                         \
    {                                                                                     \
      typedef Z DerivateFunc;                                                             \
    };                                                                                    \
                                                                                          \
    struct TFELMATH_VISIBILITY_EXPORT Fct##X                                                  \
      : public FunctionConcept<Fct##X>                                                    \
    {					                                                  \
							                                  \
      /*!						                                  \
       * \brief  Return the name of the class.		                                  \
       * \param  void.					                                  \
       * \return std::string, the name of the class.	                                  \
       * \see    Name.					                                  \
       */						                                  \
      static std::string getName(void);                                                   \
							                                  \
      float operator()(const float) const;                                                \
      qt<NoUnit,float> operator()(const qt<NoUnit,float>) const;                          \
      double operator()(const double) const;                                              \
      qt<NoUnit,double> operator()(const qt<NoUnit,double>) const;                        \
      long double operator()(const long double) const;                                    \
      qt<NoUnit,long double> operator()(const qt<NoUnit,long double>) const;              \
							                                  \
      Complex<float>							                  \
      operator()(const Complex<float>) const;                                             \
     						                                          \
      qt<NoUnit,Complex<float> >                                                          \
      operator()(const qt<NoUnit,Complex<float> >) const;                                 \
                                                                                          \
      Complex<double>                                                                     \
      operator()(const Complex<double>) const;                                            \
							                                  \
      qt<NoUnit,Complex<double> >                                                         \
      operator()(const qt<NoUnit,Complex<double> >) const;                                \
							                                  \
      Complex<long double>                                                                \
      operator()(const Complex<long double>) const;                                       \
                                                                                          \
      qt<NoUnit,Complex<long double> >                                                    \
      operator()(const qt<NoUnit,Complex<long double> >) const;                           \
                                                                                          \
      template<typename T>      			                                  \
      typename tfel::meta::EnableIf<                                                      \
        !tfel::typetraits::IsInvalid<typename ComputeUnaryResult<T,Fct##X>::Result>::cond,\
	 typename ComputeUnaryResult<T,Fct##X>::Handle			                  \
      >::type                                                                             \
      operator()(const T& v) const{                                                       \
	typedef typename ComputeUnaryResult<T,Fct##X>::Handle Handle;                     \
	return Handle(v,*this);                                                           \
      }                                                                                   \
    };                                                                                    \
                                                                                          \
    /*                                                                                    \
     * \brief return the derivate of this function                                        \
     * \param void                                                                        \
     * \return const Z, the derivate                                                      \
     */                                                                                   \
    TFELMATH_VISIBILITY_EXPORT                                                                \
    const Z derivate(const Fct##X);                                                       \
                                                                                          \
    namespace stdfunctions{                                                               \
      extern TFELMATH_VISIBILITY_EXPORT Fct##X Y TFEL_UNUSED_ATTRIBUTE;                   \
    }


#define TFEL_MATH_UNARY_FUNCTION_II(X,Y,Z)                                                \
    template<>                                                                            \
    struct FunctionTraits<Fct##X>                                                         \
    {                                                                                     \
      typedef Z DerivateFunc;                                                             \
    };                                                                                    \
                                                                                          \
    struct TFELMATH_VISIBILITY_EXPORT Fct##X                                                  \
      : public FunctionConcept<Fct##X>                                                    \
    {            					                                  \
							                                  \
      /*!						                                  \
       * \brief  Return the name of the class.		                                  \
       * \param  void.					                                  \
       * \return std::string, the name of the class.	                                  \
       * \see    Name.					                                  \
       */						                                  \
      static std::string getName(void);                                                   \
							                                  \
      float operator()(const float) const;					          \
      qt<NoUnit,float> operator()(const qt<NoUnit,float>) const;                          \
      double operator()(const double) const;                                              \
      qt<NoUnit,double> operator()(const qt<NoUnit,double>) const;                        \
      long double operator()(const long double) const;                                    \
      qt<NoUnit,long double> operator()(const qt<NoUnit,long double>) const;              \
                                							  \
      template<typename T>      			                                  \
      typename tfel::meta::EnableIf<                                                      \
        !tfel::typetraits::IsInvalid<typename ComputeUnaryResult<T,Fct##X>::Result>::cond,\
        typename ComputeUnaryResult<T,Fct##X>::Handle                                     \
      >::type                                                                             \
      operator()(const T& v) const {                                                      \
	typedef typename ComputeUnaryResult<T,Fct##X>::Handle Handle;                     \
	return Handle(v,*this);                                                           \
      }                                                                                   \
                                                                                          \
    };                                                                                    \
                                                                                          \
    /*                                                                                    \
     * \brief return the derivate of this function                                        \
     * \param void                                                                        \
     * \return const Z, the derivate                                                      \
     */                                                                                   \
    const Z derivate(const Fct##X);                                                       \
                                                                                          \
    namespace stdfunctions{                                                               \
      extern TFELMATH_VISIBILITY_EXPORT Fct##X Y TFEL_UNUSED_ATTRIBUTE;                   \
    }

#define TFEL_MATH_UNARY_FUNCTION_RESULT_I_(X,Y)                  \
    /*!                                                          \
     * \brief Partial specialisation for X                       \
     * \see   UnaryResultType                                    \
     */                                                          \
    template<>                                                   \
    class UnaryResultType<X,Fct##Y>{                             \
    public:                                                      \
      typedef X type;                                            \
    };                                                           \
    /*!                                                          \
     * \brief Partial specialisation for X                       \
     * \see   UnaryResultType                                    \
     */                                                          \
    template<>                                                   \
    class UnaryResultType<qt<NoUnit,X>,Fct##Y>{                  \
    public:                                                      \
      typedef qt<NoUnit,X> type;                                 \
    }                                                           

#define TFEL_MATH_UNARY_FUNCTION_RESULT_II_(X,Y)                 \
    /*!                                                          \
     * \brief Partial specialisation for X                       \
     * \see   UnaryResultType                                    \
     */                                                          \
    template<>                                                   \
    class UnaryResultType<X,Fct##Y>{                             \
    public:                                                      \
      typedef X type;                                            \
    };                                                           \
    /*!                                                          \
     * \brief Partial specialisation for X                       \
     * \see   UnaryResultType                                    \
     */                                                          \
    template<>                                                   \
    class UnaryResultType<qt<NoUnit,X>,Fct##Y>{                  \
    public:                                                      \
      typedef qt<NoUnit,X> type;                                 \
    };                                                           \
    /*!                                                          \
     * \brief Partial specialisation for X                       \
     * \see   UnaryResultType                                    \
     */                                                          \
    template<>                                                   \
    class UnaryResultType<Complex<X>,Fct##Y>{                    \
    public:                                                      \
      typedef Complex<X> type;                                   \
    };                                                           \
    /*!                                                          \
     * \brief Partial specialisation for X                       \
     * \see   UnaryResultType                                    \
     */                                                          \
    template<>                                                   \
    class UnaryResultType<qt<NoUnit,Complex<X> >,Fct##Y>{        \
    public:                                                      \
      typedef qt<NoUnit,Complex<X> > type;                       \
    }

#define TFEL_MATH_UNARY_FUNCTION_RESULT_I(X)                     \
    TFEL_MATH_UNARY_FUNCTION_RESULT_I_(float,X);                 \
    TFEL_MATH_UNARY_FUNCTION_RESULT_I_(double,X);                \
    TFEL_MATH_UNARY_FUNCTION_RESULT_I_(long double,X)

#define TFEL_MATH_UNARY_FUNCTION_RESULT_II(X)                    \
    TFEL_MATH_UNARY_FUNCTION_RESULT_II_(float,X);                \
    TFEL_MATH_UNARY_FUNCTION_RESULT_II_(double,X);               \
    TFEL_MATH_UNARY_FUNCTION_RESULT_II_(long double,X)

namespace tfel{
  
  namespace math {

    // forward declaration (see Cst.hxx)
    template<short N,unsigned short D>
    struct Cst;

    /*
     * Forward declarations
     */
    struct FctSin;
    struct FctCos;
    struct FctSinh;
    struct FctCosh;
    struct FctExp;
    struct FctLog;
    struct FctTan;
    struct FctTanh;
    struct FctLog10;
    struct FctAsin;
    struct FctAcos;
    struct FctAtan;

    template<int N,unsigned int D>
    struct Power;

    typedef Power<1,2> FctSqrt;

    typedef FunctionExpr<FunctionFunctionExpr<Cst<1,1>,FctId,OpDiv> > DFctLog;

    typedef FunctionExpr<FunctionFunctionExpr<Cst<1,1>,
					      FunctionExpr<FunctionsCompositionExpr<Power<2u,1u>,
										    FctCos> >,
					      OpDiv> > DFctTan;

    typedef FunctionExpr<FunctionFunctionExpr<Cst<1,1>,
					      FunctionExpr<FunctionsCompositionExpr<Power<2u,1u>,
										    FctCosh> >,
					      OpDiv> > DFctTanh; 

    typedef FunctionExpr<ScalarFunctionExpr<double,FctId,OpDiv> > DFctLog10;

    typedef FunctionExpr<FunctionFunctionExpr<Cst<1,1>,
					      FunctionExpr<FunctionsCompositionExpr<Power<2u,1u>,
										    FctId> >,
					      OpMinus> > OMinSquareFctId;

    typedef FunctionExpr<FunctionFunctionExpr<Cst<1,1>,
					      FunctionExpr<FunctionsCompositionExpr<Power<2u,1u>,
										    FctId> >,
					      OpPlus> > OnePlusSquareFctId;

    typedef FunctionExpr<FunctionFunctionExpr<Cst<1,1>,
					      FunctionExpr<FunctionsCompositionExpr<
                                              FctSqrt,
                                              OMinSquareFctId> >,OpDiv> > DFctAsin;

    typedef FunctionExpr<FunctionFunctionExpr<Cst<-1,1>,
					      FunctionExpr<
                                              FunctionsCompositionExpr<FctSqrt,OMinSquareFctId> >,
					      OpDiv> > DFctAcos;

    typedef FunctionExpr<FunctionFunctionExpr<Cst<1,1>,OnePlusSquareFctId,OpDiv> > DFctAtan;

    TFEL_MATH_UNARY_FUNCTION_I(Sin,sin,FctCos)
    TFEL_MATH_UNARY_FUNCTION_I(Cos,cos,FunctionExpr<FunctionNegExpr<FctSin> >)
    TFEL_MATH_UNARY_FUNCTION_I(Sinh,sinh,FctCosh)
    TFEL_MATH_UNARY_FUNCTION_I(Cosh,cosh,FctSinh)
    TFEL_MATH_UNARY_FUNCTION_I(Exp,exp,FctExp)
    TFEL_MATH_UNARY_FUNCTION_I(Log,log,DFctLog)

    TFEL_MATH_UNARY_FUNCTION_II(Tan,tan,DFctTan)
    TFEL_MATH_UNARY_FUNCTION_II(Tanh,tanh,DFctTanh)
    TFEL_MATH_UNARY_FUNCTION_II(Log10,log10,DFctLog10)
    TFEL_MATH_UNARY_FUNCTION_II(Asin,asin,DFctAsin)
    TFEL_MATH_UNARY_FUNCTION_II(Acos,acos,DFctAcos)
    TFEL_MATH_UNARY_FUNCTION_II(Atan,atan,DFctAtan)

    TFEL_MATH_UNARY_FUNCTION_RESULT_I(Sin);
    TFEL_MATH_UNARY_FUNCTION_RESULT_I(Cos);
    TFEL_MATH_UNARY_FUNCTION_RESULT_I(Sinh);
    TFEL_MATH_UNARY_FUNCTION_RESULT_I(Cosh);
    TFEL_MATH_UNARY_FUNCTION_RESULT_I(Exp);
    TFEL_MATH_UNARY_FUNCTION_RESULT_I(Log);

    TFEL_MATH_UNARY_FUNCTION_RESULT_II(Tan);
    TFEL_MATH_UNARY_FUNCTION_RESULT_II(Tanh);
    TFEL_MATH_UNARY_FUNCTION_RESULT_II(Log10);
    TFEL_MATH_UNARY_FUNCTION_RESULT_II(Asin);
    TFEL_MATH_UNARY_FUNCTION_RESULT_II(Acos);
    TFEL_MATH_UNARY_FUNCTION_RESULT_II(Atan);

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_STANDARDFUNCTION_HXX */

