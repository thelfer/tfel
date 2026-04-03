/*!
 * \file   mtest/src/OxidationStatusEvolution.cxx
 * \brief
 * \author Thomas Helfer
 * \date   03/10/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MTest/PipeMesh.hxx"
#include "MTest/OxidationStatusEvolution.hxx"

namespace mtest {

  OxidationStatusEvolution::OxidationStatusEvolution(const PipeMesh& mesh,
                                                     const EvolutionManager& m)
      : inner_radius(mesh.inner_radius),
        outer_radius(mesh.outer_radius),
        evm(m) {}  // end of OxidationStatusEvolution

  real OxidationStatusEvolution::operator()(const real t) const {
    const auto p = evm.find("r");
    tfel::raise_if(
        p == evm.end(),
        "OxidationStatusEvolution::operator(): radial position undeclared.");
    const auto r = p->second->operator()(t);
    const auto l1 = this->getInnerBoundaryOxidationLength(t);
    const auto l2 = this->getOuterBoundaryOxidationLength(t);
    if ((r < this->inner_radius + l1) || (r > this->outer_radius - l2)) {
      return 1;
    }
    return 0;
  }  // end of operator t

  bool OxidationStatusEvolution::isConstant() const {
    return false;
  }  // end of false

  void OxidationStatusEvolution::setValue(const real) {
    tfel::raise(
        "OxidationStatusEvolution::setValue: "
        "this method does not makes sense "
        "for function evolution");
  }

  void OxidationStatusEvolution::setValue(const real, const real) {
    tfel::raise(
        "OxidationStatusEvolution::setValue: "
        "this method does not makes sense "
        "for function evolution");
  }

  void OxidationStatusEvolution::setInnerBoundaryOxidationLengthEvolution(
      const real t, const real v) {
    this->inner_boundary_oxidation_length_evolution[t] = v;
  }

  void OxidationStatusEvolution::setOuterBoundaryOxidationLengthEvolution(
      const real t, const real v) {
    this->outer_boundary_oxidation_length_evolution[t] = v;
  }

  real OxidationStatusEvolution::getInnerBoundaryOxidationLength(
      const real t) const {
    if (this->inner_boundary_oxidation_length_evolution.empty()) {
      return real{};
    }
    return LPIEvolution::interpolate(
        this->inner_boundary_oxidation_length_evolution, t);
  }  // end of getInnerBoundaryOxidationLength

  real OxidationStatusEvolution::getOuterBoundaryOxidationLength(
      const real t) const {
    if (this->outer_boundary_oxidation_length_evolution.empty()) {
      return real{};
    }
    return LPIEvolution::interpolate(
        this->outer_boundary_oxidation_length_evolution, t);
  }  // end of getOuterBoundaryOxidationLength

  OxidationStatusEvolution::~OxidationStatusEvolution() = default;

}  // end of namespace mtest
