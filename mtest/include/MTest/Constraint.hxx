/*!
 * \file  mtest/include/MTest/Constraint.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTCONSTRAINT_H_
#define LIB_MTEST_MTESTCONSTRAINT_H_

#include "TFEL/Math/matrix.hxx"
#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tvector.hxx"

#include "MTest/Config.hxx"
#include "MTest/Types.hxx"

namespace mtest {

  /*!
   * Base class for all the constraints
   * imposed to the point
   */
  struct MTEST_VISIBILITY_EXPORT Constraint {
    /*!
     * \return the number of Lagrange Multipliers
     * associated with this contraint
     */
    virtual unsigned short getNumberOfLagrangeMultipliers(void) const = 0;
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
    virtual void setValues(tfel::math::matrix<real>&,
                           tfel::math::vector<real>&,
                           const tfel::math::vector<real>&,
                           const tfel::math::vector<real>&,
                           const unsigned short,
                           const unsigned short,
                           const real,
                           const real,
                           const real) const = 0;
    /*!
     * \param[in]  e    : driving variables
     * \param[in]  s    : thermodynamic forces
     * \param[in]  eeps : criterium value for driving variables
     * \param[in]  seps : criterium value for thermodynamic forces
     * \param[in]  t    : beginning of the time step
     * \param[in]  dt   : time increment
     */
    virtual bool checkConvergence(const tfel::math::vector<real>&,
                                  const tfel::math::vector<real>&,
                                  const real,
                                  const real,
                                  const real,
                                  const real) const = 0;
    /*!
     * \param[in]  e    : driving variables
     * \param[in]  s    : thermodynamic forces
     * \param[in]  eeps : criterium value for driving variables
     * \param[in]  seps : criterium value for thermodynamic forces
     * \param[in]  t    : beginning of the time step
     * \param[in]  dt   : time increment
     */
    virtual std::string getFailedCriteriaDiagnostic(
        const tfel::math::vector<real>&,
        const tfel::math::vector<real>&,
        const real,
        const real,
        const real,
        const real) const = 0;
    //! destructor
    virtual ~Constraint();
  };  // end of struct Constraint

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTCONSTRAINT_H_ */
