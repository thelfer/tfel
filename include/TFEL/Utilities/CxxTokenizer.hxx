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

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/Token.hxx"

namespace tfel{

  namespace utilities{

    struct TFEL_VISIBILITY_EXPORT CxxTokenizer
    {
    
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

      static const char Separator[20] TFEL_VISIBILITY_LOCAL;

      static const std::string TFEL_VISIBILITY_LOCAL
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

