/*!
 * \file   include/TFEL/Material/IsotropicEshelbyTensor.hxx
 * \author Antoine Martin
 * \date   15 October 2024
 * \brief  This file declares the Eshelby tensor for an ellipsoidal inclusion
 * embedded in an isotropic matrix. \copyright Copyright (C) 2006-2018 CEA/DEN,
 * EDF R&D. All rights reserved. This project is publicly released under either
 * the GNU GPL Licence with linking exception or the CECILL-A licence. A copy of
 * thoses licences are delivered with the sources of TFEL. CEA or EDF may also
 * distribute this project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ISOTROPICESHELBYTENSOR_HXX
#define LIB_TFEL_MATERIAL_ISOTROPICESHELBYTENSOR_HXX

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"
#include "TFEL/Material/IsotropicModuli.hxx"
#include <stdexcept>

namespace tfel::material::homogenization::elasticity {

  /*!
   * This function builds the Eshelby tensor of a circular cylinder embedded
   * in an isotropic matrix, considering a PLANE STRAIN modelling hypothesis
   * \return an object of type st2tost2<2u,real>
   * \tparam real: underlying type
   * \param[in] nu: Poisson's ratio of the matrix
   */
  template <typename real>
  TFEL_HOST_DEVICE tfel::math::st2tost2<2u, real>
  computeDiskPlaneStrainEshelbyTensor(const real&);

  /*!
   * This function builds the Hill tensor of a circular cylinder embedded
   * in an isotropic matrix, considering a PLANE STRAIN modelling hypothesis.
   * The Hill tensor gives the opposite of the uniform strain in the ellipsoid
   * as a function of the imposed polarisation.
   * \return an object of type st2tost2<2u,compliance>
   * \tparam StressType: type of the elastic constants
   * \param[in] IM_0: Isotropic Moduli of the matrix
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<2u, types::compliance<StressType>> computeDiskPlaneStrainHillTensor(
          const IsotropicModuli<StressType>&);

  /*!
   * This function builds the Eshelby tensor of an elliptic cylinder
   * embedded in an isotropic matrix, considering a PLANE STRAIN modelling
   * hypothesis The function returns the Eshelby tensor in the basis (e1,e2)
   * where e1 corresponds to the biggest axis
   * \return an object of type st2tost2<2u,real>
   * \tparam real: underlying type
   * \param[in] nu: Poisson's ratio of the matrix
   * \param[in] e: aspect ratio of the elliptic basis
   */
  template <typename real>
  TFEL_HOST_DEVICE tfel::math::st2tost2<2u, real>
  computePlaneStrainEshelbyTensor(const real&, const real&);

  /*!
   * This function builds the Hill tensor of an elliptic cylinder embedded
   * in an isotropic matrix, considering a PLANE STRAIN modelling hypothesis.
   * The Hill tensor gives the opposite of the uniform strain in the ellipsoid
   * as a function of the imposed polarisation.
   * \return an object of type st2tost2<2u,compliance>
   * \tparam StressType: type of the elastic constants
   * \param[in] IM_0: Isotropic Moduli of the matrix
   * \param [in] n_a: direction of the principal axis whose length is \f$a\f$
   * \param [in] a: length of semi-axis relative to the direction \f$n_a\f$
   * \param [in] b: length of semi-axis relative to the other direction
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<2u, types::compliance<StressType>> computePlaneStrainHillTensor(
          const IsotropicModuli<StressType>&,
          const tfel::math::tvector<2u, types::real<StressType>>&,
          const types::length<StressType>&,
          const types::length<StressType>&);

  /*!
   * This function builds the Eshelby tensor of a sphere embedded in an
   * isotropic matrix.
   * \return an object of type st2tost2<3u,real>
   * \tparam real: underlying type
   * \param[in] nu: Poisson's ratio of the matrix
   */
  template <typename real>
  TFEL_HOST_DEVICE tfel::math::st2tost2<3u, real> computeSphereEshelbyTensor(
      const real&);

  /*!
   * This function builds the Hill tensor of a sphere embedded in an
   * isotropic matrix.
   * \return an object of type st2tost2<3u,compliance<StressType>>
   * \tparam StressType: type of the elastic constants
   * \param[in] young: Young modulus of the matrix
   * \param[in] nu: Poisson's ratio of the matrix
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE tfel::math::st2tost2<
          3u,
          types::compliance<
              StressType>> computeSphereHillPolarisationTensor(const StressType&,
                                                               const types::real<
                                                                   StressType>&);

  /*!
   * This function is an overload of computeSphereHillPolarisationTensor
   * with elastic moduli of IsotropicModuli<StressType> type
   * \return an object of type st2tost2<3u,compliance<StressType>>
   * \tparam StressType: type of the elastic constants
   * \param[in] IM0: isotropic moduli of the matrix
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::compliance<StressType>> computeSphereHillPolarisationTensor(
          const IsotropicModuli<StressType>&);

  /*!
   * This function builds the Eshelby tensor of an axisymmetrical ellipsoid
   * embedded in an isotropic matrix. The function returns the Eshelby
   * tensor in the basis (e1,e2,e3) where e1 corresponds to (one of) the
   * biggest ax(es)
   * \return an object of type st2tost2<3u,real>
   * \tparam real: underlying type
   * \param[in] nu: Poisson's ratio of the matrix
   * \param[in] e: aspect ratio of the ellipsoid (e>1 : prolate, e<1 :
   * oblate)
   * \param[in] precf, precd, precld: default arguments which aim at
   * preventing the numerical instability of the formula when the ellipsoid
   * is almost a sphere. When the absolute value of (e-1) is below precf
   * (resp. precd, precld) for real=float (resp. real= double, long double),
   * the returned tensor is computeSphereEshelbyTensor(nu).
   *
   * The expressions can be found in Torquato, Random Heterogeneous
   * Materials (2002).
   */
  template <typename real>
  TFEL_HOST_DEVICE tfel::math::st2tost2<3u, real>
  computeAxisymmetricalEshelbyTensor(const real&,
                                     const real&,
                                     const real = real{8e-3},
                                     const real = real{1.5e-4},
                                     const real = real{1e-5});

  /*!
   * This function builds the Hill tensor of an axisymmetrical ellipsoid
   * embedded in an isotropic matrix. The function takes into account the
   * orientation of the ellipsoid.
   * \return an object of type st2tost2<3u,compliance<StressType>>
   * \tparam StressType: type of the elastic constants
   * \param[in] young: Young modulus of the matrix
   * \param[in] nu: Poisson's ratio of the matrix
   * \param [in] n_a: direction of the principal axis
   * \param[in] e: aspect ratio of the ellipsoid (e>1 : prolate, e<1 :
   * oblate)
   * \param[in] precf, precd, precld: default arguments which aim at
   * preventing the numerical instability of the formula when the ellipsoid
   * is almost a sphere. When the absolute value of (e-1) is below precf
   * (resp. precd, precld) for real=float (resp. real= double, long double),
   * the returned tensor is computeSphereHillTensor(young,nu).
   *
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::compliance<StressType>> computeAxisymmetricalHillPolarisationTensor(
          const StressType&,
          const types::real<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::real<StressType>&,
          const types::real<StressType> = types::real<StressType>{8e-3},
          const types::real<StressType> = types::real<StressType>{1.5e-4},
          const types::real<StressType> = types::real<StressType>{1e-5});

  /*!
   * This function is an overload of computeAxisymmetricalHillPolarisationTensor
   *  with elastic moduli of IsotropicModuli<StressType> type
   * \return an object of type st2tost2<3u,compliance<StressType>>
   * \tparam StressType: type of the elastic constants
   * \param[in] IM0: isotropic moduli of the matrix
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::compliance<StressType>> computeAxisymmetricalHillPolarisationTensor(
          const IsotropicModuli<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::real<StressType>&,
          const types::real<StressType> = types::real<StressType>{8e-3},
          const types::real<StressType> = types::real<StressType>{1.5e-4},
          const types::real<StressType> = types::real<StressType>{1e-5});

  /*!
   * This function builds the Eshelby tensor of a general ellipsoid embedded
   * in an isotropic matrix. The function returns the Eshelby tensor in the
   * basis (e1,e2,e3) where e1 (resp. e2, e3) is aligned with the axis with
   * the first (resp. the second and third) biggest length
   * \return an object of type st2tost2<3u,real>
   * \param[in] nu: Poisson's ratio of the matrix
   * \param[in] a: length of the first semi-axis
   * \param[in] b: length of the second semi-axis
   * \param[in] c: length of the third semi-axis
   * \param[in] precf, precd, precld: default
   * arguments which aim at preventing the numerical instability of the
   * formula when the ellipsoid is almost axisymmetrical. When the absolute
   * value of (a-b)/c (or (a-c)/b or (b-c)/a) is below precf (resp. precd,
   * precld) for real=float (resp. real= double, long double), the returned
   * tensor is computeAxisymmetricalEshelbyTensor.
   *
   * The expressions for the case of three distinct semi-axes can be found
   * in Eshelby (1957).
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      tfel::math::st2tost2<3u, types::real<StressType>> computeEshelbyTensor(
          const types::real<StressType>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const types::real<StressType> = types::real<StressType>{8e-3},
          const types::real<StressType> = types::real<StressType>{1.5e-4},
          const types::real<StressType> = types::real<StressType>{1e-5});

  /*!
   * This function builds the Hill tensor of a general ellipsoid embedded
   * in an isotropic matrix. The function takes into account the orientation
   * of the ellipsoid.
   * \return an object of type st2tost2<3u,compliance<StressType>>
   * \tparam StressType: type of the elastic constants
   * \param[in] young: Young modulus of the matrix
   * \param[in] nu: Poisson's ratio of the matrix
   * \param [in] n_a: direction of the principal axis whose length is \f$a\f$
   * \param [in] a: length of semi-axis relative to the direction \f$n_a\f$
   * \param [in] n_b: direction of the principal axis whose length is \f$b\f$
   * \param [in] b: length of semi-axis relative to the direction \f$n_b\f$
   * \param [in] c: length of the remaining semi-axis
   * \param[in] precf, precd, precld: default
   * arguments which aim at preventing the numerical instability of the
   * formula when the ellipsoid is almost axisymmetrical. When the absolute
   * value of (a-b)/c (or (a-c)/b or (b-c)/a) is below precf (resp. precd,
   * precld) for real=float (resp. real= double, long double), the returned
   * tensor is computeAxisymmetricalHillTensor.
   *
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::compliance<StressType>> computeHillPolarisationTensor(
          const StressType&,
          const types::real<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const types::real<StressType> = types::real<StressType>{8e-3},
          const types::real<StressType> = types::real<StressType>{1.5e-4},
          const types::real<StressType> = types::real<StressType>{1e-5});

  /*!
   * This function is an overload of computeHillPolarisationTensor
   *  with elastic moduli of IsotropicModuli<StressType> type
   * \return an object of type st2tost2<3u,compliance<StressType>>
   * \tparam StressType: type of the elastic constants
   * \param[in] IM0: isotropic moduli of the matrix
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::compliance<StressType>> computeHillPolarisationTensor(
          const IsotropicModuli<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const types::real<StressType> = types::real<StressType>{8e-3},
          const types::real<StressType> = types::real<StressType>{1.5e-4},
          const types::real<StressType> = types::real<StressType>{1e-5});

}  // end of namespace tfel::material::homogenization::elasticity

#include "TFEL/Material/IsotropicEshelbyTensor.ixx"

#endif /* LIB_TFEL_MATERIAL_ISOTROPICESHELBYTENSOR_HXX */
