/*!
 * \file   PipeCubicElement.hxx
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

#ifndef LIB_MTEST_PIPECUBICELEMENT_HXX
#define LIB_MTEST_PIPECUBICELEMENT_HXX

#include<iosfwd>

#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/matrix.hxx"
#include"TFEL/Math/General/ConstExprMathFunctions.hxx"
#include"MTest/Types.hxx"
#include"MTest/SolverOptions.hxx"
#include"MTest/PipeElementBase.hxx"

namespace mtest{
  
  /*!
   * \brief structure describing a cubic element for pipes
   */
  struct PipeCubicElement : PipeElementBase {
#ifndef _MSC_VER
    // value of the Integration points position in the reference element
    static constexpr const real pg_radii[4] = {
        -0.861136311594053, -0.339981043584856, 0.339981043584856,
        0.861136311594053};
    // Integration point weight
    static constexpr const real wg[4] = {0.347854845137454, 0.652145154862546,
                                         0.652145154862546, 0.347854845137454};
#else /* _MSC_VER */
    // value of the Integration points position in the reference element
    static const real pg_radii[4];
    // Integration point weight
    static const real wg[4];
#endif  /* _MSC_VER */
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
     * \brief constructor
     * \param[in] m: mesh
     * \param[in] b: behaviour
     * \param[in] n: element number
     */
    PipeCubicElement(const PipeMesh&, const Behaviour&, const size_t);

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
    ~PipeCubicElement() noexcept override;

   private:
#ifndef _MSC_VER
    static constexpr const real one_third = real{ 1}/real{3};
    static constexpr const real cste      = real{ 9}/real{16};
    static constexpr const real cste2     = real{27}/real{16};
#else /* _MSC_VER */
    static const real one_third;
    static const real cste;
    static const real cste2;
#endif /* _MSC_VER */
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
  }; // end of struct PipeCubicElement

} // end of namespace mtest

#endif /* LIB_MTEST_PIPECUBICELEMENT_HXX */
