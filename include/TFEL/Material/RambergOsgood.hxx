/*!
 * \file   include/TFEL/Material/RambergOsgood.hxx
 * \brief
 * \author Brunon É.
 * \brief  21 déc. 2012
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_RAMBERGOSGOOD_H_
#define LIB_TFEL_MATERIAL_RAMBERGOSGOOD_H_

#include <cmath>

namespace tfel {

  namespace material {

    /*!
     * \brief This function computes the RambergOsgood paramters K and n
     * which allow the approximation of the traction curve by the
     * relation:
     * \[
     * p = K(s/E)**n
     * \]
     * where:
     * - p is the cumulated plastic strain
     * - s is the von mises stress
     * - E is the Young modulus
     *
     * This functions can be used with the mfront'
     * IsotropicPlasticMisesFlow parser like this:
     * \code{.cpp}
     * @LocalVariable real K_p;
     * @LocalVariable real n_p;
     *
     * @InitLocalVariables{
     *   using namespace tfel::material;
     *   const real Rp02=...;
     *   const real Rm=...;
     *   const real Ag=...;
     *   real K_r;
     *   real n_r;
     *   computeRambergOsgoodParameters(K_r,n_r,young,Rp02,Rm,Ag);
     *   K_p=1./K_r;
     *   n_p=1./n_r;
     * }
     *
     * @FlowRule{
     *   const real p0 = 1.e-6;
     *   const real tmp = young*pow(K_p*(p+p0),n_p-1);
     *   f       = seq-tmp*K_p*(p+p0);
     *   df_dseq = 1.;
     *   df_dp   = -n_p*K_p*tmp;
     * }
     * \endcode
     *
     * \param[out] K: first  RambergOsgood paramter
     * \param[out] n: second RambergOsgood paramter
     * \param[in]  E :   YoungModulus
     * \param[in]  Rp02: YieldStress
     * \param[in]  Rm:   UltimateTensileStress
     * \param[in]  Ag:   UniformElongation
     */
    template <typename T1, typename T2>
    void computeRambergOsgoodParameters(
        T1& K, T1& n, const T2 E, const T2 Rp02, const T2 Rm, const T2 Ag) {
      using namespace std;
      n = log(500 * Ag) / log(Rm / Rp02);
      K = 0.2e-2 * E * pow(Rp02 / E, 1 - n) / Rp02;
    }

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_RAMBERGOSGOOD_H_ */
