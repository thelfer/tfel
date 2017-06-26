/*!
 * \file   LogarithmicStrainHandler.hxx
 * \brief    
 * \author HELFER Thomas 202608
 * \date   25 juin 2017
 */

#ifndef LIB_TFEL_MATH_LOGARITHMICSTRAINHANDLER_HXX
#define LIB_TFEL_MATH_LOGARITHMICSTRAINHANDLER_HXX

#include"TFEL/Math/tensor.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/st2tost2.hxx"

namespace tfel
{

  namespace material
  {
    /*!
     * \brief This class gathers various methods used to
     * \tparam N: space dimension
     * \tparam StressType: type used to describe the stress.
     */
    template<unsigned short N,typename StressType>
    struct LogarithmicStrainHandler;

    /*!
     * \brief partial specialisation of the `LogarithmicStrainHandler`
     * class for 3D.
     */
    template<typename StressType>
    struct LogarithmicStrainHandler<1u,StressType>
    {
      //! numeric type
      using real = tfel::typetraits::base_type<StressType>;
      //! a simple alias
      using stress = StressType;
      //! a simple alias
      using DeformationGradient = tfel::math::tensor<1u,real>;
      //! a simple alias
      using StrainStensor = tfel::math::stensor<2u,real>;
      //! a simple alias
      using StressStensor = tfel::math::stensor<1u,stress>;
      //! a simple alias
      using TangentOperator = tfel::math::st2tost2<1u,stress>;
      //! a simple alias
      using size_type = unsigned short;
      //
      static constexpr const real eps = 1.e-14;
      /*!
       * \brief constructor
       * \param[in] F1: deformation gradient
       */
      LogarithmicStrainHandler(const DeformationGradient&);
      /*!
       * \return the logarithmic strain
       */
      StrainStensor getHenckyLogarithmicStrain() const;
      /*!
       * \return the Cauchy stress from the dual of the logarithmic strain
       * \param[in] T: dual of the logarithmic strain
       */
      StressStensor
      convertToSecondPiolaKirchhoffStress(const StressStensor&) const;
      /*!
       * \return the dual of the logarithmic strain
       * \param[in] S: Second Piola Kirchhoff stress.
       */
      StressStensor
      convertFromSecondPiolaKirchhoffStress(const StressStensor&) const;
      /*!
       * \return the Cauchy stress from the dual of the logarithmic strain
       * \param[in] T: dual of the logarithmic strain
       */
      StressStensor convertToCauchyStress(const StressStensor&) const;
      /*!
       * \return the dual of the logarithmic strain
       * \param[in] s: Cauchy stress.
       */
      StressStensor convertFromCauchyStress(const StressStensor&) const;
      /*!
       * \return the material tangent moduli
       * \param[in] Ks: tangent moduli in the logarithmic space
       * \param[in]  T: dual of the logarithmic strain
       */
      TangentOperator
      convertToMaterialTangentModuli(const TangentOperator&,
				     const StressStensor&) const;
    private:
      //! deformation gradient
      DeformationGradient F;
    }; // end of LogarithmicStrainHandler<3u,StressType>

    /*!
     * \brief partial specialisation of the `LogarithmicStrainHandler`
     * class for 2D.
     */
    template<typename StressType>
    struct LogarithmicStrainHandler<2u,StressType>
    {
      //! numeric type
      using real = tfel::typetraits::base_type<StressType>;
      //! a simple alias
      using stress = StressType;
      //! a simple alias
      using DeformationGradient = tfel::math::tensor<2u,real>;
      //! a simple alias
      using StrainStensor = tfel::math::stensor<2u,real>;
      //! a simple alias
      using StressStensor = tfel::math::stensor<2u,stress>;
      //! a simple alias
      using TangentOperator = tfel::math::st2tost2<2u,stress>;
      //! a simple alias
      using size_type = unsigned short;
      //
      static constexpr const real eps = 1.e-14;
      /*!
       * \brief constructor
       * \param[in] F1: deformation gradient
       */
      LogarithmicStrainHandler(const DeformationGradient&);
      /*!
       * \return the Cauchy stress from the dual of the logarithmic strain
       * \param[in] T: dual of the logarithmic strain
       */
      StressStensor
      convertToSecondPiolaKirchhoffStress(const StressStensor&) const;
      /*!
       * \return the logarithmic strain
       */
      StrainStensor getHenckyLogarithmicStrain() const;
      /*!
       * \return the dual of the logarithmic strain
       * \param[in] S: Second Piola Kirchhoff stress.
       */
      StressStensor
      convertFromSecondPiolaKirchhoffStress(const StressStensor&) const;
      /*!
       * \return the Cauchy stress from the dual of the logarithmic strain
       * \param[in] T: dual of the logarithmic strain
       */
      StressStensor convertToCauchyStress(const StressStensor&) const;
      /*!
       * \return the dual of the logarithmic strain
       * \param[in] s: Cauchy stress.
       */
      StressStensor convertFromCauchyStress(const StressStensor&) const;
      /*!
       * \return the material tangent moduli
       * \param[in] Ks: tangent moduli in the logarithmic space
       * \param[in]  T: dual of the logarithmic strain
       */
      TangentOperator
      convertToMaterialTangentModuli(const TangentOperator&,
				     const StressStensor&) const;
    private:
      /*!
       * \brief: an helper class used to build the a
       * `LogarithmicStrainHandler` object.
       */
      struct Builder{
	/*!
	 * \brief constructor
	 * \param[in] F1: deformation gradient
	 */
	Builder(const DeformationGradient&);
	/*!
	 * derivative of the logarithmic strain with respect to the
	 * right Cauchy-Green tensor
	 */
	tfel::math::st2tost2<2u,real> p;
	//! eigenvectors of the right Cauchy-Green tensor
	tfel::math::tmatrix<3u,3u,real>  m;
	//! eigenvalues of the right Cauchy-Green tensor
	tfel::math::tvector<3u,real>    vp;
	//! half the logarithm of the eigenvalues
	tfel::math::tvector<3u,real>    e;
      };
      /*!
       * \brief build from the `LogarithmicStrainHandlerBuilder`.
       * This trick is used to ensure that the members of the
       * `LogarithmicStrainHandler` class are `const`.
       * \param[in] b: builder object
       * \param[in] F1: deformation gradient
       */ 
      LogarithmicStrainHandler(Builder&&,
			       const DeformationGradient&);
      /*!
       * derivative of the logarithmic strain with respect to the
       * right Cauchy-Green tensor
       */
      const tfel::math::st2tost2<2u,real> p;
      //! deformation gradient
      const DeformationGradient F;
      //! eigenvectors of the right Cauchy-Green tensor
      const tfel::math::tmatrix<3u,3u,real>  m;
      //! eigenvalues of the right Cauchy-Green tensor
      const tfel::math::tvector<3u,real>    vp;
      //! eigenvalues of the right Cauchy-Green tensor
      const tfel::math::tvector<3u,real>    e;
    };
    
