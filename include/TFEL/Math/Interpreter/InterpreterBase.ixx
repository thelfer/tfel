/*! 
 * \file  InterpreterBase.ixx
 * \brief
 * \author Helfer Thomas
 * \brief 28 jan 2010
 */

#ifndef _LIB_TFEL_MATH_INTERPRETER_INTERPRETERBASE_IXX_
#define _LIB_TFEL_MATH_INTERPRETER_INTERPRETERBASE_IXX_ 

namespace tfel
{  

  namespace math
  {

    namespace interpreter
    {
      
      template<typename Child>
      InterpreterBase<Child>::InterpreterBase()
      {
	using namespace std;
	using namespace tfel::math;
	using namespace tfel::utilities;
	using namespace tfel::math::parser;
	typedef ExternalFunctionManager::value_type MVType;
	typedef SmartPtr<ExternalFunction> ExternalFunctionPtr;
	stringstream pi;
	pi.precision(15);
	pi << M_PI;
	ExternalFunctionPtr p(new Evaluator(pi.str()));
	this->functions->insert(MVType("pi",p));
	this->locks.insert("pi");
	this->registerCallBacks();
      } // end of InterpreterBase<Child>::InterpreterBase

      template<typename Child>
      void
      InterpreterBase<Child>::registerCallBack(std::map<std::string,
					       InterpreterBase<Child>::MemFuncPtr>& m,
					       const std::string& name,
					       const InterpreterBase<Child>::MemFuncPtr f)
      {
	using namespace std;
	typedef typename map<string,MemFuncPtr>::value_type MVType;
	if(!m.insert(MVType(name,f)).second){
	  string msg("InterpreterBase<Child>::registerCallBack : ");
	  msg += "keyword '"+name+"' already declared";
	  throw(runtime_error(msg));
	}
      } // end of InterpreterBase<Child>::registerCallBack
	
      template<typename Child>
      void
      InterpreterBase<Child>::registerCallBacks()
      {
	this->registerCallBack(this->callBacks,"q",&Child::treatQuit);
	this->registerCallBack(this->callBacks,"quit",&Child::treatQuit);
	this->registerCallBack(this->callBacks,"import",&Child::treatImport);
	this->registerCallBack(this->callBacks,"const",&Child::treatConst);
	this->registerCallBack(this->callBacks,"lock",&Child::treatLock);
	this->registerCallBack(this->callBacks,"nodeps",&Child::treatNoDeps);
	this->registerCallBack(this->callBacks,"include",&Child::treatInclude);
	this->registerCallBack(this->callBacks,"se",&Child::treatSet);
	this->registerCallBack(this->callBacks,"set",&Child::treatSet);
	this->registerCallBack(this->callBacks,"unset",&Child::treatUnset);
	this->registerCallBack(this->callBacks,"kriging",&Child::treatKriging);
	this->registerCallBack(this->callBacks,"print",&Child::treatPrint);
#ifdef HAVE_GSL
	this->registerCallBack(this->setCallBacks,"gsl",&Child::treatSetGSL);
#endif
      } // end of InterpreterBase<Child>::registerCallBacks()

      template<typename Child>
      void
      InterpreterBase<Child>::treatSet(TokensContainer::const_iterator& p,
				       const TokensContainer::const_iterator pe)
      {
	using namespace std;
	this->checkNotEndOfLine("InterpreterBase<Child>::treatSet","",p,pe);
	typename map<string,MemFuncPtr>::const_iterator ps;
	ps = this->setCallBacks.find(p->value);
	if(ps == this->setCallBacks.end()){
	  string msg("InterpreterBase<Child>::treatSet : ");
	  msg += "unknown option '"+p->value+"'";
	  throw(runtime_error(msg));
	}
	++p;
	(static_cast<Child *>(this)->*(ps->second))(p,pe);
      } // end of InterpreterBase<Child>::treatSet

      template<typename Child>
      void
      InterpreterBase<Child>::treatUnset(TokensContainer::const_iterator& p,
					 const TokensContainer::const_iterator pe)
      {
	using namespace std;
	this->checkNotEndOfLine("InterpreterBase<Child>::treatUnset","",p,pe);
	typename map<string,MemFuncPtr>::const_iterator ps;
	ps = this->unsetCallBacks.find(p->value);
	if(ps == this->unsetCallBacks.end()){
	  string msg("InterpreterBase<Child>::treatUnset : ");
	  msg += "unknown option '"+p->value+"'";
	  throw(runtime_error(msg));
	}
	++p;
	(static_cast<Child*>(this)->*(ps->second))(p,pe);
      } // end of InterpreterBase<Child>::treatUnset

