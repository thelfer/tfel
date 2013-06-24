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
    //! value of the static variable
    long double value;
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
		     const long double);
  }; // end of struct MFrontParserBase<Child>::StaticVarHandler

  typedef std::vector<StaticVarHandler>  StaticVarContainer;

} // end of namespace mfront

#endif /* _LIB_MFRONT_STATICVARHANDLER_H */

