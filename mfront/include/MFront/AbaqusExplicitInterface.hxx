/*!
 * \file   AbaqusExplicitInterface.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   16 mars 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_ABAQUSEXPLICITINTERFACE_HXX
#define LIB_MFRONT_ABAQUSEXPLICITINTERFACE_HX

#include"MFront/AbaqusInterfaceBase.hxx"

namespace mfront{

  /*!
   * Interface dedicated to the Abaqus-Explicit finite element solver
   */
  struct AbaqusExplicitInterface
    : public AbaqusInterfaceBase
  {
    /*!
     * \return the name of the interface
     */
    static std::string
    getName(void);
    /*!
     * \param[in] k  : keyword treated
     * \param[in] p  : iterator to the current token
     * \param[in] pe : iterator past the end of the file
     * \return a pair. The first entry is true if the keyword was
     * treated by the interface. The second entry is an iterator after
     * the last token treated.
     */
    virtual std::pair<bool,CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 CxxTokenizer::TokensContainer::const_iterator,
		 const CxxTokenizer::TokensContainer::const_iterator) override;
    /*!
     * \brief write output files
     * \param[in] mb : behaviour description
     * \param[in] fd : mfront file description
     */
    virtual void
    endTreatment(const BehaviourDescription&,
		 const FileDescription&) const override;
    /*!
     * write interface specific includes
     * \param[in] out : output file
     * \param[in] mb  : behaviour description
     */
    virtual void 
    writeInterfaceSpecificIncludes(std::ostream&,
				   const BehaviourDescription&) const override;
    /*!
     * \param[out] d  : target description
     * \param[in]  bd : behaviour description
     */
    virtual void getTargetsDescription(TargetsDescription&,
				       const BehaviourDescription&) override;
    //! destructor
    virtual ~AbaqusExplicitInterface();
    
  protected:
    /*!
     * \return the name of the interface
     */
    virtual std::string
    getInterfaceName(void) const override;
    /*!
     * \brief write the body of the VUMAT function
     * \param[out] out: ouput stream
     * \param[in]  md:  behaviour description
     * \param[in]  t:   floatting point type
     */
    virtual void
    writeVUMATFunction(std::ostream&,
		       const BehaviourDescription&,
		       const std::string&) const;
  };
  
} // end of namespace mfront

#endif /* LIB_MFRONT_ABAQUSEXPLICITINTERFACE_HXX_ */
