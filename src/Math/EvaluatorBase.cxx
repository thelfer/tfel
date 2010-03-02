/*!
 * \file   EvaluatorBase.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#include<sstream>
#include<stdexcept>
#include<iterator>
#include<algorithm>

#include"TFEL/Math/Parser/EvaluatorBase.hxx"

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      std::string
      EvaluatorBase::readNumber(std::string::iterator& p,
				std::string::iterator  pe)
      {
	using namespace std;
	string word;
	string::iterator b=p;
	//reading decimal part
	while((isdigit(*p))&&(p!=pe)){
	  ++p;
	}
	if(p!=pe){
	  if(*p=='.'){
	    ++p;
	    while((isdigit(*p))&&(p!=pe)){
	      ++p;
	    }
	  }
	}
	if(p!=pe){
	  if(*p=='e'){
	    ++p;
	    if(p==pe){
	      --p;
	    } else {
	      if((*p=='+')||(*p=='-')){
		++p;
		if(p==pe){
		  --(--p);
		} else if (isdigit(*p)){
		  while((isdigit(*p))&&(p!=pe)){
		    ++p;
		  }
		} else {
		  --(--p);
		}
	      } else if (isdigit(*p)){
		while((isdigit(*p))&&(p!=pe)){
		  ++p;
		}
	      } else {
		--p;
	      }
	    }
	  }
	}
	copy(b,p,back_inserter(word));
	return word;
      } // end of EvaluatorBase::readNumber

      inline
      void
      tokenize(std::vector<std::string>& res,
	       std::string::iterator& b,
	       std::string::iterator& p2,
	       const char c)
      {
	using namespace std;
	char w[2];
	w[0] = c;
	w[1] = '\0';
	string word;
	if(p2!=b){
	  copy(b,p2,back_inserter(word));
	  res.push_back(word);
	}
	res.push_back(w);
	++p2;
	b=p2;
      }

      void
      EvaluatorBase::splitAtTokenSeperator(std::vector<std::string>& tokens)
      {
	using namespace std;
    
	vector<string> res;
	vector<string>::iterator p;
	string::iterator p2;
	string::iterator p2e;
	string::iterator b;

	for(p=tokens.begin();p!=tokens.end();++p){
	  b   = p->begin();
	  p2  = p->begin();
	  p2e = p->end();
	  while(p2!=p2e){
	    string word;
	    if(isdigit(*p2)&&(p2==b)){
	      word = readNumber(p2,p2e);
	      res.push_back(word);
	      b=p2;
	    } else if(*p2=='+'){
	      tokenize(res,b,p2,'+');
	    } else if(*p2=='!'){
	      tokenize(res,b,p2,'!');
	    } else if(*p2=='-'){
	      tokenize(res,b,p2,'-');
	    } else if(*p2=='*'){
	      if(p2!=b){
		copy(b,p2,back_inserter(word));
		res.push_back(word);
	      }
	      ++p2;
	      if(p2!=p2e){
		if(*p2=='*'){
		  res.push_back("**");
		  ++p2;
		} else {
		  res.push_back("*");
		}
	      } else {
		res.push_back("*");
	      }
	      b=p2;
	    } else if(*p2=='|'){
	      if(p2!=b){
		copy(b,p2,back_inserter(word));
		res.push_back(word);
	      }
	      ++p2;
	      if(p2==p2e){
		string msg("EvaluatorBase::splitAtTokenSeperator : ");
		msg += "unexpected end of string";
		throw(runtime_error(msg));
	      }
	      if(*p2!='|'){
		string msg("EvaluatorBase::splitAtTokenSeperator : ");
		msg += "expected character '|' (read '";
		msg += *p2;
		msg += "')";
		throw(runtime_error(msg));
	      } 
	      res.push_back("||");
	      ++p2;
	      b=p2;
	    } else if(*p2=='&'){
	      if(p2!=b){
		copy(b,p2,back_inserter(word));
		res.push_back(word);
	      }
	      ++p2;
	      if(p2==p2e){
		string msg("EvaluatorBase::splitAtTokenSeperator : ");
		msg += "unexpected end of string";
		throw(runtime_error(msg));
	      }
	      if(*p2!='&'){
		string msg("EvaluatorBase::splitAtTokenSeperator : ");
		msg += "expected character '&&' (read '";
		msg += *p2;
		msg += "')";
		throw(runtime_error(msg));
	      } 
	      res.push_back("&&");
	      ++p2;
	      b=p2;
	    } else if(*p2=='>'){
	      if(p2!=b){
		copy(b,p2,back_inserter(word));
		res.push_back(word);
	      }
	      ++p2;
	      if(p2!=p2e){
		if(*p2=='='){
		  res.push_back(">=");
		  ++p2;
		} else {
		  res.push_back(">");
		}
	      } else {
		res.push_back(">");
	      }
	      b=p2;
	    } else if(*p2=='<'){
	      if(p2!=b){
		copy(b,p2,back_inserter(word));
		res.push_back(word);
	      }
	      ++p2;
	      if(p2!=p2e){
		if(*p2=='='){
		  res.push_back("<=");
		  ++p2;
		} else {
		  res.push_back("<");
		}
	      } else {
		res.push_back("<");
	      }
	      b=p2;
	    } else if(*p2=='='){
	      if(p2!=b){
		copy(b,p2,back_inserter(word));
		res.push_back(word);
	      }
	      ++p2;
	      if(p2!=p2e){
		if(*p2=='='){
		  res.push_back("==");
		  ++p2;
		} else {
		  res.push_back("=");
		}
	      } else {
		res.push_back("=");
	      }
	      b=p2;
	    } else if(*p2=='/'){
	      tokenize(res,b,p2,'/');
	    } else if(*p2=='('){
	      tokenize(res,b,p2,'(');
	    } else if(*p2==')'){
	      tokenize(res,b,p2,')');
	    } else if(*p2==','){
	      tokenize(res,b,p2,',');
	    } else if(*p2=='?'){
	      tokenize(res,b,p2,'?');
	    } else if(*p2==':'){
	      tokenize(res,b,p2,':');
	    } else {
	      ++p2;
	    }
	  }
	  if(b!=p2e){
	    string word;
	    copy(b,p2,back_inserter(word));
	    res.push_back(word);
	  }
	}

	tokens.swap(res);
      }

      void
      EvaluatorBase::analyse(const std::string& expr)
      {
	using namespace std;
	istringstream tokenizer(expr);
	vector<string>::iterator p;
	vector<string>::iterator pe;
	copy(istream_iterator<string>(tokenizer),
	     istream_iterator<string>(),back_inserter(this->tokens));
	EvaluatorBase::splitAtTokenSeperator(this->tokens);
	p  = this->tokens.begin();
	pe = this->tokens.end();
      } // end of EvaluatorBase::analyse

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
