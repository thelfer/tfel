/*! 
 * \file  MTestConstraint.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 */

#ifndef _LIB_MFRONT_MTESTCONSTRAINT_H_
#define _LIB_MFRONT_MTESTCONSTRAINT_H_ 

#include"TFEL/Math/matrix.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/tvector.hxx"

#include"MFront/MTestTypes.hxx"

namespace mfront
{

  /*!
   * Base class for all the constraints
   * imposed to the point
   */
  struct MTestConstraint
  {
    /*!
     * \return the number of Lagrange Multipliers
     * associated with this contraint
     */
    virtual unsigned short
    getNumberOfLagrangeMultipliers(void) const = 0;
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
	      const real) const = 0;
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
		     const tfel::math::tvector<6u,real>&,
		     const real,
		     const real,
		     const real,
		     const real) const = 0;
    //! destructor
    virtual ~MTestConstraint();
  }; // end of struct MTestConstraint
  
} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTCONSTRAINT_H */

