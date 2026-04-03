/*!
 * \file   include/TFEL/Tests/MultipleTestOutputs.hxx
 * \author Thomas Helfer
 * \date   10 Apr 10
 * \brief
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TESTS_MULTIPLETESTOUTPUTS_HXX
#define LIB_TFEL_TESTS_MULTIPLETESTOUTPUTS_HXX 1

#include <vector>
#include <memory>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Tests/TestOutput.hxx"
#include "TFEL/Tests/TestResult.hxx"

namespace tfel::tests {

  /*!
   * \brief an helper class for gathering multiple outputs.
   */
  struct TFELTESTS_VISIBILITY_EXPORT MultipleTestOutputs final
      : public TestOutput {
    //! a simple alias
    using TestOutputPtr = std::shared_ptr<TestOutput>;
    //! \brief default constructor
    MultipleTestOutputs();
    /*!
     * \brief add a test output
     * \param o: output to be added
     */
    void addTestOutput(const TestOutputPtr&);
    /*!
     * \brief begin a new test suite
     * \param n: name of the test suite
     */
    void beginTestSuite(const std::string&) override;
    /*!
     * \brief add a new test
     * \param g: group  of the test
     * \param n: name   of the test
     * \param r: result of the test
     */
    void addTest(const std::string&,
                 const std::string&,
                 const TestResult&) override;
    /*!
     * \brief end a test suite
     * \param r: (global) result of the test suite
     */
    void endTestSuite(const TestResult&) override;
    //! destructor
    ~MultipleTestOutputs() override;

   private:
    //! \brief move constructor (disabled)
    MultipleTestOutputs(MultipleTestOutputs&&) = delete;
    //! \brief copy constructor (disabled)
    MultipleTestOutputs(const MultipleTestOutputs&) = delete;
    //! \brief assignement operator (disabled)
    MultipleTestOutputs& operator=(const MultipleTestOutputs&) = delete;
    //! \brief move assignement operator (disabled)
    MultipleTestOutputs& operator=(MultipleTestOutputs&&) = delete;
    //! registred outputs
    std::vector<TestOutputPtr> outputs;
  };  // end of struct MultipleTestOutputs

}  // end of namespace tfel::tests

#endif /* LIB_TFEL_TESTS_MULTIPLETESTOUTPUTS_HXX */
