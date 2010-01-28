/*!
 * \file   EvaluatorTExpr.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2009
 */

#include"TFEL/Math/Evaluator.hxx"

namespace tfel
{

  namespace math
  {

    Evaluator::TLogicalExpr::~TLogicalExpr()
    {} // end of Evaluator::TLogicalExpr::~TLogicalExpr

    Evaluator::TNegLogicalExpr::TNegLogicalExpr(const tfel::utilities::SmartPtr<Evaluator::TLogicalExpr > e_)
      : e(e_)
    {} // end of Evaluator::TNegLogicalExpr::TNegLogicalExpr

    void
    Evaluator::TNegLogicalExpr::reduce(void){
      this->e->reduce();
    } // end of Evaluator::TNegLogicalExpr::reduce
    
    tfel::utilities::SmartPtr<tfel::math::parser::LogicalExpr>
    Evaluator::TNegLogicalExpr::analyse(void){
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      return SmartPtr<LogicalExpr>(new NegLogicalExpression(this->e->analyse()));
    } // end of struct Evaluator::TNegLogicalExpr

    Evaluator::TNegLogicalExpr::~TNegLogicalExpr()
    {} // end of Evaluator::TNegLogicalExpr::~TNegLogicalExpr()

    Evaluator::TNegation::TNegation(tfel::utilities::SmartPtr<Evaluator::TExpr> e)
      : expr(e)
    {} // end of Evaluator::TNegation::TNegation

    bool
    Evaluator::TNegation::isOperator(void) const
    {
      return false;
    }
    
    tfel::utilities::SmartPtr<tfel::math::parser::Expr>
    Evaluator::TNegation::analyse(void)
    {
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      return SmartPtr<Expr>(new Negation(this->expr->analyse()));
    }
    
    std::string
    Evaluator::TNegation::getClassName(void) const
    {
      return "Evaluator::TNegation";
    }

    void
    Evaluator::TNegation::reduce(void)
    {
      this->expr->reduce();
    }

    Evaluator::TNegation::~TNegation()
    {}

    Evaluator::TOperator::TOperator(const std::string& t_)
      : type(t_)
    {} // end of Evaluator::TOperator::TOperator(const std::string& t_)
    
    std::string
    Evaluator::TOperator::getOperatorType(void) const
    {
      return this->type;
    } // end of Evaluator::TOperator::getOperatorType(void) const
     
    bool
    Evaluator::TOperator::isOperator(void) const
    {
      return true;
    } // end of Evaluator::TOperator::isOperator(void) const
    
    void
    Evaluator::TOperator::reduce(void)
    {} // end of Evaluator::TOperator::reduce(void)
    
    std::string
    Evaluator::TOperator::getClassName(void) const
    {
      return "Evaluator::TOperator";
    } // end of Evaluator::TOperator::getClassName(void) const
    
    tfel::utilities::SmartPtr<tfel::math::parser::Expr>
    Evaluator::TOperator::analyse(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      string msg("Evaluator::TOperator : invalid call");
      throw(runtime_error(msg));
      return SmartPtr<Expr>(0);
    } // end of Evaluator::TOperator::analyse(void)

    Evaluator::TBinaryOperation::TBinaryOperation(tfel::utilities::SmartPtr<Evaluator::TExpr> a_,
						  const tfel::utilities::SmartPtr<TOperator>op_,
						  tfel::utilities::SmartPtr<Evaluator::TExpr> b_)
      : a(a_), op(op_), b(b_)
    {} // end of Evaluator::TBinaryOperation::TBinaryOperation
    
    bool
    Evaluator::TBinaryOperation::isOperator(void) const
    {
      return false;
    } // end of Evaluator::TBinaryOperation::isOperator(void) const
    
    std::string
    Evaluator::TBinaryOperation::getClassName(void) const
    {
      return "Evaluator::TBinaryOperation";
    } // end of Evaluator::TBinaryOperation::getClassName(void) const
    
    void
    Evaluator::TBinaryOperation::reduce(void)
    {
      a->reduce();
      b->reduce();
    } // end of Evaluator::TBinaryOperation::reduce(void)
     
    tfel::utilities::SmartPtr<tfel::math::parser::Expr>
    Evaluator::TBinaryOperation::analyse(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      if(op->getOperatorType()=="+"){
	return SmartPtr<Expr>(new BinaryOperation<OpPlus>(a->analyse(),b->analyse()));
      } else if(op->getOperatorType()=="-"){
	return SmartPtr<Expr>(new BinaryOperation<OpMinus>(a->analyse(),b->analyse()));
      } else if(op->getOperatorType()=="*"){
	return SmartPtr<Expr>(new BinaryOperation<OpMult>(a->analyse(),b->analyse()));
      } else if(op->getOperatorType()=="/"){
	return SmartPtr<Expr>(new BinaryOperation<OpDiv>(a->analyse(),b->analyse()));
      } else if(op->getOperatorType()=="**"){
	return SmartPtr<Expr>(new BinaryOperation<OpPower>(a->analyse(),b->analyse()));
      } else {
	string msg("Evaluator::TBinaryOperation : ");
	msg += "invalid operation type  '"+op->getOperatorType()+"'";
	throw(runtime_error(msg));
      }
      return SmartPtr<Expr>(0);
    } // end of Evaluator::TBinaryOperation::analyse(void)
    
