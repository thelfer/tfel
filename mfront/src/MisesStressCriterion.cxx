/*!
 * \file   mfront/src/MisesStressCriterion.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/MisesStressCriterion.hxx"

namespace mfront {

  namespace bbrick {

    std::vector<mfront::BehaviourSymmetryType>
    MisesStressCriterion::getSupportedBehaviourSymmetries() const {
      return {mfront::ISOTROPIC, mfront::ORTHOTROPIC};
    }  // end of MisesStressCriterion::getSupportedBehaviourSymmetries()

    void MisesStressCriterion::initialize(BehaviourDescription& bd,
                                          AbstractBehaviourDSL& dsl,
                                          const std::string& id,
                                          const DataMap& d,
                                          const Role r) {
      StressCriterionBase::initialize(bd, dsl, id, d, r);
      if (!d.empty()) {
        tfel::raise("MisesStressCriterion::initialize: no option expected");
      }
    }  // end of MisesStressCriterion::initialize

    std::string MisesStressCriterion::computeElasticPrediction(
        const std::string& id,
        const BehaviourDescription&,
        const StressPotential&) const {
      return "const auto seqel" + id + " = sigmaeq(sel" + id + ");\n";
    }  // end of MisesStressCriterion::computeElasticPrediction

    std::string MisesStressCriterion::computeCriterion(
        const std::string& id,
        const BehaviourDescription&,
        const StressPotential&) const {
      return "const auto seq" + id + " = sigmaeq(s" + id + ");\n";
    }  // end of MisesStressCriterion::computeNormal

    std::string MisesStressCriterion::computeNormal(
        const std::string& id,
        const BehaviourDescription& bd,
        const StressPotential& sp,
        const Role r) const {
      auto c = std::string{};
      if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
        c += "const auto seq" + id + " = sigmaeq(s" + id + ");\n";
        c += "const auto iseq" + id + " = 1/max(seq" + id + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
        c += "const auto dseq" + id + "_ds" + id + " = 3*deviator(s" + id +
             ")*(iseq" + id + "/2);\n";
      }
      if (r == STRESSANDFLOWCRITERION) {
        c += "const auto& n" + id + " = dseq" + id + "_ds" + id + ";\n";
      }
      if (r == FLOWCRITERION) {
        c += "const auto seqf" + id + " = sigmaeq(s" + id + ");\n";
        c += "const auto iseqf" + id + " = 1/max(seqf" + id + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
        c += "const auto n" + id + " = 3*deviator(s" + id + ")*(iseqf" + id +
             "/2);\n";
      }
      return c;
    }  // end of MisesStressCriterion::computeNormal

    std::string MisesStressCriterion::computeNormalDerivative(
        const std::string& id,
        const BehaviourDescription& bd,
        const StressPotential& sp,
        const Role r) const {
      auto c = std::string{};
      if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
        c += "const auto seq" + id + " = sigmaeq(s" + id + ");\n";
        c += "const auto iseq" + id + " = 1/max(seq" + id + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
        c += "const auto dseq" + id + "_ds" + id + " = 3*deviator(s" + id +
             ")*(iseq" + id + "/2);\n";
        c += "const auto d2seq" + id + "_ds" + id + "ds" + id + " = ";
        c += "(Stensor4::M()-(dseq" + id + "_ds" + id + "^dseq" + id + "_ds" +
             id + "))*iseq" + id + ";\n";
      }
      if (r == STRESSANDFLOWCRITERION) {
        c += "const auto& n" + id + " = dseq" + id + "_ds" + id + ";\n";
        c += "const auto& dn" + id + "_ds" + id + " = ";
        c += "d2seq" + id + "_ds" + id + "ds" + id + ";\n";
      }
      if (r == FLOWCRITERION) {
        c += "const auto seqf" + id + " = sigmaeq(s" + id + ");\n";
        c += "const auto iseqf" + id + " = 1/max(seqf" + id + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
        c += "const auto n" + id + " = 3*deviator(s" + id + ")*(iseqf" + id +
             "/2);\n";
        c += "const auto dn" + id + "_ds" + id + " = ";
        c += "(Stensor4::M()-(n" + id + "^n" + id + "))*iseqf" + id + ";\n";
      }
      return c;
    }  // end of MisesStressCriterion::computeNormalDerivative

    bool MisesStressCriterion::isNormalDeviatoric() const {
      return true;
    }  // end of MisesStressCriterion::isNormalDeviatoric

    MisesStressCriterion::~MisesStressCriterion() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
