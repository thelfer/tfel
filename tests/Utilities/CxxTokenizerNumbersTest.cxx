/*!
 * \file   CxxTokenizerNumbersTest.cxx
 * \brief  This file checks the tokenization of the numeric literals and the
 * values which are read in the number tokens, see issue #1460.
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
#include <cmath>
#include <cstdlib>
#include <limits>
#include <string>
#include <vector>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"

struct CxxTokenizerNumbersTest final : public tfel::tests::TestCase {
  CxxTokenizerNumbersTest()
      : tfel::tests::TestCase("TFEL/Utilities", "CxxTokenizerNumbersTest") {
  }  // end of CxxTokenizerNumbersTest
  tfel::tests::TestResult execute() override {
    this->testDecimalIntegers();
    this->testBinaryIntegers();
    this->testHexadecimalIntegers();
    this->testHexadecimalFloatingPointLiterals();
    this->testFloatingPointLiterals();
    this->testUserDefinedLiterals();
    this->testDigitSeparators();
    this->testIllFormedLiterals();
    this->testLiteralsFollowedByIdentifiers();
    this->testValues();
    this->testExtractingSuffixes();
    this->testParseNumberArgument();
    return this->result;
  }  // end of execute()
 private:
  /*!
   * \brief check that the given string is tokenized as a single number token
   * whose value is the expected one
   * \param[in] s: string to tokenize
   * \param[in] v: expected value of the token
   */
  void checkNumber(const std::string& s, const std::string& v) {
    tfel::utilities::CxxTokenizer t;
    t.parseString(s);
    TFEL_TESTS_ASSERT(t.size() == 1u);
    if (t.size() != 1u) {
      return;
    }
    TFEL_TESTS_ASSERT(t[0].flag == tfel::utilities::Token::Number);
    TFEL_TESTS_ASSERT(t[0].value == v);
  }
  /*!
   * \brief check that the given string is tokenized as a single number token
   * whose value is the spelling of the literal
   * \param[in] s: string to tokenize
   */
  void checkNumber(const std::string& s) { this->checkNumber(s, s); }
  /*!
   * \brief check that tokenizing the given string is rejected
   * \param[in] s: string to tokenize
   */
  void checkInvalidNumber(const std::string& s) {
    tfel::utilities::CxxTokenizer t;
    TFEL_TESTS_CHECK_THROW(t.parseString(s), std::runtime_error);
  }
  /*!
   * \brief check the values of the tokens obtained tokenizing the given string
   * \param[in] s: string to tokenize
   * \param[in] v: expected values of the tokens
   */
  void checkTokens(const std::string& s, const std::vector<std::string>& v) {
    tfel::utilities::CxxTokenizer t;
    t.parseString(s);
    TFEL_TESTS_ASSERT(t.size() == v.size());
    if (t.size() != v.size()) {
      // the values of the tokens are only checked if their number is the
      // expected one
      return;
    }
    auto i = size_t{};
    for (const auto& tok : t) {
      TFEL_TESTS_ASSERT(tok.value == v[i]);
      ++i;
    }
  }
  /*!
   * \brief check the value read by `readInt` in a number token
   * \param[in] s: string to tokenize
   * \param[in] v: expected value
   */
  void checkInteger(const std::string& s, const int v) {
    tfel::utilities::CxxTokenizer t;
    t.parseString(s);
    TFEL_TESTS_ASSERT(t.size() == 1u);
    if (t.size() != 1u) {
      return;
    }
    auto p = t.begin();
    TFEL_TESTS_ASSERT(t.readInt(p, t.end()) == v);
  }
  /*!
   * \brief check the value read by `readUnsignedInt` in a number token
   * \param[in] s: string to tokenize
   * \param[in] v: expected value
   */
  void checkUnsignedInteger(const std::string& s, const unsigned int v) {
    tfel::utilities::CxxTokenizer t;
    t.parseString(s);
    TFEL_TESTS_ASSERT(t.size() == 1u);
    if (t.size() != 1u) {
      return;
    }
    auto p = t.begin();
    TFEL_TESTS_ASSERT(t.readUnsignedInt(p, t.end()) == v);
  }
  /*!
   * \brief check the value read by `readDouble` in a number token
   * \param[in] s: string to tokenize
   * \param[in] v: expected value
   */
  void checkDouble(const std::string& s, const double v) {
    tfel::utilities::CxxTokenizer t;
    t.parseString(s);
    TFEL_TESTS_ASSERT(t.size() == 1u);
    if (t.size() != 1u) {
      return;
    }
    auto p = t.begin();
    const auto x = t.readDouble(p, t.end());
    // the values of the literals which are tested are exactly representable: a
    // relative tolerance is only used to avoid comparing floating point values
    // with `==`
    TFEL_TESTS_ASSERT(std::abs(x - v) <=
                      (std::abs(v) * std::numeric_limits<double>::epsilon()));
  }
  /*!
   * \return the single number token which the given string is tokenized as
   * \param[in] s: string to tokenize
   */
  tfel::utilities::Token getNumberToken(const std::string& s) {
    tfel::utilities::CxxTokenizer t;
    t.parseString(s);
    TFEL_TESTS_ASSERT(t.size() == 1u);
    return t[0];
  }  // end of getNumberToken
  /*!
   * \brief check the value read in a number token
   * \param[in] x: value which has been read
   * \param[in] v: expected value
   */
  void checkValue(const double x, const double v) {
    // the values of the literals which are tested are exactly representable: a
    // relative tolerance is only used to avoid comparing floating point values
    // with `==`
    TFEL_TESTS_ASSERT(std::abs(x - v) <=
                      (std::abs(v) * std::numeric_limits<double>::epsilon()));
  }
  /*!
   * \brief check the suffixes extracted from a number token
   * \param[in] s:  string to tokenize
   * \param[in] ts: expected type suffix of the literal
   * \param[in] us: expected suffix of the user defined literal
   */
  void checkSuffixes(const std::string& s,
                     const std::string& ts,
                     const std::string& us) {
    const auto tok = this->getNumberToken(s);
    TFEL_TESTS_ASSERT(
        tfel::utilities::CxxTokenizer::extractNumberTypeSuffix(tok) == ts);
    TFEL_TESTS_ASSERT(
        tfel::utilities::CxxTokenizer::extractNumberUserDefinedLiteral(tok) ==
        us);
  }
  // decimal integers
  void testDecimalIntegers() {
    for (const auto& s :
         {"0",    "42",   "1u",  "1U",  "1l",  "1L",   "1ll",
          "1LL",  "1ul",  "1UL", "1Lu", "1lu", "1ull", "1uLL",
          "1llu", "1LLu", "+1",  "-1",  "-2l", "+3u",  "-4ll"}) {
      this->checkNumber(s);
    }
    // a number with a leading zero is read as a decimal integer, which is a
    // documented deviation of C++, see issue #1460
    for (const auto& s : {"07", "010", "08", "018", "0777u"}) {
      this->checkNumber(s);
    }
  }  // end of testDecimalIntegers
  // binary integers
  void testBinaryIntegers() {
    for (const auto& s : {"0b0", "0b1010", "0B1010", "0b1010u", "0b1010LU"}) {
      this->checkNumber(s);
    }
  }  // end of testBinaryIntegers
  // hexadecimal integers
  void testHexadecimalIntegers() {
    for (const auto& s :
         {"0x0", "0xFF", "0X1a", "0xFFu", "0xffLU", "0x10ll", "0xAu"}) {
      this->checkNumber(s);
    }
  }  // end of testHexadecimalIntegers
  // hexadecimal floating point literals, which are supported since C++11
  void testHexadecimalFloatingPointLiterals() {
    for (const auto& s :
         {"0x1p3", "0x1P3", "0x1.8p3", "0x1p-2", "0x1p+3", "0x.8p3", "0xA.p3",
          "0x1p3f", "0x1p3F", "0x1p3l", "0x1p3L", "0xB.8p0", "-0x1p3"}) {
      this->checkNumber(s);
    }
    // the digit separators are not a part of the value of the token
    this->checkNumber("0x1'fp3", "0x1fp3");
    // a hexadecimal floating point literal requires an exponent
    this->checkInvalidNumber("0x1.8");
    this->checkInvalidNumber("0x1.8f");
    this->checkInvalidNumber("0x1p");
    this->checkInvalidNumber("0x.8");
    // the exponent of a hexadecimal floating point literal is introduced by
    // `p`: the literal is an integer literal when it has no such exponent
    this->checkNumber("0xe5");
    this->checkNumber("0x1e3");
  }  // end of testHexadecimalFloatingPointLiterals
  // decimal floating point literals
  void testFloatingPointLiterals() {
    for (const auto& s : {"1.", "1.5", ".5", "-.5", "0.0", "1e5", "1e+5",
                          "1e-5", "1.5e-3", "1.5f", "1.5F", "1.5l", "1.5L",
                          // the exponent of a literal makes it a floating point
                          // literal, whatever the sign of this exponent
                          "1e5f", "1E5F", "1e5l", "1E+5L", "1e-5f", "1.f",
                          ".3e11f", "2.e-5", "3e0"}) {
      this->checkNumber(s);
    }
  }  // end of testFloatingPointLiterals
  // user defined literals, which are supported since C++11
  void testUserDefinedLiterals() {
    for (const auto& s :
         {"2.e-5_kg", "2.e-5l_kg", "1_km", "0x1p3_km", "-1.5_imp_thing"}) {
      this->checkNumber(s);
    }
  }  // end of testUserDefinedLiterals
  // digit separators of the C++14 literals, which are not a part of the value
  // of the token
  void testDigitSeparators() {
    this->checkNumber("1'000", "1000");
    this->checkNumber("1'000'000", "1000000");
    this->checkNumber("0b1'010", "0b1010");
    this->checkNumber("0x1'FFF", "0x1FFF");
    this->checkNumber("3.14'15", "3.1415");
    this->checkNumber("1.0'0e1", "1.00e1");
    this->checkNumber("1'000.5e3", "1000.5e3");
    this->checkNumber("1'2", "12");
    this->checkNumber("0'1", "01");
    this->checkNumber("1e1'0", "1e10");
    // the separators of a number do not change the offsets of the tokens which
    // follow it
    {
      tfel::utilities::CxxTokenizer t;
      t.parseString("@IterMax 1'000;");
      TFEL_TESTS_ASSERT(t.size() == 3u);
      TFEL_TESTS_ASSERT(t[1].value == "1000");
      TFEL_TESTS_ASSERT(t[1].offset == 9u);
      TFEL_TESTS_ASSERT(t[2].value == ";");
      TFEL_TESTS_ASSERT(t[2].offset == 14u);
    }
  }  // end of testDigitSeparators
  // the literals which are rejected by C++ are rejected, and not split in
  // several tokens
  void testIllFormedLiterals() {
    // `2f` is rejected since `f` is a character which may begin the suffix of a
    // numeric literal and is not a valid one for an integer literal
    this->checkInvalidNumber("2f");
    // the suffixes of the floating point types of `C++23`, as `f32`, are not
    // supported: the digits which follow the `f` suffix are rejected
    for (const auto& s : {"1.0f32", "1.f32", "0.0f32"}) {
      this->checkInvalidNumber(s);
    }
    // the suffixes of the integer literals
    for (const auto& s : {"1ulu", "1lul", "1lll", "1uull", "1ulul"}) {
      this->checkInvalidNumber(s);
    }
    // the two characters of a `long long` suffix have the same case
    this->checkInvalidNumber("1lL");
    this->checkInvalidNumber("1Ll");
    // a floating point literal has at most one suffix character and can not be
    // unsigned
    this->checkInvalidNumber("1.0ff");
    this->checkInvalidNumber("1.0ll");
    this->checkInvalidNumber("0.3LL");
    this->checkInvalidNumber("1e5u");
    this->checkInvalidNumber("1e5ul");
    // an unsigned number can not be signed, which is a consequence of holding
    // the sign of a number in the token which holds the number
    for (const auto& s : {"-2U", "-2UL", "-2ULL", "-2LU", "-2LLU"}) {
      this->checkInvalidNumber(s);
    }
    // the base prefixes
    this->checkInvalidNumber("0b");
    this->checkInvalidNumber("0b2");
    this->checkInvalidNumber("0b020");
    this->checkInvalidNumber("0x");
    this->checkInvalidNumber("0xp");
    // a binary literal is an integer literal
    this->checkInvalidNumber("0b1010.5");
    this->checkInvalidNumber("0b1010e3");
    // several decimal points
    this->checkInvalidNumber("0.3.3");
    this->checkInvalidNumber(".3.3");
    // a digit separator is only allowed between two digits
    for (const auto& s : {"1'", "12'", "1''2", "1'000'", "0x'1", "0b'1"}) {
      this->checkInvalidNumber(s);
    }
  }  // end of testIllFormedLiterals
  // an identifier which follows a number is not a part of this number: the
  // maximal munch rule of the preprocessing tokens is not applied as strictly
  // as C++ does it, since the tokenizer is also used for the free form texts of
  // `MFront`, as the `@Description` blocks, in which a number is often followed
  // by an identifier, see issue #1460
  void testLiteralsFollowedByIdentifiers() {
    this->checkTokens("123abc", {"123", "abc"});
    this->checkTokens("12.3a", {"12.3", "a"});
    this->checkTokens("1.5xyz", {"1.5", "xyz"});
    this->checkTokens("0x1GG", {"0x1", "GG"});
    // a number followed by an identifier, as it may be found in a free form
    // text
    this->checkTokens("V-5Cr-5Ti", {"V", "-", "5", "Cr", "-", "5", "Ti"});
    this->checkTokens("21st", {"21", "st"});
    // the suffixes of the decimal floating point types of the `C` extensions,
    // as `dd`, are not supported and do not introduce an error either
    this->checkTokens("1.0dd", {"1.0", "dd"});
  }  // end of testLiteralsFollowedByIdentifiers
  // the value of the number tokens
  void testValues() {
    this->checkInteger("0", 0);
    this->checkInteger("42", 42);
    this->checkInteger("+3u", 3);
    this->checkInteger("-16", -16);
    this->checkInteger("-2147483648", -2147483647 - 1);
    this->checkInteger("1'000", 1000);
    // the octal literals are not supported, see issue #1460
    this->checkInteger("07", 7);
    this->checkInteger("010", 10);
    // the value of a binary or hexadecimal literal is that of the literal
    this->checkInteger("0b1010", 10);
    this->checkInteger("0B1010u", 10);
    this->checkInteger("0x10", 16);
    this->checkInteger("0X10", 16);
    this->checkInteger("0x1'FFF", 8191);
    this->checkInteger("0xffl", 255);
    this->checkUnsignedInteger("3u", 3u);
    this->checkUnsignedInteger("0x10ul", 16u);
    // a fractional part or an exponent is rejected, even if it does not change
    // the value of the literal
    for (const auto& s : {"3.0", "3.", "3e0", "1e5", "0x1p3"}) {
      tfel::utilities::CxxTokenizer t;
      t.parseString(s);
      auto p = t.begin();
      TFEL_TESTS_CHECK_THROW(t.readInt(p, t.end()), std::runtime_error);
    }
    this->checkDouble("1.5", 1.5);
    this->checkDouble("-2.5", -2.5);
    this->checkDouble(".5", 0.5);
    this->checkDouble("1.", 1.);
    this->checkDouble("3", 3.);
    this->checkDouble("1e5", 1e5);
    this->checkDouble("1e-5", 1e-5);
    // the suffix of a floating point literal is not a part of its value
    this->checkDouble("1.5f", 1.5);
    this->checkDouble("1.5F", 1.5);
    this->checkDouble("1.5l", 1.5);
    this->checkDouble("1e5f", 1e5);
    this->checkDouble("1E+5L", 1e5);
    this->checkDouble(".5e5f", 5e4);
    // the hexadecimal floating point literals
    this->checkDouble("0x1p3", 8.);
    this->checkDouble("0x1.8p3", 12.);
    this->checkDouble("0x1p-2", 0.25);
    this->checkDouble("0x1p3f", 8.);
    // the digit separators are not a part of the value of the token
    this->checkDouble("3.14'15", 3.1415);
    // the suffixes of the literal are not a part of its value: that of an
    // integer literal is read like that of a floating point literal
    this->checkDouble("3u", 3.);
    this->checkDouble("1l", 1.);
    this->checkDouble("0x10ul", 16.);
    // the value of a binary integer literal is read with the radix of its base
    // prefix, which the standard conversions do not read
    this->checkDouble("0b101", 5.);
    this->checkDouble("-0b101", -5.);
    this->checkDouble("-0x10", -16.);
    // the suffix of a user defined literal is not a part of the value of the
    // literal it follows
    this->checkInteger("1'000'000u_km", 1000000);
    this->checkUnsignedInteger("1'000'000u_km", 1000000u);
    this->checkDouble("1_km", 1.);
    this->checkDouble("1.5f_km", 1.5);
    this->checkDouble("1.5'0e3_km", 1500.);
    // a floating point literal is not the value of an integer
    {
      tfel::utilities::CxxTokenizer t;
      t.parseString("1.5f_km");
      auto p = t.begin();
      TFEL_TESTS_CHECK_THROW(t.readInt(p, t.end()), std::runtime_error);
      TFEL_TESTS_CHECK_THROW(t.readUnsignedInt(p, t.end()), std::runtime_error);
    }
  }  // end of testValues()
  // the suffixes which are extracted from the number tokens
  void testExtractingSuffixes() {
    // the type suffix of an integer literal
    this->checkSuffixes("3", "", "");
    this->checkSuffixes("3u", "u", "");
    this->checkSuffixes("3U", "U", "");
    this->checkSuffixes("1l", "l", "");
    this->checkSuffixes("1'000'000ull", "ull", "");
    this->checkSuffixes("0x10UL", "UL", "");
    this->checkSuffixes("0b101u", "u", "");
    // the type suffix of a floating point literal
    this->checkSuffixes("1.5", "", "");
    this->checkSuffixes("1.5f", "f", "");
    this->checkSuffixes("1.5F", "F", "");
    this->checkSuffixes("1e5l", "l", "");
    this->checkSuffixes("1E+5L", "L", "");
    this->checkSuffixes("3.", "", "");
    this->checkSuffixes("0x1.8p3", "", "");
    this->checkSuffixes("0x1.8p3f", "f", "");
    // the suffix of a user defined literal is never that of a floating point or
    // integer literal, but the latter is kept when it precedes it
    this->checkSuffixes("1_km", "", "_km");
    this->checkSuffixes("1'000'000u_km", "u", "_km");
    this->checkSuffixes("1.5f_km", "f", "_km");
    this->checkSuffixes("0x1p3_km", "", "_km");
    this->checkSuffixes("-1.5_imp_thing", "", "_imp_thing");
    this->checkSuffixes("2.e-5l_kg", "l", "_kg");
    // a token which is not a number has no suffix. `123abc` is split in a
    // number and an identifier, see `testLiteralsFollowedByIdentifiers`
    for (const auto& s : {"a", "\"a\"", "+"}) {
      tfel::utilities::CxxTokenizer t;
      t.parseString(s);
      TFEL_TESTS_ASSERT(!t.empty());
      TFEL_TESTS_CHECK_THROW(
          tfel::utilities::CxxTokenizer::extractNumberTypeSuffix(t[0]),
          std::runtime_error);
      TFEL_TESTS_CHECK_THROW(
          tfel::utilities::CxxTokenizer::extractNumberUserDefinedLiteral(t[0]),
          std::runtime_error);
    }
  }  // end of testExtractingSuffixes
  // the options used to read the value of a number token
  void testParseNumberArgument() {
    using CxxTokenizer = tfel::utilities::CxxTokenizer;
    const CxxTokenizer::ParseNumberArgument discard_all;
    auto keep_type_suffix = CxxTokenizer::ParseNumberArgument{};
    keep_type_suffix.discard_type_suffix = false;
    auto keep_user_defined_literal = CxxTokenizer::ParseNumberArgument{};
    keep_user_defined_literal.discard_user_defined_literal = false;
    {
      // a literal without any suffix: the options have no effect
      const auto tok = this->getNumberToken("42");
      TFEL_TESTS_ASSERT(CxxTokenizer::readInt(tok) == 42);
      TFEL_TESTS_ASSERT(CxxTokenizer::readUnsignedInt(tok) == 42u);
      this->checkValue(CxxTokenizer::readDouble(tok), 42.);
      TFEL_TESTS_ASSERT(CxxTokenizer::readInt(tok, keep_type_suffix) == 42);
      TFEL_TESTS_ASSERT(CxxTokenizer::readInt(tok, keep_user_defined_literal) ==
                        42);
      this->checkValue(CxxTokenizer::readDouble(tok, keep_type_suffix), 42.);
      this->checkValue(CxxTokenizer::readDouble(tok, keep_user_defined_literal),
                       42.);
    }
    {
      // an integer literal with a type suffix: its value is only read if such a
      // suffix can be discarded
      const auto tok = this->getNumberToken("3u");
      TFEL_TESTS_ASSERT(CxxTokenizer::readInt(tok) == 3);
      TFEL_TESTS_ASSERT(CxxTokenizer::readUnsignedInt(tok) == 3u);
      this->checkValue(CxxTokenizer::readDouble(tok), 3.);
      TFEL_TESTS_ASSERT(CxxTokenizer::readInt(tok, keep_user_defined_literal) ==
                        3);
      TFEL_TESTS_CHECK_THROW(CxxTokenizer::readInt(tok, keep_type_suffix),
                             std::runtime_error);
      TFEL_TESTS_CHECK_THROW(
          CxxTokenizer::readUnsignedInt(tok, keep_type_suffix),
          std::runtime_error);
      TFEL_TESTS_CHECK_THROW(CxxTokenizer::readDouble(tok, keep_type_suffix),
                             std::runtime_error);
    }
    {
      // a floating point literal with a type suffix: its value is never that of
      // an integer
      const auto tok = this->getNumberToken("1.5f");
      this->checkValue(CxxTokenizer::readDouble(tok), 1.5);
      this->checkValue(CxxTokenizer::readDouble(tok, keep_user_defined_literal),
                       1.5);
      TFEL_TESTS_CHECK_THROW(CxxTokenizer::readDouble(tok, keep_type_suffix),
                             std::runtime_error);
      TFEL_TESTS_CHECK_THROW(CxxTokenizer::readInt(tok), std::runtime_error);
    }
    {
      // a user defined literal: its suffix is only read if it can be discarded,
      // whether the literal it follows has a type suffix or not
      const auto tok = this->getNumberToken("1'000'000u_km");
      TFEL_TESTS_ASSERT(CxxTokenizer::readInt(tok) == 1000000);
      TFEL_TESTS_ASSERT(CxxTokenizer::readUnsignedInt(tok) == 1000000u);
      this->checkValue(CxxTokenizer::readDouble(tok), 1e6);
      TFEL_TESTS_CHECK_THROW(
          CxxTokenizer::readInt(tok, keep_user_defined_literal),
          std::runtime_error);
      TFEL_TESTS_CHECK_THROW(
          CxxTokenizer::readDouble(tok, keep_user_defined_literal),
          std::runtime_error);
      // both of the suffixes of the spelling of the literal prevent reading its
      // value when none of them can be discarded
      auto keep_both = keep_type_suffix;
      keep_both.discard_user_defined_literal = false;
      TFEL_TESTS_CHECK_THROW(CxxTokenizer::readInt(tok, keep_both),
                             std::runtime_error);
      TFEL_TESTS_CHECK_THROW(CxxTokenizer::readDouble(tok, keep_both),
                             std::runtime_error);
    }
    {
      // the value of a floating point user defined literal is read as a
      // floating point number, not as an integer
      const auto tok = this->getNumberToken("1.5f_km");
      this->checkValue(CxxTokenizer::readDouble(tok), 1.5);
      TFEL_TESTS_CHECK_THROW(CxxTokenizer::readInt(tok), std::runtime_error);
    }
    // the value of a binary or hexadecimal integer literal is read with its
    // radix, whatever the reader is: such a literal has no type suffix and can
    // not be a user defined literal
    for (const auto& v : {std::pair{"0b101", 5}, std::pair{"0x10", 16}}) {
      const auto tok = this->getNumberToken(v.first);
      TFEL_TESTS_ASSERT(CxxTokenizer::readInt(tok) == v.second);
      TFEL_TESTS_ASSERT(CxxTokenizer::readUnsignedInt(tok) ==
                        static_cast<unsigned int>(v.second));
      this->checkValue(CxxTokenizer::readDouble(tok),
                       static_cast<double>(v.second));
      this->checkValue(CxxTokenizer::readDouble(tok, keep_type_suffix),
                       static_cast<double>(v.second));
      TFEL_TESTS_ASSERT(CxxTokenizer::readInt(tok, keep_user_defined_literal) ==
                        v.second);
    }
    this->checkValue(CxxTokenizer::readDouble(this->getNumberToken("-0b101")),
                     -5.);
    this->checkValue(CxxTokenizer::readDouble(this->getNumberToken("-0x10")),
                     -16.);
  }  // end of testParseNumberArgument
};   // end of struct CxxTokenizerNumbersTest

TFEL_TESTS_GENERATE_PROXY(CxxTokenizerNumbersTest, "CxxTokenizerNumbersTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("CxxTokenizerNumbersTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
