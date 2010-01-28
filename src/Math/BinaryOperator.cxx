/*!
 * \file   BinaryOperator.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#include<iostream>
#include<limits>
#include<stdexcept>
#include<string>
#include<cmath>
#include<sstream>

#include"TFEL/Math/Parser/Function.hxx"
#include"TFEL/Math/Parser/BinaryOperator.hxx"

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      double
      OpPlus::apply(const double a,const double b)
      {
	return a+b;
      } // end of OpPlus::apply

      double
      OpMinus::apply(const double a,const double b)
      {
	return a-b;
      } // end of OpMinus::apply

      double
      OpMult::apply(const double a,const double b)
      {
	return a*b;
      } // end of OpMult::apply

      double
      OpDiv::apply(const double a,const double b)
      {
	using namespace std;
	if(fabs(b)<std::numeric_limits<double>::min()){
	  ostringstream value;
	  value << b;
	  string msg("OpDiv::apply : second argument is too small (");
	  msg += value.str()+")";
	  throw(runtime_error(msg));
	}
	return a/b;
      } // end of OpDiv::apply

      double
      OpPower::apply(const double a,const double b)
      {
	using namespace std;
	return pow(a,b);
      } // end of OpDiv::apply

      void
      BinaryOperationBase::throwUnimplementedDifferentiateFunctionException(void)
      {
	using namespace std;
	string msg("BinaryOperationBase::");
	msg += "throwUnimplementedDifferentiateFunctionException : ";
	msg += "unimplemented feature";
	throw(runtime_error(msg));
      }  // end of BinaryOperationBase::throwUnimplementedDifferentiateFunctionException()

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateBinaryOperation<OpPlus>(const tfel::utilities::SmartPtr<Expr> a,
					   const tfel::utilities::SmartPtr<Expr> b,
					   const std::vector<double>::size_type pos,
					   const std::vector<double>& v)
      {
	using namespace tfel::utilities;
	SmartPtr<Expr> da = a->differentiate(pos,v);
	SmartPtr<Expr> db = b->differentiate(pos,v);
	return SmartPtr<Expr>(new BinaryOperation<OpPlus>(da,db));
      } // end of differentiateBinaryOperation<OpPlus>

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateBinaryOperation<OpMinus>(const tfel::utilities::SmartPtr<Expr> a,
					    const tfel::utilities::SmartPtr<Expr> b,
					    const std::vector<double>::size_type pos,
					    const std::vector<double>& v)
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new BinaryOperation<OpMinus>(a->differentiate(pos,v),
							   b->differentiate(pos,v)));
      } // end of differentiateBinaryOperation<OpMinus>

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateBinaryOperation<OpMult>(const tfel::utilities::SmartPtr<Expr> a,
					   const tfel::utilities::SmartPtr<Expr> b,
					   const std::vector<double>::size_type pos,
					   const std::vector<double>& v)
      {
	using namespace tfel::utilities;
	SmartPtr<Expr> d1(new BinaryOperation<OpMult>(a->differentiate(pos,v),b->clone(v)));
	SmartPtr<Expr> d2(new BinaryOperation<OpMult>(a->clone(v),b->differentiate(pos,v)));
	return SmartPtr<Expr>(new BinaryOperation<OpPlus>(d1,d2));
      } // end of differentiateBinaryOperation<OpMult>

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateBinaryOperation<OpDiv>(const tfel::utilities::SmartPtr<Expr> a,
					  const tfel::utilities::SmartPtr<Expr> b,
					  const std::vector<double>::size_type pos,
					  const std::vector<double>& v)
      {
	using namespace std;
	using namespace tfel::utilities;
	SmartPtr<Expr> d1(new BinaryOperation<OpDiv>(a->differentiate(pos,v),b->clone(v)));
	SmartPtr<Expr> d2num(new BinaryOperation<OpMult>(a->clone(v),b->differentiate(pos,v)));
	SmartPtr<Expr> d2den(new BinaryOperation<OpMult>(b->clone(v),b->clone(v)));
	SmartPtr<Expr> d2(new BinaryOperation<OpDiv>(d2num,d2den));
	return SmartPtr<Expr>(new BinaryOperation<OpMinus>(d1,d2));
      } // end of differentiateBinaryOperation<OpDiv>

      template<>
      tfel::utilities::SmartPtr<Expr>
      differentiateBinaryOperation<OpPower>(const tfel::utilities::SmartPtr<Expr> a,
					    const tfel::utilities::SmartPtr<Expr> b,
					    const std::vector<double>::size_type  pos,
					    const std::vector<double>& v)
      {
	using namespace std;
	using namespace tfel::utilities;
	SmartPtr<Expr> ca = a->clone(v);
	SmartPtr<Expr> cb = b->clone(v);
	SmartPtr<Expr> l(new BinaryOperation<OpPower>(ca,cb));
	SmartPtr<Expr> da(a->differentiate(pos,v));
	SmartPtr<Expr> db(b->differentiate(pos,v));
	SmartPtr<Expr> lna(new StandardFunction<log>(ca));
	SmartPtr<Expr> r1(new BinaryOperation<OpMult>(db,lna));
	SmartPtr<Expr> r21(new BinaryOperation<OpDiv>(da,ca));
	SmartPtr<Expr> r2(new BinaryOperation<OpMult>(cb,r21));
	SmartPtr<Expr> r(new BinaryOperation<OpPlus>(r1,r2));
	return SmartPtr<Expr>(new BinaryOperation<OpMult>(l,r));
      } // end of differentiateBinaryOperation<OpPower>

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
