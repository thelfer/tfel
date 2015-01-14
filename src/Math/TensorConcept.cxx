/*!
 * \file   src/Math/TensorConcept.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   09 Mar 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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

    TensorInvalidIndexException::~TensorInvalidIndexException() noexcept
    {} // end of TensorInvalidIndexException::~TensorInvalidIndexException

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

    TensorInvalidIndexesException::~TensorInvalidIndexesException() noexcept
    {} // end of TensorInvalidIndexesException::~TensorInvalidIndexesException() throw
    
  } // end of namespace math

} // end of namespace tfel
