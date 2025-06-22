/*!
 * \file   include/TFEL/Material/MohrCoulombYieldCriterion.hxx
 * \brief
 * \author Thomas Nagel, Thomas Helfer
 * \date   10/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_MOHRCOULOMBYIELDCRITERION_HXX
#define LIB_TFEL_MATERIAL_MOHRCOULOMBYIELDCRITERION_HXX

#include <tuple>
#include <ostream>
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

namespace tfel {

  namespace material {

    //! a simple alias
    template <typename StressStensor>
    using MohrCoulombStressType = tfel::math::StensorNumType<StressStensor>;
    //! a simple alias
    template <typename StressStensor>
    using MohrCoulombBaseType =
        tfel::typetraits::base_type<tfel::math::StensorNumType<StressStensor>>;
    //! a simple alias
    template <typename StressStensor>
    using MohrCoulombInvertStressType =
        tfel::math::result_type<MohrCoulombBaseType<StressStensor>,
                                MohrCoulombStressType<StressStensor>,
                                tfel::math::OpDiv>;
    //! a simple alias
    template <typename StressStensor>
    using MohrCoulombStressNormalType =
        tfel::math::stensor<tfel::math::StensorTraits<StressStensor>::dime,
                            MohrCoulombBaseType<StressStensor>>;
    //! a simple alias
    template <typename StressStensor>
    using MohrCoulombStressSecondDerivativeType =
        tfel::math::st2tost2<tfel::math::StensorTraits<StressStensor>::dime,
                             MohrCoulombInvertStressType<StressStensor>>;

    /*!
     * \brief structure holding the parameters of the MohrCoulomb stress
     * criterion.
     */
    template <typename StressStensor>
    struct MohrCoulombParameters {
      //! a simple alias
      using stress = MohrCoulombStressType<StressStensor>;
      //! a simple alias
      using real = MohrCoulombBaseType<StressStensor>;
      //! \brief list of angle unit used allowed to initialize the parameters
      enum AngleUnit { RADIAN, DEGREE };
      /*!
       * \brief constructor
       * \param[in] c_c: cohesion
       * \param[in] angle_c: friction angle or dilatancy angle
       * \param[in] lodeT_c: transition angle as defined by Abbo and Sloan
       * \param[in] a_c: tension cuff-off parameter
       * \note angles are in radians, see `makeMohrCoulombParameters`
       */
      MohrCoulombParameters(const stress, const real, const real, const stress);
      //! \brief default constructor
      MohrCoulombParameters();
      //! \brief move constructor
      MohrCoulombParameters(MohrCoulombParameters&&);
      //! \brief copy constructor
      MohrCoulombParameters(const MohrCoulombParameters&);
      //! \brief move assignement
      MohrCoulombParameters& operator=(MohrCoulombParameters&&);
      //! \brief standard assignement
      MohrCoulombParameters& operator=(const MohrCoulombParameters&);
      //! \brief cohesion
      stress c;
      //! \brief friction angle or dilatancy angle
      real angle;
      //! \brief transition angle as defined by Abbo and Sloan
      real lodeT;
      //! \brief tension cuff-off parameter
      stress a;
      /* cached variables */
      //! \brief cosine of angle
      real cos_angle;
      //! \brief sine of angle
      real sin_angle;
      //! \brief cosine of lodeT
      real cos_lodeT;
      //! \brief sine of lodeT
      real sin_lodeT;
      //! \brief tangent of lodeT
      real tan_lodeT;
      //! \brief cosine of 3*lodeT
      real cos_3_lodeT;
      //! \brief sine of 3*lodeT
      real sin_3_lodeT;
      //! \brief tangent of 3*lodeT
      real tan_3_lodeT;
    };  // end of struct MohrCoulombParameters

    /*!
     * \param[in] c: cohesion
     * \param[in] angle: friction angle or dilatancy angle
     * \param[in] lodeT: transition angle as defined by Abbo and Sloan
     * \param[in] a: tension cuff-off parameter
     */
    template <typename StressStensor,
              typename MohrCoulombParameters<StressStensor>::AngleUnit>
    MohrCoulombParameters<StressStensor> makeMohrCoulombParameters(
        const MohrCoulombStressType<StressStensor>,
        const MohrCoulombBaseType<StressStensor>,
        const MohrCoulombBaseType<StressStensor>,
        const MohrCoulombStressType<StressStensor>);
    /*!
     * \return the equivalent stress
     * \tparam StressStensor: type of the stress tensor
     * \param[in] p: stress tensor
     * \param[in] sig: stress tensor
     */
    template <typename StressStensor>
    MohrCoulombStressType<StressStensor> computeMohrCoulombStressCriterion(
        const MohrCoulombParameters<StressStensor>&, const StressStensor&);
    /*!
     * \brief compute the MohrCoulomb yield stress and its first derivative
     * \tparam StressStensor: type of the stress tensor
     * \param[in] p: parameters
     * \param[in] sig: stress tensor
     */
    template <typename StressStensor>
    std::tuple<MohrCoulombStressType<StressStensor>,
               MohrCoulombStressNormalType<StressStensor>>
    computeMohrCoulombStressCriterionNormal(
        const MohrCoulombParameters<StressStensor>&, const StressStensor&);
    /*!
     * \brief compute the MohrCoulomb yield stress and its first and second
     * derivatives.
     * \tparam StressStensor: type of the stress tensor
     * \param[in] p: parameters
     * \param[in] sig: stress tensor
     */
    template <typename StressStensor>
    std::tuple<MohrCoulombStressType<StressStensor>,
               MohrCoulombStressNormalType<StressStensor>,
               MohrCoulombStressSecondDerivativeType<StressStensor>>
    computeMohrCoulombStressCriterionSecondDerivative(
        const MohrCoulombParameters<StressStensor>&, const StressStensor&);

    /*!
     * \brief output stream operator
     * \param[in,out] os: output stream
     * \param[in]     p: parameters
     * \note this is required to build `MFront` behaviours in `debug` mode
     */
    template <typename StressStensor>
    std::ostream& operator<<(std::ostream&,
                             const MohrCoulombParameters<StressStensor>&);

  }  // end of namespace material

}  // end of namespace tfel

#include "TFEL/Material/MohrCoulombYieldCriterion.ixx"

#endif /* LIB_TFEL_MATERIAL_MOHRCOULOMBYIELDCRITERION_HXX */
