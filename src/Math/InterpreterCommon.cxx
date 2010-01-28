/*!
 * \file   InterpreterCommon.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   08 mar 2008
 */

#include<cstdlib>
#include<cstdio>
#include<stdexcept>
#include<algorithm>
#include<iterator>
#include<sstream>

#include"TFEL/System/System.hxx"
#include"TFEL/System/SignalManager.hxx"
#include"TFEL/System/ProcessManager.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"

#include"TFEL/Math/Parser/ExternalFunctionManager.hxx"
#include"TFEL/Math/Parser/ExternalCFunction.hxx"
#include"TFEL/Math/Parser/ExternalCastemFunction.hxx"

#ifdef HAVE_OCTAVE
#include"TFEL/Math/Parser/ExternalOctaveFunction.hxx"
#endif /* HAVE_OCTAVE */

#ifdef HAVE_GSL
#include"TFEL/Math/Parser/EvaluatorGSLWrapper.hxx"
#endif

#include"TFEL/Math/Interpreter/InterpreterCommon.hxx"

namespace tfel
{  

  namespace math
  {

    namespace interpreter
    {

      struct InterpreterCommon::ImportOptions
      {
	enum ImportType{
	  Function,
	  Data,
	  Default
	};
	enum FunctionType{
	  C,
	  Castem,
#ifdef HAVE_OCTAVE 
	  Octave,
#endif /* HAVE_OCTAVE */
	  FDefault
	};
	ImportOptions()
	  : type(Default),
	    functionType(FDefault),
	    numberOfVariables(-1)
	{}
	ImportType type;
	FunctionType functionType;
	int numberOfVariables;
      }; // end of InterpreterCommon::ImportOptions


      InterpreterCommon::InterpreterCommon()
	: functions(new tfel::math::parser::ExternalFunctionManager())
      {
	this->treatCharAsString(true);
      } // end of InterpreterCommon::InterpreterCommon()

#ifdef HAVE_GSL
      void
      InterpreterCommon::treatSetGSL(TokensContainer::const_iterator& p, 
				     const TokensContainer::const_iterator pe)
      {
	using namespace std;
	using namespace tfel::math::parser;
	this->readSpecifiedToken("InterpreterCommon::treatSetGSL","mode",p,pe);
	this->checkNotEndOfLine("InterpreterCommon::treatSetGSL","",p,pe);
	if(p->value=="GSL_PREC_DOUBLE"){
	  getGSLMode()=GSL_PREC_DOUBLE;
	} else if (p->value=="GSL_PREC_SINGLE"){
	  getGSLMode()=GSL_PREC_SINGLE;
	} else if (p->value=="GSL_PREC_APPROX"){
	  getGSLMode()=GSL_PREC_APPROX;
	} else {
	  string msg("InterpreterCommon::treatSetGSL : ");
	  msg += "unknown gsl mode '"+p->value+"'";
	  throw(runtime_error(msg));
	}
	++p;
      } // end of InterpreterCommon::treatSetGSL
#endif
        
      tfel::utilities::SmartPtr<tfel::math::Evaluator>
      InterpreterCommon::readFunction(TokensContainer::const_iterator& p, 
				      const TokensContainer::const_iterator pe)
      {
	using namespace std;
	using namespace tfel::math;
	using namespace tfel::utilities;
	vector<string> vars;
	string f;
	unsigned short openedParenthesis = 0;
	bool cont=true;
	TokensContainer::const_iterator prev = p;
	TokensContainer::const_iterator pb   = p;
	while((p!=pe)&&(cont)){
	  if((p->value==",")&&(openedParenthesis==0)){
	    cont = false;
	  } else if(p->value=="("){
	    ++openedParenthesis;
	    f += p->value;
	    prev=p;
	    ++p;
	  } else if(p->value==")"){
	    if(openedParenthesis==0){
	      string msg("InterpreterCommon::treatFunctionPlot : ");
	      msg += "unbalanced parenthesis";
	      throw(runtime_error(msg));
	    }
	    --openedParenthesis;
	    f += p->value;
	    prev=p;
	    ++p;
	  } else {
	    if((openedParenthesis==0)&&(prev!=pb)){
	      if((prev->value=="+")||
		 (prev->value=="-")||
		 (prev->value=="*")||
		 (prev->value=="/")||
		 (prev->value=="**")){
		f += p->value;
		prev=p;
		++p;
	      } else {
		cont = false;
	      }
	    } else {
	      f += p->value;
	      prev=p;
	      ++p;
	    }
	  }
	}
	if(openedParenthesis!=0){
	  string msg("InterpreterCommon::readFunction : ");
	  msg += "unmatched parenthesis";
	  throw(runtime_error(msg));
	}
	return SmartPtr<Evaluator>(new Evaluator(vars,f,this->functions));
      } // end of InterpreterCommon::readFunction

