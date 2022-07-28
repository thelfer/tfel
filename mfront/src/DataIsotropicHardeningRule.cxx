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
                                              AbstractBehaviourDSL&,
                                              const std::string& fid,
                                              const std::string& id,
                                              const DataMap& d) {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    // interpolation type
    this->itype = [&d] {
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
          //         } else if (i == "cubic_spline") {
          //           return CUBIC_SPLINE_INTERPOLATION;
        } else {
          tfel::raise(
              "DataIsotropicHardeningRule::initialize: "
              "unsupported interpolation '" +
              i + "'");
        }
      }
      return LINEAR_INTERPOLATION;
    }();
    this->etype = [&d] {
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
    this->values = opt.get<std::map<double, double>>();
    if (this->values.empty()) {
      tfel::raise(
          "DataIsotropicHardeningRule::initialize: "
          "empty values provided");
    }
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
    opts.emplace_back("values",
                      "dictionnary giving the value of the yield surface "
                      "radius as a function of the equivalent plastic strain",
                      OptionDescription::DOUBLEDOUBLEMAP);
    opts.emplace_back(
        "interpolation",
        "interpolation type. "
        "Possible values are 'linear' (default choice) and 'cubic_spline'",
        OptionDescription::STRING);
    opts.emplace_back("extrapolation",
                      "extrapolation type. "
                      "Possible values are 'bound_to_last_value' (or "
                      "'constant') and 'extrapolation' (default choice)",
                      OptionDescription::STRING);
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
           " = this->computeYieldRadiusAndDerivative" + local_id + "(" + p +
           ");\n";
  }  // end of computeElasticLimitAndDerivative

  void DataIsotropicHardeningRule::endTreatment(BehaviourDescription& bd,
                                                const AbstractBehaviourDSL&,
                                                const std::string& fid,
                                                const std::string& id) const {
    if (this->itype == LINEAR_INTERPOLATION) {
      this->writeLinearInterpolationOfYieldRadius(bd, fid, id);
      //     } else if (this->itype == CUBIC_SPLINE_INTERPOLATION) {
      //       tfel::raise(
      //           "DataIsotropicHardeningRule::endTreatment: internal error, "
      //           "cubic spline interpolation are not yet implemented");
    } else {
      tfel::raise(
          "DataIsotropicHardeningRule::endTreatment: internal error, "
          "unsupported interpolation type");
    }
  } // end of endTreatment

  void DataIsotropicHardeningRule::writeLinearInterpolationOfYieldRadius(
      BehaviourDescription& bd,
      const std::string& fid,
      const std::string& id) const {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto local_id = id.empty() ? fid : fid + "_" + id;
    auto write_values = [this] {
      std::ostringstream os;
      os.precision(14);
      os << "constexpr auto mfront_yield_surface_abscissae = ";
      os << "tfel::math::fsarray<" + std::to_string(this->values.size()) +
                ", strain>";
      os << "{";
      for (auto p = values.begin(); p != values.end();) {
        os << "strain{" << p->first << "}";
        if (++p != values.end()) {
          os << ", ";
        }
      }
      os << "};\n";
      os << "constexpr auto mfront_yield_surface_values = ";
      os << "tfel::math::fsarray<" + std::to_string(this->values.size()) +
                ", stress>";
      os << "{";
      for (auto p = values.begin(); p != values.end();) {
        os << "stress{" << p->second << "}";
        if (++p != values.end()) {
          os << ", ";
        }
      }
      os << "};\n";
      return os.str();
    };
    const auto etype_value = this->etype ? "true" : "false";
    auto m = "stress computeYieldRadius" + local_id +
             "(const strain mfront_arg_p) const{\n";
    if (this->values.size() == 1) {
      const auto& v = *(this->values.begin());
      std::ostringstream os;
      os.precision(14);
      os << v.second;
      m += "return stress{" + os.str() + "};\n";
    } else {
      m += write_values();
      m += "return ";
      m += "tfel::math::computeLinearInterpolation<";
      m += etype_value;
      m += ">(";
      m += "mfront_yield_surface_abscissae, ";
      m += "mfront_yield_surface_values, ";
      m += "mfront_arg_p);\n";
      m += "return {};\n";
    }
    m += "}\n";
    m += "std::pair<stress, stress> computeYieldRadiusAndDerivative" +
              local_id + "(const strain mfront_arg_p) const {\n";
    if (this->values.size() == 1) {
      const auto& v = *(this->values.begin());
      std::ostringstream os;
      os.precision(14);
      os << v.second;
      m += "return {stress{" + os.str() + "}, stress{0}};\n";
    } else {
      m += write_values();
      m += "return ";
      m += "tfel::math::computeLinearInterpolationAndDerivative<";
      m += etype_value;
      m += ">(";
      m += "mfront_yield_surface_abscissae, ";
      m += "mfront_yield_surface_values, ";
      m += "mfront_arg_p);\n";
      m += "return {};\n";
    }
    m += "}\n";
    bd.appendToIncludes("#include \"TFEL/Math/LinearInterpolation.hxx\"");
    bd.appendToMembers(uh, m, true);
  }  // end of endTreatment

  DataIsotropicHardeningRule::~DataIsotropicHardeningRule() = default;

}  // end of namespace mfront::bbrick
