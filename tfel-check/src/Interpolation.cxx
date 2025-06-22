/*
 * Interpolation.cpp
 *
 *  Created on: 7 mai 2013
 *      Author: rp238441
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFELCheck/Interpolation.hxx"

namespace tfel_check {

  Interpolation::Interpolation() = default;
  Interpolation::Interpolation(Interpolation&&) = default;
  Interpolation::Interpolation(const Interpolation&) = default;
  Interpolation& Interpolation::operator=(Interpolation&&) = default;
  Interpolation& Interpolation::operator=(const Interpolation&) = default;
  Interpolation::~Interpolation() = default;

} /* namespace tfel_check */
