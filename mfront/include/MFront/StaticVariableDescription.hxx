/*!
 * \file   StaticVariableDescription.hxx
 * \brief  This file declares the StaticVariableDescription class
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#ifndef _LIB_MFRONT_STATICVARIABLEDESCRIPTION_H_
#define _LIB_MFRONT_STATICVARIABLEDESCRIPTION_H_ 

#include<string>
#include<vector>

#include"TFEL/Config/TFELConfig.hxx"
#include"MFront/VariableDescription.hxx"

namespace mfront{

  /*!
   * Class representing a static variable
   */
  struct TFEL_VISIBILITY_EXPORT StaticVariableDescription
    : public VariableDescription
  {
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
    typedef double StaticVariableValueType;
#else 
    typedef long double StaticVariableValueType;
#endif /* defined _WIN32 || defined _WIN64 ||defined __CYGWIN__ */
    //! value of the static variable
    StaticVariableValueType value;
    /*!
     * Constructor
     * \param[in] type_       : variable type
     * \param[in] name_       : variable name
     * \param[in] lineNumber_ : if greater than 1, will be considered as an array
     * \param[in] value_      : value
     */
    StaticVariableDescription(const std::string&,
			      const std::string&,
			      const unsigned short,
			      const StaticVariableValueType);
  }; // end of struct MFrontParserBase<Child>::StaticVariableDescription

  //! a simple alias for backward compatibility
  typedef StaticVariableDescription StaticVarHandler;
  //! a simple alias
  typedef std::vector<StaticVariableDescription>  StaticVariableDescriptionContainer;
  //! a simple alias for backward compatibility
  typedef StaticVariableDescriptionContainer      StaticVarContainer;

} // end of namespace mfront

#endif /* _LIB_MFRONT_STATICVARIABLEDESCRIPTION_H */
