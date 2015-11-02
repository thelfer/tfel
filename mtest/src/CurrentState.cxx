/*!
 * \file   CurrentState.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   23 oct. 2015
 */

#include"MTest/Evolution.hxx"
#include"MTest/CurrentState.hxx"

namespace mtest{

  CurrentState::CurrentState() = default;

  CurrentState::CurrentState(const CurrentState&) = default;

  CurrentState::CurrentState(CurrentState&&) = default;

  CurrentState& CurrentState::operator=(const CurrentState&) = default;

  CurrentState& CurrentState::operator=(CurrentState&&) = default;

  CurrentState::~CurrentState() noexcept = default;

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


