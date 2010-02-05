/*!
 * \file   ExternalFunctionExpr2.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#include<sstream>
#include<stdexcept>

#include"TFEL/Math/Parser/Number.hxx"
#include"TFEL/Math/Parser/BinaryOperator.hxx"
#include"TFEL/Math/Parser/ExternalFunctionExpr2.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {
      
      ExternalFunctionExpr2::ExternalFunctionExpr2(tfel::utilities::SmartPtr<ExternalFunction> ff,
						   std::vector<tfel::utilities::SmartPtr<Expr> >& fargs)
	: f(ff),
	  args(fargs)	
      {
	using namespace std;
	if(f->getNumberOfVariables()!=args.size()){
	  ostringstream msg;
	  msg << "ExternalFunctionExpr2::ExternalFunctionExpr2 : "
	      << "invalid number of arguments for function (" 
	      << this->args.size() << " given, "
	      << f->getNumberOfVariables() << " required)";
	  throw(runtime_error(msg.str()));
        }
      } // end of ExternalFunctionExpr2::ExternalFunctionExpr2

      double
      ExternalFunctionExpr2::getValue(void) const
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::math::parser;
	vector<SmartPtr<Expr> >::const_iterator p;
	vector<SmartPtr<Expr> >::size_type i;
	for(p=this->args.begin(),i=0u;p!=this->args.end();++p,++i){
	  const double val = (*p)->getValue();
	  this->f->setVariableValue(i,val);
	}
	return this->f->getValue();
      } // end of ExternalFunctionExpr2::getValue

      void
      ExternalFunctionExpr2::checkCyclicDependency(std::vector<std::string>& names) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> >::const_iterator p;
	for(p=this->args.begin();p!=this->args.end();++p){
	  vector<string> n;
	  (*p)->checkCyclicDependency(n);
	  mergeVariablesNames(names,n);
	}
      } // end of ExternalFunctionExpr2::checkCyclicDependency

      void
      ExternalFunctionExpr2::getParametersNames(std::set<std::string>& p) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> >::const_iterator pa;
	for(pa=this->args.begin();pa!=this->args.end();++pa){
	  (*pa)->getParametersNames(p);
	}
      } // end of ExternalFunctionExpr2::getParametersNames(std::set<std::string>&) const;

      tfel::utilities::SmartPtr<Expr>
      ExternalFunctionExpr2::differentiate(const std::vector<double>::size_type pos,
					   const std::vector<double>& v) const
      {
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
        SmartPtr<Expr> df_(new ExternalFunctionExpr2(this->f->differentiate(i),
						     nargs));
        SmartPtr<Expr> df = SmartPtr<Expr>(new BinaryOperation<OpMult>(df_,
								       (*p)->differentiate(pos,v)));
        ++p;
        ++i;
        while(p!=this->args.end()){
	  df_  = SmartPtr<Expr>(new ExternalFunctionExpr2(this->f->differentiate(i),
							  nargs));
	  SmartPtr<Expr> df2 = SmartPtr<Expr>(new BinaryOperation<OpMult>(df_,
									  (*p)->differentiate(pos,v)));
      
          df = SmartPtr<Expr>(new BinaryOperation<OpPlus>(df,df2));
	  ++p;
	  ++i;
        }
	return df;
      } // end of ExternalFunctionExpr2::differentiate

      tfel::utilities::SmartPtr<Expr>
      ExternalFunctionExpr2::createFunctionByChangingParametersIntoVariables(const std::vector<double>& v,
									     const std::vector<std::string>& params,
									     const std::map<std::string,
									     std::vector<double>::size_type>& pos) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> > nargs(this->args.size());
        vector<SmartPtr<Expr> >::const_iterator p;
        vector<SmartPtr<Expr> >::iterator p2;
        for(p=this->args.begin(),p2=nargs.begin();p!=this->args.end();++p,++p2){
#warning "stupid"
	  //	  *p2 = (*p)->createFunctionByChangingParametersIntoVariables(v,params,pos);
	}
        return SmartPtr<Expr>(new ExternalFunctionExpr2(this->f,nargs));
      } // end of ExternalFunctionExpr2::createFunctionByChangingParametersIntoVariables

      tfel::utilities::SmartPtr<Expr>
      ExternalFunctionExpr2::clone(const std::vector<double>& v) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> > nargs(this->args.size());
        vector<SmartPtr<Expr> >::const_iterator p;
        vector<SmartPtr<Expr> >::iterator p2;
        for(p=this->args.begin(),p2=nargs.begin();p!=this->args.end();++p,++p2){
	  *p2 = (*p)->clone(v);
	}
        return SmartPtr<Expr>(new ExternalFunctionExpr2(this->f,nargs));	
      } // end of ExternalFunctionExpr2::clone

      tfel::utilities::SmartPtr<Expr>
      ExternalFunctionExpr2::resolveDependencies() const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> > nargs(this->args.size());
        vector<SmartPtr<Expr> >::const_iterator p;
        vector<SmartPtr<Expr> >::iterator p2;
        for(p=this->args.begin(),p2=nargs.begin();p!=this->args.end();++p,++p2){
	  *p2 = (*p)->resolveDependencies();
	}
        return SmartPtr<Expr>(new ExternalFunctionExpr2(this->f,nargs));	
      } // end of ExternalFunctionExpr2::resolveDependencies

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
