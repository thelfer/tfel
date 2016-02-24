/*!
 * \file   OrthotropicAxesConvention.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   24 août 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATERIAL_ORTHOTROPICAXESCONVENTION_HXX_
#define _LIB_TFEL_MATERIAL_ORTHOTROPICAXESCONVENTION_HXX_

#include"TFEL/Math/stensor.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

namespace tfel{

  namespace material{

    /*!
     * Most finite element solver can't have a uniq definition of the
     * orthotropic axes for all the modelling hypotheses. One thus
     * need to define a convention. This leads to many troubles when
     * trying to define an the hill tensor or the thermal expansion.
     *
     * For example, one can define a pipe using the following axes
     * definition:
     * - (rr,zz,tt,...) in 3D, 2D Axysymmetric, 1D Axysymmetric
     *   generalised plane strain or generalised plane stress.
     * - (rr,zz,tt,...) in 2D plane stress, strain, generalized plane
     *   strain.
     * This means that we have to exchange the second and third
     * axis. This is the PIPE (orthotropic axes) convention.
     */
    enum class OrthotropicAxesConvention{
      DEFAULT,
      PIPE
    }; // end of enum class OrthotropicAxesConvention

    /*!
     * convert a diagonal tensor from a convention to another.
     * \tparam mh       : modelling hypothesis
     * \tparam c        : convention for the orthotropic axes
     * \tparam T        : type holded by the tensor
     * \param[in/out] s : stensor
     */
    template<ModellingHypothesis::Hypothesis mh,OrthotropicAxesConvention c,typename T>
    void convertStressFreeExpansionStrain(tfel::math::stensor<ModellingHypothesisToSpaceDimension<mh>::value,T>&);
				 
  } // end of namespace material

} // end of namespace tfel

#include"TFEL/Material/OrthotropicAxesConvention.ixx"
  
#endif /* _LIB_TFEL_MATERIAL_ORTHOTROPICAXESCONVENTION_HXX_ */


