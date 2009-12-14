/*!
 * \file   BoundsDescription.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   06 Mar 2007
 */

#include<string>
#include<stdexcept>
#include<limits>
#include"BoundsDescription.hxx"

namespace mfront{

  BoundsDescription::BoundsDescription()
    : lowerBound(std::numeric_limits<long double>::min()),
      upperBound(std::numeric_limits<long double>::max())
  {}

  void
  BoundsDescription::writeBoundsCheking(std::ofstream& file) const
  {
    using namespace std;
    if(this->boundsType==Lower){
      file << "BoundCheck<N>::lowerBoundCheck(\""
	   << this->varName << "\"," << this->varName << ","
	   << "static_cast<real>(" << this->lowerBound << "));\n";
      if(this->varCategory==ExternalStateVar){
	file << "BoundCheck<N>::lowerBoundCheck(\""
	     << this->varName << "+d" << this->varName << "\"," 
	     << this->varName << "+d" << this->varName << ","
	     << "static_cast<real>(" << this->lowerBound << "));\n";
      }
    } else if(this->boundsType==Upper){
      file << "BoundCheck<N>::upperBoundCheck(\""
	   << this->varName << "\"," << this->varName << ","
	   << "static_cast<real>(" << this->upperBound << "));\n";
      if(this->varCategory==ExternalStateVar){
	file << "BoundCheck<N>::upperBoundCheck(\""
	     << this->varName << "+d" << this->varName << "\"," 
	     << this->varName << "+d" << this->varName << ","
	     << "static_cast<real>(" << this->upperBound << "));\n";
      }
    } else if(this->boundsType==LowerAndUpper){
      file << "BoundCheck<N>::lowerAndUpperBoundCheck(\""
	   << this->varName << "\"," << this->varName    << ","
	   << "static_cast<real>("   << this->lowerBound << "),"
	   << "static_cast<real>("   << this->upperBound << "));\n";
      if(this->varCategory==ExternalStateVar){
	file << "BoundCheck<N>::lowerAndUpperBoundCheck(\""
	     << this->varName << "+d" << this->varName << "\"," 
	     << this->varName << "+d" << this->varName << ","
	     << "static_cast<real>("   << this->lowerBound << "),"
	     << "static_cast<real>("   << this->upperBound << "));\n";
      }
    } else {
      throw(runtime_error("BoundsDescription::writeBoundsChecking : internal error"));
    }
  }
  
} // end of namespace mfront
