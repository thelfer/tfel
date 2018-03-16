/*!
 * \file   include/MFront/BehaviourBrick/StressCriterionFactory.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_STRESSCRITERIONFACTORY_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_STRESSCRITERIONFACTORY_HXX

#include <map>
#include <string>
#include <memory>
#include <functional>

namespace mfront {

  namespace bbrick {

    // forward declaration
    struct StressCriterion;

    /*!
     * \brief abstract factory for inelastic potentials.
     */
    struct StressCriterionFactory {
      //! a simple alias
      using Generator = std::function<std::shared_ptr<StressCriterion>()>;
      //! \return the uniq instance of the class
      static StressCriterionFactory& getFactory();
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
      std::shared_ptr<StressCriterion> generate(const std::string&) const;

     private:
      //! default constructor
      StressCriterionFactory();
      //! move constructor (deleted)
      StressCriterionFactory(StressCriterionFactory&&) = delete;
      //! copy constructor (deleted)
      StressCriterionFactory(const StressCriterionFactory&) = delete;
      //! move assignement (deleted)
      StressCriterionFactory& operator=(StressCriterionFactory&&) =
          delete;
      //! standard assignement(deleted)
      StressCriterionFactory& operator=(const StressCriterionFactory&) =
          delete;
      //! \brief destructor
      ~StressCriterionFactory();
      //! \brief generators
      std::map<std::string, Generator> generators;
    };  // end of struct StressCriterionFactory

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_STRESSCRITERIONFACTORY_HXX */
