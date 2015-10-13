/*!
 * \file   mfront/mtest/CastemSmallStrainBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cmath>
#include<algorithm>

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/Castem/Castem.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/Castem/CastemComputeStiffnessTensor.hxx"

#include"MTest/Evolution.hxx"
#include"MTest/CastemSmallStrainBehaviour.hxx"
#include"MTest/UmatNormaliseTangentOperator.hxx"

namespace mtest
{

  struct MTestCastemRotationMatrix2D
  {
    MTestCastemRotationMatrix2D(const real *const,
			      const real *const);
    // compute the stiffness matrix in the global space
    void rotateStiffnessMatrixBackward(real *const) const;
  private:
    real MN[3][3];
  }; // end of struct MTestCastemRotationMatrix2D
  
  struct MTestCastemRotationMatrix3D
  {
    
    MTestCastemRotationMatrix3D(const real *const,
			      const real *const);
    // compute the stiffness matrix in the global space
    void rotateStiffnessMatrixBackward(real *const) const;
  private:
    real MN[6][6];
  }; // end of struct MTestCastemRotationMatrix3D

  // Constructeur
  // drot : Matrice de passage élement/global
  MTestCastemRotationMatrix2D::MTestCastemRotationMatrix2D(const real *const V,
						       const real *const drot)
  {
    // Matrice de passage matériau/global
    real a[4];
    // // Premier vecteur
    // a[1,1]
    a[0]=drot[0]*V[0]+drot[3]*V[1];
    // a[2,1]
    a[1]=drot[1]*V[0]+drot[4]*V[1];
    // Deuxième vecteur :
    //   Produit vectoriel
    // a[1,2]
    a[2]=-a[1];
    // a[2,2]
    a[3]=a[0];
    // Contruction de la matrice de passage N (pour les tenseurs)
    // Première ligne
    MN[0][0]=a[0]*a[0];
    MN[0][1]=a[1]*a[1];
    MN[0][2]=a[0]*a[1];
    // Deuxième ligne
    MN[1][0]=a[2]*a[2];
    MN[1][1]=a[3]*a[3];
    MN[1][2]=a[2]*a[3];
    // Troisième ligne
    MN[2][0]=a[0]*a[2];
    MN[2][1]=a[1]*a[3];
    MN[2][2]=a[0]*a[3]+a[1]*a[2];
  } // end of MTestCastemRotationMatrix2D::MTestCastemRotationMatrix2D

  // Calcul de la déformation dans le repère global
  // D^g=tN:D^m:N
  void
  MTestCastemRotationMatrix2D::rotateStiffnessMatrixBackward(real *const D) const
  {
    // matrice N
    real N[4][4];
    for(unsigned short i=0;i!=2;++i){
      for(unsigned short j=0;j!=2;++j){
	N[i][j] = MN[i][j];
      }
    }
    N[2][0]  = 0.;
    N[2][1]  = 0.;
    N[2][3]  = 0.;
    N[0][2]  = 0.;
    N[1][2]  = 0.;
    N[3][2]  = 0.;
    N[2][2]  = 1.;
    for(unsigned short i=0;i!=2;++i){
      N[3][i] = MN[2][i];
      N[i][3] = MN[i][2];
    }
    N[3][3]  = MN[2][2];
    N[3][0] *= real(2.);
    N[3][1] *= real(2.);
    N[3][2] *= real(2.);
    // matrice temporaire
    using namespace std;
    real t[4][4];
    for(unsigned short i=0;i!=4;++i){
      for(unsigned short j=0;j!=4;++j){
	t[i][j] = 0.;
	for(unsigned short k=0;k!=4;++k){
	  t[i][j] += D[k*4+i]*(N[k][j]);
	}
      }
    }
    // matrice finale
    for(unsigned short i=0;i!=4;++i){
      for(unsigned short j=0;j!=4;++j){
	D[j*4+i] = 0.;
	for(unsigned short k=0;k!=4;++k){
	  D[j*4+i] += N[k][i]*t[k][j];
	}
      }
    }
  } // end of MTestCastemRotationMatrix2D::rotateStiffnessMatrixBackward

  // Constructeur
  // drot : Matrice de passage élement/global
  MTestCastemRotationMatrix3D::MTestCastemRotationMatrix3D(const real *const V,
						       const real *const drot)
  {
    // Matrice de passage matériau/global
    real a[9];
    
    // Premier vecteur
    a[0]=drot[0]*V[0]+drot[3]*V[1]+drot[6]*V[2];
    a[1]=drot[1]*V[0]+drot[4]*V[1]+drot[7]*V[2];
    a[2]=drot[2]*V[0]+drot[5]*V[1]+drot[8]*V[2];
    // Deuxième vecteur
    a[3]=drot[0]*V[3]+drot[3]*V[4]+drot[6]*V[5];
    a[4]=drot[1]*V[3]+drot[4]*V[4]+drot[7]*V[5];
    a[5]=drot[2]*V[3]+drot[5]*V[4]+drot[8]*V[5];
    // Troisième vecteur :
    //   produit vectoriel des deux premiers
    a[6]=a[1]*a[5]-a[4]*a[2];
    a[7]=a[2]*a[3]-a[5]*a[0];
    a[8]=a[0]*a[4]-a[3]*a[1];
    // Contruction de la matrice de passage N (pour les tenseurs)
    // Première ligne
    MN[0][0]=a[0]*a[0];
    MN[0][1]=a[1]*a[1];
    MN[0][2]=a[2]*a[2];
    MN[0][5]=a[1]*a[2];
    MN[0][4]=a[2]*a[0];
    MN[0][3]=a[0]*a[1];
    // Deuxième ligne
    MN[1][0]=a[3]*a[3];
    MN[1][1]=a[4]*a[4];
    MN[1][2]=a[5]*a[5];
    MN[1][5]=a[4]*a[5];
    MN[1][4]=a[5]*a[3];
    MN[1][3]=a[3]*a[4];
    // Troisième ligne
    MN[2][0]=a[6]*a[6];
    MN[2][1]=a[7]*a[7];
    MN[2][2]=a[8]*a[8];
    MN[2][5]=a[7]*a[8];
    MN[2][4]=a[8]*a[6];
    MN[2][3]=a[6]*a[7];
    // Quatrième ligne
    MN[5][0]=a[3]*a[6];
    MN[5][1]=a[4]*a[7];
    MN[5][2]=a[5]*a[8];
    MN[5][5]=a[4]*a[8]+a[5]*a[7];
    MN[5][4]=a[5]*a[6]+a[3]*a[8];
    MN[5][3]=a[3]*a[7]+a[4]*a[6];
    // Cinquième ligne
    MN[4][0]=a[6]*a[0];
    MN[4][1]=a[7]*a[1];
    MN[4][2]=a[8]*a[2];
    MN[4][5]=a[7]*a[2]+a[8]*a[1];
    MN[4][4]=a[8]*a[0]+a[6]*a[2];
    MN[4][3]=a[6]*a[1]+a[7]*a[0];
    // Sixième ligne
    MN[3][0]=a[0]*a[3];
    MN[3][1]=a[1]*a[4];
    MN[3][2]=a[2]*a[5];
    MN[3][5]=a[1]*a[5]+a[2]*a[4];
    MN[3][4]=a[2]*a[3]+a[0]*a[5];
    MN[3][3]=a[0]*a[4]+a[1]*a[3];
  } // end of MTestCastemRotationMatrix3D::MTestCastemRotationMatrix3D

  // compute the stiffness matrix in the global space
  void
  MTestCastemRotationMatrix3D::rotateStiffnessMatrixBackward(real *const D) const
  {
    // matrice N
    real N[6][6];
    for(unsigned short i=0;i!=6;++i){
      for(unsigned short j=0;j!=6;++j){
	N[i][j] = MN[i][j];
      }
    }
    N[3][0] *= real(2.);
    N[3][1] *= real(2.);
    N[3][2] *= real(2.);
    N[4][0] *= real(2.);
    N[4][1] *= real(2.);
    N[4][2] *= real(2.);
    N[5][0] *= real(2.);
    N[5][1] *= real(2.);
    N[5][2] *= real(2.);
    // matrice temporaire
    real t[6][6];
    for(unsigned short i=0;i!=6;++i){
      for(unsigned short j=0;j!=6;++j){
	t[i][j] = 0.;
	for(unsigned short k=0;k!=6;++k){
	  t[i][j] += D[k*6+i]*(N[k][j]);
	}
      }
    }
    // matrice finale
    for(unsigned short i=0;i!=6;++i){
      for(unsigned short j=0;j!=6;++j){
	D[j*6+i] = 0.;
	for(unsigned short k=0;k!=6;++k){
	  D[j*6+i] += N[k][i]*t[k][j];
	}
      }
    }
  } // end of MTestCastemRotationMatrix3D::rotateStiffnessMatrixBackward

  static tfel::material::ModellingHypothesis::Hypothesis
  getEffectiveModellingHypothesis(const tfel::material::ModellingHypothesis::Hypothesis& h,
				  const std::string& l,
				  const std::string& b)
  {
    using namespace tfel::system;
    using namespace tfel::material;
    typedef ExternalLibraryManager ELM;
    if(h==ModellingHypothesis::PLANESTRESS){
      auto& elm = ELM::getExternalLibraryManager();
      if(elm.checkIfUMATBehaviourUsesGenericPlaneStressAlgorithm(l,b)){
	return ModellingHypothesis::GENERALISEDPLANESTRAIN;
      }
    }
    return h;
  } // end of getEffectiveModellingHypothesis

  CastemSmallStrainBehaviour::CastemSmallStrainBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
							       const std::string& l,
							       const std::string& b)
    : CastemStandardBehaviour(getEffectiveModellingHypothesis(h,l,b),l,b)
  {
    using namespace tfel::system;
    using namespace tfel::material;
    typedef ExternalLibraryManager ELM;
    if(h==ModellingHypothesis::PLANESTRESS){
      auto& elm = ELM::getExternalLibraryManager();
      if(elm.checkIfUMATBehaviourUsesGenericPlaneStressAlgorithm(l,b)){
	this->ivnames.push_back("AxialStrain");
	this->ivtypes.push_back(0);
      }
    }
  }
  
  void
  CastemSmallStrainBehaviour::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    using namespace std;
    fill(v.begin(),v.end(),real(0));
  } // end of CastemSmallStrainBehaviour::setDrivingVariablesDefaultInitialValue  

  bool
  CastemSmallStrainBehaviour::computePredictionOperator(tfel::math::matrix<real>& Kt,
							   const tfel::math::tmatrix<3u,3u,real>& r,
							   const tfel::math::vector<real>& e0,
							   const tfel::math::vector<real>& s0,
							   const tfel::math::vector<real>& mp,
							   const tfel::math::vector<real>& iv0,
							   const tfel::math::vector<real>& esv0,
							   const tfel::material::ModellingHypothesis::Hypothesis h,
							   const StiffnessMatrixType::mtype ktype) const
  {
    using namespace tfel::math;
    if(ktype!=StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
      vector<real> s1(s0);
      vector<real> e1(e0);
      vector<real> iv1(iv0);
      vector<real> desv(esv0.size(),real(0));
      return this->call_behaviour(Kt,s1,iv1,r,e0,e1,s0,
				  mp,iv0,esv0,desv,
				  h,real(1),ktype,false);
    }
    // compute the stiffness operator from material properties
    tmatrix<3u,3u,real> drot(0.);
    tmatrix<3u,3u,real>::size_type i,j;
    for(i=0;i!=r.getNbRows();++i){
      for(j=0;j!=r.getNbCols();++j){
	drot(i,j) = r(j,i);
      }
    }
    this->computeElasticStiffness(Kt,mp,drot,h);
    return true;
  } // end of CastemSmallStrainBehaviour::computePredictionOperator

  bool
  CastemSmallStrainBehaviour::integrate(tfel::math::matrix<real>& Kt,
					   tfel::math::vector<real>& s1,
					   tfel::math::vector<real>& iv1,
					   const tfel::math::tmatrix<3u,3u,real>& r,
					   const tfel::math::vector<real>& e0,
					   const tfel::math::vector<real>& e1,
					   const tfel::math::vector<real>& s0,
					   const tfel::math::vector<real>& mp,
					   const tfel::math::vector<real>& iv0,
					   const tfel::math::vector<real>& ev0,
					   const tfel::math::vector<real>& dev,
					   const tfel::material::ModellingHypothesis::Hypothesis h,
					   const real dt,
					   const StiffnessMatrixType::mtype ktype) const
  {
    return this->call_behaviour(Kt,s1,iv1,r,e0,e1,s0,
				mp,iv0,ev0,dev,h,dt,
				ktype,true);
  } // end of CastemSmallStrainBehaviour::integrate

  bool
  CastemSmallStrainBehaviour::call_behaviour(tfel::math::matrix<real>& Kt,
						tfel::math::vector<real>& s1,
						tfel::math::vector<real>& iv1,
						const tfel::math::tmatrix<3u,3u,real>& r,
						const tfel::math::vector<real>& e0,
						const tfel::math::vector<real>& e1,
						const tfel::math::vector<real>& s0,
						const tfel::math::vector<real>& mp,
						const tfel::math::vector<real>& iv0,
						const tfel::math::vector<real>& ev0,
						const tfel::math::vector<real>& dev,
						const tfel::material::ModellingHypothesis::Hypothesis h,
						const real dt,
						const StiffnessMatrixType::mtype ktype,
						const bool b) const
  {
    using namespace std;
    using namespace tfel::math;
    using namespace castem;
    typedef tfel::material::ModellingHypothesis MH;
    using tfel::math::vector;
    using castem::CastemComputeStiffnessTensor;
    static const real sqrt2 = sqrt(real(2));
    CastemInt ntens;
    CastemInt ndi;
    CastemInt nprops = static_cast<CastemInt>(mp.size());
    CastemInt nstatv;
    unsigned short dimension;
    if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      ndi   = 14;
      ntens = 3;
      dimension = 1u;
    } else if (h==MH::AXISYMMETRICAL){
      ndi = 0;
      ntens = 4;
      dimension = 2u;
    } else if (h==MH::PLANESTRESS){
      ndi = -2;
      ntens = 4;
      dimension = 2u;
    } else if (h==MH::PLANESTRAIN){
      ndi = -1;
      ntens = 4;
      dimension = 2u;
    } else if (h==MH::GENERALISEDPLANESTRAIN){
      ndi = -3;
      ntens = 4;
      dimension = 2u;
    } else if (h==MH::TRIDIMENSIONAL){
      ndi = 2;
      ntens = 6;
      dimension = 3u;
    } else {
      string msg("CastemSmallStrainBehaviour::integrate : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    if((this->D.getNbRows()!=Kt.getNbRows())||
       (this->D.getNbCols()!=Kt.getNbCols())){
      string msg("CastemSmallStrainBehaviour::integrate : ");
      msg += "the memory has not been allocated correctly";
      throw(runtime_error(msg));
    }
    if(((iv0.size()==0)&&(this->ivs.size()!=1u))||
       ((iv0.size()!=0)&&(iv0.size()!=this->ivs.size()))){
      string msg("CastemSmallStrainBehaviour::integrate : ");
      msg += "the memory has not been allocated correctly";
      throw(runtime_error(msg));
    }
    fill(this->D.begin(),this->D.end(),0.);
    // choosing the type of stiffness matrix
    UmatBehaviourBase::initializeTangentOperator(ktype,b);
    // state variable initial values
    if(iv0.size()!=0){
      copy(iv0.begin(),iv0.end(),
	   this->ivs.begin());
    }
    nstatv = static_cast<CastemInt>(this->ivs.size());
    // rotation matrix
    tmatrix<3u,3u,real> drot(0.);
    tmatrix<3u,3u,real>::size_type i,j;
    for(i=0;i!=r.getNbRows();++i){
      for(j=0;j!=r.getNbCols();++j){
	drot(i,j) = r(j,i);
      }
    }
    CastemInt kinc(1);
    stensor<3u,real> ue0(real(0));
    stensor<3u,real> ude(real(0));
    copy(e0.begin(),e0.end(),ue0.begin());
    for(i=0;i!=e1.size();++i){
      ude(i) = e1(i)-e0(i);
    }
    copy(s0.begin(),s0.end(),s1.begin());
    for(i=3;i!=static_cast<unsigned short>(ntens);++i){
      s1(i)  /= sqrt2;
      ue0(i) *= sqrt2;
      ude(i) *= sqrt2;
    }
    CastemReal ndt(1.);
    (this->fct)(&s1(0),&this->ivs(0),&D(0,0),
		nullptr,nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr,
		&ue0(0),&ude(0),nullptr,&dt,
		&ev0(0),&dev(0),
		&ev0(0)+1,&dev(0)+1,
		nullptr,&ndi,nullptr,&ntens,&nstatv,&mp(0),
		&nprops,nullptr,&drot(0,0),&ndt,
		nullptr,nullptr,nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr,&kinc,0);
    if(kinc!=1){
      return false;
    }
    if(!iv1.empty()){
      copy_n(this->ivs.begin(), iv1.size(),iv1.begin());
    }
    // tangent operator (...)
    if(ktype!=StiffnessMatrixType::NOSTIFFNESS){ 
      if(ktype==StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
	this->computeElasticStiffness(Kt,mp,drot,h);
      } else {
	UmatNormaliseTangentOperator::exe(Kt,D,dimension);
      }
    }
    // turning things in standard conventions
    for(i=3;i!=static_cast<unsigned short>(ntens);++i){
      s1(i) *= sqrt2;
    }
    return true;
  } // end of CastemSmallStrainBehaviour::integrate

  void
  CastemSmallStrainBehaviour::computeElasticStiffness(tfel::math::matrix<real>& Kt,
							 const tfel::math::vector<real>& mp,
							 const tfel::math::tmatrix<3u,3u,real>& drot,
							 const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    using namespace tfel::math;
    using castem::CastemComputeStiffnessTensor;
    typedef tfel::material::ModellingHypothesis MH;
    tmatrix<3u,3u,real>::size_type i,j;
    if(this->stype==0u){
      if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	st2tost2<1u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   MH::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				   castem::ISOTROPIC,false>::exe(De,&mp(0));
	for(i=0;i!=3u;++i){
	  for(j=0;j!=3u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::AXISYMMETRICAL){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   MH::AXISYMMETRICAL,
				   castem::ISOTROPIC,false>::exe(De,&mp(0));
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::PLANESTRESS){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   MH::PLANESTRESS,
				   castem::ISOTROPIC,false>::exe(De,&mp(0));
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::PLANESTRAIN){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   MH::PLANESTRAIN,
				   castem::ISOTROPIC,false>::exe(De,&mp(0));
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::GENERALISEDPLANESTRAIN){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   MH::GENERALISEDPLANESTRAIN,
				   castem::ISOTROPIC,false>::exe(De,&mp(0));
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::TRIDIMENSIONAL){
	st2tost2<3u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   MH::TRIDIMENSIONAL,
				   castem::ISOTROPIC,false>::exe(De,&mp(0));
	for(i=0;i!=6u;++i){
	  for(j=0;j!=6u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else {
	string msg("CastemSmallStrainBehaviour::integrate : ");
	msg += "unsupported hypothesis";
	throw(runtime_error(msg));
      }
    } else if(this->stype==1u){
      if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	st2tost2<1u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   MH::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				   castem::ORTHOTROPIC,false>::exe(De,&mp(0));
	for(i=0;i!=3u;++i){
	  for(j=0;j!=3u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::AXISYMMETRICAL){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   MH::AXISYMMETRICAL,
				   castem::ORTHOTROPIC,false>::exe(De,&mp(0));
	De(3,3) /= 2; // On passe en format castem
	MTestCastemRotationMatrix2D m(&mp(7),&drot(0,0));
	m.rotateStiffnessMatrixBackward(&De(0,0));
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
	UmatNormaliseTangentOperator::exe(Kt,Kt,2u);
      } else if (h==MH::PLANESTRESS){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   MH::PLANESTRESS,
				   castem::ORTHOTROPIC,false>::exe(De,&mp(0));
	De(3,3) /= 2; // On passe en format castem
	MTestCastemRotationMatrix2D m(&mp(4),&drot(0,0));
	m.rotateStiffnessMatrixBackward(&De(0,0));
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
	UmatNormaliseTangentOperator::exe(Kt,Kt,2u);
      } else if (h==MH::PLANESTRAIN){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   MH::PLANESTRAIN,
				   castem::ORTHOTROPIC,false>::exe(De,&mp(0));
	De(3,3) /= 2; // On passe en format castem
	MTestCastemRotationMatrix2D m(&mp(7),&drot(0,0));
	m.rotateStiffnessMatrixBackward(&De(0,0));
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
	UmatNormaliseTangentOperator::exe(Kt,Kt,2u);
      } else if (h==MH::GENERALISEDPLANESTRAIN){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   MH::GENERALISEDPLANESTRAIN,
				   castem::ORTHOTROPIC,false>::exe(De,&mp(0));
	De(3,3) /= 2; // On passe en format castem
	MTestCastemRotationMatrix2D m(&mp(7),&drot(0,0));
	m.rotateStiffnessMatrixBackward(&De(0,0));
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
	UmatNormaliseTangentOperator::exe(Kt,Kt,2u);
      } else if (h==MH::TRIDIMENSIONAL){
	st2tost2<3u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   MH::TRIDIMENSIONAL,
				   castem::ORTHOTROPIC,false>::exe(De,&mp(0));
	MTestCastemRotationMatrix3D m(&mp(9),&drot(0,0));
	De(3,3) /= 2; // On passe en format castem
	De(4,4) /= 2; // On passe en format castem
	De(5,5) /= 2; // On passe en format castem
	m.rotateStiffnessMatrixBackward(&De(0,0));
	for(i=0;i!=6u;++i){
	  for(j=0;j!=6u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
	UmatNormaliseTangentOperator::exe(Kt,Kt,3u);
      } else {
	throw(std::runtime_error("CastemSmallStrainBehaviour::integrate : "
				 "unsupported hypothesis"));
      }
    } else {
      throw(std::runtime_error("CastemSmallStrainBehaviour::integrate : "
			       "invalid behaviour type (neither "
			       "isotropic or orthotropic)"));
    }
  }

  void
  CastemSmallStrainBehaviour::setOptionalMaterialPropertiesDefaultValues(EvolutionManager& mp,
									    const EvolutionManager& evm) const
  {
    using namespace std;
    using tfel::material::ModellingHypothesis;
    const ModellingHypothesis::Hypothesis h = ModellingHypothesis::fromString(this->hypothesis);
    CastemStandardBehaviour::setOptionalMaterialPropertiesDefaultValues(mp,evm);
    if(this->stype==0){
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"ThermalExpansion",0.);
    } else if(this->stype==1){
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"ThermalExpansion1",0.);
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"ThermalExpansion2",0.);
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"ThermalExpansion3",0.);
    } else {
      string msg("CastemSmallStrainBehaviour::setOptionalMaterialPropertiesDefaultValues : "
		 "unsupported symmetry type");
      throw(runtime_error(msg));
    }
    if(h==ModellingHypothesis::PLANESTRESS){
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"PlateWidth",0.);
    }
  } // end of CastemSmallStrainBehaviour::setOptionalMaterialPropertiesDefaultValues
      
  CastemSmallStrainBehaviour::~CastemSmallStrainBehaviour()
  {}
  
} // end of namespace mtest

