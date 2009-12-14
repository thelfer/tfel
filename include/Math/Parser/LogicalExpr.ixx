/*!
 * \file   LogicalExpr.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_LOGICALEXPR_IXX_
#define _LIB_TFEL_LOGICALEXPR_IXX_ 

#include<vector>
#include<string>
#include<stdexcept>

#include"Utilities/SmartPtr.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {

      template<typename Op>
      LogicalOperation<Op>::LogicalOperation(const tfel::utilities::SmartPtr<Expr> a_,
					     const tfel::utilities::SmartPtr<Expr> b_)
	: a(a_),b(b_)
      {} // end of LogicalOperation::LogicalOperation

      template<typename Op>
      bool
      LogicalOperation<Op>::getValue(void) const
      {
	return Op::apply(this->a->getValue(),
			 this->b->getValue());
      } // end of LogicalOperation::getValue(void) const

      template<typename Op>
      void
      LogicalOperation<Op>::checkCyclicDependency(const std::vector<std::string>& vars) const
	throw(std::runtime_error)
      {
	this->a->checkCyclicDependency(vars);
	this->b->checkCyclicDependency(vars);
      } // end of LogicalOperation::checkCyclicDependency

      template<typename Op>
      tfel::utilities::SmartPtr<LogicalExpr>
      LogicalOperation<Op>::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<LogicalExpr>(new LogicalOperation<Op>(this->a->resolveDependencies(),
							      this->b->resolveDependencies()));
      } // end of LogicalOperation<Op>::resolveDependencies(void) const

      template<typename Op>
      tfel::utilities::SmartPtr<LogicalExpr>
      LogicalOperation<Op>::clone(const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return SmartPtr<LogicalExpr>(new LogicalOperation<Op>(this->a->clone(v),
							      this->b->clone(v)));
      } // end of LogicalOperation<Op>::clone

      template<typename Op>
      LogicalOperation<Op>::~LogicalOperation()
      {} // end of LogicalOperation<Op>::~LogicalOperation()

      template<typename Op>
      LogicalBinaryOperation<Op>::LogicalBinaryOperation(const tfel::utilities::SmartPtr<LogicalExpr> a_,
							 const tfel::utilities::SmartPtr<LogicalExpr> b_)
	: a(a_),b(b_)
      {} // end of LogicalBinaryOperation::LogicalBinaryOperation

      template<typename Op>
      bool
      LogicalBinaryOperation<Op>::getValue(void) const
      {
	return Op::apply(this->a->getValue(),
			 this->b->getValue());
      } // end of LogicalBinaryOperation::getValue(void) const

      template<typename Op>
      void
      LogicalBinaryOperation<Op>::checkCyclicDependency(const std::vector<std::string>& vars) const
	throw(std::runtime_error)
      {
	this->a->checkCyclicDependency(vars);
	this->b->checkCyclicDependency(vars);
      } // end of LogicalBinaryOperation::checkCyclicDependency

      template<typename Op>
      tfel::utilities::SmartPtr<LogicalExpr>
      LogicalBinaryOperation<Op>::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<LogicalExpr>(new LogicalBinaryOperation<Op>(this->a->resolveDependencies(),
								    this->b->resolveDependencies()));
      } // end of LogicalBinaryOperation<Op>::resolveDependencies(void) const

      template<typename Op>
      tfel::utilities::SmartPtr<LogicalExpr>
      LogicalBinaryOperation<Op>::clone(const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return SmartPtr<LogicalExpr>(new LogicalBinaryOperation<Op>(this->a->clone(v),
								    this->b->clone(v)));
      } // end of LogicalBinaryOperation<Op>::clone

      template<typename Op>
      LogicalBinaryOperation<Op>::~LogicalBinaryOperation()
      {} // end of LogicalBinaryOperation<Op>::~LogicalBinaryOperation()

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_LOGICALEXPR_IXX_ */
