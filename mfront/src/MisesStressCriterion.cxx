/*!
 * \file   mfront/src/MisesStressCriterion.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/MisesStressCriterion.hxx"

namespace mfront{

  namespace bbrick {

    void MisesStressCriterion::initialize(BehaviourDescription&,
                                          AbstractBehaviourDSL&,
                                          const std::string&,
                                          const DataMap& d) {
      if (!d.empty()) {
        tfel::raise("MisesStressCriterion::initialize: no option expected");
      }
    }  // end of MisesStressCriterion::initialize

    std::vector<OptionDescription> MisesStressCriterion::getOptions() const{
      return {};
    }  // end of MisesStressCriterion::getOptions

    std::string MisesStressCriterion::computeElasticPrediction(
        const std::string&id) const {
      return "const auto seqel" + id + " = sigmaeq(sel" + id + ");\n";
    }  // end of MisesStressCriterion::computeElasticPrediction

    std::string MisesStressCriterion::computeCriterion(
        const std::string& id) const {
      return "const auto seq" + id + " = sigmaeq(s" + id + ");\n";
    }  // end of MisesStressCriterion::computeNormal

    std::string MisesStressCriterion::computeNormal(
        const std::string& id) const {
      auto c = std::string{};
      c += "const auto seq" + id + " = sigmaeq(s"+id+");\n";
      c += "const auto iseq" + id + " = 1/max(seq" + id + ",1.e-12*young);\n";
      c += "const auto dseq" + id + "_ds" + id + " = 3*deviator(s" + id +
           ")*(iseq" + id + "/2);\n";
      return c;
    }  // end of MisesStressCriterion::computeNormal

    std::string MisesStressCriterion::computeNormalDerivative(
        const std::string& id) const {
      auto c = std::string{};
      c += "const auto seq" + id + " = sigmaeq(s"+id+");\n";
      c += "const auto iseq" + id + " = 1/max(seq" + id + ",1.e-12*young);\n";
      c += "const auto dseq" + id + "_ds" + id + " = 3*deviator(s" + id +
           ")*(iseq" + id + "/2);\n";
      c += "const auto d2seq" + id + "_ds" + id + "ds" + id + " = ";
      c += "(Stensor4::M()-(dseq" + id + "_ds" + id + "^dseq" + id + "_ds" +
           id + "))*iseq" + id + ";\n";
      return c;
    }  // end of MisesStressCriterion::computeNormalDerivative

    MisesStressCriterion::~MisesStressCriterion() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront