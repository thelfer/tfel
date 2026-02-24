/*!
 * \file   tests/Math/tensor.cxx
 * \brief
 * \author Thomas Helfer
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

#include <cmath>
#include <string>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Tensor/TensorViewFromStensor.hxx"
#include "TFEL/Math/General/Complex.hxx"

template <typename T>
struct Name;

template <>
struct Name<long double> {
  static std::string getName() { return "long double"; }
};

template <>
struct Name<double> {
  static std::string getName() { return "double"; }
};

template <>
struct Name<float> {
  static std::string getName() { return "float"; }
};

struct TensorTest final : public tfel::tests::TestCase {
  TensorTest()
      : tfel::tests::TestCase("TFEL/Math", "TensorTest") {
  }  // end of TensorTest
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using tensor1 = tensor<1>;
    typedef tensor<1, unsigned short> tensor2;
    tensor1 v1;
    tensor2 v2;
    tensor1 v3;
    v2(0) = 2;
    v2(1) = 1;
    v2(2) = 5;
    v1(0) = 4.;
    v1(1) = 1.;
    v1(2) = 25.;
    v3 = v1 + v2;
    TFEL_TESTS_ASSERT(abs(v3(0) - 6) < 1.e-14);
    TFEL_TESTS_ASSERT(abs(v3(1) - 2) < 1.e-14);
    TFEL_TESTS_ASSERT(abs(v3(2) - 30) < 1.e-14);
    return this->result;
  }  // end of execute
};

struct TensorTest2 final : public tfel::tests::TestCase {
  TensorTest2()
      : tfel::tests::TestCase("TFEL/Math", "TensorTest2") {
  }  // end of TensorTest2
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    constexpr float prec = 1.e-3f;
    tensor<1, Complex<float>> s;
    tensor<1, Complex<float>> s2;
    s(0) = Complex<float>(2.f);
    s(1) = Complex<float>(3.1415929f);
    s(2) = Complex<float>(4231.421f);
    s2 = 0.5f * s;
    TFEL_TESTS_ASSERT((abs(real(s2(0) - Complex<float>(1.f))) < prec));
    TFEL_TESTS_ASSERT(abs(imag(s2(0))) < prec);
    TFEL_TESTS_ASSERT((abs(real(s2(1) - Complex<float>(1.5708f))) < prec));
    TFEL_TESTS_ASSERT(abs(imag(s2(1))) < prec);
    TFEL_TESTS_ASSERT((abs(real(s2(2) - Complex<float>(2115.7f))) < 1.f));
    TFEL_TESTS_ASSERT(abs(imag(s2(2))) < prec);
    return this->result;
  }  // end of execute
};

template <typename real>
struct TensorTest3_1D final : public tfel::tests::TestCase {
  TensorTest3_1D()
      : tfel::tests::TestCase("TFEL/Math",
                              "TensorTest3<1D," + Name<real>::getName() + ">") {
  }  // end of TensorTest3
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const real eps = 10 * numeric_limits<real>::epsilon();
    stensor<1u, real> s;
    tensor<1u, real> t;
    for (unsigned short i = 0; i != t.size(); ++i) {
      t[i] = exp(cos(real(i)));
    }
    s = syme(t);
    TFEL_TESTS_ASSERT((abs(s(0) - exp(cos(real(0)))) < eps));
    TFEL_TESTS_ASSERT((abs(s(1) - exp(cos(real(1)))) < eps));
    TFEL_TESTS_ASSERT((abs(s(2) - exp(cos(real(2)))) < eps));
    return this->result;
  }  // end of execute
};

template <typename real>
struct TensorTest3_2D final : public tfel::tests::TestCase {
  TensorTest3_2D()
      : tfel::tests::TestCase("TFEL/Math",
                              "TensorTest3<2D," + Name<real>::getName() + ">") {
  }  // end of TensorTest3
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const real eps = 10 * numeric_limits<real>::epsilon();
    const real cste = sqrt(real(2)) / real(2);
    stensor<2u, real> s;
    tensor<2u, real> t;
    for (unsigned short i = 0; i != t.size(); ++i) {
      t[i] = exp(cos(real(i) / t.size()));
    }
    s = syme(t);
    TFEL_TESTS_ASSERT((abs(s(0) - exp(cos(real(0) / 5))) < eps));
    TFEL_TESTS_ASSERT((abs(s(1) - exp(cos(real(1) / 5))) < eps));
    TFEL_TESTS_ASSERT((abs(s(2) - exp(cos(real(2) / 5))) < eps));
    TFEL_TESTS_ASSERT((abs(s(3) - cste * (exp(cos(real(3) / 5)) +
                                          exp(cos(real(4) / 5)))) < eps));
    return this->result;
  }  // end of execute
};

template <typename real>
struct TensorTest3_3D final : public tfel::tests::TestCase {
  TensorTest3_3D()
      : tfel::tests::TestCase("TFEL/Math",
                              "TensorTest3<3D," + Name<real>::getName() + ">") {
  }  // end of TensorTest3
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    const real eps = 10 * numeric_limits<real>::epsilon();
    const real cste = sqrt(real(2)) / real(2);
    stensor<3u, real> s;
    tensor<3u, real> t;
    for (unsigned short i = 0; i != t.size(); ++i) {
      t[i] = exp(cos(real(i)));
    }
    s = syme(t);
    TFEL_TESTS_ASSERT((abs(s(0) - exp(cos(real(0)))) < eps));
    TFEL_TESTS_ASSERT((abs(s(1) - exp(cos(real(1)))) < eps));
    TFEL_TESTS_ASSERT((abs(s(2) - exp(cos(real(2)))) < eps));
    TFEL_TESTS_ASSERT(
        (abs(s(3) - cste * (exp(cos(real(3))) + exp(cos(real(4))))) < eps));
    TFEL_TESTS_ASSERT(
        (abs(s(4) - cste * (exp(cos(real(5))) + exp(cos(real(6))))) < eps));
    TFEL_TESTS_ASSERT(
        (abs(s(5) - cste * (exp(cos(real(7))) + exp(cos(real(8))))) < eps));
    return this->result;
  }  // end of execute
};

struct TensorTest4 final : public tfel::tests::TestCase {
  TensorTest4()
      : tfel::tests::TestCase("TFEL/Math", "TensorTest4") {
  }  // end of TensorTest4
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using real = double;
    const real eps = 10. * numeric_limits<real>::epsilon();
    tensor<3u, real> t;
    for (unsigned short i = 0; i != t.size(); ++i) {
      t[i] = exp(cos(real(i)));
    }
    TFEL_TESTS_ASSERT((abs(trace(t) - exp(cos(real(0))) - exp(cos(real(1))) -
                           exp(cos(real(2)))) < eps));
    return this->result;
  }  // end of execute
};

struct TensorTest5_1D final : public tfel::tests::TestCase {
  TensorTest5_1D()
      : tfel::tests::TestCase("TFEL/Math", "TensorTest5_3D") {
  }  // end of TensorTest5
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using real = double;
    const real eps = 10. * numeric_limits<real>::epsilon();
    tensor<1u, real> t;
    for (unsigned short i = 0; i != t.size(); ++i) {
      t[i] = exp(cos(real(i)));
    }
    const tensor<1u, real> t_c = t;
    TFEL_TESTS_ASSERT((abs(t_c(0, 0) - exp(cos(real(0)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(1, 1) - exp(cos(real(1)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(2, 2) - exp(cos(real(2)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(0, 1) - real(0)) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(1, 0) - real(0)) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(0, 2) - real(0)) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(2, 0) - real(0)) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(1, 2) - real(0)) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(2, 1) - real(0)) < eps));
    return this->result;
  }  // end of execute
};

struct TensorTest5_2D final : public tfel::tests::TestCase {
  TensorTest5_2D()
      : tfel::tests::TestCase("TFEL/Math", "TensorTest5_2D") {
  }  // end of TensorTest5
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using real = double;
    const real eps = 10. * numeric_limits<real>::epsilon();
    tensor<2u, real> t;
    for (unsigned short i = 0; i != t.size(); ++i) {
      t[i] = exp(cos(real(i)));
    }
    const tensor<2u, real> t_c = t;
    TFEL_TESTS_ASSERT((abs(t_c(0, 0) - exp(cos(real(0)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(1, 1) - exp(cos(real(1)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(2, 2) - exp(cos(real(2)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(0, 1) - exp(cos(real(3)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(1, 0) - exp(cos(real(4)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(0, 2) - real(0)) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(2, 0) - real(0)) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(1, 2) - real(0)) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(2, 1) - real(0)) < eps));
    return this->result;
  }  // end of execute
};

struct TensorTest5_3D final : public tfel::tests::TestCase {
  TensorTest5_3D()
      : tfel::tests::TestCase("TFEL/Math", "TensorTest5_3D") {
  }  // end of TensorTest5
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using real = double;
    const real eps = 10. * numeric_limits<real>::epsilon();
    tensor<3u, real> t;
    for (unsigned short i = 0; i != t.size(); ++i) {
      t[i] = exp(cos(real(i)));
    }
    const TensorConceptBase<tensor<3u, real>>& t_c = t;
    TFEL_TESTS_ASSERT((abs(t_c(0, 0) - exp(cos(real(0)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(1, 1) - exp(cos(real(1)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(2, 2) - exp(cos(real(2)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(0, 1) - exp(cos(real(3)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(1, 0) - exp(cos(real(4)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(0, 2) - exp(cos(real(5)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(2, 0) - exp(cos(real(6)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(1, 2) - exp(cos(real(7)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(2, 1) - exp(cos(real(8)))) < eps));
    return this->result;
  }  // end of execute
};

struct TensorTest6 final : public tfel::tests::TestCase {
  TensorTest6()
      : tfel::tests::TestCase("TFEL/Math", "TensorTest6") {
  }  // end of TensorTest5
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using real = double;
    const real eps = 10. * numeric_limits<real>::epsilon();
    tensor<3u, real> t;
    for (unsigned short i = 0; i != t.size(); ++i) {
      t[i] = exp(cos(real(i)));
    }
    const TensorConceptBase<tensor<3u, real>>& t_c = t;
    TFEL_TESTS_ASSERT((abs(t_c(0, 0) - exp(cos(real(0)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(1, 1) - exp(cos(real(1)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(2, 2) - exp(cos(real(2)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(0, 1) - exp(cos(real(3)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(1, 0) - exp(cos(real(4)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(0, 2) - exp(cos(real(5)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(2, 0) - exp(cos(real(6)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(1, 2) - exp(cos(real(7)))) < eps));
    TFEL_TESTS_ASSERT((abs(t_c(2, 1) - exp(cos(real(8)))) < eps));
    return this->result;
  }  // end of execute
};

struct TensorTest7 : public tfel::tests::TestCase {
  TensorTest7()
      : tfel::tests::TestCase("TFEL/Math", "TensorTest7") {
  }  // end of TensorTest7
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using real = double;
    const real cste = real(1) / sqrt(real(2));
    const real eps = 100. * numeric_limits<real>::epsilon();
    typedef TensorViewFromStensor<const stensor<3u, real>&>::type TensorView;
    auto s = stensor<3u, real>{};
    for (unsigned short i = 0; i != s.size(); ++i) {
      s[i] = exp(cos(real(i)));
    }
    const auto t = TensorView{s};
    const auto v = t(0) - exp(cos(real(0)));
    TFEL_TESTS_ASSERT((abs(v) < eps));
    TFEL_TESTS_ASSERT((abs(t(1) - exp(cos(real(1)))) < eps));
    TFEL_TESTS_ASSERT((abs(t(2) - exp(cos(real(2)))) < eps));
    TFEL_TESTS_ASSERT((abs(t(3) - cste * exp(cos(real(3)))) < eps));
    TFEL_TESTS_ASSERT((abs(t(4) - cste * exp(cos(real(3)))) < eps));
    TFEL_TESTS_ASSERT((abs(t(5) - cste * exp(cos(real(4)))) < eps));
    TFEL_TESTS_ASSERT((abs(t(6) - cste * exp(cos(real(4)))) < eps));
    TFEL_TESTS_ASSERT((abs(t(7) - cste * exp(cos(real(5)))) < eps));
    TFEL_TESTS_ASSERT((abs(t(8) - cste * exp(cos(real(5)))) < eps));
    return this->result;
  }  // end of execute
};

template <unsigned short N>
struct TensorTest8;

template <>
struct TensorTest8<1u> final : public tfel::tests::TestCase {
  TensorTest8()
      : tfel::tests::TestCase("TFEL/Math", "TensorTest8<1>") {
  }  // end of TensorTest8
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using real = double;
    const real eps = 10. * numeric_limits<real>::epsilon();
    stensor<1u, real> s;
    tensor<1u, real> t;
    for (unsigned short i = 0; i != s.size(); ++i) {
      s[i] = exp(real(i * i) / (s.size() * s.size()));
    }
    for (unsigned short i = 0; i != t.size(); ++i) {
      t[i] = exp(sin(real(i * i + 1)));
    }
    const tensor<1u, real> t2(0.62 * (0.45 * s + 2.3 * t));
    TFEL_TESTS_ASSERT((abs(t2(0) - (0.62 * (0.45 * s(0) + 2.3 * t(0)))) < eps));
    TFEL_TESTS_ASSERT((abs(t2(1) - (0.62 * (0.45 * s(1) + 2.3 * t(1)))) < eps));
    TFEL_TESTS_ASSERT((abs(t2(2) - (0.62 * (0.45 * s(2) + 2.3 * t(2)))) < eps));
    return this->result;
  }  // end of execute
};

template <>
struct TensorTest8<2u> final : public tfel::tests::TestCase {
  TensorTest8()
      : tfel::tests::TestCase("TFEL/Math", "TensorTest8<2>") {
  }  // end of TensorTest8
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using real = double;
    const real cste = real(1) / sqrt(real(2));
    const real eps = 1e-12;
    stensor<2u, real> s;
    tensor<2u, real> t;
    for (unsigned short i = 0; i != s.size(); ++i) {
      s[i] = exp(real(i * i) / (s.size() * s.size()));
    }
    for (unsigned short i = 0; i != t.size(); ++i) {
      t[i] = exp(sin(real(i * i + 1)));
    }
    const tensor<2u, real> t2(0.62 * (0.45 * s + 2.3 * t));
    TFEL_TESTS_ASSERT((abs(t2(0) - (0.62 * (0.45 * s(0) + 2.3 * t(0)))) < eps));
    TFEL_TESTS_ASSERT((abs(t2(1) - (0.62 * (0.45 * s(1) + 2.3 * t(1)))) < eps));
    TFEL_TESTS_ASSERT((abs(t2(2) - (0.62 * (0.45 * s(2) + 2.3 * t(2)))) < eps));
    TFEL_TESTS_ASSERT(
        (abs(t2(3) - (0.62 * (0.45 * cste * s(3) + 2.3 * t(3)))) < eps));
    TFEL_TESTS_ASSERT(
        (abs(t2(4) - (0.62 * (0.45 * cste * s(3) + 2.3 * t(4)))) < eps));
    return this->result;
  }  // end of execute
};

template <>
struct TensorTest8<3u> final : public tfel::tests::TestCase {
  TensorTest8()
      : tfel::tests::TestCase("TFEL/Math", "TensorTest8<3>") {
  }  // end of TensorTest8
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using real = double;
    const real cste = real(1) / sqrt(real(2));
    const real eps = 100. * numeric_limits<real>::epsilon();
    stensor<3u, real> s;
    tensor<3u, real> t;
    for (unsigned short i = 0; i != s.size(); ++i) {
      s[i] = exp(real(i * i) / (s.size() * s.size()));
    }
    for (unsigned short i = 0; i != t.size(); ++i) {
      t[i] = exp(sin(real(i * i + 1)));
    }
    const tensor<3u, real> t2(0.62 * (0.45 * s + 2.3 * t));

    TFEL_TESTS_ASSERT((abs(t2(0) - (0.62 * (0.45 * s(0) + 2.3 * t(0)))) < eps));
    TFEL_TESTS_ASSERT((abs(t2(1) - (0.62 * (0.45 * s(1) + 2.3 * t(1)))) < eps));
    TFEL_TESTS_ASSERT((abs(t2(2) - (0.62 * (0.45 * s(2) + 2.3 * t(2)))) < eps));
    TFEL_TESTS_ASSERT(
        (abs(t2(3) - (0.62 * (0.45 * cste * s(3) + 2.3 * t(3)))) < eps));
    TFEL_TESTS_ASSERT(
        (abs(t2(4) - (0.62 * (0.45 * cste * s(3) + 2.3 * t(4)))) < eps));
    TFEL_TESTS_ASSERT(
        (abs(t2(5) - (0.62 * (0.45 * cste * s(4) + 2.3 * t(5)))) < eps));
    TFEL_TESTS_ASSERT(
        (abs(t2(6) - (0.62 * (0.45 * cste * s(4) + 2.3 * t(6)))) < eps));
    TFEL_TESTS_ASSERT(
        (abs(t2(7) - (0.62 * (0.45 * cste * s(5) + 2.3 * t(7)))) < eps));
    TFEL_TESTS_ASSERT(
        (abs(t2(8) - (0.62 * (0.45 * cste * s(5) + 2.3 * t(8)))) < eps));
    return this->result;
  }  // end of execute
};

template <unsigned short N>
struct TensorTest9;

template <>
struct TensorTest9<1u> final : public tfel::tests::TestCase {
  TensorTest9()
      : tfel::tests::TestCase("TFEL/Math", "TensorTest9<1>") {
  }  // end of TensorTest9
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using real = double;
    const real eps = 10. * numeric_limits<real>::epsilon();
    stensor<1u, real> s;
    tensor<1u, real> t;
    for (unsigned short i = 0; i != s.size(); ++i) {
      s[i] = exp(real(i * i) / (s.size() * s.size()));
    }
    for (unsigned short i = 0; i != t.size(); ++i) {
      t[i] = exp(sin(real(i * i + 1)));
    }
    const tensor<1u, real> t2(0.62 * (2.3 * t + 0.45 * s));
    TFEL_TESTS_ASSERT((abs(t2(0) - (0.62 * (0.45 * s(0) + 2.3 * t(0)))) < eps));
    TFEL_TESTS_ASSERT((abs(t2(1) - (0.62 * (0.45 * s(1) + 2.3 * t(1)))) < eps));
    TFEL_TESTS_ASSERT((abs(t2(2) - (0.62 * (0.45 * s(2) + 2.3 * t(2)))) < eps));
    return this->result;
  }  // end of execute
};

template <>
struct TensorTest9<2u> final : public tfel::tests::TestCase {
  TensorTest9()
      : tfel::tests::TestCase("TFEL/Math", "TensorTest9<2>") {
  }  // end of TensorTest9
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using real = double;
    const real cste = real(1) / sqrt(real(2));
    const real eps = 1e-12;
    stensor<2u, real> s;
    tensor<2u, real> t;
    for (unsigned short i = 0; i != s.size(); ++i) {
      s[i] = exp(real(i * i) / (s.size() * s.size()));
    }
    for (unsigned short i = 0; i != t.size(); ++i) {
      t[i] = exp(sin(real(i * i + 1)));
    }
    const tensor<2u, real> t2(0.62 * (2.3 * t + 0.45 * s));
    TFEL_TESTS_ASSERT((abs(t2(0) - (0.62 * (0.45 * s(0) + 2.3 * t(0)))) < eps));
    TFEL_TESTS_ASSERT((abs(t2(1) - (0.62 * (0.45 * s(1) + 2.3 * t(1)))) < eps));
    TFEL_TESTS_ASSERT((abs(t2(2) - (0.62 * (0.45 * s(2) + 2.3 * t(2)))) < eps));
    TFEL_TESTS_ASSERT(
        (abs(t2(3) - (0.62 * (2.3 * t(3) + 0.45 * cste * s(3)))) < eps));
    TFEL_TESTS_ASSERT(
        (abs(t2(4) - (0.62 * (2.3 * t(4) + 0.45 * cste * s(3)))) < eps));
    return this->result;
  }  // end of execute
};

template <>
struct TensorTest9<3u> : public tfel::tests::TestCase {
  TensorTest9()
      : tfel::tests::TestCase("TFEL/Math", "TensorTest9<3>") {
  }  // end of TensorTest9
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using real = double;
    const real icste = real(1) / sqrt(real(2));
    const real eps = 10. * numeric_limits<real>::epsilon();
    stensor<3u, real> s;
    tensor<3u, real> t;
    for (unsigned short i = 0; i != s.size(); ++i) {
      s[i] = exp(real(i * i) / (s.size() * s.size()));
    }
    for (unsigned short i = 0; i != t.size(); ++i) {
      t[i] = exp(sin(real(i * i + 1)));
    }
    const tensor<3u, real> t2(0.62 * (2.3 * t + 0.45 * s));
    TFEL_TESTS_ASSERT(
        (std::abs(t2(0) - (0.62 * (0.45 * s(0) + 2.3 * t(0)))) < eps));
    TFEL_TESTS_ASSERT(
        (std::abs(t2(1) - (0.62 * (0.45 * s(1) + 2.3 * t(1)))) < eps));
    TFEL_TESTS_ASSERT(
        (std::abs(t2(2) - (0.62 * (0.45 * s(2) + 2.3 * t(2)))) < eps));
    TFEL_TESTS_ASSERT(
        (std::abs(t2(3) - (0.62 * (0.45 * icste * s(3) + 2.3 * t(3)))) < eps));
    TFEL_TESTS_ASSERT(
        (std::abs(t2(4) - (0.62 * (0.45 * icste * s(3) + 2.3 * t(4)))) < eps));
    TFEL_TESTS_ASSERT(
        (std::abs(t2(5) - (0.62 * (0.45 * icste * s(4) + 2.3 * t(5)))) < eps));
    TFEL_TESTS_ASSERT(
        (std::abs(t2(6) - (0.62 * (0.45 * icste * s(4) + 2.3 * t(6)))) < eps));
    TFEL_TESTS_ASSERT(
        (std::abs(t2(7) - (0.62 * (0.45 * icste * s(5) + 2.3 * t(7)))) < eps));
    TFEL_TESTS_ASSERT(
        (std::abs(t2(8) - (0.62 * (0.45 * icste * s(5) + 2.3 * t(8)))) < eps));
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(TensorTest, "TensorTest");
TFEL_TESTS_GENERATE_PROXY(TensorTest2, "TensorTest2");
using TensorTest3Float1D = TensorTest3_1D<float>;
using TensorTest3Double1D = TensorTest3_1D<double>;
using TensorTest3Float2D = TensorTest3_2D<float>;
using TensorTest3Double2D = TensorTest3_2D<double>;
using TensorTest3Float3D = TensorTest3_3D<float>;
using TensorTest3Double3D = TensorTest3_3D<double>;
TFEL_TESTS_GENERATE_PROXY(TensorTest3Float1D, "TensorTest3<1u,float>");
TFEL_TESTS_GENERATE_PROXY(TensorTest3Double1D, "TensorTest3<1u,double>");
TFEL_TESTS_GENERATE_PROXY(TensorTest3Float2D, "TensorTest3<2u,float>");
TFEL_TESTS_GENERATE_PROXY(TensorTest3Double2D, "TensorTest3<2u,double>");
TFEL_TESTS_GENERATE_PROXY(TensorTest3Float3D, "TensorTest3<3u,float>");
TFEL_TESTS_GENERATE_PROXY(TensorTest3Double3D, "TensorTest3<3u,double>");
TFEL_TESTS_GENERATE_PROXY(TensorTest4, "TensorTest4");
TFEL_TESTS_GENERATE_PROXY(TensorTest5_1D, "TensorTest5_1D");
TFEL_TESTS_GENERATE_PROXY(TensorTest5_2D, "TensorTest5_2D");
TFEL_TESTS_GENERATE_PROXY(TensorTest5_3D, "TensorTest5_3D");
TFEL_TESTS_GENERATE_PROXY(TensorTest6, "TensorTest6");
TFEL_TESTS_GENERATE_PROXY(TensorTest7, "TensorTest7");
using TensorTest8_1D = TensorTest8<1U>;
using TensorTest8_2D = TensorTest8<2U>;
using TensorTest8_3D = TensorTest8<3U>;
TFEL_TESTS_GENERATE_PROXY(TensorTest8_1D, "TensorTest8_1D");
TFEL_TESTS_GENERATE_PROXY(TensorTest8_2D, "TensorTest8_2D");
TFEL_TESTS_GENERATE_PROXY(TensorTest8_3D, "TensorTest8_3D");
using TensorTest9_1D = TensorTest9<1U>;
using TensorTest9_2D = TensorTest9<2U>;
using TensorTest9_3D = TensorTest9<3U>;
TFEL_TESTS_GENERATE_PROXY(TensorTest9_1D, "TensorTest9_1D");
TFEL_TESTS_GENERATE_PROXY(TensorTest9_2D, "TensorTest9_2D");
TFEL_TESTS_GENERATE_PROXY(TensorTest9_3D, "TensorTest9_3D");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("Tensor.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
