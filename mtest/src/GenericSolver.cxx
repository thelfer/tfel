/*!
 * \file   GenericSolver.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08 nov. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iterator>
#include <ostream>
#include <fstream>
#include <sstream>

#include "TFEL/Raise.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/Types.hxx"
#include "MTest/RoundingMode.hxx"
#include "MTest/SolverOptions.hxx"
#include "MTest/AccelerationAlgorithm.hxx"
#include "MTest/SolverWorkSpace.hxx"
#include "MTest/StudyCurrentState.hxx"
#include "MTest/Study.hxx"
#include "MTest/GenericSolver.hxx"

namespace mtest {

  static void report(const std::vector<std::string>& failed_criteria) {
    auto& log = mfront::getLogStream();
    log << "No convergence, the following criteria were not met:\n";
    for (const auto& fc : failed_criteria) {
      log << "- " << fc << '\n';
    }
    log << '\n';
  }

  static std::pair<bool, real> iterate2(StudyCurrentState& scs,
                                        SolverWorkSpace& wk,
                                        const Study& s,
                                        const SolverOptions& o,
                                        const real t,
                                        const real dt) {
    using namespace tfel::math;
    using namespace tfel::material;
    // compute material properties and state variables
    s.prepare(scs, t, dt);
    // packaging step
    if (scs.period == 1) {
      if (!s.doPackagingStep(scs)) {
        if (mfront::getVerboseMode() > mfront::VERBOSE_QUIET) {
          auto& log = mfront::getLogStream();
          log << "GenericSolver::execute: "
              << "behaviour compute prediction matrix failed\n";
        }
      }
    }
    const auto r =
        s.computeStiffnessMatrixAndResidual(scs, wk.K, wk.r, t, dt, o.ktype);
    if (!r.first) {
      return r;
    }
    if (!s.postConvergence(scs, t, dt, scs.period)) {
      return {false, 0.1};
    }
    return r;
  }  // end of iterate2

  static std::pair<bool, real> iterate(StudyCurrentState& scs,
                                       SolverWorkSpace& wk,
                                       const Study& s,
                                       const SolverOptions& o,
                                       const real t,
                                       const real dt) {
    using namespace tfel::math;
    using namespace tfel::material;
    using size_type = matrix<real>::size_type;
    auto& u1 = scs.u1;
    // compute material properties and state variables
    s.prepare(scs, t, dt);
    // initializing the acceleration algorithm
    if (o.aa != nullptr) {
      o.aa->preExecuteTasks();
    }
    // packaging step
    if (scs.period == 1) {
      if (!s.doPackagingStep(scs)) {
        if (mfront::getVerboseMode() > mfront::VERBOSE_QUIET) {
          auto& log = mfront::getLogStream();
          log << "GenericSolver::execute: "
              << "behaviour compute prediction matrix failed\n";
        }
      }
    }
    // prediction phase
    if (o.ppolicy == PredictionPolicy::LINEARPREDICTION) {
      s.makeLinearPrediction(scs, dt);
    } else if ((o.ppolicy == PredictionPolicy::ELASTICPREDICTION) ||
               (o.ppolicy ==
                PredictionPolicy::ELASTICPREDICTIONFROMMATERIALPROPERTIES) ||
               (o.ppolicy == PredictionPolicy::SECANTOPERATORPREDICTION) ||
               (o.ppolicy == PredictionPolicy::TANGENTOPERATORPREDICTION)) {
      std::fill(wk.K.begin(), wk.K.end(), 0.);
      auto smt = StiffnessMatrixType::ELASTIC;
      if (o.ppolicy ==
          PredictionPolicy::ELASTICPREDICTIONFROMMATERIALPROPERTIES) {
        smt = StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES;
      } else if (o.ppolicy == PredictionPolicy::SECANTOPERATORPREDICTION) {
        smt = StiffnessMatrixType::SECANTOPERATOR;
      } else if (o.ppolicy == PredictionPolicy::TANGENTOPERATORPREDICTION) {
        smt = StiffnessMatrixType::TANGENTOPERATOR;
      }
      if (s.computePredictionStiffnessAndResidual(scs, wk.K, wk.r, t, dt, smt)
              .first) {
        wk.du = wk.r;
        setRoundingMode();
        LUSolve::exe(wk.K, wk.du, wk.x, wk.p_lu);
        setRoundingMode();
        u1 -= wk.du;
      } else {
        if (mfront::getVerboseMode() > mfront::VERBOSE_QUIET) {
          auto& log = mfront::getLogStream();
          log << "GenericSolver::execute: "
              << "behaviour compute prediction matrix failed\n";
        }
      }
    } else {
      tfel::raise_if(o.ppolicy != PredictionPolicy::NOPREDICTION,
                     "GenericSolver::execute: internal error, "
                     "unsupported prediction policy");
    }
    /* resolution */
    auto converged = false;
    unsigned int iter = 0;
    real ne = 0.;   // norm of the increment of the driving variables
    real nep = 0.;  // norm of the increment of the driving variables at the
    // previous iteration
    real nep2 = 0.;  // norm of the increment of the driving variables two
    // iterations before...
    /* prediction */
    auto r_dt = real{};
    while ((!converged) && (iter != o.iterMax)) {
      ++(scs.iterations);
      ++iter;
      nep2 = nep;
      nep = ne;
      auto r =
          s.computeStiffnessMatrixAndResidual(scs, wk.K, wk.r, t, dt, o.ktype);
      if (!r.first) {
        return r;
      }
      r_dt = r.second;
      if ((mfront::getVerboseMode() >= mfront::VERBOSE_DEBUG) &&
          (o.ktype != StiffnessMatrixType::NOSTIFFNESS)) {
        auto& log = mfront::getLogStream();
        log << "Stiffness matrix:\n";
        for (size_type i = 0; i != wk.K.getNbRows(); ++i) {
          for (size_type j = 0; j != wk.K.getNbCols(); ++j) {
            log << wk.K(i, j) << " ";
          }
          log << '\n';
        }
        log << '\n';
      }
      if (mfront::getVerboseMode() >= mfront::VERBOSE_DEBUG) {
        auto& log = mfront::getLogStream();
        log << "residual :\n";
        for (size_type i = 0; i != wk.r.size(); ++i) {
          log << wk.r(i) << " ";
        }
        log << '\n';
      }
      wk.du = wk.r;
      setRoundingMode();
      LUSolve::exe(wk.K, wk.du, wk.x, wk.p_lu);
      setRoundingMode();
      u1 -= wk.du;
      converged =
          (o.ppolicy == PredictionPolicy::NOPREDICTION) ? (iter > 1) : true;
      converged =
          s.checkConvergence(scs, wk.du, wk.r, o, iter, t, dt) && converged;
      ne = s.getErrorNorm(wk.du);
      if (!converged) {
        if (iter == o.iterMax) {
          if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
            report(s.getFailedCriteriaDiagnostic(scs, wk.du, wk.r, o, t, dt));
          }
          return {false, r.second};
        } else {
          if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL3) {
            report(s.getFailedCriteriaDiagnostic(scs, wk.du, wk.r, o, t, dt));
          }
          s.computeLoadingCorrection(scs, wk, o, t, dt);
          // call the acceleration algorithm
          if (o.aa != nullptr) {
            o.aa->execute(u1, wk.du, wk.r, o.eeps, o.seps, iter);
            if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL2) {
              wk.du = u1 - scs.u10;
              const auto nit = s.getErrorNorm(wk.du);
              auto& log = mfront::getLogStream();
              log << "accelerated-sequence-convergence " << iter << " " << nit
                  << '\n';
            }
          }
          scs.u10 = u1;
        }
      }
    }
    if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
      if (iter == 1u) {
        auto& log = mfront::getLogStream();
        log << "convergence, after one iteration \n\n";
      } else {
        auto& log = mfront::getLogStream();
        if ((iter >= 3) && (ne > 100 * std::numeric_limits<real>::min()) &&
            (nep > 100 * std::numeric_limits<real>::min()) &&
            (nep2 > 100 * std::numeric_limits<real>::min())) {
          if (std::abs(std::log(nep / nep2)) >
              100 * std::numeric_limits<real>::min()) {
            log << "convergence, after " << iter << " iterations, "
                << "order " << std::log(ne / nep) / std::log(nep / nep2)
                << "\n\n";
          } else {
            log << "convergence, after " << iter << " iterations, "
                << "order undefined\n\n";
          }
        } else {
          log << "convergence, after " << iter << " iterations, "
              << "order undefined\n\n";
        }
      }
    }
    if (o.aa != nullptr) {
      o.aa->postExecuteTasks();
    }
    if (!s.postConvergence(scs, t, dt, scs.period)) {
      return {false, 0.1};
    }
    return {true, r_dt};
  }  // end of iterate

  void GenericSolver::execute(StudyCurrentState& scs,
                              SolverWorkSpace& wk,
                              const Study& s,
                              const SolverOptions& o,
                              const real ti,
                              const real te) {
    unsigned short subStep = 0;
    auto t_eps = (te - ti) * 100 * std::numeric_limits<real>::epsilon();
    auto t = ti;
    auto dt = te - ti;
    tfel::raise_if(dt < 0,
                   "GenericSolver::execute: "
                   "negative time step");
    // almost one
    constexpr real aone = 1 - 10 * std::numeric_limits<real>::epsilon();
    bool end = false;
    if (o.dynamic_time_step_scaling) {
      if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
        auto& log = mfront::getLogStream();
        log << "Dynamic time step scaling: \n"
            << "- minimal time step: " << o.minimal_time_step << "\n"
            << "- maximal time step: " << o.maximal_time_step << "\n"
            << "- minimal time step scaling factor: "
            << o.minimal_time_step_scaling_factor << "\n";
        if (o.maximal_time_step > 0) {
          log << "- maximal time step scaling factor: "
              << o.maximal_time_step_scaling_factor << "\n";
        }
      }
    }

    while ((!end) && (subStep != o.mSubSteps)) {
      const auto r = [&]() {
        if (scs.u1.empty()) {
          return iterate2(scs, wk, s, o, t, dt);
        }
        return iterate(scs, wk, s, o, t, dt);
      }();
      const auto converged = o.dynamic_time_step_scaling
                                 ? (r.first && (r.second >= aone))
                                 : r.first;
      if (converged) {
        scs.update(dt);
        t += dt;
        end = (std::abs(te - t) < t_eps) || (te < t);
        if (o.dynamic_time_step_scaling) {
          const auto rdt = std::max(
              std::min(o.maximal_time_step_scaling_factor, r.second), 1.);
          if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
            auto& log = mfront::getLogStream();
            log << "Increasing time step by a factor: " << rdt
                << " (time step scaling factor proposed by the behaviour "
                << r.second << ")\n\n";
          }
          dt *= rdt;
        }
        ++scs.period;
        if (!end) {
          s.printOutput(t, scs, false);
        }
      } else {
        ++(scs.subSteps);
        ++subStep;
        tfel::raise_if(subStep == o.mSubSteps,
                       "GenericSolver::execute: "
                       "maximum number of sub stepping reached");
        scs.revert();
        if (o.dynamic_time_step_scaling) {
          real rdt;
          if (r.first) {
            rdt = std::max(r.second, o.minimal_time_step_scaling_factor);
          } else {
            rdt = std::max(std::min(0.5, r.second),
                           o.minimal_time_step_scaling_factor);
          }
          if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
            auto& log = mfront::getLogStream();
            log << "Reducing time step by a factor: " << rdt
                << " (time step scaling factor proposed by the behaviour "
                << r.second << ")\n\n";
          }
          dt *= rdt;
        } else {
          if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
            auto& log = mfront::getLogStream();
            log << "Dividing time step by two\n\n";
          }
          dt *= 0.5;
        }
      }
      if (!end) {
        if (o.dynamic_time_step_scaling) {
          if (o.maximal_time_step > 0) {
            dt = std::min(dt, o.maximal_time_step);
          }
          if (dt > te - t - o.minimal_time_step) {
            dt = te - t;
          }
        }
        tfel::raise_if(dt < 0,
                       "GenericSolver::execute: "
                       "negative time step");
        tfel::raise_if(dt < o.minimal_time_step,
                       "GenericSolver::execute: "
                       "time step is below its minimal value");
      }
    }
  }

  GenericSolver::~GenericSolver() = default;

}  // end of namespace mtest
