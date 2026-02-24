/*!
 * \file  mtest/include/MTest/Constraint.hxx
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

#ifndef LIB_MTEST_MTESTCONSTRAINT_HXX
#define LIB_MTEST_MTESTCONSTRAINT_HXX

#include <vector>
#include <string>
#include "TFEL/Math/matrix.hxx"
#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tvector.hxx"

#include "MTest/Config.hxx"
#include "MTest/Types.hxx"

namespace mtest {

  /*!
   * \brief a structure containing constraint' options
   */
  struct MTEST_VISIBILITY_EXPORT ConstraintOptions {
    //! default constructor
    ConstraintOptions();
    //! move constructor
    ConstraintOptions(ConstraintOptions&&);
    //! copy constructor
    ConstraintOptions(const ConstraintOptions&);
    //! move assignement
    ConstraintOptions& operator=(ConstraintOptions&&);
    //! standard assignement
    ConstraintOptions& operator=(const ConstraintOptions&);
    //! current state
    bool active = true;
    //! activating events
    std::vector<std::string> activating_events;
    //! desactivating events
    std::vector<std::string> desactivating_events;
  };  // end of struct ConstraintsOptions

  /*!
   * \brief Abstract class for all the constraints
   * imposed to a single mechanical point
   */
  struct MTEST_VISIBILITY_EXPORT Constraint {
    /*!
     * \brief treat the given event
     * \param[in] e :event
     * \return true if the event led to activate or desactivate the constraint
     */
    virtual bool treatEvent(const std::string&) = 0;
    /*!
     * \brief set the activating events
     * \param[in] evs: list of events
     */
    virtual void setActivatingEvents(const std::vector<std::string>&) = 0;
    /*!
     * \brief set the desactivating events
     * \param[in] evs: list of events
     */
    virtual void setDesactivatingEvents(const std::vector<std::string>&) = 0;
    /*!
     * \brief activate or desactivate the constraint
     * \param[in] b: state
     */
    virtual void setActive(const bool) = 0;
    //! \return if the constraint is active
    virtual bool isActive() const = 0;
    /*!
     * \return the number of Lagrange Multipliers
     * associated with this contraint
     */
    virtual unsigned short getNumberOfLagrangeMultipliers() const = 0;
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

  /*!
   * \brief apply options to the given constraint
   * \param[in] c: constraints
   * \param[in] o: options
   */
  MTEST_VISIBILITY_EXPORT void applyConstraintOptions(Constraint&,
                                                      const ConstraintOptions&);

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTCONSTRAINT_HXX */
