/*!
 * \file   BinaryFunction.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#include<cerrno>
#include<stdexcept>

#include"TFEL/Math/Parser/BinaryFunction.hxx"

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      void
      StandardBinaryFunctionBase::throwUnimplementedDifferentiateFunctionException(void)
      {
	using namespace std;
	string msg("StandardBinaryFunctionBase::");
	msg += "throwUnimplementedDifferentiateFunctionException : ";
	msg += "unimplemented feature";
	throw(runtime_error(msg));
      } // end of StandardBinaryFunctionBase::throwUnimplementedDifferentiateFunctionException()

      void
      StandardBinaryFunctionBase::throwInvalidCallException(const int e)
      {

	using namespace std;
	string msg("StandardBinaryFunctionBase::throwInvalidCallException : ");
	msg += "call to function failed (";
	msg += strerror(e);
	msg += ")";
	throw(runtime_error(msg));
      } // end of struct StandardBinaryFunctionBase::throwInvalidCallException()

      BinaryFunction::~BinaryFunction()
      {} // end of BinaryFunction::~BinaryFunction

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
