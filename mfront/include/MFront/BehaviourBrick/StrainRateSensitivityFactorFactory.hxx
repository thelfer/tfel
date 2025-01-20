/*!
 * \file   include/MFront/BehaviourBrick/StrainRateSensitivityFactorFactory.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_STRAINRATESENSITIVITYFACTORFACTORY_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_STRAINRATESENSITIVITYFACTORFACTORY_HXX

#include <map>
#include <string>
#include <memory>
#include <functional>
#include "MFront/MFrontConfig.hxx"

namespace mfront::bbrick {

  // forward declaration
  struct StrainRateSensitivityFactor;

  //! \brief abstract factory for isotropic hardening rules.
  struct MFRONT_VISIBILITY_EXPORT StrainRateSensitivityFactorFactory {
    //! a simple alias
    using Generator =
        std::function<std::shared_ptr<StrainRateSensitivityFactor>()>;
    //! \return the uniq instance of the class
    static StrainRateSensitivityFactorFactory& getFactory();
    //! \brief return the list of available isotropic hardening rules
    std::vector<std::string> getRegistredStrainRateSensitivityFactors() const;
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
    std::shared_ptr<StrainRateSensitivityFactor> generate(
        const std::string&) const;

   private:
    //! default constructor
    StrainRateSensitivityFactorFactory();
    //! move constructor (deleted)
    StrainRateSensitivityFactorFactory(StrainRateSensitivityFactorFactory&&) =
        delete;
    //! copy constructor (deleted)
    StrainRateSensitivityFactorFactory(
        const StrainRateSensitivityFactorFactory&) = delete;
    //! move assignement (deleted)
    StrainRateSensitivityFactorFactory& operator=(
        StrainRateSensitivityFactorFactory&&) = delete;
    //! standard assignement(deleted)
    StrainRateSensitivityFactorFactory& operator=(
        const StrainRateSensitivityFactorFactory&) = delete;
    //! \brief destructor
    ~StrainRateSensitivityFactorFactory();
    //! \brief generators
    std::map<std::string, Generator> generators;
  };  // end of struct StrainRateSensitivityFactorFactory

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_STRAINRATESENSITIVITYFACTORFACTORY_HXX */
