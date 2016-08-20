/*!
 * \file   FiniteStrainBehaviourTangentOperator.ixx
 * \brief    
 * \author THOMAS HELFER
 * \date   19 août 2016
 */

#ifndef LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATOR_IXX_
#define LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATOR_IXX_

/*!
 * \brief a convenient macro for shorted declaration of partial
 * specialisation of the FiniteStrainBehaviourTangentOperatorConverter
 * structure
 */
#define  TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(OP1,OP2)                                   \
  template<>                                                                                                    \
  struct FiniteStrainBehaviourTangentOperatorConverter<FiniteStrainBehaviourTangentOperatorBase::OP1,           \
						       FiniteStrainBehaviourTangentOperatorBase::OP2>           \
      : public FiniteStrainBehaviourTangentOperatorConverterBase<FiniteStrainBehaviourTangentOperatorBase::OP1, \
								 FiniteStrainBehaviourTangentOperatorBase::OP2>

namespace tfel
{

  namespace material
  {

    /*!
     * An helper class used to define simple alias
     */
    template<FiniteStrainBehaviourTangentOperatorBase::Flag TangenOperatorType1,
	     FiniteStrainBehaviourTangentOperatorBase::Flag TangenOperatorType2>
    struct FiniteStrainBehaviourTangentOperatorConverterBase{
      //! a simple alias
      template<unsigned short N,typename stress>
      using Result = tangent_operator<TangenOperatorType1,N,stress>;
      //! a simple alias
      template<unsigned short N,typename stress>
      using Source = tangent_operator<TangenOperatorType2,N,stress>;
      //! a simple alias
      template<unsigned short N,typename stress>
      using DeformationGradientTensor =
	tfel::math::tensor<N,tfel::typetraits::base_type<stress>>;
      //! a simple alias
      template<unsigned short N,typename stress>
      using StressStensor = tfel::math::stensor<N,stress>;
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverterBase
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DS_DC,DS_DEGL)
    {
      /*!
       * \param[out] Kr: the result of the convertion
       * \param[in]  Ks: the initial stiffness tensor
       * \param[in]  F0:  the deformation gradient
       * \param[in]  F1:  the deformation gradient
       * \param[in]  s:  the Cauchy stress tensor
       */
      template<unsigned short N,typename stress>
      static TFEL_MATERIAL_INLINE void
      exe(Result<N,stress>& Kr,
	  const Source<N,stress>& Ks,
	  const DeformationGradientTensor<N,stress>&,
	  const DeformationGradientTensor<N,stress>&,
	  const StressStensor<N,stress>&)
      {
	Kr=Ks/2;
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter<FiniteStrainBehaviourTangentOperatorBase::DS_DC,FiniteStrainBehaviourTangentOperatorBase::DS_DEGL>
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DS_DEGL,DS_DC)
    {
      /*!
       * \param[out] Kr: the result of the convertion
       * \param[in]  Ks: the initial stiffness tensor
       * \param[in]  F0:  the deformation gradient
       * \param[in]  F1:  the deformation gradient
       * \param[in]  s:  the Cauchy stress tensor
       */
      template<unsigned short N,typename stress>
      static TFEL_MATERIAL_INLINE void
      exe(Result<N,stress>& Kr,
	  const Source<N,stress>& Ks,
	  const DeformationGradientTensor<N,stress>&,
	  const DeformationGradientTensor<N,stress>&,
	  const StressStensor<N,stress>&)
      {
	Kr=2*Ks;
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter<FiniteStrainBehaviourTangentOperatorBase::DS_DC,FiniteStrainBehaviourTangentOperatorBase::DS_DEGL>
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(SPATIAL_MODULI,DS_DEGL)
    {
      /*!
       * \param[out] Kr: the result of the convertion
       * \param[in]  Ks: the initial stiffness tensor
       * \param[in]  F0:  the deformation gradient
       * \param[in]  F1:  the deformation gradient
       * \param[in]  s:  the Cauchy stress tensor
       */
      template<unsigned short N,typename stress>
      static TFEL_MATERIAL_INLINE void
      exe(Result<N,stress>& Kr,
	  const Source<N,stress>& Ks,
	  const DeformationGradientTensor<N,stress>&,
	  const DeformationGradientTensor<N,stress>& F1,
	  const StressStensor<N,stress>&)
      {
	Kr=tfel::math::push_forward(Ks,F1);
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter<FiniteStrainBehaviourTangentOperatorBase::SPATIAL_MODULI,FiniteStrainBehaviourTangentOperatorBase::DS_DEGL>

    template<FiniteStrainBehaviourTangentOperatorBase::Flag TangenOperatorType1,
    	     FiniteStrainBehaviourTangentOperatorBase::Flag TangenOperatorType2,
    	     unsigned short N,typename StressType>
    tangent_operator<TangenOperatorType1,N,StressType>
    convert(const tangent_operator<TangenOperatorType2,N,StressType>& K,
    	    const tfel::math::tensor<N,tfel::typetraits::base_type<StressType>>& F0,
    	    const tfel::math::tensor<N,tfel::typetraits::base_type<StressType>>& F1,
    	    const tfel::math::stensor<N,StressType>& s){
      tangent_operator<TangenOperatorType1,N,StressType> r;
      FiniteStrainBehaviourTangentOperatorConverter<TangenOperatorType1,TangenOperatorType2>::exe(r,K,F0,F1,s);
      return r;
    }

    template<FiniteStrainBehaviourTangentOperatorBase::Flag TangenOperatorType1,
	     FiniteStrainBehaviourTangentOperatorBase::Flag TangenOperatorType2,
	     unsigned short N,typename StressType>
    void convert(tangent_operator<TangenOperatorType1,N,StressType>& Kr,
		 const tangent_operator<TangenOperatorType2,N,StressType>& Ks,
		 const tfel::math::tensor<N,tfel::typetraits::base_type<StressType>>& F0,
		 const tfel::math::tensor<N,tfel::typetraits::base_type<StressType>>& F1,
		 const tfel::math::stensor<N,StressType>& s){
      FiniteStrainBehaviourTangentOperatorConverter<TangenOperatorType1,TangenOperatorType2>::exe(Kr,Ks,F0,F1,s);
    }
    
  } // end of namespace material

} // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATOR_IXX_ */
