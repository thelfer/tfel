/*! 
 * \file  include/TFEL/Material/ModellingHypothesis.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 16 mar 2011
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATERIAL_MODELLINGHYPOTHESIS_H_
#define _LIB_TFEL_MATERIAL_MODELLINGHYPOTHESIS_H_ 

#include<string>
#include<vector>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel
{

  namespace material
  {
    
    /*!
     * a helper structure defining the Hypothesis enum.
     */
    struct TFELMATERIAL_VISIBILITY_EXPORT ModellingHypothesis
    {
      /*!
       * the list of supported modelling hypothesis
       */
      enum Hypothesis{
	AXISYMMETRICALGENERALISEDPLANESTRAIN,
	AXISYMMETRICALGENERALISEDPLANESTRESS,
	AXISYMMETRICAL,
	PLANESTRESS,
	PLANESTRAIN,
	GENERALISEDPLANESTRAIN,
	TRIDIMENSIONAL,
	UNDEFINEDHYPOTHESIS
      }; // end of enum ModellingHypothesis

      static const std::vector<Hypothesis>&
      getModellingHypotheses(void);

      /*!
       * convert a string to a modelling hypothesis
       * 
       * AXISYMMETRICALGENERALISEDPLANESTRAIN <=> AxisymmetricalGeneralisedPlaneStrain
       * AXISYMMETRICALGENERALISEDPLANESTRESS <=> AxisymmetricalGeneralisedPlaneStress,
       * AXISYMMETRICAL                       <=> Axisymmetrical
       * PLANESTRESS                          <=> PlaneStress			     
       * PLANESTRAIN                          <=> PlaneStrain
       * GENERALISEDPLANESTRAIN               <=> GeneralisedPlaneStrain		     
       * TRIDIMENSIONAL                       <=> Tridimensional
       */
      static Hypothesis
      fromString(const std::string&);
      /*!
       * \return a string correpsonding to the modelling hypothesis
       * AXISYMMETRICALGENERALISEDPLANESTRAIN <=> AxisymmetricalGeneralisedPlaneStrain
       * AXISYMMETRICALGENERALISEDPLANESTRESS <=> AxisymmetricalGeneralisedPlaneStress,
       * AXISYMMETRICAL                       <=> Axisymmetrical
       * PLANESTRESS                          <=> PlaneStress			     
       * PLANESTRAIN                          <=> PlaneStrain
       * GENERALISEDPLANESTRAIN               <=> GeneralisedPlaneStrain		     
       * TRIDIMENSIONAL                       <=> Tridimensional
       */
      static std::string
      toString(const Hypothesis);
      /*!
       * \return an upper case string correpsonding to the modelling
       * hypothesis
       */
      static std::string
      toUpperCaseString(const Hypothesis);
      /*!
       * \return true if the given string stands for a modelling
       * hypothesis
       */
      static bool
      isModellingHypothesis(const std::string&);
    }; // end of struct ModellingHypothesis

    /*!
     * a metafunction returning the space dimension of a modelling
     * hypothesis.
     */
    template<ModellingHypothesis::Hypothesis>
    struct ModellingHypothesisToSpaceDimension;

    /*!
     * partial specialisation for
     * ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN
     */
    template<>
    struct ModellingHypothesisToSpaceDimension<ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN>
    {
      //! the return of the metafunction
      static const unsigned short value = 1u;
    };

    /*!
     * partial specialisation for
     * ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS
     */
    template<>
    struct ModellingHypothesisToSpaceDimension<ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS>
    {
      //! the return of the metafunction
      static const unsigned short value = 1u;
    };

    /*!
     * partial specialisation for
     * ModellingHypothesis::AXISYMETRICAL
     */
    template<>
    struct ModellingHypothesisToSpaceDimension<ModellingHypothesis::AXISYMMETRICAL>
    {
      //! the return of the metafunction
      static const unsigned short value = 2u;
    };

    /*!
     * partial specialisation for
     * ModellingHypothesis::PLANESTRESS
     */
    template<>
    struct ModellingHypothesisToSpaceDimension<ModellingHypothesis::PLANESTRESS>
    {
      //! the return of the metafunction
      static const unsigned short value = 2u;
    };

    /*!
     * partial specialisation for
     * ModellingHypothesis::PLANESTRAIN
     */
    template<>
    struct ModellingHypothesisToSpaceDimension<ModellingHypothesis::PLANESTRAIN>
    {
      //! the return of the metafunction
      static const unsigned short value = 2u;
    };

    /*!
     * partial specialisation for
     * ModellingHypothesis::GENERALISEDPLANESTRAIN
     */
    template<>
    struct ModellingHypothesisToSpaceDimension<ModellingHypothesis::GENERALISEDPLANESTRAIN>
    {
      //! the return of the metafunction
      static const unsigned short value = 2u;
    };

    /*!
     * partial specialisation for
     * ModellingHypothesis::TRIDIMENSIONAL
     */
    template<>
    struct ModellingHypothesisToSpaceDimension<ModellingHypothesis::TRIDIMENSIONAL>
    {
      //! the return of the metafunction
      static const unsigned short value = 3u;
    };

  } // end of namespace material

} // end of namespace tfel

#endif /* _LIB_TFEL_MATERIAL_MODELLINGHYPOTHESIS_H */

