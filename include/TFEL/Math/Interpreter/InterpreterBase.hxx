/*! 
 * \file  InterpreterBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 28 jan 2010
 */

#ifndef _LIB_TFEL_MATH_INTERPRETER_INTERPRETERBASE_HXX_
#define _LIB_TFEL_MATH_INTERPRETER_INTERPRETERBASE_HXX_ 

#include<set>
#include<map>
#include<cmath>
#include<string>
#include<sstream>

#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"TFEL/Math/Evaluator.hxx"

#include"TFEL/Math/Interpreter/InterpreterCommon.hxx"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif /* M_PI */

namespace tfel
{  

  namespace math
  {

    namespace interpreter
    {

      template<typename Child>
      struct InterpreterBase
	: public InterpreterCommon
      {
	
	InterpreterBase();
	
	const std::string&
	getCurrentLine(void) const;

	void
	clearCurrentLine(void);

	void
	analyseFile(const std::string&);

	void
	analyseLine(const std::string&,
		    const unsigned short);

      protected:

	typedef void (Child::* MemFuncPtr)(TokensContainer::const_iterator&,
					   const TokensContainer::const_iterator);

	void
	treatLine();
      
	void
	registerCallBack(std::map<std::string,MemFuncPtr>&,
			 const std::string&,
			 const MemFuncPtr);

	void
	registerCallBacks(void);
	
	void
	treatSet(TokensContainer::const_iterator&, 
		 const TokensContainer::const_iterator);

	void
	treatInclude(TokensContainer::const_iterator&, 
		     const TokensContainer::const_iterator);

	void
	treatUnset(TokensContainer::const_iterator&, 
		   const TokensContainer::const_iterator);

	void
	analyse(TokensContainer::const_iterator,
		const TokensContainer::const_iterator);

	std::map<std::string,MemFuncPtr>  callBacks;
	std::map<std::string,MemFuncPtr>  setCallBacks;
	std::map<std::string,MemFuncPtr>  unsetCallBacks;

	std::string currentLine;
	unsigned short currentLineNbr;

      }; // end of struct InterpreterBase

    } // end of namespace interpreter

  } // end of  namespace math

} // end of namespace tfel

#include"TFEL/Math/Interpreter/InterpreterBase.ixx"

#endif /* _LIB_TFEL_MATH_INTERPRETER_INTERPRETERBASE_HXX */
