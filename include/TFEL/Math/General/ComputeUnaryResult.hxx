/*!
 * \file   ComputeUnaryResult.hxx
 * \brief  This file declares the ComputeUnaryResult metafunction
 * \author Helfer Thomas
 * \date   13 Oct 2006
 */

#ifndef _LIB_TFEL_COMPUTEUNARYRESULT_H_
#define _LIB_TFEL_COMPUTEUNARYRESULT_H_ 

#include"TFEL/Metaprogramming/RemoveConstness.hxx"

#include"TFEL/Math/General/UnaryResultType.hxx"
#include"TFEL/Math/General/ComputeObjectTag.hxx"

namespace tfel{

  namespace math{

    template<typename TagA,typename TagOp,
	     typename A,typename Op>
    struct ComputeUnaryResult_
    {
      typedef tfel::meta::InvalidType Result;
      typedef tfel::meta::InvalidType Handle;
    };

    template<typename A,typename Op>
    struct ComputeUnaryResult
    {
    private:
      typedef typename ComputeObjectTag<A>::type TagA;
      typedef typename ComputeObjectTag<Op>::type TagOp;
    public:
      typedef typename ComputeUnaryResult_<TagA,TagOp,A,Op>::Result Result;
      typedef typename ComputeUnaryResult_<TagA,TagOp,A,Op>::Handle Handle;
    };

    /*
     * Partial Specialisation of ComputeUnaryResult_ for scalars
     */
    template<typename A,typename TagOp,typename Op>
    struct ComputeUnaryResult_<ScalarTag,TagOp,A,Op>
    {
      typedef typename tfel::meta::RemoveConstness<A>::type A_;
      typedef typename UnaryResultType<A_,Op>::type Result;
      typedef typename UnaryResultType<A_,Op>::type Handle;
    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_COMPUTEUNARYRESULT_H */

