/*! 
 * \file  tfel-doc/include/TFEL/Utilities/TestDocParser.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
 */

#ifndef _LIB_TFEL_UTILITIES_TESTDOCPARSER_H_
#define _LIB_TFEL_UTILITIES_TESTDOCPARSER_H_ 

#include"TFEL/Utilities/ParserBase.hxx"
#include"TFEL/Utilities/TestDocumentation.hxx"

namespace tfel{

  namespace utilities {

    /*!
     * Structure in charge of reading a test documentation file
     */
    struct TestDocParser
      : public ParserBase
    {
      /*!
       * \param f : file to be read
       */
      TestDocParser(const std::string&);
      /*!
       * add the documentation of the tests
       */
      virtual void
      addDocumentation(std::map<std::string,std::vector<TestDocumentation> >& tests);
      /*!
       * desctructor
       */
      virtual ~TestDocParser();
    private:
      
      void
      treatTest(TestDocumentation& t,
		std::string& c,
		const_iterator& p);

      void
      getStringValue(std::string& s,
		     const std::string& name,
		     const std::string& v,
		     const_iterator& p);

      void
      treatDescription(std::map<std::string,std::string>& dm,
		       std::string& n,
		       const_iterator& p);
      
      void
      treatIndex(std::map<std::string,std::vector<std::string> >& i,
		 const std::string& n,
		 const_iterator& p,
		 const bool b);
      
    }; // end of struct TestDocParser

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_UTILITIES_TESTDOCPARSER_H */

