/*!
 * \file   OrthotropicPlasticityTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   14/01/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <iostream>
#include <cstdlib>
#include <cmath>

#include <cassert>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Material/OrthotropicPlasticity.hxx"

template <unsigned short N, typename real>
static tfel::math::stensor<N, real> getNumericalApproximation(
    const tfel::math::stensor<N, real>& sig,
    const tfel::math::tvector<11, real>& c,
    const real e) {
  tfel::math::stensor<N, real> r;
  for (unsigned short j = 0; j != sig.size(); ++j) {
    auto sigp = sig;
    auto sigm = sig;
    sigp(j) += e;
    sigm(j) -= e;
    const auto Jp = tfel::material::computeJ3O(sigp, c);
    const auto Jm = tfel::material::computeJ3O(sigm, c);
    const auto dJ = (Jp - Jm) / (2 * e);
    r(j) = dJ;
  }
  return r;
}  // end of getNumericalApproximation

struct OrthotropicPlasticityTest final : public tfel::tests::TestCase {
  OrthotropicPlasticityTest()
      : tfel::tests::TestCase("TFEL/Material", "OrthotropicPlasticityTest") {
  }  // end of OrthotropicPlasticityTest
  tfel::tests::TestResult execute() override {
    const double sqrt2 = std::sqrt(2.);
    const double v1[6] = {8.2e-2, -4.5e-2, 7.2e-2, 2.3e-2 * sqrt2, 0, 0.};
    const double v2[6] = {-8.2e-2, 4.5e-2, 7.2e-2, 0e-2, 2.3e-2 * sqrt2, 0.};
    const double v3[6] = {8.2e-2, 4.5e-2, -7.2e-2, 0e-2, 0.e-2, 2.3e-2 * sqrt2};
    const double v4[6] = {8.2e-2,          4.5e-2,           -7.2e-2,
                          3.14e-2 * sqrt2, -6.42e-2 * sqrt2, 2.3e-2 * sqrt2};
    for (const auto v : {v1, v2, v3, v4}) {
      this->check(tfel::math::stensor<1u, double>(v));
      this->check(tfel::math::stensor<2u, double>(v));
      this->check(tfel::math::stensor<3u, double>(v));
    }
    return this->result;
  }  // end of execute
 private:
  template <unsigned short N>
  void check(const tfel::math::stensor<N, double>& s) {
    this->test0(s);
    this->test1(s);
    this->test2(s);
    // this->test3(s);
  }
  template <unsigned short N>
  void test0(const tfel::math::stensor<N, double>& s) {
    constexpr auto prec = 1e-14;
    constexpr auto one = double(1);
    const auto J3 = det(deviator(s));
    const auto J3O = tfel::material::computeJ3O(s, one, one, one, one, one, one,
                                                one, one, one, one, one);
    TFEL_TESTS_ASSERT(std::abs(J3 - J3O) < prec);
  }
  template <unsigned short N>
  void test1(const tfel::math::stensor<N, double>& s) {
    constexpr auto eps = 1e-6;
    constexpr auto prec = 2e-13;
    constexpr auto one = double(1);
    const auto c = tfel::math::tvector<11, double>{one, one, one, one, one, one,
                                                   one, one, one, one, one};
    const auto ndJ = getNumericalApproximation(s, c, eps);
    const auto dJ = tfel::material::computeJ3ODerivative(s, c);
    for (unsigned short i = 0; i != s.size(); ++i) {
      if (std::abs(dJ(i) - ndJ(i)) > prec) {
        std::cout << "dJ : " << i << " " << dJ(i) << " " << ndJ(i) << " "
                  << std::abs(dJ(i) - ndJ(i)) << std::endl;
      }
      TFEL_TESTS_ASSERT(std::abs(dJ(i) - ndJ(i)) < prec);
    }
  }
  template <unsigned short N>
  void test2(const tfel::math::stensor<N, double>& s) {
    using namespace tfel::math;
    using namespace tfel::material;
    using size_type = typename stensor<N, double>::size_type;
    constexpr auto one = double(1);
    const auto c = tfel::math::tvector<11, double>{one, one, one, one, one, one,
                                                   one, one, one, one, one};
    const double eps = 1.e-5;
    const double prec = 1.e-10;
    const auto d2J3O = computeJ3OSecondDerivative(s, c);
    st2tost2<N, double> nd2J3O;
    for (size_type i = 0; i != s.size(); ++i) {
      stensor<N> s_e(s);
      s_e[i] += eps;
      const stensor<N> dJ3Op = computeJ3ODerivative(s_e, c);
      s_e[i] -= 2 * eps;
      const stensor<N> dJ3Om = computeJ3ODerivative(s_e, c);
      for (size_type j = 0; j != s.size(); ++j) {
        nd2J3O(j, i) = (dJ3Op(j) - dJ3Om(j)) / (2 * eps);
      }
    }
    for (size_type i = 0; i != s.size(); ++i) {
      for (size_type j = 0; j != s.size(); ++j) {
        TFEL_TESTS_ASSERT(std::abs(nd2J3O(i, j) - d2J3O(i, j)) < prec);
        if (std::abs(nd2J3O(i, j) - d2J3O(i, j)) > prec) {
          std::cout << "Error " << N << " (" << i << ", " << j << ") "
                    << "[" << i * StensorDimeToSize<N>::value + j
                    << "]: " << nd2J3O(i, j) << " vs " << d2J3O(i, j) << " "
                    << std::abs(nd2J3O(i, j) - d2J3O(i, j)) << std::endl;
        }
      }
    }
  }
  // template<unsigned short N>
  // void test3(const tfel::math::stensor<N,double> s){
  //   using namespace tfel::math;
  //   using Stensor  = stensor<N,double>;
  //   using Stensor4 = st2tost2<N,double>;
  //   using size_type = typename stensor<N,double>::size_type;
  //   const auto id = Stensor::Id();
  //   const double prec = 1.e-10;
  //   const auto i1 = trace(s);
  //   const auto di2  = i1*id-s;
  //   const auto d2i2 = (id^id)-Stensor4::Id();
  //   const auto d2i3 = computeDeterminantSecondDerivative(s);
  //   const auto dJ3  = tfel::material::computeJ3SecondDerivative(s);
  //   const st2tost2<N,double> dJ3_2 =
  //     (4*i1/9)*(id^id)-((id^di2)+(di2^id)+i1*d2i2)/3+d2i3;
  //   for(size_type i=0;i!=s.size();++i){
  //     for(size_type j=0;j!=s.size();++j){
  //   	TFEL_TESTS_ASSERT(std::abs(dJ3_2(i,j)-dJ3(i,j))<prec);
  //   	if(std::abs(dJ3_2(i,j)-dJ3(i,j))>prec){
  //   	  std::cout << "Error " << N << " (" << i << ", " << j << ") "
  //   		    << "[" << i*StensorDimeToSize<N>::value+j << "]: "
  //   		    << dJ3_2(i,j) << " vs " << dJ3(i,j) << " "
  //   		    << std::abs(dJ3_2(i,j)-dJ3(i,j)) << std::endl;
  //   	}
  //     }
  //   }
  // }
};

TFEL_TESTS_GENERATE_PROXY(OrthotropicPlasticityTest,
                          "OrthotropicPlasticityTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("OrthotropicPlasticityTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
