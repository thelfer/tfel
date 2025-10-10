/*!
 * \file   include/TFEL/Material/LocalisationTensor.hxx
 * \author Antoine Martin
 * \date   18 June 2025
 * \brief  This file declares the localisation tensors for an ellipsoidal
 * inclusion embedded in an isotropic matrix. \copyright Copyright (C) 2006-2018
 * CEA/DEN, EDF R&D. All rights reserved. This project is publicly released
 * under either the GNU GPL Licence or the CECILL-A licence. A copy of thoses
 * licences are delivered with the sources of TFEL. CEA or EDF may also
 * distribute this project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_LOCALISATIONTENSOR_HXX
#define LIB_TFEL_MATERIAL_LOCALISATIONTENSOR_HXX

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"
#include "TFEL/Material/IsotropicModuli.hxx"
#include "TFEL/Material/IsotropicEshelbyTensor.hxx"
#include <stdexcept>

namespace tfel::material::homogenization::elasticity {

  /*!
   * This function builds the strain localisation tensor of a circular
   * cylinder embedded in an isotropic matrix,
   * with a general elasticity, considering a PLANE STRAIN modelling hypothesis.
   * \return an object of type st2tost2
   * \tparam StressType: type of the elastic constants related to the matrix
   * and the ellipsoid
   * \param [in] IM_0: Isotropic Moduli of the matrix
   * \param [in] C_i: elasticity of the inclusion
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<2u, types::real<StressType>> computeDiskPlaneStrainLocalisationTensor(
          const IsotropicModuli<StressType>&,
          const tfel::math::st2tost2<2u, StressType>&);

  /*!
   * This function builds the strain localisation tensor of an elliptic
   * cylinder embedded in an isotropic matrix,
   * with a general elasticity, considering a PLANE STRAIN modelling hypothesis.
   * \return an object of type st2tost2
   * \tparam StressType: type of the elastic constants related to the matrix
   * and the ellipsoid
   * \param [in] IM_0: Isotropic Moduli of the matrix
   * \param [in] C_i: elasticity of the inclusion
   * \param [in] n_a: direction of the principal axis whose length is \f$a\f$
   * \param [in] a: length of semi-axis relative to the direction \f$n_a\f$
   * \param [in] b: length of semi-axis relative to the other direction
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<2u, types::real<StressType>> computePlaneStrainLocalisationTensor(
          const IsotropicModuli<StressType>&,
          const tfel::math::st2tost2<2u, StressType>&,
          const tfel::math::tvector<2u, types::real<StressType>>&,
          const types::length<StressType>&,
          const types::length<StressType>&);

  /*!
   * This function builds the strain localisation tensor of a sphere
   * with a general elasticity, embedded in an isotropic matrix.
   * \return an object of type st2tost2
   * \tparam StressType: type of the elastic constants related to the matrix
   * and the ellipsoid
   * \param [in] young,nu: Young modulus and Poisson's ratio of the matrix
   * \param [in] young_i,nu_i: Young modulus and Poisson's ratio of the
   * inclusions
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE tfel::math::
          st2tost2<3u, types::real<StressType>> computeSphereLocalisationTensor(
              const StressType&,
              const types::real<StressType>&,
              const StressType&,
              const types::real<StressType>&);

  /*!
   * This function is an overload of computeSphereLocalisationTensor
   *  with elastic moduli of IsotropicModuli<StressType> type
   * \return an object of type st2tost2
   * \tparam StressType: type of the elastic constants related to the matrix
   * and the ellipsoid
   * \param[in] IM0: isotropic moduli of the matrix
   * \param[in] IM_i: isotropic moduli of the inclusions
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE tfel::math::
          st2tost2<3u, types::real<StressType>> computeSphereLocalisationTensor(
              const IsotropicModuli<StressType>&,
              const IsotropicModuli<StressType>&);

  /*!
   * This function builds the strain localisation tensor of an
   * axisymmetrical ellipsoid with a general elasticity, embedded in an
   * isotropic matrix. The ellipsoid also has a specific orientation given
   * by the vector \f$n_a\f$, axis of the ellipsoid, whose semi-length is
   * \f$a\f$.
   * \return an object of type st2tost2
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
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::real<StressType>> computeAxisymmetricalEllipsoidLocalisationTensor(
          const StressType&,
          const types::real<StressType>&,
          const StressType&,
          const types::real<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::real<StressType>&);

  /*!
   * This function is an overload of
   * computeAxisymmetricalEllipsoidLocalisationTensor with elastic moduli of
   * IsotropicModuli<StressType> type \return an object of type st2tost2
   * \tparam StressType: type of the elastic constants
   * related to the matrix and the ellipsoid
   * \param[in] IM0: isotropic moduli of
   * the matrix
   * \param[in] IM_i: isotropic moduli of the inclusions \param [in]
   * n_a: direction of the axis of the ellipsoid (whose semi-length is \f$a\f$)
   * \param [in] a: length of semi-axis relative to the
   * direction \f$n_a\f$
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::real<StressType>> computeAxisymmetricalEllipsoidLocalisationTensor(
          const IsotropicModuli<StressType>&,
          const IsotropicModuli<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::real<StressType>&);

  /*!
   * This function builds the strain localisation tensor of a general
   * ellipsoid with a general elasticity, embedded in an isotropic matrix.
   * The localisation tensor \f$A\f$ is defined as follows : \f[\epsilon =
   * A:E_0\f] where \f$E_0\f$ is the uniform strain tensor imposed at
   * infinity, and \f$\epsilon\f$ is the strain tensor solution of Eshelby
   * problem for the ellipsoid. The ellipsoid also has a specific
   * orientation given by the vectors \f$n_a\f$, \f$n_b\f$, which provides a
   * direct local basis \f$(n_a,n_b,n_c)\f$. The elasticity \f$C_i\f$ is
   * specified in this local basis. \return an object of type st2tost2, which is
   * the fourth-order localisation tensor \f$A\f$ (in the global basis). \return
   * an object of type st2tost2 \tparam StressType: type of the elastic
   * constants related to the matrix and the ellipsoid \param[in] IM0: isotropic
   * moduli of the matrix \param[in] C_i: stiffness of the inclusions \param
   * [in] n_a: direction of the principal axis whose length is \f$a\f$ \param
   * [in] a: length of semi-axis relative to the direction \f$n_a\f$ \param [in]
   * n_b: direction of the principal axis whose length is \f$b\f$ \param [in] b:
   * length of semi-axis relative to the direction \f$n_b\f$ \param [in] c:
   * length of the remaining semi-axis
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::real<StressType>> computeEllipsoidLocalisationTensor(
          const IsotropicModuli<StressType>&,
          const tfel::math::st2tost2<3u, StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const types::length<StressType>&);

  /*!
   * This function is an overload of computeEllipsoidLocalisationTensor
   * with elastic moduli in the form (young,nu)
   * \return an object of type st2tost2, which is the fourth-order localisation
   * tensor \f$A\f$
   * \tparam StressType: type of
   * the elastic constants related to the matrix and the ellipsoid
   * \param [in] young,nu: Young modulus and Poisson's ratio of the matrix
   * \param [in] young_i,nu_i: Young modulus and Poisson's ratio of the
   * inclusions \param [in] n_a: direction of the principal axis whose length is
   * \f$a\f$ \param [in] a: length of semi-axis relative to the direction
   * \f$n_a\f$ \param [in] n_b: direction of the principal axis whose length is
   * \f$b\f$ \param [in] b: length of semi-axis relative to the direction
   * \f$n_b\f$ \param [in] c: length of the remaining semi-axis
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::real<StressType>> computeEllipsoidLocalisationTensor(
          const StressType&,
          const types::real<StressType>&,
          const StressType&,
          const types::real<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const types::length<StressType>&);

  /*!
   * This function is another overload of computeEllipsoidLocalisationTensor
   * with elastic moduli of IsotropicModuli<StressType> type
   * \return an object of type st2tost2, which is the fourth-order localisation
   * tensor \f$A\f$
   * \tparam StressType: type of
   * the elastic constants related to the matrix and the ellipsoid
   * \param[in] IM0: isotropic moduli of the matrix
   * \param[in] IM_i: isotropic moduli of the inclusions
   * \param [in] n_a: direction of the principal axis whose length is \f$a\f$
   * \param [in] a: length of semi-axis relative to the direction \f$n_a\f$
   * \param [in] n_b: direction of the principal axis whose length is \f$b\f$
   * \param [in] b: length of semi-axis relative to the direction \f$n_b\f$
   * \param [in] c: length of the remaining semi-axis
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::real<StressType>> computeEllipsoidLocalisationTensor(
          const IsotropicModuli<StressType>&,
          const IsotropicModuli<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const types::length<StressType>&);

  /*!
   * struct for dispatching computeIsotropicLocalisationTensor over the
   * dimension \tparam N: dimension \tparam StressType: type of the elastic
   * constants related to the matrix and the ellipsoid
   */
  template <unsigned short int N, tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) struct IsotropicLocalisationTensor;

  /*!
   * This function just groups implementations of localisation
   * tensors for dimension 2 and 3
   * \tparam N: dimension
   * \tparam StressType: type of
   * the elastic constants related to the matrix and the ellipsoid
   * \param[in] IM0: isotropic moduli of the matrix
   * \param[in] C_i: stiffness of the inclusions
   * \param [in] n_a: direction of the principal axis relative to first
   * semi-length \param [in] n_b: direction of the principal axis relative to
   * second semi-length \param [in] semiLengths: array of lengths of the N
   * semi-axes. The first one for direction \f$n_a\f$, the second for \f$n_b\f$.
   */
  template <unsigned short int N, tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<N, types::real<StressType>> computeIsotropicLocalisationTensor(
          const IsotropicModuli<StressType>&,
          const tfel::math::st2tost2<N, StressType>&,
          const tfel::math::tvector<N, types::real<StressType>>&,
          const tfel::math::tvector<N, types::real<StressType>>&,
          const std::array<types::length<StressType>, N>&);

}  // end of namespace tfel::material::homogenization::elasticity

#include "TFEL/Material/LocalisationTensor.ixx"

#endif /* LIB_TFEL_MATERIAL_LOCALISATIONTENSOR_HXX */
