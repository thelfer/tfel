/*!
 * \file   tests/Math/xtensor.cxx
 * \brief
 *
 * \author Antoine Martin
 * \date   19 sept 2025
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
#include <iostream>
#include "TFEL/Math/xtensor.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/qt.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct XtensorTest final : public tfel::tests::TestCase {
  XtensorTest()
      : tfel::tests::TestCase("TFEL/Math", "XtensorTest") {
  }  // end of XtensorTest
  tfel::tests::TestResult execute() override {

    using stress = typename tfel::config::Types<1u, double, true>::stress;
    static constexpr auto eps = std::numeric_limits<double>::epsilon();
    using real = double;
    using namespace tfel::math;

    xtensor<1,3,real> s01({1.,0.,0.});
    xtensor<1,3,real> s02({0.,1.,0.});
    xtensor<1,3,real> s03({0.,0.,5.});

    xtensor<1,3,real> sv;
    sv(1)=1;
    std::cout <<"hey "<< sv(1) <<std::endl;

    xtensor<4,3,real> sv4;
    std::cout <<"hey "<< sv4(1,0,0,3) <<std::endl;

    xtensor<2,3,real> s1({s01,s02,s03});
    
    std::cout << s01(0) <<std::endl;
    s01(0)=2;
    std::cout << s01(0) <<" "<<s1(0,0) <<std::endl;
    std::cout <<s1(2,0) <<s1(2,1)<<s1(2,2) <<std::endl;
    s1(2,1)=-1;
    std::cout <<s1(2,0) <<s1(2,1)<<s1(2,2) <<std::endl;

    s1(2)=xtensor<1,3,real>({1,0,3});
    std::cout <<s1(2,0) <<s1(2,1)<<s1(2,2) <<std::endl;

    

    xtensor<1,3,real> szero=xtensor<1,3,real>::zero();
    std::cout << szero(0) <<std::endl;
    xtensor<5,3,real> szero5=xtensor<5,3,real>::zero();
    std::cout << szero5(0,0,0,0,0) <<std::endl;
    szero5(0,0,0,0,0)=real(1);
    std::cout <<"hey"<<szero5(0,0,0,0,0) <<std::endl;

    szero5(0,0,0,0)=xtensor<1,3,real>({1,0,3});
    std::cout <<"hey"<<szero5(0,0,0,0,2) <<std::endl;

    xtensor<1,3,real> s= real(4)*s01;
    std::cout << s(0)<<s(1)<<s(2) <<std::endl;
    xtensor<5,3,real> ss= real(4)*szero5;
    std::cout << ss(0,0,0,0,0) <<std::endl;
    
    auto s5 = s01.otimes<1>(s01);
    std::cout << s5(0,0) <<std::endl;
    auto s6 = s1.otimes<2>(s1);
    std::cout << s6(0,0,0,0) <<std::endl;

    auto ssum = s1+s5;
    std::cout << ssum(0,0) <<ssum(2,2) <<std::endl;

    auto ssub = s1-s5;
    std::cout << ssub(0,0) <<ssub(2,2) <<std::endl;

    auto scont = ssum.contract(0);
    std::cout << scont <<std::endl;

    auto scont6 = s6.contract(2);
    std::cout << scont6(0,0) <<std::endl;

    auto sdot = s01.dot(s01);
    std::cout << sdot <<std::endl;

    auto sdot2 = s01.dot(s1);
    std::cout << sdot2(0) <<std::endl;

    auto scm = ssum.contract_on<1>({0});
    std::cout << scm <<scont <<std::endl;

    auto scm6 = s6.contract_on<1>({2});
    std::cout << scm6(0,0)<<scont6(0,0) <<std::endl;

    auto scm66 = s6.contract_on<2>({1,0});
    std::cout << scm66 <<std::endl;

    auto s1dbldots1 = s1.contract_with<2,2>(s1);
    std::cout << s1dbldots1 <<std::endl;

    auto s6quaddots6 = s6.contract_with<4,4>(s6);
    std::cout << s6quaddots6 <<std::endl;

    auto s33=s1.otimes<1>(s01);
    auto s6tpldots33 = s6.contract_with<3,3>(s33);
    std::cout << s6tpldots33(0) <<std::endl;

    xtensor<1,6,real> st({1.,0.,0.,0.,0.,0.});
    auto stens = st.toStensor();
    auto xtens = toXtensor(stens);
    std::cout << stens(0) <<" "<<xtens(0) <<std::endl;


    xtensor<2,6,real> st4 = st.otimes<1>(st);
    auto stens4 = st4.toStensor4();
    auto xtens4 = toXtensor(stens4);
    std::cout << stens4(0,0)<<" "<<xtens4(0,0) <<std::endl;

    auto xdr = s1.drop();
    std::cout << s1(2,0) <<" "<<xdr(0,2)<<std::endl;

    auto xtr = s1.transpose();
    std::cout << s1(2,0) <<" "<<xtr(0,2)<<std::endl;

    auto xdr6 = s6.drop();
    std::cout << s6(2,0,2,2) <<" "<<xdr6(0,2,2,2)<<std::endl;

    auto xdr3 = s33.drop();
    std::cout << s33(2,2,0) <<" "<<xdr3(2,0,2)<<std::endl;

    auto xtr3 = s33.transpose();
    std::cout << s33(2,2,0) <<" "<<xtr3(0,2,2)<<std::endl;

    auto xtr6 = s6.transpose();
    std::cout << s6(2,0,2,2) <<" "<<xtr6(2,2,0,2)<<std::endl;

    auto st2 = st2tost2<3,real>::zero();
    st2(0,0)=1;
    tmatrix<12,12,real> m;
    fill_matrix<12,12,real>(m,st2,0,0);
    std::cout<<"oui "<<m(0,0)<<std::endl;
    //TFEL_TESTS_ASSERT(my_abs(C1111 - C(0, 0)) < seps);


    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(XtensorTest, "XtensorTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("xtensor.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
