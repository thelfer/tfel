/*!
 * \file   StandardFunctions.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 nov 2006
 */

#include<cmath>
#include<string>

#include"Math/qt.hxx"
#include"Math/Function/Id.hxx"
#include"Math/Function/Cst.hxx"
#include"Math/Function/StandardFunctions.hxx"
#include"Math/functions.hxx"

#ifndef M_LN10
#define M_LN10      2.30258509299404568402
#endif

#define TFEL_MATH_STANDARD_FUNCTION_MEMBER_I(X,Y)                                         \
      std::string Fct##X::getName(void){                                                  \
        return "Fct"#X;                                                                   \
      }                                                                                   \
      float Fct##X::operator()(const float x) const {			                  \
        using namespace std;                                                              \
        return Y(x);                                                                      \
      }                                                                                   \
      qt<NoUnit,float> Fct##X::operator()(const qt<NoUnit,float> x) const {               \
        using namespace std;                                                              \
        return qt<NoUnit,float>(Y(x.getValue()));			                  \
      }                                                                                   \
      double Fct##X::operator()(const double x) const {			                  \
        using namespace std;                                                              \
        return ::Y(x);                                                                    \
      }                                                                                   \
      qt<NoUnit,double> Fct##X::operator()(const qt<NoUnit,double> x) const {             \
        using namespace std;                                                              \
        return qt<NoUnit,double>(::Y(x.getValue()));			                  \
      }                                                                                   \
      long double Fct##X::operator()(const long double x) const {	                  \
        using namespace std;                                                              \
        return Y(x);                                                                      \
      }                                                                                   \
      qt<NoUnit,long double>						                  \
      Fct##X::operator()(const qt<NoUnit,long double> x) const {                          \
        using namespace std;                                                              \
        return qt<NoUnit,long double>(Y(x.getValue()));	 	                          \
      }                                                                                   \
							                                  \
      Complex<float>							                  \
      Fct##X::operator()(const Complex<float> x) const {                                  \
        using namespace std;                                                              \
        return Y(x);                                                                      \
      }                                                                                   \
							                                  \
      qt<NoUnit,Complex<float> >					                  \
      Fct##X::operator()(const qt<NoUnit,Complex<float> > x) const {                      \
        typedef Complex<float> Comp;                                                      \
        using namespace std;                                                              \
	return qt<NoUnit,Complex<float> >(Comp(Y(static_cast<Complex<float> >(x.getValue())))); \
      }                                                                                   \
                                                                                          \
      Complex<double>							                  \
      Fct##X::operator()(const Complex<double> x) const {                                 \
        using namespace std;                                                              \
        return Y(x);                                                                      \
      }                                                                                   \
							                                  \
      qt<NoUnit,Complex<double> >					                  \
      Fct##X::operator()(const qt<NoUnit,Complex<double> > x) const {                     \
        typedef Complex<double> Comp;                                                     \
        using namespace std;                                                              \
        return qt<NoUnit,Complex<double> >(Comp(Y(static_cast<Complex<double> >(x.getValue())))); \
      }                                                                                   \
							                                  \
      Complex<long double>						                  \
      Fct##X::operator()(const Complex<long double> x) const {                            \
	using namespace std;						                  \
	return Y(x);							                  \
      }                                                                                   \
                                                                                          \
      qt<NoUnit,Complex<long double> >					                  \
      Fct##X::operator()(const qt<NoUnit,Complex<long double> > x) const {                \
        typedef Complex<long double> Comp;                                                \
        using namespace std;                                                              \
	return qt<NoUnit,Complex<long double> >(Comp(Y(static_cast<Complex<long double> >(x.getValue())))); \
      }

#define TFEL_MATH_STANDARD_FUNCTION_MEMBER_II(X,Y)                                        \
      std::string Fct##X::getName(void){					          \
        using namespace std;                                                              \
	return "Fct"#X;							                  \
      }                                                                                   \
      float Fct##X::operator()(const float x) const {			                  \
	using namespace std;						                  \
	return Y(x);							                  \
      }                                                                                   \
      qt<NoUnit,float> Fct##X::operator()(const qt<NoUnit,float> x) const {               \
        using namespace std;                                                              \
	return qt<NoUnit,float>(Y(x.getValue()));			                  \
      }                                                                                   \
      double Fct##X::operator()(const double x) const {			                  \
        using namespace std;                                                              \
	return ::Y(x);							                  \
      }                                                                                   \
      qt<NoUnit,double> Fct##X::operator()(const qt<NoUnit,double> x) const {             \
        using namespace std;						                  \
	return qt<NoUnit,double>(::Y(x.getValue()));			                  \
      }                                                                                   \
      long double Fct##X::operator()(const long double x) const {	                  \
        using namespace std;						                  \
	return Y(x);							                  \
      }                                                                                   \
      qt<NoUnit,long double>						                  \
      Fct##X::operator()(const qt<NoUnit,long double> x) const {                          \
        using namespace std;                                                              \
	return qt<NoUnit,long double>(Y(x.getValue()));	                                  \
      }

