/*!
 * \file   src/Math/EvaluatorTExpr.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2009
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<iostream>
#include"TFEL/Math/Evaluator.hxx"

namespace tfel
{

  namespace math
  {

    Evaluator::TExpr::~TExpr()
    {} // end of Evaluator::TExpr::~TExpr

    Evaluator::TLogicalExpr::~TLogicalExpr()
    {} // end of Evaluator::TLogicalExpr::~TLogicalExpr

    Evaluator::TNegLogicalExpr::TNegLogicalExpr(const std::shared_ptr<Evaluator::TLogicalExpr > e_)
      : e(e_)
    {} // end of Evaluator::TNegLogicalExpr::TNegLogicalExpr

    void
    Evaluator::TNegLogicalExpr::reduce(void){
      this->e->reduce();
    } // end of Evaluator::TNegLogicalExpr::reduce
    
    std::shared_ptr<tfel::math::parser::LogicalExpr>
    Evaluator::TNegLogicalExpr::analyse(void){
      using namespace tfel::math::parser;
      return std::shared_ptr<LogicalExpr>(new NegLogicalExpression(this->e->analyse()));
    } // end of struct Evaluator::TNegLogicalExpr

    Evaluator::TNegLogicalExpr::~TNegLogicalExpr()
    {} // end of Evaluator::TNegLogicalExpr::~TNegLogicalExpr()

    Evaluator::TNegation::TNegation(std::shared_ptr<Evaluator::TExpr> e)
      : expr(e)
    {} // end of Evaluator::TNegation::TNegation

    bool
    Evaluator::TNegation::isOperator(void) const
    {
      return false;
    }
    
    parser::ExprPtr
    Evaluator::TNegation::analyse(void)
    {
      using namespace tfel::math::parser;
      return ExprPtr(new Negation(this->expr->analyse()));
    }

    void
    Evaluator::TNegation::reduce(void)
    {
      this->expr->reduce();
    }

    Evaluator::TNegation::~TNegation() noexcept
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
    
    parser::ExprPtr
    Evaluator::TOperator::analyse(void)
    {
      using namespace std;
      string msg("Evaluator::TOperator : invalid call");
      throw(runtime_error(msg));
      return parser::ExprPtr();
    } // end of Evaluator::TOperator::analyse(void)

    Evaluator::TOperator::~TOperator() noexcept
    {}

    Evaluator::TBinaryOperation::TBinaryOperation(std::shared_ptr<Evaluator::TExpr> a_,
						  const std::shared_ptr<TOperator>op_,
						  std::shared_ptr<Evaluator::TExpr> b_)
      : a(a_), op(op_), b(b_)
    {} // end of Evaluator::TBinaryOperation::TBinaryOperation
    
    bool
    Evaluator::TBinaryOperation::isOperator(void) const
    {
      return false;
    } // end of Evaluator::TBinaryOperation::isOperator(void) const
    
    void
    Evaluator::TBinaryOperation::reduce(void)
    {
      a->reduce();
      b->reduce();
    } // end of Evaluator::TBinaryOperation::reduce(void)
     
    parser::ExprPtr
    Evaluator::TBinaryOperation::analyse(void)
    {
      using namespace std;
      using namespace tfel::math::parser;
      if(op->getOperatorType()=="+"){
	return ExprPtr(new BinaryOperation<OpPlus>(a->analyse(),b->analyse()));
      } else if(op->getOperatorType()=="-"){
	return ExprPtr(new BinaryOperation<OpMinus>(a->analyse(),b->analyse()));
      } else if(op->getOperatorType()=="*"){
	return ExprPtr(new BinaryOperation<OpMult>(a->analyse(),b->analyse()));
      } else if(op->getOperatorType()=="/"){
	return ExprPtr(new BinaryOperation<OpDiv>(a->analyse(),b->analyse()));
      } else if(op->getOperatorType()=="**"){
	return ExprPtr(new BinaryOperation<OpPower>(a->analyse(),b->analyse()));
      } else {
	string msg("Evaluator::TBinaryOperation : ");
	msg += "invalid operation type  '"+op->getOperatorType()+"'";
	throw(runtime_error(msg));
      }
      return ExprPtr();
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
    
    void
    Evaluator::TVariable::reduce(void)
    {}

    parser::ExprPtr
    Evaluator::TVariable::analyse(void)
    {
      using namespace tfel::math::parser;
      return ExprPtr(new Variable(this->vars,this->pos));
    }  

    bool
    Evaluator::TGroup::isOperator(void) const
    {
      return false;
    }
    
    void
    Evaluator::TGroup::add(std::shared_ptr<Evaluator::TExpr>const e)
    {
      this->subExpr.push_back(e);
    } // end of Evaluator::TGroup::add

    void
    Evaluator::TGroup::reduce(void)
    {
      using namespace std;
      vector<std::shared_ptr<Evaluator::TExpr>>::iterator p  = this->subExpr.begin();
      vector<std::shared_ptr<Evaluator::TExpr>>::iterator pe = this->subExpr.end();
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
    
    parser::ExprPtr
    Evaluator::TGroup::analyse(void)
    {
      using namespace std;
      if(this->subExpr.size()!=1u){
	string msg("TGroup::analyse : tgroup has not been reduced.");
	throw(runtime_error(msg));
      }
      return (this->subExpr[0])->analyse();
    }
    
    Evaluator::TGroup::~TGroup()
    {}

    void
    Evaluator::TGroup::reduce(const std::string& op)
    {
      using namespace std;
      using namespace tfel::math::parser;
      vector<shared_ptr<Evaluator::TExpr>>::iterator p  = this->subExpr.begin();
      vector<shared_ptr<Evaluator::TExpr>>::iterator previous;
      vector<shared_ptr<Evaluator::TExpr>>::iterator next;
      while(p!=this->subExpr.end()){
	if((*p)->isOperator()){
	  auto o = make_shared<TOperator>(static_cast<const TOperator &>(*(p->get())));
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
		*next = shared_ptr<Evaluator::TExpr>(new TNegation(*next));
		cout << "*next : " << *next << endl;
		this->subExpr.erase(p);
		p  = this->subExpr.begin();
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
		cout << "HERE" << endl;
		*p = shared_ptr<Evaluator::TExpr>(new TNegation(*next));
		cout << "*p : " << p->get() << endl;
		cout << "*p : " << next->get() << endl;
		p=this->subExpr.erase(next);
		cout << "*p : " << next->get() << endl;
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
		  vector<shared_ptr<Evaluator::TExpr>>::iterator nnext = next+1;
		  if(nnext==this->subExpr.end()){
		    string msg("TGroup::reduce group ends by operator "+op);
		    throw(runtime_error(msg));
		  }
		  if((*nnext)->isOperator()){
		    string msg("TGroup::reduce group two successive operators");
		    throw(runtime_error(msg));
		  }
		  *nnext = shared_ptr<Evaluator::TExpr>(new TNegation(*nnext));
		  next=this->subExpr.erase(next);
		  p = next-1;
		  previous=next-2;
		}
		*previous = shared_ptr<Evaluator::TExpr>(new TBinaryOperation(*previous,o,*next));
		++next;
		p=this->subExpr.erase(p,next);
		--p;
	      }
	    }
	  }
	}
	++p;
      }
    } // end of Evaluator::TGroup::reduce
    
    Evaluator::TFunction::TFunction(Evaluator::FunctionGenerator f_,
				    std::shared_ptr<Evaluator::TExpr> g_)
      : f(f_), arg(g_)
    {}
    
    bool
    Evaluator::TFunction::isOperator(void) const
    {
      return false;
    }

    parser::ExprPtr
    Evaluator::TFunction::analyse(void)
    {
      return (this->f)(this->arg->analyse());
    }
    
    void
    Evaluator::TFunction::reduce(void){
      this->arg->reduce();
    }
    
    Evaluator::TFunction::~TFunction()
    {} // end of Evaluator::TFunction::~TFunction()

    Evaluator::TBinaryFunction::TBinaryFunction(Evaluator::BinaryFunctionGenerator f_,
						std::shared_ptr<Evaluator::TExpr> a1_,
						std::shared_ptr<Evaluator::TExpr> a2_)
      : f(f_),arg1(a1_),arg2(a2_)
    {}
    
    bool
    Evaluator::TBinaryFunction::isOperator(void) const
    {
      return false;
    }
    
    parser::ExprPtr
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
    
    parser::ExprPtr
    Evaluator::TNumber::analyse(void)
    {
      using namespace tfel::math::parser;
      return ExprPtr(new tfel::math::parser::Number(value));
    }
    
    void
    Evaluator::TNumber::reduce(void)
    {}

    Evaluator::TExternalFunctionExpr::TExternalFunctionExpr(const std::string& fname,
							    std::vector<std::shared_ptr<Evaluator::TExpr>>& fargs,
							    std::shared_ptr<tfel::math::parser::ExternalFunctionManager>& m)
      : name(fname),
	args(fargs),
	manager(m)
    {} // end of TExternalFunctionExpr

    bool
    Evaluator::TExternalFunctionExpr::isOperator(void) const
    {
      return false;
    }

    parser::ExprPtr
    Evaluator::TExternalFunctionExpr::analyse(void)
    {
      using namespace std;
      using namespace tfel::math::parser;
      vector<shared_ptr<Evaluator::TExpr>>::iterator p;
      vector<ExprPtr> fargs;
      for(p=this->args.begin();p!=this->args.end();++p){
	fargs.push_back((*p)->analyse());
      }
      return ExprPtr(new ExternalFunctionExpr(name,fargs,this->manager));
    }
      
    void
    Evaluator::TExternalFunctionExpr::reduce(void)
    {
      for(auto& a : this->args){
	a->reduce();
      }
    }

    Evaluator::TExternalFunctionExpr::~TExternalFunctionExpr() noexcept
    {} // end of ~TExternalFunctionExpr

    Evaluator::TDifferentiatedFunctionExpr::TDifferentiatedFunctionExpr(std::shared_ptr<ExternalFunction> ff,
									std::vector<std::shared_ptr<Evaluator::TExpr>>& fargs,
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
    
    parser::ExprPtr
    Evaluator::TDifferentiatedFunctionExpr::analyse(void)
    {
      using namespace std;
      using namespace tfel::math::parser;
      vector<shared_ptr<Evaluator::TExpr>>::iterator p;
      vector<ExprPtr> fargs;
      for(p=this->args.begin();p!=this->args.end();++p){
	fargs.push_back((*p)->analyse());
      }
      return ExprPtr(new DifferentiatedFunctionExpr(this->f,fargs,this->var));
    }

    void
    Evaluator::TDifferentiatedFunctionExpr::reduce(void)
    {
      using namespace std;
      vector<shared_ptr<Evaluator::TExpr>>::iterator p;
      for(p=this->args.begin();p!=this->args.end();++p){
	(*p)->reduce();
      }
    }

    Evaluator::TConditionalExpr::TConditionalExpr(std::shared_ptr<Evaluator::TLogicalExpr> c_,
						  std::shared_ptr<Evaluator::TExpr> a_,
						  std::shared_ptr<Evaluator::TExpr> b_)
      : c(c_),
	a(a_),
	b(b_)
    {} // end of Evaluator::TConditionalExpr::TConditionalExpr

    bool
    Evaluator::TConditionalExpr::isOperator(void) const
    {
      return false;
    } // end of Evaluator::TConditionalExpr::isOperator
    
    void
    Evaluator::TConditionalExpr::reduce(void)
    {
      this->c->reduce();
      this->a->reduce();
      this->b->reduce();
    } // end of Evaluator::TConditionalExpr::reduce

    parser::ExprPtr
    Evaluator::TConditionalExpr::analyse(void)
    {
      using namespace tfel::math::parser;
      return ExprPtr(new ConditionalExpr(this->c->analyse(),
					 this->a->analyse(),
					 this->b->analyse()));
    } // end of Evaluator::TConditionalExpr::analyse

    Evaluator::TDifferentiatedFunctionExpr::~TDifferentiatedFunctionExpr() noexcept
    {} // end of Evaluator::TDifferentiatedFunctionExpr::~TDifferentiatedFunctionExpr

    Evaluator::TConditionalExpr::~TConditionalExpr()
    {} // end of Evaluator::TConditionalExpr::~TConditionalExpr()

    Evaluator::TExternalOperator::TExternalOperator(const Evaluator::ExternalFunctionGenerator f_,
						    const std::vector<std::string>& param_,
						    std::vector<std::shared_ptr<Evaluator::TExpr>>& a_)
      : f(f_),param(param_),args(a_)
    {} // end of Evaluator::TExternalOperator::TExternalOperator

    bool
    Evaluator::TExternalOperator::isOperator(void) const
    {
      return false;
    } // end of Evaluator::TExternalOperator::isOperator(void) const
    
    parser::ExprPtr
    Evaluator::TExternalOperator::analyse(void)
    {
      using namespace std;
      using namespace tfel::math::parser;
      vector<ExprPtr> fargs;
      for(auto& a : this->args){
	fargs.push_back(a->analyse());
      }
      return (*(this->f))(this->param,fargs);
    } // end of Evaluator::TExternalOperator::analyse(void)
    
    void
    Evaluator::TExternalOperator::reduce(void)
    {
      for(auto& a : this->args){
	a->reduce();
      }
    } // end of Evaluator::TExternalOperator::reduce(void)
    
    Evaluator::TExternalOperator::~TExternalOperator()
    {} // end of Evaluator::TExternalOperator::~TExternalOperator()

  } // end of namespace math

} // end of namespace tfel
