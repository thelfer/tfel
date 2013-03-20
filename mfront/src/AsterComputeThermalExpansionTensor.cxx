/*!
 * \file   AsterComputeThermalExpansionTensor.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   18 mar 2008
 */

#include<algorithm>

#include"TFEL/Math/stensor.hxx"
#include"MFront/Aster/AsterComputeThermalExpansionTensor.hxx"

namespace aster
{

  static void
  AsterComputeIsotropicThermalExpansionTensor2D(const AsterReal* const props,
					       tfel::config::Types<2u,AsterReal,false>::ThermalExpansionTensor& A)
  {
    const double a = props[3];
    A(0)=A(1)=A(2)=a;
    A(3)=0.;
  }

  static void
  AsterComputeOrthotropicThermalExpansionTensor2D(const AsterReal* const props,
						 tfel::config::Types<2u,AsterReal,false>::ThermalExpansionTensor& A)
  {
    const double a0 = props[8];
    const double a1 = props[9];
    const double a2 = props[10];
    A(0)=a0;
    A(1)=a1;
    A(2)=a2;
    A(3)=0.;
  } // end of struct AsterComputeThermalExpansionTensor
 
  void
  AsterComputeThermalExpansionTensor<1u,ISOTROPIC>::exe(const AsterReal* const props,
							tfel::config::Types<1u,AsterReal,false>::ThermalExpansionTensor& A)
  {
    const double a = props[3];
    A(0)=A(1)=A(2)=a;
  } // end of struct AsterComputeThermalExpansionTensor

  void
  AsterComputeThermalExpansionTensor<2u,ISOTROPIC>::exe(const AsterReal* const props,
							 tfel::config::Types<2u,AsterReal,false>::ThermalExpansionTensor& A)
  {
    AsterComputeIsotropicThermalExpansionTensor2D(props,A);
  } // end of struct AsterComputeThermalExpansionTensor

  void
  AsterComputeThermalExpansionTensor<3u,ISOTROPIC>::exe(const AsterReal* const props,
						       tfel::config::Types<3u,AsterReal,false>::ThermalExpansionTensor& A)
  {
    const double a = props[3];
    A(0)=A(1)=A(2)=a;
    A(3)=A(4)=A(5)=0.;
  } // end of struct AsterComputeThermalExpansionTensor

  void
  AsterComputeThermalExpansionTensor<1u,ORTHOTROPIC>::exe(const AsterReal* const props,
							  tfel::config::Types<1u,AsterReal,false>::ThermalExpansionTensor& A)
  {
    const double a0 = props[7];
    const double a1 = props[8];
    const double a2 = props[9];
    A(0)=a0;
    A(1)=a1;
    A(2)=a2;
  } // end of struct AsterComputeThermalExpansionTensor

  void
  AsterComputeThermalExpansionTensor<2u,ORTHOTROPIC>::exe(const AsterReal* const props,
							  tfel::config::Types<2u,AsterReal,false>::ThermalExpansionTensor& C)
  {
    AsterComputeOrthotropicThermalExpansionTensor2D(props,C);
  } // end of struct AsterComputeThermalExpansionTensor

  void
  AsterComputeThermalExpansionTensor<3u,ORTHOTROPIC>::exe(const AsterReal* const props,
							 tfel::config::Types<3u,AsterReal,false>::ThermalExpansionTensor& A)
  {
    const double a0 = props[10];
    const double a1 = props[11];
    const double a2 = props[12];
    A(0)=a0;
    A(1)=a1;
    A(2)=a2;
    A(3)=A(4)=A(5)=0.;
  } // end of struct AsterComputeThermalExpansionTensor

} // end of namespace aster
