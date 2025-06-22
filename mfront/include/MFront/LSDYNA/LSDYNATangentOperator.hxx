/*!
 * \file  mfront/include/MFront/LSDYNA/LSDYNATangentOperator.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 07 févr. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_LSDYNA_LSDYNATANGENTOPERATOR_HXX
#define LIB_MFRONT_LSDYNA_LSDYNATANGENTOPERATOR_HXX

#include <type_traits>

#include "TFEL/Math/ST2toST2/ST2toST2View.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "TFEL/Material/MechanicalBehaviourTraits.hxx"

#include "MFront/LSDYNA/LSDYNA.hxx"
#include "MFront/LSDYNA/LSDYNATraits.hxx"
#include "MFront/LSDYNA/LSDYNAConfig.hxx"

namespace lsdyna {

  template <LSDYNABehaviourType btype, typename real, unsigned short N>
  struct LSDYNATangentOperatorType {
    using type = tfel::math::st2tost2<N, real>;
    using view_type = tfel::math::ST2toST2View<N, real>;
  };

  /*!
   * \brief an helper structure introduced to normalise the tangent
   * operator to follow the umat interface
   */
  template <typename real>
  struct LSDYNATangentOperator {
    /*!
     * \brief normalize the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void normalize(tfel::math::ST2toST2View<1u, real>&);
    /*!
     * \brief normalize the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void normalize(tfel::math::ST2toST2View<2u, real>&);
    /*!
     * \brief normalize the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void normalize(tfel::math::ST2toST2View<3u, real>&);
    /*!
     * \brief transpose the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void transpose(tfel::math::ST2toST2View<1u, real>&);
    /*!
     * \brief transpose the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void transpose(tfel::math::ST2toST2View<2u, real>&);
    /*!
     * \brief transpose the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void transpose(tfel::math::ST2toST2View<3u, real>&);
  };  // end of struct LSDYNATangentOperator

  /*!
   * \brief this class extracts and normalise (convert to
   * lsdyna/fortran conventions) the tangent operator
   */
  template <tfel::material::ModellingHypothesis::Hypothesis H>
  struct ExtractAndConvertTangentOperator {
    template <typename Behaviour, typename real>
    static void exe(const Behaviour& b, real* const DDSDDE) {
      using namespace tfel::material;
      typedef MechanicalBehaviourTraits<Behaviour> Traits;
      using handler = typename std::conditional<
          Traits::isConsistentTangentOperatorSymmetric,
          SymmetricConsistentTangentOperatorComputer,
          GeneralConsistentTangentOperatorComputer>::type;
      handler::exe(b, DDSDDE);
    }  // end of exe
   private:
    struct ConsistentTangentOperatorComputer {
      template <typename Behaviour, typename real>
      static void exe(const Behaviour& bv, real* const DDSDDE) {
        using tfel::material::ModellingHypothesisToSpaceDimension;
        const unsigned short N = ModellingHypothesisToSpaceDimension<H>::value;
        using TangentOperatorType =
            typename LSDYNATangentOperatorType<LSDYNATraits<Behaviour>::btype,
                                               real, N>::type;
        using TangentOperatorViewType =
            typename LSDYNATangentOperatorType<LSDYNATraits<Behaviour>::btype,
                                               real, N>::view_type;
        TangentOperatorViewType Dt{DDSDDE};
        Dt = static_cast<const TangentOperatorType&>(bv.getTangentOperator());
        LSDYNATangentOperator<real>::normalize(Dt);
      }  // end of exe
    };
    struct SymmetricConsistentTangentOperatorComputer {
      template <typename Behaviour, typename real>
      static void exe(const Behaviour& bv, real* const DDSDDE) {
        ConsistentTangentOperatorComputer::exe(bv, DDSDDE);
      }  // end of exe
    };
    struct GeneralConsistentTangentOperatorComputer {
      template <typename Behaviour, typename real>
      static void exe(const Behaviour& bv, real* const DDSDDE) {
        using tfel::material::ModellingHypothesisToSpaceDimension;
        const unsigned short N = ModellingHypothesisToSpaceDimension<H>::value;
        using TangentOperatorViewType =
            typename LSDYNATangentOperatorType<LSDYNATraits<Behaviour>::btype,
                                               real, N>::view_type;
        ConsistentTangentOperatorComputer::exe(bv, DDSDDE);
        // les conventions fortran....
        TangentOperatorViewType Dt{DDSDDE};
        LSDYNATangentOperator<real>::transpose(Dt);
      }  // end of exe
    };
  };  // end of struct ExtractAndConvertTangentOperator
  /*!
   * \brief partial specialisation of the
   * ExtractAndConvertTangentOperator for the plane stress modelling
   * hypothesis
   */
  template <>
  struct ExtractAndConvertTangentOperator<
      tfel::material::ModellingHypothesis::PLANESTRESS> {
    template <typename Behaviour, typename real>
    static void exe(const Behaviour& b, real* const DDSDDE) {
      using namespace tfel::material;
      typedef MechanicalBehaviourTraits<Behaviour> Traits;
      using handler = typename std::conditional<
          Traits::isConsistentTangentOperatorSymmetric,
          SymmetricConsistentTangentOperatorComputer,
          GeneralConsistentTangentOperatorComputer>::type;
      handler::exe(b, DDSDDE);
    }  // end of exe
   private:
    struct SymmetricConsistentTangentOperatorComputer {
      template <typename Behaviour, typename real>
      static void exe(const Behaviour& bv, real* const DDSDDE) {
        using TangentOperatorType =
            typename LSDYNATangentOperatorType<LSDYNATraits<Behaviour>::btype,
                                               real, 2u>::type;
        constexpr const auto icste = tfel::math::Cste<real>::isqrt2;
        TFEL_CONSTEXPR const auto one_half = real(1) / real(2);
        auto Dt =
            static_cast<const TangentOperatorType&>(bv.getTangentOperator());
        DDSDDE[0] = Dt(0, 0);
        DDSDDE[1] = Dt(1, 0);
        DDSDDE[2] = Dt(3, 0) * icste;
        DDSDDE[3] = Dt(0, 1);
        DDSDDE[4] = Dt(1, 1);
        DDSDDE[5] = Dt(3, 1) * icste;
        DDSDDE[6] = Dt(0, 3) * icste;
        DDSDDE[7] = Dt(1, 3) * icste;
        DDSDDE[8] = Dt(3, 3) * one_half;
      }  // end of exe
    };
    struct GeneralConsistentTangentOperatorComputer {
      template <typename Behaviour, typename real>
      static void exe(const Behaviour& bv, real* const DDSDDE) {
        using TangentOperatorType =
            typename LSDYNATangentOperatorType<LSDYNATraits<Behaviour>::btype,
                                               real, 2u>::type;
        constexpr const auto icste = tfel::math::Cste<real>::isqrt2;
        TFEL_CONSTEXPR const auto one_half = real(1) / real(2);
        auto Dt =
            static_cast<const TangentOperatorType&>(bv.getTangentOperator());
        DDSDDE[0] = Dt(0, 0);
        DDSDDE[1] = Dt(0, 1);
        DDSDDE[2] = Dt(0, 3) * icste;
        DDSDDE[3] = Dt(1, 0);
        DDSDDE[4] = Dt(1, 1);
        DDSDDE[5] = Dt(1, 3) * icste;
        DDSDDE[6] = Dt(3, 0) * icste;
        DDSDDE[7] = Dt(3, 1) * icste;
        DDSDDE[8] = Dt(3, 3) * one_half;
      }  // end of exe
    };
  };

}  // end of namespace lsdyna

#include "MFront/LSDYNA/LSDYNATangentOperator.ixx"

#endif /* LIB_MFRONT_LSDYNA_LSDYNATANGENTOPERATOR_HXX */
