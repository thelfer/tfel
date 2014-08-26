/*!
 * \file   StaticVarHandler.hxx
 * \brief  This file declares the StaticVarHandler class
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#ifndef _LIB_MFRONT_STATICVARHANDLER_H_
#define _LIB_MFRONT_STATICVARHANDLER_H_ 

#include<string>
#include<vector>

#include"TFEL/Config/TFELConfig.hxx"
#include"MFront/VarHandler.hxx"

namespace mfront{

  /*!
   * Class representing a static variable
   */
  struct TFEL_VISIBILITY_EXPORT StaticVarHandler
    : public VarHandler
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
    StaticVarHandler(const std::string&,
		     const std::string&,
		     const unsigned short,
		     const StaticVariableValueType);
  }; // end of struct MFrontParserBase<Child>::StaticVarHandler

  typedef std::vector<StaticVarHandler>  StaticVarContainer;

} // end of namespace mfront

#endif /* _LIB_MFRONT_STATICVARHANDLER_H */

