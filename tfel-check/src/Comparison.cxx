/*!
 * Comparison.cxx
 *
 *  Created on: 28 mai 2013
 *      Author: rp238441
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <sstream>

#include "TFELCheck/Comparison.hxx"

namespace tfel_check {

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

  Comparison::~Comparison() = default;

} /* namespace tfel_check */
