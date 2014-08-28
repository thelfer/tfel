/*!
 * \file   include/TFEL/Utilities/ToString.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 fév 2008
 */

#ifndef _LIB_TFEL_TOSTRING_IXX_
#define _LIB_TFEL_TOSTRING_IXX_ 

namespace tfel{
  
  namespace utilities{

    template<typename T>
    std::string
    ToString(const T& src)
    {
      std::ostringstream os;
      os << src;
      return os.str();
    }

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_TOSTRING_IXX */

