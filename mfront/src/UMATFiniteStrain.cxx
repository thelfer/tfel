/*! 
 * \file  mfront/src/UMATFiniteStrain.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 févr. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cmath>
#include<sstream>
#include<stdexcept>

#include"TFEL/Math/tensor.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/Math/Stensor/StensorView.hxx"
#include"TFEL/Math/Stensor/ConstStensorView.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2View.hxx"
#include"TFEL/Math/ST2toST2/ConstST2toST2View.hxx"
#include"MFront/UMAT/UMATFiniteStrain.hxx"

namespace umat
{
  
  static void
  UMATCheckNDIValue(const UMATInt NDI)
  {
    using namespace std;
    if((NDI!=2)&&(NDI!=0)&&(NDI!=-1)&&
       (NDI!=-2)&&(NDI!=-3)&&(NDI!=14)){
      ostringstream msg;
      msg << "UMATCheckNDIValue : unsupported modelling hypothesis (" 
	  << NDI<< ")";
      throw(runtime_error(msg.str()));
    }
  }

  void
  UMATFiniteStrain::computeGreenLagrangeStrain(UMATReal* const e,
					       const UMATReal* const F,
					       const UMATInt NTENS,
					       const UMATInt NDI)
  {
    UMATCheckNDIValue(NDI);
    // warning : F is given in the fortran convention
    // (F0 F3 F6
    //  F1 F4 F7
    //  F2 F5 F8)
    // maxima :  
    // (0.5*(F2^2+F1^2+F0^2-1)  0.5*(F2*F5+F1*F4+F0*F3) 0.5*(F2*F8+F1*F7+F0*F6)
    //  0.5*(F2*F5+F1*F4+F0*F3) 0.5*(F5^2+F4^2+F3^2-1)  0.5*(F5*F8+F4*F7+F3*F6)
    //  0.5*(F2*F8+F1*F7+F0*F6) 0.5*(F5*F8+F4*F7+F3*F6) 0.5*(F8^2+F7^2+F6^2-1)
    e[0]=0.5*(F[0]*F[0]+F[1]*F[1]+F[2]*F[2]-1);
    e[1]=0.5*(F[3]*F[3]+F[4]*F[4]+F[5]*F[5]-1);
    if(NDI!=-2){
      e[2]=0.5*(F[6]*F[6]+F[7]*F[7]+F[8]*F[8]-1);
    } else {
      e[2]=0.;
    }
    // warning : e must be computed using Voigt
    // notations. Extradiagonals terms are mulitplied by two
    if(NTENS==4){
      e[3]= F[2]*F[5]+F[1]*F[4]+F[0]*F[3];
    } else if (NTENS==6){
      e[3]=F[2]*F[5]+F[1]*F[4]+F[0]*F[3];
      e[4]=F[2]*F[8]+F[1]*F[7]+F[0]*F[6];
      e[5]=F[5]*F[8]+F[4]*F[7]+F[3]*F[6];
    }
  } // end of UMATFiniteStrain::computeGreenLagrangeStrain


  void
  UMATFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress(UMATReal* const STRESS,
  								      const UMATReal* const F,
  								      const UMATInt NTENS,
  								      const UMATInt NDI,
								      const UMATReal Fzz)
  {
    UMATCheckNDIValue(NDI);
    // warning F is given in the fortran convention
    // (F0 F3 F6
    //  F1 F4 F7
    //  F2 F5 F8) 
    const UMATReal F0 = F[0];
    const UMATReal F1 = F[1];
    const UMATReal F2 = F[2];
    const UMATReal F3 = F[3];
    const UMATReal F4 = F[4];
    const UMATReal F5 = F[5];
    const UMATReal F6 = F[6];
    const UMATReal F7 = F[7];
    const UMATReal F8 = NDI!=-2 ? F[8] : Fzz;
    // determinant
    const UMATReal J  = F0*(F4*F8-F5*F7)-F3*(F1*F8-F2*F7)+(F1*F5-F2*F4)*F6;
    // inverse of the determinant
    const UMATReal iJ = 1/J;
    // inverse of F
    // maxima          :     F4*F8-F5*F7 
    const UMATReal iF0 = iJ*(F4*F8-F5*F7);
    // maxima          :     F2*F7-F1*F8 
    const UMATReal iF1 = iJ*(F2*F7-F1*F8);
    // maxima          :     F1*F5-F2*F4 
    const UMATReal iF2 = iJ*(F1*F5-F2*F4);
    // maxima          :     F5*F6-F3*F8 
    const UMATReal iF3 = iJ*(F5*F6-F3*F8); 
    // maxima          :     F0*F8-F2*F6 
    const UMATReal iF4 = iJ*(F0*F8-F2*F6); 
    // maxima          :     F2*F3-F0*F5 
    const UMATReal iF5 = iJ*(F2*F3-F0*F5); 
    // maxima          :     F3*F7-F4*F6 
    const UMATReal iF6 = iJ*(F3*F7-F4*F6); 
    // maxima          :     F1*F6-F0*F7 
    const UMATReal iF7 = iJ*(F1*F6-F0*F7); 
    // maxima          :     F0*F4-F1*F3 
    const UMATReal iF8 = iJ*(F0*F4-F1*F3); 
    // sk2
    const UMATReal p0 = STRESS[0];
    const UMATReal p1 = STRESS[1];
    const UMATReal p2 = STRESS[2];
    const UMATReal p3 = (NTENS>=4) ? STRESS[3] : UMATReal(0);
    const UMATReal p4 = (NTENS==6) ? STRESS[4] : UMATReal(0);
    const UMATReal p5 = (NTENS==6) ? STRESS[5] : UMATReal(0);
    STRESS[0] = J    *(iF3*(p5*iF6+p1*iF3+p3*iF0)+iF0*(p4*iF6+p3*iF3+p0*iF0)+iF6*(p2*iF6+p5*iF3+p4*iF0));
    STRESS[1] = J*(iF4*(p5*iF7+p1*iF4+p3*iF1)+iF1*(p4*iF7+p3*iF4+p0*iF1)+iF7*(p2*iF7+p5*iF4+p4*iF1));
    STRESS[2] = J*(iF5*(p5*iF8+p1*iF5+p3*iF2)+iF2*(p4*iF8+p3*iF5+p0*iF2)+iF8*(p2*iF8+p5*iF5+p4*iF2));
    if(NTENS==4){
      if(NDI==-2){
	STRESS[2] = 0;
      }
      STRESS[3] = J*(iF3*(p5*iF7+p1*iF4+p3*iF1)+iF0*(p4*iF7+p3*iF4+p0*iF1)+iF6*(p2*iF7+p5*iF4+p4*iF1));
    } else if(NTENS==6){
      STRESS[3] = J*(iF3*(p5*iF7+p1*iF4+p3*iF1)+iF0*(p4*iF7+p3*iF4+p0*iF1)+iF6*(p2*iF7+p5*iF4+p4*iF1));
      STRESS[4] = J*(iF3*(p5*iF8+p1*iF5+p3*iF2)+iF0*(p4*iF8+p3*iF5+p0*iF2)+iF6*(p2*iF8+p5*iF5+p4*iF2));
      STRESS[5] = J*(iF4*(p5*iF8+p1*iF5+p3*iF2)+iF1*(p4*iF8+p3*iF5+p0*iF2)+iF7*(p2*iF8+p5*iF5+p4*iF2));
    }
  } // end of UMATFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress

  void
  UMATFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress(UMATReal* const sk2,
								      const UMATReal* const STRESS,
  								      const UMATReal* const F,
  								      const UMATInt NTENS,
  								      const UMATInt NDI,
								      const UMATReal Fzz)
  {
    sk2[0] = STRESS[0];
    sk2[1] = STRESS[1];
    sk2[2] = STRESS[2];
    if(NTENS==4){
      sk2[3] = STRESS[3];
    } else if(NTENS==6){
      sk2[3] = STRESS[3];
      sk2[4] = STRESS[4];
      sk2[5] = STRESS[5];
    }
    UMATFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress(sk2,F,NTENS,NDI,Fzz);
  } // end of UMATFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress

  void
  UMATFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress(UMATReal* const s,
								      const UMATReal* const F,
								      const UMATInt NTENS,
  								      const UMATInt NDI,
								      const UMATReal Fzz)
  {
    UMATCheckNDIValue(NDI);
    // warning F is given in the fortran convention
    // (F0 F3 F6
    //  F1 F4 F7
    //  F2 F5 F8) 
    const UMATReal F0 = F[0];
    const UMATReal F1 = F[1];
    const UMATReal F2 = F[2];
    const UMATReal F3 = F[3];
    const UMATReal F4 = F[4];
    const UMATReal F5 = F[5];
    const UMATReal F6 = F[6];
    const UMATReal F7 = F[7];
    const UMATReal F8 = NDI!=-2 ? F[8] : Fzz;
    // determinant
    const UMATReal inv_J = 1/(F0*(F4*F8-F5*F7)-F3*(F1*F8-F2*F7)+(F1*F5-F2*F4)*F6);
    const UMATReal p0 = s[0];
    const UMATReal p1 = s[1];
    const UMATReal p2 = s[2];
    const UMATReal p3 = (NTENS>=4) ? s[3] : UMATReal(0);
    const UMATReal p4 = (NTENS==6) ? s[4] : UMATReal(0);
    const UMATReal p5 = (NTENS==6) ? s[5] : UMATReal(0);
    // maxima :   F3*(p5*F6+p1*F3+p3*F0)+F0*(p4*F6+p3*F3+p0*F0)+F6*(p2*F6+p5*F3+p4*F0) 
    s[0] = inv_J*(F3*(p5*F6+p1*F3+p3*F0)+F0*(p4*F6+p3*F3+p0*F0)+F6*(p2*F6+p5*F3+p4*F0));
    // maxima :   F4*(p5*F7+p1*F4+p3*F1)+F1*(p4*F7+p3*F4+p0*F1)+F7*(p2*F7+p5*F4+p4*F1) 
    s[1] = inv_J*(F4*(p5*F7+p1*F4+p3*F1)+F1*(p4*F7+p3*F4+p0*F1)+F7*(p2*F7+p5*F4+p4*F1));
    // maxima :   F5*(p5*F8+p1*F5+p3*F2)+F2*(p4*F8+p3*F5+p0*F2)+F8*(p2*F8+p5*F5+p4*F2) 
    s[2] = inv_J*(F5*(p5*F8+p1*F5+p3*F2)+F2*(p4*F8+p3*F5+p0*F2)+F8*(p2*F8+p5*F5+p4*F2));
    if(NTENS==4){
      if(NDI==-2){
	s[2] = 0;
      }
      // maxima :   F3*(p5*F7+p1*F4+p3*F1)+F0*(p4*F7+p3*F4+p0*F1)+F6*(p2*F7+p5*F4+p4*F1) 
      s[3] = inv_J*(F3*(p5*F7+p1*F4+p3*F1)+F0*(p4*F7+p3*F4+p0*F1)+F6*(p2*F7+p5*F4+p4*F1));
    } else if(NTENS==6){
      // maxima :   F3*(p5*F7+p1*F4+p3*F1)+F0*(p4*F7+p3*F4+p0*F1)+F6*(p2*F7+p5*F4+p4*F1) 
      s[3] = inv_J*(F3*(p5*F7+p1*F4+p3*F1)+F0*(p4*F7+p3*F4+p0*F1)+F6*(p2*F7+p5*F4+p4*F1));
      // maxima :   F3*(p5*F8+p1*F5+p3*F2)+F0*(p4*F8+p3*F5+p0*F2)+F6*(p2*F8+p5*F5+p4*F2) 
      s[4] = inv_J*(F3*(p5*F8+p1*F5+p3*F2)+F0*(p4*F8+p3*F5+p0*F2)+F6*(p2*F8+p5*F5+p4*F2));
      // maxima :   F4*(p5*F8+p1*F5+p3*F2)+F1*(p4*F8+p3*F5+p0*F2)+F7*(p2*F8+p5*F5+p4*F2) 
      s[5] = inv_J*(F4*(p5*F8+p1*F5+p3*F2)+F1*(p4*F8+p3*F5+p0*F2)+F7*(p2*F8+p5*F5+p4*F2));
    }
  } // end of UMATFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress

  static void
  UMATFiniteStrainComputeLogarithmicStrainAndDerivative1D(UMATReal* const P,
							  UMATReal* const E,
							  const UMATReal* const F,
							  const UMATInt NDI)
  {
    using namespace std;
    UMATCheckNDIValue(NDI);
    E[0] = log(F[0]);
    E[1] = log(F[4]);
    E[2] = log(F[8]);
    P[0] = 1/(F[0]*F[0]);
    P[1] = 1/(F[4]*F[4]);
    P[2] = 1/(F[8]*F[8]);
  } // end of UMATFiniteStrainComputeLogarithmicStrainAndDerivative1D

  static void
  UMATFiniteStrainComputeLogarithmicStrainAndDerivative2D(UMATReal* const P,
							  UMATReal* const E,
							  const UMATReal* const F,
							  const UMATInt NDI)
  {
    using namespace tfel::math;
    using std::log;
    using std::sqrt;
    static const UMATReal cste = sqrt(UMATReal(2));
    UMATCheckNDIValue(NDI);
    tvector<3u,UMATReal>    vp;
    tvector<3u,UMATReal>    log_vp;
    tmatrix<3u,3u,UMATReal> m;
    stensor<2u,UMATReal>  n0;
    stensor<2u,UMATReal>  n1;
    stensor<2u,UMATReal>  n2;
    tensor<2u,UMATReal> f;
    tensor<2u,UMATReal>::buildFromFortranMatrix(f,F);
    if(NDI==-2){
      f(2)=1.;
    }
    const stensor<2u,UMATReal> C = computeRightCauchyGreenTensor(f);
    C.computeEigenVectors(vp,m);
    log_vp(0) = log(vp(0));
    log_vp(1) = log(vp(1));
    if(NDI!=-2){
      log_vp(2) = log(vp(2));
    } else {
      log_vp(2) = 0.;
    }
    stensor<2u,UMATReal>::computeEigenTensors(n0,n1,n2,m);
    StensorView<2u,UMATReal>{E} = (log_vp(0)*n0+log_vp(1)*n1)/2;
    if(NDI==-2){
      E[2]  = 0;
    } else {
      E[2]  = log_vp(2)/2;
    }
    E[3] *= cste;
    // computing P
    const tvector<3u,UMATReal> v0 = m.column_view<0u>();
    const tvector<3u,UMATReal> v1 = m.column_view<1u>();
    const stensor<2u,UMATReal> n01 = stensor<2u,UMATReal>::buildFromVectorsSymmetricDiadicProduct(v0,v1)/cste;
    if(abs(vp(0)-vp(1))>1.e-12){
      ST2toST2View<2u,UMATReal>{P} = (n0^n0)/vp(0)+(n1^n1)/vp(1)+(n2^n2)/vp(2)+(log_vp(0)-log_vp(1))/(vp(0)-vp(1))*(n01^n01);
    } else {
      ST2toST2View<2u,UMATReal>{P} = (n0^n0)/vp(0)+(n1^n1)/vp(1)+(n2^n2)/vp(2)+(n01^n01)/vp(0);
    }
  } // end of UMATFiniteStrainComputeLogarithmicStrainAndDerivative2D

  static void
  UMATFiniteStrainComputeLogarithmicStrainAndDerivative3D(UMATReal* const P,
							  UMATReal* const E,
							  const UMATReal* const F)
  {
    using namespace tfel::math;
    using std::log;
    static const UMATReal cste = sqrt(UMATReal(2));
    tvector<3u,UMATReal>    vp,log_vp;
    stensor<3u,UMATReal>    n0,n1,n2;
    tmatrix<3u,3u,UMATReal> m;
    tensor<3u,UMATReal>  f;
    tensor<3u,UMATReal>::buildFromFortranMatrix(f,F);
    const stensor<3u,UMATReal> C = computeRightCauchyGreenTensor(f);
    C.computeEigenVectors(vp,m);
    log_vp(0) = log(vp(0));
    log_vp(1) = log(vp(1));
    log_vp(2) = log(vp(2));
    stensor<3u,UMATReal>::computeEigenTensors(n0,n1,n2,m);
    // logarithmic strain
    StensorView<3u,UMATReal>{E} = (log_vp(0)*n0+log_vp(1)*n1+log_vp(2)*n2)/2;
    E[3] *= cste;
    E[4] *= cste;
    E[5] *= cste;
    // computing P
    if((abs(vp(0)-vp(1))<1.e-12)&&(abs(vp(0)-vp(2))<1.e-12)){
      UMATReal vpm = (vp(0)+vp(1)+vp(2))/3;
      ST2toST2View<3u,UMATReal>{P} = st2tost2<3u,UMATReal>::Id()/vpm;
    } else if(abs(vp(0)-vp(1))<1.e-12){
      const tvector<3u,UMATReal> v0 = m.column_view<0u>();
      const tvector<3u,UMATReal> v1 = m.column_view<1u>();
      const tvector<3u,UMATReal> v2 = m.column_view<2u>();
      const stensor<3u,UMATReal> n01 = stensor<3u,UMATReal>::buildFromVectorsSymmetricDiadicProduct(v0,v1)/cste;
      const stensor<3u,UMATReal> n02 = stensor<3u,UMATReal>::buildFromVectorsSymmetricDiadicProduct(v0,v2)/cste;
      const stensor<3u,UMATReal> n12 = stensor<3u,UMATReal>::buildFromVectorsSymmetricDiadicProduct(v1,v2)/cste;
      UMATReal vpm = (vp(0)+vp(1))/2;
      ST2toST2View<3u,UMATReal>{P} = (((n0^n0)+(n1^n1)+(n01^n01))/vpm+(n2^n2)/vp(2)+
				      (log_vp(0)-log_vp(2))/(vpm-vp(2))*((n02^n02)+(n12^n12)));
    } else if(abs(vp(0)-vp(2))<1.e-12){
      const tvector<3u,UMATReal> v0 = m.column_view<0u>();
      const tvector<3u,UMATReal> v1 = m.column_view<1u>();
      const tvector<3u,UMATReal> v2 = m.column_view<2u>();
      const stensor<3u,UMATReal> n01 = stensor<3u,UMATReal>::buildFromVectorsSymmetricDiadicProduct(v0,v1)/cste;
      const stensor<3u,UMATReal> n02 = stensor<3u,UMATReal>::buildFromVectorsSymmetricDiadicProduct(v0,v2)/cste;
      const stensor<3u,UMATReal> n12 = stensor<3u,UMATReal>::buildFromVectorsSymmetricDiadicProduct(v1,v2)/cste;
      UMATReal vpm = (vp(0)+vp(2))/2;
      ST2toST2View<3u,UMATReal>{P} = (((n0^n0)+(n2^n2)+(n02^n02))/vpm+(n1^n1)/vp(1)+
				      (log_vp(0)-log_vp(1))/(vpm-vp(1))*((n01^n01)+(n12^n12)));
    } else if(abs(vp(1)-vp(2))<1.e-12){
      const tvector<3u,UMATReal> v0 = m.column_view<0u>();
      const tvector<3u,UMATReal> v1 = m.column_view<1u>();
      const tvector<3u,UMATReal> v2 = m.column_view<2u>();
      const stensor<3u,UMATReal> n01 = stensor<3u,UMATReal>::buildFromVectorsSymmetricDiadicProduct(v0,v1)/cste;
      const stensor<3u,UMATReal> n02 = stensor<3u,UMATReal>::buildFromVectorsSymmetricDiadicProduct(v0,v2)/cste;
      const stensor<3u,UMATReal> n12 = stensor<3u,UMATReal>::buildFromVectorsSymmetricDiadicProduct(v1,v2)/cste;
      UMATReal vpm = (vp(1)+vp(2))/2;
      ST2toST2View<3u,UMATReal>{P} = ((n0^n0)/vp(0)+((n1^n1)+(n2^n2)+(n12^n12))/vpm+
				      ((log_vp(0)-log_vp(1))/(vp(0)-vpm))*((n01^n01)+(n02^n02)));
    } else {
      st2tost2<3u,UMATReal> dn0_dC;
      st2tost2<3u,UMATReal> dn1_dC;
      st2tost2<3u,UMATReal> dn2_dC;
      stensor<3u,UMATReal>::computeEigenTensorsDerivatives(dn0_dC,dn1_dC,dn2_dC,vp,m,1.e-12);
      ST2toST2View<3u,UMATReal>{P} = (n0^n0)/vp(0)+log_vp(0)*dn0_dC+(n1^n1)/vp(1)+log_vp(1)*dn1_dC+(n2^n2)/vp(2)+log_vp(2)*dn2_dC;
    }
  }

   void
   UMATFiniteStrain::computeLogarithmicStrainAndDerivative(UMATReal* const P,
							   UMATReal* const E,
							   const UMATReal* const F,
							   const UMATInt NTENS,
							   const UMATInt NDI)
   {
     using namespace std;
     if(NTENS==3u){
       UMATFiniteStrainComputeLogarithmicStrainAndDerivative1D(P,E,F,NDI);
     } else if(NTENS==4u){
       UMATFiniteStrainComputeLogarithmicStrainAndDerivative2D(P,E,F,NDI);
     } else {
       UMATFiniteStrainComputeLogarithmicStrainAndDerivative3D(P,E,F);
     }
   }

   void
   UMATFiniteStrain::computeDualStressOfLogarithmicStrainFromCauchyStress(UMATReal* const s,
									  const UMATReal* const STRESS,
									  const UMATReal* const P,
									  const UMATReal* const F,
									  const UMATInt NTENS,
									  const UMATInt NDI,
									  const UMATReal Fzz)
   {
     using namespace tfel::math;
     using std::sqrt;
     static const UMATReal cste  = sqrt(UMATReal(2));
     static const UMATReal icste = UMATReal(1)/sqrt(UMATReal(2));
     // now, we compute the second Piolay Kirchoff stress
     UMATReal sk2[6u];
     // first we compute the second Piola-Kirchhoff stress
     UMATFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress(sk2,STRESS,F,NTENS,NDI,Fzz);
     // now the dual stress of the strain tensor
     if(NTENS==3u){
       s[0] = sk2[0] / P[0];
       s[1] = sk2[1] / P[1];
       s[2] = sk2[2] / P[2];
     } else if(NTENS==4u){
       sk2[3]    *= cste;
       const st2tost2<2u,UMATReal> iP(invert(ConstST2toST2View<2u,UMATReal>{P}));
       StensorView<2u,UMATReal>{s} = ConstStensorView<2u,UMATReal>{sk2}|iP;
       s[3] *= icste;
     } else if(NTENS==6u){
       sk2[3] *= cste;
       sk2[4] *= cste;
       sk2[5] *= cste;
       const st2tost2<3u,UMATReal> iP(invert(ConstST2toST2View<3u,UMATReal>{P}));
       StensorView<3u,UMATReal>{s} = ConstStensorView<3u,UMATReal>{sk2}|iP;
       s[3] *= icste;
       s[4] *= icste;
       s[5] *= icste;
     }
   }

   void
   UMATFiniteStrain::computeCauchyStressFromDualStressOfLogarithmicStrain(UMATReal* const STRESS,
									  const UMATReal* const s,
									  const UMATReal* const P,
									  const UMATReal* const F,
									  const UMATInt NTENS,
									  const UMATInt NDI,
									  const UMATReal Fzz)
   {
     using namespace tfel::math;
     using std::sqrt;
     static const UMATReal cste = UMATReal(1)/sqrt(UMATReal(2));
     // first we compute the second Piola-Kirchhoff stress
     if(NTENS==3u){
       STRESS[0] = s[0] * P[0];
       STRESS[1] = s[1] * P[1];
       STRESS[2] = s[2] * P[2];
     } else if(NTENS==4u){
       stensor<2u,UMATReal> T;
       T.importTab(s);
       StensorView<2u,UMATReal>{STRESS} = T|ConstST2toST2View<2u,UMATReal>{P};
       STRESS[3] *= cste;
     } else {
       stensor<3u,UMATReal> T;
       T.importTab(s);
       StensorView<3u,UMATReal>{STRESS} = T|ConstST2toST2View<3u,UMATReal>{P};
       STRESS[3] *= cste;
       STRESS[4] *= cste;
       STRESS[5] *= cste;
     }
     // now, we compute the cauchy stress
     UMATFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress(STRESS,F,NTENS,NDI,Fzz);
   }

} // end of namespace umat
