/*!
 * \file  mtest/src/MTest.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 12/04/2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <map>
#include <cmath>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <cstdlib>
#include <iterator>
#include <algorithm>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/TextData.hxx"
#include "TFEL/Utilities/ArgumentParserBase.hxx"
#include "TFEL/Utilities/TerminalColors.hxx"
#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Math/matrix.hxx"
#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/RoundingMode.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/MTestParser.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/FunctionEvolution.hxx"
#include "MTest/CastemEvolution.hxx"
#include "MTest/Constraint.hxx"
#include "MTest/ImposedThermodynamicForce.hxx"
#include "MTest/ImposedGradient.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/StructureCurrentState.hxx"
#include "MTest/GenericSolver.hxx"
#include "MTest/UserDefinedPostProcessing.hxx"
#include "MTest/MTest.hxx"

#ifdef min
#undef min
#endif

namespace mtest {

  static void updateStiffnessAndResidual(tfel::math::matrix<real>& k,
                                         tfel::math::vector<real>& r,
                                         const Behaviour& b,
                                         const tfel::math::matrix<real>& kt,
                                         const tfel::math::vector<real>& s) {
    using namespace tfel::material;
    using size_type = tfel::math::matrix<real>::size_type;
    const auto ndv = b.getGradientsSize();
    const auto nth = b.getThermodynamicForcesSize();
    std::fill(k.begin(), k.end(), real(0));
    std::fill(r.begin(), r.end(), real(0));
    if (b.getBehaviourType() ==
        MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) {
      for (size_type i = 0; i != 3u; ++i) {
        r(i) += s(i);
        for (size_type j = 0; j != ndv; ++j) {
          k(i, j) += kt(i, j);
        }
      }
      if (getSpaceDimension(b.getHypothesis()) > 1u) {
        for (size_type i = 0; i != nth - 3u; ++i) {
          r(2 * i + 3u) += s(i);
          r(2 * i + 3u + 1) += s(i);
          for (size_type j = 0; j != ndv; ++j) {
            k(2 * i + 3u, j) += kt(i + 3u, j);
            k(2 * i + 3u + 1, j) += kt(i + 3u, j);
          }
        }
      }
    } else {
      for (size_type i = 0; i != nth; ++i) {
        r(i) += s(i);
        for (size_type j = 0; j != ndv; ++j) {
          k(i, j) += kt(i, j);
        }
      }
    }
  }  // end of updateStiffnessAndResidual

  MTest::UTest::~UTest() = default;

  MTest::MTest() : rm(real(0)) {}

  void MTest::readInputFile(const std::string& f,
                            const std::vector<std::string>& ecmds,
                            const std::map<std::string, std::string>& s) {
    MTestParser().execute(*this, f, ecmds, s);
    if (mfront::getVerboseMode() >= mfront::VERBOSE_DEBUG) {
      mfront::getLogStream() << "Defined evolutions:\n";
      for (const auto& e : *(this->evm)) {
        mfront::getLogStream() << "- " << e.first << '\n';
      }
      mfront::getLogStream() << '\n';
    }
  }  // end of readInputFile

  std::string MTest::name() const {
    return "unit behaviour test";
  }  // end of name

  std::string MTest::classname() const { return "MTest"; }

  void MTest::addConstraint(const std::shared_ptr<Constraint> c) {
    tfel::raise_if(this->initialisationFinished,
                   "MTest::addConstraint: "
                   "constraints can't be added after that "
                   "the `completeInitialisation` method has not been called");
    this->constraints.push_back(c);
  }

  void MTest::setGradientEpsilon(const real e) {
    tfel::raise_if(this->options.eeps > 0,
                   "MTest::setGradientEpsilon: the epsilon "
                   "value has already been declared");
    tfel::raise_if(e < 100 * std::numeric_limits<real>::min(),
                   "MTest::setGradientEpsilon:"
                   " invalid value");
    this->options.eeps = e;
  }

  void MTest::setThermodynamicForceEpsilon(const real s) {
    tfel::raise_if(this->options.seps > 0,
                   "MTest::setThermodynamicForceEpsilon: the epsilon "
                   "value has already been declared");
    tfel::raise_if(s < 100 * std::numeric_limits<real>::min(),
                   "MTest::setThermodynamicForceEpsilon: invalid value");
    this->options.seps = s;
  }

  void MTest::setRotationMatrix(const tfel::math::tmatrix<3u, 3u, real>& r,
                                const bool bo) {
    using namespace tfel::math;
    constexpr auto eps = 100 * std::numeric_limits<real>::epsilon();
    tfel::raise_if(this->b == nullptr,
                   "MTest::setRotationMatrix: "
                   "no behaviour defined");
    tfel::raise_if(this->b->getSymmetryType() != 1,
                   "MTest::setRotationMatrix: "
                   "rotation matrix may only be defined "
                   "for orthotropic behaviours");
    tfel::raise_if((this->isRmDefined) && (!bo),
                   "MTest::setRotationMatrix: "
                   "rotation matrix already defined");
    this->isRmDefined = true;
    // checking that the given matrix is a rotation one
    const tvector<3u, real> c0 = r.column_view<0>();
    const tvector<3u, real> c1 = r.column_view<1>();
    const tvector<3u, real> c2 = r.column_view<2>();
    tfel::raise_if((std::abs(norm(c0) - real(1)) > eps) ||
                       (std::abs(norm(c1) - real(1)) > eps) ||
                       (std::abs(norm(c2) - real(1)) > eps),
                   "MTest::setRotationMatrix: "
                   "at least one column is not normalised");
    tfel::raise_if((std::abs(c0 | c1) > eps) || (std::abs(c0 | c2) > eps) ||
                       (std::abs(c1 | c2) > eps),
                   "MTest::setRotationMatrix : "
                   "at least two columns are not orthogonals");
    this->rm = r;
  }

  void MTest::setDefaultModellingHypothesis() {
    using tfel::material::ModellingHypothesis;
    tfel::raise_if(this->b != nullptr,
                   "MTest::setDefaultModellingHypothesis: "
                   "behaviour already defined");
    tfel::raise_if(this->hypothesis != ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                   "MTest::setDefaultModellingHypothesis: "
                   "internal error : the modelling "
                   "hypothesis is already defined");
    if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
      auto& log = mfront::getLogStream();
      log << "No hypothesis defined, using default\n";
    }
    this->hypothesis = ModellingHypothesis::TRIDIMENSIONAL;
  }

  void MTest::addTest(const std::shared_ptr<MTest::UTest> t) {
    this->tests.push_back(t);
  }

  void MTest::setGradientsInitialValues(const std::vector<real>& v) {
    tfel::raise_if(!this->e_t0.empty(),
                   "MTest::setGradientsInitialValues: "
                   "the initial values of the strains have "
                   "already been declared");
    const auto N = this->b->getGradientsSize();
    tfel::raise_if(v.size() != N,
                   "MTest::setGradientsInitialValues: "
                   "invalid initial values size");
    this->e_t0.resize(N, 0);
    std::copy(v.begin(), v.end(), this->e_t0.begin());
  }

  void MTest::setThermodynamicForcesInitialValues(const std::vector<real>& v) {
    tfel::raise_if(!this->s_t0.empty(),
                   "MTest::setThermodynamicForcesInitialValues: "
                   "the initial values of the strains have "
                   "already been declared");
    const auto N = this->b->getThermodynamicForcesSize();
    tfel::raise_if(v.size() != N,
                   "MTest::setThermodynamicForcesInitialValues: "
                   "invalid initial values size");
    this->s_t0.resize(N, 0);
    std::copy(v.begin(), v.end(), this->s_t0.begin());
  }  // end of setThermodynamicForcesInitialValues

  void MTest::setGaussPointPositionForEvolutionsEvaluation(
      const CurrentState&) const {
  }  // end of setGaussPointPositionForEvolutionsEvaluation

  void MTest::completeInitialisation() {
    using namespace tfel::material;
    // additional constraints
    // must be set *before* `SingleStructureScheme::completeInitialisation`
    if (this->hypothesis == ModellingHypothesis::PLANESTRAIN) {
      if ((this->b->getBehaviourType() ==
           MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) ||
          (this->b->getBehaviourType() ==
           MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR)) {
        std::shared_ptr<Evolution> eev;
        if (this->b->getBehaviourType() ==
            MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) {
          eev = make_evolution(1.);
        } else {
          eev = make_evolution(0.);
        }
        auto ec = std::make_shared<ImposedGradient>(2, eev);
        this->constraints.push_back(ec);
      }
    }
    if (this->hypothesis ==
        ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) {
      // shall be in the behaviour
      if ((this->b->getBehaviourType() ==
           MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) ||
          (this->b->getBehaviourType() ==
           MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR)) {
        std::shared_ptr<Evolution> eev;
        if (this->b->getBehaviourType() ==
            MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) {
          eev = make_evolution(1.);
        } else {
          eev = make_evolution(0.);
        }
        auto ec = std::make_shared<ImposedGradient>(1, eev);
        std::shared_ptr<Evolution> sev;
        auto pev = this->evm->find("AxialStress");
        if (pev != this->evm->end()) {
          sev = pev->second;
        } else {
          sev = make_evolution(0.);
        }
        auto sc = std::make_shared<ImposedThermodynamicForce>(1, sev);
        this->constraints.push_back(ec);
        this->constraints.push_back(sc);
      }
    }
    if (this->hypothesis == ModellingHypothesis::PLANESTRESS) {
      // shall be in the behaviour
      if ((this->b->getBehaviourType() ==
           MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) ||
          (this->b->getBehaviourType() ==
           MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR)) {
        std::shared_ptr<Evolution> eev;
        if (this->b->getBehaviourType() ==
            MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) {
          eev = make_evolution(1.);
        } else {
          eev = make_evolution(0.);
        }
        auto ec = std::make_shared<ImposedGradient>(2, eev);
        auto sev = make_evolution(0.);
        auto sc = std::make_shared<ImposedThermodynamicForce>(2, sev);
        this->constraints.push_back(ec);
        this->constraints.push_back(sc);
      }
    }
    SingleStructureScheme::completeInitialisation();
    // post-processing
    unsigned short cnbr = 2;
    const char* dvn;
    const char* thn;
    if (this->out) {
      this->out << "# first column: time\n";
      if (this->b->getBehaviourType() ==
          MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) {
        dvn = "strain";
        thn = "stress";
      } else if (this->b->getBehaviourType() ==
                 MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) {
        dvn = "deformation gradient";
        thn = "Cauchy stress";
      } else if (this->b->getBehaviourType() ==
                 MechanicalBehaviourBase::COHESIVEZONEMODEL) {
        dvn = "opening displacement";
        thn = "cohesive force";
      } else {
        dvn = "gradients";
        thn = "thermodynamic forces";
      }
      const auto ndv = this->b->getGradientsSize();
      const auto gc = this->b->getGradientsComponents();
      const auto nth = this->b->getThermodynamicForcesSize();
      const auto thc = this->b->getThermodynamicForcesComponents();
      for (unsigned short i = 0; i != ndv; ++i) {
        this->out << "# " << cnbr << " column: " << i + 1
                  << "th component of the " << dvn << " (" << gc[i] << ")\n";
        ++cnbr;
      }
      if (this->shallPrintLagrangeMultipliers) {
        auto nc = std::size_t{1};
        for (const auto& pc : this->constraints) {
          const auto nlm = pc->getNumberOfLagrangeMultipliers();
          for (unsigned short i = 0; i != nlm; ++i) {
            this->out << "# " << cnbr << " column: " << i + 1
                      << "th Lagrange multplier of constraint " << nc << "\n";
            ++cnbr;
          }
          ++nc;
        }
      }
      for (unsigned short i = 0; i != nth; ++i) {
        this->out << "# " << cnbr << " column: " << i + 1
                  << "th component of the " << thn << " (" << thc[i] << ")\n";
        ++cnbr;
      }
      const auto& ivdes = this->b->getInternalStateVariablesDescriptions();
      tfel::raise_if(ivdes.size() != this->b->getInternalStateVariablesSize(),
                     "MTest::completeInitialisation : internal error "
                     "(the number of descriptions given by "
                     "the mechanical behaviour don't match "
                     "the number of internal state variables)");
      for (std::vector<std::string>::size_type i = 0; i != ivdes.size(); ++i) {
        this->out << "# " << cnbr << " column: " << ivdes[i] << '\n';
        ++cnbr;
      }
      this->out << "# " << cnbr << " column: stored energy\n";
      this->out << "# " << cnbr + 1 << " column: disspated energy\n";
      ++cnbr;
    }
    // convergence criterion value for driving variables
    if (this->options.eeps < 0) {
      this->options.eeps = 1.e-12;
    }
    // convergence criterion value for thermodynamic forces
    if (this->options.seps < 0) {
      this->options.seps = 1.e-3;
    }
    // tangent operator comparison criterion
    if (this->toeps < 0) {
      this->toeps = (this->options.seps / 1e-3) * 1.e7;
    }
    // perturbation value
    if (this->pv < 0) {
      this->pv = 10 * this->options.eeps;
    }
    if (!this->isRmDefined) {
      for (unsigned short i = 0; i != 3; ++i) {
        rm(i, i) = real(1);
      }
    }
    // thermal expansion reference temperature
    auto pev = this->evm->find("ThermalExpansionReferenceTemperature");
    if (pev != this->evm->end()) {
      const auto& ev = *(pev->second);
      tfel::raise_if(!ev.isConstant(),
                     "MTest::completeInitialisation: "
                     "'ThermalExpansionReferenceTemperature' "
                     "must be a constant evolution");
    }
    // residual file
    if (this->residual) {
      this->residual << "#first  column: iteration number\n"
                     << "#second column: driving variable residual\n"
                     << "#third  column: thermodynamic force residual\n"
                     << "#The following columns are the components of the "
                        "driving variable\n";
    }
  }

  void MTest::initializeCurrentState(StudyCurrentState& s) const {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "MTest::initializeCurrentState: " + m);
    };
    throw_if(this->b == nullptr, "mechanical behaviour not set");
    throw_if(this->hypothesis ==
                 tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS,
             "modelling hypothesis is not set");
    // unknowns
    const auto psz = this->getNumberOfUnknowns();
    s.initialize(psz);
    // initializing the state of the one and only structure
    auto& ss = s.getStructureCurrentState("");
    ss.setBehaviour(this->b);
    ss.setModellingHypothesis(this->hypothesis);
    // state of the one and only integration point
    ss.istates.resize(1);
    auto& cs = ss.istates[0];
    this->b->allocateCurrentState(cs);
    // setting the intial  values of strains
    this->b->getGradientsDefaultInitialValues(s.u_1);
    copy(this->e_t0.begin(), this->e_t0.end(), s.u_1.begin());
    s.u0 = s.u_1;
    s.u1 = s.u_1;
    // setting the intial  values of stresses
    copy(this->s_t0.begin(), this->s_t0.end(), cs.s0.begin());
    // getting the initial values of internal state variables
    throw_if((this->iv_t0.size() > cs.iv_1.size()) ||
                 (this->iv_t0.size() > cs.iv0.size()) ||
                 (this->iv_t0.size() > cs.iv1.size()),
             "the number of initial values declared "
             "by the user for the internal state variables exceeds the "
             "number of internal state variables declared by the behaviour");
    std::copy(this->iv_t0.begin(), this->iv_t0.end(), cs.iv_1.begin());
    std::copy(this->iv_t0.begin(), this->iv_t0.end(), cs.iv0.begin());
    // revert the current state
    mtest::revert(ss);
    // rotation matrix
    cs.r = this->rm;
    // reference temperature
    const auto pev = this->evm->find("ThermalExpansionReferenceTemperature");
    if (pev != this->evm->end()) {
      const auto& ev = *(pev->second);
      throw_if(!ev.isConstant(),
               "'ThermalExpansionReferenceTemperature' "
               "must be a constant evolution");
      cs.Tref = ev(0);
    }
  }  // end of initializeCurrentState

  void MTest::initializeWorkSpace(SolverWorkSpace& wk) const {
    tfel::raise_if(!this->initialisationFinished,
                   "MTest::initializeWorkSpace: "
                   "object not initialised");
    const auto psz = this->getNumberOfUnknowns();
    // clear
    wk.K.clear();
    wk.p_lu.clear();
    wk.x.clear();
    wk.r.clear();
    wk.du.clear();
    // resizing
    wk.K.resize(psz, psz);
    wk.p_lu.resize(psz);
    wk.x.resize(psz);
    wk.r.resize(psz, 0.);
    wk.du.resize(psz, 0.);
  }  // end of initializeWorkSpace

  size_t MTest::getNumberOfUnknowns() const {
    tfel::raise_if(!this->initialisationFinished,
                   "MTest::getNumberOfUnknowns: "
                   "object not initialised");
    // getting the total number of unknowns
    size_t s = this->b->getGradientsSize();;
    for (const auto& pc : this->constraints) {
      s += pc->getNumberOfLagrangeMultipliers();
    }
    return s;
  }  // end of getNumberOfUnknowns

  tfel::tests::TestResult MTest::execute() {
    return this->execute(true);
  }  // end of execute

  tfel::tests::TestResult MTest::execute(const bool bInit) {
    auto report = [](const char* msg, const StudyCurrentState& s,
                     const bool bs) {
      if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
        auto& log = mfront::getLogStream();
        log << "Execution " << (bs ? "succeeded" : "failed");
        if (msg) {
          log << " (" << msg << ")";
        } else if (!bs) {
          log << " (unknown reason)";
        }
        log << '\n'
            << "-number of period:     " << s.period - 1 << '\n'
            << "-number of iterations: " << s.iterations << '\n'
            << "-number of sub-steps:  " << s.subSteps << '\n';
      }
    };
    if (!bInit) {
      tfel::raise_if(!this->initialisationFinished,
                     "the completeInitialisation has not been called");
    }
    // initialize current state and work space
    StudyCurrentState state;
    SolverWorkSpace wk;
    try {
      // some checks
      tfel::raise_if(times.empty(), "MTest::execute: no times defined");
      tfel::raise_if(times.size() < 2,
                     "MTest::execute: invalid number of times defined");
      // finish initialization
      if (bInit) {
        this->completeInitialisation();
      }
      this->initializeCurrentState(state);
      this->initializeWorkSpace(wk);
      // integrating over the loading path
      auto pt = this->times.begin();
      auto pt2 = pt + 1;
      this->printOutput(*pt, state, true);
      // real work begins here
      while (pt2 != this->times.end()) {
        // allowing subdivisions of the time step
        this->execute(state, wk, *pt, *pt2);
        this->printOutput(*pt2, state, true);
        ++pt;
        ++pt2;
      }
    } catch (std::exception& e) {
      this->out.flush();
      report(e.what(), state, false);
      throw;
    } catch (...) {
      this->out.flush();
      report(nullptr, state, false);
      throw;
    }
    this->out.flush();
    report(nullptr, state, true);
    tfel::tests::TestResult tr;
    for (const auto& t : this->tests) {
      tr.append(t->getResults());
    }
    return tr;
  }

  void MTest::printLagrangeMultipliers(const bool b){
    this->shallPrintLagrangeMultipliers = b;
  } // end of printLagrangeMultipliers

  void MTest::setCompareToNumericalTangentOperator(const bool bo) {
    this->cto = bo;
  }  // end of setCompareToNumericalTangentOperator

  void MTest::setTangentOperatorComparisonCriterion(const real v) {
    tfel::raise_if(v < 100 * std::numeric_limits<real>::min(),
                   "MTest::setTangentOperatorComparisonCriterion: "
                   "invalid comparison criterium");
    this->toeps = v;
  }  // end of setTangentOperatorComparisonCriterion

  void MTest::setNumericalTangentOperatorPerturbationValue(const real v) {
    tfel::raise_if(v < 100 * std::numeric_limits<real>::min(),
                   "MTest::setNumericalTangentOperatorPerturbationValue: "
                   "invalid perturbation value");
    this->pv = v;
  }  // end of setNumericalTangentOperatorPerturbationValue

  std::pair<bool, real> MTest::prepare(StudyCurrentState& state,
                                       const real t,
                                       const real dt) const {
    auto& scs = state.getStructureCurrentState("");
    tfel::raise_if(scs.istates.size() != 1u, "MTest::prepare: invalid state");
    // driving variables at the beginning of the time step
    for (auto& s : scs.istates) {
      const auto ndv = this->b->getGradientsSize();
      for (unsigned short i = 0; i != ndv; ++i) {
        s.e0[i] = state.u0[i];
      }
    }
    return SingleStructureScheme::prepare(state, t, dt);
  }  // end of prepare

  void MTest::makeLinearPrediction(StudyCurrentState& state,
                                   const real dt) const {
    auto& scs = state.getStructureCurrentState("");
    tfel::raise_if(scs.istates.size() != 1u, "MTest::prepare: invalid state");
    auto& s = scs.istates[0];
    if (state.period > 1) {
      const auto r = dt / state.dt_1;
      state.u1 = state.u0 + (state.u0 - state.u_1) * r;
      s.iv1 = s.iv0 + (s.iv0 - s.iv_1) * r;
      s.s1 = s.s0 + (s.s0 - s.s_1) * r;
    }
  }  // end of makeLinearPrediction

  std::pair<bool, real> MTest::computePredictionStiffnessAndResidual(
      StudyCurrentState& state,
      tfel::math::matrix<real>& k,
      tfel::math::vector<real>& r,
      const real& t,
      const real& dt,
      const StiffnessMatrixType smt) const {
    using tfel::material::MechanicalBehaviourBase;
    using index_type = unsigned short;
    auto& scs = state.getStructureCurrentState("");
    auto& bwk = scs.getBehaviourWorkSpace();
    tfel::raise_if(scs.istates.size() != 1u, "MTest::prepare: invalid state");
    auto& s = scs.istates[0];
    auto res = this->b->computePredictionOperator(bwk, s, smt);
    if (!res.first) {
      return res;
    }
    std::fill(k.begin(), k.end(), 0.);
    std::fill(r.begin(), r.end(), 0.);
    updateStiffnessAndResidual(k, r, *(this->b), bwk.kt, s.s0);
    const auto ndv = this->b->getGradientsSize();
    const auto nth = this->b->getThermodynamicForcesSize();
    // free dilatation treatment
    if (this->b->getBehaviourType() ==
        MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR) {
      for (index_type i = 0; i != nth; ++i) {
        for (index_type j = 0; j != ndv; ++j) {
          r(i) -= k(i, j) * (s.e_th1[j] - s.e_th0[j]);
        }
      }
    }
    if (!state.containsParameter("LagrangeMultipliersNormalisationFactor")) {
      state.setParameter(
          "LagrangeMultipliersNormalisationFactor",
          std::abs(*(std::max_element(k.begin(), k.end(),
                                      [](const real v1, const real v2) {
                                        return std::abs(v1) < std::abs(v2);
                                      }))));
    }
    const auto a =
        state.getParameter<real>("LagrangeMultipliersNormalisationFactor");
    size_type pos = ndv;
    for (const auto& c : this->constraints) {
      const auto nl = c->getNumberOfLagrangeMultipliers();
      c->setValues(k, r, state.u0, state.u0, bwk.kt, s.s0, pos,
                   getSpaceDimension(this->hypothesis), t, dt, a);
      pos = static_cast<size_type>(pos + nl);
    }
    return res;
  }  // end of computePredictionStiffnessAndResidual

  std::pair<bool, real> MTest::computeStiffnessMatrixAndResidual(
      StudyCurrentState& state,
      tfel::math::matrix<real>& k,
      tfel::math::vector<real>& r,
      const real t,
      const real dt,
      const StiffnessMatrixType mt) const {
    using namespace tfel::material;
    auto display = [](const tfel::math::matrix<real>& m, const size_type mi,
                      const size_type mj) {
      using tfel::utilities::TerminalColors;
      auto& os = mfront::getLogStream();
      for (size_type i = 0; i != m.getNbRows(); ++i) {
        for (size_type j = 0; j != m.getNbCols();) {
          if ((i == mi) && (j == mj)) {
            os.write(TerminalColors::Red, sizeof(TerminalColors::Red));
            os << m(i, j);
            os.write(TerminalColors::Reset, sizeof(TerminalColors::Reset));
          } else {
            os << m(i, j);
          }
          if (++j != m.getNbCols()) {
            os << " ";
          }
        }
        os << "\n";
      }
    };
    auto& scs = state.getStructureCurrentState("");
    auto& bwk = scs.getBehaviourWorkSpace();
    tfel::raise_if(scs.istates.size() != 1u, "MTest::prepare: invalid state");
    auto& s = scs.istates[0];
    const auto ndv = this->b->getGradientsSize();
    const auto nth = this->b->getThermodynamicForcesSize();
    for (size_type i = 0; i != ndv; ++i) {
      s.e1[i] = state.u1[i];
    }
    // behaviour integration
    setRoundingMode();
    const auto rb = this->b->integrate(s, bwk, dt, mt);
    setRoundingMode();
    if (!rb.first) {
      if (mfront::getVerboseMode() > mfront::VERBOSE_QUIET) {
        auto& log = mfront::getLogStream();
        log << "MTest::computeStiffnessMatrixAndResidual : "
            << "behaviour intregration failed\n";
      }
      return rb;
    }
    if ((this->cto) && (mt == StiffnessMatrixType::CONSISTENTTANGENTOPERATOR)) {
      bool ok = true;
      bwk.ne.swap(s.e1);
      bwk.ns.swap(s.s1);
      bwk.nivs.swap(s.iv1);
      for (size_type i = 0; i != ndv; ++i) {
        revert(s);
        std::copy(bwk.ne.begin(), bwk.ne.end(), s.e1.begin());
        s.e1[i] += this->pv;
        try {
          setRoundingMode();
          ok = this->b->integrate(s, bwk, dt, mt).first;
          setRoundingMode();
        } catch (...) {
          ok = false;
        }
        if (!ok) {
          break;
        }
        for (size_type j = 0; j != nth; ++j) {
          bwk.nk(j, i) = s.s1(j);
        }
        revert(s);
        std::copy(bwk.ne.begin(), bwk.ne.end(), s.e1.begin());
        s.e1[i] -= this->pv;
        try {
          setRoundingMode();
          ok = this->b->integrate(s, bwk, dt, mt).first;
          setRoundingMode();
        } catch (...) {
          ok = false;
        }
        if (!ok) {
          break;
        }
        for (size_type j = 0; j != nth; ++j) {
          bwk.nk(j, i) -= s.s1(j);
          bwk.nk(j, i) /= 2 * (this->pv);
        }
      }
      bwk.ne.swap(s.e1);
      bwk.ns.swap(s.s1);
      bwk.nivs.swap(s.iv1);
      if (ok) {
        real merr(0);
        size_type mi = 0;
        size_type mj = 0;
        for (size_type i = 0; i != nth; ++i) {
          for (size_type j = 0; j != ndv; ++j) {
            const auto err = std::abs(bwk.k(i, j) - bwk.nk(i, j));
            if (err > merr) {
              merr = err;
              mi = i;
              mj = j;
            }
          }
        }
        if (merr > this->toeps) {
          auto& log = mfront::getLogStream();
          log << "Comparison to numerical jacobian failed "
              << "(error : " << merr << " for (" << mi << "," << mj
              << "), criterium " << this->toeps << ").\n"
              << "Tangent operator returned by the behaviour : \n";
          display(bwk.k, mi, mj);
          log << "Numerical tangent operator (perturbation value : " << this->pv
              << ") : \n";
          display(bwk.nk, mi, mj);
        }
      } else {
        auto& log = mfront::getLogStream();
        log << "Numerical evalution of tangent operator failed.\n\n";
      }
    }
    //! update the stiffness matrix and the residual
    std::fill(k.begin(), k.end(), real(0));
    updateStiffnessAndResidual(k, r, *(this->b), bwk.k, s.s1);
    if (!this->constraints.empty()) {
      if (!state.containsParameter("LagrangeMultipliersNormalisationFactor")) {
        if (k.empty()) {
          state.setParameter("LagrangeMultipliersNormalisationFactor", real(1));
        } else {
          state.setParameter(
              "LagrangeMultipliersNormalisationFactor",
              std::abs(*(std::max_element(k.begin(), k.end(),
                                          [](const real v1, const real v2) {
                                            return std::abs(v1) < std::abs(v2);
                                          }))));
        }
      }
      const auto a =
          state.getParameter<real>("LagrangeMultipliersNormalisationFactor");
      const auto d = getSpaceDimension(this->hypothesis);
      size_type pos = ndv;
      for (const auto& c : this->constraints) {
        const auto nl = c->getNumberOfLagrangeMultipliers();
        c->setValues(k, r, state.u0, state.u1, bwk.k, s.s1, pos, d, t, dt, a);
        pos = static_cast<size_type>(pos + nl);
      }
    }
    return rb;
  }  // end of computeStiffnessMatrixAndResidual

  /*!
   * \brief compute the error norm
   * \param[in] v: values
   * \param[in] s: size
   */
  static real MTest_getErrorNorm(const tfel::math::vector<real>& v,
                                 const tfel::math::vector<real>::size_type s) {
    using size_type = tfel::math::vector<real>::size_type;
    auto n = real(0);
    for (size_type i = 0; i != s; ++i) {
      n = std::max(n, std::abs(v(i)));
    }
    return n;
  }

  real MTest::getErrorNorm(const tfel::math::vector<real>& du) const {
    const auto ndv = this->b->getGradientsSize();
    return MTest_getErrorNorm(du, ndv);
  }  // end of getErrorNorm

  bool MTest::checkConvergence(StudyCurrentState& state,
                               const tfel::math::vector<real>& du,
                               const tfel::math::vector<real>& r,
                               const SolverOptions& o,
                               const unsigned int iter,
                               const real t,
                               const real dt) const {
    auto report = [&iter, &state](std::ostream& os,
                                  const tfel::math::vector<real>::size_type ndv,
                                  const real ne, const real nr) {
      os << "iteration " << iter << " : " << ne << " " << nr << " (";
      for (size_type i = 0; i != ndv;) {
        os << state.u1(i);
        if (++i != ndv) {
          os << " ";
        }
      }
      os << ")\n";
    };
    const auto& scs = state.getStructureCurrentState("");
    tfel::raise_if(scs.istates.size() != 1u, "MTest::prepare: invalid state");
    const auto& s = scs.istates[0];
    const auto ndv = this->b->getGradientsSize();
    const auto ne = getErrorNorm(du);
    const auto nr = MTest_getErrorNorm(r, ndv);
    if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
      auto& log = mfront::getLogStream();
      report(log, ndv, ne, nr);
    }
    if (this->residual) {
      report(this->residual, ndv, ne, nr);
    }
    if ((!tfel::math::ieee754::isfinite(ne)) ||
        (!tfel::math::ieee754::isfinite(nr))) {
      return false;
    }
    if ((ne > o.eeps) || (nr > o.seps)) {
      return false;
    }
    for (const auto& c : this->constraints) {
      if (c->isActive()) {
        if (!c->checkConvergence(state.u1, s.s1, o.eeps, o.seps, t, dt)) {
          return false;
        }
      }
    }
    return true;
  }

  std::vector<std::string> MTest::getFailedCriteriaDiagnostic(
      const StudyCurrentState& state,
      const tfel::math::vector<real>& du,
      const tfel::math::vector<real>& r,
      const SolverOptions& o,
      const real t,
      const real dt) const {
    const auto& scs = state.getStructureCurrentState("");
    tfel::raise_if(scs.istates.size() != 1u,
                   "MTest::getFailedCriteriaDiagnostic: invalid state");
    const auto& s = scs.istates[0];
    const auto ndv = this->b->getGradientsSize();
    const auto ne = this->getErrorNorm(du);
    const auto nr = MTest_getErrorNorm(r, ndv);
    auto fc = std::vector<std::string>{};
    if (ne > o.eeps) {
      std::ostringstream msg;
      msg << "test on driving variables (error : " << ne
          << ", criterion value : " << o.eeps << ")";
      fc.push_back(msg.str());
    }
    if (nr > o.seps) {
      std::ostringstream msg;
      msg << "test on thermodynamic forces (error : " << nr
          << ", criterion value : " << o.seps << ")";
      fc.push_back(msg.str());
    }
    for (const auto& c : this->constraints) {
      if (c->isActive()) {
        if (!c->checkConvergence(state.u1, s.s1, o.eeps, o.seps, t, dt)) {
          fc.push_back(c->getFailedCriteriaDiagnostic(state.u1, s.s1, o.eeps,
                                                      o.seps, t, dt));
        }
      }
    }
    return fc;
  }  // end of getFailedCriteriaDiagnostic

  void MTest::computeLoadingCorrection(StudyCurrentState&,
                                       SolverWorkSpace&,
                                       const SolverOptions&,
                                       const real,
                                       const real) const {
  }  // end of computeLoadingCorrection

  bool MTest::postConvergence(StudyCurrentState& state,
                              const real t,
                              const real dt,
                              const unsigned int p) const {
    auto& scs = state.getStructureCurrentState("");
    tfel::raise_if(scs.istates.size() != 1u,
                   "MTest::postConvergence: invalid state");
    auto& s = scs.istates[0];
    for (const auto& test : this->tests) {
      test->check(s, t, dt, p);
    }
    for (const auto& up : this->upostprocessings) {
      up->exe(s, t, dt);
    }
    return true;
  }  // end of postConvergence

  void MTest::execute(StudyCurrentState& state,
                      SolverWorkSpace& wk,
                      const real ti,
                      const real te) {
    GenericSolver().execute(state, wk, *this, this->options, ti, te);
    auto p = this->events.begin();
    while (p != this->events.end()) {
      if (!(p->first > te)) {
        for (const auto& e : p->second) {
          for (const auto& c : this->constraints) {
            c->treatEvent(e);
          }
        }
        p = this->events.erase(p);
      } else {
        break;
      }
    }
  }  // end of execute

  void MTest::printOutput(const real t,
                          const StudyCurrentState& s,
                          const bool o) const {
    if ((!o) && (this->output_frequency == USERDEFINEDTIMES)) {
      return;
    }
    if (this->out) {
      auto& cs = s.getStructureCurrentState("").istates[0];
      this->out << t << " ";
      if (this->shallPrintLagrangeMultipliers) {
        std::copy(s.u0.begin(), s.u0.end(),
                  std::ostream_iterator<real>(this->out, " "));
      } else {
        for (unsigned short i = 0; i != this->b->getGradientsSize(); ++i) {
          this->out << s.u0[i] << " ";
        }
      }
      const auto nth = this->b->getThermodynamicForcesSize();
      for (unsigned short i = 0; i != nth; ++i) {
        this->out << cs.s0[i] << " ";
      }
      std::copy(cs.iv0.begin(), cs.iv0.end(),
                std::ostream_iterator<real>(this->out, " "));
      // stored and dissipated energies
      this->out << cs.se0 << " " << cs.de0 << '\n';
    }
  }  // end of printOutput

  void MTest::addEvent(const std::string& e,
                       const std::vector<double>& evtimes) {
    for (const auto t : evtimes) {
      this->events[t].push_back(e);
    }
  }  // end of addEvent

  void MTest::addUserDefinedPostProcessing(const std::string& f,
                                           const std::vector<std::string>& p) {
    this->upostprocessings.push_back(
        std::make_shared<UserDefinedPostProcessing>(
            *(this->getBehaviour()), this->getEvolutions(), f, p));
  }  // end of addUserDefinedPostProcessing

  MTest::~MTest() = default;

}  // end namespace mtest
