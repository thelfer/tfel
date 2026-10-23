/*!
 * \file   include/MFront/BehaviourBrick/PorosityNucleationModelFactory.hxx
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
    [[nodiscard]] static PorosityNucleationModelFactory& getFactory();
    //
    PorosityNucleationModelFactory(PorosityNucleationModelFactory&&) = delete;
    PorosityNucleationModelFactory(const PorosityNucleationModelFactory&) =
        delete;
    PorosityNucleationModelFactory& operator=(
        PorosityNucleationModelFactory&&) = delete;
    PorosityNucleationModelFactory& operator=(
        const PorosityNucleationModelFactory&) = delete;
    /*!
     * \brief add a new generator
     * \param[in] n: name of the generator
     * \param[in] g: generator
     */
    void addGenerator(const std::string&, const Generator&);
    //! \return the list of available inelastic flows
    [[nodiscard]] std::vector<std::string>
    getRegistredPorosityNucleationModels() const;
    /*!
     * \brief generate a new inelastic flow
     * \param[in] n: name of the inelastic flow
     */
    [[nodiscard]] std::shared_ptr<PorosityNucleationModel> generate(
        const std::string&) const;

   private:
    //! \brief default constructor
    PorosityNucleationModelFactory();
    //! \brief destructor
    ~PorosityNucleationModelFactory();
    //! \brief generators
    std::map<std::string, Generator> generators;
  };  // end of struct PorosityNucleationModelFactory

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_POROSITYNUCLEATIONMODELFACTORY_HXX */
