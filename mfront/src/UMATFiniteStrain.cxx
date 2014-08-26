/*! 
 * \file  UMATFiniteStrain.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 févr. 2013
 */

#include"MFront/UMAT/UMATFiniteStrain.hxx"

namespace umat
{

  void
  UMATFiniteStrain::computeGreenLagrangeStrain(UMATReal* const e,
					       const UMATReal* const F,
					       const UMATInt NTENS)
  {
    // warning F is given in the fortran convention
    e[0]=0.5*(F[0]*F[0]+F[1]*F[1]+F[2]*F[2]-1);
    e[1]=0.5*(F[3]*F[3]+F[4]*F[4]+F[5]*F[5]-1);
    e[2]=0.5*(F[6]*F[6]+F[7]*F[7]+F[8]*F[8]-1);
    if(NTENS==4){
      e[3]= F[2]*F[5]+F[1]*F[4]+F[0]*F[3];
    } else if (NTENS==6){
      e[3]=F[2]*F[5]+F[1]*F[4]+F[0]*F[3];
      e[4]=F[2]*F[8]+F[1]*F[7]+F[0]*F[6];
      e[5]=F[5]*F[8]+F[4]*F[7]+F[3]*F[6];
    }
  } // end of UMATFiniteStrain::computeGreenLagrangeStrain


  void
  UMATFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress(UMATReal* const s,
								      const UMATReal* const F,
								      const UMATInt NTENS)
  {
    // warning F is given in the fortran convention
    const UMATReal F0 = F[0];
    const UMATReal F1 = F[1];
    const UMATReal F2 = F[2];
    const UMATReal F3 = F[3];
    const UMATReal F4 = F[4];
    const UMATReal F5 = F[5];
    const UMATReal F6 = F[6];
    const UMATReal F7 = F[7];
    const UMATReal F8 = F[8];
    // determinant
    const UMATReal inv_J = F0*(F4*F8-F5*F7)-F3*(F1*F8-F2*F7)+(F1*F5-F2*F4)*F6;
    const UMATReal p0 = s[0];
    const UMATReal p1 = s[1];
    const UMATReal p2 = s[2];
    const UMATReal p3 = (NTENS>=4) ? s[3] : UMATReal(0);
    const UMATReal p4 = (NTENS==6) ? s[4] : UMATReal(0);
    const UMATReal p5 = (NTENS==6) ? s[5] : UMATReal(0);
    s[0] = inv_J*(F3*(p5*F6+p1*F3+p3*F0)+F0*(p4*F6+p3*F3+p0*F0)+F6*(p2*F6+p5*F3+p4*F0));
    s[1] = inv_J*(F4*(p5*F7+p1*F4+p3*F1)+F1*(p4*F7+p3*F4+p0*F1)+F7*(p2*F7+p5*F4+p4*F1));
    s[2] = inv_J*(F5*(p5*F8+p1*F5+p3*F2)+F2*(p4*F8+p3*F5+p0*F2)+F8*(p2*F8+p5*F5+p4*F2));
    if(NTENS==4){
      s[3] = inv_J*(F3*(p5*F7+p1*F4+p3*F1)+F0*(p4*F7+p3*F4+p0*F1)+F6*(p2*F7+p5*F4+p4*F1));
    } else if(NTENS==6){
      s[3] = inv_J*(F3*(p5*F7+p1*F4+p3*F1)+F0*(p4*F7+p3*F4+p0*F1)+F6*(p2*F7+p5*F4+p4*F1));
      s[4] = inv_J*(F3*(p5*F8+p1*F5+p3*F2)+F0*(p4*F8+p3*F5+p0*F2)+F6*(p2*F8+p5*F5+p4*F2));
      s[5] = inv_J*(F4*(p5*F8+p1*F5+p3*F2)+F1*(p4*F8+p3*F5+p0*F2)+F7*(p2*F8+p5*F5+p4*F2));
    }
  } // end of UMATFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress

} // end of namespace umat
