/*!
 * \file   PipeQuadraticElement.hxx
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

#ifndef LIB_MTEST_PIPEQUADRATICELEMENT_HXX
#define LIB_MTEST_PIPEQUADRATICELEMENT_HXX

#include<iosfwd>

#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/matrix.hxx"
#include"TFEL/Math/General/ConstExprMathFunctions.hxx"
#include"MTest/Types.hxx"
#include"MTest/SolverOptions.hxx"
#include"MTest/PipeElement.hxx"

namespace mtest{

  // forward declaration
  struct Behaviour;
  // forward declaration
  struct StructureCurrentState;
  // forward declaration
  struct PipeMesh;

  /*!
   * \brief structure describing a quadratic element for pipes
   */
  struct PipeQuadraticElement : PipeElement {
#ifndef _MSC_VER
    // absolute value of the Gauss points position in the reference
    // element
    static constexpr const real abs_pg =
      tfel::math::constexpr_fct::sqrt(real(3)/real(5));
    // value of the Gauss points position in the reference element
    static constexpr const real pg_radii[3] = {-abs_pg,real(0),abs_pg};
    // Gauss point weight
    static constexpr const real wg[3] =  {real(5.)/real(9.),
					  real(8.)/real(9.),
					  real(5.)/real(9.)};
#else /* _MSC_VER */
    // value of the Gauss points position in the reference element
    static const real pg_radii[3];
    // Gauss point weight
    static const real wg[3];
#endif  /* _MSC_VER */
    /*!
     * \brief interpolate nodal value in the element
     * \param[in] v0: value at the first node
     * \param[in] v1: value at the second node
     * \param[in] v1: value at the third node
     * \param[in] x:  position in the reference element (-1<x<1)
     */
    static real interpolate(const real, const real, const real, const real);

    /*!
     * \brief constructor
     * \param[in] n: index of this element
     */
    PipeQuadraticElement(const size_t) noexcept;

    void setGaussPointsPositions(StructureCurrentState&,
                                 const PipeMesh&) const override;

    void computeStrain(StructureCurrentState&,
                       const PipeMesh&,
                       const tfel::math::vector<real>&,
                       const bool) const override;

    std::pair<bool, real> updateStiffnessMatrixAndInnerForces(
        tfel::math::matrix<real>&,
        tfel::math::vector<real>&,
        StructureCurrentState&,
        const Behaviour&,
        const tfel::math::vector<real>&,
        const PipeMesh&,
        const real,
        const StiffnessMatrixType) const override;
    //! \brief destructor
    ~PipeQuadraticElement() noexcept override;

   private:
    //! index of the element
    const size_t index;
  };  // end of struct PipeQuadraticElement

} // end of namespace mtest

#endif /* LIB_MTEST_PIPEQUADRATICELEMENT_HXX */
