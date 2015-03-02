/*!
 * \file   include/TFEL/Utilities/CxxKeywords.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   16 Nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_CXXKEYWORDS_H_
#define LIB_TFEL_CXXKEYWORDS_H_ 

#include<string>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  namespace utilities{

    /*!
     * \return the number of c++ keywords
     */
    TFELUTILITIES_VISIBILITY_EXPORT 
    unsigned short
    getNumberOfCxxKeywords(void);

    /*!
     * \return true if the specified string is a of c++ keyword
     * \param[in] s : string to be tested
     */
    TFELUTILITIES_VISIBILITY_EXPORT 
    bool
    isReservedCxxKeywords(const std::string&);

  } // end of namespace utilites

} // end of namespace tfel

#endif /* LIB_TFEL_CXXKEYWORDS_H_ */

