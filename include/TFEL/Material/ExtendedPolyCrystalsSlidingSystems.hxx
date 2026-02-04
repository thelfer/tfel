/*!
 * \file   include/TFEL/Material/ExtendedPolyCrystalsSlidingSystems.hxx
 * \brief
 * \author Thomas Helfer
 * \brief  16 mai 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef TFEL_MATERIAL_EXTENDEDPOLYCRISTALSSLIDINGSYSTEMS_HXX
#define TFEL_MATERIAL_EXTENDEDPOLYCRISTALSSLIDINGSYSTEMS_HXX 1

#include <string>
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/vector.hxx"

namespace tfel {

  namespace material {

    /*!
     * \param[in] Np      : number of phases
     * \param[in] GS      : class describing the sliding system of one phase
     * \param[in] NumType : numeric type used
     */
    template <unsigned short Ng, typename GS, typename NumType>
    struct ExtendedPolyCrystalsSlidingSystems {
      //! number of sliding systems of the mono crystal
      static constexpr unsigned short Nss = GS::Nss;
      //! a simple alias
      typedef NumType real;
      //! a simple alias
      typedef tfel::math::stensor<3u, real> StrainStensor;
      //! return the uniq instance of the class
      static const ExtendedPolyCrystalsSlidingSystems &getPolyCrystalsSlidingSystems(
          const std::string &);
      //! tensor of directional senses, sorted by phases
      tfel::math::vector<tfel::math::vector<StrainStensor>> mus;
      //! \brief volume fractions per phases
      tfel::math::vector<real> volume_fractions;

     private:
      /*!
       * default constructor (declared private~: it can only be used
       * by the getExtendedPolyCrystalsSlidingSystems method)
       * \param[in] f : file where the angles are declared
       */
      ExtendedPolyCrystalsSlidingSystems(const std::string &);
      /*!
       * copy constructor (declared private to disable it)
       */
      ExtendedPolyCrystalsSlidingSystems(const ExtendedPolyCrystalsSlidingSystems &);
      /*!
       * assignement operator (declared private to disable it)
       */
      ExtendedPolyCrystalsSlidingSystems &operator=(
          const ExtendedPolyCrystalsSlidingSystems &);
    };

  }  // end of namespace material

}  // end of namespace tfel

#include "./ExtendedPolyCrystalsSlidingSystems.ixx"

#endif /* TFEL_MATERIAL_EXTENDEDPOLYCRISTALSSLIDINGSYSTEMS_HXX */
