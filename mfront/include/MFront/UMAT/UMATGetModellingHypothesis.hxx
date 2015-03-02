/*! 
 * \file  mfront/include/MFront/UMAT/UMATGetModellingHypothesis.hxx
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

#ifndef LIB_MFRONT_UMAT_UMATGETMODELLINGHYPOTHESIS_H_
#define LIB_MFRONT_UMAT_UMATGETMODELLINGHYPOTHESIS_H_ 

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

#endif /* LIB_MFRONT_UMAT_UMATGETMODELLINGHYPOTHESIS_H_ */

