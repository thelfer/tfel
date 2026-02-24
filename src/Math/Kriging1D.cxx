/*!
 * \file  src/Math/Kriging1D.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 18 mai 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "TFEL/Math/Kriging1D.hxx"

namespace tfel::math {

  Kriging1D::Kriging1D() = default;

  Kriging1D::Kriging1D(const std::vector<double>& vx,
                       const std::vector<double>& vy) {
    raise_if<KrigingErrorInvalidLength>(vx.size() != vy.size());
    const auto n = KrigingUtilities::normalize(vx);
    this->a = n.first;
    this->b = n.second;
    auto px = vx.begin();
    auto py = vy.begin();
    for (; px != vx.end(); ++px, ++py) {
      Kriging<1u, double>::addValue((this->a) * (*px) + (this->b), *py);
    }
    Kriging<1u, double>::buildInterpolation();
  }

  Kriging1D::Kriging1D(const tfel::math::vector<double>& vx,
                       const tfel::math::vector<double>& vy) {
    using tfel::math::vector;
    raise_if<KrigingErrorInvalidLength>(vx.size() != vy.size());
    const auto n = KrigingUtilities::normalize(vx);
    this->a = n.first;
    this->b = n.second;
    auto px = vx.begin();
    auto py = vy.begin();
    for (; px != vx.end(); ++px, ++py) {
      Kriging<1u, double>::addValue((this->a) * (*px) + (this->b), *py);
    }
    Kriging<1u, double>::buildInterpolation();
  }

  double Kriging1D::operator()(const double vx) const {
    return Kriging<1u, double>::operator()(this->a* vx + this->b);
  }  // end of Kriging1D::operator()

  Kriging1D::~Kriging1D() = default;

}  // end of namespace tfel::math
