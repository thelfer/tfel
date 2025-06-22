/*!
 * \file  include/TFEL/Material/DDIF2Base.hxx
 * \brief
 * \author Thomas Helfer
 * \date 12 mar 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_DDIF2BASE_HXX
#define LIB_TFEL_MATERIAL_DDIF2BASE_HXX

#include <iosfwd>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"

namespace tfel::material {

  /*!
   * \brief states allowed in each crack directions
   */
  enum struct DDIF2State {
    COMPRESSION = 0,
    UNDAMAGED_TENSION = 1,
    GROWING_DAMAGE = 2,
    ELASTIC_UNLOADING = 3
  };  // end of enum struct DDIF2State

  /*!
   * \brief stream operator for debugging purposes
   * \param[in] os: output stream
   * \param[in] s: state
   * \return the output stream
   */
  TFELMATERIAL_VISIBILITY_EXPORT std::ostream& operator<<(std::ostream& os,
                                                          const DDIF2State& s);

  /*!
   * \brief helper class providing static methods for implementing the DDIF2
   * behaviour.
   */
  struct TFELMATERIAL_VISIBILITY_EXPORT DDIF2Base {
    /*!
     * \brief determine the state of the material at the beginning of the time
     * step according to the stress state and internal variables for a given
     * cracking direction.
     *
     * \tparam StressStensor: type of the stress tensor
     * \tparam Stensor: type of the tensor holding the considered crack
     * direction
     * \tparam strain: type of strain variables
     * \tparam stress: type of stress variables
     *
     * \param[in] sig: stress tensor
     * \param[in] n: crack direction
     * \param[in] em:  maximum crack strain
     * \param[in] smax: elastic limit
     * \param[in] H: softening modulus (expect a negative value)
     * \return the state
     */
    template <typename StressStensor,
              typename Stensor,
              typename strain,
              typename stress>
    static DDIF2State determineState(const StressStensor&,
                                     const Stensor&,
                                     const strain&,
                                     const stress&,
                                     const stress&);
    /*!
     * \brief method called to build the implicit equations associated with
     * the crack strain.
     *
     * \tparam StressStensor: type of the stress tensor
     * \tparam Stensor: type of the tensor holding the considered crack
     * direction
     * \tparam strain: type of strain variables
     * \tparam stress: type of stress variables
     *
     * \param[in] s: current estimation of the state.
     * \param[in] sig: current estimate of the stress tensor at the end of the
     * time step.
     * \param[in] n: crack direction
     * \param[in] em:  maximum crack strain at the beginning of the time step
     * \param[in] e: current estimate of the crack strain
     * \param[in] l: first Lamé coefficient
     * \param[in] m: second Lamé coefficient (shear modulus)
     * \param[in] smax: elastic limit
     * \param[in] H: softening modulus (expect a negative value)
     */
    template <typename StressStensor,
              typename StensorView,
              typename Stensor,
              typename strain,
              typename stress,
              typename real>
    static void buildImplicitEquation(strain&,
                                      real&,
                                      StensorView&,
                                      const DDIF2State&,
                                      const StressStensor&,
                                      const Stensor&,
                                      const strain&,
                                      const strain&,
                                      const strain&,
                                      const stress&,
                                      const stress&,
                                      const stress&,
                                      const stress&,
                                      const stress&);
    /*!
     * \brief method called after the convergence of the behaviour integration
     * to see if the predicted state is consistent with the solution found
     * \tparam StressStensor: type of the stress tensor
     * \tparam Stensor: type of the tensor holding the considered crack
     * direction
     * \tparam strain: type of strain variables
     * \tparam stress: type of stress variables
     *
     * \param[in,out] s: on input, the current estimation of the state. If
     * this estimate is not consistent, the state is changed on output
     * \param[in] sig: current estimate of the stress tensor at the end of the
     * time step.
     * \param[in] n: crack direction
     * \param[in] em:  maximum crack strain at the beginning of the time step
     * \param[in] e: current estimate of the crack strain
     * \param[in] smax: elastic limit
     * \param[in] seps: tolerance on the stress state
     * \param[in] eeps: tolerance on the strain state
     * \return a boolean stating if the state of the material is constitent
     with the current solution
     */
    template <typename StressStensor,
              typename Stensor,
              typename strain,
              typename stress>
    static bool checkStateConsistency(DDIF2State&,
                                      const StressStensor&,
                                      const Stensor&,
                                      const strain&,
                                      const strain&,
                                      const stress&,
                                      const stress&,
                                      const strain&);

    /*!
     * \brief class used to update the implicit equation and the jacobian
     * matrix.
     */
    template <typename Stensor2Type1, typename Stensor, typename real>
    static TFEL_VISIBILITY_LOCAL void treatFracture(Stensor2Type1&,
                                                    real&,
                                                    real&,
                                                    const real,
                                                    const real,
                                                    const real,
                                                    const Stensor&,
                                                    const Stensor&,
                                                    const real,
                                                    const real,
                                                    const real,
                                                    const real,
                                                    const real,
                                                    const real = real(0));
    /*!
     * \brief function used to define the equations associated with fracture
     * in explicit integration.
     */
    template <typename real>
    static TFEL_VISIBILITY_LOCAL void rk(tfel::math::tmatrix<3, 3, real>&,
                                         tfel::math::tvector<3, real>&,
                                         real&,
                                         const real,
                                         const real,
                                         const real,
                                         const real,
                                         const real,
                                         const real,
                                         const real,
                                         const unsigned short,
                                         const unsigned short,
                                         const unsigned short);

    template <typename real>
    static TFEL_VISIBILITY_LOCAL void cart2cyl(tfel::math::stensor<1u, real>&,
                                               const real);

    template <typename real>
    static TFEL_VISIBILITY_LOCAL void cart2cyl(tfel::math::stensor<2u, real>&,
                                               const real);

    template <typename real>
    static TFEL_VISIBILITY_LOCAL void cart2cyl(tfel::math::stensor<3u, real>&,
                                               const real);

    template <typename real>
    static TFEL_VISIBILITY_LOCAL void cyl2cart(tfel::math::stensor<1u, real>&,
                                               const real);

    template <typename real>
    static TFEL_VISIBILITY_LOCAL void cyl2cart(tfel::math::stensor<2u, real>&,
                                               const real);

    template <typename real>
    static TFEL_VISIBILITY_LOCAL void cyl2cart(tfel::math::stensor<3u, real>&,
                                               const real);

    template <typename real>
    static TFEL_VISIBILITY_LOCAL void invmat(tfel::math::tmatrix<3, 3, real>&,
                                             tfel::math::tmatrix<3, 3, real>&,
                                             tfel::math::tvector<3, real>&);

#ifndef _MSC_VER
    //! small parameter which guarantees that Ef is finite
    static constexpr double emin1 = 1.e-7;
    //! small parameter which guarantees that Ef is finite
    static constexpr double emin_rk1 = 1.e-5;
    //! small numerical parameter
    static constexpr double eps = 1.e-10;
#else
    //! small parameter which guarantees that Ef is finite
    static const double emin1;
    //! small parameter which guarantees that Ef is finite
    static const double emin_rk1;
    //! small numerical parameter
    static const double eps;
#endif

  };  // end of struct DDIF2Base

}  // end of namespace tfel::material

#include "TFEL/Material/DDIF2Base.ixx"

#endif /* LIB_TFEL_MATERIAL_DDIF2BASE_H */
