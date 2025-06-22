/*!
 * \file   src/NUMODIS/FrankRead.cxx
 * \brief
 * \author Laurent Dupuy
 * \date   9/06/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Math/General/CubicRoots.hxx"
#include "NUMODIS/FrankRead.hxx"

namespace numodis {

  //======================================================
  // FrankRead::FrankRead
  //------------------------------------------------------
  //! Constructor
  //------------------------------------------------------
  /*!
    \param mu shear modulus
    \param burgers Burgers vector
    \param nu Poisson coefficient
  */
  //======================================================
  FrankRead::FrankRead(const IsotropicLineTensionModel& linetension)
      : _linetension(linetension) {}

  //=================================================================
  // FrankRead::computeBetaF
  //-----------------------------------------------------------------
  //! Compute the final tangent angle knowing the first one
  //-----------------------------------------------------------------
  /*!
    \param alpha angle between ... and ...
    \param beta0 first tangent angle in range[0;pi]
    \param tol tolerance / precision of the calculation
    \param Nmax maximum number of iterations
    \return betaF final tangent angle
  */
  //=================================================================
  double FrankRead::computeBetaF(double alpha,
                                 double beta0,
                                 double tol,
                                 int Nmax) const {
    // pi = 3.14159...
    double pi = atan(1.0) * 4.0;

    // range for betaF
    double bFmin = beta0 - pi;
    double bFmax = 0.0;
    if (bFmin > bFmax) throw -1;  // throw: ill-defined beta0 value

    double Eq4min = this->Equation4(alpha, beta0, bFmin);
    double Eq4max = this->Equation4(alpha, beta0, bFmax);
    if (Eq4min * Eq4max > 0.0)
      throw -2;  // throw: both sides have positive values

    // find betaF using a simple bisection method
    int N = 0;
    while (N < Nmax) {
      double bFmid = (bFmax + bFmin) / 2.0;
      double Eq4mid = this->Equation4(alpha, beta0, bFmid);
      if ((tfel::math::ieee754::fpclassify(Eq4mid) == FP_ZERO) ||
          ((bFmax - bFmin) / 2.0 < tol)) {
        return bFmid;
      }
      ++N;

      if (std::signbit(Eq4mid) == std::signbit(Eq4min)) {
        bFmin = bFmid;
        Eq4min = Eq4mid;
      } else {
        bFmax = bFmid;
        Eq4max = Eq4mid;
      }
    }
    throw -3;  // throw: calculation did not converge
  }

  //=================================================================
  // FrankRead::computeActivationAngle
  //-----------------------------------------------------------------
  //! Compute the tangent angle beta0 at activation stress
  //-----------------------------------------------------------------
  /*!
    This method was directly taken out of the code used by
    Dupuy & Fivel in Acta Materiala 2002 (jojo5).

    \param phi0 angle between the initial source and the Burgers
    \return activation angle betaF (in range[0:pi])

  */
  //=================================================================
  double FrankRead::computeActivationAngle(double phi0) const {
    using tfel::math::CubicRoots;
    double nu = _linetension.getNu();
    double alpha = 1. - nu * cos(phi0) * cos(phi0);
    double beta = 1. - nu * sin(phi0) * sin(phi0);
    double gamma = -nu * sin(2. * phi0);
    double Q = (2. * alpha - beta) / (3. * alpha);
    double R = -gamma / (2. * alpha);
    double D = Q * Q * Q + R * R;
    double SC = R + sqrt(D);
    double TC = R - sqrt(D);
    double x0 = CubicRoots::cbrt(SC) + CubicRoots::cbrt(TC);
    return atan2(1., x0);
  }

}  // end of namespace numodis
