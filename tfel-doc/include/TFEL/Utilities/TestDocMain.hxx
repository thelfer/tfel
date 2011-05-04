/*! 
 * \file  TestDocMain.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
 */

#ifndef _LIB_TFEL_UTILITIES_TESTDOCMAIN_H_
#define _LIB_TFEL_UTILITIES_TESTDOCMAIN_H_ 

#include<string>
#include<ostream>
#include<fstream>

#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Utilities/ArgumentParserBase.hxx"

namespace tfel{

  namespace utilities{

    struct TestDocMain
      : public ArgumentParserBase<TestDocMain>
    {

      TestDocMain(const int, const char *const *const);

      void treatUnknownArgument(void);
      
      int execute(void);
      
      ~TestDocMain();

    private :
      
      void registerArgumentCallBacks(void);    

      std::string
      getVersionDescription(void) const;
      
      std::string
      getUsageDescription(void) const;
      
      void treatSrc(void);

      void treatLang(void);

      void treatLogFile(void);

      void treatKeyFile(void);

      void treatCategoryFile(void);
      
      void treatTranslationFile(void);

      std::string lang;

      std::string srcdir;

      std::string outputFile;

      std::string logFile;

      std::ofstream  output;

      std::ostream* log;

      tfel::utilities::shared_ptr<std::ostream> logptr;

    }; // end of struct TestDocMain
    
  } // end of namespace utilities
  
} // end of namespace tfel

#endif /* _LIB_TFEL_UTILITIES_TESTDOCMAIN_H */

