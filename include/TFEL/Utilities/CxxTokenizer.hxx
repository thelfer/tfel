/*!
 * \file   CxxTokenizer.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   12 nov 2006
 */

#ifndef _LIB_TFEL_CPPTOKENIZER_HXX_
#define _LIB_TFEL_CPPTOKENIZER_HXX_ 

#include<string>
#include<vector>
#include<list>
#include<ostream>
#include<fstream>

#include"Token.hxx"

namespace tfel{

  namespace utilities{

    struct CxxTokenizer{
    
      typedef std::list<Token> TokensContainer;

      static bool
      isValidIdentifier(const std::string&,const bool=true);

      CxxTokenizer();

      CxxTokenizer(const std::string&);
    
      void openFile(const std::string&);

      void printFileTokens(std::ostream&) const;

      void beautifyCode(void);

      void stripComments(void);

      void clear(void);

      void treatCharAsString(const bool);

      void extractNumbers(const bool);

      void
      checkNotEndOfLine(const std::string&,
			TokensContainer::const_iterator&, 
			const TokensContainer::const_iterator);

      void
      checkNotEndOfLine(const std::string&,
			const std::string&,
			TokensContainer::const_iterator&, 
			const TokensContainer::const_iterator);

      void
      readSpecifiedToken(const std::string&,
			 const std::string&,
			 TokensContainer::const_iterator&, 
			 const TokensContainer::const_iterator);

      unsigned int
      readUnsignedInt(TokensContainer::const_iterator&, 
		      const TokensContainer::const_iterator);

      std::string
      readString(TokensContainer::const_iterator&, 
		 const TokensContainer::const_iterator);
      
      double
      readDouble(TokensContainer::const_iterator&, 
		 const TokensContainer::const_iterator);	

      TokensContainer::const_iterator
      begin(void) const;

      TokensContainer::const_iterator
      end(void) const;

    private:

      static const char Separator[20];

      static const std::string
      readNumber(std::string::const_iterator&,
		 const std::string::const_iterator);

      static void
      extractNumbers(std::vector<std::string>&,
		     const std::string&);

      static void 
      joinIncludes(std::vector<std::string>&);

      static void 
      join(std::vector<std::string>&,const std::string&,const std::string&);

      static std::vector<std::string>
      splitStringAtSpaces(const std::string&);

      void
      splitAtCxxTokenSperator(std::vector<std::string>&);
    
      static void
      splitString(std::vector<std::string>&,std::string,const std::string);

      static void
      splitString2(std::vector<std::string>&,std::string);

      static void
      splitString3(std::vector<std::string>&,std::string);

      static TokensContainer::iterator
      joinPreviousCurrentNext(TokensContainer&,TokensContainer&,
			      TokensContainer::iterator);

    protected:

      static bool
      isValidFunctionIdentifier(const std::string&,const bool=true);

      template<typename T>
      static std::string toString(const T&);

      void
      splitLine(std::string,const unsigned short);

      void
      treatPreprocessorDirectives(void);

      void
      splitTokens(void);

      TokensContainer fileTokens;

      bool cStyleCommentOpened;

      bool bExtractNumbers;

      bool charAsString;

    }; // end of struct CxxTokenizer

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_CPPTOKENIZER_HXX */

