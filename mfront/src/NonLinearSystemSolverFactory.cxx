/*! 
 * \file  mfront/src/NonLinearSystemSolverFactory.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juin 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<stdexcept>

#include"MFront/NonLinearSystemSolver.hxx"
#include"MFront/NewtonRaphsonSolvers.hxx"
#include"MFront/BroydenSolvers.hxx"
#include"MFront/SecondBroydenSolver.hxx"
#include"MFront/LevenbergMarquardtSolvers.hxx"
#include"MFront/NonLinearSystemSolverFactory.hxx"

namespace mfront
{

  template<typename T>
  static tfel::utilities::shared_ptr<NonLinearSystemSolver>
  buildSolverConstructor(void)
  {
    return tfel::utilities::shared_ptr<NonLinearSystemSolver>(new T());
  } // end of buildAlgoritmConstructor

  NonLinearSystemSolverFactory&
  NonLinearSystemSolverFactory::getNonLinearSystemSolverFactory()
  {
    static NonLinearSystemSolverFactory factory;
    return factory;
  }
  
  tfel::utilities::shared_ptr<NonLinearSystemSolver>
  NonLinearSystemSolverFactory::getSolver(const std::string& a) const
  {
    using namespace std;
    map<string,constructor>::const_iterator p;
    p = this->constructors.find(a);
    if(p==this->constructors.end()){
      string msg("NonLinearSystemSolverFactory::getSolver : "
		 "no solver '"+a+"' registred");
      throw(runtime_error(msg));
    }
    return (*(p->second))();
  }

  NonLinearSystemSolverFactory::NonLinearSystemSolverFactory()
  {
    this->registerSolver("NewtonRaphson",
			 buildSolverConstructor<MFrontNewtonRaphsonSolver>);
    this->registerSolver("NewtonRaphson_NumericalJacobian",
			 buildSolverConstructor<MFrontNewtonRaphsonNumericalJacobianSolver>);
    this->registerSolver("PowellDogLeg_NewtonRaphson",
			 buildSolverConstructor<MFrontPowellDogLegNewtonRaphsonSolver>);
    this->registerSolver("PowellDogLeg_NewtonRaphson_NumericalJacobian",
			 buildSolverConstructor<MFrontPowellDogLegNewtonRaphsonNumericalJacobianSolver>);
    this->registerSolver("Broyden",
			 buildSolverConstructor<MFrontBroydenSolver>);
    this->registerSolver("PowellDogLeg_Broyden",
			 buildSolverConstructor<MFrontPowellDogLegBroydenSolver>);
    this->registerSolver("Broyden2",
			 buildSolverConstructor<SecondBroydenSolver>);
    this->registerSolver("LevenbergMarquardt",
			 buildSolverConstructor<MFrontLevenbergMarquartSolver>);
    this->registerSolver("LevenbergMarquardt_NumericalJacobian",
			 buildSolverConstructor<MFrontLevenbergMarquartNumericalJacobianSolver>);
  } // end of NonLinearSystemSolverFactory::NonLinearSystemSolverFactory

  void
  NonLinearSystemSolverFactory::registerSolver(const std::string& a,
						       const constructor c)
  {
    using namespace std;
    typedef map<string,constructor>::value_type value_type;
    if(!this->constructors.insert(value_type(a,c)).second){
      string msg("NonLinearSystemSolverFactory::registerSolver : "
		 "solver '"+a+"' already declared");
      throw(runtime_error(msg));
    }
  } // end of NonLinearSystemSolverFactory::registerSolver

} // end of namespace mfront
