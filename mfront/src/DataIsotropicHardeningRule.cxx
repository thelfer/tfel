/*!
 * \file   mfront/src/DataIsotropicHardeningRule.cxx
 * \brief
 * \author Thomas Helfer
 * \date   25/07/2022
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/DataIsotropicHardeningRule.hxx"

namespace mfront::bbrick {

  void DataIsotropicHardeningRule::initialize(BehaviourDescription& bd,
                                              AbstractBehaviourDSL& dsl,
                                              const std::string& fid,
                                              const std::string& id,
                                              const DataMap& d) {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    // interpolation type
    enum InterpolationType {
      LINEAR_INTERPOLATION,
      CUBLIC_SPLINE_INTERPOLATION
    };
    const auto itype = [&d] {
      if (d.count("interpolation")) {
        const auto& opt = d.at("interpolation");
        if (!opt.is<std::string>()) {
          tfel::raise(
              "DataIsotropicHardeningRule::initialize: "
              "unsupported type for option 'interpolation' "
              "(must be a string)");
        }
        const auto& i = opt.get<std::string>();
        if (i == "linear") {
          return LINEAR_INTERPOLATION;
        } else if (i == "cubic_spline") {
          return CUBLIC_SPLINE_INTERPOLATION;
        } else {
          tfel::raise(
              "DataIsotropicHardeningRule::initialize: "
              "unsupported interpolation '" +
              i + "'");
        }
      }
      return LINEAR_INTERPOLATION;
    }();
    const auto etype = [&d] {
      if (d.count("extrapolation")) {
        const auto& opt = d.at("extrapolation");
        if (opt.is<bool>()) {
          return opt.get<bool>();
        } else if (opt.is<std::string>()) {
          const auto& v = opt.get<std::string>();
          if ((v == "bound_to_last_value") || (v == "constant")) {
            return false;
          } else {
            tfel::raise(
                "DataIsotropicHardeningRule::initialize: "
                "unsupported value for option 'extrapolation' "
                "(expected 'bound_to_last_value' or 'constant')");
          }
        } else {
          tfel::raise(
              "DataIsotropicHardeningRule::initialize: "
              "unsupported type for option 'extrapolation' "
              "(must be a string or a boolean value)");
        }
      }
      return true;
    }();
    // values
    if (!d.count("values")) {
      tfel::raise(
          "DataIsotropicHardeningRule::initialize: "
          "no values specified");
    }
    const auto& opt = d.at("values");
    if (!opt.is<std::map<double, double>>()) {
      tfel::raise(
          "DataIsotropicHardeningRule::initialize: "
          "invalid type for option 'values'");
    }
    const auto& values = opt.get<std::map<double, double>>();
    //
    
    //
    const auto Rel = id.empty() ? "Rel" + fid : "Rel" + fid + "_" + id;
    const auto R = id.empty() ? "R" + fid : "R" + fid + "_" + id;
    const auto dR = "d" + R + "_ddp" + fid;
    bd.reserveName(uh, Rel);
    bd.reserveName(uh, R);
    bd.reserveName(uh, dR);
  }  // end of initialize

  std::vector<OptionDescription> DataIsotropicHardeningRule::getOptions()
      const {
    std::vector<OptionDescription> opts;
    opts.emplace_back("R0", "Yield strength",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("Rinf", "Ultimate Yield strength",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("b", "Parameter describing the hardening evolution",
                      OptionDescription::MATERIALPROPERTY);
    return opts;
  }  // end of getOptions

  std::string DataIsotropicHardeningRule::computeElasticPrediction(
      const BehaviourDescription&,
      const std::string& fid,
      const std::string& id) const {
    const auto local_id = id.empty() ? fid : fid + "_" + id;
    const auto Rel = "Rel" + local_id;
    return "const auto " + Rel +  //
           " = this->computeYieldRadius" + local_id + "(this->p" + fid + ");\n";
  }  // end of computeElasticPrediction

  std::string DataIsotropicHardeningRule::computeElasticLimit(
      const BehaviourDescription&,
      const std::string& fid,
      const std::string& id) const {
    const auto local_id = id.empty() ? fid : fid + "_" + id;
    const auto R = "R" + local_id;
    const auto p = "this->p" + fid + " + (this->theta) * (this->dp" + fid + ")";
    return "const auto " + R +  //
           " = this->computeYieldRadius" + local_id + "(" + p + ");\n";
  }  // end of computeElasticLimit

  std::string DataIsotropicHardeningRule::computeElasticLimitAndDerivative(
      const BehaviourDescription&,
      const std::string& fid,
      const std::string& id) const {
    const auto local_id = id.empty() ? fid : fid + "_" + id;
    const auto R = "R" + local_id;
    const auto dR = "d" + R + "_ddp" + fid;
    const auto p = "this->p" + fid + " + (this->theta) * (this->dp" + fid + ")";
    return "const auto [" + R + ", " + dR + "]" +  //
           " = this->computeYieldRadiusAndDerivative" + local_id + "(" + p + ");\n";
  }  // end of computeElasticLimitAndDerivative

  void DataIsotropicHardeningRule::endTreatment(BehaviourDescription& bd,
                                                const AbstractBehaviourDSL& dsl,
                                                const std::string& fid,
                                                const std::string& id) const {
    auto mts = getMiddleOfTimeStepModifier(bd);
    // computation of the material properties
    if ((!this->R0.is<BehaviourDescription::ConstantMaterialProperty>()) ||
        (!this->Rinf.is<BehaviourDescription::ConstantMaterialProperty>()) ||
        (!this->b.is<BehaviourDescription::ConstantMaterialProperty>())) {
      constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      CodeBlock i;
      std::ostringstream mps;
      if (!this->R0.is<BehaviourDescription::ConstantMaterialProperty>()) {
        const auto R0n = IsotropicHardeningRule::getVariableId("R0", fid, id);
        mps << "this->" + R0n + " = ";
        dsl.writeMaterialPropertyEvaluation(mps, this->R0, mts);
        mps << ";\n";
      }
      if (!this->Rinf.is<BehaviourDescription::ConstantMaterialProperty>()) {
        const auto Rin = IsotropicHardeningRule::getVariableId("Rinf", fid, id);
        mps << "this->" + Rin + " = ";
        dsl.writeMaterialPropertyEvaluation(mps, this->Rinf, mts);
        mps << ";\n";
      }
      if (!this->b.is<BehaviourDescription::ConstantMaterialProperty>()) {
        const auto bn = IsotropicHardeningRule::getVariableId("b", fid, id);
        mps << "this->" + bn + " = ";
        dsl.writeMaterialPropertyEvaluation(mps, this->b, mts);
        mps << ";\n";
      }
      i.code += mps.str();
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
  }  // end of endTreatment

  DataIsotropicHardeningRule::~DataIsotropicHardeningRule() = default;

}  // end of namespace mfront::bbrick
