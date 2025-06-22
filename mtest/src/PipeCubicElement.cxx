/*!
 * \file   PipeCubicElement.cxx
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

#include <ostream>
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/RoundingMode.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/StructureCurrentState.hxx"
#include "MTest/PipeCubicElement.hxx"

namespace mtest {

#ifndef _MSC_VER
  constexpr const real PipeCubicElement::one_third;
  constexpr const real PipeCubicElement::cste;
  constexpr const real PipeCubicElement::cste2;
  constexpr const real PipeCubicElement::wg[4];
  constexpr const real PipeCubicElement::pg_radii[4];
#else  /* _MSC_VER */
  const real PipeCubicElement::one_third = real{1} / real{3};
  const real PipeCubicElement::cste = real{9} / real{16};
  const real PipeCubicElement::cste2 = real{27} / real{16};
  const real PipeCubicElement::pg_radii[4] = {
      -0.861136311594053, -0.339981043584856, 0.339981043584856,
      0.861136311594053};
  const real PipeCubicElement::wg[4] = {0.347854845137454, 0.652145154862546,
                                        0.652145154862546, 0.347854845137454};
#endif /* _MSC_VER */

  constexpr real PipeCubicElement::sf0(const real x) {
    return cste * (1 - x) * (x - one_third) * (x + one_third);
  }

  constexpr real PipeCubicElement::dsf0(const real x) {
    return (cste / 9) * ((-27 * x + 18) * x + 1);
  }

  constexpr real PipeCubicElement::sf1(const real x) {
    return cste2 * (x - 1) * (x + 1) * (x - one_third);
  }

  constexpr real PipeCubicElement::dsf1(const real x) {
    return (cste2 / 3) * ((9 * x - 2) * x - 3);
  }

  constexpr real PipeCubicElement::sf2(const real x) {
    return cste2 * (1 - x) * (x + 1) * (x + one_third);
  }

  constexpr real PipeCubicElement::dsf2(const real x) {
    return (cste2 / 3) * (3 - (9 * x + 2) * x);
  }

  constexpr real PipeCubicElement::sf3(const real x) {
    return cste * (1 + x) * (x - one_third) * (x + one_third);
  }

  constexpr real PipeCubicElement::dsf3(const real x) {
    return (cste / 9) * ((27 * x + 18) * x - 1);
  }

  real PipeCubicElement::interpolate(const real v0,
                                     const real v1,
                                     const real v2,
                                     const real v3,
                                     const real x) {
    return v0 * sf0(x) + v1 * sf1(x) + v2 * sf2(x) + v3 * sf3(x);
  }  // end of PipeCubicElement::interpolate

  constexpr real PipeCubicElement::jacobian(const real r0,
                                            const real r1,
                                            const real r2,
                                            const real r3,
                                            const real x) {
    return r0 * dsf0(x) + r1 * dsf1(x) + r2 * dsf2(x) + r3 * dsf3(x);
  }  // end of PipeCubicElement::jacobian

  void PipeCubicElement::setGaussPointsPositions(StructureCurrentState& scs,
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
      const auto r1 = r0 + dr / 3;
      // radial position of the third node
      const auto r2 = r0 + 2 * dr / 3;
      // radial position of the fourth node
      const auto r3 = r0 + dr;
      // loop over Gauss point
      for (const auto g : {0, 1, 2, 3}) {
        // current state
        auto& s = scs.istates[4 * i + g];
        // Gauss point position in the reference element
        const auto pg = pg_radii[g];
        // radial position of the Gauss point
        s.position = interpolate(r0, r1, r2, r3, pg);
      }
    }
  }

  void PipeCubicElement::computeStrain(StructureCurrentState& scs,
                                       const PipeMesh& m,
                                       const tfel::math::vector<real>& u,
                                       const size_t i,
                                       const bool b) {
    // number of elements
    const auto ne = size_t(m.number_of_elements);
    // number of nodes
    const auto n = 3 * ne + 1;
    // inner radius
    const auto Ri = m.inner_radius;
    // outer radius
    const auto Re = m.outer_radius;
    // radius increment
    const auto dr = (Re - Ri) / ne;
    // radial position of the first node
    const auto r0 = Ri + dr * i;
    // radial position of the second node
    const auto r1 = r0 + dr / 3;
    // radial position of the third node
    const auto r2 = r0 + 2 * dr / 3;
    // radial position of the fourth node
    const auto r3 = r0 + dr;
    // radial displacement of the first node
    const auto ur0 = u[3 * i];
    // radial displacement of the second node
    const auto ur1 = u[3 * i + 1];
    // radial displacement of the third node
    const auto ur2 = u[3 * i + 2];
    // radial displacement of the fourth node
    const auto ur3 = u[3 * i + 3];
    // axial strain
    const auto& ezz = u[n];
    // loop over Gauss point
    for (const auto g : {0, 1, 2, 3}) {
      // Gauss point position in the reference element
      const auto pg = pg_radii[g];
      // inverse of the jacobian
      const auto iJ = 1 / PipeCubicElement::jacobian(r0, r1, r2, r3, pg);
      // current state
      auto& s = scs.istates[4 * i + g];
      // radial position of the Gauss point
      const auto rg = s.position;
      // strain
      auto& e = b ? s.e1 : s.e0;
      e[0] =
          (ur0 * dsf0(pg) + ur1 * dsf1(pg) + ur2 * dsf2(pg) + ur3 * dsf3(pg)) *
          iJ;
      e[1] = ezz;
      e[2] = interpolate(ur0, ur1, ur2, ur3, pg) / rg;
    }
  }  // end of PipeCubicElement::computeStrain

  std::pair<bool, real> PipeCubicElement::updateStiffnessMatrixAndInnerForces(
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
    const auto n = 3 * ne + 1;
    // inner radius
    const auto Ri = m.inner_radius;
    // outer radius
    const auto Re = m.outer_radius;
    // radius increment
    const auto dr = (Re - Ri) / ne;
    // radial position of the first node
    const auto r0 = Ri + dr * i;
    // radial position of the second node
    const auto r1 = r0 + dr / 3;
    // radial position of the third node
    const auto r2 = r0 + 2 * dr / 3;
    // radial position of the fourth node
    const auto r3 = r0 + dr;
    /* inner forces */
    auto& bwk = scs.getBehaviourWorkSpace();
    // compute the strain
    computeStrain(scs, m, u1, i, true);
    // loop over Gauss point
    auto r_dt = real{};
    for (const auto g : {0, 1, 2, 3}) {
      // Gauss point position in the reference element
      const auto pg = pg_radii[g];
      // current state
      auto& s = scs.istates[4 * i + g];
      // radial position of the Gauss point
      const auto rg = s.position;
      const real sfv[4] = {sf0(rg), sf1(rg), sf2(rg), sf3(rg)};
      const real dsfv[4] = {dsf0(rg), dsf1(rg), dsf2(rg), dsf3(rg)};
      // jacobian of the transformation
      const auto J = PipeCubicElement::jacobian(r0, r1, r2, r3, pg);
      setRoundingMode();
      const auto rb = b.integrate(s, bwk, dt, mt);
      setRoundingMode();
      r_dt = (g == 0) ? rb.second : std::min(rb.second, r_dt);
      if (!rb.first) {
        if (mfront::getVerboseMode() > mfront::VERBOSE_QUIET) {
          auto& log = mfront::getLogStream();
          log << "PipeCubicElement::computeStiffnessMatrixAndResidual : "
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
      for (const auto j : {0, 1, 2, 3}) {
        r[3 * i + j] += w * (rg * pi_rr * dsfv[j] / J + pi_tt * sfv[j]);
      }
      // axial forces
      r[n] += w * rg * pi_zz;
      // jacobian matrix
      if (mt != StiffnessMatrixType::NOSTIFFNESS) {
        const auto& bk = bwk.k;
        for (const auto l : {0, 1, 2, 3}) {
          for (const auto j : {0, 1, 2, 3}) {
            const auto de0_du = dsfv[j] / J;
            const auto de2_du = sfv[j] / rg;
            k(3 * i + l, 3 * i + j) +=
                w *
                (rg * dsfv[l] / J * (bk(0, 0) * de0_du + bk(0, 2) * de2_du) +
                 sfv[l] * (bk(2, 0) * de0_du + bk(2, 2) * de2_du));
          }
          k(3 * i + l, n) +=
              w * (rg * dsfv[l] / J * bk(0, 1) + bk(2, 1) * sfv[l]);
        }  // loop over nodes
        for (const auto j : {0, 1, 2, 3}) {
          const auto de0_du = dsfv[j] / J;
          const auto de2_du = sfv[j] / rg;
          k(n, 3 * i + j) += w * rg * (bk(1, 0) * de0_du + bk(1, 2) * de2_du);
        }
        k(n, n) += w * rg * bk(1, 1);
      }
    }  // loop over gauss point
    return {true, r_dt};
  }

}  // end of namespace mtest
