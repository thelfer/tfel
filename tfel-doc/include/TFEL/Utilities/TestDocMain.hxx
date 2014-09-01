/*! 
 * \file  tfel-doc/include/TFEL/Utilities/TestDocMain.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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

      void treatSplit(void);

      void treatFragment(void);
      
      void treatSrc(void);

      void treatLang(void);

      void treatLogFile(void);

      void treatPrefix(void);

      void treatKeyFile(void);

      void treatCategoryFile(void);
      
      void treatTranslationFile(void);

      void treatMTest(void);

      std::string lang;

      std::string srcdir;

      std::string outputDirectory;

      std::string outputFile;

      std::string logFile;

      std::string prefix;

      std::ofstream  output;

      std::ostream* log;

      bool mtest;

      bool fragment;

      bool split;

      tfel::utilities::shared_ptr<std::ostream> logptr;

    }; // end of struct TestDocMain
    
  } // end of namespace utilities
  
} // end of namespace tfel

#endif /* _LIB_TFEL_UTILITIES_TESTDOCMAIN_H */

