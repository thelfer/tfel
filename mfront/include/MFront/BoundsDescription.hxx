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

#include"TFEL/Config/TFELConfig.hxx"
#include"MFront/SupportedTypes.hxx"

namespace mfront{

  /*!
   * a structure in charge of describing bounds
   */
  struct TFEL_VISIBILITY_EXPORT BoundsDescription
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
      MaterialProperty,
      LocalVariable,
      StateVariable,
      ExternalStateVariable,
      ExternalStateVariableII
    };

    BoundsDescription();

    void
    writeBoundsChecks(std::ofstream&) const;

    // name of the variable on which the bounds acts
    std::string    name;
    // component. If component is egal to -1, bounds acts of all
    // components
    short component;
    // name of the variable and the component name
    std::string    varName;
    
    SupportedTypes::TypeFlag varType;
    Category       category;
    VarCategory    varCategory;
    unsigned short arraySize;
    BoundsType     boundsType;
    long double    lowerBound;
    long double    upperBound;
    unsigned int lineNumber;

  protected:

    void
    writeBoundsChecks(std::ofstream&,
		      const std::string&) const;

  }; // end of class BoundsDescription

} // end of namespace mfront

#endif /* _LIB_MFRONT_BOUNDSDESCRIPTION_HXX */

