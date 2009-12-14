/*!
 * \file   EvaluatorTExpr.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2009
 */


namespace tfel
{

  namespace math
  {

    template<typename Op>
    Evaluator::TLogicalOperation<Op>::TLogicalOperation(tfel::utilities::SmartPtr<Evaluator::TExpr> a_,
							tfel::utilities::SmartPtr<Evaluator::TExpr> b_)
      : a(a_), b(b_)
    {} // end of Evaluator::TLogicalOperation::TLogicalOperation

    template<typename Op>
    void
    Evaluator::TLogicalOperation<Op>::reduce(void)
    {
      this->a->reduce();
      this->b->reduce();
    } // end of Evaluator::TLogicalOperation::reduce

    template<typename Op>
    tfel::utilities::SmartPtr<tfel::math::parser::LogicalExpr>
    Evaluator::TLogicalOperation<Op>::analyse(void)
    {
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      return SmartPtr<LogicalExpr>(new LogicalOperation<Op>(this->a->analyse(),
							    this->b->analyse()));
    } // end of Evaluator::TLogicalOperation::TLogicalOperation::analyse(void)

    template<typename Op>
    Evaluator::TLogicalBinaryOperation<Op>::TLogicalBinaryOperation(tfel::utilities::SmartPtr<Evaluator::TLogicalExpr> a_,
								    tfel::utilities::SmartPtr<Evaluator::TLogicalExpr> b_)
      : a(a_),b(b_)
    {} // end of Evaluator::TLogicalBinaryOperation<Op>::TLogicalBinaryOperation

    template<typename Op>
    void
    Evaluator::TLogicalBinaryOperation<Op>::reduce(void)
    {
      this->a->reduce();
      this->b->reduce();
    }

    template<typename Op>
    tfel::utilities::SmartPtr<tfel::math::parser::LogicalExpr>
    Evaluator::TLogicalBinaryOperation<Op>::analyse(void)
    {
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      return SmartPtr<LogicalExpr>(new LogicalBinaryOperation<Op>(this->a->analyse(),
								  this->b->analyse()));
    } // end of Evaluator::TLogicalBinaryOperation<Op>::analyse

  } // end of namespace math

} // end of namespace tfel

