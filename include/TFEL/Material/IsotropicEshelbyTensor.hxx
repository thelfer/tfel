/*!
 * \file   include/TFEL/Material/IsotropicEshelbyTensor.hxx
 * \author Antoine Martin
 * \date   15 October 2024
 * \brief  This file declares the Eshelby tensor for an ellipsoidal inclusion
 * embedded in an isotropic matrix. \copyright Copyright (C) 2006-2018 CEA/DEN,
 * EDF R&D. All rights reserved. This project is publicly released under either
 * the GNU GPL Licence or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ISOTROPICESHELBYTENSOR_HXX
#define LIB_TFEL_MATERIAL_ISOTROPICESHELBYTENSOR_HXX

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"
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
  computeCircularCylinderEshelbyTensor(const real&);

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
  computeEllipticCylinderEshelbyTensor(const real&, const real&);

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
   * This function builds the Eshelby tensor of a general ellipsoid embedded
   * in an isotropic matrix. The function returns the Eshelby tensor in the
   * basis (e1,e2,e3) where e1 (resp. e2, e3) is aligned with the axis with
   * the first (resp. the second and third) biggest length
   * \return an object of type st2tost2<3u,real>
   * \tparam real: underlying type
   * \tparam LengthType: type of the dimensions of the ellipsoid
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
  template <typename real, typename LengthType>
  TFEL_HOST_DEVICE tfel::math::st2tost2<3u, real> computeEshelbyTensor(
      const real&,
      const LengthType&,
      const LengthType&,
      const LengthType&,
      const real = real{8e-3},
      const real = real{1.5e-4},
      const real = real{1e-5});

  /*!
   * This function builds the strain localisation tensor of a general
   * ellipsoid with a general elasticity, embedded in an isotropic matrix.
   * The localisation tensor \f$A\f$ is defined as follows : \f[\epsilon =
   * A:E_0\f] where \f$E_0\f$ is the uniform strain tensor imposed at
   * infinity, and \f$\epsilon\f$ is the strain tensor solution of Eshelby
   * problem for the ellipsoid. The ellipsoid also has a specific
   * orientation given by the vectors \f$n_a\f$, \f$n_b\f$.
   * \return an object of type st2tost2, which is the fourth-order localisation
   * tensor \f$A\f$
   * \tparam real: underlying type
   * \tparam StressType: type of
   * the elastic constants related to the matrix and the ellipsoid
   * \tparam LengthType: type of the dimensions of the ellipsoid
   * \param [in] young,nu: Young modulus and Poisson's ratio of the matrix
   * \param [in] young_i,nu_i: Young modulus and Poisson's ratio of the
   * inclusions
   * \param [in] n_a: direction of the principal axis whose length is \f$a\f$
   * \param [in] a: length of semi-axis relative to the direction \f$n_a\f$
   * \param [in] n_b: direction of the principal axis whose length is \f$b\f$
   * \param [in] b: length of semi-axis relative to the direction \f$n_b\f$
   * \param [in] c: length of the remaining semi-axis
   */
  template <typename real, typename StressType, typename LengthType>
  TFEL_HOST_DEVICE tfel::math::st2tost2<3u, real>
  computeEllipsoidLocalisationTensor(const StressType&,
                                     const real&,
                                     const StressType&,
                                     const real&,
                                     const tfel::math::tvector<3u, real>&,
                                     const LengthType&,
                                     const tfel::math::tvector<3u, real>&,
                                     const LengthType&,
                                     const LengthType&);

  /*!
   * This function builds the strain localisation tensor of an
   * axisymmetrical ellipsoid with a general elasticity, embedded in an
   * isotropic matrix. The ellipsoid also has a specific orientation given
   * by the vector \f$n_a\f$, axis of the ellipsoid, whose semi-length is
   * \f$a\f$.
   * \return an object of type st2tost2
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants related to the
   * matrix and the ellipsoid
   * \param [in] young,nu: Young modulus and Poisson's ratio of the matrix
   * \param [in] young_i,nu_i: Young modulus and Poisson's ratio of the
   * inclusions
   * \param [in] n_a: direction of the axis of the ellipsoid (whose
   * semi-length is \f$a\f$)
   * \param [in] a: length of semi-axis relative to the
   * direction \f$n_a\f$
   */
  template <typename real, typename StressType>
  TFEL_HOST_DEVICE tfel::math::st2tost2<3u, real>
  computeAxisymmetricalEllipsoidLocalisationTensor(
      const StressType&,
      const real&,
      const StressType&,
      const real&,
      const tfel::math::tvector<3u, real>&,
      const real&);

  /*!
   * This function builds the strain localisation tensor of a sphere
   * with a general elasticity, embedded in an isotropic matrix.
   * \return an object of type st2tost2
   * \tparam    real: underlying type
   * \tparam StressType: type of the elastic constants related to the matrix
   * and the ellipsoid
   * \param [in] young,nu: Young modulus and Poisson's ratio of the matrix
   * \param [in] young_i,nu_i: Young modulus and Poisson's ratio of the
   * inclusions
   */
  template <typename real, typename StressType>
  TFEL_HOST_DEVICE tfel::math::st2tost2<3u, real>
  computeSphereLocalisationTensor(const StressType&,
                                  const real&,
                                  const StressType&,
                                  const real&);

}  // end of namespace tfel::material::homogenization::elasticity

#include "TFEL/Material/IsotropicEshelbyTensor.ixx"

#endif /* LIB_TFEL_MATERIAL_ISOTROPICESHELBYTENSOR_HXX */
