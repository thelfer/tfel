/*!
 * \file   AmstrongFrederickKinematicHardeningRule.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04/04/2018
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
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/AmstrongFrederickKinematicHardeningRule.hxx"

namespace mfront {

  namespace bbrick {

    std::vector<OptionDescription> AmstrongFrederickKinematicHardeningRule::getOptions()
        const {
      auto opts = KinematicHardeningRuleBase::getOptions();
      opts.emplace_back("D", "back-strain callback coefficient",
                        OptionDescription::MATERIALPROPERTY);
      return opts;
    }  // end of AmstrongFrederickKinematicHardeningRule::getOptions()

    void AmstrongFrederickKinematicHardeningRule::initialize(
        BehaviourDescription& bd,
        AbstractBehaviourDSL& dsl,
        const std::string& fid,
        const std::string& kid,
        const DataMap& d) {
      KinematicHardeningRuleBase::initialize(bd, dsl, fid, kid, d);
      const auto Dn = KinematicHardeningRule::getVariableId("D", fid, kid);
      // kinematic moduli
      tfel::raise_if(d.count("D") == 0,
                     "AmstrongFrederickKinematicHardeningRule::initialize: "
                     "material property 'D' is not defined");
      this->D = getBehaviourDescriptionMaterialProperty(dsl, "D", d.at("D"));
      declareParameterOrLocalVariable(bd, this->D, Dn);
    }  // end of AmstrongFrederickKinematicHardeningRule::initialize

    void AmstrongFrederickKinematicHardeningRule::endTreatment(
        BehaviourDescription& bd,
        const AbstractBehaviourDSL& dsl,
        const std::string& fid,
        const std::string& kid) const {
      KinematicHardeningRuleBase::endTreatment(bd, dsl, fid, kid);
      auto mts = getMiddleOfTimeStepModifier(bd);
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if (!this->D.is<BehaviourDescription::ConstantMaterialProperty>()) {
        const auto Dn = KinematicHardeningRule::getVariableId("D", fid, kid);
        CodeBlock i;
        std::ostringstream mps;
        mps << "this->" + Dn + " = ";
        dsl.writeMaterialPropertyEvaluation(mps, this->D, mts);
        mps << ";\n";
        i.code += mps.str();
        bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
    }  // end of AmstrongFrederickKinematicHardeningRule::endTreatment

    std::string
    AmstrongFrederickKinematicHardeningRule::buildBackStrainImplicitEquations(
        const BehaviourDescription& bd,
        const StressPotential& sp,
        const std::string& fid,
        const std::string& kid,
        const bool b) const {
      const auto an = KinematicHardeningRule::getVariableId("a", fid, kid);
      const auto Cn = KinematicHardeningRule::getVariableId("C", fid, kid);
      const auto Dn = KinematicHardeningRule::getVariableId("D", fid, kid);
      auto c = std::string{};
      c = "f" + an + " -= ";
      c += "(this->dp" + fid + ")*(dseq" + fid + "_ds" + fid + "-(this->" + Dn +
           ")*((this->" + an + ")+(this->theta)*(this->d" + an + ")));\n";
      if (b) {
        c += "df" + an + "_ddp" + fid + " = -(dseq" + fid + "_ds" + fid +
             "-(this->" + Dn + ")*((this->" + an + ")+(this->theta)*(this->d" +
             an + ")));\n";
        c += sp.computeDerivatives(bd, an, "-(this->dp" + fid + ")*d2seq" +
                                               fid + "_ds" + fid + "ds" + fid);
        c += "df" + an + "_dd" + an + " += ";
        c += "(this->theta)*(this->dp" + fid + ")*((2*(this->" + Cn +
             ")/3)*d2seq" + fid + "_ds" + fid + "ds" + fid + "+(this->" + Dn +
             ")*Stensor4::Id());\n";
      }
      return c;
    }  // end of
       // AmstrongFrederickKinematicHardeningRule::buildBackStrainImplicitEquations

    AmstrongFrederickKinematicHardeningRule::
        ~AmstrongFrederickKinematicHardeningRule() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
