/*!
 * \file  BehaviourBrickBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   21 oct. 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICKBASE_HXX
#define LIB_MFRONT_BEHAVIOURBRICKBASE_HXX

#include "MFront/AbstractBehaviourBrick.hxx"

namespace tfel {
  namespace glossary {
    struct GlossaryEntry;
  }  // end of namespace glossary
}  // end of namespace tfel

namespace mfront {

  //! forward declaration
  struct VariableDescriptionContainer;
  //! forward declaration
  struct AbstractBehaviourDSL;
  //! forward declaration
  struct BehaviourData;
  //! forward declaration
  struct BehaviourDescription;

  /*!
   * an helper class providing a set of helper method to build BehaviourBricks.
   */
  struct BehaviourBrickBase : public AbstractBehaviourBrick {
    /*!
     * constructor
     * \param[in] dsl_ : domain specific language
     * \param[in] bd_  : behaviour description
     */
    BehaviourBrickBase(AbstractBehaviourDSL&, BehaviourDescription&);
    /*!
     * \brief add requirements for the given modelling hypothesis
     * \param[in] r : requirement manager
     * \param[in] h : modelling hypothesis
     */
    void addRequirements(bbrick::RequirementManager&,
                         const Hypothesis) const override;
    /*!
     * \brief treat a keyword
     * \param[in] key: keyword to be treated
     * \param[in,out] p:   current position in the file
     * \param[in] pe:  iterator past the end of the file
     * \return a pair. The first entry is true if the keyword was
     * treated by the interface. The second entry is an iterator after
     * the last token treated.
     */
    std::pair<bool, tokens_iterator> treatKeyword(
        const std::string&, tokens_iterator&, const tokens_iterator) override;
    //! destructor
    ~BehaviourBrickBase() override;

   protected:
    //! a simple alias
    using GlossaryEntry = tfel::glossary::GlossaryEntry;
    /*!
     * \brief check that the options names are in a given set
     * of keys.
     * \param[in] d: options
     */
    virtual void checkOptionsNames(const DataMap&);
    /*!
     * \brief throw an exception if the given parameter has a value
     * \param[in] p : parameter
     */
    virtual void checkThatParameterHasNoValue(const Parameter&) const;
    /*!
     * \brief add a new material property
     * \param[in] t: type of the material property
     * \param[in] n: name of the variable
     * \param[in] g: glossary name
     * \param[in] s: array size
     */
    virtual void addMaterialPropertyIfNotDefined(
        const std::string&,
        const std::string&,
        const GlossaryEntry&,
        const unsigned short = 1u) const;
    /*!
     * \brief add a new material property
     * \param[in] t: type of the material property
     * \param[in] n: name of the variable
     * \param[in] e: entry name
     * \param[in] s: array size
     */
    virtual void addMaterialPropertyIfNotDefined(
        const std::string&,
        const std::string&,
        const std::string&,
        const unsigned short = 1u) const;
    /*!
     * \brief add a new external state variable
     * \param[in] t: type of the material property
     * \param[in] n: name of the variable
     * \param[in] s: array size
     */
    virtual void addExternalStateVariable(const std::string&,
                                          const std::string&,
                                          const unsigned short = 1u) const;
    /*!
     * \brief add a new external state variable
     * \param[in] t: type of the material property
     * \param[in] n: name of the variable
     * \param[in] g: glossary entry
     * \param[in] s: array size
     */
    virtual void addExternalStateVariable(const std::string&,
                                          const std::string&,
                                          const GlossaryEntry&,
                                          const unsigned short = 1u) const;
    /*!
     * \brief add a new external state variable
     * \param[in] t: type of the material property
     * \param[in] n: name of the variable
     * \param[in] e: entry name
     * \param[in] s: array size
     */
    virtual void addExternalStateVariable(const std::string&,
                                          const std::string&,
                                          const std::string&,
                                          const unsigned short = 1u) const;
    /*!
     * \brief add a new local variable
     * \param[in] t: type of the material property
     * \param[in] n: name of the variable
     * \param[in] s: array size
     */
    virtual void addLocalVariable(const std::string&,
                                  const std::string&,
                                  const unsigned short = 1u) const;
    /*!
     * \brief add a new material property
     * \param[in] n: name of the variable
     * \param[in] e: entry name
     * \param[in] p: parameter default value
     */
    virtual void addParameter(const std::string&,
                              const GlossaryEntry& e,
                              const double) const;
    /*!
     * \brief add a new material property
     * \param[in] n: name of the variable
     * \param[in] e: entry name
     * \param[in] s: array size
     * \param[in] p: parameter default value
     */
    virtual void addParameter(const std::string&,
                              const GlossaryEntry&,
                              const unsigned short,
                              const double) const;
    /*!
     * \brief add a new material property
     * \param[in] n: name of the variable
     * \param[in] e: entry name
     * \param[in] s: array size
     * \param[in] p: parameter default values
     */
    virtual void addParameter(const std::string&,
                              const GlossaryEntry&,
                              const unsigned short,
                              const std::vector<double>&) const;
    /*!
     * \brief add a new material property
     * \param[in] n: name of the variable
     * \param[in] e: entry name
     * \param[in] p: parameter default value
     */
    virtual void addParameter(const std::string&,
                              const std::string&,
                              const double) const;
    /*!
     * \brief add a new material property
     * \param[in] n: name of the variable
     * \param[in] e: entry name
     * \param[in] s: array size
     * \param[in] p: parameter default value
     */
    virtual void addParameter(const std::string&,
                              const std::string&,
                              const unsigned short,
                              const double) const;
    /*!
     * \brief add a new material property
     * \param[in] n: name of the variable
     * \param[in] e: entry name
     * \param[in] s: array size
     * \param[in] p: parameter default values
     */
    virtual void addParameter(const std::string&,
                              const std::string&,
                              const unsigned short,
                              const std::vector<double>&) const;
    //! calling domain specific language
    AbstractBehaviourDSL& dsl;
    //! mechanical behaviour description of which the BehaviourBrick acts
    BehaviourDescription& bd;
  };  // end of struct BehaviourBrickBase
}  // namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICKBASE_H */
