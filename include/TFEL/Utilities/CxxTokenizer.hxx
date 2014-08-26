/*!
 * \file   CxxTokenizer.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   12 nov 2006
 */

#ifndef _LIB_TFEL_CXXTOKENIZER_HXX_
#define _LIB_TFEL_CXXTOKENIZER_HXX_ 

#include<map>
#include<list>
#include<string>
#include<vector>
#include<ostream>
#include<fstream>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/Token.hxx"

namespace tfel{

  namespace utilities{

    /*!
     * class in charge of reading a C++-style file and converting it
     * in tokens.
     */
    struct TFELUTILITIES_VISIBILITY_EXPORT CxxTokenizer
    {
    
      //! a simple alias
      typedef std::list<Token> TokensContainer;
      //! a simple alias
      typedef TokensContainer::const_iterator const_iterator;
      //! a simple alias
      typedef TokensContainer::size_type size_type;

      /*!
       * \return true if the given string is a valid variable name.
       * \param[in] s : variable name
       * \param[in] b : boolean telling if C++ keywords are allowed.
       */
      static bool
      isValidIdentifier(const std::string&,const bool=true);

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
      template<typename InputIterator>
      void import(const InputIterator,
		  const InputIterator);
    
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
       * \brief print the stored tokens to the specified stream
       * \param[out] out : output stream
       */ 
      void beautifyCode(void);

      /*!
       * \brief remove all comment tokens
       */ 
      void stripComments(void);

      /*!
       * \brief clear the object
       */ 
      void clear(void);

      /*!
       * \brief set if char tokens shall be treated as string tokens
       * \param[in] b : boolean telling if char tokens shall be
       * treated as string tokens
       */ 
      void treatCharAsString(const bool);

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
      static void
      checkNotEndOfLine(const std::string&,
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
      static void
      checkNotEndOfLine(const std::string&,
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
      static void
      readSpecifiedToken(const std::string&,
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
      static unsigned int
      readUnsignedInt(const_iterator&, 
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
      static std::string
      readString(const_iterator&, 
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
      static void
      readArray(const std::string&,
		std::vector<std::string>&,
		const_iterator&, 
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
      static double
      readDouble(const_iterator&, 
		 const const_iterator);	
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
      static int
      readInt(const_iterator&, 
	      const const_iterator);	

      /*!
       * \return an iterator to the first token
       */ 
      const_iterator
      begin(void) const;

      /*!
       * \return an iterator past the last token
       */ 
      const_iterator
      end(void) const;

      /*!
       * \return the number of token
       */ 
      size_type
      size() const;

    private:

      static const char Separator[20] TFEL_VISIBILITY_LOCAL;

      static std::string TFEL_VISIBILITY_LOCAL
      readNumber(std::string::const_iterator&,
		 const std::string::const_iterator);

      static void TFEL_VISIBILITY_LOCAL
      extractNumbers(std::vector<std::string>&,
		     const std::string&);

      static void TFEL_VISIBILITY_LOCAL
      joinIncludes(std::vector<std::string>&);

      static void TFEL_VISIBILITY_LOCAL
      join(std::vector<std::string>&,const std::string&,const std::string&);

      static std::vector<std::string> TFEL_VISIBILITY_LOCAL
      splitStringAtSpaces(const std::string&);

      void TFEL_VISIBILITY_LOCAL
      splitAtCxxTokenSperator(std::vector<std::string>&);
    
      static void TFEL_VISIBILITY_LOCAL
      splitString(std::vector<std::string>&,std::string,const std::string);

      static void TFEL_VISIBILITY_LOCAL
      splitString2(std::vector<std::string>&,std::string);

      static void TFEL_VISIBILITY_LOCAL
      splitString3(std::vector<std::string>&,std::string);

      TFEL_VISIBILITY_LOCAL 
      static TokensContainer::iterator TFEL_VISIBILITY_LOCAL
      joinPreviousCurrentNext(TokensContainer&,TokensContainer&,
			      TokensContainer::iterator);

    protected:

      static bool
      isValidFunctionIdentifier(const std::string&,const bool=true);

      void
      splitLine(std::string,const unsigned int);

      void
      treatPreprocessorDirectives(void);

      void
      splitTokens(void);

      TokensContainer fileTokens;

      //! store all the comments of a line
      std::map<unsigned int,
	       std::string> comments;

      bool cStyleCommentOpened;

      bool bExtractNumbers;

      bool charAsString;

    }; // end of struct CxxTokenizer

  } // end of namespace utilities

} // end of namespace tfel

#include"TFEL/Utilities/CxxTokenizer.ixx"

#endif /* _LIB_TFEL_CXXTOKENIZER_HXX */