      void
      InterpreterCommon::treatQuit(TokensContainer::const_iterator&, 
				   const TokensContainer::const_iterator)
      {
	::exit(EXIT_SUCCESS);
      } // end of InterpreterCommon::treatQuit

      bool
      InterpreterCommon::isUnsignedInteger(const std::string& s)
      {
	using namespace std;
	string::const_iterator p;
	for(p=s.begin();p!=s.end();++p){
	  if(!isdigit(*p)){
	    return false;
	  }
	}
	return true;
      } // end of InterpreterCommon::isUnsignedInteger

      double
      InterpreterCommon::readDouble(const std::string& s,
				    const unsigned short l)
      {
	using namespace std;
	double res;
	istringstream is(s);
	is >> res;
	if(!is&&(!is.eof())){
	  ostringstream msg;
	  msg << "InterpreterCommon::readDouble : ";
	  msg << "could not read value from token '"+s+"'.\n";
	  msg << "Error at line : " << l;
	  throw(runtime_error(msg.str()));
	}
	return res;
      } // end of InterpreterCommon::readDouble

      std::vector<std::string>
      InterpreterCommon::readVariableList(TokensContainer::const_iterator& p,
					  const TokensContainer::const_iterator pe)
      {
	using namespace std;
	vector<string> vars;
	if(p==pe){
	  string msg("InterpreterCommon::readVariableList : ");
	  msg += "unexpected end of line (expected ')').";
	  throw(runtime_error(msg));
	}
	if(p->value!="("){
	  string msg("InterpreterCommon::readVariableList : ");
	  msg += "unexpected token (read '"+p->value+", 'expected ')').";
	  throw(runtime_error(msg));
	}
	++p;
	if(p==pe){
	  string msg("InterpreterCommon::readVariableList : ");
	  msg += "unexpected end of line";
	  throw(runtime_error(msg));
	}
	if(!this->isValidIdentifier(p->value)){
	  string msg("InterpreterCommon::readVariableList : ");
	  msg += p->value+" is not a valid identifer.";
	  throw(runtime_error(msg));
	}
	vars.push_back(p->value);
	++p;
	if(p==pe){
	  string msg("InterpreterCommon::readVariableList : ");
	  msg += "unexpected end of line";
	  throw(runtime_error(msg));
	}
	while(p->value==","){
	  ++p;
	  if(p==pe){
	    string msg("InterpreterCommon::readVariableList : ");
	    msg += "unexpected end of line";
	    throw(runtime_error(msg));
	  }
	  if(!this->isValidIdentifier(p->value)){
	    string msg("InterpreterCommon::readVariableList : ");
	    msg += p->value+" is not a valid variable name.";
	    throw(runtime_error(msg));
	  }
	  vars.push_back(p->value);
	  ++p;
	  if(p==pe){
	    string msg("InterpreterCommon::readVariableList : ");
	    msg += "unexpected end of line";
	    throw(runtime_error(msg));
	  }
	}
	if(p->value!=")"){
	  string msg("InterpreterCommon::readVariableList : ");
	  msg += "unexpected token '"+p->value+"' (expected ')')";
	  throw(runtime_error(msg));
	}
	++p;
	return vars;
      } // end of InterpreterCommon::readVariableList

