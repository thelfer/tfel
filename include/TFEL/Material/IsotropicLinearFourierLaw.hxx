/*!
 * \file   include/TFEL/Material/IsotropicLinearFourierLaw.hxx
 * \brief  This file declares the IsotropicLinearFourierLaw class
 * \author Helfer Thomas
 * \date   25 Sep 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_ISOTROPICLINEARFOURIERLAW_H_
#define LIB_TFEL_ISOTROPICLINEARFOURIERLAW_H_

#include "TFEL/Material/ThermalBehaviour.hxx"
#include "TFEL/Material/ThermalBehaviourData.hxx"

namespace tfel {

  namespace material {

    /*!
     * \class IsotropicLinearFourierLaw
     * \brief This class implements the fourier thermal behaviour.
     * \param N, space dimension.
     * \param T, numerical type.
     * \author Helfer Thomas
     * \date   28 Jul 2006
     */
    template <unsigned short N, typename T, bool use_qt>
    class IsotropicLinearFourierLaw : public ThermalBehaviour<N, T, use_qt> {
      TFEL_STATIC_ASSERT(N == 1 || N == 2 || N == 3);
      typedef tfel::config::Types<N, T, use_qt> Types;
      typedef typename Types::real real;
      typedef typename Types::temperature temperature;

     public:
      /*
       * Default Constructor
       */
      IsotropicLinearFourierLaw(){};
      /*!
       * Law integration
       */
      void doThermalIntegration(ThermalBehaviourData<N, T, use_qt>& data) {
        CheckThermalDataValidity<IsotropicLinearFourierLaw>::exe(data);
      }
    };

  }  // end of namespace material

}  // end of namespace  tfel

#endif /* LIB_TFEL_ISOTROPICLINEARFOURIERLAW_H_ */
