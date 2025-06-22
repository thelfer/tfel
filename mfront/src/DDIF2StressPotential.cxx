/*!
 * \file  DDIF2StressPotential.cxx
 * \brief
 * \author Thomas Helfer
 * \date   20 oct. 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#include <sstream>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/LocalDataStructure.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/DDIF2StressPotential.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief extract a material property or an array of material properties
     * \param[out] mps: array of material properties. If only one material
     * property is defined in the options, then only the first entry of the
     * array is defined.
     * \param[out] bd: behaviour description
     * \param[in] dsl: abstract behaviour dsl
     * \param[in] n: material property name
     * \param[in] an: array name
     * \param[in] vn: variable name
     * \param[in] vt: variable type
     * \param[in] en: entry name
     * \param[in] d: options passed to the brick
     * \param[in] b: if this parameter is true, the material property must be
     * strictly positive, otherwise, it must be strictly negative.
     */
    static void extractDDIF2MaterialProperty(
        std::array<BehaviourDescription::MaterialProperty, 3u>& mps,
        BehaviourDescription& bd,
        AbstractBehaviourDSL& dsl,
        const std::string& n,
        const std::string& an,
        const std::string& vn,
        const std::string& vt,
        const std::string& en,
        const std::map<std::string, tfel::utilities::Data>& d,
        const bool b) {
      using ConstantMaterialProperty =
          BehaviourDescription::ConstantMaterialProperty;
      if (d.count(n)) {
        const auto& s = d.at(n);
        mps[0] = getBehaviourDescriptionMaterialProperty(dsl, n, s);
        if (mps[0].is<ConstantMaterialProperty>()) {
          const auto& cmp = mps[0].get<ConstantMaterialProperty>();
          if (b) {
            checkIsStrictlyPositive(mps[0]);
          } else {
            checkIsStrictlyNegative(mps[0]);
          }
          mps[1] = mps[2] = mps[0];
          addParameter(bd, vn, en, 3, cmp.value);
        } else {
          addLocalVariable(bd, vt, vn, 3);
        }
      }
      if (d.count(an)) {
        const auto& s = d.at(an);
        mps = getArrayOfBehaviourDescriptionMaterialProperties<3u>(dsl, an, s);
        if (mps[0].is<ConstantMaterialProperty>()) {
          std::vector<double> values(3u);
          for (unsigned short i = 0; i != 3; ++i) {
            tfel::raise_if(!mps[i].is<ConstantMaterialProperty>(),
                           "if one  component of '" + an +
                               "' is a constant value, all components must be "
                               "constant values");
            if (b) {
              checkIsStrictlyPositive(mps[i]);
            } else {
              checkIsStrictlyNegative(mps[i]);
            }
            values[i] = mps[i].get<ConstantMaterialProperty>().value;
          }
          addParameter(bd, vn, en, 3, values);
        } else {
          addLocalVariable(bd, vt, vn, 3);
        }
      }
    }  // end of extractDDIF2MaterialProperty

    DDIF2StressPotential::DDIF2StressPotential() = default;

    std::vector<OptionDescription> DDIF2StressPotential::getOptions(
        const BehaviourDescription& bd, const bool b) const {
      auto opts = HookeStressPotential::getOptions(bd, b);
      opts.emplace_back(
          "fracture_stress", "fracture stress, assumed egal in all directions",
          OptionDescription::MATERIALPROPERTY, std::vector<std::string>{},
          std::vector<std::string>{"fracture_stresses"});
      opts.emplace_back("fracture_stresses",
                        "fracture stresses in all directions",
                        OptionDescription::ARRAYOFMATERIALPROPERTIES,
                        std::vector<std::string>{},
                        std::vector<std::string>{"fracture_stress"});
      opts.emplace_back(
          "softening_slope", "softening slope, assumed egal in all directions",
          OptionDescription::MATERIALPROPERTY, std::vector<std::string>{},
          std::vector<std::string>{"softening_slopes"});
      opts.emplace_back("softening_slopes",
                        "softening slopes in all directions",
                        OptionDescription::ARRAYOFMATERIALPROPERTIES,
                        std::vector<std::string>{},
                        std::vector<std::string>{"softening_slope"});
      opts.emplace_back(
          "fracture_energy", "fracture energy, assumed egal in all directions",
          OptionDescription::MATERIALPROPERTY, std::vector<std::string>{},
          std::vector<std::string>{"fracture_energies"});
      opts.emplace_back("fracture_energies",
                        "fracture energies in all directions",
                        OptionDescription::ARRAYOFMATERIALPROPERTIES,
                        std::vector<std::string>{},
                        std::vector<std::string>{"fracture_energy"});
      opts.emplace_back(
          "use_status_algorithm",
          "if true (default value), we use an algorithm based on statuses. "
          "Otherwise the algorithm used up to versions 3.2.1 is selected.",
          OptionDescription::BOOLEAN);
      opts.emplace_back("first_converge_on_damage",
                        "if true, the time step is set to zero to converge on "
                        "the damage state first.",
                        OptionDescription::BOOLEAN);
      opts.emplace_back("handle_pressure_on_crack_surface",
                        "if true, a pressure is applied on the crack surface",
                        OptionDescription::BOOLEAN);
      return opts;
    }  // end of DDIF2StressPotential::getOptions

    void DDIF2StressPotential::initialize(BehaviourDescription& bd,
                                          AbstractBehaviourDSL& dsl,
                                          const DataMap& d) {
      auto throw_if = [](const bool b, const std::string& m) {
        tfel::raise_if(b, "DDIF2StressPotential::DDIF2StressPotential: " + m);
      };
      // checking options
      check(d, this->getOptions(bd, true));
      //
      HookeStressPotential::initialize(bd, dsl, d);
      // undefined hypothesis
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      throw_if(bd.getElasticSymmetryType() != mfront::ISOTROPIC,
               "the DDIF2 brick is only usable for isotropic behaviours");
      // reserve some specific variables
      bd.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "ddif2bdata");
      bd.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                     "dfef_ddeel_tmp");
      // reserve some specific variables
      bd.appendToIncludes("#include\"TFEL/Material/DDIF2Base.hxx\"");
      VariableDescription ef("strain", "ef", 3u, 0u);
      ef.description = "fracture strain";
      bd.addStateVariable(uh, ef);
      bd.setEntryName(uh, "ef", "FractureStrain");
      VariableDescription efm("strain", "efm", 3u, 0u);
      efm.description = "fracture strain";
      bd.addAuxiliaryStateVariable(uh, efm);
      bd.setEntryName(uh, "efm", "MaximumFractureStrain");
      addLocalVariable(bd, "StrainStensor", "nf", 3u);
      // algorithm
      if (d.count("use_status_algorithm")) {
        const auto& b = d.at("use_status_algorithm");
        throw_if(!b.is<bool>(), "invalid type for data 'use_status_algorithm'");
        if (b.get<bool>()) {
          this->algorithm = STATUS;
        } else {
          this->algorithm = UPDATE_IMPLICIT_EQUATIONS_DURING_ITERATIONS;
        }
      }
      if (d.count("first_converge_on_damage")) {
        const auto& b = d.at("first_converge_on_damage");
        throw_if(!b.is<bool>(),
                 "invalid type for data 'first_converge_on_damage'");
        this->firstConvergeOnDamage = b.get<bool>();
      }
      // data
      throw_if((d.count("fracture_stress")) && (d.count("fracture_stresses")),
               "can't specify 'fracture_stress' and 'fracture_stresses");
      throw_if((d.count("softening_slope")) && (d.count("softening_slopes")) &&
                   (d.count("fracture_energy")) &&
                   (d.count("fracture_energies")),
               "only of of 'softening_slope', 'softening_slopes', "
               "'fracture_energy' and 'fracture_energies' must be specified");
      extractDDIF2MaterialProperty(this->sr, bd, dsl, "fracture_stress",
                                   "fracture_stresses", "sigr", "stress",
                                   "FractureStress", d, true);
      extractDDIF2MaterialProperty(this->rp, bd, dsl, "softening_slope",
                                   "softening_slopes", "Rp", "stress",
                                   "SofteningSlope", d, false);
      extractDDIF2MaterialProperty(this->gc, bd, dsl, "fracture_energy",
                                   "fracture_energies", "Gc", "real",
                                   "FractureEnergy", d, true);
      if (d.count("fracture_energy") || d.count("fracture_energies")) {
        addLocalVariable(bd, "stress", "Rp", 3);
      }
      if (d.count("handle_pressure_on_crack_surface")) {
        const auto& b = d.at("handle_pressure_on_crack_surface");
        throw_if(!b.is<bool>(),
                 "invalid type for data 'handle_pressure_on_crack_surface'");
        this->pr = b.get<bool>();
        if (this->pr) {
          addExternalStateVariable(bd, "stress", "pr",
                                   "PressureOnCrackSurface");
        }
      }
      addParameter(bd, "efm_min", "MinimalFractureStrainValue", 1.e-12);
    }  // end of DDIF2StressPotential::DDIF2StressPotential

    std::string DDIF2StressPotential::getName() const { return "DDIF2"; }

    void DDIF2StressPotential::completeVariableDeclaration(
        BehaviourDescription& bd, const AbstractBehaviourDSL& dsl) const {
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        getLogStream()
            << "DDIF2StressPotential::completeVariableDeclaration: begin\n";
      }
      HookeStressPotential::completeVariableDeclaration(bd, dsl);
      const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(dsl);
      LocalDataStructure d;
      d.name = "ddif2bdata";
      d.addVariable(uh, {"StressStensor", "sig"});
      if (this->algorithm == STATUS) {
        auto state = LocalDataStructure::Variable{};
        state.type = "DDIF2State";
        state.name = "state";
        state.asize = 3u;
        d.addVariable(uh, state);
        d.addVariable(uh, {"unsigned short", "nchanges"});
      }
      if ((idsl.getSolver().usesJacobian()) &&
          (idsl.getSolver().requiresNumericalJacobian())) {
        d.addVariable(uh, {"real", "dfe_dde"});
      }
      d.addVariable(uh, {"Stensor", "dfe_ddeel"});
      if (this->firstConvergeOnDamage) {
        d.addVariable(uh, {"time", "dt"});
        d.addVariable(uh, {"bool", "bvp"});
      }
      bd.addLocalDataStructure(d, BehaviourData::ALREADYREGISTRED);
      // modelling hypotheses supported by the brick
      const auto smh = this->getSupportedModellingHypotheses(bd, dsl);
      // modelling hypotheses supported by the behaviour
      const auto bmh = bd.getModellingHypotheses();
      for (const auto& h : bmh) {
        if (std::find(smh.begin(), smh.end(), h) == smh.end()) {
          tfel::raise(
              "DDIF2StressPotential::completeVariableDeclaration: "
              "unsupported hypothesis '" +
              ModellingHypothesis::toString(h) + "'");
        }
      }
      // fracture stresses
      if (this->sr[0].empty()) {
        addMaterialPropertyIfNotDefined(bd, "stress", "sigr", "FractureStress",
                                        3u);
      }
      // softening slopes
      if ((this->rp[0].empty()) && (this->gc[0].empty())) {
        addMaterialPropertyIfNotDefined(bd, "stress", "Rp", "SofteningSlope",
                                        3u);
      }
      if (!this->gc[0].empty()) {
        addMaterialPropertyIfNotDefined(bd, "length", "Lc", "ElementSize", 3u);
      }
      for (const auto h : bd.getModellingHypotheses()) {
        if (h != ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
          auto a = VariableDescription("real", "angl", 1u, 0u);
          a.setEntryName("AngularCoordinate");
          bd.addMaterialProperty(h, a);
        }
      }
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        getLogStream()
            << "DDIF2StressPotential::completeVariableDeclaration: end\n";
      }
    }

    void DDIF2StressPotential::endTreatment(
        BehaviourDescription& bd, const AbstractBehaviourDSL& dsl) const {
      using tfel::glossary::Glossary;
      using MaterialPropertyInput = BehaviourDescription::MaterialPropertyInput;
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        getLogStream() << "DDIF2StressPotential::endTreatment: begin\n";
      }
      HookeStressPotential::endTreatment(bd, dsl);
      auto throw_if = [](const bool b, const std::string& m) {
        tfel::raise_if(b, "DDIF2StressPotential::endTreatment: " + m);
      };
      std::function<std::string(const MaterialPropertyInput&)> ets =
          [bd, throw_if](const MaterialPropertyInput& i) -> std::string {
        if ((i.category == MaterialPropertyInput::TEMPERATURE) ||
            (i.category ==
             MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
            (i.category == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
          return "this->" + i.name + "+this->d" + i.name;
        } else if ((i.category == MaterialPropertyInput::MATERIALPROPERTY) ||
                   (i.category == MaterialPropertyInput::PARAMETER)) {
          return "this->" + i.name;
        }
        throw_if(i.category != MaterialPropertyInput::STATICVARIABLE,
                 "unsupported input type for variable '" + i.name + "'");
        return bd.getClassName() + "::" + i.name;
      };
      const auto young = [&bd]() -> std::string {
        if (bd.areElasticMaterialPropertiesDefined()) {
          return "this->young_tdt";
        }
        if (bd.hasAttribute("HookeStressPotentialBase::YoungModulus")) {
          return "this->" + bd.getAttribute<std::string>(
                                "HookeStressPotentialBase::YoungModulus");
        }
        return "this->young";
      }();
      const std::string lambda = bd.areElasticMaterialPropertiesDefined()
                                     ? "this->lambda_tdt"
                                     : "this->sebdata.lambda";
      const std::string mu = bd.areElasticMaterialPropertiesDefined()
                                 ? "this->mu_tdt"
                                 : "this->sebdata.mu";
      CodeBlock init;
      // fracture stresses
      if (!this->sr[0].empty()) {
        if (!this->sr[0].is<BehaviourDescription::ConstantMaterialProperty>()) {
          std::ostringstream ssigr;
          if (this->sr[1].empty()) {
            for (unsigned short i = 0; i != 3; ++i) {
              ssigr << "this->sigr[" << i << "] = ";
              dsl.writeMaterialPropertyEvaluation(ssigr, this->sr[i], ets);
              ssigr << ";\n";
            }
          } else {
            ssigr << "this->sigr[0] = ";
            dsl.writeMaterialPropertyEvaluation(ssigr, this->sr[0], ets);
            ssigr << ";\n";
            ssigr << "this->sigr[2] = this->sigr[1] = this->sigr[0];\n";
          }
          init.code += ssigr.str();
        }
      }
      // softening slopes
      if ((!this->rp[0].empty()) &&
          (!this->rp[0].is<BehaviourDescription::ConstantMaterialProperty>())) {
        std::ostringstream srp;
        if (!this->rp[1].empty()) {
          for (unsigned short i = 0; i != 3; ++i) {
            srp << "this->Rp[" << i << "] = ";
            dsl.writeMaterialPropertyEvaluation(srp, this->rp[i], ets);
            srp << ";\n";
          }
        } else {
          srp << "this->Rp[0] = ";
          dsl.writeMaterialPropertyEvaluation(srp, this->rp[0], ets);
          srp << ";\n";
          srp << "this->Rp[2] = this->Rp[1] = this->Rp[0];\n";
        }
        init.code += srp.str();
      }
      //  fracture energies
      if ((!this->gc[0].empty()) &&
          (!this->gc[0].is<BehaviourDescription::ConstantMaterialProperty>())) {
        std::ostringstream sgc;
        if (!this->gc[1].empty()) {
          for (unsigned short i = 0; i != 3; ++i) {
            sgc << "this->Gc[" << i << "] = ";
            dsl.writeMaterialPropertyEvaluation(sgc, this->gc[0], ets);
            sgc << ";\n";
          }
        } else {
          sgc << "this->Gc[0] = ";
          dsl.writeMaterialPropertyEvaluation(sgc, this->gc[0], ets);
          sgc << ";\n";
          sgc << "this->Gc[2] = this->Gc[1] = this->Gc[0];\n";
        }
        init.code += sgc.str();
      }
      if (!this->gc[0].empty()) {
        init.code +=
            "this->Rp[0]=-((this->Lc[0])*(this->sigr[0])*(this->sigr[0]))/"
            "(2*(this->Gc[0]));\n";
        init.code +=
            "this->Rp[1]=-((this->Lc[1])*(this->sigr[1])*(this->sigr[1]))/"
            "(2*(this->Gc[1]));\n";
        init.code +=
            "this->Rp[2]=-((this->Lc[2])*(this->sigr[2])*(this->sigr[2]))/"
            "(2*(this->Gc[2]));\n";
      }
      // init local variables
      init.code +=
          "for(unsigned short idx=0;idx!=3;++idx){\n"
          "this->nf[idx]      = Stensor(real(0));\n"
          "this->nf[idx][idx] = real(1);\n"
          "if(tfel::math::ieee754::fpclassify(this->efm[idx])!=FP_ZERO){\n"
          "this->efm[idx]=std::max(this->efm[idx],"
          "strain(this->efm_min));\n"
          "}\n"
          "}\n";
      if (this->algorithm == STATUS) {
        init.code += "this->ddif2bdata.nchanges = 0;\n";
        init.code += "this->ddif2bdata.sig=(" + lambda +
                     "*trace(this->eel))*StrainStensor::Id()+\n"
                     "                     2*" +
                     mu + "*(this->eel);\n";
        init.code +=
            "for(unsigned short idx=0;idx!=3;++idx){\n"
            "this->ddif2bdata.state[idx] = "
            "DDIF2Base::determineState(this->ddif2bdata.sig,this->nf[idx],"
            "this->efm[idx],this->sigr[idx],this->Rp[idx]);\n"
            "}\n";
      }
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, init,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
      for (const auto h : bd.getModellingHypotheses()) {
        if (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
          continue;
        }
        CodeBlock init2;
        init2.code +=
            "// change to cylindrical coordinates\n"
            "DDIF2Base::cart2cyl(this->deto,this->angl);\n"
            "DDIF2Base::cart2cyl(this->sig,this->angl);\n";
        bd.setCode(h, BehaviourData::BeforeInitializeLocalVariables, init2,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
      }
      CodeBlock prediction;
      if (this->firstConvergeOnDamage) {
        prediction.code += "this->ddif2bdata.dt = this->dt;\n";
        prediction.code += "this->ddif2bdata.bvp = false;\n";
        prediction.code += "this->dt = 0;\n";
      }
      bd.setCode(uh, BehaviourData::ComputePredictor, prediction,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      CodeBlock integrator;
      // updating the implicit equations associated with the elastic strain
      const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(dsl);
      integrator.code =
          "feel += "
          "((this->def)[0])*(this->nf[0])+\n"
          "((this->def)[1])*(this->nf[1])+\n"
          "((this->def)[2])*(this->nf[2]);\n";
      if ((idsl.getSolver().usesJacobian()) &&
          (!idsl.getSolver().requiresNumericalJacobian())) {
        integrator.code +=
            "this->dfeel_ddef(0)(0) = real(1);\n"
            "this->dfeel_ddef(1)(1) = real(1);\n"
            "this->dfeel_ddef(2)(2) = real(1);\n";
      }
      // stress at the end of the time step
      integrator.code += "this->ddif2bdata.sig=(" + lambda +
                         "*trace(this->eel+deel))*StrainStensor::Id()+\n"
                         "                     2*" +
                         mu + "*(this->eel+this->deel);\n";
      auto dfe_dde = std::string{};
      auto dfe_ddeel = std::string{};
      if ((idsl.getSolver().usesJacobian()) &&
          (!idsl.getSolver().requiresNumericalJacobian())) {
        dfe_dde = "dfef_ddef(idx,idx)";
        dfe_ddeel = "dfef_ddeel_tmp";
      } else {
        dfe_dde = "ddif2bdata.dfe_dde";
        dfe_ddeel = "ddif2bdata.dfe_ddeel";
      }
      integrator.code += "for(unsigned short idx=0;idx!=3;++idx){\n";
      if ((idsl.getSolver().usesJacobian()) &&
          (!idsl.getSolver().requiresNumericalJacobian())) {
        integrator.code += "auto dfef_ddeel_tmp = dfef_ddeel(idx);\n";
      }
      if (this->algorithm == STATUS) {
        integrator.code +=
            "DDIF2Base::buildImplicitEquation(fef(idx), " + dfe_dde + ", " +
            dfe_ddeel +
            ", this->ddif2bdata.state[idx], this->ddif2bdata.sig,"
            "this->nf[idx], this->efm[idx], this->ef[idx], def[idx], " +
            young + ", " + lambda + "," + mu + ", " +
            "this->sigr[idx], this->Rp[idx]);";
      } else {
        // implicit equation associated with the crack strains
        if (this->pr) {
          integrator.code +=
              "DDIF2Base::treatFracture(" + dfe_ddeel + "," + dfe_dde +
              ",fef(idx),\n"
              "this->efm(idx),this->ef(idx),(this->def)(idx),\n"
              "this->ddif2bdata.sig,this->nf(idx),\n"
              "this->sigr[idx],this->Rp[idx],\n" +
              young + ", " + lambda + ", " + mu + ",this->pr+this->dpr);\n";
        } else {
          integrator.code += "DDIF2Base::treatFracture(" + dfe_ddeel + "," +
                             dfe_dde +
                             ",fef(idx),\n"
                             "this->efm(idx),this->ef(idx),(this->def)(idx),\n"
                             "this->ddif2bdata.sig,this->nf(idx),\n"
                             "this->sigr[idx],this->Rp[idx],\n" +
                             young + "," + lambda + ',' + mu + ");\n";
        }
      }
      integrator.code += "}\n";
      bd.setCode(uh, BehaviourData::Integrator, integrator,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
      /* post-processing checks */
      CodeBlock acc;
      acc.code = "if (converged) {\n";
      if (this->algorithm == STATUS) {
        acc.code +=
            "this->ddif2bdata.sig=(" + lambda +
            "*trace(this->eel+deel))*StrainStensor::Id()+\n"
            "2*" +
            mu +
            "*(this->eel+this->deel);\n"
            "for (unsigned short idx = 0; idx != 3; ++idx) {\n"
            "converged = DDIF2Base::checkStateConsistency("
            "this->ddif2bdata.state[idx],this->ddif2bdata.sig, "
            "this->nf[idx], this->efm[idx], this->ef[idx] + this->def[idx], "
            "this->sigr[idx],2 * " +
            young +
            "* this->epsilon, 2 * (this->epsilon));\n"
            "if (!converged) {\n"
            "if (this->ddif2bdata.nchanges < 10) {\n"
            "this->iter = 1;\n"
            "++(this->ddif2bdata.nchanges);\n"
            "} // end of if (this->ddif2bdata.nchanges < 10)\n"
            "return;\n"
            "} // end of if (!converged)\n"
            "} // end of for (unsigned short idx = 0; idx != 3; ++idx)\n";
      }
      if (this->firstConvergeOnDamage) {
        acc.code +=
            "if (!this->ddif2bdata.bvp) {\n"
            "this->ddif2bdata.bvp = true;\n"
            "this->dt = this->ddif2bdata.dt;\n"
            "converged = false;\n"
            "} // end of (!this->ddif2bdata.bvp)\n";
      }
      acc.code += "} // end of if (converged)\n";
      bd.setCode(uh, BehaviourData::AdditionalConvergenceChecks, acc,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      /* update auxiliary state variables */
      for (const auto h : bd.getModellingHypotheses()) {
        CodeBlock uasv;
        uasv.code =
            "this->efm[0]=std::max(this->efm[0],this->ef[0]);\n"
            "this->efm[1]=std::max(this->efm[1],this->ef[1]);\n"
            "this->efm[2]=std::max(this->efm[2],this->ef[2]);\n";
        if (h != ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
          uasv.code +=
              "// change to cartesian coordinates\n"
              "DDIF2Base::cyl2cart(this->sig,this->angl);\n";
        }
        bd.setCode(h, BehaviourData::UpdateAuxiliaryStateVariables, uasv,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
      }
    }  // end of DDIF2StressPotential::endTreatment

    DDIF2StressPotential::~DDIF2StressPotential() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
