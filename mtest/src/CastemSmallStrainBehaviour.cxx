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
#include<limits>
#include<algorithm>

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/Castem/Castem.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/Castem/CastemComputeStiffnessTensor.hxx"

#include"MTest/Evolution.hxx"
#include"MTest/CurrentState.hxx"
#include"MTest/BehaviourWorkSpace.hxx"
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
    using tfel::material::ModellingHypothesis;
    if(h==ModellingHypothesis::PLANESTRESS){
      auto& elm = tfel::system::ExternalLibraryManager::getExternalLibraryManager();
      if(elm.checkIfUMATBehaviourUsesGenericPlaneStressAlgorithm(l,b)){
	return ModellingHypothesis::GENERALISEDPLANESTRAIN;
      }
    }
    return h;
  } // end of getEffectiveModellingHypothesis

  CastemSmallStrainBehaviour::CastemSmallStrainBehaviour(const Hypothesis h,
							 const std::string& l,
							 const std::string& b)
    : CastemStandardBehaviour(getEffectiveModellingHypothesis(h,l,b),l,b)
  {
    if(h==ModellingHypothesis::PLANESTRESS){
      auto& elm = tfel::system::ExternalLibraryManager::getExternalLibraryManager();
      if(elm.checkIfUMATBehaviourUsesGenericPlaneStressAlgorithm(l,b)){
	this->usesGenericPlaneStressAlgorithm = true;
	this->ivnames.push_back("AxialStrain");
	this->ivtypes.push_back(0);
      }
    }
  }
  
  void
  CastemSmallStrainBehaviour::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    std::fill(v.begin(),v.end(),real(0));
  } // end of CastemSmallStrainBehaviour::setDrivingVariablesDefaultInitialValue  

  std::pair<bool,real>
  CastemSmallStrainBehaviour::computePredictionOperator(BehaviourWorkSpace& wk,
							const CurrentState& s,
							const StiffnessMatrixType ktype) const
  {
    if(ktype!=StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
      wk.cs=s;
      return this->call_behaviour(wk.kt,wk.cs,wk,real(1),ktype,false);
    }
    // compute the stiffness operator from material properties
    this->computeElasticStiffness(wk.kt,s.mprops1,transpose(s.r));
    return {true,1};
  } // end of CastemSmallStrainBehaviour::computePredictionOperator

  std::pair<bool,real>
  CastemSmallStrainBehaviour::integrate(CurrentState& s,
					BehaviourWorkSpace& wk,
					const real dt,
					const StiffnessMatrixType ktype) const
  {
    return this->call_behaviour(wk.k,s,wk,dt,ktype,true);
  } // end of CastemSmallStrainBehaviour::integrate

  std::pair<bool,real>
  CastemSmallStrainBehaviour::call_behaviour(tfel::math::matrix<real>& Kt,
					     CurrentState& s,
					     BehaviourWorkSpace& wk,
					     const real dt,
					     const StiffnessMatrixType ktype,
					     const bool b) const
  {
    using namespace std;
    using namespace tfel::math;
    using namespace castem;
    using tfel::math::vector;
    using castem::CastemComputeStiffnessTensor;
    static const real sqrt2 = sqrt(real(2));
    const auto h = this->usesGenericPlaneStressAlgorithm ?
                   ModellingHypothesis::PLANESTRESS :this->getHypothesis();
    const auto nprops = static_cast<CastemInt>(s.mprops1.size());
    CastemInt ntens,ndi,nstatv;
    unsigned short dimension;
    if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      ndi   = 14;
      ntens = 3;
      dimension = 1u;
    } else if (h==ModellingHypothesis::AXISYMMETRICAL){
      ndi = 0;
      ntens = 4;
      dimension = 2u;
    } else if (h==ModellingHypothesis::PLANESTRESS){
      ndi = -2;
      ntens = 4;
      dimension = 2u;
    } else if (h==ModellingHypothesis::PLANESTRAIN){
      ndi = -1;
      ntens = 4;
      dimension = 2u;
    } else if (h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
      ndi = -3;
      ntens = 4;
      dimension = 2u;
    } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
      ndi = 2;
      ntens = 6;
      dimension = 3u;
    } else {
      throw(runtime_error("CastemSmallStrainBehaviour::integrate: "
			  "unsupported hypothesis"));
    }
    if((wk.D.getNbRows()!=Kt.getNbRows())||
       (wk.D.getNbCols()!=Kt.getNbCols())){
      throw(runtime_error("CastemSmallStrainBehaviour::integrate: "
			  "the memory has not been allocated correctly"));
    }
    if(((s.iv0.size()==0)&&(wk.ivs.size()!=1u))||
       ((s.iv0.size()!=0)&&(s.iv0.size()!=wk.ivs.size()))){
      throw(runtime_error("CastemSmallStrainBehaviour::integrate: "
			  "the memory has not been allocated correctly (2)"));
    }
    fill(wk.D.begin(),wk.D.end(),0.);
    // choosing the type of stiffness matrix
    UmatBehaviourBase::initializeTangentOperator(wk.D,ktype,b);
    // state variable initial values
    if(s.iv0.size()!=0){
      copy(s.iv0.begin(),s.iv0.end(),wk.ivs.begin());
    }
    nstatv = static_cast<CastemInt>(wk.ivs.size());
    // rotation matrix
    tmatrix<3u,3u,real> drot = transpose(s.r);
    tmatrix<3u,3u,real>::size_type i;
    CastemInt kinc(1);
    stensor<3u,real> ue0(real(0));
    stensor<3u,real> ude(real(0));
    copy(s.e0.begin(),s.e0.end(),ue0.begin());
    for(i=0;i!=s.e1.size();++i){
      ude(i) = s.e1(i)-s.e0(i);
    }
    copy(s.s0.begin(),s.s0.end(),s.s1.begin());
    // thermal strain
    for(i=0;i!=s.e1.size();++i){
      ue0(i) -= s.e_th0(i);
      ude(i) -= s.e_th1(i)-s.e_th0(i);
    }
    // castem conventions
    for(i=3;i!=static_cast<unsigned short>(ntens);++i){
      s.s1(i) /= sqrt2;
      ue0(i)  *= sqrt2;
      ude(i)  *= sqrt2;
    }
    auto ndt = std::numeric_limits<CastemReal>::max();
    (this->fct)(&(s.s1(0)),&wk.ivs(0),&(wk.D(0,0)),
		nullptr,nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr,
		&ue0(0),&ude(0),nullptr,&dt,
		&(s.esv0(0))  ,&(s.desv(0)),
		&(s.esv0(0))+1,&(s.desv(0))+1,
		nullptr,&ndi,nullptr,&ntens,&nstatv,
		&(s.mprops1(0)),
		&nprops,nullptr,&drot(0,0),&ndt,
		nullptr,nullptr,nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr,&kinc,0);
    if(kinc!=1){
      return {false,ndt};
    }
    // tangent operator (...)
    if(ktype!=StiffnessMatrixType::NOSTIFFNESS){ 
      if(ktype==StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
	this->computeElasticStiffness(Kt,s.mprops1,drot);
      } else {
	UmatNormaliseTangentOperator::exe(Kt,wk.D,dimension);
      }
    }
    if(!s.iv1.empty()){
      std::copy_n(wk.ivs.begin(),s.iv1.size(),s.iv1.begin());
    }
    // turning things in standard conventions
    for(i=3;i!=static_cast<unsigned short>(ntens);++i){
      s.s1(i) *= sqrt2;
    }
    return {true,ndt};
  } // end of CastemSmallStrainBehaviour::integrate

  void
  CastemSmallStrainBehaviour::computeElasticStiffness(tfel::math::matrix<real>& Kt,
						      const tfel::math::vector<real>& mp,
						      const tfel::math::tmatrix<3u,3u,real>& drot) const
  {
    using namespace std;
    using namespace tfel::math;
    using castem::CastemComputeStiffnessTensor;
    tmatrix<3u,3u,real>::size_type i,j;
    const auto h = this->getHypothesis();
    if(this->stype==0u){
      if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	st2tost2<1u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				   castem::ISOTROPIC,false>::exe(De,&mp(0));
	for(i=0;i!=3u;++i){
	  for(j=0;j!=3u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==ModellingHypothesis::AXISYMMETRICAL){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::AXISYMMETRICAL,
				   castem::ISOTROPIC,false>::exe(De,&mp(0));
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==ModellingHypothesis::PLANESTRESS){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::PLANESTRESS,
				   castem::ISOTROPIC,false>::exe(De,&mp(0));
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==ModellingHypothesis::PLANESTRAIN){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::PLANESTRAIN,
				   castem::ISOTROPIC,false>::exe(De,&mp(0));
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::GENERALISEDPLANESTRAIN,
				   castem::ISOTROPIC,false>::exe(De,&mp(0));
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
	st2tost2<3u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::TRIDIMENSIONAL,
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
      if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	st2tost2<1u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				   castem::ORTHOTROPIC,false>::exe(De,&mp(0));
	for(i=0;i!=3u;++i){
	  for(j=0;j!=3u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==ModellingHypothesis::AXISYMMETRICAL){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::AXISYMMETRICAL,
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
      } else if (h==ModellingHypothesis::PLANESTRESS){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::PLANESTRESS,
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
      } else if (h==ModellingHypothesis::PLANESTRAIN){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::PLANESTRAIN,
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
      } else if (h==ModellingHypothesis::GENERALISEDPLANESTRAIN){
	st2tost2<2u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::GENERALISEDPLANESTRAIN,
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
      } else if (h==ModellingHypothesis::TRIDIMENSIONAL){
	st2tost2<3u,real> De;
	CastemComputeStiffnessTensor<castem::SMALLSTRAINSTANDARDBEHAVIOUR,
				   ModellingHypothesis::TRIDIMENSIONAL,
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
    const auto h = this->getHypothesis();
    CastemStandardBehaviour::setOptionalMaterialPropertiesDefaultValues(mp,evm);
    if(this->stype==0){
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"ThermalExpansion",0.);
    } else if(this->stype==1){
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"ThermalExpansion1",0.);
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"ThermalExpansion2",0.);
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"ThermalExpansion3",0.);
    } else {
      throw(std::runtime_error("CastemSmallStrainBehaviour::"
			       "setOptionalMaterialPropertiesDefaultValues: "
			       "unsupported symmetry type"));
    }
    if(h==ModellingHypothesis::PLANESTRESS){
      Behaviour::setOptionalMaterialPropertyDefaultValue(mp,evm,"PlateWidth",0.);
    }
  } // end of CastemSmallStrainBehaviour::setOptionalMaterialPropertiesDefaultValues
      
  CastemSmallStrainBehaviour::~CastemSmallStrainBehaviour()
  {}
  
} // end of namespace mtest

