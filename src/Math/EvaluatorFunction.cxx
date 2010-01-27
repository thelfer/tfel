/*!
 * \file   EvaluatorFunction.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2009
 */

#include"Math/Parser/EvaluatorFunction.hxx"
   
namespace tfel
{

  namespace math
  {

    namespace parser
    {
    
      EvaluatorFunctionBase::EvaluatorFunctionBase()
      {} // end of EvaluatorFunctionBase::EvaluatorFunctionBase

      tfel::utilities::SmartPtr<Expr>
      EvaluatorFunctionBase::differentiate(const std::vector<double>::size_type,
					   const std::vector<double>&) const
      {
	using namespace std;
	string msg("EvaluatorFunctionBase::differentiate : ");
	msg += "unimplemented feature";
	throw(runtime_error(msg));
      } // end of EvaluatorFunctionBase::differentiate

      EvaluatorFunctionBase::~EvaluatorFunctionBase()
      {} // end of EvaluatorFunctionBase::~EvaluatorFunctionBase

      EvaluatorFunction1VBase::EvaluatorFunction1VBase(const tfel::utilities::SmartPtr<Expr> e)
	: expr(e)
      {} // end of EvaluatorFunction1VBase::EvaluatorFunction1VBase

      void
      EvaluatorFunction1VBase::checkCyclicDependency(const std::vector<std::string>& names) const
	throw(std::runtime_error)
      {
	this->expr->checkCyclicDependency(names);
      } // end of EvaluatorFunction1VBase::checkCyclicDependency

      EvaluatorFunction1VBase::~EvaluatorFunction1VBase()
      {} // end of EvaluatorFunction1VBase::~EvaluatorFunction1VBase()

      EvaluatorFunction2VBase::EvaluatorFunction2VBase(const tfel::utilities::SmartPtr<Expr> e1_,
						       const tfel::utilities::SmartPtr<Expr> e2_)
	: e1(e1_),
	  e2(e2_)
      {} // end of EvaluatorFunction2VBase::EvaluatorFunction2VBase

      void
      EvaluatorFunction2VBase::checkCyclicDependency(const std::vector<std::string>& names) const
	throw(std::runtime_error)
      {
	this->e1->checkCyclicDependency(names);
	this->e2->checkCyclicDependency(names);
      } // end of EvaluatorFunction2VBase::checkCyclicDependency

      EvaluatorFunction2VBase::~EvaluatorFunction2VBase()
      {} // end of EvaluatorFunction2VBase::~EvaluatorFunction2VBase()

      EvaluatorFunction1P1V::EvaluatorFunction1P1V(const EvaluatorProxyFunctionPtr1P1V f_,
						   const int n_,
						   const tfel::utilities::SmartPtr<Expr> e)
	: EvaluatorFunction1VBase(e),
	  f(f_),
	  n(n_)
      {} // end of EvaluatorFunction1P1V<f>::EvaluatorFunction1P1V

      double
      EvaluatorFunction1P1V::getValue(void) const
      {
	return (*(this->f))(this->n,this->expr->getValue());
      } // end of EvaluatorFunction1P1V::getValue(void) const

      tfel::utilities::SmartPtr<Expr>
      EvaluatorFunction1P1V::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new EvaluatorFunction1P1V(this->f,
							this->n,
							this->expr->resolveDependencies()));
      } // end of EvaluatorFunction1P1V::resolveDependencies(void) const

