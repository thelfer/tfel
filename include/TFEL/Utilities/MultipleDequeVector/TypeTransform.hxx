/*!
 * \file   include/TFEL/Utilities/MultipleDequeVector/TypeTransform.hxx
 * \info   This file defines two transformations of types which are intended to be used with the multiple_vector and multiple_deque containers
 * \see    TLTransform, multiple_vector, multiple_deque
 * \author Helfer Thomas
 * \date   01 jun 2006
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

