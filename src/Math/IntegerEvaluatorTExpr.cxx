/*!
 * \file   IntegerEvaluatorTExpr.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2009
 */

#include"TFEL/Math/IntegerEvaluator.hxx"
#include"TFEL/Math/Parser/IntegerEvaluatorTExpr.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser{

      IntegerExpr::~IntegerExpr()
      {} // end of IntegerExpr::~IntegerExpr

    }
    
    IntegerEvaluator::TNegation::TNegation(tfel::utilities::shared_ptr<IntegerEvaluator::TExpr> e)
      : expr(e)
    {} // end of IntegerEvaluator::TNegation::TNegation

    bool
    IntegerEvaluator::TNegation::isOperator(void) const
    {
      return false;
    }
    
    tfel::utilities::shared_ptr<tfel::math::parser::IntegerExpr>
    IntegerEvaluator::TNegation::analyse(void)
    {
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      return shared_ptr<IntegerExpr>(new Negation(this->expr->analyse()));
    }
    
    std::string
    IntegerEvaluator::TNegation::getClassName(void) const
    {
      return "IntegerEvaluator::TNegation";
    }

    void
    IntegerEvaluator::TNegation::reduce(void)
    {
      this->expr->reduce();
    }

    IntegerEvaluator::TNegation::~TNegation()
    {}

    IntegerEvaluator::TNegation::Negation::Negation(const tfel::utilities::shared_ptr<tfel::math::parser::IntegerExpr> e)
      : expr(e)
    {} // end of IntegerEvaluator::TNegation::Negation::Negation
    
    int
    IntegerEvaluator::TNegation::Negation::getValue(void) const
    {
      return -(this->expr->getValue());
    } // end of IntegerEvaluator::TNegation::Negation::getValue(void)
    
    tfel::utilities::shared_ptr<tfel::math::parser::IntegerExpr>
    IntegerEvaluator::TNegation::Negation::clone(const std::vector<int>& v) const
    {
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      return shared_ptr<IntegerExpr>(new Negation(this->expr->clone(v)));
    }
    
    IntegerEvaluator::TNegation::Negation::~Negation()
    {} // end of IntegerEvaluator::TNegation::Negation::~Negation()
    
    IntegerEvaluator::TOperator::TOperator(const std::string& t_)
      : type(t_)
    {} // end of IntegerEvaluator::TOperator::TOperator(const std::string& t_)
    
    std::string
    IntegerEvaluator::TOperator::getOperatorType(void) const
    {
      return this->type;
    } // end of IntegerEvaluator::TOperator::getOperatorType(void) const
     
    bool
    IntegerEvaluator::TOperator::isOperator(void) const
    {
      return true;
    } // end of IntegerEvaluator::TOperator::isOperator(void) const
    
    void
    IntegerEvaluator::TOperator::reduce(void)
    {} // end of IntegerEvaluator::TOperator::reduce(void)
    
    std::string
    IntegerEvaluator::TOperator::getClassName(void) const
    {
      return "IntegerEvaluator::TOperator";
    } // end of IntegerEvaluator::TOperator::getClassName(void) const
    
    tfel::utilities::shared_ptr<tfel::math::parser::IntegerExpr>
    IntegerEvaluator::TOperator::analyse(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      string msg("IntegerEvaluator::TOperator : invalid call");
      throw(runtime_error(msg));
      return shared_ptr<IntegerExpr>(static_cast<IntegerExpr*>(0));
    } // end of IntegerEvaluator::TOperator::analyse(void)

    int
    IntegerEvaluator::TBinaryOperation::IntegerOpPlus::apply(const int a,
							     const int b)
    {
      return a+b;
    }

    int
    IntegerEvaluator::TBinaryOperation::IntegerOpMinus::apply(const int a,
							      const int b)
    {
      return a-b;
    }

    int
    IntegerEvaluator::TBinaryOperation::IntegerOpMult::apply(const int a,
							     const int b)
    {
      return a*b;
    }

    int
    IntegerEvaluator::TBinaryOperation::IntegerOpDiv::apply(const int a,
							    const int b)
    {
      return a/b;
    }

    template<typename Op>
    IntegerEvaluator::TBinaryOperation::BinaryOperation<Op>::BinaryOperation(const tfel::utilities::shared_ptr<tfel::math::parser::IntegerExpr> a_,
									     const tfel::utilities::shared_ptr<tfel::math::parser::IntegerExpr> b_)
      : a(a_),b(b_)
    {} // end of IntegerEvaluator::TBinaryOperation::BinaryOperation<Op>::BinaryOperation 
      
    template<typename Op>
    IntegerEvaluator::TBinaryOperation::BinaryOperation<Op>::~BinaryOperation()
    {} // end of IntegerEvaluator::TBinaryOperation::BinaryOperation<Op>::~BinaryOperation()
      
    template<typename Op>
    int
    IntegerEvaluator::TBinaryOperation::BinaryOperation<Op>::getValue(void) const
    {
      return Op::apply(this->a->getValue(),this->b->getValue());
    } // end of IntegerEvaluator::TBinaryOperation::BinaryOperation<Op>::getValue

    template<typename Op>
    tfel::utilities::shared_ptr<tfel::math::parser::IntegerExpr>
    IntegerEvaluator::TBinaryOperation::BinaryOperation<Op>::clone(const std::vector<int>& v) const
    {
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      return shared_ptr<IntegerExpr>(new IntegerEvaluator::TBinaryOperation::BinaryOperation<Op>(this->a->clone(v),
												 this->b->clone(v)));
    } // end of IntegerEvaluator::TBinaryOperation::BinaryOperation<Op>::clone
    
    IntegerEvaluator::TBinaryOperation::TBinaryOperation(tfel::utilities::shared_ptr<IntegerEvaluator::TExpr> a_,
							 const tfel::utilities::shared_ptr<TOperator>op_,
							 tfel::utilities::shared_ptr<IntegerEvaluator::TExpr> b_)
      : a(a_), op(op_), b(b_)
    {} // end of IntegerEvaluator::TBinaryOperation::TBinaryOperation
    
    bool
    IntegerEvaluator::TBinaryOperation::isOperator(void) const
    {
      return false;
    } // end of IntegerEvaluator::TBinaryOperation::isOperator(void) const
    
    std::string
    IntegerEvaluator::TBinaryOperation::getClassName(void) const
    {
      return "IntegerEvaluator::TBinaryOperation";
    } // end of IntegerEvaluator::TBinaryOperation::getClassName(void) const
    
    void
    IntegerEvaluator::TBinaryOperation::reduce(void)
    {
      a->reduce();
      b->reduce();
    } // end of IntegerEvaluator::TBinaryOperation::reduce(void)
     
    tfel::utilities::shared_ptr<tfel::math::parser::IntegerExpr>
    IntegerEvaluator::TBinaryOperation::analyse(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      if(op->getOperatorType()=="+"){
	return shared_ptr<IntegerExpr>(new BinaryOperation<IntegerOpPlus>(a->analyse(),b->analyse()));
      } else if(op->getOperatorType()=="-"){
	return shared_ptr<IntegerExpr>(new BinaryOperation<IntegerOpMinus>(a->analyse(),b->analyse()));
      } else if(op->getOperatorType()=="*"){
	return shared_ptr<IntegerExpr>(new BinaryOperation<IntegerOpMult>(a->analyse(),b->analyse()));
      } else if(op->getOperatorType()=="/"){
	return shared_ptr<IntegerExpr>(new BinaryOperation<IntegerOpDiv>(a->analyse(),b->analyse()));
      } else {
	string msg("IntegerEvaluator::TBinaryOperation : ");
	msg += "invalid operation type  '"+op->getOperatorType()+"'";
	throw(runtime_error(msg));
      }
      return shared_ptr<IntegerExpr>(static_cast<IntegerExpr*>(0));
    } // end of IntegerEvaluator::TBinaryOperation::analyse(void)
    
    IntegerEvaluator::TBinaryOperation::~TBinaryOperation()
    {} // end of IntegerEvaluator::TBinaryOperation::~TBinaryOperation()

    IntegerEvaluator::TVariable::TVariable(const std::string& name,
					   IntegerEvaluator & e)
      : vars(e.variables), pos(e.registerVariable(name))
    {} // end of IntegerEvaluator::TVariable::TVariable


    IntegerEvaluator::TVariable::TVariable(const std::vector<int>::size_type ppos,
					   std::vector<int>& vvars)
      : vars(vvars), pos(ppos)
    {}
    
    bool
    IntegerEvaluator::TVariable::isOperator(void) const
    {
      return false;
    }
    
    std::string
    IntegerEvaluator::TVariable::getClassName(void) const
    {
      return "IntegerEvaluator::TVariable";
    }
    
    void
    IntegerEvaluator::TVariable::reduce(void)
    {}

    tfel::utilities::shared_ptr<tfel::math::parser::IntegerExpr>
    IntegerEvaluator::TVariable::analyse(void)
    {
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      return shared_ptr<IntegerExpr>(new Variable(this->vars,this->pos));
    }  

    IntegerEvaluator::TVariable::Variable::Variable(const std::vector<int>& v_,
						    const std::vector<int>::size_type p_)
      : v(v_), pos(p_)
    {} // end of IntegerEvaluator::TVariable::Variable::Variable
    
    int
    IntegerEvaluator::TVariable::Variable::getValue(void) const
    {
      return this->v[this->pos];
    } // end of IntegerEvaluator::TVariable::Variable::getValue

    tfel::utilities::shared_ptr<tfel::math::parser::IntegerExpr>
    IntegerEvaluator::TVariable::Variable::clone(const std::vector<int>& v_) const
    {
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      return shared_ptr<IntegerExpr>(new Variable(v_,this->pos));
    } // end of IntegerEvaluator::TVariable::Variable::clone

    bool
    IntegerEvaluator::TGroup::isOperator(void) const
    {
      return false;
    }
    
    void
    IntegerEvaluator::TGroup::add(tfel::utilities::shared_ptr<IntegerEvaluator::TExpr>const e)
    {
      this->subExpr.push_back(e);
    } // end of IntegerEvaluator::TGroup::add

    void
    IntegerEvaluator::TGroup::reduce(void)
    {
      using namespace std;
      vector<tfel::utilities::shared_ptr<IntegerEvaluator::TExpr> >::iterator p  = this->subExpr.begin();
      vector<tfel::utilities::shared_ptr<IntegerEvaluator::TExpr> >::iterator pe = this->subExpr.end();
      while(p!=pe){
	(*p)->reduce();
	++p;
      }
      // treating operator/
      this->reduce("/");
      // treating operator*
      this->reduce("*");
      // treating operator* -
      this->reduce("-");
      // operator+ has the lowest priority
      this->reduce("+");
    }
    
    tfel::utilities::shared_ptr<tfel::math::parser::IntegerExpr>
    IntegerEvaluator::TGroup::analyse(void)
    {
      using namespace std;
      if(this->subExpr.size()!=1u){
	string msg("TGroup::analyse : tgroup has not been reduced.");
	throw(runtime_error(msg));
      }
      return (this->subExpr[0])->analyse();
    }
    
    std::string
    IntegerEvaluator::TGroup::getClassName(void) const
    {
      return "IntegerEvaluator::TGroup";
    }
    
    IntegerEvaluator::TGroup::~TGroup()
    {}

    void
    IntegerEvaluator::TGroup::reduce(const std::string& op)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      vector<shared_ptr<IntegerEvaluator::TExpr> >::iterator p  = this->subExpr.begin();
      vector<shared_ptr<IntegerEvaluator::TExpr> >::iterator pe = this->subExpr.end();
      vector<shared_ptr<IntegerEvaluator::TExpr> >::iterator previous;
      vector<shared_ptr<IntegerEvaluator::TExpr> >::iterator next;
      
      while(p!=pe){
	if((*p)->isOperator()){
	  shared_ptr<TOperator> o = shared_ptr<TOperator>(new TOperator(static_cast<const TOperator &>(*(p->get()))));
	  if(o->getOperatorType()==op){
	    previous = p-1;
	    next     = p+1;
	    if(p==this->subExpr.begin()){
	      if(op!="-"){
		string msg("TGroup::reduce group began with an operator "+op);
		throw(runtime_error(msg));
	      } else {
		if(next==this->subExpr.end()){
		  string msg("TGroup::reduce group ends by operator "+op);
		  throw(runtime_error(msg));
		}
		if((*next)->isOperator()){
		  string msg("TGroup::reduce group two successive operators");
		  throw(runtime_error(msg));
		}
		*next = shared_ptr<IntegerEvaluator::TExpr>(new TNegation(*next));
		this->subExpr.erase(p);
		p = this->subExpr.begin();
	      }
	    } else {
	      if(next==this->subExpr.end()){
		string msg("TGroup::reduce group ends by operator "+op);
		throw(runtime_error(msg));
	      }
	      if((*previous)->isOperator()){
		if(op!="-"){
		  string msg("TGroup::reduce group two successive operators");
		  throw(runtime_error(msg));
		}
		shared_ptr<TOperator> po = shared_ptr<TOperator>(dynamic_cast<TOperator *>(previous->get()));
		if(po->getOperatorType()!="+"){
		  string msg("TGroup::reduce group two successive operators");
		  throw(runtime_error(msg));
		}
		if((*next)->isOperator()){
		  string msg("TGroup::reduce group three successive operators");
		  throw(runtime_error(msg));
		}
		*p = shared_ptr<IntegerEvaluator::TExpr>(new TNegation(*next));
		p=this->subExpr.erase(next);
		--p;
	      } else {  
		if((*next)->isOperator()){
		  if(op=="-"){
		    string msg("TGroup::reduce group two successive operators");
		    throw(runtime_error(msg));
		  }
		  TOperator * const no = dynamic_cast<TOperator *>(next->get());
		  if(no->getOperatorType()!="-"){
		    string msg("TGroup::reduce group two successive operators");
		    throw(runtime_error(msg));
		  }
		  vector<shared_ptr<IntegerEvaluator::TExpr> >::iterator nnext = next+1;
		  if(nnext==this->subExpr.end()){
		    string msg("TGroup::reduce group ends by operator "+op);
		    throw(runtime_error(msg));
		  }
		  if((*nnext)->isOperator()){
		    string msg("TGroup::reduce group two successive operators");
		    throw(runtime_error(msg));
		  }
		  *nnext = shared_ptr<IntegerEvaluator::TExpr>(new TNegation(*nnext));
		  next=this->subExpr.erase(next);
		  p = next-1;
		  previous=next-2;
		}
		*previous = shared_ptr<IntegerEvaluator::TExpr>(new TBinaryOperation(*previous,o,*next));
		++next;
		p=this->subExpr.erase(p,next);
		--p;
		pe=this->subExpr.end();
	      }
	    }
	  }
	}
	++p;
      }
    } // end of IntegerEvaluator::TGroup::reduce

    IntegerEvaluator::TNumber::TNumber(const int v)
      : value(v)
    {}
    
    bool
    IntegerEvaluator::TNumber::isOperator(void) const
    {
      return false;
    }
    
    std::string
    IntegerEvaluator::TNumber::getClassName(void) const
    {
      return "IntegerEvaluator::TNumber";
    }
    
    tfel::utilities::shared_ptr<tfel::math::parser::IntegerExpr>
    IntegerEvaluator::TNumber::analyse(void)
    {
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      return shared_ptr<IntegerExpr>(new Number(value));
    }
    
    void
    IntegerEvaluator::TNumber::reduce(void)
    {}

    IntegerEvaluator::TNumber::Number::Number(const int v)
      : value(v)
    {} // end of IntegerEvaluator::TNumber::Number::Number
    
    int
    IntegerEvaluator::TNumber::Number::getValue() const
    {
      return this->value;
    } // end of IntegerEvaluator::TNumber::Number::getValue
    
    tfel::utilities::shared_ptr<tfel::math::parser::IntegerExpr>
    IntegerEvaluator::TNumber::Number::clone(const std::vector<int>&) const
    {
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      return shared_ptr<IntegerExpr>(new Number(this->value));
    } // end of IntegerEvaluator::TNumber::Number::clone
    
  } // end of namespace math

} // end of namespace tfel
