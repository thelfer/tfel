/*! 
 * \file   mfront/mtest/MTestAsterFiniteStrainBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  07 avril 2013
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
#include"TFEL/Math/tensor.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/t2tost2.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/Aster/Aster.hxx"
#include"MFront/Aster/AsterComputeStiffnessTensor.hxx"

#include"MFront/MTestUmatNormaliseTangentOperator.hxx"
#include"MFront/MTestAsterFiniteStrainBehaviour.hxx"

namespace mfront
{

  static unsigned short
  getRowIndex(const unsigned short i)
  {
    switch(i){
    case 0:
      return 0;
    case 1:
      return 1;
    case 2:
      return 2;
    case 3:
      return 0;
    case 4:
      return 1;
    case 5:
      return 0;
    case 6:
      return 2;
    case 7:
      return 1;
    case 8:
      return 2;
    }
    return 0;
  }
  
  static unsigned short
  getColumnIndex(const unsigned short j)
  {
    switch(j){
    case 0:
      return 0;
    case 1:
      return 1;
    case 2:
      return 2;
    case 3:
      return 1;
    case 4:
      return 0;
    case 5:
      return 2;
    case 6:
      return 0;
    case 7:
      return 2;
    case 8:
      return 1;
    }
    return 0;
  }

  template<unsigned short N>
  static void
  convertTangentOperator(tfel::math::matrix<real>& Kt,
			 tfel::math::matrix<aster::AsterReal>& D,
			 const tfel::math::vector<real>& sv,
			 const tfel::math::vector<real>& Fv0,
			 const tfel::math::vector<real>& Fv1)
  {
    using namespace std;
    using namespace aster;
    using namespace tfel::math;
    t2tost2<N,real> dtau_ddF;
    t2tost2<N,real> dtau;
    t2tost2<N,real> dsig;
    stensor<N,real> sig(&sv[0]);
    tensor<N,real>  F0(&Fv0[0]);
    tensor<N,real>  F1(&Fv1[0]);
    tensor<N,real>  inv_F0 = invert(F0);
    // reverting things
    AsterReal *v = &D(0,0);
    for(unsigned short i=0;i!=StensorDimeToSize<N>::value;++i){  // boucle sur tau
      for(unsigned short j=0;j!=TensorDimeToSize<N>::value;++j){  // boucle sur F
	const unsigned short mi = getRowIndex(j);
	const unsigned short mj = getColumnIndex(j);
	dtau_ddF(i,j) = v[i+(mi+3*mj)*StensorDimeToSize<N>::value];
      }
    }
    dtau = dtau_ddF*t2tot2<N,real>::tpld(inv_F0);
    computeCauchyStressDerivativeFromKirchoffStressDerivative(dsig,dtau,sig,F1);
    copy(dsig.begin(),dsig.end(),Kt.begin());
  }

  MTestAsterFiniteStrainBehaviour::MTestAsterFiniteStrainBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
								 const std::string& l,
								 const std::string& b)
    : MTestAsterStandardBehaviour(h,l,b)
  {}

  void
  MTestAsterFiniteStrainBehaviour::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    using namespace std;
    fill(v.begin(),v.end(),real(0));
    v[0] = real(1);
    v[1] = real(1);
    v[2] = real(1);
  }

  void
  MTestAsterFiniteStrainBehaviour::allocate(const tfel::material::ModellingHypothesis::Hypothesis h)
  {
    MTestAsterStandardBehaviour::allocate(h);
    this->D.resize(6u,9u);
  }

  bool
  MTestAsterFiniteStrainBehaviour::call_behaviour(tfel::math::matrix<real>& Kt,
						 tfel::math::vector<real>& s1,
						 tfel::math::vector<real>& iv1,
						 const tfel::math::tmatrix<3u,3u,real>& r,
						 const tfel::math::vector<real>& u0,
						 const tfel::math::vector<real>& u1,
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
    using namespace aster;
    typedef tfel::material::ModellingHypothesis MH;
    using tfel::math::vector;
    static const real sqrt2 = sqrt(real(2));
    AsterInt ntens;
    AsterInt dimension;
    AsterInt nprops = static_cast<AsterInt>(mp.size());
    AsterInt nstatv;
    AsterInt nummod;
    if (h==MH::AXISYMMETRICAL){
      ntens = 4;
      dimension = 2u;
      nummod = 4u;
    } else if (h==MH::PLANESTRESS){
      ntens = 4;
      dimension = 2u;
      nummod = 5u;
    } else if (h==MH::PLANESTRAIN){
      ntens = 4;
      dimension = 2u;
      nummod = 6u;
    } else if (h==MH::TRIDIMENSIONAL){
      ntens = 6;
      dimension = 3u;
      nummod = 3u;
    } else {
      string msg("MTestAsterFiniteStrainBehaviour::call_beahviour : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    fill(this->D.begin(),this->D.end(),0.);
    // choosing the type of stiffness matrix
    MTestUmatBehaviourBase::initializeTangentOperator(ktype,b);
    // using a local copy of internal state variables to handle the
    // case where iv0 is empty
    copy(iv0.begin(),iv0.end(),this->iv.begin());
    if(iv0.empty()){
      iv[0] = real(0.);
    }
    nstatv = static_cast<AsterInt>(iv.size());
    // rotation matrix
    tmatrix<3u,3u,real> drot;
    for(tmatrix<3u,3u,real>::size_type i=0;i!=3u;++i){
      for(tmatrix<3u,3u,real>::size_type j=0;j!=3u;++j){
	drot(i,j) = r(j,i);
      }
    }
    tmatrix<3u,3u,real> uu0(real(0));
    tmatrix<3u,3u,real> uu1(real(0));
    uu0(0,0) = u0(0); uu1(0,0) = u1(0);
    uu0(1,1) = u0(1); uu1(1,1) = u1(1);
    uu0(2,2) = u0(2); uu1(2,2) = u1(2);
    if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      uu0(1,0) = 0.; uu1(1,0) = 0.;
      uu0(0,1) = 0.; uu1(0,1) = 0.;
      uu0(2,0) = 0.; uu1(2,0) = 0.;
      uu0(0,2) = 0.; uu1(0,2) = 0.;
      uu0(2,1) = 0.; uu1(2,1) = 0.;
      uu0(1,2) = 0.; uu1(1,2) = 0.;
    } else if ((h==MH::AXISYMMETRICAL)||(h==MH::PLANESTRESS)||
	       (h==MH::PLANESTRAIN)||(h==MH::GENERALISEDPLANESTRAIN)){
      // uu0 and uu1 must be built using Fortran notations
      uu0(1,0) = u0(3); uu1(1,0) = u1(3);
      uu0(0,1) = u0(4); uu1(0,1) = u1(4);
      uu0(2,0) = 0.; uu1(2,0) = 0.;
      uu0(0,2) = 0.; uu1(0,2) = 0.;
      uu0(2,1) = 0.; uu1(2,1) = 0.;
      uu0(1,2) = 0.; uu1(1,2) = 0.;
    } else if (h==MH::TRIDIMENSIONAL){
      // uu0 and uu1 must be built using Fortran notations
      uu0(1,0) = u0(3); uu1(1,0) = u1(3);
      uu0(0,1) = u0(4); uu1(0,1) = u1(4);
      uu0(2,0) = u0(5); uu1(2,0) = u1(5);
      uu0(0,2) = u0(6); uu1(0,2) = u1(6);
      uu0(2,1) = u0(7); uu1(2,1) = u1(7);
      uu0(1,2) = u0(8); uu1(1,2) = u1(8);
    } else {
      string msg("MTestUmatFiniteStrainBehaviour::integrate : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    copy(s0.begin(),s0.end(),s1.begin());
    for(unsigned short i=3;i!=static_cast<unsigned short>(ntens);++i){
      s1(i)  /= sqrt2;
    }
    AsterReal ndt(1.);
    (this->fct)(&s1(0),&iv(0),&D(0,0),
		&uu0(0,0),&uu1(0,0),&dt,
		&ev0(0),&dev(0),
		&ev0(0)+1,&dev(0)+1,
		&ntens,&nstatv,&mp(0),
		&nprops,&drot(0,0),&ndt,&nummod);
    if(ndt<0.){
      return false;
    }
    if(b){
      if(!iv0.empty()){
	copy(iv.begin(),iv.end(),iv1.begin());
      }
      // turning things in standard conventions
      for(unsigned short i=3;i!=static_cast<unsigned short>(ntens);++i){
	s1(i) *= sqrt2;
      }
    }
    if(ktype!=MTestStiffnessMatrixType::NOSTIFFNESS){
      if(dimension==1u){
	convertTangentOperator<1u>(Kt,D,s1,u0,u1);
      } else if(dimension==2u){
	convertTangentOperator<2u>(Kt,D,s1,u0,u1);
      } else if(dimension==3u){
	convertTangentOperator<3u>(Kt,D,s1,u0,u1);
      }
    }
    return true;
  }

  MTestAsterFiniteStrainBehaviour::~MTestAsterFiniteStrainBehaviour()
  {}
  
} // end of namespace mfront









