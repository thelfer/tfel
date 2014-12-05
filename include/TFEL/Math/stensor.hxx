/*!
 * \file   include/TFEL/Math/stensor.hxx  
 * \brief    
 * \author Helfer Thomas
 * \date   04 May 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_STENSOR_H_
#define _LIB_TFEL_STENSOR_H_ 

#include<string>
#include<cassert>
#include<cstddef>
#include<type_traits>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/TypeTraits/IsScalar.hxx"
#include"TFEL/TypeTraits/BaseType.hxx"
#include"TFEL/TypeTraits/IsAssignableTo.hxx"
#include"TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include"TFEL/Math/Function/Power.hxx"
#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Vector/VectorConcept.hxx"
#include"TFEL/Math/Stensor/StensorConcept.hxx"
#include"TFEL/Math/Stensor/StensorConceptOperations.hxx"
#include"TFEL/Math/Stensor/StensorExpr.hxx"
#include"TFEL/Math/Matrix/MatrixConcept.hxx"

#include"TFEL/Math/Forward/tvector.hxx"
#include"TFEL/Math/Forward/tmatrix.hxx"
#include"TFEL/Math/Forward/stensor.hxx"

#include"TFEL/Math/fsarray.hxx"

namespace tfel{
  
  namespace math {

    template<unsigned short N, typename T>
    struct TFEL_VISIBILITY_LOCAL StensorTraits<stensor<N,T> >
    {
      typedef T NumType;
      typedef unsigned short IndexType;
      static constexpr unsigned short dime = N;
    };

    /*!
     * \brief a base for stensor or classes acting like stensor.
     * \param Child : child class
     * \param N     : spatial dimension
     * \param T     : numerical type
     */
    template<typename Child>
    struct stensor_base
    {
      /*!
       * Assignement operator
       */
      template<typename StensorType>
      TFEL_MATH_INLINE 
      typename std::enable_if<
	tfel::meta::Implements<StensorType,StensorConcept>::cond &&
	StensorTraits<Child>::dime==StensorTraits<StensorType>::dime &&
	tfel::typetraits::IsAssignableTo<typename StensorTraits<StensorType>::NumType,
					 typename StensorTraits<Child>::NumType>::cond,
	Child&>::type
      operator=(const StensorType&);
      //! Assignement operator
      template<typename StensorType>
      TFEL_MATH_INLINE 
      typename std::enable_if<
	tfel::meta::Implements<StensorType,StensorConcept>::cond &&
	StensorTraits<Child>::dime==StensorTraits<StensorType>::dime &&
	tfel::typetraits::IsAssignableTo<typename StensorTraits<StensorType>::NumType,
					 typename StensorTraits<Child>::NumType>::cond,
	Child&>::type
      operator+=(const StensorType&);
      //! Assignement operator
      template<typename StensorType>
      TFEL_MATH_INLINE 
      typename std::enable_if<
	tfel::meta::Implements<StensorType,StensorConcept>::cond &&
	StensorTraits<Child>::dime==StensorTraits<StensorType>::dime &&
	tfel::typetraits::IsAssignableTo<typename StensorTraits<StensorType>::NumType,
					 typename StensorTraits<Child>::NumType>::cond,
	Child&>::type
      operator-=(const StensorType&);
      /*!
       * operator*=
       */
      template<typename T2>
      TFEL_MATH_INLINE 
      typename std::enable_if<
	tfel::typetraits::IsScalar<T2>::cond&&
	std::is_same<typename ResultType<typename StensorTraits<Child>::NumType,
					 T2,OpMult>::type,
		     typename StensorTraits<Child>::NumType>::value,
		       Child&>::type
      operator*=(const T2);
      /*!
       * operator/=
       */
      template<typename T2>
      TFEL_MATH_INLINE 
      typename std::enable_if<
	tfel::typetraits::IsScalar<T2>::cond&&
        std::is_same<typename ResultType<typename StensorTraits<Child>::NumType,
						   T2,OpDiv>::type,
			       typename StensorTraits<Child>::NumType>::value,
	Child&>::type
      operator/=(const T2);
    }; // end of struct stensor_base

    template<unsigned short N,typename T>
    struct TFEL_VISIBILITY_LOCAL stensor
      : public StensorConcept<stensor<N,T> >,
	public stensor_base<stensor<N,T> >,
	public fsarray<StensorDimeToSize<N>::value,T>
    {
      /*
       * This is a StensorConcept requirement.
       */
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \brief Default Constructor 
       */
      explicit stensor()
      {}
      /*!
       * \brief Default Constructor 
       * \param T, value used to initialise the components of the stensor 
       */
      explicit stensor(const T);
      /*!
       * \brief Default Constructor.
       * \param const typename tfel::typetraits::BaseType<T>::type*
       * const, pointer to a tabular used to initialise the components
       * of the stensor. This tabular is left unchanged.
       */
      explicit stensor(const typename tfel::typetraits::BaseType<T>::type* const);
      /*!
       * \brief Default Constructor.
       * \param typename tfel::typetraits::BaseType<T>::type* const,
       * pointer to a tabular used to initialise the components of the stensor. 
       */
      explicit stensor(typename tfel::typetraits::BaseType<T>::type* const);
      /*!
       * \brief Copy Constructor
       */
      stensor(const stensor<N,T>&);
      /*!
       * \brief constructor
       */
      template<typename T2,
	       typename std::enable_if<tfel::typetraits::IsAssignableTo<T2,T>::cond,bool>::type = true>
      stensor(const stensor<N,T2>&);
      /*!
       * copy from stensor expression template object
       */
      template<typename T2,typename Expr,
	       typename std::enable_if<tfel::typetraits::IsAssignableTo<T2,T>::cond,bool>::type = true>
      stensor(const StensorExpr<stensor<N,T2>,Expr>& src);
      /*!
       * \brief Import from Voigt
       */
      template<typename T2>
      TFEL_MATH_INLINE2 
      typename std::enable_if<
	tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
	void
	>::type
      importVoigt(const T2* const);
      /*!
       * Import from Tab (Voigt notations for stresses)
       */
      template<typename T2>
      TFEL_MATH_INLINE2 
      typename std::enable_if<
	tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
	void>::type
      importTab(const T2* const);
      /*!
       * Export to Tab (Voigt notations for stresses)
       */
      template<typename T2>
      TFEL_MATH_INLINE2 
      typename std::enable_if<
	tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
	void>::type
      exportTab(T2* const) const;
      /*!
       * Write to Tab
       */
      template<typename T2>
      TFEL_MATH_INLINE2 
      typename std::enable_if<
	tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
	void
	>::type
      write(T2* const) const;
      /*!
       * Import values
       */
      template<typename T2>
      TFEL_MATH_INLINE2 
      typename std::enable_if<
	tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
	void>::type
      import(const T2* const);
      //! using stensor_base::operator=
      using stensor_base<stensor>::operator=;
      /*!
       * compute eigenvalues
       * \param[in] b  : refine eigenvalues
       */
      TFEL_MATH_INLINE2 void
      computeEigenValues(T&,T&,T&,
			 const bool = false) const;
      /*!
       * compute eigenvalues
       * \param[in] b  : refine eigenvalues
       */
      TFEL_MATH_INLINE2 void
      computeEigenValues(tvector<3u,T>&,
			 const bool = false) const;
      /*!
       * compute eigenvector associated to a given eigenvalue
       * \note the given vector must be properly allocated and the
       * eigenvector is stored in the three first locations
       * \note no check made that the given value is an eigenvalue of
       * the tensor
       * \note the eigen value geometric multiplicity must be egal to
       * one.
       * \param[out] ev : eigen vector
       * \param[in]  vp : eigenvalue
       */
      template<typename VectorType>
      TFEL_MATH_INLINE2 bool
      computeEigenVector(VectorType&,const T) const;
      /*!
       * compute eigenvectors and eigenvalues 
       * \param[out] vp : eigenvalues
       * \param[out] m  : rotation matrix
       * \param[in]  b  : refine eigenvalues
       */
      TFEL_MATH_INLINE2 bool
      computeEigenVectors(tvector<3u,T>&,
			  tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>&,
			  const bool = false) const;
      /*!
       * change basis
       */
      TFEL_MATH_INLINE2 void
      changeBasis(const tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>&);
      /*!
       * Return Identity
       */
      TFEL_MATH_INLINE static const stensor<N,T>& Id(void);

      TFEL_MATH_INLINE const T& operator()(const unsigned short) const;      
      TFEL_MATH_INLINE       T& operator()(const unsigned short);

      TFEL_MATH_INLINE const T& operator[](const unsigned short) const;      
      TFEL_MATH_INLINE       T& operator[](const unsigned short);
       /*
       * Return the RunTimeProperties of the tvector
       * \return tvector::RunTimeProperties
       */
      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties(void) const;
      /*!
       * copy the value from a container
       */
      template<typename InputIterator>
      TFEL_MATH_INLINE2 void 
      copy(const InputIterator src);

      /*!
       * build a symmetric tensor from a matrix
       * \param[in] m : matrix
       */
      template<typename MatrixType>
      static TFEL_MATH_INLINE2
      typename std::enable_if<
	tfel::typetraits::IsAssignableTo<typename MatrixTraits<MatrixType>::NumType,T>::cond,
	stensor<N,T> >::type
       buildFromMatrix(const MatrixType&);

      /*!
       * build a symmetric tensor from the diadic product of a vector with itself
       * \param[in] v : vector
       */
      template<typename VectorType>
      static TFEL_MATH_INLINE2
      typename std::enable_if<
	tfel::typetraits::IsAssignableTo<
	  typename ComputeUnaryResult<typename VectorTraits<VectorType>::NumType,
				      Power<2> >::Result,T
	  >::cond,
	stensor<N,T> >::type
       buildFromVectorDiadicProduct(const VectorType&);

      /*!
       * build a symmetric tensor from the symmetric diadic product of two vectors
       * \param[in] v1 : first  vector
       * \param[in] v1 : second vector
       */
      template<typename VectorType,
	       typename VectorType2>
      static TFEL_MATH_INLINE2
      typename std::enable_if<
	tfel::typetraits::IsAssignableTo<
	  typename ComputeBinaryResult<typename VectorTraits<VectorType>::NumType,
				       typename VectorTraits<VectorType2>::NumType,
				       OpMult>::Result,T
	  >::cond,
	stensor<N,T> >::type
      buildFromVectorsSymmetricDiadicProduct(const VectorType&,
					     const VectorType2&);
    
      /*!
       * build a symmetric tensor from its eigen values and vectors
       * \param[in] v1 : first  eigen value
       * \param[in] v2 : second eigen value
       * \param[in] v2 : third  eigen value
       * \param[in] m  : matrix containing the eigen vectors
       */
      static TFEL_MATH_INLINE2
      stensor<N,T>
      buildFromEigenValuesAndVectors(const T&,const T&,const T&,
				     const tmatrix<3u,3u,
				     typename tfel::typetraits::BaseType<T>::type>&);

      /*!
       * build a symmetric tensor from its eigen values and vectors
       * \param[in] vp : eigen values
       * \param[in] m  : matrix containing the eigen vectors
       */
      static TFEL_MATH_INLINE2
      stensor<N,T>
      buildFromEigenValuesAndVectors(const tvector<3u,T>&,
				     const tmatrix<3u,3u,
				     typename tfel::typetraits::BaseType<T>::type>&);

      
      /*!
       * build the logarithm of a symmetric tensor given through its eigen values and vectors
       * \param[in] v1 : first  eigen value
       * \param[in] v2 : second eigen value
       * \param[in] v2 : third  eigen value
       * \param[in] m  : matrix containing the eigen vectors
       */
      static TFEL_MATH_INLINE2
      stensor<N,T>
      buildLogarithmFromEigenValuesAndVectors(const T&,const T&,const T&,
					      const tmatrix<3u,3u,
					      typename tfel::typetraits::BaseType<T>::type>&);

      /*!
       * build the logarithm of a symmetric tensor given through its eigen values and vectors
       * \param[in] vp : eigen values
       * \param[in] m  : matrix containing the eigen vectors
       */
      static TFEL_MATH_INLINE2
      stensor<N,T>
      buildLogarithmFromEigenValuesAndVectors(const tvector<3u,T>&,
					      const tmatrix<3u,3u,
					      typename tfel::typetraits::BaseType<T>::type>&);

      /*!
       * build the absolute value of a symmetric tensor given through its eigen values and vectors
       * \param[in] v1 : first  eigen value
       * \param[in] v2 : second eigen value
       * \param[in] v2 : third  eigen value
       * \param[in] m  : matrix containing the eigen vectors
       */
      static TFEL_MATH_INLINE2
      stensor<N,T>
      buildAbsoluteValueFromEigenValuesAndVectors(const T&,const T&,const T&,
						  const tmatrix<3u,3u,
						  typename tfel::typetraits::BaseType<T>::type>&);

      /*!
       * build the absolute value of a symmetric tensor given through its eigen values and vectors
       * \param[in] vp : eigen values
       * \param[in] m  : matrix containing the eigen vectors
       */
      static TFEL_MATH_INLINE2
      stensor<N,T>
      buildAbsoluteValueFromEigenValuesAndVectors(const tvector<3u,T>&,
						  const tmatrix<3u,3u,
						  typename tfel::typetraits::BaseType<T>::type>&);

      /*!
       * build the positive part of a symmetric tensor given through its eigen values and vectors
       * \param[in] v1 : first  eigen value
       * \param[in] v2 : second eigen value
       * \param[in] v2 : third  eigen value
       * \param[in] m  : matrix containing the eigen vectors
       */
      static TFEL_MATH_INLINE2
      stensor<N,T>
      buildPositivePartFromEigenValuesAndVectors(const T&,const T&,const T&,
						 const tmatrix<3u,3u,
						 typename tfel::typetraits::BaseType<T>::type>&);

      /*!
       * build the positive part of a symmetric tensor given through its eigen values and vectors
       * \param[in] vp : eigen values
       * \param[in] m  : matrix containing the eigen vectors
       */
      static TFEL_MATH_INLINE2
      stensor<N,T>
      buildPositivePartFromEigenValuesAndVectors(const tvector<3u,T>&,
						 const tmatrix<3u,3u,
						 typename tfel::typetraits::BaseType<T>::type>&);

      /*!
       * build the positive part of a symmetric tensor given through its eigen values and vectors
       * \param[in] v1 : first  eigen value
       * \param[in] v2 : second eigen value
       * \param[in] v2 : third  eigen value
       * \param[in] m  : matrix containing the eigen vectors
       */
      static TFEL_MATH_INLINE2
      stensor<N,T>
      buildNegativePartFromEigenValuesAndVectors(const T&,const T&,const T&,
						 const tmatrix<3u,3u,
						 typename tfel::typetraits::BaseType<T>::type>&);

      /*!
       * build the negative part of a symmetric tensor given through its eigen values and vectors
       * \param[in] vp : eigen values
       * \param[in] m  : matrix containing the eigen vectors
       */
      static TFEL_MATH_INLINE2
      stensor<N,T>
      buildNegativePartFromEigenValuesAndVectors(const tvector<3u,T>&,
						 const tmatrix<3u,3u,
						 typename tfel::typetraits::BaseType<T>::type>&);
      /*!
       * compute the eigenvalues derivatives with respect with this tensor
       * \param[out] n0 : derivative of the first  eigenvalue
       * \param[out] n1 : derivative of the second eigenvalue
       * \param[out] n2 : derivative of the third  eigenvalue
       * \param[in]  m  : eigenvectors
       * This eigenvalues of the derivatives is given by~:
       * \[
       * \frac{\displaystyle \partial}{\lambda_{i}}{\displaystyle \underbrace{s}}=\underbrace{n}_{i}=\vec{e}_{i}\otimes\vec{e}_{i}
       * \]
       * \(\underbrace{n}_{i}\) are the eigen tensors associated to the given tensor.
       */
      template<typename StensorType>
      static TFEL_MATH_INLINE2
      typename std::enable_if<
	  (tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
	(StensorTraits<StensorType>::dime==N)&&
	(tfel::typetraits::IsAssignableTo<typename tfel::typetraits::BaseType<T>::type,
					  typename StensorTraits<StensorType>::NumType>::cond),
	void>::type
      computeEigenValuesDerivatives(StensorType&,
				    StensorType&,
				    StensorType&,
				    const tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>&);
      /*!
       * compute the "eigentensors"
       * \param[out] n0  : derivative of the first  eigenvalue
       * \param[out] n1  : derivative of the second eigenvalue
       * \param[out] n2  : derivative of the third  eigenvalue
       * \param[in]  m   : eigenvectors
       */
      template<typename StensorType>
      static TFEL_MATH_INLINE2
      typename std::enable_if<
	(tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
	(StensorTraits<StensorType>::dime==N)&&
	(tfel::typetraits::IsAssignableTo<typename tfel::typetraits::BaseType<T>::type,
					  typename StensorTraits<StensorType>::NumType>::cond),
	void>::type
      computeEigenTensors(StensorType&,
			  StensorType&,
			  StensorType&,
			  const tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>&);
      /*!
       * compute the "eigentensors" derivatives
       * \param[out] dn0_ds : derivative of the first  eigenvalue
       * \param[out] dn1_ds : derivative of the second eigenvalue
       * \param[out] dn2_ds : derivative of the third  eigenvalue
       * \param[in]  vp     : eigen values
       * \param[in]  m      : eigenvectors
       * \param[in]  eps    : numerical parameter for regularisation
       */
      template<typename ST2toST2Type>
      static TFEL_MATH_INLINE2
      typename std::enable_if<
	(tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond)&&
	(ST2toST2Traits<ST2toST2Type>::dime==N)&&
	(tfel::typetraits::IsAssignableTo<typename ComputeBinaryResult<typename tfel::typetraits::BaseType<T>::type,
								       T,OpDiv>::Result,
					  typename ST2toST2Traits<ST2toST2Type>::NumType>::cond),
	void>::type
      computeEigenTensorsDerivatives(ST2toST2Type&,
				     ST2toST2Type&,
				     ST2toST2Type&,
				     const tvector<3u,T>&,
				     const tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>&,
				     const T);

    private:      
      //! a simple check
      TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));
    }; // end of class stensor

    /*!
     * export the given vector to an array of the 
     */
    template<unsigned short N,typename T,
	     typename OutputIterator>
    TFEL_MATH_INLINE2
    typename std::enable_if<
      tfel::typetraits::IsScalar<T>::cond,
      void>::type
    exportToBaseTypeArray(const stensor<N,T>&,
			  OutputIterator);

    /*!
     * compute the tresca stress
     * for a symmetric tensor
     *
     * \param s : symmetric tensor
     */
    template<unsigned short N,
	     typename T>
    T tresca(const stensor<N,T>&,
	     const bool = false);
    
    /*!
     * compute the tresca stress
     * for a symmetric tensor.
     *
     * Partial specialisation in 1D
     *
     * \param s : symmetric tensor
     */
    template<typename T>
    T tresca(const stensor<1u,T>&,
	     const bool = false);

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,StensorConcept>::cond,
      stensor<StensorTraits<StensorType>::dime,
	      typename StensorTraits<StensorType>::NumType>
    >::type
    square_root(const StensorType&);

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,StensorConcept>::cond,
      typename ComputeUnaryResult<typename StensorTraits<StensorType>::NumType,Power<3> >::Result
    >::type
    det(const StensorType&);
  
    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,StensorConcept>::cond,
      stensor<StensorTraits<StensorType>::dime,
	      typename ComputeBinaryResult<typename tfel::typetraits::BaseType<typename StensorTraits<StensorType>::NumType>::type,
					   typename StensorTraits<StensorType>::NumType,OpDiv>::Result>
      >::type
    invert(const StensorType&);

    /*!
     * \brief compute the logarithm of a symmetric tensor
     * \param s : tensor
     * \param b : if true, refinement of eigen values is performed
     */
    template<typename StensorType>
    TFEL_MATH_INLINE2
    typename std::enable_if<
      ((tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
       (StensorTraits<StensorType>::dime==1u) &&
       (tfel::typetraits::IsFundamentalNumericType<typename StensorTraits<StensorType>::NumType>::cond)),
      stensor<1u,typename StensorTraits<StensorType>::NumType>
    >::type
    logarithm(const StensorType&,
	      const bool = false);

    /*!
     * \brief compute the logarithm of a symmetric tensor
     * \param s : tensor
     * \param b : if true, refinement of eigen values is performed
     */
    template<typename StensorType>
    TFEL_MATH_INLINE2
    typename std::enable_if<
      ((tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
       ((StensorTraits<StensorType>::dime==2u)||
	(StensorTraits<StensorType>::dime==3u))&&
       (tfel::typetraits::IsFundamentalNumericType<typename StensorTraits<StensorType>::NumType>::cond)),
      stensor<StensorTraits<StensorType>::dime,typename StensorTraits<StensorType>::NumType>
    >::type
    logarithm(const StensorType&,
	      const bool = false);

    /*!
     * \brief compute the absolute value of a symmetric tensor
     * \param s : tensor
     * \param b : if true, refinement of eigen values is performed
     */
    template<typename StensorType>
    TFEL_MATH_INLINE2
    typename std::enable_if<
      ((tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
       (StensorTraits<StensorType>::dime==1u) &&
       (tfel::typetraits::IsFundamentalNumericType<typename StensorTraits<StensorType>::NumType>::cond)),
      stensor<1u,typename StensorTraits<StensorType>::NumType>
    >::type
    absolute_value(const StensorType&,
		   const bool = false);

    /*!
     * \brief compute the absolute value of a symmetric tensor
     * \param s : tensor
     * \param b : if true, refinement of eigen values is performed
     */
    template<typename StensorType>
    TFEL_MATH_INLINE2
    typename std::enable_if<
      ((tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
       ((StensorTraits<StensorType>::dime==2u)||
	(StensorTraits<StensorType>::dime==3u))&&
       (tfel::typetraits::IsFundamentalNumericType<typename StensorTraits<StensorType>::NumType>::cond)),
      stensor<StensorTraits<StensorType>::dime,typename StensorTraits<StensorType>::NumType>
    >::type
    absolute_value(const StensorType&,
		   const bool = false);

    /*!
     * \brief compute the positive part of a symmetric tensor
     * \param s : tensor
     * \param b : if true, refinement of eigen values is performed
     */
    template<typename StensorType>
    TFEL_MATH_INLINE2
    typename std::enable_if<
      ((tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
       (StensorTraits<StensorType>::dime==1u) &&
       (tfel::typetraits::IsFundamentalNumericType<typename StensorTraits<StensorType>::NumType>::cond)),
      stensor<1u,typename StensorTraits<StensorType>::NumType>
    >::type
    positive_part(const StensorType&,
		  const bool = false);

    /*!
     * \brief compute the positive part of a symmetric tensor
     * \param s : tensor
     * \param b : if true, refinement of eigen values is performed
     */
    template<typename StensorType>
    TFEL_MATH_INLINE2
    typename std::enable_if<
      ((tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
       ((StensorTraits<StensorType>::dime==2u)||
	(StensorTraits<StensorType>::dime==3u))&&
       (tfel::typetraits::IsFundamentalNumericType<typename StensorTraits<StensorType>::NumType>::cond)),
      stensor<StensorTraits<StensorType>::dime,typename StensorTraits<StensorType>::NumType>
    >::type
    positive_part(const StensorType&,
		  const bool = false);
    
    /*!
     * \brief compute the negative part of a symmetric tensor
     * \param s : tensor
     * \param b : if true, refinement of eigen values is performed
     */
    template<typename StensorType>
    TFEL_MATH_INLINE2
    typename std::enable_if<
      ((tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
       (StensorTraits<StensorType>::dime==1u) &&
       (tfel::typetraits::IsFundamentalNumericType<typename StensorTraits<StensorType>::NumType>::cond)),
      stensor<1u,typename StensorTraits<StensorType>::NumType>
    >::type
    negative_part(const StensorType&,
		  const bool = false);
    
    /*!
     * \brief compute the negative part of a symmetric tensor
     * \param s : tensor
     * \param b : if true, refinement of eigen values is performed
     */
    template<typename StensorType>
    TFEL_MATH_INLINE2
    typename std::enable_if<
      ((tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
       ((StensorTraits<StensorType>::dime==2u)||
	(StensorTraits<StensorType>::dime==3u))&&
       (tfel::typetraits::IsFundamentalNumericType<typename StensorTraits<StensorType>::NumType>::cond)),
      stensor<StensorTraits<StensorType>::dime,typename StensorTraits<StensorType>::NumType>
    >::type
    negative_part(const StensorType&,
		  const bool = false);
    
  } // end of namespace math

  namespace typetraits{
    
    template<unsigned short N,typename T2,typename T>
    struct IsAssignableTo<tfel::math::stensor<N,T2>,
			  tfel::math::stensor<N,T> >
    {
      /*!
       *  Result
       */
      static constexpr bool cond = IsAssignableTo<T2,T>::cond;
    };

  } // end of namespace typetraits

} // end of namespace tfel

#include"TFEL/Math/Stensor/StensorSizeToDime.hxx"
#include"TFEL/Math/Stensor/stensor.ixx"
#include"TFEL/Math/Stensor/stensorResultType.hxx"
#include"TFEL/Math/Stensor/DecompositionInPositiveAndNegativeParts.hxx"

#endif /* _LIB_TFEL_STENSOR_H */

