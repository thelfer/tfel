/*!
 * \file   PipeElement.hxx
 * \brief
 * \author Thomas Helfer
 * \date   10 déc. 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_PIPEELEMENT_HXX
#define LIB_MTEST_PIPEELEMENT_HXX

#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/matrix.hxx"
#include "MTest/Types.hxx"
#include "MTest/SolverOptions.hxx"

namespace mtest {

  // forward declaration
  struct Behaviour;
  // forward declaration
  struct StructureCurrentState;
  // forward declaration
  struct PipeMesh;

  /*!
   * \brief structure describing a Hybrid High Order element for pipes
   */
  struct PipeElement {
    /*!
     * \return the number of nodes of the element
     * This number of nodes is used to compute the number of unknowns.
     */
    virtual size_type getNumberOfNodes() const = 0;
    /*!
     * \return the number of integration points of the element
     */
    virtual size_type getNumberOfIntegrationPoints() const = 0;
    /*!
     * \brief set the position of the gauss points for the whole mesh
     * \param[out] scs: structure current state
     */
    virtual void setIntegrationPointsPositions(
        StructureCurrentState&) const = 0;
    /*!
     * \brief compute the strain
     * \param[out] scs: structure current state
     * \param[in]  u0:  displacement at the beginnig of the time step
     * \param[in]  b: if true, compute the strain at end of time
     * step. If false, compute the strain at the beginning of the time
     * step
     */
    virtual void computeStrain(StructureCurrentState&,
                               const tfel::math::vector<real>&,
                               const bool) const = 0;
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
     * \param[in]  dt:  time increment
     * \param[in]  mt:  stiffness matrix type
     * \param[in]  i:   element number
     */
    virtual std::pair<bool, real> updateStiffnessMatrixAndInnerForces(
        tfel::math::matrix<real>&,
        tfel::math::vector<real>&,
        StructureCurrentState&,
        const tfel::math::vector<real>&,
        const real,
        const StiffnessMatrixType) const = 0;
    //! destructor
    virtual ~PipeElement() noexcept;
  };  // end of struct PipeElement

}  // end of namespace mtest

#endif /* LIB_MTEST_PIPEELEMENT_HXX */
