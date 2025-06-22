/*!
 * \file   include/TFEL/Material/GursonTvergaardNeedleman.hxx
 * \brief
 * \author Jeremy Hure
 * \brief  04 Septembre 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_GURSONTVERGAARDNEEDLEMAN_H_
#define LIB_TFEL_MATERIAL_GURSONTVERGAARDNEEDLEMAN_H_

#include <cmath>

namespace tfel {

  namespace material {

    /*!
     * \brief This function computes the implicit flow stress
     * \f$\sigma^{\star}\f$ of the Gurson-Tvergaard-Needleman model.
     *
     * \f$\sigma^{\star}\f$ is implicitely defined by the following
     * relation:
     * \f[
     * \left(\frac{\sigma_{\mathrm{eq}}}{\sigma^{\star}}\right)^{2}+
     * 2\,q_{1}\,f^{\star}\,\cosh\left(\frac{q_{2}\,\mathrm{tr}\left(\underline{\sigma}\right)}{2\,\sigma^{\star}}\right)
     * -1-q_{1}\,f^{\star}=0
     * \f]
     * \return the value of \f$\sigma^{\star}\f$
     * \param[in] seq : Von Mises stress
     * \param[in] skk : stress trace
     * \param[in] fst : void volume fraction
     * \param[in] q1  : \f$q_1\f$ parameter
     * \param[in] q2  : \f$q_2\f$ parameter
     */
    template <typename T>
    T computeFlowStressGTN(
        const T seq, const T skk, const T fst, const T q1, const T q2) {
      using namespace std;
      T sst = seq;
      T S = pow((seq / sst), 2) + 2 * q1 * fst * cosh((q2 * skk) / (2 * sst)) -
            1 - pow((q1 * fst), 2);
      T SP =
          (-2 * pow((seq / sst), 2) / (sst)) -
          ((q1 * q2 * fst * skk) / (sst * sst)) * sinh((q2 * skk) / (2 * sst));
      while (S > 1.e-10) {
        sst = sst - S / SP;
        S = pow((seq / sst), 2) + 2 * q1 * fst * cosh((q2 * skk) / (2 * sst)) -
            1 - pow((q1 * fst), 2);
        SP = (-2 * pow((seq / sst), 2) / (sst)) -
             ((q1 * q2 * fst * skk) / (sst * sst)) *
                 sinh((q2 * skk) / (2 * sst));
      }
      return sst;
    }  // end of computeFlowStressGTN

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIAL_GURSONTVERGAARDNEEDLEMAN_H_ */
