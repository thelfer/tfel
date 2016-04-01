/*! 
 * \file   mfront/mtest/EuroplexusFiniteStrainBehaviour.cxx
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
#include"TFEL/Math/T2toST2/T2toST2ConceptIO.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/Europlexus/Europlexus.hxx"
#include"MFront/Europlexus/EuroplexusComputeStiffnessTensor.hxx"

#include"MTest/CurrentState.hxx"
#include"MTest/BehaviourWorkSpace.hxx"
#include"MTest/UmatNormaliseTangentOperator.hxx"
#include"MTest/EuroplexusFiniteStrainBehaviour.hxx"

namespace mtest
{

  EuroplexusFiniteStrainBehaviour::EuroplexusFiniteStrainBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
								 const std::string& l,
								 const std::string& b)
    : EuroplexusStandardBehaviour(h,l,b)
  {}

  void
  EuroplexusFiniteStrainBehaviour::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    std::fill(v.begin(),v.end(),real(0));
    v[0] = v[1] = v[2] = real(1);
  }

  void
  EuroplexusFiniteStrainBehaviour::allocate(BehaviourWorkSpace& wk,
				       const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    EuroplexusStandardBehaviour::allocate(wk,h);
    wk.D.resize(6u,9u);
  }

  std::pair<bool,real>
  EuroplexusFiniteStrainBehaviour::call_behaviour(tfel::math::matrix<real>& Kt,
						  CurrentState& s,
						  BehaviourWorkSpace&,
						  const tfel::material::ModellingHypothesis::Hypothesis h,
						  const real dt,
						  const StiffnessMatrixType ktype,
						  const bool b) const
  {
    typedef tfel::material::ModellingHypothesis MH;
    const epx::EuroplexusInt nprops  = static_cast<epx::EuroplexusInt>(s.mprops1.size());
    const epx::EuroplexusInt nstatv  = static_cast<epx::EuroplexusInt>(s.iv0.size());
    const epx::EuroplexusInt npredef = static_cast<epx::EuroplexusInt>(s.esv0.size())-1;
    const epx::EuroplexusInt hv = [&h](){
      if (h==MH::AXISYMMETRICAL){
	return 3;
      } else if (h==MH::PLANESTRESS){
	return 2;
      } else if (h==MH::PLANESTRAIN){
	return 1;
      } else if (h==MH::TRIDIMENSIONAL){
	return 0;
      } else {
	throw(std::runtime_error("EuroplexusFiniteStrainBehaviour::call_beahviour: "
				 "unsupported hypothesis"));
      }
    }();
    UmatBehaviourBase::initializeTangentOperator(Kt,ktype,b);
    tfel::math::tensor<3u,real> uu0(real(0));
    tfel::math::tensor<3u,real> uu1(real(0));
    tfel::math::stensor<3u,real> s1(real(0));
    std::copy(s.e0.begin(),s.e0.end(),uu0.begin());
    std::copy(s.e1.begin(),s.e1.end(),uu1.begin());
    std::copy(s.s0.begin(),s.s0.end(),s1.begin());
    std::copy(s.iv0.begin(),s.iv0.end(),s.iv1.begin());
    if(this->stype==1u){
      tfel::math::change_basis(uu0,s.r);
      tfel::math::change_basis(uu1,s.r);
    }
    epx::EuroplexusReal ndt(1.);
    epx::EuroplexusInt  status(0);
    char msg[256];
    (this->fct)(&status,&(s1(0)),
		nstatv ==0 ? nullptr : &(s.iv1(0)),
		&Kt(0,0),&ndt,msg,
		&nstatv,&hv,&dt,&uu0(0),&uu1(0),
    		nprops ==0 ? nullptr : &(s.mprops1(0)),
		&nprops,&(s.esv0(0)),&(s.desv(0)),
		npredef==0 ? nullptr : &(s.esv0(1)),
		npredef==0 ? nullptr : &(s.desv(1)),
    		&npredef);
    if(status!=0){
      return {false,ndt};
    }
    if(this->stype==1u){
      auto tr = tfel::math::transpose(s.r);
      tfel::math::change_basis(s1,tr);
      if((h==MH::AXISYMMETRICAL)||(h==MH::PLANESTRESS)||(h==MH::PLANESTRAIN)){
	tfel::math::t2tost2<2u,real> D;
	std::copy(Kt.begin(),Kt.end(),D.begin());
#pragma message("HERE")
	//	D.changeBasis(tr);
	std::copy(D.begin(),D.end(),Kt.begin());
      } else if (h==MH::TRIDIMENSIONAL){
	tfel::math::t2tost2<3u,real> D;
	std::copy(Kt.begin(),Kt.end(),D.begin());
#pragma message("HERE")
	//	tfel::math::change_basis(D,tr);
	std::copy(D.begin(),D.end(),Kt.begin());
      }
    }
    std::copy_n(s1.begin(),s.s1.size(),s.s1.begin());
    return {true,ndt};
  }

  EuroplexusFiniteStrainBehaviour::~EuroplexusFiniteStrainBehaviour()
  {}
  
} // end of namespace mtest
