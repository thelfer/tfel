/*!
 * \file   src/Math/StandardFunctions.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cmath>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/qt.hxx"
#include"TFEL/Math/Function/Id.hxx"
#include"TFEL/Math/Function/Cst.hxx"
#include"TFEL/Math/Function/StandardFunctions.hxx"
#include"TFEL/Math/functions.hxx"

#ifndef M_LN10
#define M_LN10      2.30258509299404568402
#endif

#define TFEL_MATH_STANDARD_FUNCTION_MEMBER_I(X,Y)                                         \
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
      TFELMATH_VISIBILITY_EXPORT tfel::math::FctId   id TFEL_UNUSED_ATTRIBUTE;
      TFELMATH_VISIBILITY_EXPORT tfel::math::FctSin  sin TFEL_UNUSED_ATTRIBUTE;
      TFELMATH_VISIBILITY_EXPORT tfel::math::FctCos  cos TFEL_UNUSED_ATTRIBUTE;
      TFELMATH_VISIBILITY_EXPORT tfel::math::FctSinh sinh TFEL_UNUSED_ATTRIBUTE;
      TFELMATH_VISIBILITY_EXPORT tfel::math::FctCosh cosh TFEL_UNUSED_ATTRIBUTE;
      TFELMATH_VISIBILITY_EXPORT tfel::math::FctExp  exp TFEL_UNUSED_ATTRIBUTE;
      TFELMATH_VISIBILITY_EXPORT tfel::math::FctLog  log TFEL_UNUSED_ATTRIBUTE;
      TFELMATH_VISIBILITY_EXPORT tfel::math::FctTan   tan TFEL_UNUSED_ATTRIBUTE;
      TFELMATH_VISIBILITY_EXPORT tfel::math::FctTanh  tanh TFEL_UNUSED_ATTRIBUTE;
      TFELMATH_VISIBILITY_EXPORT tfel::math::FctLog10 log10 TFEL_UNUSED_ATTRIBUTE;
      TFELMATH_VISIBILITY_EXPORT tfel::math::FctAsin  asin TFEL_UNUSED_ATTRIBUTE;
      TFELMATH_VISIBILITY_EXPORT tfel::math::FctAcos  acos TFEL_UNUSED_ATTRIBUTE;
      TFELMATH_VISIBILITY_EXPORT tfel::math::FctAtan  atan TFEL_UNUSED_ATTRIBUTE;
    } // end of namespace stdfunctions

    const FctCos derivate(const FctSin)
    {
      return FctCos();
    }
    
    const FunctionExpr<FunctionNegExpr<FctSin> > 
    derivate(const FctCos)
    {
      return -FctSin();
    }
    
    const FctCosh derivate(const FctSinh)
    {
      return FctCosh();
    }

    const FctSinh derivate(const FctCosh)
    {
      return FctSinh();
    }

    const FctExp derivate(const FctExp)
    {
      return FctExp();
    }
    
    const DFctLog derivate(const FctLog)
    {
      return (Cst<1>())/FctId();
    }

    const DFctTan derivate(const FctTan)
    {
      using tfel::math::stdfunctions::power;
      return Cst<1>()/(power<2>(FctCos()));
    }

    const DFctTanh derivate(const FctTanh)
    {
      using tfel::math::stdfunctions::power;
      return Cst<1>()/(power<2>(FctCosh()));
    }

    const DFctLog10 derivate(const FctLog10)
    {
      return M_LN10/FctId();
    }

    const DFctAsin derivate(const FctAsin)
    {
      using tfel::math::stdfunctions::power;
      FctSqrt sqrt;
      return Cst<1>()/(sqrt[Cst<1>()-power<2>(FctId())]);
    }
    
    const DFctAcos derivate(const FctAcos)
    {
      using tfel::math::stdfunctions::power;
      FctSqrt sqrt;
      return Cst<-1>()/(sqrt[Cst<1>()-power<2>(FctId())]);
    }
    
    const DFctAtan derivate(const FctAtan)
    {
      using tfel::math::stdfunctions::power;
      return Cst<1>()/(Cst<1>()+power<2>(FctId()));
    }

  } // end of namespace math

} // end of namespace tfel
