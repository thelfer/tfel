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

namespace tfel::material
{
	namespace internals{
		
		/*!
		* This function is the Levi-Civita symbol
		* \return an object of type int
		* \param[in] i,j,k: indices
		*/
		TFEL_HOST_DEVICE int leviCivita(int i, int j, int k){
			if ((i==j)||(j==k)||(k==i)){
				return 0;
			}
			else if (((i==0) and (j==1) and (k==2))||((i==1) and (j==2) and (k==0))||((i==2) and (j==0) and (k==1))){
				return 1;
			}
			return -1;
		};//end of leviCivita

		template<typename T, typename real, typename F>
		TFEL_HOST_DEVICE T integrate(const real a, const real b, const F& f, const int nb_pas){
			const auto h=(b-a)/(nb_pas);
			T res;
			res = (f(a)+f(b))*h/2;
			for (int i=1;i<nb_pas;i++)
			res = res + f(a+i*h)*h;
			return res ;
		};//end of integrate
		
		
		
		template<typename real,typename StressType,typename LengthType>
		struct HillIntegration{
		
			
			
			//Sextic
			TFEL_HOST_DEVICE std::complex<real> q3d(const real a0, const real a1, const real a2, const real a3, 
					  const real a4, const real a5, const real a6, const std::complex<real>& z){
				std::complex<real> z2 = z*z, z4=z2*z2;
				return (z4*z2)*a6 + (z4*z)*a5 + z4*a4 + (z2*z)*a3 + z2*a2 + z*a1 + a0;
			};

			//Sextic derivative
			TFEL_HOST_DEVICE std::complex<real> q3dprime(const real a0, const real a1, const real a2, const real a3, 
					  const real a4, const real a5, const real a6, const std::complex<real>& z){
				std::complex<real> z2 = z*z, z4 = z2*z2;
				return 6*(z4*z)*a6 + 5*z4*a5 + 4*(z2*z)*a4 + 3*z2*a3 + 2*z*a2 + a1;
			};

			 //Sextic second derivative
			TFEL_HOST_DEVICE std::complex<real> q3dseconde(const real a0, const real a1, const real a2, const real a3, 
					  const real a4, const real a5, const real a6, const std::complex<real>& z){
			  std::complex<real> z2 = z*z;
			  return 30*(z2*z2)*a6 + 20*(z2*z)*a5 + 12*z2*a4 + 6*z*a3 + 2*a2;
			};
			
			

