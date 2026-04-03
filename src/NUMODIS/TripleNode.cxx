/*!
 * \file   src/NUMODIS/TripleNode.cxx
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

#include <iostream>

#include "TFEL/Math/General/IEEE754.hxx"
#include "NUMODIS/TripleNode.hxx"
#include "NUMODIS/IsotropicLineTensionModel.hxx"
#include "NUMODIS/FrankRead.hxx"
#include "NUMODIS/GSystem.hxx"
#include "NUMODIS/IDirection.hxx"
#include "NUMODIS/Crystallo.hxx"

namespace numodis {

  //===========================================================================
  //! TripleNode::computeFx
  //---------------------------------------------------------------------------
  //! compute the force acting on the triple node along the junction direction
  //---------------------------------------------------------------------------
  /*!
    see Dupuy & Fivel (Acta Materiala 2002)

    \param beta0 angle between the first segment and its rest position
    \param beta1 angle between the second segment and its rest position
    \return force acting on the triple node along the junction direction

  */
  //===========================================================================
  double TripleNode::computeFx(double phi0,
                               double beta0,
                               double phi1,
                               double beta1) const {
    return _lt0.Fx(_alpha0, phi0 + beta0) + _lt1.Fx(_alpha1, phi1 + beta1) -
           _lt2.Fx(_alpha2, 0.0);
  }

  //===================================================================
  //! TripleNode::computeInitialAngle
  //-------------------------------------------------------------------
  //! compute the equilibrium angle of a triple node under zero stress
  //-------------------------------------------------------------------
  /*!
    see Dupuy & Fivel (Acta Materiala 2002)

    \return angle in the range [0;pi]

  */
  //===================================================================
  double TripleNode::computeInitialAngle(double tol, int Nmax) const {
    // pi = 3.14159...
    double pi = std::atan(1.0) * 4.0;

    // range for betaF
    double phimin = 0.0;
    double phimax = pi;

    double Eqmin = this->computeFx(phimin, 0.0, phimin, 0.0);
    double Eqmax = this->computeFx(phimax, 0.0, phimax, 0.0);
    if (Eqmin * Eqmax > 0.0)
      throw -1;  // throw: both sides have positive values

    // find phi using a simple bisection method
    int N = 0;
    while (N < Nmax) {
      double phimid = (phimax + phimin) / 2.0;
      double Eqmid = this->computeFx(phimid, 0.0, phimid, 0.0);
      if ((tfel::math::ieee754::fpclassify(Eqmid) == FP_ZERO) ||
          ((phimax - phimin) / 2.0 < tol)) {
        return phimid;
      }
      ++N;
      if (std::signbit(Eqmid) == std::signbit(Eqmin)) {
        phimin = phimid;
        Eqmin = Eqmid;
      } else {
        phimax = phimid;
        Eqmax = Eqmid;
      }
    }

    throw -2;  // throw: calculation did not converge
  }

  //===================================================================
  // TripleNode::computeCriticalStress
  //-------------------------------------------------------------------
  //! Compute the activation stress of a triple node
  //-------------------------------------------------------------------
  /*!
    \param phi0 initial angle under zero stress
    \param step angular step
    \param tol tolerance / precision of the calculation
    \param Nmax maximum number of iterations
    \return activation stress of the triple node

  */
  //===================================================================
  double TripleNode::computeCriticalStress(double phi0,
                                           double step,
                                           double tol,
                                           int Nmax) const {
    // initialization
    bool activated = false;
    double phi = phi0 - step;
    double sigmaMax = 0.0;
    // increase angle phi
    while (!activated && phi > 0.0) {
      // compute beta0 needed to equilibrate the triple node
      double beta0;
      try {
        beta0 = this->computeEquilibriumBeta0(phi);
      } catch (int) {
        std::cerr << "=> could not find beta0 at equilibrium" << std::endl;
        throw -1;
      }

      // compute betaF from beta0
      double betaF;
      try {
        betaF = _lt0.computeBetaF(_alpha0 - phi, beta0, tol, Nmax);
      } catch (int) {
        std::cerr << "=> could not find betaF from beta0" << std::endl;
        throw -2;
      }
      // compute the corresponding stress
      double sigma =
          _lt0.SigmaBurgersLength(_alpha0 - phi, beta0, betaF) * sin(phi);

      //      std::cout << " ? phi =" << phi << " => sigma = " << sigma <<
      //      std::endl;

      // maximum value?
      if (sigma > sigmaMax) {
        sigmaMax = sigma;
      } else if (sigma < 0.95 * sigmaMax)
        activated = true;

      // decrease phi
      phi -= step;
    }

    return sigmaMax;
  }

  //===================================================================
  // TripleNode::computeEquilibriumBeta0
  //-------------------------------------------------------------------
  //! Compute beta0 such that the triple node is at equilibrium
  //-------------------------------------------------------------------
  /*
   */
  //===================================================================
  double TripleNode::computeEquilibriumBeta0(double phi,
                                             double tol,
                                             int Nmax) const {
    // range for beta0
    double beta0min = 0.0;
    double beta0max = _lt0.computeActivationAngle(_alpha0 - phi);

    double Fxmin = this->computeFx(phi, beta0min, phi, 0.0);
    double Fxmax = this->computeFx(phi, beta0max, phi, 0.0);
    if (Fxmin * Fxmax > 0.0)
      throw -1;  // throw: both sides have the same values

    // find beta0 using a simple bisection method
    int N = 0;
    while (N < Nmax) {
      double beta0mid = (beta0max + beta0min) / 2.0;
      double Fxmid = this->computeFx(phi, beta0mid, phi, 0.0);
      if ((tfel::math::ieee754::fpclassify(Fxmid) == FP_ZERO) ||
          ((beta0max - beta0min) / 2.0 < tol)) {
        return beta0mid;
      }
      ++N;
      if (std::signbit(Fxmid) == std::signbit(Fxmin)) {
        beta0min = beta0mid;
        Fxmin = Fxmid;
      } else {
        beta0max = beta0mid;
        Fxmax = Fxmid;
      }
    }
    throw -2;  // throw: calculation did not converge
  }

}  // end of namespace numodis
