/*!
 * \file   mfront/include/MFront/BoundsDescription.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 mar 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_BOUNDSDESCRIPTION_HXX_
#define LIB_MFRONT_BOUNDSDESCRIPTION_HXX_ 

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
    BoundsDescription(const BoundsDescription&) = default;
    BoundsDescription(BoundsDescription&&) = default;
    BoundsDescription&
    operator=(const BoundsDescription&) = default;
    BoundsDescription&
    operator=(BoundsDescription&&) = default;

    void
    writeBoundsChecks(std::ofstream&) const;

    ~BoundsDescription() noexcept;

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

#endif /* LIB_MFRONT_BOUNDSDESCRIPTION_HXX_ */

