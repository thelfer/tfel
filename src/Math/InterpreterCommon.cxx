/*!
 * \file   InterpreterCommon.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   08 mar 2008
 */

#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<stdexcept>
#include<algorithm>
#include<iterator>
#include<sstream>

#include"TFEL/Utilities/TextData.hxx"

#include"TFEL/System/System.hxx"
#include"TFEL/System/SignalManager.hxx"
#include"TFEL/System/ProcessManager.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"

#include"TFEL/Math/Parser/ExternalFunctionManager.hxx"
#include"TFEL/Math/Parser/ExternalCFunction.hxx"
#include"TFEL/Math/Parser/ExternalCastemFunction.hxx"
#include"TFEL/Math/Parser/KrigedFunction.hxx"
#include"TFEL/Math/LevenbergMarquardt.hxx"
#include"TFEL/Math/LevenbergMarquardt/LevenbergMarquardtExternalFunctionWrapper.hxx"

#ifdef HAVE_OCTAVE
#include"TFEL/Math/Parser/ExternalOctaveFunction.hxx"
#endif /* HAVE_OCTAVE */

#ifdef HAVE_GSL
#include"TFEL/Math/Parser/EvaluatorGSLWrapper.hxx"
#endif

#include"TFEL/Math/Interpreter/InterpreterCommon.hxx"
#include"TFEL/Math/Interpreter/InterpreterBase.hxx"

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
        
      tfel::utilities::shared_ptr<tfel::math::Evaluator>
      InterpreterCommon::readFunction(TokensContainer::const_iterator& p, 
				      const TokensContainer::const_iterator pe)
      {
	using namespace std;
	using namespace tfel::math;
	using namespace tfel::utilities;
	using std::vector;
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
	return shared_ptr<Evaluator>(new Evaluator(vars,f,this->functions));
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

      void
      InterpreterCommon::readVariableList(std::vector<std::string>& vars,
					  TokensContainer::const_iterator& p,
					  const TokensContainer::const_iterator pe,
					  const bool b)
      {
	using namespace std;
	using std::vector;
	vars.clear();
	if(b){
	  this->readSpecifiedToken("InterpreterCommon::readVariableList","(",p,pe);
	}
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
	if(b){
	  if(p==pe){
	    string msg("InterpreterCommon::readVariableList : ");
	    msg += "unexpected end of line";
	    throw(runtime_error(msg));
	  }
	}
	while(((p!=pe))&&(p->value==",")){
	  ++p;
	  this->checkNotEndOfLine("InterpreterCommon::readVariableList",
				  "expected variable name",p,pe);
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
	  if(b){
	    if(p==pe){
	      string msg("InterpreterCommon::readVariableList : ");
	      msg += "unexpected end of line";
	      throw(runtime_error(msg));
	    }
	  }
	} // p!=pe
	if(b){
	  this->readSpecifiedToken("InterpreterCommon::readVariableList",")",p,pe);
	}
      } // end of InterpreterCommon::readVariableList

      std::vector<std::string>
      InterpreterCommon::readVariableList(TokensContainer::const_iterator& p,
					  const TokensContainer::const_iterator pe,
					  const bool b)
      {
	using namespace std;
	using std::vector;
	vector<string> vars;
	this->readVariableList(vars,p,pe,b);
	return vars;
      } // end of InterpreterCommon::readVariableList

      void
      InterpreterCommon::treatImport(TokensContainer::const_iterator& p,
				     const TokensContainer::const_iterator pe,
				     const bool b)
      {
	using namespace std;
	ImportOptions options;
	this->checkNotEndOfLine("InterpreterCommon::treatImport : ",
				"expected function name",p,pe);
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
	typedef shared_ptr<ExternalFunction> EFunctionPtr;
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
      InterpreterCommon::getData(std::vector<double>& v,
				 const tfel::utilities::TextData& data,
				 TokensContainer::const_iterator& p,
				 const TokensContainer::const_iterator pe)
      {
	using namespace std;
	using std::vector;
	this->checkNotEndOfLine("InterpreterCommon::getData : ",
				"expected column number",p,pe);
	if(p->value=="("){
	  this->getData(v,data,this->readNextGroup(p,pe));
	} else {
	  data.getColumn(v,this->convertToUnsignedShort(p->value));
	  ++p;
	}
      } // end of InterpreterCommon::getData

      std::string
      InterpreterCommon::getData(std::vector<double>& v,
				 const tfel::utilities::TextData& fdata,
				 const std::string& s)
      {
	using namespace std;
	using namespace tfel::math;
	using namespace tfel::utilities;
	using std::vector;
	if(InterpreterCommon::isUnsignedInteger(s)){
	  unsigned short c = this->convertToUnsignedShort(s);
	  fdata.getColumn(v,c);
	  return fdata.getLegend(c);
	} else {
	  // assuming a function
	  vector<pair<string,unsigned short> > vars;
	  Evaluator e(s,this->functions);
	  const vector<string>& vnames = e.getVariablesNames();
	  if(vnames.empty()){
	    string msg("InterpreterCommon::getData : ");
	    msg += "function '"+s+"' does not declare any variable";
	    throw(runtime_error(msg));
	  }
	  vector<string>::const_iterator p;
	  vector<TextData::Line>::const_iterator p2;
	  vector<pair<string,unsigned short> >::const_iterator p3;
	  for(p=vnames.begin();p!=vnames.end();++p){
	    if(((*p)[0]!='$')){
	      parser::ExternalFunctionManager::const_iterator p4;
	      p4 = this->functions->find(*p);
	      if(p4==this->functions->end()){
		string msg("InterpreterCommon::getData : ");
		msg += "invalid variable '"+*p+"'";
		throw(runtime_error(msg));
	      }
	      if(p4->second->getNumberOfVariables()!=0){
		string msg("InterpreterCommon::getData : ");
		msg += "invalid variable '"+*p+"'";
		throw(runtime_error(msg));
	      }
	      e.setVariableValue(*p,p4->second->getValue());
	    } else {
	      if(!InterpreterCommon::isUnsignedInteger(p->substr(1))){
		string msg("InterpreterCommon::getData : ");
		msg += "invalid variable name '"+*p;
		msg += "' in function '"+s+"'";
		throw(runtime_error(msg));
	      }
	      const unsigned short vc = this->convertToUnsignedShort(p->substr(1));
	      if(vc==0){
		string msg("InterpreterCommon::getData : ");
		msg += "invalid variable name "+*p;
		msg += " in function '"+s+"'.";
		throw(runtime_error(msg));
	      }
	      vars.push_back(make_pair(*p,vc));
	    }
	  }
	  for(p2=fdata.begin();p2!=fdata.end();++p2){
	    for(p3=vars.begin();p3!=vars.end();++p3){
	      if(p2->tokens.size()<p3->second){
		ostringstream msg;
		msg << "TextData::getColumn : line '" 
		    << p2->nbr << "' "
		    << "does not have '" << p3->second << "' columns.";
		throw(runtime_error(msg.str()));
	      }
	      e.setVariableValue(p3->first,
				 InterpreterCommon::readDouble(p2->tokens[p3->second-1],
							       p2->nbr));
	    }
	    v.push_back(e.getValue());
	  }
	}
	return "";
      } // end of InterpreterCommon::getData

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
	typedef shared_ptr<ExternalFunction> EFunctionPtr;
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
	typedef shared_ptr<ExternalFunction> EFunctionPtr;
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
				     tfel::utilities::shared_ptr<tfel::math::parser::ExternalFunction> pev,
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
	using std::vector;
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
	  shared_ptr<tfel::math::parser::ExternalFunction> pev(new Evaluator(vars,group,functions));
	  Evaluator* ev = static_cast<Evaluator *>(pev.get());
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
	  shared_ptr<tfel::math::parser::ExternalFunction> ev;
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
	  ev = shared_ptr<tfel::math::parser::ExternalFunction> (new Evaluator(vars,group,functions));
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
	using std::vector;
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

      void
      InterpreterCommon::treatKriging(TokensContainer::const_iterator& p,
				      const TokensContainer::const_iterator pe)
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::math;
	using namespace tfel::math::parser;
	using std::vector;
	typedef shared_ptr<ExternalFunction> EFunctionPtr;
	vector<double> vx1;
	vector<double> vx2;
	vector<double> vx3;
	vector<double> vx4;
	vector<double> vy;
	vector<double>::const_iterator pt;
	vector<double>::const_iterator pt2;
	vector<double>::const_iterator pt3;
	vector<double>::const_iterator pt4;
	int numberOfVariables = -1;
	this->checkNotEndOfLine("InterpreterCommon::treatKriging : ",
				"expected function name",p,pe);
	string function = p->value;
	string file;
	++p;
	unsigned short varNumber = 0;
	if(numberOfVariables==-1){
	  this->checkNotEndOfLine("InterpreterCommon::treatKriging : ",
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
	      if(vars.size()!=static_cast<unsigned short>(numberOfVariables)){
		string msg("InterpreterCommon::treatKriging : ");
		msg += "the number variables of function '"+function+"'";
		msg += "is not the same as that specified in the options";
		throw(runtime_error(msg));
	      }
	    }
	  }
	  varNumber = static_cast<unsigned short>(numberOfVariables);
	}
	if(varNumber==0){
	  string msg("InterpreterCommon::treatKriging : ");
	  msg += "the number variables is 0";
	  throw(runtime_error(msg));
	}
	this->checkNotEndOfLine("InterpreterCommon::treatKriging : ",
				"expected file name",p,pe);
	TextData data(this->readString(p,pe));
	if(p==pe){
	  if(varNumber==1){
	    data.getColumn(vx1,1);
	    data.getColumn(vy,2);
	    if(vx1.size()!=vy.size()){
	      string msg("InterpreterCommon::treatKriging : ");
	      msg += "columns don't have the same size";
	      throw(runtime_error(msg));
	    }
	  } else if(varNumber==2){
	    data.getColumn(vx1,1);
	    data.getColumn(vx2,2);
	    data.getColumn(vy,3);
	  } else if(varNumber==3){
	    data.getColumn(vx1,1);
	    data.getColumn(vx2,2);
	    data.getColumn(vx3,3);
	    data.getColumn(vy,4);
	  } else {
	    string msg("InterpreterCommon::treatKriging : ");
	    msg += "unsupported number of variables";
	    throw(runtime_error(msg));
	  }
	} else {
	  // using 
	  this->readSpecifiedToken("InterpreterCommon::treatKriging : ",
				   "using",p,pe);
	  this->checkNotEndOfLine("InterpreterCommon::treatKriging","",p,pe);
	  if(varNumber==1){
	    this->getData(vx1,data,p,pe);
	    this->readSpecifiedToken("InterpreterCommon::treatKriging : ",
				     ":",p,pe);
	    this->getData(vy,data,p,pe);
	  } else if(varNumber==2){
	    this->getData(vx1,data,p,pe);
	    this->readSpecifiedToken("InterpreterCommon::treatKriging : ",
				     ":",p,pe);
	    this->getData(vx2,data,p,pe);
	    this->readSpecifiedToken("InterpreterCommon::treatKriging : ",
				     ":",p,pe);
	    this->getData(vy,data,p,pe);
	  } else if(varNumber==3){
	    this->getData(vx1,data,p,pe);
	    this->readSpecifiedToken("InterpreterCommon::treatKriging : ",
				     ":",p,pe);
	    this->getData(vx2,data,p,pe);
	    this->readSpecifiedToken("InterpreterCommon::treatKriging : ",
				     ":",p,pe);
	    this->getData(vx3,data,p,pe);
	    this->readSpecifiedToken("InterpreterCommon::treatKriging : ",
				     ":",p,pe);
	    this->getData(vy,data,p,pe);
	  } else {
	    string msg("InterpreterCommon::treatKriging : ");
	    msg += "unsupported number of variables";
	    throw(runtime_error(msg));
	  }
	}
	if(varNumber==1){
	  vector<pair<double,double> > points(vx1.size());
	  vector<pair<double,double> >::size_type i;
	  for(pt=vx1.begin(),pt2=vy.begin(),i=0;pt!=vx1.end();
	      ++pt,++pt2,++i){
	    points[i] = pair<double,double>(*pt,*pt2);
	  }
	  this->addFunction(function,EFunctionPtr(new KrigedFunction<1>(points)),
			    false,false);
	} else if(varNumber==2){
	  if((vx1.size()!=vx2.size())||
	     (vx1.size()!=vy.size())){
	    string msg("InterpreterCommon::treatKriging : ");
	    msg += "columns don't have the same size";
	    throw(runtime_error(msg));
	  }
	  vector<pair<tvector<2>,double> > points(vx1.size());
	  vector<pair<tvector<2>,double> >::size_type i;
	  for(pt=vx1.begin(),pt2=vx2.begin(),pt3=vy.begin(),i=0;
	      pt!=vx1.end();++pt,++pt2,++pt3,++i){
	    tvector<2> v;
	    v(0) = *pt;
	    v(1) = *pt2;
	    points[i] = pair<tvector<2>,double>(v,*pt3);
	  }
	  this->addFunction(function,EFunctionPtr(new KrigedFunction<2>(points)),
			    false,false);
	} else if(varNumber==3){
	  if((vx1.size()!=vx2.size())||
	     (vx1.size()!=vx3.size())||
	     (vx1.size()!=vy.size())){
	    string msg("InterpreterCommon::treatKriging : ");
	    msg += "columns don't have the same size";
	    throw(runtime_error(msg));
	  }
	  vector<pair<tvector<3>,double> > points(vx1.size());
	  vector<pair<tvector<3>,double> >::size_type i;
	  for(pt=vx1.begin(),pt2=vx2.begin(),pt3=vx3.begin(),pt4=vy.begin(),i=0;
	      pt!=vx1.end();++pt,++pt2,++pt3,++pt4,++i){
	    tvector<3> v;
	    v(0) = *pt;
	    v(1) = *pt2;
	    v(2) = *pt3;
	    points[i] = pair<tvector<3>,double>(v,*pt4);
	  }
	  this->addFunction(function,EFunctionPtr(new KrigedFunction<3>(points)),
			    false,false);
	} else {
	  string msg("InterpreterCommon::treatKriging : ");
	  msg += "unsupported number of variables";
	  throw(runtime_error(msg));
	}
      } // end of InterpreterCommon::treatKriging

      void
      InterpreterCommon::treatPrint(TokensContainer::const_iterator& p,
				    const TokensContainer::const_iterator pe)
      {
	using namespace std;
	using namespace tfel::utilities;
	using namespace tfel::math;
	using namespace tfel::math::parser;
	using std::vector;
	vector<string> vars;
	bool cont = true;
	ostringstream res;
	res.precision(15);
	this->checkNotEndOfLine("InterpreterCommon::treatPrint","",p,pe);
	while((p!=pe)&&(cont)){
	  if(p->flag==Token::String){
	    res << p->value.substr(1,p->value.size()-2);
	    ++p;
	  } else {
	    string group = this->readNextGroup(p,pe);
	    if(group.empty()){
	      string msg("InterpreterCommon::treatPrint : ");
	      msg += "invalid expression";
	      throw(runtime_error(msg));
	    }
	    shared_ptr<Evaluator> ev(new Evaluator(vars,group,this->functions));
	    ev->removeDependencies();
	    if(ev->getNumberOfVariables()!=0u){
	      const vector<string>& ev_vars = ev->getVariablesNames();
	      ostringstream msg;
	      if(ev_vars.size()!=1u){
		msg << "InterpreterCommon::treatPrint : invalid print declaration (unknown variables ";
		copy(ev_vars.begin(),ev_vars.end(),ostream_iterator<string>(msg," "));
		msg << ")";
	      } else {
		msg << "InterpreterCommon::treatPrint : invalid print declaration (unknown variable "
		    << ev_vars[0] << ")";
	      }
	      throw(runtime_error(msg.str()));
	    }
	    res << ev->getValue();
	  }
	  if(p!=pe){
	    this->readSpecifiedToken("InterpreterCommon::treatPrint : ",
				     ",",p,pe);
	    this->checkNotEndOfLine("InterpreterCommon::treatPrint : ","",p,pe);
	  }
	}
	cout << res.str() << endl;
      } // end of InterpreterCommon::treatPrint

      void
      InterpreterCommon::readDataFunctionInUsingDeclaration(std::string& d,
							    TokensContainer::const_iterator& p,
							    const TokensContainer::const_iterator pe)
      {
	using namespace std;
	unsigned short openedParenthesis;
	this->checkNotEndOfLine("readDataFunctionInUsingDeclaration",
				"expected using declaration",p,pe);
	if(p->value=="("){
	++p;
	this->checkNotEndOfLine("readDataFunctionInUsingDeclaration","",p,pe);
	openedParenthesis = 0;
	while(!((p->value==")")&&(openedParenthesis==0))){
	  if(p->value=="("){
	    ++openedParenthesis;
	  }
	  if(p->value==")"){
	    if(openedParenthesis==0){
	      string msg("readDataFunctionInUsingDeclaration : ");
	      msg += "unbalanced parenthesis";
	      throw(runtime_error(msg));
	    }
	    --openedParenthesis;
	  }
	  d += p->value;
	  ++p;
	  this->checkNotEndOfLine("readDataFunctionInUsingDeclaration","",p,pe);
	}
	++p;
	} else {
	  // this shall be a column number
	  if(!InterpreterCommon::isUnsignedInteger(p->value)){
	    string msg("readDataFunctionInUsingDeclaration : ");
	    msg += "unexpected token '"+p->value+"', expected column number";
	    throw(runtime_error(msg));
	  }
	  d = p->value;
	  ++p;
	}
      } // end of InterpreterCommon::readDataFunctionInUsingDeclaration

      void
      InterpreterCommon::treatFit(TokensContainer::const_iterator& p,
				  const TokensContainer::const_iterator pe)
      {
	using namespace std;
	using namespace tfel::math;
	using namespace tfel::math::parser;
	using namespace tfel::utilities;
	using std::vector;
	typedef LevenbergMarquardtExternalFunctionWrapper ExternalFunctionWrapper;
	this->checkNotEndOfLine("InterpreterCommon::treatFit","",p,pe);
	shared_ptr<Evaluator> ev;
	vector<vector<double> > values;
	vector<string> params;
	set<string> ev_params;
	tfel::math::vector<double> v_values;
	tfel::math::vector<double> p_values;
	vector<string> columns;
	vector<string> vars;
	string f = p->value;
	string function;
	string file;
	vector<string>::const_iterator ps;
	ExternalFunctionManager::const_iterator pf;
	ExternalFunctionManager::iterator pf2;
	vector<string>::size_type i;
	vector<string>::size_type j;
	vector<double>::size_type size;
	if(!this->isValidIdentifier(f)){
	  string msg("InterpreterCommon::treatFit : '");
	  msg += f+"' is not a valid function name.";
	  throw(runtime_error(msg));
	}
	++p;
	this->readVariableList(vars,p,pe);
	if(vars.empty()){
	  string msg("InterpreterCommon::treatFit : ");
	  msg += "no variable defined for function '"+f+"'";
	  throw(runtime_error(msg));
	}
	function=f+'(';
	for(ps=vars.begin();ps!=vars.end();){
	  function+=*ps;
	  if(++ps!=vars.end()){
	    function+=',';
	  }
	}
	function+=')';
	ev = shared_ptr<Evaluator>(new Evaluator(vars,function,this->functions));
	vars = ev->getVariablesNames();
	this->checkNotEndOfLine("InterpreterCommon::treatFit","",p,pe);
	file = this->readString(p,pe);
	columns.resize(vars.size()+1);
	if((p->value=="using")||
	   (p->value=="u")){
	  ++p;
	this->checkNotEndOfLine("InterpreterCommon::treatFit",
				"expected using declaration",p,pe);
	  for(i=0;i!=vars.size();++i){
	    this->readDataFunctionInUsingDeclaration(columns[i],p,pe);
	    this->readSpecifiedToken("InterpreterCommon::treatFit",":",p,pe);
	  }
	  this->readDataFunctionInUsingDeclaration(columns[i],p,pe);
	} else {
	  for(i=0;i!=vars.size()+1;++i){
	    ostringstream converter;
	    converter << i+1;
	    columns[i] = converter.str();
	  }
	}
	this->readSpecifiedToken("InterpreterCommon::treatFit","via",p,pe);
	this->readVariableList(params,p,pe,false);
	if(params.empty()){
	  string msg("InterpreterCommon::treatFit : ");
	  msg += "no parameter defined for function '"+f+"'";
	  throw(runtime_error(msg));
	}
	for(ps=vars.begin();ps!=vars.end();++ps){
	  if(find(params.begin(),params.end(),*ps)!=params.end()){
	    string msg("InterpreterCommon::treatFit : ");
	    msg += "'"+*ps+"' is both a variable and a parameter";
	    throw(runtime_error(msg));
	  }
	}
	// preparing the evaluator
	shared_ptr<ExternalFunction> nev = ev->createFunctionByChangingParametersIntoVariables(params);
	// reading data
	values.resize(vars.size()+1);
	TextData data(file);
	for(i=0;i!=vars.size()+1;++i){
	  this->getData(values[i],data,columns[i]);
	}
	size = values[0].size();
	for(i=1;i!=vars.size()+1;++i){
	  if(values[i].size()!=size){
	    string msg("InterpreterCommon::treatFit : ");
	    msg += "data columns does not have the same size";
	    throw(runtime_error(msg));
	  }
	}
	v_values.resize(vars.size());
	vector<string> v;
	LevenbergMarquardt<ExternalFunctionWrapper> levmar(ExternalFunctionWrapper(nev,vars.size(),params.size()));
	for(i=0;i!=size;++i){
	  for(j=0;j!=vars.size();++j){
	    v_values[j] = values[j][i];
	  }
	  levmar.addData(v_values,values[vars.size()][i]);
	}
	// clean-up
	values.clear();
	// initial guess
	p_values.resize(params.size());
	for(i=0,ps=params.begin();ps!=params.end();++ps,++i){
	  pf = this->functions->find(*ps);
	  if(pf==this->functions->end()){
	    string msg("InterpreterCommon::treatFit : ");
	    msg += "no initial value for parameter '"+*ps+"'";
	    throw(runtime_error(msg));
	  }
	  if(pf->second->getNumberOfVariables()!=0){
	    string msg("InterpreterCommon::treatFit : '");
	    msg += *ps+"' is not a parameter";
	    throw(runtime_error(msg));
	  }
	  p_values[i] = pf->second->getValue();
	}
	levmar.setInitialGuess(p_values);
	// execute
	p_values = levmar.execute();
	streamsize prec = cout.precision();
	cout.precision(15);
	cout << "levenberg-marquart converged in "
	     << levmar.getNumberOfIterations() << " iterations" << endl;
	cout << "Fit results : " << endl;
	for(i=0,ps=params.begin();ps!=params.end();++ps,++i){
	  pf2 = this->functions->find(*ps);
	  if(pf2==this->functions->end()){
	    string msg("InterpreterCommon::treatFit : ");
	    msg += "internal error (no parameter '"+*ps+"')";
	    throw(runtime_error(msg));
	  }
	  if(pf2->second->getNumberOfVariables()!=0){
	    string msg("InterpreterCommon::treatFit : '");
	    msg += *ps+"' is not a parameter";
	    throw(runtime_error(msg));
	  }
	  pf2->second = shared_ptr<ExternalFunction>(new Evaluator(p_values[i]));
	  cout << " - " << *ps << " = " << p_values[i] << endl;
	}
	cout.precision(prec);
      } // end of InterpreterCommon::treatFit

      InterpreterCommon::~InterpreterCommon()
      {} // end of InterpreterCommon::~InterpreterCommon()

    } // end of namespace interpreter

  } // end of  namespace math

} // end of namespace tfel

