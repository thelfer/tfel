/*!
 * \file   mfront/include/MFront/BehaviourBrick/HookeStressPotential.hxx
 * \brief
 * \author Thomas Helfer
 * \date   20/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_HOOKESTRESSPOTENTIAL_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_HOOKESTRESSPOTENTIAL_HXX

#include "MFront/BehaviourBrick/HookeStressPotentialBase.hxx"

namespace mfront {

  namespace bbrick {

    /*!
     * \brief class describing the computation of the stress through the
     * standard Hooke law.
     */
    struct HookeStressPotential : HookeStressPotentialBase{
      HookeStressPotential();
      //! destructor
      ~HookeStressPotential() override;

     protected:
      bool handleIsotropicDamage() const override;
    };  // end of struct HookeStressPotential

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_HOOKESTRESSPOTENTIAL_HXX */
