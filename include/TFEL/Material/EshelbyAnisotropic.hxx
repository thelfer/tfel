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
    
    //TODO 2D
      /*!
   * This function builds the Hill tensor of a general 2d ellipse embedded in an ANISOTROPIC matrix.
   * The function returns the Hill tensor in the basis (e1,e2) where e1 (resp. e2) is aligned with the axis with the biggest (resp. the smallest) length
   * \return an object of type st2tost2<2u,tfel::math::invert_type<StressType>>
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants
   * \param[in] C: stiffness tensor of the matrix
   * \param[in] e: aspect ratio
   * 
   */
  template <typename real, typename StressType>
  TFEL_HOST_DEVICE static tfel::math::st2tost2<2u,typename tfel::math::invert_type<StressType>> compute2DAnisotropicHillTensor(
      const tfel::math::st2tost2<2u,StressType>&,
      const real&);
      
      //TODO 2D
       /*!
   * This function builds the Eshelby tensor of a general 2d ellipse embedded in an ANISOTROPIC matrix.
   * The function returns the Eshelby tensor in the basis (e1,e2) where e1 (resp. e2) is aligned with the axis with the biggest (resp. the smallest) length
   * \return an object of type st2tost2<2u,real>
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants
  * \param[in] C: stiffness tensor of the matrix
   * \param[in] e: aspect ratio
   */
  template <typename real, typename StressType>
  TFEL_HOST_DEVICE static tfel::math::st2tost2<2u,real> compute2DAnisotropicEshelbyTensor(
      const tfel::math::st2tost2<2u,StressType>&,
      const real&);
  

   /*!
   * This function builds the Hill tensor of a general ellipsoid embedded in an ANISOTROPIC matrix.
   * The function returns the Hill tensor in the basis (e1,e2,e3) where e1 (resp. e2, e3) is aligned with the axis with the first (resp. the second and third) biggest length
   * \return an object of type st2tost2<3u,tfel::math::invert_type<StressType>>
   * \tparam real: underlying type
   * \tparam LengthType: type of the dimensions of the ellipsoid
   * \tparam StressType: type of the elastic constants
   * \param[in] C: stiffness tensor of the matrix
   * \param[in] a: length of the first semi-axis
   * \param[in] b: length of the second semi-axis
   * \param[in] c: length of the third semi-axis
   * 
   */
  template <typename real, typename StressType,typename LengthType>
  TFEL_HOST_DEVICE static tfel::math::st2tost2<3u,typename tfel::math::invert_type<StressType>> computeAnisotropicHillTensor(
      const tfel::math::st2tost2<3u,StressType>&,
      const LengthType&,
      const LengthType&,
      const LengthType&);
      
   /*!
   * This function builds the Eshelby tensor of a general ellipsoid embedded in an ANISOTROPIC matrix.
   * The function returns the Eshelby tensor in the basis (e1,e2,e3) where e1 (resp. e2, e3) is aligned with the axis with the first (resp. the second and third) biggest length
   * \return an object of type st2tost2<3u,real>
   * \tparam real: underlying type
   * \tparam LengthType: type of the dimensions of the ellipsoid
   * \tparam StressType: type of the elastic constants
   * \param[in] C: stiffness tensor of the matrix
   * \param[in] a: length of the first semi-axis
   * \param[in] b: length of the second semi-axis
   * \param[in] c: length of the third semi-axis
   * 
   */
  template <typename real, typename StressType,typename LengthType>
  TFEL_HOST_DEVICE static tfel::math::st2tost2<3u,real> computeAnisotropicEshelbyTensor(
      const tfel::math::st2tost2<3u,StressType>&,
      const LengthType&,
      const LengthType&,
      const LengthType&);
      
    /*!
   * This function builds the strain localisation tensor of a general ellipsoid embedded in an ANISOTROPIC matrix.
   * The localisation tensor \f$A\f$ is defined as follows : \f[\epsilon = A:E_0\f] where \f$E_0\f$ is the uniform strain tensor imposed at infinity,
   * and \f$\epsilon\f$ is the strain tensor solution of Eshelby problem for the ellipsoid.
   * The ellipsoid also has a specific orientation given by the vectors \f$n_a\f$, \f$n_b\f$. A LOCAL basis can then be defined as n_a, n_b, n_c where n_c=cross_product(n_a,n_b)
   * \return an object of type st2tost2<3u,real>, which is the fourth-order localisation tensor \f$A\f$ in the GLOBAL BASIS
   * \tparam real: underlying type
   * \tparam StressType: type of the elastic constants related to the matrix and the ellipsoid
   * \tparam LengthType: type of the dimensions of the ellipsoid
   * \param[in] C_0_glob: stiffness tensor of the matrix in the GLOBAL basis
   * \param[in] C_i_loc: stiffness tensor of the inclusion in the LOCAL basis
   * \param [in] n_a: direction of the principal axis whose length is \f$a\f$
   * \param [in] a: length of semi-axis relative to the direction \f$n_a\f$
   * \param [in] n_b: direction of the principal axis whose length is \f$b\f$
   * \param [in] b: length of semi-axis relative to the direction \f$n_b\f$
   * \param [in] c: length of the remaining semi-axis
   */
      template <typename real, typename StressType, typename LengthType>
	TFEL_HOST_DEVICE tfel::math::st2tost2<3u,real> computeAnisotropicLocalisationTensor(const tfel::math::st2tost2<3u,StressType>&, const tfel::math::st2tost2<3u,StressType>&,
											const tfel::math::tvector<3u,real>&,const LengthType&, const tfel::math::tvector<3u,real>&,
											const LengthType&, const LengthType&);
      
        }//end of namespace elasticity
   }//end of namespace homogenization
      
} // end of namespace tfel::material

#include "TFEL/Material/EshelbyAnisotropic.ixx"

#endif /* LIB_TFEL_MATERIAL_ESHELBYANISOTROPIC_HXX */
