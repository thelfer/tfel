/*!
 * \file  DDIF2Brick.cxx
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
#include "MFront/DDIF2Brick.hxx"

namespace mfront {

  DDIF2Brick::DDIF2Brick(AbstractBehaviourDSL& dsl_,
                         BehaviourDescription& mb_,
                         const Parameters& p,
                         const DataMap& d)
      : StandardElasticityBrick(dsl_, mb_, p, DataMap()) {
    using MaterialPropertyInput = BehaviourDescription::MaterialPropertyInput;
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error("DDIF2Brick::DDIF2Brick: " + m));
      }
    };
    std::function<std::string(const MaterialPropertyInput&)> ets =
        [this, throw_if](const MaterialPropertyInput& i) -> std::string {
      if ((i.type == MaterialPropertyInput::TEMPERATURE) ||
          (i.type ==
           MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
          (i.type == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
        return "this->" + i.name + "+this->d" + i.name;
      } else if ((i.type == MaterialPropertyInput::MATERIALPROPERTY) ||
                 (i.type == MaterialPropertyInput::PARAMETER)) {
        return "this->" + i.name;
      }
      throw_if(true, "unsupported input type for variable '" + i.name + "'");
    };
    // undefined hypothesis
    constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    // reserve some specific variables
    this->bd.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                         "ddif2bdata");
    // reserve some specific variables
    this->bd.appendToIncludes("#include\"TFEL/Material/DDIF2Base.hxx\"");
    VariableDescription ef("strain", "ef", 3u, 0u);
    ef.description = "fracture strain";
    this->bd.addStateVariable(uh, ef);
    this->bd.setEntryName(uh, "ef", "FractureStrain");
    VariableDescription efm("strain", "efm", 3u, 0u);
    efm.description = "fracture strain";
    this->bd.addAuxiliaryStateVariable(uh, efm);
    this->bd.setEntryName(uh, "efm", "MaximumFractureStrain");
    VariableDescription nf("StrainStensor", "nf", 3u, 0u);
    this->bd.addLocalVariable(uh, nf);
    // data
    this->checkOptionsNames(d, {"fracture_stress", "softening_slope"},
                            this->getName());
    CodeBlock init;
    if (d.count("fracture_stress")) {
      const auto& s = d.at("fracture_stress");
      if (s.is<double>()) {
        const auto v = s.get<double>();
        throw_if(v < 0, "invalid fracture stress");
        this->addParameter("sigr", "FractureStress", s.get<double>());
      } else if (s.is<std::string>()) {
        const auto& f = s.get<std::string>();
        this->addLocalVariable("stress", "sigr");
        this->sr = this->dsl.handleMaterialPropertyDescription(f);
        BehaviourDescription::ComputedMaterialProperty mp_sr;
        mp_sr.mpd = this->sr;
        std::ostringstream ssigr;
        ssigr << "this->sigr = ";
        this->dsl.writeMaterialPropertyEvaluation(ssigr, mp_sr, ets);
        ssigr << ";\n";
        init.code += ssigr.str();
      } else {
        throw_if(true, "invalid type for data 'fracture_stress'");
      }
    }
    if (d.count("softening_slope")) {
      const auto& s = d.at("softening_slope");
      if (s.is<double>()) {
        const auto v = s.get<double>();
        throw_if(v > 0, "invalid fracture stress");
        this->addParameter("Rp", "SofteningSlope", s.get<double>());
      } else if (s.is<std::string>()) {
        const auto& f = s.get<std::string>();
        this->addLocalVariable("stress", "Rp");
        this->rp = this->dsl.handleMaterialPropertyDescription(f);
        BehaviourDescription::ComputedMaterialProperty mp_rp;
        mp_rp.mpd = this->rp;
        std::ostringstream srp;
        srp << "this->Rp = ";
        this->dsl.writeMaterialPropertyEvaluation(srp, mp_rp, ets);
        srp << ";\n";
        init.code += srp.str();
      } else {
        throw_if(true, "invalid type for data 'softening_slope'");
      }
    }
    if (!init.code.empty()) {
      this->bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, init,
                       BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
    }
  }  // end of DDIF2Brick::DDIF2Brick

  void DDIF2Brick::endTreatment() const {
    using tfel::glossary::Glossary;
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error("DDIF2Brick::endTreatment: " + m));
      }
    };
    constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "DDIF2Brick::endTreatment: begin\n";
    }
    // fracture properties
    if (this->sr == nullptr) {
      this->addMaterialPropertyIfNotDefined("stress", "sigr", "FractureStress");
    }
    if (this->rp == nullptr) {
      this->addMaterialPropertyIfNotDefined("stress", "Rp", "SofteningSlope");
    }
    LocalDataStructure d;
    d.name = "ddif2bdata";
    d.addVariable(uh, {"StressStensor", "sig"});
    // modelling hypotheses supported by the brick
    const auto smh = this->getSupportedModellingHypotheses();
    // modelling hypotheses supported by the behaviour
    const auto bmh = this->bd.getModellingHypotheses();
    for (const auto& h : bmh) {
      throw_if(
          std::find(smh.begin(), smh.end(), h) == smh.end(),
          "unsupported hypothesis '" + ModellingHypothesis::toString(h) + "'");
    }
    StandardElasticityBrick::endTreatment();
    // init local variables
    CodeBlock init;
    init.code =
        "for(unsigned short idx=0;idx!=3;++idx){\n"
        "this->nf[idx]      = Stensor(real(0));\n"
        "this->nf[idx][idx] = real(1);\n"
        "}\n";
    this->bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, init,
                     BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
    // implicit equation associated with the crack strains
    const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(this->dsl);
    CodeBlock integrator;
    integrator.code =
        "feel += "
        "((this->def)[0])*(this->nf[0])+((this->def)[1])*(this->nf[1])+((this->"
        "def)[2])*(this->nf[2]);\n";
    if ((idsl.getSolver().usesJacobian()) &&
        (!idsl.getSolver().requiresNumericalJacobian())) {
      const std::string young = this->bd.areElasticMaterialPropertiesDefined()
                                    ? "this->young_tdt"
                                    : "this->young";
      const std::string lambda = this->bd.areElasticMaterialPropertiesDefined()
                                     ? "this->lambda_tdt"
                                     : "this->sebdata.lambda";
      const std::string mu = this->bd.areElasticMaterialPropertiesDefined()
                                 ? "this->mu_tdt"
                                 : "this->sebdata.mu";
      integrator.code +=
          "this->ddif2bdata.sig=(" + lambda +
          "*trace(this->eel+deel))*StrainStensor::Id()+\n"
          "                     2*" +
          mu +
          "*(this->eel+this->deel);\n"
          "this->dfeel_ddef(0)(0) = real(1);\n"
          "this->dfeel_ddef(0)(0) = real(1);\n"
          "this->dfeel_ddef(1)(1) = real(1);\n"
          "this->dfeel_ddef(2)(2) = real(1);\n"
          "for(unsigned short idx=0;idx!=3;++idx){\n"
          "auto dfe_ddeel = dfef_ddeel(idx);\n"
          "DDIF2Base::treatFracture(dfe_ddeel,dfef_ddef(idx,idx),fef(idx),\n"
          "		          "
          "this->efm(idx),this->ef(idx),(this->def)(idx),\n"
          "                         this->ddif2bdata.sig,this->nf(idx),\n"
          "		          this->sigr,this->Rp,\n"
          "		          " +
          young + "," + lambda + ',' + mu +
          ");\n"
          "}\n";
    }
    /* fracture */
    this->bd.setCode(uh, BehaviourData::Integrator, integrator,
                     BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
    /* update auxiliary state variables */
    CodeBlock uasv;
    uasv.code =
        "this->efm[0]=std::max(this->efm[0],this->ef[0]);\n"
        "this->efm[1]=std::max(this->efm[1],this->ef[1]);\n"
        "this->efm[2]=std::max(this->efm[2],this->ef[2]);\n";
    this->bd.setCode(uh, BehaviourData::UpdateAuxiliaryStateVariables, uasv,
                     BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
    this->bd.addLocalDataStructure(d, BehaviourData::ALREADYREGISTRED);
  }  // end of DDIF2Brick::endTreatment

  std::string DDIF2Brick::getName() const { return "DDIF2"; }

  std::vector<tfel::material::ModellingHypothesis::Hypothesis>
  DDIF2Brick::getSupportedModellingHypotheses() const {
    const auto mh = this->dsl.getDefaultModellingHypotheses();
    return {mh.begin(), mh.end()};
  }  // end of DDIF2Brick::getSupportedModellingHypothesis

  DDIF2Brick::~DDIF2Brick() = default;

}  // end of namespace mfront
