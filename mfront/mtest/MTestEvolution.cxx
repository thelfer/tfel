/*! 
 * \file  MTestEvolution.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 */

#include<string>
#include<stdexcept>

#include"MFront/MTestEvolution.hxx"

namespace mfront
{
  
  MTestEvolution::~MTestEvolution()
  {} // end of MTestEvolution::~MTestEvolution()

  MTestConstantEvolution::MTestConstantEvolution(const real v)
    : value(v)
  {}

  real
  MTestConstantEvolution::operator()(const real) const
  {
    return this->value;
  } // end of MTestConstantEvolution::operator()

  bool
  MTestConstantEvolution::isConstant() const
  {
    return true;
  } // end of MTestConstantEvolution::isConstant

  MTestConstantEvolution::~MTestConstantEvolution()
  {} // end of MTestConstantEvolution::~MTestConstantEvolution

  MTestLPIEvolution::MTestLPIEvolution(const std::vector<real>& t,
				       const std::vector<real>& v)
  {
    using namespace std;
    typedef map<real,real>::value_type MVType;
    vector<real>::const_iterator pt;
    vector<real>::const_iterator pv;
    if(t.size()!=v.size()){
      string msg("MTestLPIEvolution::MTestLPIEvolution : "
		 "the number of values of the times don't match "
		 "the number of values of the evolution");
      throw(runtime_error(msg));
    }
    pt = t.begin();
    pv = v.begin();
    while(pt!=t.end()){
      this->values.insert(MVType(*pt,*pv));
      ++pt;
      ++pv;
    }
  }

  real
  MTestLPIEvolution::operator()(const real t) const
  {
    using namespace std;
    map<real,real>::const_iterator p;
    if(this->values.empty()){
      string msg("LPILoadingEvolution::getValue : ");
      msg += "no values specified";
      throw(runtime_error(msg));
    }
    p = this->values.lower_bound(t);
    real x0;
    real x1;
    real y0;
    real y1;
    if(this->values.size()==1u){
      return this->values.begin()->second;
    }
    if(p==this->values.begin()){
      return p->second;
    } else if(p==this->values.end()){
      --p;
      return p->second;
    } else {
      x1 = p->first;
      y1 = p->second;
      --p;
      x0 = p->first;
      y0 = p->second;
    }
    return (y1-y0)/(x1-x0)*(t-x0)+y0;
  }

  bool
  MTestLPIEvolution::isConstant() const
  {
    return (this->values.size()==1);
  } // end of MTestLPIEvolution::isLPI

  MTestLPIEvolution::~MTestLPIEvolution()
  {}

} // end of namespace mfront

