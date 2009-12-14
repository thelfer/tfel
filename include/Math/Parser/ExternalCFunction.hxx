/*!
 * \file   ExternalCFunction.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_MATH_PARSER_EXTERNALCFUNCTION_HXX_
#define _LIB_TFEL_MATH_PARSER_EXTERNALCFUNCTION_HXX_ 

#include<sstream>
#include<vector>
#include<stdexcept>

#include"Math/Parser/ExternalFunction.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {

      template<unsigned short N>
      struct ExternalCFunctionBase
	: public ExternalFunction
      {
	void
	setVariableValue(const std::vector<double>::size_type,
			 const double);
	std::vector<double>::size_type
	getNumberOfVariables(void) const;
	void
	checkCyclicDependency(const std::string&) const
	  throw(std::runtime_error);
	void
	checkCyclicDependency(const std::vector<std::string>&) const
	  throw(std::runtime_error);
	tfel::utilities::SmartPtr<ExternalFunction>
	differentiate(const std::vector<double>::size_type) const;
	tfel::utilities::SmartPtr<ExternalFunction>
	differentiate(const std::string&) const;
      protected:
	double variables[N];
      }; // end of struct ExternalCFunction

      template<unsigned short N>
      void
      ExternalCFunctionBase<N>::checkCyclicDependency(const std::string&) const
	throw(std::runtime_error)
      {} // end of ExternalCFunctionBase::checkCyclicDependency

      template<unsigned short N>
      void
      ExternalCFunctionBase<N>::checkCyclicDependency(const std::vector<std::string>&) const
	throw(std::runtime_error)
      {} // end of ExternalCFunctionBase::checkCyclicDependency

      template<unsigned short N>
      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunctionBase<N>::differentiate(const std::vector<double>::size_type) const
      {
	using namespace std;
	string msg("ExternalCFunctionBase<N>::differentiate : ");
	msg += "can't differentiate external function";
	throw(runtime_error(msg));
      } // end of ExternalCFunctionBase<N>::differentiate

      template<unsigned short N>
      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunctionBase<N>::differentiate(const std::string&) const
      {
	using namespace std;
	string msg("ExternalCFunctionBase<N>::differentiate : ");
	msg += "can't differentiate external function";
	throw(runtime_error(msg));
      } // end of ExternalCFunctionBase<N>::differentiate

      template<unsigned short N>
      void
      ExternalCFunctionBase<N>::setVariableValue(const std::vector<double>::size_type pos,
						 const double value)
      {
	using namespace std;
	if(pos>=N){
	  ostringstream msg;
	  msg << "ExternalCFunctionBase::setVariableValue : "
	      << "invalid index " << pos 
	      << " (function has only " << N << " variables).";
	  throw(runtime_error(msg.str()));
	}
	this->variables[pos] = value;
      } // end of ExternalCFunctionBase::setVariableValue;

      template<unsigned short N>
      std::vector<double>::size_type
      ExternalCFunctionBase<N>::getNumberOfVariables(void) const
      {
	return N;
      } // end of ExternalCFunctionBase<N>::getNumberOfVariables(void) const

      template<>
      struct ExternalCFunctionBase<0u>
	: public ExternalFunction
      {
	void
	setVariableValue(const std::vector<double>::size_type,
			 const double);
	std::vector<double>::size_type
	getNumberOfVariables(void) const;
	void
	checkCyclicDependency(const std::string&) const
	  throw(std::runtime_error);
	void
	checkCyclicDependency(const std::vector<std::string>&) const
	  throw(std::runtime_error);
	tfel::utilities::SmartPtr<ExternalFunction>
	differentiate(const std::vector<double>::size_type) const;
	tfel::utilities::SmartPtr<ExternalFunction>
	differentiate(const std::string&) const;
      }; // end of struct ExternalCFunction

      template<unsigned short N>
      struct ExternalCFunction
      {}; // end of struct ExternalCFunction

      template<>
      struct ExternalCFunction<0u>
	: public ExternalCFunctionBase<0u>
      {
	typedef double (* FunctionPtr)(void);
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::SmartPtr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      }; // end of struct ExternalCFunction

      template<>
      struct ExternalCFunction<1u>
	: public ExternalCFunctionBase<1u>
      {
	typedef double (*FunctionPtr)(double);
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::SmartPtr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      }; // end of struct ExternalCFunction

      template<>
      struct ExternalCFunction<2u>
	: public ExternalCFunctionBase<2u>
      {
	typedef double (*FunctionPtr)(double,double);
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::SmartPtr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      }; // end of struct ExternalCFunction

      template<>
      struct ExternalCFunction<3u>
	: public ExternalCFunctionBase<3u>
      {
	typedef double (*FunctionPtr)(double,double,double);
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::SmartPtr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      }; // end of struct ExternalCFunction

      template<>
      struct ExternalCFunction<4u>
	: public ExternalCFunctionBase<4u>
      {
	typedef double (*FunctionPtr)(double,double,
				      double,double);
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::SmartPtr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      }; // end of struct ExternalCFunction

      template<>
      struct ExternalCFunction<5u>
	: public ExternalCFunctionBase<5u>
      {
	typedef double (*FunctionPtr)(double,double,
				      double,double,
				      double);
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::SmartPtr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction

      template<>
      struct ExternalCFunction<6u>
	: public ExternalCFunctionBase<6u>
      {
	typedef double (*FunctionPtr)(double,double,
				      double,double,
				      double,double);
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::SmartPtr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction

      template<>
      struct ExternalCFunction<7u>
	: public ExternalCFunctionBase<7u>
      {
	typedef double (*FunctionPtr)(double,double,
				      double,double,
				      double,double,
				      double);
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::SmartPtr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction

      template<>
      struct ExternalCFunction<8u>
	: public ExternalCFunctionBase<8u>
      {
	typedef double (*FunctionPtr)(double,double,
				      double,double,
				      double,double,
				      double,double);
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::SmartPtr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction

      template<>
      struct ExternalCFunction<9u>
	: public ExternalCFunctionBase<9u>
      {
	typedef double (*FunctionPtr)(double,double,
				      double,double,
				      double,double,
				      double,double,
				      double);
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::SmartPtr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction

      template<>
      struct ExternalCFunction<10u>
	: public ExternalCFunctionBase<10u>
      {
	typedef double (*FunctionPtr)(double,double,
				      double,double,
				      double,double,
				      double,double,
				      double,double);
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::SmartPtr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction

      template<>
      struct ExternalCFunction<11u>
	: public ExternalCFunctionBase<11u>
      {
	typedef double (*FunctionPtr)(double,double,
				      double,double,
				      double,double,
				      double,double,
				      double,double,
				      double);
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::SmartPtr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction

      template<>
      struct ExternalCFunction<12u>
	: public ExternalCFunctionBase<12u>
      {
	typedef double (*FunctionPtr)(double,double,
				      double,double,
				      double,double,
				      double,double,
				      double,double,
				      double,double);
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::SmartPtr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction

      template<>
      struct ExternalCFunction<13u>
	: public ExternalCFunctionBase<13u>
      {
	typedef double (*FunctionPtr)(double,double,
				      double,double,
				      double,double,
				      double,double,
				      double,double,
				      double,double,
				      double);
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::SmartPtr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction

      template<>
      struct ExternalCFunction<14u>
	: public ExternalCFunctionBase<14u>
      {
	typedef double (*FunctionPtr)(double,double,
				      double,double,
				      double,double,
				      double,double,
				      double,double,
				      double,double,
				      double,double);
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::SmartPtr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction

      template<>
      struct ExternalCFunction<15u>
	: public ExternalCFunctionBase<15u>
      {
	typedef double (*FunctionPtr)(double,double,
				      double,double,
				      double,double,
				      double,double,
				      double,double,
				      double,double,
				      double,double,
				      double);
	ExternalCFunction(FunctionPtr);
	double getValue(void) const;
	tfel::utilities::SmartPtr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	FunctionPtr f;
      };// end of struct ExternalCFunction
 
    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_PARSER_EXTERNALCFUNCTION_HXX */
