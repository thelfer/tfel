/*!
 * \file   TFELTypes.hxx
 * \brief  This file implements the Types class
 * \author Helfer Thomas
 * \date   08 Aug 2006
 */

#ifndef _LIB_TFELTYPES_H_
#define _LIB_TFELTYPES_H_ 

#include"TFEL/Math/Forward/qt.hxx"
#include"TFEL/Math/Forward/tvector.hxx"
#include"TFEL/Math/Forward/tmatrix.hxx"
#include"TFEL/Math/Forward/tensor.hxx"
#include"TFEL/Math/Forward/stensor.hxx"
#include"TFEL/Math/Forward/st2tost2.hxx"
#include"TFEL/Math/Stensor/StensorSizeToDime.hxx"
#include"TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include"TFEL/Config/Internals/TFELTypesInternals.hxx"

namespace tfel
{
  
  namespace config
  {

    /*!
     * \class Types
     * \brief A set of useful typedef.
     * \param unsigned short N, space dimension.
     * \param typename T, numerical type, double by default.
     * \param bool use_quantities, says if one shall use quantities. 
     * \author Helfer Thomas
     * \date   08 Aug 2006
     */
    template<unsigned short N,typename T = double, bool use_quantities = true>
    struct Types{

      static const unsigned short Size = tfel::math::StensorDimeToSize<N>::value;

      typedef tfel::math::qt<tfel::math::NoUnit,T>         real;
      typedef tfel::math::qt<tfel::math::Time,T>           time;
      typedef tfel::math::qt<tfel::math::Frequency,T>      frequency;
      typedef tfel::math::qt<tfel::math::Length,T>         length;
      typedef tfel::math::qt<tfel::math::Length,T>         displacement;
      typedef tfel::math::qt<tfel::math::InvLength,T>      inv_length;
      typedef tfel::math::qt<tfel::math::NoUnit,T>         strain;
      typedef tfel::math::qt<tfel::math::Frequency,T>      strainrate;
      typedef tfel::math::qt<tfel::math::Force,T>         force;
      typedef tfel::math::qt<tfel::math::Stress,T>         stress;
      typedef tfel::math::qt<tfel::math::StressRate,T>     stressrate;
      typedef tfel::math::qt<tfel::math::Temperature,T>    temperature;
      typedef tfel::math::qt<tfel::math::InvTemperature,T> thermalexpansion;
      typedef tfel::math::qt<tfel::math::Density,T>        density;
      typedef tfel::math::qt<tfel::math::EnergyDensity,T>  energy_density;
      typedef tfel::math::tvector<N,real>                  TVector;
      typedef tfel::math::stensor<N,real>                  Stensor;
      typedef tfel::math::stensor<N,frequency>             FrequencyStensor;
      typedef tfel::math::tvector<N,force>                 ForceTVector;
      typedef tfel::math::stensor<N,stress>                StressStensor;
      typedef tfel::math::stensor<N,stressrate>            StressRateStensor;
      typedef tfel::math::tvector<N,length>                DisplacementTVector;
      typedef tfel::math::stensor<N,tfel::math::qt<tfel::math::NoUnit,T> >    StrainStensor;
      typedef tfel::math::stensor<N,tfel::math::qt<tfel::math::Frequency,T> > StrainRateStensor;
      typedef tfel::math::stensor<N,thermalexpansion>      ThermalExpansionTensor;
      typedef tfel::math::tensor<N,tfel::math::qt<tfel::math::NoUnit,T> > Tensor;
      typedef tfel::math::tensor<N,tfel::math::qt<tfel::math::NoUnit,T> > DeformationGradientTensor;
      typedef tfel::math::st2tost2<N,real>                 Stensor4;
      typedef tfel::math::st2tost2<N,stress>               StiffnessTensor;
      typedef typename tfel::config::internals::PositionType<N,T,true>::type PositionType;
      typedef typename tfel::config::internals::SpatialGradType<N,T,true>::type SpatialGradType;
      typedef typename tfel::config::internals::JacobianType<N,T,true>::type JacobianType;
      typedef typename tfel::config::internals::InvJacobianType<N,T,true>::type InvJacobianType;
      /* Thermal related stuff */
      typedef tfel::math::qt<tfel::math::ThermalConductivity,T> thermal_conductivity;
      typedef typename tfel::config::internals::HeatFluxVectorType<N,T,true>::type HeatFluxVector;
      typedef typename tfel::config::internals::ThermalConductivityMatrixType<N,T,true>::type ThermalConductivityMatrix;
      
    };
    
    /*
     * Partial specialisation
     */
    template<unsigned short N,typename T>
    struct Types<N,T,false>{

      static const unsigned short Size = tfel::math::StensorDimeToSize<N>::value;
      typedef T real;
      typedef T time;
      typedef T frequency;
      typedef T length;
      typedef T inv_length;
      typedef T displacement;
      typedef T strain;
      typedef T strainrate;
      typedef T force;
      typedef T stress;
      typedef T stressrate;
      typedef T temperature;
      typedef T thermalexpansion;
      typedef T density;
      typedef T energy_density;
      typedef tfel::math::tvector<N,T> TVector;
      typedef tfel::math::stensor<N,T> Stensor;
      typedef tfel::math::stensor<N,T> FrequencyStensor;
      typedef tfel::math::tvector<N,T> ForceTVector;
      typedef tfel::math::stensor<N,T> StressStensor;
      typedef tfel::math::stensor<N,T> StressRateStensor;
      typedef tfel::math::tvector<N,T> DisplacementTVector;
      typedef tfel::math::stensor<N,T> StrainStensor;
      typedef tfel::math::stensor<N,T> StrainRateStensor;
      typedef tfel::math::stensor<N,T> ThermalExpansionTensor;
      typedef tfel::math::tensor<N,T>  Tensor;
      typedef tfel::math::tensor<N,T>  DeformationGradientTensor;
      typedef tfel::math::st2tost2<N,T> Stensor4;
      typedef tfel::math::st2tost2<N,T> StiffnessTensor;
      typedef typename tfel::config::internals::PositionType<N,T,false>::type  PositionType;
      typedef typename tfel::config::internals::SpatialGradType<N,T,true>::type SpatialGradType;
      typedef typename tfel::config::internals::JacobianType<N,T,false>::type JacobianType;
      typedef typename tfel::config::internals::InvJacobianType<N,T,false>::type InvJacobianType;
      /* Thermal related stuff */
      typedef T thermal_conductivity;
      typedef typename tfel::config::internals::HeatFluxVectorType<N,T,false>::type HeatFluxVector;
      typedef typename tfel::config::internals::ThermalConductivityMatrixType<N,T,false>::type ThermalConductivityMatrix;

    };

  } // end of namespace config

} // end of namespace tfel

#endif /* _LIB_TFELTYPES_H */

