/*!
 * \file   BoundsDescription.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 mar 2007
 */

#ifndef _LIB_MFRONT_BOUNDSDESCRIPTION_HXX_
#define _LIB_MFRONT_BOUNDSDESCRIPTION_HXX_ 

#include<fstream>
#include"SupportedTypes.hxx"

namespace mfront{

  struct BoundsDescription
  {
    enum BoundsType{Lower,Upper,LowerAndUpper};
    enum VarCategory{Coef,LocalVar,StateVar,ExternalStateVar,ComputedVar};

    SupportedTypes::TypeFlag varType;
    VarCategory    varCategory;
    BoundsType     boundsType;
    std::string    varName;
    unsigned short lineNumber;

    long double lowerBound;

    long double upperBound;

    BoundsDescription();

    void
    writeBoundsCheking(std::ofstream&) const;

  }; // end of class BoundsDescription

} // end of namespace mfront

#endif /* _LIB_MFRONT_BOUNDSDESCRIPTION_HXX */

