/*!
 * \file   include/NUMODIS/IsotropicLineTensionModel.hxx
 * \brief compute the line energy of a straight dislocation within
 * isotropic elasticity.
 * \author Laurent Dupuy
 * \date   9/06/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_NUMODIS_ISOTROPICLINETENSIONMODEL_HXX
#define LIB_NUMODIS_ISOTROPICLINETENSIONMODEL_HXX

#include <cmath>
#include "NUMODIS/Config.hxx"

namespace numodis {

  //=============================================================================
  // class IsotropicLineTensionModel
  //-----------------------------------------------------------------------------
  //! Provide all the tools to compute the line energy of a straight dislocation
  //=============================================================================
  struct TFELNUMODIS_VISIBILITY_EXPORT IsotropicLineTensionModel {
    //======================================================
    // IsotropicLineTensionModel::IsotropicLineTensionModel
    //------------------------------------------------------
    //! Constructor
    //------------------------------------------------------
    /*!
      \param mu shear modulus
      \param burgers Burgers vector
      \param nu Poisson coefficient
    */
    //======================================================
    IsotropicLineTensionModel(double mu, double burgers, double nu)
        : _mu(mu), _burgers(burgers), _nu(nu) {}

    //===============================================================
    // IsotropicLineTensionModel::E
    //---------------------------------------------------------------
    //! compute the energy per unit length of a straight dislocation
    //---------------------------------------------------------------
    /*!
      \param theta angle between the Burgers vector and the line
      \return energy per unit length
    */
    //===============================================================
    double E(double theta) const {
      return _mu * _burgers * _burgers * (1.0 - _nu * pow(cos(theta), 2)) /
             (1.0 - _nu);
    }

    //================================================================
    // IsotropicLineTensionModel::dEdtheta
    //----------------------------------------------------------------
    //! compute the derivative of the energy per unit length wr theta
    //----------------------------------------------------------------
    /*!
      \param theta angle between the Burgers vector and the line
      \return derivative of the energy per unit length
    */
    //================================================================
    double dEdtheta(double theta) const {
      return _mu * _burgers * _burgers * _nu * sin(2.0 * theta) / (1.0 - _nu);
    }

    //================================================================
    // IsotropicLineTensionModel::getNu
    //----------------------------------------------------------------
    //! get the Poisson coefficient used in this model
    //----------------------------------------------------------------
    /*! \return Poisson coefficient                                 */
    //================================================================
    double getNu() const { return _nu; }

   private:
    //! shear modulus
    double _mu;

    //! burgers vector
    double _burgers;

    //! poisson coefficient
    double _nu;
  };

}  // end of namespace numodis

#endif /* IsotropicLineTensionModel */
