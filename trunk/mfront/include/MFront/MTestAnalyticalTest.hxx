/*! 
 * \file  MTestAnalyticalTest.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 avril 2013
 */

#ifndef _LIB_MFRONT_MTESTANALYTICALTEST_H_
#define _LIB_MFRONT_MTESTANALYTICALTEST_H_ 

#include<map>
#include<string>

#include"TFEL/Math/Evaluator.hxx"
#include"TFEL/Tests/TestResult.hxx"
#include"TFEL/Utilities/SmartPtr.hxx"

#include"MFront/MTest.hxx"

namespace mfront{

  /*!
   * Test based on the comparison of the solution to analytical
   * results
   */
  struct MTestAnalyticalTest
    : public MTest::UTest
  {
    /*!
     * constructor
     * \param[in] f_   : function
     * \param[in] v    : variable
     * \param[in] vt   : variable type
     * \param[in] vp   : variable position
     * \param[in] evm_ : user defined evolution
     * \param[in] eps : criterium value
     */
    MTestAnalyticalTest(const std::string&,
			const std::string&,
			const MTest::UTest::TestedVariable,
			const unsigned short,
			tfel::utilities::shared_ptr<std::map<std::string,
							     tfel::utilities::shared_ptr<MTestEvolution> > >,
			const real);
    /*!
     * \param[in] e  : strains
     * \param[in] s  : strains
     * \param[in] iv : internal state variables
     * \param[in] t  : time
     * \param[in] dt : time increment
     * \param[in] p  : period
     */
    void
    check(const tfel::math::vector<real>&,
	  const tfel::math::vector<real>&,
	  const tfel::math::vector<real>&,
	  const real,
	  const real,
	  const unsigned int);
    /*!
     * \return the results of the test
     */
    tfel::tests::TestResult
    getResults() const;
    //! destructor
    ~MTestAnalyticalTest();
  protected:
    //! analytical solution 
    tfel::math::Evaluator f;
    //! results of the test
    tfel::tests::TestResult results;
    //! variable names of the evaluator
    std::vector<std::string> vnames;
    //! variable name
    const std::string name;
    //! type of variable
    const TestedVariable type;
    //! position of variable
    const unsigned short pos;
    //! external evolution
    const tfel::utilities::shared_ptr<std::map<std::string,
					       tfel::utilities::shared_ptr<MTestEvolution> > > evm;
    //! criterium value
    const real eps;
  }; 

} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTANALYTICALTEST_H */

