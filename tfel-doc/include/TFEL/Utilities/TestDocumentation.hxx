/*!
 * \file  tfel-doc/include/TFEL/Utilities/TestDocumentation.hxx
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

#ifndef LIB_TFEL_UTILITIES_TESTDOCUMENTATION_HXX
#define LIB_TFEL_UTILITIES_TESTDOCUMENTATION_HXX

#include <map>
#include <vector>
#include <string>
#include <ostream>

namespace tfel::utilities {

  struct TestDocumentation {
    TestDocumentation();
    TestDocumentation(const TestDocumentation&);
    TestDocumentation(TestDocumentation&&);
    TestDocumentation& operator=(const TestDocumentation&);
    TestDocumentation& operator=(TestDocumentation&&);
    //! destructor
    ~TestDocumentation();
    //! name of the test
    std::string name;
    //! date of creation
    std::string date;
    //! author of the test
    std::string author;
    //! location in the src
    std::string src;
    //! location in the install directory
    std::string install;
    /*!
     * list of tested models (if any).
     * - the keys are the models names.
     * - the values are a list of specific aspects
     *   tested used to index the test more
     *   precisely.
     */
    std::map<std::string, std::vector<std::string>> models;
    //! description of the test, index by languages
    std::map<std::string, std::string> descriptions;
    /*!
     * index keys :
     * - the keys of the map are the main entries
     * - the values of the map are sub-entries
     *   of the key
     */
    std::map<std::string, std::vector<std::string>> keys;
  };

}  // end of namespace tfel::utilities

#endif /* LIB_TFEL_UTILITIES_TESTDOCUMENTATION_H */
