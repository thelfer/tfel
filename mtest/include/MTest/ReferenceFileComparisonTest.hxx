/*! 
 * \file  mtest/include/MTest/ReferenceFileComparisonTest.hxx
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

#ifndef LIB_MTEST_MTESTREFERENCEFILECOMPARISONTEST_H_
#define LIB_MTEST_MTESTREFERENCEFILECOMPARISONTEST_H_ 

#include<map>
#include<string>

#include"TFEL/Tests/TestResult.hxx"
#include<memory>
#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"TFEL/Utilities/TextData.hxx"

#include"MTest/MTest.hxx"

namespace mtest{

  /*!
   * Test based on the comparison of the solution to a reference file
   */
  struct ReferenceFileComparisonTest
    : public MTest::UTest
  {
    /*!
     * constructor
     * \param[in] d    : data
     * \param[in] v    : variable
     * \param[in] c    : column
     * \param[in] vt   : variable type
     * \param[in] vp   : variable position
     * \param[in] eps : criterium value
     */
    ReferenceFileComparisonTest(const std::shared_ptr<tfel::utilities::TextData>,
				     const std::string&,
				     const unsigned int,
				     const MTest::UTest::TestedVariable,
				     const unsigned short,
				     const real);
    /*!
     * \param[in] e  : strains
     * \param[in] s  : strains
     * \param[in] iv : internal state variables
     * \param[in] t  : time
     * \param[in] dt : time increment
     * \param[in] p  : period
     */
    virtual void
    check(const tfel::math::vector<real>&,
	  const tfel::math::vector<real>&,
	  const tfel::math::vector<real>&,
	  const real,
	  const real,
	  const unsigned int) override;
    /*!
     * \return the results of the test
     */
    virtual tfel::tests::TestResult
    getResults() const override;
    //! destructor
    virtual ~ReferenceFileComparisonTest();
  protected:
    ReferenceFileComparisonTest& operator=(const ReferenceFileComparisonTest&) = delete;
    ReferenceFileComparisonTest& operator=(ReferenceFileComparisonTest&&) = delete;
    //! reference values
    const std::vector<real> values;
    //! results of the test
    tfel::tests::TestResult results;
    //! variable name
    const std::string name;
    //! type of variable
    const TestedVariable type;
    //! position of variable
    const unsigned short pos;
    //! criterium value
    const real eps;
  }; 

} // end of namespace mtest

#endif /* LIB_MTEST_MTESTREFERENCEFILECOMPARISONTEST_H_ */

