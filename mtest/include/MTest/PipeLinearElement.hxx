/*!
 * \file   PipeLinearElement.hxx
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

#ifndef LIB_MTEST_PIPELINEARELEMENT_HXX
#define LIB_MTEST_PIPELINEARELEMENT_HXX

#include<iosfwd>

#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/matrix.hxx"
#include"TFEL/Math/General/ConstExprMathFunctions.hxx"
#include"MTest/Types.hxx"
#include"MTest/SolverOptions.hxx"
#include"MTest/PipeElementBase.hxx"

namespace mtest{

  // forward declaration
  struct Behaviour;
  // forward declaration
  struct StructureCurrentState;
  
  /*!
   * \brief structure describing a linear element for pipes
   */
  struct PipeLinearElement : PipeElementBase {
#ifndef _MSC_VER
    // absolute value of the Integration points position in the reference
    // element
    static constexpr const real abs_pg =
      real(1)/tfel::math::constexpr_fct::sqrt(real(3));
    // value of the Integration points position in the reference element
    static constexpr const real pg_radii[2] = {-abs_pg,abs_pg};
    // Integration point weight
    static constexpr const real wg = real(1);
#else /* _MSC_VER */
    // absolute value of the Integration points position in the reference
    // element
    static const real abs_pg;
    // value of the Integration points position in the reference element
    static const real pg_radii[2];
    // Integration point weight
    static const real wg;
#endif /* _MSC_VER */

    static real interpolate(const real, const real, const real);

    /*!
     * \brief constructor
     * \param[in] m: mesh
     * \param[in] b: behaviour
     * \param[in] n: element number
     */
    PipeLinearElement(const PipeMesh&, const Behaviour&, const size_t);

    size_type getNumberOfNodes() const override;

    size_type getNumberOfIntegrationPoints() const override;

    void setIntegrationPointsPositions(StructureCurrentState&) const override;

    void computeStrain(StructureCurrentState&,
                       const tfel::math::vector<real>&,
                       const bool) const override;

    std::pair<bool, real> updateStiffnessMatrixAndInnerForces(
        tfel::math::matrix<real>&,
        tfel::math::vector<real>&,
        StructureCurrentState&,
        const tfel::math::vector<real>&,
        const real,
        const StiffnessMatrixType) const override;
    //! \brief destructor
    ~PipeLinearElement() noexcept override;
  }; // end of struct PipeLinearElement

} // end of namespace mtest

#endif /* LIB_MTEST_PIPELINEARELEMENT_HXX */
