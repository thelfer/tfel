/*!
 * \file   include/TFEL/Material/Eshelby.hxx
 * \author Antoine Martin
 * \date   15 October 2024
 * \brief  This file declares the Eshelby tensor for an ellipsoidal inclusion embedded in an isotropic matrix.
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ESHELBY_HXX
#define LIB_TFEL_MATERIAL_ESHELBY_HXX

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"

namespace tfel::material
{
  /*!
   * This function builds the Eshelby tensor for an ellipsoid whose semi-axis lengths are a,b,c
   * embedded in an isotropic matrix (young,nu). 
   * It returns an object of type st2tost2, which is the fourth-order Eshelby tensor, in a basis which is adapted to the ellipsoid :
   * see Eshelby.ixx for the details. The basis IS NOT RELATED to the given order of the values a,b,c
   * These expressions can be found in Torquato 2002 Random Heterogeneous Materials for the axisymmetric ellipsoid
   * and in Eshelby 1957 for other cases.
   * \tparam    real: underlying type
   * param 1 : nu
   * param 2 : semi-axis of ellipsoid in direction 1
   * param 3 :semi-axis of ellipsoid in direction 2
   * param 4 : semi-axis of ellipsoid in direction 3
   */
     template <typename real, typename LengthType>
  TFEL_HOST_DEVICE tfel::math::st2tost2<3u,real> EshelbyTensor(
      const LengthType&,
      const LengthType&,
      const LengthType&,
      const real&);
      
  /*!
   * This function builds the localisation tensor for an ellipsoid whose semi-axis lengths are a,b,c,
   * whose elasticity tensor is C_i, 
   * and embedded in an isotropic matrix (young,nu).
   * The localisation tensor A is defined as follows : eps=A:eps0 where eps0 is the uniform strain tensor imposed at infinity,
   * and eps is the strain tensor solution of Eshelby problem for the ellipsoid.
   * The ellipsoid also has a specific orientation given by the vectors n_a, n_b.
   * \returns an object of type st2tost2, which is the fourth-order localisation tensor A.
   * \tparam    real: underlying type
   * param 1,2 : young,nu : parameters of the isotropic matrix
   * param 3 : st2tost2<3u,real> C_i : elastic tensor of the ellipsoid (may be anisotropic).
   * param 4 : n_a : tfel::math::tvector<3u,real>,  direction of the principal axis whose length is a
   * param 5 : a : length of semi-axis relative to the direction n_a
   * param 6: n_b : tfel::math::tvector<3u,real>,  direction of the principal axis whose length is b
   * param 7 : b : length of semi-axis relative to the direction n_b
   * param 8 : c : length of the remaining semi-axis
   */
     template <typename real, typename StressType, typename LengthType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,real> GeneralEllipsoidLocalisationTensor(
      const StressType&,
      const real&,
      const tfel::math::st2tost2<3u, StressType>&,
      const tfel::math::tvector<3u,real>&,
      const real&,
      const tfel::math::tvector<3u,real>&,
      const real&,
      const real&);
      
     /*
      * The same function as GeneralEllipsoidLocalisationTensor for the case of an axisymmetric ellipsoid
      * The geometrical parameters are now n_a, the axis of the ellipsoid, and the aspect ratio e=a/b
      */
     template <typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,real> AxisymmetricalEllipsoidLocalisationTensor(
      const StressType&,
      const real&,
      const tfel::math::st2tost2<3u, StressType>&,
      const tfel::math::tvector<3u,real>&,
      const real&);
      
      /*
      * The same function as EllipsoidLocalisationTensor for the case of a sphere
      */
    template <typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,real> SphereLocalisationTensor(
      const StressType&,
      const real&,
      const tfel::math::st2tost2<3u, StressType>&,
      const tfel::math::tvector<3u,real>&,
      const LengthType&,
      const tfel::math::tvector<3u,real>&,
      const LengthType&,
      const LengthType&);
      
      
      template <typename LengthType>
  TFEL_HOST_DEVICE const std::array<int,3> sort_ind(const LengthType&, const LengthType&, const LengthType&);
  

}  // end of namespace tfel::material

#include "TFEL/Material/Eshelby.ixx"

#endif /* LIB_TFEL_MATERIAL_ESHELBY_HXX */
