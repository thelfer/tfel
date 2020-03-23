/*!
 * \file   mtest/src/PipeQuadraticElement.cxx
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

#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/RoundingMode.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/CurrentStateView.hxx"
#include "MTest/StructureCurrentState.hxx"
#include "MTest/PipeMesh.hxx"
#include "MTest/PipeQuadraticElement.hxx"

namespace mtest {

#ifndef _MSC_VER
  constexpr const real PipeQuadraticElement::abs_pg;
  constexpr const real PipeQuadraticElement::wg[3];
  constexpr const real PipeQuadraticElement::pg_radii[3];
#else  /* _MSC_VER */
  // value of the integration points position in the reference element
  const real PipeQuadraticElement::pg_radii[3] = {
      -std::sqrt(real(3) / real(5)), real(0), std::sqrt(real(3) / real(5))};
  // integration point weight
  const real PipeQuadraticElement::wg[3] = {
      real(5.) / real(9.), real(8.) / real(9.), real(5.) / real(9.)};
#endif /* _MSC_VER */

  real PipeQuadraticElement::interpolate(const real v0,
                                         const real v1,
                                         const real v2,
                                         const real x) {
    return (-(1. - x) * x * v0 + (1 + x) * x * v2) / 2 +
           (1. + x) * (1. - x) * v1;
  }  // end of PipeQuadraticElement::interpolate

  PipeQuadraticElement::PipeQuadraticElement(const PipeMesh& m,
                                             const Behaviour& b,
                                             const size_t n) 
      : PipeElementBase(m, b, n) {
  }  // end of PipeQuadraticElement::PipeQuadraticElement

  size_type PipeQuadraticElement::getNumberOfNodes() const {
    return 3;
  }  // end of PipeQuadraticElement::getNumberOfNodes

  size_type PipeQuadraticElement::getNumberOfIntegrationPoints() const {
    return 3;
  }  // end of PipeQuadraticElement::getNumberOfIntegrationPoints

  void PipeQuadraticElement::setIntegrationPointsPositions(
      StructureCurrentState& scs) const {
    // radial position of the second node
    const auto r1 = (this->inner_radius + this->outer_radius) / 2;
    // loop over integration point
    for (const auto g : {0, 1, 2}) {
      // current state
      auto& s = scs.istates[3 * (this->index) + g];
      // integration point position in the reference element
      const auto pg = pg_radii[g];
      // radial position of the integration point
      s.position = interpolate(this->inner_radius, r1, this->outer_radius, pg);
    }
  }

  void PipeQuadraticElement::computeStrain(StructureCurrentState& scs,
                                           const tfel::math::vector<real>& u,
                                           const bool b) const {
    const auto r0 = this->inner_radius;
    const auto r2 = this->outer_radius;
    const auto r1 = (r0 + r2) / 2;
    // radial displacement of the first node
    const auto ur0 = u[2 * (this->index)];
    // radial displacement of the second node
    const auto ur1 = u[2 * (this->index) + 1];
    // radial displacement of the third node
    const auto ur2 = u[2 * (this->index) + 2];
    // axial strain
    const auto& ezz = u.back();
    // loop over integration point
    for (const auto g : {0, 1, 2}) {
      // integration point position in the reference element
      const auto pg = pg_radii[g];
      // inverse of the jacobian
      const auto iJ = 1 / (r0 * (pg - 0.5) + r2 * (pg + 0.5) - 2 * r1 * pg);
      // radial position of the integration point
      const auto rg = interpolate(r0, r1, r2, pg);
      // current state
      auto& s = scs.istates[3 * (this->index) + g];
      // strain
      auto& e = b ? s.e1 : s.e0;
      e[0] = ((pg - 0.5) * ur0 + (pg + 0.5) * ur2 - 2. * pg * ur1) * iJ;
      e[1] = ezz;
      e[2] = interpolate(ur0, ur1, ur2, pg) / rg;
    }
  }  // end of PipeQuadraticElement::computeStrain

  std::pair<bool, real>
  PipeQuadraticElement::updateStiffnessMatrixAndInnerForces(
      tfel::math::matrix<real>& k,
      tfel::math::vector<real>& r,
      StructureCurrentState& scs,
      const tfel::math::vector<real>& u1,
      const real dt,
      const StiffnessMatrixType mt) const {
    //! a simple alias
    constexpr const real pi = 3.14159265358979323846;
    const auto r0 = this->inner_radius;
    const auto r2 = this->outer_radius;
    const auto r1 = (r0 + r2) / 2;
    /* inner forces */
    auto& bwk = scs.getBehaviourWorkSpace();
    // compute the strain
    this->computeStrain(scs, u1, true);
    auto r_dt = real{};
    // loop over integration point
    const auto i = this->index;
    const auto n = static_cast<size_type>(r.size() - 1);
    for (const auto g : {0, 1, 2}) {
      setRoundingMode();
      // integration point position in the reference element
      const auto pg = pg_radii[g];
      // radial position of the integration point
      const auto rg = interpolate(r0, r1, r2, pg);
      // jacobian of the transformation
      const auto J = r0 * (pg - 0.5) + r2 * (pg + 0.5) - 2 * r1 * pg;
      // shape function value
      const real sf[3] = {-0.5 * (1. - pg) * pg, (1. + pg) * (1. - pg),
                          0.5 * (1 + pg) * pg};
      // shape function derivative
      const real dsf[3] = {pg - 0.5, -2. * pg, pg + 0.5};
      // current state
      auto& s = scs.istates[3 * i + g];
      auto sview = makeView(s);
      setRoundingMode();
      const auto rb = this->behaviour.integrate(sview, bwk, dt, mt);
      setRoundingMode();
      r_dt = (g == 0) ? rb.second : std::min(rb.second, r_dt);
      if (!rb.first) {
        if (mfront::getVerboseMode() > mfront::VERBOSE_QUIET) {
          auto& log = mfront::getLogStream();
          log << "PipeQuadraticElement::computeStiffnessMatrixAndResidual : "
              << "behaviour intregration failed" << std::endl;
        }
        return {false, r_dt};
      }
      // stress tensor
      const auto pi_rr = s.s1[0];
      const auto pi_zz = s.s1[1];
      const auto pi_tt = s.s1[2];
      const auto w = 2 * pi * wg[g] * J;
      // innner forces
      for (const auto j : {0, 1, 2}) {
        r[2 * i + j] += w * (rg * pi_rr * dsf[j] / J + pi_tt * sf[j]);
      }
      // axial forces
      r[n] += w * rg * pi_zz;
      // jacobian matrix
      if (mt != StiffnessMatrixType::NOSTIFFNESS) {
        const auto& bk = bwk.k;
        for (const auto l : {0, 1, 2}) {
          for (const auto j : {0, 1, 2}) {
            const auto de0_du = dsf[j] / J;
            const auto de2_du = sf[j] / rg;
            k(2 * i + l, 2 * i + j) +=
                w * (rg * dsf[l] / J * (bk(0, 0) * de0_du + bk(0, 2) * de2_du) +
                     sf[l] * (bk(2, 0) * de0_du + bk(2, 2) * de2_du));
          }
          k(2 * i + l, n) +=
              w * (rg * dsf[l] / J * bk(0, 1) + bk(2, 1) * sf[l]);
        }  // loop over nodes
        for (const auto j : {0, 1, 2}) {
          const auto de0_du = dsf[j] / J;
          const auto de2_du = sf[j] / rg;
          k(n, 2 * i + j) += w * rg * (bk(1, 0) * de0_du + bk(1, 2) * de2_du);
        }
        k(n, n) += w * rg * bk(1, 1);
      }
    }  // loop over gauss point
    return {true, r_dt};
  }

  PipeQuadraticElement::~PipeQuadraticElement() = default;

}  // end of namespace mtest

