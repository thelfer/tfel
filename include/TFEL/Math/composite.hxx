/*!
 * \file   include/TFEL/Math/composite.hxx
 * \see    GenerateComposite.cxx to see how this file was generated.
 * \brief  This file declares the composite template class.
 * \author Helfer Thomas
 */

#ifndef _LIB_TFEL_COMPOSITE_H_
#define _LIB_TFEL_COMPOSITE_H_ 

#include"TFEL/Metaprogramming/TypeList.hxx"
#include"TFEL/Metaprogramming/GenerateTypeList.hxx"
#include"TFEL/Math/Composite/CompositeConcept.hxx"

namespace tfel{

  namespace math{

    typedef tfel::meta::TLE CompositeEndType;

    /*!
     * \class composite
     *
     * composite can contain up to 10 elements.
     *
     * All template parameters takes the default value CompositeEndType.
     *
     * \author Helfer Thomas
     */
    template<typename T0,typename T1=CompositeEndType,
	     typename T2=CompositeEndType,typename T3=CompositeEndType,
	     typename T4=CompositeEndType,typename T5=CompositeEndType,
	     typename T6=CompositeEndType,typename T7=CompositeEndType,
	     typename T8=CompositeEndType,typename T9=CompositeEndType>
    class composite;

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Composite/composite.ixx"

#include"TFEL/Math/Composite/compositeResultType.ixx"

#endif /* _LIB_TFEL_COMPOSITE_H_ */
