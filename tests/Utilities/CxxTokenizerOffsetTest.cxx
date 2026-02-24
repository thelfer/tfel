/*!
 * \file   CxxTokenizerOffsetTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   24 août 2015
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

#include <tuple>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"

struct CxxTokenizerOffsetTest final : public tfel::tests::TestCase {
  //! a simple alias
  using size_type = tfel::utilities::Token::size_type;
  using Token = tfel::utilities::Token;
  using flags = Token::TokenFlag;
  CxxTokenizerOffsetTest()
      : tfel::tests::TestCase("TFEL/Utilities", "CxxTokenizerOffsetTest") {
  }  // end of MyTest
  tfel::tests::TestResult execute() override {
    auto make_result =
        [](const std::string& r, const size_type s,
           const flags f) -> std::tuple<std::string, size_type, flags> {
      return std::make_tuple(r, s, f);
    };
    this->check("  void", {make_result("void", 2u, Token::Standard)});
    // this->check(R"("my test" //toto   "second test")",
    // 		{make_result(R"("my test")",0u,Token::String),
    // 		 make_result{R"(toto   "second test")",12u,Token::Comment)});
    // this->check(R"("my test" /*toto*/ "second test")",
    // 		{make_result(R"("my test")",0u,Token::String),
    // 	         make_result(R"(toto)",12u,Token::Comment),
    // 		 make_result(R"("second test")",19u,Token::String)});
    // this->check("void readBlock(std::vector<std::string>& b,\n"
    // 		"               const_iterator& p,\n"
    // 		"               const const_iterator pe)",
    // 		{make_result("void",0u,Token::Standard),
    // 		 make_result("readBlock",5u,Token::Standard),
    // 		 make_result("(",14u,Token::Standard),
    // 		 make_result("std",15u,Token::Standard),
    // 		 make_result("::",18u,Token::Standard),
    // 		 make_result("vector",20u,Token::Standard),
    // 		 make_result("<",26u,Token::Standard),
    // 		 make_result("std",27u,Token::Standard),
    // 		 make_result("::",30u,Token::Standard),
    // 		 make_result("string",32u,Token::Standard),
    // 		 make_result(">",38u,Token::Standard),
    // 		 make_result("&",39u,Token::Standard),
    // 		 make_result("b",41u,Token::Standard),
    // 		 make_result(",",42u,Token::Standard),
    // 		 make_result("const_iterator",15u,Token::Standard),
    // 		 make_result("&",29u,Token::Standard),
    // 		 make_result("p",31u,Token::Standard),
    // 		 make_result(",",32u,Token::Standard),
    // 		 make_result("const",15u,Token::Standard),
    // 		 make_result("const_iterator",21u,Token::Standard),
    // 		 make_result("pe",36u,Token::Standard),
    // 		 make_result(")",38u,Token::Standard)});
    return this->result;
  }  // end of execute()
 private:
  /*!
   * \param[in] s:  string to be parsed
   * \param[in] et: expected tokens and offset
   */
  void check(const char* const s,
             const std::vector<std::tuple<std::string, size_type, flags>>& et) {
    tfel::utilities::CxxTokenizer tokenizer;
    tokenizer.parseString(s);
    TFEL_TESTS_ASSERT(tokenizer.size() == et.size());
    if (tokenizer.size() != et.size()) {
      return;
    }
    auto pt = tokenizer.begin();
    auto pet = et.begin();
    while (pt != tokenizer.end()) {
      TFEL_TESTS_ASSERT(pt->value == std::get<0>(*pet));
      TFEL_TESTS_ASSERT(pt->offset == std::get<1>(*pet));
      TFEL_TESTS_ASSERT(pt->flag == std::get<2>(*pet));
      ++pt;
      ++pet;
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(CxxTokenizerOffsetTest, "CxxTokenizerOffset");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("CxxTokenizer.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
