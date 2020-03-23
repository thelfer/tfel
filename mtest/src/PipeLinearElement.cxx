/*!
 * \file   mtest/src/PipeLinearElement.cxx
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
#include "MTest/PipeLinearElement.hxx"

namespace mtest {

#ifndef _MSC_VER
  constexpr const real PipeLinearElement::abs_pg;
  constexpr const real PipeLinearElement::wg;
  constexpr const real PipeLinearElement::pg_radii[2];
#else  /* _MSC_VER */
  const real PipeLinearElement::abs_pg = real(1) / (std::sqrt(3));
  const real PipeLinearElement::wg = 1;
  const real PipeLinearElement::pg_radii[2] = {-real(1) / (std::sqrt(3)),
                                               real(1) / (std::sqrt(3))};
#endif /* _MSC_VER */

  real PipeLinearElement::interpolate(const real v0,
                                      const real v1,
                                      const real x) {
    return 0.5 * ((1 - x) * v0 + (1 + x) * v1);
  }  // end of PipeLinearElement::interpolate

  PipeLinearElement::PipeLinearElement(const PipeMesh& m,
                                       const Behaviour& b,
                                       const size_t n) 
      : PipeElementBase(m, b, n) {
  }  // end of PipeLinearElement::PipeLinearElement

  size_type PipeLinearElement::getNumberOfNodes() const {
    return 2;
  }  // end of PipeLinearElement::getNumberOfNodes

  size_type PipeLinearElement::getNumberOfIntegrationPoints() const {
    return 2;
  }  // end of PipeLinearElement::getNumberOfIntegrationPoints

  void PipeLinearElement::setIntegrationPointsPositions(
      StructureCurrentState& scs) const {
    // loop over integration point
    for (const auto g : {0, 1}) {
      // current state
      auto& s = scs.istates[2 * (this->index) + g];
      // integration point position in the reference element
      const auto pg = pg_radii[g];
      // radial position of the integration point
      s.position = interpolate(this->inner_radius, this->outer_radius, pg);
    }
  }

  void PipeLinearElement::computeStrain(StructureCurrentState& scs,
                                        const tfel::math::vector<real>& u,
                                        const bool b) const {
    const auto dr = this->outer_radius - this->inner_radius;
    // radial displacement of the first node
    const auto ur0 = u[this->index];
    // radial displacement of the second node
    const auto ur1 = u[this->index + 1];
    // axial strain
    const auto& ezz = u.back();
    // loop over integration point
    for (const auto g : {0, 1}) {
      // integration point position in the reference element
      const auto pg = pg_radii[g];
      // radial position of the integration point
      const auto rg = interpolate(this->inner_radius, this->outer_radius, pg);
      // current state
      auto& s = scs.istates[2 * (this->index) + g];
      // strain
      auto& e = b ? s.e1 : s.e0;
      e[0] = (ur1 - ur0) / dr;
      e[1] = ezz;
      e[2] = interpolate(ur0, ur1, pg) / rg;
    }
  }  // end of PipeLinearElement::computeStrain

  std::pair<bool, real> PipeLinearElement::updateStiffnessMatrixAndInnerForces(
      tfel::math::matrix<real>& k,
      tfel::math::vector<real>& r,
      StructureCurrentState& scs,
      const tfel::math::vector<real>& u1,
      const real dt,
      const StiffnessMatrixType mt) const {
    //! a simple alias
    constexpr const real pi = 3.14159265358979323846;
    // size of the element
    const auto dr = this->outer_radius - this->inner_radius;
    // jacobian of the transformation
    const auto J = dr / 2;
    /* inner forces */
    auto& bwk = scs.getBehaviourWorkSpace();
    // compute the strain
    this->computeStrain(scs, u1, true);
    auto r_dt = real{};
    const auto n = static_cast<size_type>(r.size() - 1);
    // loop over integration point
    for (const auto g : {0, 1}) {
      // integration point position in the reference element
      const auto pg = pg_radii[g];
      // radial position of the integration point
      const auto rg = interpolate(this->inner_radius, this->outer_radius, pg);
      // current state
      auto& s = scs.istates[2 * (this->index) + g];
      auto sview = makeView(s);
      setRoundingMode();
      const auto rb = this->behaviour.integrate(sview, bwk, dt, mt);
      setRoundingMode();
      r_dt = (g == 0) ? rb.second : std::min(rb.second, r_dt);
      if (!rb.first) {
        if (mfront::getVerboseMode() > mfront::VERBOSE_QUIET) {
          auto& log = mfront::getLogStream();
          log << "PipeLinearElement::computeStiffnessMatrixAndResidual : "
              << "behaviour intregration failed" << std::endl;
        }
        return {false, r_dt};
      }
      // stress tensor
      const auto pi_rr = s.s1[0];
      const auto pi_zz = s.s1[1];
      const auto pi_tt = s.s1[2];
      const auto w = 2 * pi * wg * J;
      // innner forces
      const auto i = this->index;
      r[i] += w * (pi_rr * (-rg / dr) + pi_tt * (1 - pg) / 2);
      r[i + 1] += w * (pi_rr * (rg / dr) + pi_tt * (1 + pg) / 2);
      // axial forces
      r[n] += w * rg * pi_zz;
      // jacobian matrix
      if (mt != StiffnessMatrixType::NOSTIFFNESS) {
        const auto& bk = bwk.k;
        const real de10_dur0 = -1 / dr;
        const real de12_dur0 = (1 - pg) / (2 * rg);
        const real de10_dur1 = 1 / dr;
        const real de12_dur1 = (1 + pg) / (2 * rg);
        k(i, i) += w * (bk(0, 0) * de10_dur0 * (-rg / dr) +
                        bk(0, 2) * de12_dur0 * (-rg / dr) +
                        bk(2, 0) * de10_dur0 * (1 - pg) / 2 +
                        bk(2, 2) * de12_dur0 * (1 - pg) / 2);
        k(i, i + 1) += w * (bk(0, 0) * de10_dur1 * (-rg / dr) +
                            bk(0, 2) * de12_dur1 * (-rg / dr) +
                            bk(2, 0) * de10_dur1 * (1 - pg) / 2 +
                            bk(2, 2) * de12_dur1 * (1 - pg) / 2);
        k(i, n) += w * (bk(0, 1) * (-rg / dr) + bk(2, 1) * (1 - pg) / 2);
        k(i + 1, i) += w * (bk(0, 0) * de10_dur0 * (rg / dr) +
                            bk(0, 2) * de12_dur0 * (rg / dr) +
                            bk(2, 0) * de10_dur0 * (1 + pg) / 2 +
                            bk(2, 2) * de12_dur0 * (1 + pg) / 2);
        k(i + 1, i + 1) += w * (bk(0, 0) * de10_dur1 * (rg / dr) +
                                bk(0, 2) * de12_dur1 * (rg / dr) +
                                bk(2, 0) * de10_dur1 * (1 + pg) / 2 +
                                bk(2, 2) * de12_dur1 * (1 + pg) / 2);
        k(i + 1, n) += w * (bk(0, 1) * (rg / dr) + bk(2, 1) * (1 + pg) / 2);
        // axial forces
        k(n, i) += w * rg * (bk(1, 0) * de10_dur0 + bk(1, 2) * de12_dur0);
        k(n, i + 1) += w * rg * (bk(1, 0) * de10_dur1 + bk(1, 2) * de12_dur1);
        k(n, n) += w * rg * bk(1, 1);
      }
    }
    return {true, r_dt};
  }

  PipeLinearElement::~PipeLinearElement() = default;

}  // end of namespace mtest
