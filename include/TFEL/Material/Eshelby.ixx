/*!
 * \file   include/TFEL/Material/Eshelby.ixx
 * \author Antoine Martin
 * \date   15 October 2024
 * \brief  This file defines the Eshelby tensor for an ellipsoidal inclusion embedded in an isotropic matrix.
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ESHELBY_IXX
#define LIB_TFEL_MATERIAL_ESHELBY_IXX

#include <cmath>

namespace tfel::material
{
    /*!
     * \brief a structure in charge of computing the Eshelby tensor
     * \tparam real: numeric type
     */
	template <unsigned short N,typename real>
	struct ComputeEshelbyTensor
	{
		/* Just Eshelby tensor of a sphere
		*/
	      	TFEL_HOST_DEVICE static const tfel::math::st2tost2<3u, real>
	      	EshelbyTensorSphere(const real& nu)
	      	{
	      		const auto zero = real{0};
		      	const auto J=(5*nu-1)/15/(1-nu);
			const auto I=2*(4-5*nu)/15/(1-nu);
			return {I+J, J, J, 	  zero, zero, zero,
				J, I+J, J, 	  zero, zero, zero,
				J, J, I+J, 	  zero, zero, zero,
				zero, zero, zero, I,  zero, zero,
				zero, zero, zero, zero, I,  zero,
				zero, zero, zero, zero, zero, I};
		};//end EshelbyTensorSphere
		
		static constexpr auto eps = std::numeric_limits<real>::epsilon();
		
		/*
		* This function q_ is needed just for the axisymetric ellipsoid
		*/
	      	TFEL_HOST_DEVICE static real q_(const real& e) {if (e<1-eps) return e/tfel::math::power<3,2>(1-e*e)*(acos(e)-e*sqrt(1-e*e)); else if (e>1+eps) return e/tfel::math::power<3,2>(e*e-1)*(e*sqrt(e*e-1)-acosh(e)); else return 2./3;};
		
		/* EshelbyTensorAxisym is relative to an axisymetric ellipsoid which has two equal semi-axes and another semi-axis which is different
		* The arguments are nu and the aspect ratio e of the ellipsoid (e>1 : prolate, e<1 : oblate)
		* The function returns the Eshelby tensor in the basis (e1,e2,e3) where e3 is aligned with the axis of revolution of the ellipsoid
		* The formulae come from Torquato, Random Heterogeneous Materials, 2002.
		*/
		TFEL_HOST_DEVICE static const tfel::math::st2tost2<3u, real>
		EshelbyTensorAxisym(const real& nu, const real& e)
		{
			// if (abs(e-1)<value(eps)){
			//	the formulae must be adapted in the limit case e->1
			//			   }
			const auto zero = real{0};
			const auto e2=e*e;
			const auto e21=e2-1;
			const auto u2nu=1-2*nu;
			const auto q=q_(e);
			const auto S11 = 3./(8*(1-nu))*e2/e21+q/4/(1-nu)*(u2nu-9./4/e21);
			const auto S12= 1./4/(1-nu)*(e2/2/e21-q*(u2nu+3./4/e21));
			const auto S13= 1./2/(1-nu)*(-e2/e21+q/2*(3.*e2/e21-u2nu));
			const auto S31= 1./2/(1-nu)*(-u2nu-1./e21+q*(u2nu+3./2/e21));
			const auto S33= 1./2/(1-nu)*(u2nu+(3.*e2-1.)/e21-q*(u2nu+3.*e2/e21));
			const auto S44= 2./4/(1-nu)*(e2/2/e21+q*(u2nu-3./4/e21));
			const auto S55= 2./4/(1-nu)*(u2nu-(e2+1.)/e21-q/2*(u2nu-3.*(e2+1)/e21));
			return {S11, S12, S13, 	  zero, zero, zero,
				S12, S11, S13, 	  zero, zero, zero,
				S31, S31, S33, 	  zero, zero, zero,
				zero, zero, zero, S44,  zero, zero,
				zero, zero, zero, zero, S55,  zero,
				zero, zero, zero, zero, zero, S55};
		};//end EshelbyTensorAxisym
		
		/* EshelbyTensorGeneral is relative to a general ellipsoid which has three different semi-axes
		* The arguments are nu and the three semi-axes, assumed to be ordered from the biggest to the smallest : a>b>c
		* The function returns the Eshelby tensor in the basis (e1,e2,e3) where e1 (resp. e2, e3) is aligned with axis whose length is a (resp. b, c)
		* The formulae come from Eshelby, The determination of the elastic field..., 1957.
		*/    						  
		TFEL_HOST_DEVICE static const tfel::math::st2tost2<3u, real>
		EshelbyTensorGeneral(const real& nu, const real& a, const real& b, const real& c)
		{
			// if (abs(a-b)<value(eps)){
			//	the formulae must be adapted in the limit case a->b
			//			   }
			// if (abs(b-c)<value(eps)){
			//	the formulae must be adapted in the limit case b->c
			//			   }
			// if (abs(a-c)<value(eps)){
			//	the formulae must be adapted in the limit case a->b->c
			//			   }
			// Il faudrait vérifier les formules d'Eshelby ci-dessous
			const auto a2=a*a;
		    	const auto b2=b*b;
		    	const auto c2=c*c;
		    	const auto Q = 3./8/M_PI/(1-nu);
		    	const auto R = (1.-2*nu)/8/M_PI/(1-nu);
		    	const auto k = sqrt(a2-b2)/sqrt(a2-c2);
		    	const auto theta = asin(sqrt(1-c2/a2));
		    	const auto F = std::ellint_1(k,theta);
		    	const auto E = std::ellint_2(k,theta);
		    	
		    	const auto Ia = 4.*M_PI*a*b*c/(a2-b2)/sqrt(a2-c2)*(F-E);
		    	const auto Ic = 4.*M_PI*a*b*c/(b2-c2)/sqrt(a2-c2)*(b*sqrt(a2-c2)/a/c-E);
		    	const auto Ib = 4.*M_PI-Ia-Ic;
			const auto Iab = (Ib-Ia)/3/(a2-b2);
			const auto Iac = (4.*M_PI/3-Ia-(a2-b2)*Iab)/(a2-c2);
			const auto Ibc = (4.*M_PI/3-Ic-(c2-a2)*Iac)/(c2-b2);
			const auto Iaa = 4.*M_PI/3/a2-Iab-Iac;
			const auto Ibb = 4.*M_PI/3/b2-Iab-Ibc;
			const auto Icc = 4.*M_PI/3/c2-Ibc-Iac;
			
			const auto S11 = Q*a2*Iaa+R*Ia;
			const auto S12 = Q*b2*Iab-R*Ia;
			const auto S44 = 2.*Q/2*(a2+b2)*Iab+R/2*(Ia+Ib);
			const auto S22 = Q*b2*Ibb+R*Ib;
			const auto S21 = Q*a2*Iab-R*Ib;
			const auto S13 = Q*c2*Iac-R*Ia;
			const auto S31 = Q*a2*Iac-R*Ic;
			const auto S33 = Q*c2*Icc+R*Ic;
			const auto S23 = Q*c2*Ibc-R*Ib;
			const auto S32 = Q*b2*Ibc-R*Ic;
			const auto S55 = 2.*Q/2*(a2+c2)*Iac+R/2*(Ia+Ic);
			const auto S66 = 2.*Q/2*(b2+c2)*Ibc+R/2*(Ib+Ic);
			
			const auto zero = real{0};
			return {S11, S12, S13, 	  zero, zero, zero,
				S21, S22, S23, 	  zero, zero, zero,
				S31, S32, S33, 	  zero, zero, zero,
				zero, zero, zero, S44,  zero, zero,
				zero, zero, zero, zero, S55,  zero,
				zero, zero, zero, zero, zero, S66};
		};//end EshelbyTensorGeneral
         
  	}; //end of struct ComputeEshelbyTensor ;
  	
  	
  /*
  * A simple function that takes a,b,c and returns the indices of the values (a,b,c) sorted from the biggest to the smallest 
  */
  template <typename real>
  const std::array<int,3> sort_ind(const real& a, const real& b, const real& c)
  {
	if ((a>b) and (a>c))
	{
		if (b>c) return {0,1,2};
		else return {0,2,1};
	}
	else if ((b>a) and (b>c))
	{
		if (a>c) return {1,0,2};
		else return {1,2,0};
	}
	else
	{
		if (a>b) return {2,0,1};
		else return {2,1,0}; 
    	};
  };//end of sort_ind

 /*
  * A simple function that takes two vectors and returns the vectorial product
  */
  template <typename real>
  const std::vector<real> VectProd(const std::vector<real>& u, const std::vector<real>& v)
  {return {u[1]*v[2]-u[2]*v[1],u[2]*v[0]-u[0]*v[2],u[0]*v[1]-u[1]*v[0]};};//end of VectProd
    	     						     		    		  
  //The function EshelbyTensor (see .hxx)
  template <typename real>
  const tfel::math::st2tost2<3u, real> EshelbyTensor(const real& nu, const real& a, const real& b, const real& c)
  {
  	static constexpr auto eps = std::numeric_limits<real>::epsilon();
    	if ((std::abs(a-b)<eps) and (std::abs(b-c)<eps) and (std::abs(a-c)<eps))
    	{return tfel::material::ComputeEshelbyTensor<3u, real>::EshelbyTensorSphere(nu);}
    	else if ((std::abs(a-b)>eps) and (std::abs(b-c)<eps))
    	{
    		const auto EshBase=tfel::material::ComputeEshelbyTensor<3u, real>::EshelbyTensorAxisym(nu,a/b);
    		return EshBase;
    	}
    	else if ((std::abs(b-a)>eps) and (std::abs(a-c)<eps))
    	{
    		const auto EshBase=tfel::material::ComputeEshelbyTensor<3u, real>::EshelbyTensorAxisym(nu,b/a);
    		return EshBase;
    	}
    	else if ((std::abs(c-b)>eps) and (std::abs(a-b)<eps))
    	{
    		const auto EshBase=tfel::material::ComputeEshelbyTensor<3u, real>::EshelbyTensorAxisym(nu,c/b);
    		return EshBase;
    	}
    	else
    	{	
    		const std::array<real,3> abc_={a,b,c};
    		const auto sig=sort_ind<real>(a,b,c);
    		const auto EshBase=tfel::material::ComputeEshelbyTensor<3u, real>::EshelbyTensorGeneral(nu,abc_[sig[0]],abc_[sig[1]],abc_[sig[2]]);
    		return EshBase;
    	}
  };//end of function EshelbyTensor;
  
    														   
  template <typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,real> SphereLocalisationTensor(const StressType& young, const real& nu,
  	const tfel::math::st2tost2<3u,StressType>& C_i)
  {
	const auto S0 = tfel::material::ComputeEshelbyTensor<3u, real>::EshelbyTensorSphere(nu);
	tfel::math::st2tost2<3u,StressType> C_0;
	using namespace tfel::material;
	static constexpr auto value = StiffnessTensorAlterationCharacteristic::UNALTERED;
	computeIsotropicStiffnessTensorII<3u,value,StressType,real>(C_0,young,nu);
	using namespace tfel::math;
	st2tost2<3u,StressType> C = C_i-C_0;
	const auto invC0 = invert(C_0);
	const auto Pr = invC0 * C;
	const auto PPr = S0 * Pr;
	const auto A = invert(st2tost2<3u,real>::Id()+PPr);
	return A;
  };//end of function SphereLocalisationTensor
     			 			     			 
   template <typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,real> GeneralEllipsoidLocalisationTensor(const StressType& young, const real& nu, const tfel::math::st2tost2<3u,StressType>& C_i,
        const std::vector<real>& n_a, const real& a, const std::vector<real>& n_b, const real& b, const real& c)
   {
    	const auto n_c=tfel::material::VectProd<real>(n_a,n_b);
    	const std::array<real,3> abc_={a,b,c};
	const auto sig=sort_ind(a,b,c);
	const auto S0 = tfel::material::ComputeEshelbyTensor<3u, real>::EshelbyTensorGeneral(nu,abc_[sig[0]],abc_[sig[1]],abc_[sig[2]]);
	const std::array<std::vector<real>,3> nabc_={n_a,n_b,n_c};
	const auto n_1=nabc_[sig[0]];
	const auto n_2=nabc_[sig[1]];
	using namespace tfel::material;
	const auto n_3=VectProd<real>(n_1,n_2);
	const tfel::math::rotation_matrix<real> r={n_1[0],n_1[1],n_1[2],n_2[0],n_2[1],n_2[2],n_3[0],n_3[1],n_3[2]};
	const auto S0_basis = change_basis(S0,r);
	tfel::math::st2tost2<3u,StressType> C_0;
	using namespace tfel::material;
	static constexpr auto value = StiffnessTensorAlterationCharacteristic::UNALTERED;
	computeIsotropicStiffnessTensorII<3u,value,StressType,real>(C_0,young,nu);
	using namespace tfel::math;
	st2tost2<3u,StressType> C = C_i-C_0;
	const auto invC0 = invert(C_0);
	const auto Pr = typename Expr<st2tost2<3u,real>, ST2toST2ST2toST2ProductExpr<3u>>::Expr(invC0,C);
	const auto PPr = typename Expr<st2tost2<3u,real>, ST2toST2ST2toST2ProductExpr<3u>>::Expr(S0_basis,Pr);
	const auto A = invert(st2tost2<3u,real>::Id()+PPr);
	return A;
	
   };//end of function EllipsoidLocalisationTensor
      
      			 			     			 
   template <typename real, typename StressType>
  TFEL_HOST_DEVICE const tfel::math::st2tost2<3u,real> AxisymEllipsoidLocalisationTensor(const StressType& young, const real& nu, const tfel::math::st2tost2<3u,StressType>& C_i,
        const std::vector<real>& n_a, const real& e)
   {
	std::vector<real> n_;
	if ((n_a[1]!=0.)||(n_a[2]!=0.)){n_ = {1.,0.,0.};}
	else {n_ = {0.,1.,0.}; };
	using namespace tfel::material;
	const auto n_2 = VectProd<real>(n_a,n_);
	const auto n_1=VectProd<real>(n_2,n_a);
	const auto S0 = tfel::material::ComputeEshelbyTensor<3u, real>::EshelbyTensorAxisym(nu,e);
	const tfel::math::rotation_matrix<real> r={n_1[0],n_1[1],n_1[2],n_2[0],n_2[1],n_2[2],n_a[0],n_a[1],n_a[2]};
	const auto S0_basis = change_basis(S0,r);
	tfel::math::st2tost2<3u,StressType> C_0;
	using namespace tfel::material;
	static constexpr auto value = StiffnessTensorAlterationCharacteristic::UNALTERED;
	computeIsotropicStiffnessTensorII<3u,value,StressType,real>(C_0,young,nu);
	using namespace tfel::math;
	st2tost2<3u,StressType> C = C_i-C_0;
	const auto invC0 = invert(C_0);
	const auto Pr = typename Expr<st2tost2<3u,real>, ST2toST2ST2toST2ProductExpr<3u>>::Expr(invC0,C);
	const auto PPr = typename Expr<st2tost2<3u,real>, ST2toST2ST2toST2ProductExpr<3u>>::Expr(S0_basis,Pr);
	const auto A = invert(st2tost2<3u,real>::Id()+PPr);
	return A;
   };//end of function AxisymEllipsoidLocalisationTensor
   
}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_ESHELBY_IXX */
