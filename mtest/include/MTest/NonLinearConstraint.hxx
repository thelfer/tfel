/*!
 * \file  mtest/include/MTest/NonLinearConstraint.hxx
 * \brief
 * \author Thomas Helfer
 * \brief  03/03/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTNONLINEARCONSTRAINT_HXX
#define LIB_MTEST_MTESTNONLINEARCONSTRAINT_HXX

#include <vector>
#include <string>
#include <memory>
#include "TFEL/Math/matrix.hxx"
#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/Evaluator.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MTest/Types.hxx"
#include "MTest/Config.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/ConstraintBase.hxx"

namespace mtest {

  // forward declaration
  struct Behaviour;

  //! \brief Impose a contraint on the driving variables.
  struct MTEST_VISIBILITY_EXPORT NonLinearConstraint final
      : public ConstraintBase {
    //! \brief define the normalisation policy
    enum NormalisationPolicy {
      /*!
       * \brief this policy assumes that the constraint is defined by
       * a function which values are of the order of magnitude of the
       * driving variables.
       */
      DRIVINGVARIABLECONSTRAINT,
      /*!
       * \brief this policy assumes that the constraint is defined by
       * a function which values are of the order of magnitude of the
       * thermodynamic forces.
       */
      THERMODYNAMICFORCECONSTRAINT
    };  // end of enum NormalisationPolicy
    /*!
     * constructor
     * \param[in] b_:  behaviour
     * \param[in] f:   analytic defintion of the constraint
     * \param[in] evm: evolution manager
     * \param[in] p:   normalisation policy
     */
    NonLinearConstraint(const Behaviour&,
                        const std::string&,
                        const EvolutionManager&,
                        const NormalisationPolicy);
    //
    NonLinearConstraint& operator=(const NonLinearConstraint&) = delete;
    NonLinearConstraint& operator=(NonLinearConstraint&&) = delete;
    //
    [[nodiscard]] unsigned short getNumberOfLagrangeMultipliers()
        const override;
    void setValues(tfel::math::matrix<real>&,
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
    [[nodiscard]] bool checkConvergence(const tfel::math::vector<real>&,
                                        const tfel::math::vector<real>&,
                                        const real,
                                        const real,
                                        const real,
                                        const real) const override;
    [[nodiscard]] std::string getFailedCriteriaDiagnostic(
        const tfel::math::vector<real>&,
        const tfel::math::vector<real>&,
        const real,
        const real,
        const real,
        const real) const override;
    //! \brief destructor
    ~NonLinearConstraint() override;

   protected:
    // Internal structure
    struct Constraint;
    /*!
     * \brief evaluate the constraint or one of its derivatives
     * \param[in]  d:  driving variables
     * \param[in]  s:  thermodynamic forces
     * \param[in]  t:  beginning of the time step
     * \param[in]  dt: time increment
     */
    [[nodiscard]] double eval(tfel::math::Evaluator&,
                              const tfel::math::vector<real>&,
                              const tfel::math::vector<real>&,
                              const real,
                              const real) const;
    //! \brief evaluation of the constraint
    std::shared_ptr<Constraint> c;
    //! normalisation policy
    const Behaviour& b;
  };  // end of struct NonLinearConstraint

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTNONLINEARCONSTRAINT_HXX */
