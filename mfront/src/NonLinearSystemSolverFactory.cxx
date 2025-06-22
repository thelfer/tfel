/*!
 * \file  mfront/src/NonLinearSystemSolverFactory.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 10 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/NewtonRaphsonSolvers.hxx"
#include "MFront/BroydenSolvers.hxx"
#include "MFront/SecondBroydenSolver.hxx"
#include "MFront/LevenbergMarquardtSolvers.hxx"
#include "MFront/NonLinearSystemSolverFactory.hxx"

namespace mfront {

  template <typename T>
  static std::shared_ptr<NonLinearSystemSolver> buildSolverConstructor() {
    return std::shared_ptr<NonLinearSystemSolver>(new T());
  }  // end of buildAlgoritmConstructor

  NonLinearSystemSolverFactory&
  NonLinearSystemSolverFactory::getNonLinearSystemSolverFactory() {
    static NonLinearSystemSolverFactory factory;
    return factory;
  }

  std::shared_ptr<NonLinearSystemSolver>
  NonLinearSystemSolverFactory::getSolver(const std::string& a) const {
    const auto p = this->constructors.find(a);
    tfel::raise_if(p == this->constructors.end(),
                   "NonLinearSystemSolverFactory::getSolver : "
                   "no solver '" +
                       a + "' registred");
    return (*(p->second))();
  }

  NonLinearSystemSolverFactory::NonLinearSystemSolverFactory() {
    this->registerSolver("NewtonRaphson",
                         buildSolverConstructor<MFrontNewtonRaphsonSolver>);
    this->registerSolver(
        "NewtonRaphson_NumericalJacobian",
        buildSolverConstructor<MFrontNewtonRaphsonNumericalJacobianSolver>);
    this->registerSolver(
        "PowellDogLeg_NewtonRaphson",
        buildSolverConstructor<MFrontPowellDogLegNewtonRaphsonSolver>);
    this->registerSolver(
        "PowellDogLeg_NewtonRaphson_NumericalJacobian",
        buildSolverConstructor<
            MFrontPowellDogLegNewtonRaphsonNumericalJacobianSolver>);
    this->registerSolver("Broyden",
                         buildSolverConstructor<MFrontBroydenSolver>);
    this->registerSolver(
        "PowellDogLeg_Broyden",
        buildSolverConstructor<MFrontPowellDogLegBroydenSolver>);
    this->registerSolver("Broyden2",
                         buildSolverConstructor<SecondBroydenSolver>);
    this->registerSolver("LevenbergMarquardt",
                         buildSolverConstructor<LevenbergMarquardtSolver>);
    this->registerSolver(
        "LevenbergMarquardt_NumericalJacobian",
        buildSolverConstructor<LevenbergMarquardtNumericalJacobianSolver>);
  }  // end of NonLinearSystemSolverFactory::NonLinearSystemSolverFactory

  void NonLinearSystemSolverFactory::registerSolver(const std::string& a,
                                                    const constructor c) {
    tfel::raise_if(!this->constructors.insert({a, c}).second,
                   "NonLinearSystemSolverFactory::registerSolver : "
                   "solver '" +
                       a + "' already declared");
  }  // end of NonLinearSystemSolverFactory::registerSolver

  std::vector<std::string> NonLinearSystemSolverFactory::getRegistredSolvers()
      const {
    std::vector<std::string> solvers;
    for (const auto& s : this->constructors) {
      solvers.push_back(s.first);
    }
    return solvers;
  }  // end of NonLinearSystemSolverFactory::getRegistredSolvers

}  // end of namespace mfront
