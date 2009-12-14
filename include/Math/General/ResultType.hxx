/*!
 * \file   ResultType.hxx
 * \brief  This file declares the ResultType class.
 * \author Helfer Thomas
 * \date   24 jun 2006
 */

#ifndef _LIB_TFEL_RESULT_TYPE_HXX_
#define _LIB_TFEL_RESULT_TYPE_HXX_

#include"Metaprogramming/InvalidType.hxx"
#include"Math/General/ComputeObjectTag.hxx"

namespace tfel{

  namespace math{
    
    /*
     * \class ResultType_
     * \brief A helper class for ResultType.
     * This default version returns InvalidType both for
     * Result and Handle.
     * \param typename TagA, tag of the type of the first 
     * argument of the operation.
     * \param typename TagB, tag of the type of second 
     * argument of the operation.
     * \param typename A, type of the first argument of the
     * operation.
     * \param typename B, type of the second argument of the
     * operation.
     * \param typename Op, operation.
     * \return type, type of the result of the operation.
     * \see ResultType.
     * \see ComputeObjectTag.
     */
    template<typename TagA,typename TagB,
	     typename A,   typename B,typename Op>
    struct ResultType_
    {
      /*
       * Result of the binary operation.
       */
      typedef tfel::meta::InvalidType type;

    }; // end of ResultType_

    /*
     * \class ResultType
     * This metafunction gives the result of a binary operation.
     * This class is meant to be specialised on a per objet basis.
     * (in comparison, ComputeBinaryResult is meant to be specialised
     * on a per concept basis)
     * The helper class ResultType_ is only convenient to declare
     * scalar to objet operations.
     * \param typename A, type of the first argument of the
     * operation.
     * \param typename B, type of the second argument of the
     * operation.
     * \param typename Op, operation.
     * \return type, type of the result.
     */
    template<typename A, typename B, typename Op>
    class ResultType
    {
      /*
       * Tag of the object A
       */
      typedef typename ComputeObjectTag<A>::type TagA;
      /*
       * Tag of the object A
       */
      typedef typename ComputeObjectTag<B>::type TagB;
    public:
      /*
       * Result
       */
      typedef typename ResultType_<TagA,TagB,A,B,Op>::type type;
    };

  } // end of namespace math

} // end of namespace tfel  

#endif /* _LIB_TFEL_RESULT_TYPE_HXX */

