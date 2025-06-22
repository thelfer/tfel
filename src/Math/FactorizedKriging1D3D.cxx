/*!
 * \file  src/Math/FactorizedKriging1D3D.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 19 mai 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "TFEL/Math/FactorizedKriging1D3D.hxx"

namespace tfel::math {

  FactorizedKriging1D3D::FactorizedKriging1D3D(const std::vector<double>& vt,
                                               const std::vector<double>& vx,
                                               const std::vector<double>& vy,
                                               const std::vector<double>& vz,
                                               const std::vector<double>& vv) {
    raise_if<KrigingErrorInvalidLength>(
        (vt.size() != vx.size()) || (vt.size() != vy.size()) ||
        (vt.size() != vz.size()) || (vt.size() != vv.size()));
    const auto n0 = KrigingUtilities::normalize(vt);
    this->a0 = n0.first;
    this->b0 = n0.second;
    const auto n1 = KrigingUtilities::normalize(vx);
    this->a1 = n1.first;
    this->b1 = n1.second;
    const auto n2 = KrigingUtilities::normalize(vy);
    this->a2 = n2.first;
    this->b2 = n2.second;
    const auto n3 = KrigingUtilities::normalize(vz);
    this->a3 = n3.first;
    this->b3 = n3.second;
    auto pt = vt.cbegin();
    auto px = vx.cbegin();
    auto py = vy.cbegin();
    auto pz = vz.cbegin();
    auto pv = vv.begin();
    for (; pt != vt.end(); ++pt, ++px, ++py, ++pz, ++pv) {
      const tvector<3u> v = {this->a1 * (*px) + b1, this->a2 * (*py) + b2,
                             this->a3 * (*pz) + b3};
      FK::addValue(this->a0 * (*pt) + b0, v, *pv);
    }
    FK::buildInterpolation();
  }

  FactorizedKriging1D3D::FactorizedKriging1D3D(
      const tfel::math::vector<double>& vt,
      const tfel::math::vector<double>& vx,
      const tfel::math::vector<double>& vy,
      const tfel::math::vector<double>& vz,
      const tfel::math::vector<double>& vv) {
    raise_if<KrigingErrorInvalidLength>(
        (vt.size() != vx.size()) || (vt.size() != vy.size()) ||
        (vt.size() != vz.size()) || (vt.size() != vv.size()));
    const auto n0 = KrigingUtilities::normalize(vt);
    this->a0 = n0.first;
    this->b0 = n0.second;
    const auto n1 = KrigingUtilities::normalize(vx);
    this->a1 = n1.first;
    this->b1 = n1.second;
    const auto n2 = KrigingUtilities::normalize(vy);
    this->a2 = n2.first;
    this->b2 = n2.second;
    const auto n3 = KrigingUtilities::normalize(vz);
    this->a3 = n3.first;
    this->b3 = n3.second;
    auto pt = vt.cbegin();
    auto px = vx.cbegin();
    auto py = vy.cbegin();
    auto pz = vz.cbegin();
    auto pv = vv.begin();
    for (; pt != vt.end(); ++pt, ++px, ++py, ++pz, ++pv) {
      const tvector<3u> v = {this->a1 * (*px) + b1, this->a2 * (*py) + b2,
                             this->a3 * (*pz) + b3};
      FK::addValue(this->a0 * (*pt) + b0, v, *pv);
    }
    FK::buildInterpolation();
  }

  double FactorizedKriging1D3D::operator()(const double vt,
                                           const double vx,
                                           const double vy,
                                           const double vz) const {
    const tvector<3u> v = {this->a1 * (vx) + b1, this->a2 * (vy) + b2,
                           this->a3 * (vz) + b3};
    return FK::operator()(this->a0* vt + this->b0, v);
  }  // end of FactorizedKriging1D3D::operator()

  FactorizedKriging1D3D::~FactorizedKriging1D3D() = default;

}  // end of namespace tfel::math