			//Numerator orthotropic case
			TFEL_HOST_DEVICE std::complex<real>  pijkl3d(int IV, int JV, const real n3, const real m1, const real m2, 
						 const tfel::math::tmatrix<3u,3u,StressType>& Q, const tfel::math::tmatrix<3u,3u,StressType>& S, const tfel::math::tmatrix<3u,3u,StressType>& T, 
						 const std::complex<real>& z){
				std::complex<real> res = real{0};
				std::complex<real>  x2 = z*z;
				if (IV==0){
					if (JV==0) res = 4.0*m1*m1*( Q(1,1)*Q(2,2)*x2*x2+(Q(1,1)*T(2,2)+Q(2,2)*T(1,1)-S(1,2)*S(1,2))*x2+T(1,1)*T(2,2) );
					if (JV==1) res = 4.0*m1*m2*( (S(1,2)*S(0,2)-Q(2,2)*T(0,1))*x2-T(0,1)*T(2,2) );
					if (JV==2) res = 4.0*m1*n3*( -Q(1,1)*S(0,2)*x2*x2+(S(1,2)*T(0,1)-S(0,2)*T(1,1))*x2 );
				}
				if (IV==1){
					if (JV==1) res = 4.0*m2*m2*(Q(0,0)*Q(2,2)*x2*x2+(Q(0,0)*T(2,2)+Q(2,2)*T(0,0)-S(0,2)*S(0,2))*x2+T(0,0)*T(2,2) );
					if (JV==2) res = 4.0*m2*n3*( -Q(0,0)*S(1,2)*x2*x2+(S(0,2)*T(0,1)-S(1,2)*T(0,0))*x2 );
				}
				if (IV==2){
					if (JV==2) res = 4.0*n3*n3*(Q(0,0)*Q(1,1)*x2*x2*x2+(Q(1,1)*T(0,0)+Q(0,0)*T(1,1))*x2*x2+(T(0,0)*T(1,1)-T(0,1)*T(0,1))*x2);
				}
				if ((IV==3)&&(JV==IV)){
					res = ( n3*n3*Q(0,0)*Q(2,2)*x2*x2*x2+(n3*n3*(Q(0,0)*T(2,2)+Q(2,2)*T(0,0)-S(0,2)*S(0,2))-2.0*m2*n3*Q(0,0)*S(1,2)+m2*m2*Q(0,0)*Q(1,1))*x2*x2+(n3*n3*T(0,0)*T(2,2)+2.0*m2*n3*(S(0,2)*T(0,1)-S(1,2)*T(0,0))+m2*m2*(Q(1,1)*T(0,0)+Q(0,0)*T(1,1)))*x2+m2*m2*(T(0,0)*T(1,1)-T(0,1)*T(0,1)) );
				}
				if ((IV==4)&&(JV==IV)){
					res = ( n3*n3*Q(1,1)*Q(2,2)*x2*x2*x2+(n3*n3*(Q(1,1)*T(2,2)+Q(2,2)*T(1,1)-S(1,2)*S(1,2))-2.0*m1*n3*Q(1,1)*S(0,2)+m1*m1*Q(0,0)*Q(1,1))*x2*x2+(n3*n3*T(1,1)*T(2,2)+2.0*m1*n3*(S(1,2)*T(0,1)-S(0,2)*T(1,1))+m1*m1*(Q(1,1)*T(0,0)+Q(0,0)*T(1,1)))*x2+m1*m1*(T(0,0)*T(1,1)-T(0,1)*T(0,1)) );
				}
				if ((IV==5)&&(JV==IV)){
					res = ((m2*m2*Q(1,1)*Q(2,2)+m1*m1*Q(0,0)*Q(2,2))*x2*x2+(m1*m1*(Q(0,0)*T(2,2)+Q(2,2)*T(0,0)-S(0,2)*S(0,2))+m2*m2*(Q(1,1)*T(2,2)+Q(2,2)*T(1,1)-S(1,2)*S(1,2))+2.0*m1*m2*(S(1,2)*S(0,2)-Q(2,2)*T(0,1)))*x2+m1*m1*T(0,0)*T(2,2)+m2*m2*T(1,1)*T(2,2)-2*m1*m2*T(0,1)*T(2,2) );
				}
				return res;
			};

