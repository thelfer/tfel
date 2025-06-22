/*!
 * \file   include/TFEL/Math/Stensor/stensor.ixx
 * \brief  This file implements the methods of the class stensor.
 * \author Thomas Helfer
 * \date   02 jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSOR_IXX
#define LIB_TFEL_MATH_STENSOR_IXX

#include <cmath>
#include <iterator>

#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"

#include "TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "TFEL/Math/Vector/VectorUtilities.hxx"

#include "TFEL/Math/Stensor/SymmetricStensorProduct.hxx"
#include "TFEL/Math/Stensor/Internals/StensorImport.hxx"
#include "TFEL/Math/Stensor/Internals/StensorExport.hxx"
#include "TFEL/Math/Stensor/Internals/StensorChangeBasis.hxx"
#include "TFEL/Math/Stensor/Internals/StensorInvert.hxx"
#include "TFEL/Math/Stensor/Internals/StensorDeterminant.hxx"
#include "TFEL/Math/Stensor/Internals/BuildStensorFromMatrix.hxx"
#include "TFEL/Math/Stensor/Internals/BuildStensorFromVectorDiadicProduct.hxx"
#include "TFEL/Math/Stensor/Internals/BuildStensorFromVectorsSymmetricDiadicProduct.hxx"
#include "TFEL/Math/Stensor/Internals/BuildStensorFromEigenValuesAndVectors.hxx"
#include "TFEL/Math/Stensor/Internals/StensorEigenSolver.hxx"
#include "TFEL/Math/Stensor/Internals/SortEigenValues.hxx"
#include "TFEL/Math/Stensor/Internals/SortEigenVectors.hxx"
#include "TFEL/Math/Stensor/Internals/StensorComputeEigenValuesDerivatives.hxx"
#include "TFEL/Math/Stensor/Internals/StensorComputeEigenVectorsDerivatives.hxx"
#include "TFEL/Math/Stensor/Internals/StensorComputeIsotropicFunctionDerivative.hxx"

namespace tfel {

  namespace math {

    template <typename Child>
    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<Child>::dime == StensorTraits<StensorType>::dime &&
            tfel::typetraits::IsAssignableTo<StensorNumType<StensorType>,
                                             StensorNumType<Child>>::cond,
        Child&>::type
    stensor_base<Child>::operator=(const StensorType& src) {
      auto& child = static_cast<Child&>(*this);
      vectorToTab<StensorDimeToSize<StensorTraits<Child>::dime>::value>::exe(
          src, child);
      return child;
    }

    template <typename Child>
    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<Child>::dime == StensorTraits<StensorType>::dime &&
            tfel::typetraits::IsAssignableTo<StensorNumType<StensorType>,
                                             StensorNumType<Child>>::cond,
        Child&>::type
    stensor_base<Child>::operator+=(const StensorType& src) {
      auto& child = static_cast<Child&>(*this);
      VectorUtilities<StensorDimeToSize<StensorTraits<Child>::dime>::value>::
          PlusEqual(child, src);
      return child;
    }

    template <typename Child>
    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<Child>::dime == StensorTraits<StensorType>::dime &&
            tfel::typetraits::IsAssignableTo<StensorNumType<StensorType>,
                                             StensorNumType<Child>>::cond,
        Child&>::type
    stensor_base<Child>::operator-=(const StensorType& src) {
      auto& child = static_cast<Child&>(*this);
      VectorUtilities<StensorDimeToSize<StensorTraits<Child>::dime>::value>::
          MinusEqual(child, src);
      return child;
    }

    // *= operator
    template <typename Child>
    template <typename T2>
    typename std::enable_if<
        tfel::typetraits::IsScalar<T2>::cond &&
            std::is_same<
                typename ResultType<StensorNumType<Child>, T2, OpMult>::type,
                StensorNumType<Child>>::value,
        Child&>::type
    stensor_base<Child>::operator*=(const T2 s) {
      auto& child = static_cast<Child&>(*this);
      VectorUtilities<
          StensorDimeToSize<StensorTraits<Child>::dime>::value>::scale(child,
                                                                       s);
      return child;
    }

    // /= operator
    template <typename Child>
    template <typename T2>
    typename std::enable_if<
        tfel::typetraits::IsScalar<T2>::cond &&
            std::is_same<
                typename ResultType<StensorNumType<Child>, T2, OpDiv>::type,
                StensorNumType<Child>>::value,
        Child&>::type
    stensor_base<Child>::operator/=(const T2 s) {
      auto& child = static_cast<Child&>(*this);
      VectorUtilities<StensorDimeToSize<StensorTraits<Child>::dime>::value>::
          scale(child, (static_cast<tfel::typetraits::base_type<T2>>(1u)) / s);
      return child;
    }

    template <unsigned short N, typename T>
    template <
        typename T2,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    constexpr stensor<N, T>::stensor(const T2& init)
        : fsarray<StensorDimeToSize<N>::value, T>(init) {}

    template <unsigned short N, typename T>
    template <
        typename T2,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    constexpr stensor<N, T>::stensor(const std::initializer_list<T2>& init)
        : fsarray<StensorDimeToSize<N>::value, T>(init) {}

    template <unsigned short N, typename T>
    template <typename InputIterator,
              typename std::enable_if<
                  std::is_same<
                      typename std::iterator_traits<InputIterator>::value_type,
                      tfel::typetraits::base_type<T>>::value,
                  bool>::type>
    stensor<N, T>::stensor(const InputIterator p) {
      typedef tfel::typetraits::base_type<T> base;
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsSafelyReinterpretCastableTo<T, base>::cond));
      tfel::fsalgo::copy<StensorDimeToSize<N>::value>::exe(
          p, reinterpret_cast<base*>(this->v));
    }

    template <unsigned short N, typename T>
    template <
        typename T2,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    constexpr stensor<N, T>::stensor(const stensor<N, T2>& src)
        : fsarray<StensorDimeToSize<N>::value, T>(src) {}

    template <unsigned short N, typename T>
    template <
        typename T2,
        typename Operation,
        typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                bool>::type>
    stensor<N, T>::stensor(const Expr<stensor<N, T2>, Operation>& src) {
      vectorToTab<StensorDimeToSize<N>::value>::exe(src, this->v);
    }

    template <unsigned short N, typename T>
    T& stensor<N, T>::operator()(const unsigned short i) noexcept {
      return this->v[i];
    }

    template <unsigned short N, typename T>
    constexpr const T& stensor<N, T>::operator()(
        const unsigned short i) const noexcept {
      return this->v[i];
    }

    template <unsigned short N, typename T>
    constexpr typename stensor<N, T>::RunTimeProperties
    stensor<N, T>::getRunTimeProperties() const noexcept {
      return RunTimeProperties();
    }

    // Import from Voigt
    template <unsigned short N, typename T>
    template <typename InputIterator>
    typename std::enable_if<
        std::is_same<typename std::iterator_traits<InputIterator>::value_type,
                     tfel::typetraits::base_type<T>>::value,
        void>::type
    stensor<N, T>::importVoigt(const InputIterator p) {
      typedef tfel::typetraits::base_type<T> base;
      tfel::math::internals::ImportFromVoigt<N>::exe(
          reinterpret_cast<base*>(this->v), p);
    }

    // Import from Tab
    template <unsigned short N, typename T>
    template <typename InputIterator>
    typename std::enable_if<
        std::is_same<typename std::iterator_traits<InputIterator>::value_type,
                     tfel::typetraits::base_type<T>>::value,
        void>::type
    stensor<N, T>::importTab(const InputIterator p) {
      typedef tfel::typetraits::base_type<T> base;
      typedef tfel::math::internals::ImportFromTab<N> Import;
      Import::exe(reinterpret_cast<base*>(this->v), p);
    }

    // Import from values
    template <unsigned short N, typename T>
    template <typename InputIterator>
    typename std::enable_if<
        std::is_same<typename std::iterator_traits<InputIterator>::value_type,
                     tfel::typetraits::base_type<T>>::value,
        void>::type
    stensor<N, T>::import(const InputIterator p) {
      typedef tfel::typetraits::base_type<T> base;
      tfel::fsalgo::copy<StensorDimeToSize<N>::value>::exe(
          p, reinterpret_cast<base*>(this->v));
    }

    // Export to Tab
    template <unsigned short N, typename T>
    template <typename T2>
    typename std::enable_if<tfel::typetraits::IsSafelyReinterpretCastableTo<
                                T2,
                                tfel::typetraits::base_type<T>>::cond,
                            void>::type
    stensor<N, T>::exportTab(T2* const src) const {
      typedef tfel::typetraits::base_type<T> base;
      typedef tfel::math::internals::ExportToTab<N> Export;
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsSafelyReinterpretCastableTo<T, base>::cond));
      Export::exe(reinterpret_cast<const T2*>(this->v), src);
    }

    // Write to Tab
    template <unsigned short N, typename T>
    template <typename T2>
    typename std::enable_if<tfel::typetraits::IsSafelyReinterpretCastableTo<
                                T2,
                                tfel::typetraits::base_type<T>>::cond,
                            void>::type
    stensor<N, T>::write(T2* const t) const {
      typedef tfel::typetraits::base_type<T> base;
      typedef tfel::fsalgo::copy<StensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsSafelyReinterpretCastableTo<T, base>::cond));
      Copy::exe(reinterpret_cast<const base*>(this->v), t);
    }

    template <unsigned short N, typename T>
    template <typename stensor_common::EigenSolver es>
    void stensor<N, T>::computeEigenValues(T& vp0,
                                           T& vp1,
                                           T& vp2,
                                           const bool b) const {
      using ESolver = tfel::math::internals::StensorEigenSolver<es, N, T>;
      ESolver::computeEigenValues(vp0, vp1, vp2, this->v, b);
    }  // end of stensor<N,T>::computeEigenValues

    template <unsigned short N, typename T>
    template <typename stensor_common::EigenSolver es>
    void stensor<N, T>::computeEigenValues(tvector<3u, T>& vp,
                                           const bool b) const {
      this->template computeEigenValues<es>(vp(0), vp(1), vp(2), b);
    }

    template <unsigned short N, typename T>
    template <typename stensor_common::EigenSolver es>
    tvector<3u, T> stensor<N, T>::computeEigenValues(const bool b) const {
      tvector<3u, T> vp;
      this->template computeEigenValues<es>(vp, b);
      return vp;
    }  // end of stensor<N,T>::computeEigenValues

    template <unsigned short N, typename T>
    template <typename stensor_common::EigenSolver es>
    void stensor<N, T>::computeEigenValues(T& vp0,
                                           T& vp1,
                                           T& vp2,
                                           const EigenValuesOrdering o,
                                           const bool b) const {
      this->template computeEigenValues<es>(vp0, vp1, vp2, b);
      tfel::math::internals::SortEigenValues<N>::exe(vp0, vp1, vp2, o);
    }  // end of stensor<N,T>::computeEigenValues

    template <unsigned short N, typename T>
    template <typename stensor_common::EigenSolver es>
    void stensor<N, T>::computeEigenValues(tvector<3u, T>& vp,
                                           const EigenValuesOrdering o,
                                           const bool b) const {
      this->template computeEigenValues<es>(vp(0), vp(1), vp(2), o, b);
    }

    template <unsigned short N, typename T>
    template <typename stensor_common::EigenSolver es>
    tvector<3u, T> stensor<N, T>::computeEigenValues(
        const EigenValuesOrdering o, const bool b) const {
      tvector<3u, T> vp;
      this->template computeEigenValues<es>(vp, o, b);
      return vp;
    }  // end of stensor<N,T>::computeEigenValues

    template <unsigned short N, typename T>
    template <typename stensor_common::EigenSolver es>
    void stensor<N, T>::computeEigenVectors(
        tvector<3u, T>& vp,
        tmatrix<3u, 3u, tfel::typetraits::base_type<T>>& m,
        const bool b) const {
      using ESolver = tfel::math::internals::StensorEigenSolver<es, N, T>;
      ESolver::computeEigenVectors(vp, m, this->v, b);
    }

    // computeEigenVectors
    template <unsigned short N, typename T>
    template <typename stensor_common::EigenSolver es>
    std::tuple<tvector<3u, T>, tmatrix<3u, 3u, tfel::typetraits::base_type<T>>>
    stensor<N, T>::computeEigenVectors(const bool b) const {
      std::tuple<tvector<3u, T>,
                 tmatrix<3u, 3u, tfel::typetraits::base_type<T>>>
          r;
      this->template computeEigenVectors<es>(std::get<0>(r), std::get<1>(r), b);
      return r;
    }  // end of stensor<N,T>::computeEigenVectors

    template <unsigned short N, typename T>
    template <typename stensor_common::EigenSolver es>
    void stensor<N, T>::computeEigenVectors(
        tvector<3u, T>& vp,
        tmatrix<3u, 3u, tfel::typetraits::base_type<T>>& m,
        const EigenValuesOrdering o,
        const bool b) const {
      this->template computeEigenVectors<es>(vp, m, b);
      tfel::math::internals::SortEigenVectors<N>::exe(vp, m, o);
    }  // end of stensor<N,T>::computeEigenVectors

    // computeEigenVectors
    template <unsigned short N, typename T>
    template <typename stensor_common::EigenSolver es>
    std::tuple<tvector<3u, T>, tmatrix<3u, 3u, tfel::typetraits::base_type<T>>>
    stensor<N, T>::computeEigenVectors(const EigenValuesOrdering o,
                                       const bool b) const {
      std::tuple<tvector<3u, T>,
                 tmatrix<3u, 3u, tfel::typetraits::base_type<T>>>
          r;
      this->template computeEigenVectors<es>(std::get<0>(r), std::get<1>(r), o,
                                             b);
      return r;
    }  // end of stensor<N,T>::computeEigenVectors

    // computeEigenVectors
    template <unsigned short N, typename T>
    template <typename VectorType>
    bool stensor<N, T>::computeEigenVector(VectorType& ev, const T vp) const {
      using real = VectorNumType<VectorType>;
      typedef tfel::math::internals::StensorComputeEigenVectors<N> SCEV;
      TFEL_STATIC_ASSERT(
          tfel::typetraits::IsFundamentalNumericType<real>::cond);
      TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<real>::cond);
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsSafelyReinterpretCastableTo<T, real>::cond));
      return SCEV::computeEigenVector(ev,
                                      reinterpret_cast<const real*>(this->v),
                                      *(reinterpret_cast<const real*>(&vp)));
    }

    template <unsigned short N, typename T>
    std::tuple<stensor<N, T>, stensor<N, T>, stensor<N, T>>
    stensor<N, T>::computeEigenValuesDerivatives(
        const tmatrix<3u, 3u, tfel::typetraits::base_type<T>>& m) {
      return stensor<N, T>::computeEigenTensors(m);
    }  // end of stensor<N,T>::computeEigenValuesDerivatives

    template <unsigned short N, typename T>
    template <typename StensorType>
    typename std::enable_if<
        (tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
            (StensorTraits<StensorType>::dime == N) &&
            (tfel::typetraits::IsAssignableTo<
                tfel::typetraits::base_type<T>,
                StensorNumType<StensorType>>::cond),
        void>::type
    stensor<N, T>::computeEigenValuesDerivatives(
        StensorType& n0,
        StensorType& n1,
        StensorType& n2,
        const tmatrix<3u, 3u, tfel::typetraits::base_type<T>>& m) {
      return stensor<N, T>::computeEigenTensors(n0, n1, n2, m);
    }  // end of stensor<N,T>::computeEigenValuesDerivatives

    template <unsigned short N, typename T>
    std::tuple<stensor<N, T>, stensor<N, T>, stensor<N, T>>
    stensor<N, T>::computeEigenTensors(
        const tmatrix<3u, 3u, tfel::typetraits::base_type<T>>& m) {
      std::tuple<stensor<N, T>, stensor<N, T>, stensor<N, T>> r;
      tfel::math::internals::StensorComputeEigenValuesDerivatives<N>::exe(
          std::get<0>(r), std::get<1>(r), std::get<2>(r), m);
      return r;
    }  // end of stensor<N,T>::computeEigenTensors

    template <unsigned short N, typename T>
    template <typename StensorType>
    typename std::enable_if<
        (tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
            (StensorTraits<StensorType>::dime == N) &&
            (tfel::typetraits::IsAssignableTo<
                tfel::typetraits::base_type<T>,
                StensorNumType<StensorType>>::cond),
        void>::type
    stensor<N, T>::computeEigenTensors(StensorType& n0,
                                       StensorType& n1,
                                       StensorType& n2,
                                       const rotation_matrix<T>& m) {
      return tfel::math::internals::StensorComputeEigenValuesDerivatives<
          N>::exe(n0, n1, n2, m);
    }  // end of stensor<N,T>::computeEigenTensors

    template <unsigned short N, typename T>
    template <typename ST2toST2Type>
    typename std::enable_if<
        (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond) &&
            (ST2toST2Traits<ST2toST2Type>::dime == N) &&
            (tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<tfel::typetraits::base_type<T>,
                                             T,
                                             OpDiv>::Result,
                ST2toST2NumType<ST2toST2Type>>::cond),
        void>::type
    stensor<N, T>::computeEigenTensorsDerivatives(ST2toST2Type& dn0_ds,
                                                  ST2toST2Type& dn1_ds,
                                                  ST2toST2Type& dn2_ds,
                                                  const tvector<3u, T>& vp,
                                                  const rotation_matrix<T>& m,
                                                  const T eps) {
      return tfel::math::internals::StensorComputeEigenTensorsDerivatives<
          N>::exe(dn0_ds, dn1_ds, dn2_ds, vp, m, eps);
    }

    template <unsigned short N, typename T>
    template <typename Function>
    stensor<N, typename std::result_of<Function(T)>::type>
    stensor<N, T>::computeIsotropicFunction(const Function& f,
                                            const tvector<3u, T>& vp,
                                            const rotation_matrix<T>& m) {
      return stensor<N, T>::buildFromEigenValuesAndVectors(f(vp(0)), f(vp(1)),
                                                           f(vp(2)), m);
    }

    template <unsigned short N, typename T>
    template <typename T2>
    stensor<N, T2> stensor<N, T>::computeIsotropicFunction(
        const tvector<3u, T2>& f, const rotation_matrix<T>& m) {
      return stensor<N, T>::buildFromEigenValuesAndVectors(f[0], f[1], f[2], m);
    }

    template <unsigned short N, typename T>
    template <typename Function, typename FunctionDerivative>
    st2tost2<N, typename std::result_of<FunctionDerivative(T)>::type>
    stensor<N, T>::computeIsotropicFunctionDerivative(
        const Function& f,
        const FunctionDerivative& df,
        const tvector<3u, T>& vp,
        const rotation_matrix<T>& m,
        const T eps) {
      st2tost2<N, typename std::result_of<FunctionDerivative(T)>::type> r;
      stensor<N, T>::computeIsotropicFunctionDerivative(r, f, df, vp, m, eps);
      return r;
    }  // end of stensor<N,T>::computeIsotropicFunctionDerivative

    template <unsigned short N, typename T>
    template <typename ST2toST2Type,
              typename Function,
              typename FunctionDerivative>
    typename std::enable_if<
        (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond) &&
            (ST2toST2Traits<ST2toST2Type>::dime == N) &&
            (tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<tfel::typetraits::base_type<T>,
                                             T,
                                             OpDiv>::Result,
                ST2toST2NumType<ST2toST2Type>>::cond),
        void>::type
    stensor<N, T>::computeIsotropicFunctionDerivative(
        ST2toST2Type& d,
        const Function& f,
        const FunctionDerivative& df,
        const tvector<3u, T>& vp,
        const rotation_matrix<T>& m,
        const T eps) {
      tfel::math::internals::StensorComputeIsotropicFunctionDerivative<N>::exe2(
          d, f, df, vp, m, eps);
    }

    template <unsigned short N, typename T>
    template <typename T1, typename T2>
    st2tost2<N, T2> stensor<N, T>::computeIsotropicFunctionDerivative(
        const tvector<3u, T1>& f,
        const tvector<3u, T2>& df,
        const tvector<3u, T>& vp,
        const rotation_matrix<T>& m,
        const T eps) {
      st2tost2<N, T2> r;
      stensor<N, T>::computeIsotropicFunctionDerivative(r, f, df, vp, m, eps);
      return r;
    }  // end of stensor<N,T>::computeIsotropicFunctionDerivative

    template <unsigned short N, typename T>
    template <typename ST2toST2Type, typename T1, typename T2>
    typename std::enable_if<
        (tfel::meta::Implements<ST2toST2Type, ST2toST2Concept>::cond) &&
            (ST2toST2Traits<ST2toST2Type>::dime == N) &&
            (tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<tfel::typetraits::base_type<T>,
                                             T,
                                             OpDiv>::Result,
                ST2toST2NumType<ST2toST2Type>>::cond),
        void>::type
    stensor<N, T>::computeIsotropicFunctionDerivative(
        ST2toST2Type& d,
        const tvector<3u, T1>& f,
        const tvector<3u, T2>& df,
        const tvector<3u, T>& vp,
        const rotation_matrix<T>& m,
        const T eps) {
      tfel::math::internals::StensorComputeIsotropicFunctionDerivative<N>::exe(
          d, f, df, vp, m, eps);
    }

    template <unsigned short N, typename T>
    template <typename stensor_common::EigenSolver es, typename Function>
    stensor<N, typename std::result_of<Function(T)>::type>
    stensor<N, T>::computeIsotropicFunction(const Function& f,
                                            const bool b) const {
      using base = tfel::typetraits::base_type<T>;
      tvector<3u, T> vp;
      tmatrix<3u, 3u, base> m;
      this->template computeEigenVectors<es>(vp, m, b);
      return stensor<N, T>::buildFromEigenValuesAndVectors(f(vp(0)), f(vp(1)),
                                                           f(vp(2)), m);
    }

    template <unsigned short N, typename T>
    template <typename stensor_common::EigenSolver es,
              typename Function,
              typename FunctionDerivative>
    st2tost2<N, typename std::result_of<FunctionDerivative(T)>::type>
    stensor<N, T>::computeIsotropicFunctionDerivative(
        const Function& f,
        const FunctionDerivative& df,
        const T eps,
        const bool b) const {
      using base = tfel::typetraits::base_type<T>;
      st2tost2<N, T> r;
      tvector<3u, T> vp;
      tmatrix<3u, 3u, base> m;
      this->template computeEigenVectors<es>(vp, m, b);
      stensor<N, T>::computeIsotropicFunctionDerivative(r, f, df, vp, m, eps);
      return r;
    }

    template <unsigned short N, typename T>
    template <typename stensor_common::EigenSolver es,
              typename Function,
              typename FunctionDerivative>
    std::pair<stensor<N, typename std::result_of<Function(T)>::type>,
              st2tost2<N, typename std::result_of<FunctionDerivative(T)>::type>>
    stensor<N, T>::computeIsotropicFunctionAndDerivative(
        const Function& f,
        const FunctionDerivative& df,
        const T eps,
        const bool b) const {
      using base = tfel::typetraits::base_type<T>;
      std::pair<stensor<N, T>, st2tost2<N, T>> r;
      tvector<3u, T> vp;
      tmatrix<3u, 3u, base> m;
      this->template computeEigenVectors<es>(vp, m, b);
      const auto fv = map(f, vp);
      const auto dfv = map(df, vp);
      r.first =
          stensor<N, T>::buildFromEigenValuesAndVectors(fv(0), fv(1), fv(2), m);
      stensor<N, T>::computeIsotropicFunctionDerivative(r.second, fv, dfv, vp,
                                                        m, eps);
      return r;
    }  // end of stensor<N,T>::computeIsotropicFunctionAndDerivative

    // ChangeBasis
    template <unsigned short N, typename T>
    void stensor<N, T>::changeBasis(const rotation_matrix<T>& m) {
      return tfel::math::internals::StensorChangeBasis<N>::exe(this->v, m);
    }

    // Return Id
    template <unsigned short N, typename T>
    constexpr stensor<N, T> stensor<N, T>::Id() {
      return stensor<N, T>{{T{1}, T{1}, T{1}, T{0}, T{0}, T{0}}};
    }  // end of stensor<N,T>::Id

    template <unsigned short N, typename T>
    template <typename InputIterator>
    void stensor<N, T>::copy(const InputIterator src) {
      tfel::fsalgo::copy<StensorDimeToSize<N>::value>::exe(src, *this);
    }

    template <unsigned short N, typename T, typename OutputIterator>
    TFEL_MATH_INLINE2
        typename std::enable_if<tfel::typetraits::IsScalar<T>::cond, void>::type
        exportToBaseTypeArray(const stensor<N, T>& s, OutputIterator p) {
      typedef tfel::typetraits::base_type<T> base;
      typedef tfel::fsalgo::copy<StensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT(
          (tfel::typetraits::IsSafelyReinterpretCastableTo<T, base>::cond));
      Copy::exe(reinterpret_cast<const base*>(&s[0]), p);
    }

    template <typename T>
    T tresca(const stensor<1u, T>& s, const bool) {
      const auto sd1 = std::abs(s[0] - s[1]);
      const auto sd2 = std::abs(s[0] - s[2]);
      const auto sd3 = std::abs(s[2] - s[1]);
      const auto tmp = sd1 > sd2 ? sd1 : sd2;
      const auto tmp2 = sd3 > tmp ? sd3 : tmp;
      return tmp2;
    }  // end of tresca

    template <unsigned short N, typename T>
    T tresca(const stensor<N, T>& s, const bool b) {
      T s1, s2, s3;
      s.computeEigenValues(s1, s2, s3, b);
      const auto sd1 = std::abs(s1 - s2);
      const auto sd2 = std::abs(s1 - s3);
      const auto sd3 = std::abs(s3 - s2);
      const auto tmp = sd1 > sd2 ? sd1 : sd2;
      const auto tmp2 = sd3 > tmp ? sd3 : tmp;
      return tmp2;
    }  // end of tresca

    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        stensor<StensorTraits<StensorType>::dime,
                StensorNumType<StensorType>>>::type
    square_root(const StensorType& s) {
      using tfel::fsalgo::copy;
      typedef StensorNumType<StensorType> T;
      typedef tfel::typetraits::base_type<T> real;
      stensor<StensorTraits<StensorType>::dime, T> tmp(s);
      stensor<StensorTraits<StensorType>::dime, T> r(T(0));
      tmatrix<3u, 3u, real> vec;
      tvector<3u, T> vp;
      tmp.computeEigenVectors(vp, vec);
      r[0] = std::sqrt(vp[0]);
      r[1] = std::sqrt(vp[1]);
      r[2] = std::sqrt(vp[2]);
      r.changeBasis(transpose(vec));
      return r;
    }  // end of square_root

    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        typename ComputeUnaryResult<StensorNumType<StensorType>,
                                    Power<3>>::Result>::type
    det(const StensorType& s) {
      const stensor<StensorTraits<StensorType>::dime,
                    StensorNumType<StensorType>>
          tmp(s);
      return tfel::math::internals::StensorDeterminant<
          StensorTraits<StensorType>::dime>::exe(tmp);
    }

    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        stensor<StensorTraits<StensorType>::dime,
                typename ComputeBinaryResult<
                    tfel::typetraits::base_type<StensorNumType<StensorType>>,
                    StensorNumType<StensorType>,
                    OpDiv>::Result>>::type
    invert(const StensorType& s) {
      const stensor<StensorTraits<StensorType>::dime,
                    StensorNumType<StensorType>>
          tmp(s);
      return tfel::math::internals::StensorInvert<
          StensorTraits<StensorType>::dime>::exe(tmp);
    }

    template <unsigned short N, typename T>
    template <typename MatrixType>
    typename std::enable_if<
        tfel::typetraits::IsAssignableTo<MatrixNumType<MatrixType>, T>::cond,
        stensor<N, T>>::type
    stensor<N, T>::buildFromMatrix(const MatrixType& m) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromMatrix<N>::exe(s, m);
      return s;
    }

    template <unsigned short N, typename T>
    template <typename VectorType>
    typename std::enable_if<
        tfel::typetraits::IsAssignableTo<
            typename ComputeUnaryResult<VectorNumType<VectorType>,
                                        Power<2>>::Result,
            T>::cond,
        stensor<N, T>>::type
    stensor<N, T>::buildFromVectorDiadicProduct(const VectorType& vec) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromVectorDiadicProduct<N>::exe(s,
                                                                         vec);
      return s;
    }

    template <unsigned short N, typename T>
    template <typename VectorType, typename VectorType2>
    typename std::enable_if<
        tfel::typetraits::IsAssignableTo<
            typename ComputeBinaryResult<VectorNumType<VectorType>,
                                         VectorNumType<VectorType2>,
                                         OpMult>::Result,
            T>::cond,
        stensor<N, T>>::type
    stensor<N, T>::buildFromVectorsSymmetricDiadicProduct(
        const VectorType& v1, const VectorType2& v2) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromVectorsSymmetricDiadicProduct<
          N>::exe(s, v1, v2);
      return s;
    }

    template <unsigned short N, typename T>
    stensor<N, T> stensor<N, T>::buildFromEigenValuesAndVectors(
        const T& v1,
        const T& v2,
        const T& v3,
        const tmatrix<3u, 3u, tfel::typetraits::base_type<T>>& m) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(
          s, v1, v2, v3, m);
      return s;
    }

    template <unsigned short N, typename T>
    stensor<N, T> stensor<N, T>::buildFromEigenValuesAndVectors(
        const tvector<3u, T>& vp,
        const tmatrix<3u, 3u, tfel::typetraits::base_type<T>>& m) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(
          s, vp(0), vp(1), vp(2), m);
      return s;
    }

    template <unsigned short N, typename T>
    stensor<N, T> stensor<N, T>::buildLogarithmFromEigenValuesAndVectors(
        const T& v1,
        const T& v2,
        const T& v3,
        const tmatrix<3u, 3u, tfel::typetraits::base_type<T>>& m) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(
          s, std::log(v1), std::log(v2), std::log(v3), m);
      return s;
    }

    template <unsigned short N, typename T>
    stensor<N, T> stensor<N, T>::buildLogarithmFromEigenValuesAndVectors(
        const tvector<3u, T>& vp,
        const tmatrix<3u, 3u, tfel::typetraits::base_type<T>>& m) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(
          s, std::log(vp(0)), std::log(vp(1)), std::log(vp(2)), m);
      return s;
    }

    template <unsigned short N, typename T>
    stensor<N, T> stensor<N, T>::buildPositivePartFromEigenValuesAndVectors(
        const T& v1,
        const T& v2,
        const T& v3,
        const tmatrix<3u, 3u, tfel::typetraits::base_type<T>>& m) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(
          s, std::max(T(0), v1), std::max(T(0), v2), std::max(T(0), v3), m);
      return s;
    }

    template <unsigned short N, typename T>
    stensor<N, T> stensor<N, T>::buildPositivePartFromEigenValuesAndVectors(
        const tvector<3u, T>& vp,
        const tmatrix<3u, 3u, tfel::typetraits::base_type<T>>& m) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(
          s, std::max(T(0), vp(0)), std::max(T(0), vp(1)),
          std::max(T(0), vp(2)), m);
      return s;
    }

    template <unsigned short N, typename T>
    stensor<N, T> stensor<N, T>::buildNegativePartFromEigenValuesAndVectors(
        const T& v1,
        const T& v2,
        const T& v3,
        const tmatrix<3u, 3u, tfel::typetraits::base_type<T>>& m) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(
          s, std::min(T(0), v1), std::min(T(0), v2), std::min(T(0), v3), m);
      return s;
    }

    template <unsigned short N, typename T>
    stensor<N, T> stensor<N, T>::buildNegativePartFromEigenValuesAndVectors(
        const tvector<3u, T>& vp,
        const tmatrix<3u, 3u, tfel::typetraits::base_type<T>>& m) {
      stensor<N, T> s;
      tfel::math::internals::BuildStensorFromEigenValuesAndVectors<N>::exe(
          s, std::min(T(0), vp(0)), std::min(T(0), vp(1)),
          std::min(T(0), vp(2)), m);
      return s;
    }

    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         (StensorTraits<StensorType>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<StensorType>>::cond)),
        stensor<1u, StensorNumType<StensorType>>>::type
    logarithm(const StensorType& s, const bool) {
      typedef StensorNumType<StensorType> NumType;
      stensor<1u, NumType> l;
      l(0) = std::log(s(0));
      l(1) = std::log(s(1));
      l(2) = std::log(s(2));
      return l;
    }

    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         ((StensorTraits<StensorType>::dime == 2u) ||
          (StensorTraits<StensorType>::dime == 3u)) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<StensorType>>::cond)),
        stensor<StensorTraits<StensorType>::dime,
                StensorNumType<StensorType>>>::type
    logarithm(const StensorType& s_, const bool b) {
      typedef StensorNumType<StensorType> NumType;
      typedef tfel::typetraits::base_type<NumType> base;
      typedef stensor<StensorTraits<StensorType>::dime, NumType> Stensor;
      tvector<3u, NumType> vp;
      tmatrix<3u, 3u, base> m;
      stensor<StensorTraits<StensorType>::dime, NumType> s(s_);
      s.computeEigenVectors(vp, m, b);
      return Stensor::buildFromEigenValuesAndVectors(
          std::log(vp(0)), std::log(vp(1)), std::log(vp(2)), m);
    }

    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         (StensorTraits<StensorType>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<StensorType>>::cond)),
        stensor<1u, StensorNumType<StensorType>>>::type
    absolute_value(const StensorType& s, const bool) {
      return {std::abs(s(0)), std::abs(s(1)), std::abs(s(2))};
    }

    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         ((StensorTraits<StensorType>::dime == 2u) ||
          (StensorTraits<StensorType>::dime == 3u)) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<StensorType>>::cond)),
        stensor<StensorTraits<StensorType>::dime,
                StensorNumType<StensorType>>>::type
    absolute_value(const StensorType& s_, const bool b) {
      typedef StensorNumType<StensorType> NumType;
      typedef tfel::typetraits::base_type<NumType> base;
      typedef stensor<StensorTraits<StensorType>::dime, NumType> Stensor;
      tvector<3u, NumType> vp;
      tmatrix<3u, 3u, base> m;
      stensor<StensorTraits<StensorType>::dime, NumType> s(s_);
      s.computeEigenVectors(vp, m, b);
      return Stensor::buildFromEigenValuesAndVectors(
          std::abs(vp(0)), std::abs(vp(1)), std::abs(vp(2)), m);
    }

    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         (StensorTraits<StensorType>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<StensorType>>::cond)),
        stensor<1u, StensorNumType<StensorType>>>::type
    positive_part(const StensorType& s, const bool) {
      typedef StensorNumType<StensorType> NumType;
      return {std::max(s(0), NumType(0)), std::max(s(1), NumType(0)),
              std::max(s(2), NumType(0))};
    }

    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         ((StensorTraits<StensorType>::dime == 2u) ||
          (StensorTraits<StensorType>::dime == 3u)) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<StensorType>>::cond)),
        stensor<StensorTraits<StensorType>::dime,
                StensorNumType<StensorType>>>::type
    positive_part(const StensorType& s_, const bool b) {
      typedef StensorNumType<StensorType> NumType;
      typedef tfel::typetraits::base_type<NumType> base;
      typedef stensor<StensorTraits<StensorType>::dime, NumType> Stensor;
      tvector<3u, NumType> vp;
      tmatrix<3u, 3u, base> m;
      stensor<StensorTraits<StensorType>::dime, NumType> s(s_);
      s.computeEigenVectors(vp, m, b);
      return Stensor::buildFromEigenValuesAndVectors(
          std::max(vp(0), NumType(0)), std::max(vp(1), NumType(0)),
          std::max(vp(2), NumType(0)), m);
    }

    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         (StensorTraits<StensorType>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<StensorType>>::cond)),
        stensor<1u, StensorNumType<StensorType>>>::type
    negative_part(const StensorType& s, const bool) {
      typedef StensorNumType<StensorType> NumType;
      return {std::min(s(0), NumType(0)), std::min(s(1), NumType(0)),
              std::min(s(2), NumType(0))};
    }

    template <typename StensorType>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
         ((StensorTraits<StensorType>::dime == 2u) ||
          (StensorTraits<StensorType>::dime == 3u)) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<StensorType>>::cond)),
        stensor<StensorTraits<StensorType>::dime,
                StensorNumType<StensorType>>>::type
    negative_part(const StensorType& s_, const bool b) {
      typedef StensorNumType<StensorType> NumType;
      typedef tfel::typetraits::base_type<NumType> base;
      typedef stensor<StensorTraits<StensorType>::dime, NumType> Stensor;
      tvector<3u, NumType> vp;
      tmatrix<3u, 3u, base> m;
      stensor<StensorTraits<StensorType>::dime, NumType> s(s_);
      s.computeEigenVectors(vp, m, b);
      return Stensor::buildFromEigenValuesAndVectors(
          std::min(vp(0), NumType(0)), std::min(vp(1), NumType(0)),
          std::min(vp(2), NumType(0)), m);
    }

    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 1u,
        stensor<1u,
                typename ComputeBinaryResult<StensorNumType<StensorType>,
                                             StensorNumType<StensorType>,
                                             OpMult>::Result>>::type
    square(const StensorType& s) {
      using T = StensorNumType<StensorType>;
      using T2 = typename ComputeBinaryResult<T, T, OpMult>::Result;
      return stensor<1u, T2>{s(0) * s(0), s(1) * s(1), s(2) * s(2)};
    }

    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 2u,
        stensor<2u,
                typename ComputeBinaryResult<StensorNumType<StensorType>,
                                             StensorNumType<StensorType>,
                                             OpMult>::Result>>::type
    square(const StensorType& s) {
      using T = StensorNumType<StensorType>;
      using T2 = typename ComputeBinaryResult<T, T, OpMult>::Result;
      using base = tfel::typetraits::base_type<T>;
      TFEL_CONSTEXPR const base one_half = 1 / base(2);
      return stensor<2u, T2>{(s(3) * s(3) + 2 * s(0) * s(0)) * one_half,
                             (s(3) * s(3) + 2 * s(1) * s(1)) * one_half,
                             s(2) * s(2), (s(1) + s(0)) * s(3)};
    }

    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond &&
            StensorTraits<StensorType>::dime == 3u,
        stensor<3u,
                typename ComputeBinaryResult<StensorNumType<StensorType>,
                                             StensorNumType<StensorType>,
                                             OpMult>::Result>>::type
    square(const StensorType& s) {
      using T = StensorNumType<StensorType>;
      using T2 = typename ComputeBinaryResult<T, T, OpMult>::Result;
      using base = tfel::typetraits::base_type<T>;
      typedef StensorNumType<StensorType> T;
      constexpr const base cste = Cste<base>::sqrt2;
      TFEL_CONSTEXPR const base one_half = 1 / (base(2));
      return stensor<3u, T2>{
          (s(4) * s(4) + s(3) * s(3) + 2 * s(0) * s(0)) * one_half,
          (s(5) * s(5) + s(3) * s(3) + 2 * s(1) * s(1)) * one_half,
          (s(5) * s(5) + s(4) * s(4) + 2 * s(2) * s(2)) * one_half,
          (cste * s(4) * s(5) + 2 * (s(1) + s(0)) * s(3)) * one_half,
          (cste * s(3) * s(5) + 2 * (s(2) + s(0)) * s(4)) * one_half,
          (2 * (s(2) + s(1)) * s(5) + cste * s(3) * s(4)) * one_half};
    }

    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        stensor<StensorTraits<StensorType>::dime,
                StensorNumType<StensorType>>>::type
    change_basis(
        const StensorType& s,
        const tmatrix<3u,
                      3u,
                      tfel::typetraits::base_type<StensorNumType<StensorType>>>&
            r) {
      stensor<StensorTraits<StensorType>::dime, StensorNumType<StensorType>> ns{
          s};
      ns.changeBasis(r);
      return ns;
    }

    template <typename stensor_common::EigenSolver es,
              typename Function,
              typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        stensor<StensorTraits<StensorType>::dime,
                StensorNumType<StensorType>>>::type
    computeIsotropicFunction(const Function& f,
                             const StensorType& s,
                             const bool b) {
      return s.template computeIsotropicFunction<es>(f, b);
    }

    template <typename stensor_common::EigenSolver es,
              typename Function,
              typename FunctionDerivative,
              typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        st2tost2<StensorTraits<StensorType>::dime,
                 StensorNumType<StensorType>>>::type
    computeIsotropicFunctionDerivative(const Function& f,
                                       const FunctionDerivative& df,
                                       const StensorType& s,
                                       const StensorNumType<StensorType> eps,
                                       const bool b) {
      return s.template computeIsotropicFunctionDerivative<es>(f, df, eps, b);
    }

    template <typename stensor_common::EigenSolver es,
              typename Function,
              typename FunctionDerivative,
              typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        std::pair<stensor<StensorTraits<StensorType>::dime,
                          StensorNumType<StensorType>>,
                  st2tost2<StensorTraits<StensorType>::dime,
                           StensorNumType<StensorType>>>>::type
    computeIsotropicFunctionDerivative(const Function& f,
                                       const FunctionDerivative& df,
                                       const StensorType& s,
                                       const StensorNumType<StensorType> eps,
                                       const bool b) {
      return s.template computeIsotropicFunctionDerivative<es>(f, df, eps, b);
    }

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 1u) &&
         (tfel::meta::Implements<T2, StensorConcept>::cond) &&
         (StensorTraits<T2>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<T2>>::cond)),
        stensor<1u, StensorNumType<T>>>::type
    convertCorotationnalCauchyStressToSecondPiolaKirchhoffStress(const T& s,
                                                                 const T2& U) {
      const auto J = det(U);
      return {J * s[0] / (U[0] * U[0]), J * s[1] / (U[1] * U[1]),
              J * s[2] / (U[2] * U[2])};
    }

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 2u) &&
         (tfel::meta::Implements<T2, StensorConcept>::cond) &&
         (StensorTraits<T2>::dime == 2u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<T2>>::cond)),
        stensor<2u, StensorNumType<T>>>::type
    convertCorotationnalCauchyStressToSecondPiolaKirchhoffStress(const T& s,
                                                                 const T2& U) {
      const auto J = det(U);
      const auto iU = invert(U);
      return {J *
                  (s[1] * iU[3] * iU[3] + 2 * s[3] * iU[0] * iU[3] +
                   2 * s[0] * iU[0] * iU[0]) /
                  2,
              J *
                  (s[0] * iU[3] * iU[3] + 2 * s[3] * iU[1] * iU[3] +
                   2 * s[1] * iU[1] * iU[1]) /
                  2,
              J * s[2] * iU[2] * iU[2],
              J *
                  (s[3] * iU[3] * iU[3] +
                   (2 * s[1] * iU[1] + 2 * s[0] * iU[0]) * iU[3] +
                   2 * s[3] * iU[0] * iU[1]) /
                  2};
    }

    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 3u) &&
         (tfel::meta::Implements<T2, StensorConcept>::cond) &&
         (StensorTraits<T2>::dime == 3u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<T2>>::cond)),
        stensor<3u, StensorNumType<T>>>::type
    convertCorotationnalCauchyStressToSecondPiolaKirchhoffStress(const T& s,
                                                                 const T2& U) {
      using real = tfel::typetraits::base_type<StensorNumType<T>>;
      constexpr real cste = Cste<real>::sqrt2;
      const auto J = det(U);
      const auto iU = invert(U);
      return {
          J *
              (s[2] * iU[4] * iU[4] +
               (cste * s[5] * iU[3] + 2 * s[4] * iU[0]) * iU[4] +
               s[1] * iU[3] * iU[3] + 2 * s[3] * iU[0] * iU[3] +
               2 * s[0] * iU[0] * iU[0]) /
              2,
          J *
              (s[2] * iU[5] * iU[5] +
               (cste * s[4] * iU[3] + 2 * s[5] * iU[1]) * iU[5] +
               s[0] * iU[3] * iU[3] + 2 * s[3] * iU[1] * iU[3] +
               2 * s[1] * iU[1] * iU[1]) /
              2,
          J *
              (s[1] * iU[5] * iU[5] +
               (cste * s[3] * iU[4] + 2 * s[5] * iU[2]) * iU[5] +
               s[0] * iU[4] * iU[4] + 2 * s[4] * iU[2] * iU[4] +
               2 * s[2] * iU[2] * iU[2]) /
              2,
          J *
              ((cste * s[2] * iU[4] + s[5] * iU[3] + cste * s[4] * iU[0]) *
                   iU[5] +
               (s[4] * iU[3] + cste * s[5] * iU[1]) * iU[4] +
               s[3] * iU[3] * iU[3] +
               (2 * s[1] * iU[1] + 2 * s[0] * iU[0]) * iU[3] +
               2 * s[3] * iU[0] * iU[1]) /
              2,
          J *
              ((s[5] * iU[4] + cste * s[1] * iU[3] + cste * s[3] * iU[0]) *
                   iU[5] +
               s[4] * iU[4] * iU[4] +
               (s[3] * iU[3] + 2 * s[2] * iU[2] + 2 * s[0] * iU[0]) * iU[4] +
               cste * s[5] * iU[2] * iU[3] + 2 * s[4] * iU[0] * iU[2]) /
              2,
          J *
              (s[5] * iU[5] * iU[5] +
               (s[4] * iU[4] + s[3] * iU[3] + 2 * s[2] * iU[2] +
                2 * s[1] * iU[1]) *
                   iU[5] +
               (cste * s[0] * iU[3] + cste * s[3] * iU[1]) * iU[4] +
               cste * s[4] * iU[2] * iU[3] + 2 * s[5] * iU[1] * iU[2]) /
              2};
    }
    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 1u) &&
         (tfel::meta::Implements<T2, StensorConcept>::cond) &&
         (StensorTraits<T2>::dime == 1u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<T2>>::cond)),
        stensor<1u, StensorNumType<T>>>::type
    convertSecondPiolaKirchhoffStressToCorotationnalCauchyStress(const T& S,
                                                                 const T2& U) {
      const auto iJ = 1 / det(U);
      return {iJ * U[0] * S[0] * U[0], iJ * U[1] * S[1] * U[1],
              iJ * U[2] * S[2] * U[2]};
    }
    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 2u) &&
         (tfel::meta::Implements<T2, StensorConcept>::cond) &&
         (StensorTraits<T2>::dime == 2u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<T2>>::cond)),
        stensor<2u, StensorNumType<T>>>::type
    convertSecondPiolaKirchhoffStressToCorotationnalCauchyStress(const T& S,
                                                                 const T2& U) {
      const auto iJ = 1 / det(U);
      return {
          iJ *
              (S[1] * U[3] * U[3] + 2 * S[3] * U[0] * U[3] +
               2 * S[0] * U[0] * U[0]) /
              2,
          iJ *
              (S[0] * U[3] * U[3] + 2 * S[3] * U[1] * U[3] +
               2 * S[1] * U[1] * U[1]) /
              2,
          iJ * S[2] * U[2] * U[2],
          iJ *
              (S[3] * U[3] * U[3] + (2 * S[1] * U[1] + 2 * S[0] * U[0]) * U[3] +
               2 * S[3] * U[0] * U[1]) /
              2};
    }
    template <typename T, typename T2>
    typename std::enable_if<
        ((tfel::meta::Implements<T, StensorConcept>::cond) &&
         (StensorTraits<T>::dime == 3u) &&
         (tfel::meta::Implements<T2, StensorConcept>::cond) &&
         (StensorTraits<T2>::dime == 3u) &&
         (tfel::typetraits::IsFundamentalNumericType<
             StensorNumType<T2>>::cond)),
        stensor<3u, StensorNumType<T>>>::type
    convertSecondPiolaKirchhoffStressToCorotationnalCauchyStress(const T& S,
                                                                 const T2& U) {
      using real = tfel::typetraits::base_type<StensorNumType<T>>;
      constexpr const auto cste = Cste<real>::sqrt2;
      const auto iJ = 1 / det(U);
      return {
          iJ *
              (S[2] * U[4] * U[4] +
               (cste * S[5] * U[3] + 2 * S[4] * U[0]) * U[4] +
               S[1] * U[3] * U[3] + 2 * S[3] * U[0] * U[3] +
               2 * S[0] * U[0] * U[0]) /
              2,
          iJ *
              (S[2] * U[5] * U[5] +
               (cste * S[4] * U[3] + 2 * S[5] * U[1]) * U[5] +
               S[0] * U[3] * U[3] + 2 * S[3] * U[1] * U[3] +
               2 * S[1] * U[1] * U[1]) /
              2,
          iJ *
              (S[1] * U[5] * U[5] +
               (cste * S[3] * U[4] + 2 * S[5] * U[2]) * U[5] +
               S[0] * U[4] * U[4] + 2 * S[4] * U[2] * U[4] +
               2 * S[2] * U[2] * U[2]) /
              2,
          iJ *
              ((cste * S[2] * U[4] + S[5] * U[3] + cste * S[4] * U[0]) * U[5] +
               (S[4] * U[3] + cste * S[5] * U[1]) * U[4] + S[3] * U[3] * U[3] +
               (2 * S[1] * U[1] + 2 * S[0] * U[0]) * U[3] +
               2 * S[3] * U[0] * U[1]) /
              2,
          iJ *
              ((S[5] * U[4] + cste * S[1] * U[3] + cste * S[3] * U[0]) * U[5] +
               S[4] * U[4] * U[4] +
               (S[3] * U[3] + 2 * S[2] * U[2] + 2 * S[0] * U[0]) * U[4] +
               cste * S[5] * U[2] * U[3] + 2 * S[4] * U[0] * U[2]) /
              2,
          iJ *
              (S[5] * U[5] * U[5] +
               (S[4] * U[4] + S[3] * U[3] + 2 * S[2] * U[2] + 2 * S[1] * U[1]) *
                   U[5] +
               (cste * S[0] * U[3] + cste * S[3] * U[1]) * U[4] +
               cste * S[4] * U[2] * U[3] + 2 * S[5] * U[1] * U[2]) /
              2};
    }
    template <typename StensorType1, typename StensorType2>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
         (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
         (StensorTraits<StensorType1>::dime == 1u) &&
         (StensorTraits<StensorType2>::dime == 1u)),
        stensor<3u,
                typename ResultType<StensorNumType<StensorType1>,
                                    StensorNumType<StensorType2>,
                                    OpMult>::type>>::type
    symmetric_product(const StensorType1& s1, const StensorType2& s2) {
      return {2 * s1[0] * s2[0], 2 * s1[1] * s2[1], 2 * s1[2] * s2[2]};
    }
    template <typename StensorType1, typename StensorType2>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
         (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
         (StensorTraits<StensorType1>::dime == 2u) &&
         (StensorTraits<StensorType2>::dime == 2u)),
        stensor<2u,
                typename ResultType<StensorNumType<StensorType1>,
                                    StensorNumType<StensorType2>,
                                    OpMult>::type>>::type
    symmetric_product(const StensorType1& s1, const StensorType2& s2) {
      return {2 * s1[0] * s2[0] + s1[3] * s2[3],
              2 * s1[1] * s2[1] + s1[3] * s2[3], 2 * s1[2] * s2[2],
              (s1[1] + s1[0]) * s2[3] + s1[3] * s2[1] + s1[3] * s2[0]};
    }
    template <typename StensorType1, typename StensorType2>
    typename std::enable_if<
        ((tfel::meta::Implements<StensorType1, StensorConcept>::cond) &&
         (tfel::meta::Implements<StensorType2, StensorConcept>::cond) &&
         (StensorTraits<StensorType1>::dime == 3u) &&
         (StensorTraits<StensorType2>::dime == 3u)),
        stensor<3u,
                typename ResultType<StensorNumType<StensorType1>,
                                    StensorNumType<StensorType2>,
                                    OpMult>::type>>::type
    symmetric_product(const StensorType1& s1, const StensorType2& s2) {
      using res =
          typename ResultType<StensorNumType<StensorType1>,
                              StensorNumType<StensorType2>, OpMult>::type;
      constexpr const auto icste = Cste<res>::isqrt2;
      return {s1[4] * s2[4] + s1[3] * s2[3] + 2 * s1[0] * s2[0],
              s1[5] * s2[5] + s1[3] * s2[3] + 2 * s1[1] * s2[1],
              s1[5] * s2[5] + s1[4] * s2[4] + 2 * s1[2] * s2[2],
              icste * s1[4] * s2[5] + icste * s1[5] * s2[4] +
                  (s1[1] + s1[0]) * s2[3] + s1[3] * s2[1] + s1[3] * s2[0],
              icste * s1[3] * s2[5] + (s1[2] + s1[0]) * s2[4] +
                  icste * s1[5] * s2[3] + s1[4] * s2[2] + s1[4] * s2[0],
              (s1[2] + s1[1]) * s2[5] + icste * s1[3] * s2[4] +
                  icste * s1[4] * s2[3] + s1[5] * s2[2] + s1[5] * s2[1]};
    }

    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        stensor<StensorTraits<StensorType>::dime,
                typename ComputeUnaryResult<StensorNumType<StensorType>,
                                            Power<2>>::Result>>::type
    computeDeterminantDerivative(const StensorType& s) {
      stensor<StensorTraits<StensorType>::dime,
              typename ComputeUnaryResult<StensorNumType<StensorType>,
                                          Power<2>>::Result>
          dJ;
      computeDeterminantDerivative(dJ, s);
      return dJ;
    }

    template <typename StensorType>
    typename std::enable_if<
        tfel::meta::Implements<StensorType, StensorConcept>::cond,
        stensor<StensorTraits<StensorType>::dime,
                typename ComputeUnaryResult<StensorNumType<StensorType>,
                                            Power<2>>::Result>>::type
    computeDeviatorDeterminantDerivative(const StensorType& s) {
      stensor<StensorTraits<StensorType>::dime,
              typename ComputeUnaryResult<StensorNumType<StensorType>,
                                          Power<2>>::Result>
          dJ;
      computeDeviatorDeterminantDerivative(dJ, s);
      return dJ;
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_STENSOR_IXX */
