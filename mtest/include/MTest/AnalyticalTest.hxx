/*!
 * \file  mtest/include/MTest/AnalyticalTest.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 18 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTANALYTICALTEST_HXX
#define LIB_MTEST_MTESTANALYTICALTEST_HXX

#include <map>
#include <memory>
#include <string>
#include "TFEL/Math/Evaluator.hxx"
#include "TFEL/Tests/TestResult.hxx"
#include "MTest/Config.hxx"
#include "MTest/MTest.hxx"

namespace mtest {

  /*!
   * Test based on the comparison of the solution to analytical
   * results
   */
  struct MTEST_VISIBILITY_EXPORT AnalyticalTest : public MTest::UTest {
    /*!
     * constructor
     * \param[in] f_:   function
     * \param[in] v:    variable name
     * \param[in] g:    getter
     * \param[in] evm_: user defined evolution
     * \param[in] eps:  criterium value
     */
    AnalyticalTest(const std::string&,
                   const std::string&,
                   const std::function<real(const CurrentState&)>&,
                   const EvolutionManager&,
                   const real);
    /*!
     * \param[in] s: current material state
     * \param[in] t  : time
     * \param[in] dt : time increment
     * \param[in] p  : period
     */
    void check(const CurrentState&,
               const real,
               const real,
               const unsigned int) override;
    //! \return the results of the test
    tfel::tests::TestResult getResults() const override;
    //! destructor
    ~AnalyticalTest() override;

   protected:
    AnalyticalTest& operator=(const AnalyticalTest&) = delete;
    AnalyticalTest& operator=(AnalyticalTest&&) = delete;
    //! analytical solution
    tfel::math::Evaluator f;
    //! results of the test
    tfel::tests::TestResult results;
    //! variable name
    const std::string name;
    //! value getter
    std::function<real(const CurrentState&)> get;
    //! external evolution
    const EvolutionManager evm;
    //! criterium value
    const real eps;
  };

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTANALYTICALTEST_HXX */
