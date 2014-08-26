/*!
 * \file   MTestUmatSmallStrainBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 */

#include<cmath>
#include<algorithm>

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/UMAT/UMAT.hxx"
#include"MFront/MTestUmatSmallStrainBehaviour.hxx"
#include"MFront/MTestUmatNormaliseTangentOperator.hxx"
#include"MFront/UMAT/UMATComputeStiffnessOperator.hxx"


namespace mfront
{

  struct MTestUmatRotationMatrix2D
  {
    MTestUmatRotationMatrix2D(const real *const);
    // compute the stiffness matrix in the global space
    void rotateStiffnessMatrixBackward(real *const) const;
  private:
    real MN[3][3];
  }; // end of struct MTestUmatRotationMatrix2D
  
  struct MTestUmatRotationMatrix3D
  {
    
    MTestUmatRotationMatrix3D(const real *const);
    // compute the stiffness matrix in the global space
    void rotateStiffnessMatrixBackward(real *const) const;
  private:
    real MN[6][6];
  }; // end of struct MTestUmatRotationMatrix3D

  // Constructeur
  // drot : Matrice de passage élement/global
  MTestUmatRotationMatrix2D::MTestUmatRotationMatrix2D(const real *const drot)
  {
    // Matrice de passage matériau/global
    real a[4];
    // // Premier vecteur
    // a[1,1]
    a[0]=drot[0];
    // a[2,1]
    a[1]=drot[1];
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
  } // end of MTestUmatRotationMatrix2D::MTestUmatRotationMatrix2D

