/*!
 * \file   OrthotropicAxesConvention.ixx
 * \brief
 * \author Thomas Helfer
 * \date   24 août 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ORTHOTROPICAXESCONVENTION_IXX
#define LIB_TFEL_MATERIAL_ORTHOTROPICAXESCONVENTION_IXX

namespace tfel {

  namespace material {

    namespace internals {

      /*!
       * \brief an helper structure for appropriatly swapping
       * symmetric tensor coefficients according the modelling
       * hypothesis and the orthotropic axes convention.
       * \tparam mh: modelling hypothesis
       * \tparam c:  orthotropic axes convention
       */
      template <ModellingHypothesis::Hypothesis mh, OrthotropicAxesConvention c>
      struct OrthotropicAxesConventionConverter {
        //! a simple alias
        static constexpr unsigned short N =
            ModellingHypothesisToSpaceDimension<mh>::value;
        /*!
         * convert a symmetric tensor to the new convention
         */
        template <typename T>
        static TFEL_MATERIAL_INLINE void convert(tfel::math::stensor<N, T>&) {
        }  // end of convert
      };   // end of struct OrthotropicAxesConventionConverter

      /*!
       * \brief an helper structure used in "*plane*" generalised
       * modelling hypotheses to perform convertion
       */
      struct PipeOrthotropicAxesConventionConverter {
        /*!
         * convert a symmetric tensor to the new convention
         * \param[in] s : the tensor to be converted
         */
        template <typename T>
        static TFEL_MATERIAL_INLINE void convert(
            tfel::math::stensor<2u, T>& s) {
          std::swap(s[1], s[2]);
        }  // end of convert
      };
      /*!
       * \brief partial specialisation for the:
       * - `ModellingHypothesis::PLANESTRESS` modelling hypothesis.
       * - `OrthotropicAxesConvention::PIPE` orthotropic axes
       *   convention.
       */
      template <>
      struct OrthotropicAxesConventionConverter<
          ModellingHypothesis::PLANESTRESS,
          OrthotropicAxesConvention::PIPE>
          : public PipeOrthotropicAxesConventionConverter {
      };  // end of struct OrthotropicAxesConventionConverter
      /*!
       * \brief partial specialisation for the:
       * - `ModellingHypothesis::PLANESTRAIN` modelling hypothesis.
       * - `OrthotropicAxesConvention::PIPE` orthotropic axes
       *   convention.
       */
      template <>
      struct OrthotropicAxesConventionConverter<
          ModellingHypothesis::PLANESTRAIN,
          OrthotropicAxesConvention::PIPE>
          : public PipeOrthotropicAxesConventionConverter {
      };  // end of struct OrthotropicAxesConventionConverter
      /*!
       * \brief partial specialisation for the:
       * - `ModellingHypothesis::GENERALISEDPLANESTRAIN` modelling hypothesis.
       * - `OrthotropicAxesConvention::PIPE` orthotropic axes
       *   convention.
       */
      template <>
      struct OrthotropicAxesConventionConverter<
          ModellingHypothesis::GENERALISEDPLANESTRAIN,
          OrthotropicAxesConvention::PIPE>
          : public PipeOrthotropicAxesConventionConverter {
      };  // end of struct OrthotropicAxesConventionConverter

    }  // end of namespace internals

    template <ModellingHypothesis::Hypothesis mh,
              OrthotropicAxesConvention c,
              typename T>
    void convertStressFreeExpansionStrain(
        tfel::math::stensor<ModellingHypothesisToSpaceDimension<mh>::value, T>&
            s) {
      internals::OrthotropicAxesConventionConverter<mh, c>::convert(s);
    }  // end of convert

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_ORTHOTROPICAXESCONVENTION_IXX */
