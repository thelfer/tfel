/*!
 * \file   include/MFront/BehaviourBrick/PorosityNucleationModelFactory.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_POROSITYNUCLEATIONMODELFACTORY_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_POROSITYNUCLEATIONMODELFACTORY_HXX

#include <map>
#include <string>
#include <memory>
#include <functional>
#include "MFront/MFrontConfig.hxx"

namespace mfront::bbrick {

  // forward declaration
  struct PorosityNucleationModel;

  //! \brief abstract factory for inelastic Flows.
  struct MFRONT_VISIBILITY_EXPORT PorosityNucleationModelFactory {
    //! a simple alias
    using Generator = std::function<std::shared_ptr<PorosityNucleationModel>()>;
    //! \return the uniq instance of the class
    static PorosityNucleationModelFactory& getFactory();
    /*!
     * \brief add a new generator
     * \param[in] n: name of the generator
     * \param[in] g: generator
     */
    void addGenerator(const std::string&, const Generator&);
    //! \return the list of available inelastic flows
    std::vector<std::string> getRegistredPorosityNucleationModels() const;
    /*!
     * \brief generate a new inelastic flow
     * \param[in] n: name of the inelastic flow
     */
    std::shared_ptr<PorosityNucleationModel> generate(const std::string&) const;

   private:
    //! default constructor
    PorosityNucleationModelFactory();
    //! move constructor (deleted)
    PorosityNucleationModelFactory(PorosityNucleationModelFactory&&) = delete;
    //! copy constructor (deleted)
    PorosityNucleationModelFactory(const PorosityNucleationModelFactory&) =
        delete;
    //! move assignement (deleted)
    PorosityNucleationModelFactory& operator=(
        PorosityNucleationModelFactory&&) = delete;
    //! standard assignement(deleted)
    PorosityNucleationModelFactory& operator=(
        const PorosityNucleationModelFactory&) = delete;
    //! \brief destructor
    ~PorosityNucleationModelFactory();
    //! \brief generators
    std::map<std::string, Generator> generators;
  };  // end of struct PorosityNucleationModelFactory

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_POROSITYNUCLEATIONMODELFACTORY_HXX */