  // Calcul de la déformation dans le repère global
  // D^g=tN:D^m:N
  void
  MTestUmatRotationMatrix2D::rotateStiffnessMatrixBackward(real *const D) const
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
  } // end of MTestUmatRotationMatrix2D::rotateStiffnessMatrixBackward

  // Constructeur
  // drot : Matrice de passage élement/global
  MTestUmatRotationMatrix3D::MTestUmatRotationMatrix3D(const real *const drot)
  {
    // Contruction de la matrice de passage N (pour les tenseurs)
    // Première ligne
    MN[0][0]=drot[0]*drot[0];
    MN[0][1]=drot[1]*drot[1];
    MN[0][2]=drot[2]*drot[2];
    MN[0][5]=drot[1]*drot[2];
    MN[0][4]=drot[2]*drot[0];
    MN[0][3]=drot[0]*drot[1];

    // Deuxième ligne
    MN[1][0]=drot[3]*drot[3];
    MN[1][1]=drot[4]*drot[4];
    MN[1][2]=drot[5]*drot[5];
    MN[1][5]=drot[4]*drot[5];
    MN[1][4]=drot[5]*drot[3];
    MN[1][3]=drot[3]*drot[4];

    // Troisième ligne
    MN[2][0]=drot[6]*drot[6];
    MN[2][1]=drot[7]*drot[7];
    MN[2][2]=drot[8]*drot[8];
    MN[2][5]=drot[7]*drot[8];
    MN[2][4]=drot[8]*drot[6];
    MN[2][3]=drot[6]*drot[7];

    // Quatrième ligne
    MN[5][0]=drot[3]*drot[6];
    MN[5][1]=drot[4]*drot[7];
    MN[5][2]=drot[5]*drot[8];
    MN[5][5]=drot[4]*drot[8]+drot[5]*drot[7];
    MN[5][4]=drot[5]*drot[6]+drot[3]*drot[8];
    MN[5][3]=drot[3]*drot[7]+drot[4]*drot[6];

    // Cinquième ligne
    MN[4][0]=drot[6]*drot[0];
    MN[4][1]=drot[7]*drot[1];
    MN[4][2]=drot[8]*drot[2];
    MN[4][5]=drot[7]*drot[2]+drot[8]*drot[1];
    MN[4][4]=drot[8]*drot[0]+drot[6]*drot[2];
    MN[4][3]=drot[6]*drot[1]+drot[7]*drot[0];

    // Sixième ligne
    MN[3][0]=drot[0]*drot[3];
    MN[3][1]=drot[1]*drot[4];
    MN[3][2]=drot[2]*drot[5];
    MN[3][5]=drot[1]*drot[5]+drot[2]*drot[4];
    MN[3][4]=drot[2]*drot[3]+drot[0]*drot[5];
    MN[3][3]=drot[0]*drot[4]+drot[1]*drot[3];
  } // end of MTestUmatRotationMatrix3D::MTestUmatRotationMatrix3D

  // compute the stiffness matrix in the global space
  void
  MTestUmatRotationMatrix3D::rotateStiffnessMatrixBackward(real *const D) const
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
  } // end of MTestUmatRotationMatrix3D::rotateStiffnessMatrixBackward

  MTestUmatSmallStrainBehaviour::MTestUmatSmallStrainBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
							       const std::string& l,
							       const std::string& b)
    : MTestUmatStandardBehaviour(h,l,b)
  {
    using namespace tfel::system;
    using namespace tfel::material;
    typedef ExternalLibraryManager ELM;
    if(h==ModellingHypothesis::PLANESTRESS){
      ELM& elm = ELM::getExternalLibraryManager();
      if(elm.checkIfUMATBehaviourUsesGenericPlaneStressAlgorithm(l,b)){
	this->ivnames.push_back("AxialStrain");
	this->ivtypes.push_back(0);
      }
    }
  }
  
  void
  MTestUmatSmallStrainBehaviour::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    using namespace std;
    fill(v.begin(),v.end(),real(0));
  } // end of MTestUmatSmallStrainBehaviour::setDrivingVariablesDefaultInitialValue  

  bool
  MTestUmatSmallStrainBehaviour::computePredictionOperator(tfel::math::matrix<real>& Kt,
							   const tfel::math::tmatrix<3u,3u,real>& r,
							   const tfel::math::vector<real>& e0,
							   const tfel::math::vector<real>& s0,
							   const tfel::math::vector<real>& mp,
							   const tfel::math::vector<real>& iv0,
							   const tfel::math::vector<real>& esv0,
							   const tfel::material::ModellingHypothesis::Hypothesis h,
							   const MTestStiffnessMatrixType::mtype ktype) const
  {
    using namespace tfel::math;
    if(ktype!=MTestStiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
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
  } // end of MTestUmatSmallStrainBehaviour::computePredictionOperator

  bool
  MTestUmatSmallStrainBehaviour::integrate(tfel::math::matrix<real>& Kt,
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
					    const MTestStiffnessMatrixType::mtype ktype) const
  {
    return this->call_behaviour(Kt,s1,iv1,r,e0,e1,s0,
				mp,iv0,ev0,dev,h,dt,
				ktype,true);
  } // end of MTestUmatSmallStrainBehaviour::integrate

  bool
  MTestUmatSmallStrainBehaviour::call_behaviour(tfel::math::matrix<real>& Kt,
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
						const MTestStiffnessMatrixType::mtype ktype,
						const bool b) const
  {
    using namespace std;
    using namespace tfel::math;
    using namespace umat;
    typedef tfel::material::ModellingHypothesis MH;
    using tfel::math::vector;
    using umat::UMATComputeStiffnessOperator;
    static const real sqrt2 = sqrt(real(2));
    UMATInt ntens;
    UMATInt ndi;
    UMATInt nprops = static_cast<UMATInt>(mp.size());
    UMATInt nstatv;
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
      string msg("MTestUmatSmallStrainBehaviour::integrate : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    if((this->D.getNbRows()!=Kt.getNbRows())||
       (this->D.getNbCols()!=Kt.getNbCols())){
      string msg("MTestUmatSmallStrainBehaviour::integrate : ");
      msg += "the memory has not been allocated correctly";
      throw(runtime_error(msg));
    }
    if(((iv0.size()==0)&&(this->iv.size()!=1u))||
       ((iv0.size()!=0)&&(iv0.size()!=this->iv.size()))){
      string msg("MTestUmatSmallStrainBehaviour::integrate : ");
      msg += "the memory has not been allocated correctly";
      throw(runtime_error(msg));
    }
    fill(this->D.begin(),this->D.end(),0.);
    // choosing the type of stiffness matrix
    if(b){
      if((ktype==MTestStiffnessMatrixType::NOSTIFFNESS)||
	 (ktype==MTestStiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES)){
	// do nothing
      } else if(ktype==MTestStiffnessMatrixType::ELASTIC){
	D(0,0) = real(1);
      } else if(ktype==MTestStiffnessMatrixType::SECANTOPERATOR){
	D(0,0) = real(2);
      } else if(ktype==MTestStiffnessMatrixType::TANGENTOPERATOR){
	D(0,0) = real(3);
      } else if(ktype==MTestStiffnessMatrixType::CONSISTANTTANGENTOPERATOR){
	D(0,0) = real(4);
      } else {
	string msg("MTestAsterSmallStrainBehaviour::call_behaviour : "
		   "invalid or unspecified stiffness matrix type");
	throw(runtime_error(msg));
      }
    } else {
      if(ktype==MTestStiffnessMatrixType::ELASTIC){
	D(0,0) = real(-1);
      } else if(ktype==MTestStiffnessMatrixType::SECANTOPERATOR){
	D(0,0) = real(-2);
      } else if(ktype==MTestStiffnessMatrixType::TANGENTOPERATOR){
	D(0,0) = real(-3);
      } else {
	string msg("MTestAsterSmallStrainBehaviour::call_behaviour : "
		   "invalid or unspecified stiffness matrix type");
	throw(runtime_error(msg));
      }
    }
    if(iv0.size()!=0){
      copy(iv0.begin(),iv0.end(),
	   this->iv.begin());
    }
    nstatv = static_cast<UMATInt>(iv.size());
    // rotation matrix
    tmatrix<3u,3u,real> drot(0.);
    tmatrix<3u,3u,real>::size_type i,j;
    for(i=0;i!=r.getNbRows();++i){
      for(j=0;j!=r.getNbCols();++j){
	drot(i,j) = r(j,i);
      }
    }
    UMATInt kinc(1);
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
    (this->fct)(&ntens,&dt,&drot(0,0),
		&D(0,0),&ue0(0),&ude(0),
		&ev0(0),&dev(0),
		&mp(0),&nprops,
		&ev0(0)+1,&dev(0)+1,
		&iv(0),&nstatv,&s1(0),
		&ndi,&kinc);
    if(kinc!=1){
      return false;
    }
    if(!iv1.empty()){
      copy(iv.begin(),iv.end(),iv1.begin());
    }
    // tangent operator (...)
    if(ktype!=MTestStiffnessMatrixType::NOSTIFFNESS){ 
      if(ktype==MTestStiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
	this->computeElasticStiffness(Kt,mp,drot,h);
      } else {
	MTestUmatNormaliseTangentOperator::exe(Kt,D,dimension);
      }
    }
    // turning things in standard conventions
    for(i=3;i!=static_cast<unsigned short>(ntens);++i){
      s1(i) *= sqrt2;
    }
    return true;
  } // end of MTestUmatSmallStrainBehaviour::integrate

  void
  MTestUmatSmallStrainBehaviour::computeElasticStiffness(tfel::math::matrix<real>& Kt,
							 const tfel::math::vector<real>& mp,
							 const tfel::math::tmatrix<3u,3u,real>& drot,
							 const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    using namespace tfel::math;
    using umat::UMATComputeStiffnessOperator;
    typedef tfel::material::ModellingHypothesis MH;
    tmatrix<3u,3u,real>::size_type i,j;
    if(this->stype==0u){
      if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	st2tost2<1u,real> De;
	UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				     MH::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				     umat::ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=3u;++i){
	  for(j=0;j!=3u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::AXISYMMETRICAL){
	st2tost2<2u,real> De;
	UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				     MH::AXISYMMETRICAL,
				     umat::ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::PLANESTRESS){
	st2tost2<2u,real> De;
	UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				     MH::PLANESTRESS,
				     umat::ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::PLANESTRAIN){
	st2tost2<2u,real> De;
	UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				     MH::PLANESTRAIN,
				     umat::ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::GENERALISEDPLANESTRAIN){
	st2tost2<2u,real> De;
	UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				     MH::GENERALISEDPLANESTRAIN,
				     umat::ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::TRIDIMENSIONAL){
	st2tost2<3u,real> De;
	UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				     MH::TRIDIMENSIONAL,
				     umat::ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=6u;++i){
	  for(j=0;j!=6u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else {
	string msg("MTestUmatSmallStrainBehaviour::integrate : ");
	msg += "unsupported hypothesis";
	throw(runtime_error(msg));
      }
    } else if(this->stype==1u){
      if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	st2tost2<1u,real> De;
	UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				     MH::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				     umat::ORTHOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=3u;++i){
	  for(j=0;j!=3u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::AXISYMMETRICAL){
	st2tost2<2u,real> De;
	UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				     MH::AXISYMMETRICAL,
				     umat::ORTHOTROPIC>::exe(&mp(0),De);
	MTestUmatRotationMatrix2D m(&drot(0,0));
	m.rotateStiffnessMatrixBackward(&De(0,0));
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::PLANESTRESS){
	st2tost2<2u,real> De;
	UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				     MH::PLANESTRESS,
				     umat::ORTHOTROPIC>::exe(&mp(0),De);
	MTestUmatRotationMatrix2D m(&drot(0,0));
	m.rotateStiffnessMatrixBackward(&De(0,0));
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::PLANESTRAIN){
	st2tost2<2u,real> De;
	UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				     MH::PLANESTRAIN,
				     umat::ORTHOTROPIC>::exe(&mp(0),De);
	MTestUmatRotationMatrix2D m(&drot(0,0));
	m.rotateStiffnessMatrixBackward(&De(0,0));
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::GENERALISEDPLANESTRAIN){
	st2tost2<2u,real> De;
	UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				     MH::GENERALISEDPLANESTRAIN,
				     umat::ORTHOTROPIC>::exe(&mp(0),De);
	MTestUmatRotationMatrix2D m(&drot(0,0));
	m.rotateStiffnessMatrixBackward(&De(0,0));
	for(i=0;i!=4u;++i){
	  for(j=0;j!=4u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else if (h==MH::TRIDIMENSIONAL){
	st2tost2<3u,real> De;
	UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
				     MH::TRIDIMENSIONAL,
				     umat::ORTHOTROPIC>::exe(&mp(0),De);
	MTestUmatRotationMatrix3D m(&drot(0,0));
	m.rotateStiffnessMatrixBackward(&De(0,0));
	for(i=0;i!=6u;++i){
	  for(j=0;j!=6u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else {
	string msg("MTestUmatSmallStrainBehaviour::integrate : ");
	msg += "unsupported hypothesis";
	throw(runtime_error(msg));
      }
    } else {
      string msg("MTestUmatSmallStrainBehaviour::integrate : ");
      msg += "invalid behaviour type (neither "
	"isotropic or orthotropic)";
      throw(runtime_error(msg));
    }

  }
      
  MTestUmatSmallStrainBehaviour::~MTestUmatSmallStrainBehaviour()
  {}
  
} // end of namespace mfront

