/*!
 * \file   include/TFEL/TypeTraits/IsTemporary.hxx
 * \brief  This file declares the IsTemporary traits class.
 * \author Helfer Thomas
 * \date   16 Oct 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_ISTEMPORARY_H_
#define _LIB_TFEL_ISTEMPORARY_H_ 

namespace tfel{

  namespace typetraits{

    /*!
     * \brief  Traits class which says if its argument is a tempory,
     * such a VectorExpr, MatrixExpr, etc..
     *
     * This information usually determines wether an object shall be
     * copied or if a simple reference to it is enough.
     *
     * \param  T, type to be tested.
     * \return bool cond, true if T is a temporary, false otherwise.
     *
     * \see VectorVectorExpr for an example of use
     *
     * \author Helfer Thomas
     * \date   Apr 2007
     */
    template<typename T>
    struct IsTemporary
    {
      /*!
       *  Result
       */
      static const bool cond = false;
    };

  } // end of namespace typetraits

} // end of namespace tfel

#endif /* _LIB_TFEL_ISTEMPORARY_H */

