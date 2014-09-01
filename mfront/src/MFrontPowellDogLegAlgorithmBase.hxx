/*! 
 * \file  mfront/src/MFrontPowellDogLegAlgorithmBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 22 août 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONTPOWELLDOGLEGALGORITHMBASE_H_
#define _LIB_MFRONTPOWELLDOGLEGALGORITHMBASE_H_ 

namespace mfront{

  /*!
   * base class providing some helper function to build non linear
   * systeme solver based on the Powell dogleg algorithm
   */
  struct MFrontPowellDogLegAlgorithmBase
  {
  protected:
    /*!
     * write a step of the Powell dogleg method
     * \param[in] h  : modelling hypothesis
     * \param[in] B  : name of the jacobian
     * \param[in] f  : name of values of f
     * \param[in] pn : Newton step
     */
    virtual void writePowellDogLegStep(const Hypothesis,
				       const std::string&,
				       const std::string&,
				       const std::string&);
  }; // end of struct MFrontPowellDogLegAlgorithmBase

} // end of namespace mfront

#endif /* _LIB_MFRONTPOWELLDOGLEGALGORITHMBASE_H */

