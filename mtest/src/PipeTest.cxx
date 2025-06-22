/*!
 * \file   PipeTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   24 nov. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <memory>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Math/LUSolve.hxx"
#include "TFEL/Utilities/TextData.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/RoundingMode.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/SolverWorkSpace.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/StudyCurrentState.hxx"
#include "MTest/StructureCurrentState.hxx"
#include "MTest/GenericSolver.hxx"
#include "MTest/GasEquationOfState.hxx"
#include "MTest/PipeLinearElement.hxx"
#include "MTest/PipeQuadraticElement.hxx"
#include "MTest/PipeCubicElement.hxx"
#include "MTest/PipeProfile.hxx"
#include "MTest/PipeProfileHandler.hxx"
#include "MTest/PipeTest.hxx"

namespace mtest {

  PipeTest::UTest::~UTest() = default;

  struct IntegralTest : public PipeTest::UTest {
    IntegralTest(const std::string& n,
                 const tfel::utilities::TextData& d,
                 const unsigned short c,
                 const real e)
        : name(n), values(d.getColumn(c)), eps(e) {
      if (n == "InnerDisplacement") {
        this->v = INNERDISPLACEMENT;
      } else if (n == "OuterDisplacement") {
        this->v = OUTERDISPLACEMENT;
      } else if (n == "AxialGrowth") {
        this->v = AXIALGROWTH;
      } else {
        tfel::raise(
            "IntegralTest::IntegralTest: "
            "unsupported variable '" +
            n + "'");
      }
    }
    void check(const StudyCurrentState& s,
               const real t,
               const real dt,
               const unsigned int p) override {
      const auto uv = [](const tfel::math::vector<real>& u, const Variable vn) {
        if (vn == INNERDISPLACEMENT) {
          return *(u.begin());
        } else if (vn == OUTERDISPLACEMENT) {
          return *(u.rbegin() + 1);
        } else if (vn == AXIALGROWTH) {
          return *(u.rbegin());
        }
        tfel::raise(
            "IntegralTest::check: "
            "unsupported variable");
      }(s.u1, this->v);
      if (p >= this->values.size()) {
        std::ostringstream msg;
        msg << "IntegralTest::check : comparison for variable '" << this->name
            << "' failed for time '" << t + dt << "' "
            << "(reference value is not available for period  '" << p << "')";
        this->results.append(tfel::tests::TestResult(false, msg.str()));
      } else {
        const real err = std::abs(uv - this->values[p]);
        if (err > this->eps) {
          std::ostringstream msg;
          msg << "IntegralTest::check : comparison for variable '" << this->name
              << "' failed for time '" << t + dt << "' "
              << "(computed value: '" << uv << "', "
              << "expected value: '" << this->values[p] << "', "
              << "error: '" << err << "', criterion '" << this->eps << "')";
          this->results.append(tfel::tests::TestResult(false, msg.str()));
        }
      }
    }  // end of check
    tfel::tests::TestResult getResults() const override {
      if (this->results.success()) {
        std::ostringstream msg;
        msg << "IntegralTest::check : comparison for variable '" << this->name
            << "' was successfull for all times ("
            << "criterion '" << this->eps << "')";
        return tfel::tests::TestResult(true, msg.str());
      }
      return this->results;
    }
    ~IntegralTest() override = default;

   protected:
    enum Variable { INNERDISPLACEMENT, OUTERDISPLACEMENT, AXIALGROWTH } v;
    //! results of the test
    tfel::tests::TestResult results;
    //! name of the variable
    const std::string name;
    //! values of for the test
    const std::vector<real> values;
    //! criterion value
    const real eps;
  };

  struct ProfileTest : public PipeTest::UTest {
    // simple alias
    using size_type = tfel::math::vector<real>::size_type;
    /*!
     * \param[in] n: name of the variable
     * \param[in] p: variable position in the appropriate array
     * \param[in] d: text data
     * \param[in] c: column of the data
     * \param[in] e: criterion value
     */
    ProfileTest(const std::string& n,
                const size_type p,
                const tfel::utilities::TextData& d,
                const unsigned short c,
                const real e)
        : name(n), values(d.getColumn(c)), vpos(p), eps(e) {}
    void check(const StudyCurrentState& ss,
               const real t,
               const real dt,
               const unsigned int p) override {
      auto& scs = ss.getStructureCurrentState("");
      // number of gauss points
      const auto ng = scs.istates.size();
      // current position
      size_type i = 0;
      // loop over the gauss points
      for (const auto& s : scs.istates) {
        const auto v = this->getComputedValues(s)[vpos];
        // data position
        const auto dp = ng * (p - 1) + i;
        if (dp >= this->values.size()) {
          std::ostringstream msg;
          msg << "ProfileTest::check : comparison for variable '" << this->name
              << "' failed for time '" << t + dt << "' "
              << "(reference value is not available for period  '" << p << "')";
          this->results.append(tfel::tests::TestResult(false, msg.str()));
        } else {
          const real err = std::abs(v - this->values[dp]);
          if (err > this->eps) {
            std::ostringstream msg;
            msg << "ProfileTest::check : comparison for variable '"
                << this->name << "' failed for time '" << t + dt << "' "
                << "(computed value: '" << v << "', "
                << "expected value: '" << this->values[dp] << "', "
                << "error: '" << err << "', criterion '" << this->eps << "')";
            this->results.append(tfel::tests::TestResult(false, msg.str()));
          }
        }
        ++i;
      }
    }  // end of check
    tfel::tests::TestResult getResults() const override {
      if (this->results.success()) {
        std::ostringstream msg;
        msg << "ProfileTest::check : comparison for variable '" << this->name
            << "' was successfull for all times "
            << "and all gauss points (criterion '" << this->eps << "')";
        return tfel::tests::TestResult(true, msg.str());
      }
      return this->results;
    }
    ~ProfileTest() override = default;

   protected:
    /*!
     * \return the array of values in which the variable tested is
     * extracted
     * \param[in] s: gauss point state
     */
    virtual const tfel::math::vector<real>& getComputedValues(
        const CurrentState&) const = 0;
    //! results of the test
    tfel::tests::TestResult results;
    //! name of the variable
    const std::string name;
    //! values of for the test
    const std::vector<real> values;
    //! variable position
    size_type vpos;
    //! criterion
    const real eps;
  };

  struct StressProfileTest : public ProfileTest {
    StressProfileTest(const std::string& n,
                      const size_type p,
                      const tfel::utilities::TextData& d,
                      const unsigned short c,
                      const real e)
        : ProfileTest(n, p, d, c, e) {}

   protected:
    /*!
     * \return the array of values in which the variable tested is
     * extracted
     * \param[in] s: gauss point state
     */
    const tfel::math::vector<real>& getComputedValues(
        const CurrentState& s) const override {
      return s.s1;
    }
  };

  struct StrainProfileTest : public ProfileTest {
    StrainProfileTest(const std::string& n,
                      const size_type p,
                      const tfel::utilities::TextData& d,
                      const unsigned short c,
                      const real e)
        : ProfileTest(n, p, d, c, e) {}

   protected:
    /*!
     * \return the array of values in which the variable tested is
     * extracted
     * \param[in] s: gauss point state
     */
    const tfel::math::vector<real>& getComputedValues(
        const CurrentState& s) const override {
      return s.e1;
    }
  };

  struct InternalStateVariableProfileTest : public ProfileTest {
    InternalStateVariableProfileTest(const std::string& n,
                                     const size_type p,
                                     const tfel::utilities::TextData& d,
                                     const unsigned short c,
                                     const real e)
        : ProfileTest(n, p, d, c, e) {}

   protected:
    /*!
     * \return the array of values in which the variable tested is
     * extracted
     * \param[in] s: gauss point state
     */
    const tfel::math::vector<real>& getComputedValues(
        const CurrentState& s) const override {
      return s.iv1;
    }
  };

  template <typename T>
  static void setMeshValue(T& v,
                           const char* const m,
                           const char* const n,
                           const T nv,
                           const bool b) {
    tfel::raise_if(v >= 0, std::string(m) +
                               ": "
                               "value '" +
                               std::string(n) + "' already set");
    tfel::raise_if(b ? nv < T(0) : nv <= T(0), std::string(m) +
                                                   ": "
                                                   "invalid value for '" +
                                                   std::string(n) + "'");
    v = nv;
  }  // end of setMeshValue

  static void insert(EvolutionManager& evm,
                     const std::string& n,
                     const real v) {
    tfel::raise_if(evm.find(n) != evm.end(),
                   "insert: variable 'n' already declared");
    evm.insert({n, make_evolution(v)});
  }  // end of setCurrentPosition

  PipeTest::PipeTest() { insert(*(this->evm), "r", 0); }  // end of PipeTest

  void PipeTest::setInnerRadius(const real r) {
    if (this->mesh.outer_radius > 0) {
      tfel::raise_if(r >= this->mesh.outer_radius,
                     "PipeTest::setInnerRadius: invalid value. "
                     "Inner radius would be greater than the "
                     "external radius");
    }
    setMeshValue(this->mesh.inner_radius, "PipeTest::setInnerRadius",
                 "inner radius", r, true);
    insert(*(this->evm), "Ri", this->mesh.inner_radius);
  }  // end of setInnerRadius

  void PipeTest::setOuterRadius(const real r) {
    if (this->mesh.inner_radius > 0) {
      tfel::raise_if(r <= this->mesh.inner_radius,
                     "PipeTest::setOuterRadius: invalid value. "
                     "Inner radius would be greater than the "
                     "external radius");
    }
    setMeshValue(this->mesh.outer_radius, "PipeTest::setOuterRadius",
                 "outer radius", r, false);
    insert(*(this->evm), "Re", this->mesh.outer_radius);
  }  // end of setOuterRadius

  void PipeTest::setNumberOfElements(const int r) {
    setMeshValue(this->mesh.number_of_elements, "PipeTest::setNumberOfElements",
                 "number of elements", r, false);
  }  // end of setNumberOfElements

  const PipeMesh& PipeTest::getMesh() const { return this->mesh; }

  template <typename T>
  static void checkValue(const T& v, const char* const n) {
    tfel::raise_if(v < T(0),
                   "PipeTest::getMesh: "
                   "uninitialized value for '" +
                       std::string(n) + "'");
  }  // end of checkValue

  static void setCurrentPosition(EvolutionManager& evm, const real r) {
    auto p = evm.find("r");
    tfel::raise_if(p == evm.end(),
                   "setCurrentPosition: radial position undeclared. ");
    p->second->setValue(r);
  }  // end of setCurrentPosition

  void PipeTest::setGaussPointPositionForEvolutionsEvaluation(
      const CurrentState& s) const {
    setCurrentPosition(*(this->evm), s.position);
  }  // end of setGaussPointPositionForEvolutionsEvaluation

  void PipeTest::addIntegralTest(const std::string& n,
                                 const tfel::utilities::TextData& d,
                                 const unsigned short c,
                                 const real e) {
    if ((n == "InnerDisplacement") || (n == "OuterDisplacement") ||
        (n == "AxialGrowth")) {
      this->tests.push_back(
          std::shared_ptr<UTest>(new IntegralTest(n, d, c, e)));
    } else {
      tfel::raise(
          "PipeTest::addIntegralTest: "
          "unsupported variable '" +
          n + "'");
    }
  }  // end of addIntegralTest

  void PipeTest::addProfileTest(const std::string& n,
                                const tfel::utilities::TextData& d,
                                const unsigned short c,
                                const real e) {
    if (n == "SRR") {
      this->tests.push_back(
          std::shared_ptr<UTest>(new StressProfileTest(n, 0u, d, c, e)));
    } else if (n == "SZZ") {
      this->tests.push_back(
          std::shared_ptr<UTest>(new StressProfileTest(n, 1u, d, c, e)));
    } else if (n == "STT") {
      this->tests.push_back(
          std::shared_ptr<UTest>(new StressProfileTest(n, 2u, d, c, e)));
    } else if (n == "ERR") {
      this->tests.push_back(
          std::shared_ptr<UTest>(new StrainProfileTest(n, 0u, d, c, e)));
    } else if (n == "EZZ") {
      this->tests.push_back(
          std::shared_ptr<UTest>(new StrainProfileTest(n, 1u, d, c, e)));
    } else if (n == "ETT") {
      this->tests.push_back(
          std::shared_ptr<UTest>(new StrainProfileTest(n, 2u, d, c, e)));
    } else {
      const auto piv =
          std::find(this->ivfullnames.begin(), this->ivfullnames.end(), n);
      tfel::raise_if(piv == this->ivfullnames.end(),
                     "PipeTest::addProfileTest: "
                     "no internal state variable named '" +
                         n + "'");
      const auto p = piv - this->ivfullnames.begin();
      this->tests.push_back(std::shared_ptr<UTest>(
          new InternalStateVariableProfileTest(n, p, d, c, e)));
    }
  }  // end of addReferenceFileComparisonTest

  void PipeTest::completeInitialisation() {
    checkValue(mesh.inner_radius, "inner radius");
    checkValue(mesh.outer_radius, "outer radius");
    checkValue(mesh.number_of_elements, "number of elements");
    setCurrentPosition(*(this->evm),
                       (mesh.outer_radius - mesh.inner_radius) / 2);
    if (this->mesh.etype == PipeMesh::DEFAULT) {
      this->mesh.etype = PipeMesh::QUADRATIC;
    }
    SingleStructureScheme::completeInitialisation();
    if (this->options.eeps < 0) {
      this->options.eeps = 1.e-11;
    }
    if (this->options.seps < 0) {
      this->options.seps = 1.e-3;
    }
    tfel::raise_if(
        this->hypothesis !=
            ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
        "PipeTest::completeInitialisation: "
        "invalid modelling hypothesis "
        "('" +
            ModellingHypothesis::toString(this->hypothesis) + "')");
    if (this->al == DEFAULTAXIALLOADING) {
      this->al = ENDCAPEFFECT;
    }
    if (this->al == IMPOSEDAXIALFORCE) {
      tfel::raise_if(this->axial_force == nullptr,
                     "PipeTest::completeInitialisation: "
                     "the axial force evolution must be defined");
    }
    if (this->al == IMPOSEDAXIALGROWTH) {
      if (this->axial_growth == nullptr) {
        this->axial_growth = make_evolution(real(0));
      }
    }
    if (this->rl == DEFAULTLOADINGTYPE) {
      this->rl = IMPOSEDPRESSURE;
    }
    if (this->rl == IMPOSEDOUTERRADIUS) {
      tfel::raise_if(this->orev == nullptr,
                     "PipeTest::completeInitialisation: "
                     "the outer radius evolution must be defined");
    } else {
      if ((this->rl != TIGHTPIPE) && (this->al != IMPOSEDAXIALFORCE) &&
          (this->al != IMPOSEDAXIALGROWTH)) {
        tfel::raise_if((this->inner_pressure == nullptr) &&
                           (this->outer_pressure == nullptr),
                       "PipeTest::completeInitialisation: "
                       "either an inner pressure evolution or "
                       "an outer pressure evolution must be defined");
      }
    }
    if (this->rl == TIGHTPIPE) {
      tfel::raise_if(this->P0 < 0,
                     "PipeTest::completeInitialisation: "
                     "filling pressure not set");
      tfel::raise_if(this->T0 < 0,
                     "PipeTest::completeInitialisation: "
                     "filling temperature not set");
    }
    if (this->out) {
      unsigned short c = 7;
      this->out << "# first  column : time\n"
                   "# second column : inner radius\n"
                   "# third  column : outer radius\n"
                   "# fourth column : inner radius displacement\n"
                   "# fifth  column : outer radius displacement\n"
                   "# sixth  column : axial displacement\n";
      if ((this->rl == IMPOSEDOUTERRADIUS) || (this->rl == TIGHTPIPE)) {
        this->out << "# " << c << "th column : inner pressure\n";
        ++c;
      }
      if (this->al == IMPOSEDAXIALGROWTH) {
        this->out << "# " << c << "th column : axial force\n";
        ++c;
      }
      for (const auto& ao : this->aoutputs) {
        this->out << "# " << c << "th column : " << ao.d << '\n';
        ++c;
      }
    }
    if (this->rl == TIGHTPIPE) {
      if (this->gseq != nullptr) {
        constexpr real pi = 3.14159265358979323846;
        const auto Ri = this->mesh.inner_radius;
        const auto V = pi * Ri * Ri;
        this->n0 = this->gseq->computeNumberOfMoles(this->P0, V, this->T0);
      }
    }
  }  // end of completeInitialisation

  PipeTest::size_type PipeTest::getNumberOfNodes() const {
    tfel::raise_if(this->mesh.number_of_elements <= 0,
                   "PipeTest::getNumberOfNodes: "
                   "uninitialized number of elements");
    tfel::raise_if(this->mesh.etype == PipeMesh::DEFAULT,
                   "PipeTest::getNumberOfNodes: "
                   "element type not defined");
    // number of elements
    const auto ne = size_type(this->mesh.number_of_elements);
    if (this->mesh.etype == PipeMesh::LINEAR) {
      return ne + 1;
    } else if (this->mesh.etype == PipeMesh::QUADRATIC) {
      return 2 * ne + 1;
    } else if (this->mesh.etype == PipeMesh::CUBIC) {
      return 3 * ne + 1;
    }
    tfel::raise(
        "PipeTest::getNumberOfNodes: "
        "unknown element type");
  }

  PipeTest::size_type PipeTest::getNumberOfUnknowns() const {
    return this->getNumberOfNodes() + 1;
  }  // end of getNumberOfUnknowns

  void PipeTest::setDisplacementEpsilon(const real e) {
    tfel::raise_if(this->options.eeps > 0,
                   "PipeTest::setDisplacementEpsilon: "
                   "criterion value already set");
    tfel::raise_if(e < 100 * std::numeric_limits<real>::min(),
                   "PipeTest::setDisplacementEpsilon: "
                   "invalid criterion value");
    this->options.eeps = e;
  }  // end of setDisplacementEpsilon

  void PipeTest::setResidualEpsilon(const real s) {
    tfel::raise_if(this->options.seps > 0,
                   "PipeTest::setResidualEpsilon: the epsilon "
                   "value has already been declared");
    tfel::raise_if(s < 100 * std::numeric_limits<real>::min(),
                   "PipeTest::setResidualEpsilon: invalid value");
    this->options.seps = s;
  }

  void PipeTest::initializeCurrentState(StudyCurrentState& s) const {
    tfel::raise_if(this->b == nullptr,
                   "PipeTest::initializeCurrentState: "
                   "mechanical behaviour not set");
    tfel::raise_if(this->hypothesis ==
                       tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                   "PipeTest::initializeCurrentState: "
                   "modelling hypothesis not set");
    tfel::raise_if((this->mesh.number_of_elements == -1) ||
                       (this->mesh.inner_radius < 0) ||
                       (this->mesh.outer_radius < 0) ||
                       (this->mesh.etype == PipeMesh::DEFAULT),
                   "PipeTest::initializeCurrentState: "
                   "mesh not properly initialised");
    // unknowns
    const auto psz = this->getNumberOfUnknowns();
    s.initialize(psz);
    // initializing the state of the one and only structure
    std::fill(s.u_1.begin(), s.u_1.end(), real(0));
    s.u0 = s.u_1;
    s.u1 = s.u_1;
    auto& ss = s.getStructureCurrentState("");
    ss.setBehaviour(this->b);
    ss.setModellingHypothesis(this->hypothesis);
    if (this->mesh.etype == PipeMesh::LINEAR) {
      // each element has two integration points
      ss.istates.resize(2 * this->mesh.number_of_elements);
      PipeLinearElement::setGaussPointsPositions(ss, this->mesh);
    } else if (this->mesh.etype == PipeMesh::QUADRATIC) {
      // each element has three integration points
      ss.istates.resize(3 * this->mesh.number_of_elements);
      PipeQuadraticElement::setGaussPointsPositions(ss, this->mesh);
    } else if (this->mesh.etype == PipeMesh::CUBIC) {
      // each element has three integration points
      ss.istates.resize(4 * this->mesh.number_of_elements);
      PipeCubicElement::setGaussPointsPositions(ss, this->mesh);
    } else {
      tfel::raise(
          "PipeTest::getNumberOfUnknowns: "
          "unknown element type");
    }
    // intial values of strains
    tfel::math::vector<real> e0(this->b->getGradientsSize());
    this->b->getGradientsDefaultInitialValues(e0);
    // intial values of stresses
    for (auto& cs : ss.istates) {
      mtest::allocate(cs, this->b);
      std::copy(e0.begin(), e0.end(), cs.e0.begin());
      std::copy(e0.begin(), e0.end(), cs.e1.begin());
      std::fill(cs.e_th0.begin(), cs.e_th0.end(), real(0));
      std::fill(cs.e_th1.begin(), cs.e_th1.end(), real(0));
      std::fill(cs.s0.begin(), cs.s0.end(), real(0));
      std::fill(cs.s1.begin(), cs.s1.end(), real(0));
      std::fill(cs.iv_1.begin(), cs.iv_1.end(), real(0));
      std::fill(cs.iv0.begin(), cs.iv0.end(), real(0));
      std::fill(cs.iv1.begin(), cs.iv1.end(), real(0));

      //      if(!this->e_t0.empty()){
      // there could be an inconsistency with displacements
      // we should also be able to give a profile
      // throw(std::runtime_error("PipeTest::initializeCurrentState: "
      // 			 "initialisation of strain is not supported
      // yet")); std::copy(this->e_t0.begin(),this->e_t0.end(),s.e0.begin());
      //      }
      // setting the intial  values of stresses
      // if(!this->s_t0.empty()){
      // 	// we should be able to give a profile to do this
      // 	throw(std::runtime_error("PipeTest::initializeCurrentState: "
      // 				 "initialisation of stress is not
      // supported yet"));
      // 	// std::copy(this->s_t0.begin(),this->s_t0.end(),cs.s0.begin());
      // }

      // We should be able to give a profile here.
      // getting the initial values of internal state variables
      tfel::raise_if(
          (this->iv_t0.size() > cs.iv_1.size()) ||
              (this->iv_t0.size() > cs.iv0.size()) ||
              (this->iv_t0.size() > cs.iv1.size()),
          "PipeTest::initializeCurrentState: "
          "the number of initial values declared "
          "by the user for the internal state variables exceeds the "
          "number of internal state variables declared by the behaviour");
      std::copy(this->iv_t0.begin(), this->iv_t0.end(), cs.iv_1.begin());
      std::copy(this->iv_t0.begin(), this->iv_t0.end(), cs.iv0.begin());
      // revert the current state
      mtest::revert(ss);
      // // rotation matrix
      // cs.r = this->rm;
      // reference temperature
      const auto pev = this->evm->find("ThermalExpansionReferenceTemperature");
      if (pev != this->evm->end()) {
        const auto& ev = *(pev->second);
        tfel::raise_if(!ev.isConstant(),
                       "PipeTest::initializeCurrentState : "
                       "'ThermalExpansionReferenceTemperature' "
                       "must be a constant evolution");
        cs.Tref = ev(0);
      }
    }
  }  // end of initializeCurrentState

  std::string PipeTest::name() const { return "pipe test"; }  // end of name

  std::string PipeTest::classname() const { return "MTest"; }

  tfel::tests::TestResult PipeTest::execute() {
    return this->execute(true);
  }  // end of execute

  tfel::tests::TestResult PipeTest::execute(const bool bInit) {
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
            << "-number of period:     " << (s.period - 1) << '\n'
            << "-number of iterations: " << s.iterations << '\n'
            << "-number of sub-steps:  " << s.subSteps << '\n';
      }
    };
    if (!bInit) {
      tfel::raise_if(!this->initialisationFinished,
                     "the completeInitialisation has not been called");
    }
    StudyCurrentState state;
    SolverWorkSpace wk;
    try {
      // some checks
      tfel::raise_if(this->times.empty(),
                     "PipeTest::execute: no times defined");
      tfel::raise_if(this->times.size() < 2,
                     "PipeTest::execute: invalid number of times defined");
      // finish initialization
      if (bInit) {
        this->completeInitialisation();
      }
      // initialize current state and work space
      this->initializeCurrentState(state);
      this->initializeWorkSpace(wk);
      // integrating over the loading path
      auto pt = this->times.begin();
      auto pt2 = pt + 1;
      if (this->rl == IMPOSEDOUTERRADIUS) {
        state.addEvolution("InnerPressure",
                           std::shared_ptr<Evolution>(new LPIEvolution(
                               {*pt, *pt2}, {real(0), real(0)})));
      }
      if (this->rl == TIGHTPIPE) {
        state.addEvolution("InnerPressure",
                           std::shared_ptr<Evolution>(new LPIEvolution(
                               {*pt, *pt2}, {this->P0, this->P0})));
      }
      if (this->al == IMPOSEDAXIALGROWTH) {
        state.addEvolution("AxialForce",
                           std::shared_ptr<Evolution>(new LPIEvolution(
                               {*pt, *pt2}, {real(0), real(0)})));
      }
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
    report(nullptr, state, true);
    tfel::tests::TestResult tr;
    for (const auto& t : this->tests) {
      tr.append(t->getResults());
    }
    return tr;
  }

  void PipeTest::execute(StudyCurrentState& state,
                         SolverWorkSpace& wk,
                         const real ti,
                         const real te) const {
    using vector = std::vector<real>;
    if (this->rl == IMPOSEDOUTERRADIUS) {
      if (!state.containsEvolution("InnerPressure")) {
        // first call with this study state
        state.addEvolution("InnerPressure",
                           std::make_shared<LPIEvolution>(
                               vector{ti, te}, vector{real(0), real(0)}));
      }
    }
    if (this->rl == TIGHTPIPE) {
      if (!state.containsEvolution("InnerPressure")) {
        state.addEvolution("InnerPressure",
                           std::make_shared<LPIEvolution>(
                               vector{ti, te}, vector{this->P0, this->P0}));
      }
    }
    if (this->al == IMPOSEDAXIALFORCE) {
      if (!state.containsEvolution("AxialForce")) {
        // first call with this study state
        state.addEvolution("AxialForce",
                           std::make_shared<LPIEvolution>(
                               vector{ti, te}, vector{real(0), real(0)}));
      }
    }
    GenericSolver().execute(state, wk, *this, this->options, ti, te);
  }

  void PipeTest::initializeWorkSpace(SolverWorkSpace& wk) const {
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

  void PipeTest::prepare(StudyCurrentState& state,
                         const real t,
                         const real dt) const {
    auto& scs = state.getStructureCurrentState("");
    // number of elements
    const auto ne = size_t(this->mesh.number_of_elements);
    // loop over the elements
    for (size_type i = 0; i != ne; ++i) {
      if (this->mesh.etype == PipeMesh::LINEAR) {
        PipeLinearElement::computeStrain(scs, this->mesh, state.u0, i, false);
      } else if (this->mesh.etype == PipeMesh::QUADRATIC) {
        PipeQuadraticElement::computeStrain(scs, this->mesh, state.u0, i,
                                            false);
      } else if (this->mesh.etype == PipeMesh::CUBIC) {
        PipeCubicElement::computeStrain(scs, this->mesh, state.u0, i, false);
      } else {
        tfel::raise("PipeTest::prepare: unknown element type");
      }
    }
    SingleStructureScheme::prepare(state, t, dt);
    // in case of substepping, reset unknown forces
    if (this->al == IMPOSEDAXIALGROWTH) {
      auto& F = state.getEvolution("AxialForce");
      F.setValue(t + dt, F(t));
    }
    if ((this->rl == IMPOSEDOUTERRADIUS) || (this->rl == TIGHTPIPE)) {
      auto& Pi = state.getEvolution("InnerPressure");
      Pi.setValue(t + dt, Pi(t));
    }
  }  // end of prepare

  void PipeTest::makeLinearPrediction(StudyCurrentState& state,
                                      const real dt) const {
    if (state.period > 1) {
      const auto r = dt / state.dt_1;
      state.u1 = state.u0 + (state.u0 - state.u_1) * r;
      auto& scs = state.getStructureCurrentState("");
      for (auto& s : scs.istates) {
        s.iv1 = s.iv0 + (s.iv0 - s.iv_1) * r;
        s.s1 = s.s0 + (s.s0 - s.s_1) * r;
      }
    }
  }  // end of makeLinearPrediction

  std::pair<bool, real> PipeTest::computePredictionStiffnessAndResidual(
      StudyCurrentState&,
      tfel::math::matrix<real>&,
      tfel::math::vector<real>&,
      const real&,
      const real&,
      const StiffnessMatrixType) const {
    return {false, 1};
  }  // end of PipeTest

  std::pair<bool, real> PipeTest::computeStiffnessMatrixAndResidual(
      StudyCurrentState& state,
      tfel::math::matrix<real>& k,
      tfel::math::vector<real>& r,
      const real t,
      const real dt,
      const StiffnessMatrixType mt) const {
    using LE = PipeLinearElement;
    using QE = PipeQuadraticElement;
    using CE = PipeCubicElement;
    constexpr real pi = 3.14159265358979323846;
    // reset r and k
    std::fill(r.begin(), r.end(), real(0));
    if (mt != StiffnessMatrixType::NOSTIFFNESS) {
      std::fill(k.begin(), k.end(), real(0));
    }
    // current pipe state
    auto& scs = state.getStructureCurrentState("");
    // inner radius
    const auto Ri = this->mesh.inner_radius;
    // outer radius
    const auto Re = this->mesh.outer_radius;
    // number of elements
    const auto ne = size_type(this->mesh.number_of_elements);
    // number of nodes
    const auto n = this->getNumberOfNodes();
    // axial strain
    const auto& ezz = state.u1[n];
    /* external forces */
    if ((this->rl == IMPOSEDOUTERRADIUS) ||
        ((this->rl == IMPOSEDPRESSURE) && (this->inner_pressure != nullptr))) {
      const auto Pi = (this->rl == IMPOSEDPRESSURE)
                          ? (*(this->inner_pressure))(t + dt)
                          : state.getEvolution("InnerPressure")(t + dt);
      const auto Ri_ = (this->hpp) ? Ri : Ri + state.u1[0];
      if (this->hpp) {
        r(0) -= 2 * pi * Pi * Ri_;
      } else {
        r(0) -= 2 * pi * (1 + ezz) * Pi * Ri_;
        if (mt != StiffnessMatrixType::NOSTIFFNESS) {
          k(0, 0) -= 2 * pi * (1 + ezz) * Pi;
          k(0, n) -= 2 * pi * Pi * Ri_;
        }
      }
      if (this->al == ENDCAPEFFECT) {
        r(n) -= pi * Ri_ * Ri_ * Pi;
        if (!this->hpp) {
          if (mt != StiffnessMatrixType::NOSTIFFNESS) {
            k(n, 0) -= 2 * pi * Ri_ * Pi;
          }
        }
      }
    }
    if (this->rl == TIGHTPIPE) {
      const auto Ri_ = (this->hpp) ? Ri : Ri + state.u1[0];
      // current temperature
      setCurrentPosition((*this->evm), Ri);
      auto pT = this->evm->find("Temperature");
      tfel::raise_if(pT == this->evm->end(),
                     "PipeTest::computeStiffnessMatrixAndResidual: "
                     "temperature evolution is not defined");
      const auto T = (*(pT->second))(t + dt);
      if (this->hpp) {
        // pi capture is required for Visual Studio 2015
        const auto Pi = [this, T, Ri_] {
          if (this->gseq == nullptr) {
            return this->P0 * (T / this->T0);
          }
          return this->gseq->computePressure(pi * Ri_ * Ri_, this->n0, T);
        }();
        // for post-processing
        state.getEvolution("InnerPressure").setValue(t + dt, Pi);
        r(0) -= 2 * pi * Pi * Ri_;
        if (this->al == ENDCAPEFFECT) {
          r(n) -= pi * Ri_ * Ri_ * Pi;
        }
      } else {
        if (this->gseq == nullptr) {
          const auto Pi_ =
              (this->P0) * (T / this->T0) * Ri * Ri / ((Ri_ * Ri_) * (1 + ezz));
          const auto tmp = pi * (this->P0) * (T / this->T0) * Ri * Ri;
          // for post-processing
          state.getEvolution("InnerPressure").setValue(t + dt, Pi_);
          r(0) -= 2 * tmp / Ri_;
          //          r(0) -= 2 * pi * Pi_ * Ri_ * (1 + ezz);
          if (mt != StiffnessMatrixType::NOSTIFFNESS) {
            k(0, 0) += 2 * tmp / (Ri_ * Ri_);
          }
          if (this->al == ENDCAPEFFECT) {
            r(n) -= tmp / (1 + ezz);
            if (mt != StiffnessMatrixType::NOSTIFFNESS) {
              k(n, n) += tmp / ((1 + ezz) * (1 + ezz));
            }
          }
        } else {
          const auto V = pi * Ri_ * Ri_ * (1 + ezz);
          const auto dV_du = 2 * pi * Ri_ * (1 + ezz);
          const auto dV_dezz = pi * Ri_ * Ri_;
          const auto Pi = this->gseq->computePressure(V, this->n0, T);
          const auto K =
              this->gseq->computeIsothermalBulkModulus(V, this->n0, T);
          const auto dP_dV = -K / V;
          // for post-processing
          state.getEvolution("InnerPressure").setValue(t + dt, Pi);
          r(0) -= 2 * pi * Pi * Ri_ * (1 + ezz);
          if (mt != StiffnessMatrixType::NOSTIFFNESS) {
            k(0, 0) -= 2 * pi * (1 + ezz) * (Pi + Ri_ * dP_dV * dV_du);
            k(0, n) -= 2 * pi * Ri_ * (Pi + (1 + ezz) * dP_dV * dV_dezz);
          }
          if (this->al == ENDCAPEFFECT) {
            r(n) -= pi * Ri_ * Ri_ * Pi;
            if (mt != StiffnessMatrixType::NOSTIFFNESS) {
              k(n, n) -= pi * Ri_ * (2 * Pi + Ri_ * dP_dV * dV_du);
            }
          }
        }
      }
    }
    if (this->outer_pressure != nullptr) {
      const size_type ln = n - 1;
      const auto Pe = (*(this->outer_pressure))(t + dt);
      const auto Re_ = (this->hpp) ? Re : Re + state.u1[ln];
      if (this->hpp) {
        r(ln) += 2 * pi * Pe * Re_;
      } else {
        r(ln) += 2 * pi * Pe * (1 + ezz) * Re_;
        if (mt != StiffnessMatrixType::NOSTIFFNESS) {
          k(ln, ln) += 2 * pi * Pe * (1 + ezz);
          k(ln, n) += 2 * pi * Pe * Re_;
        }
      }
      if (this->al == ENDCAPEFFECT) {
        r(n) += pi * Re_ * Re_ * Pe;
        if (!this->hpp) {
          if (mt != StiffnessMatrixType::NOSTIFFNESS) {
            k(n, ln) += 2 * pi * Re_ * Pe;
          }
        }
      }
    }
    if (this->al == IMPOSEDAXIALFORCE) {
      r(n) -= (*(this->axial_force))(t + dt);
    }
    if (this->al == IMPOSEDAXIALGROWTH) {
      r(n) -= state.getEvolution("AxialForce")(t + dt);
    }
    // loop over the elements
    auto r_dt = real{};
    for (size_type i = 0; i != ne; ++i) {
      auto ri = std::pair<bool, real>{};
      if (this->mesh.etype == PipeMesh::LINEAR) {
        ri = LE::updateStiffnessMatrixAndInnerForces(
            k, r, scs, *(this->b), state.u1, this->mesh, dt, mt, i);
      } else if (this->mesh.etype == PipeMesh::QUADRATIC) {
        ri = QE::updateStiffnessMatrixAndInnerForces(
            k, r, scs, *(this->b), state.u1, this->mesh, dt, mt, i);
      } else if (this->mesh.etype == PipeMesh::CUBIC) {
        ri = CE::updateStiffnessMatrixAndInnerForces(
            k, r, scs, *(this->b), state.u1, this->mesh, dt, mt, i);
      } else {
        tfel::raise(
            "PipeTest::computeStiffnessMatrixAndResidual: "
            "unknown element type");
      }
      if (i == 0) {
        r_dt = ri.second;
      }
      r_dt = std::min(r_dt, ri.second);
      if (!ri.first) {
        return {false, r_dt};
      }
    }
    return {true, r_dt};
  }  // end of computeStiffnessMatrixAndResidual

  void PipeTest::checkBehaviourConsistency(
      const std::shared_ptr<Behaviour>& bp) {
    using tfel::material::MechanicalBehaviourBase;
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "PipeTest::checkBehaviourConsistency: " + m);
    };
    throw_if(bp == nullptr, "null behaviour pointer");
    if (this->hpp) {
      throw_if(bp->getBehaviourType() !=
                   MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,
               "the behaviour must be a small strain one "
               "when performing small strain analysis");
    } else {
      throw_if(bp->getBehaviourType() !=
                   MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR,
               "the behaviour must be a finite strain one "
               "when performing a finite strain analysis");
      throw_if(bp->getBehaviourKinematic() !=
                   MechanicalBehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1,
               "the behaviour must have the ETO_PK1 kinematic");
    }
  }

  void PipeTest::performSmallStrainAnalysis() {
    using tfel::material::MechanicalBehaviourBase;
    if (this->b != nullptr) {
      tfel::raise_if(this->b->getBehaviourType() !=
                         MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,
                     "PipeTest::performSmallStrainAnalysis: "
                     "the behaviour must be small strain");
    }
    this->hpp = true;
  }  // en dof performSmallStrainAnalysis

  static real PipeTest_getErrorNorm(
      const tfel::math::vector<real>& v,
      const tfel::math::vector<real>::size_type s) {
    using size_type = tfel::math::vector<real>::size_type;
    auto n = real(0);
    for (size_type i = 0; i != s; ++i) {
      n = std::max(n, std::abs(v(i)));
    }
    return n;
  }

  real PipeTest::getErrorNorm(const tfel::math::vector<real>& du) const {
    return PipeTest_getErrorNorm(du, this->getNumberOfUnknowns());
  }  // end of

  bool PipeTest::checkConvergence(const StudyCurrentState& state,
                                  const tfel::math::vector<real>& du,
                                  const tfel::math::vector<real>& r,
                                  const SolverOptions&,
                                  const unsigned int iter,
                                  const real t,
                                  const real dt) const {
    auto report = [&iter](std::ostream& os, const real ne, const real nr) {
      os << "iteration " << iter << " : " << ne << " " << nr << '\n';
    };
    constexpr real pi = 3.14159265358979323846;
    const auto Re = this->mesh.outer_radius;
    auto nu = PipeTest_getErrorNorm(du, this->getNumberOfUnknowns());
    auto nr =
        PipeTest_getErrorNorm(r, this->getNumberOfUnknowns()) / (2 * pi * Re);
    if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
      auto& log = mfront::getLogStream();
      report(log, nu, nr);
    }
    if (this->residual) {
      report(this->residual, nu, nr);
    }
    if ((!tfel::math::ieee754::isfinite(nu)) ||
        (!tfel::math::ieee754::isfinite(nr))) {
      return false;
    }
    auto ok = ((nu < Re * this->options.eeps) && (nr < this->options.seps));
    if (this->rl == IMPOSEDOUTERRADIUS) {
      // imposed external displacement
      const real iue = (*(this->orev))(t + dt) - Re;
      // computed external displacement
      const real cue = *(state.u1.rbegin() + 1);
      ok = ok && (std::abs(cue - iue) < Re * this->options.eeps);
    }
    if (this->al == IMPOSEDAXIALGROWTH) {
      // imposed axial growth
      const real iag = (*(this->axial_growth))(t + dt);
      // computed axial growth
      const real cag = *(state.u1.rbegin());
      ok = ok && (std::abs(cag - iag) < this->options.eeps);
    }
    return ok;
  }  // end of

  std::vector<std::string> PipeTest::getFailedCriteriaDiagnostic(
      const StudyCurrentState& state,
      const tfel::math::vector<real>& du,
      const tfel::math::vector<real>& r,
      const SolverOptions& o,
      const real t,
      const real dt) const {
    constexpr real pi = 3.14159265358979323846;
    auto cd = std::vector<std::string>{};
    const auto Re = this->mesh.outer_radius;
    auto nu = PipeTest_getErrorNorm(du, this->getNumberOfUnknowns());
    auto nr =
        PipeTest_getErrorNorm(r, this->getNumberOfUnknowns()) / (2 * pi * Re);
    if (nu > Re * o.eeps) {
      std::ostringstream msg;
      msg << "test on displacement (error : " << nu
          << ", criterion value : " << Re * o.eeps << ")";
      cd.push_back(msg.str());
    }
    if (nr > o.seps) {
      std::ostringstream msg;
      msg << "test on residual (error : " << nr
          << ", criterion value : " << o.seps << ")";
      cd.push_back(msg.str());
    }
    if (this->rl == IMPOSEDOUTERRADIUS) {
      // imposed external displacement
      const real iue = (*(this->orev))(t + dt) - Re;
      // computed external displacement
      const real cue = *(state.u1.rbegin() + 1);
      if (std::abs(cue - iue) > Re * o.eeps) {
        std::ostringstream msg;
        msg << "test on imposed outer radius (error : " << std::abs(cue - iue)
            << ", criterion value : " << Re * o.eeps << ")";
        cd.push_back(msg.str());
      }
    }
    if (this->al == IMPOSEDAXIALGROWTH) {
      // imposed axial growth
      const real iag = (*(this->axial_growth))(t + dt);
      // computed axial growth
      const real cag = *(state.u1.rbegin());
      if (std::abs(cag - iag) >= o.eeps) {
        std::ostringstream msg;
        msg << "test on imposed axial growth (error : " << std::abs(cag - iag)
            << ", criterion value : " << o.eeps << ")";
        cd.push_back(msg.str());
      }
    }
    return cd;
  }  // end of

  static PipeTest::size_type getNumberOfGaussPoints(const PipeMesh& m) {
    // number of elements
    const auto ne = PipeTest::size_type(m.number_of_elements);
    if (m.etype == PipeMesh::LINEAR) {
      return 2 * ne;
    } else if (m.etype == PipeMesh::QUADRATIC) {
      return 3 * ne;
    } else if (m.etype == PipeMesh::CUBIC) {
      return 4 * ne;
    }
    tfel::raise("getNumberOfGaussPoints: unknown element type");
  }  // end of getNumberOfGaussPoints

  void PipeTest::computeLoadingCorrection(StudyCurrentState& state,
                                          SolverWorkSpace& wk,
                                          const SolverOptions&,
                                          const real t,
                                          const real dt) const {
    constexpr real pi = 3.14159265358979323846;
    if ((this->rl != IMPOSEDOUTERRADIUS) && (this->al != IMPOSEDAXIALGROWTH)) {
      return;
    }
    // temporary vector
    tfel::math::vector<real> du(wk.du.size());
    // number of nodes
    const auto n = this->getNumberOfNodes();
    // axial strain
    const auto& ezz = state.u1[n];
    if (this->rl == IMPOSEDOUTERRADIUS) {
      // inner radius
      const auto Ri = this->mesh.inner_radius;
      // outer radius
      const auto Re = this->mesh.outer_radius;
      // imposed external displacement
      const real iue = (*(this->orev))(t + dt) - Re;
      // computed external displacement
      const real cue = *(state.u1.rbegin() + 1);
      std::fill(du.begin(), du.end(), real(0));
      // du first contains dFe_dp
      const auto Ri_ = (this->hpp) ? Ri : Ri + state.u1[0];
      if (this->hpp) {
        du(0) += 2 * pi * Ri_;
      } else {
        du(0) += 2 * pi * (1 + ezz) * Ri_;
      }
      if (this->al == ENDCAPEFFECT) {
        du(n) += pi * Ri_ * Ri_;
      }
      setRoundingMode();
      tfel::math::LUSolve::back_substitute(wk.K, du, wk.x, wk.p_lu);
      setRoundingMode();
      const real due_dp = *(du.rbegin() + 1);
      auto& Pi = state.getEvolution("InnerPressure");
      // current inner pressure value
      const real cv = Pi(t + dt);
      Pi.setValue(t + dt, cv - (cue - iue) / due_dp);
    }
    if (this->al == IMPOSEDAXIALGROWTH) {
      // imposed axial growth
      const real iag = (*(this->axial_growth))(t + dt);
      // computed axial growth
      // du first contains dFe_dF
      std::fill(du.begin(), du.end(), real(0));
      du(n) = 1;
      setRoundingMode();
      tfel::math::LUSolve::back_substitute(wk.K, du, wk.x, wk.p_lu);
      setRoundingMode();
      const real dezz_dF = du(n);
      auto& F = state.getEvolution("AxialForce");
      // current axial force value
      const real Fv = F(t + dt);
      F.setValue(t + dt, Fv - (ezz - iag) / dezz_dF);
    }
  }  // end of computeLoadingCorrection

  void PipeTest::postConvergence(StudyCurrentState& state,
                                 const real t,
                                 const real dt,
                                 const unsigned int p) const {
    // current pipe state
    auto& scs = state.getStructureCurrentState("");
    for (const auto& pr : this->profiles) {
      *(pr.out) << "#Time " << t + dt << '\n';
      // loop over the elements
      for (size_type i = 0; i != getNumberOfGaussPoints(this->mesh); ++i) {
        const auto& s = scs.istates[i];
        *(pr.out) << s.position;
        for (const auto& pp : pr.profiles) {
          pp->report(*(pr.out), s);
        }
        *(pr.out) << '\n';
      }
    }
    for (const auto& test : this->tests) {
      test->check(state, t, dt, p);
    }
  }  // end of postConvergence

  std::pair<real, real> PipeTest::computeMinimumAndMaximumValues(
      const StudyCurrentState& state, const std::string& n) const {
    // current pipe state
    auto& scs = state.getStructureCurrentState("");
    auto g = buildValueExtractor(*(this->b), n);
    auto vmin = std::numeric_limits<real>::max();
    auto vmax = -std::numeric_limits<real>::max();
    // loop over the elements
    for (size_type i = 0; i != getNumberOfGaussPoints(this->mesh); ++i) {
      const auto v = g(scs.istates[i]);
      vmin = std::min(vmin, v);
      vmax = std::max(vmax, v);
    }
    return {vmin, vmax};
  }  // end of computeMinimumAndMaximumValues

  void PipeTest::addOutput(const std::string& t, const std::string& n) {
    if (t == "minimum_value") {
      this->aoutputs.push_back(
          {"minimum value of '" + n + "'",
           [this, n](std::ostream& os, const StudyCurrentState& s) {
             os << this->computeMinimumValue(s, n);
           }});
    } else if (t == "maximum_value") {
      this->aoutputs.push_back(
          {"maximum value of '" + n + "'",
           [this, n](std::ostream& os, const StudyCurrentState& s) {
             os << this->computeMaximumValue(s, n);
           }});
    } else {
      tfel::raise(
          "PipeTest::addAdditionalOutput: "
          "invalid additional output type '" +
          t +
          "'.\n"
          "Valid additional output types are:\n"
          "- minimum_value\n"
          "- maximum_value\n");
    }
  }  // end of addAdditionalOutput

  real PipeTest::computeMinimumValue(const StudyCurrentState& state,
                                     const std::string& n) const {
    return this->computeMinimumAndMaximumValues(state, n).first;
  }  // end of computeMaximumValue

  real PipeTest::computeMaximumValue(const StudyCurrentState& state,
                                     const std::string& n) const {
    return this->computeMinimumAndMaximumValues(state, n).second;
  }  // end of computeMaximumValue

  void PipeTest::addProfile(const std::string& f,
                            const std::vector<std::string>& cn) {
    auto ph = PipeProfileHandler{};
    ph.out = std::make_shared<std::ofstream>(f);
    tfel::raise_if(!(*(ph.out)),
                   "PipeTest::addProfile: "
                   "can't open file '" +
                       f + "'");
    ph.out->exceptions(std::ofstream::failbit | std::ofstream::badbit);
    if (this->oprec != -1) {
      ph.out->precision(static_cast<std::streamsize>(this->oprec));
    }
    *(ph.out) << "#First column: radial position\n";
    auto i = 1;
    for (const auto& c : cn) {
      ++i;
      *(ph.out) << "#Column " << i << ": ";
      if (c == "SRR") {
        *(ph.out) << "radial stress \n";
        ph.profiles.emplace_back(new PipeStressProfile(0u));
      } else if (c == "STT") {
        *(ph.out) << "hoop stress \n";
        ph.profiles.emplace_back(new PipeStressProfile(2u));
      } else if (c == "SZZ") {
        *(ph.out) << "axial stress \n";
        ph.profiles.emplace_back(new PipeStressProfile(1u));
      } else if (c == "ERR") {
        *(ph.out) << "radial strain \n";
        ph.profiles.emplace_back(new PipeStrainProfile(0u));
      } else if (c == "ETT") {
        *(ph.out) << "hoop strain \n";
        ph.profiles.emplace_back(new PipeStrainProfile(2u));
      } else if (c == "EZZ") {
        *(ph.out) << "axial strain \n";
        ph.profiles.emplace_back(new PipeStrainProfile(1u));
      } else {
        const auto piv =
            std::find(this->ivfullnames.begin(), this->ivfullnames.end(), c);
        tfel::raise_if(piv == this->ivfullnames.end(),
                       "PipeTest::addProfile: "
                       "no internal state variable named '" +
                           c + "'");
        *(ph.out) << c << " internal state variable\n";
        auto pos = size_type(piv - this->ivfullnames.begin());
        ph.profiles.emplace_back(new PipeInternalStateVariableProfile(pos));
      }
    }
    this->profiles.push_back(ph);
  }  // end of addProfile

  void PipeTest::setModellingHypothesis(const std::string& h) {
    tfel::raise_if(h != "AxisymmetricalGeneralisedPlaneStrain",
                   "PipeTest::setModellingHypothesis: "
                   "invalid modelling hypothesis '" +
                       h + "'");
    tfel::raise_if(this->hypothesis != ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                   "PipeTest::setModellingHypothesis: "
                   "modelling hypothesis already defined");
    this->hypothesis = ModellingHypothesis::fromString(h);
  }  // end of setModellingHypothesis

  void PipeTest::setDefaultModellingHypothesis() {
    tfel::raise_if(this->hypothesis != ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                   "PipeTest::setDefaultModellingHypothesis: "
                   "modelling hypothesis already defined");
    this->hypothesis =
        ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN;
  }  // end of setDefaultModellingHypothesis

  void PipeTest::setAxialLoading(const PipeTest::AxialLoading ph) {
    tfel::raise_if(this->al != DEFAULTAXIALLOADING,
                   "PipeTest::setAxialLoading: "
                   "axial loading already defined");
    this->al = ph;
  }  // end of setAxialLoading

  PipeTest::AxialLoading PipeTest::getAxialLoading() const {
    tfel::raise_if(this->al == DEFAULTAXIALLOADING,
                   "PipeTest::setAxialLoading: "
                   "axial loading undefined");
    return this->al;
  }  // end of setAxialLoading

  void PipeTest::setRadialLoading(const PipeTest::RadialLoading t) {
    tfel::raise_if(this->rl != DEFAULTLOADINGTYPE,
                   "PipeTest::setRadialLoading: "
                   "loading type already defined");
    this->rl = t;
  }  // end of setRadialLoading

  PipeTest::RadialLoading PipeTest::getRadialLoading() const {
    tfel::raise_if(this->rl == DEFAULTLOADINGTYPE,
                   "PipeTest::setRadialLoading: "
                   "radial loading undefined");
    return this->rl;
  }  // end of getRadialLoading

  void PipeTest::setElementType(const PipeMesh::ElementType ph) {
    tfel::raise_if(this->mesh.etype != PipeMesh::DEFAULT,
                   "PipeTest::setElementType: "
                   "element type already defined");
    this->mesh.etype = ph;
  }  // end of setElementType

  void PipeTest::setInnerPressureEvolution(std::shared_ptr<Evolution> p) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "PipeTest::setInnerPressureEvolution: " + m);
    };
    throw_if(this->rl == IMPOSEDOUTERRADIUS,
             "inner pressure evolution can't be set");
    if (this->rl == DEFAULTLOADINGTYPE) {
      this->setRadialLoading(IMPOSEDPRESSURE);
    }
    throw_if(this->inner_pressure != nullptr,
             "inner pressure evolution already set");
    this->inner_pressure = p;
  }  // end of setInnerPressureEvolution

  void PipeTest::setOuterPressureEvolution(std::shared_ptr<Evolution> p) {
    tfel::raise_if(this->outer_pressure != nullptr,
                   "PipeTest::setOuterPressureEvolution: "
                   "outer pressure evolution already set");
    this->outer_pressure = p;
  }  // end of setOuterPressureEvolution

  void PipeTest::setAxialForceEvolution(std::shared_ptr<Evolution> f) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "PipeTest::setAxialForceEvolution: " + m);
    };
    throw_if(this->al != IMPOSEDAXIALFORCE,
             "axial force evolution can be set "
             "only if the pipe modelling hypothesis is 'ImposedAxialForce'");
    throw_if(this->axial_force != nullptr, "axial force evolution already set");
    this->axial_force = f;
  }  // end of setAxialForceEvolution

  void PipeTest::setAxialGrowthEvolution(std::shared_ptr<Evolution> f) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "PipeTest::setOuterRadiusEvolution: " + m);
    };
    throw_if(this->al != IMPOSEDAXIALGROWTH,
             "axial growth evolution can be set "
             "only if the pipe modelling hypothesis is 'ImposedAxialGrowth'");
    throw_if(this->axial_growth != nullptr,
             "axial growth evolution already set");
    this->axial_growth = f;
  }  // end of setAxialGrowthEvolution

  void PipeTest::setOuterRadiusEvolution(std::shared_ptr<Evolution> e) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "PipeTest::setOuterRadiusEvolution: " + m);
    };
    throw_if(this->rl != IMPOSEDOUTERRADIUS,
             "outer radius evolution can be set "
             "only if the loading type is 'ImposedOuterRadius'");
    throw_if(this->orev != nullptr, "axial force evolution already set");
    this->orev = e;
  }  // end of setOuterRadiusEvolution

  void PipeTest::setGasEquationOfState(const std::string& e) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "PipeTest::setGasEquationOfState: " + m);
    };
    throw_if(this->rl != TIGHTPIPE,
             "the gas equation of state is only meaningfull "
             "if the radial loading type is 'TightPipe'");
    throw_if(this->gseq != nullptr, "gas equation of state already defined");
    this->gseq = std::unique_ptr<GasEquationOfState>(
        new GasEquationOfState(e, *(this->evm)));
  }  // end of setGasEquationOfState

  void PipeTest::setFillingPressure(const real p) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "PipeTest::setFillingPressure: " + m);
    };
    throw_if(this->rl != TIGHTPIPE,
             "filling pressure can be set "
             "only if the loading type is 'TightPipe'");
    throw_if(this->P0 >= 0, "filling pressure already set ");
    throw_if(p < 0,
             "invalid  filling pressure value ('" + std::to_string(p) + "') ");
    this->P0 = p;
  }  // end of setFillingPressure

  void PipeTest::setFillingTemperature(const real T) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "PipeTest::setFillingTemperature: " + m);
    };
    throw_if(this->rl != TIGHTPIPE,
             "filling temperature can be set "
             "only if the loading type is 'TightPipe'");
    throw_if(this->T0 >= 0, "filling temperature already set ");
    throw_if(T < 0, "invalid  filling temperature value ('" +
                        std::to_string(T) + "') ");
    this->T0 = T;
  }  // end of setFillingTemperature

  void PipeTest::printOutput(const real t,
                             const StudyCurrentState& state,
                             const bool o) const {
    if ((!o) && (this->output_frequency == USERDEFINEDTIMES)) {
      return;
    }
    if (this->out) {
      const auto& u1 = state.u1;
      const auto n = this->getNumberOfNodes();
      // inner radius
      const auto Ri = this->mesh.inner_radius;
      // outer radius
      const auto Re = this->mesh.outer_radius;
      this->out << t << " " << Ri + u1[0] << " " << Re + u1[n - 1] << " "
                << u1[0] << " " << u1[n - 1] << " " << u1[n];
      if ((this->rl == IMPOSEDOUTERRADIUS) || (this->rl == TIGHTPIPE)) {
        this->out << " " << state.getEvolution("InnerPressure")(t);
      }
      if (this->al == IMPOSEDAXIALGROWTH) {
        this->out << " " << state.getEvolution("AxialForce")(t);
      }
      for (const auto& ao : this->aoutputs) {
        this->out << " ";
        ao.f(out, state);
      }
      this->out << '\n';
    }
  }  // end of printOutput

  PipeTest::~PipeTest() = default;

}  // end of namespace mtest
