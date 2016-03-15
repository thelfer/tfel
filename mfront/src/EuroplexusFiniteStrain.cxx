/*!
 * \file   EuroplexusFiniteStrain.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   15 mars 2016
 */

#include<iostream>
#include<cstdlib>
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/tensor.hxx"
#include"TFEL/Math/Stensor/StensorView.hxx"
#include"TFEL/Math/T2toST2/T2toST2View.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2View.hxx"
#include"MFront/Europlexus/EuroplexusFiniteStrain.hxx"

namespace epx{

  void
  computeGreenLagrangeStrain(EuroplexusReal * const eto,
			     EuroplexusReal * const deto,
			     const EuroplexusReal * const F0,
			     const EuroplexusReal * const F1,
			     const EuroplexusInt h)
  {
    using namespace tfel::math;
    using real = EuroplexusReal;
    if((h==3)||(h==2)||(h==1)){
      StensorView<2u,real> e(eto);
      StensorView<2u,real> de(deto);
      e  = computeGreenLagrangeTensor(tensor<2u,real>(F0));
      de = computeGreenLagrangeTensor(tensor<2u,real>(F1))-e;
    } else if(h==0){
      StensorView<3u,real> e(eto);
      StensorView<3u,real> de(deto);
      e  = computeGreenLagrangeTensor(tensor<3u,real>(F0));
      de = computeGreenLagrangeTensor(tensor<3u,real>(F1))-e;
    } else {
      std::cerr << "epx::computeGreenLagrangeStrain: "
		<< "unsupported hypothesis" << std::endl;
      ::exit(-1);
    }
  }

  void
  computeSecondPiolaKirchhoffStressFromCauchyStress(EuroplexusReal * const sig,
						    const EuroplexusReal * const STRESS,
						    const EuroplexusReal * const F0,
						    const EuroplexusInt h)
  {
    using namespace tfel::math;
    using real = EuroplexusReal;
    if((h==3)||(h==2)||(h==1)){
      StensorView<2u,real> s(sig);
      stensor<2u,real> c(STRESS);
      s = convertCauchyStressToSecondPiolaKirchhoffStress(c,tensor<2u,real>(F0));
    } else if(h==0){
      StensorView<3u,real> s(sig);
      stensor<3u,real> c(STRESS);
      s = convertCauchyStressToSecondPiolaKirchhoffStress(c,tensor<3u,real>(F0));
    } else {
      std::cerr << "epx::computeSecondPiolaKirchhoffStressFromCauchyStress: "
		<< "unsupported hypothesis" << std::endl;
      ::exit(-1);
    }
  }

  void
  computeCauchyStressFromSecondPiolaKirchhoffStress(EuroplexusReal * const STRESS,
						    const EuroplexusReal * const sig,
						    const EuroplexusReal * const F1,
						    const EuroplexusInt h)
  {
    using namespace tfel::math;
    using real = EuroplexusReal;
    if((h==3)||(h==2)||(h==1)){
      stensor<2u,real> s(sig);
      StensorView<2u,real> c(STRESS);
      c = convertSecondPiolaKirchhoffStressToCauchyStress(s,tensor<2u,real>(F1));
    } else if(h==0){
      stensor<3u,real> s(sig);
      StensorView<3u,real> c(STRESS);
      c = convertSecondPiolaKirchhoffStressToCauchyStress(s,tensor<3u,real>(F1));
    } else {
      std::cerr << "epx::computeCauchyStressFromSecondPiolaKirchhoffStress: "
		<< "unsupported hypothesis" << std::endl;
      ::exit(-1);
    }
  }  

  void
  computeCauchyStressDerivativeFromSecondPiolaKirchhoffStressDerivative(EuroplexusReal * const D,
									const EuroplexusReal * const sig,
									const EuroplexusReal * const F1,
									const EuroplexusInt h)
  {
    using namespace tfel::math;
    using real = EuroplexusReal;
    if((h==3)||(h==2)||(h==1)){
      ST2toST2View<2u,real> De{D};
      stensor<2u,real> s{sig};
      tensor<2u,real> F(F1);
      t2tost2<2u,real> dS = 0.5*De*t2tost2<2u,real>::dCdF(F); 
      t2tost2<2u,real> dtau;
      computePushForwardDerivative(dtau,dS,s,F) ; 
      T2toST2View<2u,real> ds{D};
      computeCauchyStressDerivativeFromKirchoffStressDerivative(ds,dtau,s,F);
    } else if(h==0){
      ST2toST2View<3u,real> De{D};
      stensor<3u,real> s{sig};
      tensor<3u,real> F(F1);
      t2tost2<3u,real> dS = 0.5*De*t2tost2<3u,real>::dCdF(F); 
      t2tost2<3u,real> dtau;
      computePushForwardDerivative(dtau,dS,s,F) ; 
      T2toST2View<3u,real> ds{D};
      computeCauchyStressDerivativeFromKirchoffStressDerivative(ds,dtau,s,F);
    } else {
      std::cerr << "epx::computeCauchyStressDerivativeFromSecondPiolaKirchhoffStressDerivative: "
		<< "unsupported hypothesis" << std::endl;
      ::exit(-1);
    }
  }
  
} // end of namespace epx
