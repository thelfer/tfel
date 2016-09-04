/*!
 * \file   src/Math/DifferentiatedFunctionExpr.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>
#include<stdexcept>

#include"TFEL/Math/Parser/Number.hxx"
#include"TFEL/Math/Parser/Variable.hxx"
#include"TFEL/Math/Parser/BinaryOperator.hxx"
#include"TFEL/Math/Parser/ExternalFunctionExpr2.hxx"
#include"TFEL/Math/Parser/DifferentiatedFunctionExpr.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {
      
      DifferentiatedFunctionExpr::DifferentiatedFunctionExpr(std::shared_ptr<ExternalFunction> ff,
							     std::vector<std::shared_ptr<Expr> >& fargs,
							     const std::vector<std::vector<double>::size_type>& fpvar)
	: f(std::move(ff)),
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

      std::shared_ptr<ExternalFunction>
      DifferentiatedFunctionExpr::getDerivative() const
      {
	using namespace std;
	using namespace tfel::math::parser;
	vector<vector<double>::size_type>::const_iterator p;
	shared_ptr<ExternalFunction> df = this->f;
	for(p=this->pvar.begin();p!=this->pvar.end();++p){
	  df = df->differentiate(*p);
	}
	return df;
      } // end of DifferentiatedFunctionExpr::getDerivative
      
      double
      DifferentiatedFunctionExpr::getValue() const
      {
	using namespace std;
	using namespace tfel::math::parser;
	vector<shared_ptr<Expr> >::const_iterator p;
	vector<shared_ptr<Expr> >::size_type i;
	shared_ptr<ExternalFunction> df = this->getDerivative();
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
	vector<shared_ptr<Expr> >::const_iterator p;
	vector<string> v(names);
	this->f->checkCyclicDependency(names);
	for(p=this->args.begin();p!=this->args.end();++p){
	  vector<string> n(v);
	  (*p)->checkCyclicDependency(n);
	  mergeVariablesNames(names,n);
	}
      } // end of DifferentiatedFunctionExpr::checkCyclicDependency

      std::shared_ptr<Expr>
      DifferentiatedFunctionExpr::differentiate(const std::vector<double>::size_type pos,
						const std::vector<double>& v) const
      {
	using namespace std;
	using namespace std;
	vector<shared_ptr<Expr> > nargs(this->args.size());
        auto p = this->args.begin();
	vector<shared_ptr<Expr> >::const_iterator p3;
        vector<shared_ptr<Expr> >::iterator p4;
	unsigned short i = 0;
	if(args.size()==0){
	  return shared_ptr<Expr>(new Number(0.));
	}
        for(p3=this->args.begin(),p4=nargs.begin();
	    p3!=this->args.end();++p3,++p4){
	  *p4 = (*p3)->clone(v);
	}
	shared_ptr<ExternalFunction> ndf = this->getDerivative();
        shared_ptr<Expr> df_(new ExternalFunctionExpr2(ndf->differentiate(i),
						     nargs));
        shared_ptr<Expr> df = shared_ptr<Expr>(new BinaryOperation<OpMult>(df_,
								       (*p)->differentiate(pos,v)));
        ++p;
        ++i;
        while(p!=this->args.end()){
	  df_  = shared_ptr<Expr>(new ExternalFunctionExpr2(ndf->differentiate(i),
							  nargs));
	  shared_ptr<Expr> df2 = shared_ptr<Expr>(new BinaryOperation<OpMult>(df_,
									  (*p)->differentiate(pos,v)));
      
          df = shared_ptr<Expr>(new BinaryOperation<OpPlus>(df,df2));
	  ++p;
	  ++i;
        }
	return df;
      } // end of DifferentiatedFunctionExpr::differentiate

      std::shared_ptr<Expr>
      DifferentiatedFunctionExpr::clone(const std::vector<double>& v) const
      {
	using namespace std;
	vector<shared_ptr<Expr> > nargs(this->args.size());
        vector<shared_ptr<Expr> >::const_iterator p;
        vector<shared_ptr<Expr> >::iterator p2;
        for(p=this->args.begin(),p2=nargs.begin();p!=this->args.end();++p,++p2){
	  *p2 = (*p)->clone(v);
	}
        return shared_ptr<Expr>(new DifferentiatedFunctionExpr(this->f,nargs,this->pvar));	
      } // end of DifferentiatedFunctionExpr::clone

      std::shared_ptr<Expr>
      DifferentiatedFunctionExpr::createFunctionByChangingParametersIntoVariables(const std::vector<double>& v,
										  const std::vector<std::string>& params,
										  const std::map<std::string,
										  std::vector<double>::size_type>& pos) const
      {
	using namespace std;
	vector<shared_ptr<Expr> > nargs;
	vector<string> vnames;
	shared_ptr<ExternalFunction> nf;
        vector<shared_ptr<Expr> >::const_iterator p;
        vector<shared_ptr<Expr> >::iterator p2;
	map<string,vector<double>::size_type>::const_iterator p3;
	vector<string>::size_type i;
	nf = this->f->createFunctionByChangingParametersIntoVariables(vnames,v,params,pos);
	if(nf->getNumberOfVariables()<=this->args.size()){
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
	  nargs[args.size()+i] = shared_ptr<Expr>(new Variable(v,p3->second));
	}
        return shared_ptr<Expr>(new DifferentiatedFunctionExpr(nf,nargs,this->pvar));
      } // end of DifferentiatedFunctionExpr::createFunctionByChangingParametersIntoVariables

      void
      DifferentiatedFunctionExpr::getParametersNames(std::set<std::string>& p) const
      {
	using namespace std;
        vector<shared_ptr<Expr> >::const_iterator pt;
	this->f->getParametersNames(p);
        for(pt=this->args.begin();pt!=this->args.end();++pt){
	  (*pt)->getParametersNames(p);
	}
      } // end of DifferentiatedFunctionExpr::getParametersNames

      std::shared_ptr<Expr>
      DifferentiatedFunctionExpr::resolveDependencies(const std::vector<double>& v) const
      {
	using namespace std;
	vector<shared_ptr<Expr> > nargs(this->args.size());
        vector<shared_ptr<Expr> >::const_iterator p;
        vector<shared_ptr<Expr> >::iterator p2;
        for(p=this->args.begin(),p2=nargs.begin();p!=this->args.end();++p,++p2){
	  *p2 = (*p)->resolveDependencies(v);
	}
        return shared_ptr<Expr>(new ExternalFunctionExpr2(this->getDerivative(),nargs));	
      } // end of DifferentiatedFunctionExpr::resolveDependencies

      DifferentiatedFunctionExpr::~DifferentiatedFunctionExpr()
      {} // end of DifferentiatedFunctionExpr::~DifferentiatedFunctionExpr()

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
