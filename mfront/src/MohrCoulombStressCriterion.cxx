/*!
 * \file   mfront/src/MohrCoulombStressCriterion.cxx
 * \brief
 * \author Thomas Helfer
 * \date   10/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/MohrCoulombStressCriterion.hxx"

namespace mfront {

  namespace bbrick {

    std::vector<OptionDescription> MohrCoulombStressCriterion::getOptions()
        const {
      auto opts = StressCriterionBase::getOptions();
      opts.emplace_back("c", "cohesion", OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("phi", "friction angle or dilatancy angle",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("lodeT",
                        "transition angle as defined by Abbo and Sloan",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("a", "tension cuff-off parameter",
                        OptionDescription::MATERIALPROPERTY);
      return opts;
    }  // end of MohrCoulombStressCriterion::getOptions()

    std::vector<mfront::BehaviourSymmetryType>
    MohrCoulombStressCriterion::getSupportedBehaviourSymmetries() const {
      return {mfront::ISOTROPIC, mfront::ORTHOTROPIC};
    }  // end of MohrCoulombStressCriterion::getSupportedBehaviourSymmetries()

    void MohrCoulombStressCriterion::initialize(BehaviourDescription& bd,
                                                AbstractBehaviourDSL& dsl,
                                                const std::string& id,
                                                const DataMap& d,
                                                const Role r) {
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      StressCriterionBase::initialize(bd, dsl, id, d, r);
      bd.appendToIncludes(
          "#include\"TFEL/Material/MohrCoulombYieldCriterion.hxx\"");
      const auto cn = StressCriterion::getVariableId("c", id, r);
      tfel::raise_if(d.count("c") == 0,
                     "MohrCoulombStressCriterion::initialize: "
                     "material property 'c' is not defined");
      this->c = getBehaviourDescriptionMaterialProperty(dsl, "c", d.at("c"));
      declareParameterOrLocalVariable(bd, this->c, "real", cn);
      const auto phin = StressCriterion::getVariableId("phi", id, r);
      tfel::raise_if(d.count("phi") == 0,
                     "MohrCoulombStressCriterion::initialize: "
                     "material property 'phi' is not defined");
      this->phi =
          getBehaviourDescriptionMaterialProperty(dsl, "phi", d.at("phi"));
      declareParameterOrLocalVariable(bd, this->phi, "real", phin);
      const auto lodeTn = StressCriterion::getVariableId("lodeT", id, r);
      tfel::raise_if(d.count("lodeT") == 0,
                     "MohrCoulombStressCriterion::initialize: "
                     "material property 'lodeT' is not defined");
      this->lodeT =
          getBehaviourDescriptionMaterialProperty(dsl, "lodeT", d.at("lodeT"));
      declareParameterOrLocalVariable(bd, this->lodeT, "real", lodeTn);
      const auto an = StressCriterion::getVariableId("a", id, r);
      tfel::raise_if(d.count("a") == 0,
                     "MohrCoulombStressCriterion::initialize: "
                     "material property 'a' is not defined");
      this->a = getBehaviourDescriptionMaterialProperty(dsl, "a", d.at("a"));
      declareParameterOrLocalVariable(bd, this->a, "real", an);
      const auto pn = StressCriterion::getVariableId("mcsc_parameters", id, r);
      bd.addLocalVariable(
          uh, VariableDescription("MohrCoulombParameters<StressStensor>", pn,
                                  1u, 0u));
    }  // end of MohrCoulombStressCriterion::initialize

    void MohrCoulombStressCriterion::endTreatment(
        BehaviourDescription& bd,
        const AbstractBehaviourDSL& dsl,
        const std::string& id,
        const Role r) {
      constexpr const auto uh =
          tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      const auto cn = StressCriterion::getVariableId("c", id, r);
      auto ic =
          generateMaterialPropertyInitializationCode(dsl, bd, cn, this->c);
      const auto phin = StressCriterion::getVariableId("phi", id, r);
      ic +=
          generateMaterialPropertyInitializationCode(dsl, bd, phin, this->phi);
      const auto lodeTn = StressCriterion::getVariableId("lodeT", id, r);
      ic += generateMaterialPropertyInitializationCode(dsl, bd, lodeTn,
                                                       this->lodeT);
      const auto an = StressCriterion::getVariableId("a", id, r);
      ic += generateMaterialPropertyInitializationCode(dsl, bd, an, this->a);
      const auto pn = StressCriterion::getVariableId("mcsc_parameters", id, r);
      ic += "this->" + pn +
            " = makeMohrCoulombParameters<StressStensor, "
            "MohrCoulombParameters<StressStensor>::RADIAN>(this->" +
            cn + ", this->" + phin + ", this->" + lodeTn + ", this->" + an +
            ");";
      CodeBlock i;
      i.code = ic;
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }  // end of MohrCoulombStressCriterion::endTreatment

    std::string MohrCoulombStressCriterion::computeElasticPrediction(
        const std::string& id,
        const BehaviourDescription&,
        const StressPotential&) const {
      const auto pn = StressCriterion::getVariableId(
          "mcsc_parameters", id, StressCriterion::STRESSCRITERION);
      return "const auto seqel" + id +
             " = computeMohrCoulombStressCriterion(this->" + pn + ",sel" + id +
             ");\n";
    }  // end of MohrCoulombStressCriterion::computeElasticPrediction

    std::string MohrCoulombStressCriterion::computeCriterion(
        const std::string& id,
        const BehaviourDescription&,
        const StressPotential&) const {
      const auto pn = StressCriterion::getVariableId(
          "mcsc_parameters", id, StressCriterion::STRESSCRITERION);
      return "const auto seq" + id +
             " = computeMohrCoulombStressCriterion(this->" + pn + ", s" + id +
             ");\n";
    }  // end of MohrCoulombStressCriterion::computeCriterion

    std::string MohrCoulombStressCriterion::computeNormal(
        const std::string& id,
        const BehaviourDescription&,
        const StressPotential&,
        const Role r) const {
      const auto pn = StressCriterion::getVariableId("mcsc_parameters", id, r);
      auto code = std::string{};
      if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
#if __cplusplus >= 201703L
        code += "const auto [seq" + id + ",dseq" + id + "_ds" + id + "] = ";
        code += "computeMohrCoulombStressCriterionNormal(this->" + pn + ",s" +
                id + ");\n";
#else  /* __cplusplus >= 201703L */
        code += "stress seq" + id + ";\n";
        code += "Stensor dseq" + id + "_ds" + id + ";\n";
        code += "std::tie(seq" + id + ",dseq" + id + "_ds" + id + ") = ";
        code += "computeMohrCoulombStressCriterionNormal(this->" + pn + ", s" +
                id + ");\n";
