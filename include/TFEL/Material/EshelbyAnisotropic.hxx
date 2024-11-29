/*!
 * \file   include/TFEL/Material/EshelbyAnisotropic.hxx
 * \author Antoine Martin
 * \date   18 November 2024
 * \brief  This file declares the Eshelby tensor for an ellipsoidal inclusion embedded in an anisotropic matrix.
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ESHELBYANISOTROPIC_HXX
#define LIB_TFEL_MATERIAL_ESHELBYANISOTROPIC_HXX

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/StiffnessTensor.hxx"
#include <stdexcept>

namespace tfel::material
{

namespace homogenization{
     namespace elasticity{

   /*!
   * This function builds the Hill tensor of a general ellipsoid embedded in an ANISOTROPIC matrix.
   * The function returns the Hill tensor in the basis (e1,e2,e3) where e1 (resp. e2, e3) is aligned with a (resp. b,c)
   * \return an object of type st2tost2<3u,tfel::math::invert_type<StressType>>
   * \tparam real: underlying type
   * \tparam LengthType: type of the dimensions of the ellipsoid
   * \tparam StressType: type of the elastic constants
   * \param[in] C: stiffness tensor of the matrix
   * \param[in] a: length of the first semi-axis
   * \param[in] b: length of the second semi-axis
   * \param[in] c: length of the third semi-axis
   * 
   * The mathematical expressions come from Masson (2007), New explicit expressions of the Hill polarization tensor
for general anisotropic elastic solids. The computation is inspired from Renaud Masson's library micmac.
   */
  template <typename real, typename StressType,typename LengthType>
  TFEL_HOST_DEVICE static tfel::math::st2tost2<3u,typename tfel::math::invert_type<StressType>> computeAnisotropicHillTensor(
      const tfel::math::st2tost2<3u,StressType>&,
      const LengthType&,
      const LengthType&,
      const LengthType&);
      
      
      
   /*!
   * This function builds the Eshelby tensor of a general ellipsoid embedded in an ANISOTROPIC matrix.
   * The function returns the Eshelby tensor in the basis (e1,e2,e3) where e1 (resp. e2, e3) is aligned with a (resp. b,c)
   * \return an object of type st2tost2<3u,real>
   * \tparam real: underlying type
   * \tparam LengthType: type of the dimensions of the ellipsoid
   * \tparam StressType: type of the elastic constants
   * \param[in] C: stiffness tensor of the matrix
   * \param[in] a: length of the first semi-axis
   * \param[in] b: length of the second semi-axis
   * \param[in] c: length of the third semi-axis
   * 
   * The mathematical expressions come from Masson (2007), New explicit expressions of the Hill polarization tensor
for general anisotropic elastic solids. The computation is inspired from Renaud Masson's library micmac.
   */
  template <typename real, typename StressType,typename LengthType>
  TFEL_HOST_DEVICE static tfel::math::st2tost2<3u,real> computeAnisotropicEshelbyTensor(
      const tfel::math::st2tost2<3u,StressType>&,
      const LengthType&,
      const LengthType&,
      const LengthType&);
      
        }//end of namespace elasticity
   }//end of namespace homogenization
      
} // end of namespace tfel::material

#include "TFEL/Material/EshelbyAnisotropic.ixx"

#endif /* LIB_TFEL_MATERIAL_ESHELBYANISOTROPIC_HXX */
