/*!
 * \file   mfront/src/AbaqusComputeThermalExpansionCoefficientTensor.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   18 mar 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<algorithm>

#include"TFEL/Math/stensor.hxx"
#include"MFront/Abaqus/AbaqusComputeThermalExpansionCoefficientTensor.hxx"

namespace abaqus
{

  static void
  AbaqusComputeIsotropicThermalExpansionCoefficientTensor2D(const AbaqusReal* const props,
					       tfel::config::Types<2u,AbaqusReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    const AbaqusReal a = props[0];
    A(0)=A(1)=A(2)=a;
    A(3)=0.;
  }

  static void
  AbaqusComputeOrthotropicThermalExpansionCoefficientTensor2D(const AbaqusReal* const props,
						 tfel::config::Types<2u,AbaqusReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    const AbaqusReal a0 = props[0];
    const AbaqusReal a1 = props[1];
    const AbaqusReal a2 = props[2];
    A(0)=a0;
    A(1)=a1;
    A(2)=a2;
    A(3)=0.;
  } // end of struct AbaqusComputeThermalExpansionCoefficientTensor
 
  void
  AbaqusComputeThermalExpansionCoefficientTensor<abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::PLANESTRESS,
				    ISOTROPIC>::exe(const AbaqusReal* const props,
						    tfel::config::Types<2u,AbaqusReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    AbaqusComputeIsotropicThermalExpansionCoefficientTensor2D(props,A);
  } // end of struct AbaqusComputeThermalExpansionCoefficientTensor

  void
  AbaqusComputeThermalExpansionCoefficientTensor<abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::AXISYMMETRICAL,
				    ISOTROPIC>::exe(const AbaqusReal* const props,
						    tfel::config::Types<2u,AbaqusReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    AbaqusComputeIsotropicThermalExpansionCoefficientTensor2D(props,A);
  } // end of struct AbaqusComputeThermalExpansionCoefficientTensor

  void
  AbaqusComputeThermalExpansionCoefficientTensor<abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::PLANESTRAIN,
				    ISOTROPIC>::exe(const AbaqusReal* const props,
						    tfel::config::Types<2u,AbaqusReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    AbaqusComputeIsotropicThermalExpansionCoefficientTensor2D(props,A);
  } // end of struct AbaqusComputeThermalExpansionCoefficientTensor

  void
  AbaqusComputeThermalExpansionCoefficientTensor<abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
				    ISOTROPIC>::exe(const AbaqusReal* const props,
						    tfel::config::Types<2u,AbaqusReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    AbaqusComputeIsotropicThermalExpansionCoefficientTensor2D(props,A);
  } // end of struct AbaqusComputeThermalExpansionCoefficientTensor

  void
  AbaqusComputeThermalExpansionCoefficientTensor<abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
				    ISOTROPIC>::exe(const AbaqusReal* const props,
						    tfel::config::Types<3u,AbaqusReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    const AbaqusReal a = props[0];
    A(0)=A(1)=A(2)=a;
    A(3)=A(4)=A(5)=0.;
  } // end of struct AbaqusComputeThermalExpansionCoefficientTensor

  void
  AbaqusComputeThermalExpansionCoefficientTensor<abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::PLANESTRESS,
				    ORTHOTROPIC>::exe(const AbaqusReal* const props,
						      tfel::config::Types<2u,AbaqusReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    AbaqusComputeOrthotropicThermalExpansionCoefficientTensor2D(props,A);
  } // end of struct AbaqusComputeThermalExpansionCoefficientTensor

  void
  AbaqusComputeThermalExpansionCoefficientTensor<abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::AXISYMMETRICAL,
				    ORTHOTROPIC>::exe(const AbaqusReal* const props,
						      tfel::config::Types<2u,AbaqusReal,false>::ThermalExpansionCoefficientTensor& C)
  {
    AbaqusComputeOrthotropicThermalExpansionCoefficientTensor2D(props,C);
  } // end of struct AbaqusComputeThermalExpansionCoefficientTensor
  
  void
  AbaqusComputeThermalExpansionCoefficientTensor<abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::PLANESTRAIN,
				    ORTHOTROPIC>::exe(const AbaqusReal* const props,
						      tfel::config::Types<2u,AbaqusReal,false>::ThermalExpansionCoefficientTensor& C)
  {
    AbaqusComputeOrthotropicThermalExpansionCoefficientTensor2D(props,C);
  } // end of struct AbaqusComputeThermalExpansionCoefficientTensor
  
  void
  AbaqusComputeThermalExpansionCoefficientTensor<abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
				    ORTHOTROPIC>::exe(const AbaqusReal* const props,
						      tfel::config::Types<2u,AbaqusReal,false>::ThermalExpansionCoefficientTensor& C)
  {
    AbaqusComputeOrthotropicThermalExpansionCoefficientTensor2D(props,C);
  } // end of struct AbaqusComputeThermalExpansionCoefficientTensor
  
  void
  AbaqusComputeThermalExpansionCoefficientTensor<abaqus::SMALLSTRAINSTANDARDBEHAVIOUR,
				    tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
				    ORTHOTROPIC>::exe(const AbaqusReal* const props,
						      tfel::config::Types<3u,AbaqusReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    const AbaqusReal a0 = props[0];
    const AbaqusReal a1 = props[1];
    const AbaqusReal a2 = props[2];
    A(0)=a0;
    A(1)=a1;
    A(2)=a2;
    A(3)=A(4)=A(5)=0.;
  } // end of struct AbaqusComputeThermalExpansionCoefficientTensor

} // end of namespace abaqus
