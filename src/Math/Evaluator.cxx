/*!
 * \file   src/Math/Evaluator.cxx
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

#include<cmath>
#include<cassert>
#include<stdexcept>
#include<algorithm>
#include<iterator>
#include<sstream>
#include<cctype>

#include"TFEL/Math/power.hxx"
#include"TFEL/Math/Evaluator.hxx"

#ifdef __CYGWIN__
static bool isascii(const int c){
  return ((c>=0) && (c<128));
}
#endif

namespace tfel
{

  namespace math
  {

    void
    Evaluator::checkNotEndOfExpression(const std::string& method,
				       const std::string& error,
				       const std::vector<std::string>::const_iterator p,
				       const std::vector<std::string>::const_iterator pe)
    {
      if(p==pe){
	auto msg = method + " : unexpected end of line";
	if(!error.empty()){
	  msg += " ("+error+")";
	}
	throw(std::runtime_error(msg));
      }
    } // end of Evaluator::checkNotEndOfExpression

    void
    Evaluator::checkNotEndOfExpression(const std::string& method,
				       const std::vector<std::string>::const_iterator p,
				       const std::vector<std::string>::const_iterator pe)
    {
      Evaluator::checkNotEndOfExpression(method,"",p,pe);
    } // end of Evaluator::checkNotEndOfExpression

    void
    Evaluator::readSpecifiedToken(const std::string& method,
				  const std::string& value,
				  std::vector<std::string>::const_iterator& p,
				  const std::vector<std::string>::const_iterator pe)
    {
      Evaluator::checkNotEndOfExpression(method,"expected '"+value+"'",p,pe);
      if(*p!=value){
	auto msg = method;
	msg += " : unexpected token '"+*p+"'";
	msg += " (expected '"+value+"')";
	throw(std::runtime_error(msg));
      }
      ++p;
    } // end of Evaluator::readSpecifiedToken

    unsigned short
    Evaluator::convertToUnsignedShort(const std::string& method,
				      const std::string& value)
    {
      using namespace std;
      string::const_iterator ps;
      istringstream converter(value);
      for(ps=value.begin();ps!=value.end();++ps){
	if(!isdigit(*ps)){
	  throw(runtime_error(method+" : invalid entry"));
	}
      }
      unsigned short u;
      converter >> u;
      if(!converter||(!converter.eof())){
	string msg(method+" : ");
	msg += "not read value from token '"+value+"'";
	throw(runtime_error(msg));
      }
      return u;
    } // end of Evaluator::convertToUnsignedShort

    unsigned int
    Evaluator::convertToUnsignedInt(const std::string& method,
				    const std::string& value)
    {
      using namespace std;
      string::const_iterator ps;
      istringstream converter(value);
      for(ps=value.begin();ps!=value.end();++ps){
	if(!isdigit(*ps)){
	  throw(runtime_error(method+" : invalid entry"));
	}
      }
      unsigned int u;
      converter >> u;
      if(!converter||(!converter.eof())){
	string msg(method+" : ");
	msg += "not read value from token '"+value+"'";
	throw(runtime_error(msg));
      }
      return u;
    } // end of Evaluator::convertToUnsignedInt

    int
    Evaluator::convertToInt(const std::string& method,
			    const std::string& value)
    {
      using namespace std;
      string::const_iterator ps;
      istringstream converter(value);
      for(ps=value.begin();ps!=value.end();++ps){
	if(!isdigit(*ps)){
	  throw(runtime_error(method+" : invalid entry"));
	}
      }
      int u;
      converter >> u;
      if(!converter||(!converter.eof())){
	string msg(method+" : ");
	msg += "not read value from token '"+value+"'";
	throw(runtime_error(msg));
      }
      return u;
    } // end of Evaluator::convertToInt

    unsigned short
    Evaluator::readUnsignedShortValue(const std::string& method,
				      std::vector<std::string>::const_iterator& p,
				      const std::vector<std::string>::const_iterator pe)
    {
      Evaluator::checkNotEndOfExpression(method,"expected unsigned short value",p,pe);
      const unsigned short nbr = convertToUnsignedShort(method,*p);
      ++p;
      return nbr;
    } // end of Evaluator::readUnsignedShortValue

    void
    Evaluator::checkParameterNumber(const std::vector<double>::size_type n,
				    const std::vector<double>::size_type p)
    {
      using namespace std;
      if(p!=n){
	string msg("Evaluator::checkParameterNumber : ");
	if(p==0){
	  msg += "no parameter given";
	} else {	
	  msg += "too many parameters given";
	}
	throw(runtime_error(msg));
      }
    } // end of Evaluator::checkParameterNubmer

    void
    Evaluator::checkVariableNumber(const std::vector<double>::size_type n,
				   const std::vector<double>::size_type p)
    {
      using namespace std;
      if(p!=n){
	string msg("Evaluator::checkVariableNumber : ");
	if(p==0){
	  msg += "no parameter given";
	} else {	
	  msg += "too many parameters given";
	}
	throw(runtime_error(msg));
      }
    } // end of Evaluator::checkVariableNubmer

    struct TFEL_VISIBILITY_LOCAL Evaluator::ExternalFunctionRegister
    {
      ExternalFunctionRegister()
      {} // end of Evaluator::ExternalFunctionRegister()
    }; // end of struct Evaluator::ExternalFunctionRegister

    Evaluator::ExternalFunctionRegister Evaluator::externalFunctionRegister;

#if !(defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
    template<unsigned short N>
    static std::shared_ptr<tfel::math::parser::Expr>
    EvaluatorPowerFunctionGenerator(const std::shared_ptr<tfel::math::parser::Expr> e)
    {
      using namespace tfel::math::parser;
      using namespace tfel::math::stdfunctions;
      return std::shared_ptr<Expr>(new StandardFunction<power<N> >(e));
    } // end of EvaluatorPowerFunctionGenerator
#endif

    static std::shared_ptr<tfel::math::parser::Expr>
    EvaluatorTreatPower(const std::vector<std::string>& params,
			std::vector<std::shared_ptr<tfel::math::parser::Expr> >& args)
    {
      using namespace std;
      using namespace tfel::math;
      using namespace tfel::math::parser;
      unsigned short nbr;
      Evaluator::checkParameterNumber(1,params.size());
      Evaluator::checkVariableNumber(1,args.size());
      nbr = Evaluator::convertToUnsignedShort("EvaluatorTreatPower",params[0]);
      switch (nbr){
      case 0:
	return shared_ptr<Expr>(new Number(1.));
#if !(defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
	  case 1:
	return EvaluatorPowerFunctionGenerator<1>(args[0]);
      case 2:
	return EvaluatorPowerFunctionGenerator<2>(args[0]);
      case 3:
	return EvaluatorPowerFunctionGenerator<3>(args[0]);
      case 4:
	return EvaluatorPowerFunctionGenerator<4>(args[0]);
      case 5:
	return EvaluatorPowerFunctionGenerator<5>(args[0]);
      case 6:
	return EvaluatorPowerFunctionGenerator<6>(args[0]);
      case 7:
	return EvaluatorPowerFunctionGenerator<7>(args[0]);
      case 8:
	return EvaluatorPowerFunctionGenerator<8>(args[0]);
      case 9:
	return EvaluatorPowerFunctionGenerator<9>(args[0]);
      case 10:
	return EvaluatorPowerFunctionGenerator<10>(args[0]);
      case 11:
	return EvaluatorPowerFunctionGenerator<11>(args[0]);
      case 12:
	return EvaluatorPowerFunctionGenerator<12>(args[0]);
      case 13:
	return EvaluatorPowerFunctionGenerator<13>(args[0]);
      case 14:
	return EvaluatorPowerFunctionGenerator<14>(args[0]);
      case 15:
	return EvaluatorPowerFunctionGenerator<15>(args[0]);
      case 16:
	return EvaluatorPowerFunctionGenerator<16>(args[0]);
#endif
      default:
	string msg("EvaluatorTreatPower : only exponent below 16 are supported");
	throw(runtime_error(msg));
      }
    } // end of EvaluatorTreatPower

    void
    Evaluator::treatDiff(std::vector<std::string>::const_iterator& p,
			 const std::vector<std::string>::const_iterator pe,
			 Evaluator::TGroup * const g,
			 const bool b)
    {
      using namespace std;
      using namespace tfel::math::parser;
      using std::vector;
      vector<string>::const_iterator pv;
      vector<shared_ptr<Evaluator::TExpr> > args;
      string f;
      vector<string> var;
      vector<vector<double>::size_type> pvar;
      unsigned short openedParenthesis=0u;
      int diffNumber = -1;
      Evaluator::checkNotEndOfExpression("Evaluator::treatDiff","(",p,pe);
      if(*p=="<"){
	++p;
	Evaluator::checkNotEndOfExpression("Evaluator::treatDiff","expected number",p,pe);
	string::const_iterator ps  = p->begin();
	string::const_iterator pse = p->end();
	while(ps!=pse){
	  if(!isdigit(*ps)){
	    string msg("Evaluator::treatDiff : ");
	    msg += "expected a number";
	    throw(runtime_error(msg));
	  }
	  ++ps;
	}
	istringstream converter(*p);
	converter >> diffNumber;
	if(diffNumber<=1){
	  string msg("Evaluator::treatDiff : ");
	  msg += "order of differentiation must be greater or egal than 1";
	  throw(runtime_error(msg));
	}
	++p;
	Evaluator::readSpecifiedToken("Evaluator::treatDiff",">",p,pe);
	Evaluator::checkNotEndOfExpression("Evaluator::treatDiff","expected ')'",p,pe);
      }
      Evaluator::readSpecifiedToken("Evaluator::treatDiff","(",p,pe);
      Evaluator::checkNotEndOfExpression("Evaluator::treatDiff",p,pe);
      while(!((*p==",")&&(openedParenthesis==0u))){
	if(*p=="("){
	  ++openedParenthesis;
	}
	if(*p==")"){
	  if(openedParenthesis==0){
	    string msg("Evaluator::treatDiff : ");
	    msg += "unbalanced parenthesis";
	    throw(runtime_error(msg));
	  }
	  --openedParenthesis;
	}
	f+=*p;
	++p;
	Evaluator::checkNotEndOfExpression("Evaluator::treatDiff",p,pe);
      }
      if(f.empty()){
	string msg("Evaluator::treatDiff : ");
	msg += "function definition is empty.";
	throw(runtime_error(msg));
      }
      ++p;
      Evaluator::checkNotEndOfExpression("Evaluator::treatDiff",
					 "expected variable name",p,pe);
      // now reading var list
      while(*p!=")"){
	var.push_back(*p);
	++p;
	Evaluator::checkNotEndOfExpression("Evaluator::treatDiff",
					   "expected  ',' or ')'",p,pe);
	if(*p!=")"){
	  if(*p!=","){
	    string msg("Evaluator::treatDiff : ");
	    msg += "unexpected token '"+*p+"' (expected ',' or ')')";
	    throw(runtime_error(msg));
	  }
	  ++p;
	  Evaluator::readSpecifiedToken("Evaluator::treatDiff",")",p,pe);
	}
      }
      if(var.empty()){
	string msg("Evaluator::treatDiff : ");
	msg += "empty var list";
	throw(runtime_error(msg));
      }
      if((var.size()>1)&&(diffNumber!=-1)){
	string msg("Evaluator::treatDiff : ");
	msg += "only one variable name allowed when degree of differentiation specified";
	throw(runtime_error(msg));
      }
      shared_ptr<Evaluator::ExternalFunction> pev;
      if(this->manager.get()==nullptr){
	if(b){
	  // variable names are fixed
	  pev= shared_ptr<Evaluator::ExternalFunction>(new Evaluator(this->getVariablesNames(),f));
	} else {
	  pev= shared_ptr<Evaluator::ExternalFunction>(new Evaluator(f));
	}
      } else {
	if(b){
	  // variable names are fixed
	  pev= shared_ptr<Evaluator::ExternalFunction>(new Evaluator(this->getVariablesNames(),f,this->manager));
	} else {
	  pev= shared_ptr<Evaluator::ExternalFunction>(new Evaluator(f,this->manager));
	}
      }
      const auto& fvars = static_cast<Evaluator *>(pev.get())->getVariablesNames();
      for(pv=var.begin();pv!=var.end();++pv){
	if(find(fvars.begin(),fvars.end(),*pv)==fvars.end()){
	  string msg("Evaluator::treatDiff : ");
	  msg += "expression '"+f+"'can't be differentiated by variable '"+*pv+"'";
	  throw(runtime_error(msg));
	}
      }
      if(diffNumber!=-1){
	var.resize(static_cast<vector<string>::size_type >(diffNumber),var[0]);
      }
      for(pv=fvars.begin();pv!=fvars.end();++pv){
	if(!b){
	  this->registerVariable(*pv);
	}
	const std::vector<double>::size_type pos = this->getVariablePosition(*pv);
	args.push_back(shared_ptr<Evaluator::TExpr>(new Evaluator::TVariable(pos,this->variables)));
      }
      for(pv=var.begin();pv!=var.end();++pv){
	pvar.push_back(static_cast<vector<double>::size_type>(find(fvars.begin(),fvars.end(),*pv)-fvars.begin()));
      }
      g->add(shared_ptr<Evaluator::TExpr>(new Evaluator::TDifferentiatedFunctionExpr(pev,args,pvar)));
    } // end of Evaluator::treatDiff

    void
    Evaluator::FunctionGeneratorManager::checkName(const std::string& name) const
    {
      using namespace std;
      if(this->fctGenerators.find(name)!=this->fctGenerators.end()){
	string msg("Evaluator::checkName : ");
	msg += "a function named '"+name+"' has already been declared.";
	throw(runtime_error(msg));
      }
      if(this->bFctGenerators.find(name)!=this->bFctGenerators.end()){
	string msg("Evaluator::checkName : ");
	msg += "a binary function named '"+name+"' has already been declared.";
	throw(runtime_error(msg));
      }
      if(this->extOpGenerators.find(name)!=this->extOpGenerators.end()){
	string msg("Evaluator::checkName : ");
	msg += "an external operator named '"+name+"' has already been declared.";
	throw(runtime_error(msg));
      }
    } // end of FunctionGeneratorManager::checkName

    void
    Evaluator::FunctionGeneratorManager::insert(const std::string& name,
						const Evaluator::FunctionGenerator f)
    {
      this->checkName(name);
      this->fctGenerators.insert(make_pair(name,f));
    } // end of FunctionGeneratorManager::insert
    
    void
    Evaluator::FunctionGeneratorManager::insert(const std::string& name,
						const Evaluator::BinaryFunctionGenerator f)
    {
      this->checkName(name);
      this->bFctGenerators.insert(make_pair(name,f));
    } // end of FunctionGeneratorManager::insert

    void
    Evaluator::FunctionGeneratorManager::insert(const std::string& name,
						const Evaluator::ExternalFunctionGenerator f)
    {
      this->checkName(name);
      this->extOpGenerators.insert(make_pair(name,f));
    } // end of FunctionGeneratorManager::insert
    
    Evaluator::FunctionGeneratorManager::FunctionGeneratorManager(void)
    {
      using namespace std;
      typedef map<string,ExternalFunctionGenerator>::value_type MVType;
      this->fctGenerators.insert(make_pair(string("exp"),
					   &StandardFctGenerator<exp>));
      this->fctGenerators.insert(make_pair(string("abs"),
					   &StandardFctGenerator<fabs>));
      this->fctGenerators.insert(make_pair(string("sin"),
					   &StandardFctGenerator<sin>));
      this->fctGenerators.insert(make_pair(string("cos"),
					   &StandardFctGenerator<cos>));
      this->fctGenerators.insert(make_pair(string("tan"),
					   &StandardFctGenerator<tan>));
      this->fctGenerators.insert(make_pair(string("sqrt"),
					   &StandardFctGenerator<sqrt>));
      this->fctGenerators.insert(make_pair(string("log"),
					   &StandardFctGenerator<log>));
      this->fctGenerators.insert(make_pair(string("ln"),
					   &StandardFctGenerator<log>));
      this->fctGenerators.insert(make_pair(string("log10"),
					   &StandardFctGenerator<log10>));
      this->fctGenerators.insert(make_pair(string("asin"),
					   &StandardFctGenerator<asin>));
      this->fctGenerators.insert(make_pair(string("acos"),
					   &StandardFctGenerator<acos>));
      this->fctGenerators.insert(make_pair(string("atan"),
					   &StandardFctGenerator<atan>));
      this->fctGenerators.insert(make_pair(string("sinh"),
					   &StandardFctGenerator<sinh>));
      this->fctGenerators.insert(make_pair(string("cosh"),
					   &StandardFctGenerator<cosh>));
      this->fctGenerators.insert(make_pair(string("tanh"),
					   &StandardFctGenerator<tanh>));
      this->fctGenerators.insert(make_pair(string("H"),
					   &StandardFctGenerator<Evaluator::Heavyside>));
      this->bFctGenerators.insert(make_pair(string("max"),
					    &StandardBinaryFctGenerator<Evaluator::max>));
      this->bFctGenerators.insert(make_pair(string("min"),
					    &StandardBinaryFctGenerator<Evaluator::min>));
      this->extOpGenerators.insert(MVType("power",EvaluatorTreatPower));
    } // end of Evaluator::FunctionGeneratorManager::FunctionGeneratorManager

    Evaluator::FunctionGeneratorManager&
    Evaluator::getFunctionGeneratorManager(void)
    {
      static FunctionGeneratorManager functionGeneratorManager;
      return functionGeneratorManager;
    } // end of Evaluator::getFunctionGeneratorManager(void)

    bool
    Evaluator::isValidIdentifier(const std::string& s)
    {
      auto& f = Evaluator::getFunctionGeneratorManager();
      if(s=="diff"){
	return false;
      }
      if(f.extOpGenerators.find(s)!=f.extOpGenerators.end()){
	return false;
      }
      if(f.fctGenerators.find(s)!=f.fctGenerators.end()){
	return false;
      }
      if(f.bFctGenerators.find(s)!=f.bFctGenerators.end()){
	return false;
      }
      return true;
    } // end of Evaluator::isValidIdentifier

    double
    Evaluator::Heavyside(const double x)
    {
      return x<0 ? 0 : 1;
    } // end of Evaluator::Heavyside

    double
    Evaluator::max(const double a,const double b)
    {
      return std::max(a,b);
    } // end of Evaluator::max

    double
    Evaluator::min(const double a,const double b)
    {
      return std::min(a,b);
    } // end of Evaluator::min

    void
    Evaluator::checkCyclicDependency(std::vector<std::string>& names) const
    {
      using namespace std;
      this->expr->checkCyclicDependency(names);
    } // end of Evaluator::checkCyclicDependency

    void
    Evaluator::checkCyclicDependency(const std::string& name) const
    {
      using namespace std;
      vector<string> names(1,name);
      this->expr->checkCyclicDependency(names);
    } // end of Evaluator::checkCyclicDependency

    void
    Evaluator::checkCyclicDependency(void) const
    {
      using namespace std;
      vector<string> names;
      this->expr->checkCyclicDependency(names);
    } // end of Evaluator::checkCyclicDependency

    template<typename T>
    bool
    Evaluator::convert(const std::string& value)
    {
      std::istringstream is(value);
      T res;
      is >> res;
      if(!is||(!is.eof())){
	return false;
      }
      return true;
    } // end of convert
    
    bool
    Evaluator::isNumber(const std::string& value)
    {
      return convert<double>(value);
    } // end of Evaluator::isNumber

    std::vector<std::string>
    Evaluator::analyseParameters(std::vector<std::string>::const_iterator& p,
				 const std::vector<std::string>::const_iterator  pe)
    {
      using namespace std;
      vector<string> params;
      string::const_iterator ps;
      checkNotEndOfExpression("Evaluator::analyseParameters",p,pe);
      while(*p!=">"){
	// check parameter validity
	for(ps=p->begin();ps!=p->end();++ps){
	  if(!(isascii(*ps)||isdigit(*ps))||(*ps=='_')||(*ps=='-')){
	    string msg("Evaluator::analyseParameters : ");
	    msg += "invalid parameter '"+*p+"'";
	    throw(runtime_error(msg));
	  }
	}
	params.push_back(*p);
	++p;
	Evaluator::checkNotEndOfExpression("Evaluator::analyseParameters",p,pe);
	if(*p!=">"){
	  if(*p==","){
	    ++p;
	    Evaluator::checkNotEndOfExpression("Evaluator::analyseParameters",p,pe);
	  } else {
	    string msg("Evaluator::analyseParameters");
	    msg += "unexpected token '"+*p+"' ";
	    msg += "(expected ',' or '>')";
	    throw(runtime_error(msg));
	  }
	}
      }
      ++p;
      return params;
    } // end of Evaluator::analyseParameters

    std::vector<std::shared_ptr<Evaluator::TExpr> >
    Evaluator::analyseArguments(const unsigned short nbr,
				std::vector<std::string>::const_iterator& p,
				const std::vector<std::string>::const_iterator  pe,
				const bool b)
    {
      using namespace std;
      vector<shared_ptr<Evaluator::TExpr> > res;
      if(nbr>0){
	unsigned short i;
	for(i=0;i!=nbr-1u;++i){
	  res.push_back(this->treatGroup(p,pe,b,","));
	  ++p;
	}
	res.push_back(this->treatGroup(p,pe,b,")"));
      }
      return res;
    } // end of Evaluator::analyseArguments

    std::vector<std::shared_ptr<Evaluator::TExpr> >
    Evaluator::analyseArguments(std::vector<std::string>::const_iterator& p,
				const std::vector<std::string>::const_iterator  pe,
				const bool b)
    {
      using namespace std;
      vector<shared_ptr<Evaluator::TExpr> > res;
      unsigned short nbr = this->countNumberOfArguments(p,pe);
      if(nbr>0){
	for(unsigned short i=0;i!=nbr-1u;++i){
	  res.push_back(this->treatGroup(p,pe,b,","));
	  ++p;
	}
	res.push_back(this->treatGroup(p,pe,b,")"));
      }
      return res;
    } // end of Evaluator::analyseArguments

    unsigned short
    Evaluator::countNumberOfArguments(std::vector<std::string>::const_iterator p,
				      const std::vector<std::string>::const_iterator pe)
    {
      using namespace std;
      unsigned short opened = 1;
      unsigned short nbr = 1;
      Evaluator::checkNotEndOfExpression("Evaluator::countNumberOfArguments",p,pe);
      if(*p==")"){
	return 0;
      }
      while(p!=pe){
	if(*p=="("){
	  ++opened;
	} else if(*p==")"){
	  --opened;
	  if(opened==0){
	    return nbr;
	  }
	} else if(*p==","){
	  if(opened==1){
	    ++nbr;
	  }
	}
	++p;
      }
      ostringstream msg;
      if(opened==1){
	msg << "Evaluator::countNumberOfArguments : group ended while " 
	    << "a parenthesis was still opened";
      } else {
	msg << "Evaluator::countNumberOfArguments : group ended while " 
	    << opened << " parenthesis were still opened";
      }
      throw(runtime_error(msg.str()));
      return 0;
    } // end of Evaluator::analyseArguments

    std::pair<bool,std::vector<std::string>::const_iterator>
    Evaluator::search(std::vector<std::string>::const_iterator p,
		      std::vector<std::string>::const_iterator pe,
		      const std::string& m,
		      const std::string& s)
    {
      using namespace std;
      unsigned short openedParenthesis = 0;
      bool test;
      if(s.empty()){
	test = (p!=pe);
      } else {
	test = (p!=pe)&&(*p!=s);
      }
      while(test){
	if(*p=="("){
	  ++openedParenthesis;
	}
	if(*p==")"){
	  if(openedParenthesis==0){
	    string msg("Analyser::readNextGroup : ");
	    msg += "unbalanced parenthesis";
	    throw(runtime_error(msg));
	  }
	  --openedParenthesis;
	}
	if((*p==m)&&(openedParenthesis==0)){
	  return make_pair(true,p);
	}
	++p;
	if(s.empty()){
	  test = (p!=pe);
	} else {
	  test = (p!=pe)&&(*p!=s);
	}
      }
      return make_pair(false,p);
    } // end of Evaluator::search

    std::shared_ptr<Evaluator::TLogicalExpr>
    Evaluator::treatLogicalExpression(const std::vector<std::string>::const_iterator p,
				      const std::vector<std::string>::const_iterator pe,
				      const bool b)
    {
      using namespace std;
      using namespace tfel::math::parser;
      vector<string>::const_iterator pb(p);           
      vector<string>::const_iterator pbe(pe);
      checkNotEndOfExpression("Evaluator::treatLogicalExpression",pb,pbe);
      pair<bool,vector<string>::const_iterator> pa = this->search(pb,pbe,"&&","");
      pair<bool,vector<string>::const_iterator> po = this->search(pb,pbe,"||","");
      if((pa.second!=pe)||(po.second!=pe)){
	auto pt = pa.first ? pa.second : po.second;
	if(pt==pb){
	  string msg("Evaluator::treatLogicalExpression : ");
	  msg += "no left logical expression";
	  throw(runtime_error(msg));
	}
	if(pt+1==pbe){
	  string msg("Evaluator::treatLogicalExpression : ");
	  msg += "no right logical expression";
	  throw(runtime_error(msg));
	}
	shared_ptr<Evaluator::TLogicalExpr> lo = this->treatLogicalExpression(pb,pt,b);
	shared_ptr<Evaluator::TLogicalExpr> ro = this->treatLogicalExpression(pt+1,pbe,b);
	if(*pt=="&&"){
	  return shared_ptr<Evaluator::TLogicalExpr>(new TLogicalBinaryOperation<OpAnd>(lo,ro));
	} else if (*pt=="||"){
	  return shared_ptr<Evaluator::TLogicalExpr>(new TLogicalBinaryOperation<OpOr>(lo,ro));
	} else {
	  string msg("Evaluator::treatLogicalExpression : ");
	  msg += "unkwown operator '"+*pt+"'";
	  throw(runtime_error(msg));
	}
      }
      if(*pb=="("){
	--pbe;
	Evaluator::checkNotEndOfExpression("Evaluator::treatLogicalExpression",pb,pbe);
	if(*pbe!=")"){
	  string msg("Evaluator::treatLogicalExpression : ");
	  msg += "unmatched parenthesis";
	  throw(runtime_error(msg));
	}
	++pb;
	return this->treatLogicalExpression(pb,pbe,b);
      }
      if(*pb=="!"){
	++pb;
	return shared_ptr<Evaluator::TLogicalExpr>(new TNegLogicalExpr(this->treatLogicalExpression(pb,pbe,b)));
      }
      return this->treatLogicalExpression2(pb,pbe,b);
    } // end of Evaluator::treatLogicalExpression
    
    std::vector<std::string>::const_iterator
    Evaluator::searchComparisonOperator(const std::vector<std::string>::const_iterator pb,
					const std::vector<std::string>::const_iterator pe)
    {
      using namespace std;
      unsigned short openedParenthesis = 0;
      bool found = false;
      bool test;
      map<string,Evaluator::ExternalFunctionGenerator>::const_iterator peo;
      vector<string>::const_iterator prev;
      vector<string>::const_iterator p;
      vector<string>::const_iterator po;
      p = pb;
      while(p!=pe){
	if(*p=="("){
	  ++openedParenthesis;
	}
	if(*p==")"){
	  if(openedParenthesis==0){
	    string msg("Analyser::readNextGroup : ");
	    msg += "unbalanced parenthesis";
	    throw(runtime_error(msg));
	  }
	  --openedParenthesis;
	}
	if(openedParenthesis==0){
	  if((*p=="==")||(*p=="<=")||
	     (*p==">")||(*p==">=")){
	    if(found){
	      ostringstream msg;
	      msg << "Evaluator::treatLogicalExpression : "
		  << "more than one logical operator found (error while parsing '";
	      copy(pb,pe,ostream_iterator<string>(msg," "));
	      msg << "')";
	      throw(runtime_error(msg.str()));
	    }
	    po = p;
	    found = true;
	  }
	  if(*p=="<"){
	    if(p==pb){
	      ostringstream msg;
	      msg << "Evaluator::treatLogicalExpression : "
		  << "no left part to logical operator (error while parsing '";
	      copy(p,pe,ostream_iterator<string>(msg," "));
	      msg << "')";
	      throw(runtime_error(msg.str()));
	    }
	    prev = p-1;
	    peo = Evaluator::getFunctionGeneratorManager().extOpGenerators.find(*prev);
	    if(peo!=Evaluator::getFunctionGeneratorManager().extOpGenerators.end()){
	      ++p;
	      Evaluator::checkNotEndOfExpression("Evaluator::searchComparisonOperator",p,pe);
	      test=true;
	      while(test){
		if(*p==">"){
		  test=false;
		} 
		++p;
		Evaluator::checkNotEndOfExpression("Evaluator::searchComparisonOperator",p,pe);
	      }
	      if(*p!="("){
		string msg("Evaluator::searchComparisonOperator : ");
		msg += "expected '(', read '"+*p+"'";
		throw(runtime_error(msg));
	      }
	      ++openedParenthesis;
	    } else {
	      if(found){
		ostringstream msg;
		msg << "Evaluator::treatLogicalExpression : "
		    << "more than one logical operator found (error while parsing '";
		copy(pb,pe,ostream_iterator<string>(msg," "));
		msg << "')";
		throw(runtime_error(msg.str()));
	      }
	      po = p;
	      found = true;
	    }
	  }
	}
	++p;
      }
      if(!found){
	ostringstream msg;
	msg << "Evaluator::treatLogicalExpression : "
	    << "no logical operator found (error while parsing '";
	copy(pb,pe,ostream_iterator<string>(msg," "));
	msg << "')";
	throw(runtime_error(msg.str()));
      }
      if(po==pb){
	ostringstream msg;
	msg << "Evaluator::treatLogicalExpression : "
	    << "no left part to logical operator (error while parsing '";
	copy(p,pe,ostream_iterator<string>(msg," "));
	msg << "')";
	throw(runtime_error(msg.str()));
      }
      if(po+1==pe){
	ostringstream msg;
	msg << "Evaluator::treatLogicalExpression : "
	    << "no right part to logical operator (error while parsing '";
	copy(pb,pe,ostream_iterator<string>(msg," "));
	msg << "')";
	throw(runtime_error(msg.str()));
      }
      return po;
    } // end of Evaluator::searchComparisonOperator

    std::shared_ptr<Evaluator::TLogicalExpr>
    Evaluator::treatLogicalExpression2(const std::vector<std::string>::const_iterator p,
				       const std::vector<std::string>::const_iterator pe,
				       const bool b)
    {
      using namespace std;
      using namespace tfel::math::parser;
      auto plo = this->searchComparisonOperator(p,pe);
      vector<string>::const_iterator tmp2 = p;
      shared_ptr<Evaluator::TExpr> loexpr = this->treatGroup(tmp2,plo,b,"");
      auto tmp3 = plo+1;
      shared_ptr<Evaluator::TExpr> roexpr = this->treatGroup(tmp3,pe,b,"");
      if(*plo=="=="){
	return shared_ptr<Evaluator::TLogicalExpr>(new TLogicalOperation<OpEqual>(loexpr,roexpr));
      } else if(*plo==">"){
	return shared_ptr<Evaluator::TLogicalExpr>(new TLogicalOperation<OpGreater>(loexpr,roexpr));
      } else if(*plo==">="){
	return shared_ptr<Evaluator::TLogicalExpr>(new TLogicalOperation<OpGreaterOrEqual>(loexpr,roexpr));
      } else if(*plo=="<"){
	return shared_ptr<Evaluator::TLogicalExpr>(new TLogicalOperation<OpLesser>(loexpr,roexpr));
      } else if(*plo=="<="){
	return shared_ptr<Evaluator::TLogicalExpr>(new TLogicalOperation<OpLesserOrEqual>(loexpr,roexpr));
      } else {
	string msg("Evaluator::treatGroup : ");
	msg += "unsupported logical operator '"+*plo+"'";
	throw(runtime_error(msg));
      }
      return shared_ptr<Evaluator::TLogicalExpr>();
    } // end of Evaluator::treatLogicalExpression2

    std::shared_ptr<Evaluator::TExpr>
    Evaluator::treatGroup(std::vector<std::string>::const_iterator& p,
			  const std::vector<std::string>::const_iterator  pe,
			  const bool b,
			  const std::string& s)
    {
      using namespace std;
      using namespace tfel::math::parser;
      Evaluator::checkNotEndOfExpression("Evaluator::treatGroup",p,pe);
      if(*p==s){
	string msg("Evaluator::treatGroup : ");
	msg += "empty group";
	throw(runtime_error(msg));
      }
      pair<bool,vector<string>::const_iterator> res = this->search(p,pe,"?",s);
      if(res.first){
	if(p==res.second){
	  string msg("Evaluator::treatGroup : ");
	  msg += "no conditional expression preceeding '?' character";
	  throw(runtime_error(msg));
	}
	if(res.second+1==pe){
	 string msg("Evaluator::treatGroup : ");
	 msg += "nothing expression following '?' character";
	 throw(runtime_error(msg));
	}
	if(this->search(res.second+1,pe,"?",s).first){
	  string msg("Evaluator::treatGroup : ");
	  msg += "imbricated conditional expression are not supported ";
	  msg += "(use parenthesis as as workaround)";
	  throw(runtime_error(msg));
	}
	pair<bool,vector<string>::const_iterator> res2 = this->search(res.second+1,pe,":",s);
	if(!res2.first){
	  string msg("Evaluator::treatGroup : ");
	  msg += "no matching ':' character to '?' character";
	  throw(runtime_error(msg));
	}
	if(res.second+1==res2.second){
	  string msg("Evaluator::treatGroup : ");
	  msg += "empty left conditional expression";
	  throw(runtime_error(msg));
	}
	if(res2.second+1==pe){
	  string msg("Evaluator::treatGroup : ");
	  msg += "empty right conditional expression";
	  throw(runtime_error(msg));
	}
	shared_ptr<Evaluator::TLogicalExpr> l = this->treatLogicalExpression(p,res.second,b);
	auto tmp2 = res.second+1;
	shared_ptr<Evaluator::TExpr> lexpr = this->treatGroup(tmp2,res2.second,b,"");
	p = res2.second+1;
	shared_ptr<Evaluator::TExpr> rexpr = this->treatGroup(p,pe,b,s);
	return shared_ptr<TExpr>(new TConditionalExpr(l,lexpr,rexpr));
      }
      return this->treatGroup2(p,pe,b,s);
    } // end of Evaluator::treatGroup

    std::shared_ptr<Evaluator::TExpr>
    Evaluator::treatGroup2(std::vector<std::string>::const_iterator& p,
			   std::vector<std::string>::const_iterator  pe,
			   const bool b,
			   const std::string& s)
    {
      using namespace std;
      map<string,Evaluator::FunctionGenerator>::const_iterator p2;
      map<string,Evaluator::BinaryFunctionGenerator>::const_iterator p3;
      map<string,Evaluator::ExternalFunctionGenerator>::const_iterator p4;
      assert(p!=pe);
      shared_ptr<Evaluator::TExpr> pg = shared_ptr<Evaluator::TExpr>(new TGroup());
      TGroup * g = static_cast<TGroup *>(pg.get());
      bool test;
      if(s.empty()){
	test = (p!=pe);
      } else {
	test = (p!=pe)&&(*p!=s);
      }
      while(test){
	if(*p=="diff"){
	  ++p;
	  this->treatDiff(p,pe,g,b);
	} else if((p4=Evaluator::getFunctionGeneratorManager().extOpGenerators.find(*p))!=
		  Evaluator::getFunctionGeneratorManager().extOpGenerators.end()){
	  ++p;
	  Evaluator::checkNotEndOfExpression("Evaluator::treatGroup2",p,pe);
	  if(*p=="<"){
	    ++p;
	    const auto& params = this->analyseParameters(p,pe);
	    Evaluator::readSpecifiedToken("Evaluator::treatGroup2","(",p,pe);
	    auto args   = this->analyseArguments(p,pe,b);
	    g->add(shared_ptr<Evaluator::TExpr>(new TExternalOperator(p4->second,
								      params,args)));
	  } else {
	    Evaluator::readSpecifiedToken("Evaluator::treatGroup2","(",p,pe);
	    vector<string> params;
	    auto args = this->analyseArguments(p,pe,b);
	    g->add(shared_ptr<Evaluator::TExpr>(new TExternalOperator(p4->second,
								      params,args)));
	  }
	} else if((p3=Evaluator::getFunctionGeneratorManager().bFctGenerators.find(*p))!=
		  Evaluator::getFunctionGeneratorManager().bFctGenerators.end()){
	  // call to binary function
	  ++p;
	  Evaluator::readSpecifiedToken("Evaluator::treatGroup2","(",p,pe);
	  const auto args = this->analyseArguments(2,p,pe,b);
	  g->add(shared_ptr<Evaluator::TExpr>(new TBinaryFunction(p3->second,args[0],args[1])));
	} else if((p2=Evaluator::getFunctionGeneratorManager().fctGenerators.find(*p))!=
		  Evaluator::getFunctionGeneratorManager().fctGenerators.end()){
	  // call to function
	  ++p;
	  Evaluator::readSpecifiedToken("Evaluator::treatGroup2","(",p,pe);
	  g->add(shared_ptr<TExpr>(new TFunction(p2->second,this->treatGroup(p,pe,b))));
	} else if(isNumber(*p)){
	  // number
	  istringstream converter(*p);
	  double value;
	  converter >> value;
	  g->add(shared_ptr<TExpr>(new TNumber(value)));
	} else if(*p=="("){
	  ++p;
	  // begin group
	  g->add(shared_ptr<TExpr>(this->treatGroup(p,pe,b)));
	} else if(*p=="+"){
	  g->add(shared_ptr<TExpr>(new TOperator("+")));
	} else if(*p=="-"){
	  g->add(shared_ptr<TExpr>(new TOperator("-")));
	} else if(*p=="*"){
	  g->add(shared_ptr<TExpr>(new TOperator("*")));
	} else if(*p=="/"){
	  g->add(shared_ptr<TExpr>(new TOperator("/")));
	} else if(*p=="**"){
	  g->add(shared_ptr<TExpr>(new TOperator("**")));
	} else {
	  auto pt = p;
	  ++pt;
	  if((pt!=pe)&&(*pt=="(")){
	    if(this->manager.get()!=nullptr){
	      this->addExternalFunctionToGroup(g,p,pe,b);
	    } else {
	      string msg("Evaluator::treatGroup2 : unknown function '"+*p+"'");
	      throw(runtime_error(msg));
	    }
	  } else {
	    if(b){
	      // variable name is fixed
	      if(this->positions.find(*p)==this->positions.end()){
		vector<shared_ptr<Evaluator::TExpr> > args;
		if(this->manager.get()==nullptr){
		  string msg("Evaluator::treatGroup2 : ");
		  msg += "unknown variable '"+*p+"'";
		  throw(runtime_error(msg));
		}
		g->add(shared_ptr<Evaluator::TExpr>(new TExternalFunctionExpr(*p,
									    args,
									    this->manager)));

	      } else {
		g->add(shared_ptr<TExpr>(new TVariable(*p,*this)));
	      }
	    } else {
	      g->add(shared_ptr<TExpr>(new TVariable(*p,*this)));
	    }
	  }
	}
	++p;
	if(s.empty()){
	  test = (p!=pe);
	} else {
	  test = (p!=pe)&&(*p!=s);
	}
      }
      if((!s.empty())&&(p==pe)){
	string msg("Evaluator::treatGroup2 : unterminated group (group began with '"+s+"').");
	throw(runtime_error(msg));
      }
      return pg;
    } // end of Evaluator::treatGroup2

    std::vector<double>::size_type
    Evaluator::registerVariable(const std::string& vname)
    {
      const auto p  = this->positions.find(vname); 
      const auto pe = this->positions.end();
      if(p!=pe){
	return p->second;
      }
      const auto pos = this->variables.size();
      this->positions.insert({vname,pos});
      this->variables.resize(pos+1u);
      return pos;
    } // end of Evaluator::registerVariable

    Evaluator::~Evaluator()
    {}

    void
    Evaluator::setVariableValue(const std::string& vname,
				const double value)
    {
      using namespace std;
      auto p = this->positions.find(vname);
      if(p==this->positions.end()){
	string msg("Evaluator::setVariableValue : variable '");
	msg += vname + "' does not exist";
	throw(runtime_error(msg));
      }
      this->variables[p->second] = value;
    }//end of Evaluator::setVariableValue

    void
    Evaluator::setVariableValue(const std::vector<double>::size_type pos,
				const double value)
    {
      using namespace std;
      if(pos>=this->variables.size()){
	ostringstream msg;
	msg << "Evaluator::setVariableValue : position '" << pos << "' is invalid ";
	if(this->variables.size()==0){
	  msg << "(function has no variable).";
	} else if(this->variables.size()==1){
	  msg << "(function has one variable).";
	} else {
	  msg << "(function has only '" <<  this->variables.size() << "' variable(s)).";
	}
	throw(runtime_error(msg.str()));
      }
      this->variables[pos] = value;
    }//end of Evaluator::setVariableValue

    double
    Evaluator::getValue(void) const
    {
      using namespace std;
      if(this->expr.get()==nullptr){
	string msg("Evaluator::getValue : ");
	msg += "uninitialized expression.";
	throw(runtime_error(msg));
      }
      double v = this->expr->getValue();
      return v;
    }//end of Evaluator::getValue

    std::vector<std::string>
    Evaluator::getVariablesNames(void) const
    {
      using namespace std;
      typedef map<vector<double>::size_type,string>::value_type MVType;
      vector<string> res;
      map<string,vector<double>::size_type>::const_iterator p;
      map<vector<double>::size_type,string>::const_iterator p2;
      map<vector<double>::size_type,string> vnames;
      for(p=this->positions.begin();p!=this->positions.end();++p){
	vnames.insert(MVType(p->second,p->first));
      }
      for(p2=vnames.begin();p2!=vnames.end();++p2){
	res.push_back(p2->second);
      }
      return res;
    } // end of Evaluator::getVariablesNames

    std::vector<double>::size_type
    Evaluator::getNumberOfVariables(void) const
    {
      return this->variables.size();
    } // end of Evaluator::getNumberOfVariables(void) const;

    void
    Evaluator::analyse(const std::string& f,
		       const bool b)
    {
      using namespace std;
      using namespace tfel::math::parser;
      vector<string>::const_iterator p;
      vector<string>::const_iterator pe;
      EvaluatorBase::analyse(f);
      p  = this->tokens.begin(); 
      pe = this->tokens.end();
      shared_ptr<TExpr> g = this->treatGroup(p,pe,b,"");
      g->reduce();
      this->expr = shared_ptr<Expr>(g->analyse());
    }

    void
    Evaluator::addExternalFunctionToGroup(TGroup * const g,
					  std::vector<std::string>::const_iterator& p,
					  const std::vector<std::string>::const_iterator pe,
					  const bool b)
    {
      using namespace std;
      string fname = *p;
      ++p;
      Evaluator::readSpecifiedToken("Evaluator::addExternalFunctionToGroup","(",p,pe);
      vector<shared_ptr<Evaluator::TExpr> > args = this->analyseArguments(p,pe,b);
      g->add(shared_ptr<Evaluator::TExpr>(new TExternalFunctionExpr(fname,
								  args,
								  this->manager)));
    } // end of Evaluator::addExternalFunctionToGroup

    Evaluator::Evaluator()
    {} // end of Evaluator::Evaluator

    Evaluator::Evaluator(const Evaluator& src)
      : EvaluatorBase(src),
	variables(src.variables),
	positions(src.positions)
    {
      this->manager = src.manager;
      if(src.expr.get()!=nullptr){
	this->expr = src.expr->clone(this->variables);
      }
    } // end of Evaluator::Evaluator

    Evaluator&
    Evaluator::operator = (const Evaluator& src)
    {
      if(this!=&src){
	this->variables = src.variables;
	this->positions = src.positions;
	this->manager   = src.manager;
	if(src.expr.get()!=nullptr){
	  this->expr = src.expr->clone(this->variables);
	} else {
	  this->expr.reset();
	}
      }
      return *this;
    } // end of Evaluator::Evaluator

    Evaluator::Evaluator(const std::string& f)
    {
      this->setFunction(f);
    } // end of Evaluator::Evaluator

    Evaluator::Evaluator(const std::vector<std::string>& vars,
			 const std::string& f)
    {
      this->setFunction(vars,f);
    } // end of Evaluator::Evaluator

    Evaluator::Evaluator(const std::string& f,
			 std::shared_ptr<tfel::math::parser::ExternalFunctionManager>& m)
    {
      this->setFunction(f,m);
    } // end of Evaluator::Evaluator

    Evaluator::Evaluator(const std::vector<std::string>& vars,
			 const std::string& f,
			 std::shared_ptr<tfel::math::parser::ExternalFunctionManager>& m)
    {
      this->setFunction(vars,f,m);
    } // end of Evaluator::Evaluator

    Evaluator::Evaluator(const double v)
    {
      using namespace tfel::math::parser;
      this->expr = std::shared_ptr<Expr>(new Number(v));
    } // end of Evaluator::Evaluator

    void
    Evaluator::setFunction(const std::string& f)
    {
      using namespace std;
      using namespace tfel::math::parser;
      this->variables.clear();
      this->positions.clear();
      this->expr.reset();
      this->manager.reset();
      this->analyse(f,false);
    } // end of Evaluator::setFunction

    void
    Evaluator::setFunction(const std::vector<std::string>& vars,
			   const std::string& f)
    {
      using namespace std;
      using namespace tfel::math::parser;
      vector<string>::const_iterator p;
      vector<double>::size_type pos;
      this->variables.clear();
      this->positions.clear();
      this->expr.reset();
      this->manager.reset();
      this->variables.resize(vars.size());
      for(p=vars.begin(),pos=0u;p!=vars.end();++p,++pos){
	if(this->positions.find(*p)!=this->positions.end()){
	  string msg("Evaluator::setFunction : ");
	  msg += "variable '"+*p+"' multiply defined.";
	  throw(runtime_error(msg));
	}
	if(!Evaluator::isValidIdentifier(*p)){
	  string msg("Evaluator::setFunction : ");
	  msg += "variable '"+*p+"' is not valid.";
	  throw(runtime_error(msg));
	}
	this->positions[*p]=pos;
      }
      this->analyse(f,true);
    } // end of Evaluator::setFunction

    void
    Evaluator::setFunction(const std::string& f,
			   std::shared_ptr<tfel::math::parser::ExternalFunctionManager>& m)
    {
      using namespace std;
      using namespace tfel::math::parser;
      this->variables.clear();
      this->positions.clear();
      this->expr.reset();
      this->manager = m;
      this->analyse(f,false);
    } // end of Evaluator::setFunction

    void
    Evaluator::setFunction(const std::vector<std::string>& vars,
			   const std::string& f,
			   std::shared_ptr<tfel::math::parser::ExternalFunctionManager>& m)
    {
      using namespace std;
      using namespace tfel::math::parser;
      vector<string>::const_iterator p;
      vector<double>::size_type pos;
      this->variables.clear();
      this->positions.clear();
      this->expr.reset();
      this->manager = m;
      this->variables.resize(vars.size());
      for(p=vars.begin(),pos=0u;p!=vars.end();++p,++pos){
	if(this->positions.find(*p)!=this->positions.end()){
	  string msg("Evaluator::setFunction : ");
	  msg += "variable '"+*p+"' multiply defined.";
	  throw(runtime_error(msg));
	}
	if(!Evaluator::isValidIdentifier(*p)){
	  string msg("Evaluator::setFunction : ");
	  msg += "variable '"+*p+"' is not valid.";
	  throw(runtime_error(msg));
	}
	this->positions[*p]=pos;
      }
      this->analyse(f,true);
    } // end of Evaluator::setFunction
    
    std::shared_ptr<tfel::math::parser::ExternalFunction>
    Evaluator::differentiate(const std::vector<double>::size_type pos) const
    {
      using namespace std;
      using namespace tfel::math::parser;
      shared_ptr<ExternalFunction> pev(new Evaluator());
      auto& ev = static_cast<Evaluator&>(*(pev.get()));
      if(this->expr.get()==nullptr){
	string msg("Evaluator::differentiate : ");
	msg += "uninitialized expression.";
	throw(runtime_error(msg));
      }
      ev.variables.resize(this->variables.size());
      ev.positions = this->positions;
      if(this->variables.size()==0){
	// no variable
	ev.expr = shared_ptr<Expr>(new Number(0.));
      } else {
	if(pos>=this->variables.size()){
	  ostringstream msg;
	  msg << "Evaluator::differentiate : position '" << pos << "' is invalid ";
	  if(this->variables.size()==0){
	    msg << "(function has no variable).";
	  } else if(this->variables.size()==1){
	    msg << "(function has one variable).";
	  } else {
	    msg << "(function has only '" <<  this->variables.size() << "' variable(s)).";
	  }
	  throw(runtime_error(msg.str()));
	}
	ev.expr = this->expr->differentiate(pos,ev.variables);
      }
      return pev;
    } // end of Evaluator::differentiate

    std::shared_ptr<tfel::math::parser::ExternalFunction>
    Evaluator::differentiate(const std::string&) const
    {
      using namespace std;
      using namespace tfel::math::parser;
      string msg("Evaluator::differentiate : unimplemented feature");
      throw(runtime_error(msg));
      return shared_ptr<ExternalFunction>();
    } // end of std::shared_ptr<ExternalFunction>

    std::shared_ptr<tfel::math::parser::ExternalFunction>
    Evaluator::resolveDependencies() const
    {
      this->checkCyclicDependency();
      std::shared_ptr<ExternalFunction> f(new Evaluator(*this));
      Evaluator *pf = static_cast<Evaluator *>(f.get());
      pf->expr = pf->expr->resolveDependencies(pf->variables);
      return f;
    } // end of Evaluator::resolveDependencies() const

    void
    Evaluator::removeDependencies(void)
    {
      this->checkCyclicDependency();
      this->expr = this->expr->resolveDependencies(this->variables);
    } // end of Evaluator::removeDependencies() const

    std::shared_ptr<tfel::math::parser::ExternalFunctionManager>
    Evaluator::getExternalFunctionManager(void)
    {
      return this->manager;
    } // end of Evaluator::getExternalFunctionManager
    
    std::vector<double>::size_type
    Evaluator::getVariablePosition(const std::string& name) const
    {
      using namespace std;
      map<string,vector<double>::size_type>::const_iterator p;
      p = this->positions.find(name);
      if(p==this->positions.end()){
	string msg("Evaluator::getVariablePosition : ");
	msg += "unknown variable '"+name+"'";
	throw(runtime_error(msg));
      }
      return p->second;
    } // end of Evaluator::getVariablePosition(const std::string&)

    std::shared_ptr<tfel::math::parser::ExternalFunction>
    Evaluator::createFunctionByChangingParametersIntoVariables(std::vector<std::string>& nparams,
							       const std::vector<double>&,
							       const std::vector<std::string>& params,
							       const std::map<std::string,
							       std::vector<double>::size_type>&) const
    {
      using namespace std;
      set<string> ev_params;
      vector<string>::const_iterator p;
      nparams.clear();
      this->getParametersNames(ev_params);
      for(p=params.begin();p!=params.end();++p){
	if(ev_params.find(*p)!=ev_params.end()){
	  if(find(nparams.begin(),nparams.end(),*p)==nparams.end()){
	    nparams.push_back(*p);
	  }
	}
      }
      return this->createFunctionByChangingParametersIntoVariables(nparams);
    } // end of Evaluator::createFunctionByChangingParametersIntoVariables

    std::shared_ptr<tfel::math::parser::ExternalFunction>
    Evaluator::createFunctionByChangingParametersIntoVariables(const std::vector<std::string>& params) const
    {
      using namespace std;
      using namespace tfel::math::parser;
      typedef map<string,vector<double>::size_type>::value_type MVType;
      set<string> ev_params;
      vector<string>::const_iterator pp;
      vector<double>::size_type i;
      assert(this->variables.size()==this->positions.size());
      this->getParametersNames(ev_params);
      for(pp=params.begin();pp!=params.end();++pp){
	if(ev_params.find(*pp)==ev_params.end()){
	  string msg("Evaluator::createFunctionByChangingParametersIntoVariables : ");
	  msg += "no parameter '"+*pp+"'";
	  throw(runtime_error(msg));
	}
      }
      for(pp=params.begin();pp!=params.end();++pp){
	if(this->positions.find(*pp)!=this->positions.end()){
	  string msg("Evaluator::createFunctionByChangingParametersIntoVariables : ");
	  msg += "'"+*pp+"' is alredy a variable of this function";
	  throw(runtime_error(msg));
	}
      }
      auto  ev = new Evaluator();
      shared_ptr<ExternalFunction> pev = shared_ptr<ExternalFunction>(ev);
      ev->variables.resize(this->variables.size()+params.size());
      ev->positions = this->positions;
      for(pp=params.begin(),i=this->variables.size();pp!=params.end();++pp,++i){
	if(!(ev->positions.insert(MVType(*pp,i)).second)){
	  string msg("Evaluator::createFunctionByChangingParametersIntoVariables : ");
	  msg += "internal error (variable '"+*pp+"' alredy declared)";
	  throw(runtime_error(msg));
	}
      }
      ev->manager = this->manager;
      ev->expr = this->expr->createFunctionByChangingParametersIntoVariables(ev->variables,
									     params,ev->positions);      
      return pev;
    } // end of Evaluator::createFunctionByChangingParametersIntoVariables

    void
    Evaluator::getParametersNames(std::set<std::string>& n) const
    {
      return this->expr->getParametersNames(n);
    } // end of Evaluator::getParametersNames

  } // end of namespace math
  
} // end of namespace tfel
