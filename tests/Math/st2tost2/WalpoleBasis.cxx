/*!
 * \file   tests/Math/st2tost2/WalpoleBasis.cxx
 * \brief
 * \author Antoine Martin
 * \date   15 may 2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif NDEBUG

#include <cmath>
#include <cstdlib>
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/ST2toST2/WalpoleBasis.hxx"
#include "TFEL/Material/IsotropicModuli.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct WalpoleBasis final : public tfel::tests::TestCase {
  WalpoleBasis()
      : tfel::tests::TestCase("TFEL/Math", "WalpoleBasis") {
  }  // end of WalpoleBasis
  tfel::tests::TestResult execute() override {
 
    static constexpr auto eps = std::numeric_limits<double>::epsilon();
  
    using namespace tfel::math;
    const auto n = tvector<3u,double>{0.,0.,1.};
    
    const auto E1=TransverseIsotropicWalpoleBasis<double>::E1(n);
    const auto E2=TransverseIsotropicWalpoleBasis<double>::E2(n);
    const auto E3=TransverseIsotropicWalpoleBasis<double>::E3(n);
    const auto E4=TransverseIsotropicWalpoleBasis<double>::E4(n);
    const auto F=TransverseIsotropicWalpoleBasis<double>::F(n);
    const auto G=TransverseIsotropicWalpoleBasis<double>::G(n);
    const auto E1E1 = E1*E1;
    const auto E1E3 = E1*E3;
    TFEL_TESTS_ASSERT(norm(E1E1-E1)<eps);
    TFEL_TESTS_ASSERT(norm(E1E3-E3)<eps);
    const auto E2E2 = E2*E2;
    const auto E2E4 = E2*E4;
    TFEL_TESTS_ASSERT(norm(E2E2-E2)<eps);
    TFEL_TESTS_ASSERT(norm(E2E4-E4)<eps);
    const auto E3E2 = E3*E2;
    const auto E3E4 = E3*E4;
    TFEL_TESTS_ASSERT(norm(E3E2-E3)<eps);
    TFEL_TESTS_ASSERT(norm(E3E4-E1)<10*eps);
    const auto E4E1 = E4*E1;
    const auto E4E3 = E4*E3;
    TFEL_TESTS_ASSERT(norm(E4E1-E4)<eps);
    TFEL_TESTS_ASSERT(norm(E4E3-E2)<10*eps);
    const auto FF = F*F;
    const auto GG = G*G;
    TFEL_TESTS_ASSERT(norm(FF-F)<eps);
    TFEL_TESTS_ASSERT(norm(GG-G)<eps);
    
    const auto I=st2tost2<3u,double>::Id();
    const auto I_=TransverseIsotropicWalpoleBasis<double>::components(n,I);
    const auto J=st2tost2<3u,double>::J();
    const auto J_=TransverseIsotropicWalpoleBasis<double>::components(n,J);
    const auto K=st2tost2<3u,double>::K();
    const auto K_=TransverseIsotropicWalpoleBasis<double>::components(n,K);
    
    TFEL_TESTS_ASSERT(my_abs(I_[0]-1)<eps);
    TFEL_TESTS_ASSERT(my_abs(I_[1]-1)<eps);
    TFEL_TESTS_ASSERT(my_abs(I_[2]-0)<eps);
    TFEL_TESTS_ASSERT(my_abs(I_[3]-0)<eps);
    TFEL_TESTS_ASSERT(my_abs(I_[4]-1)<eps);
    TFEL_TESTS_ASSERT(my_abs(I_[5]-1)<eps);
    
    TFEL_TESTS_ASSERT(my_abs(J_[0]-1./3)<eps);
    TFEL_TESTS_ASSERT(my_abs(J_[1]-2./3)<eps);
    TFEL_TESTS_ASSERT(my_abs(J_[2]-sqrt(2)/3)<eps);
    TFEL_TESTS_ASSERT(my_abs(J_[3]-sqrt(2)/3)<eps);
    TFEL_TESTS_ASSERT(my_abs(J_[4])<eps);
    TFEL_TESTS_ASSERT(my_abs(J_[5])<eps);
    
    TFEL_TESTS_ASSERT(my_abs(K_[0]-2./3)<eps);
    TFEL_TESTS_ASSERT(my_abs(K_[1]-1./3)<eps);
    TFEL_TESTS_ASSERT(my_abs(K_[2]+sqrt(2)/3)<eps);
    TFEL_TESTS_ASSERT(my_abs(K_[3]+sqrt(2)/3)<eps);
    TFEL_TESTS_ASSERT(my_abs(K_[4]-1)<eps);
    TFEL_TESTS_ASSERT(my_abs(K_[5]-1)<eps);
    
    const st2tost2<3u,double> tens =E1+2*E2+3*E3+4*E4+5*F+6*G;
    const auto c1=TransverseIsotropicWalpoleBasis<double>::components(n,tens);
    TFEL_TESTS_ASSERT(my_abs(c1[0]-1)<eps);
    TFEL_TESTS_ASSERT(my_abs(c1[1]-2)<eps);
    TFEL_TESTS_ASSERT(my_abs(c1[2]-3)<10*eps);
    TFEL_TESTS_ASSERT(my_abs(c1[3]-4)<10*eps);
    TFEL_TESTS_ASSERT(my_abs(c1[4]-5)<eps);
    TFEL_TESTS_ASSERT(my_abs(c1[5]-6)<eps);

    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(WalpoleBasis, "WalpoleBasis");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("WalpoleBasis.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