			//case z1=z2=z3=i
			TFEL_HOST_DEVICE real mijkl3d_dg(int IV, int JV, const real n3, const real m1, const real m2, 
					  const tfel::math::tmatrix<3u,3u,StressType>& Q, const tfel::math::tmatrix<3u,3u,StressType>& S, const tfel::math::tmatrix<3u,3u,StressType>& T){
				const real pi = std::numbers::pi_v<real>;
				auto res=real{0};
				if (IV==0){
					if (JV==0) res = m1*m1*( Q(1,1)*Q(2,2)*2/(35*pi)+(Q(1,1)*T(2,2)+Q(2,2)*T(1,1)-S(1,2)*S(1,2)+6.0*T(1,1)*T(2,2))*8/(105*pi));
					if (JV==1) res = m1*m2*( (S(1,2)*S(0,2)-Q(2,2)*T(0,1)-6*T(0,1)*T(2,2))*8/(105*pi) );
					if (JV==2) res = m1*n3*( -Q(1,1)*S(0,2)*2/(35*pi)+(S(1,2)*T(0,1)-S(0,2)*T(1,1))*8.0/(105.0*pi) );
				}
				if (IV==1){
					if (JV==1) res = m2*m2*(Q(0,0)*Q(2,2)*2/(35*pi)+(Q(0,0)*T(2,2)+Q(2,2)*T(0,0)-S(0,2)*S(0,2)+6*T(0,0)*T(2,2))*8/(105*pi) );
					if (JV==2) res = m2*n3*( -Q(0,0)*S(1,2)*2/(35*pi)+(S(0,2)*T(0,1)-S(1,2)*T(0,0))*8/(105*pi) );
				}
				if (IV==2){
					if (JV==2) res = n3*n3*(Q(0,0)*Q(1,1)/(7.0*pi)+(Q(1,1)*T(0,0)+Q(0,0)*T(1,1))*2/(35*pi)+(T(0,0)*T(1,1)-T(0,1)*T(0,1))*8/(105*pi));
				}
				if ((IV==3)&&(JV==IV)){
					res =0.25*( n3*n3*Q(0,0)*Q(2,2)/(7.0*pi)+(n3*n3*(Q(0,0)*T(2,2)+Q(2,2)*T(0,0)-S(0,2)*S(0,2))-2*m2*n3*Q(0,0)*S(1,2)+m2*m2*Q(0,0)*Q(1,1))*2.0/(35*pi)+(n3*n3*T(0,0)*T(2,2)+2.0*m2*n3*(S(0,2)*T(0,1)-S(1,2)*T(0,0))+m2*m2*(Q(1,1)*T(0,0)+Q(0,0)*T(1,1))+6.0*m2*m2*(T(0,0)*T(1,1)-T(0,1)*T(0,1)))*8/(105*pi) );
				}
				if ((IV==4)&&(JV==IV)){
				res =0.25*( n3*n3*Q(1,1)*Q(2,2)/(7*pi)+(n3*n3*(Q(1,1)*T(2,2)+Q(2,2)*T(1,1)-S(1,2)*S(1,2))-2.0*m1*n3*Q(1,1)*S(0,2)+m1*m1*Q(0,0)*Q(1,1))*2.0/(35.0*pi)+(n3*n3*T(1,1)*T(2,2)+2.0*m1*n3*(S(1,2)*T(0,1)-S(0,2)*T(1,1))+m1*m1*(Q(1,1)*T(0,0)+Q(0,0)*T(1,1))+6*m1*m1*(T(0,0)*T(1,1)-T(0,1)*T(0,1)))*8.0/(105*pi) );
				}
				if ((IV==5)&&(JV==IV)){
				res =0.25*((m2*m2*Q(1,1)*Q(2,2)+m1*m1*Q(0,0)*Q(2,2))*2.0/(35.0*pi)+(m1*m1*(Q(0,0)*T(2,2)+Q(2,2)*T(0,0)-S(0,2)*S(0,2))+m2*m2*(Q(1,1)*T(2,2)+Q(2,2)*T(1,1)-S(1,2)*S(1,2))+2.0*m1*m2*(S(1,2)*S(0,2)-Q(2,2)*T(0,1))+6*(m1*m1*T(0,0)*T(2,2)+m2*m2*T(1,1)*T(2,2)-2*m1*m2*T(0,1)*T(2,2)))*8/(105*pi) );
				}
				return res;
			};

