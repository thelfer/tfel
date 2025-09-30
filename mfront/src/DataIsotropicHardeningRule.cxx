/*!
 * \file   mfront/src/DataIsotropicHardeningRule.cxx
 * \brief
 * \author Thomas Helfer
 * \date   25/07/2022
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
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/DataIsotropicHardeningRule.hxx"

namespace mfront::bbrick {

  void DataIsotropicHardeningRule::initialize(BehaviourDescription& bd,
                                              AbstractBehaviourDSL&,
                                              const std::string& fid,
                                              const std::string& id,
                                              const DataMap& d) {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    static_cast<SingleVariableInterpolatedData&>(*this) =
        SingleVariableInterpolatedData::extract(d);
    //
    const auto Rel = id.empty() ? "Rel" + fid : "Rel" + fid + "_" + id;
    const auto R = id.empty() ? "R" + fid : "R" + fid + "_" + id;
    const auto dR = "d" + R + "_dp" + fid;
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
           " = this->mfront_computeYieldRadius" + local_id + "(this->p" + fid +
           ");\n";
  }  // end of computeElasticPrediction

  std::string DataIsotropicHardeningRule::computeElasticLimit(
      const BehaviourDescription&,
      const std::string& fid,
      const std::string& id) const {
    const auto local_id = id.empty() ? fid : fid + "_" + id;
    const auto R = "R" + local_id;
    const auto p = "this->p" + fid + " + (this->theta) * (this->dp" + fid + ")";
    return "const auto " + R +  //
           " = this->mfront_computeYieldRadius" + local_id + "(" + p + ");\n";
  }  // end of computeElasticLimit

  std::string DataIsotropicHardeningRule::computeElasticLimitAndDerivative(
      const BehaviourDescription&,
      const std::string& fid,
      const std::string& id) const {
    const auto local_id = id.empty() ? fid : fid + "_" + id;
    const auto R = "R" + local_id;
    const auto dR = "d" + R + "_dp" + fid;
    const auto p = "this->p" + fid + " + (this->theta) * (this->dp" + fid + ")";
    return "const auto [" + R + ", " + dR + "]" +  //
           " = this->mfront_computeYieldRadiusAndDerivative" + local_id + "(" +
           p + ");\n";
  }  // end of computeElasticLimitAndDerivative

  void DataIsotropicHardeningRule::endTreatment(BehaviourDescription& bd,
                                                const AbstractBehaviourDSL&,
                                                const std::string& fid,
                                                const std::string& id) const {
    if (this->itype == LINEAR_INTERPOLATION) {
      this->writeLinearInterpolationOfYieldRadius(bd, fid, id);
    } else if (this->itype == CUBIC_SPLINE_INTERPOLATION) {
      this->writeCubicSplineInterpolationOfYieldRadius(bd, fid, id);
    } else {
      tfel::raise(
          "DataIsotropicHardeningRule::endTreatment: internal error, "
          "unsupported interpolation type");
    }
  }  // end of endTreatment

  void DataIsotropicHardeningRule::writeLinearInterpolationOfYieldRadius(
      BehaviourDescription& bd,
      const std::string& fid,
      const std::string& id) const {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto local_id = id.empty() ? fid : fid + "_" + id;
    const auto args =
        SingleVariableInterpolatedData::WriteLinearInterpolationValuesArguments{
            .abscissae_name = "mfront_yield_surface_abscissae",
            .abscissae_type = "strain",
            .ordinates_name = "mfront_yield_surface_values",
            .ordinates_type = "stress"};
    const auto etype_value = this->etype ? "true" : "false";
    auto m = "stress mfront_computeYieldRadius" + local_id +
             "(const strain mfront_arg_p) const{\n";
    if (this->values.size() == 1) {
      const auto& v = *(this->values.begin());
      std::ostringstream os;
      os.precision(14);
      os << v.second;
      m += "return stress{" + os.str() + "};\n";
    } else {
      m += writeLinearInterpolationValues(*this, args);
      m += "return ";
      m += "tfel::math::computeLinearInterpolation<";
      m += etype_value;
      m += ">(";
      m += args.abscissae_name + ", ";
      m += args.ordinates_name + ", ";
      m += "mfront_arg_p);\n";
    }
    m += "}\n";
    m += "std::pair<stress, stress> mfront_computeYieldRadiusAndDerivative" +
         local_id + "(const strain mfront_arg_p) const {\n";
    if (this->values.size() == 1) {
      const auto& v = *(this->values.begin());
      std::ostringstream os;
      os.precision(14);
      os << v.second;
      m += "return {stress{" + os.str() + "}, stress{0}};\n";
    } else {
      m += writeLinearInterpolationValues(*this, args);
      m += "return ";
      m += "tfel::math::computeLinearInterpolationAndDerivative<";
      m += etype_value;
      m += ">(";
      m += args.abscissae_name + ", ";
      m += args.ordinates_name + ", ";
      m += "mfront_arg_p);\n";
    }
    m += "}\n";
    bd.appendToIncludes("#include \"TFEL/Math/LinearInterpolation.hxx\"");
    bd.appendToMembers(uh, m, true);
  }  // end of endTreatment

  void DataIsotropicHardeningRule::writeCubicSplineInterpolationOfYieldRadius(
      BehaviourDescription& bd,
      const std::string& fid,
      const std::string& id) const {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto local_id = id.empty() ? fid : fid + "_" + id;
    const auto args =
        SingleVariableInterpolatedData::WriteCollocationPointsArguments{
            .collocation_points_name =
                "mfront_yield_surface_collocation_points",
            .abscissae_type = "strain",
            .ordinates_type = "stress",
            .ordinates_derivatives_type = "stress"};
    const auto etype_value = this->etype ? "true" : "false";
    const auto m1 = "mfront_computeYieldRadius" + local_id;
    const auto m2 = "mfront_computeYieldRadiusAndDerivative" + local_id;
    auto m = "stress " + m1 + "(const strain mfront_arg_p) const{\n";
    if (this->values.size() == 1) {
      const auto& v = *(this->values.begin());
      std::ostringstream os;
      os.precision(14);
      os << v.second;
      m += "return stress{" + os.str() + "};\n";
    } else {
      m += writeCollocationPoints(*this, args);
      m += "return ";
      m += "tfel::math::computeCubicSplineInterpolation<";
      m += etype_value;
      m += ">(";
      m += "mfront_yield_surface_collocation_points, ";
      m += "mfront_arg_p);\n";
      m += "return {};\n";
    }
    m += "} // end of " + m1 + "\n\n";
    m += "std::pair<stress, stress> " + m2 + local_id +
         "(const strain mfront_arg_p) const {\n";
    if (this->values.size() == 1) {
      const auto& v = *(this->values.begin());
      std::ostringstream os;
      os.precision(14);
      os << v.second;
      m += "return {stress{" + os.str() + "}, stress{0}};\n";
    } else {
      m += writeCollocationPoints(*this, args);
      m += "return ";
      m += "tfel::math::computeCubicSplineInterpolationAndDerivative<";
      m += etype_value;
      m += ">(";
      m += args.collocation_points_name + ", ";
      m += "mfront_arg_p);\n";
    }
    m += "} // end of " + m2 + "\n\n";
    bd.appendToIncludes("#include \"TFEL/Math/CubicSpline.hxx\"");
    bd.appendToMembers(uh, m, true);
  }  // end of endTreatment

  DataIsotropicHardeningRule::~DataIsotropicHardeningRule() = default;

}  // end of namespace mfront::bbrick