      void
      InterpreterCommon::treatImport(TokensContainer::const_iterator& p,
				     const TokensContainer::const_iterator pe,
				     const bool b)
      {
	using namespace std;
	ImportOptions options;
	if(p==pe){
	  string msg("InterpreterCommon::treatImport : ");
	  msg += "unexpected end of line";
	  throw(runtime_error(msg));
	}
	if(p->value=="<"){
	  ++p;
	  if(p==pe){
	    string msg("InterpreterCommon::treatImport : ");
	    msg += "unexpected end of line";
	    throw(runtime_error(msg));
	  }
	  // import options
	  while(p->value!=">"){
	    if(p->value=="function"){
	      if(options.type!=ImportOptions::Default){
		string msg("InterpreterCommon::treatImport : ");
		msg += "import type has already been specified";
		throw(runtime_error(msg));
	      }
	      options.type=ImportOptions::Function;
	    } else if (p->value=="c"){
	      if(!((options.type==ImportOptions::Default)||
		   (options.type==ImportOptions::Function))){
		string msg("InterpreterCommon::treatImport : ");
		msg += "import type is neither 'Default' nor 'Function', ";
		msg += "which is required for the 'c' option";
		throw(runtime_error(msg));
	      }
	      if(options.functionType!=ImportOptions::FDefault){
		string msg("InterpreterCommon::treatImport : ");
		msg += "a function type has already been specified.";
		throw(runtime_error(msg));
	      }
	      options.type=ImportOptions::Function;
	      options.functionType=ImportOptions::C;
	    } else if (p->value=="castem"){
	      if(!((options.type==ImportOptions::Default)||
		   (options.type==ImportOptions::Function))){
		string msg("InterpreterCommon::treatImport : ");
		msg += "import type is neither 'Default' nor 'Function', ";
		msg += "which is required for the 'castem' option";
		throw(runtime_error(msg));
	      }
	      if(options.functionType!=ImportOptions::FDefault){
		string msg("InterpreterCommon::treatImport : ");
		msg += "a function type has already been specified.";
		throw(runtime_error(msg));
	      }
	      options.type=ImportOptions::Function;
	      options.functionType=ImportOptions::Castem;
#ifdef HAVE_OCTAVE 
	    } else if (p->value=="octave"){
	      if(!((options.type==ImportOptions::Default)||
		   (options.type==ImportOptions::Function))){
		string msg("InterpreterCommon::treatImport : ");
		msg += "import type is neither 'Default' nor 'Function', ";
		msg += "which is required for the 'octave' option";
		throw(runtime_error(msg));
	      }
	      if(options.functionType!=ImportOptions::FDefault){
		string msg("InterpreterCommon::treatImport : ");
		msg += "a function type has already been specified.";
		throw(runtime_error(msg));
	      }
	      options.type=ImportOptions::Function;
	      options.functionType=ImportOptions::Octave;
#endif /* HAVE_OCTAVE */
	    } else if(p->value=="data"){
	      if(options.functionType!=ImportOptions::FDefault){
		string msg("InterpreterCommon::treatImport : ");
		msg += "a function type has been specified, which is inconsistent with 'data' options";
		throw(runtime_error(msg));
	      }
	      if(options.type!=ImportOptions::Default){
		string msg("InterpreterCommon::treatImport : ");
		msg += "import type has already been specified";
		throw(runtime_error(msg));
	      }
	      options.type=ImportOptions::Data;
	    } else {
	      // must be the number of variables
	      unsigned short nbr;
	      try{
		nbr = this->convertToUnsignedShort(p->value);
	      } catch(runtime_error& e){
		string msg("InterpreterCommon::treatImport : ");
		msg += "unknown option '"+p->value+"'";
		throw(runtime_error(msg));
	      }
	      if(options.numberOfVariables!=-1){
		string msg("InterpreterCommon::treatImport : ");
		msg += "the number of variables has alread been specified";
		throw(runtime_error(msg));
	      }
	      options.numberOfVariables = static_cast<int>(nbr);
	    }
	    ++p;
	    if(p==pe){
	      string msg("InterpreterCommon::treatImport : ");
	      msg += "unexpected end of line";
	      throw(runtime_error(msg));
	    }
	    if((p->value!=">")&&
	       (p->value!=",")){
	      string msg("InterpreterCommon::treatImport : ");
	      msg += "unexpected token '"+p->value+"', expected ',' or '>'";
	      throw(runtime_error(msg));
	    }
	    if(p->value==","){
	      ++p;
	    }
	  }
	  ++p;	
	}
	if((options.type==ImportOptions::Default)||
	   (options.type==ImportOptions::Function)){
	  this->importFunction(p,pe,options,b);
	} else {
	  string msg("InterpreterCommon::treatImport : ");
	  msg += "only function import is supported";
	  throw(runtime_error(msg));
	}
      } // end of InterpreterCommon::importFunction

#ifdef HAVE_OCTAVE
      void
      InterpreterCommon::importOctaveFunction(TokensContainer::const_iterator&, 
					      const TokensContainer::const_iterator,
					      const std::string& f,
					      const unsigned short nbr,
					      const bool b)
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::math::parser;
	typedef SmartPtr<ExternalFunction> EFunctionPtr;
	this->addFunction(f,EFunctionPtr(new ExternalOctaveFunction(f,nbr)),b,false);
      } // end of Graph::importOctaveFunction
#endif /* HAVE_OCTAVE*/
    
