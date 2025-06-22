/*!
 * \file  mtest/include/MTest/CastemEvolution.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTCASTEMEVOLUTION_H_
#define LIB_MTEST_MTESTCASTEMEVOLUTION_H_

#include <map>
#include <string>
#include <vector>

#include <memory>
#include "TFEL/System/ExternalLibraryManager.hxx"

#include "MTest/Config.hxx"
#include "MTest/Types.hxx"
#include "MTest/Evolution.hxx"

namespace mtest {

  /*!
   * An evolution defined by a castem fonction
   */
  struct MTEST_VISIBILITY_EXPORT CastemEvolution : public Evolution {
    /*!
     * \brief constructor
     * \param[in] l    : library
     * \param[in] f    : function
     * \param[in] evm_ : externally defined evolutions
     * \param[in] vm_  : externally defined values
     */
    CastemEvolution(const std::string&,
                    const std::string&,
                    const EvolutionManager&);
    /*!
     * \return the value of the CastemEvolution
     * at the given time
     */
    virtual real operator()(const real) const override;
    /*!
     * \return true if the evolution
     * is constant
     */
    virtual bool isConstant(void) const override;
    /*!
     * \brief set the evolution value for a given date
     * \param[in] v  : value
     * \note most evolution will throw an exception (this
     * method only makes sense for constant evolutions)
     */
    virtual void setValue(const real) override;
    /*!
     * \brief set the evolution value for a given date
     * \param[in] t  : time
     * \param[in] v  : value
     */
    virtual void setValue(const real, const real) override;
    //! destructor
    virtual ~CastemEvolution();

   protected:
    //! externally defined evolutions
    const EvolutionManager& evm;
    //! the castem function
    tfel::system::CastemFunctionPtr f;
    //! names of the variables
    std::vector<std::string> vnames;
    //! arguments send to the Cast3M function
    mutable std::vector<real> args;
  };

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTCASTEMEVOLUTION_H_ */
