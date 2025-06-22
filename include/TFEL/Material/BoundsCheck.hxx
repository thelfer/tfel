/*!
 * \file   include/TFEL/Material/BoundsCheck.hxx
 * \brief  This file declares the  class
 * \author Thomas Helfer
 * \date   06 Mar 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_BOUNDSCHECK_HXX
#define LIB_TFEL_BOUNDSCHECK_HXX

#include <string>
#include <string_view>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/Material/OutOfBoundsPolicy.hxx"

// fixing a bug on current glibc++ cygwin versions (19/08/2015)
#if defined __CYGWIN__ && (!defined _GLIBCXX_USE_C99)
#include <sstream>
namespace std {
  template <typename T>
  std::string to_string(const T& v) {
    std::ostringstream s;
    s << v;
    return s.str();
  }
}  // namespace std
#endif /* defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99) */

namespace tfel::material {

  /*!
   * \brief an helper class
   */
  struct BoundsCheckBase {
    /*!
     * \brief convert to string
     * \param[in] value: value to be converted
     */
    template <typename T>
    static std::string convert_to_string(const T& value) {
      if constexpr (tfel::math::isQuantity<T>()) {
        return std::to_string(value.getValue());
      } else {
        return std::to_string(value);
      }
    }

    /*!
     * \param[in] n : variable name
     * \param[in] v : value
     * \param[in] b : lower bound value
     */
    [[noreturn]] TFEL_VISIBILITY_EXPORT static void
    throwOutOfLowerBoundsException(const std::string_view,
                                   const std::string_view,
                                   const std::string_view);
    /*!
     * \param[in] n : variable name
     * \param[in] v : value
     * \param[in] b : upper bound value
     */
    [[noreturn]] TFEL_VISIBILITY_EXPORT static void
    throwOutOfUpperBoundsException(const std::string_view,
                                   const std::string_view,
                                   const std::string_view);
    /*!
     * \param[in] n  : variable name
     * \param[in] v  : value
     * \param[in] lb : lower bound value
     * \param[in] ub : upper bound value
     */
    [[noreturn]] TFEL_VISIBILITY_EXPORT static void throwOutOfBoundsException(
        const std::string_view,
        const std::string_view,
        const std::string_view,
        const std::string_view);
    /*!
     * \param[in] n : variable name
     * \param[in] v : value
     * \param[in] b : lower bound value
     */
    TFEL_VISIBILITY_EXPORT static void displayOutOfLowerBoundsWarning(
        const std::string_view, const std::string_view, const std::string_view);
    /*!
     * \param[in] n : variable name
     * \param[in] v : value
     * \param[in] b : upper bound value
     */
    TFEL_VISIBILITY_EXPORT static void displayOutOfUpperBoundsWarning(
        const std::string_view, const std::string_view, const std::string_view);
    /*!
     * \param[in] n  : variable name
     * \param[in] v  : value
     * \param[in] lb : lower bound value
     * \param[in] ub : upper bound value
     */
    TFEL_VISIBILITY_EXPORT static void displayOutOfBoundsWarning(
        const std::string_view,
        const std::string_view,
        const std::string_view,
        const std::string_view);
    template <typename T>
    static inline void lowerBoundCheck(const std::string_view name,
                                       const T value,
                                       const T lBound,
                                       const OutOfBoundsPolicy p = Strict) {
      if (value < lBound) {
        if (p == None) {
          return;
        }
        const auto value_as_string = convert_to_string(value);
        const auto lower_bound_as_string = convert_to_string(lBound);
        if (p == Strict) {
          throwOutOfLowerBoundsException(name, value_as_string,
                                         lower_bound_as_string);
        }
        displayOutOfLowerBoundsWarning(name, value_as_string,
                                       lower_bound_as_string);
      }
    }
    template <typename T, typename Unit, typename OwnerShipPolicy>
    static inline void lowerBoundCheck(
        const std::string_view name,
        const tfel::math::Quantity<Unit, T, OwnerShipPolicy> value,
        const T lBound,
        const OutOfBoundsPolicy p = Strict) {
      if (value.getValue() < lBound) {
        if (p == None) {
          return;
        }
        const auto value_as_string = convert_to_string(value);
        const auto lower_bound_as_string = convert_to_string(lBound);
        if (p == Strict) {
          throwOutOfLowerBoundsException(name, value_as_string,
                                         lower_bound_as_string);
        }
        displayOutOfLowerBoundsWarning(name, value_as_string,
                                       lower_bound_as_string);
      }
    }
    template <typename T>
    static inline void upperBoundCheck(const std::string_view name,
                                       const T value,
                                       const T uBound,
                                       const OutOfBoundsPolicy p = Strict) {
      if (value > uBound) {
        if (p == None) {
          return;
        }
        const auto value_as_string = convert_to_string(value);
        const auto upper_bound_as_string = convert_to_string(uBound);
        if (p == Strict) {
          throwOutOfUpperBoundsException(name, value_as_string,
                                         upper_bound_as_string);
        }
        displayOutOfUpperBoundsWarning(name, value_as_string,
                                       upper_bound_as_string);
      }
    }
    template <typename T, typename Unit, typename OwnerShipPolicy>
    static inline void upperBoundCheck(
        const std::string_view name,
        const tfel::math::Quantity<Unit, T, OwnerShipPolicy> value,
        const T uBound,
        const OutOfBoundsPolicy p = Strict) {
      if (value.getValue() > uBound) {
        if (p == None) {
          return;
        }
        const auto value_as_string = convert_to_string(value);
        const auto upper_bound_as_string = convert_to_string(uBound);
        if (p == Strict) {
          throwOutOfUpperBoundsException(name, value_as_string,
                                         upper_bound_as_string);
        }
        displayOutOfUpperBoundsWarning(name, value_as_string,
                                       upper_bound_as_string);
      }
    }
    template <typename T>
    static inline void lowerAndUpperBoundsChecks(
        const std::string_view name,
        const T value,
        const T lBound,
        const T uBound,
        const OutOfBoundsPolicy p = Strict) {
      if ((value < lBound) || (value > uBound)) {
        if (p == None) {
          return;
        }
        const auto value_as_string = convert_to_string(value);
        const auto lower_bound_as_string = convert_to_string(lBound);
        const auto upper_bound_as_string = convert_to_string(uBound);
        if (p == Strict) {
          throwOutOfBoundsException(name, value_as_string,
                                    lower_bound_as_string,
                                    upper_bound_as_string);
        }
        displayOutOfBoundsWarning(name, value_as_string, lower_bound_as_string,
                                  upper_bound_as_string);
      }
    }
    template <typename T, typename Unit, typename OwnerShipPolicy>
    static inline void lowerAndUpperBoundsChecks(
        const std::string_view name,
        const tfel::math::Quantity<Unit, T, OwnerShipPolicy> value,
        const T lBound,
        const T uBound,
        const OutOfBoundsPolicy p = Strict) {
      if ((value.getValue() < lBound) || (value.getValue() > uBound)) {
        if (p == None) {
          return;
        }
        const auto value_as_string = convert_to_string(value);
        const auto lower_bound_as_string = convert_to_string(lBound);
        const auto upper_bound_as_string = convert_to_string(uBound);
        if (p == Strict) {
          throwOutOfBoundsException(name, value_as_string,
                                    lower_bound_as_string,
                                    upper_bound_as_string);
        }
        displayOutOfBoundsWarning(name, value_as_string, lower_bound_as_string,
                                  upper_bound_as_string);
      }
    }
  };  // end of struct BoundsCheckBase

  template <unsigned short N>
  struct BoundsCheck;

  template <>
  struct BoundsCheck<1u> : public BoundsCheckBase {
    using BoundsCheckBase::lowerAndUpperBoundsChecks;
    using BoundsCheckBase::lowerBoundCheck;
    using BoundsCheckBase::upperBoundCheck;
    template <typename T>
    static void lowerBoundCheck(const std::string& name,
                                const tfel::math::stensor<1u, T>& s,
                                const T lBound,
                                const OutOfBoundsPolicy p = Strict) {
      BoundsCheckBase::lowerBoundCheck(name + "(0)", s(0), lBound, p);
      BoundsCheckBase::lowerBoundCheck(name + "(1)", s(1), lBound, p);
      BoundsCheckBase::lowerBoundCheck(name + "(2)", s(2), lBound, p);
    }
    template <typename T>
    static void upperBoundCheck(const std::string& name,
                                const tfel::math::stensor<1u, T>& s,
                                const T uBound,
                                const OutOfBoundsPolicy p = Strict) {
      BoundsCheckBase::upperBoundCheck(name + "(0)", s(0), uBound, p);
      BoundsCheckBase::upperBoundCheck(name + "(1)", s(1), uBound, p);
      BoundsCheckBase::upperBoundCheck(name + "(2)", s(2), uBound, p);
    }
    template <typename T>
    static void lowerAndUpperBoundsChecks(const std::string& name,
                                          const tfel::math::stensor<1u, T>& s,
                                          const T lBound,
                                          const T uBound,
                                          const OutOfBoundsPolicy p = Strict) {
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(0)", s(0), lBound,
                                                 uBound, p);
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(1)", s(1), lBound,
                                                 uBound, p);
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(2)", s(2), lBound,
                                                 uBound, p);
    }
    template <typename T, typename Unit, typename OwnerShip>
    static void lowerBoundCheck(
        const std::string& name,
        const tfel::math::stensor<1u, tfel::math::Quantity<Unit, T, OwnerShip>>&
            s,
        const T lBound,
        const OutOfBoundsPolicy p = Strict) {
      BoundsCheckBase::lowerBoundCheck(name + "(0)", s(0).getValue(), lBound,
                                       p);
      BoundsCheckBase::lowerBoundCheck(name + "(1)", s(1).getValue(), lBound,
                                       p);
      BoundsCheckBase::lowerBoundCheck(name + "(2)", s(2).getValue(), lBound,
                                       p);
    }
    template <typename T, typename Unit, typename OwnerShip>
    static void upperBoundCheck(
        const std::string& name,
        const tfel::math::stensor<1u, tfel::math::Quantity<Unit, T, OwnerShip>>&
            s,
        const T uBound,
        const OutOfBoundsPolicy p = Strict) {
      BoundsCheckBase::upperBoundCheck(name + "(0)", s(0).getValue(), uBound,
                                       p);
      BoundsCheckBase::upperBoundCheck(name + "(1)", s(1).getValue(), uBound,
                                       p);
      BoundsCheckBase::upperBoundCheck(name + "(2)", s(2).getValue(), uBound,
                                       p);
    }
    template <typename T, typename Unit, typename OwnerShip>
    static void lowerAndUpperBoundsChecks(
        const std::string& name,
        const tfel::math::stensor<1u, tfel::math::Quantity<Unit, T, OwnerShip>>&
            s,
        const T lBound,
        const T uBound,
        const OutOfBoundsPolicy p = Strict) {
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(0)", s(0).getValue(),
                                                 lBound, uBound, p);
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(1)", s(1).getValue(),
                                                 lBound, uBound, p);
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(2)", s(2).getValue(),
                                                 lBound, uBound, p);
    }
  };  // end of struct BoundsCheck<1u>;

  template <>
  struct BoundsCheck<2u> : public BoundsCheckBase {
    using BoundsCheckBase::lowerAndUpperBoundsChecks;
    using BoundsCheckBase::lowerBoundCheck;
    using BoundsCheckBase::upperBoundCheck;
    template <typename T, typename Unit, typename OwnerShip>
    static inline void lowerAndUpperBoundsChecks(
        const std::string& name,
        const tfel::math::Quantity<Unit, T, OwnerShip> value,
        const T lBound,
        const T uBound,
        const OutOfBoundsPolicy p = Strict) {
      BoundsCheckBase::lowerBoundCheck(name, value.getValue(), lBound, p);
      BoundsCheckBase::upperBoundCheck(name, value.getValue(), uBound, p);
    }
    template <typename T>
    static void lowerBoundCheck(const std::string& name,
                                const tfel::math::stensor<2u, T>& s,
                                const T lBound,
                                const OutOfBoundsPolicy p = Strict) {
      BoundsCheckBase::lowerBoundCheck(name + "(0)", s(0), lBound, p);
      BoundsCheckBase::lowerBoundCheck(name + "(1)", s(1), lBound, p);
      BoundsCheckBase::lowerBoundCheck(name + "(2)", s(2), lBound, p);
      BoundsCheckBase::lowerBoundCheck(name + "(3)", s(3), lBound, p);
    }
    template <typename T>
    static void upperBoundCheck(const std::string& name,
                                const tfel::math::stensor<2u, T>& s,
                                const T uBound,
                                const OutOfBoundsPolicy p = Strict) {
      BoundsCheckBase::upperBoundCheck(name + "(0)", s(0), uBound, p);
      BoundsCheckBase::upperBoundCheck(name + "(1)", s(1), uBound, p);
      BoundsCheckBase::upperBoundCheck(name + "(2)", s(2), uBound, p);
      BoundsCheckBase::upperBoundCheck(name + "(3)", s(3), uBound, p);
    }
    template <typename T>
    static void lowerAndUpperBoundsChecks(const std::string& name,
                                          const tfel::math::stensor<2u, T>& s,
                                          const T lBound,
                                          const T uBound,
                                          const OutOfBoundsPolicy p = Strict) {
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(0)", s(0), lBound,
                                                 uBound, p);
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(1)", s(1), lBound,
                                                 uBound, p);
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(2)", s(2), lBound,
                                                 uBound, p);
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(3)", s(3), lBound,
                                                 uBound, p);
    }
    template <typename T, typename Unit, typename OwnerShip>
    static void lowerBoundCheck(
        const std::string& name,
        const tfel::math::stensor<2u, tfel::math::Quantity<Unit, T, OwnerShip>>&
            s,
        const T lBound,
        const OutOfBoundsPolicy p = Strict) {
      BoundsCheckBase::lowerBoundCheck(name + "(0)", s(0).getValue(), lBound,
                                       p);
      BoundsCheckBase::lowerBoundCheck(name + "(1)", s(1).getValue(), lBound,
                                       p);
      BoundsCheckBase::lowerBoundCheck(name + "(2)", s(2).getValue(), lBound,
                                       p);
      BoundsCheckBase::lowerBoundCheck(name + "(3)", s(3).getValue(), lBound,
                                       p);
    }
    template <typename T, typename Unit, typename OwnerShip>
    static void upperBoundCheck(
        const std::string& name,
        const tfel::math::stensor<2u, tfel::math::Quantity<Unit, T, OwnerShip>>&
            s,
        const T uBound,
        const OutOfBoundsPolicy p = Strict) {
      BoundsCheckBase::upperBoundCheck(name + "(0)", s(0).getValue(), uBound,
                                       p);
      BoundsCheckBase::upperBoundCheck(name + "(1)", s(1).getValue(), uBound,
                                       p);
      BoundsCheckBase::upperBoundCheck(name + "(2)", s(2).getValue(), uBound,
                                       p);
      BoundsCheckBase::upperBoundCheck(name + "(3)", s(3).getValue(), uBound,
                                       p);
    }
    template <typename T, typename Unit, typename OwnerShip>
    static void lowerAndUpperBoundsChecks(
        const std::string& name,
        const tfel::math::stensor<2u, tfel::math::Quantity<Unit, T, OwnerShip>>&
            s,
        const T lBound,
        const T uBound,
        const OutOfBoundsPolicy p = Strict) {
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(0)", s(0).getValue(),
                                                 lBound, uBound, p);
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(1)", s(1).getValue(),
                                                 lBound, uBound, p);
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(2)", s(2).getValue(),
                                                 lBound, uBound, p);
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(3)", s(3).getValue(),
                                                 lBound, uBound, p);
    }
  };  // end of struct BoundsCheck<2u>;

  template <>
  struct BoundsCheck<3u> : public BoundsCheckBase {
    using BoundsCheckBase::lowerAndUpperBoundsChecks;
    using BoundsCheckBase::lowerBoundCheck;
    using BoundsCheckBase::upperBoundCheck;
    template <typename T>
    static void lowerBoundCheck(const std::string& name,
                                const tfel::math::stensor<3u, T>& s,
                                const T lBound,
                                const OutOfBoundsPolicy p = Strict) {
      BoundsCheckBase::lowerBoundCheck(name + "(0)", s(0), lBound, p);
      BoundsCheckBase::lowerBoundCheck(name + "(1)", s(1), lBound, p);
      BoundsCheckBase::lowerBoundCheck(name + "(2)", s(2), lBound, p);
      BoundsCheckBase::lowerBoundCheck(name + "(3)", s(3), lBound, p);
      BoundsCheckBase::lowerBoundCheck(name + "(4)", s(4), lBound, p);
      BoundsCheckBase::lowerBoundCheck(name + "(5)", s(5), lBound, p);
    }
    template <typename T>
    static void upperBoundCheck(const std::string& name,
                                const tfel::math::stensor<3u, T>& s,
                                const T uBound,
                                const OutOfBoundsPolicy p = Strict) {
      BoundsCheckBase::upperBoundCheck(name + "(0)", s(0), uBound, p);
      BoundsCheckBase::upperBoundCheck(name + "(1)", s(1), uBound, p);
      BoundsCheckBase::upperBoundCheck(name + "(2)", s(2), uBound, p);
      BoundsCheckBase::upperBoundCheck(name + "(3)", s(3), uBound, p);
      BoundsCheckBase::upperBoundCheck(name + "(4)", s(4), uBound, p);
      BoundsCheckBase::upperBoundCheck(name + "(5)", s(5), uBound, p);
    }
    template <typename T>
    static void lowerAndUpperBoundsChecks(const std::string& name,
                                          const tfel::math::stensor<3u, T>& s,
                                          const T lBound,
                                          const T uBound,
                                          const OutOfBoundsPolicy p = Strict) {
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(0)", s(0), lBound,
                                                 uBound, p);
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(1)", s(1), lBound,
                                                 uBound, p);
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(2)", s(2), lBound,
                                                 uBound, p);
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(3)", s(3), lBound,
                                                 uBound, p);
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(4)", s(4), lBound,
                                                 uBound, p);
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(5)", s(5), lBound,
                                                 uBound, p);
    }
    template <typename T, typename Unit, typename OwnerShip>
    static void lowerBoundCheck(
        const std::string& name,
        const tfel::math::stensor<3u, tfel::math::Quantity<Unit, T, OwnerShip>>&
            s,
        const T lBound,
        const OutOfBoundsPolicy p = Strict) {
      BoundsCheckBase::lowerBoundCheck(name + "(0)", s(0).getValue(), lBound,
                                       p);
      BoundsCheckBase::lowerBoundCheck(name + "(1)", s(1).getValue(), lBound,
                                       p);
      BoundsCheckBase::lowerBoundCheck(name + "(2)", s(2).getValue(), lBound,
                                       p);
      BoundsCheckBase::lowerBoundCheck(name + "(3)", s(3).getValue(), lBound,
                                       p);
      BoundsCheckBase::lowerBoundCheck(name + "(4)", s(4).getValue(), lBound,
                                       p);
      BoundsCheckBase::lowerBoundCheck(name + "(5)", s(5).getValue(), lBound,
                                       p);
    }
    template <typename T, typename Unit, typename OwnerShip>
    static void upperBoundCheck(
        const std::string& name,
        const tfel::math::stensor<3u, tfel::math::Quantity<Unit, T, OwnerShip>>&
            s,
        const T uBound,
        const OutOfBoundsPolicy p = Strict) {
      BoundsCheckBase::upperBoundCheck(name + "(0)", s(0).getValue(), uBound,
                                       p);
      BoundsCheckBase::upperBoundCheck(name + "(1)", s(1).getValue(), uBound,
                                       p);
      BoundsCheckBase::upperBoundCheck(name + "(2)", s(2).getValue(), uBound,
                                       p);
      BoundsCheckBase::upperBoundCheck(name + "(3)", s(3).getValue(), uBound,
                                       p);
      BoundsCheckBase::upperBoundCheck(name + "(4)", s(4).getValue(), uBound,
                                       p);
      BoundsCheckBase::upperBoundCheck(name + "(5)", s(5).getValue(), uBound,
                                       p);
    }
    template <typename T, typename Unit, typename OwnerShip>
    static void lowerAndUpperBoundsChecks(
        const std::string& name,
        const tfel::math::stensor<3u, tfel::math::Quantity<Unit, T, OwnerShip>>&
            s,
        const T lBound,
        const T uBound,
        const OutOfBoundsPolicy p = Strict) {
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(0)", s(0).getValue(),
                                                 lBound, uBound, p);
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(1)", s(1).getValue(),
                                                 lBound, uBound, p);
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(2)", s(2).getValue(),
                                                 lBound, uBound, p);
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(3)", s(3).getValue(),
                                                 lBound, uBound, p);
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(4)", s(4).getValue(),
                                                 lBound, uBound, p);
      BoundsCheckBase::lowerAndUpperBoundsChecks(name + "(5)", s(5).getValue(),
                                                 lBound, uBound, p);
    }
  };  // end of struct BoundsCheck<3u>;

}  // end of namespace tfel::material

#endif /* LIB_TFEL_BOUNDSCHECK_HXX */
