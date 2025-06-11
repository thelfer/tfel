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
#include "TFEL/Material/AnisotropicEshelbyTensor.hxx"
#include "TFEL/Material/LinearHomogenizationSchemes.hxx"



namespace tfel::material::homogenization::elasticity {

  /*!
   * The struct HomogenizationScheme is a struct whose attributes are
   * various objects that are useful in homogenization
   * theory, as homogenized stiffness, mean strain localisation tensors,
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
    tfel::math::stensor<N, StressType> effective_polarisation;
    std::vector<tfel::math::st2tost2<N, real>> mean_strain_localisation_tensors;
    std::vector<tfel::math::stensor<N, real>> mean_free_strains;
    std::vector<tfel::math::st2tost2<N, real>> strain_second_moments;
  };

  /*!
   * Here is the Dilute scheme which returns an object of
   * type HomogenizationScheme from a MatrixInclusionMicrostructure.
   * \tparam unsigned short int: dimension \tparam
   * real: underlying type \tparam LengthType: type of the lengths related to
   * the inclusion \tparam StressType: type of the elastic constants related to
   * the inclusion \return an object of type HomogenizationScheme
   * \param[in] micro: MatrixInclusionMicrostructure
   */
  template <unsigned short int N,
            typename real,
            typename LengthType,
            typename StressType>
  HomogenizationScheme<N, real, LengthType, StressType> computeDilute(
      MatrixInclusionMicrostructure<N, real, LengthType, StressType> &micro,
      std::vector<tfel::math::stensor<N, StressType>> &polarisations,
      bool get_strain_second_moments, tfel::math::stensor<N, real> &E, bool isotropic_matrix,const int max_iter_anisotropic_integration = 12) {
    const auto np = micro.get_number_of_phases();
    if (polarisations.size()!=np){
       tfel::reportContractViolation("the size of your polarization vector does not correspond to the number of phases");
    }
    const auto C0 = micro.get_matrix_elasticity();
    const auto tau0 = polarisations[0];
    HomogenizationScheme<N, real, LengthType, StressType> h_s;
    auto Chom = C0;
    auto tau_eff=tau0;
    using compliance = typename tfel::math::invert_type<StressType>;
    std::vector<tfel::math::st2tost2<N, real>> localisators = {tfel::math::st2tost2<N,real>::Id()};
    std::vector<tfel::math::stensor<N, real>> free_strains = {};
    std::vector<tfel::math::st2tost2<N, compliance>> hill_tensors = {};
    std::vector<tfel::math::st2tost2<N, real>> second_moments = {};
    if (get_strain_second_moments){
    	second_moments.push_back(E ^ E);
    }
    for (unsigned int i = 0; i < np - 1; i++) {
      auto phasei = micro.get_inclusionPhase(i);
      auto Ci = (*phasei).stiffness;
      auto fi = (*phasei).fraction;
      auto taui=polarisations[i+1];
      
      // TO DO : 2D /////////////////////////////////
      auto Ai = (*phasei).computeMeanLocalisator(C0,isotropic_matrix,max_iter_anisotropic_integration);
      Chom += fi * (Ci - C0) * Ai;
      tau_eff+=fi*tfel::math::transpose(Ai)*(taui-tau0);
      localisators.push_back(Ai);
    }
    h_s.homogenized_stiffness = Chom;
    h_s.effective_polarisation = tau_eff;
    h_s.mean_strain_localisation_tensors = localisators;
    h_s.mean_free_strains = free_strains;// TO DO //////////////////////////////////////
    if (get_strain_second_moments){
	    ///TO DO ////
       h_s.strain_second_moments = second_moments;
    }
    return h_s;
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
      MatrixInclusionMicrostructure<N, real, LengthType, StressType> &micro,
      std::vector<tfel::math::stensor<N, StressType>> &polarisations,
      bool get_strain_second_moments, tfel::math::stensor<N, real> &E, bool isotropic_matrix,const int max_iter_anisotropic_integration = 12) {
    const auto np = micro.get_number_of_phases();
    if (polarisations.size()!=np){
       tfel::reportContractViolation("the size of your polarization vector does not correspond to the number of phases");
    }
    const auto C0 = micro.get_matrix_elasticity();
    const auto tau0 = polarisations[0];
    const auto f0 = micro.get_matrix_fraction();
    HomogenizationScheme<N, real, LengthType, StressType> h_s;
    auto Chom = C0;
    auto tau_eff=tau0;
    using compliance = typename tfel::math::invert_type<StressType>;
    std::vector<tfel::math::st2tost2<N, real>> localisators = {};
    std::vector<tfel::math::stensor<N, real>> free_strains = {};
    std::vector<tfel::math::st2tost2<N, compliance>> hill_tensors = {};
    std::vector<tfel::math::st2tost2<N, real>> second_moments = {};
    tfel::math::st2tost2<N, real> A0 = f0*tfel::math::st2tost2<N,real>::Id();
    for (unsigned int i = 0; i < np - 1; i++) {
      auto phasei = micro.get_inclusionPhase(i);
      auto fi = (*phasei).fraction;
      // TO DO : 2D /////////////////////////////////
      auto Ai = (*phasei).computeMeanLocalisator(C0,isotropic_matrix,max_iter_anisotropic_integration);
      localisators.push_back(Ai);
      A0+=fi*Ai;
    }
    A0=invert(A0);
    localisators.insert(localisators.begin(),A0);
    for (unsigned int i = 0; i < np - 1; i++) {
      auto phasei = micro.get_inclusionPhase(i);
      auto Ci = (*phasei).stiffness;
      auto fi = (*phasei).fraction;
      auto taui=polarisations[i+1];
      localisators[i+1]=localisators[i+1]*A0;
      auto Ai=localisators[i+1];
      Chom += fi * (Ci - C0) * Ai;
      tau_eff+=fi*transpose(Ai)*(taui-tau0);
    }
    h_s.homogenized_stiffness = Chom;
    h_s.effective_polarisation = tau_eff;
    h_s.mean_strain_localisation_tensors = localisators;
    h_s.mean_free_strains = free_strains;// TO DO //////////////////////////////////////////
    if (get_strain_second_moments){
	 ////////////////TO DO/////////////////
       h_s.strain_second_moments = second_moments;
    }
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
      Polycrystal<N, real, LengthType, StressType> &crystal,
      std::vector<tfel::math::stensor<N, StressType>> &polarisations,
      bool get_strain_second_moments, tfel::math::stensor<N, real> &E, bool isotropic_matrix, const int max_iter, const int max_iter_anisotropic_integration = 8) {
      
    real error = 1.;
    const auto np = crystal.get_number_of_grains();
    HomogenizationScheme<N, real, LengthType, StressType> h_s;
    std::vector<tfel::math::st2tost2<N, real>> localisators = {};
    std::vector<tfel::math::stensor<N, real>> free_strains = {};
    std::vector<tfel::math::st2tost2<N, real>> second_moments = {};
    auto Chom = tfel::math::st2tost2<N,StressType>::Id();
    auto Chom_= tfel::math::st2tost2<N, StressType>{StressType(0)};
    int iter=0;
    while (iter<max_iter){
       auto A0 = tfel::math::st2tost2<N, real>{real{}};
       for (unsigned int i = 0; i < np ; i++) {
         auto graini = crystal.get_grain(i);
         auto fi = (*graini).fraction;
         // TO DO : 2D /////////////////////////////////
         auto Ai = (*graini).computeMeanLocalisator(Chom,isotropic_matrix,max_iter_anisotropic_integration);
         localisators.push_back(Ai);
         A0+=fi*Ai;
       }
       A0=invert(A0);
       auto Ch = tfel::math::st2tost2<N, StressType>{StressType{}};
       for (unsigned int i = 0; i < np ; i++) {
          auto graini = crystal.get_grain(i);
          auto Ci = (*graini).stiffness;
          auto fi = (*graini).fraction;
          localisators[i]=localisators[i]*A0;
          auto Ai=localisators[i];
          Ch += fi * Ci * Ai;
       }
       auto size = tfel::math::StensorDimeToSize<N>::value;
       for (int i=0;i<size;i++)
       for (int j=0;j<size;j++){
          Chom_(i,j)=Chom(i,j);
          Chom(i,j)=Ch(i,j);
       }
       error=tfel::material::internals::relative_error<N,real,StressType>(Chom_,Chom);
       if ((not(isotropic_matrix)) || (iter/10==iter/10.)){
           std::cout<<"relative difference between previous Chom and actual Chom: "<<error<<std::endl;
           real C11=real(Chom(0,0).getValue());
           std::cout<<"actual Chom_11: "<<C11<<std::endl;
       }
    iter++;
    }
    h_s.homogenized_stiffness = Chom;
    //h_s.effective_polarisation = tau_eff; TO DO ///////////
    h_s.mean_strain_localisation_tensors = localisators;
    h_s.mean_free_strains = free_strains;// TO DO //////////////////////////////////////////
    if (get_strain_second_moments){
	 ////////////////TO DO/////////////////
       h_s.strain_second_moments = second_moments;
    }
    return h_s;
  }
 
}// end of namespace tfel::material::homogenization::elasticity


#endif /* LIB_TFEL_MATERIAL_MICROSTRUCTURELINEARHOMOGENIZATION_IXX */
