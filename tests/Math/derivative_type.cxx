/*!
 * \file   tests/Math/derivative_type.cxx
 * \brief
 * \author Thomas Helfer
 * \date   25/06/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <iostream>
#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/General/DerivativeType.hxx"

int main() {
  using namespace tfel::math;
  using Types = tfel::config::Types<3u>;
  static_assert(std::is_same<derivative_type<Types::strain, Types::time>,
                             Types::strainrate>::value,
                "invalid derivative type");
  static_assert(std::is_same<derivative_type<Types::StressStensor, Types::time>,
                             Types::StressRateStensor>::value,
                "invalid derivative type");
  return EXIT_SUCCESS;
}
