/*!
 * \file   include/TFEL/Material/EshelbyBasedHomogenization.hxx
 * \author Antoine Martin
 * \date   24 October 2024
 * \brief  This file declares some homogenization schemes based on solution of Eshelby problem.
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ESHELBYBASEDHOMOGENIZATION_HXX
#define LIB_TFEL_MATERIAL_ESHELBYBASEDHOMOGENIZATION_HXX

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/Eshelby.hxx"


namespace tfel::material {
   /*!
   * General Dilute scheme knowing the localisator
   * \tparam real: underlying type
   * \returns an object of type st2tost2, which is the homogenized stiffness tensor, assuming that the mean localisation tensor A is given in parameter
   * param 1,2 : young, nu : parameters of isotropic matrix
   * param 3 : f : volumic fraction 
   * param 4 : C_i : st2tost2<3u,real> elastic tensor of spheres (the same for all spheres). May be anisotropic.
   * param 5 : A : mean localisation tensor on inclusions
   */
   template <typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,StressType> DiluteScheme(
      const StressType&,
      const real&,
      const real&,
      const tfel::math::st2tost2<3u,StressType>&,
      const tfel::math::st2tost2<3u,real>&);
      
 /*!
   * General Mori-Tanaka scheme knowing the localisator
   * \tparam real: underlying type
   * \returns an object of type st2tost2, which is the homogenized stiffness tensor, assuming that the mean localisation tensor A is given in parameter
   * param 1,2 : young, nu : parameters of isotropic matrix
   * param 3 : f : volumic fraction 
   * param 4 : C_i : st2tost2<3u,real> elastic tensor of spheres (the same for all spheres). May be anisotropic.
   * param 5 : A : mean localisation tensor on inclusions (this tensor is the same as the one given in DiluteScheme)
   */
  template <typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,StressType> MoriTanakaScheme(
      const StressType&,
      const real&,
      const real&,
      const tfel::math::st2tost2<3u,StressType>&,
      const tfel::math::st2tost2<3u,real>&);
      
      /*!
   * Dilute scheme for spheres
   * Same parameters as for DiluteScheme, except A
   */
  template <typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,StressType> SphereDiluteScheme(
      const StressType&,
      const real&,
      const real&,
      const tfel::math::st2tost2<3u,StressType>&);


/*!
   * Mori-Tanaka scheme for spheres
   * Same parameters as for MoriTanakaScheme, except A
   */
  template <typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,StressType> SphereMoriTanakaScheme(
      const StressType&,
      const real&,
      const real&,
      const tfel::math::st2tost2<3u,StressType>&);
      

  /*!
   * Dilute scheme with isotropic distribution of orientations of ellipsoids
   * \tparam real: underlying type
   * \returns an object of type st2tost2, which is the homogenized stiffness tensor
   * param 1,2 : young, nu : parameters of isotropic matrix
   * param 3 : f : volumic fraction 
   * param 4 : C_i : st2tost2<3u,real> elastic tensor of inclusions (the same for all inclusions). May be anisotropic.
   * param 5,6,7 : a,b,c : lengths of semi-axes of the ellipsoids (the same triplet for all ellipsoids). Can be ordered in any way.
   */
     template <typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,StressType> IsotropicDiluteScheme(
      const StressType&,
      const real&,
      const real&,
      const tfel::math::st2tost2<3u,StressType>&,
      const real&,
      const real&,
      const real&);
      
      
     /*!
   * Dilute scheme with a "planar isotropic" distribution of ellipsoids : one principal axis (the same principal axis for all ellipsoids) is oriented in a fixed direction n. The distribution of the other axes are in the plane normal to n, and the distribution of these axes inside this plane is isotropic.
   * \tparam real: underlying type
   * \returns an object of type st2tost2, which is the homogenized stiffness tensor
   * param 1,2 : young, nu : parameters of isotropic matrix
   * param 3 : f : volumic fraction 
   * param 4 : C_i : st2tost2<3u,real> elastic tensor of inclusions (the same for all inclusions). May be anisotropic.
   * param 5 : n_a : vector<real>,  direction of the principal axis which has a fixed orientation
   * param 6 : a : length of semi-axis which has a fixed orientation n_a
   * param 7,8 : b,c : lengths of the other semi-axes
   */
     template <typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,StressType> PlanarIsotropicDiluteScheme(
      const StressType&,
      const real&,
      const real&,
      const tfel::math::st2tost2<3u,StressType>&,
      const std::vector<real>&,
      const real&,
      const real&,
      const real&);
      
     /*!
   * Dilute scheme with a unique orientation of ellipsoids : all principal axes have a fixed orientation.
   * \tparam real: underlying type
   * \returns an object of type st2tost2, which is the homogenized stiffness tensor
   * param 1,2 : young, nu : parameters of isotropic matrix
   * param 3 : f : volumic fraction 
   * param 4 : C_i : st2tost2<3u,real> elastic tensor of inclusions (the same for all inclusions). May be anisotropic.
   * param 5 : n_a : vector<real>,  direction of the principal axis whose length is a
   * param 6 : a : length of semi-axis relative to the direction n_a
   * param 7 : n_b : vector<real>,  direction of the principal axis whose length is b
   * param 8 : b : length of semi-axis relative to the direction n_b
   * param 9 : c : length of the remaining semi-axis
   */
     template <typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,StressType> OrientedDiluteScheme(
      const StressType&,
      const real&,
      const real&,
      const tfel::math::st2tost2<3u,StressType>&,
      const std::vector<real>&,
      const real&,
      const std::vector<real>&,
      const real&,
      const real&);
      
      /*!
   * Mori-Tanaka scheme with isotropic distribution of orientations of ellipsoids
   * the parameters are the same as for IsotropicDiluteScheme
   */      
     template <typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,StressType> IsotropicMoriTanakaScheme(
      const StressType&,
      const real&,
      const real&,
      const tfel::math::st2tost2<3u,StressType>&,
      const real&,
      const real&,
      const real&);
      
      /*!
   * Mori-Tanaka scheme with a "planar isotropic" distribution of ellipsoids
   * Parameters are the same as for PlanarIsotropicDiluteScheme
   */
    template <typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,StressType> PlanarIsotropicMoriTanakaScheme(
      const StressType&,
      const real&,
      const real&,
      const tfel::math::st2tost2<3u,StressType>&,
      const std::vector<real>&,
      const real&,
      const real&,
      const real&);
      
    /*!
   * Mori-Tanaka scheme with a unique orientation of ellipsoids.
   * Parameters are the same as for OrientedDiluteScheme
   */
     template <typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,StressType> OrientedMoriTanakaScheme(
      const StressType&,
      const real&,
      const real&,
      const tfel::math::st2tost2<3u,StressType>&,
      const std::vector<real>&,
      const real&,
      const std::vector<real>&,
      const real&,
      const real&);
      
      /*
      * A voir si on ajoute ce schéma
      */
     template <typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,real> PonteCastanedaWillisScheme(
      const StressType&,
      const real&,
      const real&,
      const real&);

}  // end of namespace tfel::material

#include "TFEL/Material/EshelbyBasedHomogenization.ixx"

#endif /* LIB_TFEL_MATERIAL_ESHELBYBASEDHOMOGENIZATION_HXX */
