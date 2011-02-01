/*!pe 
 * \file   ComputeBinaryResult.hxx
 * \brief  This file declares the ComputeBinaryResult metafunction
 * \author Helfer Thomas
 * \date   13 Oct 2006
 */

#ifndef _LIB_TFEL_COMPUTEBINARYRESULT_H_
#define _LIB_TFEL_COMPUTEBINARYRESULT_H_ 

#include"TFEL/Metaprogramming/RemoveConstness.hxx"
#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/ComputeObjectTag.hxx"

namespace tfel{

  namespace math{

    /*
     * \class ComputeBinaryResult_
     * \brief A helper class for ComputeBinaryResult.
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
     * \return Result, type of the result of the operation.
     * \return Handle, type that will handle the operation.
     * \see ComputeBinaryResult.
     * \see ResultType.
     * \see ComputeObjectTag.
     */
    template<typename TagA,typename TagB,
	     typename A,   typename B,typename Op>
    struct ComputeBinaryResult_
    {
      /*
       * Result of the binary operation.
       */
      typedef tfel::meta::InvalidType Result;
      /*
       * Type that will handle the operation.
       */
      typedef tfel::meta::InvalidType Handle;
    }; // end of ComputeBinaryResult_

    /*
     * \class ComputeBinaryResult.
     * A metafunction to manage binary operations.
     * Binary operations are central part of tfel::math which makes
     * heavy use of expression templates.
     * Expression templates differentiates:
     * - the result of a binary operation, which in our view is the
     * type to what the result of the binary operation will be
     * affected.
     * - the type created by the binary operation, the handle, which
     * is often a temporary that should normally be thrown away by the
     * compiler. 
     * This is not true for scalars where the result and the handle
     * are the same are the same.
     * The computation of the Result and Handle types is normally be 
     * delegated to ComputeBinaryResult_ metafunction which is called
     * by ComputeBinaryResult class after having computed the tag 
     * associated with each arguments of the binary operations thanks
     * to the ComputeObjectTag.
     * \param typename A, type of the first argument of the
     * operation.
     * \param typename B, type of the second argument of the
     * operation.
     * \param typename Op, operation.
     * \return Result, type of the result of the operation.
     * \return Handle, type that will handle the operation.
     * \see ResultType.
     * \see ComputeObjectTag.
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult
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
       * Call to ComputeBinaryResult_ to get the Result type.
       */
      typedef typename ComputeBinaryResult_<TagA,TagB,A,B,Op>::Result Result;
      /*
       * Call to ComputeBinaryResult_ to get the Handle type
       */
      typedef typename ComputeBinaryResult_<TagA,TagB,A,B,Op>::Handle Handle;

    }; // end of ComputeBinaryResult.

    /*
     * Partial Specialisation of ComputeBinaryResult_ for scalars.
     * In that case, Result.
     * \param typename A, type of the first argument of the
     * operation.
     * \param typename B, type of the second argument of the
     * operation.
     * \param typename Op, operation.
     * \see RemoveConstness.
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<ScalarTag,ScalarTag,A,B,Op>
    {
    private:
      /*
       * A little trick so we don't have to declare ResultType
       * for const scalar types.
       */
      typedef typename tfel::meta::RemoveConstness<A>::type A_;
      /*
       * A little trick so we don't have to declare ResultType
       * for const scalar types.
       */
      typedef typename tfel::meta::RemoveConstness<B>::type B_;
    public:
      /*
       * The result.
       */
      typedef typename ResultType<A_,B_,Op>::type Result;
      /*
       * The handle.
       */
      typedef typename ResultType<A_,B_,Op>::type Handle;
    }; // end of ComputeBinaryResult_.

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_COMPUTEBINARYRESULT_H */

