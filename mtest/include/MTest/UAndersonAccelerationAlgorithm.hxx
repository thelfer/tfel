/*!
 * \file  mtest/include/MTest/UAndersonAccelerationAlgorithm.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 10 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTEST_UANDERSONACCELERATIONALGORITHM_HXX
#define LIB_MTEST_MTEST_UANDERSONACCELERATIONALGORITHM_HXX

#include "TFEL/Math/AccelerationAlgorithms/UAnderson.hxx"
#include "MTest/Config.hxx"
#include "MTest/AccelerationAlgorithm.hxx"

namespace mtest {

  /*!
   * \brief This class implements the Cast3M acceleration algorithm.
   */
  struct MTEST_VISIBILITY_EXPORT UAndersonAccelerationAlgorithm final
      : public AccelerationAlgorithm {
    //! default constructor
    UAndersonAccelerationAlgorithm();
    /*!
     * \return the name of the acceleration algorithm
     */
    virtual std::string getName() const override;
    /*!
     * \param[in] psz : total number of unknowns (problem size)
     */
    virtual void initialize(const unsigned short) override;
    /*!
     * set a parameter of the acceleration algorithm
     * \param[in] p : parameter name
     * \param[in] v : parameter value
     */
    virtual void setParameter(const std::string &,
                              const std::string &) override;
    /*!
     * called at each time step, before the beginning of the Newton
     * algorithm
     */
    virtual void preExecuteTasks() override;
    /*!
     * \param[in,out] u1   : current estimate of the unknowns
     * \param[in]     u0   : previous estimate of the unknowns
     * \param[in]     r    : Newton residual
     * \param[in]     ueps : criterium on the unknowns
     * \param[in]     seps : criterium on the thermodynamic forces
     * \param[in]     iter : current iteration number
     */
    virtual void execute(tfel::math::vector<real> &,
                         const tfel::math::vector<real> &,
                         const tfel::math::vector<real> &,
                         const real,
                         const real,
                         const unsigned short) override;
    /*!
     * called at each time step, once convergence is reached
     */
    virtual void postExecuteTasks() override;
    //! destructor
    virtual ~UAndersonAccelerationAlgorithm();

   protected:
    using Field = tfel::math::vector<real>;
    using UAnderson = tfel::math::UAnderson<Field, long double>;
    //! the underlying algorithm
    std::unique_ptr<UAnderson> a;
    /* pointer to the internal fields */
    //!< previous displacement estimate
    Field *uO;
    //!< current displacement estimate
    Field *uN;
    //! method order
    int Nmax = -1;
    //! alternance order
    int alMax = -1;
  };  // end of struct UAndersonAccelerationAlgorithm

}  // end of namespace mtest

#endif /* LIB_MTEST_MTEST_UANDERSONACCELERATIONALGORITHM_HXX */
