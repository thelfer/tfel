/*!
 * \file   PipeQuadraticElement.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   10 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/StructureCurrentState.hxx"
#include "MTest/PipeQuadraticElement.hxx"

namespace mtest {

#ifndef _MSC_VER
  constexpr const real PipeQuadraticElement::abs_pg;
  constexpr const real PipeQuadraticElement::wg[3];
  constexpr const real PipeQuadraticElement::pg_radii[3];
#else  /* _MSC_VER */
  // value of the Gauss points position in the reference element
  const real PipeQuadraticElement::pg_radii[3] = {
      -std::sqrt(real(3) / real(5)), real(0), std::sqrt(real(3) / real(5))};
  // Gauss point weight
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

  void PipeQuadraticElement::setGaussPointsPositions(StructureCurrentState& scs,
                                                     const PipeMesh& m) {
    // number of elements
    const auto ne = size_t(m.number_of_elements);
    // inner radius
    const auto Ri = m.inner_radius;
    // outer radius
    const auto Re = m.outer_radius;
    // radius increment
    const auto dr = (Re - Ri) / ne;
    for (size_t i = 0; i != ne; ++i) {
      // radial position of the first node
      const auto r0 = Ri + dr * i;
      // radial position of the second node
      const auto r1 = r0 + dr / 2;
      // radial position of the second node
      const auto r2 = r0 + dr;
      // loop over Gauss point
      for (const auto g : {0, 1, 2}) {
        // current state
        auto& s = scs.istates[3 * i + g];
        // Gauss point position in the reference element
        const auto pg = pg_radii[g];
        // radial position of the Gauss point
        s.position = interpolate(r0, r1, r2, pg);
      }
    }
  }

  void PipeQuadraticElement::computeStrain(StructureCurrentState& scs,
                                           const PipeMesh& m,
                                           const tfel::math::vector<real>& u,
                                           const size_t i,
                                           const bool b) {
    // number of elements
    const auto ne = size_t(m.number_of_elements);
    // number of nodes
    const auto n = 2 * ne + 1;
    // inner radius
    const auto Ri = m.inner_radius;
    // outer radius
    const auto Re = m.outer_radius;
    // radius increment
    const auto dr = (Re - Ri) / ne;
    // radial position of the first node
    const auto r0 = Ri + dr * i;
    // radial position of the second node
    const auto r1 = r0 + dr / 2;
    // radial position of the second node
    const auto r2 = r0 + dr;
    // radial displacement of the first node
    const auto ur0 = u[2 * i];
    // radial displacement of the second node
    const auto ur1 = u[2 * i + 1];
    // radial displacement of the third node
    const auto ur2 = u[2 * i + 2];
    // axial strain
    const auto& ezz = u[n];
    // loop over Gauss point
    for (const auto g : {0, 1, 2}) {
      // Gauss point position in the reference element
      const auto pg = pg_radii[g];
      // inverse of the jacobian
      const auto iJ = 1 / (r0 * (pg - 0.5) + r2 * (pg + 0.5) - 2 * r1 * pg);
      // radial position of the Gauss point
      const auto rg = interpolate(r0, r1, r2, pg);
      // current state
      auto& s = scs.istates[3 * i + g];
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
      const Behaviour& b,
      const tfel::math::vector<real>& u1,
      const PipeMesh& m,
      const real dt,
      const StiffnessMatrixType mt,
      const size_t i) {
    //! a simple alias
    constexpr const real pi = 3.14159265358979323846;
    // number of elements
    const auto ne = size_t(m.number_of_elements);
    // number of nodes
    const auto n = 2 * ne + 1;
    // inner radius
    const auto Ri = m.inner_radius;
    // outer radius
    const auto Re = m.outer_radius;
    // radius increment
    const auto dr = (Re - Ri) / ne;
    // radial position of the first node
    const auto r0 = Ri + dr * i;
    // radial position of the second node
    const auto r1 = r0 + dr / 2;
    // radial position of the thrid node
    const auto r2 = r0 + dr;
    /* inner forces */
    auto& bwk = scs.getBehaviourWorkSpace();
    // compute the strain
    computeStrain(scs, m, u1, i, true);
    auto r_dt = real{};
    // loop over Gauss point
    for (const auto g : {0, 1, 2}) {
      // Gauss point position in the reference element
      const auto pg = pg_radii[g];
      // radial position of the Gauss point
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
      const auto rb = b.integrate(s, bwk, dt, mt);
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

}  // end of namespace mtest
