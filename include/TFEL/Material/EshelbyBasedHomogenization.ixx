/*!
 * \file   include/TFEL/Material/EshelbyBasedHomogenization.ixx
 * \author Antoine Martin
 * \date   24 October 2024
 * \brief  This file defines some homogenization schemes based on solution of Eshelby problem.
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ESHELBYBASEDHOMOGENIZATION_IXX
#define LIB_TFEL_MATERIAL_ESHELBYBASEDHOMOGENIZATION_IXX

#include <cmath>

namespace tfel::material {

   template <unsigned short N,typename real, typename StressType>
   struct EllipsoidMeanLocalisator
   {
	static constexpr auto eps = std::numeric_limits<real>::epsilon();

	TFEL_HOST_DEVICE static const tfel::math::st2tost2<3u, real>
      	Isotropic(const StressType& young, const real& nu, const tfel::math::st2tost2<3u,StressType>& C_i,
                  const real& a, const real& b, const real& c)
        {	
        	const std::vector<real> n_1 = {1.,0.,0.};
    		const std::vector<real> n_2 = {0.,1.,0.};
    		tfel::math::st2tost2<3u,real> A;
    		using namespace tfel::material;
    		if ((a==b) and (b==c)){A = SphereLocalisationTensor<real,StressType>(young,nu,C_i);}
    		else if (a==b){const auto A_=AxisymEllipsoidLocalisationTensor<real,StressType>(young,nu,C_i,n_1,c/a);
    			const auto A1111= 9./20*A_(1,1)+1./5*A_(0,0)+1./24*(A_(1,2)+A_(2,1)+2.*A_(5,5))+2./15*(A_(2,0)+A_(0,2)+2.*A_(4,4));
    			const auto A1122= 3./20*A_(1,1)+1./15*A_(0,0)+41./72*A_(1,2)+49./180*A_(0,1)+19./180*A_(1,0)-11./180*2.*A_(3,3)-1./24*2.*A_(5,5);
    			const auto G=A1111-A1122;
    			const auto ka=(A1111+2.*A1122)/3.;
    			using namespace tfel::math;
    			A=3*ka*st2tost2<3u,real>::J()+G*st2tost2<3u,real>::K();
    			      }
    		else if (a==c){const auto A_=AxisymEllipsoidLocalisationTensor<real,StressType>(young,nu,C_i,n_1,b/a);
	    		const auto A1111= 9./20*A_(1,1)+3./15*A_(0,0)+1./24*(A_(1,2)+A_(2,1)+2.*A_(5,5))+2./15*(A_(2,0)+A_(0,2)+2.*A_(4,4));
    			const auto A1122= 3./20*A_(1,1)+1./15*A_(0,0)+41./72*A_(1,2)+49./180*A_(0,1)+19./180*A_(1,0)-11./180*2.*A_(3,3)-1./24*2.*A_(5,5);
    			const auto G=A1111-A1122;
    			const auto ka=(A1111+2.*A1122)/3.;
    			using namespace tfel::math;
    			A=3*ka*st2tost2<3u,real>::J()+G*st2tost2<3u,real>::K();
    			      }
    		else if (b==c){const auto A_=AxisymEllipsoidLocalisationTensor<real,StressType>(young,nu,C_i,n_1,a/b);
	    		const auto A1111= 9./20*A_(1,1)+3./15*A_(0,0)+1./24*(A_(1,2)+A_(2,1)+2.*A_(5,5))+2./15*(A_(2,0)+A_(0,2)+2.*A_(4,4));
    			const auto A1122= 3./20*A_(1,1)+1./15*A_(0,0)+41./72*A_(1,2)+49./180*A_(0,1)+19./180*A_(1,0)-11./180*2.*A_(3,3)-1./24*2.*A_(5,5);
    			const auto G=A1111-A1122;
    			const auto ka=(A1111+2.*A1122)/3.;
    			using namespace tfel::math;
    			A=3*ka*st2tost2<3u,real>::J()+G*st2tost2<3u,real>::K();
    			      }
    		else {const auto A_ = GeneralEllipsoidLocalisationTensor<real,StressType>(young,nu,C_i,n_1,a,n_2,b,c);
    			const auto A1111= 1./5*A_(0,0)+37./160*A_(1,1)+1./5*A_(2,2)+7./80*(A_(0,1)+A_(1,0)+2.*A_(3,3))+1./15*(A_(0,2)+A_(2,0)+2.*A_(4,4))+1./15*(A_(1,2)+A_(2,1)+2.*A_(5,5));
    			const auto A1122= 1./15*A_(0,0)+1./5*A_(1,1)+1./15*A_(2,2)+19./120*A_(0,1)-1./120*2.*A_(3,3)+2./15*A_(1,0)+19./120*A_(2,1)-1./24*2.*A_(5,5)+1./15*A_(1,2)+2./15*A_(2,0)-1./30*2.*A_(4,4)+2./15*A_(0,2);
    			const auto G=A1111-A1122;
    			const auto ka=(A1111+2.*A1122)/3.;
    			using namespace tfel::math;
    			A=3*ka*st2tost2<3u,real>::J()+G*st2tost2<3u,real>::K();
    		     };
		return A;
         };//end of Isotropic
        
        TFEL_HOST_DEVICE static const tfel::math::st2tost2<3u, real>
      	PlanarIsotropic(const StressType& young, const real& nu, const real& f, const tfel::math::st2tost2<3u,StressType>& C_i,
                 	const std::vector<real>& n_a, const real& a, const real& b, const real& c)
        {
        	using namespace tfel::material;
        	std::vector<real> n_;
        	if ((n_a[1]!=0.)||(n_a[2]!=0.)){n_ = {1.,0.,0.};}
        	else {n_ = {0.,1.,0.}; };
        	const auto n_2 = VectProd(n_a,n_);
        	const auto n_3=VectProd(n_a,n_2);
        	const std::vector<real> n_x = {1.,0.,0.};
    		const std::vector<real> n_y = {0.,1.,0.};
    		const std::vector<real> n_z = {0.,0.,1.};
        	tfel::math::st2tost2<3u,real> A;
        	if ((a==b) and (b==c)){A = SphereLocalisationTensor<real,StressType>(young,nu,C_i);}
    		else if (b==c)
    		{
    			const auto A_ = AxisymEllipsoidLocalisationTensor<real,StressType>(young,nu,C_i,n_z,a/b);
    			const tfel::math::rotation_matrix<real> r={n_2[0],n_2[1],n_2[2],n_3[0],n_3[1],n_3[2],n_a[0],n_a[1],n_a[2]};
			A = change_basis(A_,r);
		}
    		else
    		{	
    			tfel::math::st2tost2<3u,real> A_;
    			if (a==b){A_=AxisymEllipsoidLocalisationTensor<real,StressType>(young,nu,C_i,n_y,c/a);}
    			else if (a==c){A_=AxisymEllipsoidLocalisationTensor<real,StressType>(young,nu,C_i,n_x,b/a);}
    			else {A_ = GeneralEllipsoidLocalisationTensor<real,StressType>(young,nu,C_i,n_z,a,n_x,b,c);};
    		     	const auto A11=3./8*(A_(0,0)+A_(1,1))+1./8*(A_(0,1)+A_(1,0)+2.*A_(3,3));
    		     	const auto A22=A11;
    		     	const auto A33=A_(2,2);
    		     	const auto A12=1./8*(A_(0,0)+A_(1,1))+3./8*(A_(0,1)+A_(1,0))-1./8*2.*A_(3,3);
    		     	const auto A21=A12;
    		     	const auto A13=1./2*(A_(0,2)+A_(1,2));
    		     	const auto A23=A13;
    		     	const auto A31=1./2*(A_(2,1)+A_(2,0));
    		     	const auto A32=A31;
    		     	const auto A44=2*(1./8*(A_(0,0)+A_(1,1)-A_(0,1)-A_(1,0))+1./2*A_(3,3)/2.);
    		     	const auto A55=2*(1./2*(A_(4,4)/2.+A_(5,5)/2.));
    		     	const auto A66=A55;
    		     	const auto zero = real{0};
    		     	const tfel::math::st2tost2<3u,real> A_moy= {A11, A12, A13,    zero, zero, zero,
								    A21, A22, A23,    zero, zero, zero,
								    A31, A32, A33,    zero, zero, zero,
								    zero, zero, zero, A44,  zero, zero,
								    zero, zero, zero, zero, A55,  zero,
								    zero, zero, zero, zero, zero, A66};
    		     	const tfel::math::rotation_matrix<real> r={n_2[0],n_2[1],n_2[2],n_3[0],n_3[1],n_3[2],n_a[0],n_a[1],n_a[2]};
			A = change_basis(A_moy,r);
    		 };
		return A;
        };//end of PlanarIsotropic
	 
	TFEL_HOST_DEVICE static const tfel::math::st2tost2<3u, real>
      	Oriented(const StressType& young, const real& nu, const real& f, const tfel::math::st2tost2<3u,StressType>& C_i,
                 const std::vector<real>& n_a, const real& a, const std::vector<real>& n_b,
                 const real& b, const real& c)
        {
        	tfel::math::st2tost2<3u,real> A;
        	using namespace tfel::material;
        	if ((a==b) and (b==c)){A = SphereLocalisationTensor<real,StressType>(young,nu,C_i);}
    		else if (a==b){const std::vector<real> n_1=VectProd(n_a,n_b); A=AxisymEllipsoidLocalisationTensor<real,StressType>(young,nu,C_i,n_1,c/a);}
    		else if (a==c){A=AxisymEllipsoidLocalisationTensor<real,StressType>(young,nu,C_i,n_b,b/a);}
    		else if (b==c){A=AxisymEllipsoidLocalisationTensor<real,StressType>(young,nu,C_i,n_a,a/b);}
    		else {A = GeneralEllipsoidLocalisationTensor<real,StressType>(young,nu,C_i,n_a,a,n_b,b,c);};
		return A;
        };//end of Oriented
	 
   }; //end of struct EllipsoidMeanLocalisator ;
				       
				       
				 
				       
  /* General Dilute Scheme knowing the mean localisator A on the inclusion
  */				      
  template <typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,StressType> DiluteScheme(const StressType& young, const real& nu, const real& f,
  									  const tfel::math::st2tost2<3u,StressType>& C_i,const tfel::math::st2tost2<3u,real>& A)
  {
  	tfel::math::st2tost2<3u,StressType> C_0;
    	using namespace tfel::material;
    	static constexpr auto value = StiffnessTensorAlterationCharacteristic::UNALTERED;
    	computeIsotropicStiffnessTensorII<3u,value,StressType,real>(C_0,young,nu);
  	using namespace tfel::math;
  	const auto C= C_i-C_0;
  	const auto Pr = typename Expr<st2tost2<3u,StressType>, ST2toST2ST2toST2ProductExpr<3u>>::Expr(C,A);
	return C_0+f*Pr; 
   };//end of DiluteScheme
  
  																				 
  /* General Mori-Tanaka Scheme knowing the mean localisator A on the inclusion
  */																		
  template <typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,StressType> MoriTanakaScheme(const StressType& young, const real& nu, const real& f,
  									      const tfel::math::st2tost2<3u,StressType>& C_i, const tfel::math::st2tost2<3u,real>& A)
  {
	tfel::math::st2tost2<3u,StressType> C_0;
    	using namespace tfel::material;
    	static constexpr auto value = StiffnessTensorAlterationCharacteristic::UNALTERED;
    	computeIsotropicStiffnessTensorII<3u,value,StressType,real>(C_0,young,nu);
  	using namespace tfel::math;
  	const auto C= C_i-C_0;
  	const auto Pr = typename Expr<st2tost2<3u,StressType>, ST2toST2ST2toST2ProductExpr<3u>>::Expr(C,A);
  	const auto inv = invert(f*A+(1-f)*st2tost2<3u,real>::Id());
  	const auto PPr = typename Expr<st2tost2<3u,StressType>, ST2toST2ST2toST2ProductExpr<3u>>::Expr(Pr,inv);
	return C_0+f*PPr; 
   };//end of MoriTanakaScheme


  /* Dilute Scheme for spheres
  */
  template <typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,StressType> SphereDiluteScheme(const StressType& young, const real& nu, const real& f,
  										const tfel::math::st2tost2<3u,StressType>& C_i){
  	using namespace tfel::material;
  	const auto A = SphereLocalisationTensor<real,StressType>(young, nu, C_i);
	return DiluteScheme<real,StressType>(young,nu,f,C_i,A); 
  																};
  								
  /* Mori-Tanaka Scheme for spheres
  */					    										   					
  template <typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,StressType> SphereMoriTanakaScheme(const StressType& young, const real& nu, const real& f,
  										    const tfel::math::st2tost2<3u,StressType>& C_i){
  	using namespace tfel::material;
  	const auto A = SphereLocalisationTensor<real,StressType>(young, nu, C_i);
	return MoriTanakaScheme<real,StressType>(young,nu,f,C_i,A);
 						    										   };
      
 /*
  * Ellipsoid-based Dilute Schemes
  *First : Isotropic distribution
  */
  template <typename real, typename StressType>
  const tfel::math::st2tost2<3u, StressType> IsotropicDiluteScheme(const StressType& young, const real& nu, const real& f, const tfel::math::st2tost2<3u,StressType>& C_i,
                                                                   const real& a, const real& b, const real& c){
    	using namespace tfel::material;
    	const auto A = EllipsoidMeanLocalisator<3u,real,StressType>::Isotropic(young, nu, C_i, a, b, c);
  	return DiluteScheme<real,StressType>(young,nu,f,C_i,A);
													       };

  /*Second : Planar isotropic distribution
  */												       								       
  template <typename real, typename StressType>
  const tfel::math::st2tost2<3u, StressType> PlanarIsotropicDiluteScheme(const StressType& young, const real& nu, const real& f, const tfel::math::st2tost2<3u,StressType>& C_i,
                                                                         const std::vector<real>& n_a, const real& a, const real& b, const real& c){
        using namespace tfel::material;
        const auto A = EllipsoidMeanLocalisator<3u,real,StressType>::PlanarIsotropic(young, nu, f, C_i, n_a, a, b, c);
        return DiluteScheme<real,StressType>(young,nu,f,C_i,A);
													 					  };
  
  /*Third : Unique orientation
  */
  template <typename real, typename StressType>
  const tfel::math::st2tost2<3u, StressType> OrientedDiluteScheme(const StressType& young, const real& nu, const real& f, const tfel::math::st2tost2<3u,StressType>& C_i,
                                                            const std::vector<real>& n_a, const real& a, const std::vector<real>& n_b,
                                                            const real& b, const real& c){
        using namespace tfel::material;
        const auto A = EllipsoidMeanLocalisator<3u,real,StressType>::Oriented(young, nu, f, C_i, n_a, a, n_b, b, c);
        return DiluteScheme<real,StressType>(young,nu,f,C_i,A);
											 };
											 
  /*
  * Ellipsoid-based Mori-Tanaka Schemes : same as for DiluteScheme
  */
  template <typename real, typename StressType>
  const tfel::math::st2tost2<3u, StressType> IsotropicMoriTanakaScheme(const StressType& young, const real& nu, const real& f, const tfel::math::st2tost2<3u,StressType>& C_i,
        							 const real& a, const real& b, const real& c){
        using namespace tfel::material;
        const auto A = EllipsoidMeanLocalisator<3u,real,StressType>::Isotropic(young, nu, f, C_i, a, b, c);
        return MoriTanakaScheme<real,StressType>(young,nu,f,C_i,A);                                                        
													     };
													     
  template <typename real, typename StressType>
  const tfel::math::st2tost2<3u, StressType> PlanarIsotropicMoriTanakaScheme(const StressType& young, const real& nu, const real& f, const tfel::math::st2tost2<3u,StressType>& C_i,
                                                                       const std::vector<real>& n_a, const real& a, const real& b, const real& c){
        using namespace tfel::material;
        const auto A = EllipsoidMeanLocalisator<3u,real,StressType>::PlanarIsotropic(young, nu, f, C_i, n_a, a, b, c);
        return MoriTanakaScheme<real,StressType>(young,nu,f,C_i,A);
													 					 };
													 					 
  template <typename real, typename StressType,StressType>
  const tfel::math::st2tost2<3u, StressType> OrientedMoriTanakaScheme(const StressType& young, const real& nu, const real& f, const tfel::math::st2tost2<3u,StressType>& C_i,
                                                                      const std::vector<real>& n_a, const real& a, const std::vector<real>& n_b,
                                                                      const real& b, const real& c){
        using namespace tfel::material;
        const auto A = EllipsoidMeanLocalisator<3u,real,StressType>::Oriented(young, nu, f, C_i, n_a, a, n_b, b, c);
        return MoriTanakaScheme<real,StressType>(young,nu,f,C_i,A);
											     };  
											     
			}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_ESHELBYBASEDHOMOGENIZATION_IXX */
