/*!
 * \file   include/TFEL/Tests/StdStreamTestOutput.hxx
 * \author Helfer Thomas
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
     *  Test Output using a standard stream
     */
    struct TFELTESTS_VISIBILITY_EXPORT StdStreamTestOutput final
        : public TestOutput {
      /*!
       * Constructor
       * \param o : output file name
       */
      StdStreamTestOutput(const std::string&);
      /*!
       * Constructor
       * \param o : output stream
       * \param b : color output
       */
      StdStreamTestOutput(std::ostream&, const bool = true);
      /*!
       * Constructor
       * \param o : output stream
       * \param b : color output
       */
      StdStreamTestOutput(const std::shared_ptr<std::ostream>&,
                          const bool = false);
      /*!
       * Begin a new test suite
       * \param n : name of the test suite
       */
      virtual void beginTestSuite(const std::string&) override;
      /*!
       *  Add a new test
       * \param g : group  of the test
       * \param n : name   of the test
       * \param r : result of the test
       */
      virtual void addTest(const std::string&,
                           const std::string&,
                           const TestResult&) override;
      /*!
       * End a test suite
       * \param r : (global) result of the test suite
       */
      virtual void endTestSuite(const TestResult&) override;
      /*!
       * Destructor
       */
      virtual ~StdStreamTestOutput();

     private:
      /*!
       * \param r : result to be treated
       * \param s : string to be added at beginning of line
       */
      TFEL_VISIBILITY_LOCAL void treatTest(const TestResult&,
                                           const std::string&);
      /*!
       * copy constructor (disabled)
       * \param src : object to be copied
       */
      TFEL_VISIBILITY_LOCAL StdStreamTestOutput(const StdStreamTestOutput&);
      /*!
       * assignement operator (disabled)
       * \param src : object to be assigned
       * \return a reference to this object
       */
      TFEL_VISIBILITY_LOCAL StdStreamTestOutput& operator=(
          const StdStreamTestOutput&);
      /*!
       * pointer used to close stream, if the class has to handle it
       */
      std::shared_ptr<std::ostream> pos;
      //! output stream
      std::ostream& os;
      //! tells if the output shall be colorized
      const bool colorOutput;
    };  // end of struct StdStreamTestOutput

  }  // namespace tests

}  // end of namespace tfel

#endif /* LIB_TFEL_TESTS_STDSTREAMTESTOUTPUT_HXX */
