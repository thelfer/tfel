/*!
 * \file   include/TFEL/Math/Stensor/stensor.ixx
 * \brief  This file implements the methods of the class stensor.
 * \author Helfer Thomas
 * \date   02 jun 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_STENSOR_IXX_
#define _LIB_TFEL_STENSOR_IXX_ 

#include <cmath>
#include <iterator>

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"

#include"TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include"TFEL/Math/General/StorageTraits.hxx"
#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/Vector/VectorUtilities.hxx"

#include"TFEL/Math/Stensor/Internals/StensorComputeEigenValues.hxx"
#include"TFEL/Math/Stensor/Internals/StensorComputeEigenVectors.hxx"
#include"TFEL/Math/Stensor/Internals/StensorImport.hxx"
#include"TFEL/Math/Stensor/Internals/StensorExport.hxx"
#include"TFEL/Math/Stensor/Internals/StensorChangeBasis.hxx"
#include"TFEL/Math/Forward/st2tost2.hxx"

namespace tfel{

  namespace math {

#ifndef DOXYGENSPECIFIC

    namespace internals{
      template<unsigned short N>
      class StensorComputeEigenVectors_;

      template<unsigned short N>
      struct StensorDeterminant;

      template<>
      struct StensorDeterminant<1u>
      {
	template<typename T>
	static typename ComputeUnaryResult<T,Power<3> >::Result
	exe(const tfel::math::stensor<1u,T>& s)
	{
	  return s(0)*s(1)*s(2);
	}
      }; // end of struct StensorDeterminant

      template<>
      struct StensorDeterminant<2u>
      {
	template<typename T>
	static TFEL_MATH_INLINE typename ComputeUnaryResult<T,Power<3> >::Result
	exe(const tfel::math::stensor<2u,T>& s)
	{
	  typedef typename tfel::typetraits::BaseType<T>::type base;
	  return s(2)*(s(0)*s(1)-s(3)*s(3)/base(2));
	}
      }; // end of struct StensorDeterminant

      template<>
      struct StensorDeterminant<3u>
      {
	template<typename T>
	static TFEL_MATH_INLINE typename ComputeUnaryResult<T,Power<3> >::Result
	exe(const tfel::math::stensor<3u,T>& s)
	{
	  typedef typename tfel::typetraits::BaseType<T>::type base;
	  static constexpr base cste = std::sqrt(base(2));
	  return (T(2)*s(0)*s(1)*s(2)+cste*s(3)*s(4)*s(5)-s(2)*s(3)*s(3)-s(1)*s(4)*s(4)-s(0)*s(5)*s(5))/base(2);
	}
      }; // end of struct StensorDeterminant

      template<unsigned short N>
      struct StensorInvert;
      
      template<>
      struct StensorInvert<1u>
      {
	template<typename T>
	static TFEL_MATH_INLINE
	stensor<1u,typename ComputeBinaryResult<typename tfel::typetraits::BaseType<T>::type,T,OpDiv>::Result>
	exe(const stensor<1u,T>& s)
	{
	  typedef typename tfel::typetraits::BaseType<T>::type base;
	  typedef typename ComputeBinaryResult<base,T,OpDiv>::Result T2;
	  stensor<1u,T2> inv;
	  inv(0)=base(1)/s(0);
	  inv(1)=base(1)/s(1);
	  inv(2)=base(1)/s(2);
	  return inv;
	}
      };

      template<>
      struct StensorInvert<2u>
      {
	template<typename T>
	static TFEL_MATH_INLINE
	stensor<2u,typename ComputeBinaryResult<typename tfel::typetraits::BaseType<T>::type,T,OpDiv>::Result>
	exe(const stensor<2u,T>& s)
	{
	  typedef typename tfel::typetraits::BaseType<T>::type base;
	  typedef typename ComputeBinaryResult<base,T,OpDiv>::Result T2;
	  typedef typename ComputeUnaryResult<T,Power<3> >::Result T3;
	  stensor<2u,T2> inv;
	  const T3 inv_det = base(1)/det(s);
	  inv(0)=s(1)*s(2)*inv_det;
	  inv(1)=s(0)*s(2)*inv_det;
	  inv(2)=base(1)/s(2);
	  inv(3)=-s(2)*s(3)*inv_det;
	  return inv;
	}
      };

      template<>
      struct StensorInvert<3u>
      {
	template<typename T>
	static TFEL_MATH_INLINE
	stensor<3u,typename ComputeBinaryResult<typename tfel::typetraits::BaseType<T>::type,T,OpDiv>::Result>
	exe(const stensor<3u,T>& s)
	{
	  typedef typename tfel::typetraits::BaseType<T>::type base;
	  typedef typename ComputeBinaryResult<base,T,OpDiv>::Result T2;
	  typedef typename ComputeUnaryResult<T,Power<3> >::Result T3;
	  static constexpr base two          = base(2);
	  static constexpr base inv_sqrt_two = base(1)/std::sqrt(two);
	  static constexpr base one_half     = base(1)/two;
	  stensor<3u,T2> inv;
	  const T3 inv_det = base(1)/det(s);
	  inv(0)=(s(1)*s(2)-s(5)*s(5)*one_half)*inv_det;
	  inv(1)=(s(0)*s(2)-s(4)*s(4)*one_half)*inv_det;
	  inv(2)=(s(0)*s(1)-s(3)*s(3)*one_half)*inv_det;
	  inv(3)=(inv_sqrt_two*s(4)*s(5)-s(2)*s(3))*inv_det;
	  inv(4)=(inv_sqrt_two*s(3)*s(5)-s(1)*s(4))*inv_det;
	  inv(5)=(inv_sqrt_two*s(3)*s(4)-s(0)*s(5))*inv_det;
	  return inv;
	}
      };

      template<unsigned short N>
      struct BuildStensorFromMatrix;
      
      template<>
      struct BuildStensorFromMatrix<1u>
      {
	template<typename T,typename MatrixType>
	static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL
	typename std::enable_if<
	  tfel::typetraits::IsAssignableTo<typename MatrixTraits<MatrixType>::NumType,T>::cond,
	  void>::type
	exe(stensor<1u,T,StensorStatic>& s,const MatrixType& m){
	  using namespace std;
	  s[0] = m(0,0);
	  s[1] = m(1,1);
	  s[2] = m(2,2);
	}
      };
    
      template<>
      struct BuildStensorFromMatrix<2u>
      {
	template<typename T,typename MatrixType>
	static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL
	typename std::enable_if<
	  tfel::typetraits::IsAssignableTo<typename MatrixTraits<MatrixType>::NumType,T>::cond,
	  void>::type
	exe(stensor<2u,T,StensorStatic>& s,const MatrixType& m){
	  using namespace std;
	  using namespace tfel::typetraits;
	  typedef typename BaseType<T>::type real;
	  static constexpr real cste = sqrt(real(2))/(real(2));
	  s[0] = m(0,0);
	  s[1] = m(1,1);
	  s[2] = m(2,2);
	  s[3] = (m(0,1)+m(0,1))*cste;
	}
      };
      
      template<>
      struct BuildStensorFromMatrix<3u>
      {
	template<typename T,typename MatrixType>
	static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL
	typename std::enable_if<
	  tfel::typetraits::IsAssignableTo<typename MatrixTraits<MatrixType>::NumType,T>::cond,
	  void>::type
	exe(stensor<3u,T,StensorStatic>& s,const MatrixType& m){
	  using namespace std;
	  using namespace tfel::typetraits;
	  typedef typename BaseType<T>::type real;
	  static constexpr real cste = sqrt(real(2))/(real(2));
	  s[0] = m(0,0);
	  s[1] = m(1,1);
	  s[2] = m(2,2);
	  s[3] = (m(0,1)+m(1,0))*cste;
	  s[4] = (m(0,2)+m(2,0))*cste;
	  s[5] = (m(2,1)+m(1,2))*cste;
	}
      };

      template<unsigned short N>
      struct BuildStensorFromVectorDiadicProduct;
      
      template<>
      struct BuildStensorFromVectorDiadicProduct<1u>
      {
	template<typename T,typename VectorType>
	static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL
	typename std::enable_if<
	  tfel::typetraits::IsAssignableTo<
	    typename ComputeUnaryResult<typename VectorTraits<VectorType>::NumType,Power<2> >::Result,T
	    >::cond,
	  void>::type
	exe(stensor<1u,T,StensorStatic>& s,const VectorType& v){
	  using namespace std;
	  s[0] = v(0)*v(0);
	  s[1] = v(1)*v(1);
	  s[2] = v(2)*v(2);
	}
      };
    
      template<>
      struct BuildStensorFromVectorDiadicProduct<2u>
      {
	template<typename T,typename VectorType>
	static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL
	typename std::enable_if<
	  tfel::typetraits::IsAssignableTo<
	    typename ComputeUnaryResult<typename VectorTraits<VectorType>::NumType,Power<2> >::Result,T
	    >::cond,
	  void>::type
	exe(stensor<2u,T,StensorStatic>& s,const VectorType& v){
	  using namespace std;
	  using namespace tfel::typetraits;
	  typedef typename BaseType<T>::type real;
	  static constexpr real cste = sqrt(real(2));
	  s[0] = v(0)*v(0);
	  s[1] = v(1)*v(1);
	  s[2] = v(2)*v(2);
	  s[3] = v(0)*v(1)*cste;
	}
      };
      
      template<>
      struct BuildStensorFromVectorDiadicProduct<3u>
      {
	template<typename T,typename VectorType>
	static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL
	typename std::enable_if<
	  tfel::typetraits::IsAssignableTo<
	    typename ComputeUnaryResult<typename VectorTraits<VectorType>::NumType,Power<2> >::Result,T
	    >::cond,
	  void>::type
	exe(stensor<3u,T,StensorStatic>& s,const VectorType& v){
	  using namespace std;
	  using namespace tfel::typetraits;
	  typedef typename BaseType<T>::type real;
	  static constexpr real cste = sqrt(real(2));
	  s[0] = v(0)*v(0);
	  s[1] = v(1)*v(1);
	  s[2] = v(2)*v(2);
	  s[3] = v(0)*v(1)*cste;
	  s[4] = v(0)*v(2)*cste;
	  s[5] = v(1)*v(2)*cste;
	}
      };

      template<unsigned short N>
      struct BuildStensorFromVectorsSymmetricDiadicProduct;
      
      template<>
      struct BuildStensorFromVectorsSymmetricDiadicProduct<1u>
      {
	template<typename T,
		 typename VectorType,
		 typename VectorType2>
	static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL
	typename std::enable_if<
	  tfel::typetraits::IsAssignableTo<
	    typename ComputeBinaryResult<typename VectorTraits<VectorType>::NumType,
					 typename VectorTraits<VectorType2>::NumType,OpMult>::Result,T
	    >::cond,
	  void>::type
	exe(stensor<1u,T,StensorStatic>& s,
	    const VectorType& v1,
	    const VectorType2& v2){
	  using namespace std;
	  s[0] = 2*v1(0)*v2(0);
	  s[1] = 2*v1(1)*v2(1);
	  s[2] = 2*v1(2)*v2(2);
	}
      };
    
      template<>
      struct BuildStensorFromVectorsSymmetricDiadicProduct<2u>
      {
	template<typename T,
		 typename VectorType,
		 typename VectorType2>
	static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL
	typename std::enable_if<
	  tfel::typetraits::IsAssignableTo<
	    typename ComputeBinaryResult<typename VectorTraits<VectorType>::NumType,
					 typename VectorTraits<VectorType2>::NumType,OpMult>::Result,T
	    >::cond,
	  void>::type
	exe(stensor<2u,T,StensorStatic>& s,
	    const VectorType& v1,
	    const VectorType2& v2){
	  using namespace std;
	  using namespace tfel::typetraits;
	  typedef typename BaseType<T>::type real;
	  static constexpr real cste = sqrt(real(2));
	  s[0] = 2*v1(0)*v2(0);
	  s[1] = 2*v1(1)*v2(1);
	  s[2] = 2*v1(2)*v2(2);
	  s[3] = cste*(v1(0)*v2(1)+v2(0)*v1(1));
	}
      };
      
      template<>
      struct BuildStensorFromVectorsSymmetricDiadicProduct<3u>
      {
	template<typename T,
		 typename VectorType,
		 typename VectorType2>
	static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL
	typename std::enable_if<
	  tfel::typetraits::IsAssignableTo<
	    typename ComputeBinaryResult<typename VectorTraits<VectorType>::NumType,
					 typename VectorTraits<VectorType2>::NumType,OpMult>::Result,T
	    >::cond,
	  void>::type
	exe(stensor<3u,T,StensorStatic>& s,
	    const VectorType& v1,
	    const VectorType2& v2){
	  using namespace std;
	  using namespace tfel::typetraits;
	  typedef typename BaseType<T>::type real;
	  static constexpr real cste = sqrt(real(2));
	  s[0] = 2*v1(0)*v2(0);
	  s[1] = 2*v1(1)*v2(1);
	  s[2] = 2*v1(2)*v2(2);
	  s[3] = (v1(0)*v2(1)+v2(0)*v1(1))*cste;
	  s[4] = (v1(0)*v2(2)+v2(0)*v1(2))*cste;
	  s[5] = (v1(1)*v2(2)+v2(1)*v1(2))*cste;
	}
      };

      template<unsigned short N>
      struct BuildStensorFromEigenValuesAndVectors;
      
      template<>
      struct BuildStensorFromEigenValuesAndVectors<1u>
      {
	template<typename T>
	static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL void
	exe(stensor<1u,T,StensorStatic>& s,
	    const T& v1,
	    const T& v2,
	    const T& v3,
	    const tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>&){
	  using namespace std;
	  s[0] = v1;
	  s[1] = v2;
	  s[2] = v3;
	}
      };
    
      template<>
      struct BuildStensorFromEigenValuesAndVectors<2u>
      {
	template<typename T>
	static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL void
	exe(stensor<2u,T,StensorStatic>& s,
	    const T& v1,
	    const T& v2,
	    const T& v3,
	    const tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>& m){
	  using namespace std;
	  using namespace tfel::typetraits;
	  typedef typename BaseType<T>::type real;
	  static constexpr real cste = sqrt(real(2));
	  s[0] = v1*m(0,0)*m(0,0)+v2*m(0,1)*m(0,1);
	  s[1] = v1*m(1,0)*m(1,0)+v2*m(1,1)*m(1,1);
	  s[2] = v3;
	  s[3] = (v1*m(0,0)*m(1,0)+v2*m(0,1)*m(1,1))*cste;
	}
      };
      
      template<>
      struct BuildStensorFromEigenValuesAndVectors<3u>
      {
	template<typename T>
	static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL void
	exe(stensor<3u,T,StensorStatic>& s,
	    const T& v1,
	    const T& v2,
	    const T& v3,
	    const tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>& m){
	  using namespace std;
	  using namespace tfel::typetraits;
	  typedef typename BaseType<T>::type real;
	  static constexpr real cste = sqrt(real(2));
	  s[0] =  v1*m(0,0)*m(0,0)+v2*m(0,1)*m(0,1)+v3*m(0,2)*m(0,2);
	  s[1] =  v1*m(1,0)*m(1,0)+v2*m(1,1)*m(1,1)+v3*m(1,2)*m(1,2);
	  s[2] =  v1*m(2,0)*m(2,0)+v2*m(2,1)*m(2,1)+v3*m(2,2)*m(2,2);
	  s[3] = (v1*m(0,0)*m(1,0)+v2*m(0,1)*m(1,1)+v3*m(0,2)*m(1,2))*cste;
	  s[4] = (v1*m(0,0)*m(2,0)+v2*m(0,1)*m(2,1)+v3*m(0,2)*m(2,2))*cste;
	  s[5] = (v1*m(1,0)*m(2,0)+v2*m(1,1)*m(2,1)+v3*m(1,2)*m(2,2))*cste;
	}
      };

      template<unsigned short N>
      struct StensorComputeEigenValuesDerivatives;
      
      template<>
      struct StensorComputeEigenValuesDerivatives<1u>
      {
	template<typename StensorType,
		 typename BaseType>
	static TFEL_MATH_INLINE2
	typename std::enable_if<
	  (tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
	  (StensorTraits<StensorType>::dime==1u)&&
	  (tfel::typetraits::IsAssignableTo<BaseType,
					    typename StensorTraits<StensorType>::NumType>::cond),
	  void>::type
	exe(StensorType& n0,
	    StensorType& n1,
	    StensorType& n2,
	    const tfel::math::tmatrix<3u,3u,BaseType>&)
	{
	  n0(0) = BaseType(1);
	  n0(1) = BaseType(0);
	  n0(2) = BaseType(0);
	  n1(0) = BaseType(0);
	  n1(1) = BaseType(1);
	  n1(2) = BaseType(0);
	  n2(0) = BaseType(0);
	  n2(1) = BaseType(0);
	  n2(2) = BaseType(1);
	}
      };

      template<>
      struct StensorComputeEigenValuesDerivatives<2u>
      {
	template<typename StensorType,
		 typename BaseType>
	static TFEL_MATH_INLINE2
	typename std::enable_if<
	  (tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
	  (StensorTraits<StensorType>::dime==2u)&&
	  (tfel::typetraits::IsAssignableTo<BaseType,
					    typename StensorTraits<StensorType>::NumType>::cond),
	  void>::type
	exe(StensorType& n0,
	    StensorType& n1,
	    StensorType& n2,
	    const tfel::math::tmatrix<3u,3u,BaseType>& m)
	{
	  using namespace tfel::math;
	  const tvector<3u,BaseType> v0 = m.template column_view<0u>();
	  const tvector<3u,BaseType> v1 = m.template column_view<1u>();
	  n0    = StensorType::buildFromVectorDiadicProduct(v0);
	  n1    = StensorType::buildFromVectorDiadicProduct(v1);
	  n2(0) = BaseType(0);
	  n2(1) = BaseType(0);
	  n2(2) = BaseType(1);
	  n2(3) = BaseType(0);
	}
      };

      template<>
      struct StensorComputeEigenValuesDerivatives<3u>
      {
	template<typename StensorType,
		 typename BaseType>
	static TFEL_MATH_INLINE2
	typename std::enable_if<
	  (tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
	  (StensorTraits<StensorType>::dime==3u)&&
	  (tfel::typetraits::IsAssignableTo<BaseType,
					    typename StensorTraits<StensorType>::NumType>::cond),
	  void>::type
	exe(StensorType& n0,
	    StensorType& n1,
	    StensorType& n2,
	    const tfel::math::tmatrix<3u,3u,BaseType>& m)
	{
	  using namespace tfel::math;
	  const tvector<3u,BaseType> v0 = m.template column_view<0u>();
	  const tvector<3u,BaseType> v1 = m.template column_view<1u>();
	  const tvector<3u,BaseType> v2 = m.template column_view<2u>();
	  n0 = StensorType::buildFromVectorDiadicProduct(v0);
	  n1 = StensorType::buildFromVectorDiadicProduct(v1);
	  n2 = StensorType::buildFromVectorDiadicProduct(v2);
	}
      };

      template<unsigned short N>
      struct StensorComputeEigenTensorsDerivatives;

      struct StensorComputeEigenTensorsDerivativesBase
      {
	template<typename NumType>
	TFEL_MATH_INLINE static
	typename ComputeBinaryResult<typename tfel::typetraits::BaseType<NumType>::type,
				     NumType,OpDiv>::Result
	regularized_inverse(const NumType x,
			    const NumType eps)
	{
	  using namespace std;
	  if(abs(x)<100*numeric_limits<NumType>::min()){
	    return NumType(0);
	  }
	  return regularization_function(x/eps)/x;
	} // end of regularization
      protected:
	template<typename NumType>
	TFEL_MATH_INLINE static
	NumType
	regularization_function(const NumType x)
	{
	  using namespace std;
	  if(abs(x)>1){
	    return NumType(1);
	  }
	  return x*x*(4-x*x)/3;
	}
      };
      
      template<>
      struct StensorComputeEigenTensorsDerivatives<1u>
      {
	template<typename ST2toST2Type,
		 typename NumType>
	static TFEL_MATH_INLINE2
	typename std::enable_if<
	  (tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond)&&
	  (ST2toST2Traits<ST2toST2Type>::dime==1u)&&
	  (tfel::typetraits::IsAssignableTo<typename ComputeBinaryResult<typename tfel::typetraits::BaseType<NumType>::type,
									 NumType,OpDiv>::Result,
					    typename ST2toST2Traits<ST2toST2Type>::NumType>::cond),
	  void>::type
	exe(ST2toST2Type& dn0_ds,
	    ST2toST2Type& dn1_ds,
	    ST2toST2Type& dn2_ds,
	    const tfel::math::tvector<3u,NumType>&,
	    const tfel::math::tmatrix<3u,3u,typename tfel::typetraits::BaseType<NumType>::type>&,
	    const NumType)
	{
	  using namespace tfel::math;
	  using namespace tfel::typetraits;
	  typedef typename ComputeBinaryResult<typename BaseType<NumType>::type,
					       NumType,OpDiv>::Result InvNumType;
	  dn0_ds = st2tost2<1u,InvNumType>(InvNumType(0));
	  dn1_ds = dn0_ds;
	  dn2_ds = dn0_ds;
	} // end of exe
      };

      template<>
      struct StensorComputeEigenTensorsDerivatives<2u>
	: public StensorComputeEigenTensorsDerivativesBase
      {
	template<typename ST2toST2Type,
		 typename NumType>
	static TFEL_MATH_INLINE2
	typename std::enable_if<
	  (tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond)&&
	  (ST2toST2Traits<ST2toST2Type>::dime==2u)&&
	  (tfel::typetraits::IsAssignableTo<typename ComputeBinaryResult<typename tfel::typetraits::BaseType<NumType>::type,
									 NumType,OpDiv>::Result,
					    typename ST2toST2Traits<ST2toST2Type>::NumType>::cond),
	  void>::type
	exe(ST2toST2Type& dn0_ds,
	    ST2toST2Type& dn1_ds,
	    ST2toST2Type& dn2_ds,
	    const tfel::math::tvector<3u,NumType>& vp,
	    const tfel::math::tmatrix<3u,3u,typename tfel::typetraits::BaseType<NumType>::type>& m,
	    const NumType eps)
	{
	  using namespace tfel::math;
	  using namespace tfel::typetraits;
	  typedef typename BaseType<NumType>::type base;
	  typedef typename ComputeBinaryResult<base,NumType,OpDiv>::Result InvNumType;
	  const base cste = base(1)/sqrt(base(2));
	  const tvector<3u,base> v0 = m.template column_view<0u>();
	  const tvector<3u,base> v1 = m.template column_view<1u>();
	  const stensor<2u,base> n01 = stensor<2u,base>::buildFromVectorsSymmetricDiadicProduct(v0,v1)*cste;
	  dn0_ds = regularized_inverse(vp(0)-vp(1),eps)*(n01^n01);
	  dn1_ds = -dn0_ds;
	  dn2_ds = st2tost2<2u,InvNumType>(InvNumType(0));
	} // end of exe
      };

      template<>
      struct StensorComputeEigenTensorsDerivatives<3u>
	: public StensorComputeEigenTensorsDerivativesBase
      {
	template<typename ST2toST2Type,
		 typename NumType>
	static TFEL_MATH_INLINE2
	typename std::enable_if<
	  (tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond)&&
	  (ST2toST2Traits<ST2toST2Type>::dime==3u)&&
	  (tfel::typetraits::IsAssignableTo<typename ComputeBinaryResult<typename tfel::typetraits::BaseType<NumType>::type,
									 NumType,OpDiv>::Result,
					    typename ST2toST2Traits<ST2toST2Type>::NumType>::cond),
	  void>::type
	exe(ST2toST2Type& dn0_ds,
	    ST2toST2Type& dn1_ds,
	    ST2toST2Type& dn2_ds,
	    const tfel::math::tvector<3u,NumType>& vp,
	    const tfel::math::tmatrix<3u,3u,typename tfel::typetraits::BaseType<NumType>::type>& m,
	    const NumType eps)
	{
	  using namespace tfel::math;
	  using namespace tfel::typetraits;
	  typedef typename BaseType<NumType>::type base;
	  const base cste = base(1)/sqrt(base(2));
	  const tvector<3u,base> v0 = m.template column_view<0u>();
	  const tvector<3u,base> v1 = m.template column_view<1u>();
	  const tvector<3u,base> v2 = m.template column_view<2u>();
	  const stensor<3u,base> n01 = stensor<3u,base>::buildFromVectorsSymmetricDiadicProduct(v0,v1)*cste;
	  const stensor<3u,base> n02 = stensor<3u,base>::buildFromVectorsSymmetricDiadicProduct(v0,v2)*cste;
	  const stensor<3u,base> n12 = stensor<3u,base>::buildFromVectorsSymmetricDiadicProduct(v1,v2)*cste;
	  dn0_ds = regularized_inverse(vp(0)-vp(1),eps)*(n01^n01)+regularized_inverse(vp(0)-vp(2),eps)*(n02^n02);
	  dn1_ds = regularized_inverse(vp(1)-vp(0),eps)*(n01^n01)+regularized_inverse(vp(1)-vp(2),eps)*(n12^n12);
	  dn2_ds = regularized_inverse(vp(2)-vp(0),eps)*(n02^n02)+regularized_inverse(vp(2)-vp(1),eps)*(n12^n12);
	} // end of exe
      };
      
    } // end of namespace internals

    template<typename Child>
    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,StensorConcept>::cond &&
      StensorTraits<Child>::dime==StensorTraits<StensorType>::dime &&
      tfel::typetraits::IsAssignableTo<typename StensorTraits<StensorType>::NumType,
				       typename StensorTraits<Child>::NumType>::cond,
      Child&>::type
    stensor_base<Child>::operator=(const StensorType& src){
      Child& child = static_cast<Child&>(*this);
      vectorToTab<StensorDimeToSize<StensorTraits<Child>::dime>::value>::exe(src,child);
      return child;
    }

    template<typename Child>
    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,StensorConcept>::cond &&
      StensorTraits<Child>::dime==StensorTraits<StensorType>::dime &&
      tfel::typetraits::IsAssignableTo<typename StensorTraits<StensorType>::NumType,
				       typename StensorTraits<Child>::NumType>::cond,
      Child&>::type
    stensor_base<Child>::operator+=(const StensorType& src){
      Child& child = static_cast<Child&>(*this);
      VectorUtilities<StensorDimeToSize<StensorTraits<Child>::dime>::value>::PlusEqual(child,src);
      return child;
    }

    template<typename Child>
    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,StensorConcept>::cond &&
      StensorTraits<Child>::dime==StensorTraits<StensorType>::dime &&
      tfel::typetraits::IsAssignableTo<typename StensorTraits<StensorType>::NumType,
				       typename StensorTraits<Child>::NumType>::cond,
      Child&>::type
    stensor_base<Child>::operator-=(const StensorType& src){
      Child& child = static_cast<Child&>(*this);
      VectorUtilities<StensorDimeToSize<StensorTraits<Child>::dime>::value>::MinusEqual(child,src);
      return child;
    }

    // *= operator
    template<typename Child>
    template<typename T2>
    typename std::enable_if<
      tfel::typetraits::IsScalar<T2>::cond&&
      std::is_same<typename ResultType<typename StensorTraits<Child>::NumType,
				       T2,OpMult>::type,
		   typename StensorTraits<Child>::NumType>::value,
    Child&>::type
    stensor_base<Child>::operator*=(const T2 s)
    {
      Child& child = static_cast<Child&>(*this);
      VectorUtilities<StensorDimeToSize<StensorTraits<Child>::dime>::value>::scale(child,s);
      return child;
    }

    // /= operator
    template<typename Child>
    template<typename T2>
    typename std::enable_if<
      tfel::typetraits::IsScalar<T2>::cond&&
      std::is_same<typename ResultType<typename StensorTraits<Child>::NumType,
						 T2,OpDiv>::type,
			     typename StensorTraits<Child>::NumType>::value,
    Child&>::type
    stensor_base<Child>::operator/=(const T2 s)
    {
      Child& child = static_cast<Child&>(*this);
      VectorUtilities<StensorDimeToSize<StensorTraits<Child>::dime>::value>::scale(child,(static_cast<typename tfel::typetraits::BaseType<T2>::type>(1u))/s);
      return child;
    }

    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    stensor<N,T,Storage>::stensor(const T init)
    {
      tfel::fsalgo::fill<StensorDimeToSize<N>::value>::exe(this->v,init);
    }

    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE
    T& 
    stensor<N,T,Storage>::operator()(const unsigned short i){
      assert(i<StensorDimeToSize<N>::value);
      return this->v[i];
    }

    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE
    const T& 
    stensor<N,T,Storage>::operator()(const unsigned short i) const{
      assert(i<StensorDimeToSize<N>::value);
      return this->v[i];
    }

    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE
    T& 
    stensor<N,T,Storage>::operator[](const unsigned short i){
      assert(i<StensorDimeToSize<N>::value);
      return this->v[i];
    }

    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    typename stensor<N,T,Storage>::RunTimeProperties
    stensor<N,T,Storage>::getRunTimeProperties(void) const{
      return RunTimeProperties();
    }

    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE
    const T& 
    stensor<N,T,Storage>::operator[](const unsigned short i) const{
      assert(i<StensorDimeToSize<N>::value);
      return this->v[i];
    }

    // Import from Voigt
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    template<typename T2>
    typename std::enable_if<
      tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
      void>::type
    stensor<N,T,Storage>::importVoigt(const T2 * const src)
    {
      tfel::math::internals::ImportFromVoigt<N>::exe(reinterpret_cast<T2*>(this->v),src);
    }
    
    // Import from Tab
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    template<typename T2>
    typename std::enable_if<
      tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
      void>::type
    stensor<N,T,Storage>::importTab(const T2* const src)
    {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::math::internals::ImportFromTab<N> Import;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
      Import::exe(reinterpret_cast<T2*>(this->v),src);
    }

    // Import from values
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    template<typename T2>
    typename std::enable_if<
      tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
      void>::type
    stensor<N,T,Storage>::import(const T2 * const src)
    {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::fsalgo::copy<StensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
      Copy::exe(src,reinterpret_cast<base*>(this->v));
    }

    // Export to Tab
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    template<typename T2>
    typename std::enable_if<
      tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
      void>::type
    stensor<N,T,Storage>::exportTab(T2* const src) const
    {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::math::internals::ExportToTab<N> Export;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
      Export::exe(reinterpret_cast<const T2*>(this->v),src);
    }

    // Write to Tab
    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    template<typename T2>
    typename std::enable_if<
      tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
      void
      >::type
    stensor<N,T,Storage>::write(T2* const t) const
    {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::fsalgo::copy<StensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
      Copy::exe(reinterpret_cast<const base*>(this->v),t);
    }

    // computeEigenValues
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    void stensor<N,T,Storage>::computeEigenValues(T& vp1,T& vp2,T& vp3,
						  const bool b) const 
    {
      TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,typename tfel::typetraits::BaseType<T>::type>::cond));
      tfel::math::internals::StensorComputeEigenValues_<N>::exe(reinterpret_cast<const typename tfel::typetraits::BaseType<T>::type*>(this->v),
								reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp1),
								reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp2),
								reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp3),b);
    }

    // computeEigenValues
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    void stensor<N,T,Storage>::computeEigenValues(tvector<3u,T>& vp,
						  const bool b) const 
    {
      TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,typename tfel::typetraits::BaseType<T>::type>::cond));
      tfel::math::internals::StensorComputeEigenValues_<N>::exe(reinterpret_cast<const typename tfel::typetraits::BaseType<T>::type* const>(this->v),
								reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp(0)),
								reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp(1)),
								reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp(2)),b);
    }

    // computeEigenVectors
    template<unsigned short N,typename T, 
	     template<unsigned short,typename> class Storage>
    bool stensor<N,T,Storage>::computeEigenVectors(tvector<3u,T>& vp,
						   tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>& vec,
						   const bool b) const 
    {
      TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,typename tfel::typetraits::BaseType<T>::type>::cond));
      return tfel::math::internals::StensorComputeEigenVectors_<N>::exe(reinterpret_cast<const typename tfel::typetraits::BaseType<T>::type*>(this->v),
									reinterpret_cast<tvector<3u,typename tfel::typetraits::BaseType<T>::type>&>(vp),vec,b);
    }

    // computeEigenVectors
    template<unsigned short N,typename T, 
	     template<unsigned short,typename> class Storage>
    template<typename VectorType>
    bool
    stensor<N,T,Storage>::computeEigenVector(VectorType& ev,const T vp) const 
    {
      typedef typename VectorTraits<VectorType>::NumType real;
      typedef tfel::math::internals::StensorComputeEigenVectors_<N> SCEV;
      TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<real>::cond);
      TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<real>::cond);
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,real>::cond));
      return SCEV::computeEigenVector(ev,reinterpret_cast<const real*>(this->v),
				      *(reinterpret_cast<const real*>(&vp)));
    }

    template<unsigned short N,typename T, 
	     template<unsigned short,typename> class Storage>
    template<typename StensorType>
    typename std::enable_if<
      (tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
      (StensorTraits<StensorType>::dime==N)&&
      (tfel::typetraits::IsAssignableTo<typename tfel::typetraits::BaseType<T>::type,
					typename StensorTraits<StensorType>::NumType>::cond),
      void>::type
    stensor<N,T,Storage>::computeEigenValuesDerivatives(StensorType& n0,
							StensorType& n1,
							StensorType& n2,
							const tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>& m)
    {
      return tfel::math::internals::StensorComputeEigenValuesDerivatives<N>::exe(n0,n1,n2,m);      
    } // end of stensor<N,T,Storage>::computeEigenValuesDerivatives

    template<unsigned short N,typename T, 
	     template<unsigned short,typename> class Storage>
    template<typename StensorType>
    typename std::enable_if<
      (tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
      (StensorTraits<StensorType>::dime==N)&&
      (tfel::typetraits::IsAssignableTo<typename tfel::typetraits::BaseType<T>::type,
					typename StensorTraits<StensorType>::NumType>::cond),
      void>::type
    stensor<N,T,Storage>::computeEigenTensors(StensorType& n0,
					      StensorType& n1,
					      StensorType& n2,
					      const tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>& m)
    {
      return tfel::math::internals::StensorComputeEigenValuesDerivatives<N>::exe(n0,n1,n2,m);      
    } // end of stensor<N,T,Storage>::computeEigenTensors

    template<unsigned short N,typename T, 
	     template<unsigned short,typename> class Storage>
    template<typename ST2toST2Type>
    typename std::enable_if<
      (tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond)&&
      (ST2toST2Traits<ST2toST2Type>::dime==N)&&
      (tfel::typetraits::IsAssignableTo<typename ComputeBinaryResult<typename tfel::typetraits::BaseType<T>::type,
								     T,OpDiv>::Result,
					typename ST2toST2Traits<ST2toST2Type>::NumType>::cond),
      void>::type
    stensor<N,T,Storage>::computeEigenTensorsDerivatives(ST2toST2Type& dn0_ds,
							 ST2toST2Type& dn1_ds,
							 ST2toST2Type& dn2_ds,
							 const tvector<3u,T>& vp,
							 const tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>& m,
							 const T eps)
    {
      return tfel::math::internals::StensorComputeEigenTensorsDerivatives<N>::exe(dn0_ds,dn1_ds,dn2_ds,
										  vp,m,eps); 
    }

    // ChangeBasis
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    void 
    stensor<N,T,Storage>::changeBasis(const tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>& m)
    {
      return tfel::math::internals::StensorChangeBasis<N>::exe(this->v,m);
    }

    // Return Id
    template<unsigned short N,typename T,
	     template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE
    const stensor<N,T,Storage>&
    stensor<N,T,Storage>::Id(void)
    {
      using base = typename tfel::typetraits::BaseType<T>::type;
      constexpr base zero{0};
      constexpr base one{1};
      constexpr base IdCoef[]  = {one,one,one,zero,zero,zero};
      static const stensor<N,T,Storage> id(IdCoef);
      return id;
    } // end of stensor<N,T,Storage>::Id

    template<unsigned short N, typename T,
	     template<unsigned short,typename> class Storage>
    template<typename InputIterator>
    void 
    stensor<N,T,Storage>::copy(const InputIterator src)
    {
      tfel::fsalgo::copy<StensorDimeToSize<N>::value>::exe(src,*this);
    }

    template<unsigned short N, typename T,
	     template<unsigned short,typename> class Storage,
	     typename OutputIterator>
    TFEL_MATH_INLINE2
    typename std::enable_if<
      tfel::typetraits::IsScalar<T>::cond,
      void>::type
    exportToBaseTypeArray(const stensor<N,T,Storage>& s,
			  OutputIterator p)
    {    
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::fsalgo::copy<StensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
      Copy::exe(reinterpret_cast<const base*>(&s[0]),p);
    }

    template<typename T,
	     template<unsigned short,typename> class Storage>
    T tresca(const stensor<1u,T,Storage>& s,
	     const bool)
    {
      using namespace std;
      const T sd1 = abs(s[0]-s[1]);
      const T sd2 = abs(s[0]-s[2]);
      const T sd3 = abs(s[2]-s[1]);
      const T tmp  = sd1 > sd2 ? sd1 : sd2;
      const T tmp2 = sd3 > tmp ? sd3 : tmp;
      return tmp2;
    } // end of tresca
    
    template<unsigned short N,
	     typename T,
	     template<unsigned short,typename> class Storage>
    T tresca(const stensor<N,T,Storage>& s,
	     const bool b)
    {
      using namespace std;
      T s1;
      T s2;
      T s3;
      s.computeEigenValues(s1,s2,s3,b);
      const T sd1 = abs(s1-s2);
      const T sd2 = abs(s1-s3);
      const T sd3 = abs(s3-s2);
      const T tmp  = sd1 > sd2  ? sd1 : sd2;
      const T tmp2 = sd3 > tmp  ? sd3 : tmp;
      return tmp2;
    } // end of tresca

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,StensorConcept>::cond,
      stensor<StensorTraits<StensorType>::dime,
	      typename StensorTraits<StensorType>::NumType>
    >::type
    square_root(const StensorType& s)
    {
      using tfel::fsalgo::copy;
      typedef typename StensorTraits<StensorType>::NumType T;
      typedef typename tfel::typetraits::BaseType<T>::type real;
      stensor<StensorTraits<StensorType>::dime,T> tmp(s);
      stensor<StensorTraits<StensorType>::dime,T> r(T(0));
      tmatrix<3u,3u,real> vec;
      tvector<3u,T> vp;
      tmp.computeEigenVectors(vp,vec);
      r[0] = std::sqrt(vp[0]);
      r[1] = std::sqrt(vp[1]);
      r[2] = std::sqrt(vp[2]);
      r.changeBasis(transpose(vec));
      return r;
    } // end of square_root

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,StensorConcept>::cond,
      typename ComputeUnaryResult<typename StensorTraits<StensorType>::NumType,Power<3> >::Result
      >::type
    det(const StensorType& s)
    {
      const stensor<StensorTraits<StensorType>::dime,
		    typename StensorTraits<StensorType>::NumType> tmp(s);
      return tfel::math::internals::StensorDeterminant<StensorTraits<StensorType>::dime>::exe(tmp);
    }

    template<typename StensorType>
    typename std::enable_if<
      tfel::meta::Implements<StensorType,StensorConcept>::cond,
      stensor<StensorTraits<StensorType>::dime,
	      typename ComputeBinaryResult<typename tfel::typetraits::BaseType<typename StensorTraits<StensorType>::NumType>::type,
					   typename StensorTraits<StensorType>::NumType,OpDiv>::Result>
      >::type
    invert(const StensorType& s)
    {
      const stensor<StensorTraits<StensorType>::dime,
    		    typename StensorTraits<StensorType>::NumType> tmp(s);
      return tfel::math::internals::StensorInvert<StensorTraits<StensorType>::dime>::exe(tmp);
    }

    template<unsigned short N,typename T,template<unsigned short,typename> class Storage>
    template<typename MatrixType>
    typename std::enable_if<
      tfel::typetraits::IsAssignableTo<typename MatrixTraits<MatrixType>::NumType,T>::cond,
      stensor<N,T,StensorStatic> >::type
    stensor<N,T,Storage>::buildFromMatrix(const MatrixType& m){
      stensor<N,T,Storage> s;
      tfel::math::internals::BuildStensorFromMatrix<N>::exe(s,m);
      return s;
    }

    template<unsigned short N,typename T,template<unsigned short,typename> class Storage>
    template<typename VectorType>
    typename std::enable_if<
      tfel::typetraits::IsAssignableTo<
	typename ComputeUnaryResult<typename VectorTraits<VectorType>::NumType,Power<2> >::Result,T
      >::cond,
      stensor<N,T,StensorStatic> >::type
    stensor<N,T,Storage>::buildFromVectorDiadicProduct(const VectorType& v){
      stensor<N,T,Storage> s;
      tfel::math::internals::BuildStensorFromVectorDiadicProduct<N>::exe(s,v);
      return s;
    }

    template<unsigned short N,typename T,template<unsigned short,typename> class Storage>
    template<typename VectorType,
	     typename VectorType2>
    typename std::enable_if<
      tfel::typetraits::IsAssignableTo<
	typename ComputeBinaryResult<typename VectorTraits<VectorType>::NumType,
				     typename VectorTraits<VectorType2>::NumType,
				     OpMult>::Result,T
				       >::cond,
      stensor<N,T,StensorStatic> >::type
    stensor<N,T,Storage>::buildFromVectorsSymmetricDiadicProduct(const VectorType&  v1,
								 const VectorType2& v2)
    {
      stensor<N,T,Storage> s;
      tfel::math::internals::BuildStensorFromVectorsSymmetricDiadicProduct<N>::exe(s,v1,v2);
      return s;
    }

    template<unsigned short N,typename T,template<unsigned short,typename> class Storage>
    stensor<N,T,StensorStatic>
    stensor<N,T,Storage>::buildFromEigenValuesAndVectors(const T& v1,const T& v2,const T& v3,
							 const tmatrix<3u,3u,
								       typename tfel::typetraits::BaseType<T>::type>& m)
    {
      stensor<N,T,Storage> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(s,v1,v2,v3,m);
      return s;
    }

    template<unsigned short N,typename T,template<unsigned short,typename> class Storage>
    stensor<N,T,StensorStatic>
    stensor<N,T,Storage>::buildFromEigenValuesAndVectors(const tvector<3u,T>& vp,
							 const tmatrix<3u,3u,
								       typename tfel::typetraits::BaseType<T>::type>& m)
    {
      stensor<N,T,Storage> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(s,vp(0),vp(1),vp(2),m);
      return s;
    }

    template<unsigned short N,typename T,template<unsigned short,typename> class Storage>
    stensor<N,T,StensorStatic>
    stensor<N,T,Storage>::buildLogarithmFromEigenValuesAndVectors(const T& v1,const T& v2,const T& v3,
								  const tmatrix<3u,3u,
								  typename tfel::typetraits::BaseType<T>::type>& m)
    {
      stensor<N,T,Storage> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(s,std::log(v1),std::log(v2),std::log(v3),m);
      return s;
    }

    template<unsigned short N,typename T,template<unsigned short,typename> class Storage>
    stensor<N,T,StensorStatic>
    stensor<N,T,Storage>::buildLogarithmFromEigenValuesAndVectors(const tvector<3u,T>& vp,
								  const tmatrix<3u,3u,
								  typename tfel::typetraits::BaseType<T>::type>& m)
    {
      stensor<N,T,Storage> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(s,std::log(vp(0)),
									   std::log(vp(1)),
									   std::log(vp(2)),m);
      return s;
    }

    template<unsigned short N,typename T,template<unsigned short,typename> class Storage>
    stensor<N,T,StensorStatic>
    stensor<N,T,Storage>::buildPositivePartFromEigenValuesAndVectors(const T& v1,const T& v2,const T& v3,
								  const tmatrix<3u,3u,
								  typename tfel::typetraits::BaseType<T>::type>& m)
    {
      stensor<N,T,Storage> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(s,std::max(T(0),v1),
									   std::max(T(0),v2),
									   std::max(T(0),v3),m);
      return s;
    }

    template<unsigned short N,typename T,template<unsigned short,typename> class Storage>
    stensor<N,T,StensorStatic>
    stensor<N,T,Storage>::buildPositivePartFromEigenValuesAndVectors(const tvector<3u,T>& vp,
								     const tmatrix<3u,3u,
										   typename tfel::typetraits::BaseType<T>::type>& m)
    {
      stensor<N,T,Storage> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(s,std::max(T(0),vp(0)),
									   std::max(T(0),vp(1)),
									   std::max(T(0),vp(2)),m);
      return s;
    }

    template<unsigned short N,typename T,template<unsigned short,typename> class Storage>
    stensor<N,T,StensorStatic>
    stensor<N,T,Storage>::buildNegativePartFromEigenValuesAndVectors(const T& v1,const T& v2,const T& v3,
								     const tmatrix<3u,3u,
								     typename tfel::typetraits::BaseType<T>::type>& m)
    {
      stensor<N,T,Storage> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(s,std::min(T(0),v1),
									   std::min(T(0),v2),
									   std::min(T(0),v3),m);
      return s;
    }

    template<unsigned short N,typename T,template<unsigned short,typename> class Storage>
    stensor<N,T,StensorStatic>
    stensor<N,T,Storage>::buildNegativePartFromEigenValuesAndVectors(const tvector<3u,T>& vp,
								     const tmatrix<3u,3u,
								     typename tfel::typetraits::BaseType<T>::type>& m)
    {
      stensor<N,T,Storage> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(s,std::min(T(0),vp(0)),
									   std::min(T(0),vp(1)),
									   std::min(T(0),vp(2)),m);
      return s;
    }

    template<typename StensorType>
    typename std::enable_if<
      ((tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
       (StensorTraits<StensorType>::dime==1u) &&
       (tfel::typetraits::IsFundamentalNumericType<typename StensorTraits<StensorType>::NumType>::cond)),
      stensor<1u,typename StensorTraits<StensorType>::NumType>
    >::type
    logarithm(const StensorType& s,
	      const bool)
    {
      typedef typename StensorTraits<StensorType>::NumType NumType;
      stensor<1u,NumType> l;
      l(0) = std::log(s(0));
      l(1) = std::log(s(1));
      l(2) = std::log(s(2));
      return l;
    }

    template<typename StensorType>
    typename std::enable_if<
      ((tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
       ((StensorTraits<StensorType>::dime==2u)||
	(StensorTraits<StensorType>::dime==3u))&&
       (tfel::typetraits::IsFundamentalNumericType<typename StensorTraits<StensorType>::NumType>::cond)),
      stensor<StensorTraits<StensorType>::dime,typename StensorTraits<StensorType>::NumType>
    >::type
    logarithm(const StensorType& s_,
	      const bool b)
    {
      typedef typename StensorTraits<StensorType>::NumType NumType;
      typedef typename tfel::typetraits::BaseType<NumType>::type base;
      typedef stensor<StensorTraits<StensorType>::dime,NumType> Stensor;
      tvector<3u,NumType> vp;
      tmatrix<3u,3u,base> m;
      stensor<StensorTraits<StensorType>::dime,NumType> s(s_);
      s.computeEigenVectors(vp,m,b);
      return Stensor::buildFromEigenValuesAndVectors(std::log(vp(0)),
						     std::log(vp(1)),
						     std::log(vp(2)),m);
    }

    template<typename StensorType>
    typename std::enable_if<
      ((tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
       (StensorTraits<StensorType>::dime==1u) &&
       (tfel::typetraits::IsFundamentalNumericType<typename StensorTraits<StensorType>::NumType>::cond)),
      stensor<1u,typename StensorTraits<StensorType>::NumType>
    >::type
    absolute_value(const StensorType& s,
		  const bool)
    {
      typedef typename StensorTraits<StensorType>::NumType NumType;
      stensor<1u,NumType> l;
      l(0) = std::abs(s(0));
      l(1) = std::abs(s(1));
      l(2) = std::abs(s(2));
      return l;
    }

    template<typename StensorType>
    typename std::enable_if<
      ((tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
       ((StensorTraits<StensorType>::dime==2u)||
	(StensorTraits<StensorType>::dime==3u))&&
       (tfel::typetraits::IsFundamentalNumericType<typename StensorTraits<StensorType>::NumType>::cond)),
      stensor<StensorTraits<StensorType>::dime,typename StensorTraits<StensorType>::NumType>
    >::type
    absolute_value(const StensorType& s_,
		   const bool b)
    {
      typedef typename StensorTraits<StensorType>::NumType NumType;
      typedef typename tfel::typetraits::BaseType<NumType>::type base;
      typedef stensor<StensorTraits<StensorType>::dime,NumType> Stensor;
      tvector<3u,NumType> vp;
      tmatrix<3u,3u,base> m;
      stensor<StensorTraits<StensorType>::dime,NumType> s(s_);
      s.computeEigenVectors(vp,m,b);
      return Stensor::buildFromEigenValuesAndVectors(std::abs(vp(0)),
						     std::abs(vp(1)),
						     std::abs(vp(2)),m);
    }

    template<typename StensorType>
    typename std::enable_if<
      ((tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
       (StensorTraits<StensorType>::dime==1u) &&
       (tfel::typetraits::IsFundamentalNumericType<typename StensorTraits<StensorType>::NumType>::cond)),
      stensor<1u,typename StensorTraits<StensorType>::NumType>
    >::type
    positive_part(const StensorType& s,
		  const bool)
    {
      typedef typename StensorTraits<StensorType>::NumType NumType;
      stensor<1u,NumType> l;
      l(0) = std::max(s(0),NumType(0));
      l(1) = std::max(s(1),NumType(0));
      l(2) = std::max(s(2),NumType(0));
      return l;
    }

    template<typename StensorType>
    typename std::enable_if<
      ((tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
       ((StensorTraits<StensorType>::dime==2u)||
	(StensorTraits<StensorType>::dime==3u))&&
       (tfel::typetraits::IsFundamentalNumericType<typename StensorTraits<StensorType>::NumType>::cond)),
      stensor<StensorTraits<StensorType>::dime,typename StensorTraits<StensorType>::NumType>
    >::type
    positive_part(const StensorType& s_,
		  const bool b)
    {
      typedef typename StensorTraits<StensorType>::NumType NumType;
      typedef typename tfel::typetraits::BaseType<NumType>::type base;
      typedef stensor<StensorTraits<StensorType>::dime,NumType> Stensor;
      tvector<3u,NumType> vp;
      tmatrix<3u,3u,base> m;
      stensor<StensorTraits<StensorType>::dime,NumType> s(s_);
      s.computeEigenVectors(vp,m,b);
      return Stensor::buildFromEigenValuesAndVectors(std::max(vp(0),NumType(0)),
						     std::max(vp(1),NumType(0)),
						     std::max(vp(2),NumType(0)),m);
    }

    template<typename StensorType>
    typename std::enable_if<
      ((tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
       (StensorTraits<StensorType>::dime==1u) &&
       (tfel::typetraits::IsFundamentalNumericType<typename StensorTraits<StensorType>::NumType>::cond)),
      stensor<1u,typename StensorTraits<StensorType>::NumType>
    >::type
    negative_part(const StensorType& s,
		  const bool)
    {
      typedef typename StensorTraits<StensorType>::NumType NumType;
      stensor<1u,NumType> l;
      l(0) = std::min(s(0),NumType(0));
      l(1) = std::min(s(1),NumType(0));
      l(2) = std::min(s(2),NumType(0));
      return l;
    }

    template<typename StensorType>
    typename std::enable_if<
      ((tfel::meta::Implements<StensorType,StensorConcept>::cond)&&
       ((StensorTraits<StensorType>::dime==2u)||
	(StensorTraits<StensorType>::dime==3u))&&
       (tfel::typetraits::IsFundamentalNumericType<typename StensorTraits<StensorType>::NumType>::cond)),
      stensor<StensorTraits<StensorType>::dime,typename StensorTraits<StensorType>::NumType>
    >::type
    negative_part(const StensorType& s_,
		  const bool b)
    {
      typedef typename StensorTraits<StensorType>::NumType NumType;
      typedef typename tfel::typetraits::BaseType<NumType>::type base;
      typedef stensor<StensorTraits<StensorType>::dime,NumType> Stensor;
      tvector<3u,NumType> vp;
      tmatrix<3u,3u,base> m;
      stensor<StensorTraits<StensorType>::dime,NumType> s(s_);
      s.computeEigenVectors(vp,m,b);
      return Stensor::buildFromEigenValuesAndVectors(std::min(vp(0),NumType(0)),
						     std::min(vp(1),NumType(0)),
						     std::min(vp(2),NumType(0)),m);
    }
    
#endif

  } //end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_STENSOR_IXX */
