/*!
 * \file   include/MFront/BehaviourBrick/KinematicHardeningRuleFactory.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_KINEMATICHARDENINGRULEFACTORY_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_KINEMATICHARDENINGRULEFACTORY_HXX

#include <map>
#include <string>
#include <memory>
#include <functional>

namespace mfront {

  namespace bbrick {

    // forward declaration
    struct KinematicHardeningRule;

    /*!
     * \brief abstract factory for Kinematic hardening rules.
     */
    struct KinematicHardeningRuleFactory {
      //! a simple alias
      using Generator = std::function<std::shared_ptr<KinematicHardeningRule>()>;
      //! \return the uniq instance of the class
      static KinematicHardeningRuleFactory& getFactory();
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
      std::shared_ptr<KinematicHardeningRule> generate(
          const std::string&) const;

     private:
      //! default constructor
      KinematicHardeningRuleFactory();
      //! move constructor (deleted)
      KinematicHardeningRuleFactory(KinematicHardeningRuleFactory&&) = delete;
      //! copy constructor (deleted)
      KinematicHardeningRuleFactory(const KinematicHardeningRuleFactory&) =
          delete;
      //! move assignement (deleted)
      KinematicHardeningRuleFactory& operator=(
          KinematicHardeningRuleFactory&&) = delete;
      //! standard assignement(deleted)
      KinematicHardeningRuleFactory& operator=(
          const KinematicHardeningRuleFactory&) = delete;
      //! \brief destructor
      ~KinematicHardeningRuleFactory();
      //! \brief generators
      std::map<std::string, Generator> generators;
    };  // end of struct KinematicHardeningRuleFactory

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_KINEMATICHARDENINGRULEFACTORY_HXX */
