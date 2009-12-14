/*!
 * \file   GenTypeBase.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 fév 2008
 */

#ifndef _LIB_TFEL_GENTYPEBASE_IXX_
#define _LIB_TFEL_GENTYPEBASE_IXX_ 

namespace tfel{
  
  namespace utilities{

    template<typename List>
    std::ostream&
    operator<<(std::ostream& os,const GenTypeBase<List>& g)
    {
      g.print(os);
      return os;
    }

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_GENTYPEBASE_IXX */

