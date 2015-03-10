/*! 
 * \file  ArgumentParserBase.cxx
 * \brief
 * \author Helfer Thomas
 * \date   04 mars 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Utilities/ArgumentParserBase.hxx"

namespace tfel
{

  namespace utilities
  {

    Argument::Argument(std::string s)
      : std::string(std::forward<std::string>(s))
    {}

    Argument::Argument(const char* const s)
      : std::string(s)
    {}
    
    bool Argument::hasOption(void) const noexcept
    {
      return this->isOptionSet;
    }

    void Argument::setOption(const std::string& o)
    {
      this->isOptionSet = true;
      this->option = o;
    }

    const std::string& Argument::getOption() const noexcept
    {
      return option;
    }

    Argument::~Argument() noexcept
    {}

  } // end of namespace utilities

} // end of namespace tfel

