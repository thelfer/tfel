/*! 
 * \file  mfront/include/MTest/AnalyticalTest.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_MTESTANALYTICALTEST_H_
#define _LIB_MFRONT_MTESTANALYTICALTEST_H_ 

#include<map>
#include<string>

#include"TFEL/Math/Evaluator.hxx"
#include"TFEL/Tests/TestResult.hxx"
#include<memory>

#include"MTest/MTest.hxx"

namespace mfront{

  /*!
   * Test based on the comparison of the solution to analytical
   * results
   */
  struct AnalyticalTest
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
    AnalyticalTest(const std::string&,
			const std::string&,
			const MTest::UTest::TestedVariable,
			const unsigned short,
			std::shared_ptr<std::map<std::string,
							     std::shared_ptr<Evolution> > >,
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
    ~AnalyticalTest();
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
    const std::shared_ptr<std::map<std::string,
					       std::shared_ptr<Evolution> > > evm;
    //! criterium value
    const real eps;
  }; 

} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTANALYTICALTEST_H */

