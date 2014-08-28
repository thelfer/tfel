/*!
 * \file   src/Math/TensorConcept.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   09 Mar 2007
 */

#include<string>
#include<sstream>
#include"TFEL/Math/Tensor/TensorConcept.hxx"

namespace tfel{

  namespace math{

    std::string
    TensorInvalidIndexException::getErrorMessage(const unsigned short i)
    {
      using namespace std;
      ostringstream msg;
      msg << "TensorInvalidIndexException::TensorInvalidIndexException : "
	  << "invalid index (" << i << ")";
      return msg.str();
    }

    TensorInvalidIndexException::TensorInvalidIndexException(const unsigned short i)
      : std::runtime_error(TensorInvalidIndexException::getErrorMessage(i))
    {} // end of TensorInvalidIndexException::TensorInvalidIndexException

    std::string
    TensorInvalidIndexesException::getErrorMessage(const unsigned short i,
						   const unsigned short j)
    {
      using namespace std;
      ostringstream msg;
      msg << "TensorInvalidIndexesException::TensorInvalidIndexesException : "
	  << "invalid indexes (" << i << "," << j << ")";
      return msg.str();
    }

    TensorInvalidIndexesException::TensorInvalidIndexesException(const unsigned short i,
								 const unsigned short j)
      : std::runtime_error(TensorInvalidIndexesException::getErrorMessage(i,j))
    {} // end of TensorInvalidIndexesException::TensorInvalidIndexesException
    
  } // end of namespace math

} // end of namespace tfel
