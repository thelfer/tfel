/*!
 * \file   CxxTokenizerContractTest.cxx
 * \brief  This file checks the contract of the tokens given by the
 * tfel::utilities::CxxTokenizer class for the lexical constructs which are not
 * covered by the other tests of the tokenizer: raw string literals, empty
 * comments, comments of a line and numbers with digit separators. Those
 * constructs have been pointed out by the libFuzzer analysis of the tokenizer
 * performed for issue #1387.
 * \author Thomas Helfer
 * \date   11/11/2026
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

#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <map>
#include <string>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/Token.hxx"

struct CxxTokenizerContractTest final : public tfel::tests::TestCase {
  CxxTokenizerContractTest()
      : tfel::tests::TestCase("TFEL/Utilities", "CxxTokenizerContractTest") {
  }  // end of CxxTokenizerContractTest

  tfel::tests::TestResult execute() override {
    this->testRawStrings();
    this->testEmptyComments();
    this->testCommentsOfALine();
    this->testNumbersWithDigitSeparators();
    return this->result;
  }  // end of execute

 private:
  using Token = tfel::utilities::Token;

  /*!
   * \brief a tokenizer which gives an access to the comments of the lines,
   * which is a protected data member of tfel::utilities::CxxTokenizer
   */
  struct Tokenizer : public tfel::utilities::CxxTokenizer {
    using tfel::utilities::CxxTokenizer::CxxTokenizer;
    //! \return the comments of the lines of the tokenized file
    const std::map<Token::size_type, std::string>& getComments() const {
      return this->comments;
    }
  };  // end of struct Tokenizer

  /*!
   * \brief raw string literals.
   *
   * The value of a raw string token is the contents of the literal: the
   * `R"delim(` prefix and the `)delim"` suffix are not included, so the value
   * of a raw string token does not follow the convention of the value of a
   * string token, which includes its quotes. The consumers of the tokens which
   * remove the quotes of a string token shall then not rely on the flag of the
   * token only. See issue #1387.
   */
  void testRawStrings() {
    using tfel::utilities::Token;
    auto t = tfel::utilities::CxxTokenizer{};
    // a raw string literal with no delimiter
    t.parseString("R\"(text)\"");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::String);
    TFEL_TESTS_ASSERT(t[0].value == "text");
    // the offset of the token is that of its contents, not that of the literal
    TFEL_TESTS_ASSERT(t[0].line == 1u);
    TFEL_TESTS_ASSERT(t[0].offset == 3u);
    // a raw string literal with a delimiter
    t.clear();
    t.parseString("R\"del(text)del\"");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::String);
    TFEL_TESTS_ASSERT(t[0].value == "text");
    TFEL_TESTS_ASSERT(t[0].offset == 6u);
    // the escape sequences of a raw string literal are not interpreted
    t.clear();
    t.parseString("s = R\"(a\\nb)\";");
    TFEL_TESTS_ASSERT(t.size() == 4u);
    TFEL_TESTS_ASSERT(t[2].flag == Token::String);
    TFEL_TESTS_ASSERT(t[2].value == "a\\nb");
    TFEL_TESTS_ASSERT(t[2].offset == 7u);
    TFEL_TESTS_ASSERT(t[3].value == ";");
    TFEL_TESTS_ASSERT(t[3].offset == 13u);
    // a raw string literal spanning several lines: the value of the token
    // gathers the lines it spans and the tokens which follow belong to the last
    // line of the literal
    t.clear();
    t.parseString("s = R\"(line1\nline2)\";\n");
    TFEL_TESTS_ASSERT(t.size() == 4u);
    TFEL_TESTS_ASSERT(t[2].flag == Token::String);
    TFEL_TESTS_ASSERT(t[2].value == "line1\nline2");
    TFEL_TESTS_ASSERT(t[2].line == 1u);
    TFEL_TESTS_ASSERT(t[2].offset == 7u);
    TFEL_TESTS_ASSERT(t[3].value == ";");
    TFEL_TESTS_ASSERT(t[3].line == 2u);
    TFEL_TESTS_ASSERT(t[3].offset == 7u);
    // a raw string literal with an empty contents gives a token with an empty
    // value, which is a known defect reported in issue #1387
    t.clear();
    t.parseString("R\"x()x\"");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::String);
    TFEL_TESTS_ASSERT(t[0].value.empty());
    // the state of the tokenizer when a raw string literal is not closed: the
    // tokenizer is designed so that a file may be tokenized line by line
    t.clear();
    t.parseString("R\"x(unterminated\n");
    TFEL_TESTS_ASSERT(t.isRawStringOpened());
    TFEL_TESTS_ASSERT(t.getCurrentRawStringDelimiter() == "x");
    t.clear();
    t.parseString("R\"(unterminated\n");
    TFEL_TESTS_ASSERT(t.isRawStringOpened());
    TFEL_TESTS_ASSERT(t.getCurrentRawStringDelimiter().empty());
    // the end of a raw string literal alone in a line is rejected
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString("b)x\";"), std::runtime_error);
    // a raw string literal which is not closed is accepted, since the tokenizer
    // is designed so that a file may be tokenized line by line: the caller is
    // responsible for checking that the tokenizer is not in such a state at the
    // end of the file. Contrary to a raw string literal, a character literal or
    // a quoted string which is not closed in its line is rejected
    t.clear();
    t.parseString("s = R\"(text\n");
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t[2].flag == Token::String);
    // the value of the token is the contents of the literal: the end of line
    // character which terminates the line is not a part of it, see issue #1396
    TFEL_TESTS_ASSERT(t[2].value == "text");
    TFEL_TESTS_ASSERT(t.isRawStringOpened());
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString("s = \"text\n"), std::runtime_error);
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString("s = 'a\n"), std::runtime_error);
  }  // end of testRawStrings

  /*!
   * \brief empty comments.
   *
   * An empty comment gives a comment token whose value is empty and whose
   * offset is the size of the line, that is one past the end of the line. Those
   * two behaviours are known defects reported in issue #1387: a token with an
   * empty value surprises the consumers of the tokens, which generally assume
   * that the value of a token is not empty.
   */
  void testEmptyComments() {
    using tfel::utilities::Token;
    auto t = tfel::utilities::CxxTokenizer{};
    // an empty C++ style comment
    t.parseString("a; //");
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t[2].flag == Token::Comment);
    TFEL_TESTS_ASSERT(t[2].value.empty());
    TFEL_TESTS_ASSERT(t[2].offset == 5u);
    // an empty C style comment
    t.clear();
    t.parseString("a; /**/");
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t[2].flag == Token::Comment);
    TFEL_TESTS_ASSERT(t[2].value.empty());
    TFEL_TESTS_ASSERT(t[2].offset == 5u);
    // an empty doxygen comment
    t.clear();
    t.parseString("a; /*!*/");
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t[2].flag == Token::DoxygenComment);
    TFEL_TESTS_ASSERT(t[2].value.empty());
    TFEL_TESTS_ASSERT(t[2].offset == 6u);
    // the spaces which follow an empty C++ style comment are ignored
    t.clear();
    t.parseString("  //   ");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::Comment);
    TFEL_TESTS_ASSERT(t[0].value.empty());
    TFEL_TESTS_ASSERT(t[0].offset == 7u);
    // an empty comment is removed by stripComments as any other comment
    t.clear();
    t.parseString("a; //\nb; /**/\n");
    TFEL_TESTS_ASSERT(t.size() == 6u);
    t.stripComments();
    TFEL_TESTS_ASSERT(t.size() == 4u);
    TFEL_TESTS_ASSERT(t[1].value == ";");
    TFEL_TESTS_ASSERT(t[1].line == 1u);
    TFEL_TESTS_ASSERT(t[3].value == ";");
    TFEL_TESTS_ASSERT(t[3].line == 2u);
    // a comment whose contents is made of spaces only is an empty comment too
    t.clear();
    t.parseString("a; /*   */");
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t[2].flag == Token::Comment);
    TFEL_TESTS_ASSERT(t[2].value.empty());
  }  // end of testEmptyComments

  /*!
   * \brief comments of a line, which are gathered by stripComments in the
   * `comments` data member of the tokenizer.
   *
   * Only the last comment of a line is kept, whatever the number of comments
   * this line contains, which is a known defect reported in issue #1387.
   */
  void testCommentsOfALine() {
    using tfel::utilities::Token;
    auto t = Tokenizer{};
    // a single comment in a line
    t.parseString("/* c */ int a;\n");
    TFEL_TESTS_ASSERT(t.size() == 4u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::Comment);
    TFEL_TESTS_ASSERT(t[0].value == "c");
    t.stripComments();
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t.getComments().size() == 1u);
    TFEL_TESTS_ASSERT(t.getComments().at(1u) == "c");
    // two comments in a line: the tokens are both removed but only the last
    // comment is kept in the comments of the line
    t.clear();
    t.parseString("/* c1 */ int a; /* c2 */");
    TFEL_TESTS_ASSERT(t.size() == 5u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::Comment);
    TFEL_TESTS_ASSERT(t[0].value == "c1");
    TFEL_TESTS_ASSERT(t[0].offset == 3u);
    TFEL_TESTS_ASSERT(t[4].flag == Token::Comment);
    TFEL_TESTS_ASSERT(t[4].value == "c2");
    TFEL_TESTS_ASSERT(t[4].offset == 19u);
    t.stripComments();
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t[0].value == "int");
    TFEL_TESTS_ASSERT(t[0].offset == 9u);
    TFEL_TESTS_ASSERT(t[2].value == ";");
    TFEL_TESTS_ASSERT(t.getComments().size() == 1u);
    TFEL_TESTS_ASSERT(t.getComments().at(1u) == "c2");
  }  // end of testCommentsOfALine

  /*!
   * \brief numbers with digit separators.
   *
   * The digit separators of the `C++14` literals are removed from the value of
   * the token, whereas the offsets of the tokens are those of the file: the
   * value of a number token is thus not the spelling of this number in the
   * file, which is documented in issue #1387.
   */
  void testNumbersWithDigitSeparators() {
    using tfel::utilities::Token;
    auto t = tfel::utilities::CxxTokenizer{};
    t.parseString("1'000");
    TFEL_TESTS_ASSERT(t.size() == 1u);
    TFEL_TESTS_ASSERT(t[0].flag == Token::Number);
    TFEL_TESTS_ASSERT(t[0].value == "1000");
    TFEL_TESTS_ASSERT(t[0].offset == 0u);
    // the offsets of the tokens which follow a number with a digit separator
    // are those of the file: the digit separators are not taken into account
    t.clear();
    t.parseString("@IterMax 1'000; tfel::raise();");
    TFEL_TESTS_ASSERT(t.size() == 9u);
    TFEL_TESTS_ASSERT(t[1].flag == Token::Number);
    TFEL_TESTS_ASSERT(t[1].value == "1000");
    TFEL_TESTS_ASSERT(t[1].offset == 9u);
    TFEL_TESTS_ASSERT(t[2].value == ";");
    TFEL_TESTS_ASSERT(t[2].offset == 14u);
    TFEL_TESTS_ASSERT(t[3].value == "tfel");
    TFEL_TESTS_ASSERT(t[3].offset == 16u);
    // the digit separators are not supported in the hexadecimal literals
    t.clear();
    TFEL_TESTS_CHECK_THROW(t.parseString("0x1'fp3"), std::runtime_error);
  }  // end of testNumbersWithDigitSeparators
};   // end of struct CxxTokenizerContractTest

TFEL_TESTS_GENERATE_PROXY(CxxTokenizerContractTest, "CxxTokenizerContractTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("CxxTokenizerContractTest.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
