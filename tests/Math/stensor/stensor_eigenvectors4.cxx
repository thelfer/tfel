/*!
 * \file  tests/Math/stensor/stensor_eigenvectors4.cxx
 * \brief This file checks the eigen decompositions of symmetric tensors
 * with close or equal eigenvalues
 * \author Tristan Chenaille
 * \date 27/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <random>
#include <cstdlib>
#include <iostream>
#include <algorithm>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"

struct StensorComputeEigenvectors4 final : public tfel::tests::TestCase {
  //! a simple alias
  using EigenSolver = tfel::math::stensor_common::EigenSolver;

  StensorComputeEigenvectors4()
      : tfel::tests::TestCase("TFEL/Math", "StensorComputeEigenvectors4") {
  }  // end of StensorComputeEigenvectors4

  tfel::tests::TestResult execute() override {
    // two close eigenvalues
    this->check<EigenSolver::HARARIEIGENSOLVER>({1, 1 + 1e-12, -0.5}, 1e-14);
    // a double eigenvalue
    for (const auto& l : {tfel::math::tvector<3u, double>{3, 3, 1},
                          tfel::math::tvector<3u, double>{-1, -1, 2},
                          tfel::math::tvector<3u, double>{1, 0, 0}}) {
      this->check<EigenSolver::FSESANALYTICALEIGENSOLVER>(l, 1e-13);
    }
    // a large isotropic part
    this->check<EigenSolver::FSESHYBRIDEIGENSOLVER>({1, 1.0002, 1.001}, 1e-13);
    return this->result;
  }  // end of execute

 private:
  /*!
   * \brief check the orthogonality of the eigenvectors and the
   * reconstruction of tensors built from the given eigenvalues in 1000
   * orientations
   * \param[in] l: eigenvalues
   * \param[in] eps: tolerance
   */
  template <EigenSolver es>
  void check(const tfel::math::tvector<3u, double>& l, const double eps) {
    using stensor = tfel::math::stensor<3u, double>;
    // portable pseudo-random sequence
    auto g = std::minstd_rand0{};
    auto rnd = [&g] { return 2 * static_cast<double>(g()) / g.max() - 1; };
    auto e = 0.;
    for (int t = 0; t != 1000; ++t) {
      // orientation given by the eigenvectors of a random tensor
      const auto r = stensor{rnd(), rnd(), rnd(), rnd(), rnd(), rnd()};
      const auto R = std::get<1>(
          r.computeEigenVectors<EigenSolver::GTESYMMETRICQREIGENSOLVER>());
      const auto s = stensor::buildFromEigenValuesAndVectors(l, R);
      const auto [vp, m] = s.template computeEigenVectors<es>();
      const auto s2 = stensor::buildFromEigenValuesAndVectors(vp, m);
      for (unsigned short i = 0; i != 6; ++i) {
        e = std::max(e, std::abs(s2[i] - s[i]));
      }
      for (unsigned short i = 0; i != 3; ++i) {
        for (unsigned short j = 0; j != 3; ++j) {
          const auto mij = m(0, i) * m(0, j) + m(1, i) * m(1, j) +  //
                           m(2, i) * m(2, j);
          e = std::max(e, std::abs(mij - (i == j ? 1 : 0)));
        }
      }
    }
    TFEL_TESTS_ASSERT(e < eps);
  }  // end of check
};

TFEL_TESTS_GENERATE_PROXY(StensorComputeEigenvectors4,
                          "StensorComputeEigenvectors4");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("StensorComputeEigenvectors4.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
