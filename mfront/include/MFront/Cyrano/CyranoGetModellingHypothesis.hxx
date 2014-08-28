/*! 
 * \file  mfront/include/MFront/Cyrano/CyranoGetModellingHypothesis.hxx
 * \brief
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#ifndef _LIB_MFRONT_CYRANO_CYRANOGETMODELLINGHYPOTHESIS_H_
#define _LIB_MFRONT_CYRANO_CYRANOGETMODELLINGHYPOTHESIS_H_ 

#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/Cyrano/CyranoConfig.hxx"
#include"MFront/Cyrano/Cyrano.hxx"

namespace cyrano
{

  /*!
   * \return the modelling hypothesis contained in the NDI parameter
   * used by the Cyrano Interface of the Cast3M finite element solver
   *
   * \param[in] ndi : integer used by the Cast3M finite element solver
   * to represent the modelling hypothesis
   */
  MFRONT_CYRANO_VISIBILITY_EXPORT
  tfel::material::ModellingHypothesis::Hypothesis
  getModellingHypothesis(const CyranoInt);

  /*!
   * \return the identifier associted with the given modelling hypothesis
   *
   * \param[in] h : modelling hypothesis
   */
  MFRONT_CYRANO_VISIBILITY_EXPORT CyranoInt
  getModellingHypothesisIdentifier(const tfel::material::ModellingHypothesis::Hypothesis&);
  
} // end of namespace cyrano

#endif /* _LIB_MFRONT_CYRANO_CYRANOGETMODELLINGHYPOTHESIS_H */

