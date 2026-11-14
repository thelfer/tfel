/*!
 * \file   additional-tests/fuzzing/CxxTokenizer/cxx-fuzzing.cxx
 * \brief  This file performs a libFuzzer based analysis of the
 * tfel::utilities::CxxTokenizer class, in which the contents of `C++` files is
 * tokenized with the default options of the tokenizer, that is as a `C++` file.
 * \author Thomas Helfer
 * \date   11/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <vector>

#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/CxxTokenizerOptions.hxx"
#include "TFEL/Utilities/Token.hxx"

/*!
 * \brief entry point of libFuzzer
 * \param[in] data:
 */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* const, const size_t);

namespace {

  /*!
   * \brief split a contents in lines, as the tokenizer does
   * \param[in] s: contents to split
   * \return the lines of the contents, without the end of line characters
   */
  std::vector<std::string> getLines(const std::string& s) {
    auto lines = std::vector<std::string>{};
    auto p = std::size_t{};
    for (;;) {
      const auto q = s.find('\n', p);
      if (q == std::string::npos) {
        lines.push_back(s.substr(p));
        return lines;
      }
      lines.push_back(s.substr(p, q - p));
      p = q + 1u;
    }
  }  // end of getLines

  /*!
   * \brief check the consistency of the tokens obtained tokenizing the given
   * contents.
   *
   * Those checks do not use `tfel::raise` since such an exception would be
   * caught: the tokenizer is expected to reject the invalid inputs libFuzzer
   * generates.
   *
   * \param[in] contents: tokenized contents
   * \param[in] size: number of bytes given to libFuzzer, used for the report
   * \param[in] t: tokenizer which tokenized the contents
   */
  void checkTokens(const std::string& contents,
                   const size_t size,
                   tfel::utilities::CxxTokenizer& t) {
    using Token = tfel::utilities::Token;
    const auto lines = getLines(contents);
    auto previous_line = std::size_t{0};
    auto previous_offset = std::size_t{0};
    for (const auto& token : t) {
      // the line numbers of the tokens are non decreasing
      if (token.line < previous_line) {
        std::cerr << "decreasing line number obtained tokenizing " << size
                  << " bytes\n";
        std::abort();
      }
      // the offsets of the tokens of a line are strictly increasing
      if ((token.line == previous_line) && (token.offset <= previous_offset)) {
        std::cerr << "non increasing offset obtained tokenizing " << size
                  << " bytes\n";
        std::abort();
      }
      previous_line = token.line;
      previous_offset = token.offset;
      // a token which is not a comment has a non empty value: the value of a
      // comment is empty when the comment is empty, which is the wanted
      // behaviour, see issue #1391. A raw string literal with an empty
      // contents holds the whole literal as its value, as a quoted string
      // holds its quotes, see issue #1390
      if (token.value.empty() && (token.flag != Token::Comment) &&
          (token.flag != Token::DoxygenComment) &&
          (token.flag != Token::DoxygenBackwardComment)) {
        std::cerr << "empty token obtained tokenizing " << size << " bytes\n";
        std::abort();
      }
      // the offset of a token is an offset in the line the token starts on.
      // The tokens which are added by the tokenizer itself, such as the curly
      // braces added by the `addCurlyBraces` option, may be out of the file
      if ((token.line == 0u) || (token.line > lines.size())) {
        continue;
      }
      const auto& line = lines[token.line - 1u];
      // the offset of a token is the position of a character of the line the
      // token starts on, so that it is strictly smaller than the size of that
      // line, even for a token whose value is empty: the offset of an empty
      // comment is that of its delimiter, see issue #1391, and the offset of a
      // raw string token is that of its `R` delimiter, see issue #1390
      if (token.offset >= line.size()) {
        std::cerr << "offset out of the line obtained tokenizing " << size
                  << " bytes\n";
        std::abort();
      }
      // a standard token, a number and a character literal cannot span several
      // lines: such a token is necessarily contained in its line. The other
      // kinds of tokens may span several lines, and the value of a number may
      // be shorter than its spelling in the file since the digit separators of
      // the C++14 literals are removed
      if (((token.flag == Token::Standard) || (token.flag == Token::Number) ||
           (token.flag == Token::Char)) &&
          ((token.offset + token.value.size()) > line.size())) {
        std::cerr << "token out of its line obtained tokenizing " << size
                  << " bytes\n";
        std::abort();
      }
      // the value of a token is the spelling of the token in the file, that is
      // the substring of the line taken at the offset of the token, except for
      // the comments, whose value leaves out their delimiters, and for the
      // numbers, whose value leaves out the digit separators. This invariant
      // only applies to the tokens which do not span several lines since the
      // value of a comment and that of a raw string literal gather the lines
      // they span. It is the reason why this analysis does not use the
      // `keepCommentBoundaries` option: the delimiters which enter the value of
      // a comment token are not taken into account in the offsets of the tokens
      // which follow that comment on the same line, see the study reported on
      // issue #1405
      if (((token.flag == Token::Standard) || (token.flag == Token::String) ||
           (token.flag == Token::Char)) &&
          ((token.offset + token.value.size()) <= line.size()) &&
          (line.compare(token.offset, token.value.size(), token.value) != 0)) {
        std::cerr << "value of a token which is not its spelling in the file "
                     "obtained tokenizing "
                  << size << " bytes\n";
        std::abort();
      }
      // the contents of a literal is a part of the value of the token, whatever
      // the kind of the literal: the quotes of a quoted string or of a
      // character literal and the `R"delim(` prefix and `)delim"` suffix of a
      // raw string literal are removed, whereas the file may end inside a
      // literal, in which case the value of the token has no closing delimiter,
      // see issues #1390 and #1395
      if ((token.flag == Token::String) || (token.flag == Token::Char) ||
          tfel::utilities::isRawString(token)) {
        const auto c = tfel::utilities::getContents(token);
        if ((c.size() > token.value.size()) ||
            (token.value.find(c) == std::string::npos)) {
          std::cerr << "contents of a literal which is not a part of the value "
                       "of its token obtained tokenizing "
                    << size << " bytes\n";
          std::abort();
        }
      }
    }
  }  // end of checkTokens

