/*!
 * \file   FiniteStrainBehaviourTangentOperator.ixx
 * \brief    
 * \author THOMAS HELFER
 * \date   19 août 2016
 */

#ifndef LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATOR_IXX_
#define LIB_TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATOR_IXX_

#include<stdexcept>
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/tensor.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/t2tost2.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/Math/General/MathConstants.hxx"
#include"TFEL/Math/ST2toST2/ConvertSpatialModuliToKirchhoffJaumanRateModuli.hxx"

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
      template<typename T>
      using base_type = tfel::typetraits::base_type<T>;
      //! a simple alias
      template<unsigned short N,typename stress>
      using Result = tangent_operator<TangenOperatorType1,N,stress>;
      //! a simple alias
      template<unsigned short N,typename stress>
      using Source = tangent_operator<TangenOperatorType2,N,stress>;
      //! a simple alias
      template<unsigned short N,typename stress>
      using DeformationGradientTensor =
	tfel::math::tensor<N,base_type<stress>>;
      //! a simple alias
      template<unsigned short N,typename stress>
      using StressStensor = tfel::math::stensor<N,stress>;
      //! a simple alias
      using size_type = unsigned short;
    protected:
      /*!
       * \return the vector index associated with a the matrix indexes
       * of a symmetric tensor
       * \param[in] i: row index
       * \param[in] j: column index
       */
      TFEL_MATERIAL_INLINE static size_type
      index(const size_type i,const size_type j){
	// i,j are valid for the space dimension considered
	if(i==j){
	  return i;
	} else if((i==0)&&(j==1)){
	  return 3;
	} else if((i==1)&&(j==0)){
	  return 3;
	} else if((i==0)&&(j==2)){
	  return 4;
	} else if((i==2)&&(j==0)){
	  return 4;
	} else if((i==1)&&(j==2)){
	  return 5;
	}
	return 5;
      }
      /*!
       * \brief: generate a deformation gradient pertubation dFkl such
       * that the associated deformation rate is:
       * D_rkl = ((ek^el)+(el^ek))/2
       * with D = (L+tL)/2 and L = dF.F^{-1}
       * \param[in] F: deformation gradient
       * \param[in] idx: index associated to (k,l) in the stensor vector convention
       */
      template<unsigned short N,typename real>
      static TFEL_MATERIAL_INLINE tfel::math::tensor<N,real>
      getDeformationGradient(const tfel::math::tensor<N,real>& F,
			     const size_type idx){
	const auto c = [&idx]()
	  -> std::pair<size_type,size_type>
	  {
	    if((idx==0)||(idx==1)||(idx==2)){
	      return {idx,idx};
	    }
	    return {2*idx-3,2*idx-2};
	  }();
	// i,j are valid for the space dimension considered
	tfel::math::tensor<N,real> dF;
	tfel::math::tensor<N,real> ekel(real(0));
	const auto v = ((idx>2) ? tfel::math::Cste<real>::sqrt2 : real(1))/2;
	ekel(c.first)+=v;
	ekel(c.second)+=v;
	return ekel*F;
      }
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
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
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
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
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
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DS_DEGL,SPATIAL_MODULI)
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
	Kr = tfel::math::pull_back(Ks,F1);	
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DSIG_DF,DS_DEGL)
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
	  const DeformationGradientTensor<N,stress>& F0,
	  const DeformationGradientTensor<N,stress>& F1,
	  const StressStensor<N,stress>& s)
      {
	using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
	const auto Cs = convert<TangentOperator::SPATIAL_MODULI,
				TangentOperator::DS_DEGL>(Ks,F0,F1,s);
	const auto Dt = convert<TangentOperator::DTAU_DF,
				TangentOperator::SPATIAL_MODULI>(Cs,F0,F1,s);
	Kr = convert<TangentOperator::DSIG_DF,
		     TangentOperator::DTAU_DF>(Dt,F0,F1,s);
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DS_DF,DS_DC)
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
	Kr=Ks*tfel::math::t2tost2<N,base_type<stress>>::dCdF(F1);
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DS_DF,DS_DEGL)
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
	Kr=2*Ks*tfel::math::t2tost2<N,base_type<stress>>::dCdF(F1);
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(ABAQUS,SPATIAL_MODULI)
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
	  const StressStensor<N,stress>& s)
      {
	const auto J = tfel::math::det(F1);
	Kr = tfel::math::convertSpatialModuliToKirchhoffJaumanRateModuli(Ks,s*J)/J;
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(ABAQUS,DS_DEGL)
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
	  const DeformationGradientTensor<N,stress>& F0,
	  const DeformationGradientTensor<N,stress>& F1,
	  const StressStensor<N,stress>& s)
      {
	using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
	const auto J = tfel::math::det(F1);
	const auto C = convert<TangentOperator::SPATIAL_MODULI,
			       TangentOperator::DS_DEGL>(Ks,F0,F1,s);
	Kr = tfel::math::convertSpatialModuliToKirchhoffJaumanRateModuli(C,s*J)/J;
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DSIG_DF,C_TRUESDELL)
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
	  const DeformationGradientTensor<N,stress>& F0,
	  const DeformationGradientTensor<N,stress>& F1,
	  const StressStensor<N,stress>& s)
      {
	using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
	const auto Cs = convert<TangentOperator::SPATIAL_MODULI,
			       TangentOperator::C_TRUESDELL>(Ks,F0,F1,s);
	const auto Dt = convert<TangentOperator::DTAU_DF,
				TangentOperator::SPATIAL_MODULI>(Cs,F0,F1,s);
	Kr = convert<TangentOperator::DSIG_DF,
		     TangentOperator::DTAU_DF>(Dt,F0,F1,s);
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(SPATIAL_MODULI,ABAQUS)
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
	  const StressStensor<N,stress>& s)
      {
	const auto J = tfel::math::det(F1);
	// a little trick: the invert of the transformation from the
	// spatial moduli to the KirchhoffJaumanRateModuli is obtained
	// by changing the sign of the Kirchoff stress
	Kr = tfel::math::convertSpatialModuliToKirchhoffJaumanRateModuli(Ks*J,-s*J);
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(C_TRUESDELL,SPATIAL_MODULI)
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
	Kr = Ks/(tfel::math::det(F1));
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(C_TRUESDELL,DS_DEGL)
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
	Kr = tfel::math::push_forward(Ks,F1)/(tfel::math::det(F1));
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(SPATIAL_MODULI,C_TRUESDELL)
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
	Kr = Ks*(tfel::math::det(F1));
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DSIG_DDF,DSIG_DF)
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
	  const DeformationGradientTensor<N,stress>& F0,
	  const DeformationGradientTensor<N,stress>&,
	  const StressStensor<N,stress>&)
      {
	Kr = Ks*tfel::math::t2tot2<N,base_type<stress>>::tpld(F0);
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DSIG_DF,DSIG_DDF)
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
	  const DeformationGradientTensor<N,stress>& F0,
	  const DeformationGradientTensor<N,stress>&,
	  const StressStensor<N,stress>&)
      {
	const auto iF0 = tfel::math::invert(F0);
	Kr = Ks*tfel::math::t2tot2<N,base_type<stress>>::tpld(iF0);
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DTAU_DDF,DTAU_DF)
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
	  const DeformationGradientTensor<N,stress>& F0,
	  const DeformationGradientTensor<N,stress>&,
	  const StressStensor<N,stress>&)
      {
	Kr = Ks*tfel::math::t2tot2<N,base_type<stress>>::tpld(F0);
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DTAU_DF,DTAU_DDF)
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
	  const DeformationGradientTensor<N,stress>& F0,
	  const DeformationGradientTensor<N,stress>&,
	  const StressStensor<N,stress>&)
      {
	const auto iF0 = tfel::math::invert(F0);
	Kr = Ks*tfel::math::t2tot2<N,base_type<stress>>::tpld(iF0);
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DSIG_DF,DTAU_DF)
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
	  const StressStensor<N,stress>& s)
      {
	tfel::math::computeCauchyStressDerivativeFromKirchhoffStressDerivative(Kr,Ks,s,F1);
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DTAU_DF,DS_DF)
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
	  const StressStensor<N,stress>& s)
      {
	const auto sk2 = tfel::math::convertCauchyStressToSecondPiolaKirchhoffStress(s,F1);
	tfel::math::computePushForwardDerivative(Kr,Ks,sk2,F1);
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(SPATIAL_MODULI,DTAU_DF)
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
	  const DeformationGradientTensor<N,stress>& F0,
	  const DeformationGradientTensor<N,stress>& F1,
	  const StressStensor<N,stress>& s)
      {
	using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
	const auto CtJ = convert<TangentOperator::C_TAU_JAUMANN,
				 TangentOperator::DTAU_DF>(Ks,F0,F1,s);
	const auto J   = tfel::math::det(F1);
	Kr=convertSpatialModuliToKirchhoffJaumanRateModuli(CtJ,-J*s);
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(C_TAU_JAUMANN,DTAU_DF)
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
	for(size_type k=0;k!=3;++k){
	  for(size_type l=0;l!=3;++l){
	    if(((k==1)&&(l==0))||((k==2)&&(l==1))){
	      continue;
	    }
	    const auto rkl = FiniteStrainBehaviourTangentOperatorConverterBase::index(k,l);
	    if(rkl>=tfel::math::StensorDimeToSize<N>::value){
	      continue;
	    }
	    const auto cCj = Ks*getDeformationGradient(F1,rkl);
	    for(size_type i=0;i!=cCj.size();++i){
	      Kr(i,rkl)=cCj(i);
	    }
	  }
	}
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(C_TRUESDELL,DTAU_DF)
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
	  const DeformationGradientTensor<N,stress>& F0,
	  const DeformationGradientTensor<N,stress>& F1,
	  const StressStensor<N,stress>& s)
      {
	using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
	const auto Cs = convert<TangentOperator::SPATIAL_MODULI,
				TangentOperator::DTAU_DF>(Ks,F0,F1,s);
	Kr=Cs/(tfel::math::det(F1));
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(ABAQUS,C_TAU_JAUMANN)
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
	Kr=Ks/(tfel::math::det(F1));
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(C_TAU_JAUMANN,ABAQUS)
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
	Kr=Ks*(tfel::math::det(F1));
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(C_TAU_JAUMANN,SPATIAL_MODULI)
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
	  const StressStensor<N,stress>& s)
      {
	const auto J = tfel::math::det(F1);
	Kr=tfel::math::convertSpatialModuliToKirchhoffJaumanRateModuli(Ks,J*s);
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(SPATIAL_MODULI,C_TAU_JAUMANN)
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
	  const StressStensor<N,stress>& s)
      {
	const auto J   = tfel::math::det(F1);
	// a little trick: the invert of the transformation from the
	// spatial moduli to the KirchhoffJaumanRateModuli is obtained
	// by changing the sign of the Kirchoff stress
	Kr=tfel::math::convertSpatialModuliToKirchhoffJaumanRateModuli(Ks,-J*s);
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(ABAQUS,DTAU_DF)
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
	const auto iJ = 1/tfel::math::det(F1);
	for(size_type k=0;k!=3;++k){
	  for(size_type l=0;l!=3;++l){
	    if(((k==1)&&(l==0))||((k==2)&&(l==1))){
	      continue;
	    }
	    const auto rkl = FiniteStrainBehaviourTangentOperatorConverterBase::index(k,l);
	    if(rkl>=tfel::math::StensorDimeToSize<N>::value){
	      continue;
	    }
	    const auto cCj = Ks*getDeformationGradient(F1,rkl);
	    for(size_type i=0;i!=cCj.size();++i){
	      Kr(i,rkl)=cCj(i)*iJ;
	    }
	  }
	}
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DTAU_DF,C_TAU_JAUMANN)
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
	  const StressStensor<N,stress>& s)
      {
	// not supported by gcc 4.7.2: auto toT2toST2=[](const t2tot2& s) -> t2tost2{
	auto toT2toST2=[&](tfel::math::t2tot2<N,stress> src){
	  tfel::math::t2tost2<N,stress> r;
	  for(unsigned short i=0;i!=3;++i){
	    for(unsigned short j=0;j!=tfel::math::TensorDimeToSize<N>::value;++j){
	      r(i,j)=src(i,j);
	    }
	  }
	  for(unsigned short i=0;i!=tfel::math::StensorDimeToSize<N>::value-3;++i){
	    for(unsigned short j=0;j!=tfel::math::TensorDimeToSize<N>::value;++j){
	      r(3+i,j)=(src(3+2*i,j)+src(3+2*i+1,j))*tfel::math::Cste<stress>::isqrt2;
	    }
	  }
	  return r;
	};
	const auto t   = s*tfel::math::det(F1);
	const auto tus = tfel::math::unsyme(t);
	const auto dD = tfel::math::computeRateOfDeformationDerivative(F1);
	const auto dW = tfel::math::computeSpinRateDerivative(F1);
	Kr=Ks*dD+toT2toST2((tfel::math::t2tot2<N,base_type<stress>>::tpld(tus)-
			    tfel::math::t2tot2<N,base_type<stress>>::tprd(tus))*dW);
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DTAU_DF,ABAQUS)
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
	  const StressStensor<N,stress>& s)
      {
	// not supported by gcc 4.7.2: auto toT2toST2=[](const t2tot2& s) -> t2tost2{
	auto toT2toST2=[&](tfel::math::t2tot2<N,stress> src){
	  tfel::math::t2tost2<N,stress> r;
	  for(unsigned short i=0;i!=3;++i){
	    for(unsigned short j=0;j!=tfel::math::TensorDimeToSize<N>::value;++j){
	      r(i,j)=src(i,j);
	    }
	  }
	  for(unsigned short i=0;i!=tfel::math::StensorDimeToSize<N>::value-3;++i){
	    for(unsigned short j=0;j!=tfel::math::TensorDimeToSize<N>::value;++j){
	      r(3+i,j)=(src(3+2*i,j)+src(3+2*i+1,j))*tfel::math::Cste<stress>::isqrt2;
	    }
	  }
	  return r;
	};
	const auto J   = tfel::math::det(F1);
	const auto t   = s*J;
	const auto tus = tfel::math::unsyme(t);
	const auto dD  = tfel::math::computeRateOfDeformationDerivative(F1);
	const auto dW  = tfel::math::computeSpinRateDerivative(F1);
	Kr=J*Ks*dD+toT2toST2((tfel::math::t2tot2<N,base_type<stress>>::tpld(tus)-
			      tfel::math::t2tot2<N,base_type<stress>>::tprd(tus))*dW);
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DTAU_DF,SPATIAL_MODULI)
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
	  const DeformationGradientTensor<N,stress>& F0,
	  const DeformationGradientTensor<N,stress>& F1,
	  const StressStensor<N,stress>& s)
      {
	using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
	// not supported by gcc 4.7.2: auto toT2toST2=[](const t2tot2& s) -> t2tost2{
	auto toT2toST2=[&](tfel::math::t2tot2<N,stress> src)
	  -> tfel::math::t2tost2<N,stress>{
	  tfel::math::t2tost2<N,stress> r;
	  for(unsigned short i=0;i!=3;++i){
	    for(unsigned short j=0;j!=tfel::math::TensorDimeToSize<N>::value;++j){
	      r(i,j)=src(i,j);
	    }
	  }
	  for(unsigned short i=0;i!=tfel::math::StensorDimeToSize<N>::value-3;++i){
	    for(unsigned short j=0;j!=tfel::math::TensorDimeToSize<N>::value;++j){
	      r(3+i,j)=(src(3+2*i,j)+src(3+2*i+1,j))*tfel::math::Cste<stress>::isqrt2;;
	    }
	  }
	  return r;
	};
	const auto CJ = convert<TangentOperator::C_TAU_JAUMANN,
				TangentOperator::SPATIAL_MODULI>(Ks,F0,F1,s);
	const auto t   = s*det(F1);
	const auto tus = tfel::math::unsyme(t);
	const auto dD = tfel::math::computeRateOfDeformationDerivative(F1);
	const auto dW = tfel::math::computeSpinRateDerivative(F1);
	Kr=CJ*dD+toT2toST2((tfel::math::t2tot2<N,base_type<stress>>::tpld(tus)-
			    tfel::math::t2tot2<N,base_type<stress>>::tprd(tus))*dW);
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DT_DELOG,DS_DEGL)
    {
      /*!
       * \brief convert the stiffness matrix expressed in the logarithmic
       * space into dS_dC in 1D.
       * \param[out] Kr: the result of the convertion
       * \param[in]  Ks: the initial stiffness tensor
       * \param[in]  F0:  the deformation gradient
       * \param[in]  F1:  the deformation gradient
       * \param[in]  s:  the Cauchy stress tensor
       */
      template<typename stress>
      static TFEL_MATERIAL_INLINE void
      exe(Result<1u,stress>& Kr,
	  const Source<1u,stress>& Ks,
	  const DeformationGradientTensor<1u,stress>&,
	  const DeformationGradientTensor<1u,stress>& F1,
	  const StressStensor<1u,stress>& sig)
      {
	using namespace tfel::math;
	using real = base_type<stress>;
	const real iC[3]  = {1/(F1[0]*F1[0]),
			     1/(F1[1]*F1[1]),
			     1/(F1[2]*F1[2])};
	const auto t = det(F1)*sig;
	Kr(0,0)   = (Ks(0,0)-2*t[0])*iC[0]*iC[0];
	Kr(0,1)   =  Ks(0,1)*iC[0]*iC[1];
	Kr(0,2)   =  Ks(0,2)*iC[0]*iC[2];
	Kr(1,0)   =  Ks(1,0)*iC[0]*iC[1];
	Kr(1,1)   = (Ks(1,1)-2*t[1])*iC[1]*iC[1];
	Kr(1,2)   =  Ks(1,2)*iC[1]*iC[2];
	Kr(2,0)   =  Ks(2,0)*iC[0]*iC[2];
	Kr(2,1)   =  Ks(2,1)*iC[1]*iC[2];
	Kr(2,2)   = (Ks(2,2)-2*t[2])*iC[2]*iC[2];
      } // end of exe
      /*!
       * \brief convert the stiffness matrix expressed in the logarithmic
       * space into dS_dC in 3D.
       * \param[out] Kr: the result of the convertion
       * \param[in]  Ks: the initial stiffness tensor
       * \param[in]  F0:  the deformation gradient
       * \param[in]  F1:  the deformation gradient
       * \param[in]  s:  the Cauchy stress tensor
       */
      template<typename stress>
      static TFEL_MATERIAL_INLINE void
      exe(Result<3u,stress>& Kr,
	  const Source<3u,stress>& Ks,
	  const DeformationGradientTensor<3u,stress>&,
	  const DeformationGradientTensor<3u,stress>& F1,
	  const StressStensor<3u,stress>& s)
      {
	using namespace tfel::math;
	using real    = base_type<stress>;
	using tvector = tvector<3u,real>;
	using stensor = stensor<3u,real>;
	using size_type = unsigned short;
	constexpr const auto eps = 1.e-14;
	auto are_all_vp_equals = [eps](const tvector& vp){
	  return ((std::abs(vp(1)-vp(0))<eps)&&
		  (std::abs(vp(1)-vp(2))<eps)&&
		  (std::abs(vp(2)-vp(0))<eps));
	};
	auto find_uniq_vp = [eps,&are_all_vp_equals](const tvector& vp){
	  if(are_all_vp_equals(vp)){
	    throw(std::runtime_error("FiniteStrainBehaviourTangentOperatorConverter"
				     "<Base::DT_DELOG,Base::DS_DEGL>::exe: "
				     "find_uniq_vp shall not be called if all "
				     "eigen values are equal."));
	  }
	  if((std::abs(vp(1)-vp(0))>eps)&&
	     (std::abs(vp(1)-vp(2))>eps)&&
	     (std::abs(vp(2)-vp(0))>eps)){
	    // all eigenvalues are different
	    return 3;
	  }
	  if((std::abs(vp(1)-vp(0))<eps)){
	    return 2;
	  }
	  if((std::abs(vp(2)-vp(0))<eps)){
	    return 1;
	  }
	  return 0;
	};
	const auto lk = [](const size_type i, const size_type j) -> size_type{
	  if(i==0){
	    return (j==1) ? 2 : 1;
	  }
	  if(i==1){
	    return (j==0) ? 2 : 0;
	  }
	  return (j==0) ? 1 : 0;
	};
	// compute the derivative of the Hencky strain with respect to C
	const auto fl   = [](const real x){return std::log1p(x-1)/2;};
	const auto dfl  = [](const real x){return 1/(2*x);};
	const auto C = computeRightCauchyGreenTensor(F1);
	auto vp = tvector{};
	auto m  = tmatrix<3u,3u,real>{};
	std::tie(vp,m) = C.template computeEigenVectors<stensor::FSESJACOBIEIGENSOLVER>();
	const auto e   = map(fl,vp);
	const auto d   = map(dfl,vp); // half compared to Miehe definition
	const auto f   = map([](const real x){return -2/(x*x);},vp);
	// p is one half of the tensor defined by Miehe
	const auto p   = stensor::computeIsotropicFunctionDerivative(e,d,vp,m,eps);
	// convert the Cauchy stress to the dual of the logaritmic strain
	const auto ip = invert(p);
	const auto S  = convertCauchyStressToSecondPiolaKirchhoffStress(s,F1);
	const auto T = (S|ip)/2;
	// real work starts here
	const auto M = [&m]{
	  const tvector v[3] = {m.template column_view<0u>(),
				m.template column_view<1u>(),
				m.template column_view<2u>()};
	  tmatrix<3u,3u,stensor> r;
	  for(size_type i=0;i!=3;++i){
	    for(size_type j=0;j!=3;++j){
	      r(i,j)=stensor::buildFromVectorsSymmetricDiadicProduct(v[i],v[j]);
	    }
	  }
	  return r;
	}();
	const auto xsi   = [&vp,&e,&d,&f,&are_all_vp_equals,&find_uniq_vp]{
	  if(are_all_vp_equals(vp)){
	    constexpr const auto zero = real{0};
	    const auto rv = (f[0]+f[1]+f[2])/24;
	    return tmatrix<3u,3u,real>{zero,rv,rv,
		rv,zero,rv,
		rv,rv,zero};
	  }
	  auto r = tmatrix<3u,3u,real>{};
	  const auto k = find_uniq_vp(vp);
	  if(k!=3){
	    for(size_type i=0;i!=3;++i){
	      for(size_type j=0;j!=3;++j){
		if(i==j){
		  r(i,j) = real{};
		} else if((i==k)||(j==k)){
		  const auto idvp = 1/(vp[i]-vp[j]);
		  r(i,j)=((e[i]-e[j])*idvp-d[j])*idvp;
		} else {
		  r(i,j)=(f[i]+f[j])/16;
		}
	      }
	    }
	    return r;
	  }
	  for(size_type i=0;i!=3;++i){
	    for(size_type j=0;j!=3;++j){
	      if(i==j){
		r(i,j) = real{};
	      } else {
		const auto idvp = 1/(vp[i]-vp[j]);
		r(i,j)=((e[i]-e[j])*idvp-d[j])*idvp;
	      }
	    }
	  }
	  return r;
	}();
	const auto eta = [&e,&vp,&lk,&f,&are_all_vp_equals,&find_uniq_vp,&d]{
	  if(are_all_vp_equals(vp)){
	    return (f[0]+f[1]+f[2])/24;
	  }
	  const auto u = find_uniq_vp(vp);
	  if(u!=3){
	    const auto i = (u==2) ? 0 : 2;
	    const auto idvp = 1/(vp[i]-vp[u]);
	    return ((e[i]-e[u])*idvp-d[u])*idvp;
	  }
	  auto r = real{};
	  for(size_type i=0;i!=3;++i){
	    for(size_type j=0;j!=3;++j){
	      if(i==j){
		continue;
	      }
	      const auto k = lk(i,j);
	      r += e[i]/(2*(vp[i]-vp[j])*(vp[i]-vp[k]));
	    }
	  }
	  return r;
	}();
	const auto dzeta = [&M,&T]{
	  auto r = tmatrix<3u,3u,real>{};
	  for(size_type i=0;i!=3;++i){
	    for(size_type j=0;j!=3;++j){
	      r(i,j)=(T|M(i,j))/2;
	    }
	  }
	  return r;
	}();
	Kr = 4*transpose(p)*Ks*p;
	for(size_type i=0;i!=3;++i){
	  Kr+=f[i]/4*dzeta(i,i)*(M(i,i)^M(i,i));
	  for(size_type j=0;j!=3;++j){
	    if(i==j){
	      continue;
	    }
	    const auto k = lk(i,j);
	    Kr+=2*eta*dzeta(i,j)*(M(i,k)^M(j,k))
	      +2*xsi(i,j)*(dzeta(i,j)*((M(i,j)^M(j,j))+(M(j,j)^M(i,j)))+
			   dzeta(j,j)*(M(i,j)^M(i,j)));
	  }
	}
      } // end of exe
    };
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DT_DELOG,DS_DC)
    {
      /*!
       * \brief convert the stiffness matrix expressed in the logarithmic
       * space into dS_dC in 1D.
       * \param[out] Kr: the result of the convertion
       * \param[in]  Ks: the initial stiffness tensor
       * \param[in]  F0:  the deformation gradient
       * \param[in]  F1:  the deformation gradient
       * \param[in]  s:  the Cauchy stress tensor
       */
      template<typename stress>
      static TFEL_MATERIAL_INLINE void
      exe(Result<1u,stress>& Kr,
	  const Source<1u,stress>& Ks,
	  const DeformationGradientTensor<1u,stress>&,
	  const DeformationGradientTensor<1u,stress>& F1,
	  const StressStensor<1u,stress>& sig)
      {
	using namespace tfel::math;
	using real = base_type<stress>;
	const real iC[3]  = {1/(F1[0]*F1[0]),
			     1/(F1[1]*F1[1]),
			     1/(F1[2]*F1[2])};
	const auto t = det(F1)*sig;
	Kr(0,0)   = (Ks(0,0)-2*t[0])*iC[0]*iC[0]/2;
	Kr(0,1)   =  Ks(0,1)*iC[0]*iC[1]/2;
	Kr(0,2)   =  Ks(0,2)*iC[0]*iC[2]/2;
	Kr(1,0)   =  Ks(1,0)*iC[0]*iC[1]/2;
	Kr(1,1)   = (Ks(1,1)-2*t[1])*iC[1]*iC[1]/2;
	Kr(1,2)   =  Ks(1,2)*iC[1]*iC[2]/2;
	Kr(2,0)   =  Ks(2,0)*iC[0]*iC[2]/2;
	Kr(2,1)   =  Ks(2,1)*iC[1]*iC[2]/2;
	Kr(2,2)   = (Ks(2,2)-2*t[2])*iC[2]*iC[2]/2;
      } // end of exe
      /*!
       * \brief convert the stiffness matrix expressed in the logarithmic
       * space into dS_dC in 2D.
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
	  const DeformationGradientTensor<N,stress>& F0,
	  const DeformationGradientTensor<N,stress>& F1,
	  const StressStensor<N,stress>& s)
      {
	using Base      = FiniteStrainBehaviourTangentOperatorBase;
	using Converter = FiniteStrainBehaviourTangentOperatorConverter<Base::DT_DELOG,
									Base::DS_DEGL>;
	auto K = Result<N,stress>();
	Converter::exe(K,Ks,F0,F1,s);
	Kr = K/2;
      } // end of exe
    };
    /*!
     * \brief partial specialisation of FiniteStrainBehaviourTangentOperatorConverter structure
     */
    TFEL_MATERIAL_FINITESTRAINBEHAVIOURTANGENTOPERATORCONVERTER(DSIG_DF,ABAQUS)
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
	  const DeformationGradientTensor<N,stress>& F0,
	  const DeformationGradientTensor<N,stress>& F1,
	  const StressStensor<N,stress>& s)
      {
	using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
	const auto Dt = convert<TangentOperator::DTAU_DF,
				TangentOperator::ABAQUS>(Ks,F0,F1,s);
	Kr = convert<TangentOperator::DSIG_DF,
		     TangentOperator::DTAU_DF>(Dt,F0,F1,s);
      } // end of exe
    }; // end of struct FiniteStrainBehaviourTangentOperatorConverter
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
