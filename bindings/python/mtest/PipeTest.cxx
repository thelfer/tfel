/*!
 * \file   bindings/python/mtest/PipeTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   07 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <memory>
#include <limits>
#include <boost/python.hpp>

#include "TFEL/Raise.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/StudyCurrentState.hxx"
#include "MTest/SolverWorkSpace.hxx"
#include "MTest/PipeProfile.hxx"
#include "MTest/PipeProfileHandler.hxx"
#include "MTest/PipeTest.hxx"

static boost::python::tuple PipeTest_computeMinimumAndMaximumValues(
    const mtest::PipeTest& t,
    const mtest::StudyCurrentState& s,
    const std::string& n) {
  const auto r = t.computeMinimumAndMaximumValues(s, n);
  return boost::python::make_tuple(r.first, r.second);
}  // end of PipeTest_computeMinimumAndMaximumValues

static void PipeTest_setMandrelRadiusEvolution(mtest::PipeTest& t,
                                               const mtest::real& v) {
  t.setMandrelRadiusEvolution(mtest::make_evolution(v));
}  // end of PipeTest_setMandrelRadiusEvolution

static void PipeTest_setMandrelRadiusEvolution2(
    mtest::PipeTest& t, const std::map<mtest::real, mtest::real>& v) {
  t.setMandrelRadiusEvolution(mtest::make_evolution(v));
}  // end of PipeTest_setMandrelRadiusEvolution

static void PipeTest_setMandrelAxialGrowthEvolution(mtest::PipeTest& t,
                                                    const mtest::real& v) {
  t.setMandrelAxialGrowthEvolution(mtest::make_evolution(v));
}  // end of PipeTest_setMandrelAxialGrowthEvolution

static void PipeTest_setMandrelAxialGrowthEvolution2(
    mtest::PipeTest& t, const std::map<mtest::real, mtest::real>& v) {
  t.setMandrelAxialGrowthEvolution(mtest::make_evolution(v));
}  // end of PipeTest_setMandrelAxialGrowthEvolution

static void PipeTest_setInnerPressureEvolution(mtest::PipeTest& t,
                                               const mtest::real& v) {
  t.setInnerPressureEvolution(mtest::make_evolution(v));
}  // end of PipeTest_setInnerPressureEvolution

static void PipeTest_setInnerPressureEvolution2(
    mtest::PipeTest& t, const std::map<mtest::real, mtest::real>& v) {
  t.setInnerPressureEvolution(mtest::make_evolution(v));
}  // end of PipeTest_setInnerPressureEvolution2

static void PipeTest_setOuterPressureEvolution(mtest::PipeTest& t,
                                               const mtest::real& v) {
  t.setOuterPressureEvolution(mtest::make_evolution(v));
}  // end of PipeTest_setOuterPressureEvolution

static void PipeTest_setOuterPressureEvolution2(
    mtest::PipeTest& t, const std::map<mtest::real, mtest::real>& v) {
  t.setOuterPressureEvolution(mtest::make_evolution(v));
}  // end of PipeTest_setOuterPressureEvolution2

static void PipeTest_setAxialForceEvolution(mtest::PipeTest& t,
                                            const mtest::real& v) {
  t.setAxialForceEvolution(mtest::make_evolution(v));
}  // end of PipeTest_setAxialForceEvolution

static void PipeTest_setAxialForceEvolution2(
    mtest::PipeTest& t, const std::map<mtest::real, mtest::real>& v) {
  t.setAxialForceEvolution(mtest::make_evolution(v));
}  // end of PipeTest_setAxialForceEvolution2

static void PipeTest_setAxialGrowthEvolution(mtest::PipeTest& t,
                                             const mtest::real& v) {
  t.setAxialGrowthEvolution(mtest::make_evolution(v));
}  // end of PipeTest_setAxialGrowthEvolution

static void PipeTest_setAxialGrowthEvolution2(
    mtest::PipeTest& t, const std::map<mtest::real, mtest::real>& v) {
  t.setAxialGrowthEvolution(mtest::make_evolution(v));
}  // end of PipeTest_setAxialGrowthEvolution2

static void PipeTest_setAxialLoading(mtest::PipeTest& t, const std::string& h) {
  if (h == "None") {
    t.setAxialLoading(mtest::PipeTest::NONE);
  } else if (h == "EndCapEffect") {
    t.setAxialLoading(mtest::PipeTest::ENDCAPEFFECT);
  } else if (h == "ImposedAxialForce") {
    t.setAxialLoading(mtest::PipeTest::IMPOSEDAXIALFORCE);
  } else if (h == "ImposedAxialGrowth") {
    t.setAxialLoading(mtest::PipeTest::IMPOSEDAXIALGROWTH);
  } else {
    tfel::raise(
        "PipeTest::setAxialLoading: "
        "invalid pipe modelling hypothesis ('" +
        h +
        "').\n"
        "Valid modelling hypothesis are "
        "'None', 'ImposedAxialForce' and 'EndCapEffect'");
  }
}  // end of PipeTest_setAxialLoading

static void PipeTest_setRadialLoading(mtest::PipeTest& t,
                                      const std::string& h) {
  if (h == "ImposedPressure") {
    t.setRadialLoading(mtest::PipeTest::IMPOSEDPRESSURE);
  } else if (h == "ImposedOuterRadius") {
    t.setRadialLoading(mtest::PipeTest::IMPOSEDOUTERRADIUS);
  } else if (h == "TightPipe") {
    t.setRadialLoading(mtest::PipeTest::TIGHTPIPE);
  } else {
    tfel::raise(
        "PipeTest::setRadialLoading: "
        "invalid pipe loading type ('" +
        h +
        "').\n"
        "Valid loading type are "
        "'ImposedPressure', 'TightPipe' "
        "and 'ImposedOuterRadius'");
  }
}  // end of PipeTest_setRadialLoading

static void PipeTest_setOuterRadiusEvolution(mtest::PipeTest& t,
                                             const mtest::real& v) {
  t.setOuterRadiusEvolution(mtest::make_evolution(v));
}  // end of PipeTest_setOuterRadiusEvolution

static void PipeTest_setOuterRadiusEvolution2(
    mtest::PipeTest& t, const std::map<mtest::real, mtest::real>& v) {
  t.setOuterRadiusEvolution(mtest::make_evolution(v));
}  // end of PipeTest_setOuterRadiusEvolution2

static void PipeTest_setElementType(mtest::PipeTest& t, const std::string& e) {
  if (e == "Linear") {
    t.setElementType(mtest::PipeMesh::LINEAR);
  } else if (e == "Quadratic") {
    t.setElementType(mtest::PipeMesh::QUADRATIC);
  } else if (e == "Cubic") {
    t.setElementType(mtest::PipeMesh::CUBIC);
  } else {
    tfel::raise(
        "PipeTest::setElementType: "
        "invalid element type ('" +
        e +
        "').\n"
        "Valid element type are "
        "'Linear' and 'Quadratic'");
  }
}  // end of PipeTest_setElementType

void declarePipeTest();

void declarePipeTest() {
  using namespace mtest;
  tfel::tests::TestResult (PipeTest::*pm)() = &PipeTest::execute;
  void (PipeTest::*pm2)(StudyCurrentState&, SolverWorkSpace&, const real,
                        const real) const = &PipeTest::execute;
  boost::python::enum_<mtest::PipeTest::Configuration>("PipeTestConfiguration")
      .value("INTIAL_CONFIGURATION",
             mtest::PipeTest::Configuration::INTIAL_CONFIGURATION)
      .value("Intial_Configuration",
             mtest::PipeTest::Configuration::INTIAL_CONFIGURATION)
      .value("CURRENT_CONFIGURATION",
             mtest::PipeTest::Configuration::CURRENT_CONFIGURATION)
      .value("Current_Configuration",
             mtest::PipeTest::Configuration::CURRENT_CONFIGURATION);
  boost::python::enum_<mtest::PipeTest::FailurePolicy>("PipeTestFailurePolicy")
      .value("REPORTONLY", mtest::PipeTest::FailurePolicy::REPORTONLY)
      .value("STOPCOMPUTATION", mtest::PipeTest::FailurePolicy::STOPCOMPUTATION)
      .value("FREEZESTATEUNTILENDOFCOMPUTATION",
             mtest::PipeTest::FailurePolicy::FREEZESTATEUNTILENDOFCOMPUTATION)
      .value("ReportOnly", mtest::PipeTest::FailurePolicy::REPORTONLY)
      .value("StopComputation", mtest::PipeTest::FailurePolicy::STOPCOMPUTATION)
      .value("FREEZESTATE",
             mtest::PipeTest::FailurePolicy::FREEZESTATEUNTILENDOFCOMPUTATION)
      .value("FreezeStateUntilEndOfComputation",
             mtest::PipeTest::FailurePolicy::FREEZESTATEUNTILENDOFCOMPUTATION)
      .value("FreezeState",
             mtest::PipeTest::FailurePolicy::FREEZESTATEUNTILENDOFCOMPUTATION);
  boost::python::class_<PipeTest, boost::noncopyable,
                        boost::python::bases<SingleStructureScheme>>("PipeTest")
      .def("setInnerRadius", &PipeTest::setInnerRadius)
      .def("setOuterRadius", &PipeTest::setOuterRadius)
      .def("setNumberOfElements", &PipeTest::setNumberOfElements)
      .def("getNumberOfUnknowns", &PipeTest::getNumberOfUnknowns)
      .def("completeInitialisation", &PipeTest::completeInitialisation)
      .def("execute", pm)
      .def("execute", pm2)
      .def("initializeCurrentState", &PipeTest::initializeCurrentState)
      .def("initializeWorkSpace", &PipeTest::initializeWorkSpace)
      .def("setInnerPressureEvolution", &PipeTest_setInnerPressureEvolution)
      .def("setInnerPressureEvolution", &PipeTest_setInnerPressureEvolution2)
      .def("setOuterPressureEvolution", &PipeTest_setOuterPressureEvolution)
      .def("setOuterPressureEvolution", &PipeTest_setOuterPressureEvolution2)
      .def("setMandrelRadiusEvolution", &PipeTest_setMandrelRadiusEvolution,
           "This method specify the value of the radius of the mandrel.")
      .def("setMandrelRadiusEvolution", &PipeTest_setMandrelRadiusEvolution2,
           "This method specify the linear evolution of the radius of the "
           "mandrel.\n"
           "* The parameter (map<double, double>) specify a linear "
           "evolution: the associative array of time-value is used "
           "to perform an interpolation. That interpolation is only "
           "performed between the minimum and maximum times given "
           "in the array. Should a time be out of the array, the returned "
           "value will be the one from the nearest association available.")
      .def("setMandrelAxialGrowthEvolution",
           &PipeTest_setMandrelAxialGrowthEvolution,
           "This method specify the value of the axial growth of the mandrel.")
      .def("setMandrelAxialGrowthEvolution",
           &PipeTest_setMandrelAxialGrowthEvolution2,
           "This method specify the linear evolution of the mandra axial "
           "growth.\n"
           "* The parameter (map<double, double>) specify a linear "
           "evolution: the associative array of time-value is used "
           "to perform an interpolation. That interpolation is only "
           "performed between the minimum and maximum times given "
           "in the array. Should a time be out of the array, the returned "
           "value will be the one from the nearest association available.")
      .def("setAxialForceEvolution", &PipeTest_setAxialForceEvolution,
           "This method specify the constant evolution of the axial force.\n"
           "* The parameter (double) is the constant evolution value.")
      .def("setAxialForceEvolution", &PipeTest_setAxialForceEvolution2,
           "This method specify the linear evolution of the axial force.\n"
           "* The parameter (map<double, double>) specify a linear "
           "evolution: the associative array of time-value is used "
           "to perform an interpolation. That interpolation is only "
           "performed between the minimum and maximum times given "
           "in the array. Should a time be out of the array, the returned "
           "value will be the one from the nearest association available.")
      .def("setAxialGrowthEvolution", &PipeTest_setAxialGrowthEvolution,
           "This method specify the constant evolution of the axial growth.\n"
           "* The parameter (double) is the constant evolution value.")
      .def("setAxialGrowthEvolution", &PipeTest_setAxialGrowthEvolution2,
           "This method specify the linear evolution of the axial growth.\n"
           "* The parameter (map<double, double>) specify a linear "
           "evolution: the associative array of time-value is used "
           "to perform an interpolation. That interpolation is only "
           "performed between the minimum and maximum times given "
           "in the array. Should a time be out of the array, the returned "
           "value will be the one from the nearest association available.")
      .def("setOuterRadiusEvolution", &PipeTest_setOuterRadiusEvolution)
      .def("setOuterRadiusEvolution", &PipeTest_setOuterRadiusEvolution2)
      .def("setFillingPressure", &PipeTest::setFillingPressure)
      .def("setFillingTemperature", &PipeTest::setFillingTemperature)
      .def("performSmallStrainAnalysis", &PipeTest::performSmallStrainAnalysis)
      .def("setDisplacementEpsilon", &PipeTest::setDisplacementEpsilon)
      .def("setResidualEpsilon", &PipeTest::setResidualEpsilon)
      .def("setAxialLoading", &PipeTest_setAxialLoading,
           "This method specify the kind of loading to apply to "
           "the pipe in the axial direction.\n"
           "* The parameter (string) is the choosen kind of loading "
           "to apply. The allowed kind of loadings allowed are:\n"
           "- 'None': no load is applied to the pipe.\n"
           "- 'EndCapEffect' (default) : load applied to the pipe "
           "modelise the inner and outer pressure effects on a closed "
           "pipe. See the 'setRadialLoading' method to specify if "
           "these pressures are specified or computed.\n"
           "- 'ImposedAxialForce': the axial force acting of the pipe "
           "is specified by the user. See the 'setAxialForceEvolution' "
           "method for details.\n"
           "- 'ImposedAxialGrowth': the axial growth of the pipe is "
           "specified by the user. See the 'setAxialGrowthEvolution' "
           "method for details.")
      .def("setRadialLoading", &PipeTest_setRadialLoading,
           "This method specify the kind of loading to apply to "
           "the pipe in radial direction.\n"
           "* The parameter (string) is the choosen kind of loading "
           "to apply. The allowed kind of loadings allowed are:\n"
           "- 'ImposedPressure' (default) : the inner pressure is "
           "specified by the user. See the 'setInnerPressureEvolution' "
           "method for details.\n"
           "- 'TightPipe': the total amount of substance of the gas is"
           "constant. The gas is ideal which allows:\n"
           "  > the computation of the amount of substance using the "
           "filling pressure and temperature. See the 'setFillingPressure' "
           "and the 'setFillingTemperature' methods for details.\n"
           "  > the computation of the inner pressure as a function "
           "of the curent temperature and enclosed volume.\n"
           "- 'ImposedOuterRadius': the evolution of the outer radius "
           "is user-specified. See the 'setOuterRadiusEvolution' "
           "method for details.\n")
      .def("setElementType", &PipeTest_setElementType)
      .def("addProfile", &PipeTest::addProfile)
      .def("computeMinimumValue",
           static_cast<real (PipeTest::*)(const StudyCurrentState&,
                                          const std::string&) const>(
               &PipeTest::computeMinimumValue),
           "compute the minimum value of a scalar variable")
      .def("computeMaximumValue",
           static_cast<real (PipeTest::*)(const StudyCurrentState&,
                                          const std::string&) const>(
               &PipeTest::computeMaximumValue),
           "compute the maximum value of a scalar variable")
      .def("computeMeanValue",
           static_cast<real (PipeTest::*)(const StudyCurrentState&,
                                          const std::string&,
                                          const PipeTest::Configuration) const>(
               &PipeTest::computeMeanValue),
           "compute the mean value of a scalar variable")
      .def("computeIntegralValue",
           static_cast<real (PipeTest::*)(const StudyCurrentState&,
                                          const std::string&,
                                          const PipeTest::Configuration) const>(
               &PipeTest::computeIntegralValue),
           "compute the integral value of a scalar variable")
      .def("computeMinimumAndMaximumValues",
           PipeTest_computeMinimumAndMaximumValues)
      .def("setGasEquationOfState", &PipeTest::setGasEquationOfState)
      .def("getMesh", &PipeTest::getMesh,
           boost::python::return_value_policy<
               boost::python::copy_const_reference>(),
           "This method returns the underlying mesh")
      .def("completeInitialisation", &PipeTest::completeInitialisation,
           "complete the initialisation. This method must be called once. This "
           "method may be called automatically by the execute method.")
      .def("addFailureCriterion", &PipeTest::addFailureCriterion,
           "add a failure criterion.")
      .def("setFailurePolicy", &PipeTest::setFailurePolicy,
           "set the failure policy.")
      .def("addOxidationModel", &PipeTest::addOxidationModel,
           (boost::python::arg("library"), "model", "boundary"),
           "add an oxidation model.");
}
