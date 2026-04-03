/*!
 * \file tfel-check/src/Column.cxx
 *
 *  Created on: 23 mai 2013
 *      Author: rp238441
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/Math/Evaluator.hxx"
#include "TFEL/Check/Column.hxx"

namespace tfel::check {

  static std::vector<double> eval(const tfel::utilities::TextData& d,
                                  const std::string& f) {
    struct Variable {
      unsigned short p;
      std::vector<double> values;
    };
    auto matches = [](const std::string& vn) {
      if (vn.size() < 2) {
        return false;
      }
      if (vn[0] != '$') {
        return false;
      }
      auto p = std::begin(vn) + 1;
      for (; p != vn.end(); ++p) {
        const char c = *p;
        if (!((c >= '0') && (c <= '9'))) {
          return false;
        }
      }
      return true;
    };
    auto convert = [](const std::string& vn) {
      auto p = std::begin(vn) + 1;
      unsigned short value{0};
      for (; p != vn.end(); ++p) {
        const char c = *p;
        raise_if(!((c >= '0') && (c <= '9')),
                 "tfel::check::eval::convert: "
                 "invalid input '" +
                     vn + "'");
        value *= 10;
        value += c - '0';
      }
      return value;
    };
    tfel::math::Evaluator e{f};
    std::vector<Variable> variables;
    unsigned short p = 0u;
    for (const auto& v : e.getVariablesNames()) {
      raise_if(!matches(v),
               "tfel::check::eval: undeclared "
               "variable '" +
                   v + "'");
      variables.push_back(Variable{p, d.getColumn(convert(v))});
      ++p;
    }
    if (variables.empty()) {
      return d.getColumn(convert(f));
    }
    std::vector<double> r;
    r.resize(variables[0].values.size());
    for (std::vector<double>::size_type i = 0; i != r.size(); ++i) {
      for (const auto& v : variables) {
        e.setVariableValue(v.p, v.values[i]);
      }
      r[i] = e.getValue();
    }
    return r;
  }  // end of eval

  Column::Column(Column&&) = default;
  Column::Column(const Column&) = default;
  Column& Column::operator=(Column&&) = default;
  Column& Column::operator=(const Column&) = default;

  Column::Column(const int n) : num(n), byName(false) {
    this->name = std::to_string(this->num);
  }

  Column::Column(std::string n) : name(std::move(n)), num(0), byName(true) {}

  std::string Column::getName() const { return this->name; }

  const std::vector<double>& Column::getValues() { return this->values; }
  void Column::clearValues() { this->values.clear(); }

  void Column::resizeValues(std::vector<double>::size_type size) {
    this->values.resize(size);
  }

  void Column::setValue(unsigned pos, double value) {
    this->values.at(pos) = value;
  }

  void Column::setFilename(std::string file) {
    this->f = file;
    this->data = std::make_shared<tfel::utilities::TextData>(file, "alcyone");
    if (this->byName) {
      const auto& l = data->getLegends();
      if (std::find(l.begin(), l.end(), this->name) != l.end()) {
        this->num = data->findColumn(name);
        this->values = this->data->getColumn(this->num);
      } else {
        this->values = eval(*(this->data), this->name);
        return;
      }
    } else {
      this->data->getColumn(this->values, this->num);
    }
  }

  const std::string& Column::getFilename() const { return this->f; }

  const std::shared_ptr<tfel::utilities::TextData> Column::getData() const {
    return this->data;
  }

  Column::~Column() = default;

}  // end of namespace tfel::check
