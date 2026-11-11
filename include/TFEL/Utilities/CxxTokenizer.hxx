/*!
 * \file   include/TFEL/Utilities/CxxTokenizer.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date 12/11/2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_CXXTOKENIZER_HXX
#define LIB_TFEL_UTILITIES_CXXTOKENIZER_HXX

#include <map>
#include <string>
#include <vector>
#include <iosfwd>
#include <string_view>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Utilities/Token.hxx"
#include "TFEL/Utilities/CxxTokenizerOptions.hxx"

namespace tfel::utilities {

  /*!
   * \brief class in charge of reading a C++-style file and converting it
   * in tokens.
   *
   * The value of a number token does not hold the digit separators of the
   * `C++14` integer literals: `@IterMax 1'000;` gives the number token
   * `1000`, whose value is therefore not the substring of the line taken at
   * the offset of this token. The offsets of the tokens remain those of the
   * file, whatever the numbers which precede them, so that a source code can
   * be rewritten from the offsets of its tokens, as the `MFront` interfaces
   * for `Abaqus`, `Aster` and `CalculiX` do.
   */
  struct TFELUTILITIES_VISIBILITY_EXPORT CxxTokenizer
      : protected CxxTokenizerOptions {
    //! a simple alias
    using TokensContainer = std::vector<Token>;
    //! a simple alias
    using const_iterator = TokensContainer::const_iterator;
    //! a simple alias
    using size_type = TokensContainer::size_type;
    //! \brief options used to parse an identifier
    struct IsValidIdentifierOptions {
      //! \brief treat `.` as a separator
      bool allowDotCharacter = false;
      //! \brief treat `+` as a separator
      bool allowPlusSign = false;
      //! \brief treat `-` as a separator
      bool allowMinusSign = false;
      // \brief boolean stating if C++ keywords are allowed
      bool allowCxxKeywords = true;
    };
    /*!
     * \return true if the given string is a valid variable name.
     * \param[in] s : variable name
     * \param[in] opts : options
     */
    static bool isValidIdentifier(std::string_view,
                                  const IsValidIdentifierOptions &);
    /*!
     * \return true if the given string is a valid variable name.
     * \param[in] s : variable name
     * \param[in] b : boolean telling if C++ keywords are allowed.
     */
    static bool isValidIdentifier(std::string_view, const bool = true);
    /*!
     * \brief an helper method throwing an exception if the given
     * iterator is egal to the third argument (which shall point
     * after the last token of the line)
     * \param[in] method : calling method name
     * \param[in] p  : iterator to the current token
     * \param[in] pe : iterator pointing after the last token of the
     * line
     * \throw std::runtime_error if the given iterator is egal to
     * the third argument
     */
    static void checkNotEndOfLine(const std::string &,
                                  const_iterator &,
                                  const const_iterator);
    /*!
     * \brief an helper method throwing an exception if the given
     * iterator is egal to the fourth argument (which shall point
     * after the last token of the line)
     * \param[in] method : calling method name
     * \param[in] error  : error message
     * \param[in] p  : iterator to the current token
     * \param[in] pe : iterator pointing after the last token of the
     * line
     * \throw std::runtime_error if the given iterator is egal to
     * the fourth argument
     */
    static void checkNotEndOfLine(const std::string &,
                                  const std::string &,
                                  const_iterator &,
                                  const const_iterator);
    /*!
     * \brief an helper method throwing an exception if the given
     * iterator is egal to the fourth argument (which shall point
     * after the last token of the line) or if the given token is
     * not equal to the expected value
     * \param[in] method : calling method name
     * \param[in] value  : expected value
     * \param[in] p  : iterator to the current token
     * \param[in] pe : iterator pointing after the last token of the
     * line
     * \throw std::runtime_error if the given iterator is egal to
     * the fourth argument
     */
    static void readSpecifiedToken(const std::string &,
                                   const std::string &,
                                   const_iterator &,
                                   const const_iterator);
    /*!
     * \brief an helper method to extract an unsigned int from the
     * given token
     * \param[in] p  : iterator to the current token
     * \param[in] pe : iterator pointing after the last token of the
     * line
     * \return the extracted value
     * \throw std::runtime_error if the given iterator is egal to
     * the second argument
     */
    static unsigned int readUnsignedInt(const_iterator &, const const_iterator);
    /*!
     * \brief an helper method to extract a string from the
     * given token
     * \param[in] p  : iterator to the current token
     * \param[in] pe : iterator pointing after the last token of the
     * line
     * \return the extracted value
     * \throw std::runtime_error if the given iterator is egal to
     * the second argument or if the given token is not a string
     */
    static std::string readString(const_iterator &, const const_iterator);
    /*!
     * \brief an helper method to extract a set of values from the
     * given as an array
     * \param[in] method : calling method name
     * \param[in]  p  : iterator to the current token
     * \param[in]  pe : iterator pointing after the last token of the
     * line
     * \throw std::runtime_error if the given iterator is egal to
     * the second argument or if the given token is not a string
     */
    static std::vector<std::string> readArray(const std::string &,
                                              const_iterator &,
                                              const const_iterator);
    /*!
     * \brief an helper method to extract a set of values from the
     * given as an array
     * \param[in] method : calling method name
     * \param[out] v  : the extracted array
     * \param[in]  p  : iterator to the current token
     * \param[in]  pe : iterator pointing after the last token of the
     * line
     * \throw std::runtime_error if the given iterator is egal to
     * the second argument or if the given token is not a string
     */
    static void readArray(const std::string &,
                          std::vector<std::string> &,
                          const_iterator &,
                          const const_iterator);
    /*!
     * \brief extract a list of token
     * \param[in]  m  : calling method name (used for error message)
     * \param[in]  db : beginning delimiter
     * \param[in]  de : end delimiter
     * \param[in]  p  : iterator to the current token
     * \param[in]  pe : iterator pointing after the last token of the
     * line
     */
    static std::vector<Token> readList(const std::string &,
                                       const std::string &,
                                       const std::string &,
                                       const_iterator &,
                                       const const_iterator);
    /*!
     * \brief extract a list of token
     * \param[out] l  : list read
     * \param[in]  m  : calling method name (used for error message)
     * \param[in]  db : beginning delimiter
     * \param[in]  de : end delimiter
     * \param[in]  p  : iterator to the current token
     * \param[in]  pe : iterator pointing after the last token of the
     * line
     */
    static void readList(std::vector<Token> &,
                         const std::string &,
                         const std::string &,
                         const std::string &,
                         const_iterator &,
                         const const_iterator);
    /*!
     * \brief an helper method to extract a set of values from the
     * given as an array
     * \param[out] v  : the extracted array
     * \param[in]  p  : iterator to the current token
     * \param[in]  pe : iterator pointing after the last token of the
     * line
     * \throw std::runtime_error if the given iterator is egal to
     * the second argument or if the given token is not a string
     */
    static std::vector<std::string> readStringArray(const_iterator &,
                                                    const const_iterator);
    /*!
     * \brief an helper method to extract a double from the given
     * token
     * \param[in] p  : iterator to the current token
     * \param[in] pe : iterator pointing after the last token of the
     * line
     * \return the extracted value
     * \throw std::runtime_error if the given iterator is egal to
     * the second argument
     */
    static double readDouble(const_iterator &, const const_iterator);
    /*!
     * \brief an helper method to extract a int from the given
     * token
     * \param[in] p  : iterator to the current token
     * \param[in] pe : iterator pointing after the last token of the
     * line
     * \return the extracted value
     * \throw std::runtime_error if the given iterator is egal to
     * the second argument
     */
    static int readInt(const_iterator &, const const_iterator);
    //! \brief default constructor
    CxxTokenizer();
    /*!
     * \brief constructor with options
     * \param[in] o: options
     */
    explicit CxxTokenizer(const CxxTokenizerOptions &);
    /*!
     * \brief constructor from a file
     * \param[in] f : file name
     */
    explicit CxxTokenizer(const std::string &);
    /*!
     * \brief constructor from a file
     * \param[in] f : file name
     * \param[in] o: options
     */
    CxxTokenizer(const std::string &, const CxxTokenizerOptions &);
    /*!
     * \brief import the specified tokens
     * \param[in] p : iterator to the first token
     * \param[in] pe : iterator past the last token
     */
    template <typename InputIterator>
    void import(const InputIterator, const InputIterator);
    /*!
     * \brief analyse the string given
     * all previous tokens are erased
     *
     * The line numbers of the tokens of the string start at the first line:
     * tokenizing a file with several calls to this method, a call per line,
     * gives the same tokens as tokenizing its whole contents with a single
     * call, but the lines of those tokens are those of the line the call is
     * given. A caller who wants to keep the line numbering of a file shall use
     * the parseStream method, which takes the current line number as an
     * argument.
     *
     * \param[in] s : string to be parsed
     */
    void parseString(const std::string &);
    /*!
     * \brief import the specified file
     * \param[in] f : file name
     */
    void openFile(const std::string &);
    /*!
     * \brief print the stored tokens to the specified stream
     * \param[out] out : output stream
     */
    void printFileTokens(std::ostream &) const;
    /*!
     * \brief assume that a C-style comment was opened at the
     * beginning of the processing.
     * This is usefull when parsing a file line by line
     * \param[in] b: boolean
     */
    void setCStyleCommentOpened(const bool);
    /*!
     * \return is a C-style comment is opened at the
     * the end of the processing.
     * This is usefull when parsing a file line by line
     *
     * A `C` style comment may span several lines: a comment which is not
     * closed on the line where it opens does not make the tokenizer throw.
     * The state of the tokenizer is left open and the lines which follow are
     * gathered in the value of the comment token until the comment is closed,
     * which is what allows tokenizing a file line by line. A file which ends
     * inside a comment is thus accepted, on the contrary to a quoted string or
     * to a character literal which are rejected when they are not closed in
     * their line: the caller is responsible for checking the state of the
     * tokenizer when the end of the file is reached, as this method does. See
     * issue #1395.
     */
    [[nodiscard]] bool isCStyleCommentOpened() const;
    /*!
     * \brief assume that a raw string was opened at the
     * beginning of the processing.
     *
     * This is usefull when parsing a file line by line
     *
     * \param[in] b: boolean
     *
     * \note the raw string delimiter is set empty.
     */
    void setRawStringOpened(const bool);
    /*!
     * \brief set the raw string deliminter. This
     * assume that a raw string was opened at the
     * beginning of the processing, i.e. sets
     * the `rawStringOpened` boolean to true.
     *
     * \param[in] d: new delimiter
     *
     * \note the raw string delimiter is set empty.
     */
    void setRawStringDelimiter(std::string_view);
    /*!
     * \return is a raw string is opened at the
     * the end of the processing.
     * This is usefull when parsing a file line by line
     *
     * The raw string literals behave as the `C` style comments: a literal
     * which is not closed when the end of the contents is reached does not
     * make the tokenizer throw and leaves the state of the tokenizer open. A
     * file which ends inside a raw string literal is thus accepted: the caller
     * is responsible for checking the state of the tokenizer when the end of
     * the file is reached, as this method does, `getCurrentRawStringDelimiter`
     * giving the delimiter of the opened literal. See issue #1395.
     */
    [[nodiscard]] bool isRawStringOpened() const;
    /*!
     * \return the current raw string delimiter.
     * This is usefull when parsing a file line by line
     */
    [[nodiscard]] std::string getCurrentRawStringDelimiter() const noexcept;
    /*!
     * \brief remove all comment tokens
     *
     * The values of the removed comments of the kind Token::Comment are
     * gathered in the comments data member: a line may hold several such
     * comments, which are gathered in the order they appear in the line,
     * separated by a space character.
     */
    void stripComments();
    /*!
     * \brief clear the object
     */
    void clear();
    /*!
     * \brief set if char tokens shall be treated as string tokens
     * \param[in] b : boolean telling if char tokens shall be
     * treated as string tokens
     */
    void treatCharAsString(const bool);
    /*!
     * \brief set if the comment boundaries must be left in the
     * resulting tokens.
     * \param[in] b : boolean telling if char tokens shall be
     * treated as string tokens
     */
    void keepCommentBoundaries(const bool);
    /*!
     * \brief set if '.' shall be treated as as sperator
     * \param[in] b : boolean telling if '.' shall be treated as as
     * sperator
     */
    void treatDotAsSeparator(const bool);
    /*!
     * \brief set if '+' shall be treated as as sperator
     * \param[in] b : boolean telling if '+' shall be treated as as
     * separator
     */
    void treatPlusAsSeparator(const bool);
    /*!
     * \brief set if '-' shall be treated as as sperator
     * \param[in] b : boolean telling if '-' shall be treated as as
     * separator
     */
    void treatMinusAsSeparator(const bool);
    /*!
     * \brief set if successive strings shall be merged
     * \param[in] b : boolean
     */
    void mergeStrings(const bool);
    /*!
     * \brief set if tokens shall be split to extract numbers
     * \param[in] b : boolean telling numbers shall be extracted
     * from tokens
     */
    void extractNumbers(const bool);
    /*!
     * \brief add a new separator
     * \param[in] s: separator
     */
    void addSeparator(const std::string &);
    /*!
     * \brief apply the given substitutions by calling `replace_all` on the
     * values hold by the standard and string tokens.
     *
     * \param[in] substitutions: list of substitutions
     */
    void substitute(const std::map<std::string, std::string> &);
    //! \return true if the token list is empty
    [[nodiscard]] bool empty() const;
    //! \return an iterator to the first token
    [[nodiscard]] const_iterator begin() const;
    //! \return an iterator past the last token
    [[nodiscard]] const_iterator end() const;
    //! \return an iterator to the first token
    [[nodiscard]] const_iterator cbegin() const;
    //! \return an iterator past the last token
    [[nodiscard]] const_iterator cend() const;
    /*!
     * \return the ith token
     * \param[in] i : index
     * \throw an exception if i>=this->size()
     */
    [[nodiscard]] const Token &operator[](const size_type) const;
    //! \return the number of token
    [[nodiscard]] size_type size() const;
    //! \brief destructor
    virtual ~CxxTokenizer();

   protected:
    [[nodiscard]] static bool isValidFunctionIdentifier(const std::string &,
                                                        const bool = true);
    /*!
     * \brief tokenize the lines of a stream
     *
     * The lines are tokenized from the current position of the stream and n is
     * increased by the number of lines which have been tokenized. The last line
     * of a stream may not be terminated by an end of line character: such a
     * line is tokenized as the others. On the contrary, an end of line
     * character at the very end of a stream does not introduce an additional
     * empty line, so that tokenizing a stream gives the same tokens as
     * tokenizing its contents line by line, up to the line numbers, which are
     * those of the stream.
     *
     * \param[in]     in:   input stream
     * \param[in,out] n:    line number
     * \param[in]     from: origin of the stream
     */
    virtual void parseStream(std::istream &,
                             Token::size_type &,
                             const std::string &);
    /*!
     * \param[in] l:    line
     * \param[in] n:    line number
     * \param[in] from: origin of the stream
     */
    virtual void splitLine(const std::string &, const Token::size_type);

    virtual void parseChar(Token::size_type &,
                           std::string::const_iterator &,
                           const std::string::const_iterator,
                           const Token::size_type);
    virtual void parseString(Token::size_type &,
                             std::string::const_iterator &,
                             const std::string::const_iterator,
                             const Token::size_type,
                             const char);
    virtual void parseCComment(Token::size_type &,
                               std::string::const_iterator &,
                               const std::string::const_iterator,
                               const Token::size_type);
    virtual void parseCxxComment(Token::size_type &,
                                 std::string::const_iterator &,
                                 const std::string::const_iterator,
                                 const Token::size_type);
    virtual void try_join(Token::size_type &,
                          std::string::const_iterator &,
                          const std::string::const_iterator,
                          const Token::size_type,
                          const char);
    virtual void try_join(Token::size_type &,
                          std::string::const_iterator &,
                          const std::string::const_iterator,
                          const Token::size_type,
                          const char,
                          const char);
    /*!
     * \brief read a number
     *
     * The digit separators of the `C++14` integer literals are removed from
     * the value of the token: `1'000` gives the value `1000`, see the
     * description of the class.
     *
     * \param[in,out] o:  offset in the line
     * \param[in,out] p:  iterator to the current character
     * \param[in]     pe: iterator past the end of the line
     * \param[in]     n:  line number
     */
    virtual void parseNumber(Token::size_type &,
                             std::string::const_iterator &,
                             const std::string::const_iterator,
                             const Token::size_type);
    virtual void parsePreprocessorDirective(Token::size_type &,
                                            std::string::const_iterator &,
                                            const std::string::const_iterator,
                                            const std::string::const_iterator,
                                            const Token::size_type);
    virtual void parseStandardLine(Token::size_type &,
                                   std::string::const_iterator &,
                                   const std::string::const_iterator,
                                   const std::string::const_iterator,
                                   const Token::size_type);
    /*!
     * \brief read the contents of a raw string literal, which starts at the
     * current iterator, past the opening `R"delim(` prefix
     *
     * The value of the token holds the whole literal, from the `R` delimiter
     * to the closing `"` included, as for a quoted string, and its offset is
     * that of the `R`, see issue #1390. The opening prefix is restored from the
     * delimiter of the literal, which the caller has read and kept in
     * `currentRawStringDelimiter`.
     *
     * \param[in,out] o:  current offset in line
     * \param[in,out] p:  current iterator
     * \param[in]     pe: iterator past the last character in the line
     * \param[in]     n:  current line number
     * \param[in]     ro: offset of the `R` which opens the literal
     */
    virtual void parseRawString(Token::size_type &,
                                std::string::const_iterator &,
                                const std::string::const_iterator,
                                const Token::size_type,
                                const Token::size_type);
    std::vector<std::string> additional_separators;
    //! list of tokens read
    TokensContainer tokens;
    /*!
     * \brief store the comments which have been removed by the stripComments
     * method. The key of the map is the line a comment starts on and the value
     * is the values of the comments of this line, in the order they appear in
     * the line, separated by a space character.
     *
     * The empty comments, that is a `//` comment or a `C` style comment whose
     * contents is empty, bring nothing and are then not stored: a line whose
     * only comment is empty gives no entry in this map.
     */
    std::map<Token::size_type, std::string> comments;
    /*!
     * \brief if true, a cStyle comment what opened when the last line
     * treatment was finised
     */
    bool cStyleCommentOpened = false;
    /*!
     * if true, a cStyle comment what opened when the last line
     * treatment was finised
     */
    bool rawStringOpened = false;
    //! \brief name of the current delimiter for a raw string
    std::string currentRawStringDelimiter;

  };  // end of struct CxxTokenizer

}  // end of namespace tfel::utilities

#include "TFEL/Utilities/CxxTokenizer.ixx"

#endif /* LIB_TFEL_UTILITIES_CXXTOKENIZER_HXX */
