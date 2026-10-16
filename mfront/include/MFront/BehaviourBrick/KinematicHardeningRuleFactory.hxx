/*!
 * \file   include/MFront/BehaviourBrick/KinematicHardeningRuleFactory.hxx
 * \brief
 * \author Thomas Helfer
 * \date 15/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of theses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_KINEMATICHARDENINGRULEFACTORY_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_KINEMATICHARDENINGRULEFACTORY_HXX

#include <map>
#include <string>
#include <memory>
#include <functional>
#include "MFront/MFrontConfig.hxx"

namespace mfront::bbrick {

  // forward declaration
  struct KinematicHardeningRule;

  /*!
   * \brief abstract factory for Kinematic hardening rules.
   */
  struct MFRONT_VISIBILITY_EXPORT KinematicHardeningRuleFactory {
    //! \brief a simple alias
    using Generator = std::function<std::shared_ptr<KinematicHardeningRule>()>;
    //! \return the uniq instance of the class
    [[nodiscard]] static KinematicHardeningRuleFactory& getFactory();
    //
    KinematicHardeningRuleFactory(KinematicHardeningRuleFactory&&) = delete;
    KinematicHardeningRuleFactory(const KinematicHardeningRuleFactory&) =
        delete;
    KinematicHardeningRuleFactory& operator=(KinematicHardeningRuleFactory&&) =
        delete;
    KinematicHardeningRuleFactory& operator=(
        const KinematicHardeningRuleFactory&) = delete;
    //! \brief return the list of kinematic hardening rules
    [[nodiscard]] std::vector<std::string> getRegistredKinematicHardeningRules()
        const;
    /*!
     * \brief add a new generator
     * \param[in] n: name of the generator
     * \param[in] g: generator
     */
    void addGenerator(const std::string&, const Generator&);
    /*!
     * \brief generate a new inelastic potential
     * \param[in] n: name of the inelastic potential
     */
    [[nodiscard]] std::shared_ptr<KinematicHardeningRule> generate(
        const std::string&) const;

   private:
    //! \brief default constructor
    KinematicHardeningRuleFactory();
    //! \brief \brief destructor
    ~KinematicHardeningRuleFactory();
    //! \brief generators
    std::map<std::string, Generator> generators;
  };  // end of struct KinematicHardeningRuleFactory

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_KINEMATICHARDENINGRULEFACTORY_HXX */