    /*!
     * \brief partial specialisation of the `LogarithmicStrainHandler`
     * class for 3D.
     */
    template<typename StressType>
    struct LogarithmicStrainHandler<3u,StressType>
    {
      //! numeric type
      using real = tfel::typetraits::base_type<StressType>;
      //! a simple alias
      using stress = StressType;
      //! a simple alias
      using DeformationGradient = tfel::math::tensor<3u,real>;
      //! a simple alias
      using StrainStensor = tfel::math::stensor<2u,real>;
      //! a simple alias
      using StressStensor = tfel::math::stensor<3u,stress>;
      //! a simple alias
      using TangentOperator = tfel::math::st2tost2<3u,stress>;
      //! a simple alias
      using size_type = unsigned short;
      //
      static constexpr const real eps = 1.e-14;
      /*!
       * \brief constructor
       * \param[in] F1: deformation gradient
       */
      LogarithmicStrainHandler(const DeformationGradient&);
      /*!
       * \return the the logarithmic strain
       */
      StrainStensor getHenckyLogarithmicStrain() const;
      /*!
       * \return the Cauchy stress from the dual of the logarithmic strain
       * \param[in] T: dual of the logarithmic strain
       */
      StressStensor
      convertToSecondPiolaKirchhoffStress(const StressStensor&) const;
      /*!
       * \return the dual of the logarithmic strain
       * \param[in] S: Second Piola Kirchhoff stress.
       */
      StressStensor
      convertFromSecondPiolaKirchhoffStress(const StressStensor&) const;
      /*!
       * \return the Cauchy stress from the dual of the logarithmic strain
       * \param[in] T: dual of the logarithmic strain
       */
      StressStensor convertToCauchyStress(const StressStensor&) const;
      /*!
       * \return the dual of the logarithmic strain
       * \param[in] s: Cauchy stress.
       */
      StressStensor convertFromCauchyStress(const StressStensor&) const;
      /*!
       * \return the material tangent moduli
       * \param[in] Ks: tangent moduli in the logarithmic space
       * \param[in]  T: dual of the logarithmic strain
       */
      TangentOperator
      convertToMaterialTangentModuli(const TangentOperator&,
				     const StressStensor&) const;
    private:
      /*!
       * \brief: an helper class used to build the a
       * `LogarithmicStrainHandler` object.
       */
      struct Builder{
	/*!
	 * \brief constructor
	 * \param[in] F1: deformation gradient
	 */
	Builder(const DeformationGradient&);
	/*!
	 * derivative of the logarithmic strain with respect to the
	 * right Cauchy-Green tensor
	 */
	tfel::math::st2tost2<3u,real> p;
	//! eigenvectors of the right Cauchy-Green tensor
	tfel::math::tmatrix<3u,3u,real>  m;
	//! eigenvalues of the right Cauchy-Green tensor
	tfel::math::tvector<3u,real>    vp;
	//! half the logarithm of the eigenvalues
	tfel::math::tvector<3u,real>    e;
      };
      /*!
       * \brief build from the `LogarithmicStrainHandlerBuilder`.
       * This trick is used to ensure that the members of the
       * `LogarithmicStrainHandler` class are `const`.
       * \param[in] b: builder object
       * \param[in] F1: deformation gradient
       */ 
      LogarithmicStrainHandler(Builder&&,
			       const DeformationGradient&);
      /*!
       * derivative of the logarithmic strain with respect to the
       * right Cauchy-Green tensor
       */
      const tfel::math::st2tost2<3u,real> p;
      //! deformation gradient
      const DeformationGradient F;
      //! eigenvectors of the right Cauchy-Green tensor
      const tfel::math::tmatrix<3u,3u,real>  m;
      //! eigenvalues of the right Cauchy-Green tensor
      const tfel::math::tvector<3u,real>    vp;
      //! eigenvalues of the right Cauchy-Green tensor
      const tfel::math::tvector<3u,real>    e;
    };
    
  } // end of namespace material

} // end of namespace material

#include"TFEL/Material/LogarithmicStrainHandler.ixx"

#endif /* LIB_TFEL_MATH_LOGARITHMICSTRAINHANDLER_HXX */
