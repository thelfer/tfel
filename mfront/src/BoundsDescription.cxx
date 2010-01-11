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
    : category(Standard),
      lowerBound(std::numeric_limits<long double>::min()),
      upperBound(std::numeric_limits<long double>::max())
  {}

  void
  BoundsDescription::writeBoundsChecks(std::ofstream& file) const
  {
    using namespace std;
    if(this->category==Standard){
      if(this->boundsType==Lower){
	file << "BoundsCheck<N>::lowerBoundCheck(\""
	     << this->varName << "\",this->" << this->varName << ","
	     << "static_cast<real>(" << this->lowerBound << "),"
	     << endl << "this->policy);\n";
	if(this->varCategory==ExternalStateVar){
	  file << "BoundsCheck<N>::lowerBoundCheck(\""
	       << this->varName << "+d" << this->varName << "\"," 
	       << this->varName << "+d" << this->varName << ","
	       << "static_cast<real>(" << this->lowerBound << "),"
	       << endl << "this->policy);\n";
	}
      } else if(this->boundsType==Upper){
	file << "BoundsCheck<N>::upperBoundCheck(\""
	     << this->varName << "\",this->" << this->varName << ","
	     << "static_cast<real>(" << this->upperBound << "),"
	     << endl << "this->policy);\n";
	if(this->varCategory==ExternalStateVar){
	  file << "BoundsCheck<N>::upperBoundCheck(\""
	       << this->varName << "+d" << this->varName << "\"," 
	       << this->varName << "+d" << this->varName << ","
	       << "static_cast<real>(" << this->upperBound << "),"
	       << endl << "this->policy);\n";
	}
      } else if(this->boundsType==LowerAndUpper){
	file << "BoundsCheck<N>::lowerAndUpperBoundsChecks(\""
	     << this->varName << "\",this->" << this->varName    << ","
	     << "static_cast<real>("   << this->lowerBound << "),"
	     << "static_cast<real>("   << this->upperBound << "),"
	     << endl << "this->policy);\n";
	if(this->varCategory==ExternalStateVar){
	  file << "BoundsCheck<N>::lowerAndUpperBoundsChecks(\""
	       << this->varName << "+d" << this->varName << "\"," 
	       << this->varName << "+d" << this->varName << ","
	       << "static_cast<real>("   << this->lowerBound << "),"
	       << "static_cast<real>("   << this->upperBound << "),"
	       << endl << "this->policy);\n";
	}
      }
    } else if(this->category==Physical){
      if(this->boundsType==Lower){
	file << "BoundsCheck<N>::lowerBoundCheck(\""
	     << this->varName << "\",this->" << this->varName << ","
	     << "static_cast<real>(" << this->lowerBound << "));\n";
	if(this->varCategory==ExternalStateVar){
	  file << "BoundsCheck<N>::lowerBoundCheck(\""
	       << this->varName << "+d" << this->varName << "\"," 
	       << this->varName << "+d" << this->varName << ","
	       << "static_cast<real>(" << this->lowerBound << "));\n";
	}
      } else if(this->boundsType==Upper){
	file << "BoundsCheck<N>::upperBoundCheck(\""
	     << this->varName << "\",this->" << this->varName << ","
	     << "static_cast<real>(" << this->upperBound << "));\n";
	if(this->varCategory==ExternalStateVar){
	  file << "BoundsCheck<N>::upperBoundCheck(\""
	       << this->varName << "+d" << this->varName << "\"," 
	       << this->varName << "+d" << this->varName << ","
	       << "static_cast<real>(" << this->upperBound << "));\n";
	}
      } else if(this->boundsType==LowerAndUpper){
	file << "BoundsCheck<N>::lowerAndUpperBoundsChecks(\""
	     << this->varName << "\",this->" << this->varName    << ","
	     << "static_cast<real>("   << this->lowerBound << "),"
	     << "static_cast<real>("   << this->upperBound << "));\n";
	if(this->varCategory==ExternalStateVar){
	  file << "BoundsCheck<N>::lowerAndUpperBoundsChecks(\""
	       << this->varName << "+d" << this->varName << "\"," 
	       << this->varName << "+d" << this->varName << ","
	       << "static_cast<real>("   << this->lowerBound << "),"
	       << "static_cast<real>("   << this->upperBound << "));\n";
	}
      }
    } else {
      throw(runtime_error("BoundsDescription::writeBoundsChecks : internal error"));
    }
  }
  
} // end of namespace mfront
