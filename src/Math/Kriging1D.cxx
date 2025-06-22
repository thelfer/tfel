/*!
 * \file  src/Math/Kriging1D.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 mai 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Math/Kriging1D.hxx"

namespace tfel {

  namespace math {

    Kriging1D::Kriging1D(const std::vector<double>& vx,
                         const std::vector<double>& vy) {
      using namespace std;
      vector<double>::const_iterator px;
      vector<double>::const_iterator py;
      if (vx.size() != vy.size()) {
        throw(KrigingErrorInvalidLength());
      }
      pair<double, double> n = KrigingUtilities::normalize(vx);
      this->a = n.first;
      this->b = n.second;
      for (px = vx.begin(), py = vy.begin(); px != vx.end(); ++px, ++py) {
        Kriging<1u, double>::addValue((this->a) * (*px) + (this->b), *py);
      }
      Kriging<1u, double>::buildInterpolation();
    }

    Kriging1D::Kriging1D(const tfel::math::vector<double>& vx,
                         const tfel::math::vector<double>& vy) {
      using namespace std;
      using namespace tfel::math;
      using tfel::math::vector;
      vector<double>::const_iterator px;
      vector<double>::const_iterator py;
      if (vx.size() != vy.size()) {
        throw(KrigingErrorInvalidLength());
      }
      pair<double, double> n = KrigingUtilities::normalize(vx);
      this->a = n.first;
      this->b = n.second;
      for (px = vx.begin(), py = vy.begin(); px != vx.end(); ++px, ++py) {
        Kriging<1u, double>::addValue((this->a) * (*px) + (this->b), *py);
      }
      Kriging<1u, double>::buildInterpolation();
    }

    double Kriging1D::operator()(const double vx) const {
      return Kriging<1u, double>::operator()(this->a* vx + this->b);
    }  // end of Kriging1D::operator()

    // Kriging1D::~Kriging1D()
    // {} // end of Kriging1D::~Kriging1D

  }  // end of namespace math

}  // end of namespace tfel
