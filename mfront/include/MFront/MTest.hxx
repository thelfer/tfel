/*! 
 * \file  MTest.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 09 avril 2013
 */

#ifndef _LIB_MFRONT_MTEST_H_
#define _LIB_MFRONT_MTEST_H_ 

#include<map>
#include<string>
#include<vector>
#include<fstream>

#include"TFEL/Material/ModellingHypothesis.hxx"
#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/MTestTypes.hxx"

namespace mfront
{

  //! forward declaration
  struct MTestBehaviour;
  //! forward declaration
  struct MTestConstraint;
  //! forward declaration
  struct MTestEvolution;

  /*!
   * MTest is a simple class 
   * to test mfront behaviours
   *
   * The unknowns are made of
   * the strain stensor followed 
   * by a number of lagrange
   * multiplier
   */
  struct MTest
    : public tfel::utilities::CxxTokenizer
  {
    /*!
     * read the input file
     * \param[in] f : file
     */
    MTest(const std::string&);
    /*!
     * integrate the behaviour
     * along the loading path
     */ 
    int execute(void);
  private:
    //! a simple alias
    typedef void (MTest::* CallBack)(TokensContainer::const_iterator&);
    //! callbacks
    std::map<std::string,CallBack> callbacks;
    /*!
     * handle the @StrainEpsilon keyword
     * \param[in,out] p : position if the input file
     */
    void handleStrainEpsilon(TokensContainer::const_iterator&);
    /*!
     * handle the @StressEpsilon keyword
     * \param[in,out] p : position if the input file
     */
    void handleStressEpsilon(TokensContainer::const_iterator&);
    /*!
     * handle the @OutputFile keyword
     * \param[in,out] p : position if the input file
     */
    void handleOutputFile(TokensContainer::const_iterator&);
    /*!
     * handle the @ModellingHypothesis keyword
     * \param[in,out] p : position if the input file
     */
    void handleModellingHypothesis(TokensContainer::const_iterator&);
    /*!
     * handle the @Times keyword
     * \param[in,out] p : position if the input file
     */
    void handleTimes(TokensContainer::const_iterator&);
    /*!
     * handle the @Strain keyword
     * \param[in,out] p : position if the input file
     */
    void handleStrain(TokensContainer::const_iterator&);
    /*!
     * handle the @Behaviour keyword
     * \param[in,out] p : position if the input file
     */
    void handleBehaviour(TokensContainer::const_iterator&);
    /*!
     * handle the @MaterialProperty keyword
     * \param[in,out] p : position if the input file
     */
    void handleMaterialProperty(TokensContainer::const_iterator&);
    /*!
     * handle the @InternalStateVariable keyword
     * \param[in,out] p : position if the input file
     */
    void handleInternalStateVariable(TokensContainer::const_iterator&);
    /*!
     * handle the @ExternalStateVariable keyword
     * \param[in,out] p : position if the input file
     */
    void handleExternalStateVariable(TokensContainer::const_iterator&);
    /*!
     * handle the @ImposedStress keyword
     * \param[in,out] p : position if the input file
     */
    void handleImposedStress(TokensContainer::const_iterator&);
    /*!
     * handle the @ImposedStrain keyword
     * \param[in,out] p : position if the input file
     */
    void handleImposedStrain(TokensContainer::const_iterator&);
    /*!
     * read a time
     * \param[in,out] p : position if the input file
     */
    real
    readTime(TokensContainer::const_iterator&);
    /*!
     * output the results
     * \param[in] t  : time
     * \param[in] N  : tensor size
     * \param[in] u  : unknowns
     * \param[in] s  : stresses
     * \param[in] iv : internal state variables
     */
    void printOutput(const real,
		     const unsigned short,
		     const tfel::math::vector<real>&,
		     const tfel::math::tvector<6u,real>&,
		     const tfel::math::vector<real>&);
    /*!
     * \brief parse an evolution
     * \param[in,out] p : position if the input file
     */
    tfel::utilities::shared_ptr<MTestEvolution>
    parseEvolution(TokensContainer::const_iterator&);
    /*!
     * read an array
     * The expected size of the array is given by the output vector
     * \param[out]    v : array to be read
     * \param[in,out] p : position if the input file
     */
    void
    readArrayOfSpecifiedSize(std::vector<real>&,
			     TokensContainer::const_iterator&);
    /*!
     * \brief set the hypothesis to the default one
     */
    void setDefaultHypothesis(void);
    //! the mechanical behaviour
    tfel::utilities::shared_ptr<MTestBehaviour> b;
    //! constraints
    std::vector<tfel::utilities::shared_ptr<MTestConstraint> > constraints;
    //! times
    std::vector<real> times;
    //! list of evolutions
    std::map<std::string,
	     tfel::utilities::shared_ptr<MTestEvolution> > evs;
    //! output file name
    std::string output;
    //! output file
    std::ofstream out;
    //! dimension used for the computation
    unsigned short dimension;
    //! modelling hypothesis
    tfel::material::ModellingHypothesis::Hypothesis hypothesis;
    // inital values of the strains
    std::vector<real> e_t0;
    // inital values of the internal state variables
    std::vector<real> iv_t0;
    /*!
     * criterium value on strains used to stop the
     * Newton-Raphson algorithm.
     * By default, a value of 1.e-12 is used.
     */
    real eeps;
    /*!
     * criterium value on stresses used to stop the
     * Newton-Raphson algorithm.
     * By default, a value of 1.e-3 is used. This value is suitable
     * for stresses expresses in Pa.
     */
    real seps;
    //! maximum number of sub steps allowed
    unsigned short mSubSteps;
    //! maximum number of iterations allowed in the Newton-Raphson algorithm
    unsigned short iterMax;
  };

}

#endif /* _LIB_MFRONT_MTEST_H */

