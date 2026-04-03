/*!
 * \file   mfront/include/MFront/BehaviourBrickFactory.hxx
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
#include "MFront/MFrontConfig.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/AbstractBehaviourBrick.hxx"

namespace mfront {

  // forward declaration
  struct AbstractBehaviourDSL;

  //! \brief abstract factor for behaviour bricks
  struct MFRONT_VISIBILITY_EXPORT BehaviourBrickFactory {
    //! a simple alias
    using CxxTokenizer = tfel::utilities::CxxTokenizer;
    //! a simple alias
    using tokens_iterator = CxxTokenizer::const_iterator;
    //! a simple alias
    using constructor = std::shared_ptr<AbstractBehaviourBrick> (*)(
        AbstractBehaviourDSL&, BehaviourDescription&);
    //! a simple alias
    using BehaviourType = BehaviourDescription::BehaviourType;
    //! a simple alias
    using IntegrationScheme = BehaviourDescription::IntegrationScheme;
    /*!
     * \return the uniq instance of the AbstractBehaviourBrick factory
     */
    static BehaviourBrickFactory& getFactory();
    /*!
     * \return the requested behaviour brick
     * \param[in]     a:   behaviour brick name
     * \param[in,out] dsl: calling domain specific language
     * \param[in,out] mb:  mechanical behaviour description to be
     * treated
     */
    std::shared_ptr<AbstractBehaviourBrick> get(const std::string&,
                                                AbstractBehaviourDSL&,
                                                BehaviourDescription&) const;
    /*!
     * \param[in] a: name of the behaviour brick
     * \param[in] t: behaviour type for which the brick is valid
     * \param[in] c: constructor of the behaviour brick
     */
    void registerAbstractBehaviourBrick(const std::string&,
                                        const BehaviourType,
                                        const constructor);
    /*!
     * \param[in] a: name of the behaviour brick
     * \param[in] t: behaviour type for which the brick is valid
     * \param[in] i: integration scheme for which the brick is valid
     * \param[in] c: constructor of the behaviour brick
     */
    void registerAbstractBehaviourBrick(const std::string&,
                                        const BehaviourType,
                                        const IntegrationScheme,
                                        const constructor);
    /*!
     * \param[in] a: name of the behaviour brick
     * \param[in] ts: behaviour types for which the brick is valid
     * \param[in] is: integration schemes for which the brick is valid
     * \param[in] c: constructor of the behaviour brick
     */
    void registerAbstractBehaviourBrick(const std::string&,
                                        const std::vector<BehaviourType>,
                                        const std::vector<IntegrationScheme>,
                                        const constructor);
    //! \return the names of all the registred bricks
    std::vector<std::string> getRegistredBricks() const;
    /*!
     * \return the names of all the registred bricks which are valid for the
     * given behaviour type and integration scheme.
     * \param[in] t: behaviour type
     * \param[in] i: integration scheme
     */
    std::vector<std::string> getRegistredBricks(const BehaviourType,
                                                const IntegrationScheme) const;

   private:
    /*!
     * default constructor
     */
    BehaviourBrickFactory();
    /*!
     * copy constructor (disabled)
     */
    BehaviourBrickFactory(const BehaviourBrickFactory&);
    /*!
     * assignement operator (disabled)
     */
    BehaviourBrickFactory& operator=(const BehaviourBrickFactory&);
    //! all registred constructors
    std::map<std::string, constructor> constructors;
    //! mapping between a brick and the behaviour type for which it is valid
    std::map<std::string, std::vector<BehaviourType>> btypes;
    //! mapping between a brick and the integration schemes for which it is
    //! valid
    std::map<std::string, std::vector<IntegrationScheme>> ischemes;
  };  // end of struct BehaviourBrickFactory

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTBEHAVIOURBRICKFACTORY_HXX */
