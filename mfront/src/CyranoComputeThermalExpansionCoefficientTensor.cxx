/*!
 * \file   CyranoComputeThermalExpansionCoefficientTensor.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#include<algorithm>

#include"TFEL/Math/stensor.hxx"
#include"MFront/Cyrano/CyranoComputeThermalExpansionCoefficientTensor.hxx"

namespace cyrano
{
  void
  CyranoComputeThermalExpansionCoefficientTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				      ISOTROPIC>::exe(const CyranoReal* const props,
						      tfel::config::Types<1u,CyranoReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    const double a = props[3];
    A(0)=A(1)=A(2)=a;
  } // end of struct CyranoComputeThermalExpansionCoefficientTensor

  void
  CyranoComputeThermalExpansionCoefficientTensor<tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				      ORTHOTROPIC>::exe(const CyranoReal* const props,
							tfel::config::Types<1u,CyranoReal,false>::ThermalExpansionCoefficientTensor& A)
  {
    const double a0 = props[7];
    const double a1 = props[8];
    const double a2 = props[9];
    A(0)=a0;
    A(1)=a1;
    A(2)=a2;
  } // end of struct CyranoComputeThermalExpansionCoefficientTensor
  
} // end of namespace cyrano
