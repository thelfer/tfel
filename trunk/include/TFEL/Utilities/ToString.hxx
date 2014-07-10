/*!
 * \file   ToString.hxx
 * \brief  This file implements the ToString function.
 * \author Helfer Thomas
 * \date   17 Jul 2006
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

