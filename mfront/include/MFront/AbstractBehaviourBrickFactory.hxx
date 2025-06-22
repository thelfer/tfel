/*!
 * \file   mfront/include/MFront/AbstractBehaviourBrickFactory.hxx
 * \brief
 * \author Thomas Helfer
 * \date   October 20, 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTBEHAVIOURBRICKFACTORY_HXX
#define LIB_MFRONT_MFRONTBEHAVIOURBRICKFACTORY_HXX

#include <map>
#include <string>
#include <memory>

#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/AbstractBehaviourBrick.hxx"

namespace mfront {

  // forward declaration
  struct AbstractBehaviourDSL;
  // forward declaration
  struct BehaviourDescription;

  /*!
   * register the non linear AbstractBehaviourBrick used by the implicit parser
   */
  struct AbstractBehaviourBrickFactory {
    //! a simple alias
    using CxxTokenizer = tfel::utilities::CxxTokenizer;
    //! a simple alias
    using tokens_iterator = CxxTokenizer::const_iterator;
    //! a simple alias
    using constructor = std::shared_ptr<AbstractBehaviourBrick> (*)(
        AbstractBehaviourDSL&,
        BehaviourDescription&,
        const AbstractBehaviourBrick::Parameters&,
        tokens_iterator& p,
        const tokens_iterator pe);
    /*!
     * \return the uniq instance of the AbstractBehaviourBrick factory
     */
    static AbstractBehaviourBrickFactory& getFactory();
    /*!
     * \return the requested behaviour brick
     * \param[in]     a:   behaviour brick name
     * \param[in,out] dsl: calling domain specific language
     * \param[in,out] mb:  mechanical behaviour description to be
     * treated
     * \param[in]     bp:  brick parameters
     * \param[in,out] p:   iterator to the current position in the input
     * file
     * \param[in]     pe:  iterator to past-the-last token
     */
    std::shared_ptr<AbstractBehaviourBrick> get(
        const std::string&,
        AbstractBehaviourDSL&,
        BehaviourDescription&,
        const AbstractBehaviourBrick::Parameters&,
        tokens_iterator& p,
        const tokens_iterator pe) const;
    /*!
     * \param[in] a : AbstractBehaviourBrick name
     * \param[in] c : AbstractBehaviourBrick constructor
     */
    void registerAbstractBehaviourBrick(const std::string&, const constructor);
    /*!
     * \return the names of all the registred bricks
     */
    std::vector<std::string> getRegistredBricks() const;

   private:
    /*!
     * default constructor
     */
    AbstractBehaviourBrickFactory();
    /*!
     * copy constructor (disabled)
     */
    AbstractBehaviourBrickFactory(const AbstractBehaviourBrickFactory&);
    /*!
     * assignement operator (disabled)
     */
    AbstractBehaviourBrickFactory& operator=(
        const AbstractBehaviourBrickFactory&);
    //! all registred constructors
    std::map<std::string, constructor> constructors;
  };  // end of struct AbstractBehaviourBrickFactory

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTBEHAVIOURBRICKFACTORY_HXX */
