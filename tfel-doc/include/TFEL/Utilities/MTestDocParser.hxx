/*!
 * \file   tfel-doc/include/TFEL/Utilities/MTestDocParser.hxx
 * \brief
 * \author Thomas Helfer
 * \brief  28 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_MTESTDOCPARSER_HXX
#define LIB_TFEL_UTILITIES_MTESTDOCPARSER_HXX

#include "TFEL/Utilities/ParserBase.hxx"
#include "TFEL/Utilities/TestDocumentation.hxx"

#include "MTest/MTest.hxx"

namespace tfel::utilities {

  /*!
   * \brief structure in charge of reading a test documentation file
   */
  struct MTestDocParser : protected mtest::MTest {
    /*!
     * \param f : file to be read
     */
    MTestDocParser(const std::string&);
    /*!
     * add a new test documentation
     */
    virtual void addDocumentation(
        std::map<std::string, std::vector<TestDocumentation>>&);
    //! \brief destructor
    ~MTestDocParser() override;

   private:
    const std::string file;
  };  // end of struct MTestDocParser

}  // end of namespace tfel::utilities

#endif /* LIB_TFEL_UTILITIES_MTESTDOCPARSER_H */
