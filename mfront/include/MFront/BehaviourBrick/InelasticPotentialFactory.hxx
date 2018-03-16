/*!
 * \file   include/MFront/BehaviourBrick/InelasticPotentialFactory.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_INELASTICPOTENTIALFACTORY_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_INELASTICPOTENTIALFACTORY_HXX

#include <map>
#include <string>
#include <memory>
#include <functional>

namespace mfront {

  namespace bbrick {

    // forward declaration
    struct InelasticPotential;

    /*!
     * \brief abstract factory for inelastic potentials.
     */
    struct InelasticPotentialFactory {
      //! a simple alias
      using Generator = std::function<std::shared_ptr<InelasticPotential>()>;
      //! \return the uniq instance of the class
      static InelasticPotentialFactory& getFactory();
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
      std::shared_ptr<InelasticPotential> generate(const std::string&) const;

     private:
      //! default constructor
      InelasticPotentialFactory();
      //! move constructor (deleted)
      InelasticPotentialFactory(InelasticPotentialFactory&&) = delete;
      //! copy constructor (deleted)
      InelasticPotentialFactory(const InelasticPotentialFactory&) = delete;
      //! move assignement (deleted)
      InelasticPotentialFactory& operator=(InelasticPotentialFactory&&) =
          delete;
      //! standard assignement(deleted)
      InelasticPotentialFactory& operator=(const InelasticPotentialFactory&) =
          delete;
      //! \brief destructor
      ~InelasticPotentialFactory();
      //! \brief generators
      std::map<std::string, Generator> generators;
    };  // end of struct InelasticPotentialFactory

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_INELASTICPOTENTIALFACTORY_HXX */
