/*!
 * \file   ExternalFunctionExpr.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#include<sstream>
#include<algorithm>

#include"TFEL/Math/Parser/Number.hxx"
#include"TFEL/Math/Parser/BinaryOperator.hxx"
#include"TFEL/Math/Parser/ExternalFunctionExpr.hxx"
#include"TFEL/Math/Parser/ExternalFunctionExpr2.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {
      
      ExternalFunctionExpr::ExternalFunctionExpr(const std::string& fname,
						 std::vector<tfel::utilities::SmartPtr<Expr> >& fargs,
						 tfel::utilities::SmartPtr<tfel::math::parser::ExternalFunctionManager>& m)
	: name(fname),
	  args(fargs),
	  manager(m)
      {} // end of ExternalFunctionExpr::ExternalFunctionExpr
      
      double
      ExternalFunctionExpr::getValue(void) const
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::math::parser;
	vector<SmartPtr<Expr> >::const_iterator p;
	vector<SmartPtr<Expr> >::size_type i;
	ExternalFunctionManager::iterator p2;
	p2=this->manager->find(this->name);
	if(p2==this->manager->end()){
	  string msg("ExternalFunctionExpr::getValue : ");
	  msg += "unknown function '"+this->name+"'";
	  throw(runtime_error(msg));
	}
	if(p2->second->getNumberOfVariables()!=this->args.size()){
	  ostringstream msg;
	  msg << "ExternalFunctionExpr::getValue : "
	      << "invalid number of arguments for function '"
	      << this->name << "' (" 
	      << this->args.size() << " given, "
	      << p2->second->getNumberOfVariables() << " required)";
	  throw(runtime_error(msg.str()));
	}
	for(p=this->args.begin(),i=0u;p!=this->args.end();++p,++i){
	  const double val = (*p)->getValue();
	  p2->second->setVariableValue(i,val);
	}
	return p2->second->getValue();
      } // end of ExternalFunctionExpr::getValue

      void
      ExternalFunctionExpr::checkCyclicDependency(const std::vector<std::string>& names) const
	throw(std::runtime_error)
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> >::const_iterator p;
	ExternalFunctionManager::const_iterator p2;
	if(find(names.begin(),names.end(),this->name)!=names.end()){
	  string msg("ExternalFunctionExpr::checkCyclicDependency : ");
	  msg += "cyclic dependency found on function '"+this->name+"'";
	  throw(runtime_error(msg));
	}
	vector<string> fnames(names);
	fnames.push_back(this->name);
	p2=this->manager->find(this->name);
	if(p2==this->manager->end()){
	  string msg("ExternalFunctionExpr::checkDependency : ");
	  msg += "unknown function '"+this->name+"'";
	  throw(runtime_error(msg));
	}
	p2->second->checkCyclicDependency(fnames);
	for(p=this->args.begin();p!=this->args.end();++p){
	  (*p)->checkCyclicDependency(fnames);
	}
      } // end of ExternalFunctionExpr::checkCyclicDependency

      tfel::utilities::SmartPtr<Expr>
      ExternalFunctionExpr::differentiate(const std::vector<double>::size_type pos,
					  const std::vector<double>& v) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> > nargs(this->args.size());
	vector<SmartPtr<Expr> >::const_iterator p = this->args.begin();
	ExternalFunctionManager::const_iterator p2;
        vector<SmartPtr<Expr> >::const_iterator p3;
        vector<SmartPtr<Expr> >::iterator p4;
	unsigned short i = 0;
	if(args.size()==0){
	  return SmartPtr<Expr>(new Number(0.));
	}
	p2=this->manager->find(this->name);
	if(p2==this->manager->end()){
	  string msg("ExternalFunctionExpr::checkDependency : ");
	  msg += "unknown function '"+this->name+"'";
	  throw(runtime_error(msg));
	}
	if(p2->second->getNumberOfVariables()!=this->args.size()){
	  ostringstream msg;
	  msg << "ExternalFunctionExpr::getValue : "
	      << "invalid number of arguments for function '"
	      << this->name << "' (" 
	      << this->args.size() << " given, "
	      << p2->second->getNumberOfVariables() << " required)";
	  throw(runtime_error(msg.str()));
	}
        for(p3=this->args.begin(),p4=nargs.begin();
	    p3!=this->args.end();++p3,++p4){
	  *p4 = (*p3)->clone(v);
	}
        SmartPtr<Expr> df_(new ExternalFunctionExpr2(p2->second->differentiate(i),
						     nargs));
        SmartPtr<Expr> df = SmartPtr<Expr>(new BinaryOperation<OpMult>(df_,
								       (*p)->differentiate(pos,v)));
        ++p;
        ++i;
        while(p!=this->args.end()){
	  df_  = SmartPtr<Expr>(new ExternalFunctionExpr2(p2->second->differentiate(i),
							  nargs));
	  SmartPtr<Expr> df2 = SmartPtr<Expr>(new BinaryOperation<OpMult>(df_,
									  (*p)->differentiate(pos,v)));
      
          df = SmartPtr<Expr>(new BinaryOperation<OpPlus>(df,df2));
	  ++p;
	  ++i;
        }
	return df;
      } // end of ExternalFunctionExpr::differentiate

      tfel::utilities::SmartPtr<Expr>
      ExternalFunctionExpr::clone(const std::vector<double>& v) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> > nargs(this->args.size());
        vector<SmartPtr<Expr> >::const_iterator p;
        vector<SmartPtr<Expr> >::iterator p2;
        for(p=this->args.begin(),p2=nargs.begin();p!=this->args.end();++p,++p2){
	  *p2 = (*p)->clone(v);
	}
        return SmartPtr<Expr>(new ExternalFunctionExpr(this->name,nargs,this->manager));	
      } // end of ExternalFunctionExpr::clone

      tfel::utilities::SmartPtr<Expr>
      ExternalFunctionExpr::resolveDependencies() const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> > nargs(this->args.size());
        vector<SmartPtr<Expr> >::const_iterator p;
        vector<SmartPtr<Expr> >::iterator p2;
	ExternalFunctionManager::iterator p3;
        for(p=this->args.begin(),p2=nargs.begin();p!=this->args.end();++p,++p2){
	  *p2 = (*p)->resolveDependencies();
	}
	p3=this->manager->find(this->name);
	if(p3==this->manager->end()){
	  string msg("ExternalFunctionExpr::getValue : ");
	  msg += "unknown function '"+this->name+"'";
	  throw(runtime_error(msg));
	}
	if(p3->second->getNumberOfVariables()!=this->args.size()){
	  ostringstream msg;
	  msg << "ExternalFunctionExpr::getValue : "
	      << "invalid number of arguments for function '"
	      << this->name << "' (" 
	      << this->args.size() << " given, "
	      << p3->second->getNumberOfVariables() << " required)";
	  throw(runtime_error(msg.str()));
	}
        return SmartPtr<Expr>(new ExternalFunctionExpr2(p3->second->resolveDependencies(),nargs));	
      } // end of ExternalFunctionExpr::resolveDependencies
      
    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
