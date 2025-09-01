/*!
 * \file  mtest/include/MTest/GenericEvolution.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 6/05/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTGENERICEVOLUTION_HXX
#define LIB_MTEST_MTESTGENERICEVOLUTION_HXX

#include <map>
#include <string>
#include <vector>
#include <memory>
#include "MFront/GenericMaterialProperty/MaterialProperty.h"
#include "MTest/Config.hxx"
#include "MTest/Types.hxx"
#include "MTest/Evolution.hxx"

namespace mtest {

  //! \brief an evolution defined by a cyrano material property
  struct MTEST_VISIBILITY_EXPORT GenericEvolution : public Evolution {
    /*!
     * \brief constructor
     * \param[in] l    : library
     * \param[in] f    : function
     * \param[in] evm_ : externally defined evolutions
     * \param[in] vm_  : externally defined values
     */
    GenericEvolution(const std::string&,
                     const std::string&,
                     const EvolutionManager&);
    real operator()(const real) const override;
    bool isConstant() const override;
    void setValue(const real) override;
    void setValue(const real, const real) override;
    //! \brief destructor
    ~GenericEvolution() override;

   protected:
    //! \brief externally defined evolutions
    const EvolutionManager& evm;
    //! \brief the generic function
    mfront_gmp_MaterialPropertyPtr f;
    //! \brief names of the variables
    std::vector<std::string> vnames;
    //! \brief arguments send to the generic function
    mutable std::vector<real> args;
  };

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTGENERICEVOLUTION_HXX */
