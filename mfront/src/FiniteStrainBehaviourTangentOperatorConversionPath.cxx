/*! 
 * \file  FiniteStrainBehaviourTangentOperatorConversionPath.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 juin 2014
 */

#include<algorithm>
#include"MFront/FiniteStrainBehaviourTangentOperatorConversionPath.hxx"

namespace mfront
{
  
  FiniteStrainBehaviourTangentOperatorConversionPath::const_iterator
  FiniteStrainBehaviourTangentOperatorConversionPath::find(const FiniteStrainBehaviourTangentOperatorConversionPath::TangentOperatorFlag t) const
  {
    const_iterator p;
    for(p=this->begin();p!=this->end();++p){
      if(p->to()==t){
	return p;
      }
    }
    return p;
  } // end of FiniteStrainBehaviourTangentOperatorConversionPath::find

  void
  FiniteStrainBehaviourTangentOperatorConversionPath::getConversionsPath(std::vector<FiniteStrainBehaviourTangentOperatorConversionPath>& r,
									 const TangentOperatorFlag& b,
									 const std::vector<TangentOperatorFlag>& cp, //<! current path
									 const std::vector<TangentOperatorFlag>& k,
									 const std::vector<FiniteStrainBehaviourTangentOperatorConversion>& converters)
  {
    using std::find;
    vector<TangentOperatorFlag> current_path(cp);
    current_path.push_back(b);
    vector<FiniteStrainBehaviourTangentOperatorConversion>::const_iterator pc;
    for(pc=converters.begin();pc!=converters.end();++pc){
      const FiniteStrainBehaviourTangentOperatorConversion& c = *pc;
      if(c.from()==b){
	if(!((find(k.begin(),k.end(),c.to())!=k.end())||
	     (find(cp.begin(),cp.end(),c.to())!=cp.end()))){
	  // not an known operator
	  vector<FiniteStrainBehaviourTangentOperatorConversionPath> paths;
	  FiniteStrainBehaviourTangentOperatorConversionPath::getConversionsPath(paths,c.to(),current_path,k,converters);
	  if(!paths.empty()){
	    vector<FiniteStrainBehaviourTangentOperatorConversionPath>::const_iterator pp;
	    for(pp=paths.begin();pp!=paths.end();++pp){
	      r.push_back(*pp);
	      r.back().insert(r.back().begin(),c);
	    }
	  } else {
	    FiniteStrainBehaviourTangentOperatorConversionPath path;
	    path.push_back(c);
	    r.push_back(path);
	  }
	}
      }
    }
  } // end of FiniteStrainBehaviourTangentOperatorConversionPath::getConversionsPath

  std::vector<FiniteStrainBehaviourTangentOperatorConversionPath>									 
  FiniteStrainBehaviourTangentOperatorConversionPath::getConversionsPath(const TangentOperatorFlag& b,
									 const std::vector<TangentOperatorFlag>& k,
									 const std::vector<FiniteStrainBehaviourTangentOperatorConversion>& converters)
  {
    using std::vector; 
    vector<FiniteStrainBehaviourTangentOperatorConversionPath> r;
    FiniteStrainBehaviourTangentOperatorConversionPath::getConversionsPath(r,b,k,vector<TangentOperatorFlag>(),converters);
    return r;
  } // end of FiniteStrainBehaviourTangentOperatorConversionPath::getConversionsPath

  FiniteStrainBehaviourTangentOperatorConversionPath
  FiniteStrainBehaviourTangentOperatorConversionPath::getShortestPath(const std::vector<FiniteStrainBehaviourTangentOperatorConversionPath>& paths,
								      const FiniteStrainBehaviourTangentOperatorConversionPath::TangentOperatorFlag& t)
  {
    using namespace std;
    // shortest path 
    FiniteStrainBehaviourTangentOperatorConversionPath path;
    vector<FiniteStrainBehaviourTangentOperatorConversionPath>::const_iterator pp;
    for(pp=paths.begin();pp!=paths.end();++pp){
      FiniteStrainBehaviourTangentOperatorConversionPath::const_iterator pc = pp->find(t);
      if(pc!=pp->end()){
	++pc;
	// we found a conversion path candidate
	if(path.empty()){
	  // no previous path
	  path.insert(path.begin(),pp->begin(),pc);
	} else {
	  // a previous path exists, select the shortest
	  const FiniteStrainBehaviourTangentOperatorConversionPath::size_type l = pc-pp->begin();
	  if(l<path.size()){
	    path.clear();
	    path.insert(path.begin(),pp->begin(),pc);
	  }
	}
      }
    }
    return path;
  } // end of FiniteStrainBehaviourTangentOperatorConversionPath::getShortestPath
  
} // end of namespace mfront