namespace tfel{

  namespace math{

    TFEL_MATH_STANDARD_FUNCTION_MEMBER_I(Sin,sin)
    TFEL_MATH_STANDARD_FUNCTION_MEMBER_I(Cos,cos)
    TFEL_MATH_STANDARD_FUNCTION_MEMBER_I(Sinh,sinh)
    TFEL_MATH_STANDARD_FUNCTION_MEMBER_I(Cosh,cosh)
    TFEL_MATH_STANDARD_FUNCTION_MEMBER_I(Exp,exp)
    TFEL_MATH_STANDARD_FUNCTION_MEMBER_I(Log,log) 

    TFEL_MATH_STANDARD_FUNCTION_MEMBER_II(Tan,tan)
    TFEL_MATH_STANDARD_FUNCTION_MEMBER_II(Tanh,tanh)
    TFEL_MATH_STANDARD_FUNCTION_MEMBER_II(Log10,log10)
    TFEL_MATH_STANDARD_FUNCTION_MEMBER_II(Asin,asin)
    TFEL_MATH_STANDARD_FUNCTION_MEMBER_II(Acos,acos)
    TFEL_MATH_STANDARD_FUNCTION_MEMBER_II(Atan,atan) 
         
    namespace stdfunctions{
      tfel::math::FctId   id;
      tfel::math::FctSin  sin;
      tfel::math::FctCos  cos;
      tfel::math::FctSinh sinh;
      tfel::math::FctCosh cosh;
      tfel::math::FctExp  exp;
      tfel::math::FctLog  log;
      tfel::math::FctTan   tan;
      tfel::math::FctTanh  tanh;
      tfel::math::FctLog10 log10;
      tfel::math::FctAsin  asin;
      tfel::math::FctAcos  acos;
      tfel::math::FctAtan  atan;
    } // end of namespace stdfunctions

    const FctCos derivate(const FctSin)
    {
      USING_TFEL_FUNCTIONS;
      return cos;
    }
    
    const FunctionExpr<FunctionNegExpr<FctSin> > 
    derivate(const FctCos)
    {
      USING_TFEL_FUNCTIONS;
      return -sin;
    }
    
    const FctCosh derivate(const FctSinh)
    {
      USING_TFEL_FUNCTIONS;
      return cosh;
    }

    const FctSinh derivate(const FctCosh)
    {
      USING_TFEL_FUNCTIONS;
      return sinh;
    }

    const FctExp derivate(const FctExp)
    {
      USING_TFEL_FUNCTIONS;
      return exp;
    }
    
    const DFctLog derivate(const FctLog)
    {
      USING_TFEL_FUNCTIONS;
      return (Cst<1>())/id;
    }

    const DFctTan derivate(const FctTan)
    {
      USING_TFEL_FUNCTIONS;
      return Cst<1>()/(power<2>(cos));
    }

    const DFctTanh derivate(const FctTanh)
    {
      USING_TFEL_FUNCTIONS;
      return Cst<1>()/(power<2>(cosh));
    }

    const DFctLog10 derivate(const FctLog10)
    {
      USING_TFEL_FUNCTIONS;
      return M_LN10/id;
    }

    const DFctAsin derivate(const FctAsin)
    {
      USING_TFEL_FUNCTIONS;
      return Cst<1>()/sqrt[Cst<1>()-power<2>(id)];
    }
    
    const DFctAcos derivate(const FctAcos)
    {
      USING_TFEL_FUNCTIONS;
      return Cst<-1>()/sqrt[Cst<1>()-power<2>(id)];
    }
    
    const DFctAtan derivate(const FctAtan)
    {
      USING_TFEL_FUNCTIONS;
      return Cst<1>()/(Cst<1>()+power<2>(id));
    }

  } // end of namespace math

} // end of namespace tfel
