/*!
 * \file   tests/Utilities/FCString.cxx
 * \brief
 * \author Thomas Helfer
 * \date   23 janv. 2017
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

#include <string>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <sstream>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Utilities/FCString.hxx"

struct FCStringTest final : public tfel::tests::TestCase {
  FCStringTest()
      : tfel::tests::TestCase("TFEL/Utilities", "FCStringTest") {
  }  // end of MyTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    this->test4();
    this->test5();
    this->test6();
    this->test7();
    this->test8();
    this->test9();
    this->test10();
    this->test11();
    return this->result;
  }  // end of execute()
 private:
  void test1() {
    using tfel::utilities::fcstring;
    tfel::utilities::fcstring<3> s = "abc";
    const tfel::utilities::fcstring<3> s2 = s;
    TFEL_TESTS_ASSERT(!s.empty());
    TFEL_TESTS_ASSERT(s[0] == 'a');
    TFEL_TESTS_ASSERT(s[1] == 'b');
    TFEL_TESTS_ASSERT(s[2] == 'c');
    TFEL_TESTS_ASSERT(s.back() == 'c');
    TFEL_TESTS_ASSERT(std::strcmp(s.data(), "abc") == 0);
    TFEL_TESTS_ASSERT(s.size() == 3);
    TFEL_TESTS_CHECK_THROW(s[3] = '1', std::out_of_range);
    TFEL_TESTS_ASSERT(!s2.empty());
    TFEL_TESTS_ASSERT(s2[0] == 'a');
    TFEL_TESTS_ASSERT(s2[1] == 'b');
    TFEL_TESTS_ASSERT(s2[2] == 'c');
    TFEL_TESTS_ASSERT(s2.back() == 'c');
    TFEL_TESTS_ASSERT(std::strcmp(s2.data(), "abc") == 0);
    TFEL_TESTS_ASSERT(s2.size() == 3);
    TFEL_TESTS_CHECK_THROW(s2[3], std::out_of_range);
  }
  void test2() {
    tfel::utilities::fcstring<3> s = "ab";
    const tfel::utilities::fcstring<3> s2 = s;
    TFEL_TESTS_ASSERT(!s.empty());
    TFEL_TESTS_ASSERT(s[0] == 'a');
    TFEL_TESTS_ASSERT(s[1] == 'b');
    TFEL_TESTS_ASSERT(std::strcmp(s.data(), "ab") == 0);
    TFEL_TESTS_ASSERT(s.size() == 2);
    TFEL_TESTS_CHECK_THROW(s[3] = '1', std::out_of_range);
    TFEL_TESTS_ASSERT(!s2.empty());
    TFEL_TESTS_ASSERT(s2[0] == 'a');
    TFEL_TESTS_ASSERT(s2[1] == 'b');
    TFEL_TESTS_ASSERT(std::strcmp(s2.data(), "ab") == 0);
    TFEL_TESTS_ASSERT(s.size() == 2);
    TFEL_TESTS_CHECK_THROW(s2[3], std::out_of_range);
  }
  void test3() {
    const tfel::utilities::fcstring<3> s = std::string("abc");
    TFEL_TESTS_ASSERT(!s.empty());
    TFEL_TESTS_ASSERT(s[0] == 'a');
    TFEL_TESTS_ASSERT(s[1] == 'b');
    TFEL_TESTS_ASSERT(s[2] == 'c');
    TFEL_TESTS_ASSERT(std::strcmp(s.data(), "abc") == 0);
    TFEL_TESTS_ASSERT(s.size() == 3);
  }
  void test4() {
    const tfel::utilities::fcstring<3> s = std::string("ab");
    TFEL_TESTS_ASSERT(!s.empty());
    TFEL_TESTS_ASSERT(s[0] == 'a');
    TFEL_TESTS_ASSERT(s[1] == 'b');
    TFEL_TESTS_ASSERT(std::strcmp(s.data(), "ab") == 0);
    TFEL_TESTS_ASSERT(s.size() == 2);
  }
  void test5() {
    using tfel::utilities::fcstring;
    fcstring<3> s;
    std::string ss = "abcd";
    TFEL_TESTS_ASSERT(s.size() == 0);
    TFEL_TESTS_ASSERT(s.empty());
    TFEL_TESTS_CHECK_THROW(s = "abcd", std::length_error);
    TFEL_TESTS_CHECK_THROW(s = ss, std::length_error);
    TFEL_TESTS_CHECK_THROW(s = fcstring<3>("abcd"), std::length_error);
    TFEL_TESTS_CHECK_THROW(s = fcstring<3>(ss), std::length_error);
  }
  void test6() {
    std::ostringstream os;
    std::istringstream is;
    const tfel::utilities::fcstring<4> s = "1234";
    tfel::utilities::fcstring<4> s2;
    os << s;
    is.str(os.str());
    is >> s2;
    TFEL_TESTS_ASSERT(::strcmp(s2.data(), "1234") == 0);
  }
  void test7() {
    const tfel::utilities::fcstring<4> s = "abc";
    const tfel::utilities::fcstring<4> s2 = s;
    const tfel::utilities::fcstring<4> s3 = "1234";
    const tfel::utilities::fcstring<4> s4 = "123";
    TFEL_TESTS_ASSERT(s == s2);
    TFEL_TESTS_ASSERT(!(s != s2));
    TFEL_TESTS_ASSERT(!(s == s3));
    TFEL_TESTS_ASSERT(!(s3 == s));
    TFEL_TESTS_ASSERT(s != s3);
    TFEL_TESTS_ASSERT(s3 != s);
    TFEL_TESTS_ASSERT(!(s == s4));
    TFEL_TESTS_ASSERT(!(s4 == s));
    TFEL_TESTS_ASSERT(s != s4);
    TFEL_TESTS_ASSERT(s4 != s);
  }
  void test8() {
    const tfel::utilities::fcstring<4> s = "abc";
    const auto s2 = "1234";
    const auto s3 = "123";
    TFEL_TESTS_ASSERT(!(s == s2));
    TFEL_TESTS_ASSERT(!(s2 == s));
    TFEL_TESTS_ASSERT(s != s2);
    TFEL_TESTS_ASSERT(s2 != s);
    TFEL_TESTS_ASSERT(!(s == s3));
    TFEL_TESTS_ASSERT(!(s3 == s));
    TFEL_TESTS_ASSERT(s != s3);
    TFEL_TESTS_ASSERT(s3 != s);
  }
  void test9() {
    const tfel::utilities::fcstring<4> s = "abc";
    const std::string s2 = "1234";
    const std::string s3 = "123";
    TFEL_TESTS_ASSERT(!(s == s2));
    TFEL_TESTS_ASSERT(!(s2 == s));
    TFEL_TESTS_ASSERT(s != s2);
    TFEL_TESTS_ASSERT(s2 != s);
    TFEL_TESTS_ASSERT(!(s == s3));
    TFEL_TESTS_ASSERT(!(s3 == s));
    TFEL_TESTS_ASSERT(s != s3);
    TFEL_TESTS_ASSERT(s3 != s);
  }
  void test10() {
    using namespace tfel::utilities;
    ;
    const char c[5] = {'1', '2', '3', '4', '5'};
    const char c2[5] = {'1', '2', '3', '\0', '5'};
    const auto sv4 = narrow<4>(c);
    const auto sv2 = narrow<2>(c);
    const auto sv5 = narrow<5>(c2);
    const auto sv5b = narrow<5>(c);
    const fcstring<4> s = sv4;
    TFEL_TESTS_ASSERT(s.size() == 4);
    TFEL_TESTS_ASSERT(s == "1234");
    const fcstring<4> s2 = sv2;
    TFEL_TESTS_ASSERT(s2.size() == 2);
    TFEL_TESTS_ASSERT(s2 == "12");
    const fcstring<4> s3 = sv5;
    TFEL_TESTS_ASSERT(s3.size() == 3);
    TFEL_TESTS_ASSERT(s3 == "123");
    fcstring<4> s4;
    TFEL_TESTS_CHECK_THROW(s4 = sv5b, std::length_error);
    TFEL_TESTS_CHECK_THROW(s4 = c, std::length_error);
    TFEL_TESTS_CHECK_THROW(s4 = fcstring<4>(c), std::length_error);
    TFEL_TESTS_CHECK_THROW(s4 = fcstring<4>(c, false), std::length_error);
    s4 = fcstring<4>(c, true);
    TFEL_TESTS_ASSERT(s4.size() == 4);
    TFEL_TESTS_ASSERT(s4 == "1234");
    s4.pop_back();
    TFEL_TESTS_ASSERT(s4.size() == 3);
    TFEL_TESTS_ASSERT(s4 == "123");
    s4.back() = 'a';
    TFEL_TESTS_ASSERT(s4 == "12a");
    auto s5 = fcstring<4>();
    TFEL_TESTS_ASSERT(s5.size() == 0);
    TFEL_TESTS_ASSERT(s5 == "");
    TFEL_TESTS_CHECK_THROW(s5.back() = 'a', std::length_error);
    TFEL_TESTS_CHECK_THROW(s5.pop_back(), std::length_error);
  }
  void test11() {
    using namespace tfel::utilities;
    ;
    char c[5] = {'\0', '\0', '\0', '\0', '\0'};
    char c2[5] = {'1', '2', '3', '4', '\0'};
    fcstring<4> s = "abcd";
    const fcstring<4> s2 = s;
    std::copy(s.begin(), s.end(), c);
    TFEL_TESTS_ASSERT(s == c);
    std::copy(s.cbegin(), s.cend(), c);
    TFEL_TESTS_ASSERT(s == c);
    std::copy(s2.begin(), s2.end(), c);
    TFEL_TESTS_ASSERT(s2 == c);
    std::copy(s.rbegin(), s.rend(), c);
    TFEL_TESTS_ASSERT(std::strncmp(c, "dcba", 4) == 0);
    std::copy(s2.rbegin(), s2.rend(), c);
    TFEL_TESTS_ASSERT(std::strncmp(c, "dcba", 4) == 0);
    std::copy(c2, c2 + 4, s.begin());
    TFEL_TESTS_ASSERT(s == "1234");
  }
};

TFEL_TESTS_GENERATE_PROXY(FCStringTest, "FCString");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("FCString.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
