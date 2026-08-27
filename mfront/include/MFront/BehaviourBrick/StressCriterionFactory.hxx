/*!
 * \file   include/MFront/BehaviourBrick/StressCriterionFactory.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURBRICK_STRESSCRITERIONFACTORY_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_STRESSCRITERIONFACTORY_HXX

#include <map>
#include <string>
#include <memory>
#include <functional>
#include "MFront/MFrontConfig.hxx"

namespace mfront::bbrick {

  // forward declaration
  struct StressCriterion;

  /*!
   * \brief abstract factory for stress criteria.
   */
  struct MFRONT_VISIBILITY_EXPORT StressCriterionFactory {
    //! \brief a simple alias
    using Generator = std::function<std::shared_ptr<StressCriterion>()>;
    //! \return the uniq instance of the class
    [[nodiscard]] static StressCriterionFactory& getFactory();
    //
    StressCriterionFactory(StressCriterionFactory&&) = delete;
    StressCriterionFactory(const StressCriterionFactory&) = delete;
    StressCriterionFactory& operator=(StressCriterionFactory&&) = delete;
    StressCriterionFactory& operator=(const StressCriterionFactory&) = delete;
    //! \brief return the list of available stress criteria
    [[nodiscard]] std::vector<std::string> getRegistredStressCriteria() const;
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
    [[nodiscard]] std::shared_ptr<StressCriterion> generate(
        const std::string&) const;

   private:
    //! \brief default constructor
    StressCriterionFactory();
    //! \brief \brief destructor
    ~StressCriterionFactory();
    //! \brief generators
    std::map<std::string, Generator> generators;
  };  // end of struct StressCriterionFactory

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_STRESSCRITERIONFACTORY_HXX */
