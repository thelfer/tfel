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
#include"TFEL/Math/Parser/Variable.hxx"
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
      ExternalFunctionExpr::checkCyclicDependency(std::vector<std::string>& names) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<string> fnames;
	vector<SmartPtr<Expr> >::const_iterator p;
	ExternalFunctionManager::const_iterator p2;
	vector<string>::const_iterator p3;
	if(find(names.begin(),names.end(),this->name)!=names.end()){
	  string msg("ExternalFunctionExpr::checkCyclicDependency : ");
	  msg += "cyclic dependency found on function '"+this->name+"'";
	  throw(runtime_error(msg));
	}
	p2=this->manager->find(this->name);
	if(p2==this->manager->end()){
	  string msg("ExternalFunctionExpr::checkCyclicDependency : ");
	  msg += "unknown function '"+this->name+"'";
	  throw(runtime_error(msg));
	}
	for(p=this->args.begin();p!=this->args.end();++p){
	  vector<string> n;
	  (*p)->checkCyclicDependency(n);
	  for(p3=names.begin();p3!=names.end();++p3){
	    if(find(n.begin(),n.end(),*p3)!=n.end()){
	      string msg("ExternalFunctionExpr::checkCyclicDependency : ");
	      msg += "cyclic dependency found on function '"+*p3+"'";
	      throw(runtime_error(msg));
	    }
	  }
	  mergeVariablesNames(names,n);
	}
	fnames.push_back(this->name);
	p2->second->checkCyclicDependency(fnames);
	mergeVariablesNames(names,fnames);
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
	  string msg("ExternalFunctionExpr::differentiate : ");
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
      ExternalFunctionExpr::createFunctionByChangingParametersIntoVariables(const std::vector<double>& v,
									    const std::vector<std::string>& params,
									    const std::map<std::string,
									    std::vector<double>::size_type>& pos) const
      {
	using namespace std;
	using namespace tfel::utilities;
        vector<SmartPtr<Expr> >::const_iterator p;
        vector<SmartPtr<Expr> >::iterator p2;
	map<string,vector<double>::size_type>::const_iterator p3;
	vector<SmartPtr<Expr> > nargs;
	vector<string> vnames;
	vector<string>::size_type i;
	tfel::utilities::SmartPtr<ExternalFunction> nf;
	ExternalFunctionManager::iterator pf;
	if(this->args.size()==0){
	  if(find(params.begin(),params.end(),this->name)!=params.end()){
	    p3 = pos.find(this->name);
	    if(p3==pos.end()){
	      string msg("ExternalFunctionExpr::createFunctionByChangingParametersIntoVariables : ");
	      msg += "internal error (no position found for parameter '"+this->name+"')";
	      throw(runtime_error(msg));
	    }
	    SmartPtr<Expr> nv = SmartPtr<Expr>(new Variable(v,p3->second));
	    return nv;
	  }
	}
	pf = this->manager->find(this->name);
	if(pf==this->manager->end()){
	  string msg("ExternalFunctionExpr::createFunctionByChangingParametersIntoVariables : ");
	  msg += "no function '"+this->name+"' declared";
	  throw(runtime_error(msg));
	}
	if(pf->second->getNumberOfVariables()!=this->args.size()){
	  ostringstream msg;
	  msg << "ExternalFunctionExpr::getValue : "
	      << "invalid number of arguments for function '"
	      << this->name << "' (" 
	      << this->args.size() << " given, "
	      << pf->second->getNumberOfVariables() << " required)";
	  throw(runtime_error(msg.str()));
	}
	nf = pf->second->createFunctionByChangingParametersIntoVariables(vnames,v,params,pos);
	if(nf->getNumberOfVariables()<this->args.size()){
	  string msg;
	  msg += "ExternalFunctionExpr::getValue : "
	    "internal error (function as less variable after "
	    "'createFunctionByChangingParametersIntoVariables' than before";
	  throw(runtime_error(msg));
	}
	nargs.resize(nf->getNumberOfVariables());
	for(p=this->args.begin(),p2=nargs.begin();p!=this->args.end();++p,++p2){
	  *p2 = (*p)->createFunctionByChangingParametersIntoVariables(v,params,pos);
	}
	for(i=0;i!=vnames.size();++i){
	  p3 = pos.find(vnames[i]);
	  if(p3==pos.end()){
	    string msg("ExternalFunctionExpr::createFunctionByChangingParametersIntoVariables : ");
	    msg += "internal error (no position found for parameter '"+vnames[i]+"')";
	    throw(runtime_error(msg));
	  }
	  nargs[args.size()+i] = SmartPtr<Expr>(new Variable(v,p3->second));
	}
        return SmartPtr<Expr>(new ExternalFunctionExpr2(nf,nargs));
      } // end of ExternalFunctionExpr::createFunctionByChangingParametersIntoVariables

      void
      ExternalFunctionExpr::getParametersNames(std::set<std::string>& p) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> >::const_iterator pa;
	ExternalFunctionManager::iterator pf;
	if(this->args.size()==0){
	  p.insert(this->name);
	} else {
	  pf = this->manager->find(this->name);
	  if(pf==this->manager->end()){
	    string msg("ExternalFunctionExpr::getParametersNames : ");
	    msg += "no function '"+this->name+"' declared";
	    throw(runtime_error(msg));
	  }
	  pf->second->getParametersNames(p);
	  for(pa=this->args.begin();pa!=this->args.end();++pa){
	    (*pa)->getParametersNames(p);
	  }
	}
      } // end of ExternalFunctionExpr::getParametersNames(std::set<std::string>&) const;

      tfel::utilities::SmartPtr<Expr>
      ExternalFunctionExpr::resolveDependencies(const std::vector<double>& v) const
      {
	using namespace std;
	using namespace tfel::utilities;
	vector<SmartPtr<Expr> > nargs(this->args.size());
        vector<SmartPtr<Expr> >::const_iterator p;
        vector<SmartPtr<Expr> >::iterator p2;
	ExternalFunctionManager::iterator p3;
        for(p=this->args.begin(),p2=nargs.begin();p!=this->args.end();++p,++p2){
	  *p2 = (*p)->resolveDependencies(v);
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
