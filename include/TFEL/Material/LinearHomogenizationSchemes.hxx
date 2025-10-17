/*!
 * \file   include/TFEL/Material/LinearHomogenizationSchemes.hxx
 * \author Antoine Martin
 * \date   24 October 2024
 * \brief  This file declares some homogenization schemes based on solution of
 * Eshelby problem.
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All
 * rights reserved. This project is publicly released under either the GNU GPL
 * Licence or the CECILL-A licence. A copy of thoses licences are delivered with
 * the sources of TFEL. CEA or EDF may also distribute this project under
 * specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_LINEARHOMOGENIZATIONSCHEMES_HXX
#define LIB_TFEL_MATERIAL_LINEARHOMOGENIZATIONSCHEMES_HXX

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/LocalisationTensor.hxx"
#include "TFEL/Material/IsotropicModuli.hxx"
#include "TFEL/Material/EshelbyTolerances.hxx"

namespace tfel::material::homogenization::elasticity {

  /*!
   * This function gives the homogenized stiffness for a dilute scheme,
   * knowing the strain localisation tensor.
   * \tparam StressType: type of the elastic constants related to the matrix and
   * the inclusions
   * \return an object of type st2tost2<3u,StressType>
   * \param [in] young,nu: Young modulus and Poisson's ratio of the matrix
   * \param [in] f: volumic fraction of inclusions
   * \param [in] young_i,nu_i: Young modulus and Poisson's ratio of the
   * inclusions
   * \param [in] A: mean strain localisation tensor of inclusions
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const tfel::math::st2tost2<3u, StressType> computeDiluteScheme(
          const StressType&,
          const types::real<StressType>&,
          const types::real<StressType>&,
          const StressType&,
          const types::real<StressType>&,
          const tfel::math::st2tost2<3u, types::real<StressType>>&);

  /*!
   * This function gives the homogenized stiffness for a Mori-Tanaka scheme
   * knowing the strain localisation tensor.
   * \return an object of type st2tost2<3u,StressType>
   * \tparam StressType: type of the elastic constants related to the matrix
   * and the inclusions
   * \param [in] young,nu: Young modulus and Poisson's ratio of the matrix
   * \param [in] f: volumic fraction of inclusions
   * \param [in] young_i,nu_i: Young modulus and Poisson's ratio of the
   * inclusions
   * \param [in] A: mean strain localisation tensor of inclusions
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const tfel::math::st2tost2<3u, StressType> computeMoriTanakaScheme(
          const StressType&,
          const types::real<StressType>&,
          const types::real<StressType>&,
          const StressType&,
          const types::real<StressType>&,
          const tfel::math::st2tost2<3u, types::real<StressType>>&);

  /*!
   * This function gives the homogenized moduli for a dilute scheme, for
   * spheres.
   * \return an object of type YoungNuModuli<StressType> containing
   * Young modulus and Poisson ratio
   * \tparam StressType: type of the elastic constants related to the matrix
   * and the spheres
   * \param [in] young,nu: Young modulus and Poisson's ratio of the matrix
   * \param [in] f: volumic fraction of spheres
   * \param [in] young_i,nu_i: Young modulus and Poisson's ratio of the
   * inclusions
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const
      YoungNuModuli<StressType> computeSphereDiluteScheme(
          const StressType&,
          const types::real<StressType>&,
          const types::real<StressType>&,
          const StressType&,
          const types::real<StressType>&);

  /*!
   * This function is an overload of computeSphereDiluteScheme
   * with IsotropicModuli.
   * \return an object of type KGModuli<StressType> containing
   * bulk modulus and shear modulus
   * \tparam StressType: type of the elastic constants related to the matrix
   * and the spheres
   * \param [in] IM0: Isotropic moduli of the matrix
   * \param [in] f: volumic fraction of spheres
   * \param [in] IM_i: Isotropic moduli of the inclusions
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const
      KGModuli<StressType> computeSphereDiluteScheme(
          const IsotropicModuli<StressType>&,
          const types::real<StressType>&,
          const IsotropicModuli<StressType>&);

  /*!
   * This function gives the homogenized moduli for a Mori-Tanaka scheme,
   * for spheres.
   * \return an object of type YoungNuModuli<StressType> containing
   * Young modulus and Poisson ratio
   * \tparam StressType: type of the elastic constants related to the matrix and
   * the spheres
   * \param [in] young,nu: Young modulus and Poisson's ratio of the
   * matrix
   * \param [in] f: volumic fraction of spheres
   * \param [in] young_i,nu_i: Young modulus and Poisson's ratio of the
   * inclusions
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE const YoungNuModuli<StressType> computeSphereMoriTanakaScheme(
          const StressType&,
          const types::real<StressType>&,
          const types::real<StressType>&,
          const StressType&,
          const types::real<StressType>&);

  /*!
   * This function is an overload of computeSphereMoriTanakaScheme
   * with IsotropicModuli.
   * \return an object of type KGModuli<StressType> containing
   * bulk modulus and shear modulus
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants related to the matrix and
   * the spheres
   * \param [in] IM0: Isotropic moduli of the matrix
   * \param [in] f: volumic fraction of spheres
   * \param [in] IM_i: Isotropic moduli of the inclusions
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE const KGModuli<StressType> computeSphereMoriTanakaScheme(
          const IsotropicModuli<StressType>&,
          const types::real<StressType>&,
          const IsotropicModuli<StressType>&);

  /*!
   * This function gives the homogenized moduli for a dilute scheme, for an
   * isotropic distribution of ellipsoids
   * \return an object of type YoungNuModuli<StressType> containing
   * Young modulus and Poisson ratio
   * \tparam StressType: type of the elastic constants related to the matrix
   * and the ellipsoids
   * \param [in] young,nu: Young modulus and Poisson's ratio of the matrix
   * \param [in] f: volumic fraction of ellipsoids
   * \param [in] young_i,nu_i: Young modulus and Poisson's ratio of the
   * inclusions \param[in] a: length of the first semi-axis
   * \param[in] b: length of the second semi-axis
   * \param[in] c: length of the third semi-axis
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE const YoungNuModuli<StressType> computeIsotropicDiluteScheme(
          const StressType&,
          const types::real<StressType>&,
          const types::real<StressType>&,
          const StressType&,
          const types::real<StressType>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const types::length<StressType>&);

  /*!
   * This function is an overload of computeIsotropicDiluteScheme
   * with IsotropicModuli.
   * \return an object of type KGModuli<StressType> containing
   * bulk modulus and shear modulus
   * \tparam StressType: type of the elastic constants related to the matrix
   * and the ellipsoids
   * ellipsoids
   * \param [in] IM0: Isotropic moduli of the matrix
   * \param [in] f: volumic fraction of the inclusions
   * \param [in] IM_i: Isotropic moduli of the inclusions
   * \param[in] a: length of the first semi-axis
   * \param[in] b: length of the second semi-axis
   * \param[in] c: length of the third semi-axis
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE const KGModuli<StressType> computeIsotropicDiluteScheme(
          const IsotropicModuli<StressType>&,
          const types::real<StressType>&,
          const IsotropicModuli<StressType>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const types::length<StressType>&);

  /*!
   * This function gives the homogenized stiffness for a dilute scheme, for
   * a transverse isotropic distribution of ellipsoids. One principal axis
   * (the same principal axis for all ellipsoids) is oriented in a fixed
   * direction n. The distribution of the other axes are in the plane normal
   * to n, and the distribution of these axes inside this plane is
   * isotropic.
   * \return an object of type st2tost2<3u,StressType>
   * \tparam StressType: type of the elastic constants related to the matrix and
   * the ellipsoids
   * \param [in] young,nu: Young modulus and
   * Poisson's ratio of the matrix
   * \param [in] f: volumic fraction of ellipsoids
   * \param [in] young_i,nu_i: Young modulus and Poisson's ratio
   * of the inclusions
   * \param [in] n_a: direction of the principal axis
   * which has a fixed orientation and a semi-length \f$a\f$
   * \param[in] a: length of semi-axis relative to the direction \f$n_a\f$
   * \param[in] b: length of the second semi-axis
   * \param[in] c: length of the third semi-axis
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const tfel::math::
          st2tost2<3u, StressType> computeTransverseIsotropicDiluteScheme(
              const StressType&,
              const types::real<StressType>&,
              const types::real<StressType>&,
              const StressType&,
              const types::real<StressType>&,
              const tfel::math::tvector<3u, types::real<StressType>>&,
              const types::length<StressType>&,
              const types::length<StressType>&,
              const types::length<StressType>&);

  /*!
   * This function is an overload of computeTransverseIsotropicDiluteScheme
   * with IsotropicModuli.
   * \return an object of type st2tost2<3u,StressType>
   * \tparam StressType: type of the elastic constants related to the matrix and
   * the ellipsoids
   * \param [in] IM0: Isotropic moduli of the matrix
   * \param [in] f: volumic fraction of the inclusions
   * \param [in] IM_i: Isotropic moduli of the inclusions
   * \param [in] n_a: direction of the principal axis
   * which has a fixed orientation and a semi-length \f$a\f$
   * \param[in] a: length of semi-axis relative to the direction \f$n_a\f$
   * \param[in] b: length of the second semi-axis
   * \param[in] c: length of the third semi-axis
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const tfel::math::
          st2tost2<3u, StressType> computeTransverseIsotropicDiluteScheme(
              const IsotropicModuli<StressType>&,
              const types::real<StressType>&,
              const IsotropicModuli<StressType>&,
              const tfel::math::tvector<3u, types::real<StressType>>&,
              const types::length<StressType>&,
              const types::length<StressType>&,
              const types::length<StressType>&);

  /*!
   * This function gives the homogenized stiffness for a dilute scheme, for
   * a distribution of oriented ellipsoids : all principal axes have the
   * same fixed orientation.
   * \return an object of type st2tost2<3u,StressType>
   * \tparam StressType: type of the elastic constants related to the matrix and
   * the ellipsoids
   * \param [in] young,nu: Young modulus and Poisson's ratio of the matrix
   * \param [in] f: volumic fraction of ellipsoids
   * \param [in] young_i,nu_i: Young modulus and Poisson's ratio of the
   * inclusions
   * \param [in] n_a: direction of the principal axis whose semi-length is
   * \f$a\f$
   * \param[in] a: length of semi-axis relative to the direction \f$n_a\f$
   * \param [in] n_b: direction of the principal axis whose semi-length is
   * \f$b\f$
   * \param[in] b: length of the semi-axis relative to the direction \f$n_b\f$
   * \param[in] c: length of the third semi-axis
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const tfel::math::st2tost2<3u, StressType> computeOrientedDiluteScheme(
          const StressType&,
          const types::real<StressType>&,
          const types::real<StressType>&,
          const StressType&,
          const types::real<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const types::length<StressType>&);

  /*!
   * This function is an overload of computeOrientedDiluteScheme
   * with IsotropicModuli.
   * \return an object of type st2tost2<3u,StressType>
   * \tparam StressType: type of the elastic constants related to the matrix and
   * the ellipsoids
   * \param [in] IM0: Isotropic moduli of the matrix
   * \param [in] f: volumic fraction of the inclusions
   * \param [in] IM_i: Isotropic moduli of the inclusions
   * \param [in] n_a: direction of the principal axis whose semi-length is
   * \f$a\f$
   * \param[in] a: length of semi-axis relative to the direction \f$n_a\f$
   * \param [in] n_b: direction of the principal axis whose semi-length is
   * \f$b\f$
   * \param[in] b: length of the semi-axis relative to the direction \f$n_b\f$
   * \param[in] c: length of the third semi-axis
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const tfel::math::st2tost2<3u, StressType> computeOrientedDiluteScheme(
          const IsotropicModuli<StressType>&,
          const types::real<StressType>&,
          const IsotropicModuli<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const types::length<StressType>&);

  /*!
   * This function gives the homogenized moduli for a Mori-Tanaka scheme,
   * for an isotropic distribution of ellipsoids
   * \return an object of type YoungNuModuli<StressType> containing
   * Young modulus and Poisson ratio
   * \tparam StressType: type of the elastic constants related to the
   * matrix and the ellipsoids
   * \param [in] young,nu: Young modulus and Poisson's ratio of the matrix
   * \param [in] f: volumic fraction of ellipsoids
   * \param [in] young_i,nu_i: Young modulus and Poisson's ratio of the
   * inclusions
   * \param[in] a: length of the first semi-axis
   * \param[in] b: length of the second semi-axis
   * \param[in] c: length of the third semi-axis
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const YoungNuModuli<StressType> computeIsotropicMoriTanakaScheme(const StressType&,
                                                            const types::real<
                                                                StressType>&,
                                                            const types::real<
                                                                StressType>&,
                                                            const StressType&,
                                                            const types::real<
                                                                StressType>&,
                                                            const types::length<
                                                                StressType>&,
                                                            const types::length<
                                                                StressType>&,
                                                            const types::length<
                                                                StressType>&);

  /*!
   * This function is an overload of computeIsotropicMoriTanakaScheme
   * with IsotropicModuli.
   * \return an object of type KGModuli<StressType> containing
   * bulk modulus and shear modulus
   * \tparam StressType: type of the elastic constants related to the
   * matrix and the ellipsoids
   * \param [in] IM0: Isotropic moduli of the matrix
   * \param [in] f: volumic fraction of the inclusions
   * \param [in] IM_i: Isotropic moduli of the inclusions
   * \param[in] a: length of the first semi-axis
   * \param[in] b: length of the second semi-axis
   * \param[in] c: length of the third semi-axis
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE const KGModuli<StressType> computeIsotropicMoriTanakaScheme(
          const IsotropicModuli<StressType>&,
          const types::real<StressType>&,
          const IsotropicModuli<StressType>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const types::length<StressType>&);

  /*!
   * This function gives the homogenized stiffness for a Mori-Tanaka scheme,
   * for a transverse isotropic distribution of ellipsoids. One principal
   * axis (the same principal axis for all ellipsoids) is oriented in a
   * fixed direction n. The distribution of the other axes are in the plane
   * normal to n, and the distribution of these axes inside this plane is
   * isotropic.
   * \return an object of type st2tost2<3u,StressType>
   * \tparam StressType: type of the elastic constants related to the matrix
   * and the ellipsoids
   * \param [in] young,nu: Young modulus and Poisson's ratio of the matrix
   * \param [in] f: volumic fraction of ellipsoids
   * \param [in] young_i,nu_i: Young modulus and Poisson's ratio
   * of the inclusions
   * \param [in] n_a: direction of the principal axis
   * which has a fixed orientation and a semi-length \f$a\f$
   * \param[in] a: length of semi-axis relative to the direction \f$n_a\f$
   * \param[in] b: length of the second semi-axis
   * \param[in] c: length of the third semi-axis
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const tfel::math::
          st2tost2<3u, StressType> computeTransverseIsotropicMoriTanakaScheme(
              const StressType&,
              const types::real<StressType>&,
              const types::real<StressType>&,
              const StressType&,
              const types::real<StressType>&,
              const tfel::math::tvector<3u, types::real<StressType>>&,
              const types::length<StressType>&,
              const types::length<StressType>&,
              const types::length<StressType>&);

  /*!
   * This function is an overload of computeTransverseIsotropicMoriTanakaScheme
   * with IsotropicModuli.
   * \return an object of type st2tost2<3u,StressType>
   * \tparam StressType: type of the elastic constants related to the matrix
   * and the ellipsoids
   * \param [in] IM0: Isotropic moduli of the matrix
   * \param [in] f: volumic fraction of the inclusions
   * \param [in] IM_i: Isotropic moduli of the inclusions
   * \param [in] n_a: direction of the principal axis
   * which has a fixed orientation and a semi-length \f$a\f$
   * \param[in] a: length of semi-axis relative to the direction \f$n_a\f$
   * \param[in] b: length of the second semi-axis
   * \param[in] c: length of the third semi-axis
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const tfel::math::
          st2tost2<3u, StressType> computeTransverseIsotropicMoriTanakaScheme(
              const IsotropicModuli<StressType>&,
              const types::real<StressType>&,
              const IsotropicModuli<StressType>&,
              const tfel::math::tvector<3u, types::real<StressType>>&,
              const types::length<StressType>&,
              const types::length<StressType>&,
              const types::length<StressType>&);

  /*!
   * This function gives the homogenized stiffness for a Mori-Tanaka scheme,
   * for a distribution of oriented ellipsoids : all principal axes have the
   * same fixed orientation.
   * \return an object of type st2tost2<3u,StressType>
   * \tparam StressType: type of the elastic constants related to the matrix and
   * the ellipsoids
   * \param [in] young,nu: Young modulus and Poisson's ratio of the matrix
   * \param [in] f: volumic fraction of ellipsoids
   * \param [in] young_i,nu_i: Young modulus and Poisson's ratio of the
   * inclusions
   * \param [in] n_a: direction of the principal axis whose semi-length is
   * \f$a\f$
   * \param[in] a: length of semi-axis relative to the direction
   * \f$n_a\f$
   * \param [in] n_b: direction of the principal axis whose
   * semi-length is \f$b\f$
   * \param[in] b: length of the semi-axis relative to the direction \f$n_b\f$
   * \param[in] c: length of the third semi-axis
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const
      tfel::math::st2tost2<3u, StressType> computeOrientedMoriTanakaScheme(
          const StressType&,
          const types::real<StressType>&,
          const types::real<StressType>&,
          const StressType&,
          const types::real<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const types::length<StressType>&);

  /*!
   * This function is an overload of computeOrientedMoriTanakaScheme
   * with IsotropicModuli.
   * \return an object of type st2tost2<3u,StressType>
   * \tparam StressType: type of the elastic constants related to the matrix and
   * the ellipsoids
   * \param [in] IM0: Isotropic moduli of the matrix
   * \param [in] f: volumic fraction of the inclusions
   * \param [in] IM_i: Isotropic moduli of the inclusions
   * \param [in] n_a: direction of the principal axis whose semi-length is
   * \f$a\f$
   * \param[in] a: length of semi-axis relative to the direction
   * \f$n_a\f$
   * \param [in] n_b: direction of the principal axis whose
   * semi-length is \f$b\f$
   * \param[in] b: length of the semi-axis relative to the direction \f$n_b\f$
   * \param[in] c: length of the third semi-axis
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const
      tfel::math::st2tost2<3u, StressType> computeOrientedMoriTanakaScheme(
          const IsotropicModuli<StressType>&,
          const types::real<StressType>&,
          const IsotropicModuli<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const types::length<StressType>&);

  /*!
   * Here is a structure that defines a distribution of inclusions for
   * Ponte-Castaneda & Willis scheme. \ n_a: direction of the principal axis
   * whose semi-length is \f$a\f$ \ a: length of the first semi-axis \ n_b:
   * direction of the principal axis whose semi-length is \f$b\f$ \ b: length of
   * the second semi-axis \ c: length of the third semi-axis
   */
  template <tfel::math::ScalarConcept StressType>
  requires(
      tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                         StressType>()) struct Distribution {
    tfel::math::tvector<3u, types::real<StressType>> n_a;
    types::length<StressType> a;
    tfel::math::tvector<3u, types::real<StressType>> n_b;
    types::length<StressType> b;
    types::length<StressType> c;
  };

  /*!
   * This function gives the homogenized stiffness for a Ponte-Castaneda and
   * Willis scheme,
   * for a distribution of ellipsoids with the same shape
   * (formula 3.27 of Ponte Castaneda 1995).
   * The mean localisator A is given by the user and depends on the
   * distribution of orientations (the user can also uses
   * computeIsotropicPCWscheme and others). \return an object of type
   * st2tost2<3u,StressType> \tparam StressType:
   * type of the elastic constants related to the matrix and the ellipsoids
   * \param [in] young,nu: Young modulus and Poisson's ratio of the matrix
   * \param [in] f: volumic fraction of ellipsoids
   * \param [in] young_i,nu_i: Young modulus and Poisson's ratio of the
   * inclusions
   * \param [in] A: mean strain localisation tensor of inclusions
   * \param [in] D: Distribution that characterizes the distribution
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const tfel::math::st2tost2<3u, StressType> computePCWScheme(
          const StressType&,
          const types::real<StressType>&,
          const types::real<StressType>&,
          const StressType&,
          const types::real<StressType>&,
          const tfel::math::st2tost2<3u, types::real<StressType>>&,
          const Distribution<StressType>&);

  /*!
   * This function gives the homogenized stiffness for a Ponte-Castaneda and
   * Willis scheme,
   * for a distribution of ellipsoids with the same shape and a uniform
   * isotropic distribution of their orientations
   * (formula 3.27 of Ponte Castaneda 1995).
   * \return an object of type st2tost2<3u,StressType>
   * \tparam StressType: type of the elastic constants related to the matrix and
   * the ellipsoids
   * \param [in] young,nu: Young modulus and Poisson's ratio of the matrix
   * \param [in] f: volumic fraction of ellipsoids
   * \param [in] young_i,nu_i: Young modulus and Poisson's ratio of the
   * inclusions
   * \param[in] a: length of the first semi-axis (of the ellipsoids)
   * \param[in] b: length of the second semi-axis
   * \param[in] c: length of the third semi-axis
   * \param [in] D: Distribution that characterizes the distribution
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const tfel::math::st2tost2<3u, StressType> computeIsotropicPCWScheme(
          const StressType&,
          const types::real<StressType>&,
          const types::real<StressType>&,
          const StressType&,
          const types::real<StressType>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const Distribution<StressType>&);

  /*!
   * This function is an overload of computeIsotropicPCWScheme
   * with IsotropicModuli.
   * \return an object of type st2tost2<3u,StressType>
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants related to the matrix and
   * the ellipsoids
   * \tparam LengthType: type of the dimensions of the ellipsoids
   * \param [in] IM0: Isotropic moduli of the matrix
   * \param [in] f: volumic fraction of the inclusions
   * \param [in] IM_i: Isotropic moduli of the inclusions
   * \param[in] a: length of the first semi-axis (of the ellipsoids)
   * \param[in] b: length of the second semi-axis
   * \param[in] c: length of the third semi-axis
   * \param [in] D: Distribution that characterizes the distribution
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const tfel::math::st2tost2<3u, StressType> computeIsotropicPCWScheme(
          const IsotropicModuli<StressType>&,
          const types::real<StressType>&,
          const IsotropicModuli<StressType>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const Distribution<StressType>&);

  /*!
   * This function gives the homogenized stiffness for a Ponte-Castaneda and
   * Willis scheme,
   * for a distribution of ellipsoids with the same shape and a transverse
   * isotropic distribution of their orientations
   * (formula 3.27 of Ponte Castaneda 1995).
   * \return an object of type st2tost2<3u,StressType>
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants related to the matrix and
   * the ellipsoids
   * \tparam LengthType: type of the dimensions of the ellipsoids
   * \param [in] young,nu: Young modulus and Poisson's ratio of the matrix
   * \param [in] f: volumic fraction of ellipsoids
   * \param [in] young_i,nu_i: Young modulus and Poisson's ratio of the
   * inclusions
   * \param [in] n_a: direction of the principal axis whose semi-length is
   * \f$a\f$ (related to the shape of the inclusions)
   * \param[in] a: length of semi-axis relative to the direction
   * \f$n_a\f$
   * \param[in] b: length of the second semi-axis
   * \param[in] c: length of the third semi-axis
   * \param [in] D: Distribution that characterizes the distribution
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const
      tfel::math::st2tost2<3u, StressType> computeTransverseIsotropicPCWScheme(
          const StressType&,
          const types::real<StressType>&,
          const types::real<StressType>&,
          const StressType&,
          const types::real<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const Distribution<StressType>&);

  /*!
   * This function is an overload of computeTransverseIsotropicPCWScheme
   * with IsotropicModuli.
   * \return an object of type st2tost2<3u,StressType>
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants related to the matrix and
   * the ellipsoids
   * \tparam LengthType: type of the dimensions of the ellipsoids
   * \param [in] IM0: Isotropic moduli of the matrix
   * \param [in] f: volumic fraction of the inclusions
   * \param [in] IM_i: Isotropic moduli of the inclusions
   * \param [in] n_a: direction of the principal axis whose semi-length is
   * \f$a\f$ (related to the shape of the inclusions)
   * \param[in] a: length of semi-axis relative to the direction
   * \f$n_a\f$
   * \param[in] b: length of the semi-axis relative to the direction \f$n_b\f$
   * \param[in] c: length of the third semi-axis
   * \param [in] D: Distribution that characterizes the distribution
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE const
      tfel::math::st2tost2<3u, StressType> computeTransverseIsotropicPCWScheme(
          const IsotropicModuli<StressType>&,
          const types::real<StressType>&,
          const IsotropicModuli<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const Distribution<StressType>&);

  /*!
   * This function gives the homogenized stiffness for a Ponte-Castaneda and
   * Willis scheme,
   * for a distribution of ellipsoids with the same shape and a fixed
   * orientation (formula 3.25 of Ponte Castaneda 1995).
   * \return an object of type st2tost2<3u,StressType>
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants related to the matrix and
   * the ellipsoids
   * \tparam LengthType: type of the dimensions of the ellipsoids
   * \param [in] young,nu: Young modulus and Poisson's ratio of the matrix
   * \param [in] f: volumic fraction of ellipsoids
   * \param [in] young_i,nu_i: Young modulus and Poisson's ratio of the
   * inclusions
   * \param [in] n_a: direction of the principal axis whose semi-length is
   * \f$a\f$ (related to the shape of the inclusions)
   * \param[in] a: length of semi-axis relative to the direction
   * \f$n_a\f$
   * \param [in] n_b: direction of the principal axis whose
   * semi-length is \f$b\f$
   * \param[in] b: length of the semi-axis relative to the direction \f$n_b\f$
   * \param[in] c: length of the third semi-axis
   * \param [in] D: Distribution that characterizes the distribution
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const tfel::math::st2tost2<3u, StressType> computeOrientedPCWScheme(
          const StressType&,
          const types::real<StressType>&,
          const types::real<StressType>&,
          const StressType&,
          const types::real<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const Distribution<StressType>&);

  /*!
   * This function is an overload of computeTransverseIsotropicPCWScheme
   * with IsotropicModuli.
   * \return an object of type st2tost2<3u,StressType>
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants related to the matrix and
   * the ellipsoids
   * \tparam LengthType: type of the dimensions of the ellipsoids
   * \param [in] IM0: Isotropic moduli of the matrix
   * \param [in] f: volumic fraction of the inclusions
   * \param [in] IM_i: Isotropic moduli of the inclusions
   * \param [in] n_a: direction of the principal axis whose semi-length is
   * \f$a\f$ (related to the shape of the inclusions)
   * \param[in] a: length of semi-axis relative to the direction
   * \f$n_a\f$
   * \param [in] n_b: direction of the principal axis whose
   * semi-length is \f$b\f$
   * \param[in] b: length of the semi-axis relative to the direction \f$n_b\f$
   * \param[in] c: length of the third semi-axis
   * \param [in] D: Distribution that characterizes the distribution
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const tfel::math::st2tost2<3u, StressType> computeOrientedPCWScheme(
          const IsotropicModuli<StressType>&,
          const types::real<StressType>&,
          const IsotropicModuli<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const Distribution<StressType>&);

}  // end of namespace tfel::material::homogenization::elasticity

#include "TFEL/Material/LinearHomogenizationSchemes.ixx"

#endif /* LIB_TFEL_MATERIAL_LINEARHOMOGENIZATIONSCHEMES_HXX */
