/*!
 * \file tfel-check/src/Test.cxx
 * \date 29/05/2013
 * \author Remy Petkantchin
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <sstream>
#include <algorithm>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Check/Curve.hxx"
#include "TFEL/Check/AreaComparison.hxx"
#include "TFEL/Check/Test.hxx"

namespace tfel::check {

  //! \return true if the comparison integrates the difference of the curves
  static bool isIntegralComparison(const Comparison& c) {
    return dynamic_cast<const AreaComparison*>(&c) != nullptr;
  }

  Test::Test() = default;
  Test::Test(Test&&) noexcept = default;
  Test::Test(const Test&) = default;
  Test& Test::operator=(Test&&) noexcept = default;
  Test& Test::operator=(const Test&) = default;
  Test::~Test() = default;

  void Test::compare() {
    this->comparison->setParameters(
        this->c1, this->c2, this->prec, this->precision2, this->ci,
        this->interpolation->getType(), this->interpolation->isConform(),
        this->colIntegralInterpolated, this->integralInterpolation);

    this->comparison->compare();
  }

  bool Test::hasSucceed() const { return this->comparison->hasSucceed(); }

  std::string Test::getMsgLog() {
    return this->comparison->getMsgLog() + "\n" + this->msgLog;
  }

  std::shared_ptr<Comparison> Test::getComparison() const {
    return this->comparison;
  }

  void Test::setComparison(const std::shared_ptr<Comparison>& c) {
    this->comparison = c;
  }

  bool Test::isAllowLessResults() const { return this->allowLessResults; }

  void Test::setAllowLessResults(const bool b) { this->allowLessResults = b; }

  void Test::setColA(const std::shared_ptr<Column>& c) { this->c1 = c; }

  void Test::setColB(const std::shared_ptr<Column>& c) { this->c2 = c; }

  void Test::setColInterpolated(const std::shared_ptr<Column>& c) {
    this->ci = c;
  }

  const std::string& Test::getFileA() const { return this->f1; }

  void Test::setFileA(const std::string& f) { this->f1 = f; }

  const std::string& Test::getFileB() const { return this->f2; }

  void Test::setFileB(const std::string& f) { this->f2 = f; }

  const std::shared_ptr<Interpolation>& Test::getInterpolation() const {
    return this->interpolation;
  }

  void Test::setInterpolation(const std::shared_ptr<Interpolation>& i) {
    this->interpolation = i;
  }

  double Test::getPrec() const { return this->prec; }

  void Test::setPrec(const double v) { this->prec = v; }

  double Test::getPrecision2() const { return this->precision2; }

  void Test::setPrecision2(const double v) { this->precision2 = v; }

  const std::shared_ptr<Column>& Test::getColIntegralInterpolated() const {
    return this->colIntegralInterpolated;
  }

  void Test::setColIntegralInterpolated(const std::shared_ptr<Column>& c) {
    this->colIntegralInterpolated = c;
  }

  const std::shared_ptr<Interpolation>& Test::getIntegralInterpolation() const {
    return this->integralInterpolation;
  }

  void Test::setIntegralInterpolation(const std::shared_ptr<Interpolation>& i) {
    this->integralInterpolation = i;
  }

  const std::shared_ptr<Column>& Test::getColA() const { return this->c1; }

  const std::shared_ptr<Column>& Test::getColB() const { return this->c2; }

  void Test::applyInterpolation() {
    using size_type = std::vector<double>::size_type;

    this->c1->setFilename(this->f1);
    this->c2->setFilename(this->f2);

    std::ostringstream msg;
    msg.str("");  // wipe stream

    auto getAbscissas = [this](const std::string& f) {
      auto x = Column(*(this->ci));
      x.setFilename(f);
      return x.getValues();
    };
    if (!(this->interpolation->isConform()) &&
        !isIntegralComparison(*(this->comparison))) {
      // tests if we can interpolate or not, based on columns' size and type
      // of interpolation
      if (this->c1->getValues().size() != this->c2->getValues().size()) {
        msg << "FAILED : ";
        msg << "columns ";
        msg << this->c1->getName();
        msg << " of files '" << this->c1->getFilename() << "' and '"
            << this->c2->getFilename()
            << "' don't have the same size, and interpolation is of type '"
            << this->interpolation->getType() << "' ("
            << this->c1->getValues().size() << " vs "
            << this->c2->getValues().size() << ")";
        this->msgLog.append(msg.str());
        raise(msg.str());
      }
      // if an abscissa column is given, the abscissas of both files must
      // be the same, up to a relative tolerance of 1e-10
      if (this->ci != nullptr) {
        const auto x1 = getAbscissas(this->f1);
        const auto x2 = getAbscissas(this->f2);
        raise_if(x1.size() != x2.size(),
                 "Test::applyInterpolation: the abscissa columns of files '" +
                     this->f1 + "' and '" + this->f2 +
                     "' do not have the same size");
        auto xmax = double{0};
        for (const auto x : x1) {
          xmax = std::max(xmax, std::abs(x));
        }
        const auto eps = 1.e-10 * xmax;
        for (size_type i = 0; i != x1.size(); ++i) {
          // written this way so that a NaN fails
          raise_if(!(std::abs(x1[i] - x2[i]) <= eps),
                   "Test::applyInterpolation: the abscissas of files '" +
                       this->f1 + "' and '" + this->f2 + "' differ (" +
                       tfel::utilities::convert(x1[i]) + " and " +
                       tfel::utilities::convert(x2[i]) + " for the value " +
                       std::to_string(i + 1) +
                       "), an interpolation is required");
        }
      }
    } else if (this->interpolation->isConform() &&
               !isIntegralComparison(*(this->comparison))) {
      raise_if(this->ci == nullptr,
               "Test::applyInterpolation: no abscissa column defined");
      auto setValues = [](Column& c, const std::vector<double>& v) {
        c.clearValues();
        c.resizeValues(v.size());
        for (size_type i = 0; i != v.size(); ++i) {
          c.setValue(static_cast<unsigned>(i), v[i]);
        }
      };
      const auto x1 = getAbscissas(this->f1);
      const auto x2 = getAbscissas(this->f2);
      // both files must have values
      const auto ca = buildCurve(this->f1, x1, this->c1->getValues());
      const auto cb = buildCurve(this->f2, x2, this->c2->getValues());
      // a NaN shall make the test fail, even if it lies between two
      // abscissas at which the curve is evaluated
      auto checkNaN = [](const std::string& f, const std::vector<double>& v) {
        raise_if(std::any_of(v.begin(), v.end(),
                             [](const double y) { return std::isnan(y); }),
                 "Test::applyInterpolation: NaN value in file '" + f + "'");
      };
      checkNaN(this->f1, this->c1->getValues());
      checkNaN(this->f2, this->c2->getValues());
      // the choice is based on the number of distinct abscissas
      if (ca.abscissas.size() <= cb.abscissas.size()) {
        // the values of the second file are interpolated at the
        // abscissas of the first one, without extrapolation
        raise_if(!covers(cb, x1),
                 "Test::applyInterpolation: the abscissas of file '" +
                     this->f2 + "' do not cover the ones of file '" + this->f1 +
                     "'");
        setValues(*(this->c2), evaluateCurve(*(this->interpolation), cb, x1));
      } else if (this->allowLessResults) {
        // the values of the first file are interpolated at the
        // abscissas of the second one, without extrapolation. The second
        // file must still reach the bounds of the first one: results
        // stopping early shall not pass
        raise_if(!covers(cb, {ca.abscissas.front(), ca.abscissas.back()}),
                 "Test::applyInterpolation: the abscissas of file '" +
                     this->f2 +
                     "' do not reach the bounds of the ones of "
                     "file '" +
                     this->f1 + "'");
        raise_if(!covers(ca, x2),
                 "Test::applyInterpolation: the abscissas of file '" +
                     this->f1 + "' do not cover the ones of file '" + this->f2 +
                     "'");
        setValues(*(this->c1), evaluateCurve(*(this->interpolation), ca, x2));
      } else {
        msg << "\nResults have less values than reference.\n";
        this->msgLog.append(msg.str());
        raise(msg.str());
      }
    }  // else if (this->interpolation->isConform())
  }

}  // end of namespace tfel::check
