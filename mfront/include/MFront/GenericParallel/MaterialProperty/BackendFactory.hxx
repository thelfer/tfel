/*!
 * \file   MFront/GenericParallel/MaterialProperty/BackendFactory.hxx
 *
 * \brief This file declares the `BackendFactory` class
 * \author Thomas Helfer
 * \date   15/04/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_BACKENDFACTORY_HXX
#define LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_BACKENDFACTORY_HXX

#include <map>
#include <string>
#include <memory>
#include <functional>
#include <string_view>
#include "TFEL/Utilities/Data.hxx"

namespace mfront::generic_parallel::material_property {

  // forward declaration
  struct AbstractBackend;

  /*!
   * \brief an abstract factory for backends
   */
  struct MFRONT_VISIBILITY_EXPORT BackendFactory {
    //! \brief a simple alias
    using DataMap = tfel::utilities::DataMap;
    //! \brief a simple alias
    using Generator =
        std::function<std::unique_ptr<AbstractBackend>(const DataMap&)>;
    //! \return the uniq instance of this class
    static BackendFactory& get() noexcept;
    /*!
     * \brief add a new generator
     *
     * \param[in] n: name
     * \param[in] g: generator
     */
    void add(std::string_view, Generator);
    /*!
     * \brief add a new generator
     *
     * \param[in] n: name
     * \param[in] g: generator
     */
    std::unique_ptr<AbstractBackend> generate(std::string_view,
                                              const DataMap&) const;

   private:
    //! \brief default constructor
    BackendFactory();
    //! \brief destructor
    ~BackendFactory();
    //
    BackendFactory(BackendFactory&&) = delete;
    BackendFactory(const BackendFactory&) = delete;
    BackendFactory& operator=(BackendFactory&&) = delete;
    BackendFactory& operator=(const BackendFactory&) = delete;
    //! \brief registred generators
    std::map<std::string, Generator, std::less<>> generators;
  };

}  // end of namespace mfront::generic_parallel::material_property

#endif /* LIB_MFRONT_GENERICPARALLEL_MATERIALPROPERTY_BACKENDFACTORY_HXX */
