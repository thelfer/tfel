/*!
 * \file  mtest/include/MTest/ImposedGradient.hxx
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

#ifndef LIB_MTEST_MTESTIMPOSEDDRIVINGVARIABLE_HXX
#define LIB_MTEST_MTESTIMPOSEDDRIVINGVARIABLE_HXX

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

  /*!
   * Impose the value of a driving variable component
   */
  struct MTEST_VISIBILITY_EXPORT ImposedGradient final : public ConstraintBase {
    /*!
     * \brief constructor
     * \param[in] b : behaviour
     * \param[in] c : driving variable component
     * \param[in] s : driving variable evolution
     */
    ImposedGradient(const Behaviour&,
                    const std::string&,
                    const std::shared_ptr<Evolution>);
    //
    ImposedGradient& operator=(const ImposedGradient&) = delete;
    ImposedGradient& operator=(ImposedGradient&&) = delete;
    /*!
     * \brief constructor
     * \param[in] c : component
     * \param[in] s : driving variable evolution
     */
    ImposedGradient(const unsigned short c, const std::shared_ptr<Evolution>);
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
    ~ImposedGradient() override;

   protected:
    //! driving variable evolution
    const std::shared_ptr<Evolution> eev;
    //! component value
    unsigned short c;
  };  // end of struct ImposedGradient

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTIMPOSEDDRIVINGVARIABLE_HXX */
