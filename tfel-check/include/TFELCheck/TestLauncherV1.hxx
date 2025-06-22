/*!
 * \file   TestLauncherV1.hxx
 * \brief
 * \author THOMAS HELFER
 * \date   25 août 2016
 *
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_CHECK_TESTLAUNCHERV1_HXX
#define LIB_TFEL_CHECK_TESTLAUNCHERV1_HXX

#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <set>
#include <sys/times.h>

#include "TFELCheck/TFELCheckConfig.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/TextData.hxx"
#include "TFELCheck/PCLogger.hxx"
#include "TFELCheck/Interpolation.hxx"
#include "TFELCheck/Column.hxx"
#include "TFELCheck/Comparison.hxx"
#include "TFELCheck/Test.hxx"

namespace tfel_check {

  struct TFELCHECK_VISIBILITY_EXPORT TestLauncherV1
      : public tfel::utilities::CxxTokenizer {
    TestLauncherV1(const std::string&, const PCLogger&);
    bool execute(void);
    ~TestLauncherV1(void);

   private:
    typedef void (TestLauncherV1::*CallBack)(void);

    //! list of all tokens of the file, sorted by line
    std::vector<std::vector<tfel::utilities::Token>> tokens;
    std::vector<std::vector<tfel::utilities::Token>>::const_iterator line;
    std::vector<tfel::utilities::Token>::const_iterator current;
    //! list of files comparison to be made
    std::vector<Test> comparisons;
    std::map<std::string, CallBack> callBacks;
    std::string logfile;
    PCLogger log;         /**!< logfile output stream */
    PCLogger glog;        /**!< global logfile output stream */
    std::string file;     /**!< file containing pleiades-check commands */
    std::string testname; /**!< test name based on file name */
    std::vector<std::string> commands;
    std::vector<std::string> meshCommands;
    std::string
        XMLResult; /**!< string containing testcase result, Junit formatted. */
    double prec;
    double precision2;
    std::shared_ptr<Comparison> comparison; /**!< the type of comparison of the
                                               test (absolute, relative...) */
    tms timeStart, timeStop; /**!< tms struct used by ClockAction */
    std::shared_ptr<Interpolation>
        interpolation; /**!< the interpolation chosen */
    std::shared_ptr<Column>
        ci; /**!< the column used for interpolation (abscissa)*/
    std::shared_ptr<Interpolation>
        integralInterpolation; /**!< the interpolation chosen for the
                                  integration*/
    std::shared_ptr<Column>
        colIntegralInterpolated; /**!< the column used for integration
                                    interpolation (abscissa)*/

    bool allowLessResults; /**!< true if user allows to interpolate ref */
    void registerCallBack(const std::string&, const CallBack&);
    //! analyse the input file
    void analyseInputFile(void);
    void treatTest(void);
    void treatPrecision(void);
    void treatCommand(void);
    void treatMeshCommand(void);
    void treatEnvironment(void);
    void treatTestType(void);
    void treatInterpolation(void);

    enum ClockEventType {
      START,
      STOP,
      GET
    }; /**!< ClockAction supported event. */
    double ClockAction(ClockEventType clockevent);
    std::map<std::string, std::string> environments;
  };
  // end of struct TestLauncherV1

}  // end of namespace tfel_check

#endif /* LIB_TFEL_CHECK_TESTLAUNCHERV1_HXX */
