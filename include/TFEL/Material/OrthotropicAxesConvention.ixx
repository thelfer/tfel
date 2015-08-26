/*!
 * \file   OrthotropicAxesConvention.ixx
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

#ifndef _LIB_TFEL_MATERIAL_ORTHOTROPICAXESCONVENTION_IXX_
#define _LIB_TFEL_MATERIAL_ORTHOTROPICAXESCONVENTION_IXX_

namespace tfel{

  namespace material{

    namespace internals{

      /*!
       * an helper structure to perform template specialisation
       */
      template<ModellingHypothesis::Hypothesis mh,
	     OrthotropicAxesConvention c>
      struct OrthotropicAxesConventionConverter
      {
	//! a simple alias
	static constexpr unsigned short N =
	  ModellingHypothesisToSpaceDimension<mh>::value;
	/*!
	 * convert a symmetric tensor to the new convention
	 */
	template<typename T>
	static TFEL_MATERIAL_INLINE void
	convert(tfel::math::stensor<N,T>&)
	{} // end of convert
      }; // end of struct OrthotropicAxesConventionConverter

      /*!
       * an helper structure used in "*plane*" generalised modelling
       * hypotheses to perform convertion
       */
      struct PipeOrthotropicAxesConventionConverter
      {
	/*!
	 * convert a symmetric tensor to the new convention
	 * \param[in] s : the tensor to be converted
	 */
	template<typename T>
	static TFEL_MATERIAL_INLINE void
	convert(tfel::math::stensor<2u,T>& s)
	{
	  const T tmp = s[1];
	  s[1] = s[2];
	  s[2] = tmp;
	} // end of convert
      };
      
      /*!
       *  partial specialisation for the PLANESTRESS modelling
       * hypothesis and PIPE orthotropic axes convention
       */
      template<>
      struct OrthotropicAxesConventionConverter<ModellingHypothesis::PLANESTRESS,
						OrthotropicAxesConvention::PIPE>
	: public PipeOrthotropicAxesConventionConverter
      {}; // end of struct OrthotropicAxesConventionConverter

      /*!
       *  partial specialisation for the PLANESTRAIN modelling
       * hypothesis and PIPE orthotropic axes convention
       */
      template<>
      struct OrthotropicAxesConventionConverter<ModellingHypothesis::PLANESTRAIN,
						OrthotropicAxesConvention::PIPE>
	: public PipeOrthotropicAxesConventionConverter
      {}; // end of struct OrthotropicAxesConventionConverter

      /*!
       *  partial specialisation for the GENERALISEDPLANESTRAIN
       * modelling hypothesis and PIPE orthotropic axes convention
       */
      template<>
      struct OrthotropicAxesConventionConverter<ModellingHypothesis::GENERALISEDPLANESTRAIN,
						OrthotropicAxesConvention::PIPE>
	: public PipeOrthotropicAxesConventionConverter
      {}; // end of struct OrthotropicAxesConventionConverter
      
    } // end of namespace internals
    
    template<ModellingHypothesis::Hypothesis mh,
	     OrthotropicAxesConvention c,
	     typename T>
    void convert(tfel::math::stensor<ModellingHypothesisToSpaceDimension<mh>::value,T>& s)
    {
      using Converter = tfel::material::internals::OrthotropicAxesConventionConverter<mh,c>;
      Converter::convert(s);
    } // end of convert
    
  } // end of namespace material

} // end of namespace tfel

#endif /* _LIB_TFEL_MATERIAL_ORTHOTROPICAXESCONVENTION_IXX_ */
