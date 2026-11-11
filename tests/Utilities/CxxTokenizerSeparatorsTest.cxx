/*!
 * \file   CxxTokenizerSeparatorsTest.cxx
 * \brief  This file checks the tokens which the additional separators of a
 * tfel::utilities::CxxTokenizer give, which the tokenizer searches in the part
 * of the line each token can span, see issue #1410. The tokens which are
 * checked here are those the tokenizer gives, whatever the way the separators
 * are searched, so that this test states the semantics the search has to keep.
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

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/CxxTokenizerOptions.hxx"
#include "TFEL/Utilities/Token.hxx"

struct CxxTokenizerSeparatorsTest final : public tfel::tests::TestCase {
  CxxTokenizerSeparatorsTest()
      : tfel::tests::TestCase("TFEL/Utilities", "CxxTokenizerSeparatorsTest") {
  }  // end of CxxTokenizerSeparatorsTest

  tfel::tests::TestResult execute() override {
    this->testWithoutAdditionalSeparators();
    this->testTheSeparatorsWhichAreConfigured();
    this->testTheEarliestSeparatorIsTaken();
    this->testTheSeparatorsWhichBeginWithASeparatorCharacter();
    this->testTheSeparatorsWhichSpanTheEndOfAToken();
    this->testTheSeparatorsInsideCommentsAndLiterals();
    this->testTheSeparatorsAndTheOtherOptions();
    this->testTheSeparatorsLineByLine();
    return this->result;
  }  // end of execute

 private:
  using Token = tfel::utilities::Token;
  using Tokenizer = tfel::utilities::CxxTokenizer;

  /*!
   * \brief return the letter which stands for the flag of a token
   * \param[in] t: token
   */
  static std::string flag_name(const Token& t) {
    switch (t.flag) {
      case Token::Standard:
        return "s";
      case Token::Comment:
        return "c";
      case Token::Number:
        return "n";
      case Token::DoxygenComment:
        return "d";
      case Token::DoxygenBackwardComment:
        return "b";
      case Token::String:
        return "S";
      case Token::Char:
        return "C";
      case Token::Preprocessor:
        return "p";
    }
    return "?";
  }  // end of flag_name

  /*!
   * \brief return a discription of the tokens of a tokenizer: the value of
   * each token followed by the letter of its flag, the tokens separated by a
   * space character
   * \param[in] t: tokenizer
   */
  static std::string describe(const Tokenizer& t) {
    auto r = std::string{};
    for (const auto& x : t) {
      if (!r.empty()) {
        r += ' ';
      }
      r += x.value + "(" + flag_name(x) + ")";
    }
    return r;
  }  // end of describe

  /*!
   * \brief tokenize the given contents with the given additional separators
   * and check the tokens they give
   * \param[in] seps: additional separators, see addSeparator
   * \param[in] contents: contents of the file
   * \param[in] expected: description of the expected tokens
   * \param[in] dot: boolean telling if the `.` character is a separator, see
   * treatDotAsSeparator
   * \param[in] pp: boolean telling if the preprocessor directives are treated,
   * see the treatPreprocessorDirectives option
   * \return true if the tokens are the expected ones
   */
  bool check(const std::vector<std::string>& seps,
             const std::string& contents,
             const std::string& expected,
             const bool dot = true,
             const bool pp = true) {
    auto o = tfel::utilities::CxxTokenizerOptions{};
    o.treatPreprocessorDirectives = pp;
    auto t = Tokenizer{o};
    for (const auto& s : seps) {
      t.addSeparator(s);
    }
    t.treatDotAsSeparator(dot);
    try {
      t.parseString(contents);
    } catch (const std::exception& e) {
      std::cout << "exception: " << e.what() << '\n';
      return false;
    }
    const auto r = describe(t);
    if (r != expected) {
      std::cout << "tokens: '" << r << "'\nexpected: '" << expected << "'\n";
      return false;
    }
    return true;
  }  // end of check

  /*!
   * \brief a tokenizer which uses no additional separator treats a character
   * which is neither a letter, nor a digit, nor one of the characters it deals
   * with by itself, as a character of the token it belongs to
   */
  void testWithoutAdditionalSeparators() {
    // the tensor product sign is a part of the token which precedes it
    TFEL_TESTS_ASSERT(this->check({}, "a\u2297b;", "a\u2297b(s) ;(s)"));
    // the same, with a separator which is not configured
    TFEL_TESTS_ASSERT(this->check({"\u2297"}, "a b c;", "a(s) b(s) c(s) ;(s)"));
  }  // end of testWithoutAdditionalSeparators

  /*!
   * \brief an additional separator splits the token it belongs to and is
   * itself a token of its own
   */
  void testTheSeparatorsWhichAreConfigured() {
    // the separator separates the two identifiers it is between
    TFEL_TESTS_ASSERT(
        this->check({"\u2297"}, "a\u2297b;", "a(s) \u2297(s) b(s) ;(s)"));
    // the same, when the separator is surrounded by spaces: it is the token
    // which precedes it which is split, and the spaces are ignored
    TFEL_TESTS_ASSERT(
        this->check({"\u2297"}, "a \u2297 b;", "a(s) \u2297(s) b(s) ;(s)"));
    // two separators in the same line
    TFEL_TESTS_ASSERT(this->check({"\u2297", "\u22C5"}, "a\u2297b\u22C5c;",
                                  "a(s) \u2297(s) b(s) \u22C5(s) c(s) ;(s)"));
    // a separator at the end of a line: the token which follows it is that of
    // the next line
    TFEL_TESTS_ASSERT(this->check({"\u2297"}, "a\u2297b;\nc\u2297d;\ne;\n",
                                  "a(s) \u2297(s) b(s) ;(s) c(s) \u2297(s) "
                                  "d(s) ;(s) e(s) ;(s)"));
    // a separator which is the last token of the file
    TFEL_TESTS_ASSERT(this->check({"\u2297"}, "a\u2297", "a(s) \u2297(s)"));
    // a separator which appears several times in a row
    TFEL_TESTS_ASSERT(this->check({"\u2297"}, "a\u2297\u2297\u2297b;",
                                  "a(s) \u2297(s) \u2297(s) \u2297(s) b(s) "
                                  ";(s)"));
    // a separator which appears twice, in the middle of a line
    TFEL_TESTS_ASSERT(this->check({"\u2297"}, "a b\u2297\u2297c d;",
                                  "a(s) b(s) \u2297(s) \u2297(s) c(s) d(s) "
                                  ";(s)"));
    // a line which holds no space nor separator character: the whole line is a
    // sequence of tokens all the same
    TFEL_TESTS_ASSERT(this->check({"\u2297"}, "aaa\u2297bbb\u2297ccc\u2297ddd",
                                  "aaa(s) \u2297(s) bbb(s) \u2297(s) ccc(s) "
                                  "\u2297(s) ddd(s)"));
    // a separator which is given twice: the second one brings nothing
    TFEL_TESTS_ASSERT(this->check({"\u2297", "\u2297"}, "a\u2297b;",
                                  "a(s) \u2297(s) b(s) ;(s)"));
    // a separator of two bytes, which is not a multibyte character of the same
    // first byte as another separator
    TFEL_TESTS_ASSERT(
        this->check({"\u00e9"}, "a\u00e9b;", "a(s) \u00e9(s) b(s) ;(s)"));
    // an empty line gives no token
    TFEL_TESTS_ASSERT(this->check({"\u2297"}, "\n\na;\n", "a(s) ;(s)"));
  }  // end of testTheSeparatorsWhichAreConfigured

  /*!
   * \brief when the line holds several separators, the one which appears
   * first is taken into account, whether it is the first one which has been
   * configured or not. When two separators begin at the same character, the one
   * which has been configured first is taken
   */
  void testTheEarliestSeparatorIsTaken() {
    // the scalar product sign is configured first but the tensor product sign
    // appears first in the line
    TFEL_TESTS_ASSERT(this->check({"\u22C5", "\u2297"}, "a\u2297b\u22C5c;",
                                  "a(s) \u2297(s) b(s) \u22C5(s) c(s) ;(s)"));
    // the two separators begin at the same character: the longest one is taken
    // because it has been configured first
    TFEL_TESTS_ASSERT(this->check({"\u2297\u22C5", "\u2297"}, "a\u2297\u22C5b;",
                                  "a(s) \u2297\u22C5(s) b(s) ;(s)"));
    // the two separators begin at the same character: the shortest one is taken
    // because it has been configured first, and the characters of the longest
    // one which are not a separator of their own build the token which follows
    TFEL_TESTS_ASSERT(this->check({"\u2297", "\u2297\u22C5"}, "a\u2297\u22C5b;",
                                  "a(s) \u2297(s) \u22C5b(s) ;(s)"));
  }  // end of testTheEarliestSeparatorIsTaken

  /*!
   * \brief a separator which begins with one of the characters the tokenizer
   * already treats as a separator cannot be found, since the token which is
   * built ends at that character
   */
  void testTheSeparatorsWhichBeginWithASeparatorCharacter() {
    // the `=` character is a separator of its own, so the two characters of the
    // `==` separator are dealt with by the tokenizer itself
    TFEL_TESTS_ASSERT(this->check({"=="}, "a==b;", "a(s) ==(s) b(s) ;(s)"));
    // the same, with the one character separator configured after the two
    // characters one
    TFEL_TESTS_ASSERT(
        this->check({"==", "="}, "a==b;", "a(s) ==(s) b(s) ;(s)"));
    // the same: the arrow is dealt with by the tokenizer, which gives the two
    // tokens it builds from the three characters of the separator
    TFEL_TESTS_ASSERT(
        this->check({"-->"}, "a-->b;", "a(s) --(s) >(s) b(s) ;(s)"));
    // a separator which begins with a space character is never found: the
    // spaces are ignored before a token is built
    TFEL_TESTS_ASSERT(
        this->check({" \u2297 "}, "a \u2297 b;", "a(s) \u2297(s) b(s) ;(s)"));
    // the same, which shows that it is the separator which is not taken into
    // account, as the tensor product sign alone would have split the token
    TFEL_TESTS_ASSERT(
        this->check({" \u2297 "}, "a\u2297b;", "a\u2297b(s) ;(s)"));
  }  // end of testTheSeparatorsWhichBeginWithASeparatorCharacter

  /*!
   * \brief a separator which begins before the end of the token which is built
   * is taken into account, even if it ends after it
   */
  void testTheSeparatorsWhichSpanTheEndOfAToken() {
    // the separator begins with the character which ends the token and ends
    // with a character which is a separator of its own
    TFEL_TESTS_ASSERT(this->check({"\u2297="}, "\u20ac\u2297=b;",
                                  "\u20ac(s) \u2297=(s) b(s) ;(s)"));
    // the same, when the last character of the separator is not that which ends
    // the token: the separator is not found and the characters it begins with
    // are those of the token
    TFEL_TESTS_ASSERT(this->check({"\u2297="}, "\u20ac\u2297; b;",
                                  "\u20ac\u2297(s) ;(s) b(s) ;(s)"));
    // a separator which is longer than the token it belongs to
    const auto long_separator = std::string(30u, 'a') + "X";
    TFEL_TESTS_ASSERT(this->check(
        {long_separator}, std::string{"\u20ac"} + std::string(30u, 'a') + "Xb;",
        "\u20ac(s) " + long_separator + "(s) b(s) ;(s)"));
    // the same, when the line holds one character less: the separator is not
    // found
    TFEL_TESTS_ASSERT(this->check(
        {long_separator}, std::string{"\u20ac"} + std::string(29u, 'a') + "Xb;",
        "\u20ac" + std::string(29u, 'a') + "Xb(s) ;(s)"));
    // a separator which is a sequence of letters: the letters which follow it
    // build the token which follows it, as they are not a part of it
    TFEL_TESTS_ASSERT(
        this->check({"int"}, "\u20acint x;", "\u20ac(s) int(s) x(s) ;(s)"));
  }  // end of testTheSeparatorsWhichSpanTheEndOfAToken

  /*!
   * \brief the separators which are held by a comment or by a literal are not
   * taken into account: they are a part of the token which holds them
   */
  void testTheSeparatorsInsideCommentsAndLiterals() {
    // the two comments of the line hold the separator, which is then a part of
    // their contents
    TFEL_TESTS_ASSERT(this->check({"\u2297"}, "a /* \u2297 */ b; // \u2297\n",
                                  "a(s) \u2297(c) b(s) ;(s) \u2297(c)"));
    // the separator held by a string literal
    TFEL_TESTS_ASSERT(this->check({"\u2297"}, "const char* s = \"\u2297\";\n",
                                  "const(s) char(s) *(s) s(s) =(s) "
                                  "\"\u2297\"(S) ;(s)"));
    // the separator held by a raw string literal
    TFEL_TESTS_ASSERT(this->check({"\u2297"}, "x = R\"(a\u2297b)\" ;\n",
                                  "x(s) =(s) R\"(a\u2297b)\"(S) ;(s)"));
    // the separators which surround a string literal
    TFEL_TESTS_ASSERT(this->check({"\u2297"}, "x \u2297 \"a\\\"b\" \u2297 y;",
                                  "x(s) \u2297(s) \"a\\\"b\"(S) \u2297(s) y(s) "
                                  ";(s)"));
  }  // end of testTheSeparatorsInsideCommentsAndLiterals

  /*!
   * \brief the separators are dealt with whatever the other options of the
   * tokenizer are
   */
  void testTheSeparatorsAndTheOtherOptions() {
    // the numbers which surround a separator are numbers of their own
    TFEL_TESTS_ASSERT(
        this->check({"\u2297"}, "1.5e3\u22972\u2297x;",
                    "1.5e3(n) \u2297(s) 2(n) \u2297(s) x(s) ;(s)"));
    // a preprocessor directive which holds a separator: the directive itself is
    // made of the tokens the tokenizer deals with, the rest of the line is
    // treated as a standard line
    TFEL_TESTS_ASSERT(this->check({"\u2297"}, "#define A b\u2297c\n",
                                  "#(p) define(p) A(s) b(s) \u2297(s) c(s)"));
    // the same, when the preprocessor directives are not treated
    TFEL_TESTS_ASSERT(this->check({"\u2297"}, "#define A b\u2297c\n",
                                  "#(s) define(s) A(s) b(s) \u2297(s) c(s)",
                                  true, false));
    // the `.` character is a separator: it ends the token which precedes the
    // separator
    TFEL_TESTS_ASSERT(this->check({"\u2297"}, "a.b\u2297c;",
                                  "a(s) .(s) b(s) \u2297(s) c(s) ;(s)"));
    // the `.` character is not a separator: it is a part of the token which
    // precedes the separator
    TFEL_TESTS_ASSERT(this->check({"\u2297"}, "a.b\u2297c;",
                                  "a.b(s) \u2297(s) c(s) ;(s)", false));
    // the two separators of the `MFront` grammar, in a line of the shape those
    // of the `MFront` files have
    TFEL_TESTS_ASSERT(this->check(
        {"\u2297", "\u22C5"}, "sigmas = ids |\u2297 (ids | T * strain_rate);",
        "sigmas(s) =(s) ids(s) |(s) \u2297(s) ((s) ids(s) |(s) T(s) *(s) "
        "strain_rate(s) )(s) ;(s)"));
    // the same separators, in a line which holds the characters the tokenizer
    // deals with by itself
    TFEL_TESTS_ASSERT(this->check({"\u2297", "\u22C5"},
                                  "auto a = b \u2297 c; auto d = e\u22C5f;\n",
                                  "auto(s) a(s) =(s) b(s) \u2297(s) c(s) "
                                  ";(s) auto(s) d(s) =(s) e(s) \u22C5(s) f(s) "
                                  ";(s)"));
  }  // end of testTheSeparatorsAndTheOtherOptions

  /*!
   * \brief the separators are searched in a line of the file, so that
   * tokenizing a contents with a single call to the parseString method or line
   * by line, as a caller which reads a file line by line does, gives the same
   * tokens
   */
  void testTheSeparatorsLineByLine() {
    const auto contents = std::string{"a\u2297b;\nc\u2297d;\n//! e\nf;\n"};
    auto t = Tokenizer{};
    t.addSeparator("\u2297");
    t.parseString(contents);
    const auto expected = describe(t);
    auto u = Tokenizer{};
    u.addSeparator("\u2297");
    u.parseString("a\u2297b;\n");
    u.parseString("c\u2297d;\n");
    u.parseString("//! e\n");
    u.parseString("f;\n");
    TFEL_TESTS_ASSERT(describe(u) == expected);
    TFEL_TESTS_ASSERT(expected ==
                      "a(s) \u2297(s) b(s) ;(s) c(s) \u2297(s) "
                      "d(s) ;(s) e(d) f(s) ;(s)");
  }  // end of testTheSeparatorsLineByLine
};   // end of struct CxxTokenizerSeparatorsTest

TFEL_TESTS_GENERATE_PROXY(CxxTokenizerSeparatorsTest,
                          "CxxTokenizerSeparatorsTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("CxxTokenizerSeparatorsTest.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
