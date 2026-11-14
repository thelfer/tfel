/*!
 * \file   CxxTokenizerStripCommentsTest.cxx
 * \brief  This file checks the tokens which tfel::utilities::CxxTokenizer::
 * stripComments removes and the comments it leaves behind: the doxygen comments
 * are attached to a neighbouring token, the comments of a line are gathered in
 * the comments of the tokenizer. The neighbours a comment is attached to are
 * those of the sequence in which the comments which have already been visited
 * are gone, which the previous implementation obtained erasing the comment
 * tokens one at a time, see issue #1409.
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
#include <map>
#include <string>
#include <vector>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/Token.hxx"

struct CxxTokenizerStripCommentsTest final : public tfel::tests::TestCase {
  CxxTokenizerStripCommentsTest()
      : tfel::tests::TestCase("TFEL/Utilities",
                              "CxxTokenizerStripCommentsTest") {
  }  // end of CxxTokenizerStripCommentsTest

  tfel::tests::TestResult execute() override {
    this->testForwardDoxygenComments();
    this->testBackwardDoxygenComments();
    this->testCommentsOfALine();
    this->testCommentsWhoseBoundariesAreKept();
    this->testStrippingLineByLine();
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
   * \brief tokenizes the given string and removes its comments
   * \param[out] t: tokenizer holding the tokens which are left
   * \param[in] s: contents of the file
   * \param[in] b: boolean telling if the boundaries of the comments are a part
   * of their value
   */
  static void parse_and_strip(Tokenizer& t,
                              const std::string& s,
                              const bool b = false) {
    t.clear();
    if (b) {
      t.keepCommentBoundaries(true);
    }
    t.parseString(s);
    t.stripComments();
  }  // end of parse_and_strip

  /*!
   * \brief doxygen comments which precede the token they comment
   *
   * A comment of the kind DoxygenComment is attached to the token which follows
   * it in the sequence of the tokens of the file, whatever the number of
   * comments which precede or follow it.
   */
  void testForwardDoxygenComments() {
    auto t = Tokenizer{};
    // a single doxygen comment
    this->parse_and_strip(t, "int z;\n//! x\nint a;\n");
    TFEL_TESTS_ASSERT(t.size() == 6u);
    TFEL_TESTS_ASSERT(t[3].value == "int");
    TFEL_TESTS_ASSERT(t[3].comment == "x");
    TFEL_TESTS_ASSERT(t.getComments().empty());
    // a `C` style doxygen comment
    this->parse_and_strip(t, "int z;\n/*! x */ int a;\n");
    TFEL_TESTS_ASSERT(t.size() == 6u);
    TFEL_TESTS_ASSERT(t[3].value == "int");
    TFEL_TESTS_ASSERT(t[3].comment == "x");
    // two comments in a row: the first one is prepended to the value of the
    // second one, which is the token the second comment is attached to
    this->parse_and_strip(t, "int z;\n//! a\n//! b\nint x;\n");
    TFEL_TESTS_ASSERT(t.size() == 6u);
    TFEL_TESTS_ASSERT(t[3].value == "int");
    TFEL_TESTS_ASSERT(t[3].comment == "a\nb");
    // three comments in a row
    this->parse_and_strip(t, "int z;\n//! a\n//! b\n//! c\nint x;\n");
    TFEL_TESTS_ASSERT(t.size() == 6u);
    TFEL_TESTS_ASSERT(t[3].value == "int");
    TFEL_TESTS_ASSERT(t[3].comment == "a\nb\nc");
    // a comment which precedes a comment of the other kind keeps its own
    // comment: the two comments are attached to different tokens
    this->parse_and_strip(t, "int z;\n//! a\nint x; //!< b\n");
    TFEL_TESTS_ASSERT(t.size() == 6u);
    TFEL_TESTS_ASSERT(t[3].value == "int");
    TFEL_TESTS_ASSERT(t[3].comment == "a");
    TFEL_TESTS_ASSERT(t[5].value == ";");
    TFEL_TESTS_ASSERT(t[5].comment == "b");
    // a comment which precedes a comment of the other kind: the forward comment
    // has no standard token to be attached to, since the token which follows it
    // is a comment, and is dropped. The backward comment is attached to the
    // token which precedes it
    this->parse_and_strip(t, "int z;\n//! a\n//!< b\nint x;\n");
    TFEL_TESTS_ASSERT(t.size() == 6u);
    TFEL_TESTS_ASSERT(t[2].value == ";");
    TFEL_TESTS_ASSERT(t[2].comment == "b");
    TFEL_TESTS_ASSERT(t[3].value == "int");
    TFEL_TESTS_ASSERT(t[3].comment.empty());
    // the last token of the file has no token to be attached to: such a comment
    // is dropped
    this->parse_and_strip(t, "int z; //! x\n");
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t[2].comment.empty());
    TFEL_TESTS_ASSERT(t.getComments().empty());
    // a comment which precedes a token which is not a standard token is
    // dropped: only the standard tokens carry a comment
    this->parse_and_strip(t, "int z;\n//! x\n1'000;\n");
    TFEL_TESTS_ASSERT(t.size() == 5u);
    TFEL_TESTS_ASSERT(t[3].flag == Token::Number);
    TFEL_TESTS_ASSERT(t[3].comment.empty());
    TFEL_TESTS_ASSERT(t.getComments().empty());
    this->parse_and_strip(t, "int z;\n//! x\n\"s\";\n");
    TFEL_TESTS_ASSERT(t.size() == 5u);
    TFEL_TESTS_ASSERT(t[3].flag == Token::String);
    TFEL_TESTS_ASSERT(t[3].comment.empty());
  }  // end of testForwardDoxygenComments

  /*!
   * \brief doxygen comments which follow the token they comment
   *
   * A comment of the kind DoxygenBackwardComment is attached to the last token
   * which precedes it among the tokens which are kept, which is not the token
   * which precedes it in the sequence of the tokens of the file when a comment
   * has been removed before it.
   */
  void testBackwardDoxygenComments() {
    auto t = Tokenizer{};
    // a single comment
    this->parse_and_strip(t, "int a; //!< x\n");
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t[2].value == ";");
    TFEL_TESTS_ASSERT(t[2].comment == "x");
    // a `C` style comment
    this->parse_and_strip(t, "int z;\n/*!< x */ int a;\n");
    TFEL_TESTS_ASSERT(t.size() == 6u);
    TFEL_TESTS_ASSERT(t[2].value == ";");
    TFEL_TESTS_ASSERT(t[2].comment == "x");
    // the first token of the file has no token to be attached to: the comment
    // of the kind Comment it gives is gathered in the comments of its line
    this->parse_and_strip(t, "//!< x\nint a;\n");
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t.getComments().size() == 1u);
    TFEL_TESTS_ASSERT(t.getComments().at(1u) == "x");
    // several comments attached to the same token: their values are gathered in
    // the order they appear in the file, without any separator, as the comment
    // which is attached to a token is appended to the comment it already holds
    this->parse_and_strip(t, "int a; //!< x\n//!< y\nint b;\n");
    TFEL_TESTS_ASSERT(t.size() == 6u);
    TFEL_TESTS_ASSERT(t[2].value == ";");
    TFEL_TESTS_ASSERT(t[2].comment == "xy");
    // a comment which follows another comment: the token it is attached to is
    // that of the first comment, not the comment which precedes it in the file
    this->parse_and_strip(t, "int a; /* c */ //!< x\n");
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t[2].value == ";");
    TFEL_TESTS_ASSERT(t[2].comment == "x");
    TFEL_TESTS_ASSERT(t.getComments().size() == 1u);
    TFEL_TESTS_ASSERT(t.getComments().at(1u) == "c");
    // the same, with two comments removed before the backward comment: both are
    // gathered on the line they belong to
    this->parse_and_strip(t, "int a; /* c1 */ // c2\n//!< x\nint b;\n");
    TFEL_TESTS_ASSERT(t.size() == 6u);
    TFEL_TESTS_ASSERT(t[2].value == ";");
    TFEL_TESTS_ASSERT(t[2].comment == "x");
    TFEL_TESTS_ASSERT(t.getComments().size() == 1u);
    TFEL_TESTS_ASSERT(t.getComments().at(1u) == "c1 c2");
    // a forward comment between the token and the backward comment: the forward
    // comment is attached to the token which follows it, which is that which
    // precedes the backward comment in the sequence of the kept tokens
    this->parse_and_strip(t, "int a; //! c\n//!< x\nint b;\n");
    TFEL_TESTS_ASSERT(t.size() == 6u);
    TFEL_TESTS_ASSERT(t[2].value == ";");
    TFEL_TESTS_ASSERT(t[2].comment == "x");
  }  // end of testBackwardDoxygenComments

  /*!
   * \brief the comments of a line are gathered in the comments of the
   * tokenizer, separated by a space character, see issue #1392, whereas the
   * doxygen comments are attached to a token
   */
  void testCommentsOfALine() {
    auto t = Tokenizer{};
    // the three kinds of comments of a single line
    this->parse_and_strip(t, "/* c1 */ int a; /* c2 */ //!< x\n");
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t[0].value == "int");
    TFEL_TESTS_ASSERT(t[0].offset == 9u);
    TFEL_TESTS_ASSERT(t[2].comment == "x");
    TFEL_TESTS_ASSERT(t.getComments().size() == 1u);
    TFEL_TESTS_ASSERT(t.getComments().at(1u) == "c1 c2");
    // the comments of a line which holds a comment spanning several lines: the
    // comment is gathered on the line it starts on and the empty comment gives
    // no separator
    this->parse_and_strip(t, "/* m1\nm2 */ int a; /**/ //! n\nint b;\n");
    TFEL_TESTS_ASSERT(t.size() == 6u);
    TFEL_TESTS_ASSERT(t.getComments().size() == 1u);
    TFEL_TESTS_ASSERT(t.getComments().at(1u) == "m1\nm2 ");
    TFEL_TESTS_ASSERT(t[2].comment.empty());
    TFEL_TESTS_ASSERT(t[3].value == "int");
    TFEL_TESTS_ASSERT(t[3].comment == "n");
    // a line which holds no comment gives no entry in the comments of the
    // tokenizer
    this->parse_and_strip(t, "int a;\nint b;\n");
    TFEL_TESTS_ASSERT(t.size() == 6u);
    TFEL_TESTS_ASSERT(t.getComments().empty());
    // removing the comments twice brings nothing: the doxygen comments which
    // have been attached to a token are not gathered a second time in the
    // comments of the lines
    const auto n = t.getComments().size();
    TFEL_TESTS_ASSERT(n == 0u);
    t.stripComments();
    TFEL_TESTS_ASSERT(t.size() == 6u);
    TFEL_TESTS_ASSERT(t.getComments().size() == n);
  }  // end of testCommentsOfALine

  /*!
   * \brief the comments whose boundaries are a part of their value, see issue
   * #1407, are removed as the others and carry their boundaries
   */
  void testCommentsWhoseBoundariesAreKept() {
    auto t = Tokenizer{};
    // the boundaries of a forward comment are a part of the comment it gives to
    // the token which follows it
    this->parse_and_strip(t, "int z;\n//! x\nint a;\n", true);
    TFEL_TESTS_ASSERT(t.size() == 6u);
    TFEL_TESTS_ASSERT(t[3].value == "int");
    TFEL_TESTS_ASSERT(t[3].offset == 0u);
    TFEL_TESTS_ASSERT(t[3].comment == "//! x");
    // the boundaries of two comments in a row are gathered as the two comments
    // are a single value
    this->parse_and_strip(t, "int z;\n//! a\n//! b\nint x;\n", true);
    TFEL_TESTS_ASSERT(t.size() == 6u);
    TFEL_TESTS_ASSERT(t[3].comment == "//! a\n//! b");
    // the boundaries of a backward comment
    this->parse_and_strip(t, "int a; //!< x\n", true);
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t[2].value == ";");
    TFEL_TESTS_ASSERT(t[2].comment == "//!< x");
    // the boundaries of the comments of a line are gathered in the comments of
    // this line
    this->parse_and_strip(t, "/* c1 */ int a; /* c2 */ //!< x\n", true);
    TFEL_TESTS_ASSERT(t.size() == 3u);
    TFEL_TESTS_ASSERT(t[0].offset == 9u);
    TFEL_TESTS_ASSERT(t[2].comment == "//!< x");
    TFEL_TESTS_ASSERT(t.getComments().size() == 1u);
    TFEL_TESTS_ASSERT(t.getComments().at(1u) == "/* c1 */ /* c2 */");
    // an empty comment gives a comment whose value is the delimiter of the
    // comment, which is then gathered in the comments of the line
    this->parse_and_strip(t, "a; //\n", true);
    TFEL_TESTS_ASSERT(t.size() == 2u);
    TFEL_TESTS_ASSERT(t.getComments().size() == 1u);
    TFEL_TESTS_ASSERT(t.getComments().at(1u) == "//");
  }  // end of testCommentsWhoseBoundariesAreKept

  /*!
   * \brief the comments are removed from the tokens of the file, whether the
   * file is tokenized with a single call to the parseString method or line by
   * line, as a caller which reads a file line by line does
   */
  void testStrippingLineByLine() {
    auto t = Tokenizer{};
    const auto s = std::string{
        "/* header */\n"
        "//! a\n"
        "int f(int a) //!< first\n"
        "{\n"
        "  /* body */\n"
        "  int b = a + 1;  // trailing\n"
        "  /*! second */\n"
        "  return b;\n"
        "}  //!< third\n"};
    this->parse_and_strip(t, s);
    auto u = Tokenizer{};
    u.parseString("/* header */\n");
    u.parseString("//! a\n");
    u.parseString("int f(int a) //!< first\n");
    u.parseString("{\n");
    u.parseString("  /* body */\n");
    u.parseString("  int b = a + 1;  // trailing\n");
    u.parseString("  /*! second */\n");
    u.parseString("  return b;\n");
    u.parseString("}  //!< third\n");
    u.stripComments();
    TFEL_TESTS_ASSERT(t.size() == u.size());
    // the lines of the tokens are not compared: the line numbers restart at the
    // first line for each call to the parseString method, as its description
    // states, which is also the case in the CxxTokenizerLineByLineTest test
    for (Token::size_type i = 0u; i != t.size(); ++i) {
      TFEL_TESTS_ASSERT(t[i].flag == u[i].flag);
      TFEL_TESTS_ASSERT(t[i].value == u[i].value);
      TFEL_TESTS_ASSERT(t[i].offset == u[i].offset);
      TFEL_TESTS_ASSERT(t[i].comment == u[i].comment);
    }
    // the tokens which are left are those of the file without its comments
    TFEL_TESTS_ASSERT(t.size() == 18u);
    TFEL_TESTS_ASSERT(t[0].value == "int");
    TFEL_TESTS_ASSERT(t[0].line == 3u);
    TFEL_TESTS_ASSERT(t[0].offset == 0u);
    TFEL_TESTS_ASSERT(t[0].comment == "a");
    TFEL_TESTS_ASSERT(t[5].value == ")");
    TFEL_TESTS_ASSERT(t[5].comment == "first");
    TFEL_TESTS_ASSERT(t[6].value == "{");
    TFEL_TESTS_ASSERT(t[6].line == 4u);
    TFEL_TESTS_ASSERT(t[7].value == "int");
    TFEL_TESTS_ASSERT(t[7].line == 6u);
    TFEL_TESTS_ASSERT(t[7].comment.empty());
    TFEL_TESTS_ASSERT(t[14].value == "return");
    TFEL_TESTS_ASSERT(t[14].line == 8u);
    TFEL_TESTS_ASSERT(t[14].comment == "second");
    TFEL_TESTS_ASSERT(t[17].value == "}");
    TFEL_TESTS_ASSERT(t[17].line == 9u);
    TFEL_TESTS_ASSERT(t[17].comment == "third");
    // the comments of the lines which are not doxygen comments
    TFEL_TESTS_ASSERT(t.getComments().size() == 3u);
    TFEL_TESTS_ASSERT(t.getComments().at(1u) == "header");
    TFEL_TESTS_ASSERT(t.getComments().at(5u) == "body");
    TFEL_TESTS_ASSERT(t.getComments().at(6u) == "trailing");
  }  // end of testStrippingLineByLine
};   // end of struct CxxTokenizerStripCommentsTest

TFEL_TESTS_GENERATE_PROXY(CxxTokenizerStripCommentsTest,
                          "CxxTokenizerStripCommentsTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("CxxTokenizerStripCommentsTest.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