      tfel::utilities::SmartPtr<Expr>
      EvaluatorFunction1P1V::clone(const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new EvaluatorFunction1P1V(this->f,
							this->n,
							this->expr->clone(v)));
      } // end of EvaluatorFunction1P1V::clone(const std::vector<double>&) const

      EvaluatorFunction1P1V::~EvaluatorFunction1P1V()
      {} // end of EvaluatorFunction2P1V::~EvaluatorFunction2P1V()

      EvaluatorFunction2P1V::EvaluatorFunction2P1V(const EvaluatorProxyFunctionPtr2P1V f_,
						   const int n_,
						   const int m_,
						   const tfel::utilities::SmartPtr<Expr> e)
	: EvaluatorFunction1VBase(e),
	  f(f_),
	  n(n_),
	  m(m_)
      {} // end of EvaluatorFunction2P1V::EvaluatorFunction2P1V

      double
      EvaluatorFunction2P1V::getValue(void) const
      {
	return (*(this->f))(this->n,this->m,this->expr->getValue());
      } // end of EvaluatorFunction2P1V::getValue(void) const

      tfel::utilities::SmartPtr<Expr>
      EvaluatorFunction2P1V::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new EvaluatorFunction2P1V(this->f,
							this->n,
							this->m,
							this->expr->resolveDependencies()));
      } // end of EvaluatorFunction2P1V::resolveDependencies(void) const

      tfel::utilities::SmartPtr<Expr>
      EvaluatorFunction2P1V::clone(const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new EvaluatorFunction2P1V(this->f,
							this->n,
							this->m,
							this->expr->clone(v)));
      } // end of EvaluatorFunction2P1V::clone(const std::vector<double>&) const

      EvaluatorFunction2P1V::~EvaluatorFunction2P1V()
      {} // end of EvaluatorFunction2P1V::~EvaluatorFunction2P1V()

      EvaluatorFunction1P2V::EvaluatorFunction1P2V(const EvaluatorProxyFunctionPtr1P2V f_,
						   const int n_,
						   const tfel::utilities::SmartPtr<Expr> e1_,
						   const tfel::utilities::SmartPtr<Expr> e2_)
	: EvaluatorFunction2VBase(e1_,e2_),
	  f(f_),
	  n(n_)
      {} // end of EvaluatorFunction1P2V::EvaluatorFunction1P2V

      double
      EvaluatorFunction1P2V::getValue(void) const
      {
	return (*(this->f))(this->n,this->e1->getValue(),this->e2->getValue());
      } // end of EvaluatorFunction1P2V::getValue(void) const

      tfel::utilities::SmartPtr<Expr>
      EvaluatorFunction1P2V::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new EvaluatorFunction1P2V(this->f,
							this->n,
							this->e1->resolveDependencies(),
							this->e2->resolveDependencies()));
      } // end of EvaluatorFunction1P2V::resolveDependencies(void) const

      tfel::utilities::SmartPtr<Expr>
      EvaluatorFunction1P2V::clone(const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new EvaluatorFunction1P2V(this->f,
							this->n,
							this->e1->clone(v),
							this->e2->clone(v)));
      } // end of EvaluatorFunction1P2V::clone(const std::vector<double>&) const

      EvaluatorFunction1P2V::~EvaluatorFunction1P2V()
      {} // end of EvaluatorFunction2P2V::~EvaluatorFunction2P2V()

      EvaluatorFunction2P2V::EvaluatorFunction2P2V(const EvaluatorProxyFunctionPtr2P2V f_,
						   const int n_,
						   const int m_,
						   const tfel::utilities::SmartPtr<Expr> e1_,
						   const tfel::utilities::SmartPtr<Expr> e2_)
	: EvaluatorFunction2VBase(e1_,e2_),
	  f(f_),
	  n(n_),
	  m(m_)
      {} // end of EvaluatorFunction2P2V::EvaluatorFunction2P2V

      double
      EvaluatorFunction2P2V::getValue(void) const
      {
	return (*(this->f))(this->n,this->m,
			    this->e1->getValue(),
			    this->e2->getValue());
      } // end of EvaluatorFunction2P2V::getValue(void) const

      tfel::utilities::SmartPtr<Expr>
      EvaluatorFunction2P2V::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new EvaluatorFunction2P2V(this->f,
							this->n,this->m,
							this->e1->resolveDependencies(),
							this->e2->resolveDependencies()));
      } // end of EvaluatorFunction2P2V::resolveDependencies(void) const

      tfel::utilities::SmartPtr<Expr>
      EvaluatorFunction2P2V::clone(const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return SmartPtr<Expr>(new EvaluatorFunction2P2V(this->f,
							this->n,this->m,
							this->e1->clone(v),
							this->e2->clone(v)));
      } // end of EvaluatorFunction2P2V::clone(const std::vector<double>&) const

      EvaluatorFunction2P2V::~EvaluatorFunction2P2V()
      {} // end of EvaluatorFunction2P2V::~EvaluatorFunction2P2V()

      double
      EvaluatorFunctionWrapper<0u>::apply(const EvaluatorFunctionWrapper<0u>::type f,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >&)
      {
	return (*f)();
      } // end of EvaluatorFunctionWrapper<0u>::apply

      double
      EvaluatorFunctionWrapper<0u>::apply(const EvaluatorFunctionWrapper<0u>::type1P f,
					  const int n,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >&)
      {
	return (*f)(n);
      } // end of EvaluatorFunctionWrapper<0u>::apply

      double
      EvaluatorFunctionWrapper<0u>::apply(const EvaluatorFunctionWrapper<0u>::type2P f,
					  const int n, const int m,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >&)
      {
	return (*f)(n,m);
      } // end of EvaluatorFunctionWrapper<0u>::apply

      double
      EvaluatorFunctionWrapper<0u>::apply(const EvaluatorFunctionWrapper<0u>::type1UP f,
					  const unsigned int n,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >&)
      {
	return (*f)(n);
      } // end of EvaluatorFunctionWrapper<0u>::apply

      double
      EvaluatorFunctionWrapper<0u>::apply(const EvaluatorFunctionWrapper<0u>::type2UP f,
					  const unsigned int n, const unsigned int m,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >&)
      {
	return (*f)(n,m);
      } // end of EvaluatorFunctionWrapper<0u>::apply

      double
      EvaluatorFunctionWrapper<1u>::apply(const EvaluatorFunctionWrapper<1u>::type f,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(expr[0]->getValue());
      } // end of EvaluatorFunctionWrapper<1u>::apply

      double
      EvaluatorFunctionWrapper<1u>::apply(const EvaluatorFunctionWrapper<1u>::type1P f,
					  const int n,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,expr[0]->getValue());
      } // end of EvaluatorFunctionWrapper<1u>::apply

      double
      EvaluatorFunctionWrapper<1u>::apply(const EvaluatorFunctionWrapper<1u>::type2P f,
					  const int n, const int m,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,m,expr[0]->getValue());
      } // end of EvaluatorFunctionWrapper<1u>::apply

      double
      EvaluatorFunctionWrapper<1u>::apply(const EvaluatorFunctionWrapper<1u>::type1UP f,
					  const unsigned int n,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,expr[0]->getValue());
      } // end of EvaluatorFunctionWrapper<1u>::apply

      double
      EvaluatorFunctionWrapper<1u>::apply(const EvaluatorFunctionWrapper<1u>::type2UP f,
					  const unsigned int n, const unsigned int m,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,m,expr[0]->getValue());
      } // end of EvaluatorFunctionWrapper<1u>::apply

      double
      EvaluatorFunctionWrapper<2u>::apply(const EvaluatorFunctionWrapper<2u>::type f,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(expr[0]->getValue(),expr[1]->getValue());
      } // end of EvaluatorFunctionWrapper<2u>::apply

      double
      EvaluatorFunctionWrapper<2u>::apply(const EvaluatorFunctionWrapper<2u>::type1P f,
					  const int n,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,expr[0]->getValue(),expr[1]->getValue());
      } // end of EvaluatorFunctionWrapper<2u>::apply

      double
      EvaluatorFunctionWrapper<2u>::apply(const EvaluatorFunctionWrapper<2u>::type2P f,
					  const int n, const int m,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,m,expr[0]->getValue(),expr[1]->getValue());
      } // end of EvaluatorFunctionWrapper<2u>::apply

      double
      EvaluatorFunctionWrapper<2u>::apply(const EvaluatorFunctionWrapper<2u>::type1UP f,
					  const unsigned int n,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,expr[0]->getValue(),expr[1]->getValue());
      } // end of EvaluatorFunctionWrapper<2u>::apply

      double
      EvaluatorFunctionWrapper<2u>::apply(const EvaluatorFunctionWrapper<2u>::type2UP f,
					  const unsigned int n, const unsigned int m,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,m,expr[0]->getValue(),expr[1]->getValue());
      } // end of EvaluatorFunctionWrapper<2u>::apply

      double
      EvaluatorFunctionWrapper<3u>::apply(const EvaluatorFunctionWrapper<3u>::type f,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue());
      } // end of EvaluatorFunctionWrapper<3u>::apply

      double
      EvaluatorFunctionWrapper<3u>::apply(const EvaluatorFunctionWrapper<3u>::type1P f,
					  const int n,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue());
      } // end of EvaluatorFunctionWrapper<3u>::apply

      double
      EvaluatorFunctionWrapper<3u>::apply(const EvaluatorFunctionWrapper<3u>::type2P f,
					  const int n, const int m,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,m,expr[0]->getValue(),expr[1]->getValue(),
                    expr[2]->getValue());
      } // end of EvaluatorFunctionWrapper<3u>::apply

      double
      EvaluatorFunctionWrapper<3u>::apply(const EvaluatorFunctionWrapper<3u>::type1UP f,
					  const unsigned int n,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,expr[0]->getValue(),expr[1]->getValue(),expr[2]->getValue());
      } // end of EvaluatorFunctionWrapper<3u>::apply

      double
      EvaluatorFunctionWrapper<3u>::apply(const EvaluatorFunctionWrapper<3u>::type2UP f,
					  const unsigned int n, const unsigned int m,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,m,expr[0]->getValue(),expr[1]->getValue(),expr[2]->getValue());
      } // end of EvaluatorFunctionWrapper<3u>::apply

      double
      EvaluatorFunctionWrapper<4u>::apply(const EvaluatorFunctionWrapper<4u>::type f,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue(),expr[3]->getValue());
      } // end of EvaluatorFunctionWrapper<4u>::apply

      double
      EvaluatorFunctionWrapper<4u>::apply(const EvaluatorFunctionWrapper<4u>::type1P f,
					  const int n,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue(),expr[3]->getValue());
      } // end of EvaluatorFunctionWrapper<4u>::apply

      double
      EvaluatorFunctionWrapper<4u>::apply(const EvaluatorFunctionWrapper<4u>::type2P f,
					  const int n, const int m,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,m,expr[0]->getValue(),expr[1]->getValue(),
                    expr[2]->getValue(),expr[3]->getValue());
      } // end of EvaluatorFunctionWrapper<4u>::apply

      double
      EvaluatorFunctionWrapper<4u>::apply(const EvaluatorFunctionWrapper<4u>::type1UP f,
					  const unsigned int n,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,expr[0]->getValue(),expr[1]->getValue(),
		    expr[2]->getValue(),expr[3]->getValue());
      } // end of EvaluatorFunctionWrapper<4u>::apply

      double
      EvaluatorFunctionWrapper<4u>::apply(const EvaluatorFunctionWrapper<4u>::type2UP f,
					  const unsigned int n, const unsigned int m,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,m,expr[0]->getValue(),expr[1]->getValue(),
		    expr[2]->getValue(),expr[3]->getValue());
      } // end of EvaluatorFunctionWrapper<4u>::apply

      double
      EvaluatorFunctionWrapper<5u>::apply(const EvaluatorFunctionWrapper<5u>::type f,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue(),expr[3]->getValue(),
                    expr[4]->getValue());
      } // end of EvaluatorFunctionWrapper<5u>::apply

      double
      EvaluatorFunctionWrapper<5u>::apply(const EvaluatorFunctionWrapper<5u>::type1P f,
					  const int n,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue(),expr[3]->getValue(),
                    expr[4]->getValue());
      } // end of EvaluatorFunctionWrapper<5u>::apply

      double
      EvaluatorFunctionWrapper<5u>::apply(const EvaluatorFunctionWrapper<5u>::type2P f,
					   const int n, const int m,
					   const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,m,expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue(),expr[3]->getValue(),
                    expr[4]->getValue());
      } // end of EvaluatorFunctionWrapper<5u>::apply

      double
      EvaluatorFunctionWrapper<5u>::apply(const EvaluatorFunctionWrapper<5u>::type1UP f,
					  const unsigned int n,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,expr[0]->getValue(),expr[1]->getValue(),
		    expr[2]->getValue(),expr[3]->getValue(),
	            expr[4]->getValue());
      } // end of EvaluatorFunctionWrapper<5u>::apply

      double
      EvaluatorFunctionWrapper<5u>::apply(const EvaluatorFunctionWrapper<5u>::type2UP f,
					  const unsigned int n, const unsigned int m,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,m,expr[0]->getValue(),expr[1]->getValue(),
		    expr[2]->getValue(),expr[3]->getValue(),
		    expr[4]->getValue());
      } // end of EvaluatorFunctionWrapper<5u>::apply

      double
      EvaluatorFunctionWrapper<6u>::apply(const EvaluatorFunctionWrapper<6u>::type f,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue(),expr[3]->getValue(),
                    expr[4]->getValue(),expr[5]->getValue());
      } // end of EvaluatorFunctionWrapper<6u>::apply

      double
      EvaluatorFunctionWrapper<6u>::apply(const EvaluatorFunctionWrapper<6u>::type1P f,
					  const int n,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue(),expr[3]->getValue(),
                    expr[4]->getValue(),expr[5]->getValue());
      } // end of EvaluatorFunctionWrapper<6u>::apply

      double
      EvaluatorFunctionWrapper<6u>::apply(const EvaluatorFunctionWrapper<6u>::type2P f,
					   const int n, const int m,
					   const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,m,expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue(),expr[3]->getValue(),
                    expr[4]->getValue(),expr[5]->getValue());
      } // end of EvaluatorFunctionWrapper<6u>::apply

      double
      EvaluatorFunctionWrapper<6u>::apply(const EvaluatorFunctionWrapper<6u>::type1UP f,
					  const unsigned int n,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,expr[0]->getValue(),expr[1]->getValue(),
		    expr[2]->getValue(),expr[3]->getValue(),
	            expr[4]->getValue(),expr[5]->getValue());
      } // end of EvaluatorFunctionWrapper<6u>::apply

      double
      EvaluatorFunctionWrapper<6u>::apply(const EvaluatorFunctionWrapper<6u>::type2UP f,
					  const unsigned int n, const unsigned int m,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,m,expr[0]->getValue(),expr[1]->getValue(),
		    expr[2]->getValue(),expr[3]->getValue(),
		    expr[4]->getValue(),expr[5]->getValue());
      } // end of EvaluatorFunctionWrapper<6u>::apply

      double
      EvaluatorFunctionWrapper<7u>::apply(const EvaluatorFunctionWrapper<7u>::type f,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue(),expr[3]->getValue(),
                    expr[4]->getValue(),expr[5]->getValue(),
		    expr[6]->getValue());
      } // end of EvaluatorFunctionWrapper<7u>::apply

      double
      EvaluatorFunctionWrapper<7u>::apply(const EvaluatorFunctionWrapper<7u>::type1P f,
					  const int n,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue(),expr[3]->getValue(),
                    expr[4]->getValue(),expr[5]->getValue(),
		    expr[6]->getValue());
      } // end of EvaluatorFunctionWrapper<7u>::apply

      double
      EvaluatorFunctionWrapper<7u>::apply(const EvaluatorFunctionWrapper<7u>::type2P f,
					   const int n, const int m,
					   const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,m,expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue(),expr[3]->getValue(),
                    expr[4]->getValue(),expr[5]->getValue(),
		    expr[6]->getValue());
      } // end of EvaluatorFunctionWrapper<7u>::apply

      double
      EvaluatorFunctionWrapper<7u>::apply(const EvaluatorFunctionWrapper<7u>::type1UP f,
					  const unsigned int n,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,expr[0]->getValue(),expr[1]->getValue(),
		    expr[2]->getValue(),expr[3]->getValue(),
	            expr[4]->getValue(),expr[5]->getValue(),
	            expr[6]->getValue());
      } // end of EvaluatorFunctionWrapper<7u>::apply

      double
      EvaluatorFunctionWrapper<7u>::apply(const EvaluatorFunctionWrapper<7u>::type2UP f,
					  const unsigned int n, const unsigned int m,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,m,expr[0]->getValue(),expr[1]->getValue(),
		    expr[2]->getValue(),expr[3]->getValue(),
		    expr[4]->getValue(),expr[5]->getValue(),
		    expr[6]->getValue());
      } // end of EvaluatorFunctionWrapper<7u>::apply

      double
      EvaluatorFunctionWrapper<8u>::apply(const EvaluatorFunctionWrapper<8u>::type f,
					   const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue(),expr[3]->getValue(),
                    expr[4]->getValue(),expr[5]->getValue(),
		    expr[6]->getValue(),expr[7]->getValue());
      } // end of EvaluatorFunctionWrapper<8u>::apply

      double
      EvaluatorFunctionWrapper<8u>::apply(const EvaluatorFunctionWrapper<8u>::type1P f,
					   const int n,
					   const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue(),expr[3]->getValue(),
                    expr[4]->getValue(),expr[5]->getValue(),
		    expr[6]->getValue(),expr[7]->getValue());
      } // end of EvaluatorFunctionWrapper<8u>::apply

      double
      EvaluatorFunctionWrapper<8u>::apply(const EvaluatorFunctionWrapper<8u>::type2P f,
					   const int n, const int m,
					   const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,m,expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue(),expr[3]->getValue(),
                    expr[4]->getValue(),expr[5]->getValue(),
		    expr[6]->getValue(),expr[7]->getValue());
      } // end of EvaluatorFunctionWrapper<8u>::apply

      double
      EvaluatorFunctionWrapper<8u>::apply(const EvaluatorFunctionWrapper<8u>::type1UP f,
					  const unsigned int n,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,expr[0]->getValue(),expr[1]->getValue(),
		    expr[2]->getValue(),expr[3]->getValue(),
	            expr[4]->getValue(),expr[5]->getValue(),
	            expr[6]->getValue(),expr[7]->getValue());
      } // end of EvaluatorFunctionWrapper<8u>::apply

      double
      EvaluatorFunctionWrapper<8u>::apply(const EvaluatorFunctionWrapper<8u>::type2UP f,
					  const unsigned int n, const unsigned int m,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,m,expr[0]->getValue(),expr[1]->getValue(),
		    expr[2]->getValue(),expr[3]->getValue(),
		    expr[4]->getValue(),expr[5]->getValue(),
	            expr[6]->getValue(),expr[7]->getValue());
      } // end of EvaluatorFunctionWrapper<8u>::apply

      double
      EvaluatorFunctionWrapper<9u>::apply(const EvaluatorFunctionWrapper<9u>::type f,
					   const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue(),expr[3]->getValue(),
                    expr[4]->getValue(),expr[5]->getValue(),
		    expr[6]->getValue(),expr[7]->getValue(),
		    expr[8]->getValue());
      } // end of EvaluatorFunctionWrapper<9u>::apply

      double
      EvaluatorFunctionWrapper<9u>::apply(const EvaluatorFunctionWrapper<9u>::type1P f,
					   const int n,
					   const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue(),expr[3]->getValue(),
                    expr[4]->getValue(),expr[5]->getValue(),
		    expr[6]->getValue(),expr[7]->getValue(),
		    expr[8]->getValue());
      } // end of EvaluatorFunctionWrapper<9u>::apply

      double
      EvaluatorFunctionWrapper<9u>::apply(const EvaluatorFunctionWrapper<9u>::type2P f,
					   const int n, const int m,
					   const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,m,expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue(),expr[3]->getValue(),
                    expr[4]->getValue(),expr[5]->getValue(),
		    expr[6]->getValue(),expr[7]->getValue(),
		    expr[8]->getValue());
      } // end of EvaluatorFunctionWrapper<9u>::apply

      double
      EvaluatorFunctionWrapper<9u>::apply(const EvaluatorFunctionWrapper<9u>::type1UP f,
					  const unsigned int n,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,expr[0]->getValue(),expr[1]->getValue(),
		    expr[2]->getValue(),expr[3]->getValue(),
	            expr[4]->getValue(),expr[5]->getValue(),
	            expr[6]->getValue(),expr[7]->getValue(),
		    expr[8]->getValue());
      } // end of EvaluatorFunctionWrapper<9u>::apply

      double
      EvaluatorFunctionWrapper<9u>::apply(const EvaluatorFunctionWrapper<9u>::type2UP f,
					  const unsigned int n, const unsigned int m,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,m,expr[0]->getValue(),expr[1]->getValue(),
		    expr[2]->getValue(),expr[3]->getValue(),
		    expr[4]->getValue(),expr[5]->getValue(),
	            expr[6]->getValue(),expr[7]->getValue(),
		    expr[8]->getValue());
      } // end of EvaluatorFunctionWrapper<9u>::apply

      double
      EvaluatorFunctionWrapper<10u>::apply(const EvaluatorFunctionWrapper<10u>::type f,
					   const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue(),expr[3]->getValue(),
                    expr[4]->getValue(),expr[5]->getValue(),
		    expr[6]->getValue(),expr[7]->getValue(),
		    expr[8]->getValue(),expr[9]->getValue());
      } // end of EvaluatorFunctionWrapper<10u>::apply

      double
      EvaluatorFunctionWrapper<10u>::apply(const EvaluatorFunctionWrapper<10u>::type1P f,
					   const int n,
					   const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue(),expr[3]->getValue(),
                    expr[4]->getValue(),expr[5]->getValue(),
		    expr[6]->getValue(),expr[7]->getValue(),
		    expr[8]->getValue(),expr[9]->getValue());
      } // end of EvaluatorFunctionWrapper<10u>::apply

      double
      EvaluatorFunctionWrapper<10u>::apply(const EvaluatorFunctionWrapper<10u>::type2P f,
					   const int n, const int m,
					   const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,m,expr[0]->getValue(),expr[1]->getValue(),
	            expr[2]->getValue(),expr[3]->getValue(),
                    expr[4]->getValue(),expr[5]->getValue(),
		    expr[6]->getValue(),expr[7]->getValue(),
		    expr[8]->getValue(),expr[9]->getValue());
      } // end of EvaluatorFunctionWrapper<10u>::apply

      double
      EvaluatorFunctionWrapper<10u>::apply(const EvaluatorFunctionWrapper<10u>::type1UP f,
					  const unsigned int n,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,expr[0]->getValue(),expr[1]->getValue(),
		    expr[2]->getValue(),expr[3]->getValue(),
	            expr[4]->getValue(),expr[5]->getValue(),
	            expr[6]->getValue(),expr[7]->getValue(),
		    expr[8]->getValue(),expr[9]->getValue());
      } // end of EvaluatorFunctionWrapper<10u>::apply

      double
      EvaluatorFunctionWrapper<10u>::apply(const EvaluatorFunctionWrapper<10u>::type2UP f,
					  const unsigned int n, const unsigned int m,
					  const std::vector<tfel::utilities::SmartPtr<Expr> >& expr)
      {
	return (*f)(n,m,expr[0]->getValue(),expr[1]->getValue(),
		    expr[2]->getValue(),expr[3]->getValue(),
		    expr[4]->getValue(),expr[5]->getValue(),
	            expr[6]->getValue(),expr[7]->getValue(),
		    expr[8]->getValue(),expr[9]->getValue());
      } // end of EvaluatorFunctionWrapper<10u>::apply

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
