#include<iostream>
#include<cstdlib>
#include<iterator>

#if defined(HAVE_READLINE_READLINE_H)
#include <readline/readline.h>
#elif defined(HAVE_READLINE_H)
#include <readline.h>
#endif /* !defined(HAVE_READLINE_H) */

#ifdef HAVE_READLINE_HISTORY
#if defined(HAVE_READLINE_HISTORY_H)
#include <readline/history.h>
#elif defined(HAVE_HISTORY_H)
#include <history.h>
#endif /* defined(HAVE_READLINE_HISTORY_H) */
#endif /* HAVE_READLINE_HISTORY */

#include"TFEL/Math/Interpreter/InterpreterBase.hxx"

namespace tfel
{

  namespace math
  {
    
    namespace interpreter
    {

      struct Interpreter
	: InterpreterBase<Interpreter>
      {

	static Interpreter&
	getInterpreter();

      private:

	void
	treatAssert(TokensContainer::const_iterator&,
		    const TokensContainer::const_iterator);

	Interpreter();

	Interpreter(const Interpreter&);

	Interpreter&
	operator=(const Interpreter&);

      }; // end of struct Interpreter

      Interpreter&
      Interpreter::getInterpreter()
      {
	static Interpreter i;
	return i;
      } // end of Interpreter

      void
      Interpreter::treatAssert(TokensContainer::const_iterator& p,
			       const TokensContainer::const_iterator pe)
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::math;
	vector<string> vars;
	string group;
	this->checkNotEndOfLine("InterpreterCommon::treatAssert","",p,pe);
	group = this->readNextGroup(p,pe);
	if(group.size()<2){
	  string msg("Interpreter::treatAssert : ");
	  msg += "invalid group";
	  throw(runtime_error(msg));
	}
	if(group[0]!='('){
	  string msg("Interpreter::treatAssert : ");
	  msg += "invalid group (shall begin with a parenthesis)";
	  throw(runtime_error(msg));
	}
	if(group[group.size()-1]!=')'){
	  string msg("Interpreter::treatAssert : ");
	  msg += "invalid group (shall end with a parenthesis)";
	  throw(runtime_error(msg));
	}
	if(group.size()==2){
	  string msg("Interpreter::treatAssert : ");
	  msg += "empty group";
	  throw(runtime_error(msg));
	}
	SmartPtr<Evaluator> ev(new Evaluator(vars,group+"?1.:-1.",this->functions));
	ev->removeDependencies();
	if(ev->getNumberOfVariables()!=0u){
	  const vector<string>& ev_vars = ev->getVariablesNames();
	  ostringstream msg;
	  if(ev_vars.size()!=1u){
	    msg << "Interpreter::treatAssert : invalid print declaration (unknown variables ";
	    copy(ev_vars.begin(),ev_vars.end(),ostream_iterator<string>(msg," "));
	    msg << ")";
	  } else {
	    msg << "Interpreter::treatAssert : invalid print declaration (unknown variable "
		<< ev_vars[0] << ")";
	  }
	  throw(runtime_error(msg.str()));
	}
	if(ev->getValue()<0.){
	  cerr << "Interpreter::treatAssert : "
	       << "assertion '"+group.substr(1,group.size()-2)+"' failed"
	       << endl;
	  exit(-1);
	}
      } // end of Interpreter::treatAssert
      
      Interpreter::Interpreter()
      {
	this->registerCallBack(this->callBacks,"assert",&Interpreter::treatAssert);
      } // end of Interpreter::Interpreter

    } // end of namespace interpreter

  } // end of namespace math

} // end of namespace tfel

static void
tfel_eval_addString(std::vector<std::string>& res,
			tfel::utilities::CxxTokenizer::TokensContainer::const_iterator ps,
			tfel::utilities::CxxTokenizer::TokensContainer::const_iterator pe)
{
  using namespace std;
  if(ps!=pe){
    string t;
    while(ps!=pe){
      t += ps->value;
      if(++ps!=pe){
	t += ' ';
      }
    }
    res.push_back(t);
  }
} // end of tfel_eval_addString

