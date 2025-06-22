/*!
 * \file
 * mfront/include/MFront/BehaviourBrick/IsotropicDamageHookeStressPotential.hxx
 * \brief
 * \author Thomas Helfer
 * \date   30/04/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICDAMAGEHOOKESTRESSPOTENTIAL_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICDAMAGEHOOKESTRESSPOTENTIAL_HXX

#include "MFront/BehaviourBrick/IsotropicDamageHookeStressPotentialBase.hxx"

namespace mfront {

  // forward declaration
  struct MaterialPropertyDescription;

}  // end of namespace mfront

namespace mfront::bbrick {

  /*!
   * \brief class describing the computation of the stress through the
   * Hooke law coupled with isotropic damage.
   * The evolution of the damage must be defined by the user.
   */
  struct IsotropicDamageHookeStressPotential
      : IsotropicDamageHookeStressPotentialBase {
    //! \brief constructor
    IsotropicDamageHookeStressPotential();
    //! destructor
    ~IsotropicDamageHookeStressPotential() override;
  };  // end of struct IsotropicDamageHookeStressPotential

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICDAMAGEHOOKESTRESSPOTENTIAL_HXX \
        */
