/*!
 * \file  bindings/python/mfront/MTest.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 20 sept. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>

#include "MTest/MTest.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/Constraint.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/StructureCurrentState.hxx"
#include "MTest/ImposedThermodynamicForce.hxx"
#include "MTest/ImposedDrivingVariable.hxx"

namespace mtest {

  struct MTestCurrentState : public StudyCurrentState {};

}  // end of namespace mtest

static void MTest_setStrain(mtest::MTest& t,
                            const std::vector<mtest::real>& v) {
  using namespace tfel::material;
  if (!((t.getBehaviourType() ==
         MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR) ||
        ((t.getBehaviourType() ==
          MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR) &&
         (t.getBehaviourKinematic() ==
          MechanicalBehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1)))) {
    throw(
        std::runtime_error("MTest::setStrain: "
                           "this method is only valid "
                           "small strain behaviour"));
  }
  t.setDrivingVariablesInitialValues(v);
}  // end of MTest_setStrain

static void MTest_setDeformationGradient(mtest::MTest& t,
                                         const std::vector<mtest::real>& v) {
  using namespace tfel::material;
  if (!((t.getBehaviourType() ==
         MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR) &&
        (t.getBehaviourKinematic() ==
         MechanicalBehaviourBase::FINITESTRAINKINEMATIC_F_CAUCHY))) {
    throw(
        std::runtime_error("MTest::setDeformationGradient: "
                           "this method is only valid "
                           "finite strain behaviour"));
  }
  t.setDrivingVariablesInitialValues(v);
}  // end of MTest_setDeformationGradient

static void MTest_setOpeningDisplacement(mtest::MTest& t,
                                         const std::vector<mtest::real>& v) {
  using namespace tfel::material;
  if (t.getBehaviourType() != MechanicalBehaviourBase::COHESIVEZONEMODEL) {
    throw(
        std::runtime_error("MTest::setOpeningDisplacement: "
                           "this method is only valid "
                           "for cohesive zone models"));
  }
  t.setDrivingVariablesInitialValues(v);
}  // end of MTest_setOpeningDisplacement

static void MTest_setStress(mtest::MTest& t,
                            const std::vector<mtest::real>& v) {
  using namespace tfel::material;
  if ((t.getBehaviourType() !=
       MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR) &&
      (t.getBehaviourType() !=
       MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR)) {
    throw(
        std::runtime_error("MTest::setStress: this method is only valid "
                           "for small or finite strain behaviours"));
  }
  t.setThermodynamicForcesInitialValues(v);
}  // end of MTest_setStress

static void MTest_setCohesiveForce(mtest::MTest& t,
                                   const std::vector<mtest::real>& v) {
  using namespace tfel::material;
  if (t.getBehaviourType() != MechanicalBehaviourBase::COHESIVEZONEMODEL) {
    throw(
        std::runtime_error("MTest::setCohesiveForce: "
                           "this method is only valid "
                           "for cohesive zone models"));
  }
  t.setThermodynamicForcesInitialValues(v);
}  // end of MTest_setCohesiveForce

static void MTest_setStrainEpsilon(mtest::MTest& t, const mtest::real& v) {
  using namespace tfel::material;
  if (!((t.getBehaviourType() ==
         MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR) ||
        ((t.getBehaviourType() ==
          MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR) &&
         (t.getBehaviourKinematic() ==
          MechanicalBehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1)))) {
    throw(
        std::runtime_error("MTest::setStrainEpsilon: "
                           "this method is only valid "
                           "small strain behaviour"));
  }
  t.setDrivingVariableEpsilon(v);
}

static void MTest_setDeformationGradientEpsilon(mtest::MTest& t,
                                                const mtest::real& v) {
  using namespace tfel::material;
  if (!((t.getBehaviourType() ==
         MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR) &&
        (t.getBehaviourKinematic() ==
         MechanicalBehaviourBase::FINITESTRAINKINEMATIC_F_CAUCHY))) {
    throw(
        std::runtime_error("MTest::setDeformationGradientEpsilon: "
                           "this method is only valid "
                           "finite strain behaviour"));
  }
  t.setDrivingVariableEpsilon(v);
}

static void MTest_setOpeningDisplacementEpsilon(mtest::MTest& t,
                                                const mtest::real& v) {
  using namespace tfel::material;
  if (t.getBehaviourType() != MechanicalBehaviourBase::COHESIVEZONEMODEL) {
    throw(
        std::runtime_error("MTest::setOpeningDisplacementEpsilon: "
                           "this method is only valid "
                           "for cohesize zone model"));
  }
  t.setDrivingVariableEpsilon(v);
}

static void MTest_setStressEpsilon(mtest::MTest& t, const mtest::real& v) {
  using namespace tfel::material;
  if ((t.getBehaviourType() !=
       MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR) &&
      (t.getBehaviourType() !=
       MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR)) {
    throw(
        std::runtime_error("MTest::setStressEpsilon: "
                           "this method is only valid "
                           "for small or finite strain behaviours"));
  }
  t.setThermodynamicForceEpsilon(v);
}

static void MTest_setCohesiveForceEpsilon(mtest::MTest& t,
                                          const mtest::real& v) {
  using namespace tfel::material;
  if (t.getBehaviourType() != MechanicalBehaviourBase::COHESIVEZONEMODEL) {
    throw(
        std::runtime_error("MTest::setCohesiveForceEpsilon: "
                           "this method is only valid "
                           "cohesive zone model"));
  }
  t.setThermodynamicForceEpsilon(v);
}

static void MTest_setImposedThermodynamicForce(mtest::MTest& t,
                                               const std::string& n,
                                               const mtest::real& v) {
  using namespace mtest;
  auto sev = make_evolution(v);
  auto sc =
      std::make_shared<ImposedThermodynamicForce>(*(t.getBehaviour()), n, sev);
  t.addEvolution(n, sev, false, true);
  t.addConstraint(sc);
}  // end of MTest_setImposedThermodynamicForce

static void MTest_setImposedThermodynamicForce2(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v) {
  using namespace mtest;
  auto sev = make_evolution(v);
  auto sc =
      std::make_shared<ImposedThermodynamicForce>(*(t.getBehaviour()), n, sev);
  t.addEvolution(n, sev, false, true);
  t.addConstraint(sc);
}  // end of MTest_setImposedThermodynamicForce

static void MTest_setImposedStress(mtest::MTest& t,
                                   const std::string& n,
                                   const mtest::real& v) {
  using namespace tfel::material;
  if ((t.getBehaviourType() !=
       MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR) &&
      (t.getBehaviourType() !=
       MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR)) {
    throw(
        std::runtime_error("MTest::handleImposedStress : "
                           "the setImposedStress method is only valid "
                           "for small and finite strain behaviours"));
  }
  MTest_setImposedThermodynamicForce(t, n, v);
}

static void MTest_setImposedStress2(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v) {
  using namespace tfel::material;
  if ((t.getBehaviourType() !=
       MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR) &&
      (t.getBehaviourType() !=
       MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR)) {
    throw(
        std::runtime_error("MTestParser::handleImposedStress : "
                           "the setImposedStress method is only valid "
                           "for small and finite strain behaviours"));
  }
  MTest_setImposedThermodynamicForce2(t, n, v);
}

static void MTest_setImposedCohesiveForce(mtest::MTest& t,
                                          const std::string& n,
                                          const mtest::real& v) {
  using namespace tfel::material;
  if (t.getBehaviourType() != MechanicalBehaviourBase::COHESIVEZONEMODEL) {
    throw(
        std::runtime_error("MTestParser::handleImposedCohesiveForce : "
                           "the setImposedCohesiveForce method is only valid "
                           "for small strain behaviours"));
  }
  MTest_setImposedThermodynamicForce(t, n, v);
}

static void MTest_setImposedCohesiveForce2(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v) {
  using namespace tfel::material;
  if (t.getBehaviourType() != MechanicalBehaviourBase::COHESIVEZONEMODEL) {
    throw(
        std::runtime_error("MTestParser::handleImposedCohesiveForce: "
                           "the setImposedCohesiveForce method is only valid "
                           "for small strain behaviours"));
  }
  MTest_setImposedThermodynamicForce2(t, n, v);
}

static void MTest_setImposedDrivingVariable(mtest::MTest& t,
                                            const std::string& n,
                                            const mtest::real& v) {
  using namespace mtest;
  auto sev = make_evolution(v);
  auto sc =
      std::make_shared<ImposedDrivingVariable>(*(t.getBehaviour()), n, sev);
  t.addEvolution(n, sev, false, true);
  t.addConstraint(sc);
}  // end of MTest_setImposedDrivingVariable

static void MTest_setImposedDrivingVariable2(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v) {
  using namespace mtest;
  auto sev = make_evolution(v);
  auto sc =
      std::make_shared<ImposedDrivingVariable>(*(t.getBehaviour()), n, sev);
  t.addEvolution(n, sev, false, true);
  t.addConstraint(sc);
}  // end of MTest_setImposedDrivingVariable

static void MTest_setImposedStrain(mtest::MTest& t,
                                   const std::string& n,
                                   const mtest::real& v) {
  using namespace tfel::material;
  if (!((t.getBehaviourType() ==
         MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR) ||
        ((t.getBehaviourType() ==
          MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR) &&
         (t.getBehaviourKinematic() ==
          MechanicalBehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1)))) {
    throw(
        std::runtime_error("MTest::setImposedStrain: "
                           "the setImposedStrain method is only valid "
                           "for small strain behaviours"));
  }
  MTest_setImposedDrivingVariable(t, n, v);
}

static void MTest_setImposedStrain2(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v) {
  using namespace tfel::material;
  if (!((t.getBehaviourType() ==
         MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR) ||
        ((t.getBehaviourType() ==
          MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR) &&
         (t.getBehaviourKinematic() ==
          MechanicalBehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1)))) {
    throw(
        std::runtime_error("MTest::handleImposedStrain: "
                           "the setImposedStrain method is only valid "
                           "for small strain behaviours"));
  }
  MTest_setImposedDrivingVariable2(t, n, v);
}

static void MTest_setImposedDeformationGradient(mtest::MTest& t,
                                                const std::string& n,
                                                const mtest::real& v) {
  using namespace tfel::material;
  if (!((t.getBehaviourType() ==
         MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR) &&
        (t.getBehaviourKinematic() ==
         MechanicalBehaviourBase::FINITESTRAINKINEMATIC_F_CAUCHY))) {
    throw(std::runtime_error(
        "MTest::setImposedDeformationGradient : "
        "the setImposedDeformationGradient method is only valid "
        "for finite strain behaviours"));
  }
  MTest_setImposedDrivingVariable(t, n, v);
}

static void MTest_setImposedDeformationGradient2(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v) {
  using namespace tfel::material;
  if (!((t.getBehaviourType() ==
         MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR) &&
        (t.getBehaviourKinematic() ==
         MechanicalBehaviourBase::FINITESTRAINKINEMATIC_F_CAUCHY))) {
    throw(std::runtime_error(
        "MTestParser::setImposedDeformationGradient: "
        "the setImposedDeformationGradient method is only valid "
        "for finite strain behaviours"));
  }
  MTest_setImposedDrivingVariable2(t, n, v);
}

static void MTest_setImposedOpeningDisplacement(mtest::MTest& t,
                                                const std::string& n,
                                                const mtest::real& v) {
  using namespace tfel::material;
  if (t.getBehaviourType() != MechanicalBehaviourBase::COHESIVEZONEMODEL) {
    throw(std::runtime_error(
        "MTest::handleImposedOpeningDisplacement : "
        "the setImposedOpeningDisplacement method is only valid "
        "for small strain behaviours"));
  }
  MTest_setImposedDrivingVariable(t, n, v);
}

static void MTest_setImposedOpeningDisplacement2(
    mtest::MTest& t,
    const std::string& n,
    const std::map<mtest::real, mtest::real>& v) {
  using namespace tfel::material;
  if (t.getBehaviourType() != MechanicalBehaviourBase::COHESIVEZONEMODEL) {
    throw(std::runtime_error(
        "MTestParser::setImposedOpeningDisplacement : "
        "the setImposedOpeningDisplacement method is only valid "
        "for small strain behaviours"));
  }
  MTest_setImposedDrivingVariable2(t, n, v);
}

static mtest::MTestCurrentState MTestCurrentState_copy(
    const mtest::MTestCurrentState& src) {
  return src;
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
    if (sc.istates.size() != 1) {                                  \
      throw(                                                       \
          std::runtime_error("MTestCurrentState::get: "            \
                             "uninitialized state"));              \
    }                                                              \
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
  if (sc.istates.size() != 1) {
    throw(
        std::runtime_error("MTestCurrentState::get: "
                           "uninitialized state"));
  }
  return sc.istates[0].Tref;
}

static void setInternalStateVariableValue3(mtest::MTestCurrentState& t,
                                           const std::string& n,
                                           const mtest::real v,
                                           const int d) {
  auto& sc = t.getStructureCurrentState("");
  if (sc.istates.size() != 1) {
    throw(
        std::runtime_error("MTestCurrentState::get: "
                           "uninitialized state"));
  }
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
  if (sc.istates.size() != 1) {
    throw(
        std::runtime_error("MTestCurrentState::get: "
                           "uninitialized state"));
  }
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

static boost::python::object getInternalStateVariableValue1(
    const mtest::MTestCurrentState& t, const std::string& n, const int i) {
  auto throw_if = [](const bool b, const std::string& m) {
    if (b) {
      throw(std::runtime_error("mtest::getInternalStateVariableValue: " + m));
    }
  };
  const auto& sc = t.getStructureCurrentState("");
  if (sc.istates.size() != 1) {
    throw(
        std::runtime_error("MTestCurrentState::get: "
                           "uninitialized state"));
  }
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
    return boost::python::object(iv[pos]);
  }
  return boost::python::object(
      std::vector<mtest::real>(iv.begin() + pos, iv.begin() + pos + size));
}

static boost::python::object getInternalStateVariableValue2(
    const mtest::MTestCurrentState& s, const std::string& n) {
  return getInternalStateVariableValue1(s, n, 1);
}  // end of getInternalStateVariableValue

void declareMTest();

void declareMTest() {
  using namespace boost;
  using namespace boost::python;
  using namespace mtest;
  using boost::python::arg;
  using tfel::tests::TestResult;

  class_<MTestCurrentState, bases<StudyCurrentState>>("MTestCurrentState")
      .def("copy", &MTestCurrentState_copy)
      .add_property("u_1", MTestCurrentState_getu_1)
      .add_property("u0", MTestCurrentState_getu0)
      .add_property("u1", MTestCurrentState_getu1)
      .add_property("s_1", MTestCurrentState_gets_1)
      .add_property("s0", MTestCurrentState_gets0)
      .add_property("s1", MTestCurrentState_gets1)
      .add_property("e0", MTestCurrentState_gete0)
      .add_property("e1", MTestCurrentState_gete1)
      .add_property("e_th0", MTestCurrentState_gete_th0)
      .add_property("e_th1", MTestCurrentState_gete_th1)
      .add_property("mprops1", MTestCurrentState_getmprops1)
      .add_property("iv_1", MTestCurrentState_getiv_1)
      .add_property("iv0", MTestCurrentState_getiv0)
      .add_property("iv1", MTestCurrentState_getiv1)
      .add_property("evs0", MTestCurrentState_getesv0)
      .add_property("desv", MTestCurrentState_getdesv)
      .add_property("dt_1", MTestCurrentState_getdt_1)
      .add_property("Tref", MTestCurrentState_getTref)
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

  TestResult (MTest::*pm)() = &MTest::execute;
  void (MTest::*pm2)(StudyCurrentState&, SolverWorkSpace&, const real,
                     const real) const = &MTest::execute;

  class_<MTest, noncopyable, bases<SingleStructureScheme>>("MTest")
      .def("execute", pm)
      .def("execute", pm2)
      .def("completeInitialisation", &MTest::completeInitialisation)
      .def("initializeCurrentState", &MTest::initializeCurrentState)
      .def("initializeWorkSpace", &MTest::initializeWorkSpace)
      .def("setEvolutionValue", &MTest::setEvolutionValue)
      .def("setDrivingVariablesInitialValues",
           &MTest::setDrivingVariablesInitialValues)
      .def("setStrain", MTest_setStrain)
      .def("setDeformationGradient", MTest_setDeformationGradient)
      .def("setOpeningDisplacement", MTest_setOpeningDisplacement)
      .def("setThermodynamicForcesInitialValues",
           &MTest::setThermodynamicForcesInitialValues)
      .def("setCohesiveForce", MTest_setCohesiveForce)
      .def("setStress", MTest_setStress)
      .def("setDrivingVariableEpsilon", &MTest::setDrivingVariableEpsilon)
      .def("setStrainEpsilon", MTest_setStrainEpsilon)
      .def("setDeformationGradientEpsilon", MTest_setDeformationGradientEpsilon)
      .def("setOpeningDisplacementEpsilon", MTest_setOpeningDisplacementEpsilon)
      .def("setThermodynamicForceEpsilon", &MTest::setThermodynamicForceEpsilon)
      .def("setStressEpsilon", MTest_setStressEpsilon)
      .def("setCohesiveForceEpsilon", MTest_setCohesiveForceEpsilon)
      .def(
          "setImposedStress", MTest_setImposedStress, (arg("name"), "values"),
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
          "setImposedStress", MTest_setImposedStress2, (arg("name"), "values"),
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
      .def("setImposedCohesiveForce", MTest_setImposedCohesiveForce,
           (arg("name"), "values"))
      .def("setImposedCohesiveForce", MTest_setImposedCohesiveForce2,
           (arg("name"), "values"))
      .def("setImposedThermodynamicForce", MTest_setImposedThermodynamicForce,
           (arg("name"), "values"))
      .def("setImposedThermodynamicForce", MTest_setImposedThermodynamicForce2,
           (arg("name"), "values"))
      .def(
          "setImposedStrain", MTest_setImposedStrain, (arg("name"), "values"),
          "This method specify the constant evolution of a strains component.\n"
          "* The first parameter (string) is the name of the choosen "
          "strains component. The allowed components (see the "
          "'setModellingHypothesis' method) are:\n"
          "- AxisymmetricalGeneralisedPlaneStrain : ERR EZZ ETT\n"
          "- Axisymmetrical                       : ERR EZZ ETT ERZ\n"
          "- PlaneStress                          : EXX EYY EZZ EXY\n"
          "- PlaneStrain                          : EXX EYY     EXY\n"
          "- GeneralisedPlaneStrain               : EXX EYY EZZ EXY\n"
          "- Tridimensional                       : EXX EYY EZZ EXY EXZ EYZ\n"
          "* The second parameter (double) is the constant value "
          "of the selected strains component.")
      .def("setImposedStrain", MTest_setImposedStrain2, (arg("name"), "values"),
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
      .def("setImposedDeformationGradient", MTest_setImposedDeformationGradient,
           (arg("name"), "values"),
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
           MTest_setImposedDeformationGradient2, (arg("name"), "values"),
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
      .def("setImposedOpeningDisplacement", MTest_setImposedOpeningDisplacement,
           (arg("name"), "values"))
      .def("setImposedOpeningDisplacement",
           MTest_setImposedOpeningDisplacement2, (arg("name"), "values"))
      .def("setImposedDrivingVariable", MTest_setImposedDrivingVariable,
           (arg("name"), "values"))
      .def("setImposedDrivingVariable", MTest_setImposedDrivingVariable2,
           (arg("name"), "values"))
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
           &MTest::setTensorInternalStateVariableInitialValues);

}  // end of declareExternalLibraryManager
