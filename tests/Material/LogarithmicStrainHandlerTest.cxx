/*!
 * \file   LogarithmicStrainHandlerTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   25 juin 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cstdlib>
#include <utility>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/Stensor/StensorConceptIO.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2ConceptIO.hxx"
#include "TFEL/Material/LogarithmicStrainHandler.hxx"

struct LogarithmicStrainHandlerTest final : public tfel::tests::TestCase {
  LogarithmicStrainHandlerTest()
      : tfel::tests::TestCase("TFEL/Material", "LogarithmicStrainHandlerTest") {
  }  // end of LogarithmicStrainHandlerTest
  tfel::tests::TestResult execute() override {
    this->check1<1u, double>();
    this->check1<2u, double>();
    this->check1<3u, double>();
    this->check2<1u, double>();
    this->check2<2u, double>();
    this->check2<3u, double>();
    return this->result;
  }

 private:
  template <unsigned short N, typename real>
  void check1() {
    using namespace tfel::math;
    using namespace tfel::material;
    using LSHandler = LogarithmicStrainHandler<N, real>;
    using stensor = tfel::math::stensor<N, real>;
    using tensor = tfel::math::tensor<N, real>;
    using size_type = unsigned short;
    const real l0 = 1.09465e+11;
    const real m0 = 5.6391e+12;
    const real eps = 1.e-10 * (2 * m0);
    // Hencky-Biot law with PK2 result
    auto hb = [&l0, &m0](const LSHandler& h) -> stensor {
      const auto e = h.getHenckyLogarithmicStrain();
      return l0 * trace(e) * stensor::Id() + 2 * m0 * e;
    };
    const real v1[9u] = {1.03, 0.98,   1.09,   0.03, -0.012,
                         0.04, -0.028, -0.015, 0.005};
    const real v2[9u] = {0.70, 1.125, 1.32,  -0.24, -0.32,
                         0.15, -0.14, -0.05, 0.08};
    const real v3[9u] = {1.70, 0.625, 0.625, 0., 0., 0., 0., 0., 0.};
    const real v4[9u] = {0.625, 1.70, 0.625, 0., 0., 0., 0., 0., 0.};
    const real v5[9u] = {0.625, 0.625, 1.70, 0., 0., 0., 0., 0., 0.};
    const real v6[9u] = {0.625, 0.625, 1.70, 0., 0., 0., 0., 0., 0.};
    for (const tensor& F : {tensor::Id(), tensor{v1}, tensor{v2}, tensor{v3},
                            tensor{v4}, tensor{v5}, tensor{v6}}) {
      const LSHandler h1(LSHandler::LAGRANGIAN, F);
      const LSHandler h2(LSHandler::EULERIAN, F);
      const auto T = hb(h1);
      const auto S = h1.convertToSecondPiolaKirchhoffStress(T);
      const auto s1 = convertSecondPiolaKirchhoffStressToCauchyStress(S, F);
      const auto s2 = h2.convertToCauchyStress(T);
      for (size_type i = 0; i != tfel::math::StensorDimeToSize<N>::value; ++i) {
        TFEL_TESTS_ASSERT(std::abs(s1(i) - s2(i)) < eps);
      }
    }
  }
  template <unsigned short N, typename real>
  void check2() {
    using namespace tfel::math;
    using namespace tfel::material;
    using LSHandler = LogarithmicStrainHandler<N, real>;
    using stensor = tfel::math::stensor<N, real>;
    using tensor = tfel::math::tensor<N, real>;
    using st2tost2 = tfel::math::st2tost2<N, real>;
    using size_type = unsigned short;
    const real l0 = 1.09465e+11;
    const real m0 = 5.6391e+12;
    const real eps = 1.e-10 * (2 * m0);
    // Hencky-Biot law with PK2 result
    auto hb = [&l0, &m0](const LSHandler& h) -> stensor {
      const auto e = h.getHenckyLogarithmicStrain();
      return l0 * trace(e) * stensor::Id() + 2 * m0 * e;
    };
    const auto D = eval(l0 * st2tost2::IxI() + 2 * m0 * st2tost2::Id());
    const real v1[9u] = {1.03, 0.98,   1.09,   0.03, -0.012,
                         0.04, -0.028, -0.015, 0.005};
    const real v2[9u] = {0.70, 1.125, 1.32,  -0.24, -0.32,
                         0.15, -0.14, -0.05, 0.08};
    const real v3[9u] = {1.70, 0.625, 0.625, 0., 0., 0., 0., 0., 0.};
    const real v4[9u] = {0.625, 1.70, 0.625, 0., 0., 0., 0., 0., 0.};
    const real v5[9u] = {0.625, 0.625, 1.70, 0., 0., 0., 0., 0., 0.};
    const real v6[9u] = {0.625, 0.625, 1.70, 0., 0., 0., 0., 0., 0.};
    for (const tensor& F : {tensor::Id(), tensor{v1}, tensor{v2}, tensor{v3},
                            tensor{v4}, tensor{v5}, tensor{v6}}) {
      const LSHandler h1(LSHandler::LAGRANGIAN, F);
      const LSHandler h2(LSHandler::EULERIAN, F);
      const auto T = hb(h1);
      const auto D1 = h1.convertToSpatialTangentModuli(D, T);
      const auto D2 = h2.convertToSpatialTangentModuli(D, T);
      std::cout << "D1: " << D1 << std::endl;
      std::cout << "D2: " << D2 << std::endl;
      for (size_type i = 0; i != tfel::math::StensorDimeToSize<N>::value; ++i) {
        for (size_type j = 0; j != tfel::math::StensorDimeToSize<N>::value;
             ++j) {
          const auto e = std::abs(D1(i, j) - D2(i, j));
          if (e > eps) {
            std::cout << i << " " << j << " " << D1(i, j) << " " << D2(i, j)
                      << " " << e << std::endl;
          }
          TFEL_TESTS_ASSERT(std::abs(D1(i, j) - D2(i, j)) < eps);
        }
      }
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(LogarithmicStrainHandlerTest,
                          "LogarithmicStrainHandlerTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("LogarithmicStrainHandlerTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
