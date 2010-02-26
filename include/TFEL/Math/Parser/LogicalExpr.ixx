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

#include"TFEL/Utilities/SmartPtr.hxx"

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
      LogicalOperation<Op>::checkCyclicDependency(std::vector<std::string>& vars) const
      {
	using namespace std;
	vector<string> a_vars;
	vector<string> b_vars;
	this->a->checkCyclicDependency(a_vars);
	this->b->checkCyclicDependency(b_vars);
	mergeVariablesNames(vars,a_vars);
	mergeVariablesNames(vars,b_vars);
      } // end of LogicalOperation::checkCyclicDependency

      template<typename Op>
      tfel::utilities::SmartPtr<LogicalExpr>
      LogicalOperation<Op>::resolveDependencies(const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return SmartPtr<LogicalExpr>(new LogicalOperation<Op>(this->a->resolveDependencies(v),
							      this->b->resolveDependencies(v)));
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
      tfel::utilities::SmartPtr<LogicalExpr>
      LogicalOperation<Op>::createFunctionByChangingParametersIntoVariables(const std::vector<double>& v,
									    const std::vector<std::string>& p,
									    const std::map<std::string,
									    std::vector<double>::size_type>& pos) const
      {
	using namespace tfel::utilities;
	SmartPtr<Expr> na = this->a->createFunctionByChangingParametersIntoVariables(v,p,pos);
	SmartPtr<Expr> nb = this->b->createFunctionByChangingParametersIntoVariables(v,p,pos);
	return SmartPtr<LogicalExpr>(new LogicalOperation<Op>(na,nb));
      } // end of LogicalOperation<Op>::createFunctionByChangingParametersIntoVariables

      template<typename Op>
      void
      LogicalOperation<Op>::getParametersNames(std::set<std::string>& p) const
      {
	this->a->getParametersNames(p);
	this->b->getParametersNames(p);
      } // end of LogicalOperation<Op>::getParametersNames
      
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
      LogicalBinaryOperation<Op>::checkCyclicDependency(std::vector<std::string>& vars) const
      {
	using namespace std;
	vector<string> a_vars;
	vector<string> b_vars;
	this->a->checkCyclicDependency(a_vars);
	this->b->checkCyclicDependency(b_vars);
	mergeVariablesNames(vars,a_vars);
	mergeVariablesNames(vars,b_vars);
      } // end of LogicalBinaryOperation::checkCyclicDependency

      template<typename Op>
      void
      LogicalBinaryOperation<Op>::getParametersNames(std::set<std::string>& p) const
      {
	this->a->getParametersNames(p);
	this->b->getParametersNames(p);
      } // end of LogicalBinaryOperation<Op>::getParametersNames

      template<typename Op>
      tfel::utilities::SmartPtr<LogicalExpr>
      LogicalBinaryOperation<Op>::resolveDependencies(const std::vector<double>& v) const
      {
	using namespace tfel::utilities;
	return SmartPtr<LogicalExpr>(new LogicalBinaryOperation<Op>(this->a->resolveDependencies(v),
								    this->b->resolveDependencies(v)));
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
      tfel::utilities::SmartPtr<LogicalExpr>
      LogicalBinaryOperation<Op>::createFunctionByChangingParametersIntoVariables(const std::vector<double>& v,
										  const std::vector<std::string>& p,
										  const std::map<std::string,
										  std::vector<double>::size_type>& pos) const
      {
 	using namespace tfel::utilities;
	SmartPtr<LogicalExpr> na = this->a->createFunctionByChangingParametersIntoVariables(v,p,pos);
	SmartPtr<LogicalExpr> nb = this->b->createFunctionByChangingParametersIntoVariables(v,p,pos);
	return SmartPtr<LogicalExpr>(new LogicalBinaryOperation<Op>(na,nb));
      } // end of LogicalBinaryOperation<Op>::createFunctionByChangingParametersIntoVariables

      template<typename Op>
      LogicalBinaryOperation<Op>::~LogicalBinaryOperation()
      {} // end of LogicalBinaryOperation<Op>::~LogicalBinaryOperation()

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_LOGICALEXPR_IXX_ */
