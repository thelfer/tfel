/*! 
 * \file  mfront/include/MFront/MTestImposedDrivingVariable.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 */

#ifndef _LIB_MFRONT_MTESTIMPOSEDDRIVINGVARIABLE_H_
#define _LIB_MFRONT_MTESTIMPOSEDDRIVINGVARIABLE_H_ 

#include"TFEL/Math/matrix.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"
#include"TFEL/Utilities/SmartPtr.hxx"

#include"MFront/MTestTypes.hxx"
#include"MFront/MTestConfig.hxx"
#include"MFront/MTestEvolution.hxx"
#include"MFront/MTestConstraint.hxx"

namespace mfront
{

  // forward declaration
  struct MTestBehaviour;

  /*!
   * Impose the value of a driving variable component
   */
  struct MFRONT_MTEST_VISIBILITY_EXPORT MTestImposedDrivingVariable
    : public MTestConstraint
  {
    /*!
     * constructor
     * \param[in] b : behaviour
     * \param[in] h : modelling hypothesis
     * \param[in] c : driving variable component
     * \param[in] s : driving variable evolution
     */
    MTestImposedDrivingVariable(const MTestBehaviour&,
				const tfel::material::ModellingHypothesis::Hypothesis,
				const std::string&,
				const tfel::utilities::shared_ptr<MTestEvolution>);
    /*!
     * constructor
     * \param[in] c : component
     * \param[in] s : driving variable evolution
     */
    MTestImposedDrivingVariable(const unsigned short c,
				const tfel::utilities::shared_ptr<MTestEvolution>);
    /*!
     * \return the number of Lagrange Multipliers
     * associated with this contraint
     */
    virtual unsigned short
    getNumberOfLagrangeMultipliers(void) const;
    /*!
     * \brief builds up the stiffness matrix and the residual
     * \param[out] K  : stiffness matrix
     * \param[out] r  : residual vector
     * \param[in]  u0 : value of the unknowns at the beginning
     *                  of the time step
     * \param[in]  u1 : current estimate of the unknowns
     * \param[in]  p  : position of the first lagrange multiplier
     *                  in the residual 
     * \param[in]  d  : space dimension
     * \param[in]  t  : beginning of the time step
     * \param[in]  dt : time increment
     * \param[in]  a  : normalisation factor
     */
    virtual void
    setValues(tfel::math::matrix<real>&,
	      tfel::math::vector<real>&,
	      const tfel::math::vector<real>&,
	      const tfel::math::vector<real>&,
	      const unsigned short,
	      const unsigned short,
	      const real,
	      const real,
	      const real) const;
    /*!
     * \param[in]  e    : driving variables
     * \param[in]  s    : stresses
     * \param[in]  eeps : criterium value for driving variables
     * \param[in]  seps : criterium value for stresses
     * \param[in]  t    : beginning of the time step
     * \param[in]  dt   : time increment
     */
    virtual bool
    checkConvergence(const tfel::math::vector<real>&,
		     const tfel::math::vector<real>&,
		     const real,
		     const real,
		     const real,
		     const real) const;
    /*!
     * \param[in]  e    : driving variables
     * \param[in]  s    : thermodynamic forces
     * \param[in]  eeps : criterium value for driving variables
     * \param[in]  seps : criterium value for thermodynamic forces
     * \param[in]  t    : beginning of the time step
     * \param[in]  dt   : time increment
     */
    virtual std::string
    getFailedCriteriaDiagnostic(const tfel::math::vector<real>&,
				const tfel::math::vector<real>&,
				const real,
				const real,
				const real,
				const real) const;
    //! destructor
    virtual ~MTestImposedDrivingVariable();
  protected:
    //! driving variable evolution
    const tfel::utilities::shared_ptr<MTestEvolution> eev;
    //! component value
    unsigned short c;
  }; // end of struct MTestImposedDrivingVariable
  
} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTIMPOSEDDRIVINGVARIABLE_H */

