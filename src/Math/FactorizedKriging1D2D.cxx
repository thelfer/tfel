/*!
 * \file  src/Math/FactorizedKriging1D2D.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 19 mai 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Math/FactorizedKriging1D2D.hxx"

namespace tfel {

  namespace math {

    FactorizedKriging1D2D::FactorizedKriging1D2D(
        const std::vector<double>& vx,
        const std::vector<double>& vy,
        const std::vector<double>& vz,
        const std::vector<double>& vv) {
      if ((vx.size() != vy.size()) || (vx.size() != vz.size()) ||
          (vx.size() != vv.size())) {
        throw(KrigingErrorInvalidLength());
      }
      const auto n0 = KrigingUtilities::normalize(vx);
      this->a0 = n0.first;
      this->b0 = n0.second;
      const auto n1 = KrigingUtilities::normalize(vy);
      this->a1 = n1.first;
      this->b1 = n1.second;
      const auto n2 = KrigingUtilities::normalize(vz);
      this->a2 = n2.first;
      this->b2 = n2.second;
      auto px = vx.begin();
      auto py = vy.begin();
      auto pz = vz.begin();
      auto pv = vv.begin();
      for (; px != vx.end(); ++px, ++py, ++pz, ++pv) {
        const tvector<2u> v = {this->a1 * (*py) + b1, this->a2 * (*pz) + b2};
        FK::addValue(this->a0 * (*px) + this->b0, v, *pv);
      }
      FK::buildInterpolation();
    }

    FactorizedKriging1D2D::FactorizedKriging1D2D(
        const tfel::math::vector<double>& vx,
        const tfel::math::vector<double>& vy,
        const tfel::math::vector<double>& vz,
        const tfel::math::vector<double>& vv) {
      if ((vx.size() != vy.size()) || (vx.size() != vz.size()) ||
          (vx.size() != vv.size())) {
        throw(KrigingErrorInvalidLength());
      }
      const auto n0 = KrigingUtilities::normalize(vx);
      this->a0 = n0.first;
      this->b0 = n0.second;
      const auto n1 = KrigingUtilities::normalize(vy);
      this->a1 = n1.first;
      this->b1 = n1.second;
      const auto n2 = KrigingUtilities::normalize(vz);
      this->a2 = n2.first;
      this->b2 = n2.second;
      auto px = vx.begin();
      auto py = vy.begin();
      auto pz = vz.begin();
      auto pv = vv.begin();
      for (; px != vx.end(); ++px, ++py, ++pz, ++pv) {
        const tvector<2u> v = {this->a1 * (*py) + b1, this->a2 * (*pz) + b2};
        FK::addValue(this->a0 * (*px) + this->b0, v, *pv);
      }
      FK::buildInterpolation();
    }

    double FactorizedKriging1D2D::operator()(const double vx,
                                             const double vy,
                                             const double vz) const {
      const tvector<2u> v = {this->a1 * (vy) + b1, this->a2 * (vz) + b2};
      return FK::operator()(this->a0* vx + this->b0, v);
    }  // end of FactorizedKriging1D2D::operator()

    FactorizedKriging1D2D::~FactorizedKriging1D2D() = default;

  }  // end of namespace math

}  // end of namespace tfel
