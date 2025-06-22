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

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Utilities/Token.hxx"

namespace tfel {

  namespace utilities {

    /*!
     * class in charge of reading a C++-style file and converting it
     * in tokens.
     */
    struct TFELUTILITIES_VISIBILITY_EXPORT CxxTokenizer {
      //! a simple alias
      typedef std::vector<Token> TokensContainer;
      //! a simple alias
      typedef TokensContainer::const_iterator const_iterator;
      //! a simple alias
      typedef TokensContainer::size_type size_type;
      /*!
       * \return true if the given string is a valid variable name.
       * \param[in] s : variable name
       * \param[in] b : boolean telling if C++ keywords are allowed.
       */
      static bool isValidIdentifier(const std::string&, const bool = true);
      /*!
       * \brief default constructor
       */
      CxxTokenizer();

      /*!
       * \brief constructor
       * Open the given file.
       * \param[in] f : file name
       */
      CxxTokenizer(const std::string&);

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
      void parseString(const std::string&);
      /*!
       * \brief import the specified file
       * \param[in] f : file name
       */
      void openFile(const std::string&);
      /*!
       * \brief print the stored tokens to the specified stream
       * \param[out] out : output stream
       */
      void printFileTokens(std::ostream&) const;
      /*!
       * \brief assume that a C-style comment was opened at the
       * beginning of the processing.
       * This is usefull when parsing a file line by line
       * \param[in] b: boolean
       */
      void setCStyleCommentOpened(const bool);
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
       * \brief set if '.' shall be treated as as sperator
       * \param[in] b : boolean telling if '.' shall be treated as as sperator
       */
      void treatDotAsSeparator(const bool);
      /*!
       * \brief set if '-' shall be treated as as sperator
       * \param[in] b : boolean telling if '-' shall be treated as as sperator
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
      static void checkNotEndOfLine(const std::string&,
                                    const_iterator&,
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
      static void checkNotEndOfLine(const std::string&,
                                    const std::string&,
                                    const_iterator&,
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
      static void readSpecifiedToken(const std::string&,
                                     const std::string&,
                                     const_iterator&,
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
      static unsigned int readUnsignedInt(const_iterator&,
                                          const const_iterator);
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
      static std::string readString(const_iterator&, const const_iterator);
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
      static std::vector<std::string> readArray(const std::string&,
                                                const_iterator&,
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
      static void readArray(const std::string&,
                            std::vector<std::string>&,
                            const_iterator&,
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
      static std::vector<Token> readList(const std::string&,
                                         const std::string&,
                                         const std::string&,
                                         const_iterator&,
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
      static void readList(std::vector<Token>&,
                           const std::string&,
                           const std::string&,
                           const std::string&,
                           const_iterator&,
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
      static std::vector<std::string> readStringArray(const_iterator&,
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
      static double readDouble(const_iterator&, const const_iterator);
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
      static int readInt(const_iterator&, const const_iterator);
      //! \return an iterator to the first token
      const_iterator begin() const;
      //! \return an iterator past the last token
      const_iterator end() const;
      /*!
       * \return the ith token
       * \param[in] i : index
       * \throw an exception if i>=this->size()
       */
      const Token& operator[](const size_type) const;
      //! \return the number of token
      size_type size() const;
      //! destructor
      virtual ~CxxTokenizer();

     protected:
      static bool isValidFunctionIdentifier(const std::string&,
                                            const bool = true);
      /*!
       * \param[in] in:   input stream
       * \param[in] n:    line number
       * \param[in] from: origin of the stream
       */
      virtual void parseStream(std::istream&,
                               Token::size_type&,
                               const std::string&);

      virtual void splitLine(const std::string&, const Token::size_type);

      virtual void treatChar(Token::size_type&,
                             std::string::const_iterator&,
                             const std::string::const_iterator,
                             const Token::size_type);
      virtual void treatString(Token::size_type&,
                               std::string::const_iterator&,
                               const std::string::const_iterator,
                               const Token::size_type,
                               const char);
      virtual void treatCComment(Token::size_type&,
                                 std::string::const_iterator&,
                                 const std::string::const_iterator,
                                 const Token::size_type);
      virtual void treatCxxComment(Token::size_type&,
                                   std::string::const_iterator&,
                                   const std::string::const_iterator,
                                   const Token::size_type);
      virtual void try_join(Token::size_type&,
                            std::string::const_iterator&,
                            const std::string::const_iterator,
                            const Token::size_type,
                            const char);
      virtual void try_join(Token::size_type&,
                            std::string::const_iterator&,
                            const std::string::const_iterator,
                            const Token::size_type,
                            const char,
                            const char);
      virtual void readNumber(Token::size_type&,
                              std::string::const_iterator&,
                              const std::string::const_iterator,
                              const Token::size_type);
      virtual void treatPreprocessorDirective(Token::size_type&,
                                              std::string::const_iterator&,
                                              const std::string::const_iterator,
                                              const std::string::const_iterator,
                                              const Token::size_type);
      virtual void treatStandardLine(Token::size_type&,
                                     std::string::const_iterator&,
                                     const std::string::const_iterator,
                                     const std::string::const_iterator,
                                     const Token::size_type);
      //! list of tokens read
      TokensContainer tokens;
      //! store all the comments of a line
      std::map<Token::size_type, std::string> comments;
      //! treat '.' as a c++ separator
      bool dotAsSeparator = true;
      //! treat '-' as a c++ separator
      bool minusAsSeparator = true;
      /*!
       * if true, one consider that single quote can be used to define
       * a string
       */
      bool charAsString = false;
      /*!
       * if true, a cStyle comment what opened when the last line
       * treatment was finised
       */
      bool cStyleCommentOpened = false;
      //! if true, successive strings are merged
      bool shallMergeStrings = false;
    };  // end of struct CxxTokenizer

  }  // end of namespace utilities

}  // end of namespace tfel

#include "TFEL/Utilities/CxxTokenizer.ixx"

#endif /* LIB_TFEL_UTILITIES_CXXTOKENIZER_HXX */
