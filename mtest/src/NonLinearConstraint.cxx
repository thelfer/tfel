/*!
 * \file   NonLinearConstraint.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   03 march 2017
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cmath>
#include<sstream>

#include"MFront/MFrontLogStream.hxx"
#include"MTest/Behaviour.hxx"
#include"MTest/NonLinearConstraint.hxx"

namespace mtest
{

  NonLinearConstraint::NonLinearConstraint(const Behaviour& b_,
					   const std::string& f,
					   const EvolutionManager& evm,
					   const NormalisationPolicy p)
    : c(f),b(b_),np(p)
  {
    using namespace tfel::math;
    auto throw_if = [](const bool cond, const std::string& m){
      if(cond){throw(std::runtime_error("NonLinearConstraint::NonLinearConstraint: "+m));}
    };
    const auto all_dvs = this->b.getDrivingVariablesComponents();
    const auto all_tfs = this->b.getThermodynamicForcesComponents();
    for(const auto& v : this->c.getVariablesNames()){
      if(std::find(all_dvs.begin(),all_dvs.end(),v)!=all_dvs.end()){
	Variable dv = {v,b.getDrivingVariableComponentPosition(v),
	  std::dynamic_pointer_cast<Evaluator>(this->c.differentiate(v))};
	this->dvs.push_back(dv);
      } else if(std::find(all_tfs.begin(),all_tfs.end(),v)!=all_tfs.end()){
	Variable tf = {v,b.getThermodynamicForceComponentPosition(v),
	  std::dynamic_pointer_cast<Evaluator>(this->c.differentiate(v))};
	this->tfs.push_back(tf);
      } else {
	const auto pev = evm.find(v);
	throw_if(pev==evm.end(),"undefined evolution or variable '"+v+"'");
	this->evs[v] = pev->second;
      }
    }
    throw_if((this->dvs.empty())&&(this->tfs.empty()),"invalid constraint '"+f+"', "
	     "no thermodynamic force nor driving variable used in its definition");
    if(mfront::getVerboseMode()>=mfront::VERBOSE_LEVEL2){
      auto& log = mfront::getLogStream();
      log << "\nAdding non linear constraint defined by: '"+f+"'\n";
      if(!this->dvs.empty()){
	log << "List of related driving variables:";
	for(const auto& dv: this->dvs){
	  log << " '" << dv.name << "'";
	}
	log << '\n';
      }
      if(!this->tfs.empty()){
	log << "List of related thermodynamic forces: ";
	for(const auto& tf: this->tfs){
	  log << " '" << tf.name << "'";
	}
	log << '\n';
      }
    }
  } // end of NonLinearConstraint::NonLinearConstraint

  unsigned short
  NonLinearConstraint::getNumberOfLagrangeMultipliers() const
  {
    return 1u;
  } // end of NonLinearConstraint::getNumberOfLagrangeMultipliers

  void NonLinearConstraint::setValues(tfel::math::matrix<real>& K,
				      tfel::math::vector<real>& r,
				      const tfel::math::vector<real>&,
				      const tfel::math::vector<real>& u1,
				      const tfel::math::matrix<real>& k,
				      const tfel::math::vector<real>& s,
				      const unsigned short pos,
				      const unsigned short,
				      const real t,
				      const real dt,
				      const real a) const
  {
    using namespace tfel::material;
    using size_type = tfel::math::matrix<real>::size_type;
    auto throw_if = [](const bool tc, const std::string& m){
      if(tc){throw(std::runtime_error("NonLinearConstraint::setValues: "+m));}
    };
    auto eval = [this,&u1,&s,t,dt,throw_if](tfel::math::Evaluator& ev){
      for(const auto& v : this->c.getVariablesNames()){
	const auto pdv = std::find_if(this->dvs.begin(),this->dvs.end(),
				      [&v](const Variable& dv){
					return v==dv.name;
				      });
	const auto ptf = std::find_if(this->tfs.begin(),this->tfs.end(),
				      [&v](const Variable& tf){
					return v==tf.name;
				      });
	if(pdv!=this->dvs.end()){
	  ev.setVariableValue(v,u1[pdv->p]);
	} else if(ptf!=this->tfs.end()){
	  ev.setVariableValue(v,s[ptf->p]);
	} else {
	  const auto p = this->evs.find(v);
	  throw_if(p==this->evs.end(),"undefined evolution or variable '"+v+"'");
	  ev.setVariableValue(v,p->second->operator()(t+dt));
	}
      }
      return ev.getValue();
    };
    auto eval2 = [this,&u1,&s,t,dt,throw_if](tfel::math::Evaluator& ev){
      for(const auto& v : this->c.getVariablesNames()){
	const auto pdv = std::find_if(this->dvs.begin(),this->dvs.end(),
				      [&v](const Variable& dv){
					return v==dv.name;
				      });
	const auto ptf = std::find_if(this->tfs.begin(),this->tfs.end(),
				      [&v](const Variable& tf){
					return v==tf.name;
				      });
	if(pdv!=this->dvs.end()){
	  ev.setVariableValue(v,u1[pdv->p]);
	} else if(ptf!=this->tfs.end()){
	  ev.setVariableValue(v,s[ptf->p]);
	} else {
	  const auto p = this->evs.find(v);
	  throw_if(p==this->evs.end(),"undefined evolution or variable '"+v+"'");
	  ev.setVariableValue(v,p->second->operator()(t+dt));
	}
      }
      return ev.getValue();
    };
    // current value of the Lagrange multiplier
    const auto l = u1(pos);
    const auto nf = this->np==DRIVINGVARIABLECONSTRAINT ? a : real(1);
    for(const auto& dv : this->dvs){
      const auto dc_dv = eval(*(dv.d));
      const auto kv    = nf*dc_dv;
      K(pos,dv.p)     -= kv;
      K(dv.p,pos)     -= kv;
      r(dv.p)         -= kv*l;
    }
    for(const auto& tf : this->tfs){
      const auto ndv = this->b.getDrivingVariablesSize();
      const auto dc_dtf = eval(*(tf.d));
      const auto kv = nf*dc_dtf;
      if(this->b.getBehaviourType()==
	 MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR){
	if(tf.p<3){
	  for(unsigned short i=0;i!=ndv;++i){
	    K(pos,tf.p)-= kv*k(tf.p,i);
	    K(tf.p,pos)-= kv*k(tf.p,i);
	    r(tf.p)    -= kv*k(tf.p,i)*l;
	  }
	} else {
	  const size_type p1 = 2*tf.p-3u;
	  const size_type p2 = p1+1;
	  for(unsigned short i=0;i!=ndv;++i){
	    K(pos,p1)-= kv*k(p1,i);
	    K(p1,pos)-= kv*k(p1,i);
	    r(p1)    -= kv*k(p1,i)*l;
	    K(pos,p2)-= kv*k(p2,i);
	    K(p2,pos)-= kv*k(p2,i);
	    r(p2)    -= kv*k(p2,i)*l;
	  }
	}
      } else {
	for(unsigned short i=0;i!=ndv;++i){
	  K(pos,tf.p)-= kv*k(tf.p,i);
	  K(tf.p,pos)-= kv*k(tf.p,i);
	  r(tf.p)    -= kv*k(tf.p,i)*l;
	}
      }
    }
    r(pos) -= nf*eval(this->c);
  } // end of NonLinearConstraint::setValues

  bool NonLinearConstraint::checkConvergence(const tfel::math::vector<real>& e,
					     const tfel::math::vector<real>& s,
					     const real eeps,
					     const real seps,
					     const real t,
					     const real dt) const
  {
    auto throw_if = [](const bool cond, const std::string& m){
      if(cond){throw(std::runtime_error("NonLinearConstraint::checkConvergence: "+m));}
    };
    for(const auto& v : this->c.getVariablesNames()){
      const auto pdv = std::find_if(this->dvs.begin(),this->dvs.end(),
				    [&v](const Variable& dv){
				      return v==dv.name;
				    });
      const auto ptf = std::find_if(this->tfs.begin(),this->tfs.end(),
				    [&v](const Variable& tf){
				      return v==tf.name;
				    });
      if(pdv!=this->dvs.end()){
	this->c.setVariableValue(v,e[pdv->p]);
      } else if(ptf!=this->tfs.end()){
	this->c.setVariableValue(v,s[ptf->p]);
      } else {
	const auto p = this->evs.find(v);
	throw_if(p==this->evs.end(),"undefined evolution or variable '"+v+"'");
	this->c.setVariableValue(v,p->second->operator()(t+dt));
      }
    }
    if(this->np==DRIVINGVARIABLECONSTRAINT){
      return std::abs(this->c.getValue()) < eeps;
    }
    return std::abs(this->c.getValue()) < seps;
  }
  
  std::string
  NonLinearConstraint::getFailedCriteriaDiagnostic(const tfel::math::vector<real>& e,
						   const tfel::math::vector<real>& s,
						   const real eeps,
						   const real seps,
						   const real t,
						   const real dt) const
  {
    auto throw_if = [](const bool cond, const std::string& m){
      if(cond){throw(std::runtime_error("NonLinearConstraint::getFailedCriteriaDiagnostic: "+m));}
    };
    for(const auto& v : this->c.getVariablesNames()){
      const auto pdv = std::find_if(this->dvs.begin(),this->dvs.end(),
				    [&v](const Variable& dv){
				      return v==dv.name;
				    });
      const auto ptf = std::find_if(this->tfs.begin(),this->tfs.end(),
				    [&v](const Variable& tf){
				      return v==tf.name;
				    });
      if(pdv!=this->dvs.end()){
	this->c.setVariableValue(v,e[pdv->p]);
      } else if(ptf!=this->tfs.end()){
	this->c.setVariableValue(v,s[ptf->p]);
      } else {
	const auto p = this->evs.find(v);
	throw_if(p==this->evs.end(),"undefined evolution or variable '"+v+"'");
	this->c.setVariableValue(v,p->second->operator()(t+dt));
      }
    }
    const auto cv = this->c.getValue();
    const auto ev = this->np==DRIVINGVARIABLECONSTRAINT ? eeps : seps;
    std::ostringstream msg;
    msg << "imposed constraint not reached "
    	<< " (constraint value: " << cv << ", criteria : " << ev << ")";
    return msg.str();
  }

  NonLinearConstraint::~NonLinearConstraint() = default;

} // end of namespace mtest
