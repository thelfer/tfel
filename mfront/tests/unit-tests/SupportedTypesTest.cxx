/*!
 * \file   mfront/tests/unit-tests/SupportedTypesTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   30/08/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * re served.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <utility>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "MFront/SupportedTypes.hxx"

struct SupportedTypesTest final : public tfel::tests::TestCase {
  SupportedTypesTest()
      : tfel::tests::TestCase("MFront", "SupportedTypesTest") {
  }  // end of SupportedTypesTest

  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    this->test4();
    this->test5();
    this->test6();
    return this->result;
  }  // end of execute

 private:
  void test1() {
    const auto opts = [] {
      auto o = mfront::SupportedTypes::TypeParsingOptions{};
      o.integer_constants = std::map<std::string, int>{{"Nss", 12}};
      o.use_qt = true;
      return o;
    }();
    auto check = [&opts, this](std::string_view v1, std::string_view v2) {
      TFEL_TESTS_CHECK_EQUAL(mfront::SupportedTypes::extractType(v1, opts), v2);
    };
    check("stress", "stress");
    check("::stress", "::stress");
    check("std::vector<real, tfel::math::matrix<3u, 3u, real> >",
          "std::vector<real,tfel::math::matrix<3,3,real>>");
    check("std::vector<real, tfel::math::matrix<3u, 3u, real>>",
          "std::vector<real,tfel::math::matrix<3,3,real>>");
    check("std::vector<std::vector<tfel::math::matrix<3u,3u,real>>>",
          "std::vector<std::vector<tfel::math::matrix<3,3,real>>>");
    check("stensor <N, real>", "tfel::math::stensor<N,real>");
    check("derivative_type<real,Stensor>",
          "tfel::math::derivative_type<real,Stensor>");
    check("result_type<toto,tata,OpDiv>",
          "tfel::math::result_type<toto,tata,tfel::math::OpDiv>");
    check("result_type<toto,tata,tfel::math::OpDiv>",
          "tfel::math::result_type<toto,tata,tfel::math::OpDiv>");
    check("quantity<real,1,0,0,3,2,-1,0>",
          "tfel::math::quantity_or_base_type<use_qt,real,1,0,0,3,2,-1,"
          "0>");
    check("test_empty_template_arguments<>", "test_empty_template_arguments<>");
    check("derivative_type<quantity<real,1,0,0,3,2,-1,0>,time>",
          "tfel::math::derivative_type<"
          "tfel::math::quantity_or_base_type<use_qt,real,1,0,0,3,2,-1,"
          "0>,time>");
    check("vector<2u,3u+3u>", "vector<2,6>");
    check("quantity<real,0,1>",
          "tfel::math::quantity_or_base_type<use_qt,real,0,1,0,0,0,0,0>");
    check("quantity<real,0,1,2>",
          "tfel::math::quantity_or_base_type<use_qt,real,0,1,2,0,0,0,0>");
    check("tvector<Nss,real>", "tfel::math::tvector<12,real>");
    check("stensor<N,real>", "tfel::math::stensor<N,real>");
    check("tensor<N,real>", "tfel::math::tensor<N,real>");
    check("st2tost2<N,real>", "tfel::math::st2tost2<N,real>");
    check("t2tot2<N,real>", "tfel::math::t2tot2<N,real>");
    check("st2tot2<N,real>", "tfel::math::st2tot2<N,real>");
    check("t2tost2<N,real>", "tfel::math::t2tost2<N,real>");
  }
  void test2() {
    const auto opts = [] {
      auto o = mfront::SupportedTypes::TypeParsingOptions{};
      o.integer_constants = std::map<std::string, int>{{"Nss", 12}};
      o.use_qt = false;
      return o;
    }();
    auto check = [&opts, this](std::string_view v1, std::string_view v2) {
      TFEL_TESTS_CHECK_EQUAL(mfront::SupportedTypes::extractType(v1, opts), v2);
    };
    check("quantity<real,1,0,0,3,2,-1,0>", "real");
    check("derivative_type<quantity<real,1,0,0,3,2,-1,0>,time>",
          "tfel::math::derivative_type<real,time>");
    check("quantity<real,0,1>", "real");
    check("quantity<real,0,1,2>", "real");
  }
  void test3() {
    using pair = std::pair<std::string, mfront::SupportedTypes::TypeFlag>;
    auto o = mfront::SupportedTypes::TypeParsingOptions{};
    o.integer_constants = std::map<std::string, int>{{"Nss", 12}};
    auto check = [this](std::string_view v,
                        const mfront::SupportedTypes::TypeFlag f) {
      TFEL_TESTS_CHECK_EQUAL(mfront::SupportedTypes::getTypeFlag(v), f);
    };
    check("Stensor", mfront::SupportedTypes::STENSOR);
    check("stensor<N,real>", mfront::SupportedTypes::STENSOR);
    check("tensor<N,real>", mfront::SupportedTypes::TENSOR);
    for (const auto& m :
         {pair{"tvector<N,real>", mfront::SupportedTypes::TVECTOR},
          pair{"tensor<N,real>", mfront::SupportedTypes::TENSOR},
          pair{"tfel::math::stensor<N,real>",
               mfront::SupportedTypes::STENSOR}}) {
      check("result_type<" + m.first + "," + m.first + ",OpPlus>", m.second);
      check("result_type<" + m.first + ",stress,OpMult>", m.second);
    }
    check("result_type<stress,stress,OpMult>", mfront::SupportedTypes::SCALAR);
    check("stensor<3u,real>", mfront::SupportedTypes::STENSOR);
  }
  void test4() {
    using mfront::SupportedTypes;
    auto check = [this](const std::string_view v,
                        const SupportedTypes::TypeSize& v2) {
      TFEL_TESTS_CHECK_EQUAL(SupportedTypes::getTypeSize(v), v2);
    };
    auto check_throw = [this](std::string_view v1) {
      TFEL_TESTS_CHECK_THROW(mfront::SupportedTypes::getTypeSize(v1),
                             std::runtime_error);
    };
    check("Stensor", SupportedTypes::STENSOR);
    check("derivative_type<Stensor,real>", SupportedTypes::STENSOR);
    check("Stensor4", SupportedTypes::TypeSize::buildFromExponents(0u, 2u, 0u));
    check("tfel::math::st2tost2<N,real>",
          SupportedTypes::TypeSize::buildFromExponents(0u, 2u, 0u));
    check("tfel::math::st2tot2<N,real>",
          SupportedTypes::TypeSize::buildFromExponents(0u, 1u, 1u));
    check("tfel::math::t2tost2<N,real>",
          SupportedTypes::TypeSize::buildFromExponents(0u, 1u, 1u));
    check("tfel::math::t2tot2<N,real>",
          SupportedTypes::TypeSize::buildFromExponents(0u, 0u, 2u));
    check("tfel::math::st2tost2<3,real>",
          SupportedTypes::TypeSize(36, SupportedTypes::SCALAR));
    check("derivative_type<Stensor4,Stensor>",
          SupportedTypes::TypeSize::buildFromExponents(0u, 3u, 0u));
    check("derivative_type<Stensor4,Tensor>",
          SupportedTypes::TypeSize::buildFromExponents(0u, 2u, 1u));
    check_throw("result_type<stress,Stensor,OpDiv>");
    check("result_type<stensor<1u,real>,stensor<1u,real>,OpPlus>",
          SupportedTypes::TypeSize(3, SupportedTypes::SCALAR));
    check_throw("result_type<stensor<1u,real>,stensor<2u,real>,OpPlus>");
  }
  void test5() {
    using mfront::SupportedTypes;
    auto check = [this](const SupportedTypes::TypeFlag& v1,
                        const SupportedTypes::TypeFlag& v2) {
      TFEL_TESTS_CHECK_EQUAL(v1, v2);
    };
    check(SupportedTypes::getTypeFlag("tvector<3u,strain>"),
          SupportedTypes::TVECTOR);
  }
  void test6() {
    using mfront::SupportedTypes;
    constexpr auto mid = 4 + (2 << 3);
    constexpr auto sid = 4 + (1 << 3) + (1 << 8);
    auto check = [this](const std::string_view v1, const int v2) {
      const auto id = SupportedTypes::getTypeIdentifier(v1);
      TFEL_TESTS_CHECK_EQUAL(id, v2);
    };
    check("strain", 0);
    check("stress", 0);
    //
    check("StrainStensor", 1);
    //
    check("tvector<N,stress>", 2);
    check("tvector<3u,strain>", 2 + (3 << 3));
    check("TVector", 2);
    //
    check("Tensor", 3);
    //
    check("st2tost2<N,stress>", sid);
    check("StiffnessTensor", sid);
    //
    check("t2tost2<N,stress>", 4 + (1 << 3) + (3 << 8));
    //
    check("tmatrix<N, N, stress>", 4 + (2 << 3) + (2 << 8));
    check("tmatrix<3u, 2u, stress>",
          4 + ((2 + (3 << 3)) << 3) + ((2 + (2 << 3)) << 8));
    //
    check("derivative_type<tvector<N,stress>,stress>", mid);
    check("derivative_type<stensor<N,stress>, stensor<N,stress>>",
          4 + (1 << 3) + (1 << 8));
    //
    check("tvector<3u, st2tost2<N,stress>>",
          5 + (1 << 3) + (3 << 6) + (sid << 13));
    check("fsarray<3u, st2tost2<N,stress>>",
          5 + (1 << 3) + (3 << 6) + (sid << 13));
    check("tmatrix<3u, 2u, st2tost2<N,stress>>",
          5 + (2 << 3) + (3 << 6) + (2 << 13) + (sid << 20));
    check("fsarray<8, derivative_type<tvector<N,stress>,stress>>",
          5 + (1 << 3) + (8 << 6) + (mid << 13));
  }
};

TFEL_TESTS_GENERATE_PROXY(SupportedTypesTest, "SupportedTypesTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("SupportedTypes.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