#endif /* __cplusplus >= 201703L */
      }
      if (r == STRESSANDFLOWCRITERION) {
        code += "const auto& n" + id + " = dseq" + id + "_ds" + id + ";\n";
      }
      if (r == FLOWCRITERION) {
#if __cplusplus >= 201703L
        code += "const auto [seqf" + id + ",dseq" + id + "_ds" + id + "] = ";
        code += "computeMohrCoulombStressCriterionNormal(this->" + pn + ", s" +
                id + ");\n";
#else  /* __cplusplus >= 201703L */
        code += "stress seqf" + id + ";\n";
        code += "Stensor n" + id + ";\n";
        code += "std::tie(seqf" + id + ",n" + id + ") = ";
        code += "computeMohrCoulombStressCriterionNormal(this->" + pn + ", s" +
                id + ");\n";
#endif /* __cplusplus >= 201703L */
      }
      return code;
    }  // end of MohrCoulombStressCriterion::computeNormal

    std::string MohrCoulombStressCriterion::computeNormalDerivative(
        const std::string& id,
        const BehaviourDescription&,
        const StressPotential&,
        const Role r) const {
      const auto pn = StressCriterion::getVariableId("mcsc_parameters", id, r);
      auto code = std::string{};
      if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
#if __cplusplus >= 201703L
        code += "const auto [seq" + id + ",dseq" + id + "_ds" + id + ",d2seq" +
                id + "_ds" + id + "ds" + id + "] = ";
        code += "computeMohrCoulombStressCriterionSecondDerivative(this->" +
                pn + ", s" + id + ");\n";
#else  /* __cplusplus >= 201703L */
        code += "stress seq" + id + ";\n";
        code += "Stensor dseq" + id + "_ds" + id + ";\n";
        code += "Stensor4 d2seq" + id + "_ds" + id + "ds" + id + ";\n";
        code += "std::tie(seq" + id + ",dseq" + id + "_ds" + id + ",d2seq" +
                id + "_ds" + id + "ds" + id + ") = ";
        code += "computeMohrCoulombStressCriterionSecondDerivative(this->" +
                pn + ", s" + id + ");\n";
#endif /* __cplusplus >= 201703L */
      }
      if (r == STRESSANDFLOWCRITERION) {
        code += "const auto& n" + id + " = dseq" + id + "_ds" + id + ";\n";
        code += "const auto& dn" + id + "_ds" + id + " = ";
        code += "d2seq" + id + "_ds" + id + "ds" + id + ";\n";
      }
      if (r == FLOWCRITERION) {
#if __cplusplus >= 201703L
        code += "const auto [seqf" + id + ", n" + id + ", dn" + id + "_ds" +
                id + "] = ";
        code += "computeMohrCoulombStressCriterionSecondDerivative(this->" +
                pn + ", s" + id + ");\n";
#else  /* __cplusplus >= 201703L */
        code += "stress seqf" + id + ";\n";
        code += "Stensor n" + id + ";\n";
        code += "Stensor4 dn" + id + "_ds" + id + ";\n";
        code +=
            "std::tie(seqf" + id + ",n" + id + ",dn" + id + "_ds" + id + ") = ";
        code += "computeMohrCoulombStressCriterionSecondDerivative(this->" +
                pn + ", s" + id + ");\n";
#endif /* __cplusplus >= 201703L */
      }
      return code;
    }  // end of MohrCoulombStressCriterion::computeNormalDerivative

    bool MohrCoulombStressCriterion::isNormalDeviatoric() const {
      return false;
    }  // end of MohrCoulombStressCriterion::isNormalDeviatoric

    MohrCoulombStressCriterion::~MohrCoulombStressCriterion() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
