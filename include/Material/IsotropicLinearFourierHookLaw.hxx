/*!
 * \file   IsotropicLinearFourierHookLaw.hxx
 * \brief  This file declares the IsotropicLinearFourierHookLaw class
 * \author Helfer Thomas
 * \date   27 Sep 2006
 */

#ifndef _LIB_TFEL_ISOTROPICLINEARFOURIERHOOKLAW_H_
#define _LIB_TFEL_ISOTROPICLINEARFOURIERHOOKLAW_H_ 

#include"MaterialLaw/IsotropicLinearFourierLaw.hxx"
#include"MaterialLaw/IsotropicLinearHookLaw.hxx"

namespace tfel{

  namespace materiallaw{

    /*!
     * \class IsotropicLinearFourierHookLaw
     * \brief This class implements the lorentz damage law.
     * \param N, space dimension.
     * \param T, numerical type.
     * \author Helfer Thomas
     * \date   28 Jul 2006
     */
    template<unsigned short N,typename T,bool use_qt>
    class IsotropicLinearFourierHookLaw
      : public IsotropicLinearFourierLaw<N,T,use_qt>,
	public IsotropicLinearHookLaw<N,T,use_qt>
    {};

  } // end of namespace materiallaw

} // end of namespace  tfel


#endif /* _LIB_TFEL_ISOTROPICLINEARFOURIERHOOKLAW_H */

