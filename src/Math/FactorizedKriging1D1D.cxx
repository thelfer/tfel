/*!
 * \file  src/Math/FactorizedKriging1D1D.cxx
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
#include "TFEL/Math/FactorizedKriging1D1D.hxx"

namespace tfel::math {

  FactorizedKriging1D1D::FactorizedKriging1D1D(const std::vector<double>& vx,
                                               const std::vector<double>& vy,
                                               const std::vector<double>& vz) {
    raise_if<KrigingErrorInvalidLength>((vx.size() != vy.size()) ||
                                        (vx.size() != vz.size()));
    const auto n0 = KrigingUtilities::normalize(vx);
    this->a0 = n0.first;
    this->b0 = n0.second;
    const auto n1 = KrigingUtilities::normalize(vy);
    this->a1 = n1.first;
    this->b1 = n1.second;
    auto px = vx.cbegin();
    auto py = vy.cbegin();
    auto pz = vz.cbegin();
    for (; px != vx.end(); ++px, ++py, ++pz) {
      FK::addValue(this->a0 * (*px) + this->b0, this->a1 * (*py) + this->b1,
                   *pz);
    }
    FK::buildInterpolation();
  }

  FactorizedKriging1D1D::FactorizedKriging1D1D(
      const tfel::math::vector<double>& vx,
      const tfel::math::vector<double>& vy,
      const tfel::math::vector<double>& vz) {
    raise_if<KrigingErrorInvalidLength>((vx.size() != vy.size()) ||
                                        (vx.size() != vz.size()));
    const auto n0 = KrigingUtilities::normalize(vx);
    this->a0 = n0.first;
    this->b0 = n0.second;
    const auto n1 = KrigingUtilities::normalize(vy);
    this->a1 = n1.first;
    this->b1 = n1.second;
    auto px = vx.cbegin();
    auto py = vy.cbegin();
    auto pz = vz.cbegin();
    for (; px != vx.end(); ++px, ++py, ++pz) {
      FK::addValue(this->a0 * (*px) + this->b0, this->a1 * (*py) + this->b1,
                   *pz);
    }
    FK::buildInterpolation();
  }

  double FactorizedKriging1D1D::operator()(const double vx,
                                           const double vy) const {
    return FK::operator()(this->a0*(vx) + this->b0, this->a1 * (vy) + this->b1);
  }  // end of FactorizedKriging1D1D::operator()

  FactorizedKriging1D1D::~FactorizedKriging1D1D() = default;

}  // end of namespace tfel::math