			// Numerator derivative, orthotropic case
			TFEL_HOST_DEVICE std::complex<real>  pijkl3dprime(int IV, int JV, const real n3, const real m1, const real m2, const tfel::math::tmatrix<3u,3u,StressType>& Q,
							const tfel::math::tmatrix<3u,3u,StressType>& S, const tfel::math::tmatrix<3u,3u,StressType>& T, const std::complex<real>& z){
				std::complex<real> res=real{0};
				if (norm(z)!=0){
					std::complex<real>  x = z, x2 = x*x;
					if (IV==0){
					if (JV==0) res = 4.0*m1*m1*( 4.0*Q(1,1)*Q(2,2)*x2*x+2.0*(Q(1,1)*T(2,2)+Q(2,2)*T(1,1)-S(1,2)*S(1,2))*x);
					if (JV==1) res = 4.0*m1*m2*( 2.0*(S(1,2)*S(0,2)-Q(2,2)*T(0,1))*x );
					if (JV==2) res = 4.0*m1*n3*( -4.0*Q(1,1)*S(0,2)*x2*x+2.0*(S(1,2)*T(0,1)-S(0,2)*T(1,1))*x );
					}
					if (IV==1){
					if (JV==1) res = 4.0*m2*m2*(4.0*Q(0,0)*Q(2,2)*x2*x+2.0*(Q(0,0)*T(2,2)+Q(2,2)*T(0,0)-S(0,2)*S(0,2))*x );
					if (JV==2) res = 4.0*m2*n3*( -4.0*Q(0,0)*S(1,2)*x2*x+2.0*(S(0,2)*T(0,1)-S(1,2)*T(0,0))*x );
					}
					if (IV==2){
					if (JV==2) res = 4.0*n3*n3*(6.0*Q(0,0)*Q(1,1)*x2*x2*x+4.0*(Q(1,1)*T(0,0)+Q(0,0)*T(1,1))*x2*x+2.0*(T(0,0)*T(1,1)-T(0,1)*T(0,1))*x);
					}
					if ((IV==3)&&(JV==IV)){
						res =( 6.0*n3*n3*Q(0,0)*Q(2,2)*x2*x2*x+4.0*(n3*n3*(Q(0,0)*T(2,2)+Q(2,2)*T(0,0)
						-S(0,2)*S(0,2))-2.0*m2*n3*Q(0,0)*S(1,2)+m2*m2*Q(0,0)*Q(1,1))*x2*x+2.0*(n3*n3*T(0,0)*T(2,2)+2.0*m2*n3*(S(0,2)*T(0,1)-S(1,2)*T(0,0))
						+m2*m2*(Q(1,1)*T(0,0)+Q(0,0)*T(1,1)))*x );
					}
					if ((IV==4)&&(JV==IV)){
						res =( 6.0*n3*n3*Q(1,1)*Q(2,2)*x2*x2*x+4.0*(n3*n3*(Q(1,1)*T(2,2)+Q(2,2)*T(1,1)
						-S(1,2)*S(1,2))-2.0*m1*n3*Q(1,1)*S(0,2)+m1*m1*Q(0,0)*Q(1,1))*x2*x+2.0*(n3*n3*T(1,1)*T(2,2)+2.0*m1*n3*(S(1,2)*T(0,1)-S(0,2)*T(1,1))
						+m1*m1*(Q(1,1)*T(0,0)+Q(0,0)*T(1,1)))*x );
					}
					if ((IV==5)&&(JV==IV)){
						res =(4.0*(m2*m2*Q(1,1)*Q(2,2)+m1*m1*Q(0,0)*Q(2,2))*x2*x+2.0*(m1*m1*(Q(0,0)*T(2,2)+Q(2,2)*T(0,0)-S(0,2)*S(0,2))+m2*m2*(Q(1,1)*T(2,2)
						+Q(2,2)*T(1,1)-S(1,2)*S(1,2))+2.0*m1*m2*(S(1,2)*S(0,2)-Q(2,2)*T(0,1)))*x );
					}
				}
				return res;
			};
			
			TFEL_HOST_DEVICE StressType permute(tfel::math::tmatrix<3u,3u,StressType>& Q1, tfel::math::tmatrix<3u,3u,StressType>& Q2, tfel::math::tmatrix<3u,3u,StressType>& Q3){
				real p = real{0};
				for (int i=0;i<3;i++){
				for (int j=0;j<3;j++){
				for (int k=0;k<3;k++){
					p+=leviCivita(i,j,k)*Q1(i,1)*Q2(j,2)*Q3(k,3);
				};};};
				return p;
			};
			
			TFEL_HOST_DEVICE int vi(int i, int j){
				if ((i==0) and (j==0)){return 0;}
				else if ((i==1) and (j==1)){return 1;}
				else if ((i==2) and (j==2)){return 2;}
				else if (((i==0) and (j==1))||((i==1) and (j==0))){return 3;}
				else if (((i==0) and (j==2))||((i==2) and (j==0))){return 4;}
				else if (((i==1) and (j==2))||((i==2) and (j==1))){return 5;}
			};
			
