/*! 
 * \file  MTestImposedStress.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 */

#ifndef _LIB_MFRONT_MTESTIMPOSEDSTRESS_H_
#define _LIB_MFRONT_MTESTIMPOSEDSTRESS_H_ 

#include"TFEL/Math/matrix.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"
#include"TFEL/Utilities/SmartPtr.hxx"

#include"MFront/MTestTypes.hxx"
#include"MFront/MTestEvolution.hxx"
#include"MFront/MTestConstraint.hxx"

namespace mfront
{

  /*!
   * Impose the value of a stress component
   */
  struct MTestImposedStress
    : public MTestConstraint
  {
    /*!
     * constructor
     * \param[in] h : modelling hypothesis
     * \param[in] c : stress component
     * \param[in] s : stress evolution
     */
    MTestImposedStress(const tfel::material::ModellingHypothesis::Hypothesis,
		       const std::string&,
		       const tfel::utilities::shared_ptr<MTestEvolution>);
    /*!
     * constructor
     * \param[in] c : stress component
     * \param[in] s : stress evolution
     */
    MTestImposedStress(const unsigned short,
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
     * \param[in]  N    : number of components of tensors
     * \param[in]  e    : strains
     * \param[in]  s    : stresses
     * \param[in]  eeps : criterium value for strains
     * \param[in]  seps : criterium value for stresses
     * \param[in]  t    : beginning of the time step
     * \param[in]  dt   : time increment
     */
    virtual bool
    checkConvergence(const unsigned short,
		     const tfel::math::vector<real>&,
		     const tfel::math::stensor<3u,real>&,
		     const real,
		     const real,
		     const real,
		     const real) const;
    //! destructor
    virtual ~MTestImposedStress();
  protected:
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] c : stress component
     */
    unsigned short
    getComponentPosition(const tfel::material::ModellingHypothesis::Hypothesis,
			 const std::string&) const;
    //! stress value
    const tfel::utilities::shared_ptr<MTestEvolution> sev;
    //! component value
    unsigned short c;
  }; // end of struct MTestImposedStress
  
} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTIMPOSEDSTRESS_H */

