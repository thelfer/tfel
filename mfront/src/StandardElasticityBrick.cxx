/*!
 * \file  StandardElasticityBrick.cxx
 * \brief
 * \author Helfer Thomas
 * \date   20 oct. 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <stdexcept>

#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/LocalDataStructure.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/StandardElasticityBrick.hxx"

namespace mfront {

  StandardElasticityBrick::StandardElasticityBrick(AbstractBehaviourDSL& dsl_,
                                                   BehaviourDescription& mb_,
                                                   const Parameters& p,
                                                   const DataMap& d)
      : BehaviourBrickBase(dsl_, mb_) {
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error(
            "StandardElasticityBrick::StandardElasticityBrick: " + m));
      }
    };
    // reserve some specific variables
    this->bd.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "sebdata");
    // basic checks

    throw_if(this->bd.getBehaviourType() !=
                 BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,
             "this behaviour brick is only usable for small strain behaviours");
    throw_if(
        this->bd.getIntegrationScheme() != BehaviourDescription::IMPLICITSCHEME,
        "this behaviour brick is only usable in implicit schemes");
    // parameters
    for (const auto& pp : p) {
      if (pp.first == "Isotropic") {
        this->checkThatParameterHasNoValue(pp);
        setElasticSymmetryType(this->bd, mfront::ISOTROPIC);
      } else if (pp.first == "Orthotropic") {
        // this is also checked later during the call of
        // setElasticSymmetryType call
        throw_if(this->bd.getSymmetryType() != mfront::ORTHOTROPIC,
                 "using the orthotropic behaviour requires that the "
                 "behaviour has not been declared orthotropic");
        this->checkThatParameterHasNoValue(pp);
        setElasticSymmetryType(this->bd, mfront::ORTHOTROPIC);
      } else {
        throw_if(true, "unsupported parameter '" + pp.first + "'");
      }
    }
    // options
    auto update = [throw_if, &d](bool& b, const char* n) {
      if (d.count(n) != 0) {
        const auto& v = d.at(n);
        throw_if(!v.is<bool>(),
                 "invalid type for option '" + std::string(n) + "'");
        b = v.get<bool>();
      }
    };
    this->checkOptionsNames(d,
                            {"plane_stress_support", "generic_tangent_operator",
                             "generic_prediction_operator"},
                            this->getName());
    update(this->pss, "plane_stress_support");
    update(this->gto, "generic_tangent_operator");
    update(this->gpo, "generic_tangent_operator");
    if (this->pss) {
      this->bd.registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                  "etozz");
      this->bd.registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                  "detozz");
      this->bd.registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                  "sigzz");
      this->bd.registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                  "dsigzz");
      this->bd.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                           "prediction_stress");
      this->bd.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                           "prediction_strain");
    }
  }  // end of StandardElasticityBrick::StandardElasticityBrick

  void StandardElasticityBrick::endTreatment() const {
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(
            std::runtime_error("StandardElasticityBrick::endTreatment: " + m));
      }
    };
    using tfel::glossary::Glossary;
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "StandardElasticityBrick::endTreatment: begin\n";
    }
    LocalDataStructure d;
    d.name = "sebdata";
    // modelling hypotheses supported by the behaviour
    const auto bmh = bd.getModellingHypotheses();
    // deformation strain
    const auto b = this->bd.checkVariableExistence("eel");
    if (b.first) {
      throw_if(!b.second,
               "'eel' is not declared for all specialisation of the behaviour");
      this->bd.checkVariableExistence("eel", "IntegrationVariable");
      this->bd.checkVariableGlossaryName("eel", Glossary::ElasticStrain);
    } else {
      VariableDescription eel("StrainStensor", "eel", 1u, 0u);
      eel.description = "elastic strain";
      this->bd.addStateVariable(uh, eel);
      this->bd.setGlossaryName(uh, "eel", Glossary::ElasticStrain);
    }
    // treating material properties and stress computation
    if ((this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                               false)) ||
        (this->bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                               false))) {
      this->declareComputeStressWhenStiffnessTensorIsDefined();
    } else {
      if (this->bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
        this->treatIsotropicBehaviour(d);
      } else if (this->bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
        this->treatOrthotropicBehaviour();
      } else {
        throw_if(true, "unsupported elastic symmetry type");
      }
    }
    // plane stress support
    if (this->pss) {
      const bool agps =
          bmh.count(
              ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) != 0;
      const bool ps = bmh.count(ModellingHypothesis::PLANESTRESS) != 0;
      if (ps) {
        this->addPlaneStressSupport(d);
      }
      if (agps) {
        this->addAxisymmetricalGeneralisedPlaneStressSupport(d);
      }
    }
    // declaring the computeElasticPrediction member
    this->declareComputeElasticPredictionMethod(d);
    // prediction operator
    if (this->gpo) {
      this->addGenericPredictionOperatorSupport(d);
    }
    // tangent operator
    if (this->gto) {
      this->addGenericTangentOperatorSupport(d);
    }
    // implicit equation associated with the elastic strain
    CodeBlock integrator;
    integrator.code = "feel -= this->deto;\n";
    this->bd.setCode(uh, BehaviourData::Integrator, integrator,
                     BehaviourData::CREATEORAPPEND,
                     BehaviourData::AT_BEGINNING);
    this->bd.addLocalDataStructure(d, BehaviourData::ALREADYREGISTRED);
  }  // end of StandardElasticityBrick::endTreatment

  void StandardElasticityBrick::declareComputeElasticPredictionMethod(
      const LocalDataStructure& d) const {
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    for (const auto h : this->bd.getDistinctModellingHypotheses()) {
      std::string m =
          "//! \\brief return an elastic prediction of the stresses\n"
          "StressStensor computeElasticPrediction(void) const{\n";
      if (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) {
        if ((this->bd.getAttribute(
                BehaviourDescription::requiresStiffnessTensor, false)) ||
            (this->bd.getAttribute(
                BehaviourDescription::computesStiffnessTensor, false))) {
          if ((this->bd.getElasticSymmetryType() != mfront::ISOTROPIC) &&
              (this->bd.getElasticSymmetryType() != mfront::ORTHOTROPIC)) {
            m += "throw(std::runtime_error(\"computeElasticPrediction: "
                 "unsupported case\"));\n";
          } else {
            if (this->bd.hasAttribute(
                    BehaviourDescription::requiresUnAlteredStiffnessTensor)) {
              m += "StrainStensor prediction_stress;\n";
              m += "StrainStensor prediction_strain = "
                   "this->eel+(this->theta)*this->deto;\n";
              m += "prediction_stress(0) = "
                   "(this->D(0,0)-((this->D(0,1))*(this->D(1,0)))/"
                   "(this->D(1,1)))*prediction_strain(0)+\n";
              m += "                       "
                   "(this->D(0,2)-((this->D(0,1))*(this->D(1,2)))/"
                   "(this->D(1,1)))*prediction_strain(2)+\n";
              m += "                       "
                   "(this->D(0,1))/"
                   "(this->D(1,1))*(this->sigzz+(this->theta)*(this->dsigzz));"
                   "\n";
              m += "prediction_stress(1) = "
                   "this->sigzz+(this->theta)*(this->dsigzz);\n";
              m += "prediction_stress(2) = "
                   "(this->D(2,0)-((this->D(2,1))*(this->D(1,0)))/"
                   "(this->D(1,1)))*prediction_strain(0)+\n";
              m += "                       "
                   "(this->D(2,2)-((this->D(2,1))*(this->D(1,2)))/"
                   "(this->D(1,1)))*prediction_strain(2)+\n";
              m += "                       "
                   "(this->D(2,1))/"
                   "(this->D(1,1))*(this->sigzz+(this->theta)*(this->dsigzz));"
                   "\n";
              m += "return prediction_stress;\n";
            } else {
              m += "throw(std::runtime_error(\"computeElasticPrediction: "
                   "unsupported case\"));\n";
            }
          }
        } else {
          if (this->bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
            const std::string lambda = d.contains(uh, "lambda")
                                           ? "this->sebdata.lambda"
                                           : "this->lambda";
            const std::string mu =
                d.contains(uh, "mu") ? "this->sebdata.mu" : "this->mu";
            m += "StrainStensor prediction_stress;\n";
            m += "StrainStensor prediction_strain = "
                 "this->eel+(this->theta)*this->deto;\n";
            m += "prediction_stress(0) = 2*(" + mu + ")*((" + lambda + ")/(" +
                 lambda + "+2*(" + mu +
                 "))*(prediction_strain(0)+prediction_strain(2))+prediction_"
                 "strain(0))+\n";
            m += "(" + lambda + ")/(" + lambda + "+2*(" + mu +
                 "))*(this->sigzz+theta*(this->dsigzz));\n";
            m += "prediction_stress(2) = 2*(" + mu + ")*((" + lambda + ")/(" +
                 lambda + "+2*(" + mu +
                 "))*(prediction_strain(0)+prediction_strain(2))+prediction_"
                 "strain(2))+\n";
            m += "(" + lambda + ")/(" + lambda + "+2*(" + mu +
                 "))*(this->sigzz+theta*(this->dsigzz));\n";
            m += "prediction_stress(1) = this->sigzz+theta*(this->dsigzz);\n";
            m += "return prediction_stress;\n";
          } else {
            if (!this->bd.getAttribute<bool>(
                    BehaviourDescription::requiresStiffnessTensor)) {
              throw(
                  std::runtime_error("StandardElasticityBrick::"
                                     "declareComputeElasticPredictionMethod: "
                                     "the stiffness tensor must be defined for "
                                     "orthotropic behaviours"));
            }
          }
        }
      } else if (h == ModellingHypothesis::PLANESTRESS) {
        if ((this->bd.getAttribute(
                BehaviourDescription::requiresStiffnessTensor, false)) ||
            (this->bd.getAttribute(
                BehaviourDescription::computesStiffnessTensor, false))) {
          if ((this->bd.getElasticSymmetryType() != mfront::ISOTROPIC) &&
              (this->bd.getElasticSymmetryType() != mfront::ORTHOTROPIC)) {
            m += "throw(std::runtime_error(\"computeElasticPrediction: "
                 "unsupported case\"));\n";
          } else {
            if (this->bd.hasAttribute(
                    BehaviourDescription::requiresUnAlteredStiffnessTensor)) {
              m += "StrainStensor prediction_stress;\n";
              m += "StrainStensor prediction_strain = "
                   "this->eel+(this->theta)*this->deto;\n";
              m += "prediction_stress(0) = "
                   "(this->D(0,0)-((this->D(0,2))*(this->D(2,0)))/"
                   "(this->D(2,2)))*prediction_strain(0)+\n";
              m += "                       "
                   "(this->D(0,1)-((this->D(0,2))*(this->D(2,1)))/"
                   "(this->D(2,2)))*prediction_strain(1);\n";
              m += "prediction_stress(1) = "
                   "(this->D(1,0)-((this->D(1,2))*(this->D(2,0)))/"
                   "(this->D(2,2)))*prediction_strain(0)+\n";
              m += "                       "
                   "(this->D(1,1)-((this->D(1,2))*(this->D(2,1)))/"
                   "(this->D(2,2)))*prediction_strain(1);\n";
              m += "prediction_stress(2) = stress(0);\n";
              m += "prediction_stress(3) = stress(0);\n";
              m += "return prediction_stress;\n";
            } else {
              m += "return (this->D)*(this->eel+(this->theta)*this->deto);";
            }
          }
        } else {
          if (this->bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
            const std::string lambda = d.contains(uh, "lambda")
                                           ? "this->sebdata.lambda"
                                           : "this->lambda";
            const std::string mu =
                d.contains(uh, "mu") ? "this->sebdata.mu" : "this->mu";
            m += "StrainStensor prediction_stress;\n";
            m += "StressStensor prediction_strain = "
                 "this->eel+(this->theta)*this->deto;\n";
            m += "prediction_stress(0) = 2*(" + mu + ")*((" + lambda + ")/(" +
                 lambda + "+2*(" + mu +
                 "))*(prediction_strain(0)+prediction_strain(1))+prediction_"
                 "strain(0));\n";
            m += "prediction_stress(1) = 2*(" + mu + ")*((" + lambda + ")/(" +
                 lambda + "+2*(" + mu +
                 "))*(prediction_strain(0)+prediction_strain(1))+prediction_"
                 "strain(1));\n";
            m += "prediction_stress(2) = stress(0);\n";
            m +=
                "prediction_stress(3) = 2*(" + mu + ")*prediction_strain(3);\n";
            m += "return prediction_stress;\n";
          } else {
            if (!this->bd.getAttribute<bool>(
                    BehaviourDescription::requiresStiffnessTensor)) {
              throw(
                  std::runtime_error("StandardElasticityBrick::"
                                     "declareComputeElasticPredictionMethod: "
                                     "the stiffness tensor must be defined for "
                                     "orthotropic behaviours"));
            }
          }
        }
      } else {
        if ((this->bd.getAttribute(
                BehaviourDescription::requiresStiffnessTensor, false)) ||
            (this->bd.getAttribute(
                BehaviourDescription::computesStiffnessTensor, false))) {
          m += "return (this->D)*(this->eel+(this->theta)*this->deto);";
        } else {
          if (this->bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
            const std::string lambda = d.contains(uh, "lambda")
                                           ? "this->sebdata.lambda"
                                           : "this->lambda";
            const std::string mu =
                d.contains(uh, "mu") ? "this->sebdata.mu" : "this->mu";
            m += "return " + lambda +
                 "*trace(this->eel+(this->theta)*(this->deto))*Stensor::Id()+"
                 "2*(" +
                 mu + ")*(this->eel+(this->theta)*(this->deto));\n";
          } else {
            if (!this->bd.getAttribute<bool>(
                    BehaviourDescription::requiresStiffnessTensor)) {
              throw(
                  std::runtime_error("StandardElasticityBrick::"
                                     "declareComputeElasticPredictionMethod: "
                                     "the stiffness tensor must be defined for "
                                     "orthotropic behaviours"));
            }
          }
        }
      }
      m += "}\n";
      this->bd.appendToMembers(h, m, false);
    }
  }  // end of StandardElasticityBrick::declareComputeElasticPredictionMethod

  void
  StandardElasticityBrick::declareComputeStressWhenStiffnessTensorIsDefined()
      const {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    CodeBlock smts, sets;
    smts.code = "this->sig=(this->D)*(this->eel+theta*(this->deel));\n";
    sets.code = "this->sig=(this->D)*(this->eel);\n";
    this->bd.setCode(h, BehaviourData::ComputeStress, smts,
                     BehaviourData::CREATE, BehaviourData::AT_BEGINNING, false);
    this->bd.setCode(h, BehaviourData::ComputeFinalStress, sets,
                     BehaviourData::CREATE, BehaviourData::AT_BEGINNING, false);
  }  // end of
     // StandardElasticityBrick::declareComputeStressWhenStiffnessTensorIsDefined

  void StandardElasticityBrick::treatIsotropicBehaviour(
      LocalDataStructure& d) const {
    using tfel::glossary::Glossary;
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream()
          << "StandardElasticityBrick::treatIsotropicBehaviour: begin\n";
    }
    this->bd.appendToIncludes("#include\"TFEL/Material/Lame.hxx\"");
    CodeBlock smts;
    CodeBlock sets;
    if (this->bd.areElasticMaterialPropertiesDefined()) {
      smts.code =
          "this->sig=this->lambda*trace(this->eel+theta*(this->deel))*Stensor::"
          "Id()+"
          "2*(this->mu)*(this->eel+theta*(this->deel));\n";
      sets.code =
          "this->sig=this->lambda_tdt*trace(this->eel)*Stensor::Id()+2*(this->"
          "mu_tdt)*this->eel;";
    } else {
      this->addMaterialPropertyIfNotDefined("stress", "young",
                                            Glossary::YoungModulus);
      this->addMaterialPropertyIfNotDefined("real", "nu",
                                            Glossary::PoissonRatio);
      d.addVariable(uh, {"stress", "lambda"});
      d.addVariable(uh, {"stress", "mu"});
      // local variable initialisation
      CodeBlock init;
      init.code =
          "// initialisation Lame's coefficient\n"
          "this->sebdata.lambda=tfel::material::computeLambda(this->young,this-"
          ">nu);\n"
          "this->sebdata.mu=tfel::material::computeMu(this->young,this->nu);\n";
      this->bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, init,
                       BehaviourData::CREATE, BehaviourData::AT_BEGINNING,
                       true);
      // Hooke law
      smts.code =
          "this->sig=(this->sebdata.lambda)*trace(this->eel+theta*(this->deel))"
          "*Stensor::Id()+"
          "2*(this->sebdata.mu)*(this->eel+theta*(this->deel));\n";
      sets.code =
          "this->sig=(this->sebdata.lambda)*trace(this->eel)*Stensor::Id()+2*("
          "this->sebdata.mu)*this->eel;";
    }
    this->bd.setCode(uh, BehaviourData::ComputeStress, smts,
                     BehaviourData::CREATE, BehaviourData::AT_BEGINNING, false);
    this->bd.setCode(uh, BehaviourData::ComputeFinalStress, sets,
                     BehaviourData::CREATE, BehaviourData::AT_BEGINNING, false);
  }  // end of StandardElasticityBrick::treatIsotropicBehaviour

  void StandardElasticityBrick::treatOrthotropicBehaviour() const {
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "StandardElasticityBrick::treatOrthotropic: begin\n";
    }
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (!this->bd.hasAttribute(BehaviourDescription::requiresStiffnessTensor)) {
      this->bd.setAttribute(h, BehaviourDescription::requiresStiffnessTensor,
                            true);
    }
    if (!this->bd.getAttribute<bool>(
            BehaviourDescription::requiresStiffnessTensor)) {
      throw(std::runtime_error(
          "StandardElasticityBrick::treatOrthotropicBehaviour: "
          "the stiffness tensor must be defined for "
          "orthotropic behaviours"));
    }
  }  // end of StandardElasticityBrick::treatOrthotropicBehaviour

  void StandardElasticityBrick::addAxisymmetricalGeneralisedPlaneStressSupport(
      LocalDataStructure& d) const {
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto agps = ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS;
    VariableDescription etozz("strain", "etozz", 1u, 0u);
    etozz.description = "axial strain";
    d.addVariable(agps, {"stress", "szz"});
    this->bd.addStateVariable(agps, etozz, BehaviourData::ALREADYREGISTRED);
    this->bd.setGlossaryName(agps, "etozz",
                             tfel::glossary::Glossary::AxialStrain);
    VariableDescription sigzz("strain", "sigzz", 1u, 0u);
    etozz.description = "axial stress";
    this->bd.addExternalStateVariable(agps, sigzz,
                                      BehaviourData::ALREADYREGISTRED);
    this->bd.setGlossaryName(agps, "sigzz",
                             tfel::glossary::Glossary::AxialStress);
    CodeBlock integrator;
    // The brick contains a reference to an abstract behaviour dsl.
    // We need to know if we have to define the jacobian terms. So we
    // downcast it to the ImplicitDSLBase class have access to the
    // solver. See the getSolver call below.
    const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(this->dsl);
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::requiresStiffnessTensor, false)) ||
        (this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false))) {
      const std::string D =
          this->bd.getAttribute<bool>(
              BehaviourDescription::computesStiffnessTensor, false)
              ? "D_tdt"
              : "D";
      integrator.code +=
          "// the generalised plane stress equation is satisfied at the end of "
          "the time step\n"
          "this->sebdata.szz = (this->" +
          D + "(1,1))*(this->eel(1)+this->deel(1))+(this->" + D +
          "(1,0))*(this->eel(0)+this->deel(0))+(this->" + D +
          "(1,2))*(this->eel(2)+this->deel(2));\n"
          "fetozz   = (this->sebdata.szz-this->sigzz-this->dsigzz)/(this->" +
          D +
          "(1,1));\n"
          "// modification of the partition of strain\n"
          "feel(1) -= this->detozz;\n";
      if ((idsl.getSolver().usesJacobian()) &&
          (!idsl.getSolver().requiresNumericalJacobian())) {
        integrator.code +=
            "// jacobian\n"
            "dfeel_ddetozz(1) = -1;\n"
            "dfetozz_ddetozz  = real(0);\n"
            "dfetozz_ddeel(1) = 1;\n"
            "dfetozz_ddeel(0) = (this->" +
            D + "(1,0))/(this->" + D +
            "(1,1));\n"
            "dfetozz_ddeel(2) = (this->" +
            D + "(1,2))/(this->" + D + "(1,1));\n";
      }
    } else {
      if (this->bd.areElasticMaterialPropertiesDefined()) {
        integrator.code +=
            "// the generalised plane stress equation is satisfied at the end "
            "of the time step\n"
            "this->sebdata.szz =   "
            "(this->lambda_tdt+2*(this->mu_tdt))*(this->eel(1)+this->deel(1))"
            "                   + "
            "(this->lambda_tdt)*(this->eel(0)+this->deel(0)+this->eel(2)+this->"
            "deel(2));\n"
            "fetozz   = "
            "(this->sebdata.szz-this->sigzz-this->dsigzz)/this->young_tdt;\n"
            "// modification of the partition of strain\n"
            "feel(1) -= this->detozz;\n";
        if ((idsl.getSolver().usesJacobian()) &&
            (!idsl.getSolver().requiresNumericalJacobian())) {
          integrator.code +=
              "// jacobian\n"
              "dfeel_ddetozz(1) = -1;\n"
              "dfetozz_ddetozz  = real(0);\n"
              "dfetozz_ddeel(1) = "
              "(this->lambda_tdt+2*(this->mu_tdt))/this->young_tdt;\n"
              "dfetozz_ddeel(0) = this->lambda_tdt/this->young_tdt;\n"
              "dfetozz_ddeel(2) = this->lambda_tdt/this->young_tdt;\n";
        }
      } else {
        const std::string lambda =
            d.contains(uh, "lambda") ? "this->sebdata.lambda" : "this->lambda";
        const std::string mu =
            d.contains(uh, "mu") ? "this->sebdata.mu" : "this->mu";
        integrator.code +=
            "// the generalised plane stress equation is satisfied at the end "
            "of the time step\n"
            "this->sebdata.szz =   (" +
            lambda + "+2*(" + mu +
            "))*(this->eel(1)+this->deel(1))"
            "                   + (" +
            lambda +
            ")*(this->eel(0)+this->deel(0)+this->eel(2)+this->deel(2));\n"
            "fetozz   = "
            "(this->sebdata.szz-this->sigzz-this->dsigzz)/this->young;\n"
            "// modification of the partition of strain\n"
            "feel(1) -= this->detozz;\n";
        if ((idsl.getSolver().usesJacobian()) &&
            (!idsl.getSolver().requiresNumericalJacobian())) {
          integrator.code +=
              "// jacobian\n"
              "dfeel_ddetozz(1) = -1;\n"
              "dfetozz_ddetozz  = real(0);\n"
              "dfetozz_ddeel(1) = (" +
              lambda + "+2*(" + mu +
              "))/this->young;\n"
              "dfetozz_ddeel(0) = " +
              lambda +
              "/this->young;\n"
              "dfetozz_ddeel(2) = " +
              lambda + "/this->young;\n";
        }
      }
    }
    this->bd.setCode(agps, BehaviourData::Integrator, integrator,
                     BehaviourData::CREATEORAPPEND,
                     BehaviourData::AT_BEGINNING);
  }

  void StandardElasticityBrick::addPlaneStressSupport(
      LocalDataStructure& d) const {
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto ps = ModellingHypothesis::PLANESTRESS;
    d.addVariable(ps, {"stress", "szz"});
    VariableDescription etozz("strain", "etozz", 1u, 0u);
    etozz.description = "axial strain";
    this->bd.addStateVariable(ps, etozz, BehaviourData::ALREADYREGISTRED);
    this->bd.setGlossaryName(ps, "etozz",
                             tfel::glossary::Glossary::AxialStrain);
    CodeBlock integrator;
    const auto& idsl = dynamic_cast<ImplicitDSLBase&>(this->dsl);
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::requiresStiffnessTensor, false)) ||
        (this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false))) {
      const std::string D =
          this->bd.getAttribute<bool>(
              BehaviourDescription::computesStiffnessTensor, false)
              ? "D_tdt"
              : "D";
      integrator.code +=
          "// the plane stress equation is satisfied at the end of the time "
          "step\n"
          "fetozz   = this->eel(2)+this->deel(2)+"
          "           ((this->" +
          D + "(2,0))/(this->" + D +
          "(2,2)))*(this->eel(0)+this->deel(0))+"
          "           ((this->" +
          D + "(2,1))/(this->" + D +
          "(2,2)))*(this->eel(1)+this->deel(1));\n"
          "// modification of the partition of strain\n"
          "feel(2)          -= this->detozz;\n";
      if ((idsl.getSolver().usesJacobian()) &&
          (!idsl.getSolver().requiresNumericalJacobian())) {
        integrator.code +=
            "// jacobian\n"
            "dfeel_ddetozz(2)  = -1;\n"
            "dfetozz_ddetozz   = real(0);\n"
            "dfetozz_ddeel(2)  = 1;\n"
            "dfetozz_ddeel(0)  = (this->" +
            D + "(1,0))/(this->" + D +
            "(1,1));\n"
            "dfetozz_ddeel(1)  = (this->" +
            D + "(2,0))/(this->" + D + "(1,1));\n";
      }
    } else {
      if (this->bd.areElasticMaterialPropertiesDefined()) {
        integrator.code +=
            "// the plane stress equation is satisfied at the end of the time "
            "step\n"
            "this->sebdata.szz = "
            "(this->lambda_tdt+2*(this->mu_tdt))*(this->eel(2)+this->deel(2))+"
            "                   "
            "(this->lambda_tdt)*(this->eel(0)+this->deel(0)+this->eel(1)+this->"
            "deel(1));\n"
            "fetozz   = this->sebdata.szz/(this->young_tdt);\n"
            "// modification of the partition of strain\n"
            "feel(2) -= detozz;\n";
        if ((idsl.getSolver().usesJacobian()) &&
            (!idsl.getSolver().requiresNumericalJacobian())) {
          integrator.code +=
              "// jacobian\n"
              "dfeel_ddetozz(2) = -1;\n"
              "dfetozz_ddetozz  = real(0);\n"
              "dfetozz_ddeel(2) = "
              "(this->lambda_tdt+2*(this->mu_tdt))/this->young_tdt;\n"
              "dfetozz_ddeel(0) = this->lambda_tdt/this->young_tdt;\n"
              "dfetozz_ddeel(1) = this->lambda_tdt/this->young_tdt;\n";
        }
      } else {
        const std::string lambda =
            d.contains(uh, "lambda") ? "this->sebdata.lambda" : "this->lambda";
        const std::string mu =
            d.contains(uh, "mu") ? "this->sebdata.mu" : "this->mu";
        integrator.code +=
            "// the plane stress equation is satisfied at the end of the time "
            "step\n"
            "this->sebdata.szz = (" +
            lambda + "+2*(" + mu +
            "))*(this->eel(2)+this->deel(2))+"
            "                   (" +
            lambda +
            ")*(this->eel(0)+this->deel(0)+this->eel(1)+this->deel(1));\n"
            "fetozz   = this->sebdata.szz/(this->young);\n"
            "// modification of the partition of strain\n"
            "feel(2) -= detozz;\n";
        if ((idsl.getSolver().usesJacobian()) &&
            (!idsl.getSolver().requiresNumericalJacobian())) {
          integrator.code +=
              "// jacobian\n"
              "dfeel_ddetozz(2) = -1;\n"
              "dfetozz_ddetozz  = real(0);\n"
              "dfetozz_ddeel(2) = (" +
              lambda + "+2*(" + mu +
              "))/this->young;\n"
              "dfetozz_ddeel(0) = " +
              lambda +
              "/this->young;\n"
              "dfetozz_ddeel(1) = " +
              lambda + "/this->young;\n";
        }
      }
    }
    this->bd.setCode(ps, BehaviourData::Integrator, integrator,
                     BehaviourData::CREATEORAPPEND,
                     BehaviourData::AT_BEGINNING);
  }

  void StandardElasticityBrick::addGenericTangentOperatorSupport(
      const LocalDataStructure& d) const {
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(
            std::runtime_error("StandardElasticityBrick::"
                               "addGenericTangentOperatorSupport: " +
                               m));
      }
    };
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(this->dsl);
    this->bd.checkVariablePosition("eel", "IntegrationVariable", 0u);
    CodeBlock tangentOperator;
    // modelling hypotheses supported by the behaviour
    const auto bmh = bd.getModellingHypotheses();
    if ((this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                               false)) ||
        (this->bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                               false))) {
      const bool agps =
          bmh.count(
              ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) != 0;
      const bool ps = bmh.count(ModellingHypothesis::PLANESTRESS) != 0;
      if (agps || ps) {
        if (this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                                  false)) {
          if (!this->bd.hasAttribute(
                  BehaviourDescription::requiresUnAlteredStiffnessTensor)) {
            this->bd.setAttribute(
                BehaviourDescription::requiresUnAlteredStiffnessTensor, true,
                false);
          }
          throw_if(!this->bd.getAttribute<bool>(
                       BehaviourDescription::requiresUnAlteredStiffnessTensor),
                   "genertic tangent operator support for "
                   "plane stress hypotheses requires the use of an unaltered "
                   "stiffness tensor");
        }
      }
      const std::string D =
          this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                                false)
              ? "this->D"
              : "this->D_tdt";
      tangentOperator.code =
          "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
          "  this->Dt = " +
          D + ";\n";
      if (idsl.getSolver().usesJacobian()) {
        tangentOperator.code +=
            "} else if (smt==CONSISTENTTANGENTOPERATOR){\n"
            "  Stensor4 Je;\n"
            "  getPartialJacobianInvert(Je);\n"
            "  this->Dt = (" +
            D + ")*Je;\n";
      }
      tangentOperator.code +=
          "} else {\n"
          "  return false;\n"
          "}";
    } else {
      if (this->bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
        const std::string lambda = d.contains(uh, "lambda")
                                       ? "this->sebdata.lambda"
                                       : "this->lambda_tdt";
        const std::string mu =
            d.contains(uh, "mu") ? "this->sebdata.mu" : "this->mu_tdt";
        tangentOperator.code =
            "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
            "  computeAlteredElasticStiffness<hypothesis,Type>::exe(Dt," +
            lambda + "," + mu + ");\n";
        if (idsl.getSolver().usesJacobian()) {
          tangentOperator.code +=
              "} else if (smt==CONSISTENTTANGENTOPERATOR){\n"
              "  StiffnessTensor Hooke;\n"
              "  Stensor4 Je;\n"
              "  computeElasticStiffness<N,Type>::exe(Hooke," +
              lambda + "," + mu +
              ");\n"
              "  getPartialJacobianInvert(Je);\n"
              "  Dt = Hooke*Je;\n";
        }
        tangentOperator.code +=
            "} else {\n"
            "  return false;\n"
            "}";
      } else if (this->bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
        throw_if(!this->bd.getAttribute<bool>(
                     BehaviourDescription::computesStiffnessTensor, false),
                 "orthotropic behaviour shall require the stiffness tensor");
        tangentOperator.code =
            "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
            "  this->Dt = this->D_tdt;\n";
        if (idsl.getSolver().usesJacobian()) {
          tangentOperator.code +=
              "} else if (smt==CONSISTENTTANGENTOPERATOR){\n"
              "  Stensor4 Je;\n"
              "  getPartialJacobianInvert(Je);\n"
              "  this->Dt = (this->D_tdt)*Je;\n";
        }
        tangentOperator.code +=
            "} else {\n"
            "  return false;\n"
            "}";
      } else {
        throw_if(true, "unsupported elastic symmetry type");
      }
    }
    this->bd.setAttribute(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                          BehaviourData::hasConsistentTangentOperator, true,
                          true);
    this->bd.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                     BehaviourData::ComputeTangentOperator, tangentOperator,
                     BehaviourData::CREATEORAPPEND,
                     BehaviourData::AT_BEGINNING);
  }

  void StandardElasticityBrick::addGenericPredictionOperatorSupport(
      const LocalDataStructure& d) const {
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(
            std::runtime_error("StandardElasticityBrick::"
                               "addGenericPredictionOperatorSupport: " +
                               m));
      }
    };
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    CodeBlock tangentOperator;
    // modelling hypotheses supported by the behaviour
    const auto bmh = bd.getModellingHypotheses();
    if ((this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                               false)) ||
        (this->bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                               false))) {
      const bool agps =
          bmh.count(
              ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) != 0;
      const bool ps = bmh.count(ModellingHypothesis::PLANESTRESS) != 0;
      if (agps || ps) {
        if (this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                                  false)) {
          if (!this->bd.hasAttribute(
                  BehaviourDescription::requiresUnAlteredStiffnessTensor)) {
            this->bd.setAttribute(
                BehaviourDescription::requiresUnAlteredStiffnessTensor, true,
                false);
          }
          throw_if(!this->bd.getAttribute<bool>(
                       BehaviourDescription::requiresUnAlteredStiffnessTensor),
                   "genertic tangent operator support for "
                   "plane stress hypotheses requires the use of an unaltered "
                   "stiffness tensor");
        }
      }
      const std::string D =
          this->bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                                false)
              ? "this->D"
              : "this->D_tdt";
      tangentOperator.code =
          "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
          "  this->Dt = " +
          D +
          ";\n"
          "} else {\n"
          "  return FAILURE;\n"
          "}";
    } else {
      if (this->bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
        const std::string lambda = d.contains(uh, "lambda")
                                       ? "this->sebdata.lambda"
                                       : "this->lambda_tdt";
        const std::string mu =
            d.contains(uh, "mu") ? "this->sebdata.mu" : "this->mu_tdt";
        tangentOperator.code =
            "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
            "  computeAlteredElasticStiffness<hypothesis,Type>::exe(Dt," +
            lambda + "," + mu +
            ");\n"
            "} else {\n"
            "  return FAILURE;\n"
            "}";
      } else if (this->bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
        throw_if(!this->bd.getAttribute<bool>(
                     BehaviourDescription::computesStiffnessTensor, false),
                 "orthotropic behaviour shall require the stiffness tensor");
        tangentOperator.code =
            "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
            "  this->Dt = this->D_tdt;\n"
            "} else {\n"
            "  return FAILURE;\n"
            "}";
      } else {
        throw_if(true, "unsupported elastic symmetry type");
      }
    }
    this->bd.setAttribute(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                          BehaviourData::hasPredictionOperator, true, true);
    this->bd.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                     BehaviourData::ComputePredictionOperator, tangentOperator,
                     BehaviourData::CREATEORAPPEND,
                     BehaviourData::AT_BEGINNING);
  }

  std::string StandardElasticityBrick::getName() const { return "Elasticity"; }

  std::vector<StandardElasticityBrick::Hypothesis>
  StandardElasticityBrick::getSupportedModellingHypotheses() const {
    auto dmh = this->dsl.getDefaultModellingHypotheses();
    std::vector<Hypothesis> mh(dmh.begin(), dmh.end());
    if (this->pss) {
      if (this->dsl.isModellingHypothesisSupported(
              ModellingHypothesis::PLANESTRESS)) {
        mh.push_back(ModellingHypothesis::PLANESTRESS);
      }
      if (this->dsl.isModellingHypothesisSupported(
              ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
        mh.push_back(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS);
      }
    }
    return mh;
  }  // end of StandardElasticityBrick::getSupportedModellingHypothesis

  StandardElasticityBrick::~StandardElasticityBrick() = default;

}  // end of namespace mfront
