/*!
 * \file  tfel-doc/include/TFEL/Utilities/TestDocMain.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 03 mai 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_TESTDOCMAIN_HXX
#define LIB_TFEL_UTILITIES_TESTDOCMAIN_HXX

#include <string>
#include <ostream>
#include <fstream>
#include <memory>

#include "TFEL/Utilities/ArgumentParserBase.hxx"
#include "TFEL/Utilities/GeneratorOptions.hxx"

namespace tfel::utilities {

  struct TestDocMain : public ArgumentParserBase<TestDocMain> {
    TestDocMain(const int, const char *const *const);

    void treatUnknownArgument() override;

    virtual int execute();

    ~TestDocMain() override;

   private:
    void registerArgumentCallBacks();

    std::string getVersionDescription() const override;

    std::string getUsageDescription() const override;

    void treatLaTeX();

    void treatMarkdown();

    void treatSplit();

    void treatFragment();

    void treatSrc();

    void treatLang();

    void treatLogFile();

    void treatPrefix();

    void treatKeyFile();

    void treatCategoryFile();

    void treatTranslationFile();

    void treatMTest();

    std::string srcdir;

    std::string outputDirectory;

    std::string outputFile;

    std::string logFile;

    std::ofstream output;

    std::ostream *log;

    std::shared_ptr<std::ostream> logptr;

    GeneratorOptions opts;

    bool latex = false;

    bool markdown = false;

    bool mtest = false;

  };  // end of struct TestDocMain

}  // end of namespace tfel::utilities

#endif /* LIB_TFEL_UTILITIES_TESTDOCMAIN_H */
