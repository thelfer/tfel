/*!
 * \file   Configuration.cxx
 * \brief
 * \author Thomas Helfer
 * \date   13 sept. 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Check/Configuration.hxx"

namespace tfel::check {

  Configuration::Configuration() = default;

  Configuration::Configuration(Configuration&&) = default;

  Configuration::Configuration(const Configuration&) = default;

  Configuration& Configuration::operator=(Configuration&&) = default;

  Configuration& Configuration::operator=(const Configuration&) = default;

}  // end of namespace tfel::check
