/*!
 * \file   AndersonBase.ixx
 * \brief
 * \author Étienne Castelier
 * \date   10 oct. 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ANDERSONBASE_IXX
#define LIB_TFEL_MATH_ANDERSONBASE_IXX

#include <cmath>
#include <stdexcept>
#include <algorithm>

namespace tfel {

  namespace math {

    // Default constructor
    // grid: Grid
    // flags: Plane construction method
    template <typename Field, typename real>
    AndersonBase<Field, real>::AndersonBase(const Allocator a) : ma(a), n(0) {}

    template <typename Field, typename real>
    const std::vector<Field*>& AndersonBase<Field, real>::getU() const {
      return this->u;
    }

    template <typename Field, typename real>
    const std::vector<Field*>& AndersonBase<Field, real>::getD() const {
      return this->D;
    }

    // Set Anderson algorithm
    // at Anderson acceleration type
    // Nmax Method order
    // alMax Alternance order (0 = optimal)
    template <typename Field, typename real>
    void AndersonBase<Field, real>::setAnderson(const size_type Nmax_i,
                                                const size_type alMax_i) {
      if ((!this->w.empty()) || (this->alMax != 0)) {
        throw(
            std::runtime_error("Anderson<Field,real>::setAnderson: "
                               "parameters already set"));
      }
      this->w.clear();
      this->cM.resize(Nmax_i);
      this->w.resize(Nmax_i);
      this->alMax = alMax_i;
    }

    template <typename Field, typename real>
    typename AndersonBase<Field, real>::size_type
    AndersonBase<Field, real>::size() const {
      return static_cast<size_type>(this->w.size());
    }  // end of AndersonBase<Field,real>::size()

    // Memory allocation
    template <typename Field, typename real>
    void AndersonBase<Field, real>::alloc() {
      // Memory allocation
      if (this->u.empty()) {
        this->u.resize(this->size());
        this->D.resize(this->size());
        auto iu = u.begin();
        auto iF = D.begin();
        for (; iu != u.end(); ++iu, ++iF) {
          *iu = this->ma();
          *iF = this->ma();
        }
        this->n = 0;
      }
      // Shift
      if (this->n) {
        std::swap(this->u[0], this->u[n - 1]);
        std::swap(this->D[0], this->D[n - 1]);
        this->n = 0;
      }
    }

    // Reseting the Covariance matrix with a new vector
    template <typename Field, typename real>
    void AndersonBase<Field, real>::reset() {
      size_type i = n - 1;
      auto Cij = cM.getLine(i);
      for (size_type j = 0; j <= i; ++j, ++Cij) {
        *Cij = anderson::scalar_product(*D[i], *D[j]);
      }
    }

    // Destructor
    template <typename Field, typename real>
    AndersonBase<Field, real>::~AndersonBase() {
      for (const auto& f : this->u) {
        delete (f);
      }
      for (const auto& f : this->D) {
        delete (f);
      }
    }

    namespace anderson {

      template <typename Field, typename real>
      void linear_combinaison(Field& u,
                              const std::vector<Field*>& fields,
                              const std::vector<real>& w,
                              const unsigned char n) {
        using value_type = typename Field::value_type;
        for (decltype(u.size()) i = 0; i != u.size(); ++i) {
          auto v = value_type{};
          for (decltype(fields.size()) j = 0; j != n; ++j) {
            const auto& f = *(fields[j]);
            v += f[i] * w[j];
          }
          u[i] = v;
        }
      }

      template <typename Field>
      auto scalar_product(const Field& f1, const Field& f2)
          -> decltype(f1[0] * f2[0]) {
        using value_type = typename Field::value_type;
        auto r = value_type{};
        for (decltype(f1.size()) i = 0; i != f1.size(); ++i) {
          r += f1[i] * f2[i];
        }
        return r;
      }

    }  // end of namespace anderson

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_ANDERSONBASE_IXX */
