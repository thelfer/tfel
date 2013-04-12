/*! 
 * \file  ModellingHypothesis.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 16 mar 2011
 */

#ifndef _LIB_TFEL_MATERIAL_MODELLINGHYPOTHESIS_H_
#define _LIB_TFEL_MATERIAL_MODELLINGHYPOTHESIS_H_ 

namespace tfel
{

  namespace material
  {
    
    /*!
     * a helper structure defining the Hypothesis enum.
     */
    struct ModellingHypothesis
    {
      
      /*!
       * the list of supported modelling hypothesis
       */
      enum Hypothesis{
	AXISYMMETRICALGENERALISEDPLANESTRAIN,
	AXISYMMETRICAL,
	PLANESTRESS,
	PLANESTRAIN,
	GENERALISEDPLANESTRAIN,
	TRIDIMENSIONAL,
	UNDEFINEDHYPOTHESIS
      }; // end of enum ModellingHypothesis

    }; // end of struct ModellingHypothesis

    /*!
     * a metafunction returning the space dimension of a modelling
     * hypothesis.
     */
    template<ModellingHypothesis::Hypothesis>
    struct ModellingHypothesisToSpaceDimension;

    /*!
     * partial specialisation for
     * ModellingHypothesis::AXISYMETRICALGENERALISEDPLANESTRAIN
     */
    template<>
    struct ModellingHypothesisToSpaceDimension<ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN>
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

