/*!
 * \file   include/TFEL/Material/Barlat.hxx
 * \brief  
 * \author Thomas Helfer
 * \date   15/11/2017
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_MATERIAL_BARLAT_HXX
#define LIB_TFEL_MATERIAL_BARLAT_HXX

#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/st2tost2.hxx"

namespace tfel{

  namespace material{

    //! a simple alias
    template<typename StressStensor>
    using BarlatStressType = tfel::math::StensorNumType<StressStensor>;
    //! a simple alias
    template<typename StressStensor>
    using BarlatBaseType =
      tfel::typetraits::base_type<tfel::math::StensorNumType<StressStensor>>;
    //! a simple alias
    template<typename StressStensor>
    using BarlatLinearTransformationType =
      tfel::math::st2tost2<tfel::math::StensorTraits<StressStensor>::dime,
			   BarlatBaseType<StressStensor>>;
    //! a simple alias
    template<typename StressStensor>
    using BarlatInvertStressType =
      tfel::math::result_type<BarlatBaseType<StressStensor>,
			      BarlatStressType<StressStensor>,
			      tfel::math::OpDiv>;
    //! a simple alias
    template<typename StressStensor>
    using BarlatStressNormalType =
      tfel::math::stensor<tfel::math::StensorTraits<StressStensor>::dime,
			  BarlatBaseType<StressStensor>>;
    //! a simple alias
    template<typename StressStensor>
    using BarlatStressEigenTensorType =
      tfel::math::stensor<tfel::math::StensorTraits<StressStensor>::dime,
			  BarlatBaseType<StressStensor>>;
    //! a simple alias
    template<typename StressStensor>
    using BarlatStressSecondDerivativeType = 
      tfel::math::st2tost2<tfel::math::StensorTraits<StressStensor>::dime,
			   BarlatInvertStressType<StressStensor>>;
    /*!
     * \tparam N:    space dimension
     * \tparam real: numerical type
     * \return a linear transformation of the stresses
     * \param[in] c12: coefficient of  linear transformation
     * \param[in] c21: coefficient of  linear transformation
     * \param[in] c13: coefficient of  linear transformation
     * \param[in] c31: coefficient of  linear transformation
     * \param[in] c23: coefficient of  linear transformation
     * \param[in] c32: coefficient of  linear transformation
     * \param[in] c44: coefficient of  linear transformation
     * \param[in] c55: coefficient of  linear transformation
     * \param[in] c66: coefficient of  linear transformation
     *
     * The linear transformation is defined as follows:
     * \f[
     * \begin{pmatrix}
     * 0 & -c_{12} & -c_{13} & 0 & 0 & 0 \\
     * -c_{21} & 0 & -c_{23} & 0 & 0 & 0 \\
     * -c_{31} & -c_{32} & 0 & 0 & 0 & 0 \\
     * 0 & 0 & 0 & c_{44} & 0 & 0 \\
     * 0 & 0 & 0 & 0 & c_{55} & 0 \\
     * 0 & 0 & 0 & 0 & 0 & c_{66} \\
     * \end{pmatrix}
     * \f]
     * \note Barlat uses the following convention for storing
     * symmetric tensors:
     * \f[
     * \begin{pmatrix}
     * xx & yy & zz & yz & zx & xy
     * \end{pmatrix}
     * \f]
     * which is not consistent with the
     * `TFEL`/`Cast3M`/`Abaqus`/`Ansys` conventions:
     * \f[
     * \begin{pmatrix}
     * xx & yy & zz & xy & xz & yz
     * \end{pmatrix}
     * \f]
     * 
     * Therefore, if one wants to uses coeficients \f(c^{B}\f) given
     * by Barlat, one shall call this function as follows:
     *
     * \code{.cpp}
     * const auto l1 = makeBarlatLinearTransformationType<3>(cB_12,cB_21,cB_13,cB_31,
     *                                                       cB_23,cB_32,cB_66,cBB_55,cBB_44);
     * \endcode
     */
    template<unsigned short N, typename real>
    tfel::math::st2tost2<N,real>
    makeBarlatLinearTransformation(const real,const real, const real,
				   const real,const real, const real,
				   const real,const real, const real);
    /*!
     * \brief compute the Barlat yield stress
     * \tparam StressStensor: type of the stress tensor
     * \tparam HosfordExponentType: type of the hosford exponent
     * (could be a numeric type or an integer type)
     * \tparam es: eigen solver to be used
     * \param[in] s: stress tensor
     * \param[in] l1: first linear transformation
     * \param[in] l2: second linear transformation
     * \param[in] a: Barlat exponent
     * \param[in] e: criterion used to check if the stress are null
     */
    template<typename StressStensor,
	     typename BarlatExponentType,
    	     tfel::math::stensor_common::EigenSolver =
    	     tfel::math::stensor_common::TFELEIGENSOLVER>
    BarlatStressType<StressStensor>
    computeBarlatStress(const StressStensor&,
    			const BarlatLinearTransformationType<StressStensor>&,
    			const BarlatLinearTransformationType<StressStensor>&,
    			const BarlatExponentType,
    			const BarlatStressType<StressStensor>);
    /*!
     * \brief compute the Barlat yield stress and the its first derivative
     * \tparam StressStensor: type of the stress tensor
     * \tparam HosfordExponentType: type of the hosford exponent
     * (could be a numeric type or an integer type)
     * \tparam es: eigen solver to be used
     * \param[in] s: stress tensor
     * \param[in] l1: first linear transformation
     * \param[in] l2: second linear transformation
     * \param[in] a: Barlat exponent
     * \param[in] e: criterion used to check if the stress are null
     */
    template<typename StressStensor,
	     typename BarlatExponentType,
    	     tfel::math::stensor_common::EigenSolver =
    	     tfel::math::stensor_common::TFELEIGENSOLVER>
    std::tuple<BarlatStressType<StressStensor>,
    	       BarlatStressNormalType<StressStensor>>
    computeBarlatStressNormal(const StressStensor&,
    			      const BarlatLinearTransformationType<StressStensor>&,
    			      const BarlatLinearTransformationType<StressStensor>&,
    			      const BarlatExponentType,
    			      const BarlatStressType<StressStensor>);
    /*!
     * \brief compute the Barlat yield stress and its first and second derivatives
     * \tparam StressStensor: type of the stress tensor
     * \tparam HosfordExponentType: type of the hosford exponent
     * (could be a numeric type or an integer type)
     * \tparam es: eigen solver to be used
     * \param[in] s: stress tensor
     * \param[in] l1: first linear transformation
     * \param[in] l2: second linear transformation
     * \param[in] a: Barlat exponent
     * \param[in] e: criterion used to check if the stress are null
     */
    template<typename StressStensor,
	     typename BarlatExponentType,
    	     tfel::math::stensor_common::EigenSolver =
    	     tfel::math::stensor_common::TFELEIGENSOLVER>
    std::tuple<BarlatStressType<StressStensor>,
    	       BarlatStressNormalType<StressStensor>,
    	       BarlatStressSecondDerivativeType<StressStensor>>
    computeBarlatStressSecondDerivative(const StressStensor&,
    					const BarlatLinearTransformationType<StressStensor>&,
    					const BarlatLinearTransformationType<StressStensor>&,
    					const BarlatExponentType,
    					const BarlatStressType<StressStensor>);
    
  } // end of namespace material
  
} // end of namespace tfel

#include"TFEL/Material/Barlat.ixx"

#endif /* LIB_TFEL_MATERIAL_BARLAT_HXX */