      template<typename Child>
      void
      InterpreterBase<Child>::treatInclude(TokensContainer::const_iterator& p, 
					   const TokensContainer::const_iterator pe)
      {
	using namespace std;
	TokensContainer::const_iterator p2;
	TokensContainer::difference_type pos;
	this->checkNotEndOfLine("InterpreterBase<Child>::treatInclude",
				"expected file name",p,pe);
	if(p->flag!=tfel::utilities::Token::String){
	  string msg("InterpreterBase<Child>::include : ");
	  msg += "expected to read a string (read '"+p->value+"').\n";
	  throw(runtime_error(msg));
	}
	p2 = this->fileTokens.begin();
	pos = 0;
	while(p2!=p){
	  ++p2;
	  ++pos;
	}
	TokensContainer tmp(fileTokens);
	this->analyseFile(p->value.substr(1,p->value.size()-2));
	this->fileTokens = tmp;
	p=this->fileTokens.begin();
	advance(p,pos+1);
      } // end of InterpreterBase<Child>::treatInclude

      template<typename Child>
      void
      InterpreterBase<Child>::analyse(TokensContainer::const_iterator p,
				      const TokensContainer::const_iterator pe)
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::math;
	typename map<string,MemFuncPtr>::const_iterator pf;
	while(p!=pe){
	  pf = this->callBacks.find(p->value);
	  if(pf==this->callBacks.end()){
	    this->analyseFunctionDefinition(p,pe,false,false);
	  } else {
	    ++p;
	    (static_cast<Child*>(this)->*(pf->second))(p,pe);
	  }
	  if(p!=pe){
	    string msg("InterpreterBase<Child>::analyse : ");
	    msg += "unexpected token '"+p->value+"'";
	    throw(runtime_error(msg));
	  }
	}
      } // end of Analyse::analyse()

      template<typename Child>
      void
      InterpreterBase<Child>::analyseFile(const std::string& name)
      {
	using namespace std;
	ifstream file(name.c_str());
	if(!file){
	  string msg("InterpreterBase<Child>::analyseFile : ");
	  msg += "can't open file '"+name+"'";
	  throw(runtime_error(msg));
	}
	unsigned short nbr=1;
	while(!file.eof()){
	  string line;
	  getline(file,line);
	  try{
	    static_cast<Child *>(this)->analyseLine(line,nbr);
	  }
	  catch(runtime_error &e){
	    ostringstream msg;
	    msg << "InterpreterBase<Child>::analyseFile : error at line "
		<< nbr << " of file " << name  << endl
		<< e.what();
	    throw(runtime_error(msg.str()));
	  }
	  ++nbr;
	}
	if(!this->currentLine.empty()){
	  string msg("InterpreterBase<Child>::analyseFile : ");
	  msg += "unfinished line at end of file";
	  throw(runtime_error(msg));
	}
      } // end of InterpreterBase<Child>::analyseFile

      template<typename Child>
      void
      InterpreterBase<Child>::analyseLine(const std::string& line,
					  const unsigned short nbr)
      {
	using namespace std;
	string nline(line);
	this->currentLineNbr = nbr;
	bool treat = false;
	if(nline.empty()){
	  treat = true;
	} else {
	  string::size_type pos = nline.find_last_not_of(" \t");
	  if(pos!=string::npos){
	    // line is not empty
	    if(nline[pos]=='\\'){
	      nline[pos] = ' ';
	      this->currentLine += nline;
	    } else {
	      this->currentLine += nline;
	      treat = true;
	    }
	  }
	}
	if(treat){
	  static_cast<Child*>(this)->treatLine();
	}
      } // end of InterpreterBase<Child>::analyseFile

      template<typename Child>
      void
      InterpreterBase<Child>::treatLine()
      {
	using namespace std;
	using namespace tfel::utilities;
        if(!this->currentLine.empty()){
	  this->clear();
	  this->splitLine(this->currentLine,this->currentLineNbr);
	  if(this->cStyleCommentOpened){
	    string msg("InterpreterBase<Child>::analyseLine : ");
	    msg += "unfinished c-style comment";
	    throw(runtime_error(msg));
	  }
	  this->treatPreprocessorDirectives();
	  this->splitTokens();
	  CxxTokenizer::stripComments();
	  this->analyse(this->begin(),
			this->end());
	  this->currentLine.clear();
	}
      } // end of InterpreterBase<Child>::treatLine

      template<typename Child>
      const std::string&
      InterpreterBase<Child>::getCurrentLine(void) const
      {
	return this->currentLine;
      } // end of InterpreterBase<Child>::getCurrentLine(void) const
      
      template<typename Child>
      void
      InterpreterBase<Child>::clearCurrentLine(void)
      {
	this->currentLine.clear();
      } // end of InterpreterBase<Child>::clearCurrentLine(void)

    } // end of namespace interpreter

  } // end of  namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_INTERPRETER_INTERPRETERBASE_IXX */
