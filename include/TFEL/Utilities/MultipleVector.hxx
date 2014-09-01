/*!
 * \file   include/TFEL/Utilities/MultipleVector.hxx
 * \brief  This file defines the MultipleVector class.
 * \author Helfer Thomas
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MULTIPLEVECTOR_H_
#define _LIB_TFEL_MULTIPLEVECTOR_H_ 1

#include <string>
#include <vector>

#include"TFEL/Utilities/MultipleDequeVector/TypeTransform.hxx"
#include"TFEL/Utilities/MultipleDequeVector/MultipleDequeVector.hxx"

namespace tfel{
  
  namespace utilities{

    /*!
     * \class  MultipleVector
     * \brief  The MultipleVector enables storage of every elements in a TL.
     * \param  T, TL.
     * \param  Transform, tells how elements are handled.
     * \see    TypeTransform.hxx
     * \author Helfer Thomas
     * \date   28 Aug 2006
     */
    template<typename T, template<class> class Transform=tfel::utilities::IdTransform>
    class  MultipleVector
      : public tfel::utilities::MultipleDequeVectorContainer<T,Transform,std::vector>
    {};
    
  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_MULTIPLEVECTOR_H_ */
