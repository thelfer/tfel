/*!
 * \file   mfront/include/MFront/UMAT/UMATComputeStiffnessTensor.hxx
 * \brief  This file declares the UMATComputeStiffnessTensor class
 * \author Helfer Thomas
 * \date   18 mar 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_UMATCOMPUTESTIFFNESSTENSOR_H_
#define LIB_MFRONT_UMATCOMPUTESTIFFNESSTENSOR_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Config/TFELTypes.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/UMAT/UMAT.hxx"
#include"MFront/UMAT/UMATConfig.hxx"
#include"MFront/UMAT/UMATTraits.hxx"

namespace umat
{

  /*!
   * \brief compute the stiffness tensor for axisymmetrical
   * generalised hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_UMAT_VISIBILITY_EXPORT void
  UMATComputeIsotropicStiffnessTensor1D(tfel::config::Types<1u,UMATReal,false>::StiffnessTensor&,
					const UMATReal* const);

  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_UMAT_VISIBILITY_EXPORT void
  UMATComputeIsotropicStiffnessTensor2D(tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&,
					const UMATReal* const);

  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_UMAT_VISIBILITY_EXPORT void
  UMATComputeIsotropicPlaneStressAlteredStiffnessTensor(tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&,
							const UMATReal* const);

  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_UMAT_VISIBILITY_EXPORT void
  UMATComputeIsotropicStiffnessTensor3D(tfel::config::Types<3u,UMATReal,false>::StiffnessTensor&,
					const UMATReal* const);

  /*!
   * \brief compute the stiffness tensor for axisymmetrical
   * generalised hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_UMAT_VISIBILITY_EXPORT void
  UMATComputeOrthotropicStiffnessTensor1D(tfel::config::Types<1u,UMATReal,false>::StiffnessTensor&,
					  const UMATReal* const);

  /*!
   * \brief compute the stiffness tensor for plane strain, generalised
   * plane strain and axisymmetrical hypotheses.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_UMAT_VISIBILITY_EXPORT void
  UMATComputeOrthotropicStiffnessTensor2D(tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&,
					  const UMATReal* const);

  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_UMAT_VISIBILITY_EXPORT void
  UMATComputeOrthotropicPlaneStressAlteredStiffnessTensor(tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&,
							  const UMATReal* const);

  /*!
   * \brief compute the stiffness tensor for plane stress modelling
   * hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_UMAT_VISIBILITY_EXPORT void
  UMATComputeOrthotropicPlaneStressUnAlteredStiffnessTensor(tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&,
							    const UMATReal* const);

  /*!
   * \brief compute the stiffness tensor for the tridimensionnal
   * modelling hypothesis.
   * \param[out] C     : stiffness tensor
   * \param[in]  props : material properties
   */
  MFRONT_UMAT_VISIBILITY_EXPORT void
  UMATComputeOrthotropicStiffnessTensor3D(tfel::config::Types<3u,UMATReal,false>::StiffnessTensor&,
					  const UMATReal* const);
  
  /*!
   * This structure is in charge of computing the Stiffness operator
   * from the material properties given by Cast3M.  The resulting
   * operator uses MFront representation of tensors and symmetric
   * tensors.
   */
  template<UMATBehaviourType,
	   tfel::material::ModellingHypothesis::Hypothesis,
	   UMATSymmetryType,bool>
  struct UMATComputeStiffnessTensor;

  template<bool b>
  struct TFEL_VISIBILITY_LOCAL
  UMATComputeStiffnessTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
			     ISOTROPIC,b>
  {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<1u,UMATReal,false>::StiffnessTensor& C,
	const UMATReal* const props)
    {
      UMATComputeIsotropicStiffnessTensor1D(C,props);
    }
  }; // end of struct UMATComputeStiffnessTensor

  template<bool b>
  struct TFEL_VISIBILITY_LOCAL
  UMATComputeStiffnessTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::AXISYMMETRICAL,
			     ISOTROPIC,b>
  {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<2u,UMATReal,false>::StiffnessTensor& C,
	const UMATReal* const props)
    {
      UMATComputeIsotropicStiffnessTensor2D(C,props);
    }
  }; // end of struct UMATComputeStiffnessTensor

  template<>
  struct TFEL_VISIBILITY_LOCAL
  UMATComputeStiffnessTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::PLANESTRESS,
			     ISOTROPIC,true>
  {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<2u,UMATReal,false>::StiffnessTensor& C,
	const UMATReal* const props)
    {
      UMATComputeIsotropicStiffnessTensor2D(C,props);
    }
  }; // end of struct UMATComputeStiffnessTensor

  template<>
  struct TFEL_VISIBILITY_LOCAL
  UMATComputeStiffnessTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::PLANESTRESS,
			     ISOTROPIC,false>
  {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<2u,UMATReal,false>::StiffnessTensor& C,
	const UMATReal* const props)
    {
      UMATComputeIsotropicPlaneStressAlteredStiffnessTensor(C,props);
    }
  }; // end of struct UMATComputeStiffnessTensor

  template<bool b>
  struct TFEL_VISIBILITY_LOCAL
  UMATComputeStiffnessTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::PLANESTRAIN,
			     ISOTROPIC,b>
  {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<2u,UMATReal,false>::StiffnessTensor& C,
	const UMATReal* const props)
    {
      UMATComputeIsotropicStiffnessTensor2D(C,props);
    }
  }; // end of struct UMATComputeStiffnessTensor

  template<bool b>
  struct TFEL_VISIBILITY_LOCAL
  UMATComputeStiffnessTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
			     ISOTROPIC,b>
  {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<2u,UMATReal,false>::StiffnessTensor& C,
	const UMATReal* const props)
    {
      UMATComputeIsotropicStiffnessTensor2D(C,props);
    }
  }; // end of struct UMATComputeStiffnessTensor

  template<bool b>
  struct TFEL_VISIBILITY_LOCAL
  UMATComputeStiffnessTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
			     ISOTROPIC,b>
  {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<3u,UMATReal,false>::StiffnessTensor& C,
	const UMATReal* const props)
    {
      UMATComputeIsotropicStiffnessTensor3D(C,props);
    }
  }; // end of struct UMATComputeStiffnessTensor

  template<bool b>
  struct TFEL_VISIBILITY_LOCAL
  UMATComputeStiffnessTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
			     ORTHOTROPIC,b>
  {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<1u,UMATReal,false>::StiffnessTensor& C,
	const UMATReal* const props)
    {
      UMATComputeOrthotropicStiffnessTensor1D(C,props);
    }
  }; // end of struct UMATComputeStiffnessTensor

  template<bool b>
  struct TFEL_VISIBILITY_LOCAL
  UMATComputeStiffnessTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::AXISYMMETRICAL,ORTHOTROPIC,b>
  {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<2u,UMATReal,false>::StiffnessTensor& C,
	const UMATReal* const props)
    {
      UMATComputeOrthotropicStiffnessTensor2D(C,props);
    }
  }; // end of struct UMATComputeStiffnessTensor

  template<>
  struct TFEL_VISIBILITY_LOCAL
  UMATComputeStiffnessTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::PLANESTRESS,
			     ORTHOTROPIC,true>
  {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<2u,UMATReal,false>::StiffnessTensor& C,
	const UMATReal* const props)
    {
      UMATComputeOrthotropicPlaneStressUnAlteredStiffnessTensor(C,props);
    }
  }; // end of struct UMATComputeStiffnessTensor

  template<>
  struct TFEL_VISIBILITY_LOCAL
  UMATComputeStiffnessTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::PLANESTRESS,
			     ORTHOTROPIC,false>
  {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<2u,UMATReal,false>::StiffnessTensor& C,
	const UMATReal* const props)
    {
      UMATComputeOrthotropicPlaneStressAlteredStiffnessTensor(C,props);
    }
  }; // end of struct UMATComputeStiffnessTensor

  template<bool b>
  struct TFEL_VISIBILITY_LOCAL
  UMATComputeStiffnessTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::PLANESTRAIN,
			     ORTHOTROPIC,b>
  {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<2u,UMATReal,false>::StiffnessTensor& C,
	const UMATReal* const props)
    {
      UMATComputeOrthotropicStiffnessTensor2D(C,props);
    }
  }; // end of struct UMATComputeStiffnessTensor

  template<bool b>
  struct TFEL_VISIBILITY_LOCAL
  UMATComputeStiffnessTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,
			     ORTHOTROPIC,b>
  {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<2u,UMATReal,false>::StiffnessTensor& C,
	const UMATReal* const props)
    {
      UMATComputeOrthotropicStiffnessTensor2D(C,props);      
    }
  }; // end of struct UMATComputeStiffnessTensor

  template<bool b>
  struct TFEL_VISIBILITY_LOCAL
  UMATComputeStiffnessTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			     tfel::material::ModellingHypothesis::TRIDIMENSIONAL,
			     ORTHOTROPIC,b>
  {
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(tfel::config::Types<3u,UMATReal,false>::StiffnessTensor& C,
	const UMATReal* const props)
    {
      UMATComputeOrthotropicStiffnessTensor3D(C,props);      
    }
  }; // end of struct UMATComputeStiffnessTensor

  /*!
   * Partial specialisation for finite strain behaviours
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   UMATSymmetryType stype,bool b>
  struct TFEL_VISIBILITY_LOCAL
  UMATComputeStiffnessTensor<umat::FINITESTRAINSTANDARDBEHAVIOUR,H,stype,b>
  {
    typedef tfel::material::ModellingHypothesisToSpaceDimension<H> ModellingHypothesisToSpaceDimension;
    /*!
     * \brief compute the stiffness tensor.
     * \param[out] C     : stiffness tensor
     * \param[in]  props : material properties
     */
    static void
    exe(typename tfel::config::Types<ModellingHypothesisToSpaceDimension::value,UMATReal,false>::StiffnessTensor& D,
	const UMATReal* const props)
    {
      UMATComputeStiffnessTensor<umat::SMALLSTRAINSTANDARDBEHAVIOUR,H,stype,b>::exe(D,props);
    }
  }; // end of struct UMATComputeStiffnessTensor

} // end of namespace umat

#endif /* LIB_MFRONT_UMATCOMPUTESTIFFNESSTENSOR_H_ */

