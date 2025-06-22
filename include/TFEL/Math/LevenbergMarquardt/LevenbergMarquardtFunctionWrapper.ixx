/*!
 * \file
 * include/TFEL/Math/LevenbergMarquardt/LevenbergMarquardtFunctionWrapper.ixx
 * \brief
 * \author Thomas Helfer
 * \date 24/02/2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_LEVENBERGMARQUARDTFUNCTIONWRAPPER_IXX
#define LIB_TFEL_MATH_LEVENBERGMARQUARDTFUNCTIONWRAPPER_IXX

namespace tfel {

  namespace math {

    template <typename T>
    LevenbergMarquardtFunctionWrapper<T>::LevenbergMarquardtFunctionWrapper(
        const typename LevenbergMarquardtFunctionWrapper<T>::size_type nv_,
        const typename LevenbergMarquardtFunctionWrapper<T>::size_type np_,
        const typename LevenbergMarquardtFunctionWrapper<T>::PtrFun f_)
        : nv(nv_),
          np(np_),
          f(f_) {
    }  // end of
       // LevenbergMarquardtFunctionWrapper::LevenbergMarquardtFunctionWrapper<T>

    template <typename T>
    LevenbergMarquardtFunctionWrapper<T>::LevenbergMarquardtFunctionWrapper(
        const LevenbergMarquardtFunctionWrapper<T>& src)
        : nv(src.nv),
          np(src.np),
          f(src.f) {
    }  // end of
       // LevenbergMarquardtFunctionWrapper<T>::LevenbergMarquardtFunctionWrapper

    template <typename T>
    LevenbergMarquardtFunctionWrapper<T>&
    LevenbergMarquardtFunctionWrapper<T>::operator=(
        const LevenbergMarquardtFunctionWrapper<T>& src) {
      this->f = src.f;
      this->np = src.np;
      this->nv = src.nv;
      return *this;
    }  // end of LevenbergMarquardtFunctionWrapper<T>::operator=

    template <typename T>
    typename LevenbergMarquardtFunctionWrapper<T>::size_type
    LevenbergMarquardtFunctionWrapper<T>::getNumberOfVariables() const {
      return this->nv;
    }  // end of LevenbergMarquardtFunctionWrapper<T>::getNumberOfVariables()

    template <typename T>
    typename LevenbergMarquardtFunctionWrapper<T>::size_type
    LevenbergMarquardtFunctionWrapper<T>::getNumberOfParameters() const {
      return this->np;
    }  // end of LevenbergMarquardtFunctionWrapper<T>::getNumberOfParameters()

    template <typename T>
    void LevenbergMarquardtFunctionWrapper<T>::operator()(
        T& r,
        tfel::math::vector<T>& g,
        const tfel::math::vector<T>& v,
        const tfel::math::vector<T>& p) const {
      (*(this->f))(r, g, v, p);
    }  // end of LevenbergMarquardtFunctionWrapper<T>::operator()

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_LEVENBERGMARQUARDTFUNCTIONWRAPPER_IXX */
