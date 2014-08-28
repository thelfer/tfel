/*! 
 * \file  mfront/include/MFront/UMAT/UMATGetModellingHypothesis.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 16 mar 2011
 */

#ifndef _LIB_MFRONT_UMAT_UMATGETMODELLINGHYPOTHESIS_H_
#define _LIB_MFRONT_UMAT_UMATGETMODELLINGHYPOTHESIS_H_ 

#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/UMAT/UMATConfig.hxx"
#include"MFront/UMAT/UMAT.hxx"

namespace umat
{

  /*!
   * \return the modelling hypothesis contained in the NDI parameter
   * used by the UMAT Interface of the Cast3M finite element solver
   *
   * \param[in] ndi : integer used by the Cast3M finite element solver
   * to represent the modelling hypothesis
   */
  MFRONT_UMAT_VISIBILITY_EXPORT
  tfel::material::ModellingHypothesis::Hypothesis
  getModellingHypothesis(const UMATInt);
  
} // end of namespace umat

#endif /* _LIB_MFRONT_UMAT_UMATGETMODELLINGHYPOTHESIS_H */

