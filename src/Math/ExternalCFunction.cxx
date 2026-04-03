/*!
 * \file   src/Math/ExternalCFunction.cxx
 * \brief
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Math/Parser/ExternalCFunction.hxx"

namespace tfel::math::parser {

  void ExternalCFunctionException::
      throwUnimplementedDifferentiateFunctionException() {
    raise(
        "ExternalCFunctionException::"
        "throwUnimplementedDifferentiateFunctionException: "
        "unimplemented feature");
  }  // end of
     // ExternalCFunctionException::throwUnimplementedDifferentiateFunctionException()

  void ExternalCFunctionException::
      throwInvalidCreateFunctionByChangingParametersIntoVariables() {
    raise(
        "ExternalCFunctionException::"
        "throwInvalidCreateFunctionByChangingParametersIntoVariables : "
        "invalid call");
  }  // end of
     // ExternalCFunctionException::throwInvalidCreateFunctionByChangingParametersIntoVariables()

  void ExternalCFunctionException::throwInvalidVariableIndex(
      const std::vector<double>::size_type pos, const unsigned short N) {
    raise(
        "ExternalCFunctionBase::setVariableValue: "
        "invalid index " +
        std::to_string(pos) + " (function has only " + std::to_string(N) +
        " variables).");
  }  // end of ExternalCFunctionException::throwInvalidVariableIndex

  std::shared_ptr<ExternalFunction>
  ExternalCFunctionBase<0u>::createFunctionByChangingParametersIntoVariables(
      std::vector<std::string>& v,
      const std::vector<double>&,
      const std::vector<std::string>&,
      const std::map<std::string, std::vector<double>::size_type>&) const {
    v.clear();
    return this->resolveDependencies();
  }  // end of
     // ExternalCFunctionBase<0u>::createFunctionByChangingParametersIntoVariables

  std::shared_ptr<ExternalFunction>
  ExternalCFunctionBase<0u>::createFunctionByChangingParametersIntoVariables(
      const std::vector<std::string>&) const {
    ExternalCFunctionException::
        throwInvalidCreateFunctionByChangingParametersIntoVariables();
  }  // end of
     // ExternalCFunctionBase<0u>::createFunctionByChangingParametersIntoVariables

  void ExternalCFunctionBase<0u>::setVariableValue(
      const std::vector<double>::size_type pos, const double) {
    raise(
        "ExternalCFunctionBase::setVariableValue: "
        "invalid index " +
        std::to_string(pos) + " (function has no variable).");
  }  // end of ExternalCFunctionBase<0u>::setVariableValue

  void ExternalCFunctionBase<0u>::getParametersNames(
      std::set<std::string>&) const {
  }  // endo of ExternalCFunction<0u>::getParametersNames

  std::vector<double>::size_type
  ExternalCFunctionBase<0u>::getNumberOfVariables() const {
    return 0u;
  }  // end of ExternalCFunctionBase<0u>::etNumberOfVariables() const

  void ExternalCFunctionBase<0u>::checkCyclicDependency(
      const std::string&) const {
  }  // end of ExternalCFunctionBase::checkCyclicDependency

  void ExternalCFunctionBase<0u>::checkCyclicDependency(
      std::vector<std::string>&) const {
  }  // end of ExternalCFunctionBase::checkCyclicDependency

  std::shared_ptr<ExternalFunction> ExternalCFunctionBase<0>::differentiate(
      const std::vector<double>::size_type) const {
    raise(
        "ExternalCFunctionBase<0>::differentiate: "
        "can't differentiate external function");
  }  // end of ExternalCFunctionBase<0>::differentiate

  std::shared_ptr<ExternalFunction> ExternalCFunctionBase<0>::differentiate(
      const std::string&) const {
    raise(
        "ExternalCFunctionBase<0>::differentiate : "
        "can't differentiate external function");
  }  // end of ExternalCFunctionBase<0>::differentiate

  ExternalCFunction<0u>::ExternalCFunction(
      ExternalCFunction<0u>::FunctionPtr f_)
      : f(f_) {}  // end of ExternalCFunction<0u>::ExternalCFunction

  double ExternalCFunction<0u>::getValue() const {
    return (*(this->f))();
  }  // end of ExternalCFunction<0u>::getValue() const

  ExternalCFunction<1u>::ExternalCFunction(
      ExternalCFunction<1u>::FunctionPtr f_)
      : f(f_) {}  // end of ExternalCFunction(FunctionPtr f_)

  double ExternalCFunction<1u>::getValue() const {
    return (*(this->f))(this->variables[0]);
  }  // end of getValue() const

  ExternalCFunction<2u>::ExternalCFunction(
      ExternalCFunction<2u>::FunctionPtr f_)
      : f(f_) {}  // end of ExternalCFunction(FunctionPtr f_)

  double ExternalCFunction<2u>::getValue() const {
    return (*(this->f))(this->variables[0], this->variables[1]);
  }  // end of getValue() const

  ExternalCFunction<3u>::ExternalCFunction(
      ExternalCFunction<3u>::FunctionPtr f_)
      : f(f_) {}  // end of ExternalCFunction(FunctionPtr f_)

  double ExternalCFunction<3u>::getValue() const {
    return (*(this->f))(this->variables[0], this->variables[1],
                        this->variables[2]);
  }  // end of getValue() const

  ExternalCFunction<4u>::ExternalCFunction(
      ExternalCFunction<4u>::FunctionPtr f_)
      : f(f_) {
  }  // end of ExternalCFunction<4u>::ExternalCFunction(FunctionPtr f_)

  double ExternalCFunction<4u>::getValue() const {
    return (*(this->f))(this->variables[0], this->variables[1],
                        this->variables[2], this->variables[3]);
  }  // end of ExternalCFunction<4u>::getValue() const

  ExternalCFunction<5u>::ExternalCFunction(
      ExternalCFunction<5u>::FunctionPtr f_)
      : f(f_) {}  // end of ExternalCFunction(FunctionPtr f_)

  double ExternalCFunction<5u>::getValue() const {
    return (*(this->f))(this->variables[0], this->variables[1],
                        this->variables[2], this->variables[3],
                        this->variables[4]);
  }  // end of getValue() const

  ExternalCFunction<6u>::ExternalCFunction(
      ExternalCFunction<6u>::FunctionPtr f_)
      : f(f_) {}  // end of ExternalCFunction(FunctionPtr f_)

  double ExternalCFunction<6u>::getValue() const {
    return (*(this->f))(this->variables[0], this->variables[1],
                        this->variables[2], this->variables[3],
                        this->variables[4], this->variables[5]);
  }  // end of ExternalCFunction<6u>::getValue() const

  ExternalCFunction<7u>::ExternalCFunction(
      ExternalCFunction<7u>::FunctionPtr f_)
      : f(f_) {
  }  // end of ExternalCFunction<7u>::ExternalCFunction(FunctionPtr f_)

  double ExternalCFunction<7u>::getValue() const {
    return (*(this->f))(this->variables[0], this->variables[1],
                        this->variables[2], this->variables[3],
                        this->variables[4], this->variables[5],
                        this->variables[6]);
  }  // end of ExternalCFunction<7u>::getValue() const

  ExternalCFunction<8u>::ExternalCFunction(
      ExternalCFunction<8u>::FunctionPtr f_)
      : f(f_) {}  // end of ExternalCFunction(FunctionPtr f_)

  double ExternalCFunction<8u>::getValue() const {
    return (*(this->f))(this->variables[0], this->variables[1],
                        this->variables[2], this->variables[3],
                        this->variables[4], this->variables[5],
                        this->variables[6], this->variables[7]);
  }  // end of getValue() const

  ExternalCFunction<9u>::ExternalCFunction(
      ExternalCFunction<9u>::FunctionPtr f_)
      : f(f_) {}  // end of ExternalCFunction(FunctionPtr f_)

  double ExternalCFunction<9u>::getValue() const {
    return (*(this->f))(
        this->variables[0], this->variables[1], this->variables[2],
        this->variables[3], this->variables[4], this->variables[5],
        this->variables[6], this->variables[7], this->variables[8]);
  }  // end of ExternalCFunction<9u>::getValue() const

  ExternalCFunction<10u>::ExternalCFunction(
      ExternalCFunction<10u>::FunctionPtr f_)
      : f(f_) {}  // end of ExternalCFunction(FunctionPtr f_)

  double ExternalCFunction<10u>::getValue() const {
    return (*(this->f))(this->variables[0], this->variables[1],
                        this->variables[2], this->variables[3],
                        this->variables[4], this->variables[5],
                        this->variables[6], this->variables[7],
                        this->variables[8], this->variables[9]);
  }  // end of ExternalCFunction<10u>::getValue() const

  ExternalCFunction<11u>::ExternalCFunction(
      ExternalCFunction<11u>::FunctionPtr f_)
      : f(f_) {
  }  // end of ExternalCFunction<11u>::ExternalCFunction(FunctionPtr f_)

  double ExternalCFunction<11u>::getValue() const {
    return (*(this->f))(
        this->variables[0], this->variables[1], this->variables[2],
        this->variables[3], this->variables[4], this->variables[5],
        this->variables[6], this->variables[7], this->variables[8],
        this->variables[9], this->variables[10]);
  }  // end of ExternalCFunction<11u>::getValue() const

  ExternalCFunction<12u>::ExternalCFunction(
      ExternalCFunction<12u>::FunctionPtr f_)
      : f(f_) {
  }  // end of ExternalCFunction<12u>::ExternalCFunction(FunctionPtr f_)

  double ExternalCFunction<12u>::getValue() const {
    return (*(this->f))(
        this->variables[0], this->variables[1], this->variables[2],
        this->variables[3], this->variables[4], this->variables[5],
        this->variables[6], this->variables[7], this->variables[8],
        this->variables[9], this->variables[10], this->variables[11]);
  }  // end of ExternalCFunction<12u>::getValue() const

  ExternalCFunction<13u>::ExternalCFunction(
      ExternalCFunction<13u>::FunctionPtr f_)
      : f(f_) {}  // end of ExternalCFunction(FunctionPtr f_)

  double ExternalCFunction<13u>::getValue() const {
    return (*(this->f))(
        this->variables[0], this->variables[1], this->variables[2],
        this->variables[3], this->variables[4], this->variables[5],
        this->variables[6], this->variables[7], this->variables[8],
        this->variables[9], this->variables[10], this->variables[11],
        this->variables[12]);
  }  // end of ExternalCFunction<13u>::getValue() const

  ExternalCFunction<14u>::ExternalCFunction(
      ExternalCFunction<14u>::FunctionPtr f_)
      : f(f_) {}  // end of
                  // ExternalCFunction<14u>::ExternalCFunction(FunctionPtr f_)

  double ExternalCFunction<14u>::getValue() const {
    return (*(this->f))(
        this->variables[0], this->variables[1], this->variables[2],
        this->variables[3], this->variables[4], this->variables[5],
        this->variables[6], this->variables[7], this->variables[8],
        this->variables[9], this->variables[10], this->variables[11],
        this->variables[12], this->variables[13]);
  }  // end of ExternalCFunction<14u>::getValue() const

  ExternalCFunction<15u>::ExternalCFunction(
      ExternalCFunction<15u>::FunctionPtr f_)
      : f(f_) {
  }  // end of ExternalCFunction<15u>::ExternalCFunction(FunctionPtr f_)

  double ExternalCFunction<15u>::getValue() const {
    return (*(this->f))(
        this->variables[0], this->variables[1], this->variables[2],
        this->variables[3], this->variables[4], this->variables[5],
        this->variables[6], this->variables[7], this->variables[8],
        this->variables[9], this->variables[10], this->variables[11],
        this->variables[12], this->variables[13], this->variables[14]);
  }  // end of getValue() const

  std::shared_ptr<ExternalFunction> ExternalCFunction<0u>::resolveDependencies()
      const {
    return std::shared_ptr<ExternalFunction>(
        new ExternalCFunction<0u>(this->f));
  }  // end of ExternalCFunction<0u>::resolveDependencies() const

  std::shared_ptr<ExternalFunction> ExternalCFunction<1u>::resolveDependencies()
      const {
    return std::shared_ptr<ExternalFunction>(
        new ExternalCFunction<1u>(this->f));
  }  // end of ExternalCFunction<1u>::resolveDependencies() const

  std::shared_ptr<ExternalFunction> ExternalCFunction<2u>::resolveDependencies()
      const {
    return std::shared_ptr<ExternalFunction>(
        new ExternalCFunction<2u>(this->f));
  }  // end of ExternalCFunction<2u>::resolveDependencies() const

  std::shared_ptr<ExternalFunction> ExternalCFunction<3u>::resolveDependencies()
      const {
    return std::shared_ptr<ExternalFunction>(
        new ExternalCFunction<3u>(this->f));
  }  // end of ExternalCFunction<3u>::resolveDependencies() const

  std::shared_ptr<ExternalFunction> ExternalCFunction<4u>::resolveDependencies()
      const {
    return std::shared_ptr<ExternalFunction>(
        new ExternalCFunction<4u>(this->f));
  }  // end of ExternalCFunction<4u>::resolveDependencies() const

  std::shared_ptr<ExternalFunction> ExternalCFunction<5u>::resolveDependencies()
      const {
    return std::shared_ptr<ExternalFunction>(
        new ExternalCFunction<5u>(this->f));
  }  // end of ExternalCFunction<5u>::resolveDependencies() const

  std::shared_ptr<ExternalFunction> ExternalCFunction<6u>::resolveDependencies()
      const {
    return std::shared_ptr<ExternalFunction>(
        new ExternalCFunction<6u>(this->f));
  }  // end of ExternalCFunction<6u>::resolveDependencies() const

  std::shared_ptr<ExternalFunction> ExternalCFunction<7u>::resolveDependencies()
      const {
    return std::shared_ptr<ExternalFunction>(
        new ExternalCFunction<7u>(this->f));
  }  // end of ExternalCFunction<7u>::resolveDependencies() const

  std::shared_ptr<ExternalFunction> ExternalCFunction<8u>::resolveDependencies()
      const {
    return std::shared_ptr<ExternalFunction>(
        new ExternalCFunction<8u>(this->f));
  }  // end of ExternalCFunction<8u>::resolveDependencies() const

  std::shared_ptr<ExternalFunction> ExternalCFunction<9u>::resolveDependencies()
      const {
    return std::shared_ptr<ExternalFunction>(
        new ExternalCFunction<9u>(this->f));
  }  // end of ExternalCFunction<9u>::resolveDependencies() const

  std::shared_ptr<ExternalFunction>
  ExternalCFunction<10u>::resolveDependencies() const {
    return std::shared_ptr<ExternalFunction>(
        new ExternalCFunction<10u>(this->f));
  }  // end of ExternalCFunction<10u>::resolveDependencies() const

  std::shared_ptr<ExternalFunction>
  ExternalCFunction<11u>::resolveDependencies() const {
    return std::shared_ptr<ExternalFunction>(
        new ExternalCFunction<11u>(this->f));
  }  // end of ExternalCFunction<11u>::resolveDependencies() const

  std::shared_ptr<ExternalFunction>
  ExternalCFunction<12u>::resolveDependencies() const {
    return std::shared_ptr<ExternalFunction>(
        new ExternalCFunction<12u>(this->f));
  }  // end of ExternalCFunction<12u>::resolveDependencies() const

  std::shared_ptr<ExternalFunction>
  ExternalCFunction<13u>::resolveDependencies() const {
    return std::shared_ptr<ExternalFunction>(
        new ExternalCFunction<13u>(this->f));
  }  // end of ExternalCFunction<13u>::resolveDependencies() const

  std::shared_ptr<ExternalFunction>
  ExternalCFunction<14u>::resolveDependencies() const {
    return std::shared_ptr<ExternalFunction>(
        new ExternalCFunction<14u>(this->f));
  }  // end of ExternalCFunction<14u>::resolveDependencies() const

  std::shared_ptr<ExternalFunction>
  ExternalCFunction<15u>::resolveDependencies() const {
    return std::shared_ptr<ExternalFunction>(
        new ExternalCFunction<15u>(this->f));
  }  // end of ExternalCFunction<15u>::resolveDependencies() const

}  // end of namespace tfel::math::parser
