/*!
 * \file   mfront/src/Hill1948StressCriterion.cxx
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
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/Hill1948StressCriterion.hxx"

namespace mfront{

  namespace bbrick {

    void Hill1948StressCriterion::initialize(BehaviourDescription& bd,
                                             AbstractBehaviourDSL& dsl,
                                             const std::string& id,
                                             const DataMap& d) {
      auto get_mp = [&dsl, &bd, &d](const char* const n) {
        if (d.count(n) == 0) {
          tfel::raise("Hill1948StressCriterion::initialize: entry '" +
                      std::string(n) + "' is not defined");
        }
        return getBehaviourDescriptionMaterialProperty(dsl, n, d.at(n));
      };
      auto v = VariableDescription{"tfel::math::st2tost2<N,stress>", "H" + id,
                                   1u, 0u};
      v.description = "Hill tensor";
      std::vector<BehaviourDescription::MaterialProperty> Hmps = {
          get_mp("F"), get_mp("G"), get_mp("H"),
          get_mp("L"), get_mp("M"), get_mp("N")};
      bd.addHillTensor(v, Hmps);
    }  // end of Hill1948StressCriterion::initialize

    std::vector<OptionDescription> Hill1948StressCriterion::getOptions() const{
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

    std::string Hill1948StressCriterion::computeElasticPrediction(
        const std::string&id) const {
      const auto sel = "sel" + id;
      const auto H = "H" + id;
      return "const auto seqel" + id +  //
             " = sqrt(" + sel + "|" + H + "*" + sel + ");\n";
    }  // end of Hill1948StressCriterion::computeElasticPrediction

    std::string Hill1948StressCriterion::computeCriterion(
        const std::string& id) const {
      const auto s = "s" + id;
      const auto H = "H" + id;
      return "const auto seq" + id +  //
             " = sqrt(" + s + "|" + H + "*" + s + ");\n";
    }  // end of Hill1948StressCriterion::computeNormal

    std::string Hill1948StressCriterion::computeNormal(
        const std::string& id) const {
      const auto s = "s" + id;
      const auto seq = "seq" + id;
      const auto H = "H" + id;
      auto c = std::string{};
      c += "const auto " + seq +  //
           " = sqrt(" + s + "|" + H + "*" + s + ");\n";
      c += "const auto i" + seq + " = 1/max(" + seq + ",1.e-12*young);\n";
      c += "const auto d" + seq + "_d" + s +  //
           " = " + H + "*" + s + "*i" + seq + ";\n";
      return c;
    }  // end of Hill1948StressCriterion::computeNormal

    std::string Hill1948StressCriterion::computeNormalDerivative(
        const std::string& id) const {
      const auto s = "s" + id;
      const auto seq = "seq" + id;
      const auto H = "H" + id;
      auto c = std::string{};
      c += "const auto " + seq +  //
           " = sqrt(" + s + "|" + H + "*" + s + ");\n";
      c += "const auto i" + seq + " = 1/max(" + seq + ",1.e-12*young);\n";
      c += "const auto d" + seq + "_d" + s +  //
           " = " + H + "*" + s + "*i" + seq + ";\n";
      c += "const auto d2"+seq+ "_d"+s+"d"+s+" = ";
      c += "(" + H + "-(d" + seq + "_d" + s + "^d" + seq + "_d" + s + "))*i" +
           seq + ";\n";
      return c;
    }  // end of Hill1948StressCriterion::computeNormalDerivative

    Hill1948StressCriterion::~Hill1948StressCriterion() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront