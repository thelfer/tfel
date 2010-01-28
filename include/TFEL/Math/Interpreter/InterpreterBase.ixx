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
      void
      InterpreterBase<Child>::registerCallBack(std::map<std::string,
					       InterpreterBase<Child>::MemFuncPtr>& m,
					       const std::string& name,
					       const InterpreterBase<Child>::MemFuncPtr f)
      {
	using namespace std;
	typedef std::map<string,MemFuncPtr>::value_type MVType;
	m.insert(MVType(name,f));      
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
	(this->*(ps->second))(p,pe);
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
	(this->*(ps->second))(p,pe);
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
	std::map<string,MemFuncPtr>::const_iterator pf;
	while(p!=pe){
	  pf = this->callBacks.find(p->value);
	  if(pf==this->callBacks.end()){
	    this->analyseFunctionDefinition(p,pe,false,false);
	  } else {
	    ++p;
	    (this->*(pf->second))(p,pe);
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
	    this->analyseLine(line,nbr);
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
	this->currentLineNbr = nbr;
	string nline = this->stripComments(line);
	bool treatLine = false;
	if(nline.empty()){
	  treatLine = true;
	} else {
	  string::size_type pos = nline.find_last_not_of(' ');
	  if(pos!=string::npos){
	    // line is not empty
	    if(nline[pos]=='\\'){
	      nline[pos] = ' ';
	      this->currentLine += nline;
	    } else {
	      this->currentLine += nline;
	      treatLine = true;
	    }
	  }
	}
	if(treatLine){
	  if(!this->currentLine.empty()){
	    if(this->currentLine[0]=='!'){
	      using namespace tfel::system;
	      ProcessManager m;
	      string::iterator p = this->currentLine.begin();
	      ++p;
	      string cmd(p,this->currentLine.end());
	      m.execute(cmd);
	      this->currentLine.clear();
	    } else {
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
	  }
	}
      } // end of InterpreterBase<Child>::analyseFile

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
