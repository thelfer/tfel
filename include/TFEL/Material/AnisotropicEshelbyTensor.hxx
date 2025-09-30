/*!
 * \file   include/TFEL/Material/AnisotropicEshelbyTensor.hxx
 * \author Antoine Martin
 * \date   18 November 2024
 * \brief  This file declares the Eshelby tensor for an ellipsoidal inclusion
 * embedded in an anisotropic matrix.
 * \copyright Copyright (C) 2006-2018
 * CEA/DEN, EDF R&D. All rights reserved. This project is publicly released
 * under either the GNU GPL Licence with linking exception or the CECILL-A
 * licence. A copy of thoses licences are delivered with the sources of TFEL.
 * CEA or EDF may also distribute this project under specific licensing
 * conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ANISOTROPICESHELBYTENSOR_HXX
#define LIB_TFEL_MATERIAL_ANISOTROPICESHELBYTENSOR_HXX

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"
#include <stdexcept>

namespace tfel::material::homogenization::elasticity {

  /*!
   * This function builds the Hill tensor of a general 2d ellipse embedded
   * in an ANISOTROPIC matrix.  The
   * ellipse also has a specific orientation given by the vector
   * \f$n_a\f$. A LOCAL basis can then be defined.
   * The function returns the Hill tensor in th GLOBAL basis.
   * \return an object of type st2tost2<2u,types::compliance<StressType>>
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants
   * \param[in] C: stiffness tensor of the matrix
   * \param [in] n_a: direction of the principal axis whose length is \f$a\f$
   * \param [in] a: length of semi-axis relative to the direction \f$n_a\f$
   * \param [in] b: length of semi-axis relative to the other direction
   * \param[in] max_it: maximal number of iterations for integration
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<2u, types::compliance<StressType>> computePlaneStrainAnisotropicHillTensor(
          const tfel::math::st2tost2<2u, StressType>&,
          const types::real<StressType>&,
          const std::size_t max_it = 12);

  /*!
   * This function builds the Eshelby tensor of a general 2d ellipse
   * embedded in an ANISOTROPIC matrix. The
   * ellipse also has a specific orientation given by the vector
   * \f$n_a\f$. A LOCAL basis can then be defined.
   * The function returns the Hill tensor in th GLOBAL basis.
   * The function returns the Eshelby
   * tensor in the global basis
   * \return an object of type st2tost2<2u,real>
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants
   * \param[in] C: stiffness tensor of the matrix
   * \param [in] n_a: direction of the principal axis whose length is \f$a\f$
   * \param [in] a: length of semi-axis relative to the direction \f$n_a\f$
   * \param [in] b: length of semi-axis relative to the other direction
   * \param[in] max_it: maximal number of iterations for integration
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<2u, types::real<StressType>> computePlaneStrainAnisotropicEshelbyTensor(
          const tfel::math::st2tost2<2u, StressType>&,
          const types::real<StressType>&,
          const std::size_t max_it = 12);

  /*!
   * This function builds the Hill tensor of a general ellipsoid embedded in
   * an ANISOTROPIC matrix. The function returns the Hill tensor in the
   * global basis
   * \return an object of type st2tost2<3u,types::compliance<StressType>>
   * \tparam real: underlying type
   * \tparam LengthType: type of the dimensions of the ellipsoid
   * \tparam StressType: type of the elastic constants
   * \param[in] C: stiffness tensor of the matrix in the global basis
   * \param [in] n_a: direction of the principal axis whose length is \f$a\f$
   * \param [in] a: length of semi-axis relative to the direction \f$n_a\f$
   * \param [in] n_b: direction of the principal axis whose length is \f$b\f$
   * \param [in] b: length of semi-axis relative to the direction \f$n_b\f$
   * \param [in] c: length of the remaining semi-axis
   * \param[in] max_it: maximal number of iterations for integration
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::compliance<StressType>> computeAnisotropicHillTensor(
          const tfel::math::st2tost2<3u, StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const std::size_t max_it = 12);

  /*!
   * This function builds the Eshelby tensor of a general ellipsoid embedded
   * in an ANISOTROPIC matrix. The function returns the Eshelby tensor in
   * the global basi
   * \return an object of type st2tost2<3u,real>
   * \tparam real: underlying type
   * \tparam LengthType: type of the dimensions of the ellipsoid
   * \tparam StressType: type of the elastic constants
   * \param[in] C: stiffness tensor of the matrix in the global basis
   * \param [in] n_a: direction of the principal axis whose length is \f$a\f$
   * \param [in] a: length of semi-axis relative to the direction \f$n_a\f$
   * \param [in] n_b: direction of the principal axis whose length is \f$b\f$
   * \param [in] b: length of semi-axis relative to the direction \f$n_b\f$
   * \param [in] c: length of the remaining semi-axis
   * \param[in] max_it: maximal number of iterations for integration
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE tfel::math::
          st2tost2<3u, types::real<StressType>> computeAnisotropicEshelbyTensor(
              const tfel::math::st2tost2<3u, StressType>&,
              const tfel::math::tvector<3u, types::real<StressType>>&,
              const types::length<StressType>&,
              const tfel::math::tvector<3u, types::real<StressType>>&,
              const types::length<StressType>&,
              const types::length<StressType>&,
              const std::size_t max_it = 12);

  /*!
   * This function builds the strain localisation tensor of a general
   * ellipsoid embedded in an ANISOTROPIC matrix. The localisation tensor
   * \f$A\f$ is defined as follows : \f[\epsilon = A:E_0\f] where \f$E_0\f$
   * is the uniform strain tensor imposed at infinity, and \f$\epsilon\f$ is
   * the strain tensor solution of Eshelby problem for the ellipsoid. The
   * ellipsoid also has a specific orientation given by the vectors
   * \f$n_a\f$, \f$n_b\f$. A LOCAL basis can then be defined as n_a, n_b,
   * n_c where n_c=cross_product(n_a,n_b)
   * \return an object of type st2tost2<3u,real>, which is the fourth-order
   * localisation tensor \f$A\f$ in the GLOBAL BASIS
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants related to the matrix
   * and the ellipsoid
   * \tparam LengthType: type of the dimensions of the ellipsoid
   * \param[in] C_0_glob: stiffness tensor of the matrix in the GLOBAL basis
   * \param[in] C_i_loc: stiffness tensor of the inclusion in the LOCAL
   * basis, which is the basis (n_a,n_b,n_c)
   * \param [in] n_a: direction of the principal axis whose length is \f$a\f$
   * \param [in] a: length of semi-axis relative to the direction \f$n_a\f$
   * \param [in] n_b: direction of the principal axis whose length is \f$b\f$
   * \param [in] b: length of semi-axis relative to the direction \f$n_b\f$
   * \param [in] c: length of the remaining semi-axis
   * \param[in] max_it: maximal number of iterations for integration
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::real<StressType>> computeAnisotropicLocalisationTensor(
          const tfel::math::st2tost2<3u, StressType>&,
          const tfel::math::st2tost2<3u, StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const tfel::math::tvector<3u, types::real<StressType>>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const std::size_t max_it = 12);

  /*!
   * This function builds the strain localisation tensor of a general 2d ellipse
   * embedded in an ANISOTROPIC matrix. The localisation tensor
   * \f$A\f$ is defined as follows : \f[\epsilon = A:E_0\f] where \f$E_0\f$
   * is the uniform strain tensor imposed at infinity, and \f$\epsilon\f$ is
   * the strain tensor solution of Eshelby problem for the ellipse. The
   * ellipse also has a specific orientation given by the vector
   * \f$n_a\f$. A LOCAL basis can then be defined.
   * \return an object of type st2tost2<2u,real>, which is the fourth-order
   * localisation tensor \f$A\f$ in the GLOBAL BASIS
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants
   * \param[in] C_0_glob: stiffness tensor of the matrix in the GLOBAL basis
   * \param[in] C_i_loc: stiffness tensor of the inclusion in the LOCAL basis,
   * which is the basis related to \f$n_a\f$
   * \param [in] n_a: direction of the principal axis whose length is \f$a\f$
   * \param [in] a: length of semi-axis relative to the direction \f$n_a\f$
   * \param [in] b: length of semi-axis relative to the other direction
   * \param[in] max_it: maximal number of iterations for integration
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<2u, types::real<StressType>> computePlaneStrainAnisotropicLocalisationTensor(
          const tfel::math::st2tost2<2u, StressType>&,
          const tfel::math::st2tost2<2u, StressType>&,
          const tfel::math::tvector<2u, types::real<StressType>>&,
          const types::length<StressType>&,
          const types::length<StressType>&,
          const std::size_t max_it = 12);
          
  /*!
   * struct for dispatching computeAnisotropicLocalisationTensor over the dimension
   * \tparam N: dimension
   * \tparam StressType: type of the elastic constants related to the matrix
   * and the ellipsoid
   */
   template <unsigned short int N, tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>())
  struct AnisotropicLocalisationTensor;
  
   /*!
   *  A function computeGeneralAnisotropicLocalisationTensor for dimensions 2 and 3.
   * \tparam N: dimension
   * \tparam StressType: type of the elastic constants related to the matrix
   * and the ellipsoid
   * \param[in] C_0_glob: stiffness tensor of the matrix in the GLOBAL basis
   * \param[in] C_i_loc: stiffness tensor of the inclusion in the LOCAL
   * basis, which is the basis given by (\f$n_a\f$,\f$n_b\f$) (\f$n_b\f$ is useless in 2d).
   * \param [in] n_a: direction of the principal axis relative to first semi-length
   * \param [in] n_b: direction of the principal axis relative to second semi-length
   * \param [in] semiLengths: array of lengths of the N semi-axes. The first one for direction
   * \f$n_a\f$, the second for \f$n_b\f$.
   * \param[in] max_it: maximal number of iterations for integration
   */
  template <unsigned short int N,tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<N, types::real<StressType>> computeGeneralAnisotropicLocalisationTensor(
          const tfel::math::st2tost2<N, StressType>&,
          const tfel::math::st2tost2<N, StressType>&,
          const tfel::math::tvector<N, types::real<StressType>>&,
          const tfel::math::tvector<N, types::real<StressType>>&,
          const std::array<types::length<StressType>,N>&,
          const std::size_t max_it = 12);

}  // namespace tfel::material::homogenization::elasticity

#include "TFEL/Material/AnisotropicEshelbyTensor.ixx"

#endif /* LIB_TFEL_MATERIAL_ANISOTROPICESHELBYTENSOR_HXX */
