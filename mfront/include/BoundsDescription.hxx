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
    enum Category{
      Physical,
      Standard
    };
    enum BoundsType{
      Lower,
      Upper,
      LowerAndUpper
    };
    enum VarCategory{
      Coef,
      LocalVar,
      StateVar,
      ExternalStateVar,
      ComputedVar
    };

    BoundsDescription();

    void
    writeBoundsChecks(std::ofstream&) const;

    std::string    varName;
    SupportedTypes::TypeFlag varType;
    Category       category;
    VarCategory    varCategory;
    BoundsType     boundsType;
    long double lowerBound;
    long double upperBound;

    unsigned short lineNumber;

  }; // end of class BoundsDescription

} // end of namespace mfront

#endif /* _LIB_MFRONT_BOUNDSDESCRIPTION_HXX */

