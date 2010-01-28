/*!
 * \file   UMATComputeThermalExpansionTensor.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   18 mar 2008
 */

#include<algorithm>

#include"MFront/UMAT/UMATComputeThermalExpansionTensor.hxx"

namespace umat
{
 
  void
  UMATComputeThermalExpansionTensor<1u,ISOTROPIC>::exe(const UMATReal* const props,
						       tfel::config::Types<1u,UMATReal,false>::ThermalExpansionTensor& A)
  {
    const double a = props[3];
    A(0)=A(1)=A(2)=a;
  } // end of struct UMATComputeThermalExpansionTensor

  void
  UMATComputeThermalExpansionTensor<2u,ISOTROPIC>::exe(const UMATReal* const props,
						       tfel::config::Types<2u,UMATReal,false>::ThermalExpansionTensor& A)
  {
    const double a = props[3];
    A(0)=A(1)=A(2)=a;
    A(3)=0.;
  } // end of struct UMATComputeThermalExpansionTensor

  void
  UMATComputeThermalExpansionTensor<3u,ISOTROPIC>::exe(const UMATReal* const props,
						       tfel::config::Types<3u,UMATReal,false>::ThermalExpansionTensor& A)
  {
    const double a = props[3];
    A(0)=A(1)=A(2)=a;
    A(3)=A(4)=A(5)=0.;
  } // end of struct UMATComputeThermalExpansionTensor

  void
  UMATComputeThermalExpansionTensor<1u,ORTHOTROPIC>::exe(const UMATReal* const props,
							 tfel::config::Types<1u,UMATReal,false>::ThermalExpansionTensor& A)
  {
    const double a0 = props[7];
    const double a1 = props[8];
    const double a2 = props[9];
    A(0)=a0;
    A(1)=a1;
    A(2)=a2;
  } // end of struct UMATComputeThermalExpansionTensor

  void
  UMATComputeThermalExpansionTensor<2u,ORTHOTROPIC>::exe(const UMATReal* const props,
							 tfel::config::Types<2u,UMATReal,false>::ThermalExpansionTensor& A)
  {
    const double a0 = props[10];
    const double a1 = props[11];
    const double a2 = props[12];
    A(0)=a0;
    A(1)=a1;
    A(2)=a2;
    A(3)=0.;
  } // end of struct UMATComputeThermalExpansionTensor

  void
  UMATComputeThermalExpansionTensor<3u,ORTHOTROPIC>::exe(const UMATReal* const props,
							 tfel::config::Types<3u,UMATReal,false>::ThermalExpansionTensor& A)
  {
    const double a0 = props[16];
    const double a1 = props[17];
    const double a2 = props[18];
    A(0)=a0;
    A(1)=a1;
    A(2)=a2;
    A(3)=A(4)=A(5)=0.;
  } // end of struct UMATComputeThermalExpansionTensor

} // end of namespace umat
