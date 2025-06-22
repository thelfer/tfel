/*!
 * \file
 * include/TFEL/Material/MetallicCFCGenericSlidingSystemsInteractionMatrix.hxx
 * \brief
 * \author Helfer Thomas/Proix Jean-Michel
 * \brief 25 mars 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_SINGLECRISTAL_METALLICCFCGENERICSLIDINGSYSTEMSINTERACTIONMATRIX_H_
#define LIB_TFEL_MATERIAL_SINGLECRISTAL_METALLICCFCGENERICSLIDINGSYSTEMSINTERACTIONMATRIX_H_

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel {

  namespace material {

    /*!
     * \brief Given six interaction coefficients, this class builds
     * only once a generic interaction matrix. This class shall not be
     * called if the interaction coefficients may evolve with time~:
     * the matrix is only compute at the first call.
     */
    template <typename NumType>
    struct MetallicCFCGenericSlidingSystemsInteractionMatrix {
      //! a simple alias
      typedef NumType real;
      /*!
       * \return the interaction matrix
       * \param[in] h1 : first  interaction coefficient
       * \param[in] h2 : second interaction coefficient
       * \param[in] h3 : third  interaction coefficient
       * \param[in] h4 : fourth interaction coefficient
       * \param[in] h5 : fifth  interaction coefficient
       * \param[in] h6 : sixth  interaction coefficient
       * The interaction matrix is the following :
       *
       */
      static const tfel::math::tmatrix<12, 12, real>& getInteractionMatrix(
          const real,
          const real,
          const real,
          const real,
          const real,
          const real);
      /*!
       * \return the interaction matrix
       * \param[in] h1 : first  interaction coefficient
       * \param[in] h2 : second interaction coefficient
       * \param[in] h3 : third  interaction coefficient
       * \param[in] h4 : fourth interaction coefficient
       * \param[in] h5 : fifth  interaction coefficient
       * \param[in] h6 : sixth  interaction coefficient
       * The interaction matrix is the following :
       *
       */
      static const tfel::math::tmatrix<12, 12, real>&
      getSecondInteractionMatrix(const real,
                                 const real,
                                 const real,
                                 const real,
                                 const real,
                                 const real);

     private:
      /*!
       * constructor (disabled)
       */
      MetallicCFCGenericSlidingSystemsInteractionMatrix();
      /*!
       * \param[in] h1 : first  interaction coefficient
       * \param[in] h2 : second interaction coefficient
       * \param[in] h3 : third  interaction coefficient
       * \param[in] h4 : fourth interaction coefficient
       * \param[in] h5 : fifth  interaction coefficient
       * \param[in] h6 : sixth  interaction coefficient
       */
      MetallicCFCGenericSlidingSystemsInteractionMatrix(const real,
                                                        const real,
                                                        const real,
                                                        const real,
                                                        const real,
                                                        const real);
      /*!
       * constructor (disabled)
       */
      MetallicCFCGenericSlidingSystemsInteractionMatrix(
          const MetallicCFCGenericSlidingSystemsInteractionMatrix&);
      /*!
       * assignement operator (disabled)
       */
      MetallicCFCGenericSlidingSystemsInteractionMatrix& operator=(
          const MetallicCFCGenericSlidingSystemsInteractionMatrix&);
      //! interaction matrix
      tfel::math::tmatrix<12, 12, real> m;
    };

  }  // end of namespace material

}  // end of namespace tfel

#include "TFEL/Material/MetallicCFCGenericSlidingSystemsInteractionMatrix.ixx"

#endif /* LIB_TFEL_MATERIAL_SINGLECRISTAL_METALLICCFCGENERICSLIDINGSYSTEMSINTERACTIONMATRIX_H_ \
        */
