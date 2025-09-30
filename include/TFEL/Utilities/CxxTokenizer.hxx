/*!
 * \file   include/TFEL/Utilities/CxxTokenizer.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   12 nov 2006
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
   * class in charge of reading a C++-style file and converting it
   * in tokens.
   */
  struct TFELUTILITIES_VISIBILITY_EXPORT CxxTokenizer
      : protected CxxTokenizerOptions {
    //! a simple alias
    using TokensContainer = std::vector<Token>;
    //! a simple alias
    using const_iterator = TokensContainer::const_iterator;
    //! a simple alias
    using size_type = TokensContainer::size_type;
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
    CxxTokenizer(const CxxTokenizerOptions &);
    /*!
     * \brief constructor from a file
     * \param[in] f : file name
     */
    CxxTokenizer(const std::string &);
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
     * \param[in] b: boolean
     */
    bool isCStyleCommentOpened() const;
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
     * \param[in] b: boolean
     */
    bool isRawStringOpened() const;
    /*!
     * \return the current raw string delimiter.
     * This is usefull when parsing a file line by line
     */
    std::string getCurrentRawStringDelimiter() const noexcept;
    /*!
     * \brief remove all comment tokens
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
    //! \return true if the token list is empty
    bool empty() const;
    //! \return an iterator to the first token
    const_iterator begin() const;
    //! \return an iterator past the last token
    const_iterator end() const;
    //! \return an iterator to the first token
    const_iterator cbegin() const;
    //! \return an iterator past the last token
    const_iterator cend() const;
    /*!
     * \return the ith token
     * \param[in] i : index
     * \throw an exception if i>=this->size()
     */
    const Token &operator[](const size_type) const;
    //! \return the number of token
    size_type size() const;
    //! \brief destructor
    virtual ~CxxTokenizer();

   protected:
    static bool isValidFunctionIdentifier(const std::string &,
                                          const bool = true);
    /*!
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
     * \brief parse a raw string
     * \param[in,out] o: current offset in line
     * \param[in,out] p: current iterator
     * \param[in,out] pe: iterator past the last character in the line
     * \param[in,out] n: current line number
     */
    virtual void parseRawString(Token::size_type &,
                                std::string::const_iterator &,
                                const std::string::const_iterator,
                                const Token::size_type);
    std::vector<std::string> additional_separators;
    //! list of tokens read
    TokensContainer tokens;
    //! store all the comments of a line
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