void
tfel_eval_tokenize(std::vector<std::string>& res,
		       const std::string& s,
		       const unsigned int n)
{
  using namespace std;
  using namespace tfel::utilities;
  struct TGTokenizer
    : public CxxTokenizer
  {
    TGTokenizer(const std::string& line,
		const unsigned int nb)
    {
      using namespace std;
      this->treatCharAsString(true);
      this->splitLine(line,static_cast<unsigned short>(nb));
      if(this->cStyleCommentOpened){
	string msg("tfel_eval_tokenize : ");
	msg += "unfinished c-style comment";
	throw(runtime_error(msg));
      }
      this->treatPreprocessorDirectives();
      this->splitTokens();
      CxxTokenizer::stripComments();
    }
  };
  TGTokenizer tokenizer(s,n);
  CxxTokenizer::TokensContainer::const_iterator p  = tokenizer.begin();
  CxxTokenizer::TokensContainer::const_iterator ps = tokenizer.begin();
  const CxxTokenizer::TokensContainer::const_iterator pe = tokenizer.end();
  res.clear();
  while(p!=pe){
    if(p->value=="#"){
      tfel_eval_addString(res,ps,p);
      return;
    } else if(p->value==";"){
      tfel_eval_addString(res,ps,p);
      ps=p;
      ++ps;
    }
    ++p;
  }
  tfel_eval_addString(res,ps,pe);
} // end of tfel_eval_tokenize

static void
tfel_eval(const std::string& line,
	  const unsigned int n)
{
  using namespace std;
  using namespace tfel::math::interpreter;
  Interpreter& a = Interpreter::getInterpreter();
  vector<string> res;
  try{
    tfel_eval_tokenize(res,line,n);
  } catch(runtime_error& e){
    cerr << "tfel_eval : error while parsing line '" 
	 << line << "'\n("
	 << e.what() << ")\nError at line " << n << endl;
    return;
  }
#ifdef HAVE_READLINE_HISTORY
  ::add_history(line.c_str());
#endif /* HAVE_READLINE_HISTORY */
  try{
    a.analyseLine(line,0);
  }
  catch(runtime_error& e){
    cout << "Error while parsing line '" << a.getCurrentLine() << "'" << endl;
    cout << e.what() << endl;
    a.clearCurrentLine();
  }
} // end of tfel_eval

int main(const int argc,
	 const char * const * const argv)
{
  using namespace std;
  unsigned int n;
  vector<string> inputFiles;
  const char* const * p;
  vector<string>::const_iterator p2;
  // looking for input files
  for(p=argv+1;p!=argv+argc;++p){
    inputFiles.push_back(*p);
  }
  n=1;
  // treating input Files
  for(p2=inputFiles.begin();p2!=inputFiles.end();++p2){
    ifstream file(p2->c_str());
    while(!file.eof()){
      string fline;
      getline(file,fline);
      if(!fline.empty()){
	tfel_eval(fline,n);
      }
      ++n;
    }
  }
  // user inputs
#ifdef HAVE_READLINE
  char *line;
  line = ::readline("tfel-eval> ");
  while(line){
    ++n;
    if(*line){
      if(strlen(line)!=0){
	tfel_eval(line,n);
      }
    }
    ::free(line);
    line = ::readline("tfel-eval> ");
  }
#else
  string line;
  cout << "tfel-eval> ";
  if(!cin.eof()){
    getline(cin,line);
  }
  while(!cin.eof()){
    ++n;
    tfel_eval(line,n);
    line.clear();
    cout << "tfel-eval> ";
    getline(cin,line);
  }
#endif /* HAVE_READLINE */
  return EXIT_SUCCESS;
} // end of main
