/*! 
 * \file  mfront/src/MFrontPowellDogLegAlgorithmBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 22 août 2014
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

