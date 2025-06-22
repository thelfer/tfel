/*!
 * \file   OrthotropicAxesConvention.hxx
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

#ifndef LIB_TFEL_MATERIAL_ORTHOTROPICAXESCONVENTION_HXX
#define LIB_TFEL_MATERIAL_ORTHOTROPICAXESCONVENTION_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

namespace tfel {

  namespace material {

    /*!
     * Most finite element solver can't have a uniq definition of the
     * orthotropic axes for all the modelling hypotheses. One thus
     * need to define a convention. This leads to many troubles when
     * trying to define an the hill tensor or the thermal expansion.
     *
     * For example, one can define a pipe using the following axes
     * definition:
     * - \f$\left(rr,zz,tt,...\right)\f$ in \f$3D\f$, \f$2D\f$
     *   axysymmetric, \f$1D\f$ axisymmetric generalised plane strain
     *   or generalised plane stress.
     * - \f$\left(rr,tt,zz,...\right)\f$ in \f$2D\f$ plane stress,
     *   strain, generalized plane strain.
     *
     * This means that we have to exchange the second and third axis
     * when changing from \f$3D\f$ to \f$2D\f$ plane strain. This is
     * the PIPE (orthotropic axes) convention.
     *
     * The various classes and functions that take an
     * `OrthotropicAxesConvention` value as template parameter will
     * take the values of the material properties in the material
     * frame corresponding to the \f$3D\f$ case and internally make
     * the appropriate swaps when necessary.
     */
    enum class OrthotropicAxesConvention {
      /*!
       * The `DEFAULT` orthotropic convention does not differentiate
       * modelling hypotheses, which is **not** physically
       * significant.
       */
      DEFAULT,
      /*!
       * \htmlonly <style>div.image
       * img[src="img/pipe.svg"]{width:400px;}</style> \endhtmlonly \image html
       * img/pipe.svg "Orthotropic axes conventions for pipes"
       *
       * The `PIPE` orthotropic convention assumes that:
       * - in \f$3D\f$, \f$2D\f$ axysymmetric, \f$1D\f$ axisymmetric
       * generalised plane strain or generalised plane stress:
       *     - The first material axis is given by the radius of the pipe.
       *     - The second material axis is given by the axis of the pipe.
       * - in \f$2D\f$ plane stress, strain, generalized plane strain:
       *     - The first material axis is given by the radius of the pipe.
       *     - The second material axis is perpendicual to the radius of
       *       the pipe and to the axis of the pipe.
       *
       *   In other words, the second and third axes are exchanged
       *   compared to the \(3D\) case.
       */
      PIPE,
      /*!
       * \htmlonly <style>div.image
       * img[src="img/plate.svg"]{width:400px;}</style> \endhtmlonly \image html
       * img/plate.svg "Orthotropic axes conventions for plates"
       *
       * The `PLATE` orthotropic convention implies that:
       * - The first material axis is the rolling direction.
       * - The second material axis is the tranverse direction.
       * - The third material axis is the normal direction.
       *
       * The `PLATE` orthotropic convention can only be used in:
       * - \f$3D\f$ modelling hypothesis.
       * - \f$2D\f$ plane stress, strain and generalized plane strain
       *   modelling hypotheses.
       */
      PLATE
    };  // end of enum class OrthotropicAxesConvention

    /*!
     * \brief convert a diagonal tensor from the \f$3D\f$ convention
     * to the specified modelling hypothesis according to the
     * orthotropic axes convention.
     * \tparam mh       : modelling hypothesis
     * \tparam c        : convention for the orthotropic axes
     * \tparam T        : type holded by the tensor
     * \param[in,out] s : stensor
     */
    template <ModellingHypothesis::Hypothesis mh,
              OrthotropicAxesConvention c,
              typename T>
    void convertStressFreeExpansionStrain(
        tfel::math::stensor<ModellingHypothesisToSpaceDimension<mh>::value,
                            T>&);

  }  // end of namespace material

}  // end of namespace tfel

#include "TFEL/Material/OrthotropicAxesConvention.ixx"

#endif /* LIB_TFEL_MATERIAL_ORTHOTROPICAXESCONVENTION_HXX */
