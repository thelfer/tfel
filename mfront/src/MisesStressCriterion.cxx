/*!
 * \file   mfront/src/MisesStressCriterion.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/MisesStressCriterion.hxx"

namespace mfront{

  namespace bbrick {

    void MisesStressCriterion::initialize(BehaviourDescription&,
                                          AbstractBehaviourDSL&,
                                          const DataMap& d) {
      if (!d.empty()) {
        tfel::raise("MisesStressCriterion::initialize: no option expected");
      }
    }  // end of MisesStressCriterion::initialize

    std::vector<OptionDescription> MisesStressCriterion::getOptions() const{
      return {};
    }  // end of MisesStressCriterion::getOptions

    std::string MisesStressCriterion::computeNormal(
        const std::string& id) const {
      auto c = std::string{};
      c += "const auto seq" + id + " = sigmaeq(this->sig);\n";
      c += "const auto iseq" + id + " = 1/max(seq" + id + ",1.e-12*young);\n";
      c += "const auto dseq_ds" + id + " = 3*deviator(s)*(iseq/2);\n";
      return c;
    }  // end of MisesStressCriterion::computeNormal

    std::string MisesStressCriterion::computeNormalDerivative(
        const std::string& id) const {
      auto c = std::string{};
      c += "const auto seq" + id + " = sigmaeq(this->sig);\n";
      c += "const auto iseq" + id + " = 1/max(seq" + id + ",1.e-12*young);\n";
      c += "const auto dseq" + id + "_ds = 3*deviator(this->sig)*(iseq/2);\n";
      c += "const auto d2seq" + id + "_ds2 = ";
      c += "(Stensor4::M()-(dseq" + id + "_ds^dseq" + id + "_ds))*iseq;\n";
      return c;
    }  // end of MisesStressCriterion::computeNormalDerivative

    MisesStressCriterion::~MisesStressCriterion() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront