/*!
 * \file   mfront/src/SupportedTypes.cxx
 *
 * \brief
 * \author Thomas Helfer
 * \date   12/01/2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * re served.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <utility>
#include <sstream>
#include <charconv>
#include <algorithm>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Math/IntegerEvaluator.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/SupportedTypes.hxx"

namespace mfront {

  //! \return a map between type names and Supported::TypeFlags
  static std::map<std::string, SupportedTypes::TypeFlag, std::less<>>
  SupportedTypes_buildFlags() {
    std::map<std::string, SupportedTypes::TypeFlag, std::less<>> flags;
    flags.insert({"NumericType", SupportedTypes::SCALAR});
    for (const auto& t : getScalarTypeAliases()) {
      flags.insert({t, SupportedTypes::SCALAR});
    }
    for (const auto& t : getTinyVectorTypeAliases()) {
      flags.insert({t, SupportedTypes::TVECTOR});
    }
    for (const auto& t : getStensorTypeAliases()) {
      flags.insert({t, SupportedTypes::STENSOR});
    }
    for (const auto& t : getTensorTypeAliases()) {
      flags.insert({t, SupportedTypes::TENSOR});
    }
    return flags;
  }  // end of SupportedTypes_getFlags

  //! \return a map between type names and Supported::TypeFlags
  static std::map<std::string, SupportedTypes::TypeFlag, std::less<>>&
  SupportedTypes_getFlags() {
    static std::map<std::string, SupportedTypes::TypeFlag, std::less<>> flags =
        SupportedTypes_buildFlags();
    return flags;
  }  // end of SupportedTypes_getFlags

  int SupportedTypes::getSizeOrSpaceDimension(
      const TypeInformation::IntegerTemplateArgument& N) {
    if (std::holds_alternative<int>(N)) {
      return std::get<int>(N);
    }
    const auto& f = std::get<std::string>(N);
    if (f != "N") {
      tfel::raise(
          "SupportedTypes::getSizeOrSpaceDimension: "
          "unsupported dimension description ('" +
          f + "')");
    }
    return 0;
  }  // end of SupportedTypes::getSizeOrSpaceDimension

  const std::map<std::string, SupportedTypes::TypeFlag, std::less<>>&
  SupportedTypes::getTypeFlags() {
    return SupportedTypes_getFlags();
  }  // end of SupportedTypes::getTypeFlags

  bool SupportedTypes::isSupportedConstexprIntegerVariables(
      const std::string_view v) {
    return (v == "N") || (v == "TVectorSize") || (v == "StensorSize") ||
           (v == "TensorSize");
  }  // end of isSupportedConstexprIntegerVariables

  SupportedTypes::TypeParsingOptions::TypeParsingOptions() = default;
  SupportedTypes::TypeParsingOptions::TypeParsingOptions(TypeParsingOptions&&) =
      default;
  SupportedTypes::TypeParsingOptions::TypeParsingOptions(
      const TypeParsingOptions&) = default;
  SupportedTypes::TypeParsingOptions&
  SupportedTypes::TypeParsingOptions::operator=(TypeParsingOptions&&) = default;
  SupportedTypes::TypeParsingOptions&
  SupportedTypes::TypeParsingOptions::operator=(const TypeParsingOptions&) =
      default;
  SupportedTypes::TypeParsingOptions::~TypeParsingOptions() = default;

  SupportedTypes::TypeSize SupportedTypes::TypeSize::getDerivativeSize(
      const TypeSize& s1, const TypeSize& s2) {
    return TypeSize::getDerivativeSize(1u, s1, s2);
  }

  SupportedTypes::TypeSize SupportedTypes::TypeSize::getDerivativeSize(
      const unsigned int a, const TypeSize& s1, const TypeSize& s2) {
    if (!s1.isMonomial()) {
      tfel::raise(
          "SupportedTypes::TypeSize::getDerivativeSize: "
          "invalid first argument");
    }
    if (!s2.isMonomial()) {
      tfel::raise(
          "SupportedTypes::TypeSize::getDerivativeSize: "
          "invalid second argument");
    }
    auto describes_scalar = [](const Monomial& m) {
      return ((m.tvector_exponent == 0u) &&  //
              (m.stensor_exponent == 0u) &&  //
              (m.tensor_exponent == 0u));
    };
    if (a == 0) {
      return TypeSize{};
    }
    auto m = Monomial{};
    const auto& m1 = s1.monomials[0];
    const auto& m2 = s2.monomials[0];
    if ((s1.isArray()) && (!describes_scalar(m1))) {
      tfel::raise(
          "SupportedTypes::TypeSize::getDerivativeSize: "
          "invalid first argument");
    }
    if ((s2.isArray()) && (!describes_scalar(m2))) {
      tfel::raise(
          "SupportedTypes::TypeSize::getDerivativeSize: "
          "invalid second argument");
    }
    m.array_size = static_cast<int>(a * m1.array_size * m2.array_size);
    m.tvector_exponent = m1.tvector_exponent + m2.tvector_exponent;
    m.stensor_exponent = m1.stensor_exponent + m2.stensor_exponent;
    m.tensor_exponent = m1.tensor_exponent + m2.tensor_exponent;
    auto r = TypeSize{};
    r.monomials.push_back(m);
    return r;
  }  // end of getDerivativeSize

  SupportedTypes::TypeSize SupportedTypes::TypeSize::buildFromExponents(
      const unsigned int i1, const unsigned int i2, const unsigned int i3) {
    return TypeSize{1, i1, i2, i3};
  }  // end of buildFromExponents

  SupportedTypes::TypeSize::TypeSize() = default;

  SupportedTypes::TypeSize::TypeSize(TypeSize&&) = default;

  SupportedTypes::TypeSize::TypeSize(const TypeSize&) = default;

  SupportedTypes::TypeSize::TypeSize(const unsigned int a,
                                     const unsigned int i1,
                                     const unsigned int i2,
                                     const unsigned int i3) {
    if (a != 0) {
      const auto ia = static_cast<int>(a);
      this->monomials.push_back(Monomial{ia, i1, i2, i3});
    }
  }

  SupportedTypes::TypeSize::TypeSize(const TypeFlag f, const unsigned int e)
      : TypeSize(1u, f, e) {}  // end of TypeSize

  SupportedTypes::TypeSize::TypeSize(const unsigned int a,
                                     const TypeFlag f,
                                     const unsigned int e) {
    if (e == 0) {
      tfel::raise(
          "SupportedTypes::TypeSize::TypeSize: "
          "null exponent");
    }
    if (a != 0) {
      const auto ia = static_cast<int>(a);
      if (f == SupportedTypes::SCALAR) {
        this->monomials.push_back(Monomial{ia, 0, 0, 0});
      } else if (f == SupportedTypes::TVECTOR) {
        this->monomials.push_back(Monomial{ia, e, 0, 0});
      } else if (f == SupportedTypes::STENSOR) {
        this->monomials.push_back(Monomial{ia, 0, e, 0});
      } else if (f == SupportedTypes::TENSOR) {
        this->monomials.push_back(Monomial{ia, 0, 0, e});
      } else {
        tfel::raise(
            "SupportedTypes::TypeSize::TypeSize: "
            "unsupported type flag");
      }
    }
  }  // end of TypeSize

  SupportedTypes::TypeSize& SupportedTypes::TypeSize::operator=(TypeSize&&) =
      default;

  SupportedTypes::TypeSize& SupportedTypes::TypeSize::operator=(
      const TypeSize&) = default;

  SupportedTypes::TypeSize& SupportedTypes::TypeSize::operator+=(
      const TypeSize& rhs) {
    for (const auto& m : rhs.monomials) {
      const auto pm = this->findMatchingMonomial(m);
      if (pm == this->monomials.end()) {
        this->monomials.push_back(m);
      } else {
        pm->array_size += m.array_size;
      }
    }
    return *this;
  }

  SupportedTypes::TypeSize& SupportedTypes::TypeSize::operator-=(
      const TypeSize& rhs) {
    for (const auto& m : rhs.monomials) {
      const auto pm = this->findMatchingMonomial(m);
      if (pm == this->monomials.end()) {
        auto nm = m;
        nm.array_size *= -1;
        this->monomials.push_back(nm);
      } else {
        if (pm->array_size == m.array_size) {
          this->monomials.erase(pm);
        } else {
          pm->array_size -= m.array_size;
        }
      }
    }
    return *this;
  }

  SupportedTypes::TypeSize& SupportedTypes::TypeSize::operator*=(
      const unsigned int a) {
    if (a == 0) {
      *this = TypeSize{};
      return *this;
    }
    const auto ia = static_cast<int>(a);
    for (auto& m : this->monomials) {
      m.array_size *= ia;
    }
    return *this;
  }  // end of operator*

  SupportedTypes::TypeSize operator*(const SupportedTypes::TypeSize& s,
                                     const unsigned int a) {
    auto r = s;
    r *= a;
    return r;
  }  // end of operator*

  SupportedTypes::TypeSize operator*(const unsigned int a,
                                     const SupportedTypes::TypeSize& s) {
    auto r = s;
    r *= a;
    return r;
  }  // end of operator *

  int SupportedTypes::TypeSize::getValueForDimension(
      const unsigned short d) const {
    const auto sizes = [&d]() -> std::array<unsigned int, 3u> {
      switch (d) {
        case 1:
          return {1, 3, 3};
        case 2:
          return {2, 4, 5};
        case 3:
          return {3, 6, 9};
      }
      tfel::raise(
          "SupportedTypes::TypeSize::getValueForDimension: "
          "invalid dimension");
    }();
    auto power = [](const unsigned int i, const unsigned int e) {
      unsigned int r = 1;
      for (unsigned int j = 0; j != e; ++j) {
        r *= i;
      }
      return r;
    };
    int r = 0;
    for (const auto& m : this->monomials) {
      r += m.array_size *  //
           power(sizes[0], m.tvector_exponent) *
           power(sizes[1], m.stensor_exponent) *
           power(sizes[2], m.tensor_exponent);
    }
    return r;
  }  // end of SupportedTypes::TypeSize::getValueForDimension

  int SupportedTypes::TypeSize::getValueForModellingHypothesis(
      const Hypothesis h) const {
    return this->getValueForDimension(tfel::material::getSpaceDimension(h));
  }

  std::string SupportedTypes::TypeSize::asString() const {
    return this->asString({"TVectorSize", "StensorSize", "TensorSize"});
  }  // end of asString

  std::string SupportedTypes::TypeSize::asString(
      const std::array<std::string, 3u>& values) const {
    if (this->monomials.empty()) {
      return "0";
    }
    std::ostringstream os;
    auto first = true;
    for (const auto& m : this->monomials) {
      if (!first) {
        os << '+';
      }
      if ((m.tvector_exponent == 0) && (m.stensor_exponent == 0) &&
          (m.tensor_exponent == 0)) {
        os << m.array_size;
      } else {
        auto b = true;
        if (m.array_size != 1) {
          os << m.array_size;
          b = false;
        }
        auto add = [&os, &b](const std::string& v, const unsigned int e) {
          for (unsigned i = 0; i != e; ++i) {
            if (!b) {
              os << '*';
            }
            os << v;
            b = false;
          }
        };
        add(values[0], m.tvector_exponent);
        add(values[1], m.stensor_exponent);
        add(values[2], m.tensor_exponent);
      }
      first = false;
    }
    return os.str();
  }  // end of asString

  bool SupportedTypes::TypeSize::isMonomial() const {
    return this->monomials.size() == 1u;
  }  // end of isMonomial

  bool SupportedTypes::TypeSize::isArray() const {
    if (this->monomials.size() != 1u) {
      return false;
    }
    return this->monomials[0].array_size != 1u;
  }  // end of isArray

  std::ostream& operator<<(std::ostream& os,
                           const SupportedTypes::TypeSize& s) {
    os << s.asString();
    return os;
  }

  SupportedTypes::SupportedTypes() = default;

  static std::map<std::tuple<SupportedTypes::TypeFlag,
                             SupportedTypes::TypeFlag,
                             std::string>,
                  SupportedTypes::TypeFlag>
  buildResultTypeFlags() {
    auto r = std::map<std::tuple<SupportedTypes::TypeFlag,
                                 SupportedTypes::TypeFlag, std::string>,
                      SupportedTypes::TypeFlag>{};
    for (const auto f : {SupportedTypes::SCALAR, SupportedTypes::TVECTOR,
                         SupportedTypes::STENSOR, SupportedTypes::TENSOR}) {
      r.insert({{f, f, "tfel::math::OpPlus"}, f});
      r.insert({{f, f, "tfel::math::OpMinus"}, f});
      r.insert({{f, SupportedTypes::SCALAR, "tfel::math::OpMult"}, f});
      r.insert({{SupportedTypes::SCALAR, f, "tfel::math::OpMult"}, f});
      r.insert({{f, SupportedTypes::SCALAR, "tfel::math::OpDiv"}, f});
    }
    r.insert({{SupportedTypes::STENSOR, SupportedTypes::STENSOR,
               "tfel::math::OpMult"},
              SupportedTypes::TENSOR});
    r.insert(
        {{SupportedTypes::TENSOR, SupportedTypes::TENSOR, "tfel::math::OpMult"},
         SupportedTypes::TENSOR});
    return r;
  }  // end of buildResultTypeFlags

  void SupportedTypes::checkNumberOfTemplateArguments(const TypeInformation& t,
                                                      const std::size_t n) {
    auto report_error = [&t] {
      tfel::raise(
          "SupportedTypes::checkNumberOfTemplateArguments: "
          "invalid template arguments for class '" +
          t.type + "'");
    };
    if (n == 0) {
      if (t.template_arguments) {
        report_error();
      }
      return;
    }
    if (!t.template_arguments) {
      report_error();
    }
    const auto& args = *(t.template_arguments);
    if (args.size() != n) {
      report_error();
    }
  }  // end of checkNumberOfTemplateArguments

  bool SupportedTypes::hasTypeFlag(const std::string_view type) {
    const auto oflag = SupportedTypes::getTypeFlag(
        SupportedTypes::getTypeInformation(type, TypeParsingOptions()));
    return oflag.has_value();
  }  // end of SupportedTypes::hasTypeFlag

  bool SupportedTypes::isScalarType(const std::string_view type) {
    return SupportedTypes::isScalarType(
        SupportedTypes::getTypeInformation(type, TypeParsingOptions()));
  }  // end of isScalarType

  bool SupportedTypes::isScalarType(const TypeInformation& t) {
    const auto oflag = SupportedTypes::getTypeFlag(t);
    if (!oflag.has_value()) {
      return false;
    }
    return *oflag == SupportedTypes::SCALAR;
  }  // end of isScalarType

  int SupportedTypes::getTypeIdentifier(const std::string_view type) {
    return SupportedTypes::getTypeIdentifier(
               SupportedTypes::getTypeInformation(type, TypeParsingOptions()))
        .first;
  }  // end of getTypeIdentifier

  std::pair<int, int> SupportedTypes::getTypeIdentifier(
      const TypeInformation& t) {
    const auto& flags = SupportedTypes_getFlags();
    const auto p = flags.find(t.type);
    if (p != flags.end()) {
      SupportedTypes::checkNumberOfTemplateArguments(t, 0u);
      switch (p->second) {
        case SupportedTypes::SCALAR:
          return {0, 3};
        case SupportedTypes::STENSOR:
          return {1, 3};
        case SupportedTypes::TVECTOR:
          return {2, 3};
        case SupportedTypes::TENSOR:
          return {3, 3};
        default:
          break;
      }
    }
    auto treatDerivative =
        [](const std::pair<int, int> id1,
           const std::pair<int, int> id2) -> std::pair<int, int> {
      return {4 + (id1.first << 3) + (id2.first << (3 + id1.second)),
              3 + id1.second + id2.second};
    };
    auto treatStandardTensorialObjectBase =
        [](const int N, const int id) -> std::pair<int, int> {
      return {id + (N << 3), 5};
    };
    auto treatStandardTensorialObject =
        [&t, &treatStandardTensorialObjectBase](
            const int id) -> std::pair<int, int> {
      const auto N = getTensorialObjectSpaceDimension(t);
      return treatStandardTensorialObjectBase(N, id);
    };
    auto treatStandardTensorObjectDerivative =
        [&treatDerivative, &treatStandardTensorialObject](
            const int id1, const int id2) -> std::pair<int, int> {
      return treatDerivative(treatStandardTensorialObject(id1),
                             treatStandardTensorialObject(id2));
    };
    auto treatArrayBase =
        [&t](const std::vector<int>& sizes,
             const TypeInformation& value_type) -> std::pair<int, int> {
      const auto n = static_cast<int>(sizes.size());
      if (n == 0) {
        tfel::raise(
            "SupportedTypes::getTypeIdentifier: "
            "invalid array arity."
            "2rror while treating type '" +
            SupportedTypes::encode(t) + "'");
      }
      const auto [id, offset] = SupportedTypes::getTypeIdentifier(value_type);
      auto r = 5 + (n << 3);
      auto o = 6;
      for (const auto size : sizes) {
        r += (size << o);
        o += 7;
      }
      r += (id << o);
      return {r, o + offset};
    };
    auto treatArray1D = [&t, &treatArrayBase]() -> std::pair<int, int> {
      SupportedTypes::checkNumberOfTemplateArguments(t, 2u);
      const auto& args = *(t.template_arguments);
      const auto& size =
          std::get<TypeInformation ::IntegerTemplateArgument>(args[0]);
      const auto& value_type = std::get<TypeInformation>(args[1]);
      if (std::holds_alternative<std::string>(size)) {
        tfel::raise(
            "SupportedTypes::getTypeIdentifier: "
            "arrays whose size is not an integer are not supported. "
            "Error while treating type '" +
            SupportedTypes::encode(t) + "'");
      }
      return treatArrayBase({std::get<int>(size)}, value_type);
    };
    auto isST2toST2TypeAliases = [](const std::string_view type) {
      const auto types = mfront::getST2toST2TypeAliases();
      return std::find(types.begin(), types.end(), type) != types.end();
    };
    if ((t.type == "tfel::math::quantity") ||
        (t.type == "mfront_quantity")) {
      return {0, 3};
    } else if (t.type == "tfel::math::fsarray") {
      return treatArray1D();
    } else if (t.type == "tfel::math::tvector") {
      SupportedTypes::checkNumberOfTemplateArguments(t, 2u);
      const auto& args = *(t.template_arguments);
      const auto N = getTensorialObjectSpaceDimension(t);
      const auto& value_type = std::get<TypeInformation>(args[1]);
      if ((SupportedTypes::isScalarType(value_type)) && (N <= 3)) {
        return treatStandardTensorialObject(2);
      } else {
        return treatArray1D();
      }
    } else if (t.type == "tfel::math::stensor") {
      return treatStandardTensorialObject(1);
    } else if (t.type == "tfel::math::tensor") {
      return treatStandardTensorialObject(3);
    } else if (t.type == "tfel::math::st2tost2") {
      return treatStandardTensorObjectDerivative(1, 1);
    } else if (isST2toST2TypeAliases(t.type)) {
      SupportedTypes::checkNumberOfTemplateArguments(t, 0u);
      return treatDerivative(treatStandardTensorialObjectBase(0, 1u),
                             treatStandardTensorialObjectBase(0, 1u));
    } else if (t.type == "tfel::math::st2tot2") {
      return treatStandardTensorObjectDerivative(3, 1);
    } else if (t.type == "tfel::math::t2tost2") {
      return treatStandardTensorObjectDerivative(1, 3);
    } else if (t.type == "tfel::math::t2tot2") {
      return treatStandardTensorObjectDerivative(3, 3);
    } else if (t.type == "tfel::math::tmatrix") {
      SupportedTypes::checkNumberOfTemplateArguments(t, 3u);
      const auto& args = *(t.template_arguments);
      const auto& N =
          std::get<TypeInformation::IntegerTemplateArgument>(args[0]);
      const auto& M =
          std::get<TypeInformation::IntegerTemplateArgument>(args[1]);
      const auto Nv = SupportedTypes::getSizeOrSpaceDimension(N);
      const auto Mv = SupportedTypes::getSizeOrSpaceDimension(M);
      const auto& value_type = std::get<TypeInformation>(args[2]);
      if ((SupportedTypes::isScalarType(value_type)) &&  //
          (Nv <= 3) && (Mv <= 3)) {
        const auto id1 = treatStandardTensorialObjectBase(Nv, 2);
        const auto id2 = treatStandardTensorialObjectBase(Mv, 2);
        return treatDerivative(id1, id2);
      } else {
        if ((!std::holds_alternative<int>(N)) ||
            (!std::holds_alternative<int>(M))) {
          tfel::raise(
              "SupportedTypes::getTypeIdentifier: "
              "Error while treating type '" +
              SupportedTypes::encode(t) + "'");
        }
        return treatArrayBase({std::get<int>(N), std::get<int>(M)}, value_type);
      }
    } else if (t.type == "tfel::math::derivative_type") {
      SupportedTypes::checkNumberOfTemplateArguments(t, 2u);
      const auto& args = *(t.template_arguments);
      const auto& t1 = std::get<TypeInformation>(args[0]);
      const auto& t2 = std::get<TypeInformation>(args[1]);
      const auto id1 = getTypeIdentifier(t1);
      const auto id2 = getTypeIdentifier(t2);
      return treatDerivative(id1, id2);
    } else if (t.type == "tfel::math::invert_type") {
      SupportedTypes::checkNumberOfTemplateArguments(t, 1u);
      const auto& args = *(t.template_arguments);
      const auto& t1 = std::get<TypeInformation>(args[0]);
      const auto id1 = getTypeIdentifier(t1);
      if (!((id1.first == 0) || (id1.first == 1) || (id1.first == 3))) {
        tfel::raise(
            "SupportedTypes::getTypeIdentifier: "
            "unsupported case for type '" +
            encode(t) + "'");
      }
      return id1;
    }
    tfel::raise(
        "SupportedTypes::getTypeIdentifier: "
        "unsupported type identifier for '" +
        encode(t) + "'");
  }  // end of getTypeIdentifier

  std::optional<SupportedTypes::TypeFlag> SupportedTypes::getTypeFlag(
      const TypeInformation& t) {
    const auto& flags = SupportedTypes_getFlags();
    const auto p = flags.find(t.type);
    if (p != flags.end()) {
      SupportedTypes::checkNumberOfTemplateArguments(t, 0u);
      return p->second;
    }
    if ((t.type == "tfel::math::quantity") || (t.type == "mfront_quantity")) {
      return SupportedTypes::SCALAR;
    } else if (t.type == "tfel::math::tvector") {
      return SupportedTypes::TVECTOR;
    } else if (t.type == "tfel::math::stensor") {
      return SupportedTypes::STENSOR;
    } else if (t.type == "tfel::math::tensor") {
      return SupportedTypes::TENSOR;
    } else if (t.type == "tfel::math::result_type") {
      SupportedTypes::checkNumberOfTemplateArguments(t, 3u);
      const auto& args = *(t.template_arguments);
      const auto& t1 = std::get<TypeInformation>(args[0]);
      const auto& t2 = std::get<TypeInformation>(args[1]);
      const auto f1 = SupportedTypes::getTypeFlag(t1);
      const auto f2 = SupportedTypes::getTypeFlag(t2);
      if ((!f1) || (!f2)) {
        return {};
      }
      SupportedTypes::checkNumberOfTemplateArguments(
          std::get<TypeInformation>(args[2]), 0u);
      const auto op = std::get<TypeInformation>(args[2]).type;
      if ((f1 != SupportedTypes::SCALAR) && (f2 != SupportedTypes::SCALAR)) {
        const auto N1 = getTensorialObjectSpaceDimension(t1);
        const auto N2 = getTensorialObjectSpaceDimension(t2);
        if (N1 != N2) {
          return {};
        }
      }
      const auto result_flags = buildResultTypeFlags();
      const auto pflag = result_flags.find(std::make_tuple(*f1, *f2, op));
      if (pflag == result_flags.end()) {
        return {};
      }
      return pflag->second;
    } else if (t.type == "tfel::math::derivative_type") {
      SupportedTypes::checkNumberOfTemplateArguments(t, 2u);
      const auto& args = *(t.template_arguments);
      const auto& t1 = std::get<TypeInformation>(args[0]);
      const auto& t2 = std::get<TypeInformation>(args[1]);
      const auto of1 = SupportedTypes::getTypeFlag(t1);
      const auto of2 = SupportedTypes::getTypeFlag(t2);
      if ((!of1) || (!of2)) {
        return {};
      }
      const auto [f1, f2] = std::make_tuple(*of1, *of2);
      if ((f1 == SupportedTypes::SCALAR) && (f2 == SupportedTypes::SCALAR)) {
        return SupportedTypes::SCALAR;
      } else if ((f1 == SupportedTypes::TVECTOR) &&
                 (f2 == SupportedTypes::SCALAR)) {
        return SupportedTypes::TVECTOR;
      } else if ((f1 == SupportedTypes::SCALAR) &&
                 (f2 == SupportedTypes::TVECTOR)) {
        return SupportedTypes::TVECTOR;
      } else if ((f1 == SupportedTypes::STENSOR) &&
                 (f2 == SupportedTypes::SCALAR)) {
        return SupportedTypes::STENSOR;
      } else if ((f1 == SupportedTypes::SCALAR) &&
                 (f2 == SupportedTypes::STENSOR)) {
        return SupportedTypes::STENSOR;
      } else if ((f1 == SupportedTypes::TENSOR) &&
                 (f2 == SupportedTypes::SCALAR)) {
        return SupportedTypes::TENSOR;
      } else if ((f1 == SupportedTypes::SCALAR) &&
                 (f2 == SupportedTypes::TENSOR)) {
        return SupportedTypes::TENSOR;
      } else {
        return {};
      }
    } else if (t.type == "tfel::math::invert_type") {
      SupportedTypes::checkNumberOfTemplateArguments(t, 1u);
      const auto& args = *(t.template_arguments);
      const auto f1 =
          SupportedTypes::getTypeFlag(std::get<TypeInformation>(args[0]));
      if (f1 == SupportedTypes::SCALAR) {
        return SupportedTypes::SCALAR;
      } else {
        return {};
      }
    }
    return {};
  }  // end of SupportedTypes::getTypeFlag

  SupportedTypes::TypeFlag SupportedTypes::getTypeFlag(
      const std::string_view type) {
    const auto oflag = SupportedTypes::getTypeFlag(
        SupportedTypes::getTypeInformation(type, TypeParsingOptions()));
    if (!oflag) {
      tfel::raise("SupportedTypes::getTypeTag: '" + std::string{type} +
                  "' has no associated flag.");
    }
    return *(oflag);
  }  // end of getTypeFlag

  SupportedTypes::TypeSize SupportedTypes::getTypeSize(
      const std::string_view type, const unsigned short a) {
    const auto t =
        SupportedTypes::getTypeInformation(type, TypeParsingOptions());
    return SupportedTypes::getTypeSize(t, a);
  }

  SupportedTypes::TypeSize SupportedTypes::getTypeSize(const TypeInformation& t,
                                                       const unsigned short a) {
    // first looking for aliases
    const auto& flags = SupportedTypes_getFlags();
    const auto p = flags.find(t.type);
    if (p != flags.end()) {
      SupportedTypes::checkNumberOfTemplateArguments(t, 0u);
      return TypeSize(a, p->second);
    }
    //
    auto treatFourthOrderTensor = [&t, a](const TypeFlag& f1,
                                          const TypeFlag& f2) {
      auto N = getTensorialObjectSpaceDimension(t);
      if (N == 0) {
        return TypeSize::getDerivativeSize(a, f1, f2);
      }
      const auto s1 = TypeSize(f1).getValueForDimension(N);
      const auto s2 = TypeSize(f2).getValueForDimension(N);
      return TypeSize(a * s1 * s2, SupportedTypes::SCALAR);
    };
    //
    const auto st2tost2_alias = getST2toST2TypeAliases();
    if (std::find(st2tost2_alias.begin(), st2tost2_alias.end(), t.type) !=
        st2tost2_alias.end()) {
      SupportedTypes::checkNumberOfTemplateArguments(t, 0u);
      return TypeSize::getDerivativeSize(a, SupportedTypes::STENSOR,
                                         SupportedTypes::STENSOR);
    }
    //
    if (t.type == "tfel::math::st2tost2") {
      return treatFourthOrderTensor(SupportedTypes::STENSOR,
                                    SupportedTypes::STENSOR);
    } else if (t.type == "tfel::math::t2tot2") {
      return treatFourthOrderTensor(SupportedTypes::TENSOR,
                                    SupportedTypes::TENSOR);
    } else if (t.type == "tfel::math::st2tot2") {
      return treatFourthOrderTensor(SupportedTypes::TENSOR,
                                    SupportedTypes::STENSOR);
    } else if (t.type == "tfel::math::t2tost2") {
      return treatFourthOrderTensor(SupportedTypes::STENSOR,
                                    SupportedTypes::TENSOR);
    } else if (t.type == "tfel::math::result_type") {
      // here we support only a limited number of operations as the most obvious
      // ones are already treated in the getTypeFlag method
      SupportedTypes::checkNumberOfTemplateArguments(t, 3u);
      const auto& args = *(t.template_arguments);
      const auto& t1 = std::get<TypeInformation>(args[0]);
      const auto& t2 = std::get<TypeInformation>(args[1]);
      const auto op = std::get<TypeInformation>(args[2]);
      SupportedTypes::checkNumberOfTemplateArguments(op, 0u);
      if ((op.type == "tfel::math::OpPlus") ||
          (op.type == "tfel::math::OpMinus")) {
        const auto s1 = SupportedTypes::getTypeSize(t1, 1u);
        const auto s2 = SupportedTypes::getTypeSize(t2, 1u);
        if (s1 != s2) {
          tfel::raise(
              "SupportedTypes::getTypeSize: "
              "unsupported result of operation.");
        }
        return a * s1;
      } else if (op.type == "tfel::math::OpMult") {
        const auto of1 = SupportedTypes::getTypeFlag(t1);
        const auto of2 = SupportedTypes::getTypeFlag(t2);
        if ((of1) && (*of1 == SupportedTypes::SCALAR)) {
          return SupportedTypes::getTypeSize(t2, a);
        } else if ((of2) && (*of2 == SupportedTypes::SCALAR)) {
          return SupportedTypes::getTypeSize(t1, a);
        } else {
          tfel::raise("SupportedTypes::getTypeSize: unsupported type.");
        }
      } else if (op.type == "tfel::math::OpDiv") {
        const auto of2 = SupportedTypes::getTypeFlag(t2);
        if ((of2) && (*of2 == SupportedTypes::SCALAR)) {
          return SupportedTypes::getTypeSize(t1, a);
        } else {
          tfel::raise("SupportedTypes::getTypeSize: unsupported type.");
        }
      } else {
        tfel::raise("SupportedTypes::getTypeSize: invalid operation '" +
                    op.type + "'.");
      }
    } else if (t.type == "tfel::math::derivative_type") {
      SupportedTypes::checkNumberOfTemplateArguments(t, 2u);
      const auto& args = *(t.template_arguments);
      const auto& t1 = std::get<TypeInformation>(args[0]);
      const auto& t2 = std::get<TypeInformation>(args[1]);
      const auto s1 = SupportedTypes::getTypeSize(t1, 1u);
      const auto s2 = SupportedTypes::getTypeSize(t2, 1u);
      return a * SupportedTypes::TypeSize::getDerivativeSize(s1, s2);
    } else if (t.type == "tfel::math::invert_type") {
      SupportedTypes::checkNumberOfTemplateArguments(t, 1u);
      const auto& args = *(t.template_arguments);
      const auto& t1 = std::get<TypeInformation>(args[0]);
      const auto s1 = SupportedTypes::getTypeSize(t1, 1u);
      return a * s1;
    }
    // this must be called **after** looking for all the other possibilities
    const auto oflag = SupportedTypes::getTypeFlag(t);
    if (oflag) {
      if (*oflag == SupportedTypes::SCALAR) {
        return TypeSize(a, *oflag);
      } else {
        const auto N = getTensorialObjectSpaceDimension(t);
        if (N == 0) {
          return TypeSize(a, *oflag);
        } else {
          const auto s = TypeSize(*oflag);
          return TypeSize(a * s.getValueForDimension(N),
                          SupportedTypes::SCALAR);
        }
      }
    }
    //
    tfel::raise(
        "SupportedTypes::getTypeSize: "
        "can't determine of size of given type "
        "('" +
        t.type + "')");
  }  // end of getTypeSize

  std::string SupportedTypes::getTimeDerivativeType(
      const std::string_view type) {
    if (type == "real") {
      return "frequency";
    } else if (type == "strain") {
      return "strainrate";
    } else if (type == "stress") {
      return "stressrate";
    } else if (type == "Stensor") {
      return "FrequencyStensor";
    } else if (type == "StressStensor") {
      return "StressRateStensor";
    } else if (type == "StrainStensor") {
      return "StrainRateStensor";
    } else if (type == "DeformationGradientTensor") {
      return "DeformationGradientRateTensor";
    } else if (type == "Tensor") {
      return "FrequencyTensor";
    }
    return "tfel::math::derivative_type<" + extractType(type) + ",time>";
  }

  bool SupportedTypes::TypeSize::matches(const Monomial& m1,
                                         const Monomial& m2) {
    return (m1.tvector_exponent == m2.tvector_exponent) &&
           (m1.stensor_exponent == m2.stensor_exponent) &&
           (m1.tensor_exponent == m2.tensor_exponent);
  }  // end of matches

  std::vector<SupportedTypes::TypeSize::Monomial>::iterator
  SupportedTypes::TypeSize::findMatchingMonomial(const Monomial& m) {
    return std::find_if(
        this->monomials.begin(), this->monomials.end(),
        [&m](const Monomial& m2) { return TypeSize::matches(m, m2); });
  }  // end of findMatchingMonomial

  std::vector<SupportedTypes::TypeSize::Monomial>::const_iterator
  SupportedTypes::TypeSize::findMatchingMonomial(const Monomial& m) const {
    return std::find_if(
        this->monomials.begin(), this->monomials.end(),
        [&m](const Monomial& m2) { return TypeSize::matches(m, m2); });
  }  // end of findMatchingMonomial

  bool SupportedTypes::TypeSize::operator==(const TypeSize& rhs) const {
    if (this->monomials.size() != rhs.monomials.size()) {
      return false;
    }
    for (const auto& m : this->monomials) {
      const auto pm = rhs.findMatchingMonomial(m);
      if (pm == rhs.monomials.end()) {
        return false;
      }
      if (pm->array_size != m.array_size) {
        return false;
      }
    }
    return true;
  }  // end of SupportedTypes::TypeSize::operator==

  bool SupportedTypes::TypeSize::operator!=(const TypeSize& rhs) const {
    return !this->operator==(rhs);
  }  // end of SupportedTypes::TypeSize::operator!=

  bool SupportedTypes::TypeSize::isNull() const {
    return this->monomials.empty();
  }  // end of SupportedTypes::TypeSize::isNull

  bool SupportedTypes::TypeSize::isOne() const {
    if (this->monomials.size() != 1) {
      return false;
    }
    const auto& m = this->monomials[0];
    return ((m.array_size == 1u) && (m.tvector_exponent == 0) &&
            (m.stensor_exponent == 0) && (m.tensor_exponent == 0));
  }  // end of SupportedTypes::TypeSize::isOne

  std::optional<int> SupportedTypes::extractInteger(const std::string_view s) {
    const auto s2 = [&s]() {
      if (s.empty()) {
        return s;
      }
      if ((s.back() == 'u') || (s.back() == 'U') || (s.back() == 'l') ||
          (s.back() == 'L')) {
        return s.substr(0, s.size() - 1);
      } else if ((tfel::utilities::ends_with(s, "ul")) ||
                 (tfel::utilities::ends_with(s, "uL")) ||
                 (tfel::utilities::ends_with(s, "lu")) ||
                 (tfel::utilities::ends_with(s, "Lu")) ||
                 (tfel::utilities::ends_with(s, "Ul")) ||
                 (tfel::utilities::ends_with(s, "UL")) ||
                 (tfel::utilities::ends_with(s, "lU")) ||
                 (tfel::utilities::ends_with(s, "LU"))) {
        return s.substr(0, s.size() - 2);
      }
      return s;
    }();
    if (s2.empty()) {
      return {};
    }
    auto p = s2.begin();
    if (*p == '-') {
      ++p;
    }
    for (; p != s2.end(); ++p) {
      if (!std::isdigit(*p)) {
        return {};
      }
    }
    // convertion
    int r;
    auto result = std::from_chars(s2.data(), s2.data() + s2.size(), r);
    if (result.ec != std::errc()) {
      return {};
    }
    return r;
  }  // end of extractInteger

  void SupportedTypes::checkIteratorValidity(const const_iterator c,
                                             const const_iterator& e) {
    if (c == e) {
      tfel::raise(
          "SupportedTypes::checkIteratorValidity: "
          "invalid token iterator");
    }
  }  // end of checkCurrentValueAndIncrementIterator

  void SupportedTypes::checkCurrentValueAndIncrementIterator(
      const_iterator& c, const const_iterator& e, const std::string_view v) {
    SupportedTypes::checkIteratorValidity(c, e);
    if (c->value != v) {
      tfel::raise(
          "SupportedTypes::checkIteratorValidity: "
          "invalid value (expected '" +
          std::string{v} + "', read '" + c->value + "')");
    }
    ++c;
  }  // end of checkIteratorValidity

  void SupportedTypes::normalizeRawScalarType(const TypeInformation& t) {
    const auto oflag = SupportedTypes::getTypeFlag(t);
    if ((!oflag.has_value()) || (*oflag != SupportedTypes::SCALAR)) {
      tfel::raise(
          "SupportedTypes::normalizeRawScalarType: "
          "unsupported scalar type '" +
          SupportedTypes::encode(t) + "'");
    }
    if (t.template_arguments) {
      tfel::raise(
          "SupportedTypes::normalizeRawScalarType: "
          "no template argument expected "
          "for scalar type '" +
          SupportedTypes::encode(t) + "'");
    }
  }  // end of normalizeRawScalarType

  bool SupportedTypes::matchesTFELMathType(const std::string_view t,
                                           const std::string& type) {
    return (t == type) || (t == "tfel::math::" + type) ||
           (t == "::tfel::math::" + type);
  }  // end of matchesTFELMathType

  void SupportedTypes::normalizeQuantity(TypeInformation& t,
                                         const TypeParsingOptions& opts) {
    if (!t.template_arguments) {
      tfel::raise(
          "SupportedTypes::normalizeQuantity: "
          "quantity must have template arguments");
    }
    auto& args = *(t.template_arguments);
    if (args.size() > 8u) {
      tfel::raise(
          "SupportedTypes::normalizeQuantity: "
          "quantity must have at most eight template arguments");
    }
    if (args.empty()) {
      TypeInformation numeric_type;
      numeric_type.type = "real";
      args.push_back(numeric_type);
    } else {
      if (!std::holds_alternative<TypeInformation>(args[0])) {
        tfel::raise(
            "SupportedTypes::normalizeQuantity: "
            "quantity must have at most eight template arguments");
      }
      SupportedTypes::normalizeRawScalarType(
          std::get<TypeInformation>(args[0]));
      for (decltype(args.size()) i = 1; i != args.size(); ++i) {
        if (!(std::holds_alternative<TypeInformation::IntegerTemplateArgument>(
                args[i]))) {
          tfel::raise(
              "SupportedTypes::normalizeQuantity: "
              "invalid template argument");
        }
      }
    }
    if (opts.use_qt) {
      // complete arguments
      for (decltype(args.size()) i = args.size(); i != 8u; ++i) {
        args.push_back(0);
      }
      t.type = "mfront_quantity";
    } else {
      t.template_arguments.reset();
      t.type = "real";
    }
  }  // end of normalizeQuantity

  void SupportedTypes::normalizeScalarType(TypeInformation& t,
                                           const TypeParsingOptions& opts) {
    if (SupportedTypes::matchesTFELMathType(t.type, "quantity")) {
      SupportedTypes::normalizeQuantity(t, opts);
    } else {
      SupportedTypes::normalizeRawScalarType(t);
    }
  }  // end of SupportedTypes::normalizeScalarType

  bool SupportedTypes::checkIntegerTemplateArgument(
      const TypeInformation::TemplateArgument& N) {
    return std::holds_alternative<TypeInformation::IntegerTemplateArgument>(N);
  }  // end of checkIntegerTemplateArgument

  void SupportedTypes::normalizeTinyVectorTemplateArguments(
      TypeInformation& t, const TypeParsingOptions& opts) {
    if (!t.template_arguments) {
      tfel::raise("tiny vectors must have 2 template arguments");
    }
    auto& args = *(t.template_arguments);
    if (args.size() != 2u) {
      tfel::raise("tiny vectors must have 2 template arguments");
    }
    if (!std::holds_alternative<TypeInformation::IntegerTemplateArgument>(
            args[0])) {
      tfel::raise(
          "tiny vector must have an integer as first template argument");
    }
    if (!std::holds_alternative<TypeInformation>(args[1])) {
      tfel::raise("tiny vector must have a type as second template argument");
    }
    SupportedTypes::normalize(std::get<TypeInformation>(args[1]), opts);
  }  // end of normalizeTinyVectorTemplateArguments

  void SupportedTypes::normalizeTinyMatrixTemplateArguments(
      TypeInformation& t, const TypeParsingOptions& opts) {
    if (!t.template_arguments) {
      tfel::raise("tiny matrices must have 3 template arguments");
    }
    auto& args = *(t.template_arguments);
    if (args.size() != 3u) {
      tfel::raise("tiny matrices must have 3 template arguments");
    }
    if (!((std::holds_alternative<TypeInformation::IntegerTemplateArgument>(
              args[0])) &&
          (std::holds_alternative<TypeInformation::IntegerTemplateArgument>(
              args[1])))) {
      tfel::raise(
          "tiny matrices must have integers as their two first template "
          "arguments");
    }
    if (!std::holds_alternative<TypeInformation>(args[2])) {
      tfel::raise("tiny matrices must have a type as third");
    }
    SupportedTypes::normalize(std::get<TypeInformation>(args[2]), opts);
  }  // end of normalizeTinyMatrixTemplateArguments

  int SupportedTypes::getTensorialObjectSpaceDimension(
      const TypeInformation& t) {
    auto raise = [&t](const std::string_view msg) {
      tfel::raise("SupportedTypes::getTensorialObjectSpaceDimension: " +
                  std::string{msg} + " (invalid type '" + t.type + "')");
    };
    auto report_invalid_space_dimension = [&raise] {
      raise(
          "the first template argument of a tensorial object "
          "must be equal to 1, 2, 3 or N");
    };
    if (!t.template_arguments) {
      raise("tensor objects must have 2 template arguments");
    }
    auto& args = *(t.template_arguments);
    if (args.size() != 2u) {
      raise("tensor objects must have 2 template arguments");
    }
    if (!std::holds_alternative<TypeInformation::IntegerTemplateArgument>(
            args[0])) {
      raise("tensorial object must have an integer as first template argument");
    }
    const auto N = SupportedTypes::getSizeOrSpaceDimension(
        std::get<TypeInformation::IntegerTemplateArgument>(args[0]));
    if (N > 3) {
      report_invalid_space_dimension();
    }
    return N;
  }  // end of getTensorialObjectSpaceDimension

  void SupportedTypes::normalizeTensorialTypeTemplateArguments(
      TypeInformation& t, const TypeParsingOptions& opts) {
    if (!t.template_arguments) {
      tfel::raise(
          "SupportedTypes::normalizeTensorialTypeTemplateArguments: "
          "tensor objects must have 2 template arguments. "
          "Error while parsing type '" +
          SupportedTypes::encode(t) + "'");
    }
    auto& args = *(t.template_arguments);
    if (args.size() != 2u) {
      tfel::raise(
          "SupportedTypes::normalizeTensorialTypeTemplateArguments: "
          "tensor objects must have 2 template arguments. "
          "Error while parsing type '" +
          SupportedTypes::encode(t) + "'");
    }
    // check if the space dimension is correctly defined
    getTensorialObjectSpaceDimension(t);
    if (!std::holds_alternative<TypeInformation>(args[1])) {
      tfel::raise(
          "SupportedTypes::normalizeTensorialTypeTemplateArguments: "
          "tensorial object must have a type as template argument. "
          "Error while parsing type '" +
          SupportedTypes::encode(t) + "'");
    }
    //
    SupportedTypes::normalizeScalarType(std::get<TypeInformation>(args[1]),
                                        opts);
  }  // end of normalizeTensorialTypeTemplateArguments

  std::vector<std::string>
  SupportedTypes::getSupportedTFELMathTensorialTypes() {
    return {"stensor", "tensor", "st2tost2", "t2tot2", "st2tot2", "t2tost2"};
  }  // end of getSupportedTFELMathTensorialTypes

  void SupportedTypes::normalize(TypeInformation& t,
                                 const TypeParsingOptions& opts) {
    for (const auto& ttype :
         SupportedTypes::getSupportedTFELMathTensorialTypes()) {
      if (SupportedTypes::matchesTFELMathType(t.type, ttype)) {
        SupportedTypes::normalizeTensorialTypeTemplateArguments(t, opts);
        t.type = "tfel::math::" + ttype;
        return;
      }
    }
    if (SupportedTypes::matchesTFELMathType(t.type, "fsarray")) {
      SupportedTypes::normalizeTinyVectorTemplateArguments(t, opts);
      t.type = "tfel::math::fsarray";
      return;
    }
    if (SupportedTypes::matchesTFELMathType(t.type, "tvector")) {
      SupportedTypes::normalizeTinyVectorTemplateArguments(t, opts);
      t.type = "tfel::math::tvector";
      return;
    }
    if (SupportedTypes::matchesTFELMathType(t.type, "tmatrix")) {
      SupportedTypes::normalizeTinyMatrixTemplateArguments(t, opts);
      t.type = "tfel::math::tmatrix";
      return;
    }
    if (SupportedTypes::matchesTFELMathType(t.type, "invert_type")) {
      if (!t.template_arguments) {
        tfel::raise("invert_type must have 1 template arguments");
      }
      auto& args = *(t.template_arguments);
      if (args.size() != 1u) {
        tfel::raise("invert_type must have an unique template argument");
      }
      if (std::holds_alternative<TypeInformation::IntegerTemplateArgument>(
              args[0])) {
        tfel::raise("invert_type must have a type as template argument");
      }
      SupportedTypes::normalize(std::get<TypeInformation>(args[0]), opts);
      t.type = "tfel::math::invert_type";
    } else if (SupportedTypes::matchesTFELMathType(t.type, "result_type")) {
      if (!t.template_arguments) {
        tfel::raise("result_type must have 3 template arguments");
      }
      auto& args = *(t.template_arguments);
      if (args.size() != 3u) {
        tfel::raise("result_type must have a three template arguments");
      }
      if (std::holds_alternative<TypeInformation::IntegerTemplateArgument>(
              args[0]) ||
          std::holds_alternative<TypeInformation::IntegerTemplateArgument>(
              args[1]) ||
          std::holds_alternative<TypeInformation::IntegerTemplateArgument>(
              args[2])) {
        tfel::raise("result_type must have only types as template argument");
      }
      auto& op = std::get<TypeInformation>(args[2]).type;
      if ((op != "OpPlus") && (op != "tfel::math::OpPlus") &&    //
          (op != "OpMinus") && (op != "tfel::math::OpMinus") &&  //
          (op != "OpMult") && (op != "tfel::math::OpMult") &&    //
          (op != "OpDiv") && (op != "tfel::math::OpDiv")) {
        tfel::raise("unsupported operation in result_type");
      }
      if ((op == "OpPlus") || (op == "OpMinus") ||  //
          (op == "OpMult") || (op == "OpDiv")) {
        op = "tfel::math::" + op;
      }
      SupportedTypes::normalize(std::get<TypeInformation>(args[0]), opts);
      SupportedTypes::normalize(std::get<TypeInformation>(args[1]), opts);
      t.type = "tfel::math::result_type";
    } else if (SupportedTypes::matchesTFELMathType(t.type, "derivative_type")) {
      if (!t.template_arguments) {
        tfel::raise("derivative_type must have 2 template arguments");
      }
      auto& args = *(t.template_arguments);
      if (args.size() != 2u) {
        tfel::raise("derivative_type must have a two template arguments");
      }
      if (std::holds_alternative<TypeInformation::IntegerTemplateArgument>(
              args[0]) ||
          std::holds_alternative<TypeInformation::IntegerTemplateArgument>(
              args[1])) {
        tfel::raise(
            "derivative_type must have only types as template argument");
      }
      SupportedTypes::normalize(std::get<TypeInformation>(args[0]), opts);
      SupportedTypes::normalize(std::get<TypeInformation>(args[1]), opts);
      t.type = "tfel::math::derivative_type";
    } else if (SupportedTypes::matchesTFELMathType(t.type, "quantity")) {
      SupportedTypes::normalizeScalarType(t, opts);
    } else {
      if (t.template_arguments) {
        for (auto& arg : *(t.template_arguments)) {
          if (std::holds_alternative<TypeInformation>(arg)) {
            SupportedTypes::normalize(std::get<TypeInformation>(arg), opts);
          }
        }
      }
    }
  }  // end of normalize

  std::string SupportedTypes::encode(const TypeInformation& t) {
    auto type = std::string{};
    type = t.type;
    if (t.template_arguments) {
      type += "<";
      auto pa = (*(t.template_arguments)).cbegin();
      const auto pae = (*(t.template_arguments)).cend();
      while (pa != pae) {
        if (std::holds_alternative<TypeInformation::IntegerTemplateArgument>(
                *pa)) {
          const auto& ia =
              std::get<TypeInformation::IntegerTemplateArgument>(*pa);
          if (std::holds_alternative<int>(ia)) {
            type += std::to_string(std::get<int>(ia));
          } else {
            type += std::get<std::string>(ia);
          }
        } else {
          type += encode(std::get<TypeInformation>(*pa));
        }
        if (++pa != pae) {
          type += ",";
        }
      }
      type += ">";
    }
    return type;
  }  // end of encode

  SupportedTypes::TypeInformation SupportedTypes::getTypeInformation(
      const std::string_view t, const TypeParsingOptions& opts) {
    tfel::utilities::CxxTokenizer tokenizer;
    tokenizer.parseString(std::string{t});
    auto c = tokenizer.begin();
    auto r = SupportedTypes::parseType(c, tokenizer.end(), opts);
    if (!r.second) {
      tfel::raise("SupportedTypes::getTypeInformation: invalid type '" +
                  std::string{t} + "' (unbalanced '>')");
    }
    if (c != tokenizer.end()) {
      tfel::raise("SupportedTypes::getTypeInformation: invalid type '" +
                  std::string{t} + "'");
    }
    SupportedTypes::normalize(r.first, opts);
    return r.first;
  }  // end of getTypeInformation

  std::string SupportedTypes::extractType(const std::string_view t,
                                          const TypeParsingOptions& opts) {
    auto i = SupportedTypes::getTypeInformation(t, opts);
    return SupportedTypes::encode(i);
  }  // end of extractType

  std::string SupportedTypes::extractType(const_iterator& current,
                                          const const_iterator& end,
                                          const TypeParsingOptions& opts) {
    auto r = SupportedTypes::parseType(current, end, opts);
    if (!r.second) {
      tfel::raise("SupportedTypes::extractType: unbalanced '>'");
    }
    SupportedTypes::normalize(r.first, opts);
    return SupportedTypes::encode(r.first);
  }  // end of extractType

  std::pair<SupportedTypes::TypeInformation, bool> SupportedTypes::parseType(
      const_iterator& current,
      const const_iterator& end,
      const TypeParsingOptions& opts) {
    using tfel::utilities::CxxTokenizer;
    auto throw_if = [](const bool b, const std::string_view m) {
      if (b) {
        tfel::raise("SupportedTypes::parseType: " + std::string(m));
      }
    };
    SupportedTypes::checkIteratorValidity(current, end);
    auto type = TypeInformation{};
    if (current->value != "::") {
      type.type = current->value;
      throw_if(!CxxTokenizer::isValidIdentifier(type.type, false),
               "SupportedTypes::parseType: given type '" + type.type +
                   "' is not valid.");
      ++(current);
      if (current == end) {
        return {type, true};
      }
    }
    while (current->value == "::") {
      ++(current);
      SupportedTypes::checkIteratorValidity(current, end);
      const auto t = current->value;
      throw_if(
          !CxxTokenizer::isValidIdentifier(t, false),
          "SupportedTypes::parseType: given token '" + t + "' is not valid.");
      type.type += "::" + t;
      ++(current);
      if (current == end) {
        return {type, true};
      }
    }
    if (current->value != "<") {
      return {type, true};
    }
    // read template arguments list
    type.template_arguments = std::vector<TypeInformation::TemplateArgument>{};
    auto& targs = *(type.template_arguments);
    SupportedTypes::checkIteratorValidity(current, end);
    ++(current);
    SupportedTypes::checkIteratorValidity(current, end);
    if (current->value == ">") {
      ++(current);
      return {type, true};
    }
    if (current->value == ">>") {
      ++(current);
      return {type, false};
    }
    auto c = true;
    while (c) {
      auto isOperation = [](const auto& v) {
        return (v == "+") || (v == "-") || (v == "*") || (v == "/");
      };
      auto n = std::next(current);
      if (n == end) {
        tfel::raise(
            "SupportedTypes::parseType: "
            "invalid end of type definition");
      }
      if (isOperation(n->value)) {
        // formula defining and integer
        auto f = std::string{};
        while ((current->value != ">") && (current->value != ">>") &&
               (current->value != ",")) {
          const auto i = SupportedTypes::extractInteger(current->value);
          if (i) {
            f += std::to_string(*i);
          } else {
            throw_if(!CxxTokenizer::isValidIdentifier(current->value, false),
                     "SupportedTypes::parseType: invalid token '" +
                         current->value + "'");
            const auto pic = opts.integer_constants.find(current->value);
            const auto biv =
                SupportedTypes::isSupportedConstexprIntegerVariables(
                    current->value);
            if (pic != opts.integer_constants.end()) {
              f += std::to_string(pic->second);
            } else if (biv) {
              f += current->value;
            } else {
              tfel::raise("SupportedTypes::parseType: unexpected token '" +
                          current->value + "'");
            }
          }
          ++(current);
          SupportedTypes::checkIteratorValidity(current, end);
          if ((current->value != ">") && (current->value != ">>") &&
              (current->value != ",")) {
            if (!isOperation(current->value)) {
              throw_if(!CxxTokenizer::isValidIdentifier(current->value, false),
                       "SupportedTypes::parseType: invalid token '" +
                           current->value + "'");
            }
            f += current->value;
            ++(current);
            SupportedTypes::checkIteratorValidity(current, end);
            if ((current->value == ">") || ((current->value == ","))) {
              throw_if(!CxxTokenizer::isValidIdentifier(current->value, false),
                       "SupportedTypes::parseType: unexpected token '" +
                           current->value + "'");
            }
          }
        }
        tfel::math::IntegerEvaluator formula(f);
        const auto all_variables_defined = [&opts, &formula]() {
          for (const auto& v : formula.getVariablesNames()) {
            if (opts.integer_constants.count(v) == 0) {
              return false;
            }
          }
          return true;
        }();
        if (all_variables_defined) {
          for (const auto& v : formula.getVariablesNames()) {
            formula.setVariableValue(v, opts.integer_constants.at(v));
          }
          targs.push_back(formula.getValue());
        } else {
          targs.push_back(f);
        }
        if (current->value == ">") {
          ++(current);
          return {type, true};
        }
        if (current->value == ">>") {
          ++(current);
          return {type, false};
        }
      } else {
        const auto o = SupportedTypes::extractInteger(current->value);
        const auto pic = opts.integer_constants.find(current->value);
        const auto biv = SupportedTypes::isSupportedConstexprIntegerVariables(
            current->value);
        if (o) {
          targs.push_back(*o);
          ++(current);
        } else if (pic != opts.integer_constants.end()) {
          targs.push_back(pic->second);
          ++(current);
        } else if (biv) {
          targs.push_back(current->value);
          ++(current);
        } else {
          const auto r = SupportedTypes::parseType(current, end, opts);
          targs.push_back(r.first);
          if (!r.second) {
            return {type, true};
          }
        }
      }
      if (c) {
        SupportedTypes::checkIteratorValidity(current, end);
        if (current->value == ",") {
          SupportedTypes::checkCurrentValueAndIncrementIterator(current, end,
                                                                ",");
        } else {
          c = false;
        }
      }
    }
    if (current->value == ">>") {
      ++(current);
      return {type, false};
    }
    SupportedTypes::checkCurrentValueAndIncrementIterator(current, end, ">");
    return {type, true};
  }  // end of parseType

  SupportedTypes::~SupportedTypes() = default;

  std::string to_string(const SupportedTypes::TypeSize& s) {
    std::ostringstream os;
    os << s;
    return os.str();
  }  // end of to_string

}  // end of namespace mfront
