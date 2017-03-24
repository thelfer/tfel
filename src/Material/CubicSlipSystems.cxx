/*!
 * \file   CubicSlipSystems.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   22/03/2017
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Material/CubicSlipSystems.hxx"

namespace tfel{

  namespace material{

    static int scalar_product(const CubicSlipSystems::vec3d& v1,
			      const CubicSlipSystems::vec3d& v2){
      return v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2];
    }

    static bool perpendicular(const CubicSlipSystems::vec3d& v1,
			      const CubicSlipSystems::vec3d& v2){
      return scalar_product(v1,v2)==0;
    }

    static bool colinear(const CubicSlipSystems::vec3d& v1,
			 const CubicSlipSystems::vec3d& v2)
    {
      const auto v3 = CubicSlipSystems::vec3d{v1[1]*v2[2]-v1[2]*v2[1],
					      v1[2]*v2[0]-v1[0]*v2[2],
					      v1[0]*v2[1]-v1[1]*v2[0]};
      return scalar_product(v3,v3)==0;
    }

    static std::vector<CubicSlipSystems::vec3d>
    permutation(const CubicSlipSystems::vec3d& v)
    {
      std::vector<CubicSlipSystems::vec3d> r;
      for(decltype(v.size()) i=0;i!=3;++i){
	for(decltype(v.size()) j=0;j!=3;++j){
	  if(i==j){continue;};
	  for(decltype(v.size()) k=0;k!=3;++k){
	    if((k==j)||(k==i)){continue;};
	    r.push_back({v[i],v[j],v[k]});
	  }
	}
      }
      return r;
    }

    static std::vector<CubicSlipSystems::vec3d>
    symmetry(const std::vector<CubicSlipSystems::vec3d>& vs)
    {
      std::vector<CubicSlipSystems::vec3d> r;
      auto exe = [&r](const CubicSlipSystems::vec3d& v,const CubicSlipSystems::vec3d::size_type i){
	if(v[i]!=0){
	  auto v2 = v;
	  v2[i] *= -1;
	  r.insert(r.end(),{v,v2});
	}
      };
      for(const auto& v : vs){
	exe(v,0);
	exe(v,1);
	exe(v,2);
      }
      return r;
    }

    static std::vector<std::pair<CubicSlipSystems::vec3d,CubicSlipSystems::vec3d>>
    merge(const std::vector<CubicSlipSystems::vec3d>& ns,
	  const std::vector<CubicSlipSystems::vec3d>& ds){
      std::vector<std::pair<CubicSlipSystems::vec3d,CubicSlipSystems::vec3d>> r;
      for(const auto& n:ns){
	for(const auto& d:ds){
	  if(perpendicular(n,d)){
	    r.push_back({n,d});
	  }
	}
      }
      return r;
    }
    
    std::vector<std::pair<CubicSlipSystems::vec3d,
			  CubicSlipSystems::vec3d>>
    CubicSlipSystems::generate(const vec3d& n,
			      const vec3d& d){
      std::vector<std::pair<vec3d,vec3d>> r;
      const auto cs = merge(symmetry(permutation(n)),
			    symmetry(permutation(d)));
      std::vector<bool> b(cs.size(),true);
      for(decltype(cs.size()) i=0;i!=cs.size();++i){
	for(decltype(cs.size()) j=i+1;j!=cs.size();++j){
	  if(!b[j]){continue;};
	  if(colinear(cs[i].first,cs[j].first)&&
	     colinear(cs[i].second,cs[j].second)){
	    b[j]=false;
	  }
	}
      }
      for(decltype(cs.size()) i=0;i!=cs.size();++i){
	if(b[i]){
	  r.push_back(cs[i]);
	}
      }
      return r;
    } // end of CubicSlipSystems::generate
    
  } // end of namespace material

} // end of namespace tfel
