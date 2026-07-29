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
#include "MFront/MFrontConfig.hxx"

namespace mfront::bbrick {

  // forward declaration
  struct StressPotential;

  //! \brief abstract factory for Stress potentials.
  struct MFRONT_VISIBILITY_EXPORT StressPotentialFactory {
    //! a simple alias
    using Generator = std::function<std::shared_ptr<StressPotential>()>;
    //! \return the uniq instance of the class
    [[nodiscard]] static StressPotentialFactory& getFactory();
    //
    StressPotentialFactory(StressPotentialFactory&&) = delete;
    StressPotentialFactory(const StressPotentialFactory&) = delete;
    StressPotentialFactory& operator=(StressPotentialFactory&&) = delete;
    StressPotentialFactory& operator=(const StressPotentialFactory&) = delete;
    /*!
     * \brief add a new generator
     * \param[in] n: name of the generator
     * \param[in] g: generator
     */
    void addGenerator(const std::string&, const Generator&);
    //! \return the list of available stress potentials
    [[nodiscard]] std::vector<std::string> getRegistredStressPotentials() const;
    /*!
     * \brief generate a new stress potential
     * \param[in] n: name of the stress potential
     */
    [[nodiscard]] std::shared_ptr<StressPotential> generate(
        const std::string&) const;

   private:
    //! \brief default constructor
    StressPotentialFactory();
    //! \brief destructor
    ~StressPotentialFactory();
    //! \brief generators
    std::map<std::string, Generator> generators;
  };  // end of struct StressPotentialFactory

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_STRESSPOTENTIALFACTORY_HXX */
