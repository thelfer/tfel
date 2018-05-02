/*!
 * \file
 * mfront/include/MFront/BehaviourBrick/IsotropicDamageHookeStressPotentialBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   30/04/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICDAMAGEHOOKESTRESSPOTENTIALBASE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICDAMAGEHOOKESTRESSPOTENTIALBASE_HXX

#include <array>
#include <memory>
#include "TFEL/Utilities/GenTypeBase.hxx"
#include "MFront/BehaviourBrick/HookeStressPotentialBase.hxx"

namespace mfront {

  // forward declaration
  struct MaterialPropertyDescription;

  namespace bbrick {

    /*!
     * \brief class describing the computation of the stress through the
     * Hooke law coupled with isotropic damage.
     */
    struct IsotropicDamageHookeStressPotentialBase : HookeStressPotentialBase {
      //! \brief constructor
      IsotropicDamageHookeStressPotentialBase();
      //! destructor
      ~IsotropicDamageHookeStressPotentialBase() override;

     protected:
      bool handleIsotropicDamage() override;
    };  // end of struct IsotropicDamageHookeStressPotentialBase

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICDAMAGEHOOKESTRESSPOTENTIALBASE_HXX \
          */
