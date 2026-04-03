/*!
 * \file   src/NUMODIS/AnalyseJunction.cxx
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

#include <algorithm>
#include <iostream>
#include "NUMODIS/AnalyseJunction.hxx"
#include "NUMODIS/GSystem.hxx"
#include "NUMODIS/Crystallo.hxx"
#include "NUMODIS/TripleNode.hxx"
#include "NUMODIS/IsotropicLineTensionModel.hxx"
#include "NUMODIS/FrankRead.hxx"

namespace numodis {

  namespace Dupuy2017 {
    //======================================================================================================================
    // precomputeJunctionParameters
    //----------------------------------------------------------------------------------------------------------------------
    //! analyse the interaction between two glide systems and return the
    //! parameters to be used for a line tension analysis
    //----------------------------------------------------------------------------------------------------------------------
    /*!
      Considering two interacting glide systems (gsystem1 = studied system,
      gsystem2 = forest), pre-compute the parameters that will be later used
      within the line tension model to estimate the strenght of the junction and
      the corresponding hardening coefficient.

      \param pcrystallo pointer to the relevant crystallography of the material
      \param gsystem1 glide system under consideration
      \param gsystem2 glide system of the forest
      \param Burgers vectors of the gsystem1, gsystem2 and the junction
      \param phi angles between the Burgers vectors and the junction direction

    */
    //======================================================================================================================
    void precomputeJunctionParameters(const Crystallo* pcrystallo,
                                      const GSystem& gsystem1,
                                      const GSystem& gsystem2,
                                      std::vector<double>& burgers,
                                      std::vector<double>& phi) {
      // same glide plane?
      if (gsystem1.getIPlane() == gsystem2.getIPlane()) {
        // same Burgers vector?
        if (Coincide(gsystem1.getIBurgers(), gsystem2.getIBurgers()) != 0) {
          std::cout << "self";
        } else {
          std::cout << "coplanar";
        }
        throw(-1);
      } else {
        // compute junction direction
        IDirection ijunction(pcrystallo->getNindices());
        pcrystallo->CrossProduct(gsystem1.getIPlane(), gsystem2.getIPlane(),
                                 ijunction);
        Vect3 xjunction(pcrystallo->direction(ijunction));

        // first Burgers vector
        Vect3 xburgers1(pcrystallo->burgers_vector(gsystem1.getIBurgers()));
        burgers[0] = xburgers1.Length();
        double cosphi1 = xburgers1.Dot(xjunction) / burgers[0];
        cosphi1 = std::max(std::min(cosphi1, 1.0), -1.0);
        phi[0] = acos(cosphi1);

        // second Burgers vector
        Vect3 xburgers2(pcrystallo->burgers_vector(gsystem2.getIBurgers()));
        burgers[1] = xburgers2.Length();
        double cosphi2 = xburgers2.Dot(xjunction) / burgers[1];
        cosphi2 = std::max(std::min(cosphi2, 1.0), -1.0);
        phi[1] = acos(cosphi2);

        // Burgers vector of the junction
        burgers[2] = 0.0;
        double cosphi3 = 0.0;
        phi[2] = 0.0;
        if (Coincide(gsystem1.getIBurgers(), gsystem2.getIBurgers()) == 0) {
          GSystem gsystem3(
              pcrystallo->ComputeJunctionGSystem(gsystem1, gsystem2));
          Vect3 xburgers3(pcrystallo->burgers_vector(gsystem3.getIBurgers()));
          burgers[2] = xburgers3.Length();
          cosphi3 = xburgers3.Dot(xjunction) / burgers[2];
          cosphi3 = std::max(std::min(cosphi3, 1.0), -1.0);
          phi[2] = acos(cosphi3);
        }
      }
    }

    //====================================================================================
    // computeNormalizedStress
    //------------------------------------------------------------------------------------
    //! compute the critical/normalized stress of a junction based on line
    //! tension
    //------------------------------------------------------------------------------------
    /*!

      In this method, we assume that the normalized stress is given by the
      stress required to unstabilize the weakest triple node of a junction. A
      very simple, yet, straightforward average is made considering all possible
      symmetric geometries of these junctions (see Dupuy et al. 2017).

      \param nu Poisson coefficient
      \param burgers Burgers vectors of the junction (b1 = glide, b2 = forest
      and b3 = junction) \param phi angles between the Burgers vectors and the
      junction direction (rad) \return normalized junction strength.

    */
    //====================================================================================
    double computeNormalizedStress(double nu,
                                   const std::vector<double>& burgers,
                                   const std::vector<double>& phi) {
      // line tension model for all three segments
      double mu = 1.0;
      IsotropicLineTensionModel lt1(mu, burgers[0], nu);
      FrankRead fr1(lt1);
      IsotropicLineTensionModel lt2(mu, burgers[1], nu);
      FrankRead fr2(lt2);
      IsotropicLineTensionModel lt3(mu, burgers[2], nu);
      FrankRead fr3(lt3);

      // consider all the 4 possibles symmetries
      double averageStress = 0;
      for (int i = 0; i != 4; i++) {
        int sgn1 = 1 - 2 * (i % 2);
        int sgn2 = 1 - 2 * (i / 2);

        TripleNode tn(fr1, sgn1 * phi[0], fr2, sgn2 * phi[1], fr3, phi[2]);
        double anglerad;
        try {
          anglerad = tn.computeInitialAngle();
        } catch (int error) {
          if (error == -1)
            std::cerr << "=> no critical angle <=> no junction " << std::endl;
          continue;
        }

        double stress = 0;
        try {
          stress = tn.computeCriticalStress(anglerad, 0.001, 0.00001, 20);
        } catch (int) {
          std::cerr << "=>could not compute critical stress of the triple node"
                    << std::endl;
        }
        averageStress += stress;
      }

      // normalized average stress for the 4 symmetries
      averageStress /= 4.0 * burgers[0] * burgers[0];

      return averageStress;
    }

    //=========================================================================
    // computeHeuristicNonColinearHardeningCoefficient
    //-------------------------------------------------------------------------
    //! Compute an heuristic hardening coefficient for non colinear junctions
    //-------------------------------------------------------------------------
    /*!

      see Dupuy et al. 2017

      \param nu Poisson coefficient
      \param sij normalized junction strength computed using linetension model
      \return hardening coefficient

    */
    //=========================================================================
    double computeHeuristicNonColinearHardeningCoefficient(double, double sij) {
      return pow(0.21 * sij + 0.37, 2.0);
    }

    //=========================================================================
    // computeHeuristicColinearHardeningCoefficient
    //-------------------------------------------------------------------------
    //! Compute an heuristic hardening coefficient for colinear junctions
    //-------------------------------------------------------------------------
    /*!

      see Dupuy et al. 2017

      \param pcrystallo pointer to a Crystallo object
      \param nu Poisson coefficient
      \param iplane1 glide plane of the first dislocation
      \param iplane2 glide plane of the second dislocation
      \return hardening coefficient

    */
    //=========================================================================
    double computeHeuristicColinearHardeningCoefficient(
        const Crystallo* pcrystallo,
        double nu,
        const IPlane& iplane1,
        const IPlane& iplane2) {
      Vect3 n1(pcrystallo->normal(iplane1));
      Vect3 n2(pcrystallo->normal(iplane2));
      return 0.4 * nu + 0.8 - 0.3 * std::abs(n1.Dot(n2));
    }

    //=========================================================================
    // computeHeuristicHardeningCoefficient
    //-------------------------------------------------------------------------
    //! Compute an heuristic hardening coefficient
    //-------------------------------------------------------------------------
    /*!

      see Dupuy et al. 2017

      \param pcrystallo pointer to a Crystallo object
      \param nu Poisson coefficient
      \param iplane1 glide plane of the first dislocation
      \param iplane2 glide plane of the second dislocation
      \param bj norm of the Burgers vector of the junction
      \param sij normalized junction strength computed using linetension model
      \return hardening coefficient

    */
    //=========================================================================
    double computeHeuristicHardeningCoefficient(const Crystallo* pcrystallo,
                                                double nu,
                                                const IPlane& iplane1,
                                                const IPlane& iplane2,
                                                double bj,
                                                double sij) {
      if (bj < 0.001)
        return computeHeuristicColinearHardeningCoefficient(pcrystallo, nu,
                                                            iplane1, iplane2);
      else
        return computeHeuristicNonColinearHardeningCoefficient(nu, sij);
    }

  }  // end of namespace Dupuy2017

}  // end of namespace numodis
