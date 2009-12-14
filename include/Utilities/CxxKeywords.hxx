/*!
 * \file   CxxKeyWords.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   16 Nov 2006
 */

#ifndef _LIB_TFEL_CXXKEYWORDS_H_
#define _LIB_TFEL_CXXKEYWORDS_H_ 

#include<string>

namespace tfel{

  namespace utilities{

    unsigned short
    getNumberOfCxxKeywords(void);

    bool
    isReservedCxxKeywords(const std::string&);

  } // end of namespace utilites

} // end of namespace tfel

#endif /* _LIB_TFEL_CXXKEYWORDS_H */

