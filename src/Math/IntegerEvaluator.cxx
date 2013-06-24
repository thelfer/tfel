/*!
 * \file   IntegerEvaluator.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#include<cassert>
#include<cmath>
#include<algorithm>
#include<iterator>
#include<sstream>
#include<cctype>

#include"TFEL/Math/IntegerEvaluator.hxx"
#include"TFEL/Math/Parser/IntegerEvaluatorTExpr.hxx"

namespace tfel
{

  namespace math
  {

    void
    IntegerEvaluator::checkNotEndOfExpression(const std::string& method,
				       const std::string& error,
				       const std::vector<std::string>::const_iterator p,
				       const std::vector<std::string>::const_iterator pe)
    {
      using namespace std;
      if(p==pe){
	string msg(method);
	msg += " : unexpected end of line";
	if(!error.empty()){
	  msg += " ("+error+")";
	}
	throw(runtime_error(msg));
      }
    } // end of IntegerEvaluator::checkNotEndOfExpression

    void
    IntegerEvaluator::checkNotEndOfExpression(const std::string& method,
				       const std::vector<std::string>::const_iterator p,
				       const std::vector<std::string>::const_iterator pe)
    {
      IntegerEvaluator::checkNotEndOfExpression(method,"",p,pe);
    } // end of IntegerEvaluator::checkNotEndOfExpression

    template<typename T>
    bool
    IntegerEvaluator::convert(const std::string& value)
    {
      using namespace std;
      T res;
      std::istringstream is(value);
      if((value=="+")||(value=="-")||(value=="*")||(value=="/")||(value==".")){
	return false;
      }
      is >> res;
      if(!is&&(!is.eof())){
	return false;
      }
      return true;
    } // end of convert
    
    bool
    IntegerEvaluator::isNumber(const std::string& value)
    {
      return convert<int>(value);
    } // end of IntegerEvaluator::isNumber

    std::vector<std::string>
    IntegerEvaluator::analyseParameters(std::vector<std::string>::const_iterator& p,
				 const std::vector<std::string>::const_iterator  pe)
    {
      using namespace std;
      vector<string> params;
      string::const_iterator ps;
      checkNotEndOfExpression("IntegerEvaluator::analyseParameters",p,pe);
      while(*p!=">"){
	// check parameter validity
	for(ps=p->begin();ps!=p->end();++ps){
	  if(!(isascii(*ps)||isdigit(*ps))||(*ps=='_')||(*ps=='-')){
	    string msg("IntegerEvaluator::analyseParameters : ");
	    msg += "invalid parameter '"+*p+"'";
	    throw(runtime_error(msg));
	  }
	}
	params.push_back(*p);
	++p;
	IntegerEvaluator::checkNotEndOfExpression("IntegerEvaluator::analyseParameters",p,pe);
	if(*p!=">"){
	  if(*p==","){
	    ++p;
	    IntegerEvaluator::checkNotEndOfExpression("IntegerEvaluator::analyseParameters",p,pe);
	  } else {
	    string msg("IntegerEvaluator::analyseParameters");
	    msg += "unexpected token '"+*p+"' ";
	    msg += "(expected ',' or '>')";
	    throw(runtime_error(msg));
	  }
	}
      }
      ++p;
      return params;
    } // end of IntegerEvaluator::analyseParameters

    std::vector<tfel::utilities::shared_ptr<IntegerEvaluator::TExpr> >
    IntegerEvaluator::analyseArguments(const unsigned short nbr,
				std::vector<std::string>::const_iterator& p,
				const std::vector<std::string>::const_iterator  pe,
				const bool b)
    {
      using namespace std;
      using namespace tfel::utilities;
      vector<shared_ptr<IntegerEvaluator::TExpr> > res;
      if(nbr>0){
	unsigned short i;
	for(i=0;i!=nbr-1u;++i){
	  res.push_back(this->treatGroup(p,pe,b,","));
	  ++p;
	}
	res.push_back(this->treatGroup(p,pe,b,")"));
      }
      return res;
    } // end of IntegerEvaluator::analyseArguments

    std::vector<tfel::utilities::shared_ptr<IntegerEvaluator::TExpr> >
    IntegerEvaluator::analyseArguments(std::vector<std::string>::const_iterator& p,
				const std::vector<std::string>::const_iterator  pe,
				const bool b)
    {
      using namespace std;
      using namespace tfel::utilities;
      vector<shared_ptr<IntegerEvaluator::TExpr> > res;
      unsigned short nbr = this->countNumberOfArguments(p,pe);
      unsigned short i;
      if(nbr>0){
	for(i=0;i!=nbr-1u;++i){
	  res.push_back(this->treatGroup(p,pe,b,","));
	  ++p;
	}
	res.push_back(this->treatGroup(p,pe,b,")"));
      }
      return res;
    } // end of IntegerEvaluator::analyseArguments

    unsigned short
    IntegerEvaluator::countNumberOfArguments(std::vector<std::string>::const_iterator p,
				      const std::vector<std::string>::const_iterator pe)
    {
      using namespace std;
      unsigned short opened = 1;
      unsigned short nbr = 1;
      IntegerEvaluator::checkNotEndOfExpression("IntegerEvaluator::countNumberOfArguments",p,pe);
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
	msg << "IntegerEvaluator::countNumberOfArguments : group ended while " 
	    << "a parenthesis was still opened";
      } else {
	msg << "IntegerEvaluator::countNumberOfArguments : group ended while " 
	    << opened << " parenthesis were still opened";
      }
      throw(runtime_error(msg.str()));
      return 0;
    } // end of IntegerEvaluator::analyseArguments

    std::pair<bool,std::vector<std::string>::const_iterator>
    IntegerEvaluator::search(std::vector<std::string>::const_iterator p,
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
    } // end of IntegerEvaluator::search

    tfel::utilities::shared_ptr<IntegerEvaluator::TExpr>
    IntegerEvaluator::treatGroup(std::vector<std::string>::const_iterator& p,
				 const std::vector<std::string>::const_iterator  pe,
				 const bool b,
				 const std::string& s)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      IntegerEvaluator::checkNotEndOfExpression("IntegerEvaluator::treatGroup",p,pe);
      assert(p!=pe);
      shared_ptr<IntegerEvaluator::TExpr> pg = shared_ptr<IntegerEvaluator::TExpr>(new TGroup());
      TGroup * g = static_cast<TGroup *>(pg.get());
      bool test;
      if(s.empty()){
	test = (p!=pe);
      } else {
	test = (p!=pe)&&(*p!=s);
      }
      while(test){
	if(isNumber(*p)){
	  // number
	  istringstream converter(*p);
	  int value;
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
	} else {
	  vector<string>::const_iterator pt = p;
	  ++pt;
	  if((pt!=pe)&&(*pt=="(")){
	    string msg("IntegerEvaluator::treatGroup2 : unknown function '"+*p+"'");
	    throw(runtime_error(msg));
	  } else {
	    if(b){
	      // variable name is fixed
	      if(this->positions.find(*p)==this->positions.end()){
		vector<shared_ptr<IntegerEvaluator::TExpr> > args;
		string msg("IntegerEvaluator::treatGroup2 : ");
		msg += "unknown variable '"+*p+"'";
		throw(runtime_error(msg));
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
	string msg("IntegerEvaluator::treatGroup2 : unterminated group (group began with '"+s+"').");
	throw(runtime_error(msg));
      }
      return pg;
    } // end of IntegerEvaluator::treatGroup2

    std::vector<double>::size_type
    IntegerEvaluator::registerVariable(const std::string& vname)
    {
      using namespace std;
      typedef map<string,vector<double>::size_type>::value_type MVType;
      map<string,vector<double>::size_type>::const_iterator p;
      map<string,vector<double>::size_type>::const_iterator pe;
      vector<double>::size_type pos;
      p  = this->positions.find(vname); 
      pe = this->positions.end();
      if(p!=pe){
	return p->second;
      }
      pos = this->variables.size();
      this->positions.insert(MVType(vname,pos));
      this->variables.resize(pos+1u);
      return pos;
    } // end of IntegerEvaluator::registerVariable

    IntegerEvaluator::~IntegerEvaluator()
    {}

    void
    IntegerEvaluator::setVariableValue(const std::string& vname,
				       const int value)
    {
      using namespace std;
      map<string,vector<int>::size_type>::iterator p;
      p = this->positions.find(vname);
      if(p==this->positions.end()){
	string msg("IntegerEvaluator::setVariableValue : variable '");
	msg += vname + "' does not exist";
	throw(runtime_error(msg));
      }
      this->variables[p->second] = value;
    }//end of IntegerEvaluator::setVariableValue

    void
    IntegerEvaluator::setVariableValue(const std::vector<int>::size_type pos,
				       const int value)
    {
      using namespace std;
      if(pos>=this->variables.size()){
	ostringstream msg;
	msg << "IntegerEvaluator::setVariableValue : position '" << pos << "' is invalid ";
	if(this->variables.size()==0){
	  msg << "(function has no variable).";
	} else if(this->variables.size()==0){
	  msg << "(function has one variable).";
	} else {
	  msg << "(function has only '" <<  this->variables.size() << "' variable(s)).";
	}
	throw(runtime_error(msg.str()));
      }
      this->variables[pos] = value;
    }//end of IntegerEvaluator::setVariableValue

    int
    IntegerEvaluator::getValue(void) const
    {
      using namespace std;
      if(this->expr.get()==0){
	string msg("IntegerEvaluator::getValue : ");
	msg += "uninitialized expression.";
	throw(runtime_error(msg));
      }
      int v = this->expr->getValue();
      return v;
    }//end of IntegerEvaluator::getValue

    std::vector<std::string>
    IntegerEvaluator::getVariablesNames(void) const
    {
      using namespace std;
      typedef map<vector<int>::size_type,string>::value_type MVType;
      vector<string> res;
      map<string,vector<int>::size_type>::const_iterator p;
      map<vector<int>::size_type,string>::const_iterator p2;
      map<vector<int>::size_type,string> vnames;
      for(p=this->positions.begin();p!=this->positions.end();++p){
	vnames.insert(MVType(p->second,p->first));
      }
      for(p2=vnames.begin();p2!=vnames.end();++p2){
	res.push_back(p2->second);
      }
      return res;
    } // end of IntegerEvaluator::getVariablesNames

    std::vector<int>::size_type
    IntegerEvaluator::getNumberOfVariables(void) const
    {
      return this->variables.size();
    } // end of IntegerEvaluator::getNumberOfVariables(void) const;

    void
    IntegerEvaluator::analyse(const std::string& f,
		       const bool b)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      vector<string>::const_iterator p;
      vector<string>::const_iterator pe;
      EvaluatorBase::analyse(f);
      p  = this->tokens.begin(); 
      pe = this->tokens.end();
      shared_ptr<TExpr> g = this->treatGroup(p,pe,b,"");
      g->reduce();
      this->expr = shared_ptr<IntegerExpr>(g->analyse());
    }

    IntegerEvaluator::IntegerEvaluator()
      : expr(static_cast<tfel::math::parser::IntegerExpr*>(0))
    {} // end of IntegerEvaluator::IntegerEvaluator

    IntegerEvaluator::IntegerEvaluator(const IntegerEvaluator& src)
      : EvaluatorBase(src),
	variables(src.variables),
	positions(src.positions),
	expr(static_cast<tfel::math::parser::IntegerExpr*>(0))
    {
      this->expr = src.expr->clone(this->variables);
    } // end of IntegerEvaluator::IntegerEvaluator

    IntegerEvaluator&
    IntegerEvaluator::operator = (const IntegerEvaluator& src)
    {
      if(this==&src){
	return *this;
      }
      this->variables = src.variables;
      this->positions = src.positions;
      this->expr = src.expr->clone(this->variables);
      return *this;
    } // end of IntegerEvaluator::IntegerEvaluator

    IntegerEvaluator::IntegerEvaluator(const std::string& f)
    {
      this->setFunction(f);
    } // end of IntegerEvaluator::IntegerEvaluator

    IntegerEvaluator::IntegerEvaluator(const std::vector<std::string>& vars,
			 const std::string& f)
    {
      this->setFunction(vars,f);
    } // end of IntegerEvaluator::IntegerEvaluator

    void
    IntegerEvaluator::setFunction(const std::string& f)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      this->variables.clear();
      this->positions.clear();
      this->expr    = shared_ptr<IntegerExpr>(static_cast<IntegerExpr*>(0));
      this->analyse(f,false);
    } // end of IntegerEvaluator::setFunction

    void
    IntegerEvaluator::setFunction(const std::vector<std::string>& vars,
			   const std::string& f)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      vector<string>::const_iterator p;
      vector<int>::size_type pos;
      this->variables.clear();
      this->positions.clear();
      this->expr    = shared_ptr<IntegerExpr>(static_cast<IntegerExpr*>(0));
      this->variables.resize(vars.size());
      for(p=vars.begin(),pos=0u;p!=vars.end();++p,++pos){
	if(this->positions.find(*p)!=this->positions.end()){
	  string msg("IntegerEvaluator::setFunction : ");
	  msg += "variable '"+*p+"' multiply defined.";
	  throw(runtime_error(msg));
	}
	this->positions[*p]=pos;
      }
      this->analyse(f,true);
    } // end of IntegerEvaluator::setFunction
    
    std::vector<int>::size_type
    IntegerEvaluator::getVariablePosition(const std::string& name) const
    {
      using namespace std;
      map<string,vector<int>::size_type>::const_iterator p;
      p = this->positions.find(name);
      if(p==this->positions.end()){
	string msg("IntegerEvaluator::getVariablePosition : ");
	msg += "unknown variable '"+name+"'";
	throw(runtime_error(msg));
      }
      return p->second;
    } // end of IntegerEvaluator::getVariablePosition(const std::string&)



  } // end of namespace math
  
} // end of namespace tfel
