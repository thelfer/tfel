/*!
 * \file   include/TFEL/Material/MicrostructureLinearHomogenization.ixx
 * \author Antoine Martin
 * \date   25 January 2025
 * \brief  This file implements some homogenization schemes for generic
 * microstructures. \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All
 * rights reserved. This project is publicly released under either the GNU GPL
 * Licence or the CECILL-A licence. A copy of thoses licences are delivered with
 * the sources of TFEL. CEA or EDF may also distribute this project under
 * specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_MICROSTRUCTURELINEARHOMOGENIZATION_IXX
#define LIB_TFEL_MATERIAL_MICROSTRUCTURELINEARHOMOGENIZATION_IXX

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/MicrostructureDescription.hxx"
#include "TFEL/Material/IsotropicEshelbyTensor.hxx"
#include "TFEL/Material/AnisotropicEshelbyTensor.ixx"

namespace tfel::math {
  template <unsigned short int N, typename real>
  real quad_product(const st2tost2<N, real> &A, const st2tost2<N, real> &B) {
    real s = real(0);
    const auto size = StensorDimeToSize<N>::value;
    for (int i = 0; i < size; i++)
      for (int j = 0; j < size; j++) {
        s += A(i, j) * B(i, j);
      }
    return s;
    std::cout << "dim " << N << "size " << size << std::endl;
  }
}  // namespace tfel::math

namespace tfel::material::homogenization::elasticity {

  /*!
   * The struct HomogenizationScheme is a struct whose attributes are
   * various objects that are useful in homogenization
   * theory, as homogenized stiffness, mean strains or stresses,
   * second-order moments...
   * \tparam unsigned short int: dimension
   * \tparam real: underlying type
   * \tparam LengthType: type of the lengths related to the inclusion
   * \tparam StressType: type of the elastic constants related to the inclusion
   */
  template <unsigned short int N,
            typename real,
            typename LengthType,
            typename StressType>
  struct HomogenizationScheme {
    tfel::math::st2tost2<N, StressType> homogenized_stiffness;
    std::vector<tfel::math::stensor<N, real>> mean_strains;
    std::vector<tfel::math::stensor<N, StressType>> mean_stresses;
    std::vector<tfel::math::st2tost2<N, real>> strain_second_moments;
    std::vector<tfel::math::st2tost2<N, StressType>>
        stress_second_moments;  // PROBLEMMEE DEE TYPPEEEE
  };

  /*!
   * Here is the MoriTanaka scheme which returns an object of
   * type HomogenizationScheme from a MatrixInclusionMicrostructure.
   * \tparam unsigned short int: dimension \tparam
   * real: underlying type \tparam LengthType: type of the lengths related to
   * the inclusion \tparam StressType: type of the elastic constants related to
   * the inclusion \return an object of type HomogenizationScheme
   */
  template <unsigned short int N,
            typename real,
            typename LengthType,
            typename StressType>
  HomogenizationScheme<N, real, LengthType, StressType> computeMoriTanaka(
      MatrixInclusionMicrostructure<N, real, LengthType, StressType> micro,
      tfel::math::stensor<N, real> E) {
    const auto np = micro.get_number_of_phases();
    const auto f0 = micro.get_matrix_fraction();
    const auto phases = micro.get_inclusionPhases();
    const auto phase0 = micro.get_matrixPhase();

    HomogenizationScheme<N, real, LengthType, StressType> h_s;
    h_s.homogenized_stiffness = phase0.stiffness;
    // h_s.mean_strains=;
    // h_s.mean_stresses=;
    // h_s.strain_second_moments=;
    // h_s.stress_second_moments=;
    return h_s;
  };

  /*!
   * Here is the Dilute scheme which returns an object of
   * type HomogenizationScheme from a MatrixInclusionMicrostructure.
   * \tparam unsigned short int: dimension \tparam
   * real: underlying type \tparam LengthType: type of the lengths related to
   * the inclusion \tparam StressType: type of the elastic constants related to
   * the inclusion \return an object of type HomogenizationScheme 
   * \param[in] micro: MatrixInclusionMicrostructure
   * \param[in] E: uniform strain loading
   * \param[in] mat_iso: boolean which precises if the matrix is considered isotropic
   */
  template <unsigned short int N,
            typename real,
            typename LengthType,
            typename StressType>
  HomogenizationScheme<N, real, LengthType, StressType> computeDilute(
      MatrixInclusionMicrostructure<N, real, LengthType, StressType> &micro,
      tfel::math::stensor<N, real> &E,
      bool mat_iso) {
    const auto np = micro.get_number_of_phases();
    const auto phases = micro.get_inclusionPhases();
    const auto phase0 = micro.get_matrixPhase();
    const auto C0 = phase0.stiffness;
    StressType E0;
    real nu0;
    if (mat_iso==true) {
      const auto kappa0 =
          StressType(1) *
          tfel::math::quad_product<N, real>(tfel::math::st2tost2<N, real>::J(),
                                            1 / StressType(1) * C0) /
          3;
      const auto mu0 =
          StressType(1) *
          tfel::math::quad_product<N, real>(tfel::math::st2tost2<N, real>::K(),
                                            1 / StressType(1) * C0) /
          10;
      E0 = 9 * kappa0 * mu0 / (3 * kappa0 + mu0);
      nu0 = (3 * kappa0 - 2 * mu0) / 2 / (3 * kappa0 + mu0);
    }

    const auto f0 = micro.get_matrix_fraction();
    HomogenizationScheme<N, real, LengthType, StressType> h_s;
    auto Chom = C0;
    std::vector<tfel::math::stensor<N, real>> strains = {E};
    std::vector<tfel::math::stensor<N, StressType>> stresses = {C0 * E};
    std::vector<tfel::math::st2tost2<N, real>> strain_sm = {E ^ E};
    //std::vector<tfel::math::st2tost2<N, StressType>> stress_sm = 
    //    {};  // PROBLEMMMEE DEEE TYPPPEEE
    for (unsigned int i = 0; i < np - 1; i++) {
      auto phasei = phases[i];
      auto Ci = phasei.stiffness;
      auto fi = phasei.fraction;
      Ellipsoid<N, real, LengthType> inc_i = phasei.inclusion;
      auto ai = inc_i.axes_semiLengths[0];
      auto bi = inc_i.axes_semiLengths[1];
      auto ci = inc_i.axes_semiLengths[2];
      auto n_a = inc_i.axes_orientations[0];
      auto n_b = inc_i.axes_orientations[1];
      tfel::math::st2tost2<N, real> Ai;
      // ATTENTTION 2D !!!
      if (N == 3u) {
        if (mat_iso==true) {
          // ATTENTION C_i base globale
          Ai = computeAnisotropicEllipsoidLocalisationTensor<real, StressType,
                                                             LengthType>(
              E0, nu0, Ci, n_a, ai, n_b, bi, ci);
        } else {
          Ai = computeAnisotropicLocalisationTensor<real, StressType,
                                                    LengthType>(
              C0, Ci, n_a, ai, n_b, bi, ci, 12);
        }
      }
      Chom += fi * (Ci - C0) * Ai;
      strains.push_back(Ai * E);
      stresses.push_back(Ci * Ai * E);
    }
    h_s.homogenized_stiffness = Chom;
    h_s.mean_strains = strains;
    h_s.mean_stresses = stresses;

    // if (mat_iso){
    //   for (unsigned int r=0;r<np;r++){
    //     auto ArE=strains[r];
    //   tfel::math::st2tost2<N, real> epsepsr=(ArE)^(ArE);
      
    //   for (unsigned int R=0;R<np-1;R++){
    //     auto CR=phases[R].stiffness;
    //     auto D_Rr=dAR/dCr;
    //   auto S_Rr=E*((Cr*ArE)+())*E;
    //     epsepsr+=phases[R].fraction*S_Rr;
    //   }
    //   strain_sm.push_back(epsepsr);
    //   }
    // }
    h_s.strain_second_moments = strain_sm;
    //h_s.stress_second_moments = stress_sm;
    return h_s;
  };

  /*!
   * Here is the Self-consistent scheme which returns an object of
   * type HomogenizationScheme from a Polycrystal.
   * \tparam unsigned short int: dimension \tparam
   * real: underlying type \tparam LengthType: type of the lengths related to
   * the inclusion \tparam StressType: type of the elastic constants related to
   * the inclusion \return an object of type HomogenizationScheme
   */
  template <unsigned short int N,
            typename real,
            typename LengthType,
            typename StressType>
  HomogenizationScheme<N, real, LengthType, StressType> computeSelfConsistent(
      Polycrystal<N, real, LengthType, StressType> crystal,
      tfel::math::stensor<N, real> E, bool mat_iso) {
    const auto np = crystal.get_number_of_grains();
    const auto grains = crystal.get_grains();
    const auto f0 = crystal.get_total_fraction();
    // IFF f0 ==1  !!!!!!
    HomogenizationScheme<N, real, LengthType, StressType> h_s;
    // h_s.homogenized_stiffness=;
    // h_s.mean_strains=;
    // h_s.mean_stresses=;
    // h_s.strain_second_moments=;
    // h_s.stress_second_moments=;
    return h_s;
  }

}  // end of namespace tfel::material::homogenization::elasticity

#endif /* LIB_TFEL_MATERIAL_MICROSTRUCTURELINEARHOMOGENIZATION_IXX */