			TFEL_HOST_DEVICE real vn(tfel::math::st2tost2<3u,real>& C, int i, int j, int k, int l){
				const int I = vi(i,j);
				const int J = vi(k,l);
				real fac = 1;
				if (I > 2){fac/=std::sqrt(2);};
				if (J > 2){fac/=std::sqrt(2);};
				return fac*C(I,J);
			};
			
			template<tfel::math::st2tost2<3u,StressType> C, LengthType a, LengthType b, LengthType c>
			TFEL_HOST_DEVICE static tfel::math::st2tost2<3u,tfel::math::invert_type<StressType>> p_phi(const real y){
				std::complex<real> jj = 1i;
				const auto a1=real{a};
				const auto a2=real{b};
				const auto a3=real{c};
				bool cas_dege=false, cas_jj = false;
				const real pi = std::numbers::pi_v<real>;
				const auto phi = 2*pi*y;
				const real normeC=C.norme();
				tfel::math::tmatrix<3u,3u,StressType> Q, S, T;
				//* Calcul des tmatrix<3u,3u,StressType> Q, S et T
				const auto n3=1/a3;
				const auto m1=std::cos(phi)/a1;
				const auto m2=std::sin(phi)/a2;
				int i1, k1, i2, k2, i3, k3;
				for (int i=0;i<3;i++){
					for (int k=0;k<3;k++){
						Q(i, k)= vn(C,i,2,k,2)*n3*n3;
						T(i, k)= vn(C,i,0,k,0)*m1*m1 + (vn(C,i,0,k,1)+vn(C,i,1,k,0))*m1*m2 + vn(C,i,1,k,1)*m2*m2;
						S(i, k)= vn(C,i,2,k,0)*n3*m1 + vn(C,i,2,k,1)*n3*m2 + vn(C,k,2,i,0)*n3*m1 + vn(C,k,2,i,1)*n3*m2;
					};
				};
				//! Calcul des racines de la sextique avec la GSL
				real coeffs_qr[7] = {permute(T,T,T),                                                                                                    //d0
						 permute(S,T,T)+permute(T,S,T)+permute(T,T,S),                                                            //d1
						 permute(Q,T,T)+permute(S,T,S)+permute(S,S,T)+permute(T,T,Q)+permute(T,Q,T)+permute(T,S,S),//d2
						 permute(Q,S,T)+permute(Q,T,S)+permute(S,S,S)
						 + permute(S,T,Q)+permute(S,Q,T)+permute(T,S,Q)+permute(T,Q,S),                                      //d3
						 permute(Q,S,S)+permute(Q,T,Q)+permute(Q,Q,T)+permute(S,S,Q)+permute(S,Q,S)+permute(T,Q,Q),//d4
						 permute(Q,S,Q)+permute(Q,Q,S)+permute(S,Q,Q),                                                            //d5
						 permute(Q,Q,Q)};                                                                                                   //d6        
				
				std::complex<real> rsextique[6];
				
				//* calcul des racines, cas orthotrope
				static constexpr auto eps = std::numeric_limits<real>::epsilon();
				//if ((std::abs(coeffs_qr[1])<normeC*eps) &&(std::abs(coeffs_qr[3])<normeC*eps) &&(std::abs(coeffs_qr[5])<=normeC*eps)){
				//
				//	//*Résolution equation de degre 3 (Cardan)
				//	POLESP rootsr;
				//	degre3(coeffs_qr[0]/coeffs_qr[6], coeffs_qr[2]/coeffs_qr[6], coeffs_qr[4]/coeffs_qr[6],rootsr);
				//	for (int i=0;i<3;i++){
				//		std::cout<<"\n phi = "<<phi<<", cas orthotrope, racine sextique = +/- "<< std::sqrt(rootsr.zr(i));
				//		rsextique[2*i] = std::sqrt(rootsr.zr(i));
				//		rsextique[2*i+1] = - rsextique[2*i];
				//	}
				//}
				//* calcul des racines, cas general
				//else{
					//real racines[12];
					//gsl_poly_complex_workspace * w = gsl_poly_complex_workspace_alloc (7);
					//gsl_poly_complex_solve (coeffs_qr, 7, w, racines);
					//gsl_poly_complex_workspace_free (w);
					//for (int i=0;i<6;i++){
					//	rsextique[i] = racines[2*i]+jj*racines[2*i+1];
					//};
				//};
				//! Résolution algebrique
				int cpt = 0, isimple=-1, i_jj=-1;
				const real ecart_jj = real{1e-5};
				std::complex<real> ti, zr[3], qt[3], invdenom[3], qp[3], zdouble;
				std::complex<real> qi = q3d(coeffs_qr[0],coeffs_qr[1],coeffs_qr[2], coeffs_qr[3],coeffs_qr[4],coeffs_qr[5],coeffs_qr[6],jj);
				while (cpt<3){
					int i=0;
					ti = rsextique[i];
					if (ti.imag()>0){
						qp[cpt] = q3dprime(coeffs_qr[0],coeffs_qr[1],coeffs_qr[2],coeffs_qr[3],coeffs_qr[4],coeffs_qr[5],coeffs_qr[6],ti);
						if (norm(qp[cpt]/coeffs_qr[0])<eps){
							cas_dege = true;
							zdouble = ti;
						}
						else{
							 isimple = cpt;
							 if (norm(ti-jj)<ecart_jj){
								    cas_jj = true;
								    i_jj = cpt;
							 }
						};
						qt[cpt] = std::sqrt(1+ti*ti);
						invdenom[cpt] = 1 / (qp[cpt]*qt[cpt]*qt[cpt]*qt[cpt]);
						zr[cpt] = ti;
						cpt++;
					}
					else if (std::abs(ti.imag())<eps){
						std::cout<<"\n Tenseur P (3D), calcul analytique, phi = "<<phi;
						std::cout<<"\n Pole reel (milieu singulier) = "<<ti<<"\n";
						tfel::reportContractViolation("Tenseur P (3D), calcul analytique, phi = ");
					};
					i++;
					if (i==6){cpt=3;};	
				};
				std::cout<<"\n cas-dege = "<<cas_dege<<" "<<false;
				//* Calcul composantes du tenseur P_{phi}
				real tmp;
				std::complex<real> jjsqi = jj/qi;
				using compliance = tfel::math::invert_type<StressType>;
				tfel::math::st2tost2<3u,compliance> Pphi;
				if (cas_dege){
					//* z1 = z2 = z3
					if (isimple==-1){
						if ((norm(zr[0]-jj)<ecart_jj)||(norm(zr[1]-jj)<ecart_jj)||(norm(zr[2]-jj)<ecart_jj)){
							for (int IV=0;IV<6;IV++){
								for (int JV=IV;JV<6;JV++){
									Pphi(IV, JV)= compliance{(mijkl3d_dg(IV,JV,n3,m1,m2,Q,S,T)/coeffs_qr[6])};
									if (JV>IV) Pphi(JV,IV)=Pphi(IV,JV);
								};
							};
						}
						else{
							std::cout<<"\n Tenseur P (3D), calcul analytique, phi = "<<phi<<"\n";
							std::cout<<"z1="<<zr[0]<<", z2="<<zr[1]<<" , z3="<<zr[2];
							tfel::reportContractViolation("cas degenere rencontre, non traite");
						};
					}
					else{
						std::cout<<"\n Tenseur P (3D), calcul analytique, phi = "<<phi<<"\n";
						std::cout<<"z1="<<zr[0]<<", z2="<<zr[1]<<" , z3="<<zr[2];
						std::cout<<"\n ** cas degenere rencontre";
						//*  racine de la sextique
						if (norm(zdouble-jj)<eps){
							std::cout<<"\n ** (i) racine double de la sextique";
						}
						//* z1 = z2
						else{
							std::complex<real> z3=zr[isimple];
							std::cout<<"\n ** racines doubles";
						};
						tfel::reportContractViolation("racine double de la sextique");
					};
				}
				else{
					std::cout<<"\n cas_jj?"<<cas_jj;
					if (cas_jj){
						int IV,JV;
						real deutier = real{2}/3;
						std::complex<real> qhati, qhatprimei;
						qhati = -0.5*jj*q3dprime(coeffs_qr[0],coeffs_qr[1],coeffs_qr[2],coeffs_qr[3],coeffs_qr[4],coeffs_qr[5],coeffs_qr[6],jj); 
						qhatprimei = -0.25*jj*(q3dseconde(coeffs_qr[0],coeffs_qr[1],coeffs_qr[2],coeffs_qr[3],coeffs_qr[4],coeffs_qr[5],coeffs_qr[6],jj)- 2.0*qhati);
						for (IV=0;IV<6;IV++){
							for (JV=IV;JV<6;JV++){
				  				tmp = (deutier*jj*pijkl3d(IV,JV,n3,m1,m2,Q,S,T,jj)/qhati+ 0.5*(pijkl3dprime(IV,JV,n3,m1,m2,Q,S,T,jj)*qhati
				  				-pijkl3d(IV,JV,n3,m1,m2,Q,S,T,jj)*qhatprimei)/(qhati*qhati)).imag();
				  				for (int i=0; i<3; i++){
				    					if (i!=i_jj) tmp = tmp - ((2.0*std::log(zr[i]+qt[i]) - jj*pi)*pijkl3d(IV,JV,n3,m1,m2,Q,S,T,zr[i])*invdenom[i]).real();
				  					Pphi(IV,JV)=compliance{(tmp/4/pi)};
				  					if (JV>IV) Pphi(JV,IV)=Pphi(IV,JV);
								}
							}
						}
					}
					else{
						for (int IV=0;IV<6;IV++){
						for (int JV=IV;JV<6;JV++){
				  			tmp = (jjsqi*pijkl3d(IV,JV,n3,m1,m2,Q,S,T,jj)).imag();
				  			for (int i=0; i<3; i++){
				     				tmp = tmp - ((2.0*std::log(zr[i]+qt[i]) - jj*pi)* pijkl3d(IV,JV,n3,m1,m2,Q,S,T,zr[i])*invdenom[i]).real();
				  			}
				  			Pphi(IV,JV)=compliance{(tmp/4/pi)};
				  			if (JV>IV) Pphi(JV,IV)=Pphi(IV,JV);
						}
						}
					}
				}
				return Pphi;
			};//end of p_phi
			
