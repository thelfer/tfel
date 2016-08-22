/*! 
 * \file  mfront/src/AbaqusFiniteStrain.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  21/08/2016
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
#include"MFront/Abaqus/AbaqusFiniteStrain.hxx"

namespace abaqus
{
  
  void
  AbaqusFiniteStrain::computeGreenLagrangeStrain(AbaqusReal* const e,
						 const AbaqusReal* const F,
						 const AbaqusInt NTENS,
						 const bool ps)
  {
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
    if(!ps){
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
  } // end of AbaqusFiniteStrain::computeGreenLagrangeStrain


  void
  AbaqusFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress(AbaqusReal* const STRESS,
  								      const AbaqusReal* const F,
  								      const AbaqusInt NTENS,
  								      const bool ps,
								      const AbaqusReal Fzz)
  {
    // warning F is given in the fortran convention
    // (F0 F3 F6
    //  F1 F4 F7
    //  F2 F5 F8) 
    const AbaqusReal F0 = F[0];
    const AbaqusReal F1 = F[1];
    const AbaqusReal F2 = F[2];
    const AbaqusReal F3 = F[3];
    const AbaqusReal F4 = F[4];
    const AbaqusReal F5 = F[5];
    const AbaqusReal F6 = F[6];
    const AbaqusReal F7 = F[7];
    const AbaqusReal F8 = !ps ? F[8] : Fzz;
    // determinant
    const AbaqusReal J  = F0*(F4*F8-F5*F7)-F3*(F1*F8-F2*F7)+(F1*F5-F2*F4)*F6;
    // inverse of the determinant
    const AbaqusReal iJ = 1/J;
    // inverse of F
    // maxima          :     F4*F8-F5*F7 
    const AbaqusReal iF0 = iJ*(F4*F8-F5*F7);
    // maxima          :     F2*F7-F1*F8 
    const AbaqusReal iF1 = iJ*(F2*F7-F1*F8);
    // maxima          :     F1*F5-F2*F4 
    const AbaqusReal iF2 = iJ*(F1*F5-F2*F4);
    // maxima          :     F5*F6-F3*F8 
    const AbaqusReal iF3 = iJ*(F5*F6-F3*F8); 
    // maxima          :     F0*F8-F2*F6 
    const AbaqusReal iF4 = iJ*(F0*F8-F2*F6); 
    // maxima          :     F2*F3-F0*F5 
    const AbaqusReal iF5 = iJ*(F2*F3-F0*F5); 
    // maxima          :     F3*F7-F4*F6 
    const AbaqusReal iF6 = iJ*(F3*F7-F4*F6); 
    // maxima          :     F1*F6-F0*F7 
    const AbaqusReal iF7 = iJ*(F1*F6-F0*F7); 
    // maxima          :     F0*F4-F1*F3 
    const AbaqusReal iF8 = iJ*(F0*F4-F1*F3); 
    // sk2
    const AbaqusReal p0 = STRESS[0];
    const AbaqusReal p1 = STRESS[1];
    const AbaqusReal p2 = STRESS[2];
    const AbaqusReal p3 = (NTENS>=4) ? STRESS[3] : AbaqusReal(0);
    const AbaqusReal p4 = (NTENS==6) ? STRESS[4] : AbaqusReal(0);
    const AbaqusReal p5 = (NTENS==6) ? STRESS[5] : AbaqusReal(0);
    STRESS[0] = J    *(iF3*(p5*iF6+p1*iF3+p3*iF0)+iF0*(p4*iF6+p3*iF3+p0*iF0)+iF6*(p2*iF6+p5*iF3+p4*iF0));
    STRESS[1] = J*(iF4*(p5*iF7+p1*iF4+p3*iF1)+iF1*(p4*iF7+p3*iF4+p0*iF1)+iF7*(p2*iF7+p5*iF4+p4*iF1));
    STRESS[2] = J*(iF5*(p5*iF8+p1*iF5+p3*iF2)+iF2*(p4*iF8+p3*iF5+p0*iF2)+iF8*(p2*iF8+p5*iF5+p4*iF2));
    if(NTENS==4){
      if(ps){
	STRESS[2] = 0;
      }
      STRESS[3] = J*(iF3*(p5*iF7+p1*iF4+p3*iF1)+iF0*(p4*iF7+p3*iF4+p0*iF1)+iF6*(p2*iF7+p5*iF4+p4*iF1));
    } else if(NTENS==6){
      STRESS[3] = J*(iF3*(p5*iF7+p1*iF4+p3*iF1)+iF0*(p4*iF7+p3*iF4+p0*iF1)+iF6*(p2*iF7+p5*iF4+p4*iF1));
      STRESS[4] = J*(iF3*(p5*iF8+p1*iF5+p3*iF2)+iF0*(p4*iF8+p3*iF5+p0*iF2)+iF6*(p2*iF8+p5*iF5+p4*iF2));
      STRESS[5] = J*(iF4*(p5*iF8+p1*iF5+p3*iF2)+iF1*(p4*iF8+p3*iF5+p0*iF2)+iF7*(p2*iF8+p5*iF5+p4*iF2));
    }
  } // end of AbaqusFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress

  void
  AbaqusFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress(AbaqusReal* const sk2,
								      const AbaqusReal* const STRESS,
  								      const AbaqusReal* const F,
  								      const AbaqusInt NTENS,
  								      const bool ps,
								      const AbaqusReal Fzz)
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
    AbaqusFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress(sk2,F,NTENS,ps,Fzz);
  } // end of AbaqusFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress

  void
  AbaqusFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress(AbaqusReal* const s,
								      const AbaqusReal* const F,
								      const AbaqusInt NTENS,
  								      const bool ps,
								      const AbaqusReal Fzz)
  {
    // warning F is given in the fortran convention
    // (F0 F3 F6
    //  F1 F4 F7
    //  F2 F5 F8) 
    const AbaqusReal F0 = F[0];
    const AbaqusReal F1 = F[1];
    const AbaqusReal F2 = F[2];
    const AbaqusReal F3 = F[3];
    const AbaqusReal F4 = F[4];
    const AbaqusReal F5 = F[5];
    const AbaqusReal F6 = F[6];
    const AbaqusReal F7 = F[7];
    const AbaqusReal F8 = !ps ? F[8] : Fzz;
    // determinant
    const AbaqusReal inv_J = 1/(F0*(F4*F8-F5*F7)-F3*(F1*F8-F2*F7)+(F1*F5-F2*F4)*F6);
    const AbaqusReal p0 = s[0];
    const AbaqusReal p1 = s[1];
    const AbaqusReal p2 = s[2];
    const AbaqusReal p3 = (NTENS>=4) ? s[3] : AbaqusReal(0);
    const AbaqusReal p4 = (NTENS==6) ? s[4] : AbaqusReal(0);
    const AbaqusReal p5 = (NTENS==6) ? s[5] : AbaqusReal(0);
    // maxima :   F3*(p5*F6+p1*F3+p3*F0)+F0*(p4*F6+p3*F3+p0*F0)+F6*(p2*F6+p5*F3+p4*F0) 
    s[0] = inv_J*(F3*(p5*F6+p1*F3+p3*F0)+F0*(p4*F6+p3*F3+p0*F0)+F6*(p2*F6+p5*F3+p4*F0));
    // maxima :   F4*(p5*F7+p1*F4+p3*F1)+F1*(p4*F7+p3*F4+p0*F1)+F7*(p2*F7+p5*F4+p4*F1) 
    s[1] = inv_J*(F4*(p5*F7+p1*F4+p3*F1)+F1*(p4*F7+p3*F4+p0*F1)+F7*(p2*F7+p5*F4+p4*F1));
    // maxima :   F5*(p5*F8+p1*F5+p3*F2)+F2*(p4*F8+p3*F5+p0*F2)+F8*(p2*F8+p5*F5+p4*F2) 
    s[2] = inv_J*(F5*(p5*F8+p1*F5+p3*F2)+F2*(p4*F8+p3*F5+p0*F2)+F8*(p2*F8+p5*F5+p4*F2));
    if(NTENS==4){
      if(ps){
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
  } // end of AbaqusFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress

  void
  AbaqusFiniteStrain::computeAbaqusTangentOperatorFromCSE(AbaqusReal* const D,
							  const AbaqusReal* const CSE,
							  const AbaqusReal* const F1,
							  const AbaqusReal* const s,
							  const AbaqusInt NTENS,
							  const bool ps){
    // using namespace tfel::math;
    // using namespace tfel::material;
    // using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
    // const AbaqusReal cste = std::sqrt(AbaqusReal(2));
    // if(NTENS==4){
    //   tensor<2u,AbaqusReal>   F = tensor<2u,AbaqusReal>::buildFromFortranMatrix(F1);
    //   st2tost2<2u,AbaqusReal> C = {
    // 	CSE[0],CSE[4],CSE[8] ,CSE[12],
    // 	CSE[1],CSE[5],CSE[9] ,CSE[13],
    // 	CSE[2],CSE[6],CSE[10],CSE[14],
    // 	CSE[3],CSE[7],CSE[11],CSE[15]};
    //   Dv = convert<TangentOperator::ABAQUS,
    // 		   TangentOperator::DS_DEGL>(C,F,F,{s[0],s[1],s[2],s[3]/cste});
    //   // now convert to abaqus      
    // } else if(NTENS==6){
    //   auto access = [&D](const int i,
    // 			 const int j) -> AbaqusReal&{
    // 	return D[6*i+j];
    //   };
    //   tensor<3u,AbaqusReal>   F = tensor<3u,AbaqusReal>::buildFromFortranMatrix(F1);
    //   st2tost2<3u,AbaqusReal> C = {
    // 	CSE[0],CSE[6] ,CSE[12],CSE[18],CSE[24],CSE[30],
    // 	CSE[1],CSE[7] ,CSE[13],CSE[19],CSE[25],CSE[31],
    // 	CSE[2],CSE[8] ,CSE[14],CSE[20],CSE[26],CSE[32],
    // 	CSE[3],CSE[9] ,CSE[15],CSE[21],CSE[27],CSE[33];
    //   	CSE[4],CSE[10],CSE[16],CSE[22],CSE[28],CSE[34];
    // 	CSE[5],CSE[11],CSE[17],CSE[23],CSE[29],CSE[35]};
    //   ST2toST2View<3u,AbaqusReal> Dv;
    //   Dv = convert<TangentOperator::ABAQUS,
    // 		   TangentOperator::DS_DEGL>(C,F,F,{s[0],s[1],s[2],s[3]/cste,s[4]/cste,s[5]/cste});
    // }
  } // end of AbaqusFiniteStrain::computeAbaqusTangentOperatorFromCSE


  
} // end of namespace abaqus
