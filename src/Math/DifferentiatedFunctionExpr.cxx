/*!
 * \file   DifferentiatedFunctionExpr.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#include<iostream>
#include<sstream>
#include<stdexcept>

#include"TFEL/Math/Parser/Number.hxx"
#include"TFEL/Math/Parser/BinaryOperator.hxx"
#include"TFEL/Math/Parser/ExternalFunctionExpr2.hxx"
#include"TFEL/Math/Parser/DifferentiatedFunctionExpr.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {
      
      DifferentiatedFunctionExpr::DifferentiatedFunctionExpr(tfel::utilities::SmartPtr<ExternalFunction> ff,
							     std::vector<tfel::utilities::SmartPtr<Expr> >& fargs,
							     const std::vector<std::vector<double>::size_type>& fpvar)
	: f(ff),
	  args(fargs),
	  pvar(fpvar)
      {
	using namespace std;
	if(f->getNumberOfVariables()!=args.size()){
	  ostringstream msg;
	  msg << "DifferentiatedFunctionExpr::DifferentiatedFunctionExpr : "
	      << "invalid number of arguments for function (" 
	      << this->args.size() << " given, "
	      << f->getNumberOfVariables() << " required)";
	  throw(runtime_error(msg.str()));
        }
      } // end of DifferentiatedFunctionExpr::DifferentiatedFunctionExpr

      tfel::utilities::SmartPtr<ExternalFunction>
      DifferentiatedFunctionExpr::getDerivative(void) const
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::math::parser;
	vector<vector<double>::size_type>::const_iterator p;
	SmartPtr<ExternalFunction> df = this->f;
	for(p=this->pvar.begin();p!=this->pvar.end();++p){
	  df = df->differentiate(*p);
	}
	return df;
      } // end of DifferentiatedFunctionExpr::getDerivative
      
      double
      DifferentiatedFunctionExpr::getValue(void) const
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::math::parser;
	vector<SmartPtr<Expr> >::const_iterator p;
	vector<SmartPtr<Expr> >::size_type i;
	SmartPtr<ExternalFunction> df = this->getDerivative();
	for(p=this->args.begin(),i=0u;p!=this->args.end();++p,++i){
	  const double val = (*p)->getValue();
	  df->setVariableValue(i,val);
	}
	return df->getValue();
      } // end of DifferentiatedFunctionExpr::getValue

      void
      DifferentiatedFunctionExpr::checkCyclicDependency(std::vector<std::string>& names) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> >::const_iterator p;
	this->f->checkCyclicDependency(names);
	for(p=this->args.begin();p!=this->args.end();++p){
	  vector<string> n;
	  (*p)->checkCyclicDependency(n);
	  mergeVariablesNames(names,n);
	}
      } // end of DifferentiatedFunctionExpr::checkCyclicDependency

      tfel::utilities::SmartPtr<Expr>
      DifferentiatedFunctionExpr::differentiate(const std::vector<double>::size_type pos,
						const std::vector<double>& v) const
      {
	using namespace std;
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> > nargs(this->args.size());
        vector<SmartPtr<Expr> >::const_iterator p = this->args.begin();
	vector<SmartPtr<Expr> >::const_iterator p3;
        vector<SmartPtr<Expr> >::iterator p4;
	unsigned short i = 0;
	if(args.size()==0){
	  return SmartPtr<Expr>(new Number(0.));
	}
        for(p3=this->args.begin(),p4=nargs.begin();
	    p3!=this->args.end();++p3,++p4){
	  *p4 = (*p3)->clone(v);
	}
	SmartPtr<ExternalFunction> ndf = this->getDerivative();
        SmartPtr<Expr> df_(new ExternalFunctionExpr2(ndf->differentiate(i),
						     nargs));
        SmartPtr<Expr> df = SmartPtr<Expr>(new BinaryOperation<OpMult>(df_,
								       (*p)->differentiate(pos,v)));
        ++p;
        ++i;
        while(p!=this->args.end()){
	  df_  = SmartPtr<Expr>(new ExternalFunctionExpr2(ndf->differentiate(i),
							  nargs));
	  SmartPtr<Expr> df2 = SmartPtr<Expr>(new BinaryOperation<OpMult>(df_,
									  (*p)->differentiate(pos,v)));
      
          df = SmartPtr<Expr>(new BinaryOperation<OpPlus>(df,df2));
	  ++p;
	  ++i;
        }
	return df;
      } // end of DifferentiatedFunctionExpr::differentiate

      tfel::utilities::SmartPtr<Expr>
      DifferentiatedFunctionExpr::clone(const std::vector<double>& v) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> > nargs(this->args.size());
        vector<SmartPtr<Expr> >::const_iterator p;
        vector<SmartPtr<Expr> >::iterator p2;
        for(p=this->args.begin(),p2=nargs.begin();p!=this->args.end();++p,++p2){
	  *p2 = (*p)->clone(v);
	}
        return SmartPtr<Expr>(new DifferentiatedFunctionExpr(this->f,nargs,this->pvar));	
      } // end of DifferentiatedFunctionExpr::clone

      tfel::utilities::SmartPtr<Expr>
      DifferentiatedFunctionExpr::resolveDependencies() const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> > nargs(this->args.size());
        vector<SmartPtr<Expr> >::const_iterator p;
        vector<SmartPtr<Expr> >::iterator p2;
        for(p=this->args.begin(),p2=nargs.begin();p!=this->args.end();++p,++p2){
	  *p2 = (*p)->resolveDependencies();
	}
        return SmartPtr<Expr>(new ExternalFunctionExpr2(this->getDerivative(),nargs));	
      } // end of DifferentiatedFunctionExpr::resolveDependencies

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
