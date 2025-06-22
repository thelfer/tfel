/*!
 * \file  mtest/include/MTest/FunctionEvolution.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 14 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTFUNCTIONEVOLUTION_HXX
#define LIB_MTEST_MTESTFUNCTIONEVOLUTION_HXX

#include "TFEL/Math/Evaluator.hxx"

#include "MTest/Config.hxx"
#include "MTest/Types.hxx"
#include "MTest/Evolution.hxx"

namespace mtest {

  struct MTEST_VISIBILITY_EXPORT FunctionEvolution : public Evolution {
    /*!
     * constructor
     * \param[in] :
     */
    FunctionEvolution(const std::string&, const EvolutionManager&);
    /*!
     * \return the value of the evolution
     * at the given time
     */
    virtual real operator()(const real) const override;
    /*!
     * \return true if the evolution is constant
     */
    virtual bool isConstant() const override;
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
    virtual ~FunctionEvolution();

   private:
    //! externally defined evolutions
    const EvolutionManager& evm;
    //! Evaluator
    mutable tfel::math::Evaluator f;
  };

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTFUNCTIONEVOLUTION_HXX */
