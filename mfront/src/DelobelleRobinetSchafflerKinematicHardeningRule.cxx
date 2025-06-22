/*!
 * \file   mfront/src/DelobelleRobinetSchafflerKinematicHardeningRule.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04/04/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/StressCriterion.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/DelobelleRobinetSchafflerKinematicHardeningRule.hxx"

namespace mfront::bbrick {

  std::vector<OptionDescription>
  DelobelleRobinetSchafflerKinematicHardeningRule::getOptions() const {
    auto opts = KinematicHardeningRuleBase::getOptions();
    opts.emplace_back("D", "back-strain callback coefficient",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("f", "memory coefficient",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("a0",
                      "normalisation coefficient of the "
                      "equivalent back strain",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("m", "memory exponent",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("Ec",
                      "coefficients of the linear transformation of the "
                      "inelastic strain rate",
                      OptionDescription::ARRAYOFMATERIALPROPERTIES);
    opts.emplace_back("Rs", "First linear transformation coefficients",
                      OptionDescription::ARRAYOFMATERIALPROPERTIES);
    opts.emplace_back("Rs", "First linear transformation coefficients",
                      OptionDescription::ARRAYOFMATERIALPROPERTIES);
    opts.emplace_back("Rd", "Second linear transformation coefficients",
                      OptionDescription::ARRAYOFMATERIALPROPERTIES);
    return opts;
  }  // end of getOptions()

  void DelobelleRobinetSchafflerKinematicHardeningRule::initialize(
      BehaviourDescription& bd,
      AbstractBehaviourDSL& dsl,
      const std::string& fid,
      const std::string& kid,
      const DataMap& d) {
    KinematicHardeningRuleBase::initialize(bd, dsl, fid, kid, d);
    const auto Dn = KinematicHardeningRule::getVariableId("D", fid, kid);
    const auto fn = KinematicHardeningRule::getVariableId("f", fid, kid);
    const auto a0n = KinematicHardeningRule::getVariableId("a0", fid, kid);
    const auto mn = KinematicHardeningRule::getVariableId("m", fid, kid);
    const auto Ecn = KinematicHardeningRule::getVariableId("Ec", fid, kid);
    const auto Rdn = KinematicHardeningRule::getVariableId("Rd", fid, kid);
    const auto Rsn = KinematicHardeningRule::getVariableId("Rs", fid, kid);
    this->Ec = extractHillTensorCoefficients(
        dsl, bd, d, "Ec", Ecn + "_coefficients",
        "InelasticStrainRateLinearTransformationCoefficients");
    this->Rd = extractHillTensorCoefficients(
        dsl, bd, d, "Rd", Rdn + "_coefficients",
        "FirstBackStrainRateLinearTransformationCoefficients");
    this->Rs = extractHillTensorCoefficients(
        dsl, bd, d, "Rs", Rsn + "_coefficients",
        "SecondBackStrainRateLinearTransformationCoefficients");
    for (const auto& n : {Ecn, Rdn, Rsn}) {
      addLocalVariable(bd, "Stensor4", n);
    }
    //
    if (d.count("D") != 0) {
      this->D = getBehaviourDescriptionMaterialProperty(dsl, "D", d.at("D"));
    } else {
      BehaviourDescription::ConstantMaterialProperty mp;
      mp.value = 1;
      this->D = mp;
    }
    declareParameterOrLocalVariable(bd, this->D, "stress", Dn);
    //
    if (d.count("f") != 0) {
      this->f = getBehaviourDescriptionMaterialProperty(dsl, "f", d.at("f"));
    } else {
      BehaviourDescription::ConstantMaterialProperty mp;
      mp.value = 1;
      this->f = mp;
    }
    declareParameterOrLocalVariable(bd, this->f, "stress", fn);
    //
    if (d.count("a0") != 0) {
      this->a0 = getBehaviourDescriptionMaterialProperty(dsl, "a0", d.at("a0"));
    } else {
      BehaviourDescription::ConstantMaterialProperty mp;
      mp.value = 1;
      this->a0 = mp;
    }
    declareParameterOrLocalVariable(bd, this->a0, "strain", a0n);
    //
    tfel::raise_if(
        d.count("m") == 0,
        "DelobelleRobinetSchafflerKinematicHardeningRule::initialize: "
        "'m' material property undefined");
    this->m = getBehaviourDescriptionMaterialProperty(dsl, "m", d.at("m"));
    declareParameterOrLocalVariable(bd, this->m, "stress", mn);
    //
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    bd.reserveName(uh, KinematicHardeningRule::getVariableId("aeq", fid, kid));
    bd.reserveName(uh,
                   KinematicHardeningRule::getVariableId("aeq_a0_m", fid, kid));
    bd.reserveName(uh, KinematicHardeningRule::getVariableId("Rs_a", fid, kid));
    bd.reserveName(uh, KinematicHardeningRule::getVariableId("Rd_a", fid, kid));
    bd.reserveName(uh, KinematicHardeningRule::getVariableId("Ec_n", fid, kid));
    bd.reserveName(uh, KinematicHardeningRule::getVariableId("iaeq", fid, kid));
    bd.reserveName(uh, KinematicHardeningRule::getVariableId("na", fid, kid));
    bd.reserveName(uh,
                   KinematicHardeningRule::getVariableId("dna_da", fid, kid));
  }  // end of initialize

  void DelobelleRobinetSchafflerKinematicHardeningRule::endTreatment(
      BehaviourDescription& bd,
      const AbstractBehaviourDSL& dsl,
      const std::string& fid,
      const std::string& kid) const {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    KinematicHardeningRuleBase::endTreatment(bd, dsl, fid, kid);
    const auto Dn = KinematicHardeningRule::getVariableId("D", fid, kid);
    const auto fn = KinematicHardeningRule::getVariableId("f", fid, kid);
    const auto a0n = KinematicHardeningRule::getVariableId("a0", fid, kid);
    const auto mn = KinematicHardeningRule::getVariableId("m", fid, kid);
    const auto Ecn = KinematicHardeningRule::getVariableId("Ec", fid, kid);
    const auto Rdn = KinematicHardeningRule::getVariableId("Rd", fid, kid);
    const auto Rsn = KinematicHardeningRule::getVariableId("Rs", fid, kid);
    auto c = generateMaterialPropertyInitializationCode(dsl, bd, Dn, this->D);
    c += generateMaterialPropertyInitializationCode(dsl, bd, fn, this->f);
    c += generateMaterialPropertyInitializationCode(dsl, bd, a0n, this->a0);
    c += generateMaterialPropertyInitializationCode(dsl, bd, mn, this->m);
    c += generateMaterialPropertiesInitializationCode(
        dsl, bd, Ecn + "_coefficients", this->Ec);
    c += generateMaterialPropertiesInitializationCode(
        dsl, bd, Rdn + "_coefficients", this->Rd);
    c += generateMaterialPropertiesInitializationCode(
        dsl, bd, Rsn + "_coefficients", this->Rs);
    const auto oac = [&bd]() -> std::string {
      if (bd.getOrthotropicAxesConvention() ==
          tfel::material::OrthotropicAxesConvention::PIPE) {
        return "tfel::material::OrthotropicAxesConvention::PIPE";
      } else if (bd.getOrthotropicAxesConvention() ==
                 tfel::material::OrthotropicAxesConvention::PLATE) {
        return "tfel::material::OrthotropicAxesConvention::PLATE";
      } else {
        tfel::raise_if(
            bd.getOrthotropicAxesConvention() !=
                tfel::material::OrthotropicAxesConvention::DEFAULT,
            "DelobelleRobinetSchafflerKinematicHardeningRule::endTreatment: "
            "internal error, unsupported orthotropic axes convention");
        for (const auto mh : bd.getDistinctModellingHypotheses()) {
          tfel::raise_if(
              mh != ModellingHypothesis::TRIDIMENSIONAL,
              "DelobelleRobinetSchafflerKinematicHardeningRule::endTreatment: "
              "an orthotropic axes convention must be choosen when "
              "defining a stress free expansion in behaviours "
              "which shall be valid in other modelling hypothesis "
              "than 'Tridimensional'.\n"
              "Either restrict the validity of the behaviour to "
              "'Tridimensional' (see @ModellingHypothesis) or "
              "choose and orthotropic axes convention as on option "
              "to the @OrthotropicBehaviour keyword");
        }
      }
      return "tfel::material::OrthotropicAxesConvention::DEFAULT";
    }();
    for (const auto& t : {Ecn, Rdn, Rsn}) {
      c += "this->" + t + " = ";
      c += "makeHillTensor<hypothesis, " + oac + ", real>(";
      c += "this->" + t + "_coefficients[0], \n";
      c += "this->" + t + "_coefficients[1], \n";
      c += "this->" + t + "_coefficients[2], \n";
      c += "this->" + t + "_coefficients[3], \n";
      c += "this->" + t + "_coefficients[4], \n";
      c += "this->" + t + "_coefficients[5]);\n";
    }
    CodeBlock i;
    i.code = c;
    bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
               BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
  }  // end of endTreatment

  std::string DelobelleRobinetSchafflerKinematicHardeningRule::
      buildBackStrainImplicitEquations(
          const BehaviourDescription& bd,
          const StressPotential& sp,
          const StressCriterion& fc,
          const std::vector<std::shared_ptr<KinematicHardeningRule>>& khrs,
          const std::string& fid,
          const std::string& kid,
          const bool b) const {
    auto get_member_id = [&fid, &kid](const char* const n) {
      return "this->" + KinematicHardeningRule::getVariableId(n, fid, kid);
    };
    const auto an = KinematicHardeningRule::getVariableId("a", fid, kid);
    const auto aeqn = KinematicHardeningRule::getVariableId("aeq", fid, kid);
    const auto nan = KinematicHardeningRule::getVariableId("na", fid, kid);
    const auto dna_da =
        KinematicHardeningRule::getVariableId("dna_da", fid, kid);
    const auto iaeqn = KinematicHardeningRule::getVariableId("iaeq", fid, kid);
    const auto aeq_a0_m =
        KinematicHardeningRule::getVariableId("aeq_a0_m", fid, kid);
    const auto Ec_n = KinematicHardeningRule::getVariableId("Ec_n", fid, kid);
    const auto Rd_a = KinematicHardeningRule::getVariableId("Rd_a", fid, kid);
    const auto Rs_a = KinematicHardeningRule::getVariableId("Rs_a", fid, kid);
    const auto Dn = get_member_id("D");
    const auto mn = get_member_id("m");
    const auto a0n = get_member_id("a0");
    const auto fn = get_member_id("f");
    const auto Ecn = get_member_id("Ec");
    const auto Rdn = get_member_id("Rd");
    const auto Rsn = get_member_id("Rs");
    const auto dpn = "dp" + fid;
    const auto n = "n" + fid;
    const auto a_mts = "this->" + an + " + " + bd.getClassName() +
                       "::theta * (this->d" + an + ")";
    auto c = std::string{};
    c += "const auto " + aeqn + " = ";
    c += "std::sqrt(std::max((" + a_mts + ") | (" + Rsn + " * (" + a_mts +
         ")), ";
    c += "                   strain(0) * strain(0)));\n";
    c += "const auto " + iaeqn + " = ";
    c += "1/std::max(strain(1e-14)," + aeqn + ");\n";
    c += "const auto " + Ec_n + " = eval((" + Ecn + ")  * " + n + ");\n";
    c += "const auto " + Rd_a + " = ";
    c += "eval((" + Rdn + ") * (" + a_mts + "));\n";
    c += "const auto " + Rs_a + " = ";
    c += "eval((" + Rsn + ") * (" + a_mts + "));\n";
    c += "const auto " + nan + " = " + Rs_a + " * " + iaeqn + ";\n";
    c += "const auto " + aeq_a0_m;
    c += " = pow(" + aeqn + " / (" + a0n + "), " + mn + ");\n";
    c += "f" + an + " -= ";
    c += "   (" + dpn + ") * " + Ec_n;
    c += " - (" + Dn + ")  * (" + dpn + ") * " + Rd_a;
    c += " - (this->dt) * (" + fn + ")  * " + aeq_a0_m + " * " + nan;
    c += ";\n";
    if (b) {
      c += "df" + an + "_d" + dpn + " = - " + Ec_n + " + ";
      c += "(" + Dn + ") * " + Rd_a + ";\n";
      const auto mdf_ds =
          "(this->dp" + fid + ") * (" + Ecn + ") * d" + n + "_ds" + fid;
      c += sp.generateImplicitEquationDerivatives(
          bd, "StrainStensor", an, "-" + mdf_ds, fc.isNormalDeviatoric());
      auto kid2 = decltype(khrs.size()){};
      for (const auto& khr : khrs) {
        c += khr->generateImplicitEquationDerivatives(an, mdf_ds, fid,
                                                      std::to_string(kid2));
        ++kid2;
      }
      c += "const auto " + dna_da + " = ";
      c += "(" + Rsn + " - ((" + nan + ") ^ (" + nan + "))) * " + iaeqn + ";\n";
      const auto theta = bd.getClassName() + "::theta";
      c += "df" + an + "_dd" + an + " += ";
      c += "(" + theta + ") * (" + dpn + ") * (" + Dn + ") * " + Rdn;
      c += " + (this->dt) * (" + fn + ")  * " + aeq_a0_m + " * " + theta;
      c += " * (" + dna_da + " + (" + mn + ") * ((" + nan + ")^(" + nan +
           ")) * " + iaeqn + ");\n";
    }
    return c;
  }  // end of buildBackStrainImplicitEquations

  DelobelleRobinetSchafflerKinematicHardeningRule::
      ~DelobelleRobinetSchafflerKinematicHardeningRule() = default;

}  // end of namespace mfront::bbrick
