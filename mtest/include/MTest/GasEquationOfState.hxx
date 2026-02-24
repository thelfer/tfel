/*!
 * \file   GasEquationOfState.hxx
 * \brief
 * \author Thomas Helfer
 * \date   19 oct. 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_GASEQUATIONOFSTATE_HXX
#define LIB_MTEST_GASEQUATIONOFSTATE_HXX

#include <string>
#include <memory>
#include <functional>
#include "MTest/Config.hxx"
#include "MTest/MTest.hxx"

namespace mtest {

  struct MTEST_VISIBILITY_EXPORT GasEquationOfState {
    /*!
     * \param[in] e: equation of state
     * \param[in] evm: evolution manager
     */
    GasEquationOfState(const std::string&, const EvolutionManager&);
    /*!
     * \param[in] P: pressure
     * \param[in] V: volume
     * \param[in] T: temperature
     */
    double computeNumberOfMoles(const double, const double, const double);
    /*!
     * \param[in] V: volume
     * \param[in] n: number of moles
     * \param[in] T: temperature
     */
    double computePressure(const double, const double, const double);
    /*!
     * \param[in] V: volume
     * \param[in] n: number of moles
     * \param[in] T: temperature
     */
    double computeIsothermalBulkModulus(const double,
                                        const double,
                                        const double);
    //! destructor
    ~GasEquationOfState();

   private:
    //! pimpl idiom
    struct GasEquationOfStateMembers;
    //! internal data member
    std::unique_ptr<GasEquationOfStateMembers> m;
  };
}  // end of namespace mtest

#endif /* LIB_MTEST_GASEQUATIONOFSTATE_HXX */
