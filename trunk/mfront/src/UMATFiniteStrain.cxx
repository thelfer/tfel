/*! 
 * \file  UMATFiniteStrain.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 févr. 2013
 */

#include"TFEL/Math/tmatrix.hxx"
#include"MFront/UMAT/UMATFiniteStrain.hxx"

namespace umat
{

  void
  UMATFiniteStrain::computeGreenLagrangeStrain(UMATReal* const e,
					       const UMATReal* const F,
					       const UMATInt NTENS)
  {
    using namespace tfel::math;
    // warning F is given in the fortran convention
    const tmatrix<3u,3u,UMATReal>& tF = *(reinterpret_cast<const tmatrix<3u,3u,UMATReal> *>(F));
    e[0]=0.5*(tF(0,2)*tF(0,2)+tF(0,1)*tF(0,1)+tF(0,0)*tF(0,0)-1.);
    e[1]=0.5*(tF(1,2)*tF(1,2)+tF(1,1)*tF(1,1)+tF(1,0)*tF(1,0)-1.);
    e[2]=0.5*(tF(2,2)*tF(2,2)+tF(2,1)*tF(2,1)+tF(2,0)*tF(2,0)-1.);
    if(NTENS==4){
      e[3]=tF(0,2)*tF(1,2)+tF(0,1)*tF(1,1)+tF(0,0)*tF(1,0);
    } else if (NTENS==6){
      e[3]=tF(0,2)*tF(1,2)+tF(0,1)*tF(1,1)+tF(0,0)*tF(1,0);
      e[4]=tF(0,2)*tF(2,2)+tF(0,1)*tF(2,1)+tF(0,0)*tF(2,0);
      e[5]=tF(1,2)*tF(2,2)+tF(1,1)*tF(2,1)+tF(1,0)*tF(2,0);
    }
  } // end of UMATFiniteStrain::computeGreenLagrangeStrain


  void
  UMATFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress(UMATReal* const s,
								      const UMATReal* const F,
								      const UMATInt NTENS)
  {
    using namespace tfel::math;
    // warning F is given in the fortran convention
    const tmatrix<3u,3u,UMATReal>& tF = *(reinterpret_cast<const tmatrix<3u,3u,UMATReal> *>(F));
    const UMATReal inv_J = 1/det(tF);
    UMATReal pks[6];
    pks[0] = s[0];
    pks[1] = s[1];
    pks[2] = s[2];
    pks[3] = 0;
    pks[4] = 0;
    pks[5] = 0;
    if(NTENS==4){
      pks[3] = s[3];
    } else if(NTENS==6){
      pks[3] = s[3];
      pks[4] = s[4];
      pks[5] = s[5];
    }
    s[0] = inv_J*(tF(2,0)*pks[2]*tF(2,0)+pks[5]*tF(1,0)+pks[4]*tF(0,0)+tF(1,0)*pks[5]*tF(2,0)+pks[1]*tF(1,0)+pks[3]*tF(0,0)+tF(0,0)*pks[4]*tF(2,0)+pks[3]*tF(1,0)+pks[0]*tF(0,0));
    s[1] = inv_J*(tF(2,1)*pks[2]*tF(2,1)+pks[5]*tF(1,1)+pks[4]*tF(0,1)+tF(1,1)*pks[5]*tF(2,1)+pks[1]*tF(1,1)+pks[3]*tF(0,1)+tF(0,1)*pks[4]*tF(2,1)+pks[3]*tF(1,1)+pks[0]*tF(0,1));
    s[2] = inv_J*(tF(2,2)*pks[2]*tF(2,2)+pks[5]*tF(1,2)+pks[4]*tF(0,2)+tF(1,2)*pks[5]*tF(2,2)+pks[1]*tF(1,2)+pks[3]*tF(0,2)+tF(0,2)*pks[4]*tF(2,2)+pks[3]*tF(1,2)+pks[0]*tF(0,2));
    if(NTENS==4){
      s[3] = inv_J*(tF(2,0)*pks[2]*tF(2,1)+pks[5]*tF(1,1)+pks[4]*tF(0,1)+tF(1,0)*pks[5]*tF(2,1)+pks[1]*tF(1,1)+pks[3]*tF(0,1)+tF(0,0)*pks[4]*tF(2,1)+pks[3]*tF(1,1)+pks[0]*tF(0,1));
    } else if(NTENS==6){
      s[3] = inv_J*(tF(2,0)*pks[2]*tF(2,1)+pks[5]*tF(1,1)+pks[4]*tF(0,1)+tF(1,0)*pks[5]*tF(2,1)+pks[1]*tF(1,1)+pks[3]*tF(0,1)+tF(0,0)*pks[4]*tF(2,1)+pks[3]*tF(1,1)+pks[0]*tF(0,1));
      s[4] = inv_J*(tF(2,0)*pks[2]*tF(2,2)+pks[5]*tF(1,2)+pks[4]*tF(0,2)+tF(1,0)*pks[5]*tF(2,2)+pks[1]*tF(1,2)+pks[3]*tF(0,2)+tF(0,0)*pks[4]*tF(2,2)+pks[3]*tF(1,2)+pks[0]*tF(0,2));
      s[5] = inv_J*(tF(2,1)*pks[2]*tF(2,2)+pks[5]*tF(1,2)+pks[4]*tF(0,2)+tF(1,1)*pks[5]*tF(2,2)+pks[1]*tF(1,2)+pks[3]*tF(0,2)+tF(0,1)*pks[4]*tF(2,2)+pks[3]*tF(1,2)+pks[0]*tF(0,2));
    }
  } // end of UMATFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress

} // end of namespace umat
