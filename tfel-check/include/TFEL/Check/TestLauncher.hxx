/*!
 * \file   TestLauncher.hxx
 * \brief  This file declares the TestLauncher class
 * \author Thomas Helfer
 * \date   28 Jan 2008
 *
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFELCHECK_TESTLAUNCHER_HXX
#define LIB_TFELCHECK_TESTLAUNCHER_HXX

#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <set>
#include <sys/times.h>

#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/TextData.hxx"
#include "TFEL/Check/TFELCheckConfig.hxx"
#include "TFEL/Check/PCLogger.hxx"
#include "TFEL/Check/Interpolation.hxx"
#include "TFEL/Check/Column.hxx"
#include "TFEL/Check/Comparison.hxx"
#include "TFEL/Check/Test.hxx"

namespace tfel {

  namespace check {

    struct TFELCHECK_VISIBILITY_EXPORT TestLauncher
        : public tfel::utilities::CxxTokenizer {
      using CxxTokenizer = tfel::utilities::CxxTokenizer;

      TestLauncher(const std::string&, const PCLogger&);
      bool execute();
      //! desctrutor
      ~TestLauncher();

     private:
      typedef void (TestLauncher::*CallBack)();
      void registerCallBack(const std::string&, const CallBack&);
      //! analyse the input file
      void analyseInputFile();
      void treatTest();
      void treatPrecision();
      void treatCommand();
      void treatEnvironment();
      void treatTestType();
      void treatInterpolation();
      /*!
       * \brief register the error and throw an runtime_error
       * \param[in] m:   calling method
       * \param[in] msg: error message
       */
      TFEL_NORETURN void throwRuntimeError(const std::string&,
                                           const std::string&);
      /*!
       * \brief check that there is still something to read
       * \param[in] m: calling method
       */
      void checkNotEndOfFile(const std::string&);
      /*!
       * \brief check that the current token has a specified value and
       * go to the next token
       * \param[in] m: calling method
       * \param[in] v: value to be read
       */
      void readSpecifiedToken(const std::string&, const std::string&);
      /*!
       * \brief check that the current token is a string, return its
       * value, and go to the next token
       * \param[in] m: calling method
       */
      std::string readString(const std::string&);
      /*!
       * \brief read the current interpolation method
       * \param[in] m: calling method
       */
      void setInterpolation(const std::string&);

      enum ClockEventType {
        START,
        STOP,
        GET
      }; /**!< ClockAction supported event. */
      double ClockAction(ClockEventType clockevent);
      std::map<std::string, std::string> environments;
      //! iterator to the current file token
      const_iterator current;
      //! list of files comparison to be made
      std::vector<Test> comparisons;
      std::map<std::string, CallBack> callBacks;
      std::string logfile;
      PCLogger log;         /**!< logfile output stream */
      PCLogger glog;        /**!< global logfile output stream */
      std::string file;     /**!< file containing tfel-check commands */
      std::string testname; /**!< test name based on file name */
      std::vector<std::string> commands;
      std::string XMLResult; /**!< string containing testcase result, Junit
                                formatted. */
      double prec = 1.e-8;
      double precision2 = 0.;
      //! the type of comparison of the test (absolute, relative...)
      std::shared_ptr<Comparison> comparison;
      tms timeStart, timeStop; /**!< tms struct used by ClockAction */
      //! the interpolation chosen
      std::shared_ptr<Interpolation> interpolation;
      //! the column used for interpolation (abscissa)
      std::shared_ptr<Column> ci;
      //! the interpolation chosen for the integration
      std::shared_ptr<Interpolation> integralInterpolation;
      //! the column used for integration interpolation (abscissa)
      std::shared_ptr<Column> colIntegralInterpolated;
      //! true if user allows to interpolate ref
      bool allowLessResults = false;
    };

    // end of struct TestLauncher

  }  // end of namespace check

}  // end of namespace tfel

#endif /* LIB_TFELCHECK_TESTLAUNCHER_HXX */
