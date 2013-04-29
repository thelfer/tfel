/*! 
 * \file  MTestReferenceFileComparisonTest.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 avril 2013
 */

#ifndef _LIB_MFRONT_MTESTREFERENCEFILECOMPARISONTEST_H_
#define _LIB_MFRONT_MTESTREFERENCEFILECOMPARISONTEST_H_ 

#include<map>
#include<string>

#include"TFEL/Tests/TestResult.hxx"
#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"TFEL/Utilities/TextData.hxx"

#include"MFront/MTest.hxx"

namespace mfront{

  /*!
   * Test based on the comparison of the solution to a reference file
   */
  struct MTestReferenceFileComparisonTest
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
    MTestReferenceFileComparisonTest(const tfel::utilities::shared_ptr<tfel::utilities::TextData>,
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
    void
    check(const tfel::math::vector<real>&,
	  const tfel::math::stensor<3u,real>&,
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
    ~MTestReferenceFileComparisonTest();
  protected:
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

} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTREFERENCEFILECOMPARISONTEST_H */

