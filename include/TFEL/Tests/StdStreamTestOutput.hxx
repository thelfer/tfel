/*!
 * \file   include/TFEL/Tests/StdStreamTestOutput.hxx
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

#ifndef LIB_TFEL_TESTS_STDSTREAMTESTOUTPUT_HXX
#define LIB_TFEL_TESTS_STDSTREAMTESTOUTPUT_HXX 1

#include <iosfwd>
#include <string>
#include <memory>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Tests/TestOutput.hxx"
#include "TFEL/Tests/TestResult.hxx"

namespace tfel {

  namespace tests {

    /*!
     * \brief Instances of this class redirects the output of a
     * TestResult to a standard stream
     */
    struct TFELTESTS_VISIBILITY_EXPORT StdStreamTestOutput final
        : public TestOutput {
      /*!
       * \brief constructor
       * \param[in] o: output file name
       */
      StdStreamTestOutput(const std::string&);
      /*!
       * \brief constructor
       * \param[in] o: output stream
       * \param[in] b: color output
       */
      StdStreamTestOutput(std::ostream&, const bool = true);
      /*!
       * \brief Constructor
       * \param[in] o: output stream
       * \param[in] b: color output
       */
      StdStreamTestOutput(const std::shared_ptr<std::ostream>&,
                          const bool = false);
      /*!
       * \brief begin a new test suite
       * \param[in] n: name of the test suite
       */
      void beginTestSuite(const std::string&) override;
      /*!
       * \brief add a new test
       * \param[in] g: group  of the test
       * \param[in] n: name   of the test
       * \param[in] r: result of the test
       */
      void addTest(const std::string&,
                   const std::string&,
                   const TestResult&) override;
      /*!
       * \brief end a test suite
       * \param[in] r: (global) result of the test suite
       */
      void endTestSuite(const TestResult&) override;
      //! \brief Destructor
      ~StdStreamTestOutput() override;

     private:
      /*!
       * \param r: result to be treated
       * \param s: string to be added at beginning of line
       */
      TFEL_VISIBILITY_LOCAL void treatTest(const TestResult&,
                                           const std::string&);
      //! \brief copy constructor (disabled)
      StdStreamTestOutput(const StdStreamTestOutput&) = delete;
      //! \brief assignement operator (disabled)
      StdStreamTestOutput& operator=(const StdStreamTestOutput&) = delete;
      //! pointer used to close stream, if the class has to handle it
      std::shared_ptr<std::ostream> pos;
      //! output stream
      std::ostream& os;
      //! tells if the output shall be colorized
      const bool colorOutput;
    };  // end of struct StdStreamTestOutput

  }  // namespace tests

}  // end of namespace tfel

#endif /* LIB_TFEL_TESTS_STDSTREAMTESTOUTPUT_HXX */
