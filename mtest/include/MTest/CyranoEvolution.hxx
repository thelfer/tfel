/*!
 * \file  mtest/include/MTest/CyranoEvolution.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTCYRANOEVOLUTION_HXX
#define LIB_MTEST_MTESTCYRANOEVOLUTION_HXX

#include <map>
#include <string>
#include <vector>
#include <memory>
#include "Cyrano/MaterialProperty.hxx"
#include "MTest/Config.hxx"
#include "MTest/Types.hxx"
#include "MTest/Evolution.hxx"

namespace mtest {

  //! \brief an evolution defined by a cyrano material property
  struct MTEST_VISIBILITY_EXPORT CyranoEvolution : public Evolution {
    /*!
     * \brief constructor
     * \param[in] l    : library
     * \param[in] f    : function
     * \param[in] evm_ : externally defined evolutions
     * \param[in] vm_  : externally defined values
     */
    CyranoEvolution(const std::string&,
                    const std::string&,
                    const EvolutionManager&);
    real operator()(const real) const override;
    bool isConstant() const override;
    void setValue(const real) override;
    void setValue(const real, const real) override;
    //! destructor
    ~CyranoEvolution() override;

   protected:
    //! externally defined evolutions
    const EvolutionManager& evm;
    //! the cyrano function
    CyranoMaterialPropertyPtr f;
    //! names of the variables
    std::vector<std::string> vnames;
    //! arguments send to the Cast3M function
    mutable std::vector<real> args;
  };

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTCYRANOEVOLUTION_HXX */
