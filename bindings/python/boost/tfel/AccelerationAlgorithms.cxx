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
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include "TFEL/Raise.hxx"
#include "TFEL/Math/vector.hxx"
#include "TFEL/Numpy/ndarray.hxx"
#include "TFEL/Math/AccelerationAlgorithms/UAnderson.hxx"
#include "TFEL/Math/AccelerationAlgorithms/FAnderson.hxx"

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
  void initialize(const boost::python::numpy::ndarray& u) {
    const auto s = tfel::numpy::get_size(u);
    if (!this->a) {
      this->a = std::unique_ptr<UAnderson>(
          new UAnderson([s] { return new tfel::math::vector<double>(s); }));
      this->a->setAnderson(this->nfields, this->period);
    }
    this->a->restart(this->uO, this->uN);
    const auto* const b = tfel::numpy::get_data(u);
    *(this->uO) = tfel::math::vector<double>(b, b + s);
  }
  /*!
   * \brief must be called at the beginning of each time step
   * \param[in] u1: current estimate
   */
  void accelerate(boost::python::numpy::ndarray& u1) {
    if (!this->a) {
      tfel::raise(
          "UAndersonAccelerationAlgorithm::accelerate: "
          "the `initialize` method has not been called");
    }
    const auto s = tfel::numpy::get_size(u1);
    auto* b = tfel::numpy::get_data(u1);
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
  void initialize(const boost::python::numpy::ndarray& u) {
    const auto s = tfel::numpy::get_size(u);
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
  void accelerate(boost::python::numpy::ndarray& u1,
                  const boost::python::numpy::ndarray& r) {
    if (!this->a) {
      tfel::raise(
          "FAndersonAccelerationAlgorithm::accelerate: "
          "the `initialize` method has not been called");
    }
    const auto s = tfel::numpy::get_size(u1);
    if ((s != tfel::numpy::get_size(r)) || (this->uO->size() != s) ||
        (this->uN->size() != s)) {
      tfel::raise(
          "FAndersonAccelerationAlgorithm::accelerate: "
          "unmatched sizes");
    }
    auto* b = tfel::numpy::get_data(u1);
    auto* br = tfel::numpy::get_data(r);
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

void declareAccelerationAlgorithms();

void declareAccelerationAlgorithms() {
  using size_type = UAndersonAccelerationAlgorithm::size_type;
  boost::python::class_<UAndersonAccelerationAlgorithm, boost::noncopyable>(
      "UAnderson", boost::python::init<size_type, size_type>())
      .def("initialize", &UAndersonAccelerationAlgorithm::initialize)
      .def("accelerate", &UAndersonAccelerationAlgorithm::accelerate);
  boost::python::class_<FAndersonAccelerationAlgorithm, boost::noncopyable>(
      "FAnderson", boost::python::init<size_type, size_type>())
      .def("initialize", &FAndersonAccelerationAlgorithm::initialize)
      .def("accelerate", &FAndersonAccelerationAlgorithm::accelerate);
}  // end of declareAccelerationAlgorithms
