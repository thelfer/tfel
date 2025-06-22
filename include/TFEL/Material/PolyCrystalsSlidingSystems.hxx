/*!
 * \file   include/TFEL/Material/PolyCrystalsSlidingSystems.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  16 mai 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef TFEL_MATERIAL_POLYCRISTALSSLIDINGSYSTEMS_HXX
#define TFEL_MATERIAL_POLYCRISTALSSLIDINGSYSTEMS_HXX 1

#include <string>

#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/General/RandomRotationMatrix.hxx"

namespace tfel {

  namespace material {

    /*!
     * \param[in] Np      : number of phases
     * \param[in] GS      : class describing the sliding system of one phase
     * \param[in] NumType : numeric type used
     */
    template <unsigned short Ng, typename GS, typename NumType>
    struct PolyCrystalsSlidingSystems {
      //! number of sliding systems of the mono crystal
      static constexpr unsigned short Nss = GS::Nss;
      //! a simple alias
      typedef NumType real;
      //! a simple alias
      typedef tfel::math::stensor<3u> StrainStensor;
      //! return the uniq instance of the class
      static const PolyCrystalsSlidingSystems& getPolyCrystalsSlidingSystems(
          const std::string& = "");
      //! tensor of directional senses, sorted by phases
      tfel::math::vector<tfel::math::vector<StrainStensor>> mus;

     private:
      /*!
       * default constructor (declared private~: it can only be used
       * by the getPolyCrystalsSlidingSystems method)
       * \param[in] f : file where the angles are declared
       */
      PolyCrystalsSlidingSystems(const std::string&);
      /*!
       * copy constructor (declared private to disable it)
       */
      PolyCrystalsSlidingSystems(const PolyCrystalsSlidingSystems&);
      /*!
       * assignement operatore (declared private to disable it)
       */
      PolyCrystalsSlidingSystems& operator=(const PolyCrystalsSlidingSystems&);
    };

  }  // end of namespace material

}  // end of namespace tfel

#include "TFEL/Material/PolyCrystalsSlidingSystems.ixx"

#endif /* TFEL_MATERIAL_POLYCRISTALSSLIDINGSYSTEMS_HXX */
