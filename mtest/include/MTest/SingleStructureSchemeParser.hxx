/*!
 * \file   SingleStructureSchemeParser.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   21 déc. 2015
 */

#ifndef _LIB_MTEST_SINGLESTRUCTURESCHEMEPARSER_HXX_
#define _LIB_MTEST_SINGLESTRUCTURESCHEMEPARSER_HXX_

#include"MTest/SchemeParserBase.hxx"

namespace mtest{

  struct SingleStructureScheme;
  
  struct SingleStructureSchemeParser
    : public SchemeParserBase
  {
    /*!
     * handle the @Behaviour keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleBehaviour(SingleStructureScheme&,TokensContainer::const_iterator&);
    /*!
     * handle the @Parameter keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleParameter(SingleStructureScheme&,TokensContainer::const_iterator&);
    /*!
     * handle the @IntegerParameter keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleIntegerParameter(SingleStructureScheme&,TokensContainer::const_iterator&);
    /*!
     * handle the @UnsignedIntegerParameter keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleUnsignedIntegerParameter(SingleStructureScheme&,TokensContainer::const_iterator&);
    /*!
     * handle the @OutOfBoundsPolicy keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleOutOfBoundsPolicy(SingleStructureScheme&,TokensContainer::const_iterator&);
    /*!
     * handle the @MaterialProperty keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleMaterialProperty(SingleStructureScheme&,TokensContainer::const_iterator&);
    /*!
     * handle the @HandleThermalExpansion keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleHandleThermalExpansion(SingleStructureScheme&,TokensContainer::const_iterator&);
    /*!
     * handle the @InternalStateVariable keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleInternalStateVariable(SingleStructureScheme&,TokensContainer::const_iterator&);
    /*!
     * handle the @ExternalStateVariable keyword
     * \param[in,out] p : position in the input file
     */
    virtual void
    handleExternalStateVariable(SingleStructureScheme&,TokensContainer::const_iterator&);
    //! destructor
    ~SingleStructureSchemeParser();
  protected:
    /*!
     * \param[in,out] p : position in the input file
     * \param[out]    n : name of the variable
     */
    void
    setInternalStateVariableValue(SingleStructureScheme&,
				  TokensContainer::const_iterator&,
				  const std::string&);
    /*!
     * register the call backs associated with each command
     */
    virtual void
    registerCallBacks(void) override;
    /*!
     * \brief treat the current keyword
     * \param[in,out] p : position in the input file
     */
    bool treatKeyword(SingleStructureScheme&,TokensContainer::const_iterator&);
    //! \return the list of keywords
    std::vector<std::string> getKeyWordsList(void) const;
  private:
    //! a simple alias
    using CallBack = void (SingleStructureSchemeParser::*)(SingleStructureScheme&,
							   TokensContainer::const_iterator&);
    //! callbacks
    std::map<std::string,CallBack> callbacks;
    /*!
     * register a call back
     * \param[in] k : key word
     * \param[in] p : pointer to a member function
     */
    void registerCallBack(const std::string&,const CallBack&);
  };
  
} // end of namespace mtest

#endif /* _LIB_MTEST_SINGLESTRUCTURESCHEMEPARSER_HXX_ */
