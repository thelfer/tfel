/*!
 * \file  include/TFEL/Math/FactorizedKriging.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 12 avr 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FACTORIZEDKRIGING_HXX
#define LIB_TFEL_MATH_FACTORIZEDKRIGING_HXX

#include "TFEL/Math/Kriging.hxx"

namespace tfel::math {

  template <typename Model>
  struct KrigingModelAdaptator : public Model {
    typedef typename Model::Drifts Drifts;
    static constexpr unsigned short nb = Model::nb - 1u; /* number of drifts */
    static const Drifts* drifts;
  };

  template <typename Model>
  const typename KrigingModelAdaptator<Model>::Drifts*
      KrigingModelAdaptator<Model>::drifts = Model::drifts + 1u;

  template <unsigned short N,
            unsigned short M,
            typename T = double,
            typename Model1 = KrigingDefaultModel<N, T>,
            typename Model2 = KrigingModelAdaptator<KrigingDefaultModel<M, T>>>
  struct FactorizedKriging {
    FactorizedKriging() = default;

    FactorizedKriging(const Model1&, const Model2&);

    void addValue(const typename KrigingVariable<N, T>::type&,
                  const typename KrigingVariable<M, T>::type&,
                  const T&);

    void buildInterpolation();

    T operator()(const typename KrigingVariable<N, T>::type&,
                 const typename KrigingVariable<M, T>::type&) const;

    ~FactorizedKriging() noexcept;

   private:
    Model1 m1;
    Model2 m2;

    tfel::math::vector<typename KrigingVariable<N, T>::type> x1;
    tfel::math::vector<typename KrigingVariable<M, T>::type> x2;
    tfel::math::vector<T> f;
    tfel::math::vector<T> a;

  };  // end of struct FactorizedKriging

}  // end of namespace tfel::math

#include "TFEL/Math/Kriging/FactorizedKriging.ixx"

#endif /* LIB_TFEL_MATH_FACTORIZEDKRIGING_HXX */
