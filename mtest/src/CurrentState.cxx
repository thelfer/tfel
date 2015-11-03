/*!
 * \file   CurrentState.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   23 oct. 2015
 */

#include"MTest/Evolution.hxx"
#include"MTest/Behaviour.hxx"
#include"MTest/CurrentState.hxx"

namespace mtest{

  CurrentState::CurrentState() = default;

  CurrentState::CurrentState(const CurrentState&) = default;

  CurrentState::CurrentState(CurrentState&&) = default;

  CurrentState& CurrentState::operator=(const CurrentState&) = default;

  CurrentState& CurrentState::operator=(CurrentState&&) = default;

  CurrentState::~CurrentState() noexcept = default;

  void allocate(CurrentState& s,const Behaviour& b,
		const tfel::material::ModellingHypothesis::Hypothesis h)
  {
    const auto mpnames(b.getMaterialPropertiesNames());
    const auto esvnames(b.getExternalStateVariablesNames());
    // clear
    s.s_1.clear();
    s.s0.clear();
    s.s1.clear();
    s.e0.clear();
    s.e1.clear();
    s.e_th0.clear();
    s.e_th1.clear();
    s.mprops1.clear();
    s.iv_1.clear();
    s.iv0.clear();
    s.iv1.clear();
    s.esv0.clear();
    s.desv.clear();
    // resizing
    const auto ndv = b.getDrivingVariablesSize(h);
    const auto nth = b.getThermodynamicForcesSize(h);
    s.s_1.resize(nth,0.);
    s.s0.resize(nth,0.);
    s.s1.resize(nth,0.);
    s.e0.resize(ndv,0.);
    s.e1.resize(ndv,0.);
    s.e_th0.resize(ndv,0.);
    s.e_th1.resize(ndv,0.);
    s.mprops1.resize(mpnames.size());
    s.iv_1.resize(b.getInternalStateVariablesSize(h),0.);
    s.iv0.resize(s.iv_1.size(),0.);
    s.iv1.resize(s.iv0.size(),0.);
    s.esv0.resize(esvnames.size(),0.);
    s.desv.resize(esvnames.size(),0.);
  }
  
  void
  computeMaterialProperties(CurrentState& s,
			    const EvolutionManager& evm,
			    const EvolutionManager& dvm,
			    const std::vector<std::string>& mpnames,
			    const real t,
			    const real dt){
    if(s.mprops1.size()!=mpnames.size()){
      throw(std::runtime_error("computeMaterialProperties:"
			       "CurrentState variable was not initialized appropriately"));
    }
    std::vector<real>::size_type i=0;
    for(const auto& mpn : mpnames){
      auto pev = evm.find(mpn);
      if(pev!=evm.end()){
	const auto& ev = *(pev->second);
	s.mprops1[i] = ev(t+dt);
      } else {
	pev = dvm.find(mpn);
	if(pev!=dvm.end()){
	  const auto& ev = *(pev->second);
	  s.mprops1[i] = ev(t+dt);
	} else {
	  throw(std::runtime_error("computeMaterialProperties: "
				   "no evolution named '"+mpn+"'"));
	}
      }
      ++i;
    }
  } // end of computeMaterialProperties
  
  void
  computeExternalStateVariables(CurrentState& s,
				const EvolutionManager& evm,
				const std::vector<std::string>& esvnames,
				const real t,
				const real dt){
    if((s.esv0.size()!=esvnames.size())||
       (s.desv.size()!=esvnames.size())){
      throw(std::runtime_error("computeExternalStateVariables:"
			       "CurrentState variable was not initialized appropriately"));
    }
    std::vector<real>::size_type i=0;
    for(const auto& evn : esvnames){
      auto pev = evm.find(evn);
      if(pev==evm.end()){
	throw(std::runtime_error("computeExternalStateVariables:"
				 "no evolution named '"+evn+"'"));
      }
      const auto& ev = *(pev->second);
      s.esv0[i] = ev(t);
      s.desv[i] = ev(t+dt)-s.esv0[i];
      ++i;
    }
  } // end of computeExternalStateVariables
  
} // end of namespace mtest


