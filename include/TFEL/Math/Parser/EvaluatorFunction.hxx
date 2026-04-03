/*!
 * \file   include/TFEL/Math/Parser/EvaluatorFunction.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   15/01/2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_PARSER_EVALUATORFUNCTION_HXX
#define LIB_TFEL_MATH_PARSER_EVALUATORFUNCTION_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Evaluator.hxx"
#include "TFEL/Math/Parser/Expr.hxx"

namespace tfel::math::parser {

#ifdef __SUNPRO_CC
  extern "C" {
#endif /*__SUNPRO_CC */
  typedef double (*EvaluatorProxyFunctionPtr1V)(double);
  typedef double (*EvaluatorProxyFunctionPtr2V)(double, double);
  typedef double (*EvaluatorProxyFunctionPtr1P1V)(int, double);
  typedef double (*EvaluatorProxyFunctionPtr1P2V)(int, double, double);
  typedef double (*EvaluatorProxyFunctionPtr2P1V)(int, int, double);
  typedef double (*EvaluatorProxyFunctionPtr2P2V)(int, int, double, double);
#ifdef __SUNPRO_CC
  }
#endif /* __SUNPRO_CC */

  /*!
   * a base class which offers a default implementation of the
   * `differentiate` method
   */
  struct EvaluatorFunctionBase : public Expr {
    //! \brief constructor
    EvaluatorFunctionBase();
    std::string getCxxFormula(const std::vector<std::string>&) const override;
    std::shared_ptr<Expr> differentiate(
        const std::vector<double>::size_type,
        const std::vector<double>&) const override;
    //! \brief destructor
    ~EvaluatorFunctionBase() override;
  };  // end of struct EvaluatorFunctionBase

  struct EvaluatorFunction1VBase : public EvaluatorFunctionBase {
    EvaluatorFunction1VBase(const std::shared_ptr<Expr>);
    //
    bool isConstant() const override;
    bool dependsOnVariable(const std::vector<double>::size_type) const override;
    void checkCyclicDependency(std::vector<std::string>&) const override;
    ~EvaluatorFunction1VBase() override;

   protected:
    EvaluatorFunction1VBase& operator=(const EvaluatorFunction1VBase&) = delete;
    EvaluatorFunction1VBase& operator=(EvaluatorFunction1VBase&&) = delete;
    const std::shared_ptr<Expr> expr;
  };  // end of struct EvaluatorFunction1V

  struct EvaluatorFunction2VBase : public EvaluatorFunctionBase {
    EvaluatorFunction2VBase(const std::shared_ptr<Expr>,
                            const std::shared_ptr<Expr>);
    //
    bool isConstant() const override;
    bool dependsOnVariable(const std::vector<double>::size_type) const override;
    void checkCyclicDependency(std::vector<std::string>&) const override;
    ~EvaluatorFunction2VBase() override;

   protected:
    EvaluatorFunction2VBase& operator=(const EvaluatorFunction2VBase&) = delete;
    EvaluatorFunction2VBase& operator=(EvaluatorFunction2VBase&&) = delete;
    const std::shared_ptr<Expr> e1;
    const std::shared_ptr<Expr> e2;
  };  // end of struct EvaluatorFunction2V

  struct EvaluatorFunction1P1V final : public EvaluatorFunction1VBase {
    EvaluatorFunction1P1V(const EvaluatorProxyFunctionPtr1P1V,
                          const int,
                          const std::shared_ptr<Expr>);
    double getValue() const override;
    std::shared_ptr<Expr> resolveDependencies(
        const std::vector<double>&) const override;
    std::shared_ptr<Expr> clone(const std::vector<double>&) const override;
    std::shared_ptr<Expr> createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&)
        const override;
    void getParametersNames(std::set<std::string>&) const override;
    ~EvaluatorFunction1P1V() override;

   private:
    EvaluatorFunction1P1V& operator=(const EvaluatorFunction1P1V&) = delete;
    EvaluatorFunction1P1V& operator=(EvaluatorFunction1P1V&&) = delete;
    EvaluatorProxyFunctionPtr1P1V f;
    const int n;
  };  // end of struct EvaluatorFunction1P1V

  struct EvaluatorFunction2P1V final : public EvaluatorFunction1VBase {
    EvaluatorFunction2P1V(const EvaluatorProxyFunctionPtr2P1V,
                          const int,
                          const int,
                          const std::shared_ptr<Expr>);
    double getValue() const override;
    std::shared_ptr<Expr> resolveDependencies(
        const std::vector<double>&) const override;
    std::shared_ptr<Expr> clone(const std::vector<double>&) const override;
    std::shared_ptr<Expr> createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&)
        const override;
    void getParametersNames(std::set<std::string>&) const override;
    ~EvaluatorFunction2P1V() override;

   private:
    EvaluatorFunction2P1V& operator=(const EvaluatorFunction2P1V&) = delete;
    EvaluatorFunction2P1V& operator=(EvaluatorFunction2P1V&&) = delete;
    EvaluatorProxyFunctionPtr2P1V f;
    const int n;
    const int m;
  };  // end of struct EvaluatorFunction2P1V

  struct EvaluatorFunction1P2V final : public EvaluatorFunction2VBase {
    EvaluatorFunction1P2V(const EvaluatorProxyFunctionPtr1P2V,
                          const int,
                          const std::shared_ptr<Expr>,
                          const std::shared_ptr<Expr>);
    double getValue() const override;
    std::shared_ptr<Expr> resolveDependencies(
        const std::vector<double>&) const override;
    std::shared_ptr<Expr> clone(const std::vector<double>&) const override;
    std::shared_ptr<Expr> createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&)
        const override;
    void getParametersNames(std::set<std::string>&) const override;
    ~EvaluatorFunction1P2V() override;

   private:
    EvaluatorFunction1P2V& operator=(const EvaluatorFunction1P2V&) = delete;
    EvaluatorFunction1P2V& operator=(EvaluatorFunction1P2V&&) = delete;
    EvaluatorProxyFunctionPtr1P2V f;
    const int n;
  };  // end of struct EvaluatorFunction1P2V

  struct EvaluatorFunction2P2V final : public EvaluatorFunction2VBase {
    EvaluatorFunction2P2V(const EvaluatorProxyFunctionPtr2P2V,
                          const int,
                          const int,
                          const std::shared_ptr<Expr>,
                          const std::shared_ptr<Expr>);
    double getValue() const override;
    std::shared_ptr<Expr> resolveDependencies(
        const std::vector<double>&) const override;
    std::shared_ptr<Expr> clone(const std::vector<double>&) const override;
    std::shared_ptr<Expr> createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&)
        const override;
    void getParametersNames(std::set<std::string>&) const override;
    ~EvaluatorFunction2P2V() override;

   private:
    EvaluatorFunction2P2V& operator=(const EvaluatorFunction2P2V&) = delete;
    EvaluatorFunction2P2V& operator=(EvaluatorFunction2P2V&&) = delete;
    EvaluatorProxyFunctionPtr2P2V f;
    const int n;
    const int m;
  };  // end of struct EvaluatorFunction2P2V

  template <unsigned short N>
  struct EvaluatorFunctionWrapper;

  template <unsigned short N>
  struct TFEL_VISIBILITY_LOCAL EvaluatorFunctionNV final
      : public EvaluatorFunctionBase {
    EvaluatorFunctionNV(typename EvaluatorFunctionWrapper<N>::type,
                        const std::vector<std::shared_ptr<Expr>>&);
    //
    bool isConstant() const override;
    bool dependsOnVariable(const std::vector<double>::size_type) const override;
    double getValue() const override;
    std::shared_ptr<Expr> resolveDependencies(
        const std::vector<double>&) const override;
    std::shared_ptr<Expr> clone(const std::vector<double>&) const override;
    std::shared_ptr<Expr> createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&)
        const override;
    void checkCyclicDependency(std::vector<std::string>&) const override;
    void getParametersNames(std::set<std::string>&) const override;
    ~EvaluatorFunctionNV() override;

   protected:
    EvaluatorFunctionNV& operator=(const EvaluatorFunctionNV&) = delete;
    EvaluatorFunctionNV& operator=(EvaluatorFunctionNV&&) = delete;
    typename EvaluatorFunctionWrapper<N>::type f;
    const std::vector<std::shared_ptr<Expr>> args;
  };  // end of struct EvaluatorFunctionNV

  template <unsigned short N>
  struct TFEL_VISIBILITY_LOCAL EvaluatorFunction1PNV final
      : public EvaluatorFunctionBase {
    EvaluatorFunction1PNV(typename EvaluatorFunctionWrapper<N>::type1P,
                          const int,
                          const std::vector<std::shared_ptr<Expr>>&);
    //
    bool isConstant() const override;
    bool dependsOnVariable(const std::vector<double>::size_type) const override;
    double getValue() const override;
    std::shared_ptr<Expr> resolveDependencies(
        const std::vector<double>&) const override;
    std::shared_ptr<Expr> clone(const std::vector<double>&) const override;
    std::shared_ptr<Expr> createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&)
        const override;
    void checkCyclicDependency(std::vector<std::string>&) const override;
    void getParametersNames(std::set<std::string>&) const override;
    ~EvaluatorFunction1PNV() override;

   protected:
    EvaluatorFunction1PNV& operator=(const EvaluatorFunction1PNV&) = delete;
    EvaluatorFunction1PNV& operator=(EvaluatorFunction1PNV&&) = delete;
    typename EvaluatorFunctionWrapper<N>::type1P f;
    const int n;
    const std::vector<std::shared_ptr<Expr>> args;
  };  // end of struct EvaluatorFunction1PNV

  template <unsigned short N>
  struct TFEL_VISIBILITY_LOCAL EvaluatorFunction2PNV final
      : public EvaluatorFunctionBase {
    EvaluatorFunction2PNV(typename EvaluatorFunctionWrapper<N>::type2P,
                          const int,
                          const int,
                          const std::vector<std::shared_ptr<Expr>>&);
    //
    bool isConstant() const override;
    bool dependsOnVariable(const std::vector<double>::size_type) const override;
    double getValue() const override;
    std::shared_ptr<Expr> resolveDependencies(
        const std::vector<double>&) const override;
    std::shared_ptr<Expr> clone(const std::vector<double>&) const override;
    std::shared_ptr<Expr> createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&)
        const override;
    void checkCyclicDependency(std::vector<std::string>&) const override;
    ~EvaluatorFunction2PNV() override;

   protected:
    EvaluatorFunction2PNV& operator=(const EvaluatorFunction2PNV&) = delete;
    EvaluatorFunction2PNV& operator=(EvaluatorFunction2PNV&&) = delete;
    typename EvaluatorFunctionWrapper<N>::type2P f;
    const int n;
    const int m;
    const std::vector<std::shared_ptr<Expr>> args;
  };  // end of struct EvaluatorFunction2PNV

  template <unsigned short N>
  struct TFEL_VISIBILITY_LOCAL EvaluatorFunction1UPNV final
      : public EvaluatorFunctionBase {
    EvaluatorFunction1UPNV(typename EvaluatorFunctionWrapper<N>::type1UP,
                           const unsigned int,
                           const std::vector<std::shared_ptr<Expr>>&);
    //
    bool isConstant() const override;
    bool dependsOnVariable(const std::vector<double>::size_type) const override;
    double getValue() const override;
    std::shared_ptr<Expr> resolveDependencies(
        const std::vector<double>&) const override;
    std::shared_ptr<Expr> clone(const std::vector<double>&) const override;
    std::shared_ptr<Expr> createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&)
        const override;
    void checkCyclicDependency(std::vector<std::string>&) const override;
    void getParametersNames(std::set<std::string>&) const override;
    ~EvaluatorFunction1UPNV();

   protected:
    EvaluatorFunction1UPNV& operator=(const EvaluatorFunction1UPNV&) = delete;
    EvaluatorFunction1UPNV& operator=(EvaluatorFunction1UPNV&&) = delete;
    typename EvaluatorFunctionWrapper<N>::type1UP f;
    const unsigned int n;
    const std::vector<std::shared_ptr<Expr>> args;
  };  // end of struct EvaluatorFunction1UPNV

  template <unsigned short N>
  struct TFEL_VISIBILITY_LOCAL EvaluatorFunction2UPNV final
      : public EvaluatorFunctionBase {
    EvaluatorFunction2UPNV(typename EvaluatorFunctionWrapper<N>::type2UP,
                           const unsigned int n,
                           const unsigned int m,
                           const std::vector<std::shared_ptr<Expr>>&);
    //
    bool isConstant() const override;
    bool dependsOnVariable(const std::vector<double>::size_type) const override;
    double getValue() const override;
    std::shared_ptr<Expr> resolveDependencies(
        const std::vector<double>&) const override;
    std::shared_ptr<Expr> clone(const std::vector<double>&) const override;
    std::shared_ptr<Expr> createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&)
        const override;
    void checkCyclicDependency(std::vector<std::string>&) const override;
    void getParametersNames(std::set<std::string>&) const override;
    ~EvaluatorFunction2UPNV() override;

   protected:
    EvaluatorFunction2UPNV& operator=(const EvaluatorFunction2UPNV&) = delete;
    EvaluatorFunction2UPNV& operator=(EvaluatorFunction2UPNV&&) = delete;
    typename EvaluatorFunctionWrapper<N>::type2UP f;
    const unsigned int n;
    const unsigned int m;
    const std::vector<std::shared_ptr<Expr>> args;
  };  // end of struct EvaluatorFunction2UPNV

}  // end of namespace tfel::math::parser

#include "TFEL/Math/Parser/EvaluatorFunction.ixx"

#endif /* LIB_TFEL_MATH_PARSER_EVALUATORFUNCTION_HXX */
