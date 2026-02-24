/*!
 * \file   include/NUMODIS/FrankRead.hxx
 * \brief  Provide all the tools to compute the properties of a FR source  .
 * \author Laurent Dupuy
 * \date   9/06/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_NUMODIS_FRANKREAD_HXX
#define LIB_NUMODIS_FRANKREAD_HXX

#include <math.h>
#include "NUMODIS/Config.hxx"
#include "NUMODIS/IsotropicLineTensionModel.hxx"

namespace numodis {

  struct Crystallo;
  struct GSystem;

  //===============================================================================================
  // class FrankRead
  //-----------------------------------------------------------------------------------------------
  //! Provide all the tools to compute the properties of a FR source within the
  //! line tension model
  //===============================================================================================
  struct TFELNUMODIS_VISIBILITY_EXPORT FrankRead {
    FrankRead(const IsotropicLineTensionModel& linetension);

    double computeBetaF(double alpha,
                        double beta0,
                        double tol = 0.00001,
                        int Nmax = 20) const;

    double computeActivationAngle(double alpha) const;

    //====================================================================
    // FrankRead::Fx
    //--------------------------------------------------------------------
    //! Compute the force acting on the first node of a FR source along x
    //--------------------------------------------------------------------
    /*!
      x is the direction conneting the two nodes of the Frank-Read source
      \param alpha angle between the Burgers vector and the x direction
      \param phi tangeant vector at the first node
      \return force acting on the first node along x direction
    */
    //====================================================================
    double Fx(double alpha, double phi) const {
      return _linetension.E(phi - alpha) * cos(phi) -
             _linetension.dEdtheta(phi - alpha) * sin(phi);
    }

    //====================================================================
    // FrankRead::Fy
    //--------------------------------------------------------------------
    //! Compute the force acting on the first node of a FR source along y
    //--------------------------------------------------------------------
    /*!
      y is the direction orthogonal to the Frank-Read source
      \param alpha angle between the Burgers vector and the x direction
      \param phi tangeant vector at the first node
      \return force acting on the first node along y direction
    */
    //====================================================================
    double Fy(double alpha, double phi) const {
      return _linetension.E(phi - alpha) * sin(phi) +
             _linetension.dEdtheta(phi - alpha) * cos(phi);
    }

    //=====================================================================
    // FrankRead::SigmaBurgersLength
    //---------------------------------------------------------------------
    //! Compute the stress acting on a Frank-Read source from its geometry
    //---------------------------------------------------------------------
    /*!
      \param alpha angle between the Burgers vector and the x direction
      \param beta0 tangeant vector at the first node
      \param betaF tangeant vector at the second node
      \return product between the stress, the Burgers vector and the Length
    */
    //=====================================================================
    double SigmaBurgersLength(double alpha, double beta0, double betaF) const {
      return this->Fy(alpha, beta0) - this->Fy(alpha, betaF);
    }

   private:
    //=====================================================================
    // FrankRead::Equation4
    //---------------------------------------------------------------------
    //! At equilibrium the Fx on the two nodes should be equal
    //---------------------------------------------------------------------
    /*!
      see Dupuy & Fivel (Acta Materiala 2002)
      \param alpha angle between the Burgers vector and the x direction
      \param beta0 tangeant vector at the first node
      \param betaF tangeant vector at the second node
      \return difference between the two Fx forces
    */
    //=====================================================================
    double Equation4(double alpha, double beta0, double betaF) const {
      return this->Fx(alpha, beta0) - this->Fx(alpha, betaF);
    }

    //! line tension model
    const IsotropicLineTensionModel& _linetension;

  };  // end of struct FrankRead

}  // end of namespace numodis

#endif /* LIB_NUMODIS_FRANKREAD_HXX*/
