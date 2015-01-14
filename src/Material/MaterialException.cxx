/*!
 * \file   src/Material/MaterialException.cxx
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

#include<utility>
#include"TFEL/Material/MaterialException.hxx"

namespace tfel{
  
  namespace material {

    MaterialException::~MaterialException() noexcept
    {} // end of MaterialException::~MaterialException

    const char*
    DivergenceException::what() const noexcept
    {
      return "DivergenceException";
    } // end of DivergenceException::~DivergenceException
    
    DivergenceException::~DivergenceException() noexcept
    {} // end of DivergenceException::~DivergenceException

    OutOfBoundsException::OutOfBoundsException(std::string m)
      : msg(std::forward<std::string>(m))
    {}

    const char*
    OutOfBoundsException::what() const noexcept
    {
      return this->msg.c_str();
    } // end of DivergenceException::~DivergenceException

    OutOfBoundsException::~OutOfBoundsException() noexcept
    {} // end of OutOfBoundsException::~OutOfBoundsException

  } // end of namespace material

} // end of namespace tfel

