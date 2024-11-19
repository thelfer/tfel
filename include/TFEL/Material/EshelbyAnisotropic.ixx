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
		
			const real pi = std::numbers::pi_v<real>;
			
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
				return (6*(z4*z)*a6 + 5*z4*a5 + 4*(z2*z)*a4 + 3*z2*a3 + 2*z*a2 + a1);
			};

			 //Sextic second derivative
			TFEL_HOST_DEVICE std::complex<real> q3dsecond(const real a0, const real a1, const real a2, const real a3, 
					  const real a4, const real a5, const real a6, const std::complex<real>& z){
			  std::complex<real> z2 = z*z;
			  return (30*(z2*z2)*a6 + 20*(z2*z)*a5 + 12*z2*a4 + 6*z*a3 + 2*a2);
			};
			
			// Numerator
			TFEL_HOST_DEVICE std::complex<real> pijkl3do(int IV, int JV, const tfel::math::tvector<3u,real>& n, const tfel::math::tvector<3u,real>& m, const tfel::math::tmatrix<3u,3u,StressType>& Q, const tfel::math::tmatrix<3u,3u,StressType>& S, const tfel::math::tmatrix<3u,3u,StressType>& T, const std::complex<real>& z){
				int i, j, k, l;
				IJ ij, kl;
				ij = VIJ(IV);
				i = ij.ij[0]; j = ij.ij[1]; 
				kl = VIJ(JV);
				k = kl.ij[0]; l = kl.ij[1];
				std::complex<real> x[3], z2=z*z, z2m1 = z2-1.0;
				tfel::math::st2tost2<3u,std::complex<real>> A, adj;
				for (int i0=0;i0<3;i0++){
					x[i0] = n(i0) * z2m1 + m(i0) * (2.0*z);
					for (int j0=0;j0<3;j0++){
						A(i0, j0)= Q(i0,j0)*z2m1*z2m1 + S(i0,j0)*z2m1*(2.0*z) + T(i0,j0)*4.0*z2;
					};
				};
				adj = adjoint3d(A);
				return z * (adj(j,k)*x[i]*x[l] + adj(i,k)*x[j]*x[l] + adj(j,l)*x[i]*x[k] + adj(i,l)*x[j]*x[k]);
			};

			//Numerator orthotropic case
			TFEL_HOST_DEVICE std::complex<real>  pijkl3d(int IV, int JV, const real n3, const real m1, const real m2, 
						 const tfel::math::tmatrix<3u,3u,StressType>& Q, const tfel::math::tmatrix<3u,3u,StressType>& S, const tfel::math::tmatrix<3u,3u,StressType>& T, 
						 const std::complex<real>& z){
				std::complex<real> res = real{0};
				std::complex<double>  x2 = z*z;
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
				if (norm(z)!=0.0){
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
				return leviCivita(i,j,k)*Q1(i,1)*Q2(j,2)*Q3(k,3);
			};
			
			template<tfel::math::st2tost2<3u,StressType> C, LengthType a, LengthType b, LengthType c>
			TFEL_HOST_DEVICE tfel::math::st2tost2<3u,tfel::math::invert_type<StressType>> p_phi(const real y){
				
				const auto a1=real{a};
				const auto a2=real{b};
				const auto a3=real{c};
				bool cas_dege=false, cas_jj = false;
				const auto phi = 2*pi*y;
				normeC=C.norme();
				tfel::math::tmatrix<3u,3u,StressType> Q, S, T;
				//* Calcul des tmatrix<3u,3u,StressType> Q, S et T
				const auto n3 = 1/a3;
				const auto m1  = std::cos(phi)/a1;
				const auto m2 = std::sin(phi)/a2;
				for (int i=0;i<3;i++){
					for (int k=0;k<3;k++){
						Q(i, k)= C((i,2), (k,2))*n3*n3;
						T(i, k)= C((i,0), (k,0))*m1*m1 + (C((i,0), (k,1))+C((i,1), (k,0)))*m1*m2 + C((i,1), (k,1))*m2*m2;
						S(i, k)= C( (i,2),(k,0) )*n3*m1 + C( (i,2),(k,1) )*n3*m2 + C( (k,2),(i,0) )*n3*m1 + C( (k,2),(i,1) )*n3*m2);
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
				if ((fabs(coeffs_qr[1])<normeC*petit) &&(fabs(coeffs_qr[3])<normeC*petit) &&(fabs(coeffs_qr[5])<=normeC*petit)){
				
					//*Résolution equation de degre 3 (Cardan)
					POLESP rootsr;
					degre3(coeffs_qr[0]/coeffs_qr[6], coeffs_qr[2]/coeffs_qr[6], coeffs_qr[4]/coeffs_qr[6],rootsr);
					for (i=0;i<3;i++){
						std::cout<<"\n phi = "<<phi<<", cas orthotrope, racine sextique = +/- "<< std::sqrt(rootsr.zr(i));
						rsextique[2*i] = std::sqrt(rootsr.zr(i));
						rsextique[2*i+1] = - rsextique[2*i];
					}
				}
				//* calcul des racines, cas general
				else{
					real racines[12];
					gsl_poly_complex_workspace * w = gsl_poly_complex_workspace_alloc (7);
					gsl_poly_complex_solve (coeffs_qr, 7, w, racines);
					gsl_poly_complex_workspace_free (w);
					for (i=0;i<6;i++){
						rsextique[i] = racines[2*i]+jj*racines[2*i+1];
					};
				};
				//! Résolution algebrique
				int compteur = 0, isimple=-1, i_jj=-1;
				std::complex<real> ti, zr[3], qt[3], invdenom[3], qp[3], zdouble
				std::complex<real> qi = q3d(coeffs_qr[0],coeffs_qr[1],coeffs_qr[2], coeffs_qr[3],coeffs_qr[4],coeffs_qr[5],coeffs_qr[6],jj);
				for (i=0;i<6;i++){
					ti = rsextique[i];
					if (ti.imag()>0){
						qp[compteur] = q3dprime(coeffs_qr[0],coeffs_qr[1],coeffs_qr[2],coeffs_qr[3],coeffs_qr[4],coeffs_qr[5],coeffs_qr[6],ti);
						if (norm(qp[compteur]/coeffs_qr[0])<1e-8){
							cas_dege = true;
							zdouble = ti;
						}
						else{
							 isimple = compteur;
							 if (norm(ti-jj)<ecart_jj){
								    cas_jj = true;
								    i_jj = compteur;
							 }
						};
						qt[compteur] = std::sqrt(1.0+ti*ti);
						invdenom[compteur] = 1 / (qp[compteur]*qt[compteur]*qt[compteur]*qt[compteur]);
						zr[compteur] = ti;
						compteur++;
					}
					else if (fabs(ti.imag())<1e-8){
						std::cout<<"\n Tenseur P (3D), calcul analytique, phi = "<<phi;
						std::cout<<"\n Pole reel (milieu singulier) = "<<ti<<"\n";
						fin();
					};
					if (compteur==3) break;
				};
				std::cout<<"\n cas-dege = "<<cas_dege<<" "<<false;
				//* Calcul composantes du tenseur P_{phi}
				real tmp;
				std::complex<real> jjsqi = jj/qi;
				st2tost2<3u,tfel::math::invert_type<StressType>> Pphi;
				if (cas_dege){
					//* z1 = z2 = z3
					if (isimple==-1){
						if ((norm(zr[0]-jj)<ecart_jj)||(norm(zr[1]-jj)<ecart_jj)||(norm(zr[2]-jj)<ecart_jj)){
							for (int IV=0;IV<6;IV++){
								for (int JV=IV;JV<6;JV++){
									Pphi(IV, JV)= mijkl3d_dg(IV,JV,n3,m1,m2,Q,S,T)/coeffs_qr[6];
									if (JV>IV) Pphi(JV,IV)=Pphi(IV,JV);
								};
							};
						}
						else{
							std::cout<<"\n Tenseur P (3D), calcul analytique, phi = "<<phi<<"\n";
							std::cout<<"z1="<<zr[0]<<", z2="<<zr[1]<<" , z3="<<zr[2];
							std::cout<<"\n ** cas degenere rencontre, non traite";
							fin();
						};
					}
					else{
						std::cout<<"\n Tenseur P (3D), calcul analytique, phi = "<<phi<<"\n";
						std::cout<<"z1="<<zr[0]<<", z2="<<zr[1]<<" , z3="<<zr[2];
						std::cout<<"\n ** cas degenere rencontre";
						//*  racine de la sextique
						if (norm(zdouble-jj)<1e-8){
							std::cout<<"\n ** (i) racine double de la sextique"<<STOP;
						}
						//* z1 = z2
						else{
							std::complex<real> z3=zr[isimple];
							std::cout<<"\n ** racines doubles"<<STOP;
						};
						fin();
					};
				}
				else{
					std::cout<<"\n cas_jj?"<<cas_jj;
					if (cas_jj){
						int IV,JV;
						real deutier = 2.0/3.0;
						std::complex<real> qhati, qhatprimei;
						qhati = -0.5*jj*q3dprime(coeffs_qr[0],coeffs_qr[1],coeffs_qr[2],coeffs_qr[3],coeffs_qr[4],coeffs_qr[5],coeffs_qr[6],jj); 
						qhatprimei = -0.25*jj*(q3dseconde(coeffs_qr[0],coeffs_qr[1],coeffs_qr[2],coeffs_qr[3],coeffs_qr[4],coeffs_qr[5],coeffs_qr[6],jj)- 2.0*qhati);
						for (IV=0;IV<6;IV++){
							for (JV=IV;JV<6;JV++){
				  				tmp = (deutier*jj*pijkl3d(IV,JV,n3,m1,m2,Q,S,T,jj)/qhati+ 0.5*(pijkl3dprime(IV,JV,n3,m1,m2,Q,S,T,jj)*qhati
				  				-pijkl3d(IV,JV,n3,m1,m2,Q,S,T,jj)*qhatprimei)/(qhati*qhati)).imag();
				  				for (i=0; i<3; i++){
				    					if (i!=i_jj) tmp = tmp - ((2.0*log(zr[i]+qt[i]) - jj*pi)*pijkl3d(IV,JV,n3,m1,m2,Q,S,T,zr[i])*invdenom[i]).real();
				  					Pphi(IV,JV)=tmp/(4.0*pi);
				  					if (JV>IV) Pphi(JV,IV)=Pphi(IV,JV);
								}
							}
						}
					}
					else{
						for (int IV=0;IV<6;IV++){
						for (int JV=IV;JV<6;JV++){
				  			tmp = (jjsqi*pijkl3d(IV,JV,n3,m1,m2,Q,S,T,jj)).imag();
				  			for (i=0; i<3; i++){
				     				tmp = tmp - ((2.0*log(zr[i]+qt[i]) - jj*pi)* pijkl3d(IV,JV,n3,m1,m2,Q,S,T,zr[i])*invdenom[i]).real();
				  			}
				  			Pphi(IV,JV)=tmp/(4.0*pi);
				  			if (JV>IV) Pphi(JV,IV)=Pphi(IV,JV);
						}
						}
					}
				}
				return Pphi;
			};//end of p_phi
		};//end of struct HillIntegration
	};//end of namespace internals


	template <typename real, typename LengthType, typename StressType>
  	TFEL_HOST_DEVICE tfel::math::st2tost2<3u,real> computeAnisotropicEshelbyTensor(
     	const tfel::math::st2tost2<3u,StressType>& C, const LengthType& a, const LengthType& b, const LengthType& c){
    		constexpr auto pi = std::numbers::pi_v<real>;
    		using namespace tfel::math;
    		typedef st2tost2<3u,invert_type<StressType>> (*F)(real);
    		const auto P =  internals::integrate<invert_type<StressType>,real,F>(real{0}, real{1}, internals::HillIntegration<real,StressType,LengthType>::p_phi<C,a,b,c>,10)*pi;
     		return P*C;
     	};

}//end of tfel::material

#endif /* LIB_TFEL_MATERIAL_ESHELBYANISOTROPIC_IXX */
  	
