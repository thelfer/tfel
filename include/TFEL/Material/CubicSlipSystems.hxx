/*!
 * \file   CubicSlipSystems.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   22/03/2017
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_MATERIAL_CUBICSLIPSYSTEMS_HXX
#define LIB_TFEL_MATERIAL_CUBICSLIPSYSTEMS_HXX

#include<vector>
#include<utility>
#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/tvector.hxx"

namespace tfel{

  namespace material{

    /*!
     * \brief An class used to retrieve information about cubic slip
     * systems.
     */
    struct TFELUTILITIES_VISIBILITY_EXPORT CubicSlipSystems
    {
      //! a simple alias
      using vec3d = tfel::math::tvector<3u,int>;
      /*!
       * \brief generate all the slip systems given a normal and a
       * slip direction.
       * \param[in] n: normal
       * \param[in] d: slip direction
       */
      static std::vector<std::pair<vec3d,vec3d>>
      generate(const vec3d&, const vec3d&);
    }; // end of struct CubicSlipSystems
    
  } // end of namespace material

} // end of namespace tfel
    
#endif /* LIB_TFEL_MATERIAL_CUBICSLIPSYSTEMS_HXX */