    Evaluator::TBinaryOperation::~TBinaryOperation()
    {} // end of Evaluator::TBinaryOperation::~TBinaryOperation()

    Evaluator::TVariable::TVariable(const std::string& name,
				    Evaluator & e)
      : vars(e.variables), pos(e.registerVariable(name))
    {} // end of Evaluator::TVariable::TVariable


    Evaluator::TVariable::TVariable(const std::vector<double>::size_type ppos,
				    std::vector<double>& vvars)
      : vars(vvars), pos(ppos)
    {}
    
    bool
    Evaluator::TVariable::isOperator(void) const
    {
      return false;
    }
    
    std::string
    Evaluator::TVariable::getClassName(void) const
    {
      return "Evaluator::TVariable";
    }
    
    void
    Evaluator::TVariable::reduce(void)
    {}

    tfel::utilities::SmartPtr<tfel::math::parser::Expr>
    Evaluator::TVariable::analyse(void)
    {
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      return SmartPtr<Expr>(new Variable(this->vars,this->pos));
    }  

    bool
    Evaluator::TGroup::isOperator(void) const
    {
      return false;
    }
    
    void
    Evaluator::TGroup::add(tfel::utilities::SmartPtr<Evaluator::TExpr>const e)
    {
      this->subExpr.push_back(e);
    } // end of Evaluator::TGroup::add

    void
    Evaluator::TGroup::reduce(void)
    {
      using namespace std;
      vector<tfel::utilities::SmartPtr<Evaluator::TExpr> >::iterator p  = this->subExpr.begin();
      vector<tfel::utilities::SmartPtr<Evaluator::TExpr> >::iterator pe = this->subExpr.end();
      while(p!=pe){
	(*p)->reduce();
	++p;
      }
      // ok, this is the tricky part
      // operator** has the highest priority
      this->reduce("**");
      // treating operator/
      this->reduce("/");
      // treating operator*
      this->reduce("*");
      // treating operator* -
      this->reduce("-");
      // operator+ has the lowest priority
      this->reduce("+");
    }
    
    tfel::utilities::SmartPtr<tfel::math::parser::Expr>
    Evaluator::TGroup::analyse(void)
    {
      using namespace std;
      if(this->subExpr.size()!=1u){
	string msg("TGroup::analyse : tgroup has not been reduced.");
	throw(runtime_error(msg));
      }
      return (this->subExpr[0])->analyse();
    }
    
    std::string
    Evaluator::TGroup::getClassName(void) const
    {
      return "Evaluator::TGroup";
    }
    
    Evaluator::TGroup::~TGroup()
    {}

    void
    Evaluator::TGroup::reduce(const std::string& op)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      vector<SmartPtr<Evaluator::TExpr> >::iterator p  = this->subExpr.begin();
      vector<SmartPtr<Evaluator::TExpr> >::iterator pe = this->subExpr.end();
      vector<SmartPtr<Evaluator::TExpr> >::iterator previous;
      vector<SmartPtr<Evaluator::TExpr> >::iterator next;
      
