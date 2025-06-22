/*!
 * \file   include/TFEL/Math/Parser/ExternalCFunction.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   02 oct 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_PARSER_EXTERNALCFUNCTION_HXX
#define LIB_TFEL_MATH_PARSER_EXTERNALCFUNCTION_HXX

#include <vector>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/System/ExternalFunctionsPrototypes.hxx"
#include "TFEL/Math/Parser/ExternalFunction.hxx"

namespace tfel {

  namespace math {

    namespace parser {

      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalCFunctionException {
        TFEL_NORETURN static void
        throwUnimplementedDifferentiateFunctionException();
        TFEL_NORETURN static void
        throwInvalidCreateFunctionByChangingParametersIntoVariables();
        TFEL_NORETURN static void throwInvalidVariableIndex(
            const std::vector<double>::size_type, const unsigned short);
      };  // end of struct ExternalCFunctionException

      template <unsigned short N>
      struct ExternalCFunctionBase : public ExternalFunction,
                                     protected ExternalCFunctionException {
        void setVariableValue(const std::vector<double>::size_type,
                              const double) override;
        std::vector<double>::size_type getNumberOfVariables() const override;
        void checkCyclicDependency(const std::string&) const override;
        void checkCyclicDependency(std::vector<std::string>&) const override;
        std::shared_ptr<ExternalFunction> differentiate(
            const std::vector<double>::size_type) const override;
        std::shared_ptr<ExternalFunction> differentiate(
            const std::string&) const override;
        std::shared_ptr<ExternalFunction>
        createFunctionByChangingParametersIntoVariables(
            const std::vector<std::string>&) const override;
        std::shared_ptr<ExternalFunction>
        createFunctionByChangingParametersIntoVariables(
            std::vector<std::string>&,
            const std::vector<double>&,
            const std::vector<std::string>&,
            const std::map<std::string, std::vector<double>::size_type>&)
            const override;
        void getParametersNames(std::set<std::string>&) const override;
        ~ExternalCFunctionBase() override = default;

       protected:
        double variables[N];
      };  // end of struct ExternalCFunction

      template <unsigned short N>
      void ExternalCFunctionBase<N>::getParametersNames(
          std::set<std::string>&) const {
      }  // end of ExternalCFunctionBase<N>::getParametersNames

      template <unsigned short N>
      void ExternalCFunctionBase<N>::checkCyclicDependency(
          const std::string&) const {
      }  // end of ExternalCFunctionBase::checkCyclicDependency

      template <unsigned short N>
      void ExternalCFunctionBase<N>::checkCyclicDependency(
          std::vector<std::string>&) const {
      }  // end of ExternalCFunctionBase::checkCyclicDependency

      template <unsigned short N>
      std::shared_ptr<ExternalFunction> ExternalCFunctionBase<N>::differentiate(
          const std::vector<double>::size_type) const {
        ExternalCFunctionBase::
            throwUnimplementedDifferentiateFunctionException();
#ifndef _MSC_VER
        return {};
#endif
      }  // end of ExternalCFunctionBase<N>::differentiate

      template <unsigned short N>
      std::shared_ptr<ExternalFunction>
      ExternalCFunctionBase<N>::createFunctionByChangingParametersIntoVariables(
          const std::vector<std::string>&) const {
        ExternalCFunctionException::
            throwInvalidCreateFunctionByChangingParametersIntoVariables();
#ifndef _MSC_VER
        return {};
#endif
      }

      template <unsigned short N>
      std::shared_ptr<ExternalFunction>
      ExternalCFunctionBase<N>::createFunctionByChangingParametersIntoVariables(
          std::vector<std::string>& v,
          const std::vector<double>&,
          const std::vector<std::string>&,
          const std::map<std::string, std::vector<double>::size_type>&) const {
        v.clear();
        return this->resolveDependencies();
      }  // end of
         // ExternalCFunctionBase<N>::createFunctionByChangingParametersIntoVariables

      template <unsigned short N>
      std::shared_ptr<ExternalFunction> ExternalCFunctionBase<N>::differentiate(
          const std::string&) const {
        ExternalCFunctionBase::
            throwUnimplementedDifferentiateFunctionException();
#ifndef _MSC_VER
        return {};
#endif
      }  // end of ExternalCFunctionBase<N>::differentiate

      template <unsigned short N>
      void ExternalCFunctionBase<N>::setVariableValue(
          const std::vector<double>::size_type pos, const double value) {
        using namespace std;
        if (pos >= N) {
          ExternalCFunctionException::throwInvalidVariableIndex(pos, N);
        }
        this->variables[pos] = value;
      }  // end of ExternalCFunctionBase::setVariableValue;

      template <unsigned short N>
      std::vector<double>::size_type
      ExternalCFunctionBase<N>::getNumberOfVariables() const {
        return N;
      }  // end of ExternalCFunctionBase<N>::getNumberOfVariables() const

      template <>
      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalCFunctionBase<0u>
          : public ExternalFunction {
        virtual void setVariableValue(const std::vector<double>::size_type,
                                      const double) override;
        virtual std::vector<double>::size_type getNumberOfVariables()
            const override;
        virtual void checkCyclicDependency(const std::string&) const override;
        virtual void checkCyclicDependency(
            std::vector<std::string>&) const override;
        virtual std::shared_ptr<ExternalFunction> differentiate(
            const std::vector<double>::size_type) const override;
        virtual std::shared_ptr<ExternalFunction> differentiate(
            const std::string&) const override;
        virtual std::shared_ptr<ExternalFunction>
        createFunctionByChangingParametersIntoVariables(
            std::vector<std::string>&,
            const std::vector<double>&,
            const std::vector<std::string>&,
            const std::map<std::string, std::vector<double>::size_type>&)
            const override;
        virtual std::shared_ptr<ExternalFunction>
        createFunctionByChangingParametersIntoVariables(
            const std::vector<std::string>&) const override;
        virtual void getParametersNames(std::set<std::string>&) const override;
      };  // end of struct ExternalCFunction

      template <unsigned short N>
      struct ExternalCFunction {};  // end of struct ExternalCFunction

      template <>
      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalCFunction<0u> final
          : public ExternalCFunctionBase<0u> {
        typedef tfel::system::CFunction0Ptr FunctionPtr;
        ExternalCFunction(FunctionPtr);
        virtual double getValue() const override;
        virtual std::shared_ptr<ExternalFunction> resolveDependencies()
            const override;

       private:
        FunctionPtr f;
      };  // end of struct ExternalCFunction

      template <>
      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalCFunction<1u> final
          : public ExternalCFunctionBase<1u> {
        typedef tfel::system::CFunction1Ptr FunctionPtr;
        ExternalCFunction(FunctionPtr);
        virtual double getValue() const override;
        virtual std::shared_ptr<ExternalFunction> resolveDependencies()
            const override;

       private:
        FunctionPtr f;
      };  // end of struct ExternalCFunction

      template <>
      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalCFunction<2u> final
          : public ExternalCFunctionBase<2u> {
        typedef tfel::system::CFunction2Ptr FunctionPtr;
        ExternalCFunction(FunctionPtr);
        virtual double getValue() const override;
        virtual std::shared_ptr<ExternalFunction> resolveDependencies()
            const override;

       private:
        FunctionPtr f;
      };  // end of struct ExternalCFunction

      template <>
      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalCFunction<3u> final
          : public ExternalCFunctionBase<3u> {
        typedef tfel::system::CFunction3Ptr FunctionPtr;
        ExternalCFunction(FunctionPtr);
        virtual double getValue() const override;
        virtual std::shared_ptr<ExternalFunction> resolveDependencies()
            const override;

       private:
        FunctionPtr f;
      };  // end of struct ExternalCFunction

      template <>
      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalCFunction<4u> final
          : public ExternalCFunctionBase<4u> {
        typedef tfel::system::CFunction4Ptr FunctionPtr;
        ExternalCFunction(FunctionPtr);
        virtual double getValue() const override;
        virtual std::shared_ptr<ExternalFunction> resolveDependencies()
            const override;

       private:
        FunctionPtr f;
      };  // end of struct ExternalCFunction

      template <>
      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalCFunction<5u> final
          : public ExternalCFunctionBase<5u> {
        typedef tfel::system::CFunction5Ptr FunctionPtr;
        ExternalCFunction(FunctionPtr);
        virtual double getValue() const override;
        virtual std::shared_ptr<ExternalFunction> resolveDependencies()
            const override;

       private:
        FunctionPtr f;
      };  // end of struct ExternalCFunction

      template <>
      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalCFunction<6u> final
          : public ExternalCFunctionBase<6u> {
        typedef tfel::system::CFunction6Ptr FunctionPtr;
        ExternalCFunction(FunctionPtr);
        virtual double getValue() const override;
        virtual std::shared_ptr<ExternalFunction> resolveDependencies()
            const override;

       private:
        FunctionPtr f;
      };  // end of struct ExternalCFunction

      template <>
      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalCFunction<7u> final
          : public ExternalCFunctionBase<7u> {
        typedef tfel::system::CFunction7Ptr FunctionPtr;
        ExternalCFunction(FunctionPtr);
        virtual double getValue() const override;
        virtual std::shared_ptr<ExternalFunction> resolveDependencies()
            const override;

       private:
        FunctionPtr f;
      };  // end of struct ExternalCFunction

      template <>
      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalCFunction<8u> final
          : public ExternalCFunctionBase<8u> {
        typedef tfel::system::CFunction8Ptr FunctionPtr;
        ExternalCFunction(FunctionPtr);
        virtual double getValue() const override;
        virtual std::shared_ptr<ExternalFunction> resolveDependencies()
            const override;

       private:
        FunctionPtr f;
      };  // end of struct ExternalCFunction

      template <>
      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalCFunction<9u> final
          : public ExternalCFunctionBase<9u> {
        typedef tfel::system::CFunction9Ptr FunctionPtr;
        ExternalCFunction(FunctionPtr);
        virtual double getValue() const override;
        virtual std::shared_ptr<ExternalFunction> resolveDependencies()
            const override;

       private:
        FunctionPtr f;
      };  // end of struct ExternalCFunction

      template <>
      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalCFunction<10u> final
          : public ExternalCFunctionBase<10u> {
        typedef tfel::system::CFunction10Ptr FunctionPtr;
        ExternalCFunction(FunctionPtr);
        virtual double getValue() const override;
        virtual std::shared_ptr<ExternalFunction> resolveDependencies()
            const override;

       private:
        FunctionPtr f;
      };  // end of struct ExternalCFunction

      template <>
      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalCFunction<11u> final
          : public ExternalCFunctionBase<11u> {
        typedef tfel::system::CFunction11Ptr FunctionPtr;
        ExternalCFunction(FunctionPtr);
        virtual double getValue() const override;
        virtual std::shared_ptr<ExternalFunction> resolveDependencies()
            const override;

       private:
        FunctionPtr f;
      };  // end of struct ExternalCFunction

      template <>
      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalCFunction<12u> final
          : public ExternalCFunctionBase<12u> {
        typedef tfel::system::CFunction12Ptr FunctionPtr;
        ExternalCFunction(FunctionPtr);
        virtual double getValue() const override;
        virtual std::shared_ptr<ExternalFunction> resolveDependencies()
            const override;

       private:
        FunctionPtr f;
      };  // end of struct ExternalCFunction

      template <>
      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalCFunction<13u> final
          : public ExternalCFunctionBase<13u> {
        typedef tfel::system::CFunction13Ptr FunctionPtr;
        ExternalCFunction(FunctionPtr);
        virtual double getValue() const override;
        virtual std::shared_ptr<ExternalFunction> resolveDependencies()
            const override;

       private:
        FunctionPtr f;
      };  // end of struct ExternalCFunction

      template <>
      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalCFunction<14u> final
          : public ExternalCFunctionBase<14u> {
        typedef tfel::system::CFunction14Ptr FunctionPtr;
        ExternalCFunction(FunctionPtr);
        virtual double getValue() const override;
        virtual std::shared_ptr<ExternalFunction> resolveDependencies()
            const override;

       private:
        FunctionPtr f;
      };  // end of struct ExternalCFunction

      template <>
      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalCFunction<15u> final
          : public ExternalCFunctionBase<15u> {
        typedef tfel::system::CFunction15Ptr FunctionPtr;
        ExternalCFunction(FunctionPtr);
        virtual double getValue() const override;
        virtual std::shared_ptr<ExternalFunction> resolveDependencies()
            const override;

       private:
        FunctionPtr f;
      };  // end of struct ExternalCFunction

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_PARSER_EXTERNALCFUNCTION_HXX */
