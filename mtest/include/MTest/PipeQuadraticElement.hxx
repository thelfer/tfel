/*!
 * \file   PipeQuadraticElement.hxx
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

#ifndef LIB_MTEST_PIPEQUADRATICELEMENT_HXX
#define LIB_MTEST_PIPEQUADRATICELEMENT_HXX

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
   * \brief structure describing a quadratic element for pipes
   */
  struct PipeQuadraticElement {
#ifndef _MSC_VER
    // absolute value of the Gauss points position in the reference
    // element
    static constexpr const real abs_pg =
        tfel::math::constexpr_fct::sqrt(real(3) / real(5));
    // value of the Gauss points position in the reference element
    static constexpr const real pg_radii[3] = {-abs_pg, real(0), abs_pg};
    // Gauss point weight
    static constexpr const real wg[3] = {
        real(5.) / real(9.), real(8.) / real(9.), real(5.) / real(9.)};
#else  /* _MSC_VER */
    // value of the Gauss points position in the reference element
    static const real pg_radii[3];
    // Gauss point weight
    static const real wg[3];
#endif /* _MSC_VER */
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
     * \param[in] v1: value at the third node
     * \param[in] x:  position in the reference element (-1<x<1)
     */
    static real interpolate(const real, const real, const real, const real);
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
  };  // end of struct PipeQuadraticElement

}  // end of namespace mtest

#endif /* LIB_MTEST_PIPEQUADRATICELEMENT_HXX */
