/*!
 * \file   include/TFEL/Material/PiPlane.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   14/12/2017
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_MATERIAL_PIPLANE_HXX
#define LIB_TFEL_MATERIAL_PIPLANE_HXX

#include<tuple>
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/General/MathConstants.hxx"

namespace tfel{

  namespace material{

    /*!
     * \brief this function project a stress state defined by the the
     * principal stress axes \f$\left(S_{1},S_{2},S_{3}\right)\f$ on
     * the \f$\pi\f$-plane, which is defined by:
     * \[
     * S_{1}+S_{2}+S_{3}=0
     * \]
     * \tparam stress: stress type
     * \param[in] s: eigen values of the stress
     * \return the projection of the stresses on the \f$\pi\f$-plane
     */
    template<typename stress>
    std::tuple<stress,stress>
    project_on_pi_plane(const tfel::math::tvector<3u,stress>&);
    /*!
     * \brief this function project a stress state defined by the the
     * principal stress axes \f$\left(S_{0},S_{1},S_{2}\right)\f$ on
     * the \f$\pi\f$-plane, which is defined by:
     * \[
     * S_{0}+S_{1}+S_{2}=0
     * \]
     * \tparam stress: stress type
     * \param[in] s0: first eigen value
     * \param[in] s1: second eigen value
     * \param[in] s2: third eigen value
     * \return the projection of the stresses on the \f$\pi\f$-plane
     */
    template<typename stress>
    std::tuple<stress,stress> project_on_pi_plane(const stress,
						  const stress,
						  const stress);
    /*!
     * \brief this function project a stress state defined by the the
     * principal stress axes \f$\left(S_{0},S_{1},S_{2}\right)\f$ on
     * the \f$\pi\f$-plane, which is defined by:
     * \[
     * S_{0}+S_{1}+S_{2}=0
     * \]
     * \tparam StressStensor: stress tensor type
     * \param[in] s: stress tensor
     * \return the projection of the stresses on the \f$\pi\f$-plane
     */
    template<typename StressStensor,
	     tfel::math::stensor_common::EigenSolver es =
	     tfel::math::stensor_common::TFELEIGENSOLVER>
    std::tuple<tfel::math::StensorNumType<StressStensor>,
	       tfel::math::StensorNumType<StressStensor>>
    project_on_pi_plane(const StressStensor&);
    
  } // end of namespace material
  
} // end of namespace tfel

#include"TFEL/Material/PiPlane.ixx"

#endif /* LIB_TFEL_MATERIAL_PIPLANE_HXX */
