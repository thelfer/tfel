/*!
 * \file   include/MFront/BehaviourBrick/IsotropicHardeningRuleFactory.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICHARDENINGRULEFACTORY_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICHARDENINGRULEFACTORY_HXX

#include <map>
#include <string>
#include <memory>
#include <functional>
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  namespace bbrick {

    // forward declaration
    struct IsotropicHardeningRule;

    //! \brief abstract factory for isotropic hardening rules.
    struct MFRONT_VISIBILITY_EXPORT IsotropicHardeningRuleFactory {
      //! a simple alias
      using Generator =
          std::function<std::shared_ptr<IsotropicHardeningRule>()>;
      //! \return the uniq instance of the class
      static IsotropicHardeningRuleFactory& getFactory();
      //! \brief return the list of available isotropic hardening rules
      std::vector<std::string> getRegistredIsotropicHardeningRules() const;
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
      std::shared_ptr<IsotropicHardeningRule> generate(
          const std::string&) const;

     private:
      //! default constructor
      IsotropicHardeningRuleFactory();
      //! move constructor (deleted)
      IsotropicHardeningRuleFactory(IsotropicHardeningRuleFactory&&) = delete;
      //! copy constructor (deleted)
      IsotropicHardeningRuleFactory(const IsotropicHardeningRuleFactory&) =
          delete;
      //! move assignement (deleted)
      IsotropicHardeningRuleFactory& operator=(
          IsotropicHardeningRuleFactory&&) = delete;
      //! standard assignement(deleted)
      IsotropicHardeningRuleFactory& operator=(
          const IsotropicHardeningRuleFactory&) = delete;
      //! \brief destructor
      ~IsotropicHardeningRuleFactory();
      //! \brief generators
      std::map<std::string, Generator> generators;
    };  // end of struct IsotropicHardeningRuleFactory

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_ISOTROPICHARDENINGRULEFACTORY_HXX */
