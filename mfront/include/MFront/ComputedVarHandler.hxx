/*!
 * \file   ComputedVarHandler.hxx
 * \brief  This file declares the ComputedVarHandler class
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#ifndef _LIB_MFRONT_COMPUTEDVARHANDLER_H_
#define _LIB_MFRONT_COMPUTEDVARHANDLER_H_ 

#include<string>
#include<vector>

#include"TFEL/Config/TFELConfig.hxx"
#include"MFront/VarHandler.hxx"

namespace mfront{

  struct TFEL_VISIBILITY_EXPORT ComputedVarHandler
    : public VarHandler
  {
    std::string description;
        
    ComputedVarHandler(const std::string&,const std::string&,
		       const unsigned short,const std::string&);

  }; // end of struct MFrontParserBase<Child>::ComputedVarHandler

  typedef std::vector<ComputedVarHandler> ComputedVarContainer;

} // end of namespace mfront

#endif /* _LIB_MFRONT_COMPUTEDVARHANDLER_H */

