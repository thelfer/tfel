/*!
 * \file   FiniteStrainHeatTransferTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   30/01/2026
 */

#include <cstdlib>
#include "TFEL/Material/FiniteStrainHeatTransfer.hxx"

int main() {
  const auto k = double{3};
  const auto id = tfel::math::tensor<3u, double>::Id();
  const auto K =
      tfel::material::computeThermalConductivyMatrixInReferenceConfiguration(
          k, id);
  return EXIT_SUCCESS;
}
