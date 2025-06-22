/*!
 * \file   PipeCubicElement.hxx
 * \brief
 * \author Thomas Helfer
 * \date   10 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_PIPECUBICELEMENT_HXX
#define LIB_MTEST_PIPECUBICELEMENT_HXX

#include <iosfwd>

#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/matrix.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "MTest/Types.hxx"
#include "MTest/SolverOptions.hxx"
#include "MTest/PipeMesh.hxx"

namespace mtest {

  // forward declaration
  struct Behaviour;
  // forward declaration
  struct StructureCurrentState;

  /*!
   * \brief structure describing a cubic element for pipes
   */
  struct PipeCubicElement {
    // value of the Gauss points position in the reference element
    static constexpr real pg_radii[4] = {-0.861136311594053, -0.339981043584856,
                                         0.339981043584856, 0.861136311594053};
    // Gauss point weight
    static constexpr real wg[4] = {0.347854845137454, 0.652145154862546,
                                   0.652145154862546, 0.347854845137454};
    /*!
     * \brief set the position of the gauss points
     * \param[out] scs: structure current state
     * \param[in]  m:   pipe mesh
     */
    static void setGaussPointsPositions(StructureCurrentState&,
                                        const PipeMesh&);
    /*!
     * \brief interpolate nodal value in the element
     * \param[in] v0: value at the first node
     * \param[in] v1: value at the second node
     * \param[in] v2: value at the third node
     * \param[in] v4: value at the fourth node
     * \param[in] x:  position in the reference element (-1<x<1)
     */
    static real interpolate(
        const real, const real, const real, const real, const real);
    /*!
     * \brief compute the strain
     * \param[out] scs: structure current state
     * \param[in]  m:   pipe mesh
     * \param[in]  u0:  displacement at the beginnig of the time step
     * \param[in]  i:   element number
     * \param[in]  b: if true, compute the strain at end of time
     * step. If false, compute the strain at the beginning of the time
     * step
     */
    static void computeStrain(StructureCurrentState&,
                              const PipeMesh&,
                              const tfel::math::vector<real>&,
                              const size_t,
                              const bool);
    /*!
     * \return a pair containing:
     * - a boolean syaing if the behaviour integration shall be
     *   performed
     * - a scaling factor that can be used to:
     *     - increase the time step if the integration was successfull
     *     - decrease the time step if the integration failed or if the
     *       results were not reliable (time step too large).
     * \param[out] k:   stiffness matrix
     * \param[out] r:   residual
     * \param[out] scs: structure current state
     * \param[in]  u1:  current displacement estimation
     * \param[in]  m:   pipe mesh
     * \param[in]  b:   behaviour
     * \param[in]  dt:  time increment
     * \param[in]  mt:  stiffness matrix type
     * \param[in]  i:   element number
     */
    static std::pair<bool, real> updateStiffnessMatrixAndInnerForces(
        tfel::math::matrix<real>&,
        tfel::math::vector<real>&,
        StructureCurrentState&,
        const Behaviour&,
        const tfel::math::vector<real>&,
        const PipeMesh&,
        const real,
        const StiffnessMatrixType,
        const size_t);

   private:
    static constexpr real one_third = real{1} / real{3};
    static constexpr real cste = real{9} / real{16};
    static constexpr real cste2 = real{27} / real{16};
    static constexpr inline real jacobian(
        const real, const real, const real, const real, const real);
    static constexpr inline real sf0(const real);
    static constexpr inline real dsf0(const real);
    static constexpr inline real sf1(const real);
    static constexpr inline real dsf1(const real);
    static constexpr inline real sf2(const real);
    static constexpr inline real dsf2(const real);
    static constexpr inline real sf3(const real);
    static constexpr inline real dsf3(const real);
  };  // end of struct PipeCubicElement

}  // end of namespace mtest

#endif /* LIB_MTEST_PIPECUBICELEMENT_HXX */
