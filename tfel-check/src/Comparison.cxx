/*
 * \file tfel-check/src/Comparison.cxx
 *
 * \date 12/06/2013
 * \author Remy Petkantchin
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <sstream>

#include "TFEL/Check/Comparison.hxx"

namespace tfel::check {

  Comparison::Comparison()
      : success(true),
        prec(0.),
        precision2(0.),
        interpolationIsConform(false) {}

  Comparison::Comparison(const std::shared_ptr<Column> c1_,
                         const std::shared_ptr<Column> c2_,
                         const double prec_,
                         const double precision2_,
                         const std::shared_ptr<Column> ci_,
                         const std::string it_,
                         const bool ic_,
                         const std::shared_ptr<Column> cii_,
                         const std::shared_ptr<Interpolation> ii_)
      : success(true),
        c1(c1_),
        c2(c2_),
        prec(prec_),
        precision2(precision2_),
        ci(ci_),
        interpolationType(it_),
        interpolationIsConform(ic_),
        colIntegralInterpolated(cii_),
        integralInterpolation(ii_) {}

  bool Comparison::hasSucceed() const { return this->success; }

  std::string Comparison::getMsgLog() const { return this->msgLog; }

  void Comparison::setParameters(const std::shared_ptr<Column> c1_,
                                 const std::shared_ptr<Column> c2_,
                                 const double prec_,
                                 const double precision2_,
                                 const std::shared_ptr<Column> ci_,
                                 const std::string it_,
                                 const bool ic_,
                                 const std::shared_ptr<Column> cii_,
                                 const std::shared_ptr<Interpolation> ii_) {
    // the same comparison is used by all the tests following a
    // `@TestType` keyword: the result of a previous test is discarded
    this->success = true;
    this->msgLog.clear();
    this->c1 = c1_;
    this->c2 = c2_;
    this->prec = prec_;
    this->precision2 = precision2_;
    this->ci = ci_;
    this->interpolationType = it_;
    this->interpolationIsConform = ic_;
    this->colIntegralInterpolated = cii_;
    this->integralInterpolation = ii_;
  }

  const std::string& Comparison::getName() const { return this->name; }

  std::string Comparison::getLocation(
      const std::optional<std::size_t>& i) const {
    if (!i.has_value()) {
      return "";
    }
    auto getLine = [&i](Column& c) -> std::optional<std::size_t> {
      const auto& l = c.getLineNumbers();
      if ((l.size() != c.getValues().size()) || (*i >= l.size())) {
        return {};
      }
      return l[*i];
    };
    const auto l1 = getLine(*(this->c1));
    const auto l2 = getLine(*(this->c2));
    const auto& f1 = this->c1->getFilename();
    const auto& f2 = this->c2->getFilename();
    if (l1.has_value() && l2.has_value()) {
      if (*l1 == *l2) {
        return " at line " + std::to_string(*l1) + " of files '" + f1 +
               "' and '" + f2 + "'";
      }
      return " at line " + std::to_string(*l1) + " of file '" + f1 +
             "' and line " + std::to_string(*l2) + " of file '" + f2 + "'";
    }
    if (l1.has_value()) {
      return " at line " + std::to_string(*l1) + " of file '" + f1 + "'";
    }
    if (l2.has_value()) {
      return " at line " + std::to_string(*l2) + " of file '" + f2 + "'";
    }
    return " at value " + std::to_string(*i + 1);
  }  // end of getLocation

  Comparison::~Comparison() = default;

}  // end of namespace tfel::check
