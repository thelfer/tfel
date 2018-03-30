/*!
 * \file   NortonInelasticFlow.cxx
 * \brief
 * \author Thomas Helfer
 * \date   28/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/StressCriterion.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/NortonInelasticFlow.hxx"

namespace mfront {

  namespace bbrick {

    void NortonInelasticFlow::initialize(BehaviourDescription& bd,
                                         AbstractBehaviourDSL& dsl,
                                         const std::string& id,
                                         const DataMap& d) {
      using namespace tfel::glossary;
      auto get_mp = [&dsl, &bd, &d](const char* const mp) {
        if (d.count(mp) == 0) {
          tfel::raise(
              "NortonInelasticFlow::initialize: "
              "material property '" +
              std::string(mp) + "' is not defined");
        }
        return getBehaviourDescriptionMaterialProperty(bd, dsl, mp, d.at(mp),
                                                       true, true);
      };
      // checking options
      mfront::bbrick::check(d, this->getOptions());
      // the base class
      InelasticFlowBase::initialize(bd, dsl, id, d);
      // Norton flow options
      this->K = get_mp("K");
      this->n = get_mp("n");
      if (!this->K.is<BehaviourDescription::ConstantMaterialProperty>()) {
        addLocalVariable(bd, "stress", "K" + id);
      }
      if (!this->n.is<BehaviourDescription::ConstantMaterialProperty>()) {
        addLocalVariable(bd, "real", "n" + id);
      }
      if (id.empty()) {
        addStateVariable(bd, "strain", "p",
                         Glossary::EquivalentViscoplasticStrain);
      } else {
        addStateVariable(bd, "strain", "p" + id,
                         static_cast<const std::string&>(
                             Glossary::EquivalentViscoplasticStrain) +
                             id);
      }
    }  // end of NortonInelasticFlow::initialize

    void NortonInelasticFlow::completeVariableDeclaration(
        BehaviourDescription&,
        const AbstractBehaviourDSL&,
        const std::string&) const {
    }  // end of NortonInelasticFlow::completeVariableDeclaration

    void NortonInelasticFlow::endTreatment(BehaviourDescription& bd,
                                           const AbstractBehaviourDSL& dsl,
                                           const StressPotential& sp,
                                           const std::string& id) const {
      const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(dsl);
      using Modifier = std::function<std::string(const MaterialPropertyInput&)>;
      Modifier mts = [&bd](const MaterialPropertyInput& i) -> std::string {
        if ((i.category == MaterialPropertyInput::TEMPERATURE) ||
            (i.category ==
             MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
            (i.category == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
          return "this->" + i.name + "+theta*(this->d" + i.name + ')';
        } else if ((i.category == MaterialPropertyInput::MATERIALPROPERTY) ||
                   (i.category == MaterialPropertyInput::STATEVARIABLE) ||
                   (i.category == MaterialPropertyInput::PARAMETER)) {
          return "this->" + i.name;
        } else if (i.category == MaterialPropertyInput::STATICVARIABLE) {
          return bd.getClassName() + "::" + i.name;
        } else {
          tfel::raise(
              "NortonInelasticFlow::endTreatment: "
              "unsupported input type for variable '" +
              i.name + "'");
        }
      };
      // computation of the material properties
      if ((!this->K.is<BehaviourDescription::ConstantMaterialProperty>()) &&
          (!this->n.is<BehaviourDescription::ConstantMaterialProperty>())) {
        CodeBlock i;
        std::ostringstream mps;
        if (!this->K.is<BehaviourDescription::ConstantMaterialProperty>()) {
          mps << "this->K" + id + " = ";
          dsl.writeMaterialPropertyEvaluation(mps, this->K, mts);
          mps << ";\n";
        }
        if (!this->n.is<BehaviourDescription::ConstantMaterialProperty>()) {
          mps << "this->n" + id + " = ";
          dsl.writeMaterialPropertyEvaluation(mps, this->n, mts);
          mps << ";\n";
        }
        i.code += mps.str();
        bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
      // implicit equation associated with the elastic strain
      CodeBlock ib;
      if ((idsl.getSolver().usesJacobian()) &&
          (!idsl.getSolver().requiresNumericalJacobian())) {
        ib.code += this->sc->computeNormalDerivative(id);
          } else {
            ib.code += this->sc->computeNormal(id);
          }
          ib.code += "feel += this->dp" + id + "* dseq" + id + "_ds;\n";
          if ((idsl.getSolver().usesJacobian()) &&
              (!idsl.getSolver().requiresNumericalJacobian())) {
            // jacobian terms
            ib.code += "dfeel_ddp" + id + " = dseq" + id + "_ds;\n";
            for (const auto& v : sp.getStressVariables()) {
              if ((id == "0") && (v.name != "eel")) {
                ib.code += "dfeel_dd" + v.name + "=";
              } else {
                ib.code += "dfeel_dd" + v.name + "+=";
              }
              ib.code += "dp" + id + "*d2seq" + id + "_ds2" + "*dsig_dd" +
                         v.name + ";\n";
            }
          }
          if ((idsl.getSolver().usesJacobian()) &&
              (!idsl.getSolver().requiresNumericalJacobian())) {
            ib.code += "const auto dvp" + id + " = ";
            ib.code += "pow(seq" + id + "/(this->K" + id + "),this->n" + id +
                       "-1)/this->K" + id + ";\n";
            ib.code += "fp" + id + " -= dvp*seq*(this->dt);\n";
            for (const auto& v : sp.getStressVariables()) {
              ib.code += "dfp" + id + "_dd" + v.name + " = -(this->n" + id +
                         ")*dvp*(this->dt)*dseq" + id + "_ds*dsig_dd" + v.name +
                         ";\n";
            }
          } else {
            ib.code += "fp" + id + " -= ";
            ib.code += "pow(seq" + id + "/(this->K" + id + "),this->n" + id +
                       ")*dt;\n";
          }
          bd.setCode(uh, BehaviourData::Integrator, ib,
                     BehaviourData::CREATEORAPPEND,
                     BehaviourData::AT_BEGINNING);
        }  // end of NortonInelasticFlow::endTreatment

        std::vector<OptionDescription> NortonInelasticFlow::getOptions() const {
          auto opts = InelasticFlowBase::getOptions();
          opts.emplace_back("K", "Norton coefficient",
                            OptionDescription::MATERIALPROPERTY);
          opts.emplace_back("n", "Norton exponent",
                            OptionDescription::MATERIALPROPERTY);
          return opts;
        }  // end of NortonInelasticFlow::getOptions

        NortonInelasticFlow::~NortonInelasticFlow() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
