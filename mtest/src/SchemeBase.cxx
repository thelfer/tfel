/*!
 * \file   mtest/src/SchemeBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   02 oct. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <algorithm>

#include "TFEL/Raise.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/AccelerationAlgorithmFactory.hxx"
#include "MTest/CastemAccelerationAlgorithm.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/SchemeBase.hxx"

namespace mtest {

  SchemeBase::SchemeBase() : evm(new EvolutionManager()) {
    // declare time variable
    this->declareVariable("t", true);
  }  // end of SchemeBase::SchemeBase

  void SchemeBase::addEvolution(const std::string& n,
                                const EvolutionPtr p,
                                const bool b1,
                                const bool b2) {
    if (b1) {
      this->declareVariable(n, b1);
    } else {
      tfel::raise_if(std::find(this->vnames.begin(), this->vnames.end(), n) ==
                         this->vnames.end(),
                     "SchemeBase::addEvolution: "
                     "variable '" +
                         n + "' is not defined");
    }
    if (b2) {
      tfel::raise_if(this->evm->find(n) != this->evm->end(),
                     "SchemeBase::addEvolution: "
                     "evolution '" +
                         n + "' already defined");
    }
    (*(this->evm))[n] = p;
  }

  void SchemeBase::setEvolutionValue(const std::string& n,
                                     const real t,
                                     const real v) {
    const auto pev = this->evm->find(n);
    tfel::raise_if(
        pev == this->evm->end(),
        "SchemeBase::setEvolutionValue : no evolution '" + n + "' declared");
    pev->second->setValue(t, v);
  }  // end of SchemeBase::setEvolutionValue

  const EvolutionManager& SchemeBase::getEvolutions() const {
    return *(this->evm);
  }  // end of SchemeBase::getEvolutions() const

  void SchemeBase::setTimes(const std::vector<real>& t) {
    tfel::raise_if(!this->times.empty(),
                   "SchemeBase::setTimes: "
                   "times already defined");
    this->times = t;
  }  // end of SchemeBase::setTimes

  void SchemeBase::setMaximumNumberOfIterations(const unsigned int i) {
    tfel::raise_if(this->options.iterMax != -1,
                   "SchemeBase::setMaximumNumberOfIterations: "
                   "the maximum number of iterations "
                   "has already been declared");
    tfel::raise_if(i == 0,
                   "SchemeBase::setMaximumNumberOfIterations: "
                   "invalid number of iterations");
    this->options.iterMax = static_cast<int>(i);
  }

  void SchemeBase::setMaximumNumberOfSubSteps(const unsigned int i) {
    tfel::raise_if(this->options.mSubSteps != -1,
                   "SchemeBase::setMaximumNumberOfSubSteps: "
                   "the maximum number of sub steps "
                   "has already been declared");
    tfel::raise_if(i == 0,
                   "SchemeBase::setMaximumNumberOfSubSteps: "
                   "invalid number of sub steps");
    this->options.mSubSteps = static_cast<int>(i);
  }

  void SchemeBase::setOutputFrequency(const OutputFrequency o) {
    this->output_frequency = o;
  }

  void SchemeBase::setDynamicTimeStepScaling(const bool b) {
    this->options.dynamic_time_step_scaling = b;
  }

  void SchemeBase::setMaximalTimeStep(const real v) {
    TFEL_CONSTEXPR const auto emin = 100 * std::numeric_limits<real>::min();
    TFEL_CONSTEXPR const auto eps = 100 * std::numeric_limits<real>::epsilon();
    tfel::raise_if(this->options.maximal_time_step > 0,
                   "SchemeBase::setMaximalTimeStep: "
                   "the maximal time step "
                   "has already been declared");
    if (this->options.minimal_time_step > 0) {
      tfel::raise_if(v <= this->options.minimal_time_step,
                     "SchemeBase::setMaximalTimeStep: "
                     "the specified maximal time step "
                     "is lower than the minimal time step");
      tfel::raise_if((std::abs(v - this->options.minimal_time_step) < emin) ||
                         (std::abs(v - this->options.minimal_time_step) < eps),
                     "SchemeBase::setMaximalTimeStep: "
                     "the minimal and the maximal time step are too close");
    }
    tfel::raise_if(v <= emin,
                   "SchemeBase::setMaximalTimeStep: "
                   "the maximal time step is either negative or too small");
    this->options.maximal_time_step = v;
  }

  void SchemeBase::setMinimalTimeStep(const real v) {
    TFEL_CONSTEXPR const auto emin = 100 * std::numeric_limits<real>::min();
    TFEL_CONSTEXPR const auto eps = 100 * std::numeric_limits<real>::epsilon();
    tfel::raise_if(this->options.minimal_time_step > 0,
                   "SchemeBase::setMinimalTimeStep: "
                   "the minimal time step "
                   "has already been declared");
    if (this->options.maximal_time_step > 0) {
      tfel::raise_if(v >= this->options.maximal_time_step,
                     "SchemeBase::setMinimalTimeStep: "
                     "the specified minimal time step "
                     "is greater than the maximal time step");
      const auto d = std::abs(this->options.maximal_time_step - v);
      tfel::raise_if((d < emin) || (d < eps * this->options.maximal_time_step),
                     "SchemeBase::setMinimalTimeStep: "
                     "the maximal and the minimal time step are too close");
    }
    tfel::raise_if(v <= emin,
                   "SchemeBase::setMinimalTimeStep: "
                   "the specified minimal time step is "
                   "either negative or too small");
    this->options.minimal_time_step = v;
  }

  void SchemeBase::setMinimalTimeStepScalingFactor(const real v) {
    tfel::raise_if(this->options.minimal_time_step_scaling_factor > 0,
                   "SchemeBase::setMinimalTimeStepScalingFactor: "
                   "the minimal time step scaling factor "
                   "has already been declared");
    tfel::raise_if(v >= 1,
                   "SchemeBase::setMinimalTimeStepScalingFactor: "
                   "the minimal time step scaling factor "
                   "is greater than one ");
    tfel::raise_if(v <= 100 * std::numeric_limits<real>::epsilon(),
                   "SchemeBase::setMinimalTimeStepScalingFactor: "
                   "the minimal time step scaling is either "
                   "negative or too small");
    this->options.minimal_time_step_scaling_factor = v;
  }

  void SchemeBase::setMaximalTimeStepScalingFactor(const real v) {
    tfel::raise_if(this->options.maximal_time_step_scaling_factor > 0,
                   "SchemeBase::setMaximalTimeStepScalingFactor: "
                   "the maximal time step scaling factor "
                   "has already been declared");
    tfel::raise_if(v < 1,
                   "SchemeBase::setMaximalTimeStepScalingFactor: "
                   "the maximal time step scaling factor "
                   "is lower than one ");
    this->options.maximal_time_step_scaling_factor = v;
  }

  void SchemeBase::setDescription(const std::string& d) {
    tfel::raise_if(!this->description.empty(),
                   "SchemeBase::setDescription: "
                   "description already set.");
    this->description = d;
  }  // end of SchemeBase::setDescription

  void SchemeBase::setAuthor(const std::string& a) {
    tfel::raise_if(!this->author.empty(),
                   "SchemeBase::setAuthor: "
                   "author already set.");
    this->author = a;
  }  // end of SchemeBase::setAuthor

  void SchemeBase::setDate(const std::string& d) {
    tfel::raise_if(!this->date.empty(),
                   "SchemeBase::setDate: "
                   "date already set.");
    this->date = d;
  }  // end of SchemeBase::setDate

  void SchemeBase::setModellingHypothesis(const std::string& h) {
    tfel::raise_if(this->hypothesis != ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                   "SchemeBase::setModellingHypothesis: "
                   "the modelling hypothesis is already defined");
    this->hypothesis = ModellingHypothesis::fromString(h);
  }  // end of SchemeBase::setModellingHypothesis

  tfel::material::ModellingHypothesis::Hypothesis
  SchemeBase::getModellingHypothesis() const {
    tfel::raise_if(this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                   "SchemeBase::getModellingHypothesis: "
                   "the modelling hypothesis is not defined");
    return this->hypothesis;
  }

  unsigned short SchemeBase::getDimension() const {
    tfel::raise_if(this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                   "SchemeBase::getDimension: "
                   "the modelling hypothesis is "
                   "not defined");
    return tfel::material::getSpaceDimension(this->hypothesis);
  }

  void SchemeBase::setAccelerationAlgorithm(const std::string& a) {
    tfel::raise_if(this->options.aa != nullptr,
                   "SchemeBase::setAccelerationAlgorithm: "
                   "acceleration algorithm already set");
    auto& f = AccelerationAlgorithmFactory::getAccelerationAlgorithmFactory();
    this->options.aa = f.getAlgorithm(a);
  }

  void SchemeBase::setAccelerationAlgorithmParameter(const std::string& p,
                                                     const std::string& v) {
    tfel::raise_if(this->options.aa == nullptr,
                   "SchemeBase::setAccelerationAlgorithmParameter: "
                   "no acceleration algorithm set");
    this->options.aa->setParameter(p, v);
  }

  void SchemeBase::setPredictionPolicy(const PredictionPolicy p) {
    tfel::raise_if(
        this->options.ppolicy != PredictionPolicy::UNSPECIFIEDPREDICTIONPOLICY,
        "SchemeBase::setPredictionPolicy: "
        "prediction policy already declared");
    this->options.ppolicy = p;
  }  // end of SchemeBase::setPredictionPolicy

  void SchemeBase::setStiffnessMatrixType(const StiffnessMatrixType k) {
    tfel::raise_if(this->options.ktype !=
                       StiffnessMatrixType::UNSPECIFIEDSTIFFNESSMATRIXTYPE,
                   "SchemeBase::setStiffnessMatrixType: "
                   "stiffness matrix type already specificed");
    this->options.ktype = k;
  }

  void SchemeBase::setUseCastemAccelerationAlgorithm(const bool ucaa) {
    if (ucaa) {
      tfel::raise_if(this->options.aa != nullptr,
                     "SchemeBase::setUseCastemAccelerationAlgorithm: "
                     "an algorithm was already set");
      this->options.aa = std::shared_ptr<AccelerationAlgorithm>(
          new CastemAccelerationAlgorithm);
    }
    this->options.useCastemAcceleration = ucaa;
  }

  void SchemeBase::setCastemAccelerationTrigger(const int i) {
    tfel::raise_if(!this->options.useCastemAcceleration,
                   "SchemeBase::setCastemAccelerationTrigger: "
                   "the castem acceleration algorithm has "
                   "not been set using the "
                   "@UseCast3mAccelerationAlgorithm keyword. "
                   "If the Cast3M acceleration algorithm "
                   "was specified using the "
                   "@AccelerationAlgorithm keyword, please "
                   "use the @AccelerationAlgorithmParameter "
                   "keyword to specify the acceleration trigger.");
    tfel::raise_if(this->options.aa == nullptr,
                   "SchemeBase::setCastemAccelerationTrigger: "
                   "internal error");
    std::ostringstream nb;
    nb << i;
    this->options.aa->setParameter("AccelerationTrigger", nb.str());
  }

  void SchemeBase::setCastemAccelerationPeriod(const int p) {
    tfel::raise_if(!this->options.useCastemAcceleration,
                   "SchemeBase::setCastemAccelerationPeriod: "
                   "the castem acceleration algorithm has not "
                   "been set using the "
                   "@UseCast3mAccelerationAlgorithm keyword. "
                   "If the Cast3M acceleration algorithm was "
                   "specified using the @AccelerationAlgorithm "
                   "keyword, please use the "
                   "@AccelerationAlgorithmParameter keyword to "
                   "specify the acceleration period.");
    tfel::raise_if(this->options.aa == nullptr,
                   "SchemeBase::setCastemAccelerationPeriod: "
                   "internal error");
    std::ostringstream nb;
    nb << p;
    this->options.aa->setParameter("AccelerationPeriod", nb.str());
  }

  void SchemeBase::setStiffnessUpdatingPolicy(const StiffnessUpdatingPolicy p) {
    tfel::raise_if(
        this->options.ks !=
            StiffnessUpdatingPolicy::UNSPECIFIEDSTIFFNESSUPDATINGPOLICY,
        "SchemeBase::setStiffnessUpdatePolicy: "
        "stiffness matrix type already specificed");
    this->options.ks = p;
  }  // end of SchemeBase::setStiffnessUpdatingPolicy

  void SchemeBase::resetOutputFile() {
    // output file
    if (!this->output.empty()) {
      this->out.close();
      this->out.open(this->output.c_str());
      tfel::raise_if(!this->out,
                     "SchemeBase::resetOutputFile: "
                     "can't open file '" +
                         this->output + "'");
      this->out.exceptions(std::ofstream::failbit | std::ofstream::badbit);
      if (this->oprec != -1) {
        this->out.precision(static_cast<std::streamsize>(this->oprec));
      }
    }
    // residual file
    if (!this->residualFileName.empty()) {
      this->residual.close();
      this->residual.open(this->residualFileName.c_str());
      tfel::raise_if(!this->residual,
                     "SchemeBase::resetOutputFile: "
                     "unable to open file '" +
                         this->residualFileName + "'");
      this->residual.exceptions(std::ofstream::failbit | std::ofstream::badbit);
      if (this->rprec != -1) {
        this->residual.precision(static_cast<std::streamsize>(this->rprec));
      } else {
        if (this->oprec != -1) {
          this->residual.precision(static_cast<std::streamsize>(this->oprec));
        }
      }
    }
  }  // end of resetOutputFile

  void SchemeBase::completeInitialisation() {
    tfel::raise_if(this->initialisationFinished,
                   "SchemeBase::completeInitialisation : "
                   "object already initialised");
    // initialisation is complete
    this->initialisationFinished = true;
    try {
      if (this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        this->setDefaultModellingHypothesis();
      }
      // numerical parameters
      if (this->options.mSubSteps == -1) {
        this->options.mSubSteps = 10;
      }
      if (this->options.iterMax == -1) {
        this->options.iterMax = 100;
      }
      if (this->options.aa != nullptr) {
        this->options.aa->initialize(this->getNumberOfUnknowns());
      }
      // prediction policy
      if (this->options.ppolicy ==
          PredictionPolicy::UNSPECIFIEDPREDICTIONPOLICY) {
        this->options.ppolicy = PredictionPolicy::NOPREDICTION;
      }
      // stiffness matrix type
      if (this->options.ktype ==
          StiffnessMatrixType::UNSPECIFIEDSTIFFNESSMATRIXTYPE) {
        this->options.ktype = this->getDefaultStiffnessMatrixType();
      }
      // options selected
      if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
        auto& log = mfront::getLogStream();
        if (this->options.aa != nullptr) {
          log << "** " << this->options.aa->getName()
              << " acceleration algorithm selected\n";
        }
        if (this->options.ppolicy == PredictionPolicy::LINEARPREDICTION) {
          log << "** using linear prediction\n";
        } else if (this->options.ppolicy ==
                   PredictionPolicy::ELASTICPREDICTION) {
          log << "** prediction using elastic stiffness\n";
        } else if (this->options.ppolicy ==
                   PredictionPolicy::ELASTICPREDICTIONFROMMATERIALPROPERTIES) {
          log << "** prediction using elastic stiffness computed from material "
                 "properties\n";
        } else if (this->options.ppolicy ==
                   PredictionPolicy::TANGENTOPERATORPREDICTION) {
          log << "** prediction using tangent operator\n";
        } else {
          tfel::raise_if(
              this->options.ppolicy != PredictionPolicy::NOPREDICTION,
              "MTest::completeInitialisation : "
              "internal error, unsupported "
              "prediction policy");
          log << "** no prediction\n";
        }
      }
      this->resetOutputFile();
    } catch (...) {
      this->initialisationFinished = false;
      throw;
    }
  }  // end of SchemeBase::completeInitialisation

  void SchemeBase::declareVariable(const std::string& v, const bool check) {
    if (std::find(this->vnames.begin(), this->vnames.end(), v) !=
        this->vnames.end()) {
      tfel::raise_if(check,
                     "SchemeBase::declareVariable: "
                     "variable '" +
                         v + "' already declared");
    } else {
      this->vnames.push_back(v);
    }
  }

  void SchemeBase::declareVariables(const std::vector<std::string>& v,
                                    const bool check) {
    for (const auto& n : v) {
      this->declareVariable(n, check);
    }
  }

  void SchemeBase::setOutputFileName(const std::string& o) {
    tfel::raise_if(!this->output.empty(),
                   "SchemeBase::setOutputFileName: "
                   "output file name already defined");
    this->output = o;
  }

  bool SchemeBase::isOutputFileNameDefined() const {
    return !this->output.empty();
  }

  void SchemeBase::setOutputFilePrecision(const unsigned int p) {
    tfel::raise_if(this->oprec != -1,
                   "SchemeBase::setOutputFileName: "
                   "output file precision already defined");
    this->oprec = static_cast<int>(p);
  }

  void SchemeBase::setResidualFileName(const std::string& o) {
    tfel::raise_if(!this->residualFileName.empty(),
                   "SchemeBase::setResidualFileName : "
                   "residual file name already defined");
    this->residualFileName = o;
  }

  bool SchemeBase::isResidualFileNameDefined() const {
    return !this->residualFileName.empty();
  }

  void SchemeBase::setResidualFilePrecision(const unsigned int p) {
    tfel::raise_if(this->rprec != -1,
                   "SchemeBase::setResidualFileName: "
                   "residual file precision already defined");
    this->rprec = static_cast<int>(p);
  }

  void SchemeBase::setXMLOutputFileName(const std::string& o) {
    tfel::raise_if(!this->xmlFileName.empty(),
                   "SchemeBase::setXMLOutputFileName : "
                   "XMLOutput file name already defined");
    this->xmlFileName = o;
  }

  bool SchemeBase::isXMLOutputFileNameDefined() const {
    return !this->xmlFileName.empty();
  }

  std::string SchemeBase::getXMLOutputFileName() const {
    tfel::raise_if(this->xmlFileName.empty(),
                   "SchemeBase::getXMLOutputFileName : "
                   "XML output file name not defined");
    return this->xmlFileName;
  }

  SchemeBase::~SchemeBase() = default;

}  // end of namespace mtest
