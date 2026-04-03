/*!
 * \file  mtest/include/MTest/ReferenceFileComparisonTest.hxx
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

#ifndef LIB_MTEST_MTESTREFERENCEFILECOMPARISONTEST_HXX
#define LIB_MTEST_MTESTREFERENCEFILECOMPARISONTEST_HXX

#include <map>
#include <memory>
#include <string>

#include "TFEL/Tests/TestResult.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/TextData.hxx"
#include "MTest/Config.hxx"
#include "MTest/MTest.hxx"

namespace mtest {

  /*!
   * Test based on the comparison of the solution to a reference file
   */
  struct MTEST_VISIBILITY_EXPORT ReferenceFileComparisonTest
      : public MTest::UTest {
    /*!
     * constructor
     * \param[in] d    : data
     * \param[in] c    : column
     * \param[in] v    : variable
     * \param[in] vt   : variable type
     * \param[in] vp   : variable position
     * \param[in] eps  : criterium value
     */
    ReferenceFileComparisonTest(const tfel::utilities::TextData&,
                                const unsigned int,
                                const std::string&,
                                const std::function<real(const CurrentState&)>&,
                                const real);
    /*!
     * constructor
     * \param[in] d    : data
     * \param[in] v    : variable
     * \param[in] m    : evolution manager
     * \param[in] f    : formulae
     * \param[in] vt   : variable type
     * \param[in] vp   : variable position
     * \param[in] eps  : criterium value
     */
    ReferenceFileComparisonTest(const tfel::utilities::TextData&,
                                const EvolutionManager&,
                                const std::string&,
                                const std::string&,
                                const std::function<real(const CurrentState&)>&,
                                const real);
    /*!
     * \param[in] s:  state
     * \param[in] t:  time
     * \param[in] dt: time increment
     * \param[in] p:  period
     */
    void check(const CurrentState&,
               const real,
               const real,
               const unsigned int) override;
    //! \return the results of the test
    tfel::tests::TestResult getResults() const override;
    //! destructor
    ~ReferenceFileComparisonTest() override;

   protected:
    ReferenceFileComparisonTest& operator=(const ReferenceFileComparisonTest&) =
        delete;
    ReferenceFileComparisonTest& operator=(ReferenceFileComparisonTest&&) =
        delete;
    //! reference values
    const std::vector<real> values;
    //! results of the test
    tfel::tests::TestResult results;
    //! variable name
    const std::string name;
    //! value getter
    std::function<real(const CurrentState&)> get;
    //! criterium value
    const real eps;
  };

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTREFERENCEFILECOMPARISONTEST_HXX */