			template<tfel::math::st2tost2<3u,StressType> C, LengthType a, LengthType b, LengthType c,int i, int j>
			TFEL_HOST_DEVICE static tfel::math::invert_type<StressType> p_phi_ij(const real y){
				return p_phi<C,a,b,c>(y)(i,j);
			};
		};//end of struct HillIntegration
	};//end of namespace internals


	template <typename real, typename LengthType, typename StressType>
  	TFEL_HOST_DEVICE static tfel::math::st2tost2<3u,real> computeAnisotropicEshelbyTensor(
     	const tfel::math::st2tost2<3u,StressType>& C, const LengthType& a, const LengthType& b, const LengthType& c){
    		constexpr auto pi = std::numbers::pi_v<real>;
    		using namespace tfel::math;
    		typedef invert_type<StressType> (*F)(real);
    		int i =0 ;
    		int j =0 ;
    		const tfel::math::invert_type<StressType> value = internals::HillIntegration<real,StressType,LengthType>::p_phi_ij<C,a,b,c,i,j>(real{0});
    		std::cout<<value <<'\n';
    		//const auto P_phi = internals::HillIntegration<real,StressType,LengthType>::p_phi_ij<C,a,b,c,0,0>;
    		//const auto P =  internals::integrate<invert_type<StressType>,real,F>(real{0}, real{1}, P_phi, 10)*pi;
     		//return P*C;
     	};

}//end of tfel::material

#endif /* LIB_TFEL_MATERIAL_ESHELBYANISOTROPIC_IXX */
  	
