/*!
 * \file  mtest/include/MTest/ImposedThermodynamicForce.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTIMPOSEDTHERMODYNAMICFORCE_HXX
#define LIB_MTEST_MTESTIMPOSEDTHERMODYNAMICFORCE_HXX

#include "TFEL/Math/matrix.hxx"
#include "TFEL/Math/vector.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include <memory>

#include "MTest/Types.hxx"
#include "MTest/Config.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/Constraint.hxx"

namespace mtest {

  // forward declaration
  struct Behaviour;

  /*!
   * Impose the value of a thermodynamic force component
   */
  struct MTEST_VISIBILITY_EXPORT ImposedThermodynamicForce final
      : public Constraint {
    /*!
     * constructor
     * \param[in] b : behaviour
     * \param[in] c : thermodynamic force component
     * \param[in] s : thermodynamic force evolution
     */
    ImposedThermodynamicForce(const Behaviour&,
                              const std::string&,
                              const std::shared_ptr<Evolution>);
    /*!
     * constructor
     * \param[in] c : thermodynamic force component
     * \param[in] s : thermodynamic force evolution
     */
    ImposedThermodynamicForce(const unsigned short,
                              const std::shared_ptr<Evolution>);
    /*!
     * \return the number of Lagrange Multipliers
     * associated with this contraint
     */
    virtual unsigned short getNumberOfLagrangeMultipliers() const override;
    /*!
     * \brief builds up the stiffness matrix and the residual.
     * \param[out] K:  stiffness matrix
     * \param[out] r:  residual vector
     * \param[in]  u0: value of the unknowns at the beginning
     *                  of the time step
     * \param[in]  u1: current estimate of the unknowns
     * \param[in]  u1: current estimate of the unknowns
     * \param[in]  k: approximation of the derivative of the
     * thermodynamic force with respect to the driving variable, as
     * returned by the mechanical behaviour.
     * \param[in]  f: thermodynamic forces.
     * \param[in]  u1: current estimate of the unknowns
     * \param[in]  p:  position of the first lagrange multiplier
     *                 in the residual
     * \param[in]  d:  space dimension
     * \param[in]  t:  beginning of the time step
     * \param[in]  dt: time increment
     * \param[in]  a:  normalisation factor
     */
    virtual void setValues(tfel::math::matrix<real>&,
                           tfel::math::vector<real>&,
                           const tfel::math::vector<real>&,
                           const tfel::math::vector<real>&,
                           const tfel::math::matrix<real>&,
                           const tfel::math::vector<real>&,
                           const unsigned short,
                           const unsigned short,
                           const real,
                           const real,
                           const real) const override;
    /*!
     * \param[in]  e    : strains
     * \param[in]  s    : ThermodynamicForcees
     * \param[in]  eeps : criterium value for strains
     * \param[in]  seps : criterium value for ThermodynamicForcees
     * \param[in]  t    : beginning of the time step
     * \param[in]  dt   : time increment
     */
    virtual bool checkConvergence(const tfel::math::vector<real>&,
                                  const tfel::math::vector<real>&,
                                  const real,
                                  const real,
                                  const real,
                                  const real) const override;
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
        const real) const override;
    //! destructor
    virtual ~ImposedThermodynamicForce();

   protected:
    ImposedThermodynamicForce& operator=(const ImposedThermodynamicForce&) =
        delete;
    ImposedThermodynamicForce& operator=(ImposedThermodynamicForce&&) = delete;
    //! thermodynamic force evolution
    const std::shared_ptr<Evolution> sev;
    //! component value
    unsigned short c;
  };  // end of struct ImposedThermodynamicForce

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTIMPOSEDTHERMODYNAMICFORCE_HXX */
