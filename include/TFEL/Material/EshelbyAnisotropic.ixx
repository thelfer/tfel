/*!
 * \file   include/TFEL/Material/EshelbyAnisotropic.ixx
 * \author Antoine Martin
 * \date   18 November 2024
 * \brief  This file defines the Eshelby tensor for an ellipsoidal inclusion embedded in an anisotropic matrix.
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ESHELBYANISOTROPIC_IXX
#define LIB_TFEL_MATERIAL_ESHELBYANISOTROPIC_IXX

#include <cmath>
#include <numbers>
#include <stdexcept>
#include<typeinfo>
#include <boost/math/quadrature/trapezoidal.hpp>

namespace tfel::material
{

namespace homogenization{
     namespace elasticity{
     
	namespace internals{
	
		template<typename real>
		TFEL_HOST_DEVICE auto dblintegrate(const std::function<real(const real&,const real&)> f, const real a, const real b, const real c, const real d){
			const auto f_ = [f,c,d](const real& x){return boost::math::quadrature::trapezoidal<std::function<real(real)>,real>([x,f](const real& y){return f(x,y);},c,d);};
			//const auto f_ = [f,c,d](const real& x){return integrate_on_y<real,F,f,c,d>(x);};
			return boost::math::quadrature::trapezoidal<std::function<real(real)>,real>(f_,a,b);
		};
	
		
		TFEL_HOST_DEVICE int vi(int i, int j){
				if ((i==0) and (j==0)){return 0;}
				else if ((i==1) and (j==1)){return 1;}
				else if ((i==2) and (j==2)){return 2;}
				else if (((i==0) and (j==1))||((i==1) and (j==0))){return 3;}
				else if (((i==0) and (j==2))||((i==2) and (j==0))){return 4;}
				else if (((i==1) and (j==2))||((i==2) and (j==1))){return 5;}
			};
		
		template<typename Type,typename real>
		TFEL_HOST_DEVICE Type getSt4(const tfel::math::st2tost2<3u,Type>& A, int i, int j, int k, int l){
				const int I = vi(i,j);
				const int J = vi(k,l);
				real fac = real(1);
				if (I > 2){fac/=std::sqrt(2);};
				if (J > 2){fac/=std::sqrt(2);};
				return fac*A(I,J);
			};
			
		template<typename Type,typename real>
		TFEL_HOST_DEVICE void setSt4(tfel::math::st2tost2<3u,Type>& A, int i, int j, int k, int l,Type Aijkl){
				const int I = vi(i,j);
				const int J = vi(k,l);
				real fac = real(1);
				if (I > 2){fac*=std::sqrt(2);};
				if (J > 2){fac*=std::sqrt(2);};
				A(I,J)=fac*Aijkl;
				if (I!=J){A(J,I)=A(I,J);};
			};
			
		template<typename Type,typename real>
		TFEL_HOST_DEVICE void setStensor(tfel::math::stensor<3u,Type>& A, int i, int j,Type Aij){
				const int I = vi(i,j);
				real fac = real(1);
				if (I > 2){fac*=std::sqrt(2);};
				A(I)=fac*Aij;
			};
			
		template<typename Type,typename real>
		TFEL_HOST_DEVICE Type getStensor(const tfel::math::stensor<3u,Type>& A, int i, int j){
				const int I = vi(i,j);
				real fac = real(1);
				if (I > 2){fac/=std::sqrt(2);};
				return fac*A(I);
		};
				
		template<typename real,typename StressType>
		TFEL_HOST_DEVICE tfel::math::stensor<3u,real> Acoustic(const tfel::math::st2tost2<3u,StressType>& C, const tfel::math::tvector<3u,real>& X){
			tfel::math::stensor<3u,real> A;
			for(int i=0;i<3;i++)
			for(int k=i;k<3;k++){
				real A_ik = real(0);
				for(int j=0;j<3;j++)
				for(int l=0;l<3;l++){
				A_ik+= real((getSt4<StressType,real>(C,i,j,k,l))/StressType(1)*X[j]*X[l]);
				}
				setStensor<real,real>(A,i,k,A_ik);
			}
			return A;
		};
		
		template<typename real,typename StressType, typename LengthType>
		TFEL_HOST_DEVICE real p_ijkl(const tfel::math::st2tost2<3u,StressType>& C, const real theta, const real phi, const LengthType& a, const LengthType& b, const LengthType& c, const int& i, const int& j, const int& k, const int& l){
			const real pi = std::numbers::pi_v<real>;
			const tfel::math::tvector<3u,real> X={std::sin(theta)*std::cos(phi)/a*LengthType(1),std::sin(theta)*std::sin(phi)/b*LengthType(1),std::cos(theta)/c*LengthType(1)};
			const auto A_inv=tfel::math::invert(Acoustic<real,StressType>(C,X));
			const auto Mijkl=(getStensor<real,real>(A_inv,j,k)*X[i]*X[l]+getStensor<real,real>(A_inv,i,k)*X[j]*X[l]+getStensor<real,real>(A_inv,j,l)*X[i]*X[k]+getStensor<real,real>(A_inv,i,l)*X[j]*X[k])/4;
			return Mijkl*std::sin(theta)/4/pi;	
		};
		
		
	};//end of namespace internals


	template<typename real,typename StressType, typename LengthType>
	TFEL_HOST_DEVICE tfel::math::st2tost2<3u,typename tfel::math::invert_type<StressType>> computeAnisotropicHillTensor(const tfel::math::st2tost2<3u,StressType>& C,
											       const LengthType& a, const LengthType& b, const LengthType& c){
		const std::array<LengthType,3> abc_={a,b,c};
		const auto sig=internals::sortEllipsoidLengths<LengthType>(a,b,c);
		const auto a_=abc_[sig[0]];
		const auto b_=abc_[sig[1]];
		const auto c_=abc_[sig[2]];
		const real pi = std::numbers::pi_v<real>;
		const real zero = real(0);
		using compliance = typename tfel::math::invert_type<StressType>;
		tfel::math::st2tost2<3u,compliance> P;
		for (int i=0;i<3;i++)
		for (int j=i;j<3;j++)
		for (int k=0;k<3;k++)
		for (int l=k;l<3;l++){
			const int I = internals::vi(i,j);
			const int J = internals::vi(k,l);
			if (I<=J){
				const auto p_ = [C,a_,b_,c_,i,j,k,l] (const real& theta, const real& phi){return internals::p_ijkl<real,StressType,LengthType>(C,theta,phi,a_,b_,c_,i,j,k,l);};
				const auto int_p = compliance(internals::dblintegrate<real>(p_,zero,pi,zero,2*pi));
				internals::setSt4<compliance,real>(P,i,j,k,l,int_p);
			}
		}
		return P;
	};
	
	
	template<typename real,typename StressType, typename LengthType>
	TFEL_HOST_DEVICE tfel::math::st2tost2<3u,real> computeAnisotropicEshelbyTensor(const tfel::math::st2tost2<3u,StressType>& C,
											       const LengthType& a, const LengthType& b, const LengthType& c){
		return computeAnisotropicHillTensor<real,StressType,LengthType>(C,a,b,c)*C;
	};
	
        }//end of namespace elasticity
   }//end of namespace homogenization

}//end of tfel::material

#endif /* LIB_TFEL_MATERIAL_ESHELBYANISOTROPIC_IXX */
  	
