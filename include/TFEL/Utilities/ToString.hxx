/*!
 * \file   include/TFEL/Utilities/ToString.hxx
 * \brief  This file implements the ToString function.
 * \author Helfer Thomas
 * \date   17 Jul 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_TOSTRING_H_
#define _LIB_TFEL_TOSTRING_H_ 

#include<string>
#include<sstream>

namespace tfel{
  
  namespace utilities{

    /*!
     * \fn    ToString
     * \brief  convert an object to a string trough an ostringstream.
     *
     * \param  T, type of the objects to be converted.
     * \param  src, object to be converted.
     * \return the string representation of the object.
     *
     * \pre T.operator<<() must be defined.
     */
    template<typename T>
    std::string
    ToString(const T&);

  } // end of namespace utilities

} // end of namespace tfel

#include"TFEL/Utilities/ToString.ixx"

#endif /* _LIB_TFEL_TOSTRING_H */

