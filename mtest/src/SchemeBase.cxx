/*!
 * \file   mtest/src/SchemeBase.cxx
 * \brief
 * \author THOMAS HELFER
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
      if (std::find(this->vnames.begin(), this->vnames.end(), n) ==
          this->vnames.end()) {
        throw(
            std::runtime_error("SchemeBase::addEvolution: "
                               "variable '" +
                               n + "' is not defined"));
      }
    }
    if (b2) {
      if (this->evm->find(n) != this->evm->end()) {
        throw(
            std::runtime_error("SchemeBase::addEvolution: "
                               "evolution '" +
                               n + "' already defined"));
      }
    }
    (*(this->evm))[n] = p;
  }

  void SchemeBase::setEvolutionValue(const std::string& n,
                                     const real t,
                                     const real v) {
    const auto pev = this->evm->find(n);
    if (pev == this->evm->end()) {
      throw(std::runtime_error(
          "SchemeBase::setEvolutionValue : no evolution '" + n + "' declared"));
    }
    pev->second->setValue(t, v);
  }  // end of SchemeBase::setEvolutionValue

  const EvolutionManager& SchemeBase::getEvolutions() const {
    return *(this->evm);
  }  // end of SchemeBase::getEvolutions() const

  void SchemeBase::setTimes(const std::vector<real>& t) {
    if (!this->times.empty()) {
      throw(
          std::runtime_error("SchemeBase::setTimes: "
                             "times already defined"));
    }
    this->times = t;
  }  // end of SchemeBase::setTimes

  void SchemeBase::setMaximumNumberOfIterations(const unsigned int i) {
    if (this->options.iterMax != -1) {
      throw(
          std::runtime_error("SchemeBase::setMaximumNumberOfIterations: "
                             "the maximum number of iterations "
                             "has already been declared"));
    }
    if (i == 0) {
      throw(
          std::runtime_error("SchemeBase::setMaximumNumberOfIterations: "
                             "invalid number of iterations"));
    }
    this->options.iterMax = static_cast<int>(i);
  }

  void SchemeBase::setMaximumNumberOfSubSteps(const unsigned int i) {
    if (this->options.mSubSteps != -1) {
      throw(
          std::runtime_error("SchemeBase::setMaximumNumberOfSubSteps: "
                             "the maximum number of sub steps "
                             "has already been declared"));
    }
    if (i == 0) {
      throw(
          std::runtime_error("SchemeBase::setMaximumNumberOfSubSteps: "
                             "invalid number of sub steps"));
    }
    this->options.mSubSteps = static_cast<int>(i);
  }

  void SchemeBase::setOutputFrequency(const OutputFrequency o) {
    this->output_frequency = o;
  }

  void SchemeBase::setDynamicTimeStepScaling(const bool b) {
    this->options.dynamic_time_step_scaling = b;
  }

  void SchemeBase::setMaximalTimeStep(const real v) {
    if (this->options.maximal_time_step > 0) {
      throw(
          std::runtime_error("SchemeBase::setMaximalTimeStep: "
                             "the maximal time step "
                             "has already been declared"));
    }
    if (this->options.minimal_time_step > 0) {
      if (v <= this->options.minimal_time_step) {
        throw(
            std::runtime_error("SchemeBase::setMaximalTimeStep: "
                               "the specified maximal time step "
                               "is lower than the minimal time step"));
      }
      if ((std::abs(v - this->options.minimal_time_step) <
           100 * std::numeric_limits<real>::min()) ||
          (std::abs(v - this->options.minimal_time_step) <
           100 * v * std::numeric_limits<real>::epsilon())) {
        throw(std::runtime_error(
            "SchemeBase::setMaximalTimeStep: "
            "the minimal and the maximal time step are too close"));
      }
    }
    if (v <= 100 * std::numeric_limits<real>::min()) {
      throw(std::runtime_error(
          "SchemeBase::setMaximalTimeStep: "
          "the maximal time step is either negative or too small"));
    }
    this->options.maximal_time_step = v;
  }

  void SchemeBase::setMinimalTimeStep(const real v) {
    if (this->options.minimal_time_step > 0) {
      throw(
          std::runtime_error("SchemeBase::setMinimalTimeStep: "
                             "the minimal time step "
                             "has already been declared"));
    }
    if (this->options.maximal_time_step > 0) {
      if (v >= this->options.maximal_time_step) {
        throw(
            std::runtime_error("SchemeBase::setMinimalTimeStep: "
                               "the specified minimal time step "
                               "is greater than the maximal time step"));
      }
      if ((std::abs(this->options.maximal_time_step - v) <
           100 * std::numeric_limits<real>::min()) ||
          (std::abs(this->options.maximal_time_step - v) <
           100 * (this->options.maximal_time_step *
                  std::numeric_limits<real>::epsilon()))) {
        throw(std::runtime_error(
            "SchemeBase::setMinimalTimeStep: "
            "the maximal and the minimal time step are too close"));
      }
    }
    if (v <= 100 * std::numeric_limits<real>::min()) {
      throw(
          std::runtime_error("SchemeBase::setMinimalTimeStep: "
                             "the specified minimal time step is "
                             "either negative or too small"));
    }
    this->options.minimal_time_step = v;
  }

  void SchemeBase::setMinimalTimeStepScalingFactor(const real v) {
    if (this->options.minimal_time_step_scaling_factor > 0) {
      throw(
          std::runtime_error("SchemeBase::setMinimalTimeStepScalingFactor: "
                             "the minimal time step scaling factor "
                             "has already been declared"));
    }
    if (v >= 1) {
      throw(
          std::runtime_error("SchemeBase::setMinimalTimeStepScalingFactor: "
                             "the minimal time step scaling factor "
                             "is greater than one "));
    }
    if (v <= 100 * std::numeric_limits<real>::epsilon()) {
      throw(
          std::runtime_error("SchemeBase::setMinimalTimeStepScalingFactor: "
                             "the minimal time step scaling is either "
                             "negative or too small"));
    }
    this->options.minimal_time_step_scaling_factor = v;
  }

  void SchemeBase::setMaximalTimeStepScalingFactor(const real v) {
    if (this->options.maximal_time_step_scaling_factor > 0) {
      throw(
          std::runtime_error("SchemeBase::setMaximalTimeStepScalingFactor: "
                             "the maximal time step scaling factor "
                             "has already been declared"));
    }
    if (v < 1) {
      throw(
          std::runtime_error("SchemeBase::setMaximalTimeStepScalingFactor: "
                             "the maximal time step scaling factor "
                             "is lower than one "));
    }
    this->options.maximal_time_step_scaling_factor = v;
  }

  void SchemeBase::setDescription(const std::string& d) {
    if (!this->description.empty()) {
      throw(
          std::runtime_error("SchemeBase::setDescription: "
                             "description already set."));
    }
    this->description = d;
  }  // end of SchemeBase::setDescription

  void SchemeBase::setAuthor(const std::string& a) {
    if (!this->author.empty()) {
      throw(
          std::runtime_error("SchemeBase::setAuthor: "
                             "author already set."));
    }
    this->author = a;
  }  // end of SchemeBase::setAuthor

  void SchemeBase::setDate(const std::string& d) {
    if (!this->date.empty()) {
      throw(
          std::runtime_error("SchemeBase::setDate: "
                             "date already set."));
    }
    this->date = d;
  }  // end of SchemeBase::setDate

  void SchemeBase::setModellingHypothesis(const std::string& h) {
    if (this->hypothesis != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      throw(
          std::runtime_error("SchemeBase::setModellingHypothesis: "
                             "the modelling hypothesis is already defined"));
    }
    this->hypothesis = ModellingHypothesis::fromString(h);
  }  // end of SchemeBase::setModellingHypothesis

  tfel::material::ModellingHypothesis::Hypothesis
  SchemeBase::getModellingHypothesis() const {
    if (this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      throw(
          std::runtime_error("SchemeBase::getModellingHypothesis: "
                             "the modelling hypothesis is not defined"));
    }
    return this->hypothesis;
  }

  unsigned short SchemeBase::getDimension() const {
    if (this->hypothesis == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      throw(
          std::runtime_error("SchemeBase::getDimension: "
                             "the modelling hypothesis is "
                             "not defined"));
    }
    return tfel::material::getSpaceDimension(this->hypothesis);
  }

  void SchemeBase::setAccelerationAlgorithm(const std::string& a) {
    if (this->options.aa.get() != nullptr) {
      throw(
          std::runtime_error("SchemeBase::setAccelerationAlgorithm: "
                             "acceleration algorithm already set"));
    }
    auto& f = AccelerationAlgorithmFactory::getAccelerationAlgorithmFactory();
    this->options.aa = f.getAlgorithm(a);
  }

  void SchemeBase::setAccelerationAlgorithmParameter(const std::string& p,
                                                     const std::string& v) {
    if (this->options.aa.get() == nullptr) {
      throw(
          std::runtime_error("SchemeBase::setAccelerationAlgorithmParameter: "
                             "no acceleration algorithm set"));
    }
    this->options.aa->setParameter(p, v);
  }

  void SchemeBase::setPredictionPolicy(const PredictionPolicy p) {
    if (this->options.ppolicy !=
        PredictionPolicy::UNSPECIFIEDPREDICTIONPOLICY) {
      throw(
          std::runtime_error("SchemeBase::setPredictionPolicy: "
                             "prediction policy already declared"));
    }
    this->options.ppolicy = p;
  }  // end of SchemeBase::setPredictionPolicy

  void SchemeBase::setStiffnessMatrixType(const StiffnessMatrixType k) {
    if (this->options.ktype !=
        StiffnessMatrixType::UNSPECIFIEDSTIFFNESSMATRIXTYPE) {
      throw(
          std::runtime_error("SchemeBase::setStiffnessMatrixType: "
                             "stiffness matrix type already specificed"));
    }
    this->options.ktype = k;
  }

  void SchemeBase::setUseCastemAccelerationAlgorithm(const bool ucaa) {
    if (ucaa) {
      if (this->options.aa.get() != nullptr) {
        throw(
            std::runtime_error("SchemeBase::setUseCastemAccelerationAlgorithm: "
                               "an algorithm was already set"));
      }
      this->options.aa = std::shared_ptr<AccelerationAlgorithm>(
          new CastemAccelerationAlgorithm);
    }
    this->options.useCastemAcceleration = ucaa;
  }

  void SchemeBase::setCastemAccelerationTrigger(const int i) {
    if (!this->options.useCastemAcceleration) {
      throw(
          std::runtime_error("SchemeBase::setCastemAccelerationTrigger: "
                             "the castem acceleration algorithm has "
                             "not been set using the "
                             "@UseCast3mAccelerationAlgorithm keyword. "
                             "If the Cast3M acceleration algorithm "
                             "was specified using the "
                             "@AccelerationAlgorithm keyword, please "
                             "use the @AccelerationAlgorithmParameter "
                             "keyword to specify the acceleration trigger."));
    }
    if (this->options.aa.get() == nullptr) {
      throw(
          std::runtime_error("SchemeBase::setCastemAccelerationTrigger: "
                             "internal error"));
    }
    std::ostringstream nb;
    nb << i;
    this->options.aa->setParameter("AccelerationTrigger", nb.str());
  }

  void SchemeBase::setCastemAccelerationPeriod(const int p) {
    if (!this->options.useCastemAcceleration) {
      throw(
          std::runtime_error("SchemeBase::setCastemAccelerationPeriod: "
                             "the castem acceleration algorithm has not "
                             "been set using the "
                             "@UseCast3mAccelerationAlgorithm keyword. "
                             "If the Cast3M acceleration algorithm was "
                             "specified using the @AccelerationAlgorithm "
                             "keyword, please use the "
                             "@AccelerationAlgorithmParameter keyword to "
                             "specify the acceleration period."));
    }
    if (this->options.aa.get() == nullptr) {
      throw(
          std::runtime_error("SchemeBase::setCastemAccelerationPeriod: "
                             "internal error"));
    }
    std::ostringstream nb;
    nb << p;
    this->options.aa->setParameter("AccelerationPeriod", nb.str());
  }

  void SchemeBase::setStiffnessUpdatingPolicy(const StiffnessUpdatingPolicy p) {
    if (this->options.ks !=
        StiffnessUpdatingPolicy::UNSPECIFIEDSTIFFNESSUPDATINGPOLICY) {
      throw(
          std::runtime_error("SchemeBase::setStiffnessUpdatePolicy: "
                             "stiffness matrix type already specificed"));
    }
    this->options.ks = p;
  }  // end of SchemeBase::setStiffnessUpdatingPolicy

  void SchemeBase::completeInitialisation() {
    if (this->initialisationFinished) {
      throw(
          std::runtime_error("SchemeBase::completeInitialisation : "
                             "object already initialised"));
    }
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
      if (this->options.aa.get() != nullptr) {
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
        if (this->options.aa.get() != nullptr) {
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
          if (this->options.ppolicy != PredictionPolicy::NOPREDICTION) {
            throw(
                std::runtime_error("MTest::completeInitialisation : "
                                   "internal error, unsupported "
                                   "prediction policy"));
          }
          log << "** no prediction\n";
        }
      }
      // output file
      if (!this->output.empty()) {
        this->out.open(this->output.c_str());
        if (!this->out) {
          throw(
              std::runtime_error("SchemeBase::completeInitialisation : "
                                 "can't open file '" +
                                 this->output + "'"));
        }
        this->out.exceptions(std::ofstream::failbit | std::ofstream::badbit);
        if (this->oprec != -1) {
          this->out.precision(static_cast<std::streamsize>(this->oprec));
        }
      }
      // residual file
      if (!this->residualFileName.empty()) {
        this->residual.open(this->residualFileName.c_str());
        if (!this->residual) {
          throw(
              std::runtime_error("MTest::completeInitialisation : "
                                 "unable to open file '" +
                                 this->residualFileName + "'"));
        }
        this->residual.exceptions(std::ofstream::failbit |
                                  std::ofstream::badbit);
        if (this->rprec != -1) {
          this->residual.precision(static_cast<std::streamsize>(this->rprec));
        } else {
          if (this->oprec != -1) {
            this->residual.precision(static_cast<std::streamsize>(this->oprec));
          }
        }
      }
    } catch (...) {
      this->initialisationFinished = false;
      throw;
    }
  }  // end of SchemeBase::completeInitialisation

  void SchemeBase::declareVariable(const std::string& v, const bool check) {
    if (find(this->vnames.begin(), this->vnames.end(), v) !=
        this->vnames.end()) {
      if (check) {
        throw(
            std::runtime_error("SchemeBase::declareVariable: "
                               "variable '" +
                               v + "' already declared"));
      }
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
    if (!this->output.empty()) {
      throw(
          std::runtime_error("SchemeBase::setOutputFileName: "
                             "output file name already defined"));
    }
    this->output = o;
  }

  bool SchemeBase::isOutputFileNameDefined() const {
    return !this->output.empty();
  }

  void SchemeBase::setOutputFilePrecision(const unsigned int p) {
    if (this->oprec != -1) {
      throw(
          std::runtime_error("SchemeBase::setOutputFileName: "
                             "output file precision already defined"));
    }
    this->oprec = static_cast<int>(p);
  }

  void SchemeBase::setResidualFileName(const std::string& o) {
    if (!this->residualFileName.empty()) {
      throw(
          std::runtime_error("SchemeBase::setResidualFileName : "
                             "residual file name already defined"));
    }
    this->residualFileName = o;
  }

  bool SchemeBase::isResidualFileNameDefined() const {
    return !this->residualFileName.empty();
  }

  void SchemeBase::setResidualFilePrecision(const unsigned int p) {
    if (this->rprec != -1) {
      throw(
          std::runtime_error("SchemeBase::setResidualFileName: "
                             "residual file precision already defined"));
    }
    this->rprec = static_cast<int>(p);
  }

  void SchemeBase::setXMLOutputFileName(const std::string& o) {
    if (!this->xmlFileName.empty()) {
      throw(
          std::runtime_error("SchemeBase::setXMLOutputFileName : "
                             "XMLOutput file name already defined"));
    }
    this->xmlFileName = o;
  }

  bool SchemeBase::isXMLOutputFileNameDefined() const {
    return !this->xmlFileName.empty();
  }

  std::string SchemeBase::getXMLOutputFileName() const {
    if (this->xmlFileName.empty()) {
      throw(
          std::runtime_error("SchemeBase::getXMLOutputFileName : "
                             "XML output file name not defined"));
    }
    return this->xmlFileName;
  }

  SchemeBase::~SchemeBase() {}  // end of SchemeBase::~SchemeBase()

}  // end of namespace mtest
