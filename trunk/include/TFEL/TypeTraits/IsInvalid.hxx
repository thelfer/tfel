/*!
 * \file   IsInvalid.hxx
 * \brief  This file declares the IsInvalid traits class.
 * \author Helfer Thomas
 * \date   14 oct 2006
 */

#ifndef _LIB_TFEL_ISINVALID_HXX_
#define _LIB_TFEL_ISINVALID_HXX_ 

#include"TFEL/Metaprogramming/InvalidType.hxx"

namespace tfel{

  namespace typetraits{

    /*!
     * \brief  Traits class which says if its argument is invalid.
     *
     * \param  T, type to be tested.
     * \return bool cond, true if T is invalid, false otherwise.
     *
     * \author Helfer Thomas
     * \date   Apr 2006
     */
    template<typename T>
    struct IsInvalid{
      /*!
       *  Result
       */
      static const bool cond = false;
    };

    /*
     * Partial Specialisation
     */
    template<>
    struct IsInvalid<tfel::meta::InvalidType>{
      /*!
       *  Result
       */
      static const bool cond = true;
    };

  } // end of namespace typetraits

} // end of namespace tfel  

#endif /* _LIB_TFEL_ISINVALID_HXX */