  /*!
   * \brief tokenize the given data with the default options of the tokenizer,
   * that is as a `C++` file, and check the consistency of the obtained tokens.
   *
   * The `C++` files of the project are tokenized by `tfel-doc` and `tfel-check`
   * with the default options of the tokenizer, which are those of the `C++`
   * language: the preprocessor directives are treated, the strings are
   * extracted, the two characters separators are joined, etc... As opposed to
   * the `mfront-fuzzing` analysis, no separator specific to a domain specific
   * language is added.
   *
   * \param[in] data: data to tokenize
   * \param[in] size: number of bytes to tokenize
   */
  void analyse(const char* const data, const size_t size) {
    auto t = tfel::utilities::CxxTokenizer{};
    auto contents = std::string{data, size};
    t.parseString(contents);
    // printing the tokens, which exercises the treatment of the lines and the
    // offsets
    auto out = std::ostringstream{};
    t.printFileTokens(out);
    // checking the consistency of the obtained tokens, which must be done
    // before the substitutions as those ones change the values of the tokens
    // without changing their offsets
    checkTokens(contents, size, t);
    // applying the kind of substitutions `tfel-check` and `MFront` apply to the
    // files they read: those patterns are those of the default substitutions of
    // TFEL, which are built from the `add_tfel_substitution` function of
    // `cmake`
    auto substitutions = std::map<std::string, std::string>{};
    substitutions["@TFEL_INSTALL_DIR@"] = "/usr/local";
    substitutions["@TFEL_INCLUDE_PATH@"] = "/usr/local/include/TFEL";
    substitutions["@NOT_A_SUBSTITUTION_PATTERN@"] = "test";
    t.substitute(substitutions);
    // no comment token is kept when the comments are stripped
    t.stripComments();
    for (const auto& token : t) {
      if (tfel::utilities::isComment(token)) {
        std::cerr << "comment token kept by stripComments when tokenizing "
                  << size << " bytes\n";
        std::abort();
      }
    }
  }  // end of analyse

}  // end of anonymous namespace

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* const data,
                                      const size_t size) {
  try {
    const auto* const s = (size > 0) ? reinterpret_cast<const char*>(data) : "";
    analyse(s, size);
  } catch (const std::exception&) {
    // the tokenizer throws an exception when the file it reads is not a valid
    // C++ file: unterminated string, stray character, etc... Such an exception
    // is not a failure of the analysis
  }
  return 0;
}  // end of LLVMFuzzerTestOneInput
