/*!
 * \file   include/MFront/BehaviourBrick/StressPotentialFactory.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_STRESSPOTENTIALFACTORY_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_STRESSPOTENTIALFACTORY_HXX

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace mfront {

  namespace bbrick {

    // forward declaration
    struct StressPotential;

    /*!
     * \brief abstract factory for Stress potentials.
     */
    struct StressPotentialFactory {
      //! a simple alias
      using Generator = std::function<std::shared_ptr<StressPotential>()>;
      //! \return the uniq instance of the class
      static StressPotentialFactory& getFactory();
      /*!
       * \brief add a new generator
       * \param[in] n: name of the generator
       * \param[in] g: generator
       */
      void addGenerator(const std::string&, const Generator&);
      //! \return the list of available stress potentials
      std::vector<std::string> getRegistredStressPotentials() const;
      /*!
       * \brief generate a new stress potential
       * \param[in] n: name of the stress potential
       * \param[in] bd: behaviour description
       * \param[in] p: parameters of the stress potential
       */
      std::shared_ptr<StressPotential> generate(const std::string&) const;

     private:
      //! default constructor
      StressPotentialFactory();
      //! move constructor (deleted)
      StressPotentialFactory(StressPotentialFactory&&) = delete;
      //! copy constructor (deleted)
      StressPotentialFactory(const StressPotentialFactory&) = delete;
      //! move assignement (deleted)
      StressPotentialFactory& operator=(StressPotentialFactory&&) = delete;
      //! standard assignement(deleted)
      StressPotentialFactory& operator=(const StressPotentialFactory&) = delete;
      //! \brief destructor
      ~StressPotentialFactory();
      //! \brief generators
      std::map<std::string, Generator> generators;
    };  // end of struct StressPotentialFactory

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_STRESSPOTENTIALFACTORY_HXX */
