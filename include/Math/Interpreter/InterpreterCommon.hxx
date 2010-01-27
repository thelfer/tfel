/*!
 * \file   InterpreterCommon.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   08 mar 2008
 */

#ifndef _LIB_TFEL_MATH_INTERPRETER_INTERPRETERCOMMON_HXX_
#define _LIB_TFEL_MATH_INTERPRETER_INTERPRETERCOMMON_HXX_ 

#include<set>
#include<map>
#include<string>

#include"Utilities/SmartPtr.hxx"
#include"Utilities/CxxTokenizer.hxx"
#include"Math/Evaluator.hxx"

namespace tfel
{  

  namespace math
  {

    namespace interpreter
    {

      struct InterpreterCommon
	: protected tfel::utilities::CxxTokenizer
      {

	InterpreterCommon();

	const std::string&
	getCurrentLine(void) const;

	void
	clearCurrentLine(void);

	void
	analyseFile(const std::string&);

	void
	analyseLine(const std::string&,
		    const unsigned short);

	~InterpreterCommon();

      protected:

	struct ImportOptions;

	typedef void (InterpreterCommon::* MemFuncPtr)(TokensContainer::const_iterator&,
						  const TokensContainer::const_iterator);
      
	std::map<std::string,MemFuncPtr>  callBacks;
	std::map<std::string,MemFuncPtr>  setCallBacks;
	std::map<std::string,MemFuncPtr>  unsetCallBacks;

	tfel::utilities::SmartPtr<tfel::math::parser::ExternalFunctionManager> functions;

	std::set<std::string> locks;
	std::string currentLine;
	unsigned short currentLineNbr;
      
	static bool
	isUnsignedInteger(const std::string&);

	static double
	readDouble(const std::string&,
		   const unsigned short);

	bool
	isValidIdentifier(const std::string&);

	std::string
	stripComments(const std::string&);

	std::vector<std::string>
	tokenize(const std::string&,
		 const char);

	void
	analyse(TokensContainer::const_iterator,
		const TokensContainer::const_iterator);

	void
	treatUnknownArgument(void);

	void setRanges(void);

	tfel::utilities::SmartPtr<tfel::math::Evaluator>
	readFunction(TokensContainer::const_iterator&, 
		     const TokensContainer::const_iterator);

#ifdef HAVE_GSL
	void
	treatSetGSL(TokensContainer::const_iterator&, 
		    const TokensContainer::const_iterator);
#endif

	void
	treatSet(TokensContainer::const_iterator&, 
		 const TokensContainer::const_iterator);
	
	void
	treatUnset(TokensContainer::const_iterator&, 
		   const TokensContainer::const_iterator);
	
	void
	analyseFunctionDefinition(TokensContainer::const_iterator&,
				  const TokensContainer::const_iterator,
				  const bool,
				  const bool);

	void
	registerCallBack(std::map<std::string,MemFuncPtr>&,
			 const std::string&,
			 const InterpreterCommon::MemFuncPtr);

	void
	registerCallBacks(void);

	unsigned short
	convertToUnsignedShort(const std::string&);

	void
	addFunction(const std::string&,
		    tfel::utilities::SmartPtr<tfel::math::parser::ExternalFunction>,
		    const bool,
		    const bool);

	void
	importCFunction(const std::string&,
			const std::string&,
			const unsigned short,
			const bool);

	void
	importCastemFunction(const std::string&,
			     const std::string&,
			     const unsigned short,
			     const bool);

#ifdef HAVE_OCTAVE
	void
	importOctaveFunction(TokensContainer::const_iterator&, 
			     const TokensContainer::const_iterator,
			     const std::string&,
			     const unsigned short nbr,
			     const bool);
#endif /* HAVE_OCTAVE*/

	void
	importFunction(TokensContainer::const_iterator&, 
		       const TokensContainer::const_iterator,
		       const ImportOptions,
		       const bool);

	std::string
	readNextGroup(TokensContainer::const_iterator&, 
		      const TokensContainer::const_iterator);

	std::string
	readUntil(TokensContainer::const_iterator&,
		  const TokensContainer::const_iterator,
		  const std::string&);

	std::vector<std::string>
	readVariableList(TokensContainer::const_iterator&, 
			 const TokensContainer::const_iterator);

	void
	treatQuit(TokensContainer::const_iterator&, 
		  const TokensContainer::const_iterator);

	void
	treatConst(TokensContainer::const_iterator&, 
		   const TokensContainer::const_iterator);

	void
	treatLock(TokensContainer::const_iterator&, 
		  const TokensContainer::const_iterator);

	void
	treatNoDeps(TokensContainer::const_iterator&, 
		    const TokensContainer::const_iterator);

	void
	treatInclude(TokensContainer::const_iterator&, 
		     const TokensContainer::const_iterator);

	void
	treatImport(TokensContainer::const_iterator&, 
		    const TokensContainer::const_iterator);

	void
	treatImport(TokensContainer::const_iterator&, 
		    const TokensContainer::const_iterator,
		    const bool);

	const std::string
	gatherTokenGroup(TokensContainer::const_iterator&, 
			 const TokensContainer::const_iterator);

      }; // end of struct InterpreterCommon

    } // end of namespace interpreter

  } // end of  namespace graphics

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_INTERPRETER_INTERPRETERCOMMON_HXX */

