/*!
 * \file  include/TFEL/Material/MetallicCFCSlidingSystems.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 21 mars 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_METALLICCFCSLIDINGSYSTEMS_HXX
#define LIB_TFEL_MATERIAL_METALLICCFCSLIDINGSYSTEMS_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"

namespace tfel {

  namespace material {

    /*!
     * An helper class which actually does the computation of the
     * tensor of directional sense for CFC crystals.
     */
    template <typename NumType>
    struct MetallicCFCSlidingSystems {
      //! a simple alias
      typedef NumType real;
      //! number of sliding systems
      static constexpr const unsigned short Nss = 12;
      //! tensor of directional sense
      tfel::math::tvector<Nss, tfel::math::tensor<3u, real>> mu;
      //! symmetric tensor of directional sense
      tfel::math::tvector<Nss, tfel::math::stensor<3u, real>> mus;
      //! return the unique instance of the class
      static const MetallicCFCSlidingSystems& getSlidingSystems();

     protected:
      /*!
       * constructor
       */
      MetallicCFCSlidingSystems();

     private:
      // copy constructor (disabled)
      MetallicCFCSlidingSystems(const MetallicCFCSlidingSystems&);
      // assignement operator (disabled)
      MetallicCFCSlidingSystems& operator=(const MetallicCFCSlidingSystems&);
    };

  }  // end of namespace material

}  // end of namespace tfel

#include "TFEL/Material/MetallicCFCSlidingSystems.ixx"

#endif /* LIB_TFEL_MATERIAL_METALLICCFCSLIDINGSYSTEMS_HXX */
