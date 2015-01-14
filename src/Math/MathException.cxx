/*!
 * \file   src/Math/MathException.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   14 fév 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Math/MathException.hxx"

namespace tfel{

  namespace math{

    MathException::~MathException() noexcept
    {}

    MathRunTimeException::~MathRunTimeException() noexcept
    {}

    const char*
    InvalidTimeStepException::what() const noexcept
    {
      return "invalid time step";
    } // end of InvalidTimeStepException::what

    InvalidTimeStepException::~InvalidTimeStepException() noexcept
    {} // end of InvalidTimeStepException::InvalidTimeStepException

    MathDivergenceException::~MathDivergenceException() noexcept
    {}

    const char*
    MaximumNumberOfIterationsReachedException::what() const noexcept
    {
      return "maximum number of iterations reached";
    } // end of MaximumNumberOfIterationsReachedException::what

    MaximumNumberOfIterationsReachedException::~MaximumNumberOfIterationsReachedException() noexcept
    {} // end of MaximumNumberOfIterationsReachedException::MaximumNumberOfIterationsReachedException

    const char*
    SingularJacobianException::what() const noexcept
    {
      return "jacobian is not invertible";
    } // end of SingularJacobianException::what

    SingularJacobianException::~SingularJacobianException() noexcept
    {} // end of SingularJacobianException::SingularJacobianException

  } // end of namespace math

} // end of namespace tfel

