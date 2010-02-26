#include<sstream>
#include<stdexcept>
#include"TFEL/Math/Parser/ExternalCFunction.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {

      void
      ExternalCFunctionException::throwUnimplementedDifferentiateFunctionException(void)
      {
	using namespace std;
	string msg("ExternalCFunctionException::");
	msg += "throwUnimplementedDifferentiateFunctionException : ";
	msg += "unimplemented feature";
	throw(runtime_error(msg));
      } // end of ExternalCFunctionException::throwUnimplementedDifferentiateFunctionException(void)

      void
      ExternalCFunctionException::throwInvalidCreateFunctionByChangingParametersIntoVariables(void)
      {
	using namespace std;
	string msg("ExternalCFunctionException::");
	msg += "throwInvalidCreateFunctionByChangingParametersIntoVariables : ";
	msg += "invalid call";
	throw(runtime_error(msg));
      } // end of ExternalCFunctionException::throwInvalidCreateFunctionByChangingParametersIntoVariables(void)

      void
      ExternalCFunctionException::throwInvalidVariableIndex(const unsigned short pos,
							    const unsigned short N)
      {
	using namespace std;
	ostringstream msg;
	msg << "ExternalCFunctionBase::setVariableValue : "
	    << "invalid index " << pos 
	    << " (function has only " << N << " variables).";
	throw(runtime_error(msg.str()));
      } // end of ExternalCFunctionException::throwInvalidVariableIndex

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunctionBase<0u>::createFunctionByChangingParametersIntoVariables(std::vector<std::string>& v,
										 const std::vector<double>&,
										 const std::vector<std::string>&,
										 const std::map<std::string,
										 std::vector<double>::size_type>&) const
      {
	v.clear();
	return this->resolveDependencies();
      } // end of ExternalCFunctionBase<0u>::createFunctionByChangingParametersIntoVariables

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunctionBase<0u>::createFunctionByChangingParametersIntoVariables(const std::vector<std::string>&) const
      {
	using namespace tfel::utilities;
	ExternalCFunctionException::throwInvalidCreateFunctionByChangingParametersIntoVariables();
	return SmartPtr<ExternalFunction>(0);
      } // end of ExternalCFunctionBase<0u>::createFunctionByChangingParametersIntoVariables

      void
      ExternalCFunctionBase<0u>::setVariableValue(const std::vector<double>::size_type pos,
						  const double)
      {
	using namespace std;
	ostringstream msg;
	msg << "ExternalCFunctionBase::setVariableValue : "
	    << "invalid index " << pos 
	    << " (function has no variable).";
	throw(runtime_error(msg.str()));
      } // end of ExternalCFunctionBase<0u>::setVariableValue

      void
      ExternalCFunctionBase<0u>::getParametersNames(std::set<std::string>&) const
      {} // endo of ExternalCFunction<0u>::getParametersNames

      std::vector<double>::size_type
      ExternalCFunctionBase<0u>::getNumberOfVariables(void) const
      {
	return 0u;
      } // end of ExternalCFunctionBase<0u>::etNumberOfVariables(void) const

      void
      ExternalCFunctionBase<0u>::checkCyclicDependency(const std::string&) const
      {} // end of ExternalCFunctionBase::checkCyclicDependency

      void
      ExternalCFunctionBase<0u>::checkCyclicDependency(std::vector<std::string>&) const
      {} // end of ExternalCFunctionBase::checkCyclicDependency

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunctionBase<0>::differentiate(const std::vector<double>::size_type) const
      {
	using namespace std;
	string msg("ExternalCFunctionBase<0>::differentiate : ");
	msg += "can't differentiate external function";
	throw(runtime_error(msg));
      } // end of ExternalCFunctionBase<0>::differentiate

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunctionBase<0>::differentiate(const std::string&) const
      {
	using namespace std;
	string msg("ExternalCFunctionBase<0>::differentiate : ");
	msg += "can't differentiate external function";
	throw(runtime_error(msg));
      } // end of ExternalCFunctionBase<0>::differentiate

      ExternalCFunction<0u>::ExternalCFunction(ExternalCFunction<0u>::FunctionPtr f_)
	: f(f_)
      {} // end of ExternalCFunction<0u>::ExternalCFunction
      
      double
      ExternalCFunction<0u>::getValue(void) const
      {
	return (*(this->f))();
      } // end of ExternalCFunction<0u>::getValue(void) const

      ExternalCFunction<1u>::ExternalCFunction(ExternalCFunction<1u>::FunctionPtr f_)
	: f(f_)
      {} // end of ExternalCFunction(FunctionPtr f_)

      double
      ExternalCFunction<1u>::getValue(void) const
      {
	return (*(this->f))(this->variables[0]);
      } // end of getValue(void) const

      ExternalCFunction<2u>::ExternalCFunction(ExternalCFunction<2u>::FunctionPtr f_)
	: f(f_)
      {} // end of ExternalCFunction(FunctionPtr f_)

      double
      ExternalCFunction<2u>::getValue(void) const
      {
	return (*(this->f))(this->variables[0],this->variables[1]);
      } // end of getValue(void) const

      ExternalCFunction<3u>::ExternalCFunction(ExternalCFunction<3u>::FunctionPtr f_)
	: f(f_)
      {} // end of ExternalCFunction(FunctionPtr f_)

      double
      ExternalCFunction<3u>::getValue(void) const
      {
	return (*(this->f))(this->variables[0],this->variables[1],
			    this->variables[2]);
      } // end of getValue(void) const

      ExternalCFunction<4u>::ExternalCFunction(ExternalCFunction<4u>::FunctionPtr f_)
	: f(f_)
      {} // end of ExternalCFunction<4u>::ExternalCFunction(FunctionPtr f_)

      double
      ExternalCFunction<4u>::getValue(void) const
      {
	return (*(this->f))(this->variables[0],this->variables[1],
			    this->variables[2],this->variables[3]);
      } // end of ExternalCFunction<4u>::getValue(void) const

      ExternalCFunction<5u>::ExternalCFunction(ExternalCFunction<5u>::FunctionPtr f_)
	: f(f_)
      {} // end of ExternalCFunction(FunctionPtr f_)
      
      double
      ExternalCFunction<5u>::getValue(void) const
      {
	return (*(this->f))(this->variables[0],this->variables[1],
			    this->variables[2],this->variables[3],
			    this->variables[4]);
      } // end of getValue(void) const
      
      ExternalCFunction<6u>::ExternalCFunction(ExternalCFunction<6u>::FunctionPtr f_)
	: f(f_)
      {} // end of ExternalCFunction(FunctionPtr f_)

      double
      ExternalCFunction<6u>::getValue(void) const
      {
	return (*(this->f))(this->variables[0],this->variables[1],
			    this->variables[2],this->variables[3],
			    this->variables[4],this->variables[5]);
      } // end of ExternalCFunction<6u>::getValue(void) const

      ExternalCFunction<7u>::ExternalCFunction(ExternalCFunction<7u>::FunctionPtr f_)
	: f(f_)
      {} // end of ExternalCFunction<7u>::ExternalCFunction(FunctionPtr f_)

      double
      ExternalCFunction<7u>::getValue(void) const
      {
	return (*(this->f))(this->variables[0],this->variables[1],
			    this->variables[2],this->variables[3],
			    this->variables[4],this->variables[5],
			    this->variables[6]);
      } // end of ExternalCFunction<7u>::getValue(void) const

      ExternalCFunction<8u>::ExternalCFunction(ExternalCFunction<8u>::FunctionPtr f_)
	: f(f_)
      {} // end of ExternalCFunction(FunctionPtr f_)

      double
      ExternalCFunction<8u>::getValue(void) const
      {
	return (*(this->f))(this->variables[0],this->variables[1],
			    this->variables[2],this->variables[3],
			    this->variables[4],this->variables[5],
			    this->variables[6],this->variables[7]);
      } // end of getValue(void) const

      ExternalCFunction<9u>::ExternalCFunction(ExternalCFunction<9u>::FunctionPtr f_)
	: f(f_)
      {} // end of ExternalCFunction(FunctionPtr f_)
      
      double
      ExternalCFunction<9u>::getValue(void) const
      {
	return (*(this->f))(this->variables[0],this->variables[1],
			    this->variables[2],this->variables[3],
			    this->variables[4],this->variables[5],
			    this->variables[6],this->variables[7],
			    this->variables[8]);
      } // end of ExternalCFunction<9u>::getValue(void) const

      ExternalCFunction<10u>::ExternalCFunction(ExternalCFunction<10u>::FunctionPtr f_)
	: f(f_)
      {} // end of ExternalCFunction(FunctionPtr f_)

      double
      ExternalCFunction<10u>::getValue(void) const
      {
	return (*(this->f))(this->variables[0],this->variables[1],
			    this->variables[2],this->variables[3],
			    this->variables[4],this->variables[5],
			    this->variables[6],this->variables[7],
			    this->variables[8],this->variables[9]);
      } // end of ExternalCFunction<10u>::getValue(void) const

      ExternalCFunction<11u>::ExternalCFunction(ExternalCFunction<11u>::FunctionPtr f_)
	: f(f_)
      {} // end of ExternalCFunction<11u>::ExternalCFunction(FunctionPtr f_)

      double
      ExternalCFunction<11u>::getValue(void) const
      {
	return (*(this->f))(this->variables[0],this->variables[1],
			    this->variables[2],this->variables[3],
			    this->variables[4],this->variables[5],
			    this->variables[6],this->variables[7],
			    this->variables[8],this->variables[9],
			    this->variables[10]);
      } // end of ExternalCFunction<11u>::getValue(void) const

      ExternalCFunction<12u>::ExternalCFunction(ExternalCFunction<12u>::FunctionPtr f_)
	: f(f_)
      {} // end of ExternalCFunction<12u>::ExternalCFunction(FunctionPtr f_)

      double
      ExternalCFunction<12u>::getValue(void) const
      {
	return (*(this->f))(this->variables[0],this->variables[1],
			    this->variables[2],this->variables[3],
			    this->variables[4],this->variables[5],
			    this->variables[6],this->variables[7],
			    this->variables[8],this->variables[9],
			    this->variables[10],this->variables[11]);
      } // end of ExternalCFunction<12u>::getValue(void) const

      ExternalCFunction<13u>::ExternalCFunction(ExternalCFunction<13u>::FunctionPtr f_)
	: f(f_)
      {} // end of ExternalCFunction(FunctionPtr f_)

      double
      ExternalCFunction<13u>::getValue(void) const
      {
	return (*(this->f))(this->variables[0],this->variables[1],
			    this->variables[2],this->variables[3],
			    this->variables[4],this->variables[5],
			    this->variables[6],this->variables[7],
			    this->variables[8],this->variables[9],
			    this->variables[10],this->variables[11],
			    this->variables[12]);
      } // end of ExternalCFunction<13u>::getValue(void) const

      ExternalCFunction<14u>::ExternalCFunction(ExternalCFunction<14u>::FunctionPtr f_)
	: f(f_)
      {} // end of
	 // ExternalCFunction<14u>::ExternalCFunction(FunctionPtr f_)
      
      double
      ExternalCFunction<14u>::getValue(void) const
      {
	return (*(this->f))(this->variables[0],this->variables[1],
			    this->variables[2],this->variables[3],
			    this->variables[4],this->variables[5],
			    this->variables[6],this->variables[7],
			    this->variables[8],this->variables[9],
			    this->variables[10],this->variables[11],
			    this->variables[12],this->variables[13]);
      } // end of ExternalCFunction<14u>::getValue(void) const

      ExternalCFunction<15u>::ExternalCFunction(ExternalCFunction<15u>::FunctionPtr f_)
	: f(f_)
      {} // end of ExternalCFunction<15u>::ExternalCFunction(FunctionPtr f_)

      double
      ExternalCFunction<15u>::getValue(void) const
      {
	return (*(this->f))(this->variables[0],this->variables[1],
			    this->variables[2],this->variables[3],
			    this->variables[4],this->variables[5],
			    this->variables[6],this->variables[7],
			    this->variables[8],this->variables[9],
			    this->variables[10],this->variables[11],
			    this->variables[12],this->variables[13],
			    this->variables[14]);
      } // end of getValue(void) const
      
      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunction<0u>::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<ExternalFunction>(new ExternalCFunction<0u>(this->f));
      } // end of ExternalCFunction<0u>::resolveDependencies(void) const

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunction<1u>::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<ExternalFunction>(new ExternalCFunction<1u>(this->f));
      } // end of ExternalCFunction<1u>::resolveDependencies(void) const

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunction<2u>::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<ExternalFunction>(new ExternalCFunction<2u>(this->f));
      } // end of ExternalCFunction<2u>::resolveDependencies(void) const

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunction<3u>::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<ExternalFunction>(new ExternalCFunction<3u>(this->f));
      } // end of ExternalCFunction<3u>::resolveDependencies(void) const

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunction<4u>::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<ExternalFunction>(new ExternalCFunction<4u>(this->f));
      } // end of ExternalCFunction<4u>::resolveDependencies(void) const

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunction<5u>::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<ExternalFunction>(new ExternalCFunction<5u>(this->f));
      } // end of ExternalCFunction<5u>::resolveDependencies(void) const

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunction<6u>::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<ExternalFunction>(new ExternalCFunction<6u>(this->f));
      } // end of ExternalCFunction<6u>::resolveDependencies(void) const

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunction<7u>::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<ExternalFunction>(new ExternalCFunction<7u>(this->f));
      } // end of ExternalCFunction<7u>::resolveDependencies(void) const

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunction<8u>::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<ExternalFunction>(new ExternalCFunction<8u>(this->f));
      } // end of ExternalCFunction<8u>::resolveDependencies(void) const

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunction<9u>::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<ExternalFunction>(new ExternalCFunction<9u>(this->f));
      } // end of ExternalCFunction<9u>::resolveDependencies(void) const

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunction<10u>::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<ExternalFunction>(new ExternalCFunction<10u>(this->f));
      } // end of ExternalCFunction<10u>::resolveDependencies(void) const

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunction<11u>::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<ExternalFunction>(new ExternalCFunction<11u>(this->f));
      } // end of ExternalCFunction<11u>::resolveDependencies(void) const

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunction<12u>::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<ExternalFunction>(new ExternalCFunction<12u>(this->f));
      } // end of ExternalCFunction<12u>::resolveDependencies(void) const

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunction<13u>::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<ExternalFunction>(new ExternalCFunction<13u>(this->f));
      } // end of ExternalCFunction<13u>::resolveDependencies(void) const

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunction<14u>::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<ExternalFunction>(new ExternalCFunction<14u>(this->f));
      } // end of ExternalCFunction<14u>::resolveDependencies(void) const

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCFunction<15u>::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<ExternalFunction>(new ExternalCFunction<15u>(this->f));
      } // end of ExternalCFunction<15u>::resolveDependencies(void) const

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
