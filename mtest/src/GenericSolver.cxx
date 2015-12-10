/*!
 * \file   GenericSolver.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   08 nov. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<iostream>
#include<iterator>

#include<ostream>
#include<fstream>
#include<sstream>

#include"MFront/MFrontLogStream.hxx"
#include"MTest/Types.hxx"
#include"MTest/SolverOptions.hxx"
#include"MTest/AccelerationAlgorithm.hxx"
#include"MTest/SolverWorkSpace.hxx"
#include"MTest/StudyCurrentState.hxx"
#include"MTest/Study.hxx"
#include"MTest/GenericSolver.hxx"

namespace mtest{

  static void
  report(const std::vector<std::string>& failed_criteria)
  {
    auto& log = mfront::getLogStream();
    log << "No convergence, the following criteria were not met:\n";
    for(const auto& fc : failed_criteria){
      log << "- " << fc << '\n';
    }
    log << '\n';
  }
  
  static bool
  iterate(StudyCurrentState& scs,
	  SolverWorkSpace& wk,
	  const Study& s,
	  const SolverOptions& o,
	  const real t,
	  const real dt)
  {
    using namespace std;
    using namespace tfel::math;
    using namespace tfel::material;
    using size_type = matrix<real>::size_type;
    const auto& u0 = scs.u0;
    auto& u1 = scs.u1;
    // compute material properties and state variables
    s.prepare(scs,t,dt);
    // initializing the acceleration algorithm
    if(o.aa.get()!=nullptr){
      o.aa->preExecuteTasks();
    }
    // prediction phase
    if(o.ppolicy==PredictionPolicy::LINEARPREDICTION){
      s.makeLinearPrediction(scs,dt);
    } else if((o.ppolicy==PredictionPolicy::ELASTICPREDICTION)||
	      (o.ppolicy==PredictionPolicy::ELASTICPREDICTIONFROMMATERIALPROPERTIES)||
	      (o.ppolicy==PredictionPolicy::SECANTOPERATORPREDICTION)||
	      (o.ppolicy==PredictionPolicy::TANGENTOPERATORPREDICTION)){
      std::fill(wk.K.begin(),wk.K.end(),0.);
      auto smt = StiffnessMatrixType::ELASTIC;
      if(o.ppolicy==PredictionPolicy::ELASTICPREDICTIONFROMMATERIALPROPERTIES){
	smt=StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES;
      } else if (o.ppolicy==PredictionPolicy::SECANTOPERATORPREDICTION){
	smt=StiffnessMatrixType::SECANTOPERATOR;
      } else if (o.ppolicy==PredictionPolicy::TANGENTOPERATORPREDICTION){
	smt=StiffnessMatrixType::TANGENTOPERATOR;
      }
      if(s.computePredictionStiffnessAndResidual(scs,wk.K,wk.r,t,dt,smt)){
	wk.du = wk.r;
	LUSolve::exe(wk.K,wk.du,wk.x,wk.p_lu);
	u1 -= wk.du;	  
      } else {
	if(mfront::getVerboseMode()>mfront::VERBOSE_QUIET){
	  auto& log = mfront::getLogStream();
	  log << "Solver::execute : behaviour compute prediction matrix failed" << endl;
	}
      }
    } else {
      if(o.ppolicy != PredictionPolicy::NOPREDICTION){
	throw(runtime_error("Solver::execute: internal error, "
			    "unsupported prediction policy"));
      }	    
    }
    /* resolution */
    auto converged = false;
    unsigned int iter = 0;
    real ne  = 0.;  // norm of the increment of the driving variables
    real nep = 0.;  // norm of the increment of the driving variables at the
    // previous iteration
    real nep2 = 0.; // norm of the increment of the driving variables two
    // iterations before...
    /* prediction */
    while((!converged)&&(iter!=o.iterMax)){
      ++iter;
      nep2 = nep;
      nep  = ne;
      //	ktype = o.ktype;
      // if(o.ks==SolverOptions::CONSTANTSTIFFNESS){
      //   if(!wk.stiffnessInitialised){
	    
      //   }
      // }
      // if(o.ks==SolverOptions::CONSTANTSTIFFNESSBYPERIOD){
	  
      // }
      if(!s.computeStiffnessMatrixAndResidual(scs,wk.K,wk.r,t,dt,o.ktype)){
	return false;
      }
      if((mfront::getVerboseMode()>=mfront::VERBOSE_DEBUG)&&
	 (o.ktype!=StiffnessMatrixType::NOSTIFFNESS)){
	auto& log = mfront::getLogStream();
	log << "Stiffness matrix:\n";
	for(size_type i=0;i!=wk.K.getNbRows();++i){
	  for(size_type j=0;j!=wk.K.getNbCols();++j){
	    log << wk.K(i,j) << " ";
	  }
	  log << '\n';
	}
	log << endl;
      }
      // scs.revert();
      // tfel::math::matrix<real> nK(wk.K.getNbRows(),wk.K.getNbCols());
      // tfel::math::vector<real> nr(wk.K.getNbRows());
      // std::fill(nK.begin(),nK.end(),real(0));
      // std::fill(nr.begin(),nr.end(),real(0));
      // for(size_type i=0;i!=wk.K.getNbRows();++i){
      // 	constexpr const real eps = 1.e-6;
      // 	scs.revert();
      // 	scs.u1[i] += eps;
      // 	s.computeStiffnessMatrixAndResidual(scs,nK,nr,t,dt,
      // 					    StiffnessMatrixType::NOSTIFFNESS);
      // 	// std::cout << "nr : ";
      // 	for(size_type j=0;j!=wk.K.getNbCols();++j){
      // 	  // std::cout << nr[j] << " ";
      // 	  nK(j,i) = nr[j];
      // 	}
      // 	// std::cout << "\n";
      // 	scs.revert();
      // 	scs.u1[i] -= eps;
      // 	s.computeStiffnessMatrixAndResidual(scs,nK,nr,t,dt,
      // 					    StiffnessMatrixType::NOSTIFFNESS);
      // 	// std::cout << "nr : ";
      // 	for(size_type j=0;j!=wk.K.getNbCols();++j){
      // 	  // std::cout << nr[j] << " ";
      // 	  nK(j,i) -= nr[j];
      // 	  nK(j,i) /= 2*eps;
      // 	}
      // 	// std::cout << "\n";
      // }
      // if(mfront::getVerboseMode()>=mfront::VERBOSE_DEBUG){
      // 	auto& log = mfront::getLogStream();
      // 	log << "Numerical stiffness matrix:\n";
      // 	for(size_type i=0;i!=wk.K.getNbRows();++i){
      // 	  for(size_type j=0;j!=wk.K.getNbCols();++j){
      // 	    log << nK(i,j) << " ";
      // 	  }
      // 	  log << '\n';
      // 	}
      // 	log << endl;
      // }
      // for(size_type i=0;i!=wk.K.getNbRows();++i){
      // 	for(size_type j=0;j!=wk.K.getNbCols();++j){
      // 	  if(std::abs(wk.K(i,j)-nK(i,j))>1){
      // 	    std::cout << i << " " << j << " " << wk.K(i,j) << " " << nK(i,j)
      // 		      << " " << std::abs(wk.K(i,j)-nK(i,j))
      // 		      << " " << std::abs(wk.K(i,j)-nK(i,j))/std::abs(nK(i,j))*100
      // 		      << std::endl;
      // 	  }
      // 	}
      // }
      // ::exit(-1);
      wk.du = wk.r;
      LUSolve::exe(wk.K,wk.du,wk.x,wk.p_lu);
      u1 -= wk.du;
      converged = (o.ppolicy == PredictionPolicy::NOPREDICTION) ? (iter>1) : true;
      converged = converged && s.checkConvergence(scs,wk.du,wk.r,o,iter,t,dt);
      ne = s.getErrorNorm(wk.du);
      if(!converged){
	if(iter==o.iterMax){
	  if(mfront::getVerboseMode()>=mfront::VERBOSE_LEVEL1){
	    report(s.getFailedCriteriaDiagnostic(scs,wk.du,wk.r,o,t,dt));
	  }
	  return false;
	} else {
	  if(mfront::getVerboseMode()>=mfront::VERBOSE_LEVEL3){
	    report(s.getFailedCriteriaDiagnostic(scs,wk.du,wk.r,o,t,dt));
	  }
	  // call the acceleration algorithm
	  if(o.aa.get()!=nullptr){
	    o.aa->execute(u1,wk.du,wk.r,o.eeps,o.seps,iter);
	    if(mfront::getVerboseMode()>=mfront::VERBOSE_LEVEL2){
	      wk.du = u1-wk.u10;
	      const auto nit = s.getErrorNorm(wk.du);
	      auto& log = mfront::getLogStream();
	      log << "accelerated-sequence-convergence " << iter << " " << nit << '\n';
	    }
	  }
	  wk.u10 = u1;
	}
      }
    }
    if(mfront::getVerboseMode()>=mfront::VERBOSE_LEVEL1){
      if(iter==1u){
	auto& log = mfront::getLogStream();
	log << "convergence, after one iteration \n\n";
      } else {
	auto& log = mfront::getLogStream();
	if((iter>=3)&&
	   (ne  >100*numeric_limits<real>::min())&&
	   (nep >100*numeric_limits<real>::min())&&
	   (nep2>100*numeric_limits<real>::min())){
	  log << "convergence, after " << iter << " iterations, "
	      << "order " << std::log(ne/nep)/std::log(nep/nep2)
	      << "\n\n";
	} else {
	  log << "convergence, after " << iter << " iterations, "
	      << "order undefined\n\n";
	}
      }
    }
    if(o.aa.get()!=nullptr){
      o.aa->postExecuteTasks();
    }
    s.postConvergence(scs,t,dt,scs.period);
    return true;
  } // end of iterate
  
  void
  GenericSolver::execute(StudyCurrentState& scs,
			 SolverWorkSpace& wk,
			 const Study& s,
			 const SolverOptions& o,
			 const real ti,
			 const real te)
  {
    unsigned short subStep = 0;
    auto t  = ti;
    auto dt = te-ti;
    while((std::abs(te-t)>0.5*dt)&&(subStep!=o.mSubSteps)){
      if(iterate(scs,wk,s,o,t,dt)){
	scs.update(dt);
	t+=dt;
	++scs.period;
      } else {
	++subStep;
	if(subStep==o.mSubSteps){
	  throw(std::runtime_error("GenericSolver::execute: "
				   "maximum number of sub stepping reached"));
	}
	if(mfront::getVerboseMode()>=mfront::VERBOSE_LEVEL1){
	  auto& log = mfront::getLogStream();
	  log << "Dividing time step by two\n\n";
	}
	scs.revert();
	dt *= 0.5;
      }
    }
  }

  GenericSolver::~GenericSolver()
  {} // end of GenericSolver::~GenericSolver
  
} // end of namespace mtest
