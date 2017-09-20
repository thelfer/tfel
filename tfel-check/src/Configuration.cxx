/*!
 * \file   Configuration.cxx
 * \brief    
 * \author HELFER Thomas 202608
 * \date   13 sept. 2017
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Check/Configuration.hxx"

namespace tfel{

  namespace check{

    Configuration::Configuration() = default;

    Configuration::Configuration(Configuration&&) = default;

    Configuration::Configuration(const Configuration&) = default;

    Configuration&
    Configuration::operator=(Configuration&&) = default;

    Configuration&
    Configuration::operator=(const Configuration&) = default;
    
  } // end of namespace check

} // end of namespace tfel
