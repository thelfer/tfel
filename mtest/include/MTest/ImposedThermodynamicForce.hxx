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

#include <memory>
#include "TFEL/Math/matrix.hxx"
#include "TFEL/Math/vector.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MTest/Types.hxx"
#include "MTest/Config.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/ConstraintBase.hxx"

namespace mtest {

  // forward declaration
  struct Behaviour;

  //! \brief Impose the value of a thermodynamic force component
  struct MTEST_VISIBILITY_EXPORT ImposedThermodynamicForce final
      : public ConstraintBase {
    /*!
     * \brief constructor
     * \param[in] b : behaviour
     * \param[in] c : thermodynamic force component
     * \param[in] s : thermodynamic force evolution
     */
    ImposedThermodynamicForce(const Behaviour&,
                              const std::string&,
                              const std::shared_ptr<Evolution>);
    /*!
     * \brief constructor
     * \param[in] c : thermodynamic force component
     * \param[in] s : thermodynamic force evolution
     */
    ImposedThermodynamicForce(const unsigned short,
                              const std::shared_ptr<Evolution>);
    //
    ImposedThermodynamicForce& operator=(const ImposedThermodynamicForce&) =
        delete;
    ImposedThermodynamicForce& operator=(ImposedThermodynamicForce&&) = delete;
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
    ~ImposedThermodynamicForce() override;

   protected:
    //! \brief thermodynamic force evolution
    const std::shared_ptr<Evolution> sev;
    //! \brief component value
    unsigned short c;
  };  // end of struct ImposedThermodynamicForce

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTIMPOSEDTHERMODYNAMICFORCE_HXX */
