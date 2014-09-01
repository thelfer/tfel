/*!
 * \file   include/TFEL/Utilities/MultipleDequeVector/TypeTransform.hxx
 * \info   This file defines two transformations of types which are intended to be used with the multiple_vector and multiple_deque containers
 * \see    TLTransform, multiple_vector, multiple_deque
 * \author Helfer Thomas
 * \date   01 jun 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_TYPETRANSFORM_HXX_
#define _LIB_TFEL_TYPETRANSFORM_HXX_ 

#include"TFEL/Utilities/SmartPtr.hxx"

namespace tfel{
  
  namespace utilities{

    /*!
     * \class IdTransform
     * \brief Transform a type to itself.
     * \author Helfer Thomas
     * \date   01 jun 2006
     */
    template<typename T>
    struct IdTransform;
    
    /*!
     * \class  shared_ptrTransform;
     * \brief  Transform a type to a tfel::Ptr to it.
     * \author Helfer Thomas
     * \date   01 jun 2006
     */
    template<typename T>
    struct shared_ptrTransform;
  
  }

}

#include"TFEL/Utilities/MultipleDequeVector/TypeTransform.ixx"

#endif /* _LIB_TFEL_TYPETRANSFORM_HXX */

