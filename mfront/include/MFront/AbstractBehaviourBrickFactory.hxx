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
    typedef std::shared_ptr<AbstractBehaviourBrick> (*constructor)(
        AbstractBehaviourDSL&,
        BehaviourDescription&,
        const AbstractBehaviourBrick::Parameters&,
        const AbstractBehaviourBrick::DataMap&);
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
     * \param[out]    p:   parameters
     * \param[out]    d:   data
     */
    std::shared_ptr<AbstractBehaviourBrick> get(
        const std::string&,
        AbstractBehaviourDSL&,
        BehaviourDescription&,
        const AbstractBehaviourBrick::Parameters&,
        const AbstractBehaviourBrick::DataMap&) const;
    /*!
     * \param[in] a : AbstractBehaviourBrick name
     * \param[in] c : AbstractBehaviourBrick constructor
     */
    void registerAbstractBehaviourBrick(const std::string&, const constructor);

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
