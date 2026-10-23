/*!
 * \file   include/MFront/BehaviourBrick/IsotropicHardeningRuleFactory.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICHARDENINGRULEFACTORY_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICHARDENINGRULEFACTORY_HXX

#include <map>
#include <string>
#include <memory>
#include <functional>
#include "MFront/MFrontConfig.hxx"

namespace mfront::bbrick {

  // forward declaration
  struct IsotropicHardeningRule;

  //! \brief abstract factory for isotropic hardening rules.
  struct MFRONT_VISIBILITY_EXPORT IsotropicHardeningRuleFactory {
    //! \brief a simple alias
    using Generator = std::function<std::shared_ptr<IsotropicHardeningRule>()>;
    //! \return the uniq instance of the class
    [[nodiscard]] static IsotropicHardeningRuleFactory& getFactory();
    //
    IsotropicHardeningRuleFactory(IsotropicHardeningRuleFactory&&) = delete;
    IsotropicHardeningRuleFactory(const IsotropicHardeningRuleFactory&) =
        delete;
    IsotropicHardeningRuleFactory& operator=(IsotropicHardeningRuleFactory&&) =
        delete;
    IsotropicHardeningRuleFactory& operator=(
        const IsotropicHardeningRuleFactory&) = delete;
    //! \brief return the list of available isotropic hardening rules
    [[nodiscard]] std::vector<std::string> getRegistredIsotropicHardeningRules()
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
    [[nodiscard]] std::shared_ptr<IsotropicHardeningRule> generate(
        const std::string&) const;

   private:
    //! \brief default constructor
    IsotropicHardeningRuleFactory();
    //! \brief \brief destructor
    ~IsotropicHardeningRuleFactory();
    //! \brief generators
    std::map<std::string, Generator> generators;
  };  // end of struct IsotropicHardeningRuleFactory

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICHARDENINGRULEFACTORY_HXX */
