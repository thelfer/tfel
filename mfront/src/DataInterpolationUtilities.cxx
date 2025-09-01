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
#include "TFEL/Math/CubicSpline.hxx"
#include "MFront/DataInterpolationUtilities.hxx"

namespace mfront {

  SingleVariableInterpolatedData SingleVariableInterpolatedData::extract(
      const tfel::utilities::DataMap& d) {
    //
    for (const auto& [k, v] : d) {
      static_cast<void>(v);
      if ((k != "interpolation") && (k != "extrapolation") && (k != "values")) {
        tfel::raise(
            "SingleVariableInterpolatedData::extract: "
            "unexpected option '" +
            k + "'");
      }
    }
    auto idata = SingleVariableInterpolatedData{};
    // interpolation type
    idata.itype = [&d] {
      if (d.count("interpolation")) {
        const auto& opt = d.at("interpolation");
        if (!opt.is<std::string>()) {
          tfel::raise(
              "SingleVariableInterpolatedData::extract: "
              "unsupported type for option 'interpolation' "
              "(must be a string)");
        }
        const auto& i = opt.get<std::string>();
        if (i == "linear") {
          return LINEAR_INTERPOLATION;
        } else if (i == "cubic_spline") {
          return CUBIC_SPLINE_INTERPOLATION;
        } else {
          tfel::raise(
              "SingleVariableInterpolatedData::extract: "
              "unsupported interpolation '" +
              i + "'");
        }
      }
      return LINEAR_INTERPOLATION;
    }();
    idata.etype = [&d] {
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
                "SingleVariableInterpolatedData::extract: "
                "unsupported value for option 'extrapolation' "
                "(expected 'bound_to_last_value' or 'constant')");
          }
        } else {
          tfel::raise(
              "SingleVariableInterpolatedData::extract: "
              "unsupported type for option 'extrapolation' "
              "(must be a string or a boolean value)");
        }
      }
      return true;
    }();
    // values
    if (!d.count("values")) {
      tfel::raise(
          "SingleVariableInterpolatedData::extract: "
          "no values specified");
    }
    const auto& opt = d.at("values");
    if (!opt.is<std::map<double, double>>()) {
      tfel::raise(
          "SingleVariableInterpolatedData::extract: "
          "invalid type for option 'values'");
    }
    idata.values = opt.get<std::map<double, double>>();
    if (idata.values.empty()) {
      tfel::raise(
          "SingleVariableInterpolatedData::extract: "
          "empty values provided");
    }
    return idata;
  }

  std::string writeLinearInterpolationValues(
      const SingleVariableInterpolatedData& idata,
      const SingleVariableInterpolatedData::
          WriteLinearInterpolationValuesArguments& args) {
    std::ostringstream os;
    os.precision(14);
    os << "constexpr auto " << args.abscissae_name << " = ";
    os << "std::array<" << args.abscissae_type
       << ", " + std::to_string(idata.values.size()) + ">";
    os << "{";
    for (auto p = idata.values.begin(); p != idata.values.end();) {
      os << "" << args.abscissae_type << "{" << p->first << "}";
      if (++p != idata.values.end()) {
        os << ", ";
      }
    }
    os << "};\n";
    os << "constexpr auto " << args.ordinates_name << " = ";
    os << "std::array<" << args.ordinates_type
       << ", " + std::to_string(idata.values.size()) + ">";
    os << "{";
    for (auto p = idata.values.begin(); p != idata.values.end();) {
      os << "" << args.ordinates_type << "{" << p->second << "}";
      if (++p != idata.values.end()) {
        os << ", ";
      }
    }
    os << "};\n";
    return os.str();
  }

  std::string writeCollocationPoints(
      const SingleVariableInterpolatedData& idata,
      const SingleVariableInterpolatedData::WriteCollocationPointsArguments&
          args) {
    // add collocation points
    tfel::math::CubicSpline<double, double> spline;
    const auto points = [&idata] {
      auto abscissae = std::vector<double>{};
      auto lvalues = std::vector<double>{};
      for (const auto& v : idata.values) {
        abscissae.push_back(v.first);
        lvalues.push_back(v.second);
      }
      return std::make_pair(abscissae, lvalues);
    }();
    spline.setCollocationPoints(points.first, points.second);
    const auto& pts = spline.getCollocationPoints();
    std::ostringstream os;
    os.precision(14);
    const auto type = "tfel::math::CubicSplineCollocationPoint<" +
                      args.abscissae_type + ", " + args.ordinates_type + ">";
    os << "constexpr auto " << args.collocation_points_name << " = "
       << "std::array<" << type << ", " << std::to_string(idata.values.size())
       << ">{\n";
    for (auto p = pts.begin(); p != pts.end();) {
      os << type << "{" << args.abscissae_type << "{" << p->x << "}, "
         << args.ordinates_type << "{" << p->y << "}, "
         << args.ordinates_derivatives_type << "{" << p->d << "}}";
      if (++p != pts.end()) {
        os << ", ";
      }
    }
    os << "};\n";
    return os.str();
  }  // end of

}  // end of namespace mfront
