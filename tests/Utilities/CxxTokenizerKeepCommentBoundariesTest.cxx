/*!
 * \file   CxxTokenizerKeepCommentBoundariesTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date 24/08/2015
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

struct CxxTokenizerKeepCommentBoundariesTest final
    : public tfel::tests::TestCase {
  CxxTokenizerKeepCommentBoundariesTest()
      : tfel::tests::TestCase("TFEL/Utilities",
                              "CxxTokenizerKeepCommentBoundariesTest") {
  }  // end of MyTest
  tfel::tests::TestResult execute() override {
    this->check("/* test */", "/* test */", true);
    this->check("/* test */", "test", false);
    this->checkOffsets();
    return this->result;
  }  // end of execute()
 private:
  //! a simple alias
  using size_type = tfel::utilities::Token::size_type;
  using Token = tfel::utilities::Token;
  using flags = Token::TokenFlag;
  /*!
   * \param[in] s: string to be parsed
   * \param[in] r: expected result
   * \param[in] b: keep comment boundaries
   */
  void check(const char* const s, const std::string r, const bool b) {
    tfel::utilities::CxxTokenizer tokenizer;
    tokenizer.keepCommentBoundaries(b);
    tokenizer.parseString(s);
    TFEL_TESTS_ASSERT(tokenizer.size() == 1u);
    if (tokenizer.size() != 1u) {
      return;
    }
    const auto& t = *(tokenizer.begin());
    std::cout << "'" << t.value << "'\n"
              << "'" << r << "'\n";
    TFEL_TESTS_ASSERT(t.value == r);
    TFEL_TESTS_ASSERT(t.flag == tfel::utilities::Token::Comment);
  }
  /*!
   * \brief a token of the expected result of the parsing of a string, that is
   * its value, the line and the offset it is found at
   */
  using token = std::tuple<std::string, size_type, size_type>;
  /*!
   * \brief the offsets of the tokens which follow a comment on the same line
   * are those of the line, whether the boundaries of the comments are kept or
   * not: reading the delimiters which open a comment advances the offset along
   * with the iterator of the line, so that those offsets do not depend on the
   * number of characters of those delimiters, see issue #1407.
   *
   * The value of a comment whose boundaries are kept is the spelling of the
   * comment in the file, taken at the offset of the token, which is that of the
   * delimiter which opens the comment.
   */
  void checkOffsets() {
    // a `C` style comment, whose opening delimiter holds two characters
    this->check("/* c */ int a;",
                {{"/* c */", 1u, 0u},
                 {"int", 1u, 8u},
                 {"a", 1u, 12u},
                 {";", 1u, 13u}},
                true);
    this->check("/* c */ int a;",
                {{"c", 1u, 3u},
                 {"int", 1u, 8u},
                 {"a", 1u, 12u},
                 {";", 1u, 13u}},
                false);
    // a doxygen comment, whose opening delimiter holds three characters
    this->check("/*! c */ int a;",
                {{"/*! c */", 1u, 0u},
                 {"int", 1u, 9u},
                 {"a", 1u, 13u},
                 {";", 1u, 14u}},
                true);
    this->check("/*! c */ int a;",
                {{"c", 1u, 4u},
                 {"int", 1u, 9u},
                 {"a", 1u, 13u},
                 {";", 1u, 14u}},
                false);
    // two comments on the same line
    this->check("/* c */ /* d */ int a;",
                {{"/* c */", 1u, 0u},
                 {"/* d */", 1u, 8u},
                 {"int", 1u, 16u},
                 {"a", 1u, 20u},
                 {";", 1u, 21u}},
                true);
    // an empty comment, whose offset is that of its delimiter, is followed by
    // the tokens of the line
    this->check("a; /**/ b;",
                {{"a", 1u, 0u},
                 {";", 1u, 1u},
                 {"/**/", 1u, 3u},
                 {"b", 1u, 8u},
                 {";", 1u, 9u}},
                true);
    // an inline comment, as those one may find in an argument list
    this->check("int f(const int& /* hidden fortran parameter */);",
                {{"int", 1u, 0u},
                 {"f", 1u, 4u},
                 {"(", 1u, 5u},
                 {"const", 1u, 6u},
                 {"int", 1u, 12u},
                 {"&", 1u, 15u},
                 {"/* hidden fortran parameter */", 1u, 17u},
                 {")", 1u, 47u},
                 {";", 1u, 48u}},
                true);
    // a backward doxygen comment, whose opening delimiter holds four
    // characters, is the last token of its line
    this->check("int a; /*!< c */",
                {{"int", 1u, 0u},
                 {"a", 1u, 4u},
                 {";", 1u, 5u},
                 {"/*!< c */", 1u, 7u}},
                true);
    // a `C++` style comment is the last token of its line, whether its
    // boundaries are kept or not
    this->check("int a; // c",
                {{"int", 1u, 0u},
                 {"a", 1u, 4u},
                 {";", 1u, 5u},
                 {"// c", 1u, 7u}},
                true);
    this->check("int a; // c",
                {{"int", 1u, 0u},
                 {"a", 1u, 4u},
                 {";", 1u, 5u},
                 {"c", 1u, 10u}},
                false);
    // a comment which spans several lines: its offset is that of the delimiter
    // which opens it and the tokens of the line which closes it are those of
    // that line
    this->check("/* c\nd */ int a;",
                {{"/* c\nd */", 1u, 0u},
                 {"int", 2u, 5u},
                 {"a", 2u, 9u},
                 {";", 2u, 10u}},
                true);
    // a doxygen comment in a line continued by a backslash, which is how the
    // defect of issue #1407 was reported by the study of the `C++` files of the
    // project
    this->check("#define M(x) \\\n  /*! c */ x \\\n  y\n",
                {{"#", 1u, 0u},
                 {"define", 1u, 1u},
                 {"M", 1u, 8u},
                 {"(", 1u, 9u},
                 {"x", 1u, 10u},
                 {")", 1u, 11u},
                 {"\\", 1u, 13u},
                 {"/*! c */", 2u, 2u},
                 {"x", 2u, 11u},
                 {"\\", 2u, 13u},
                 {"y", 3u, 2u}},
                true);
  }  // end of checkOffsets
  /*!
   * \param[in] s: string to be parsed
   * \param[in] et: expected tokens, that is the value, the line and the offset
   * of each of them
   * \param[in] b: keep comment boundaries
   */
  void check(const char* const s,
             const std::vector<token>& et,
             const bool b) {
    tfel::utilities::CxxTokenizer tokenizer;
    tokenizer.keepCommentBoundaries(b);
    tokenizer.parseString(s);
    TFEL_TESTS_ASSERT(tokenizer.size() == et.size());
    if (tokenizer.size() != et.size()) {
      for (const auto& t : tokenizer) {
        std::cout << "'" << t.value << "' at line " << t.line << ", offset "
                  << t.offset << "\n";
      }
      return;
    }
    auto pt = tokenizer.begin();
    auto pet = et.begin();
    for (; pt != tokenizer.end(); ++pt, ++pet) {
      const auto& [v, l, o] = *pet;
      if ((pt->value != v) || (pt->line != l) || (pt->offset != o)) {
        std::cout << "'" << pt->value << "' at line " << pt->line
                  << ", offset " << pt->offset << "\nexpected '" << v
                  << "' at line " << l << ", offset " << o << "\n";
      }
      TFEL_TESTS_ASSERT(pt->value == v);
      TFEL_TESTS_ASSERT(pt->line == l);
      TFEL_TESTS_ASSERT(pt->offset == o);
    }
  }  // end of check
};

TFEL_TESTS_GENERATE_PROXY(CxxTokenizerKeepCommentBoundariesTest,
                          "CxxTokenizerKeepCommentBoundaries");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("CxxTokenizerKeepCommentBoundaries.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
