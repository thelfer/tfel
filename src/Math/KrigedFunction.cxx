/*! 
 * \file  KrigedFunction.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 02 fév 2010
 */

#include<sstream>
#include<stdexcept>

#include"TFEL/Math/Parser/KrigedFunction.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {
      
      void
      KrigedFunctionBase::throwUnimplementedDifferentiateFunctionException(void)
      {
	using namespace std;
	string msg("KrigedFunctionBase::");
	msg += "throwUnimplementedDifferentiateFunctionException : ";
	msg += "unimplemented feature";
	throw(runtime_error(msg));
      } // end of KrigedFunctionBase::throwUnimplementedDifferentiateFunctionException

      void
      KrigedFunctionBase::throwInvalidIndexException(const unsigned short i,
						     const unsigned short N)
      {
	using namespace std;
	ostringstream msg;
	msg << "KrigedFunctionBase::throwInvalidIndexException : invalid index ("
	    << i << " >  " << N << ")";
	throw(runtime_error(msg.str()));
      } // end of KrigedFunctionBase::throwInvalidIndexException
      
      template<>
      void
      KrigedFunction<1u>::setVariableValue(const std::vector<double>::size_type i,
					   const double v_)
      {
	if(i>0){
	  KrigedFunctionBase::throwInvalidIndexException(i,0);
	}
	this->v = v_;
      } // end of KrigedFunction<1u>::setVariableValue

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