      void
      InterpreterCommon::treatImport(TokensContainer::const_iterator& p,
				     const TokensContainer::const_iterator pe)
      {
	this->treatImport(p,pe,false);
      } // end of InterpreterCommon::treatImport

      unsigned short
      InterpreterCommon::convertToUnsignedShort(const std::string& value)
      {
	using namespace std;
	string::const_iterator p;
	istringstream converter(value);
	for(p=value.begin();p!=value.end();++p){
	  if(!isdigit(*p)){
	    throw(runtime_error("InterpreterCommon::convertToUnsignedShort : invalid entry"));
	  }
	}
	unsigned short u;
	converter >> u;
	if(!converter&&(!converter.eof())){
	  string msg("InterpreterCommon::convertToUnsignedShort : ");
	  msg += "not read value from token '"+value+"'.\n";
	  throw(runtime_error(msg));
	}
	return u;
      } // end of InterpreterCommon::convertToUnsignedShort

      void
      InterpreterCommon::importCastemFunction(const std::string& function,
					      const std::string& library,
					      const unsigned short varNumber,
					      const bool b)
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::system;
	using namespace tfel::math::parser;
	typedef SmartPtr<ExternalFunction> EFunctionPtr;
	CastemFunctionPtr func;
	ExternalLibraryManager& elm = ExternalLibraryManager::getExternalLibraryManager();
	unsigned short nb = elm.getCastemFunctionNumberOfVariables(library,function.c_str());
	if(nb!=varNumber){
	  ostringstream msg;
	  msg << "InterpreterCommon::importCastemFunction : "
	      << "the function '" << function << "' declares "
	      << nb << " variables, not " << varNumber << " as requested";
	  throw(runtime_error(msg.str()));
	}
	func = elm.getCastemFunction(library,function.c_str());
	this->addFunction(function,EFunctionPtr(new ExternalCastemFunction(func,varNumber)),b,false);
      } // end of InterpreterCommon::importCastemFunction

