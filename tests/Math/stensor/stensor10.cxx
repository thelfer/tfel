/*!
 * \file  stensor.cxx
 * \brief
 * \author Helfer Thomas
 * \date   26 nov. 2014
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

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/Stensor/DecompositionInPositiveAndNegativeParts.hxx"

struct ComputePositivePartAndDerivativeTest1D final
    : public tfel::tests::TestCase {
  ComputePositivePartAndDerivativeTest1D()
      : tfel::tests::TestCase("TFEL/Math",
                              "ComputePositivePartAndDerivativeTest1D") {
  }  // end of ComputePositivePartAndDerivativeTest1D
  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const double v1[6] = {8.2, -4.5, 7.2};
    const double v2[6] = {-8.2, 4.5, 7.2};
    const double v3[6] = {8.2, 4.5, -7.2};
    stensor<1u, double> zero(0.);
    stensor<1u, double> s(zero);
    this->check(stensor<1u, double>(0.), 1.e-8);
    this->check(stensor<1u, double>::Id(), 1.e-6);
    // {1,1,0}
    s(0) = s(1) = 1.;
    s(2) = 0.;
    this->check(s, 1.e-6);
    // {1,0,1}
    s(0) = s(2) = 1.;
    s(1) = 0.;
    this->check(s, 1.e-8);
    // {0,1,1}
    s(0) = 0.;
    s(1) = s(2) = 1.;
    this->check(s, 1.e-8);
    s = stensor<1u, double>(v1);
    this->check(s, 1.e-10);
    s = stensor<1u, double>(v2);
    this->check(s, 1.e-10);
    s = stensor<1u, double>(v3);
    this->check(s, 1.e-10);
    return this->result;
  }  // end of execute
 private:
  void check(const tfel::math::stensor<1u, double>& s, const double prec) {
    using namespace std;
    using namespace tfel::math;
    const double eps = 1.e-4;
    st2tost2<1u, double> dpp;
    st2tost2<1u, double> ndpp;
    stensor<1u, double> pp;
    computeStensorPositivePartAndDerivative(dpp, pp, s, 1.e-10);
    for (unsigned short i = 0; i != 3u; ++i) {
      stensor<1u, double> ls(s);
      ls[i] += eps;
      const auto ppf = positive_part(ls);
      ls[i] -= 2. * eps;
      const auto ppb = positive_part(ls);
      ls[i] += eps;
      for (unsigned short j = 0; j != 3u; ++j) {
        ndpp(j, i) = (ppf(j) - ppb(j)) / (2. * eps);
      }
    }
    for (unsigned short i = 0; i != 3u; ++i) {
      for (unsigned short j = 0; j != 3u; ++j) {
        if (abs(dpp(i, j) - ndpp(i, j)) > prec) {
          cout << "dpp : " << i << " " << j << " " << dpp(i, j) << " "
               << ndpp(i, j) << " " << abs(dpp(i, j) - ndpp(i, j)) << endl;
        }
        TFEL_TESTS_ASSERT(abs(dpp(i, j) - ndpp(i, j)) < prec);
      }
    }
  }
};

struct ComputePositivePartAndDerivativeTest2D final
    : public tfel::tests::TestCase {
  ComputePositivePartAndDerivativeTest2D()
      : tfel::tests::TestCase("TFEL/Math",
                              "ComputePositivePartAndDerivativeTest2D") {
  }  // end of ComputePositivePartAndDerivativeTest2D
  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const double sqrt2 = sqrt(2.);
    const double v1[4] = {8.2, -4.5, 7.2, 2.3 * sqrt2};
    const double v2[4] = {-8.2, 4.5, 7.2, -2.3 * sqrt2};
    const double v3[4] = {8.2, 4.5, -7.2, 2.3 * sqrt2};
    stensor<2u, double> zero(0.);
    stensor<2u, double> s(zero);
    this->check(stensor<2u, double>(0.), 1.e-10);
    this->check(stensor<2u, double>::Id(), 1.e-10);
    // {1,1,0}
    s(0) = s(1) = 1.;
    s(2) = 0.;
    this->check(s, 1.e-8);
    // {1,0,1}
    s(0) = s(2) = 1.;
    s(1) = 0.;
    this->check(s, 1.e-8);
    // {0,1,1}
    s(0) = 0.;
    s(1) = s(2) = 1.;
    this->check(s, 1.e-8);
    s = stensor<2u, double>(v1);
    this->check(s, 1.e-10);
    s = stensor<2u, double>(v2);
    this->check(s, 1.e-10);
    s = stensor<2u, double>(v3);
    this->check(s, 1.e-10);
    return this->result;
  }  // end of execute
 private:
  void check(const tfel::math::stensor<2u, double>& s, const double prec) {
    using namespace std;
    using namespace tfel::math;
    const double eps = 1.e-4;
    st2tost2<2u, double> dpp;
    st2tost2<2u, double> ndpp;
    stensor<2u, double> pp;
    computeStensorPositivePartAndDerivative(dpp, pp, s, 1.e-10);
    for (unsigned short i = 0; i != 4u; ++i) {
      stensor<2u, double> ls(s);
      ls[i] += eps;
      const auto ppf = positive_part(ls);
      ls[i] -= 2. * eps;
      const auto ppb = positive_part(ls);
      ls[i] += eps;
      for (unsigned short j = 0; j != 4u; ++j) {
        ndpp(j, i) = (ppf(j) - ppb(j)) / (2. * eps);
      }
    }
    for (unsigned short i = 0; i != 4u; ++i) {
      for (unsigned short j = 0; j != 4u; ++j) {
        if (abs(dpp(i, j) - ndpp(i, j)) > prec) {
          cout << "dpp : " << i << " " << j << " " << dpp(i, j) << " "
               << ndpp(i, j) << " " << abs(dpp(i, j) - ndpp(i, j)) << endl;
        }
        TFEL_TESTS_ASSERT(abs(dpp(i, j) - ndpp(i, j)) < prec);
      }
    }
  }
};

struct ComputePositivePartAndDerivativeTest3D final
    : public tfel::tests::TestCase {
  ComputePositivePartAndDerivativeTest3D()
      : tfel::tests::TestCase("TFEL/Math",
                              "ComputePositivePartAndDerivativeTest3D") {
  }  // end of ComputePositivePartAndDerivativeTest3D
  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const double sqrt2 = sqrt(2.);
    const double v1[6] = {8.2, -4.5, 7.2, 2.3 * sqrt2, 0., 0.};
    const double v2[6] = {-8.2, 4.5, 7.2, 0, 2.3 * sqrt2, 0.};
    const double v3[6] = {8.2, 4.5, -7.2, 0, 0., 2.3 * sqrt2};
    const double v4[6] = {8.2,          4.5,           -7.2,
                          3.14 * sqrt2, -6.42 * sqrt2, 2.3 * sqrt2};
    stensor<3u, double> zero(0.);
    stensor<3u, double> s(zero);
    this->check(stensor<3u, double>(0.), 1.e-8);
    this->check(stensor<3u, double>::Id(), 1.e-6);
    // {1,1,0}
    s(0) = s(1) = 1.;
    s(2) = 0.;
    this->check(s, 3e-5);
    // {1,0,1}
    s(0) = s(2) = 1.;
    s(1) = 0.;
    this->check(s, 1.e-8);
    // {0,1,1}
    s(0) = 0.;
    s(1) = s(2) = 1.;
    this->check(s, 1.e-8);
    s = stensor<3u, double>(v1);
    this->check(s, 1.e-10);
    s = stensor<3u, double>(v2);
    this->check(s, 1.e-10);
    s = stensor<3u, double>(v3);
    this->check(s, 1.e-10);
    s = stensor<3u, double>(v4);
    this->check(s, 1.e-10);
    return this->result;
  }  // end of execute
 private:
  void check(const tfel::math::stensor<3u, double>& s, const double prec) {
    using namespace std;
    using namespace tfel::math;
    const double eps = 1.e-4;
    st2tost2<3u, double> dpp;
    st2tost2<3u, double> ndpp;
    stensor<3u, double> pp;
    computeStensorPositivePartAndDerivative(dpp, pp, s, 1.e-10);
    for (unsigned short i = 0; i != 6u; ++i) {
      stensor<3u, double> ls(s);
      ls[i] += eps;
      stensor<3u, double> ppf = positive_part(ls);
      ls[i] -= 2. * eps;
      stensor<3u, double> ppb = positive_part(ls);
      ls[i] += eps;
      for (unsigned short j = 0; j != 6u; ++j) {
        ndpp(j, i) = (ppf(j) - ppb(j)) / (2. * eps);
      }
    }
    for (unsigned short i = 0; i != 6u; ++i) {
      for (unsigned short j = 0; j != 6u; ++j) {
        if (abs(dpp(i, j) - ndpp(i, j)) > prec) {
          cout << "dpp : " << i << " " << j << " " << dpp(i, j) << " "
               << ndpp(i, j) << " " << abs(dpp(i, j) - ndpp(i, j)) << endl;
        }
        TFEL_TESTS_ASSERT(abs(dpp(i, j) - ndpp(i, j)) < prec);
      }
    }
  }
};

struct DecompositionInPositiveAndNegativePartsTest1D final
    : public tfel::tests::TestCase {
  DecompositionInPositiveAndNegativePartsTest1D()
      : tfel::tests::TestCase("TFEL/Math",
                              "DecompositionInPositiveAndNegativePartsTest1D") {
  }  // end of DecompositionInPositiveAndNegativePartsTest1D
  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const double v1[3] = {8.2, -4.5, 7.2};
    const double v2[3] = {-8.2, 4.5, 7.2};
    const double v3[3] = {8.2, 4.5, -7.2};
    stensor<1u, double> zero(0.);
    stensor<1u, double> s(zero);
    this->check(stensor<1u, double>(0.), 1.e-8);
    this->check(stensor<1u, double>::Id(), 1.e-6);
    // {1,1,0}
    s(0) = s(1) = 1.;
    s(2) = 0.;
    this->check(s, 1.e-6);
    // {1,0,1}
    s(0) = s(2) = 1.;
    s(1) = 0.;
    this->check(s, 1.e-8);
    // {0,1,1}
    s(0) = 0.;
    s(1) = s(2) = 1.;
    this->check(s, 1.e-8);
    s = stensor<1u, double>(v1);
    this->check(s, 1.e-10);
    s = stensor<1u, double>(v2);
    this->check(s, 1.e-10);
    s = stensor<1u, double>(v3);
    this->check(s, 1.e-10);
    return this->result;
  }  // end of execute
 private:
  void check(const tfel::math::stensor<1u, double>& s, const double prec) {
    using namespace std;
    using namespace tfel::math;
    const double eps = 1.e-4;
    st2tost2<1u, double> dpp;
    st2tost2<1u, double> dnp;
    st2tost2<1u, double> ndpp;
    st2tost2<1u, double> ndnp;
    stensor<1u, double> pp;
    stensor<1u, double> np;
    computeStensorDecompositionInPositiveAndNegativeParts(dpp, dnp, pp, np, s,
                                                          1.e-10);
    for (unsigned short i = 0; i != 3u; ++i) {
      stensor<1u, double> ls(s);
      ls[i] += eps;
      const auto ppf = positive_part(ls);
      const auto npf = negative_part(ls);
      ls[i] -= 2. * eps;
      const auto ppb = positive_part(ls);
      const auto npb = negative_part(ls);
      ls[i] += eps;
      for (unsigned short j = 0; j != 3u; ++j) {
        ndpp(j, i) = (ppf(j) - ppb(j)) / (2. * eps);
        ndnp(j, i) = (npf(j) - npb(j)) / (2. * eps);
      }
    }
    for (unsigned short i = 0; i != 3u; ++i) {
      for (unsigned short j = 0; j != 3u; ++j) {
        if (abs(dpp(i, j) - ndpp(i, j)) > prec) {
          cout << "dpp : " << i << " " << j << " " << dpp(i, j) << " "
               << ndpp(i, j) << " " << abs(dpp(i, j) - ndpp(i, j)) << endl;
        }
        if (abs(dnp(i, j) - ndnp(i, j)) > prec) {
          cout << "dnp : " << i << " " << j << " " << dnp(i, j) << " "
               << ndnp(i, j) << " " << abs(dnp(i, j) - ndnp(i, j)) << endl;
        }
        TFEL_TESTS_ASSERT(abs(dpp(i, j) - ndpp(i, j)) < prec);
        TFEL_TESTS_ASSERT(abs(dnp(i, j) - ndnp(i, j)) < prec);
      }
    }
  }
};

struct DecompositionInPositiveAndNegativePartsTest2D final
    : public tfel::tests::TestCase {
  DecompositionInPositiveAndNegativePartsTest2D()
      : tfel::tests::TestCase("TFEL/Math",
                              "DecompositionInPositiveAndNegativePartsTest2D") {
  }  // end of DecompositionInPositiveAndNegativePartsTest2D
  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const double sqrt2 = sqrt(2.);
    const double v1[4] = {8.2, -4.5, 7.2, 2.3 * sqrt2};
    const double v2[4] = {-8.2, 4.5, 7.2, -2.3 * sqrt2};
    const double v3[4] = {8.2, 4.5, -7.2, 2.3 * sqrt2};
    stensor<2u, double> zero(0.);
    stensor<2u, double> s(zero);
    this->check(stensor<2u, double>(0.), 1.e-10);
    this->check(stensor<2u, double>::Id(), 1.e-10);
    // {1,1,0}
    s(0) = s(1) = 1.;
    s(2) = 0.;
    this->check(s, 1.e-8);
    // {1,0,1}
    s(0) = s(2) = 1.;
    s(1) = 0.;
    this->check(s, 1.e-8);
    // {0,1,1}
    s(0) = 0.;
    s(1) = s(2) = 1.;
    this->check(s, 1.e-8);
    s = stensor<2u, double>(v1);
    this->check(s, 1.e-10);
    s = stensor<2u, double>(v2);
    this->check(s, 1.e-10);
    s = stensor<2u, double>(v3);
    this->check(s, 1.e-10);
    return this->result;
  }  // end of execute
 private:
  void check(const tfel::math::stensor<2u, double>& s, const double prec) {
    using namespace std;
    using namespace tfel::math;
    const double eps = 1.e-4;
    st2tost2<2u, double> dpp;
    st2tost2<2u, double> dnp;
    st2tost2<2u, double> ndpp;
    st2tost2<2u, double> ndnp;
    stensor<2u, double> pp;
    stensor<2u, double> np;
    computeStensorDecompositionInPositiveAndNegativeParts(dpp, dnp, pp, np, s,
                                                          1.e-10);
    for (unsigned short i = 0; i != 4u; ++i) {
      stensor<2u, double> ls(s);
      ls[i] += eps;
      const auto ppf = positive_part(ls);
      const auto npf = negative_part(ls);
      ls[i] -= 2. * eps;
      const auto ppb = positive_part(ls);
      const auto npb = negative_part(ls);
      ls[i] += eps;
      for (unsigned short j = 0; j != 4u; ++j) {
        ndpp(j, i) = (ppf(j) - ppb(j)) / (2. * eps);
        ndnp(j, i) = (npf(j) - npb(j)) / (2. * eps);
      }
    }
    for (unsigned short i = 0; i != 4u; ++i) {
      for (unsigned short j = 0; j != 4u; ++j) {
        if (abs(dpp(i, j) - ndpp(i, j)) > prec) {
          cout << "dpp : " << i << " " << j << " " << dpp(i, j) << " "
               << ndpp(i, j) << " " << abs(dpp(i, j) - ndpp(i, j)) << endl;
        }
        if (abs(dnp(i, j) - ndnp(i, j)) > prec) {
          cout << "dnp : " << i << " " << j << " " << dnp(i, j) << " "
               << ndnp(i, j) << " " << abs(dnp(i, j) - ndnp(i, j)) << endl;
        }
        TFEL_TESTS_ASSERT(abs(dpp(i, j) - ndpp(i, j)) < prec);
        TFEL_TESTS_ASSERT(abs(dnp(i, j) - ndnp(i, j)) < prec);
      }
    }
  }
};

struct DecompositionInPositiveAndNegativePartsTest3D final
    : public tfel::tests::TestCase {
  DecompositionInPositiveAndNegativePartsTest3D()
      : tfel::tests::TestCase("TFEL/Math",
                              "DecompositionInPositiveAndNegativePartsTest3D") {
  }  // end of DecompositionInPositiveAndNegativePartsTest3D
  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const double sqrt2 = sqrt(2.);
    const double v1[6] = {8.2, -4.5, 7.2, 2.3 * sqrt2, 0., 0.};
    const double v2[6] = {-8.2, 4.5, 7.2, 0, 2.3 * sqrt2, 0.};
    const double v3[6] = {8.2, 4.5, -7.2, 0, 0., 2.3 * sqrt2};
    const double v4[6] = {8.2,          4.5,           -7.2,
                          3.14 * sqrt2, -6.42 * sqrt2, 2.3 * sqrt2};
    stensor<3u, double> zero(0.);
    stensor<3u, double> s(zero);
    this->check(stensor<3u, double>(0.), 1.e-8);
    this->check(stensor<3u, double>::Id(), 1.e-6);
    // {1,1,0}
    s(0) = s(1) = 1.;
    s(2) = 0.;
    this->check(s, 3e-5);
    // {1,0,1}
    s(0) = s(2) = 1.;
    s(1) = 0.;
    this->check(s, 1.e-8);
    // {0,1,1}
    s(0) = 0.;
    s(1) = s(2) = 1.;
    this->check(s, 1.e-8);
    s = stensor<3u, double>(v1);
    this->check(s, 1.e-10);
    s = stensor<3u, double>(v2);
    this->check(s, 1.e-10);
    s = stensor<3u, double>(v3);
    this->check(s, 1.e-10);
    s = stensor<3u, double>(v4);
    this->check(s, 1.e-10);
    return this->result;
  }  // end of execute
 private:
  void check(const tfel::math::stensor<3u, double>& s, const double prec) {
    using namespace std;
    using namespace tfel::math;
    const double eps = 1.e-4;
    st2tost2<3u, double> dpp;
    st2tost2<3u, double> dnp;
    st2tost2<3u, double> ndpp;
    st2tost2<3u, double> ndnp;
    stensor<3u, double> pp;
    stensor<3u, double> np;
    computeStensorDecompositionInPositiveAndNegativeParts(dpp, dnp, pp, np, s,
                                                          1.e-10);
    for (unsigned short i = 0; i != 6u; ++i) {
      stensor<3u, double> ls(s);
      ls[i] += eps;
      const auto ppf = positive_part(ls);
      const auto npf = negative_part(ls);
      ls[i] -= 2 * eps;
      const auto ppb = positive_part(ls);
      const auto npb = negative_part(ls);
      ls[i] += eps;
      for (unsigned short j = 0; j != 6u; ++j) {
        ndpp(j, i) = (ppf(j) - ppb(j)) / (2. * eps);
        ndnp(j, i) = (npf(j) - npb(j)) / (2. * eps);
      }
    }
    for (unsigned short i = 0; i != 6u; ++i) {
      for (unsigned short j = 0; j != 6u; ++j) {
        if (abs(dpp(i, j) - ndpp(i, j)) > prec) {
          cout << "dpp : " << i << " " << j << " " << dpp(i, j) << " "
               << ndpp(i, j) << " " << abs(dpp(i, j) - ndpp(i, j)) << endl;
        }
        if (abs(dnp(i, j) - ndnp(i, j)) > prec) {
          cout << "dnp : " << i << " " << j << " " << dnp(i, j) << " "
               << ndnp(i, j) << " " << abs(dnp(i, j) - ndnp(i, j)) << endl;
        }
        TFEL_TESTS_ASSERT(abs(dpp(i, j) - ndpp(i, j)) < prec);
        TFEL_TESTS_ASSERT(abs(dnp(i, j) - ndnp(i, j)) < prec);
      }
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(ComputePositivePartAndDerivativeTest1D,
                          "ComputePositivePartAndDerivativeTest1D");
TFEL_TESTS_GENERATE_PROXY(ComputePositivePartAndDerivativeTest2D,
                          "ComputePositivePartAndDerivativeTest2D");
TFEL_TESTS_GENERATE_PROXY(ComputePositivePartAndDerivativeTest3D,
                          "ComputePositivePartAndDerivativeTest3D");

TFEL_TESTS_GENERATE_PROXY(DecompositionInPositiveAndNegativePartsTest1D,
                          "DecompositionInPositiveAndNegativePartsTest1D");
TFEL_TESTS_GENERATE_PROXY(DecompositionInPositiveAndNegativePartsTest2D,
                          "DecompositionInPositiveAndNegativePartsTest2D");
TFEL_TESTS_GENERATE_PROXY(DecompositionInPositiveAndNegativePartsTest3D,
                          "DecompositionInPositiveAndNegativePartsTest3D");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("stensor10.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
