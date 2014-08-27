/*! 
 * \file  MFrontPowellDogLegAlgorithmBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 22 août 2014
 */

#ifndef _LIB_MFRONT_MFRONTPOWELLDOGLEGALGORITHMBASE_H_
#define _LIB_MFRONT_MFRONTPOWELLDOGLEGALGORITHMBASE_H_ 

#include<iosfwd>
#include<string>

#include"TFEL/Material/ModellingHypothesis.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"

namespace mfront{

  /*!
   * base class providing some helper function to build non linear
   * systeme solver based on the Powell dogleg algorithm
   */
  struct MFrontPowellDogLegAlgorithmBase
  {
  protected:
    /*!
     * \return the reserved names
     */
    static std::vector<std::string>
    getReservedNames(void);
    /*!
     * \brief write the algorithm specific members
     * \param[in,out] md  : mechanical description
     * \param[in] key     : keyword
     * \param[in] p       : current position in file (after keyword)
     * \param[in] pe      : end of file
     */
    static std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatSpecificKeywords(MechanicalBehaviourDescription&,
			  const std::string& key,
			  const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
			  const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator);
    /*!
     * \brief method called at the end of the input file processing.
     * Solver can use it to declared additional parameters
     * \param[in,out] md  : mechanical description
     */
    static void
    endsInputFileProcessing(MechanicalBehaviourDescription&);
    /*!
     * write a step of the Powell dogleg method
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     * \param[in] h   : modelling hypothesis
     * \param[in] B   : name of the jacobian
     * \param[in] f   : name of values of f
     * \param[in] pn  : Newton or quasi-Newton step
     */
    static void
    writePowellDogLegStep(std::ostream&,
			  const MechanicalBehaviourDescription&,
			  const tfel::material::ModellingHypothesis::Hypothesis,
			  const std::string&,
			  const std::string&,
			  const std::string&);
    /*!
     * destructor
     */
    ~MFrontPowellDogLegAlgorithmBase();
  }; // end of struct MFrontPowellDogLegAlgorithmBase

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTPOWELLDOGLEGALGORITHMBASE_H */
