/*!
 * \file  bindings/python/mtest/MTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date 20/09/2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Raise.hxx"
#include "MTest/MTest.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/Constraint.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/AnalyticalTest.hxx"
#include "MTest/StructureCurrentState.hxx"
#include "MTest/ReferenceFileComparisonTest.hxx"
#include "MTest/NonLinearConstraint.hxx"
#include "MTest/ImposedGradient.hxx"
#include "MTest/ImposedThermodynamicForce.hxx"

namespace mtest {

  struct MTestCurrentState : public StudyCurrentState {};

}  // end of namespace mtest

template <typename Constraint>
static void MTest_setConstraint(mtest::MTest& t,
                                const std::string& n,
                                const mtest::real& v) {
  auto& b = *(t.getBehaviour());
  auto sev = mtest::make_evolution(v);
  auto sc = std::make_shared<Constraint>(b, n, sev);
  t.addEvolution(n, sev, false, true);
  t.addConstraint(sc);
}  // end of MTest_setConstraint

template <typename Constraint>
static void MTest_setConstraint(mtest::MTest& t,
                                const std::string& n,
                                const std::map<mtest::real, mtest::real>& v) {
  auto& b = *(t.getBehaviour());
  auto sev = mtest::make_evolution(v);
  auto sc = std::make_shared<Constraint>(b, n, sev);
  t.addEvolution(n, sev, false, true);
  t.addConstraint(sc);
}  // end of MTest_setConstraint

template <typename Constraint>
static void MTest_setConstraint(mtest::MTest& t,
                                const std::string& n,
                                const mtest::real& v,
                                const mtest::ConstraintOptions& opts) {
  auto& b = *(t.getBehaviour());
  auto sev = mtest::make_evolution(v);
  auto sc = std::make_shared<Constraint>(b, n, sev);
  mtest::applyConstraintOptions(*sc, opts);
  t.addEvolution(n, sev, false, true);
  t.addConstraint(sc);
}  // end of MTest_setConstraint

template <typename Constraint>
static void MTest_setConstraint(mtest::MTest& t,
                                const std::string& n,
                                const std::map<mtest::real, mtest::real>& v,
                                const mtest::ConstraintOptions& opts) {
  auto& b = *(t.getBehaviour());
  auto sev = mtest::make_evolution(v);
  auto sc = std::make_shared<Constraint>(b, n, sev);
  mtest::applyConstraintOptions(*sc, opts);
  t.addEvolution(n, sev, false, true);
  t.addConstraint(sc);
}  // end of MTest_setConstraint

static void MTest_setStrain(mtest::MTest& t,
                            const std::vector<mtest::real>& v) {
  using namespace tfel::material;
  const auto b = t.getBehaviourType();
  const auto k = t.getBehaviourKinematic();
  tfel::raise_if(
      !((b == MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) ||
        ((b == MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) &&
         (k == MechanicalBehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1))),
      "MTest::setStrain: this method is only valid "
      "small strain behaviour");
  t.setGradientsInitialValues(v);
}  // end of MTest_setStrain

static void MTest_setDeformationGradient(mtest::MTest& t,
                                         const std::vector<mtest::real>& v) {
  using namespace tfel::material;
  const auto b = t.getBehaviourType();
  const auto k = t.getBehaviourKinematic();
  tfel::raise_if(
      !((b == MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) &&
        (k == MechanicalBehaviourBase::FINITESTRAINKINEMATIC_F_CAUCHY)),
      "MTest::setDeformationGradient: this method is only valid "
      "for finite strain behaviour");
  t.setGradientsInitialValues(v);
}  // end of MTest_setDeformationGradient

static void MTest_setOpeningDisplacement(mtest::MTest& t,
                                         const std::vector<mtest::real>& v) {
  using namespace tfel::material;
  tfel::raise_if(
      t.getBehaviourType() != MechanicalBehaviourBase::COHESIVEZONEMODEL,
      "MTest::setOpeningDisplacement: "
      "this method is only valid "
      "for cohesive zone models");
  t.setGradientsInitialValues(v);
}  // end of MTest_setOpeningDisplacement

static void MTest_setStress(mtest::MTest& t,
                            const std::vector<mtest::real>& v) {
  using namespace tfel::material;
  const auto b = t.getBehaviourType();
  tfel::raise_if(
      (b != MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) &&
          (b != MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR),
      "MTest::setStress: this method is only valid "
      "for small or finite strain behaviours");
  t.setThermodynamicForcesInitialValues(v);
}  // end of MTest_setStress

static void MTest_setCohesiveForce(mtest::MTest& t,
                                   const std::vector<mtest::real>& v) {
  using namespace tfel::material;
  tfel::raise_if(
      t.getBehaviourType() != MechanicalBehaviourBase::COHESIVEZONEMODEL,
      "MTest::setCohesiveForce: "
      "this method is only valid "
      "for cohesive zone models");
  t.setThermodynamicForcesInitialValues(v);
}  // end of MTest_setCohesiveForce

static void MTest_setStrainEpsilon(mtest::MTest& t, const mtest::real& v) {
  using namespace tfel::material;
  const auto b = t.getBehaviourType();
  const auto k = t.getBehaviourKinematic();
  tfel::raise_if(
      !((b == MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) ||
        ((b == MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) &&
         (k == MechanicalBehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1))),
      "MTest::setStrainEpsilon: this method is only valid "
      "for small strain behaviour");
  t.setGradientEpsilon(v);
}

static void MTest_setDeformationGradientEpsilon(mtest::MTest& t,
                                                const mtest::real& v) {
  using namespace tfel::material;
  const auto b = t.getBehaviourType();
  const auto k = t.getBehaviourKinematic();
  tfel::raise_if(
      !((b == MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) &&
        (k == MechanicalBehaviourBase::FINITESTRAINKINEMATIC_F_CAUCHY)),
      "MTest::setDeformationGradientEpsilon: "
      "this method is only valid finite strain behaviour");
  t.setGradientEpsilon(v);
}

static void MTest_setOpeningDisplacementEpsilon(mtest::MTest& t,
                                                const mtest::real& v) {
  using namespace tfel::material;
  tfel::raise_if(
      t.getBehaviourType() != MechanicalBehaviourBase::COHESIVEZONEMODEL,
      "MTest::setOpeningDisplacementEpsilon: "
      "this method is only valid for cohesize zone model");
  t.setGradientEpsilon(v);
}

static void MTest_setStressEpsilon(mtest::MTest& t, const mtest::real& v) {
  using namespace tfel::material;
  const auto b = t.getBehaviourType();
  tfel::raise_if(
      (b != MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) &&
          (b != MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR),
      "MTest::setStressEpsilon: "
      "this method is only valid "
      "for small or finite strain behaviours");
  t.setThermodynamicForceEpsilon(v);
}

static void MTest_setCohesiveForceEpsilon(mtest::MTest& t,
                                          const mtest::real& v) {
  using namespace tfel::material;
  const auto b = t.getBehaviourType();
  tfel::raise_if(b != MechanicalBehaviourBase::COHESIVEZONEMODEL,
                 "MTest::setCohesiveForceEpsilon: "
                 "this method is only valid "
                 "cohesive zone model");
  t.setThermodynamicForceEpsilon(v);
}

static void MTest_setImposedThermodynamicForce1(mtest::MTest& t,
                                                const std::string& n,
                                                const mtest::real& v) {
  MTest_setConstraint<mtest::ImposedThermodynamicForce>(t, n, v);
}  // end of MTest_setImposedThermodynamicForce1

static void MTest_setImposedThermodynamicForce1b(
    mtest::MTest& t,
    const std::string& n,
    const mtest::real& v,
    const mtest::ConstraintOptions& opts) {
  MTest_setConstraint<mtest::ImposedThermodynamicForce>(t, n, v, opts);
}  // end of MTest_setImposedThermodynamicForce1b

static void MTest_setImposedThermodynamicForce2(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v) {
  MTest_setConstraint<mtest::ImposedThermodynamicForce>(t, n, v);
}  // end of MTest_setImposedThermodynamicForce2

static void MTest_setImposedThermodynamicForce2b(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v,
    const mtest::ConstraintOptions& opts) {
  MTest_setConstraint<mtest::ImposedThermodynamicForce>(t, n, v, opts);
}  // end of MTest_setImposedThermodynamicForce2b

static void MTest_setImposedStress1(mtest::MTest& t,
                                    const std::string& n,
                                    const mtest::real& v) {
  using namespace tfel::material;
  const auto b = t.getBehaviourType();
  tfel::raise_if(
      (b != MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) &&
          (b != MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR),
      "MTest::handleImposedStress : "
      "the setImposedStress method is only valid "
      "for small and finite strain behaviours");
  MTest_setImposedThermodynamicForce1(t, n, v);
}

static void MTest_setImposedStress1b(mtest::MTest& t,
                                     const std::string& n,
                                     const mtest::real& v,
                                     const mtest::ConstraintOptions& opts) {
  using namespace tfel::material;
  const auto b = t.getBehaviourType();
  tfel::raise_if(
      (b != MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) &&
          (b != MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR),
      "MTest::handleImposedStress : "
      "the setImposedStress method is only valid "
      "for small and finite strain behaviours");
  MTest_setImposedThermodynamicForce1b(t, n, v, opts);
}

static void MTest_setImposedStress2(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v) {
  using namespace tfel::material;
  const auto b = t.getBehaviourType();
  tfel::raise_if(
      (b != MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) &&
          (b != MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR),
      "MTestParser::handleImposedStress: "
      "the setImposedStress method is only valid "
      "for small and finite strain behaviours");
  MTest_setImposedThermodynamicForce2(t, n, v);
}  // end of MTest_setImposedStress2

static void MTest_setImposedStress2b(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v,
    const mtest::ConstraintOptions& opts) {
  using namespace tfel::material;
  const auto b = t.getBehaviourType();
  tfel::raise_if(
      (b != MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) &&
          (b != MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR),
      "MTestParser::handleImposedStress: "
      "the setImposedStress method is only valid "
      "for small and finite strain behaviours");
  MTest_setImposedThermodynamicForce2b(t, n, v, opts);
}  // end of MTest_setImposedStress2b

static void MTest_setImposedCohesiveForce1(mtest::MTest& t,
                                           const std::string& n,
                                           const mtest::real& v) {
  using namespace tfel::material;
  const auto b = t.getBehaviourType();
  tfel::raise_if(b != MechanicalBehaviourBase::COHESIVEZONEMODEL,
                 "MTestParser::handleImposedCohesiveForce: "
                 "the setImposedCohesiveForce method is only valid "
                 "for small strain behaviours");
  MTest_setImposedThermodynamicForce1(t, n, v);
}  // end of MTest_setImposedCohesiveForce1

static void MTest_setImposedCohesiveForce1b(
    mtest::MTest& t,
    const std::string& n,
    const mtest::real& v,
    const mtest::ConstraintOptions& opts) {
  using namespace tfel::material;
  const auto b = t.getBehaviourType();
  tfel::raise_if(b != MechanicalBehaviourBase::COHESIVEZONEMODEL,
                 "MTestParser::handleImposedCohesiveForce: "
                 "the setImposedCohesiveForce method is only valid "
                 "for small strain behaviours");
  MTest_setImposedThermodynamicForce1b(t, n, v, opts);
}  // end of MTest_setImposedCohesiveForce1b

static void MTest_setImposedCohesiveForce2(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v) {
  using namespace tfel::material;
  const auto b = t.getBehaviourType();
  tfel::raise_if(b != MechanicalBehaviourBase::COHESIVEZONEMODEL,
                 "MTestParser::handleImposedCohesiveForce: "
                 "the setImposedCohesiveForce method is only valid "
                 "for small strain behaviours");
  MTest_setImposedThermodynamicForce2(t, n, v);
}  // end of MTest_setImposedCohesiveForce2

static void MTest_setImposedCohesiveForce2b(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v,
    const mtest::ConstraintOptions& opts) {
  using namespace tfel::material;
  const auto b = t.getBehaviourType();
  tfel::raise_if(b != MechanicalBehaviourBase::COHESIVEZONEMODEL,
                 "MTestParser::handleImposedCohesiveForce: "
                 "the setImposedCohesiveForce method is only valid "
                 "for small strain behaviours");
  MTest_setImposedThermodynamicForce2b(t, n, v, opts);
}  // end of MTest_setImposedCohesiveForce2b

static void MTest_setNonLinearConstraint1(mtest::MTest& t,
                                          const std::string& f,
                                          const std::string& np) {
  using MB = tfel::material::MechanicalBehaviourBase;
  const auto b = t.getBehaviourType();
  if ((np == "Gradient") ||
      ((np == "Strain") && (b == MB::STANDARDSTRAINBASEDBEHAVIOUR)) ||
      ((np == "DeformationGradient") &&
       (b == MB::STANDARDFINITESTRAINBEHAVIOUR)) ||
      ((np == "OpeningDisplacement") && (b == MB::COHESIVEZONEMODEL))) {
    const auto p = mtest::NonLinearConstraint::DRIVINGVARIABLECONSTRAINT;
    auto c = std::make_shared<mtest::NonLinearConstraint>(
        *(t.getBehaviour()), f, t.getEvolutions(), p);
    t.addConstraint(c);
  } else if ((np == "ThermodynamicForce") ||
             ((np == "Stress") && ((b == MB::STANDARDSTRAINBASEDBEHAVIOUR) ||
                                   (b == MB::STANDARDFINITESTRAINBEHAVIOUR))) ||
             ((np == "CohesiveForce") && (b == MB::COHESIVEZONEMODEL))) {
    const auto p = mtest::NonLinearConstraint::THERMODYNAMICFORCECONSTRAINT;
    auto c = std::make_shared<mtest::NonLinearConstraint>(
        *(t.getBehaviour()), f, t.getEvolutions(), p);
    t.addConstraint(c);
  } else {
    tfel::raise(
        "MTest::setNonLinearConstraint: "
        "invalid normalisation policy");
  }
}  // end of MTest_setNonLinearConstraint1

static void MTest_setNonLinearConstraint1b(
    mtest::MTest& t,
    const std::string& f,
    const std::string& np,
    const mtest::ConstraintOptions& opts) {
  using MB = tfel::material::MechanicalBehaviourBase;
  const auto b = t.getBehaviourType();
  if ((np == "Gradient") ||
      ((np == "Strain") && (b == MB::STANDARDSTRAINBASEDBEHAVIOUR)) ||
      ((np == "DeformationGradient") &&
       (b == MB::STANDARDFINITESTRAINBEHAVIOUR)) ||
      ((np == "OpeningDisplacement") && (b == MB::COHESIVEZONEMODEL))) {
    const auto p = mtest::NonLinearConstraint::DRIVINGVARIABLECONSTRAINT;
    auto c = std::make_shared<mtest::NonLinearConstraint>(
        *(t.getBehaviour()), f, t.getEvolutions(), p);
    mtest::applyConstraintOptions(*c, opts);
    t.addConstraint(c);
  } else if ((np == "ThermodynamicForce") ||
             ((np == "Stress") && ((b == MB::STANDARDSTRAINBASEDBEHAVIOUR) ||
                                   (b == MB::STANDARDFINITESTRAINBEHAVIOUR))) ||
             ((np == "CohesiveForce") && (b == MB::COHESIVEZONEMODEL))) {
    const auto p = mtest::NonLinearConstraint::THERMODYNAMICFORCECONSTRAINT;
    auto c = std::make_shared<mtest::NonLinearConstraint>(
        *(t.getBehaviour()), f, t.getEvolutions(), p);
    mtest::applyConstraintOptions(*c, opts);
    t.addConstraint(c);
  } else {
    tfel::raise(
        "MTest::setNonLinearConstraint: "
        "invalid normalisation policy");
  }
}  // end of MTest_setNonLinearConstraint1b

static void MTest_setImposedGradient1(mtest::MTest& t,
                                      const std::string& n,
                                      const mtest::real& v) {
  MTest_setConstraint<mtest::ImposedGradient>(t, n, v);
}  // end of MTest_setImposedGradient1

static void MTest_setImposedGradient1b(mtest::MTest& t,
                                       const std::string& n,
                                       const mtest::real& v,
                                       const mtest::ConstraintOptions& opts) {
  MTest_setConstraint<mtest::ImposedGradient>(t, n, v, opts);
}  // end of MTest_setImposedGradient1b

static void MTest_setImposedGradient2(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v) {
  MTest_setConstraint<mtest::ImposedGradient>(t, n, v);
}  // end of MTest_setImposedGradient2

static void MTest_setImposedGradient2b(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v,
    const mtest::ConstraintOptions& opts) {
  MTest_setConstraint<mtest::ImposedGradient>(t, n, v, opts);
}  // end of MTest_setImposedGradient2b

static void MTest_setImposedStrain1(mtest::MTest& t,
                                    const std::string& n,
                                    const mtest::real& v) {
  using namespace tfel::material;
  const auto b = t.getBehaviourType();
  tfel::raise_if(
      !((b == MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) ||
        ((b == MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) &&
         (t.getBehaviourKinematic() ==
          MechanicalBehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1))),
      "MTest::setImposedStrain: "
      "the setImposedStrain method is only valid "
      "for small strain behaviours");
  MTest_setImposedGradient1(t, n, v);
}  // end of MTest_setImposedStrain1

static void MTest_setImposedStrain1b(mtest::MTest& t,
                                     const std::string& n,
                                     const mtest::real& v,
                                     const mtest::ConstraintOptions& opts) {
  using namespace tfel::material;
  const auto b = t.getBehaviourType();
  tfel::raise_if(
      !((b == MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) ||
        ((b == MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) &&
         (t.getBehaviourKinematic() ==
          MechanicalBehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1))),
      "MTest::setImposedStrain: "
      "the setImposedStrain method is only valid "
      "for small strain behaviours");
  MTest_setImposedGradient1b(t, n, v, opts);
}  // end of MTest_setImposedStrain1b

static void MTest_setImposedStrain2(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v) {
  using namespace tfel::material;
  tfel::raise_if(!((t.getBehaviourType() ==
                    MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) ||
                   ((t.getBehaviourType() ==
                     MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) &&
                    (t.getBehaviourKinematic() ==
                     MechanicalBehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1))),
                 "MTest::handleImposedStrain: "
                 "the setImposedStrain method is only valid "
                 "for small strain behaviours");
  MTest_setImposedGradient2(t, n, v);
}  // end of MTest_setImposedStrain2

static void MTest_setImposedStrain2b(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v,
    const mtest::ConstraintOptions& opts) {
  using namespace tfel::material;
  tfel::raise_if(!((t.getBehaviourType() ==
                    MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) ||
                   ((t.getBehaviourType() ==
                     MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) &&
                    (t.getBehaviourKinematic() ==
                     MechanicalBehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1))),
                 "MTest::handleImposedStrain: "
                 "the setImposedStrain method is only valid "
                 "for small strain behaviours");
  MTest_setImposedGradient2b(t, n, v, opts);
}  // end of MTest_setImposedStrain2b

static void MTest_setImposedDeformationGradient1(mtest::MTest& t,
                                                 const std::string& n,
                                                 const mtest::real& v) {
  using namespace tfel::material;
  tfel::raise_if(!((t.getBehaviourType() ==
                    MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) &&
                   (t.getBehaviourKinematic() ==
                    MechanicalBehaviourBase::FINITESTRAINKINEMATIC_F_CAUCHY)),
                 "MTest::setImposedDeformationGradient: "
                 "the setImposedDeformationGradient method is only valid "
                 "for finite strain behaviours");
  MTest_setImposedGradient1(t, n, v);
}

static void MTest_setImposedDeformationGradient1b(
    mtest::MTest& t,
    const std::string& n,
    const mtest::real& v,
    const mtest::ConstraintOptions& opts) {
  using namespace tfel::material;
  tfel::raise_if(!((t.getBehaviourType() ==
                    MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) &&
                   (t.getBehaviourKinematic() ==
                    MechanicalBehaviourBase::FINITESTRAINKINEMATIC_F_CAUCHY)),
                 "MTest::setImposedDeformationGradient: "
                 "the setImposedDeformationGradient method is only valid "
                 "for finite strain behaviours");
  MTest_setImposedGradient1b(t, n, v, opts);
}  // end of MTest_setImposedDeformationGradient1b

static void MTest_setImposedDeformationGradient2(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v) {
  using namespace tfel::material;
  tfel::raise_if(!((t.getBehaviourType() ==
                    MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) &&
                   (t.getBehaviourKinematic() ==
                    MechanicalBehaviourBase::FINITESTRAINKINEMATIC_F_CAUCHY)),
                 "MTestParser::setImposedDeformationGradient: "
                 "the setImposedDeformationGradient method is only valid "
                 "for finite strain behaviours");
  MTest_setImposedGradient2(t, n, v);
}  // end of MTest_setImposedDeformationGradient2

static void MTest_setImposedDeformationGradient2b(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v,
    const mtest::ConstraintOptions& opts) {
  using namespace tfel::material;
  tfel::raise_if(!((t.getBehaviourType() ==
                    MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) &&
                   (t.getBehaviourKinematic() ==
                    MechanicalBehaviourBase::FINITESTRAINKINEMATIC_F_CAUCHY)),
                 "MTestParser::setImposedDeformationGradient: "
                 "the setImposedDeformationGradient method is only valid "
                 "for finite strain behaviours");
  MTest_setImposedGradient2b(t, n, v, opts);
}  // end of MTest_setImposedDeformationGradient2b

static void MTest_setImposedOpeningDisplacement1(mtest::MTest& t,
                                                 const std::string& n,
                                                 const mtest::real& v) {
  using namespace tfel::material;
  tfel::raise_if(
      t.getBehaviourType() != MechanicalBehaviourBase::COHESIVEZONEMODEL,
      "MTest::handleImposedOpeningDisplacement : "
      "the setImposedOpeningDisplacement method is only valid "
      "for small strain behaviours");
  MTest_setImposedGradient1(t, n, v);
}  // end of MTest_setImposedOpeningDisplacement1

static void MTest_setImposedOpeningDisplacement1b(
    mtest::MTest& t,
    const std::string& n,
    const mtest::real& v,
    const mtest::ConstraintOptions& opts) {
  using namespace tfel::material;
  tfel::raise_if(
      t.getBehaviourType() != MechanicalBehaviourBase::COHESIVEZONEMODEL,
      "MTest::handleImposedOpeningDisplacement : "
      "the setImposedOpeningDisplacement method is only valid "
      "for small strain behaviours");
  MTest_setImposedGradient1b(t, n, v, opts);
}  // end of MTest_setImposedOpeningDisplacement1b

static void MTest_setImposedOpeningDisplacement2(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v) {
  using namespace tfel::material;
  tfel::raise_if(
      t.getBehaviourType() != MechanicalBehaviourBase::COHESIVEZONEMODEL,
      "MTestParser::setImposedOpeningDisplacement : "
      "the setImposedOpeningDisplacement method is only valid "
      "for small strain behaviours");
  MTest_setImposedGradient2(t, n, v);
}  // end of MTest_setImposedOpeningDisplacement2

static void MTest_setImposedOpeningDisplacement2b(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v,
    const mtest::ConstraintOptions& opts) {
  using namespace tfel::material;
  tfel::raise_if(
      t.getBehaviourType() != MechanicalBehaviourBase::COHESIVEZONEMODEL,
      "MTestParser::setImposedOpeningDisplacement : "
      "the setImposedOpeningDisplacement method is only valid "
      "for small strain behaviours");
  MTest_setImposedGradient2b(t, n, v, opts);
}  // end of MTest_setImposedOpeningDisplacement2b

static mtest::MTestCurrentState MTestCurrentState_copy(
    const mtest::MTestCurrentState& src) {
  return src;
}

static mtest::MTestCurrentState MTestCurrentState_makeDeepCopy(
    const mtest::MTestCurrentState& src) {
  auto copy = mtest::MTestCurrentState{src};
  static_cast<mtest::StudyCurrentState&>(copy) = src.makeDeepCopy();
  return copy;
}

static void MTest_setRotationMatrix1(
    mtest::MTest& t,
    const std::vector<std::vector<mtest::real>>& m,
    const bool b) {
  tfel::math::tmatrix<3u, 3u, mtest::real> r;
  for (decltype(r.size()) i = 0; i != 3; ++i) {
    for (decltype(r.size()) j = 0; j != 3; ++j) {
      r(i, j) = m[i][j];
    }
  }
  t.setRotationMatrix(r, b);
}  // end of MTest_setRotationMatrix

static void MTest_setRotationMatrix2(
    mtest::MTest& t, const std::vector<std::vector<mtest::real>>& m) {
  MTest_setRotationMatrix1(t, m, false);
}  // end of MTest_setRotationMatrix

#define TFEL_PYTHON_MTESTCURRENTSTATEGETTER(X)                     \
  static tfel::math::vector<mtest::real> MTestCurrentState_get##X( \
      const mtest::MTestCurrentState& t) {                         \
    return t.X;                                                    \
  }

#define TFEL_PYTHON_MTESTCURRENTSTATEGETTER2(X) \
  static mtest::real MTestCurrentState_get##X(  \
      const mtest::MTestCurrentState& t) {      \
    return t.X;                                 \
  }

TFEL_PYTHON_MTESTCURRENTSTATEGETTER(u_1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(u0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(u1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER2(dt_1)

#define TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(X)                    \
  static tfel::math::vector<mtest::real> MTestCurrentState_get##X( \
      const mtest::MTestCurrentState& t) {                         \
    const auto& sc = t.getStructureCurrentState("");               \
    tfel::raise_if(sc.istates.size() != 1,                         \
                   "MTestCurrentState::get: "                      \
                   "uninitialized state");                         \
    return sc.istates[0].X;                                        \
  }

TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(s_1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(s0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(s1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(e0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(e1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(e_th0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(e_th1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(mprops1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(iv_1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(iv0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(iv1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(esv0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER3(desv)

static mtest::real MTestCurrentState_getTref(
    const mtest::MTestCurrentState& t) {
  const auto& sc = t.getStructureCurrentState("");
  tfel::raise_if(sc.istates.size() != 1,
                 "MTestCurrentState::get: "
                 "uninitialized state");
  return sc.istates[0].Tref;
}

static void setInternalStateVariableValue3(mtest::MTestCurrentState& t,
                                           const std::string& n,
                                           const mtest::real v,
                                           const int d) {
  auto& sc = t.getStructureCurrentState("");
  tfel::raise_if(sc.istates.size() != 1,
                 "MTestCurrentState::get: "
                 "uninitialized state");
  auto& s = sc.istates[0];
  mtest::setInternalStateVariableValue(s, n, v, d);
}

static void setInternalStateVariableValue(mtest::MTestCurrentState& t,
                                          const std::string& n,
                                          const mtest::real v) {
  setInternalStateVariableValue3(t, n, v, -1);
  setInternalStateVariableValue3(t, n, v, 0);
  setInternalStateVariableValue3(t, n, v, 1);
}

static void setInternalStateVariableValue4(mtest::MTestCurrentState& t,
                                           const std::string& n,
                                           const std::vector<mtest::real>& v,
                                           const int d) {
  auto& sc = t.getStructureCurrentState("");
  tfel::raise_if(sc.istates.size() != 1,
                 "MTestCurrentState::get: "
                 "uninitialized state");
  auto& s = sc.istates[0];
  mtest::setInternalStateVariableValue(s, n, v, d);
}

static void setInternalStateVariableValue2(mtest::MTestCurrentState& t,
                                           const std::string& n,
                                           const std::vector<mtest::real>& v) {
  setInternalStateVariableValue4(t, n, v, -1);
  setInternalStateVariableValue4(t, n, v, 0);
  setInternalStateVariableValue4(t, n, v, 1);
}

static pybind11::object getInternalStateVariableValue1(
    const mtest::MTestCurrentState& t, const std::string& n, const int i) {
  auto throw_if = [](const bool b, const std::string& m) {
    tfel::raise_if(b, "mtest::getInternalStateVariableValue: " + m);
  };
  const auto& sc = t.getStructureCurrentState("");
  throw_if(sc.istates.size() != 1,
           "MTestCurrentState::get: "
           "uninitialized state");
  auto& s = sc.istates[0];
  throw_if(s.behaviour == nullptr, "no behaviour defined");
  const auto& ivsnames = s.behaviour->getInternalStateVariablesNames();
  throw_if(std::find(ivsnames.begin(), ivsnames.end(), n) == ivsnames.end(),
           "the behaviour don't declare an internal state "
           "variable named '" +
               n + "'");
  const auto type = s.behaviour->getInternalStateVariableType(n);
  const auto size = [&s, throw_if,
                     type]() -> std::vector<mtest::real>::size_type {
    if (type == 0) {
      return 1;
    } else if (type == 1) {
      return tfel::material::getStensorSize(s.behaviour->getHypothesis());
    } else if (type == 2) {
      return tfel::material::getSpaceDimension(s.behaviour->getHypothesis());
    } else if (type == 3) {
      return tfel::material::getTensorSize(s.behaviour->getHypothesis());
    }
    throw_if(true, "unsupported variable type");
    return 0;
  }();
  const auto pos = s.behaviour->getInternalStateVariablePosition(n);
  throw_if((s.iv_1.size() < pos + size) || (s.iv0.size() < pos + size) ||
               (s.iv1.size() < pos + size),
           "invalid size for state variables (bad initialization)");
  const auto& iv = [&s, throw_if,
                    i]() -> const tfel::math::vector<mtest::real>& {
    throw_if((i != 1) && (i != 0) && (i != -1), "invalid depth");
    if (i == -1) {
      return s.iv_1;
    } else if (i == 0) {
      return s.iv0;
    }
    return s.iv1;
  }();
  if (type == 0) {
    return pybind11::cast(iv[pos]);
  }
  return pybind11::cast(
      std::vector<mtest::real>(iv.begin() + pos, iv.begin() + pos + size));
}

static pybind11::object getInternalStateVariableValue2(
    const mtest::MTestCurrentState& s, const std::string& n) {
  return getInternalStateVariableValue1(s, n, 1);
}  // end of getInternalStateVariableValue

static void MTest_addAnalyticalTest(mtest::MTest& m,
                                    const std::string& c,
                                    const std::string& f,
                                    const double eps) {
  const auto& g = mtest::buildValueExtractor(*(m.getBehaviour()), c);
  const auto& evms = m.getEvolutions();
  auto t = std::make_shared<mtest::AnalyticalTest>(f, c, g, evms, eps);
  m.addTest(t);
}

static void MTest_addReferenceFileComparisonTest1(mtest::MTest& m,
                                                  const std::string& c,
                                                  const std::string& f,
                                                  const unsigned short cn,
                                                  const double eps) {
  const tfel::utilities::TextData data(f);
  const auto& g = mtest::buildValueExtractor(*(m.getBehaviour()), c);
  m.addTest(std::make_shared<mtest::ReferenceFileComparisonTest>(data, cn, c, g,
                                                                 eps));
}

static void MTest_addReferenceFileComparisonTest2(mtest::MTest& m,
                                                  const std::string& c,
                                                  const std::string& f,
                                                  const std::string& cn,
                                                  const double eps) {
  const tfel::utilities::TextData data(f);
  const auto& evms = m.getEvolutions();
  const auto& g = mtest::buildValueExtractor(*(m.getBehaviour()), c);
  m.addTest(std::make_shared<mtest::ReferenceFileComparisonTest>(data, evms, cn,
                                                                 c, g, eps));
}

static void MTest_addEvent(mtest::MTest& m,
                           const std::string& n,
                           const mtest::real& t) {
  m.addEvent(n, std::vector<mtest::real>(1u, t));
}  // end of MTest_addEvent

void declareMTest(pybind11::module_&);

void declareMTest(pybind11::module_& m) {
  using namespace pybind11::literals;
  using namespace mtest;
  using tfel::tests::TestResult;

  pybind11::class_<MTestCurrentState, StudyCurrentState>(m, "MTestCurrentState")
      .def(pybind11::init<>())
      .def("copy", &MTestCurrentState_copy)
      .def("makeDeepCopy", &MTestCurrentState_makeDeepCopy)
      .def_readwrite("u_1", &MTestCurrentState::u_1)
      .def_readwrite("u0", &MTestCurrentState::u0)
      .def_readwrite("u1", &MTestCurrentState::u1)
      .def_property_readonly("s_1", MTestCurrentState_gets_1)
      .def_property_readonly("s0", MTestCurrentState_gets0)
      .def_property_readonly("s1", MTestCurrentState_gets1)
      .def_property_readonly("e0", MTestCurrentState_gete0)
      .def_property_readonly("e1", MTestCurrentState_gete1)
      .def_property_readonly("e_th0", MTestCurrentState_gete_th0)
      .def_property_readonly("e_th1", MTestCurrentState_gete_th1)
      .def_property_readonly("mprops1", MTestCurrentState_getmprops1)
      .def_property_readonly("iv_1", MTestCurrentState_getiv_1)
      .def_property_readonly("iv0", MTestCurrentState_getiv0)
      .def_property_readonly("iv1", MTestCurrentState_getiv1)
      .def_property_readonly("evs0", MTestCurrentState_getesv0)
      .def_property_readonly("desv", MTestCurrentState_getdesv)
      .def_property_readonly("dt_1", MTestCurrentState_getdt_1)
      .def_property_readonly("Tref", MTestCurrentState_getTref)
      .def("setInternalStateVariableValue", ::setInternalStateVariableValue,
           "set the value of a scalar internal state variable\n"
           "\n"
           "param[in]  n: variable name\n"
           "param[in]  v: values\n"
           "\n"
           "This overwrites the values of the internal state variables:\n"
           "- at the beginning of the previous time step\n"
           "- at the beginning of the current time step\n"
           "- at the end of the current time step")
      .def("setInternalStateVariableValue", ::setInternalStateVariableValue2,
           "set the value of an internal state variable\n"
           "\n"
           "param[in]  n: variable name\n"
           "param[in]  v: values\n"
           "\n"
           "This overwrites the values of the internal state variables:\n"
           "- at the beginning of the previous time step\n"
           "- at the beginning of the current time step\n"
           "- at the end of the current time step")
      .def("setInternalStateVariableValue", ::setInternalStateVariableValue3,
           "set the value of a scalar internal state variable\n"
           "\n"
           "param[in]  n: variable name\n"
           "param[in]  v: value\n"
           "param[in]  d: depth\n"
           "\n"
           "The depth value has the following meaning:\n"
           "- -1 means that we are modifying the internal state variable\n"
           "  value at the beginning of the previous time step\n"
           "- 0 means that we are modifying the internal state variable value\n"
           "  at the beginning of the current time step\n"
           "- 1 means that we are modifying the internal state variable value\n"
           "  at the end of the current time step")
      .def("setInternalStateVariableValue", ::setInternalStateVariableValue4,
           "set the value of an internal state variable\n"
           "\n"
           "param[in]  n: variable name\n"
           "param[in]  v: value\n"
           "param[in]  d: depth\n"
           "\n"
           "The depth value has the following meaning:\n"
           "- -1 means that we are modifying the internal state variable\n"
           "  value at the beginning of the previous time step\n"
           "- 0 means that we are modifying the internal state variable value\n"
           "  at the beginning of the current time step\n"
           "- 1 means that we are modifying the internal state variable value\n"
           "  at the end of the current time step")
      .def("getInternalStateVariableValue", ::getInternalStateVariableValue2,
           "get the value of an internal state variable at the end of the time "
           "\n"
           "step\n"
           "\n"
           "param[in]  n: variable name")
      .def("getInternalStateVariableValue", ::getInternalStateVariableValue2,
           "get the value of an internal state variable\n"
           "\n"
           "param[in]  n: variable name\n"
           "param[in]  d: depth\n"
           "\n"
           "The depth value has the following meaning:\n"
           "- -1 means that we request the value at the beginning of the "
           "previous time step\n"
           "- 0 means that we request the  value at the beginning of the "
           "current time step\n"
           "- 1 means that we request the  value at the end of the current "
           "time step");

  TestResult (MTest::*pm)(const bool) = &MTest::execute;
  TestResult (MTest::*pm2)() = &MTest::execute;
  void (MTest::*pm3)(StudyCurrentState&, SolverWorkSpace&, const real,
                     const real) = &MTest::execute;

  pybind11::class_<MTest, SingleStructureScheme>(m, "MTest")
      .def(pybind11::init<>())
      .def("execute", pm)
      .def("execute", pm2)
      .def("execute", pm3)
      .def("completeInitialisation", &MTest::completeInitialisation)
      .def("initializeCurrentState", &MTest::initializeCurrentState)
      .def("initializeWorkSpace", &MTest::initializeWorkSpace)
      .def("setEvolutionValue", &MTest::setEvolutionValue)
      .def("setGradientsInitialValues", &MTest::setGradientsInitialValues)
      .def("setStrain", MTest_setStrain)
      .def("setDeformationGradient", MTest_setDeformationGradient)
      .def("setOpeningDisplacement", MTest_setOpeningDisplacement)
      .def("setThermodynamicForcesInitialValues",
           &MTest::setThermodynamicForcesInitialValues)
      .def("setCohesiveForce", MTest_setCohesiveForce)
      .def("setStress", MTest_setStress)
      .def("setGradientEpsilon", &MTest::setGradientEpsilon)
      .def("setStrainEpsilon", MTest_setStrainEpsilon)
      .def("setDeformationGradientEpsilon", MTest_setDeformationGradientEpsilon)
      .def("setOpeningDisplacementEpsilon", MTest_setOpeningDisplacementEpsilon)
      .def("setThermodynamicForceEpsilon", &MTest::setThermodynamicForceEpsilon)
      .def("setStressEpsilon", MTest_setStressEpsilon)
      .def("setCohesiveForceEpsilon", MTest_setCohesiveForceEpsilon)
      .def(
          "setImposedStress", MTest_setImposedStress1, "name"_a, "values"_a,
          "This method specify the constant evolution of a stresses "
          "component.\n"
          "* The first parameter (string) is the name of the choosen "
          "stresses component. The allowed components (see the "
          "'setModellingHypothesis' method) are:\n"
          "- 'AxisymmetricalGeneralisedPlaneStrain' : SRR SZZ STT\n"
          "- 'Axisymmetrical'                       : SRR SZZ STT SRZ\n"
          "- 'PlaneStress'                          : SXX SYY SXY\n"
          "- 'PlaneStrain'                          : SXX SYY SZZ SXY\n"
          "- 'GeneralisedPlaneStrain'               : SXX SYY SZZ SXY\n"
          "- 'Tridimensional'                       : SXX SYY SZZ SXY SXZ SYZ\n"
          "* The second parameter (double) is the constant value "
          "of the selected stresses component.")
      .def(
          "setImposedStress", MTest_setImposedStress1b, "name"_a, "values"_a,
          "options"_a,
          "This method specify the constant evolution of a stresses "
          "component.\n"
          "* The first parameter (string) is the name of the choosen "
          "stresses component. The allowed components (see the "
          "'setModellingHypothesis' method) are:\n"
          "- 'AxisymmetricalGeneralisedPlaneStrain' : SRR SZZ STT\n"
          "- 'Axisymmetrical'                       : SRR SZZ STT SRZ\n"
          "- 'PlaneStress'                          : SXX SYY SXY\n"
          "- 'PlaneStrain'                          : SXX SYY SZZ SXY\n"
          "- 'GeneralisedPlaneStrain'               : SXX SYY SZZ SXY\n"
          "- 'Tridimensional'                       : SXX SYY SZZ SXY SXZ SYZ\n"
          "* The second parameter (double) is the constant value "
          "of the selected stresses component.")
      .def(
          "setImposedStress", MTest_setImposedStress2, "name"_a, "values"_a,
          "This method specify the linear evolution of a stresses component.\n"
          "* The first parameter (string) is the name of the choosen "
          "stresses component. The allowed components (see the "
          "'setModellingHypothesis' method) are:\n"
          "- 'AxisymmetricalGeneralisedPlaneStrain' : SRR SZZ STT\n"
          "- 'Axisymmetrical'                       : SRR SZZ STT SRZ\n"
          "- 'PlaneStress'                          : SXX SYY SXY\n"
          "- 'PlaneStrain'                          : SXX SYY SZZ SXY\n"
          "- 'GeneralisedPlaneStrain'               : SXX SYY SZZ SXY\n"
          "- 'Tridimensional'                       : SXX SYY SZZ SXY SXZ SYZ\n"
          "* The second parameter (map<double, double>) specify "
          "a linear evolution: the associative array of time-value "
          "is used to perform an interpolation. That interpolation "
          "is only performed between the minimum and maximum times "
          "given in the array. Should a time be out of the array, "
          "the returned value will be the one from the nearest "
          "association available.")
      .def(
          "setImposedStress", MTest_setImposedStress2b, "name"_a, "values"_a,
          "options"_a,
          "This method specify the linear evolution of a stresses component.\n"
          "* The first parameter (string) is the name of the choosen "
          "stresses component. The allowed components (see the "
          "'setModellingHypothesis' method) are:\n"
          "- 'AxisymmetricalGeneralisedPlaneStrain' : SRR SZZ STT\n"
          "- 'Axisymmetrical'                       : SRR SZZ STT SRZ\n"
          "- 'PlaneStress'                          : SXX SYY SXY\n"
          "- 'PlaneStrain'                          : SXX SYY SZZ SXY\n"
          "- 'GeneralisedPlaneStrain'               : SXX SYY SZZ SXY\n"
          "- 'Tridimensional'                       : SXX SYY SZZ SXY SXZ SYZ\n"
          "* The second parameter (map<double, double>) specify "
          "a linear evolution: the associative array of time-value "
          "is used to perform an interpolation. That interpolation "
          "is only performed between the minimum and maximum times "
          "given in the array. Should a time be out of the array, "
          "the returned value will be the one from the nearest "
          "association available.")
      .def("setImposedCohesiveForce", MTest_setImposedCohesiveForce1, "name"_a,
           "values"_a)
      .def("setImposedCohesiveForce", MTest_setImposedCohesiveForce1b, "name"_a,
           "values"_a, "options"_a)
      .def("setImposedCohesiveForce", MTest_setImposedCohesiveForce2, "name"_a,
           "values"_a)
      .def("setImposedCohesiveForce", MTest_setImposedCohesiveForce2b, "name"_a,
           "values"_a, "options"_a)
      .def("setImposedThermodynamicForce", MTest_setImposedThermodynamicForce1,
           "name"_a, "values"_a)
      .def("setImposedThermodynamicForce", MTest_setImposedThermodynamicForce1b,
           "name"_a, "values"_a, "options"_a)
      .def("setImposedThermodynamicForce", MTest_setImposedThermodynamicForce2,
           "name"_a, "values"_a)
      .def("setImposedThermodynamicForce", MTest_setImposedThermodynamicForce2b,
           "name"_a, "values"_a, "options"_a)
      .def("setNonLinearConstraint", MTest_setNonLinearConstraint1,
           "constraint"_a, "normalisation_policy"_a,
           "This method add a non linear constraint "
           "on driving variables or thermodynamic forces.\n"
           "The normalisation policy can have one of the following values:\n"
           "- `Gradient`, `Strain`, `DeformationGradient`, "
           "`OpeningDisplacement` "
           "stating that the constraint is of the order "
           "of magnitude of the driving variable\n"
           "- `ThermodynamicForce`, `Stress`, `CohesiveForce` "
           "stating that the constraint is of the order "
           "of magnitude of the thermodynamic force'\n")
      .def("setNonLinearConstraint", MTest_setNonLinearConstraint1b,
           "constraint"_a, "normalisation_policy"_a, "options"_a,
           "This method add a non linear constraint "
           "on driving variables or thermodynamic forces.\n"
           "The normalisation policy can have one of the following values:\n"
           "- `Gradient`, `Strain`, `DeformationGradient`, "
           "`OpeningDisplacement` "
           "stating that the constraint is of the order "
           "of magnitude of the driving variable\n"
           "- `ThermodynamicForce`, `Stress`, `CohesiveForce` "
           "stating that the constraint is of the order "
           "of magnitude of the thermodynamic force'\n")
      .def(
          "setImposedStrain", MTest_setImposedStrain1, "name"_a, "values"_a,
          "This method specify the constant evolution of a strains component.\n"
          "* The first parameter (string) is the name of the choosen "
          "strains component. The allowed components (see the "
          "'setModellingHypothesis' method) are:\n"
          "- AxisymmetricalGeneralisedPlaneStrain : ERR EZZ ETT\n"
          "- Axisymmetrical                       : ERR EZZ ETT ERZ\n"
          "- PlaneStress                          : EXX EYY EZZ EXY\n"
          "- PlaneStrain                          : EXX EYY     EXY\n"
          "- GeneralisedPlaneStrain               : EXX EYY EZZ EXY\n"
          "- Tridimensional                       :  EXX EYY EZZ EXY EXZ EYZ\n"
          "* The second parameter (double) is the constant value "
          "of the selected strains component.")
      .def(
          "setImposedStrain", MTest_setImposedStrain1b, "name"_a, "values"_a,
          "options"_a,
          "This method specify the constant evolution of a strains component.\n"
          "* The first parameter (string) is the name of the choosen "
          "strains component. The allowed components (see the "
          "'setModellingHypothesis' method) are:\n"
          "- AxisymmetricalGeneralisedPlaneStrain : ERR EZZ ETT\n"
          "- Axisymmetrical                       : ERR EZZ ETT ERZ\n"
          "- PlaneStress                          : EXX EYY EZZ EXY\n"
          "- PlaneStrain                          : EXX EYY     EXY\n"
          "- GeneralisedPlaneStrain               : EXX EYY EZZ EXY\n"
          "- Tridimensional                       :  EXX EYY EZZ EXY EXZ EYZ\n"
          "* The second parameter (double) is the constant value "
          "of the selected strains component.")
      .def("setImposedStrain", MTest_setImposedStrain2, "name"_a, "values"_a,
           "This method specify the linear evolution of a strains component.\n"
           "* The first parameter (string) is the name of the choosen "
           "strains component. The allowed components (see the "
           "'setModellingHypothesis' method) are:\n"
           "- AxisymmetricalGeneralisedPlaneStrain : ERR EZZ ETT\n"
           "- Axisymmetrical                       : ERR EZZ ETT ERZ\n"
           "- PlaneStress                          : EXX EYY EZZ EXY\n"
           "- PlaneStrain                          : EXX EYY     EXY\n"
           "- GeneralisedPlaneStrain               : EXX EYY EZZ EXY\n"
           "- Tridimensional                       : EXX EYY EZZ EXY EXZ EYZ\n"
           "* The second parameter (map<double, double>) specify "
           "a linear evolution: the associative array of time-value "
           "is used to perform an interpolation. That interpolation "
           "is only performed between the minimum and maximum times "
           "given in the array. Should a time be out of the array, "
           "the returned value will be the one from the nearest "
           "association available.")
      .def("setImposedStrain", MTest_setImposedStrain2b, "name"_a, "values"_a,
           "options"_a,
           "This method specify the linear evolution of a strains component.\n"
           "* The first parameter (string) is the name of the choosen "
           "strains component. The allowed components (see the "
           "'setModellingHypothesis' method) are:\n"
           "- AxisymmetricalGeneralisedPlaneStrain : ERR EZZ ETT\n"
           "- Axisymmetrical                       : ERR EZZ ETT ERZ\n"
           "- PlaneStress                          : EXX EYY EZZ EXY\n"
           "- PlaneStrain                          : EXX EYY     EXY\n"
           "- GeneralisedPlaneStrain               : EXX EYY EZZ EXY\n"
           "- Tridimensional                       : EXX EYY EZZ EXY EXZ EYZ\n"
           "* The second parameter (map<double, double>) specify "
           "a linear evolution: the associative array of time-value "
           "is used to perform an interpolation. That interpolation "
           "is only performed between the minimum and maximum times "
           "given in the array. Should a time be out of the array, "
           "the returned value will be the one from the nearest "
           "association available.")
      .def("setImposedDeformationGradient",
           MTest_setImposedDeformationGradient1, "name"_a, "values"_a,
           "This method specify the constant evolution of a deformation "
           "gradient component.\n"
           "* The first parameter (string) is the name of the choosen "
           "deformation gradient component. The allowed components (see "
           "the 'setModellingHypothesis' method) are:\n"
           "- 'AxisymmetricalGeneralisedPlaneStrain' : FRR FZZ FTT\n"
           "- 'Axisymmetrical'                       : FRR FZZ FTT ERZ FZR\n"
           "- 'PlaneStress'                          : FXX FYY FZZ FXY FYX\n"
           "- 'PlaneStrain'                          : FXX FYY     FXY FYX\n"
           "- 'GeneralisedPlaneStrain'               : FXX FYY FZZ FXY FYX\n"
           "- 'Tridimensional'                       : FXX FYY FZZ FXY FYX FXZ "
           "FZX FYZ FZY\n"
           "* The second parameter (double) is the constant value "
           "of the selected deformation gradient component.")
      .def("setImposedDeformationGradient",
           MTest_setImposedDeformationGradient1b, "name"_a, "values"_a,
           "options"_a,
           "This method specify the constant evolution of a deformation "
           "gradient component.\n"
           "* The first parameter (string) is the name of the choosen "
           "deformation gradient component. The allowed components (see "
           "the 'setModellingHypothesis' method) are:\n"
           "- 'AxisymmetricalGeneralisedPlaneStrain' : FRR FZZ FTT\n"
           "- 'Axisymmetrical'                       : FRR FZZ FTT ERZ FZR\n"
           "- 'PlaneStress'                          : FXX FYY FZZ FXY FYX\n"
           "- 'PlaneStrain'                          : FXX FYY     FXY FYX\n"
           "- 'GeneralisedPlaneStrain'               : FXX FYY FZZ FXY FYX\n"
           "- 'Tridimensional'                       : FXX FYY FZZ FXY FYX FXZ "
           "FZX FYZ FZY\n"
           "* The second parameter (double) is the constant value "
           "of the selected deformation gradient component.")
      .def("setImposedDeformationGradient",
           MTest_setImposedDeformationGradient2, "name"_a, "values"_a,
           "This method specify the linear evolution of a deformation "
           "gradient component.\n"
           "* The first parameter (string) is the name of the choosen "
           "deformation gradient component. The allowed components (see "
           "the 'setModellingHypothesis' method) are:\n"
           "- 'AxisymmetricalGeneralisedPlaneStrain' : FRR FZZ FTT\n"
           "- 'Axisymmetrical'                       : FRR FZZ FTT ERZ FZR\n"
           "- 'PlaneStress'                          : FXX FYY FZZ FXY FYX\n"
           "- 'PlaneStrain'                          : FXX FYY     FXY FYX\n"
           "- 'GeneralisedPlaneStrain'               : FXX FYY FZZ FXY FYX\n"
           "- 'Tridimensional'                       : FXX FYY FZZ FXY FYX FXZ "
           "FZX FYZ FZY\n"
           "* The second parameter (map<double, double>) specify "
           "a linear evolution: the associative array of time-value "
           "is used to perform an interpolation. That interpolation "
           "is only performed between the minimum and maximum times "
           "given in the array. Should a time be out of the array, "
           "the returned value will be the one from the nearest "
           "association available.")
      .def("setImposedDeformationGradient",
           MTest_setImposedDeformationGradient2b, "name"_a, "values"_a,
           "options"_a,
           "This method specify the linear evolution of a deformation "
           "gradient component.\n"
           "* The first parameter (string) is the name of the choosen "
           "deformation gradient component. The allowed components (see "
           "the 'setModellingHypothesis' method) are:\n"
           "- 'AxisymmetricalGeneralisedPlaneStrain' : FRR FZZ FTT\n"
           "- 'Axisymmetrical'                       : FRR FZZ FTT ERZ FZR\n"
           "- 'PlaneStress'                          : FXX FYY FZZ FXY FYX\n"
           "- 'PlaneStrain'                          : FXX FYY     FXY FYX\n"
           "- 'GeneralisedPlaneStrain'               : FXX FYY FZZ FXY FYX\n"
           "- 'Tridimensional'                       : FXX FYY FZZ FXY FYX FXZ "
           "FZX FYZ FZY\n"
           "* The second parameter (map<double, double>) specify "
           "a linear evolution: the associative array of time-value "
           "is used to perform an interpolation. That interpolation "
           "is only performed between the minimum and maximum times "
           "given in the array. Should a time be out of the array, "
           "the returned value will be the one from the nearest "
           "association available.")
      .def("setImposedOpeningDisplacement",
           MTest_setImposedOpeningDisplacement1, "name"_a, "values"_a)
      .def("setImposedOpeningDisplacement",
           MTest_setImposedOpeningDisplacement1b, "name"_a, "values"_a,
           "options"_a)
      .def("setImposedOpeningDisplacement",
           MTest_setImposedOpeningDisplacement2, "name"_a, "values"_a)
      .def("setImposedOpeningDisplacement",
           MTest_setImposedOpeningDisplacement2b, "name"_a, "values"_a,
           "options"_a)
      .def("setImposedGradient", MTest_setImposedGradient1, "name"_a,
           "values"_a)
      .def("setImposedGradient", MTest_setImposedGradient1b, "name"_a,
           "values"_a, "options"_a)
      .def("setImposedGradient", MTest_setImposedGradient2, "name"_a,
           "values"_a)
      .def("setImposedGradient", MTest_setImposedGradient2b, "name"_a,
           "values"_a, "options"_a)
      .def("setScalarInternalStateVariableInitialValue",
           &MTest::setScalarInternalStateVariableInitialValue)
      .def("setRotationMatrix", &MTest_setRotationMatrix1,
           "This method specify a rotation matrix, for the "
           "principal material directions to be different from "
           "those used for the resolution and the boundary "
           "conditions.\n"
           "* The parameter (vector<vector<double> >) specify the "
           "3x3 rotation matrix.")
      .def("setRotationMatrix", &MTest_setRotationMatrix2,
           "This method specify a rotation matrix, for the "
           "principal material directions to be different from "
           "those used for the resolution and the boundary "
           "conditions.\n"
           "* The parameter (vector<vector<double> >) specify the "
           "3x3 rotation matrix.")
      .def("setScalarInternalStateVariableInitialValue",
           &MTest::setScalarInternalStateVariableInitialValue)
      .def("setStensorInternalStateVariableInitialValues",
           &MTest::setStensorInternalStateVariableInitialValues)
      .def("setTensorInternalStateVariableInitialValues",
           &MTest::setTensorInternalStateVariableInitialValues)
      .def("addAnalyticalTest", MTest_addAnalyticalTest,
           "Add a test comparing the results to an analytical solution:\n"
           "- result to be tested (strain, stress, internal state variable)\n"
           "- formulae giving the analytical solution\n"
           "- test criterion\n")
      .def("addReferenceFileComparisonTest",
           MTest_addReferenceFileComparisonTest1,
           "Add a test comparing the results to the one given in a reference "
           "file:\n"
           "- result to be tested (strain, stress, internal state variable)\n"
           "- file name\n"
           "- column number\n"
           "- test criterion\n")
      .def("addReferenceFileComparisonTest",
           MTest_addReferenceFileComparisonTest2,
           "Add a test comparing the results to the one given in a reference "
           "file:\n"
           "- result to be tested (strain, stress, internal state variable)\n"
           "- file name\n"
           "- formulae used to extract the data from the file ($1 is the first "
           "column)\n"
           "- test criterion\n")
      .def("addEvent", &MTest::addEvent, "Add a new event")
      .def("addEvent", MTest_addEvent, "Add a new event")
      .def("completeInitialisation", &MTest::completeInitialisation,
           "complete the initialisation. This method must be called once. This "
           "method may be called automatically by the execute method.")
      .def("addUserDefinedPostProcessing", &MTest::addUserDefinedPostProcessing,
           "add a new user defined post-processing.\n"
           "- the first argument is the name of the output file\n"
           "- the second argument gives a list of post-processings as an array "
           "of "
           "  strings. Those strings defines formulae which are evaluated at "
           "the end "
           "  of the time step. Those formulae may depend on:\n"
           "    - the behaviour' driving variable\n"
           "    - the behaviour' thermodynamic forces\n"
           "    - the behaviour' internal state variables\n"
           "    - the behaviour' external state variables\n"
           "    - any evolution defined in the input file\n")
      .def("printLagrangeMultipliers", &MTest::printLagrangeMultipliers,
           "set if Lagrange multipliers shall be print in the output file")
      .def("setCompareToNumericalTangentOperator",
           &MTest::setCompareToNumericalTangentOperator,
           "set if a comparison of the tangent operator returned by the "
           "behaviour "
           "to a numerical approximation")
      .def("setTangentOperatorComparisonCriterion",
           &MTest::setTangentOperatorComparisonCriterion,
           "set the criterion used to compare the tangent operator returned "
           "by the behaviour and its numerical approximation")
      .def("setNumericalTangentOperatorPerturbationValue",
           &MTest::setNumericalTangentOperatorPerturbationValue,
           "set the perburtation of the gradient used to compute a numerical "
           "approximation of the tangent operator")
      .def("printLagrangeMultipliers", &MTest::printLagrangeMultipliers,
           "if True, prints the Lagrange mulpliers in the output file");

}  // end of declareExternalLibraryManager