      void
      InterpreterCommon::importCFunction(const std::string& function,
					 const std::string& library,
					 const unsigned short varNumber,
					 const bool b)
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::system;
	using namespace tfel::math::parser;
	typedef SmartPtr<ExternalFunction> EFunctionPtr;
	ExternalLibraryManager& elm = ExternalLibraryManager::getExternalLibraryManager();
	switch (varNumber){
	  CFunction0Ptr  func0;
	  CFunction1Ptr  func1;
	  CFunction2Ptr  func2;
	  CFunction3Ptr  func3;
	  CFunction4Ptr  func4;
	  CFunction5Ptr  func5;
	  CFunction6Ptr  func6;
	  CFunction7Ptr  func7;
	  CFunction8Ptr  func8;
	  CFunction9Ptr  func9;
	  CFunction10Ptr func10;
	  CFunction11Ptr func11;
	  CFunction12Ptr func12;
	  CFunction13Ptr func13;
	  CFunction14Ptr func14;
	  CFunction15Ptr func15;
	case 0:
	  func0= elm.getCFunction0(library,function.c_str());
	  this->addFunction(function,EFunctionPtr(new ExternalCFunction<0>(func0)),b,false);
	  break;
	case 1:
	  func1 = elm.getCFunction1(library,function.c_str());
	  this->addFunction(function,EFunctionPtr(new ExternalCFunction<1>(func1)),b,false);
	  break;
	case 2:
	  func2 = elm.getCFunction2(library,function.c_str());
	  this->addFunction(function,EFunctionPtr(new ExternalCFunction<2>(func2)),b,false);
	  break;
	case 3:
	  func3 = elm.getCFunction3(library,function.c_str());
	  this->addFunction(function,EFunctionPtr(new ExternalCFunction<3>(func3)),b,false);
	  break;
	case 4:
	  func4 = elm.getCFunction4(library,function.c_str());
	  this->addFunction(function,EFunctionPtr(new ExternalCFunction<4>(func4)),b,false);
	  break;
	case 5:
	  func5 = elm.getCFunction5(library,function.c_str());
	  this->addFunction(function,EFunctionPtr(new ExternalCFunction<5>(func5)),b,false);
	  break;
	case 6:
	  func6 = elm.getCFunction6(library,function.c_str());
	  this->addFunction(function,EFunctionPtr(new ExternalCFunction<6>(func6)),b,false);
	  break;
	case 7:
	  func7 = elm.getCFunction7(library,function.c_str());
	  this->addFunction(function,EFunctionPtr(new ExternalCFunction<7>(func7)),b,false);
	  break;
	case 8:
	  func8 = elm.getCFunction8(library,function.c_str());
	  this->addFunction(function,EFunctionPtr(new ExternalCFunction<8>(func8)),b,false);
	  break;
	case 9:
	  func9 = elm.getCFunction9(library,function.c_str());
	  this->addFunction(function,EFunctionPtr(new ExternalCFunction<9>(func9)),b,false);
	  break;
	case 10:
	  func10 = elm.getCFunction10(library,function.c_str());
	  this->addFunction(function,EFunctionPtr(new ExternalCFunction<10>(func10)),b,false);
	  break;
	case 11:
	  func11 = elm.getCFunction11(library,function.c_str());
	  this->addFunction(function,EFunctionPtr(new ExternalCFunction<11>(func11)),b,false);
	  break;
	case 12:
	  func12 = elm.getCFunction12(library,function.c_str());
	  this->addFunction(function,EFunctionPtr(new ExternalCFunction<12>(func12)),b,false);
	  break;
	case 13:
	  func13 = elm.getCFunction13(library,function.c_str());
	  this->addFunction(function,EFunctionPtr(new ExternalCFunction<13>(func13)),b,false);
	  break;
	case 14:
	  func14 = elm.getCFunction14(library,function.c_str());
	  this->addFunction(function,EFunctionPtr(new ExternalCFunction<14>(func14)),b,false);
	  break;
	case 15:
	  func15 = elm.getCFunction15(library,function.c_str());
	  this->addFunction(function,EFunctionPtr(new ExternalCFunction<15>(func15)),b,false);
	  break;
	default:
	  string msg("InterpreterCommon::importCFunction : function with more than 15 variables are not allowed.");
	  throw(runtime_error(msg));
	}
      } // end of InterpreterCommon::importCFunction

      void
      InterpreterCommon::importFunction(TokensContainer::const_iterator& p,
					const TokensContainer::const_iterator pe,
					const InterpreterCommon::ImportOptions options,
					const bool b)
      {
	using namespace std;
	this->checkNotEndOfLine("InterpreterCommon::importFunction : ",
				"expected function name",p,pe);
	string function = p->value;
	++p;
	unsigned short varNumber = 0;
	if(options.numberOfVariables==-1){
	  this->checkNotEndOfLine("InterpreterCommon::importFunction : ",
				  "expected variable list",p,pe);
	  if(p->value=="("){
	    const std::vector<std::string>& vars = this->readVariableList(p,pe);
	    varNumber = static_cast<unsigned short>(vars.size());
	  } else {
	    varNumber = 0u;
	  }
	} else {
	  if(p!=pe){
	    if(p->value=="("){
	      const std::vector<std::string>& vars = this->readVariableList(p,pe);
	      if(vars.size()!=static_cast<unsigned short>(options.numberOfVariables)){
		string msg("InterpreterCommon::importFunction : ");
		msg += "the number variables of function '"+function+"'";
		msg += "is not the same as that specified in the options";
		throw(runtime_error(msg));
	      }
	    }
	  }
	  varNumber = static_cast<unsigned short>(options.numberOfVariables);
	}
#ifdef HAVE_OCTAVE 
	if(options.functionType==ImportOptions::Octave){
	  this->importOctaveFunction(p,pe,function,varNumber,b);
	} else {
#endif /* HAVE_OCTAVE */
	  this->checkNotEndOfLine("InterpreterCommon::importFunction : ",
				  "expected library name",p,pe);
	  string library = this->readString(p,pe);
	  // adding the new function
	  if((options.functionType==ImportOptions::FDefault)||
	     (options.functionType==ImportOptions::C)){
	    this->importCFunction(function,library,varNumber,b);
	  } else if (options.functionType==ImportOptions::Castem){
	    this->importCastemFunction(function,library,varNumber,b);
	  } else {
	    string msg("InterpreterCommon::importFunction : ");
	    msg += "unknown function type";
	    throw(runtime_error(msg));
	  }
#ifdef HAVE_OCTAVE 
	}
#endif /* HAVE_OCTAVE */
      } // end of InterpreterCommon::importFunction()
      
      std::string
      InterpreterCommon::readUntil(TokensContainer::const_iterator& p,
				   const TokensContainer::const_iterator pe,
				   const std::string& delim)
      {
	using namespace std;
	string res;
	unsigned short openedParenthesis=0;
	this->checkNotEndOfLine("InterpreterCommon::readUntil","",p,pe);
	while((p!=pe)&&(p->value!=delim)){
	  if(p->value=="("){
	    ++openedParenthesis;
	  }
	  if(p->value==")"){
	    if(openedParenthesis==0){
	      string msg("InterpreterCommon::readUntil : ");
	      msg += "unbalanced parenthesis";
	      throw(runtime_error(msg));
	    }
	    --openedParenthesis;
	  }
	  res += p->value;
	  ++p;
	}
	if(p->value!=delim){
	  string msg("InterpreterCommon::readUntil : ");
	  msg += "did not find token '"+delim+"'";
	  throw(runtime_error(msg));
	}
	if(openedParenthesis!=0){
	  string msg("InterpreterCommon::readUntil : ");
	  msg += "unclosed parenthesis";
	  throw(runtime_error(msg));
	}
	++p;
	return res;
      } // end of InterpreterCommon::readUntil

      std::string
      InterpreterCommon::readNextGroup(TokensContainer::const_iterator& p,
				       const TokensContainer::const_iterator pe)
      {
	using namespace std;
	string res;
	unsigned short openedParenthesis=0;
	this->checkNotEndOfLine("InterpreterCommon::readNextGroup","",p,pe);
	while((p!=pe)&&(!((p->value==",")&&(openedParenthesis==0)))){
	  if(p->value=="("){
	    ++openedParenthesis;
	  }
	  if(p->value==")"){
	    if(openedParenthesis==0){
	      string msg("InterpreterCommon::readNextGroup : ");
	      msg += "unbalanced parenthesis";
	      throw(runtime_error(msg));
	    }
	    --openedParenthesis;
	  }
	  res += p->value;
	  ++p;
	}
	if(openedParenthesis!=0){
	  string msg("InterpreterCommon::readNextGroup : ");
	  msg += "unclosed parenthesis";
	  throw(runtime_error(msg));
	}
	return res;
      } // end of InterpreterCommon::readNextGroup

      const std::string
      InterpreterCommon::gatherTokenGroup(TokensContainer::const_iterator& p,
					  const TokensContainer::const_iterator pe)
      {
	using namespace std;
	string g;
	while(p!=pe){
	  g += p->value;
	  ++p;
	}
	return g;
      } // end of InterpreterCommon::gatherTokenGroup

      bool
      InterpreterCommon::isValidIdentifier(const std::string& name)
      {
	using namespace tfel::math;
	if(!Evaluator::isValidIdentifier(name)){
	  return false;
	}
	return CxxTokenizer::isValidIdentifier(name);
      } // end of InterpreterCommon::isValidIdentifier

      void
      InterpreterCommon::addFunction(const std::string& name,
				     tfel::utilities::SmartPtr<tfel::math::parser::ExternalFunction> pev,
				     const bool b1,
				     const bool b2)
      {
	using namespace std;
	if(!this->isValidIdentifier(name)){
	  string msg("InterpreterCommon::addFunction : ");
	  msg += "name '"+name+"' is not valid.";
	  throw(runtime_error(msg));
	}
	if(this->locks.find(name)!=this->locks.end()){
	  if((*(this->functions)).find(name)==(*(this->functions)).end()){
	    string msg("InterpreterCommon::addFunction : ");
	    msg += "internal error, a lock has been posed for a variable called '";
	    msg += name;
	    msg += "' that apparently don't exist";
	    throw(runtime_error(msg));
	  }
	  string msg("InterpreterCommon::addFunction : ");
	  msg += "a variable '"+name+"' has already been declared, with const attribute";
	  throw(runtime_error(msg));
	}
	if(b1){
	  this->locks.insert(name);
	}
	if(b2){
	  (*(this->functions))[name]=pev->resolveDependencies();
	} else {
	  (*(this->functions))[name]=pev;
	}
      } // end of InterpreterCommon::addFunction

      void
      InterpreterCommon::analyseFunctionDefinition(TokensContainer::const_iterator& p,
						   const TokensContainer::const_iterator pe,
						   const bool b1,
						   const bool b2)
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::math;
	this->checkNotEndOfLine("InterpreterCommon::analyseFunctionDefinition","",p,pe);
	string var = p->value;
	// variable or function definition
	if(!this->isValidIdentifier(var)){
	  string msg("InterpreterCommon::analyseFunctionDefinition : ");
	  msg += p->value+" is not a valid identifer.";
	  throw(runtime_error(msg));
	}
	++p;
	if(p==pe){
	  string msg("InterpreterCommon::analyseFunctionDefinition : ");
	  msg += "unexpected end of line";
	  throw(runtime_error(msg));
	}
	if(p->value=="="){
	  vector<string> vars;
	  // adding a variable
	  ++p;
	  if(p==pe){
	    string msg("InterpreterCommon::analyseFunctionDefinition : ");
	    msg += "unexpected end of line";
	    throw(runtime_error(msg));
	  }
	  string group = this->gatherTokenGroup(p,pe);
	  if(group.empty()){
	    string msg("InterpreterCommon::analyseFunctionDefinition : ");
	    msg += "invalid declaraction of variable "+var;
	    throw(runtime_error(msg));
	  }
	  SmartPtr<tfel::math::parser::ExternalFunction> pev(new Evaluator(vars,group,functions));
	  Evaluator* ev = static_cast<Evaluator *>(pev.getPtr());
	  if(ev->getNumberOfVariables()!=0u){
	    string msg("InterpreterCommon::analyseFunctionDefinition : ");
	    msg += "error while declaring variable "+var;
	    if(ev->getNumberOfVariables()==1u){
	      msg += ", unknown variable "+ev->getVariablesNames()[0];
	    } else {
	      const std::vector<string>& evars = ev->getVariablesNames();		
	      std::vector<string>::const_iterator pv;
	      msg += ", unknown variables ";
	      for(pv=evars.begin();pv!=evars.end();){
		msg+=*pv;
		++pv;
		if(pv!=evars.end()){
		  msg+=",";
		}
	      }
	    }
	    throw(runtime_error(msg));
	  }
	  this->addFunction(var,pev,b1,b2);
	} else if (p->value=="("){
	  SmartPtr<tfel::math::parser::ExternalFunction> ev;
	  vector<string>::const_iterator p2;
	  // adding a new function
	  vector<string> vars = this->readVariableList(p,pe);
	  if(vars.empty()){
	    string msg("InterpreterCommon::analyseFunctionDefinition : ");
	    msg += "no variable defined";
	    throw(runtime_error(msg));
	  }
	  if(p==pe){
	    string msg("InterpreterCommon::analyseFunctionDefinition : ");
	    msg += "unexpected end of line";
	    throw(runtime_error(msg));
	  }
	  if(p->value!="="){
	    string msg("InterpreterCommon::analyseFunctionDefinition : ");
	    msg += "unexpected token '"+p->value+"' (expected '=')";
	    throw(runtime_error(msg));
	  }
	  ++p;
	  if(p==pe){
	    string msg("InterpreterCommon::analyseFunctionDefinition : ");
	    msg += "unexpected end of line";
	    throw(runtime_error(msg));
	  }
	  string group = this->gatherTokenGroup(p,pe);
	  if(group.empty()){
	    string msg("InterpreterCommon::analyseFunctionDefinition : ");
	    msg += "invalid declaraction of function "+var;
	    throw(runtime_error(msg));
	  }
	  ev = SmartPtr<tfel::math::parser::ExternalFunction> (new Evaluator(vars,group,functions));
	  this->addFunction(var,ev,b1,b2);
	} else {
	  string msg("InterpreterCommon::analyseFunctionDefinition : ");
	  msg += "unexpected token ('"+p->value+"')";
	  throw(runtime_error(msg));
	}
      } // end of InterpreterCommon::analyseFunctionDefinition

      void
      InterpreterCommon::treatConst(TokensContainer::const_iterator& p,
				    const TokensContainer::const_iterator pe)
      {
	this->analyseFunctionDefinition(p,pe,true,true);
      } // end of InterpreterCommon::treatConst

      void
      InterpreterCommon::treatLock(TokensContainer::const_iterator& p,
				   const TokensContainer::const_iterator pe)
      {
	this->analyseFunctionDefinition(p,pe,true,false);
      } // end of InterpreterCommon::treatLock

      void
      InterpreterCommon::treatNoDeps(TokensContainer::const_iterator& p,
				     const TokensContainer::const_iterator pe)
      {
	this->analyseFunctionDefinition(p,pe,false,true);
      } // end of InterpreterCommon::treatNoDeps
    
      std::vector<std::string>
      InterpreterCommon::tokenize(const std::string& s,
				  const char c)
      {
	using namespace std;
	vector<string> res;
	string::size_type b = 0u;
	string::size_type e = s.find_first_of(c, b);
	while (string::npos != e || string::npos != b){
	  // Found a token, add it to the vector.
	  res.push_back(s.substr(b, e - b));
	  b = s.find_first_not_of(c, e);
	  e = s.find_first_of(c, b);
	}
	return res;
      } // end of InterpreterCommon::tokenize

      std::string
      InterpreterCommon::stripComments(const std::string& line)
      {
	using namespace std;
	const vector<string>& res = this->tokenize(line,'#');
	return res[0];
      } // end of InterpreterCommon::stripComments

      InterpreterCommon::~InterpreterCommon()
      {} // end of InterpreterCommon::~InterpreterCommon()

    } // end of namespace interpreter

  } // end of  namespace math

} // end of namespace tfel

