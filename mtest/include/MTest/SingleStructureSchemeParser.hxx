/*!
 * \file   SingleStructureSchemeParser.hxx
 * \brief
 * \author Thomas Helfer
 * \date   21 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_SINGLESTRUCTURESCHEMEPARSER_HXX
#define LIB_MTEST_SINGLESTRUCTURESCHEMEPARSER_HXX

#include "MTest/SchemeParserBase.hxx"

namespace mtest {

  struct SingleStructureScheme;

  struct SingleStructureSchemeParser : public SchemeParserBase {
    /*!
     * \brief handle the `@Behaviour` keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleBehaviour(SingleStructureScheme&, tokens_iterator&);
    /*!
     * \brief handle the `@Parameter` keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleParameter(SingleStructureScheme&, tokens_iterator&);
    /*!
     * \brief handle the `@IntegerParameter` keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleIntegerParameter(SingleStructureScheme&,
                                        tokens_iterator&);
    /*!
     * \brief handle the `@UnsignedIntegerParameter` keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleUnsignedIntegerParameter(SingleStructureScheme&,
                                                tokens_iterator&);
    /*!
     * \brief handle the `@OutOfBoundsPolicy` keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleOutOfBoundsPolicy(SingleStructureScheme&,
                                         tokens_iterator&);
    /*!
     * \brief handle the `@MaterialProperty` keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleMaterialProperty(SingleStructureScheme&,
                                        tokens_iterator&);
    /*!
     * \brief handle the `@HandleThermalExpansion` keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleHandleThermalExpansion(SingleStructureScheme&,
                                              tokens_iterator&);
    /*!
     * \brief handle the `@InternalStateVariable` keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleInternalStateVariable(SingleStructureScheme&,
                                             tokens_iterator&);
    /*!
     * \brief handle the `@ExternalStateVariable` keyword
     * \param[in,out] p : position in the input file
     */
    virtual void handleExternalStateVariable(SingleStructureScheme&,
                                             tokens_iterator&);
    //! destructor
    ~SingleStructureSchemeParser() override;

   protected:
    /*!
     * \param[in,out] p : position in the input file
     * \param[out]    n : name of the variable
     */
    void setInternalStateVariableValue(SingleStructureScheme&,
                                       tokens_iterator&,
                                       const std::string&);
    /*!
     * register the call backs associated with each command
     */
    void registerCallBacks() override;
    /*!
     * \brief treat the current keyword
     * \param[in,out] p : position in the input file
     */
    bool treatKeyword(SingleStructureScheme&, tokens_iterator&);
    //! \return the list of keywords
    std::vector<std::string> getKeyWordsList() const;

   private:
    //! a simple alias
    using CallBack = void (SingleStructureSchemeParser::*)(
        SingleStructureScheme&, tokens_iterator&);
    //! callbacks
    std::map<std::string, CallBack> callbacks;
    /*!
     * register a call back
     * \param[in] k : key word
     * \param[in] p : pointer to a member function
     */
    void registerCallBack(const std::string&, const CallBack&);
  };

}  // end of namespace mtest

#endif /* LIB_MTEST_SINGLESTRUCTURESCHEMEPARSER_HXX */
