/*!
 * \file   AbstractTestCaseFactory.hxx
 * \brief
 * \author Thomas Helfer
 * \date   17/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFM_TEST_GENERATOR_ABSTRACTTESTCASEFACTORY_HXX
#define LIB_MFM_TEST_GENERATOR_ABSTRACTTESTCASEFACTORY_HXX

#include <map>
#include <string>
#include <memory>
#include <functional>
#include "MFMTestGenerator/Config.hxx"
#include "MFMTestGenerator/TestCaseParameters.hxx"

namespace mfmtg {

  // forward declaration
  struct AbstractTestCase;

  //! \brief an abstract factory for test cases.
  struct MFMTG_VISIBILITY_EXPORT AbstractTestCaseFactory {
    //! \brief a simple alias
    using generator = std::function<std::shared_ptr<AbstractTestCase>(
        const TestCaseParameters&)>;
    //! \return the uniq instance of this class
    static AbstractTestCaseFactory& get();
    /*!
     * \brief register a new test case
     * \param[in] n: test case
     * \param[in] g: test case generator
     */
    void add(const std::string&, const generator);
    /*!
     * \brief generate a test case
     * \param[in] n: name of the test case
     * \param[in] p: parameters
     */
    std::shared_ptr<AbstractTestCase> generate(const std::string&,
                                               const TestCaseParameters&) const;

   private:
    //! \brief default constructor
    AbstractTestCaseFactory();
    //! \brief move constructor (disabled)
    AbstractTestCaseFactory(AbstractTestCaseFactory&&) = delete;
    //! \brief copy constructor (disabled)
    AbstractTestCaseFactory(const AbstractTestCaseFactory&) = delete;
    //! \brief move assignement (disabled)
    AbstractTestCaseFactory& operator=(AbstractTestCaseFactory&&) = delete;
    //! \brief standard assignement (disabled)
    AbstractTestCaseFactory& operator=(const AbstractTestCaseFactory&) = delete;
    //! destructor
    ~AbstractTestCaseFactory();
    //! list of registred generators
    std::map<std::string, generator> generators;

  };  // end of struct AbstractTestCaseFactory

}  // end of namespace mfmtg

#endif /* LIB_MFM_TEST_GENERATOR_ABSTRACTTESTCASEFACTORY_HXX */