      while(p!=pe){
	if((*p)->isOperator()){
	  SmartPtr<TOperator> o = SmartPtr<TOperator>(new TOperator(static_cast<const TOperator &>(*(p->getPtr()))));
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
		*next = SmartPtr<Evaluator::TExpr>(new TNegation(*next));
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
		SmartPtr<TOperator> po = SmartPtr<TOperator>(dynamic_cast<TOperator *>(previous->getPtr()));
		if(po->getOperatorType()!="+"){
		  string msg("TGroup::reduce group two successive operators");
		  throw(runtime_error(msg));
		}
		if((*next)->isOperator()){
		  string msg("TGroup::reduce group three successive operators");
		  throw(runtime_error(msg));
		}
		*p = SmartPtr<Evaluator::TExpr>(new TNegation(*next));
		p=this->subExpr.erase(next);
		--p;
	      } else {  
		if((*next)->isOperator()){
		  if(op=="-"){
		    string msg("TGroup::reduce group two successive operators");
		    throw(runtime_error(msg));
		  }
		  TOperator * const no = dynamic_cast<TOperator * const>(next->getPtr());
		  if(no->getOperatorType()!="-"){
		    string msg("TGroup::reduce group two successive operators");
		    throw(runtime_error(msg));
		  }
		  vector<SmartPtr<Evaluator::TExpr> >::iterator nnext = next+1;
		  if(nnext==this->subExpr.end()){
		    string msg("TGroup::reduce group ends by operator "+op);
		    throw(runtime_error(msg));
		  }
		  if((*nnext)->isOperator()){
		    string msg("TGroup::reduce group two successive operators");
		    throw(runtime_error(msg));
		  }
		  *nnext = SmartPtr<Evaluator::TExpr>(new TNegation(*nnext));
		  next=this->subExpr.erase(next);
		  p = next-1;
		  previous=next-2;
		}
		*previous = SmartPtr<Evaluator::TExpr>(new TBinaryOperation(*previous,o,*next));
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
    } // end of Evaluator::TGroup::reduce
    
    Evaluator::TFunction::TFunction(Evaluator::FunctionGenerator f_,
				    tfel::utilities::SmartPtr<Evaluator::TExpr> g_)
      : f(f_), arg(g_)
    {}
    
    bool
    Evaluator::TFunction::isOperator(void) const
    {
      return false;
    }

    tfel::utilities::SmartPtr<tfel::math::parser::Expr>
    Evaluator::TFunction::analyse(void)
    {
      return (this->f)(this->arg->analyse());
    }
    
    void
    Evaluator::TFunction::reduce(void){
      this->arg->reduce();
    }
    
    std::string
    Evaluator::TFunction::getClassName(void) const
    {
      return "Evaluator::TFunction";
    }
    
    Evaluator::TFunction::~TFunction()
    {} // end of Evaluator::TFunction::~TFunction()

    Evaluator::TBinaryFunction::TBinaryFunction(Evaluator::BinaryFunctionGenerator f_,
						tfel::utilities::SmartPtr<Evaluator::TExpr> a1_,
						tfel::utilities::SmartPtr<Evaluator::TExpr> a2_)
      : f(f_),arg1(a1_),arg2(a2_)
    {}
    
    bool
    Evaluator::TBinaryFunction::isOperator(void) const
    {
      return false;
    }
    
    tfel::utilities::SmartPtr<tfel::math::parser::Expr>
    Evaluator::TBinaryFunction::analyse(void)
    {
      return (this->f)(this->arg1->analyse(),
		       this->arg2->analyse());
    }
    
    void
    Evaluator::TBinaryFunction::reduce(void){
      this->arg1->reduce();
      this->arg2->reduce();
    }
     
    std::string
    Evaluator::TBinaryFunction::getClassName(void) const
    {
      return "Evaluator::TBinaryFunction";
    }

    Evaluator::TBinaryFunction::~TBinaryFunction()
    {}

    Evaluator::TNumber::TNumber(const double v)
      : value(v)
    {}
    
    bool
    Evaluator::TNumber::isOperator(void) const
    {
      return false;
    }
    
    std::string
    Evaluator::TNumber::getClassName(void) const
    {
      return "Evaluator::TNumber";
    }
    
    tfel::utilities::SmartPtr<tfel::math::parser::Expr>
    Evaluator::TNumber::analyse(void)
    {
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      return SmartPtr<Expr>(new tfel::math::parser::Number(value));
    }
    
    void
    Evaluator::TNumber::reduce(void)
    {}

    Evaluator::TExternalFunctionExpr::TExternalFunctionExpr(const std::string& fname,
							    std::vector<tfel::utilities::SmartPtr<Evaluator::TExpr> >& fargs,
							    tfel::utilities::SmartPtr<tfel::math::parser::ExternalFunctionManager>& m)
      : name(fname),
	args(fargs),
	manager(m)
    {} // end of TExternalFunctionExpr

    bool
    Evaluator::TExternalFunctionExpr::isOperator(void) const
    {
      return false;
    }

    std::string
    Evaluator::TExternalFunctionExpr::getClassName(void) const
    {
      return "Evaluator::TExternalFunctionExpr";
    }
    
    tfel::utilities::SmartPtr<tfel::math::parser::Expr>
    Evaluator::TExternalFunctionExpr::analyse(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      vector<SmartPtr<Evaluator::TExpr> >::iterator p;
      vector<SmartPtr<Expr> > fargs;
      for(p=this->args.begin();p!=this->args.end();++p){
	fargs.push_back((*p)->analyse());
      }
      return SmartPtr<Expr>(new ExternalFunctionExpr(name,fargs,this->manager));
    }
      
    void
    Evaluator::TExternalFunctionExpr::reduce(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      vector<SmartPtr<Evaluator::TExpr> >::iterator p;
      for(p=this->args.begin();p!=this->args.end();++p){
	(*p)->reduce();
      }
    }

    Evaluator::TDifferentiatedFunctionExpr::TDifferentiatedFunctionExpr(tfel::utilities::SmartPtr<ExternalFunction> ff,
									std::vector<tfel::utilities::SmartPtr<Evaluator::TExpr> >& fargs,
									const std::vector<std::vector<double>::size_type>& fvar)
      : f(ff),
	args(fargs),
	var(fvar)
    {} // end of TDifferentiatedFunctionExpr

    bool
    Evaluator::TDifferentiatedFunctionExpr::isOperator(void) const
    {
      return false;
    }
    
    std::string
    Evaluator::TDifferentiatedFunctionExpr::getClassName(void) const
    {
      return "Evaluator::TDifferentiatedFunctionExpr";
    }
    
    tfel::utilities::SmartPtr<tfel::math::parser::Expr>
    Evaluator::TDifferentiatedFunctionExpr::analyse(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      vector<SmartPtr<Evaluator::TExpr> >::iterator p;
      vector<SmartPtr<Expr> > fargs;
      for(p=this->args.begin();p!=this->args.end();++p){
	fargs.push_back((*p)->analyse());
      }
      return SmartPtr<Expr>(new DifferentiatedFunctionExpr(this->f,fargs,this->var));
    }

    void
    Evaluator::TDifferentiatedFunctionExpr::reduce(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      vector<SmartPtr<Evaluator::TExpr> >::iterator p;
      for(p=this->args.begin();p!=this->args.end();++p){
	(*p)->reduce();
      }
    }

    Evaluator::TConditionalExpr::TConditionalExpr(tfel::utilities::SmartPtr<Evaluator::TLogicalExpr> c_,
						  tfel::utilities::SmartPtr<Evaluator::TExpr> a_,
						  tfel::utilities::SmartPtr<Evaluator::TExpr> b_)
      : c(c_),
	a(a_),
	b(b_)
    {} // end of Evaluator::TConditionalExpr::TConditionalExpr

    bool
    Evaluator::TConditionalExpr::isOperator(void) const
    {
      return false;
    } // end of Evaluator::TConditionalExpr::isOperator
    
    std::string
    Evaluator::TConditionalExpr::getClassName(void) const
    {
      return "Evaluator::TConditionalExpr";
    } // end of Evaluator::TConditionalExpr::getClassName
    
    void
    Evaluator::TConditionalExpr::reduce(void)
    {
      this->c->reduce();
      this->a->reduce();
      this->b->reduce();
    } // end of Evaluator::TConditionalExpr::reduce

    tfel::utilities::SmartPtr<tfel::math::parser::Expr>
    Evaluator::TConditionalExpr::analyse(void)
    {
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      return SmartPtr<Expr>(new ConditionalExpr(this->c->analyse(),
						this->a->analyse(),
						this->b->analyse()));
    } // end of Evaluator::TConditionalExpr::analyse

    Evaluator::TConditionalExpr::~TConditionalExpr()
    {} // end of Evaluator::TConditionalExpr::~TConditionalExpr()

    Evaluator::TExternalOperator::TExternalOperator(const Evaluator::ExternalFunctionGenerator f_,
						    const std::vector<std::string>& param_,
						    std::vector<tfel::utilities::SmartPtr<Evaluator::TExpr> >& a_)
      : f(f_),param(param_),args(a_)
    {} // end of Evaluator::TExternalOperator::TExternalOperator

    bool
    Evaluator::TExternalOperator::isOperator(void) const
    {
      return false;
    } // end of Evaluator::TExternalOperator::isOperator(void) const
    
    tfel::utilities::SmartPtr<tfel::math::parser::Expr>
    Evaluator::TExternalOperator::analyse(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      vector<SmartPtr<Expr> > fargs;
      vector<SmartPtr<Evaluator::TExpr> >::iterator p;
      for(p=this->args.begin();p!=this->args.end();++p){
	fargs.push_back((*p)->analyse());
      }
      return (*(this->f))(this->param,fargs);
    } // end of Evaluator::TExternalOperator::analyse(void)
    
    void
    Evaluator::TExternalOperator::reduce(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      vector<SmartPtr<Evaluator::TExpr> >::iterator p;
      for(p=this->args.begin();p!=this->args.end();++p){
	(*p)->reduce();
      }
    } // end of Evaluator::TExternalOperator::reduce(void)
    
    std::string
    Evaluator::TExternalOperator::getClassName(void) const
    {
      return "Evaluator::TExternalOperator";
    } // end of Evaluator::TExternalOperator::getClassName
    
    Evaluator::TExternalOperator::~TExternalOperator()
    {} // end of Evaluator::TExternalOperator::~TExternalOperator()

  } // end of namespace math

} // end of namespace tfel
