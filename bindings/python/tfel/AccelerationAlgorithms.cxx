/*!
 * \file   AccelerationAlgorithms.cxx
 * \brief
 * \author Thomas Helfer
 * \date   23/03/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <vector>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "TFEL/Raise.hxx"
#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/AccelerationAlgorithms/UAnderson.hxx"
#include "TFEL/Math/AccelerationAlgorithms/FAnderson.hxx"

static void check(const pybind11::array_t<double>& a) {
  const auto info = a.request();
  /* Some basic validation checks ... */
  if (info.format != pybind11::format_descriptor<double>::format()) {
    tfel::raise("incompatible format: expected a double array!");
  }
  if (info.ndim != 1) {
    tfel::raise("incompatible buffer dimension!");
  }
}

static auto size(const pybind11::array_t<double>& a) {
  check(a);
  const auto info = a.request();
  return static_cast<tfel::math::vector<double>::size_type>(info.shape[0]);
}

static auto data(const pybind11::array_t<double>& a) {
  check(a);
  const auto info = a.request();
  return static_cast<const double*>(info.ptr);
}

static auto data(pybind11::array_t<double>& a) {
  check(a);
  const auto info = a.request();
  return static_cast<double*>(info.ptr);
}

/*!
 * \brief a simple wrapper around the UAnderson class.
 */
struct UAndersonAccelerationAlgorithm {
  //! a simple alias
  using Field = tfel::math::vector<double>;
  //! a simple alias
  using UAnderson = tfel::math::UAnderson<Field, long double>;
  //! a simple alias
  using size_type = Field::size_type;
  /*!
   * \brief constructor
   * \param[in] s: field size
   * \param[in] n: the number of fields used for the acceleration.
   * \param[in] p: period of acceleration
   */
  UAndersonAccelerationAlgorithm(const size_type n, const size_type p)
      : nfields(n), period(p) {}  // end of UAndersonAccelerationAlgorithm
  /*!
   * \brief must be called at the beginning of each time step
   */
  void initialize(const pybind11::array_t<double>& u) {
    check(u);
    const auto s = size(u);
    if (!this->a) {
      this->a = std::unique_ptr<UAnderson>(
          new UAnderson([s] { return new tfel::math::vector<double>(s); }));
      this->a->setAnderson(this->nfields, this->period);
    }
    this->a->restart(this->uO, this->uN);
    const auto* const b = data(u);
    *(this->uO) = tfel::math::vector<double>(b, b + s);
  }
  /*!
   * \brief must be called at the beginning of each time step
   * \param[in] u1: current estimate
   */
  void accelerate(pybind11::array_t<double>& u1) {
    if (!this->a) {
      tfel::raise(
          "UAndersonAccelerationAlgorithm::accelerate: "
          "the `initialize` method has not been called");
    }
    const auto s = size(u1);
    auto* b = data(u1);
    if ((this->uO->size() != s) || (this->uN->size() != s)) {
      tfel::raise(
          "UAndersonAccelerationAlgorithm::accelerate: "
          "unmatched sizes");
    }
    *(this->uN) = tfel::math::vector<double>(b, b + s);
    this->a->newIter(this->uO, this->uN);
    std::copy(this->uO->begin(), this->uO->end(), b);
  }  // end of accelerate

 private:
  //! the underlying algorithm
  std::unique_ptr<UAnderson> a;
  /* pointer to the internal fields */
  //!< previous displacement estimate
  Field* uO = nullptr;
  //!< current displacement estimate
  Field* uN = nullptr;
  //! number of fields
  const size_type nfields;
  //! acceleration period
  const size_type period;
};  // end of UAndersonAccelerationAlgorithm

/*!
 * \brief a simple wrapper around the FAnderson class.
 */
struct FAndersonAccelerationAlgorithm {
  //! a simple alias
  using Field = tfel::math::vector<double>;
  //! a simple alias
  using FAnderson = tfel::math::FAnderson<Field, long double>;
  //! a simple alias
  using size_type = Field::size_type;
  /*!
   * \brief constructor
   * \param[in] s: field size
   * \param[in] n: the number of fields used for the acceleration.
   * \param[in] p: period of acceleration
   */
  FAndersonAccelerationAlgorithm(const size_type n, const size_type p)
      : nfields(n), period(p) {}  // end of FAndersonAccelerationAlgorithm
  /*!
   * \brief must be called at the beginning of each time step
   */
  void initialize(const pybind11::array_t<double>& u) {
    const auto s = size(u);
    if (!this->a) {
      this->a = std::unique_ptr<FAnderson>(
          new FAnderson([s] { return new tfel::math::vector<double>(s); }));
      this->a->setAnderson(this->nfields, this->period);
    }
    this->a->restart(this->uO, this->uN, this->Df);
  }
  /*!
   * \brief must be called at the beginning of each time step
   * \param[in] u1: current estimate
   */
  void accelerate(pybind11::array_t<double>& u1,
                  const pybind11::array_t<double>& r) {
    if (!this->a) {
      tfel::raise(
          "FAndersonAccelerationAlgorithm::accelerate: "
          "the `initialize` method has not been called");
    }
    const auto s = size(u1);
    if ((s != size(r)) || (this->uO->size() != s) || (this->uN->size() != s)) {
      tfel::raise(
          "FAndersonAccelerationAlgorithm::accelerate: "
          "unmatched sizes");
    }
    auto* b = data(u1);
    auto* br = data(r);
    *(this->uN) = tfel::math::vector<double>(b, b + s);
    *(this->Df) = tfel::math::vector<double>(br, br + s);
    this->a->newIter(this->uO, this->uN, this->Df);
    std::copy(this->uO->begin(), this->uO->end(), b);
  }  // end of accelerate

 private:
  //! the underlying algorithm
  std::unique_ptr<FAnderson> a;
  /* pointer to the internal fields */
  //!< previous displacement estimate
  Field* uO = nullptr;
  //!< current displacement estimate
  Field* uN = nullptr;
  //! current residual
  Field* Df = nullptr;
  //! number of fields
  const size_type nfields;
  //! acceleration period
  const size_type period;
};  // end of FAndersonAccelerationAlgorithm

void declareAccelerationAlgorithms(pybind11::module_&);

void declareAccelerationAlgorithms(pybind11::module_& m) {
  using size_type = UAndersonAccelerationAlgorithm::size_type;
  pybind11::class_<UAndersonAccelerationAlgorithm>(m, "UAnderson")
      .def(pybind11::init<size_type, size_type>())
      .def("initialize", &UAndersonAccelerationAlgorithm::initialize)
      .def("accelerate", &UAndersonAccelerationAlgorithm::accelerate);
  pybind11::class_<FAndersonAccelerationAlgorithm>(m, "FAnderson")
      .def(pybind11::init<size_type, size_type>())
      .def("initialize", &FAndersonAccelerationAlgorithm::initialize)
      .def("accelerate", &FAndersonAccelerationAlgorithm::accelerate);
}  // end of declareAccelerationAlgorithms
