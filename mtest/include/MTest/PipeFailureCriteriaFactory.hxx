/*!
 * \file   mtest/include/MTest/PipeFailureCriteriaFactory.hxx
 * \brief
 * \author Thomas Helfer
 * \date   12/09/2022
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_PIPEFAILURECRITERIAFACTORY_HXX
#define LIB_MTEST_PIPEFAILURECRITERIAFACTORY_HXX

#include <map>
#include <memory>
#include <string>
#include <functional>
#include "TFEL/Utilities/Data.hxx"
#include "MTest/Config.hxx"

namespace mtest {

  // forward declaration
  struct PipeFailureCriterion;

  /*!
   * \brief factory associated with failure criteria
   */
  struct MTEST_VISIBILITY_EXPORT PipeFailureCriteriaFactory {
    //! \brief a simple alias
    using ParametersMap = tfel::utilities::DataMap;
    //! \brief a simple alias
    using Generator = std::function<std::unique_ptr<PipeFailureCriterion>(
        const std::string&, const ParametersMap&)>;
    //! \brief return the unique instance of the factory
    static PipeFailureCriteriaFactory& getFactory() noexcept;
    /*!
     * \brief declare a new generator
     * \param[in] n: name of the failure criterion
     * \param[in] g: generator
     */
    void addGenerator(const std::string&, const Generator&);
    /*!
     * \brief create a new failure criterion
     * \param[in] n: name of the failure criterion
     * \param[in] d: data passed to initialize the failure criterion
     */
    std::unique_ptr<PipeFailureCriterion> generate(const std::string&,
                                                   const ParametersMap&) const;

   private:
    //! \brief default constructor
    PipeFailureCriteriaFactory() noexcept;
    //! \brief move constructor (disabled)
    PipeFailureCriteriaFactory(PipeFailureCriteriaFactory&&) = delete;
    //! \brief copy constructor (disabled)
    PipeFailureCriteriaFactory(const PipeFailureCriteriaFactory&) = delete;
    //! \brief move assignement (disabled)
    PipeFailureCriteriaFactory& operator=(PipeFailureCriteriaFactory&&) =
        delete;
    //! \brief copy assignement (disabled)
    PipeFailureCriteriaFactory& operator=(const PipeFailureCriteriaFactory&) =
        delete;
    //! \brief destructor
    ~PipeFailureCriteriaFactory();
    //! \brief list of registred generators
    std::map<std::string, Generator> generators;
  };  // end of struct PipeFailureCriteriaFactory

}  // end of namespace mtest

#endif /* LIB_MTEST_PIPEFAILURECRITERIAFACTORY_HXX */
