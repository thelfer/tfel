/*!
 * \file   include/NUMODIS/TripleNode.hxx
 * \brief  compute the properties of a triple node
 * \author Laurent Dupuy
 * \date   9/06/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_NUMODIS_TRIPLENODE_HXX
#define LIB_NUMODIS_TRIPLENODE_HXX

#include "NUMODIS/Config.hxx"

namespace numodis {

  struct FrankRead;

  //===================================================================
  // class TripleNode
  //-------------------------------------------------------------------
  //! Provide all the tools to compute the properties of a triple node
  //===================================================================
  struct TFELNUMODIS_VISIBILITY_EXPORT TripleNode {
    double computeInitialAngle(double tol = 0.00001, int Nmax = 20) const;

    double computeCriticalStress(double phi0,
                                 double step = 0.001,
                                 double tol = 0.00001,
                                 int Nmax = 20) const;

    TripleNode(const FrankRead& lt0,
               const double alpha0,
               const FrankRead& lt1,
               const double alpha1,
               const FrankRead& lt2,
               const double alpha2)
        : _lt0(lt0),
          _alpha0(alpha0),
          _lt1(lt1),
          _alpha1(alpha1),
          _lt2(lt2),
          _alpha2(alpha2) {}

   private:
    double computeFx(double phi0, double beta0, double ph1, double beta1) const;

    double computeEquilibriumBeta0(double phi,
                                   double tol = 0.00001,
                                   int Nmax = 20) const;

    //! line tension properties of the first segment of the node
    const FrankRead& _lt0;

    //! angle between the junction direction and the Burgers vector of the first
    //! segment
    double _alpha0;

    //! line tension properties of the second segment of the node
    const FrankRead& _lt1;

    //! angle between the junction direction and the Burgers vector of the
    //! second segment
    double _alpha1;

    //! line tension properties of the junction
    const FrankRead& _lt2;

    //! angle between the junction direction and the Burgers vector of the
    //! junction
    double _alpha2;
  };

}  // end of namespace numodis

#endif /* LIB_NUMODIS_TRIPLENODE_HXX */
