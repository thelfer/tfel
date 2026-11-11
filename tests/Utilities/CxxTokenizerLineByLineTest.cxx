/*!
 * \file   CxxTokenizerLineByLineTest.cxx
 * \brief  This file checks that tokenizing a file line by line, with several
 * calls to the parseString method, gives the same tokens as tokenizing its
 * whole contents with a single call, see issue #1396. \author Thomas Helfer
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
#include <string>
#include <vector>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/Token.hxx"

struct CxxTokenizerLineByLineTest final : public tfel::tests::TestCase {
  CxxTokenizerLineByLineTest()
      : tfel::tests::TestCase("TFEL/Utilities", "CxxTokenizerLineByLineTest") {
  }  // end of CxxTokenizerLineByLineTest

  tfel::tests::TestResult execute() override {
    this->testStandardLines();
    this->testMultiLineComments();
    this->testMultiLineRawStrings();
    this->testUnterminatedTokens();
    return this->result;
  }  // end of execute

 private:
  using Token = tfel::utilities::Token;
  using Tokens = std::vector<Token>;

  /*!
   * \brief tokenize a whole contents with a single call to parseString
   * \param[in] contents: contents to tokenize
   * \return the tokens of the contents
   */
  static Tokens tokenize(const std::string& contents) {
    auto t = tfel::utilities::CxxTokenizer{};
    t.parseString(contents);
    return Tokens{t.begin(), t.end()};
  }  // end of tokenize

  /*!
   * \brief split a contents in lines, keeping the end of line characters, as a
   * caller which reads a file line by line would do
   * \param[in] contents: contents to split
   * \return the lines of the contents
   */
  static std::vector<std::string> getLines(const std::string& contents) {
    auto lines = std::vector<std::string>{};
    auto p = std::size_t{};
    for (;;) {
      const auto q = contents.find('\n', p);
      if (q == std::string::npos) {
        if (p != contents.size()) {
          lines.push_back(contents.substr(p));
        }
        return lines;
      }
      lines.push_back(contents.substr(p, q - p + 1u));
      p = q + 1u;
    }
  }  // end of getLines

  /*!
   * \brief tokenize a contents line by line, with a call to parseString per
   * line \param[in] contents: contents to tokenize \return the tokens of the
   * contents
   */
  static Tokens tokenizeLineByLine(const std::string& contents) {
    auto t = tfel::utilities::CxxTokenizer{};
    for (const auto& line : getLines(contents)) {
      t.parseString(line);
    }
    return Tokens{t.begin(), t.end()};
  }  // end of tokenizeLineByLine

  /*!
   * \brief describe the flags, offsets and values of the given tokens
   * \param[in] tokens: tokens to describe
   * \return a discription of the tokens, used in the report of the test
   */
  static std::string describe(const Tokens& tokens) {
    auto description = std::string{};
    for (const auto& token : tokens) {
      description += "{" + std::to_string(int{token.flag}) + "," +
                     std::to_string(token.offset) + ",<" + token.value + ">}";
    }
    return description;
  }  // end of describe

  /*!
   * \brief check that a contents gives the same tokens tokenized as a whole and
   * tokenized line by line
   *
   * The lines of the tokens are not compared since the line numbers restart at
   * the first line for each call to the parseString method: only the flags, the
   * offsets within the lines and the values are compared.
   *
   * \param[in] contents: contents to tokenize
   */
  void checkEquivalence(const std::string& contents) {
    const auto whole = tokenize(contents);
    const auto lines = tokenizeLineByLine(contents);
    TFEL_TESTS_ASSERT(whole.size() == lines.size());
    if (whole.size() != lines.size()) {
      std::cerr << "for contents <" << contents
                << ">\n  as a whole    : " << describe(whole)
                << "\n  line by line  : " << describe(lines) << "\n";
      return;
    }
    for (std::size_t i = 0u, m = whole.size(); i != m; ++i) {
      TFEL_TESTS_ASSERT(whole[i].flag == lines[i].flag);
      TFEL_TESTS_ASSERT(whole[i].offset == lines[i].offset);
      TFEL_TESTS_ASSERT(whole[i].value == lines[i].value);
    }
  }  // end of checkEquivalence

  //! \brief standard tokens, blank lines and lines not terminated by an end of
  //! line character
  void testStandardLines() {
    this->checkEquivalence("int a;\nint b;\n");
    // the last line of a file may not be terminated by an end of line
    // character: such a line is tokenized once, not twice
    this->checkEquivalence("int a;\nint b;");
    // a blank line, which gives no token, is a line as the others
    this->checkEquivalence("int a;\n\n\nint b;\n");
    // the number of tokens of a file whose last line is not terminated is that
    // of the tokens of its lines
    const auto t = tokenize("int a;\nint b;");
    TFEL_TESTS_ASSERT(t.size() == 6u);
    TFEL_TESTS_ASSERT(t[3].value == "int");
    TFEL_TESTS_ASSERT(t[5].value == ";");
  }  // end of testStandardLines

  //! \brief comments which span several lines
  void testMultiLineComments() {
    this->checkEquivalence("/* c1 */ int a;\n");
    this->checkEquivalence("/* c1\nc2 */ int a;\n");
    // the spaces which precede the end of a comment spanning several lines are
    // kept, whereas those which precede the end of the first line of such a
    // comment are not
    {
      const auto tokens = tokenize("/* c1\nc2 */ int a;\n");
      TFEL_TESTS_ASSERT(tokens.size() == 4u);
      TFEL_TESTS_ASSERT(tokens[0].flag == Token::Comment);
      TFEL_TESTS_ASSERT(tokens[0].value == "c1\nc2 ");
    }
    // a blank line inside a comment is kept as an end of line character
    this->checkEquivalence("/* c1\n\nc2 */ int a;\n");
    // the same for a C++ style comment, which cannot span several lines
    this->checkEquivalence("int a;  // c1\nint b;  // c2\n");
  }  // end of testMultiLineComments

  //! \brief raw string literals which span several lines
  void testMultiLineRawStrings() {
    this->checkEquivalence("s = R\"(line1\nline2)\";\n");
    this->checkEquivalence("s = R\"delim(line1\nline2)delim\";\n");
    this->checkEquivalence("s = R\"(a\n\nb)\";\n");
    {
      const auto tokens = tokenize("s = R\"(line1\nline2)\";\n");
      TFEL_TESTS_ASSERT(tokens.size() == 4u);
      TFEL_TESTS_ASSERT(tokens[2].flag == Token::String);
      TFEL_TESTS_ASSERT(tokens[2].value == "line1\nline2");
      // the tokens which follow a raw string literal belong to its last line
      TFEL_TESTS_ASSERT(tokens[3].line == 2u);
    }
  }  // end of testMultiLineRawStrings

  /*!
   * \brief a comment or a raw string literal which is not closed at the end of
   * a file is left unterminated, without an end of line character which does
   * not belong to the file
   */
  void testUnterminatedTokens() {
    this->checkEquivalence("int a;\n/* c1\nc2\n");
    this->checkEquivalence("int a;\ns = R\"(line1\nline2\n");
    {
      const auto tokens = tokenize("/* c1\n");
      TFEL_TESTS_ASSERT(tokens.size() == 1u);
      TFEL_TESTS_ASSERT(tokens[0].value == "c1");
    }
    {
      const auto tokens = tokenize("/* c1");
      TFEL_TESTS_ASSERT(tokens.size() == 1u);
      TFEL_TESTS_ASSERT(tokens[0].value == "c1");
    }
  }  // end of testUnterminatedTokens
};   // end of struct CxxTokenizerLineByLineTest

TFEL_TESTS_GENERATE_PROXY(CxxTokenizerLineByLineTest,
                          "CxxTokenizerLineByLineTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("CxxTokenizerLineByLineTest.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
