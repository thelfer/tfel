/*!
 * \file  mfront/include/MFront/CalculiX/CalculiXTangentOperator.hxx
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

#ifndef LIB_MFRONT_CALCULIX_CALCULIXTANGENTOPERATOR_HXX
#define LIB_MFRONT_CALCULIX_CALCULIXTANGENTOPERATOR_HXX

#include <type_traits>
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "TFEL/Material/MechanicalBehaviourTraits.hxx"
#include "MFront/CalculiX/CalculiX.hxx"
#include "MFront/CalculiX/CalculiXTraits.hxx"
#include "MFront/CalculiX/CalculiXConfig.hxx"

namespace calculix {

  /*!
   * \brief this class extracts and normalise (convert to
   * calculix/fortran conventions) the tangent operator
   */
  struct ExtractTangentOperator {
    template <typename Behaviour, typename real>
    static void exe(const Behaviour &bv, real *const DDSDDE) {
      using TangentOperatorType = tfel::math::st2tost2<3u, real>;
      const auto &Dt =
          static_cast<const TangentOperatorType &>(bv.getTangentOperator());
      std::copy(Dt.begin(), Dt.end(), DDSDDE);
    }
  };  // end of ExtractTangentOperator

  struct ConvertSymmetricTangentOperator {
    template <typename real>
    static void exe(real *const DDSDDE, const real *const D) {
      constexpr const auto one_half = real(1) / real(2);
      constexpr const auto icste = tfel::math::Cste<real>::isqrt2;
      tfel::math::ConstST2toST2View<3u, real> Dt(D);
      DDSDDE[0] = Dt(0, 0);
      DDSDDE[1] = Dt(0, 1);
      DDSDDE[2] = Dt(1, 1);
      DDSDDE[3] = Dt(0, 2);
      DDSDDE[4] = Dt(1, 2);
      DDSDDE[5] = Dt(2, 2);
      DDSDDE[6] = Dt(0, 3) * icste;
      DDSDDE[7] = Dt(1, 3) * icste;
      DDSDDE[8] = Dt(2, 3) * icste;
      DDSDDE[9] = Dt(3, 3) * one_half;
      DDSDDE[10] = Dt(0, 4) * icste;
      DDSDDE[11] = Dt(1, 4) * icste;
      DDSDDE[12] = Dt(2, 4) * icste;
      DDSDDE[13] = Dt(3, 4) * one_half;
      DDSDDE[14] = Dt(4, 4) * one_half;
      DDSDDE[15] = Dt(0, 5) * icste;
      DDSDDE[16] = Dt(1, 5) * icste;
      DDSDDE[17] = Dt(2, 5) * icste;
      DDSDDE[18] = Dt(3, 5) * one_half;
      DDSDDE[19] = Dt(4, 5) * one_half;
      DDSDDE[20] = Dt(5, 5) * one_half;
    }  // end of exe
  };

  struct ConvertUnsymmetricTangentOperator {
    template <typename real>
    static void exe(real *const DDSDDE, const real *const D) {
      // In CalculiX, the stiffness tensor is written using Voigt
      // notations
      // (sxx ... sxy ... ) = D. (exx ... 2exy ...)
      // This is *not* consistent with the representation of tensors
      // This can be very misleading
      constexpr const auto one_half = real(1) / real(2);
      constexpr const auto one_fourth = real(1) / real(4);
      constexpr const auto hicste = tfel::math::Cste<real>::isqrt2 / 2;
      tfel::math::ConstST2toST2View<3u, real> Dt(D);
      DDSDDE[0] = Dt(0, 0);
      DDSDDE[1] = (Dt(0, 1) + Dt(1, 0)) * one_half;
      DDSDDE[2] = Dt(1, 1);
      DDSDDE[3] = (Dt(0, 2) + Dt(2, 0)) * one_half;
      DDSDDE[4] = (Dt(1, 2) + Dt(2, 1)) * one_half;
      DDSDDE[5] = Dt(2, 2);
      DDSDDE[6] = (Dt(0, 3) + Dt(3, 0)) * hicste;
      DDSDDE[7] = (Dt(1, 3) + Dt(3, 1)) * hicste;
      DDSDDE[8] = (Dt(2, 3) + Dt(3, 2)) * hicste;
      DDSDDE[9] = Dt(3, 3) * one_half;
      DDSDDE[10] = (Dt(0, 4) + Dt(4, 0)) * hicste;
      DDSDDE[11] = (Dt(1, 4) + Dt(4, 1)) * hicste;
      DDSDDE[12] = (Dt(2, 4) + Dt(4, 2)) * hicste;
      DDSDDE[13] = (Dt(3, 4) + Dt(4, 3)) * one_fourth;
      DDSDDE[14] = Dt(4, 4) * one_half;
      DDSDDE[15] = (Dt(0, 5) + Dt(5, 0)) * hicste;
      DDSDDE[16] = (Dt(1, 5) + Dt(5, 1)) * hicste;
      DDSDDE[17] = (Dt(2, 5) + Dt(5, 2)) * hicste;
      DDSDDE[18] = (Dt(3, 5) + Dt(5, 3)) * one_fourth;
      DDSDDE[19] = (Dt(4, 5) + Dt(5, 4)) * one_fourth;
      DDSDDE[20] = Dt(5, 5) * one_half;
    }  // end of exe
  };

}  // end of namespace calculix

#endif /* LIB_MFRONT_CALCULIX_CALCULIXTANGENTOPERATOR_HXX */
