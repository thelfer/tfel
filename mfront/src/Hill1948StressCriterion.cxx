/*!
 * \file   mfront/src/Hill1948StressCriterion.cxx
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
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/Hill1948StressCriterion.hxx"

namespace mfront {

  namespace bbrick {

    std::vector<OptionDescription> Hill1948StressCriterion::getOptions() const {
      std::vector<OptionDescription> opts;
      opts.emplace_back("F", "First Hill parameter",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("G", "Hill parameter",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("H", "Hill parameter",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("L", "Hill parameter",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("M", "Hill parameter",
                        OptionDescription::MATERIALPROPERTY);
      opts.emplace_back("N", "Hill parameter",
                        OptionDescription::MATERIALPROPERTY);
      return opts;
    }  // end of Hill1948StressCriterion::getOptions

    void Hill1948StressCriterion::initialize(BehaviourDescription& bd,
                                             AbstractBehaviourDSL& dsl,
                                             const std::string& id,
                                             const DataMap& d,
                                             const Role r) {
      auto get_mp = [&dsl, &bd, &d](const char* const n) {
        if (d.count(n) == 0) {
          tfel::raise("Hill1948StressCriterion::initialize: entry '" +
                      std::string(n) + "' is not defined");
        }
        return getBehaviourDescriptionMaterialProperty(dsl, n, d.at(n));
      };
      tfel::raise_if(bd.getSymmetryType() != mfront::ORTHOTROPIC,
                     "Hill1948StressCriterion::initialize: "
                     "the behaviour must be orthotropic");
      StressCriterionBase::initialize(bd, dsl, id, d, r);
      const auto Hn = StressCriterion::getVariableId("H", id, r);
      auto v =
          VariableDescription{"tfel::math::st2tost2<N,stress>", Hn, 1u, 0u};
      v.description = "Hill tensor";
      std::vector<BehaviourDescription::MaterialProperty> Hmps = {
          get_mp("F"), get_mp("G"), get_mp("H"),
          get_mp("L"), get_mp("M"), get_mp("N")};
      bd.addHillTensor(v, Hmps);
    }  // end of Hill1948StressCriterion::initialize

    std::string Hill1948StressCriterion::computeElasticPrediction(
        const std::string& id,
        const BehaviourDescription&,
        const StressPotential&) const {
      const auto Hn = StressCriterion::getVariableId(
          "H", id, StressCriterion::STRESSCRITERION);
      const auto sel = "sel" + id;
      return "const auto seqel" + id +  //
             " = sqrt(" + sel + "|((this->" + Hn + ")*" + sel + "));\n";
    }  // end of Hill1948StressCriterion::computeElasticPrediction

    std::string Hill1948StressCriterion::computeCriterion(
        const std::string& id,
        const BehaviourDescription&,
        const StressPotential&) const {
      const auto s = "s" + id;
      const auto Hn = StressCriterion::getVariableId(
          "H", id, StressCriterion::STRESSCRITERION);
      return "const auto seq" + id +  //
             " = sqrt(" + s + "|((this->" + Hn + ")*" + s + "));\n";
    }  // end of Hill1948StressCriterion::computeNormal

    std::string Hill1948StressCriterion::computeNormal(
        const std::string& id,
        const BehaviourDescription& bd,
        const StressPotential& sp,
        const Role r) const {
      const auto Hn = StressCriterion::getVariableId("H", id, r);
      const auto s = "s" + id;
      const auto seq = [r, &id]() -> std::string {
        if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
          return "seq" + id;
        }
        return "seqf" + id;
      }();
      const auto n = [r, &id]() -> std::string {
        if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
          return "dseq" + id + "_ds" + id;
        }
        return "n" + id;
      }();
      auto c = std::string{};
      c += "const auto " + seq +  //
           " = sqrt(" + s + "|((this->" + Hn + ")*" + s + "));\n";
      c += "const auto i" + seq + " = 1/max(" + seq + "," +
           sp.getEquivalentStressLowerBound(bd) + ");\n";
      c +=
          "const auto " + n + " = (this->" + Hn + ")*" + s + "*i" + seq + ";\n";
      if (r == STRESSANDFLOWCRITERION) {
        c += "const auto& n" + id + " = dseq" + id + "_ds" + id + ";\n";
      }
      return c;
    }  // end of Hill1948StressCriterion::computeNormal

    std::string Hill1948StressCriterion::computeNormalDerivative(
        const std::string& id,
        const BehaviourDescription& bd,
        const StressPotential& sp,
        const Role r) const {
      const auto s = "s" + id;
      const auto Hn = StressCriterion::getVariableId("H", id, r);
      const auto seq = [r, &id]() -> std::string {
        if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
          return "seq" + id;
        }
        return "seqf" + id;
      }();
      const auto n = [r, &id]() -> std::string {
        if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
          return "dseq" + id + "_ds" + id;
        }
        return "n" + id;
      }();
      const auto dn = [r, &id]() -> std::string {
        if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
          return "d2seq" + id + "_ds" + id + "ds" + id;
        }
        return "dn" + id + "_ds" + id;
      }();
      auto c = std::string{};
      c += "const auto " + seq +  //
           " = sqrt(" + s + "|((this->" + Hn + ")*" + s + "));\n";
      c += "const auto i" + seq + " = 1/max(" + seq + "," +
           sp.getEquivalentStressLowerBound(bd) + ");\n";
      c +=
          "const auto " + n + " = (this->" + Hn + ")*" + s + "*i" + seq + ";\n";
      c += "const auto " + dn + " = ";
      c += "((this->" + Hn + ")-(" + n + "^" + n + "))*i" + seq + ";\n";
      if (r == STRESSANDFLOWCRITERION) {
        c += "const auto& n" + id + " = dseq" + id + "_ds" + id + ";\n";
        c += "const auto& dn" + id + "_ds" + id + " = ";
        c += "d2seq" + id + "_ds" + id + "ds" + id + ";\n";
      }
      return c;
    }  // end of Hill1948StressCriterion::computeNormalDerivative

    bool Hill1948StressCriterion::isNormalDeviatoric() const {
      return true;
    }  // end of Hill1948StressCriterion::isNormalDeviatoric

    Hill1948StressCriterion::~Hill1948StressCriterion() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
