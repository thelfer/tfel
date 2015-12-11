/*!
 * \file   PipeQuadraticElement.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   10 déc. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Material/ModellingHypothesis.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MTest/Behaviour.hxx"
#include"MTest/BehaviourWorkSpace.hxx"
#include"MTest/CurrentState.hxx"
#include"MTest/StructureCurrentState.hxx"
#include"MTest/PipeQuadraticElement.hxx"

namespace mtest{

  constexpr const real PipeQuadraticElement::abs_pg;
  constexpr const real PipeQuadraticElement::wg[3];
  constexpr const real PipeQuadraticElement::pg_radii[3];

  real
  PipeQuadraticElement::interpolate(const real v0,const real v1,
				    const real v2,const real x)
  {
    return ((1.-x)*x*v0+(1+x)*x*v2+2*(1.+x)*(1.-x)*v1)/2;
  } // end of PipeQuadraticElement::interpolate
  
  void
  PipeQuadraticElement::computeStrain(StructureCurrentState& scs,
				      const PipeMesh& m,
				      const tfel::math::vector<real>& u,
				      const size_t i,
				      const bool b){
    // number of elements
    const auto ne = size_t(m.number_of_elements);
    // inner radius
    const auto Ri = m.inner_radius;
    // outer radius
    const auto Re = m.outer_radius;
    // radius increment
    const auto dr = (Re-Ri)/ne;
    // radial position of the first node
    const auto r0 = Ri+dr*i;
    // radial position of the second node
    const auto r1 = Ri+dr*(i+1)/2;
    // radial position of the second node
    const auto r2 = Ri+dr*(i+1);
    // radial displacement of the first node
    const auto ur0 = u[2*i];
    // radial displacement of the second node
    const auto ur1 = u[2*i+1];
    // radial displacement of the third node
    const auto ur2 = u[2*i+2];
    // axial strain
    const auto& ezz = u[ne+1];
    // loop over Gauss point
    for(const auto g : {0,1,2}){
      // Gauss point position in the reference element
      const auto pg = pg_radii[g];
      // inverse of the jacobian
      const auto iJ = 1/(r0*(pg-0.5)+r2*(pg+0.5)-2*r1*pg);
      // radial position of the Gauss point
      const auto rg = 0.5*((1-pg)*r0+(1+pg)*r2);
      // current state
      auto& s = scs.istates[3*i+g];
      // strain
      auto& e = b ? s.e1 : s.e0;
      e[0] = ((pg-0.5)*ur0+(pg+0.5)*ur2-2.*pg*ur1)*iJ;
      e[1] = ezz;
      e[2] = interpolate(ur0,ur1,ur2,pg)/rg;
    }    
  } // end of PipeQuadraticElement::computeStrain
  
    bool
    PipeQuadraticElement::updateStiffnessMatrixAndInnerForces(tfel::math::matrix<real>& k,
  							 tfel::math::vector<real>& r,
  							 StructureCurrentState& scs,
  							 const Behaviour& b,
  							 const tfel::math::vector<real>& u1,
  							 const PipeMesh& m,
  							 const real dt,
  							 const StiffnessMatrixType mt,
  							 const size_t i){
      //! a simple alias
      using ModellingHypothesis = tfel::material::ModellingHypothesis;
      constexpr const real pi = 3.14159265358979323846;
      // number of elements
      const auto ne = size_t(m.number_of_elements);
      // number of nodes
      const auto n  = ne+1;
      // inner radius
      const auto Ri = m.inner_radius;
      // outer radius
      const auto Re = m.outer_radius;
      // radius increment
      const auto dr = (Re-Ri)/ne;
      // radial position of the first node
      const auto r0 = Ri+dr*i;
      // radial position of the second node
      const auto r1 = Ri+dr*(i+1)/2;
      // radial position of the second node
      const auto r2 = Ri+dr*(i+1);
      /* inner forces */
      auto& bwk = scs.getBehaviourWorkSpace();
      // compute the strain
      computeStrain(scs,m,u1,i,true);
      // loop over Gauss point
      for(const auto g : {0,1,2}){
	// Gauss point position in the reference element
	const auto pg = pg_radii[g];
	// radial position of the Gauss point
	const auto rg = 0.5*((1-pg)*r0+(1+pg)*r2);
	// jacobian of the transformation
	const auto J = r0*(pg-0.5)+r2*(pg+0.5)-2*r1*pg;
	// current state
	auto& s = scs.istates[3*i+g];
	if(!b.integrate(s,bwk,ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,dt,mt)){
	  if(mfront::getVerboseMode()>mfront::VERBOSE_QUIET){
	    auto& log = mfront::getLogStream();
	    log << "PipeQuadraticElement::computeStiffnessMatrixAndResidual : "
		<< "behaviour intregration failed" << std::endl;
	  }
	  return false;
	}
	// stress tensor
	const auto pi_rr = s.s1[0];
	const auto pi_zz = s.s1[1];
	const auto pi_tt = s.s1[2];
      	const auto w = 2*pi*wg[g]*J;
      	// innner forces
      	r[2*i]   += w*(rg*pi_rr*(pg-0.5)/J+pi_tt*(-0.5*(1.-pg)*pg));
	r[2*i+1] += w*(rg*pi_rr*(-2.*pg)/J+pi_tt*((1.+pg)*(1.-pg)));
	r[2*i+2] += w*(rg*pi_rr*(pg+0.5)/J+pi_tt*(0.5*(1+pg)*pg));
      	// axial forces
      	r[n]   += w*rg*pi_zz;
	// jacobian matrix
	if(mt!=StiffnessMatrixType::NOSTIFFNESS){
	  const auto& bk = bwk.k;
      	  const real de10_dur0 = (pg-0.5)/J;
      	  const real de12_dur0 = (1.-pg)*pg/(2*rg);
      	  const real de10_dur1 = -2.*pg/J;
      	  const real de12_dur1 = (1.+pg)*(1.-pg)/rg;
      	  const real de10_dur2 = (0.5+pg)/J;
      	  const real de12_dur2 = (1+pg)*pg/(2*rg);
	  // k(2*i,2*i)   += w*(bk(0,0)*de10_dur0*(-rg/dr)+
	  // 		 bk(0,2)*de12_dur0*(-rg/dr)+
	  // 		 bk(2,0)*de10_dur0*(1-pg)/2+
	  // 		 bk(2,2)*de12_dur0*(1-pg)/2);
	  // k(2*i,2*i+1) += w*(bk(0,0)*de10_dur1*(-rg/dr)+
	  // 		 bk(0,2)*de12_dur1*(-rg/dr)+
	  // 		 bk(2,0)*de10_dur1*(1-pg)/2+
	  // 		 bk(2,2)*de12_dur1*(1-pg)/2);
	  // k(2*i,n)   += w*(bk(0,1)*(-rg/dr)+bk(2,1)*(1-pg)/2);
	  // k(2*i+1,2*i) += w*(bk(0,0)*de10_dur0*( rg/dr)+
	  // 		 bk(0,2)*de12_dur0*( rg/dr)+
	  // 		 bk(2,0)*de10_dur0*(1+pg)/2+
	  // 		 bk(2,2)*de12_dur0*(1+pg)/2);
	  // k(2*i+1,2*i+1) += w*(bk(0,0)*de10_dur1*( rg/dr)+
	  // 		       bk(0,2)*de12_dur1*( rg/dr)+
	  // 		       bk(2,0)*de10_dur1*(1+pg)/2+
	  // 		       bk(2,2)*de12_dur1*(1+pg)/2);
	  // k(i+1,n)   += w*(bk(0,1)*( rg/dr)+bk(2,1)*(1+pg)/2);
	  //       	  axial forces
      	  // k(n,i)    += w*rg*(bk(1,0)*de10_dur0+bk(1,2)*de12_dur0);
      	  // k(n,i+1)  += w*rg*(bk(1,0)*de10_dur1+bk(1,2)*de12_dur1);
      	  k(n,n)    += w*rg*bk(1,1);
	}
      }
      return true;
    }
} // end of namespace mtest
