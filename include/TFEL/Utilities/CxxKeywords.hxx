/*!
 * \file   CxxKeyWords.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   16 Nov 2006
 */

#ifndef _LIB_TFEL_CXXKEYWORDS_H_
#define _LIB_TFEL_CXXKEYWORDS_H_ 

#include<string>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  namespace utilities{

    TFEL_VISIBILITY_EXPORT 
    unsigned short
    getNumberOfCxxKeywords(void);

    TFEL_VISIBILITY_EXPORT 
    bool
    isReservedCxxKeywords(const std::string&);

  } // end of namespace utilites

} // end of namespace tfel

#endif /* _LIB_TFEL_CXXKEYWORDS_H */

