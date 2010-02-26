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

#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Utilities/TextData.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"TFEL/Math/Evaluator.hxx"

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

	~InterpreterCommon();

      protected:

	struct ImportOptions;

	tfel::utilities::SmartPtr<tfel::math::parser::ExternalFunctionManager> functions;

	std::set<std::string> locks;
      
	static bool
	isUnsignedInteger(const std::string&);

	static double
	readDouble(const std::string&,
		   const unsigned short);

	bool
	isValidIdentifier(const std::string&);

	std::vector<std::string>
	tokenize(const std::string&,
		 const char);

	void
	treatImport(TokensContainer::const_iterator&, 
		    const TokensContainer::const_iterator);

	void
	treatImport(TokensContainer::const_iterator&, 
		    const TokensContainer::const_iterator,
		    const bool);

	tfel::utilities::SmartPtr<tfel::math::Evaluator>
	readFunction(TokensContainer::const_iterator&, 
		     const TokensContainer::const_iterator);

#ifdef HAVE_GSL
	void
	treatSetGSL(TokensContainer::const_iterator&, 
		    const TokensContainer::const_iterator);
#endif
	
	void
	analyseFunctionDefinition(TokensContainer::const_iterator&,
				  const TokensContainer::const_iterator,
				  const bool,
				  const bool);

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
	getData(std::vector<double>&,
		const tfel::utilities::TextData&,
		TokensContainer::const_iterator&, 
		const TokensContainer::const_iterator);

	std::string
	getData(std::vector<double>&,
		const tfel::utilities::TextData&,
		const std::string&);

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

	void
	readDataFunctionInUsingDeclaration(std::string&,
					   TokensContainer::const_iterator&,
					   const TokensContainer::const_iterator);

	void
	readVariableList(std::vector<std::string>&,
			 TokensContainer::const_iterator&, 
			 const TokensContainer::const_iterator,
			 const bool = true);

	std::vector<std::string>
	readVariableList(TokensContainer::const_iterator&, 
			 const TokensContainer::const_iterator,
			 const bool = true);
	
	void
	treatKriging(TokensContainer::const_iterator&, 
		     const TokensContainer::const_iterator);

	void
	treatFit(TokensContainer::const_iterator&, 
		 const TokensContainer::const_iterator);

	void
	treatPrint(TokensContainer::const_iterator&, 
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

	const std::string
	gatherTokenGroup(TokensContainer::const_iterator&, 
			 const TokensContainer::const_iterator);

      }; // end of struct InterpreterCommon

    } // end of namespace interpreter

  } // end of  namespace graphics

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_INTERPRETER_INTERPRETERCOMMON_HXX */

