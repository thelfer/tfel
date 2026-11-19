/*!
 * \file   CxxTokenizerPunctuatorsTest.cxx
 * \brief  This file checks the tokens of the punctuators of `C++`, which are
 * the sequences of characters of the source file which are not identifiers,
 * numbers, literals or comments. A punctuator is the longest such sequence, as
 * the `C++` grammar is maximal munch, so that `<<=` is a single token and not
 * the `<<` punctuator followed by the `=` one. Those constructs have been
 * pointed out while checking the tokenizer against the grammar of `C++`, see
 * issue #1460.
 * \author Thomas Helfer
 * \date   19/11/2026
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
#include "TFEL/Utilities/CxxTokenizerOptions.hxx"
#include "TFEL/Utilities/Token.hxx"

struct CxxTokenizerPunctuatorsTest final : public tfel::tests::TestCase {
  CxxTokenizerPunctuatorsTest()
      : tfel::tests::TestCase("TFEL/Utilities", "CxxTokenizerPunctuatorsTest") {
  }  // end of CxxTokenizerPunctuatorsTest

  tfel::tests::TestResult execute() override {
    this->testAssignmentOperators();
    this->testComparisonOperators();
    this->testMemberAccessOperators();
    this->testEllipsis();
    this->testTilde();
    this->testTwoCharactersSeparatorsNotJoined();
    this->testTokenPasting();
    return this->result;
  }  // end of execute

 private:
  using Token = tfel::utilities::Token;

  /*!
   * \brief tokenize a line and check the values of the tokens it gives
   * \param[in] s:  the line to tokenize
   * \param[in] v:  the values of the expected tokens
   * \param[in] o:  the options of the tokenizer
   */
  void check(const std::string& s,
             const std::vector<std::string>& v,
             const tfel::utilities::CxxTokenizerOptions& o =
                 tfel::utilities::CxxTokenizerOptions{}) {
    auto t = tfel::utilities::CxxTokenizer{o};
    t.parseString(s);
    if (t.size() != v.size()) {
      std::cout << "'" << s << "' gives " << t.size() << " tokens [";
      for (const auto& tk : t) {
        std::cout << tk.value << "][";
      }
      std::cout << "] instead of " << v.size() << " tokens" << std::endl;
    }
    TFEL_TESTS_ASSERT(t.size() == v.size());
    for (std::vector<std::string>::size_type i = 0u; i != v.size(); ++i) {
      if (t[i].value != v[i]) {
        std::cout << "token " << i << " of '" << s << "' is '" << t[i].value
                  << "' and not '" << v[i] << "'" << std::endl;
      }
      TFEL_TESTS_CHECK_EQUAL(t[i].value, v[i]);
    }
  }  // end of check

  /*!
   * \brief the assignment operators, which are compound assignment operators
   * when the operator of the assignment is that of an arithmetic or a bitwise
   * operation. The `=` sign which follows such an operator is a part of it
   * although it may be the beginning of another token, as in `a+=b` where `+=`
   * is the addition assignment operator and not the `+` punctuator followed by
   * the `=` one
   */
  void testAssignmentOperators() {
    this->check("a=b", {"a", "=", "b"});
    this->check("a+=b", {"a", "+=", "b"});
    this->check("a-=b", {"a", "-=", "b"});
    this->check("a*=b", {"a", "*=", "b"});
    this->check("a/=b", {"a", "/=", "b"});
    this->check("a%=b", {"a", "%=", "b"});
    this->check("a&=b", {"a", "&=", "b"});
    this->check("a^=b", {"a", "^=", "b"});
    this->check("a|=b", {"a", "|=", "b"});
    // the shift assignment operators, which are three characters long: `<<=`
    // shall not be split in `<<` and `=`, as `>>=` shall not be split in `>>`
    // and `=`
    this->check("a<<=b", {"a", "<<=", "b"});
    this->check("a>>=b", {"a", ">>=", "b"});
    // an assignment operator which is not the longest sequence of characters
    // which builds a punctuator is split in several tokens
    this->check("a<< = b", {"a", "<<", "=", "b"});
    this->check("a >> = b", {"a", ">>", "=", "b"});
    // `&&=` is not a punctuator of `C++`: the logical and operator is followed
    // by the assignment one
    this->check("a&&=b", {"a", "&&", "=", "b"});
    // the increment and decrement operators
    this->check("a++", {"a", "++"});
    this->check("--a", {"--", "a"});
    this->check("a+++b", {"a", "++", "+", "b"});
  }  // end of testAssignmentOperators

  /*!
   * \brief the comparison operators, among which the three way comparison
   * operator of the `C++20` standard, `<=>`, which is the longest sequence of
   * characters which builds a punctuator although that punctuator is only
   * defined by the recent versions of the standard: a `C++17` compiler reads
   * `a<=>b` as the `a`, `<=>` and `b` tokens too and complains about the `>`
   * punctuator, which it cannot give a meaning to
   */
  void testComparisonOperators() {
    this->check("a==b", {"a", "==", "b"});
    this->check("a!=b", {"a", "!=", "b"});
    this->check("a<b", {"a", "<", "b"});
    this->check("a<=b", {"a", "<=", "b"});
    this->check("a>b", {"a", ">", "b"});
    this->check("a>=b", {"a", ">=", "b"});
    this->check("a<=>b", {"a", "<=>", "b"});
    this->check("x = a<=>0;", {"x", "=", "a", "<=>", "0", ";"});
    // a closing parenthesis or a space between the `<=` punctuator and the `>`
    // one prevents `<=>` from being read
    this->check("f(a<=c)>d", {"f", "(", "a", "<=", "c", ")", ">", "d"});
    this->check("a<= > b", {"a", "<=", ">", "b"});
    // the logical operators
    this->check("a&&b", {"a", "&&", "b"});
    this->check("a||b", {"a", "||", "b"});
    this->check("!a", {"!", "a"});
    this->check("a&b", {"a", "&", "b"});
    this->check("a|b", {"a", "|", "b"});
    this->check("a^b", {"a", "^", "b"});
  }  // end of testComparisonOperators

  /*!
   * \brief the punctuators which are several characters long and are not
   * operators of an arithmetic, bitwise or comparison operation
   */
  void testMemberAccessOperators() {
    this->check("a->b", {"a", "->", "b"});
    this->check("a->*b", {"a", "->*", "b"});
    this->check("a.*b", {"a", ".*", "b"});
    this->check("a::b", {"a", "::", "b"});
    this->check("std::vector<int> v;",
                {"std", "::", "vector", "<", "int", ">", "v", ";"});
    // the arrow operator is not split although the `-` punctuator may introduce
    // a number with a leading sign
    this->check("x=-1;->b", {"x", "=", "-1", ";", "->", "b"});
  }  // end of testMemberAccessOperators

  /*!
   * \brief the ellipsis, which is the longest of the punctuators made of dots
   * and which is followed in the list of candidates by the `..` separator
   * specific to `MFront`
   */
  void testEllipsis() {
    this->check("int f(int,...);",
                {"int", "f", "(", "int", ",", "...", ")", ";"});
    this->check("f(args...)", {"f", "(", "args", "...", ")"});
    this->check("a...b", {"a", "...", "b"});
    // the range separator of `MFront` and the member access one are preserved
    this->check("a..b", {"a", "..", "b"});
    this->check("a.b", {"a", ".", "b"});
    this->check("a . . b", {"a", ".", ".", "b"});
    // a number followed by a range separator is rejected since `1..2` is not a
    // sequence of tokens of `C++`: the maximal munch of the grammar builds the
    // number `1.`, which the `.` punctuator may not follow
    auto t = tfel::utilities::CxxTokenizer{};
    TFEL_TESTS_CHECK_THROW(t.parseString("1..2"), std::runtime_error);
  }  // end of testEllipsis

  /*!
   * \brief the `~` punctuator, which is a separator of the tokenizer as the
   * `!` or `&` ones are: it belongs to no identifier, so that `~y` gives two
   * tokens whereas the tokenizer used to give the `~y` identifier
   */
  void testTilde() {
    this->check("x=~y;", {"x", "=", "~", "y", ";"});
    this->check("a~=b", {"a", "~", "=", "b"});
    this->check("m = ~0u;", {"m", "=", "~", "0u", ";"});
    this->check("~Class() = default;",
                {"~", "Class", "(", ")", "=", "default", ";"});
  }  // end of testTilde

  /*!
   * \brief when the two characters separators are not joined, the punctuators
   * of several characters are split in as many tokens as they hold characters
   */
  void testTwoCharactersSeparatorsNotJoined() {
    auto o = tfel::utilities::CxxTokenizerOptions{};
    o.joinCxxTwoCharactersSeparators = false;
    this->check("a<<=b", {"a", "<", "<", "=", "b"}, o);
    this->check("a<=>b", {"a", "<", "=", ">", "b"}, o);
    this->check("a&&b", {"a", "&", "&", "b"}, o);
    this->check("a&=b", {"a", "&", "=", "b"}, o);
    this->check("a..b", {"a", ".", ".", "b"}, o);
    this->check("a...b", {"a", ".", ".", ".", "b"}, o);
    this->check("a+=b", {"a", "+", "=", "b"}, o);
    // the numbers with a leading sign and the increment operator are not
    // affected by the option
    this->check("x=-1;", {"x", "=", "-1", ";"}, o);
    this->check("a++", {"a", "++"}, o);
  }  // end of testTwoCharactersSeparatorsNotJoined

  /*!
   * \brief the token pasting operator of the preprocessor, which the tokenizer
   * meets in the directives it tokenizes, as the `#define` ones. The `#`
   * punctuator of `#define NAME(A) A ## _sfx` introduces neither a keyword nor
   * a comment, so that the tokenizer used to reject the directive with a
   * message about a stray hash character
   */
  void testTokenPasting() {
    this->check("#define SUFFIX(A) A ## _sfx",
                {"#", "define", "SUFFIX", "(", "A", ")", "A", "##", "_sfx"});
    // the stringizer operator, which is a single `#` punctuator followed by the
    // argument of the macro
    this->check("#define STRINGIZE(A) #A",
                {"#", "define", "STRINGIZE", "(", "A", ")", "#", "A"});
  }  // end of testTokenPasting
};   // end of struct CxxTokenizerPunctuatorsTest

TFEL_TESTS_GENERATE_PROXY(CxxTokenizerPunctuatorsTest,
                          "CxxTokenizerPunctuatorsTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("CxxTokenizerPunctuatorsTest.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
